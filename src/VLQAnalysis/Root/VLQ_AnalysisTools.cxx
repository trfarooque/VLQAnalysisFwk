#include "VLQAnalysis/VLQ_AnalysisTools.h"

#include "IFAETopFramework/OutputHistManager.h"
#include "IFAETopFramework/AnalysisObject.h"
#include "IFAETopFramework/AnalysisUtils.h"
#include "IFAETopFramework/TriggerInfo.h"

#include "VLQAnalysis/VLQ_NtupleData.h"
#include "VLQAnalysis/VLQ_Options.h"
#include "VLQAnalysis/VLQ_OutputData.h"
#include "VLQAnalysis/VLQ_TRFManager.h"
#include "VLQAnalysis/VLQ_VariableComputer.h"
#include "VLQAnalysis/VLQ_ResonanceMaker.h"

#include "VLQAnalysis/VLQ_WeightManager.h"
#include "VLQAnalysis/VLQ_Enums.h"

#include <iostream>
#include <utility> // pair

using std::cout;
using std::endl;
using std::pair;

//_________________________________________________________________________
//
VLQ_AnalysisTools::VLQ_AnalysisTools( VLQ_Options* opt, OutputHistManager* outMngr,
  const VLQ_NtupleData *m_ntupData, VLQ_OutputData* outData, VLQ_WeightManager *weightManager,
  VLQ_TRFManager* trfMngr, VLQ_VariableComputer *varCptr ):
m_opt(opt),
m_outputMngr(outMngr),
m_ntupData(m_ntupData),
m_outData(outData),
m_weightMngr(weightManager),
m_trfMngr(trfMngr),
m_varComputer(varCptr),
m_resonance_maker(nullptr),
m_lepWRecoOpt(0),
m_leptopRecoOpt(0)
{

  m_resonance_maker = new VLQ_ResonanceMaker(opt, outData);

  if(m_opt->LepWOpt().find("VANILLA") != std::string::npos)
    m_lepWRecoOpt |= VLQ_ResonanceMaker::VANILLA;
  if(m_opt->LepWOpt().find("COLLINEAR") != std::string::npos)
    m_lepWRecoOpt |= VLQ_ResonanceMaker::COLLINEAR;
  if(m_opt->LepWOpt().find("MW_CONSTRAINT") != std::string::npos)
    m_lepWRecoOpt |= VLQ_ResonanceMaker::MW_CONSTRAINT;


  if(m_opt->LeptopOpt().find("VETO_RCMATCH") != std::string::npos)
    m_leptopRecoOpt |= VLQ_ResonanceMaker::VETO_RCMATCH;
  if(m_opt->LeptopOpt().find("USE_LIGHT") != std::string::npos)
    m_leptopRecoOpt |= VLQ_ResonanceMaker::USE_LIGHT;
  if(m_opt->LeptopOpt().find("PREF_BTAG") != std::string::npos)
    m_leptopRecoOpt |= VLQ_ResonanceMaker::PREF_BTAG;

}

//_________________________________________________________________________
//
VLQ_AnalysisTools::VLQ_AnalysisTools( const VLQ_AnalysisTools &q )
{
  m_opt               = q.m_opt;
  m_outputMngr        = q.m_outputMngr;
  m_ntupData          = q.m_ntupData;
  m_outData           = q.m_outData;
  m_trfMngr           = q.m_trfMngr;
  m_resonance_maker   = q.m_resonance_maker;
}

//_________________________________________________________________________
//
VLQ_AnalysisTools::~VLQ_AnalysisTools(){
  delete m_resonance_maker;
}

//_________________________________________________________________________
//
bool VLQ_AnalysisTools::GetObjectVectors(){

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> Event number = " << m_ntupData -> d_eventNumber   << std::endl;

  //Leptons pTcut
  const double pTCut = m_opt -> LepPtCut();

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Electrons
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  for ( unsigned int iEle = 0; iEle < m_ntupData -> d_el_pt -> size(); ++iEle ) {
    if( m_ntupData -> d_el_pt -> at(iEle) < pTCut ) continue;
    if( TMath::Abs(m_ntupData -> d_el_eta -> at(iEle)) > 2.47  ) continue;
    m_outData->o_el_loose_n++;
    m_outData->o_lep_loose_n++;
    //if(!(m_opt->IsR21() || m_opt -> SampleName() == SampleName::QCDE || m_ntupData  -> d_el_isSignal -> at(iEle)) ) continue;

    if(m_opt -> MsgLevel() == Debug::DEBUG){
      std::cout << "==> In Electron builder:  pt " << m_ntupData -> d_el_pt->at(iEle);
      std::cout << "   eta " << m_ntupData -> d_el_eta -> at(iEle) << std::endl;
    }

    AnalysisObject *obj = new AnalysisObject();
    obj -> SetPtEtaPhiE( m_ntupData -> d_el_pt->at(iEle), m_ntupData -> d_el_eta -> at(iEle), m_ntupData -> d_el_phi -> at(iEle), m_ntupData -> d_el_e -> at(iEle) );
    //other information for plotting
    /*
      obj -> SetMoment("d0sig"        ,   m_ntupData -> d_el_d0sig        -> at(iEle));
      obj -> SetMoment("z0"           ,   m_ntupData -> d_el_z0           -> at(iEle));
      obj -> SetMoment("ptvarcone20"  ,   m_ntupData -> d_el_ptvarcone20  -> at(iEle)/m_ntupData -> d_el_pt->at(iEle));
      obj -> SetMoment("topoetcone20" ,   m_ntupData -> d_el_topoetcone20 -> at(iEle));
      obj -> SetMoment("isSignal"     ,   m_ntupData -> d_el_isSignal     -> at(iEle) ); //to get the fake weights
    */
    //trigger matching information
    for(TriggerInfo* trig : m_outData -> o_trigger_list){
      if( ! ( (trig->Type() == VLQ_Enums::TRIGELEC) && (m_outData->o_period & trig->Period()) ) ) continue;
      obj -> SetMoment( trig->Name(), m_ntupData -> d_el_trigMatch.at(trig->Name())->at(iEle) );
    }
    m_outData -> o_el -> push_back(obj);
    m_outData -> o_lep -> push_back(obj);
  }
  m_outData -> o_el_n = m_outData -> o_el -> size();

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After filling electrons" << std::endl;

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Muons
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  for ( unsigned int iMu = 0; iMu < m_ntupData -> d_mu_pt -> size(); ++iMu ) {

    if(m_opt -> MsgLevel() == Debug::DEBUG){
      std::cout << "==> In Muon builder:  pt " << m_ntupData  -> d_mu_pt-> at(iMu);
      std::cout << "   eta " << m_ntupData -> d_mu_eta -> at(iMu) << std::endl;
    }

    if( m_ntupData  -> d_mu_pt -> at(iMu) < pTCut  ) continue;
    if( TMath::Abs(m_ntupData  -> d_mu_eta -> at(iMu)) > 2.5 ) continue;
    m_outData->o_mu_loose_n++;
    m_outData->o_lep_loose_n++;

    //if(!(m_opt->IsR21() || m_opt -> SampleName() == SampleName::QCDMU || m_ntupData  -> d_mu_isSignal -> at(iMu)) ) continue;

    AnalysisObject *obj = new AnalysisObject();
    obj -> SetPtEtaPhiE( m_ntupData -> d_mu_pt -> at(iMu), m_ntupData -> d_mu_eta -> at(iMu), m_ntupData -> d_mu_phi -> at(iMu), m_ntupData -> d_mu_e -> at(iMu) );
    //if(!m_opt->IsR21()){
    /*
      //some more information to be stored in the outputs
      obj -> SetMoment( "d0sig"        ,   m_ntupData -> d_mu_d0sig           -> at(iMu) );
      obj -> SetMoment( "z0"           ,   m_ntupData -> d_mu_z0              -> at(iMu) );
      obj -> SetMoment( "ptvarcone30"  ,   m_ntupData -> d_mu_ptvarcone30     -> at(iMu) / m_ntupData -> d_mu_pt -> at(iMu));
      obj -> SetMoment( "topoetcone20" ,   m_ntupData -> d_mu_topoetcone20    -> at(iMu));
      obj -> SetMoment( "isSignal"      ,   m_ntupData -> d_mu_isSignal -> at(iMu) ); //to check fake weights
    */
    //}
    //trigger matching information
    for(TriggerInfo* trig : m_outData -> o_trigger_list){
      if( ! ( (trig->Type() == VLQ_Enums::TRIGMUON) && (m_outData->o_period & trig->Period()) ) ) continue;
      obj -> SetMoment( trig->Name(), m_ntupData->d_mu_trigMatch.at(trig->Name())->at(iMu) );
    }
    m_outData -> o_mu -> push_back(obj);
    m_outData -> o_lep -> push_back(obj);
  }
  m_outData -> o_mu_n = m_outData -> o_mu -> size();
  m_outData -> o_lep_n = m_outData -> o_lep -> size();
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After filling muons" << std::endl;

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Lepton veto
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  m_outData -> o_lepForVeto_n = m_outData -> o_lep -> size();


  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Selected Lepton
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  double pTmax_lep = 0;
  for(AnalysisObject* lep : *(m_outData -> o_lep)){
    if(lep->Pt() > pTmax_lep){
      pTmax_lep = lep->Pt();
      m_outData -> o_selLep = lep;
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Track jets -only if trkjet b-tagging is used
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if(m_opt->BtagCollection()==VLQ_Options::TRACK){

    for ( unsigned int iTJet = 0; iTJet < m_ntupData -> d_trkjet_pt -> size(); ++iTJet ) {

      if(m_opt -> MsgLevel() == Debug::DEBUG){
	std::cout << "==> In the Track Jet builder:  pt " << m_ntupData -> d_trkjet_pt -> at(iTJet);
	std::cout << "    eta " <<  TMath::Abs(m_ntupData -> d_trkjet_eta -> at(iTJet))  << std::endl;
      }

      bool isSignalJet = m_ntupData -> d_trkjet_pt -> at(iTJet)  >= m_opt->TrkJetsPtCut();
      isSignalJet = isSignalJet && TMath::Abs( m_ntupData -> d_trkjet_eta -> at(iTJet) ) < 2.5;
      if( isSignalJet ){
	AnalysisObject *obj = new AnalysisObject();
	obj -> SetPtEtaPhiM( m_ntupData -> d_trkjet_pt -> at(iTJet), m_ntupData -> d_trkjet_eta -> at(iTJet), m_ntupData -> d_trkjet_phi -> at(iTJet), 0. );

	int isB = m_ntupData -> d_trkjet_isb -> at(iTJet);
	obj -> SetMoment("isb", isB );
	obj -> SetMoment("btagw", m_ntupData -> d_trkjet_btag_weight -> at(iTJet) );

	if ( !(m_opt -> IsData() || (m_opt -> StrSampleName().find("QCD") != std::string::npos)) ) {
	  obj -> SetMoment("truthLabel", m_ntupData -> d_trkjet_truthLabel -> at(iTJet) );
	}

	obj->SetMoment("calomatch",0); //initial default
	obj -> SetMoment("bjet",isB);

	double reff = max(0.02,min(0.4,30./obj->Pt()));
	obj->SetMoment("reff",reff);

	if( isB == 1 ){
	  m_outData -> o_trkbjets -> push_back(obj);

	  if( !m_opt->IsData() ){
	    if( abs(obj->GetMoment("truthLabel")) == 5 ){
	      m_outData -> o_trkbjets_truth_b -> push_back(obj);
	    }
	    else if( abs(obj->GetMoment("truthLabel")) == 4 ){
	      m_outData -> o_trkbjets_truth_c -> push_back(obj);
	    }
	    else if( abs(obj->GetMoment("truthLabel")) == 15 ){
	      m_outData -> o_trkbjets_truth_tau -> push_back(obj);
	    }
	    else{
	      m_outData -> o_trkbjets_truth_lqg -> push_back(obj);
	    }

	  }//MC

	} else {
	  m_outData -> o_trkljets -> push_back(obj);
	}

	m_outData -> o_trkjets -> push_back(obj);

	if( !m_opt->IsData() ){
	  if( abs(obj->GetMoment("truthLabel")) == 5 ){
	    m_outData -> o_trkjets_truth_b -> push_back(obj);
	  }
	  else if( abs(obj->GetMoment("truthLabel")) == 4 ){
	    m_outData -> o_trkjets_truth_c -> push_back(obj);
	  }
	  else if( abs(obj->GetMoment("truthLabel")) == 15 ){
	    m_outData -> o_trkjets_truth_tau -> push_back(obj);
	  }
	  else{
	    m_outData -> o_trkjets_truth_lqg -> push_back(obj);
	  }

	}//MC


      }

    }//loop over all track jets


    //=============== Re-run loop to clean concentric track jets ===========

    for( std::vector<AnalysisObject*>::iterator tj_i = m_outData->o_trkjets->begin();
	 tj_i < (m_outData -> o_trkjets)->end(); ){

      double reff_i = (*tj_i)->GetMoment("reff");
      bool is_concentric = false;

      for( std::vector<AnalysisObject*>::iterator tj_j = tj_i+1;
	   tj_j < (m_outData -> o_trkjets)->end(); ){

	double reff_j = (*tj_j)->GetMoment("reff");

	double dr = (*tj_i)->DeltaR(*(*tj_j));
	if(dr < min(reff_i,reff_j) ){
	  is_concentric = true;
	  tj_j = (m_outData -> o_trkjets)->erase(tj_j);
	  m_outData -> o_trkjets_nconcentric++;
	}
	else{
	  tj_j++;
	}

      }//inner loop

      if(is_concentric){
	tj_i = (m_outData -> o_trkjets)->erase(tj_i);
      }
      else{
	tj_i++;
      }

    }//

    m_outData -> o_trkjets_n      = m_outData -> o_trkjets -> size();
    m_outData -> o_trkbjets_n      = m_outData -> o_trkbjets -> size();
    m_outData -> o_trkljets_n      = m_outData -> o_trkljets -> size();

    if( !m_opt->IsData() ){
      m_outData -> o_trkjets_truth_b_n = m_outData -> o_trkjets_truth_b -> size();
      m_outData -> o_trkjets_truth_c_n = m_outData -> o_trkjets_truth_c -> size();
      m_outData -> o_trkjets_truth_tau_n = m_outData -> o_trkjets_truth_tau -> size();
      m_outData -> o_trkjets_truth_lqg_n = m_outData -> o_trkjets_truth_lqg -> size();

      m_outData -> o_trkbjets_truth_b_n = m_outData -> o_trkbjets_truth_b -> size();
      m_outData -> o_trkbjets_truth_c_n = m_outData -> o_trkbjets_truth_c -> size();
      m_outData -> o_trkbjets_truth_tau_n = m_outData -> o_trkbjets_truth_tau -> size();
      m_outData -> o_trkbjets_truth_lqg_n = m_outData -> o_trkbjets_truth_lqg -> size();

    }//MC


  } // if track jets are to be used


  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Small-radius jets
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  for ( unsigned int iJet = 0; iJet < m_ntupData -> d_jet_pt -> size(); ++iJet ) {

    if(m_opt -> MsgLevel() == Debug::DEBUG){
      std::cout << "==> In the Jet builder:  pt " << m_ntupData -> d_jet_pt -> at(iJet);
      std::cout << "    eta " <<  TMath::Abs(m_ntupData -> d_jet_eta -> at(iJet))  << std::endl;
    }

    bool isSignalJet = m_ntupData -> d_jet_pt -> at(iJet)  >= m_opt->JetsPtCut();
    isSignalJet = isSignalJet && TMath::Abs( m_ntupData -> d_jet_eta -> at(iJet) ) < m_opt->JetsEtaCut();
    isSignalJet = isSignalJet && ( ( (m_ntupData -> d_jet_jvt -> at(iJet) > 0.11) && (TMath::Abs( m_ntupData -> d_jet_eta -> at(iJet) ) > 2.4) )
    				   || (m_ntupData -> d_jet_jvt -> at(iJet) > 0.59)
    				   || (m_ntupData -> d_jet_pt -> at(iJet) > 120.) );
    if( isSignalJet ){
      AnalysisObject *obj = new AnalysisObject();
      obj -> SetPtEtaPhiE( m_ntupData -> d_jet_pt -> at(iJet), m_ntupData -> d_jet_eta -> at(iJet), m_ntupData -> d_jet_phi -> at(iJet), m_ntupData -> d_jet_E -> at(iJet) );

      obj -> SetMoment("isb", ( m_ntupData -> d_jet_isb -> at(iJet) )  );
      obj -> SetMoment("btagw",    m_ntupData -> d_jet_btag_weight -> at(iJet) );

      if ( !(m_opt -> IsData() || (m_opt -> StrSampleName().find("QCD") != std::string::npos)) ) {
        obj -> SetMoment("truthLabel", m_ntupData -> d_jet_truthLabel -> at(iJet) );
      }
      obj -> SetMoment("jvt", m_ntupData -> d_jet_jvt -> at(iJet) );

      obj -> SetMoment("RCtag_match", 0); //initial default

      int isB = 0;

      if(m_opt->BtagCollection()==VLQ_Options::CALOTOPO || m_opt->BtagCollection()==VLQ_Options::CALOPFLOW){
	if(TMath::Abs(obj->Eta())<2.5){
          isB = obj -> GetMoment("isb");
	}
      }
      else{
	//Match to track b-jets
	for(AnalysisObject* trkbjet : *(m_outData->o_trkbjets)){
	  if(trkbjet->GetMoment("calomatch")>0) continue;
	  if(trkbjet->DeltaR(*obj) < 0.4){
	    isB = 1; trkbjet->SetMoment("calomatch",1);
	    break;
	  }
	}

      }

      obj -> SetMoment("bjet",isB);

      if( isB == 1 ){
	m_outData -> o_bjets -> push_back(obj);

	if( !m_opt->IsData() ){
	  if( abs(obj->GetMoment("truthLabel")) == 5 ){
	    m_outData -> o_bjets_truth_b -> push_back(obj);
	  }
	  else if( abs(obj->GetMoment("truthLabel")) == 4 ){
	    m_outData -> o_bjets_truth_c -> push_back(obj);
	  }
	  else if( abs(obj->GetMoment("truthLabel")) == 15 ){
	    m_outData -> o_bjets_truth_tau -> push_back(obj);
	  }
	  else{
	    m_outData -> o_bjets_truth_lqg -> push_back(obj);
	  }

	}//MC

      } else {
	m_outData -> o_ljets -> push_back(obj);
      }


      m_outData -> o_jets -> push_back(obj);

      if( !m_opt->IsData() ){
	if( abs(obj->GetMoment("truthLabel")) == 5 ){
	  m_outData -> o_jets_truth_b -> push_back(obj);
	}
	else if( abs(obj->GetMoment("truthLabel")) == 4 ){
	  m_outData -> o_jets_truth_c -> push_back(obj);
	}
	else if( abs(obj->GetMoment("truthLabel")) == 15 ){
	  m_outData -> o_jets_truth_tau -> push_back(obj);
	}
	else{
	  m_outData -> o_jets_truth_lqg -> push_back(obj);
	}

      }//MC

    }

  }//loop over all jets


  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Forward-radius jets
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  for ( unsigned int iFwdjet = 0; iFwdjet < m_ntupData -> d_fwdjet_pt -> size(); ++iFwdjet ) {

    if(m_opt -> MsgLevel() == Debug::DEBUG){
      std::cout << "==> In the Fwd-jet builder:  pt " << m_ntupData -> d_fwdjet_pt -> at(iFwdjet);
      std::cout << "    eta " <<  TMath::Abs(m_ntupData -> d_fwdjet_eta -> at(iFwdjet))  << std::endl;
    }
    // Forward jets
    bool isFwdJet = m_ntupData -> d_fwdjet_pt -> at(iFwdjet)  >= m_opt->FwdJetsPtCut();
    isFwdJet = isFwdJet && TMath::Abs( m_ntupData -> d_fwdjet_eta -> at(iFwdjet) ) < m_opt->FwdJetsEtaCut() && TMath::Abs( m_ntupData -> d_fwdjet_eta -> at(iFwdjet) ) >= m_opt->JetsEtaCut();
    isFwdJet = isFwdJet && ( (m_ntupData -> d_fwdjet_fJvt -> at(iFwdjet) < 0.5) || (m_ntupData -> d_fwdjet_pt -> at(iFwdjet) > 120.) );

    if( isFwdJet ){
      AnalysisObject *obj = new AnalysisObject();
      obj -> SetPtEtaPhiE( m_ntupData -> d_fwdjet_pt -> at(iFwdjet), m_ntupData -> d_fwdjet_eta -> at(iFwdjet), m_ntupData -> d_fwdjet_phi -> at(iFwdjet), m_ntupData -> d_fwdjet_E -> at(iFwdjet) );
      m_outData -> o_fwdjets -> push_back(obj);
      if(TMath::Abs(obj->Eta()) < 3.0 ) m_outData -> o_fwdjets_eta25_30 -> push_back(obj);
      else if(TMath::Abs(obj->Eta()) < 3.5 ) m_outData -> o_fwdjets_eta30_35 -> push_back(obj);
      else if(TMath::Abs(obj->Eta()) < 4.0 ) m_outData -> o_fwdjets_eta35_40 -> push_back(obj);
      else if(TMath::Abs(obj->Eta()) < 4.5 ) m_outData -> o_fwdjets_eta40_45 -> push_back(obj);
    }
  }//loop over all fwd jets
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After filling fwd jets  " << std::endl;

  //If doing low-b regions, fill the low-b fake b's
  if( m_opt->DoLowBRegions() ){
    //ONLY FOR CALO-JETS
    AOVector bjets_lowb_3b = AnalysisUtils::SortObjectValues(*(m_outData->o_bjets), "btagw", 3 );
    AOVector bjets_lowb_4b = AnalysisUtils::SortObjectValues(*(m_outData->o_bjets), "btagw", 4 );
    m_outData -> o_bjets_lowb_3b -> insert( m_outData -> o_bjets_lowb_3b -> begin(), bjets_lowb_3b.begin(), bjets_lowb_3b.end()  );
    m_outData -> o_bjets_lowb_4b -> insert( m_outData -> o_bjets_lowb_4b -> begin(), bjets_lowb_4b.begin(), bjets_lowb_4b.end()  );
  }
  m_outData -> o_jets_n       = m_outData -> o_jets  -> size();
  m_outData -> o_fwdjets_n    = m_outData -> o_fwdjets  -> size();
  m_outData -> o_fwdjets_eta25_30_n    = m_outData -> o_fwdjets_eta25_30  -> size();
  m_outData -> o_fwdjets_eta30_35_n    = m_outData -> o_fwdjets_eta30_35  -> size();
  m_outData -> o_fwdjets_eta35_40_n    = m_outData -> o_fwdjets_eta35_40  -> size();
  m_outData -> o_fwdjets_eta40_45_n    = m_outData -> o_fwdjets_eta40_45  -> size();
  m_outData -> o_bjets_n      = m_outData -> o_bjets -> size();
  m_outData -> o_ljets_n      = m_outData -> o_ljets -> size();

  if( !m_opt->IsData() ){
    m_outData -> o_jets_truth_b_n = m_outData -> o_jets_truth_b -> size();
    m_outData -> o_jets_truth_c_n = m_outData -> o_jets_truth_c -> size();
    m_outData -> o_jets_truth_tau_n = m_outData -> o_jets_truth_tau -> size();
    m_outData -> o_jets_truth_lqg_n = m_outData -> o_jets_truth_lqg -> size();

    m_outData -> o_bjets_truth_b_n = m_outData -> o_bjets_truth_b -> size();
    m_outData -> o_bjets_truth_c_n = m_outData -> o_bjets_truth_c -> size();
    m_outData -> o_bjets_truth_tau_n = m_outData -> o_bjets_truth_tau -> size();
    m_outData -> o_bjets_truth_lqg_n = m_outData -> o_bjets_truth_lqg -> size();

  }//MC

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After filling jets: signalJets vector  " << std::endl;

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // MET
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  m_outData -> o_AO_met = new AnalysisObject();
  m_outData -> o_AO_met -> SetPtEtaPhiM( m_ntupData -> d_met_met, 0, m_ntupData -> d_met_phi, 0. );

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Reclustered jets
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  for ( unsigned int iRCJet = 0; iRCJet < m_ntupData -> d_rcjets_pt -> size(); ++iRCJet ) {

    if(m_opt -> MsgLevel() == Debug::DEBUG){
      std::cout << "==> In reclustered jet builder:    pt " << m_ntupData -> d_rcjets_pt -> at(iRCJet);
      std::cout << "    eta " <<  TMath::Abs(m_ntupData -> d_rcjets_eta -> at(iRCJet));
      std::cout << "    nconsts " << m_ntupData -> d_rcjets_nconsts -> at(iRCJet);
    }

    bool isSignalRCJet = m_ntupData -> d_rcjets_pt -> at(iRCJet) >= m_opt->RCJetsPtCut();
    isSignalRCJet = isSignalRCJet && TMath::Abs(m_ntupData -> d_rcjets_eta -> at(iRCJet)) < 2.0;
    if( isSignalRCJet ){
      AnalysisObject *obj = new AnalysisObject();
      obj -> SetPtEtaPhiM( m_ntupData -> d_rcjets_pt -> at(iRCJet), m_ntupData -> d_rcjets_eta -> at(iRCJet),
      m_ntupData -> d_rcjets_phi -> at(iRCJet), m_ntupData -> d_rcjets_m -> at(iRCJet) );
      obj -> SetMoment("nconsts",     m_ntupData -> d_rcjets_nconsts -> at(iRCJet));

      int nb_match = 0;
      //Find number of b-tagged jets matched to this jet (//do this inside BTagVariables)
      AOVector* source_bjets = (m_opt->BtagCollection() == VLQ_Options::TRACK) ? m_outData -> o_trkbjets : m_outData -> o_bjets ;
      for(AnalysisObject* sbjet : *(source_bjets)){
	if( obj->DeltaR(*sbjet) < 1.0 ) nb_match++;
      }
      obj -> SetMoment("nbconsts",   nb_match);

      //
      // Truth matching
      //
      obj -> SetMoment( "pdgId_truth", 0 );
      obj -> SetMoment( "nmatch_truth", 0 );


      //
      // Tagging by different algorithms
      //
      // Simple HOT jets
      const bool isHOT = obj->M() > 100 && m_ntupData -> d_rcjets_nconsts -> at(iRCJet) >= m_opt->RCNsubjetsCut() && obj -> Pt() > 300;
      obj -> SetMoment("isRCTTMass", isHOT);
      // Exclusive top-tagging
      bool isTop = obj -> Pt() > 400 && obj->M() > 140;
      if(m_opt -> DoOldBoost()){
	isTop = isTop && ( obj -> Pt() < 800 ? m_ntupData -> d_rcjets_nconsts -> at(iRCJet) >= 2 : m_ntupData -> d_rcjets_nconsts -> at(iRCJet) >= 1);
      }
      else{
	isTop = isTop && ( obj -> Pt() < 700 ? m_ntupData -> d_rcjets_nconsts -> at(iRCJet) >= 2 : m_ntupData -> d_rcjets_nconsts -> at(iRCJet) >= 1);
      }
      obj -> SetMoment("isRCMTop", isTop);
      // Exclusive Higgs tagging
      bool isHiggs = obj -> Pt() > 350 && obj->M() > 105 && obj->M() < 140;
      if(m_opt->DoOldBoost()){
	 isHiggs = isHiggs && ( obj -> Pt() < 500 ? m_ntupData -> d_rcjets_nconsts -> at(iRCJet) == 2 : m_ntupData -> d_rcjets_nconsts -> at(iRCJet) <= 2);
      }
      else{
	isHiggs = isHiggs && ( m_ntupData -> d_rcjets_nconsts -> at(iRCJet) == 2 ? obj -> Pt() > 350 :
			       m_ntupData -> d_rcjets_nconsts -> at(iRCJet) == 1 && obj -> Pt() > 600 );
      }
      obj -> SetMoment("isRCMHiggs", isHiggs);
      //Exclusive V(W/Z)-tagging
      bool isV = obj -> Pt() > 350 && obj->M() > 70 && obj->M() < 105;
      if(m_opt->DoOldBoost()){
	 isV = isV && ( obj -> Pt() < 400 ? m_ntupData -> d_rcjets_nconsts -> at(iRCJet) == 2 : m_ntupData -> d_rcjets_nconsts -> at(iRCJet) <= 2);
      }
      else{
	isV = isV && ( m_ntupData -> d_rcjets_nconsts -> at(iRCJet) == 2 ? obj -> Pt() > 350 :
		       m_ntupData -> d_rcjets_nconsts -> at(iRCJet) == 1 && obj -> Pt() > 450 );
      }
      obj -> SetMoment("isRCMV", isV);
      // Very loose HOT jets
      const bool isLooseHOT = obj->M() > 50 && m_ntupData -> d_rcjets_nconsts -> at(iRCJet) >= 1 && obj -> Pt() > 300;
      obj -> SetMoment("isLooseRCTTMass", isLooseHOT);

      obj -> SetMoment("isRCMTopHiggs", (isTop || isHiggs) );

      if( isHOT ){
        m_outData -> o_taggedjets.at("RCTTMass") -> push_back(obj);
      }
      if( isTop || isHiggs ){
        m_outData -> o_taggedjets.at("RCMTopHiggs") -> push_back(obj);
      }
      if( isTop ){
        m_outData -> o_taggedjets.at("RCMTop") -> push_back(obj);
	if(nb_match == 0) m_outData -> o_taggedjets.at("RCMTop0b") -> push_back(obj);
	else if(nb_match == 1) m_outData -> o_taggedjets.at("RCMTop1b") -> push_back(obj);
	else if(nb_match >= 2) m_outData -> o_taggedjets.at("RCMTop2bin") -> push_back(obj);

      }
      if( isHiggs ){
        m_outData -> o_taggedjets.at("RCMHiggs") -> push_back(obj);
	if(nb_match == 0) m_outData -> o_taggedjets.at("RCMHiggs0b") -> push_back(obj);
	else if(nb_match == 1) m_outData -> o_taggedjets.at("RCMHiggs1b") -> push_back(obj);
	else if(nb_match >= 2) m_outData -> o_taggedjets.at("RCMHiggs2bin") -> push_back(obj);
      }
      if( isV ){
        m_outData -> o_taggedjets.at("RCMV") -> push_back(obj);
	if(nb_match == 0) m_outData -> o_taggedjets.at("RCMV0b") -> push_back(obj);
	else if(nb_match >= 1) m_outData -> o_taggedjets.at("RCMV1bin") -> push_back(obj);
      }
      if( isLooseHOT ){
	m_outData -> o_taggedjets.at("LooseRCTTMass") -> push_back(obj);
      }

      //===== Loop over small-R jet container and set a flag if it is matched to one of the tagged jets ====
      if( isTop || isHiggs || isV ){
	for( AnalysisObject* jet : *(m_outData -> o_jets) ){
	  if(obj -> DeltaR(*jet) < 1.0) jet->UpdateMoment("RCtag_match", 1);
	}
      }

      if(m_outData -> o_selLep){
	obj -> SetMoment ("dPhi_lep", fabs(obj->DeltaPhi( *(m_outData -> o_selLep) )) );
	obj -> SetMoment ("dR_lep", fabs(obj->DeltaR( *(m_outData -> o_selLep) )) );
      }

      obj -> SetMoment ("dPhi_met", fabs(obj->DeltaPhi( *(m_outData -> o_AO_met) )) );

      m_outData -> o_rcjets -> push_back(obj);
    }
  }
  m_outData -> o_rcjets_n = m_outData -> o_rcjets -> size();
  m_outData -> o_taggedjets_n.at("RCTTMass") = m_outData -> o_taggedjets.at("RCTTMass")   -> size();
  m_outData -> o_taggedjets_n.at("RCMTop")   = m_outData -> o_taggedjets.at("RCMTop")     -> size();
  m_outData -> o_taggedjets_n.at("RCMTop0b")   = m_outData -> o_taggedjets.at("RCMTop0b")     -> size();
  m_outData -> o_taggedjets_n.at("RCMTop1b")   = m_outData -> o_taggedjets.at("RCMTop1b")     -> size();
  m_outData -> o_taggedjets_n.at("RCMTop2bin")   = m_outData -> o_taggedjets.at("RCMTop2bin")     -> size();
  m_outData -> o_taggedjets_n.at("RCMHiggs") = m_outData -> o_taggedjets.at("RCMHiggs")   -> size();
  m_outData -> o_taggedjets_n.at("RCMHiggs0b") = m_outData -> o_taggedjets.at("RCMHiggs0b")   -> size();
  m_outData -> o_taggedjets_n.at("RCMHiggs1b") = m_outData -> o_taggedjets.at("RCMHiggs1b")   -> size();
  m_outData -> o_taggedjets_n.at("RCMHiggs2bin") = m_outData -> o_taggedjets.at("RCMHiggs2bin")   -> size();
  m_outData -> o_taggedjets_n.at("RCMV")     = m_outData -> o_taggedjets.at("RCMV")       -> size();
  m_outData -> o_taggedjets_n.at("RCMV0b")     = m_outData -> o_taggedjets.at("RCMV0b")       -> size();
  m_outData -> o_taggedjets_n.at("RCMV1bin")     = m_outData -> o_taggedjets.at("RCMV1bin")       -> size();
  m_outData -> o_taggedjets_n.at("RCMTopHiggs")   = m_outData -> o_taggedjets.at("RCMTopHiggs")     -> size();
  m_outData -> o_taggedjets_n.at("LooseRCTTMass") = m_outData -> o_taggedjets.at("LooseRCTTMass")   -> size();
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After filling Reclustered jets" << std::endl;

  if( m_outData -> o_selLep ){
    //
    // Make leptonic W
    //
    //m_resonance_maker -> MakeLepW(VLQ_ResonanceMaker::MW_CONSTRAINT);
    m_resonance_maker -> MakeLepW(m_lepWRecoOpt);
    //m_resonance_maker -> MakeLepW(VLQ_ResonanceMaker::COLLINEAR);

  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Large-radius jets
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if( m_opt -> UseLargeRJets() ){
    for ( unsigned int iFJet = 0; iFJet < m_ntupData -> d_ljet_pt -> size(); ++iFJet ) {

      if(m_opt -> MsgLevel() == Debug::DEBUG){
        std::cout << "==> In fat jet builder:    pt " << m_ntupData -> d_ljet_pt -> at(iFJet);
        std::cout << "    eta " <<  TMath::Abs(m_ntupData -> d_ljet_eta -> at(iFJet));
        std::cout << "    TTL " << m_ntupData -> d_ljet_topTag -> at(iFJet);
        std::cout << "    TTT " << m_ntupData -> d_ljet_topTag_loose -> at(iFJet)  << std::endl;
      }

      if(m_ntupData -> d_ljet_pt -> at(iFJet) >= 300 && TMath::Abs(m_ntupData -> d_ljet_eta -> at(iFJet)) < 2.0){
        AnalysisObject *obj = new AnalysisObject();
        obj -> SetPtEtaPhiM( m_ntupData -> d_ljet_pt -> at(iFJet), m_ntupData -> d_ljet_eta -> at(iFJet), m_ntupData -> d_ljet_phi -> at(iFJet), m_ntupData -> d_ljet_m -> at(iFJet) );
        obj -> SetMoment("mass",    m_ntupData -> d_ljet_m -> at(iFJet));
        obj -> SetMoment("d12",     m_ntupData -> d_ljet_sd12 -> at(iFJet));
        obj -> SetMoment("d23",     m_ntupData -> d_ljet_sd23 -> at(iFJet));
        obj -> SetMoment("tau21",   m_ntupData -> d_ljet_tau21_wta -> at(iFJet));
        obj -> SetMoment("tau32",   m_ntupData -> d_ljet_tau32_wta -> at(iFJet));
        obj -> SetMoment("isTTL",   m_ntupData -> d_ljet_topTag_loose -> at(iFJet));
        obj -> SetMoment("isTTT",   m_ntupData -> d_ljet_topTag -> at(iFJet));
        obj -> SetMoment("isTTMass",  obj->M() > 100 ? 1 : 0);
        m_outData -> o_fjets -> push_back(obj);
      }
    }
    m_outData -> o_fjets_n = m_outData -> o_fjets -> size();
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After filling large-R jets" << std::endl;
  }

  if(m_opt -> MsgLevel() == Debug::DEBUG){

    std::cout << "==============================" << std::endl;
    std::cout << "Summary of selected objects" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << std::endl;
    std::cout << "----- Electrons -----" << std::endl;
    for ( const AnalysisObject* ele : *(m_outData->o_el) ) {
      std::cout << "   -> pt: " << ele->Pt() << "     eta: " << ele -> Eta() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "----- Muons -----" << std::endl;
    for ( const AnalysisObject* muon : *(m_outData->o_mu) ) {
      std::cout << "   -> pt: " << muon->Pt() << "     eta: " << muon -> Eta() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "----- Jets -----" << std::endl;
    // for ( const AnalysisObject* jet : *(m_outData->o_jets) ) {
    //   std::cout << "   -> pt: " << jet->Pt() << "     eta: " << jet->Eta() << "    isB60: " << jet->GetMoment("isb_60");
    //   std::cout << "    isB70: " << jet->GetMoment("isb_70")  << "   isB85: " <<  jet->GetMoment("isb_85") << std::endl;
    // }
    std::cout << std::endl;
    std::cout << "----- Reclustered jets -----" << std::endl;
    for ( const AnalysisObject* RCjet : *(m_outData->o_rcjets) ) {
      std::cout << "   -> pt: " << RCjet->Pt() << "     eta: " << RCjet->Eta() << "     m: " << RCjet->M() << std::endl;
    }
    if( m_opt -> UseLargeRJets() ){
      std::cout << std::endl;
      std::cout << "----- Large-R jets -----" << std::endl;
      for ( const AnalysisObject* LRJet : *(m_outData->o_fjets) ) {
        std::cout << "   -> pt: " << LRJet->Pt() << "     eta: " << LRJet->Eta() << "     m: " << LRJet->M() << std::endl;
      }
    }
  }
  return true;
}


//##########################################################
//
// Histograms utilities (handling of VLQ classification)
//
//##########################################################
//________________________________________________________________________
//
void VLQ_AnalysisTools::BookAllHistograms( const std::string &key/*, const bool split*/, const bool useSyst ){
  for ( unsigned int i = 0 ; i <= ( ( m_opt -> SampleName() == SampleName::VLQ  && m_opt -> SplitVLQDecays() /* && split*/ ) ? 6 : 0 ) ; ++i) {
    std::string name = key;
    if( i > 0 ){
      name += "_vlq";
      name += std::to_string(i);
    }
    m_outputMngr -> BookStandardTH1(name, useSyst);
    m_outputMngr -> BookStandardTH2(name, useSyst);
  }
}

//________________________________________________________________________
//
void VLQ_AnalysisTools::FillAllHistograms( const std::string &key, const int type/*, const bool split*/ ){

  if( m_opt -> SampleName() != SampleName::VLQ /*|| !split*/ || !m_opt -> SplitVLQDecays() ){
    m_outputMngr -> FillStandardTH1(key);
    m_outputMngr -> FillStandardTH2(key);
  } else {
    if(type>0){
      std::string str_vlqType = key + "_vlq";
      str_vlqType += std::to_string(type);
      m_outputMngr -> FillStandardTH1(str_vlqType);
      m_outputMngr -> FillStandardTH2(str_vlqType);
    }
    m_outputMngr -> FillStandardTH1(key);
    m_outputMngr -> FillStandardTH2(key);
  }
}

//________________________________________________________________________
//
void VLQ_AnalysisTools::BookAllTH1DHistogramsHistManager( /*const bool split,*/ const std::string &name, const std::string &title, double binsize, double xlow, double xup, const std::string &key, const std::string &xtitle, const std::string &ytitle,int lw, int lc){
  for ( unsigned int i = 0 ; i <= ( ( m_opt -> SampleName() == SampleName::VLQ  && m_opt -> SplitVLQDecays() /* && split */ ) ? 6 : 0 ) ; ++i) {
    std::string newname = name;
    if( i > 0 ){
      newname += "_vlq";
      newname += std::to_string(i);
    }
    m_outputMngr -> HistMngr() -> BookTH1D(newname,title,binsize,xlow,xup,key,xtitle,ytitle,lw,lc);
  }
}

//________________________________________________________________________
//
void VLQ_AnalysisTools::BookAllTH1DHistogramsHistManager( /*const bool split,*/ const std::string &name, const std::string &title, int nbins, double* xedges, const std::string &key, const std::string &xtitle, const std::string &ytitle,int lw, int lc){
  for ( unsigned int i = 0 ; i <= ( ( m_opt -> SampleName() == SampleName::VLQ  && m_opt -> SplitVLQDecays() /* && split*/ ) ? 6 : 0 ) ; ++i) {
    std::string newname = name;
    if( i > 0 ){
      newname += "_vlq";
      newname += std::to_string(i);
    }
    m_outputMngr -> HistMngr() -> BookTH1D(newname,title,nbins,xedges,key,xtitle,ytitle,lw,lc);
  }
}

//________________________________________________________________________
//
void VLQ_AnalysisTools::FillAllTH1DHistogramsHistManager( const std::string &key,double val, double wgt, const int type/*, const bool split*/ ){
  if( m_opt -> SampleName() != SampleName::VLQ /*|| !split*/ || !m_opt -> SplitVLQDecays() ){
    m_outputMngr -> HistMngr() -> FillTH1D(key,val,wgt);
  } else {
    if(type>0){
      std::string str_vlqType = key + "_vlq";
      str_vlqType += std::to_string(type);
      m_outputMngr -> HistMngr() -> FillTH1D(str_vlqType,val,wgt);
    }
    m_outputMngr -> HistMngr() -> FillTH1D(key,val,wgt);
  }
}


//____________________________________________________________________________________
//
bool VLQ_AnalysisTools::UpdateRegionDependentWeight( const std::string &region_name ){
  //m_weightMngr -> SetTtbarGeneratorSystematics(region_name);
  m_weightMngr -> ComputeAllWeights();
  if( m_opt->ReweightKinematics() && !m_opt->ComputeWeightSys() ){
    m_weightMngr -> ComputeSystematicWeights();
  }

  return true;
}

//____________________________________________________________________________________
//
bool VLQ_AnalysisTools::PassBTagRequirement( const int btag_req, const bool isIncl ){


  if( !m_opt -> DoTRF() || m_opt->IsData() ||
      (m_opt -> StrSampleName().find("QCD") != std::string::npos) ){
    int source_bjets_n = (m_opt->BtagCollection() == VLQ_Options::TRACK) ? m_outData -> o_trkbjets_n : m_outData -> o_bjets_n ;
    if(!isIncl){
      if( source_bjets_n != btag_req ) return false;
      return true;
    } else {
      if( source_bjets_n < btag_req ) return false;
      return true;
    }
  }//Direct tagging
  else {
    if( ( m_outData -> o_TRF_bjets_n == btag_req ) && ( m_outData -> o_TRF_isIncl == isIncl ) ){
      //the TRF weights are already computed in this configuration ... not need to spend time redoing it
      return true;
    }

    // Calling the UpdateBTagging functon from TRFManager allows to update the
    // TRFWeigths to the proper ones, clear and refill the m_outData->o_bjets
    // vector with the current permutation.
    m_trfMngr -> UpdateBTagging( isIncl, btag_req );
    // The compute BTagVariables function is designed to compute all the
    // variables needed by the user that depend on b-tagged jets, to make sure
    // the proper permutation is indeed used when calling PassBTagRequirement.
    this -> ComputeBTagVariables();

    this -> UpdateBTagMoments();
    // Updating the OutputData to contain the latest loaded b-tagging configuration
    m_outData -> o_TRF_bjets_n = btag_req;
    m_outData -> o_TRF_isIncl = isIncl;
  }//TRF

  return true;
}

//____________________________________________________________________________________
//
bool VLQ_AnalysisTools::ComputeAllVariables(){
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Entering in VLQ_AnalysisTools::ComputeAllVariables()" << std::endl;

  //
  // Filling the output variables (kinematics)
  //
  m_outData -> o_pileup_mu    = m_ntupData    -> d_mu;
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_pileup_mu (" << m_outData -> o_pileup_mu << ")"  << std::endl;
  m_outData -> o_meff         = m_varComputer -> GetMeff( *(m_outData->o_jets), *(m_outData->o_el), *(m_outData->o_mu), m_outData->o_AO_met );
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_meff (" << m_outData -> o_meff << ")"  << std::endl;
  m_outData -> o_meffred      = m_varComputer -> GetMeffRed( *(m_outData->o_jets), *(m_outData->o_el), *(m_outData->o_mu), m_outData->o_AO_met );
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_meffred (" << m_outData -> o_meffred << ")"  << std::endl;
  m_outData -> o_met          = m_outData     -> o_AO_met -> Pt();
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_met (" << m_outData -> o_met << ")"  << std::endl;
  m_outData -> o_mtwl         = m_varComputer -> GetMTw( *(m_outData->o_el), *(m_outData->o_mu), m_outData->o_AO_met );
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_mtwl (" << m_outData -> o_mtwl << ")"  << std::endl;
  m_outData -> o_ptwl         = m_varComputer -> GetPTw( *(m_outData->o_el), *(m_outData->o_mu), m_outData->o_AO_met );
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_ptwl (" << m_outData -> o_ptwl << ")"  << std::endl;

  AOVector v_lep;
  v_lep.insert( v_lep.end(), m_outData -> o_el->begin(), m_outData -> o_el->end());
  v_lep.insert( v_lep.end(), m_outData -> o_mu->begin(), m_outData -> o_mu->end());
  m_outData -> o_mll          = m_varComputer -> GetMjjLeadingJets( v_lep );
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_mll (" << m_outData -> o_mll << ")"  << std::endl;

  m_outData -> o_hthad        = m_varComputer -> GetHtHad( *(m_outData->o_jets) );
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_hthad (" << m_outData -> o_hthad << ")"  << std::endl;
  m_outData -> o_hthadRC      = m_varComputer -> GetHtHad( *(m_outData->o_rcjets) );
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_hthadRC (" << m_outData -> o_hthad << ")"  << std::endl;
  m_outData -> o_hthadRCtag   = m_varComputer -> GetHtHad( *(m_outData->o_rcjets), "isRCMTopHiggs" );
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_hthadRCtag (" << m_outData -> o_hthad << ")"  << std::endl;
  m_outData -> o_hthadRCM     = m_varComputer -> GetHtHad( *(m_outData->o_rcjets), "isRCTTMass" );
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_hthadRCM (" << m_outData -> o_hthad << ")"  << std::endl;
  m_outData -> o_mJsum        = m_varComputer -> GetMJSum( *(m_outData->o_rcjets), "isRCTTMass" );
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_mJsum (" << m_outData -> o_hthad << ")"  << std::endl;

  m_outData -> o_metsig_ev      = m_varComputer -> GetMetSignificance( m_outData->o_AO_met->Pt(), m_outData -> o_hthad );
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_metsig_ev (" << m_outData -> o_metsig_ev << ")"  << std::endl;
  m_outData -> o_metsig_obj      = m_ntupData -> d_met_sig;
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_metsig_obj (" << m_outData -> o_metsig_obj << ")"  << std::endl;
  if(m_opt->VerboseOutput()){
    m_outData -> o_dRmin_lepjet  = m_varComputer -> GetMindR( m_outData->o_selLep, *(m_outData->o_jets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dRmin_lepjet (" << m_outData -> o_dRmin_lepjet << ")"  << std::endl;
    m_outData -> o_dRmin_ejets  = m_varComputer -> GetMindR( *(m_outData->o_jets), *(m_outData->o_el) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dRmin_ejets (" << m_outData -> o_dRmin_ejets << ")"  << std::endl;
    m_outData -> o_dRmin_mujets = m_varComputer -> GetMindR( *(m_outData->o_jets), *(m_outData->o_mu) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dRmin_mujets (" << m_outData -> o_dRmin_mujets << ")"  << std::endl;
    m_outData -> o_dRmin_jetjet = m_varComputer -> GetMindR( *(m_outData->o_jets), *(m_outData->o_jets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dRmin_jetjet (" << m_outData -> o_dRmin_jetjet << ")"  << std::endl;
    m_outData -> o_dPhi_lepmet  = m_outData -> o_selLep ? TMath::Abs( m_outData -> o_selLep -> DeltaPhi( *(m_outData->o_AO_met) ) ) : -1;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dPhi_lepmet (" << m_outData -> o_dPhi_lepmet << ")"  << std::endl;
    m_outData -> o_dPhi_jetmet  = TMath::Abs(m_varComputer -> GetMindPhi( m_outData->o_AO_met, *(m_outData->o_jets), 4 ));
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dPhi_jetmet (" << m_outData -> o_dPhi_jetmet << ")"  << std::endl;
    m_outData -> o_dPhi_jetmet5  = TMath::Abs(m_varComputer -> GetMindPhi( m_outData->o_AO_met, *(m_outData->o_jets), 5 ));
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dPhi_jetmet (" << m_outData -> o_dPhi_jetmet5 << ")"  << std::endl;
    m_outData -> o_dPhi_jetmet6  = TMath::Abs(m_varComputer -> GetMindPhi( m_outData->o_AO_met, *(m_outData->o_jets), 6 ));
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dPhi_jetmet (" << m_outData -> o_dPhi_jetmet6 << ")"  << std::endl;
    m_outData -> o_dPhi_jetmet7  = TMath::Abs(m_varComputer -> GetMindPhi( m_outData->o_AO_met, *(m_outData->o_jets), 7 ));
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dPhi_jetmet (" << m_outData -> o_dPhi_jetmet7 << ")"  << std::endl;
    m_outData -> o_dPhi_lepjet  = TMath::Abs(m_varComputer -> GetMindPhi( m_outData->o_selLep, *(m_outData->o_jets) ) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dPhi_lepjet (" << m_outData -> o_dPhi_lepjet << ")"  << std::endl;
    m_outData -> o_jets40_n  = m_varComputer -> GetNjets( *(m_outData->o_jets), 40. );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_jets40_n (" << m_outData -> o_jets40_n << ")"  << std::endl;

    m_outData -> o_fwdjets30_n  = m_varComputer -> GetNjets( *(m_outData->o_fwdjets), 30. );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_fwdjets30_n (" << m_outData -> o_fwdjets30_n << ")"  << std::endl;
    m_outData -> o_fwdjets40_n  = m_varComputer -> GetNjets( *(m_outData->o_fwdjets), 40. );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_fwdjets40_n (" << m_outData -> o_fwdjets40_n << ")"  << std::endl;

    m_outData -> o_centrality = m_varComputer -> GetCentrality( *(m_outData->o_jets), *(m_outData->o_el), *(m_outData->o_mu) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_centrality (" << m_outData -> o_centrality << ")"  << std::endl;
    m_outData -> o_J_lepton_invariant_mass = m_varComputer -> GetJLeptonInvariantMass( *(m_outData->o_taggedjets.at("RCTTMass")), *(m_outData->o_el), *(m_outData->o_mu) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_J_lepton_invariant_mass (" << m_outData -> o_J_lepton_invariant_mass << ")"  << std::endl;
    m_outData -> o_J_J_invariant_mass = m_varComputer -> GetJJInvariantMass(*(m_outData->o_taggedjets.at("RCTTMass")));
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_J_J_invariant_mass  (" << m_outData -> o_J_J_invariant_mass << ")"  << std::endl;
    m_outData ->  o_dRaverage_jetjet = m_varComputer -> GetAveragedR( *(m_outData->o_jets), *(m_outData->o_jets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData ->  o_dRaverage_jetjet  (" << m_outData ->  o_dRaverage_jetjet << ")"  << std::endl;
    m_outData -> o_mjj_leading_jets = m_varComputer -> GetMjjLeadingJets( *(m_outData->o_jets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_mjj_leading_jets (" << m_outData -> o_mjj_leading_jets << ")" << std::endl;
    m_outData -> o_mjj_maxdR = m_varComputer -> GetMjjMaxDr( *(m_outData->o_jets) ) ;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_mjj_maxdR (" << m_outData -> o_mjj_maxdR << ")" << std::endl;
    m_outData -> o_mjj_mindR = m_varComputer -> GetMjjMinDr( *(m_outData->o_jets) ) ;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_mjj_mindR (" << m_outData -> o_mjj_mindR << ")" << std::endl;
    m_outData -> o_dPhijj_leading_jets = m_varComputer -> GetDphijjLeadingJets( *(m_outData->o_jets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_dPhijj_leading_jets (" << m_outData -> o_dPhijj_leading_jets << ")" << std::endl;
    m_outData -> o_dPhijj_mindR = m_varComputer -> GetDphijjMinDr( *(m_outData->o_jets) ) ;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_dPhijj_mindR (" << m_outData -> o_dPhijj_mindR << ")" << std::endl;
    m_outData -> o_dPhijj_maxdR = m_varComputer -> GetDphijjMaxDr( *(m_outData->o_jets) ) ;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_dPhijj_maxdR (" << m_outData -> o_dPhijj_maxdR << ")" << std::endl;
    m_outData -> o_dEtajj_leading_jets = m_varComputer -> GetDetajjLeadingJets( *(m_outData->o_jets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_dEtajj_leading_jets (" << m_outData -> o_dEtajj_leading_jets << ")" << std::endl;
    m_outData -> o_dEtajj_mindR = m_varComputer -> GetDetajjMinDr( *(m_outData->o_jets) ) ;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_dEtajj_mindR (" << m_outData -> o_dEtajj_mindR << ")" << std::endl;
    m_outData -> o_dEtajj_maxdR = m_varComputer -> GetDetajjMaxDr( *(m_outData->o_jets) ) ;
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_dEtajj_maxdR (" << m_outData -> o_dEtajj_maxdR << ")" << std::endl;
  }
  this -> ComputeBTagVariables();

  this -> UpdateBTagMoments();

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving VLQ_AnalysisTools::ComputeAllVariables()" << std::endl;

  return true;
}

//____________________________________________________________________________________
//g
bool VLQ_AnalysisTools::ComputeBTagVariables() {

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Entering in VLQ_AnalysisTools::ComputeBTagVariables()" << std::endl;

  AOVector* source_bjets = (m_opt->BtagCollection() == VLQ_Options::TRACK) ? m_outData -> o_trkbjets : m_outData -> o_bjets ;

  //
  // Filling only the b-tag sensitive kinematic variables
  //
  if(m_opt->VerboseOutput()){
    m_outData -> o_dRmin_lepbjet  = m_varComputer -> GetMindR( m_outData->o_selLep, *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dRmin_lepbjet (" << m_outData -> o_dRmin_lepbjet << ")"  << std::endl;
    m_outData -> o_mbb_mindR        = m_varComputer -> GetMbb( *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_mbb_mindR (" << m_outData -> o_mbb_mindR << ")"  << std::endl;
    m_outData -> o_dRmin_mubjets    = m_varComputer -> GetMindR( *(source_bjets), *(m_outData->o_mu) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dRmin_mubjets (" << m_outData -> o_dRmin_mubjets << ")"  << std::endl;
    m_outData -> o_dRmin_ebjets     = m_varComputer -> GetMindR( *(source_bjets), *(m_outData->o_el) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dRmin_ebjets (" << m_outData -> o_dRmin_ebjets << ")"  << std::endl;
    m_outData -> o_dRmin_bjetbjet   = m_varComputer -> GetMindR( *(source_bjets), *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dRmin_bjetbjet (" << m_outData -> o_dRmin_bjetbjet << ")"  << std::endl;
    m_outData -> o_dPhi_lepbjet     = TMath::Abs(m_varComputer -> GetMindPhi( m_outData->o_selLep, *(source_bjets) ) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dPhi_lepbjet (" << m_outData -> o_dPhi_lepbjet << ")"  << std::endl;
  }
  m_outData -> o_mTbmin           = m_varComputer -> GetMTbmin( *(source_bjets), m_outData -> o_AO_met );
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_mTbmin (" << m_outData -> o_mTbmin << ")"  << std::endl;
  if(m_opt->VerboseOutput()){
    m_outData -> o_mbb_leading_bjets= m_varComputer -> GetMbbLeadingBjets( *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_mbb_leading_bjets (" << m_outData -> o_mbb_leading_bjets << ")"  << std::endl;
    m_outData -> o_mbb_softest_bjets= m_varComputer -> GetMbbSoftestBjets( *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_mbb_softest_bjets (" << m_outData -> o_mbb_softest_bjets << ")"  << std::endl;
    m_outData -> o_J_leadingb_invariant_mass = m_varComputer -> GetJLeadingBInvariantMass( *(m_outData->o_taggedjets.at("RCTTMass")), *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_J_leadingb_invariant_mass (" << m_outData -> o_J_leadingb_invariant_mass << ")"  << std::endl;
    m_outData ->  o_dRaverage_bjetbjet = m_varComputer -> GetAveragedR( *(source_bjets), *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData ->  o_dRaverage_bjetbjet  (" << m_outData ->  o_dRaverage_bjetbjet << ")"  << std::endl;
    m_outData -> o_mbb_maxdR        =m_varComputer -> GetMbbMaxDr( *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData ->  o_mbb_maxdR (" << m_outData -> o_mbb_maxdR << ")" << std::endl;
    m_outData -> o_dPhibb_leading_bjets = m_varComputer -> GetDphibbLeadingBjets( *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_dPhibb_leading_bjets (" << m_outData -> o_dPhibb_leading_bjets << ")" << std::endl;
    m_outData -> o_dPhibb_mindR = m_varComputer -> GetDphibbMinDr( *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_dPhibb_mindR (" << m_outData -> o_dPhibb_mindR << ")" << std::endl;
    m_outData -> o_dPhibb_maxdR = m_varComputer -> GetDphibbMaxDr( *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_dPhibb_maxdR (" << m_outData ->o_dPhibb_maxdR << ")" << std::endl;
    m_outData -> o_dEtabb_leading_bjets = m_varComputer -> GetDetabbLeadingBjets( *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_dEtabb_leading_bjets (" << m_outData -> o_dEtabb_leading_bjets << ")" << std::endl;
    m_outData -> o_dEtabb_mindR = m_varComputer -> GetDetabbMinDr( *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_dEtabb_mindR (" << m_outData -> o_dEtabb_mindR << ")" << std::endl;
    m_outData -> o_dEtabb_maxdR = m_varComputer -> GetDetabbMaxDr( *(source_bjets) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    ->After m_outData -> o_dEtabb_maxdR (" << m_outData ->o_dEtabb_maxdR << ")" << std::endl;
  }

  if(m_opt->DoLowBRegions()){
    //Fill fake low-b variables with the fake low-b vectors
    m_outData -> o_dRmin_bjetbjet_lowb_3b   = m_varComputer -> GetMindR( *(m_outData->o_bjets_lowb_3b), *(m_outData->o_bjets_lowb_3b) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dRmin_bjetbjet_lowb_3b ("
    << m_outData -> o_dRmin_bjetbjet_lowb_3b << ")"  << std::endl;
    m_outData -> o_dRmin_bjetbjet_lowb_4b   = m_varComputer -> GetMindR( *(m_outData->o_bjets_lowb_4b), *(m_outData->o_bjets_lowb_4b) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_dRmin_bjetbjet_lowb_4b ("
    << m_outData -> o_dRmin_bjetbjet_lowb_4b << ")"  << std::endl;

    m_outData -> o_mTbmin_lowb_3b           = m_varComputer -> GetMTbmin( *(m_outData->o_bjets_lowb_3b), m_outData -> o_AO_met );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_mTbmin_lowb_3b ("
    << m_outData -> o_mTbmin_lowb_3b << ")"  << std::endl;
    m_outData -> o_mTbmin_lowb_4b           = m_varComputer -> GetMTbmin( *(m_outData->o_bjets_lowb_4b), m_outData -> o_AO_met );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_mTbmin_lowb_4b ("
    << m_outData -> o_mTbmin_lowb_4b << ")"  << std::endl;

    m_outData -> o_mbb_mindR_lowb_3b        = m_varComputer -> GetMbb( *(m_outData->o_bjets_lowb_3b) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_mbb_mindR_lowb_3b ("
    << m_outData -> o_mbb_mindR_lowb_3b << ")"  << std::endl;
    m_outData -> o_mbb_mindR_lowb_4b        = m_varComputer -> GetMbb( *(m_outData->o_bjets_lowb_4b) );
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "    -> After m_outData -> o_mbb_mindR_lowb_4b ("
    << m_outData -> o_mbb_mindR_lowb_4b << ")"  << std::endl;
  }//DoLowBRegions

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Exiting in VLQ_AnalysisTools::ComputeBTagVariables()" << std::endl;



  return true;
}

bool VLQ_AnalysisTools::UpdateBTagMoments(){

  if( m_outData -> o_selLep ){
    //m_resonance_maker -> MakeLeptop(VLQ_ResonanceMaker::VETO_RCMATCH & VLQ_ResonanceMaker::USE_LIGHT & VLQ_ResonanceMaker::PREF_BTAG);
    m_resonance_maker -> MakeLeptop(m_leptopRecoOpt, m_opt->MaxLeptopDR());
  }
  m_resonance_maker -> MakeSemiBoostedHadtop();
  if(m_opt -> VerboseOutput()){
    if( m_opt->DoRecoVLQ() == "single" ){
      m_resonance_maker->MakeSingleVLQ("Ht");
      m_resonance_maker->MakeSingleVLQ("Zt");
      m_resonance_maker->MakeSingleVLQ("Wb");
    }
    else if( m_opt->DoRecoVLQ() == "pair" ){
      m_resonance_maker->MakePairVLQ("Ht");
      m_resonance_maker->MakePairVLQ("Zt");
      m_resonance_maker->MakePairVLQ("Wb");
    }
  }
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Leptop-related variables in RC jets
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if( m_outData -> o_leptop && m_outData -> o_leptop->Pt() >= 300.){

    m_outData->o_leptop_n = 1.;

    for( std::pair<std::string, AOVector*> tagcol : m_outData->o_taggedjets ){

      double drmin_leptop = 99.;
      double drmin_leptop_b = 99.;

      for( AnalysisObject* tagjet : *(tagcol.second) ){

  double dr_leptop = (m_outData->o_leptop)->DeltaR(*tagjet);
  double dr_leptop_b = (m_outData->o_leptop_b)->DeltaR(*tagjet);

  if(dr_leptop < drmin_leptop) { drmin_leptop = dr_leptop; }
  if(dr_leptop_b < drmin_leptop_b) { drmin_leptop_b = dr_leptop_b; }



      }//Loop over tagged jet

      m_outData -> o_leptop ->SetMoment ( "dRmin_"+tagcol.first, drmin_leptop);
      m_outData -> o_leptop ->SetMoment ( "dRmin_b_"+tagcol.first, drmin_leptop_b);

    }//loop over tag collection

  } //only if there is a leptonic top

  //b-tagged contituents

  AOVector* source_bjets = (m_opt->BtagCollection() == VLQ_Options::TRACK) ? m_outData -> o_trkbjets : m_outData -> o_bjets ;

  for(AnalysisObject* obj : *(m_outData->o_rcjets)){

    int nb_match = 0;
    //Find number of b-tagged jets matched to this jet (//do this inside BTagVariables)
    for(AnalysisObject* sbjet : *(source_bjets )){
      if( obj->DeltaR(*sbjet) < 1.0 ) nb_match++;
    }
    obj -> UpdateMoment("nbconsts",   nb_match);


    if( m_outData -> o_leptop ){
      obj -> SetMoment( "dPhi_leptop", (m_outData->o_leptop)->DeltaPhi(*obj) );
      obj -> SetMoment( "dR_leptop", (m_outData->o_leptop)->DeltaR(*obj) );
      AnalysisObject recoVLQ(*obj);
      recoVLQ += *(m_outData->o_leptop);
      obj -> SetMoment( "minv_leptop", recoVLQ.M() );
    }
    /*
      TO BE REMOVED
    else{
      obj -> SetMoment( "dPhi_leptop", -10. );
      obj -> SetMoment( "dR_leptop", -10. );
      obj -> SetMoment( "minv_leptop", -1. );
    }
    */

  }// RC jet collection

  return true;

}
