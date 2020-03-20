#include <iostream>

#include "IFAETopFramework/AnalysisObject.h"
#include "IFAETopFramework/WeightManager.h"

#include "VLQAnalysis/VLQ_OutputData.h"
#include "VLQAnalysis/VLQ_Options.h"
#include "VLQAnalysis/VLQ_NtupleData.h"

//TRF
#include "BtaggingTRFandRW/TRFinterface.h"
#include "VLQAnalysis/VLQ_TRFManager.h"


//______________________________________________________________________________
//
VLQ_TRFManager::VLQ_TRFManager( VLQ_Options* opt, WeightManager* weightMngr, const VLQ_NtupleData* ntupData, VLQ_OutputData* outData  ):
TRFManager( opt, weightMngr, outData, static_cast<const NtupleData*>(ntupData) ),
m_opt(opt),
m_weightMngr(weightMngr),
m_ntupData(ntupData),
m_outData(outData),
m_trfint(0),
m_btag_calib_scheme("default"){
}

//______________________________________________________________________________
//
void VLQ_TRFManager::Init(){
  //initialize TRF
  if(m_opt -> MsgLevel() == Debug::DEBUG){
    std::cout << "In VLQ_TRFManager constructor: Creating TRF Object" << std::endl;
  }
  if(m_opt->BtagOP()==""){
    m_trfint = new TRFinterface("FixedCutBEff_77",//b-tag OP
    "AntiKt4EMTopoJets",//jet collection
    m_opt->TRFCDIPath(),//CDI file
    false, //ignore SF
    ((m_opt -> MsgLevel() == Debug::DEBUG)?5:0), //debug level
    true,//rwSystForPerm (weight for permutation to be the same with systematics)
    false,//tag bins
    1,//n calibrations
    true//add properties (because of the following line)
    );
  }
  else {
    m_trfint = new TRFinterface(m_opt->BtagOP(),//b-tag OP
    "AntiKt4EMTopoJets",//jet collection
    m_opt->TRFCDIPath(),//CDI file
    false, //ignore SF
    ((m_opt -> MsgLevel() == Debug::DEBUG)?5:0), //debug level
    true,//rwSystForPerm (weight for permutation to be the same with systematics)
    false,//tag bins
    1,//n calibrations
    true//add properties (because of the following line)
    );
  }
  m_trfint->setEffProperty("EfficiencyBCalibrations","default;410004;410006");
  //m_trfint->setEffProperty("ScaleFactorCCalibration","ttbar");
  m_trfint->Initialize();
}

//______________________________________________________________________________
//
VLQ_TRFManager::VLQ_TRFManager( const VLQ_TRFManager &q ):
TRFManager(q)
{
  m_opt               = q.m_opt;
  m_weightMngr        = q.m_weightMngr;
  m_ntupData          = q.m_ntupData;
  m_outData           = q.m_outData;
  m_trfint            = q.m_trfint;
}

//______________________________________________________________________________
//
VLQ_TRFManager::~VLQ_TRFManager(){
  delete m_trfint;
}

//______________________________________________________________________________
//
bool VLQ_TRFManager::ComputeTRFWeights(){

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "=> Entering in VLQ_TRFManager::EvaluateTRF:    Event number = " << m_ntupData -> d_eventNumber << std::endl;

  if(!m_trfint){
    std::cerr << "<!> Error in VLQ_TRFManager::EvaluateTRF(): the TRFInterface object has not been initialized. Please check !" << std::endl;
    abort();
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // TRF CONFIGURATION
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  std::vector<double> sel_jets_pt;
  std::vector<double> sel_jets_eta;
  std::vector<double> sel_jets_phi;
  std::vector<int> sel_jets_truthLabel;
  std::vector<double> sel_jets_btag_weight;

  sel_jets_pt.clear();
  sel_jets_phi.clear();
  sel_jets_eta.clear();
  sel_jets_btag_weight.clear();
  sel_jets_truthLabel.clear();

  for( const AnalysisObject* jet : *( m_outData -> o_jets ) ){
    if(TMath::Abs(jet->Eta())>2.5) continue;
    if(jet->Pt() > 10000){
      std::cout << "This jet has a pT larger than 10 TeV ... Are you sure there is not MeV/GeV mismatch ? Will continue, but please check ! " << std::endl;
    }
    sel_jets_pt.push_back( jet->Pt()*1000 );
    sel_jets_phi.push_back( jet->Phi() );
    sel_jets_eta.push_back( jet->Eta() );
    sel_jets_btag_weight.push_back( jet-> GetMoment("btagw") );
    sel_jets_truthLabel.push_back( (int)jet -> GetMoment("truthLabel") );
  }

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> Vectors for TRF Filled = " << sel_jets_pt.size() << std::endl;

  m_trfint->setSeed( m_ntupData -> d_eventNumber + m_outData -> o_jets_n );
  m_trfint->setJets( sel_jets_pt, sel_jets_eta, sel_jets_truthLabel, sel_jets_btag_weight );

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> TRF configured" << std::endl;


  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // TRF OUTPUTS
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //
  // TRF event weight
  //
  if( m_opt -> DoTRF() && m_opt -> RecomputeTRF() ){

    //
    // Compute TRF weights and permutations
    //
    m_trfint->getTRFweights( m_opt->MaxTRFB(),*(m_outData -> o_TRFweight_ex),*(m_outData -> o_TRFweight_in));
    m_trfint->chooseTagPermutation(m_opt->MaxTRFB(),*(m_outData -> o_TRFPerm_ex),*(m_outData -> o_TRFPerm_in));

    //
    // Computing the systematic uncertainties
    //
    if(m_opt -> ComputeWeightSys() && m_opt->DoExpSys()){
      if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> TRF: Starting systematics part  " << std::endl;

      //
      // Now effectively computing the systematic uncertainties
      //

      //-- b-jets
      if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "-> b-jets " << std::endl;
      m_trfint->getTRFweightsForSyst( m_opt->MaxTRFB(),*(m_outData -> o_BTag_BreakUp_TRF_ex),  *(m_outData -> o_BTag_BreakUp_TRF_in),    "B", true);
      m_trfint->getTRFweightsForSyst( m_opt->MaxTRFB(),*(m_outData -> o_BTag_BreakDown_TRF_ex),*(m_outData -> o_BTag_BreakDown_TRF_in),  "B", false);

      //-- c-jets
      if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "-> c-jets " << std::endl;
      m_trfint->getTRFweightsForSyst( m_opt->MaxTRFB(),*(m_outData -> o_CTag_BreakUp_TRF_ex),*(m_outData -> o_CTag_BreakUp_TRF_in),      "C", true);
      m_trfint->getTRFweightsForSyst( m_opt->MaxTRFB(),*(m_outData -> o_CTag_BreakDown_TRF_ex),*(m_outData -> o_CTag_BreakDown_TRF_in),  "C", false);

      //-- l-jets
      if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "-> l-jets " << std::endl;
      m_trfint->getTRFweightsForSyst( m_opt->MaxTRFB(),*(m_outData -> o_LTag_BreakUp_TRF_ex),*(m_outData -> o_LTag_BreakUp_TRF_in),     "Light",   true);
      m_trfint->getTRFweightsForSyst( m_opt->MaxTRFB(),*(m_outData -> o_LTag_BreakDown_TRF_ex),*(m_outData -> o_LTag_BreakDown_TRF_in), "Light",   false);

      //-- extrapolation uncertainty
      if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "-> extrapolation " << std::endl;
      m_trfint -> getTRFweightsForSyst( m_opt->MaxTRFB(), *(m_outData -> o_BTagExtrapUp_TRF_ex),   *(m_outData -> o_BTagExtrapUp_TRF_in),   "Extrap",   true );
      m_trfint -> getTRFweightsForSyst( m_opt->MaxTRFB(), *(m_outData -> o_BTagExtrapDown_TRF_ex), *(m_outData -> o_BTagExtrapDown_TRF_in), "Extrap",   false );

      //-- extrapolation from charm uncertainty
      if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "-> extrapolation from charm " << std::endl;
      m_trfint -> getTRFweightsForSyst( m_opt->MaxTRFB(), *(m_outData -> o_BTagExtrapFromCharmUp_TRF_ex),   *(m_outData -> o_BTagExtrapFromCharmUp_TRF_in),   "ExtrapFromCharm", true );
      m_trfint -> getTRFweightsForSyst( m_opt->MaxTRFB(), *(m_outData -> o_BTagExtrapFromCharmDown_TRF_ex), *(m_outData -> o_BTagExtrapFromCharmDown_TRF_in), "ExtrapFromCharm", false );
    }

    if(m_opt -> MsgLevel() == Debug::DEBUG){
      std::cout << "==> After filling vectors and weights for TRF  " << std::endl;
    }

  }
  //
  // Direct tagging weight recomputation
  //
  else if( !m_opt -> DoTRF() && m_opt -> RecomputeBtagSF() ) {
    //
    // Compute direct tagging SF and systematics
    // !!!!!!! The weight configuration file has to be updated accordingly !!!!!!!
    //
    if(!m_weightMngr){
      std::cerr << "<!> Error in VLQ_TRFManager::EvaluateTRF(): the WeightManager object has not been initialized. Please check !" << std::endl;
      abort();
    }

    // Nominal SF
    const double recomputedBTagSF = m_trfint->getEvtSF(m_btag_calib_scheme,"",0,true);
    if(m_opt -> MsgLevel() == Debug::DEBUG){
      std::cout << "==> SF EVT =  "<< recomputedBTagSF << std::endl;
    }
    m_weightMngr->SetNominalComponent("weight_btag", recomputedBTagSF);

    // Systematics
    if(m_opt -> ComputeWeightSys() && m_opt->DoExpSys()){
      if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> TRF: Starting systematics part  " << std::endl;

      const unsigned int nbBev = m_trfint->getNumEV("B",m_btag_calib_scheme);
      const unsigned int nbCev = m_trfint->getNumEV("C",m_btag_calib_scheme);
      const unsigned int nbLev = m_trfint->getNumEV("Light",m_btag_calib_scheme);

      //Systematics
      for(unsigned int ibev=0; ibev<nbBev; ibev++) {
        const double evUp   = m_trfint->getEvtSF(m_btag_calib_scheme,"B",ibev,true);
        const double evDown = m_trfint->getEvtSF(m_btag_calib_scheme,"B",ibev,false);
        m_weightMngr->SetSystematicComponent(Form("weight_btag_B_EV_Up_%i",ibev), evUp);
        m_weightMngr->SetSystematicComponent(Form("weight_btag_B_EV_Down_%i",ibev), evDown);
      }
      for(unsigned int icev=0; icev<nbCev; icev++) {
        const double evUp   = m_trfint->getEvtSF(m_btag_calib_scheme,"C",icev,true);
        const double evDown = m_trfint->getEvtSF(m_btag_calib_scheme,"C",icev,false);
        m_weightMngr->SetSystematicComponent(Form("weight_btag_C_EV_Up_%i",icev), evUp);
        m_weightMngr->SetSystematicComponent(Form("weight_btag_C_EV_Down_%i",icev), evDown);
      }
      for(unsigned int ilev=0; ilev<nbLev; ilev++) {
        const double evUp   = m_trfint->getEvtSF(m_btag_calib_scheme,"Light",ilev,true);
        const double evDown = m_trfint->getEvtSF(m_btag_calib_scheme,"Light",ilev,false);
        m_weightMngr->SetSystematicComponent(Form("weight_btag_Light_EV_Up_%i",ilev), evUp);
        m_weightMngr->SetSystematicComponent(Form("weight_btag_Light_EV_Down_%i",ilev), evDown);
      }
      m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_Up", m_trfint->getEvtSF(m_btag_calib_scheme,"Extrap",0,true) );
      m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_Down", m_trfint->getEvtSF(m_btag_calib_scheme,"Extrap",0,false) );

      m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_from_charm_Up", m_trfint->getEvtSF(m_btag_calib_scheme,"ExtrapFromCharm",0,true) );
      m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_from_charm_Down", m_trfint->getEvtSF(m_btag_calib_scheme,"ExtrapFromCharm",0,false) );
    }
  }
  return true;
}

//______________________________________________________________________________
//
bool VLQ_TRFManager::UpdateBTagging( const bool isIncl, const int req_nbjets ){

  if(!m_weightMngr){
    std::cerr << "<!> Error in VLQ_TRFManager::UpdateBTagging(): the WeightManager object has not been initialized. Please check !" << std::endl;
    abort();
  }

  if(req_nbjets > m_opt -> MaxTRFB()){
    std::cerr << "<!> Error in VLQ_TRFManager::UpdateBTagging(): the b-jets";
    std::cout << " requirement is larger than the one used for the TRF tool (";
    std::cout << m_opt -> MaxTRFB();
    std::cout << "). Aborting since this error could be dramatic !" << std::endl;
    abort();
  }

  if( m_opt -> DoTRF() && m_opt -> RecomputeTRF() ){
    //
    // All the variables have been computed in the function ComputeTRFWeights ... Just setting the good ones here
    //
    //
    //---~~~~>> Nominal
    //
    m_weightMngr->SetNominalComponent("weight_btag", (isIncl ? m_outData -> o_TRFweight_in -> at(req_nbjets) : m_outData -> o_TRFweight_ex -> at(req_nbjets)) );

    //
    //---~~~~>> Systematics
    //
    if(m_opt -> ComputeWeightSys() && m_opt->DoExpSys()){
      //-- b
      std::vector < std::vector < double > > *BTag_BreakUp_TRF = isIncl ? m_outData -> o_BTag_BreakUp_TRF_in : m_outData -> o_BTag_BreakUp_TRF_ex;
      std::vector < std::vector < double > > *BTag_BreakDown_TRF = isIncl ? m_outData -> o_BTag_BreakDown_TRF_in : m_outData -> o_BTag_BreakDown_TRF_ex;
      for ( unsigned int ibev = 0; ibev < BTag_BreakUp_TRF -> size(); ++ibev ) {
        m_weightMngr->SetSystematicComponent(Form("weight_btag_B_EV_Up_%i",ibev), BTag_BreakUp_TRF -> at(ibev)[req_nbjets] );
        m_weightMngr->SetSystematicComponent(Form("weight_btag_B_EV_Down_%i",ibev), BTag_BreakDown_TRF -> at(ibev)[req_nbjets]);
      }
      //-- c
      std::vector < std::vector < double > > *CTag_BreakUp_TRF = isIncl ? m_outData -> o_CTag_BreakUp_TRF_in : m_outData -> o_CTag_BreakUp_TRF_ex;
      std::vector < std::vector < double > > *CTag_BreakDown_TRF = isIncl ? m_outData -> o_CTag_BreakDown_TRF_in : m_outData -> o_CTag_BreakDown_TRF_ex;
      for ( unsigned int icev = 0; icev < CTag_BreakUp_TRF -> size(); ++icev ) {
        m_weightMngr->SetSystematicComponent(Form("weight_btag_C_EV_Up_%i",icev), CTag_BreakUp_TRF -> at(icev)[req_nbjets]);
        m_weightMngr->SetSystematicComponent(Form("weight_btag_C_EV_Down_%i",icev), CTag_BreakDown_TRF -> at(icev)[req_nbjets]);
      }
      //-- l
      std::vector < std::vector < double > > *LTag_BreakUp_TRF = isIncl ? m_outData -> o_LTag_BreakUp_TRF_in : m_outData -> o_LTag_BreakUp_TRF_ex;
      std::vector < std::vector < double > > *LTag_BreakDown_TRF = isIncl ? m_outData -> o_LTag_BreakDown_TRF_in : m_outData -> o_LTag_BreakDown_TRF_ex;
      for ( unsigned int ilev = 0; ilev < LTag_BreakUp_TRF -> size(); ++ilev ) {
        m_weightMngr->SetSystematicComponent(Form("weight_btag_Light_EV_Up_%i",ilev), LTag_BreakUp_TRF -> at(ilev)[req_nbjets]);
        m_weightMngr->SetSystematicComponent(Form("weight_btag_Light_EV_Down_%i",ilev), LTag_BreakDown_TRF -> at(ilev)[req_nbjets]);
      }
      //-- extrapolation
      std::vector < double > *temp_extrap_up     = isIncl ? &(m_outData -> o_BTagExtrapUp_TRF_in -> at(0)) : &(m_outData -> o_BTagExtrapUp_TRF_ex -> at(0));
      std::vector < double > *temp_extrap_down   = isIncl ? &(m_outData -> o_BTagExtrapDown_TRF_in -> at(0)) : &(m_outData -> o_BTagExtrapDown_TRF_ex -> at(0));
      m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_Up", temp_extrap_up->at(req_nbjets) );
      m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_Down", temp_extrap_down->at(req_nbjets) );
      //-- extrapolation from charm
      std::vector < double > *temp_extrap_from_charm_up = isIncl ? &(m_outData->o_BTagExtrapFromCharmUp_TRF_in->at(0)) : &(m_outData->o_BTagExtrapFromCharmUp_TRF_ex->at(0));
      std::vector < double > *temp_extrap_from_charm_down = isIncl ? &(m_outData->o_BTagExtrapFromCharmDown_TRF_in->at(0)) : &(m_outData->o_BTagExtrapFromCharmDown_TRF_ex->at(0));
      m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_from_charm_Up", temp_extrap_from_charm_up -> at(req_nbjets) );
      m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_from_charm_Down", temp_extrap_from_charm_down -> at(req_nbjets) );
    }//if compute weight systematics
    m_weightMngr -> ComputeAllWeights();//propagates the change to all weights handled by WeightManager
    if( m_opt->ReweightKinematics() && !m_opt->ComputeWeightSys() ){
      m_weightMngr -> ComputeSystematicWeights();
    }

    //
    // Now handling the vector o_bjets that has to be cleared and refilled again with the proper combination
    //
    m_outData -> o_bjets -> clear();
    m_outData -> o_bjets_n = 0;
    const std::vector < bool > *permutations = &( isIncl ? m_outData->o_TRFPerm_in->at(req_nbjets) : m_outData->o_TRFPerm_ex->at(req_nbjets) );
    if(permutations->size() != m_outData->o_jets->size()){
      std::cout << "<!> Error in VLQ_TRFManager::UpdateBTagging(): the vector of permutations and the vector of jets don't have the same size !" << std::endl;
      std::cout << "    This should NEVER happen !" << std::endl;
      return false;
    }
    int counter = 0;
    for ( AnalysisObject* jet : *(m_outData->o_jets) ){
      if(permutations->at(counter)){
        m_outData -> o_bjets -> push_back(jet);
      }
      counter++;
    }
    m_outData -> o_bjets_n = m_outData -> o_bjets -> size();

  } 
  else if ( m_opt -> DoTRF() ){
    //
    // Only a few combinations are in the tree ... so, let's return 0 is case this is an unknown combination
    //
    if( ( isIncl && (req_nbjets==2 || req_nbjets==3) ) || ( !isIncl && (req_nbjets==0 || req_nbjets==1) ) ){
      m_weightMngr->SetNominalComponent("weight_btag", 0. );
      if(m_opt -> ComputeWeightSys() && m_opt->DoExpSys()){
        for ( unsigned int ibev = 0; ibev <= 5; ++ibev ) {
          m_weightMngr->SetSystematicComponent(Form("weight_btag_B_EV_Up_%i",ibev), 0. );
          m_weightMngr->SetSystematicComponent(Form("weight_btag_B_EV_Down_%i",ibev), 0. );
        }
        for ( unsigned int icev = 0; icev <= 3; ++icev ) {
          m_weightMngr->SetSystematicComponent(Form("weight_btag_C_EV_Up_%i",icev), 0. );
          m_weightMngr->SetSystematicComponent(Form("weight_btag_C_EV_Down_%i",icev), 0. );
        }
        for ( unsigned int ilev = 0; ilev <= 16; ++ilev ) {
          m_weightMngr->SetSystematicComponent(Form("weight_btag_Light_EV_Up_%i",ilev), 0. );
          m_weightMngr->SetSystematicComponent(Form("weight_btag_Light_EV_Down_%i",ilev), 0. );
        }
        m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_Up", 0. );
        m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_Down", 0. );
        m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_from_charm_Up", 0. );
        m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_from_charm_Down", 0. );
      }
      m_outData -> o_TRF_bjets_n  = req_nbjets;
      m_outData -> o_TRF_isIncl   = isIncl;
      return true;
    }

    //
    // We have to set the variables based on what is in the tree
    //
    //
    //---~~~~>> Nominal
    //
    double trf_weight = 0;
    if( !isIncl && req_nbjets==2 ) trf_weight = m_ntupData -> d_trf_weight_77_2ex;
    else if( !isIncl && req_nbjets==3 ) trf_weight = m_ntupData -> d_trf_weight_77_3ex;
    else if( isIncl && req_nbjets==4 ) trf_weight = m_ntupData -> d_trf_weight_77_4in;
    m_weightMngr->SetNominalComponent( "weight_btag", trf_weight );

    //
    //---~~~~>> Systematics
    //
    if(m_opt -> ComputeWeightSys() && m_opt->DoExpSys()){
      //-- b
      std::vector < double > *BTag_BreakUp_TRF = nullptr;
      std::vector < double > *BTag_BreakDown_TRF = nullptr;
      if( !isIncl && req_nbjets==2 ){
        BTag_BreakUp_TRF = m_ntupData -> d_trf_weight_77_2ex_eigenvars_B_up;
        BTag_BreakDown_TRF = m_ntupData -> d_trf_weight_77_2ex_eigenvars_B_down;
      } else if( !isIncl && req_nbjets==3 ){
        BTag_BreakUp_TRF = m_ntupData -> d_trf_weight_77_3ex_eigenvars_B_up;
        BTag_BreakDown_TRF = m_ntupData -> d_trf_weight_77_3ex_eigenvars_B_down;
      } else if( isIncl && req_nbjets==4 ){
        BTag_BreakUp_TRF = m_ntupData -> d_trf_weight_77_4in_eigenvars_B_up;
        BTag_BreakDown_TRF = m_ntupData -> d_trf_weight_77_4in_eigenvars_B_down;
      }
      for ( unsigned int ibev = 0; ibev < BTag_BreakUp_TRF -> size(); ++ibev ) {
        m_weightMngr->SetSystematicComponent( Form("weight_btag_B_EV_Up_%i",ibev), BTag_BreakUp_TRF -> at(ibev) );
        m_weightMngr->SetSystematicComponent( Form("weight_btag_B_EV_Down_%i",ibev), BTag_BreakDown_TRF -> at(ibev) );
      }
      //-- c
      std::vector < double > *CTag_BreakUp_TRF = nullptr;
      std::vector < double > *CTag_BreakDown_TRF = nullptr;
      if( !isIncl && req_nbjets==2 ){
        CTag_BreakUp_TRF = m_ntupData -> d_trf_weight_77_2ex_eigenvars_C_up;
        CTag_BreakDown_TRF = m_ntupData -> d_trf_weight_77_2ex_eigenvars_C_down;
      } else if( !isIncl && req_nbjets==3 ){
        CTag_BreakUp_TRF = m_ntupData -> d_trf_weight_77_3ex_eigenvars_C_up;
        CTag_BreakDown_TRF = m_ntupData -> d_trf_weight_77_3ex_eigenvars_C_down;
      } else if( isIncl && req_nbjets==4 ){
        CTag_BreakUp_TRF = m_ntupData -> d_trf_weight_77_4in_eigenvars_C_up;
        CTag_BreakDown_TRF = m_ntupData -> d_trf_weight_77_4in_eigenvars_C_down;
      }
      for ( unsigned int icev = 0; icev < CTag_BreakUp_TRF -> size(); ++icev ) {
        m_weightMngr->SetSystematicComponent( Form("weight_btag_C_EV_Up_%i",icev), CTag_BreakUp_TRF -> at(icev) );
        m_weightMngr->SetSystematicComponent( Form("weight_btag_C_EV_Down_%i",icev), CTag_BreakDown_TRF -> at(icev) );
      }
      //-- l
      std::vector < double > *LTag_BreakUp_TRF = nullptr;
      std::vector < double > *LTag_BreakDown_TRF = nullptr;
      if( !isIncl && req_nbjets==2 ){
        LTag_BreakUp_TRF = m_ntupData -> d_trf_weight_77_2ex_eigenvars_Light_up;
        LTag_BreakDown_TRF = m_ntupData -> d_trf_weight_77_2ex_eigenvars_Light_down;
      } else if( !isIncl && req_nbjets==3 ){
        LTag_BreakUp_TRF = m_ntupData -> d_trf_weight_77_3ex_eigenvars_Light_up;
        LTag_BreakDown_TRF = m_ntupData -> d_trf_weight_77_3ex_eigenvars_Light_down;
      } else if( isIncl && req_nbjets==4 ){
        LTag_BreakUp_TRF = m_ntupData -> d_trf_weight_77_4in_eigenvars_Light_up;
        LTag_BreakDown_TRF = m_ntupData -> d_trf_weight_77_4in_eigenvars_Light_down;
      }
      for ( unsigned int ilev = 0; ilev < LTag_BreakUp_TRF -> size(); ++ilev ) {
        m_weightMngr->SetSystematicComponent(Form("weight_btag_Light_EV_Up_%i",ilev), LTag_BreakUp_TRF -> at(ilev) );
        m_weightMngr->SetSystematicComponent(Form("weight_btag_Light_EV_Down_%i",ilev), LTag_BreakDown_TRF -> at(ilev) );
      }
      //-- extrapolation
      double extrapolation_up = 0.;
      double extrapolation_down = 0.;
      double extrapolation_from_charm_up = 0.;
      double extrapolation_from_charm_down = 0.;
      if( !isIncl && req_nbjets==2 ){
        extrapolation_up              = m_ntupData -> d_trf_weight_77_2ex_extrapolation_up;
        extrapolation_down            = m_ntupData -> d_trf_weight_77_2ex_extrapolation_down;
        extrapolation_from_charm_up   = m_ntupData -> d_trf_weight_77_2ex_extrapolation_from_charm_up;
        extrapolation_from_charm_down = m_ntupData -> d_trf_weight_77_2ex_extrapolation_from_charm_down;
      } else if( !isIncl && req_nbjets==3 ){
        extrapolation_up              = m_ntupData -> d_trf_weight_77_3ex_extrapolation_up;
        extrapolation_down            = m_ntupData -> d_trf_weight_77_3ex_extrapolation_down;
        extrapolation_from_charm_up   = m_ntupData -> d_trf_weight_77_3ex_extrapolation_from_charm_up;
        extrapolation_from_charm_down = m_ntupData -> d_trf_weight_77_3ex_extrapolation_from_charm_down;

      } else if( isIncl && req_nbjets==4 ){
        extrapolation_up              = m_ntupData -> d_trf_weight_77_4in_extrapolation_up;
        extrapolation_down            = m_ntupData -> d_trf_weight_77_4in_extrapolation_down;
        extrapolation_from_charm_up   = m_ntupData -> d_trf_weight_77_4in_extrapolation_from_charm_up;
        extrapolation_from_charm_down = m_ntupData -> d_trf_weight_77_4in_extrapolation_from_charm_down;
      }
      m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_Up", extrapolation_up );
      m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_Down", extrapolation_down );
      m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_from_charm_Up", extrapolation_from_charm_up );
      m_weightMngr->SetSystematicComponent( "weight_btag_extrapolation_from_charm_Down", extrapolation_from_charm_down );
    }
    m_weightMngr -> ComputeAllWeights();//propagates the change to all weights handled by WeightManager
    if( m_opt->ReweightKinematics() && !m_opt->ComputeWeightSys() ){
      m_weightMngr -> ComputeSystematicWeights();
    }

    //
    // Now handling the vector o_bjets that has to be cleared and refilled again with the proper combination
    //
    m_outData -> o_bjets -> clear();
    m_outData -> o_bjets_n = 0;
    const std::vector < bool > *permutations = nullptr;
    if( !isIncl && req_nbjets==2 ) permutations = m_ntupData -> d_trf_tagged_77_2ex;
    else if( !isIncl && req_nbjets==3 ) permutations = m_ntupData -> d_trf_tagged_77_3ex;
    else if( isIncl && req_nbjets==4 ) permutations = m_ntupData -> d_trf_tagged_77_4in;
    if( !permutations ){
      std::cout << "<!> Error in VLQ_TRFManager::UpdateBTagging(): the permutation vector cannot be found !!!" << std::endl;
      return false;
    }
    if(permutations->size() != m_outData->o_jets->size()){
      std::cout << "<!> Error in VLQ_TRFManager::UpdateBTagging(): the vector of permutations and the vector of jets don't have the same size !" << std::endl;
      std::cout << "    This should NEVER happen !" << std::endl;
      return false;
    }

    int counter = 0;
    for ( AnalysisObject* jet : *(m_outData->o_jets) ){
      if(permutations->at(counter)){
        m_outData -> o_bjets -> push_back(jet);
      }
      counter++;
    }
    m_outData -> o_bjets_n = m_outData -> o_bjets -> size();
  }//end of RecomputeTRF() === false

  m_outData -> o_TRF_bjets_n  = req_nbjets;
  m_outData -> o_TRF_isIncl   = isIncl;

  return true;
}
