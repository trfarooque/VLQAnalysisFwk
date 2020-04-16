#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "stdlib.h"
#include "TBits.h"
#include "TMath.h"
#include "TSystem.h"

#include "IFAETopFramework/OutputHistManager.h"
#include "IFAETopFramework/OutputTreeManager.h"
#include "IFAETopFramework/CommonConstants.h"
#include "IFAETopFramework/AnalysisObject.h"
#include "IFAETopFramework/AnalysisUtils.h"
#include "IFAETopFramework/TriggerInfo.h"
#include "IFAETopFramework/Selection.h"

#include "VLQAnalysis/VLQ_Options.h"
#include "VLQAnalysis/VLQ_NtupleData.h"
#include "VLQAnalysis/VLQ_NtupleReader.h"
#include "VLQAnalysis/VLQ_OutputData.h"
#include "VLQAnalysis/VLQ_AnalysisTools.h"
#include "VLQAnalysis/VLQ_VariableComputer.h"
#include "VLQAnalysis/VLQ_Enums.h"
#include "VLQAnalysis/VLQ_TruthManager.h"
#include "VLQAnalysis/VLQ_WeightManager.h"
#include "VLQAnalysis/VLQ_TRFManager.h"
#include "VLQAnalysis/VLQ_Selector.h"
#include "VLQAnalysis/string_utils.h"
#include "VLQAnalysis/VLQ_PropagateSingleTopSystematics.h"
#include "VLQAnalysis/VLQ_Analysis_Data2015.h"

#include <algorithm> // copy_if
#include <iterator> // back_inserter
#include <sstream> // ostringstream
#include <stdexcept> // invalid_argument
#include <utility> // pair

using std::cout;
using std::endl;

//____________________________________________________________________________
//
VLQ_Analysis_Data2015::VLQ_Analysis_Data2015( VLQ_Options* opt ):
m_opt(opt),
m_ntupData(0),
m_reader(0),
m_outData(0),
m_outMngrHist(0),
m_outMngrTree(0),
m_outMngrOvlerlapTree(0),
m_anaTools(0),
m_varComputer(0),
m_truthMngr(0),
m_TRFMngr(0),
m_weightMngr(0)
{
  m_channels.clear();
  m_topTaggers.clear();
  m_truthRCTypes.clear();
}

//____________________________________________________________________________
//
VLQ_Analysis_Data2015::~VLQ_Analysis_Data2015(){
}

//____________________________________________________________________________
//
bool VLQ_Analysis_Data2015::Begin(){

  //
  // The begin function allows the proper initialisation of all the data members of the
  // analysis class.
  //
  if(!m_opt){
    std::cerr << "<!> The VLQ_Options data member is not defined ..." << std::endl;
    return false;
  }
  m_opt->checkConcistency();

  //############################################################################
  //
  // Declaration of the NtupleReader object
  //
  //############################################################################
  m_reader = new VLQ_NtupleReader(m_opt);
  m_ntupData = m_reader -> Data();

  //############################################################################
  //
  // Declaration of the OutputData-derived object
  //
  //############################################################################
  m_outData = new VLQ_OutputData(m_opt);

  //############################################################################
  //
  // Defining the is ttbar boolean here
  //
  //############################################################################
  m_outData -> o_is_ttbar =  (m_opt -> SampleName() == SampleName::TTBAR);
  m_outData -> o_is_ttbar =  m_outData -> o_is_ttbar || (m_opt -> SampleName() == SampleName::TTBARBB);
  m_outData -> o_is_ttbar =  m_outData -> o_is_ttbar || (m_opt -> SampleName() == SampleName::TTBARCC);
  m_outData -> o_is_ttbar =  m_outData -> o_is_ttbar || (m_opt -> SampleName() == SampleName::TTBARLIGHT);

  //############################################################################
  //
  // Triggers
  //
  //############################################################################
  if( m_opt->UseMETTrigger() || m_opt->UseMETTriggerOneLep() ){
    //MET
    m_outData -> o_trigger_list.push_back(new TriggerInfo( "MET", VLQ_Enums::TRIGMET,(VLQ_Enums::DATA2015 | VLQ_Enums::DATA2016 | VLQ_Enums::DATA2017 | VLQ_Enums::DATA2018) ));
  }
  if( m_opt->UseLeptonTrigger() ){

    //ELEC
    m_outData -> o_trigger_list.push_back( new TriggerInfo("HLT_e24_lhmedium_L1EM20VH", VLQ_Enums::TRIGELEC, 
							   VLQ_Enums::DATA2015 | VLQ_Enums::DATA2016) );
    m_outData -> o_trigger_list.push_back( new TriggerInfo("HLT_e60_lhmedium", VLQ_Enums::TRIGELEC, VLQ_Enums::DATA2015) );
    m_outData -> o_trigger_list.push_back( new TriggerInfo("HLT_e120_lhloose", VLQ_Enums::TRIGELEC, VLQ_Enums::DATA2015) );
    m_outData -> o_trigger_list.push_back( new TriggerInfo("HLT_e26_lhtight_nod0_ivarloose", VLQ_Enums::TRIGELEC, 
							   (VLQ_Enums::DATA2016 | VLQ_Enums::DATA2017 | VLQ_Enums::DATA2018) ) );
    m_outData -> o_trigger_list.push_back( new TriggerInfo("HLT_e60_lhmedium_nod0", VLQ_Enums::TRIGELEC, 
							   (VLQ_Enums::DATA2016 | VLQ_Enums::DATA2017 | VLQ_Enums::DATA2018)) );
    m_outData -> o_trigger_list.push_back( new TriggerInfo("HLT_e140_lhloose_nod0", VLQ_Enums::TRIGELEC, 
							   (VLQ_Enums::DATA2016 | VLQ_Enums::DATA2017 | VLQ_Enums::DATA2018)) );
    m_outData -> o_trigger_list.push_back( new TriggerInfo("HLT_e300_etcut", VLQ_Enums::TRIGELEC, 
							   (VLQ_Enums::DATA2017 | VLQ_Enums::DATA2018)) );

    //MUON
    m_outData -> o_trigger_list.push_back( new TriggerInfo("HLT_mu20_iloose_L1MU15", VLQ_Enums::TRIGMUON, VLQ_Enums::DATA2015) );
    m_outData -> o_trigger_list.push_back( new TriggerInfo("HLT_mu26_ivarmedium", VLQ_Enums::TRIGMUON, 
							   (VLQ_Enums::DATA2016 | VLQ_Enums::DATA2017 | VLQ_Enums::DATA2018)) );
    m_outData -> o_trigger_list.push_back( new TriggerInfo("HLT_mu50", VLQ_Enums::TRIGMUON, 
							   (VLQ_Enums::DATA2015 | VLQ_Enums::DATA2016 | VLQ_Enums::DATA2017 | VLQ_Enums::DATA2018)) );
    m_outData -> o_trigger_list.push_back( new TriggerInfo("HLT_mu60_0eta105_msonly", VLQ_Enums::TRIGMUON, 
							   (VLQ_Enums::DATA2017 | VLQ_Enums::DATA2018)) );

  }
  //Print the triggers
  std::cout << "==================================================================================" << std::endl;
  std::cout << " Trigger List " << std::endl;
  std::cout << "=================================" << std::endl;
  std::cout << " Name    :   MET     :    ELEC   :      MUON     :    2015     :     2016   :     2017:     2018" << std::endl;
  for(TriggerInfo* trig : m_outData -> o_trigger_list){
    std::cout << trig->Name() << "  :  " << (trig->Type() == VLQ_Enums::TRIGMET) 
	      << "  :  " << (trig->Type() == VLQ_Enums::TRIGELEC) 
	      << "  :  " << (trig->Type() == VLQ_Enums::TRIGMUON)
	      << "  :  " << (trig->Period() & VLQ_Enums::DATA2015) 
	      << "  :  " << (trig->Period() & VLQ_Enums::DATA2016) 
	      << "  :  " << (trig->Period() & VLQ_Enums::DATA2017) 
	      << "  :  " << (trig->Period() & VLQ_Enums::DATA2018) << std::endl;
  }
  std::cout << "==================================================================================" << std::endl;
  //Adds the triggers in the NtupleReader to read the corresponding branches in
  //the input tree
  m_reader->SetTriggerList(m_outData->o_trigger_list);

  //############################################################################
  //
  // Declaration of the WeightManager, and reading in of nominal and systematic
  // weight components
  //
  //############################################################################
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Declaring WeightManager" << std::endl;
  m_weightMngr = new VLQ_WeightManager( m_opt, m_ntupData, m_outData );
  m_weightMngr -> AddVLQNominalWeights();
  if(m_opt -> ComputeWeightSys()){
    m_weightMngr -> AddVLQSystematicWeights();
  }
  if(m_opt -> ReweightKinematics()){
    m_weightMngr -> AddKinReweightings();
  }

  m_weightMngr -> Print( false );

  //############################################################################
  //
  // Declaration of the OutputManager
  //
  //############################################################################
  m_outMngrHist = new OutputHistManager( m_opt, m_outData, m_weightMngr->SystMap());

  //############################################################################
  //
  // Declaration of the TRFManager
  //
  //############################################################################
  m_TRFMngr = new VLQ_TRFManager( m_opt, m_weightMngr, m_ntupData, m_outData );
  m_TRFMngr -> Init();

  //############################################################################
  //
  // Declaration of the VariableComputer
  //
  //############################################################################
  m_varComputer = new VLQ_VariableComputer(m_opt);

  //############################################################################
  //
  // Declaration of the VLQ_AnalysisTools object
  //
  //############################################################################
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Declaring VLQ_AnalysisTools" << std::endl;
  m_anaTools = new VLQ_AnalysisTools( m_opt, m_outMngrHist, m_ntupData, m_outData, m_weightMngr, m_TRFMngr, m_varComputer );

  //############################################################################
  //
  // Define selector
  //
  //############################################################################
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Declaring VLQ_Selector" << std::endl;
  m_selector = new VLQ_Selector( m_opt, m_ntupData, m_outData, m_anaTools, false/*useDecisions*/, false/*addPrimaries*/ );

  //############################################################################
  //
  // Declares and Initialise the VLQ_NtupleReader
  //
  //############################################################################
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << " Initialise VLQ_NtupleReader with weight maps" << std::endl;
  int stat = m_reader -> Init( m_weightMngr -> NomMap(), m_weightMngr -> SystMap() );
  if(!stat){
    std::cerr<< "Failed NtupleReader::Init(). Exiting" << std::endl;
    abort();
  }

  //############################################################################
  //
  // Set all branches
  //
  //############################################################################
  if(m_opt -> MsgLevel() == Debug::DEBUG){
    std::cout << "Setting all branch addresses in Example_NtupleReader" << std::endl;
  }
  int i_stat = m_reader -> SetAllBranchAddresses();
  if(i_stat != 0){
    std::cerr << "Failed NtupleReader::SetAllBranchAddresses(). Exiting" << std::endl;
    abort();
  }
  if( m_opt -> DoTRF() && !(m_opt -> IsData() || (m_opt -> StrSampleName().find("QCD") != std::string::npos)) && !m_opt -> RecomputeTRF() ){
    i_stat = m_reader -> SetTRFBranchAddresses();
    if(i_stat != 0){
      std::cerr << "Failed NtupleReader::SetTRFBranchAddresses(). Exiting" << std::endl;
      abort();
    }
  }

  //############################################################################
  //
  // Declaration of the OutputTreeManager for the overlap tree
  //
  //############################################################################
  if(m_opt->DumpOverlapTree()){
    m_outMngrOvlerlapTree = new OutputTreeManager( m_opt, m_outData );
    m_outMngrOvlerlapTree->AddStandardBranch("bjets_n",       "bjets_n",      &(m_outData->o_bjets_n));
    m_outMngrOvlerlapTree->AddStandardBranch("run_number",    "run_number",   &(m_outData->o_run_number));
    m_outMngrOvlerlapTree->AddStandardBranch("event_number",  "event_number", &(m_outData->o_event_number));
    m_outMngrOvlerlapTree->AddStandardBranch("jets_n",        "jets_n",       &(m_outData->o_jets_n));
    m_outMngrOvlerlapTree->AddStandardBranch("fwdjets_n",     "fwdjets_n",    &(m_outData->o_fwdjets_n));
    m_outMngrOvlerlapTree->AddStandardBranch("lep_n",         "lep_n",        &(m_outData->o_lep_n));
    m_outMngrOvlerlapTree->AddStandardBranch("meff",          "meff",         &(m_outData->o_meff));
    m_outMngrOvlerlapTree->AddStandardBranch("meffred",       "meffred",      &(m_outData->o_meffred));
    m_outMngrOvlerlapTree->AddStandardBranch("met",           "met",          &(m_outData->o_met));
    m_outMngrOvlerlapTree->AddStandardBranch("mtw",           "mtw",          &(m_outData->o_mtwl));
    m_outMngrOvlerlapTree->AddStandardBranch("weight",        "weight",       &(m_outData->o_eventWeight_Nom));
    m_outMngrOvlerlapTree->AddStandardBranch("region",        "region",       &(m_outData->o_region));
    m_outMngrOvlerlapTree->BookStandardTree("overlap", "small tree");
  }


  //############################################################################
  //
  // Declaration of the OutputTreeManager for the small tree
  //
  //############################################################################
  if(m_opt->DumpTree()){
    m_outMngrTree = new OutputTreeManager( m_opt, m_outData );
    m_outMngrTree->AddAllWeightBranches("nomWeight", m_weightMngr, true);

    m_outMngrTree->AddStandardBranch("run_number", "Run Number",  &(m_outData->o_run_number));
    m_outMngrTree->AddStandardBranch("event_number", "Event Number",  &(m_outData->o_event_number));
    m_outMngrTree->AddStandardBranch("VLQtype", "VLQ decay type",  &(m_outData->o_VLQtype));

    //m_outMngrTree->AddStandardBranch("data_period", "Data Period",  &(m_outData->o_period));
    //m_outMngrTree->AddStandardBranch("pileup_mu", "<#mu>", &(m_outData->o_pileup_mu));
    //m_outMngrTree->AddStandardBranch("npv", "N_{PV}",  &(m_outData->o_npv));
    m_outMngrTree->AddStandardBranch("channel", "Channel Type",  &(m_outData->o_channel_type));

    m_outMngrTree->AddStandardBranch("jets_n", "Number of jets",  &(m_outData->o_jets_n));
    m_outMngrTree->AddStandardBranch("trkjets_n", "Number of track jets",  &(m_outData->o_trkjets_n));
    m_outMngrTree->AddStandardBranch("fwdjets_n", "Number of fwd-jets",  &(m_outData->o_fwdjets_n));
    m_outMngrTree->AddStandardBranch("bjets_n", "Number of b-jets",  &(m_outData->o_bjets_n));
    m_outMngrTree->AddStandardBranch("trkbjets_n", "Number of track b-jets",  &(m_outData->o_trkbjets_n));

    //m_outMngrTree->AddStandardBranch("fjets_n", "Number of fat jets",  &(m_outData->o_fjets_n));
    m_outMngrTree->AddStandardBranch("rcjets_n", "Number of re-clustered jets",  &(m_outData->o_rcjets_n));
    m_outMngrTree->AddStandardBranch("mtjets_n", "Number of mass-tagged jets",  &(m_outData->o_taggedjets_n.at("RCTTMass")));
    m_outMngrTree->AddStandardBranch("ttjets_n", "Number of top-tagged jets",  &(m_outData->o_taggedjets_n.at("RCMTop")));
    m_outMngrTree->AddStandardBranch("htjets_n", "Number of higgs-tagged jets",  &(m_outData->o_taggedjets_n.at("RCMHiggs")));
    m_outMngrTree->AddStandardBranch("vjets_n", "Number of V-tagged jets",  &(m_outData->o_taggedjets_n.at("RCMV")));

    m_outMngrTree->AddStandardBranch("jets_pt", "Jets  p_{T} [GeV]",  &(m_outData->o_jets), -1, "Pt");
    m_outMngrTree->AddStandardBranch("jets_m", "Jets Mass [GeV]",  &(m_outData->o_jets), -1, "M");
    m_outMngrTree->AddStandardBranch("jets_eta", "Jets #eta",  &(m_outData->o_jets), -1, "Eta");
    m_outMngrTree->AddStandardBranch("jets_phi", "Jets #phi",  &(m_outData->o_jets), -1, "Phi");
    m_outMngrTree->AddStandardBranch("jets_btagw", "Jets MV2c20 weight",  &(m_outData->o_jets), -1, "btagw");
    m_outMngrTree->AddStandardBranch("jets_isb", "Jets is b-tagged",  &(m_outData->o_jets), -1, "bjet");
    m_outMngrTree->AddStandardBranch("jets_jvt", "Jets JVT weight",  &(m_outData->o_jets), -1, "jvt");
    m_outMngrTree->AddStandardBranch("jets_truthlabel", "Jets truth flavour",  &(m_outData->o_jets), -1, "truthLabel");

    if(m_opt->BtagCollection() == VLQ_Options::TRACK){
      m_outMngrTree->AddStandardBranch("trkjets_pt", "Trkjets  p_{T} [GeV]",  &(m_outData->o_trkjets), -1, "Pt");
      m_outMngrTree->AddStandardBranch("trkjets_m", "Trkjets Mass [GeV]",  &(m_outData->o_trkjets), -1, "M");
      m_outMngrTree->AddStandardBranch("trkjets_eta", "Trkjets #eta",  &(m_outData->o_trkjets), -1, "Eta");
      m_outMngrTree->AddStandardBranch("trkjets_phi", "Trkjets #phi",  &(m_outData->o_trkjets), -1, "Phi");
      m_outMngrTree->AddStandardBranch("trkjets_btagw", "Trkjets MV2c20 weight",  &(m_outData->o_trkjets), -1, "btagw");
      m_outMngrTree->AddStandardBranch("trkjets_isb", "Trkjets is b-tagged",  &(m_outData->o_trkjets), -1, "bjet");
      m_outMngrTree->AddStandardBranch("trkjets_truthlabel", "Trkjets truth flavour",  &(m_outData->o_trkjets), -1, "truthLabel");
    }
    m_outMngrTree->AddStandardBranch("fwdjets_pt", "Fwd-jets  p_{T} [GeV]",  &(m_outData->o_fwdjets), -1, "Pt");
    m_outMngrTree->AddStandardBranch("fwdjets_m", "Fwd-jets Mass [GeV]",  &(m_outData->o_fwdjets), -1, "M");
    m_outMngrTree->AddStandardBranch("fwdjets_eta", "Fwd-jets #eta",  &(m_outData->o_fwdjets), -1, "Eta");
    m_outMngrTree->AddStandardBranch("fwdjets_phi", "Fwd-jets #phi",  &(m_outData->o_fwdjets), -1, "Phi");

    if(m_opt->DoTruthAnalysis()){
      m_outMngrTree->AddStandardBranch("jets_nmatch_btruth", "Jets no. of matched truth b-quarks from VLQ/ttbar decay",  &(m_outData->o_jets), -1, "nmatch_btruth");
      m_outMngrTree->AddStandardBranch("jets_nmatch_bpartons", "Jets no. of matched truth b-quarks",  &(m_outData->o_jets), -1, "nmatch_bpartons");
    }

    if( m_opt -> UseLargeRJets() ){

      //------------------------------------------------------------------------------------------------------------------
      m_outMngrTree->AddStandardBranch("fjets_pt", "Fat jets p_{T} [GeV]",  &(m_outData->o_fjets), -1, "Pt");
      m_outMngrTree->AddStandardBranch("fjets_m", "Fat jets Mass [GeV]",  &(m_outData->o_fjets), -1, "M");
      m_outMngrTree->AddStandardBranch("fjets_eta", "Fat jets #eta",  &(m_outData->o_fjets), -1, "Eta");
      m_outMngrTree->AddStandardBranch("fjets_phi", "Fat jets #phi",  &(m_outData->o_fjets), -1, "Phi");
      
    }

    //-----------------------------------------------------------------------------------------------------------------
    m_outMngrTree->AddStandardBranch("rcjets_pt", "RC jets p_{T} [GeV]",  &(m_outData->o_rcjets), -1, "Pt");
    m_outMngrTree->AddStandardBranch("rcjets_m", "RC jets Mass [GeV]",  &(m_outData->o_rcjets), -1, "M");
    //m_outMngrTree->AddStandardBranch("rcjets_eta", "RC jets #eta",  &(m_outData->o_rcjets), -1, "Eta");
    //m_outMngrTree->AddStandardBranch("rcjets_phi", "RC jets #phi",  &(m_outData->o_rcjets), -1, "Phi");
    //m_outMngrTree->AddStandardBranch("rcjets_nconsts", "RC jets N_{sj}",  &(m_outData->o_rcjets), -1, "nconsts");

    //-----------------------------------------------------------------------------------------------------------------
    m_outMngrTree->AddStandardBranch("el_n", "Number of electrons",  &(m_outData->o_el_n));
    m_outMngrTree->AddStandardBranch("mu_n", "Number of muons",  &(m_outData->o_mu_n));
    m_outMngrTree->AddStandardBranch("lep_n", "Number of leptons",  &(m_outData->o_lep_n));

    //-----------------------------------------------------------------------------------------------------------------
    //m_outMngrTree->AddStandardBranch("el_loose_n", "Number of loose electrons",  &(m_outData->o_el_loose_n));
    //m_outMngrTree->AddStandardBranch("mu_loose_n", "Number of loose muons",  &(m_outData->o_mu_loose_n));
    //m_outMngrTree->AddStandardBranch("lep_loose_n", "Number of loose leptons",  &(m_outData->o_lep_loose_n));

    //------------------------------------------------------------------------------------------------------------------
    m_outMngrTree->AddStandardBranch("el1_pt", "Electron 1 p_{T} [GeV]",  &(m_outData->o_el), 0, "Pt");
    //m_outMngrTree->AddStandardBranch("el1_eta", "Electron 1 #eta",  &(m_outData->o_el), 0, "Eta");
    //m_outMngrTree->AddStandardBranch("el1_phi", "Electron 1 #phi",  &(m_outData->o_el), 0, "Phi");
    //m_outMngrTree->AddStandardBranch("el1_isSignal", "Electron 1 isSignal",  &(m_outData->o_el), 0, "isSignal");

    //------------------------------------------------------------------------------------------------------------------
    m_outMngrTree->AddStandardBranch("mu1_pt", "Muon 1 p_{T} [GeV]",  &(m_outData->o_mu), 0, "Pt");
    //m_outMngrTree->AddStandardBranch("mu1_eta", "Muon 1 #eta",  &(m_outData->o_mu), 0, "Eta");
    //m_outMngrTree->AddStandardBranch("mu1_phi", "Muon 1 #phi",  &(m_outData->o_mu), 0, "Phi");
    //m_outMngrTree->AddStandardBranch("mu1_isSignal", "Muon 1 isSignal",  &(m_outData->o_mu), 0, "isSignal");

    //-------------------------------------------------------------------------------------------------------------------
    m_outMngrTree->AddStandardBranch("meff", "Effective mass", &(m_outData->o_meff));
    m_outMngrTree->AddStandardBranch("meffred", "Effective mass reduced", &(m_outData->o_meffred));
    m_outMngrTree->AddStandardBranch("met", "Missing E_{T}",  &(m_outData->o_AO_met), -1, "Pt");
    //m_outMngrTree->AddStandardBranch("met_phi", "#phi_{MET}",  &(m_outData->o_AO_met), -1, "Phi");
    m_outMngrTree->AddStandardBranch("mtw", "Transverse W mass", &(m_outData->o_mtwl));
    m_outMngrTree->AddStandardBranch("ptw", "Transverse W p_{T}", &(m_outData->o_ptwl));
    m_outMngrTree->AddStandardBranch("hthad", "H_{T}^{had}", &(m_outData->o_hthad));
    /*
    m_outMngrTree->AddStandardBranch("dRmin_ejets", "#DeltaR_{min}(e, jets)", &(m_outData->o_dRmin_ejets));
    m_outMngrTree->AddStandardBranch("dRmin_mujets", "#DeltaR_{min}(#mu, jets)", &(m_outData->o_dRmin_mujets));
    m_outMngrTree->AddStandardBranch("dRmin_jetjet", "#DeltaR_{min}(jet, jet)", &(m_outData->o_dRmin_jetjet));
    m_outMngrTree->AddStandardBranch("dRmin_bb", "#DeltaR_{min}(b-jet, b-jet)", &(m_outData->o_dRmin_bjetbjet));

    m_outMngrTree->AddStandardBranch("mbb_mindr", "m_{inv}^{min#DeltaR}", &(m_outData->o_mbb_mindR));
    m_outMngrTree->AddStandardBranch("dPhi_lepmet", "#Delta#phi(lep, MET)", &(m_outData->o_dPhi_lepmet));
    m_outMngrTree->AddStandardBranch("dPhi_jetmet", "#Delta#phi_{min}(jet, MET)", &(m_outData->o_dPhi_jetmet));
    m_outMngrTree->AddStandardBranch("dPhi_lepjet", "#Delta#phi_{min}(lep, jet)", &(m_outData->o_dPhi_lepjet));
    m_outMngrTree->AddStandardBranch("dPhi_lepbjet", "#Delta#phi_{min}(lep, b-jet)", &(m_outData->o_dPhi_lepbjet));

    m_outMngrTree->AddStandardBranch("dRmin_lepbjet", "#DeltaR_{min}(lep, b-jet)", &(m_outData->o_dRmin_lepbjet));
    m_outMngrTree->AddStandardBranch("dRmin_lepjet", "#DeltaR_{min}(lep, b-jet)", &(m_outData->o_dRmin_lepjet));
    //m_outMngrTree->AddStandardBranch("dRmin_ebjets", "#DeltaR_{min}(e, b-jets)", &(m_outData->o_dRmin_ebjets));
    //m_outMngrTree->AddStandardBranch("dRmin_mubjets", "#DeltaR_{min}(#mu, b-jets)", &(m_outData->o_dRmin_mubjets));
    m_outMngrTree->AddStandardBranch("mT_bmin", "m_{T}^{min}(b-jets, MET)", &(m_outData->o_mTbmin));

    m_outMngrTree->AddStandardBranch("jets40_n", "Number of jets with p_{T}>40 GeV", &(m_outData->o_jets40_n ));
    m_outMngrTree->AddStandardBranch("centrality", "Centrality", &(m_outData->o_centrality ));
    m_outMngrTree->AddStandardBranch("dRaverage_jetjet", "#DeltaR_{ave.}(jet, jet)", &(m_outData->o_dRaverage_jetjet));
    m_outMngrTree->AddStandardBranch("dRaverage_bb", "#DeltaR_{ave.}(b-jet, b-jet)", &(m_outData->o_dRaverage_bjetbjet));
    */

    if(m_opt->DoLowBRegions()){
      m_outMngrTree->AddStandardBranch("mbb_mindr_lowb_3b", "m_{inv}^{min#DeltaR} (lowb, 3b)", &(m_outData->o_mbb_mindR_lowb_3b));
      m_outMngrTree->AddStandardBranch("mbb_mindr_lowb_4b", "m_{inv}^{min#DeltaR} (lowb, 4b)", &(m_outData->o_mbb_mindR_lowb_4b));

      m_outMngrTree->AddStandardBranch("mT_bmin_lowb_3b", "m_{T}^{min}(b-jets, MET) (lowb, 3b)", &(m_outData->o_mTbmin_lowb_3b));
      m_outMngrTree->AddStandardBranch("mT_bmin_lowb_4b", "m_{T}^{min}(b-jets, MET) (lowb, 4b)", &(m_outData->o_mTbmin_lowb_4b));

      m_outMngrTree->AddStandardBranch("dRmin_bb_lowb_3b", "#DeltaR_{min}(b-jet, b-jet) (lowb, 3b)", &(m_outData->o_dRmin_bjetbjet_lowb_3b));
      m_outMngrTree->AddStandardBranch("dRmin_bb_lowb_4b", "#DeltaR_{min}(b-jet, b-jet) (lowb, 4b)", &(m_outData->o_dRmin_bjetbjet_lowb_4b));
    }

    if( m_opt->DoTruthAnalysis() ){ //&& (m_opt -> SampleName() == SampleName::VLQ) ){
      for ( const std::string truthType : {"VLQ", "VLQ_Ht", "VLQ_Zt", "VLQ_Wb",
      "VLQ_Hbdect", "VLQ_Wlepb", "VLQ_Whadb", "VLQ_Zhadt",
      "H", "Hbb", "hadtop", "leptop", 
      "hadZ", "lepZ", "invZ", "hadW", "lepW"} ){
  m_outMngrTree->AddStandardBranch("truth_"+truthType+"_n", "Number of truth " + truthType,  &(m_outData->o_truth_partons_n.at(truthType)));
      }
    }

    m_outMngrTree->BookStandardTree("tree", "small tree");

  }//DumpTree

  const bool otherVariables = m_opt -> MultipleVariables();
  const bool DrawTruth = (m_opt -> DoTruthAnalysis());
  const bool DrawReco = true; //!DrawTruth;

  //
  // Filling the top-tagging type vector
  //
  m_topTaggers.push_back("RCTTMass");
  m_topTaggers.push_back("RCMTop");
  m_topTaggers.push_back("RCMHiggs");
  m_topTaggers.push_back("RCMV");

  if(m_opt->VerboseOutput()){
    m_topTaggers.push_back("RCMTop0b");
    m_topTaggers.push_back("RCMTop1b");
    m_topTaggers.push_back("RCMTop2bin");
    m_topTaggers.push_back("RCMHiggs0b");
    m_topTaggers.push_back("RCMHiggs1b");
    m_topTaggers.push_back("RCMHiggs2bin");
    m_topTaggers.push_back("RCMV0b");
    m_topTaggers.push_back("RCMV1bin");
    m_topTaggers.push_back("RCMTopHiggs");
    m_topTaggers.push_back("LooseRCTTMass");
  }

  m_truthRCTypes.push_back("truthTop");
  m_truthRCTypes.push_back("truthW");
  m_truthRCTypes.push_back("truthZ");
  m_truthRCTypes.push_back("truthHiggs");
  m_truthRCTypes.push_back("truthOther");

  m_truthRCTypes.push_back("truthTop_inMassWindow");
  m_truthRCTypes.push_back("truthZ_inMassWindow");
  m_truthRCTypes.push_back("truthW_inMassWindow");
  m_truthRCTypes.push_back("truthHiggs_inMassWindow");

  if(m_opt->DumpHistos()){

    if(!m_opt->DoOneLeptonAna() && !m_opt->DoZeroLeptonAna() && !m_opt->DoTwoLeptonAna()) {
      throw std::invalid_argument("Must choose a lepton channel");
    }

    const int hopt_nouflow = HistManager::FCHECK | HistManager::OFLOW;

    if( DrawReco ){
      //Event variables
      m_outMngrHist -> AddStandardTH1( "mu",          1, 0, 80,       ";<#mu>",         false, &(m_outData -> o_pileup_mu) );
      m_outMngrHist -> AddStandardTH1( "meff",        50, 0, 5000,    ";m_{eff} [GeV]", true, &(m_outData -> o_meff) );
      m_outMngrHist -> AddStandardTH1( "meffred",        50, 0, 5000,    ";m_{eff} reduced [GeV]", true, &(m_outData -> o_meffred) );
      m_outMngrHist -> AddStandardTH1( "mJsum",       25, 0, 2000,    ";m_{J}^{#Sigma} [GeV]", otherVariables, &(m_outData -> o_mJsum) );
      m_outMngrHist -> AddStandardTH1( "met",         20, 0, 1000,    ";E_{T}^{miss} [GeV]",otherVariables, &(m_outData -> o_met) );
      m_outMngrHist -> AddStandardTH1( "met_phi",     0.2, -3.5, 3.5, ";#phi_{MET}", false, &(m_outData->o_AO_met), -1, "Phi");
      m_outMngrHist -> AddStandardTH1( "mtw",         25, 0, 1000,    ";m_{T}(W) [GeV]", otherVariables, &(m_outData -> o_mtwl) );
      m_outMngrHist -> AddStandardTH1( "ptw",         25, 0, 1000,    ";p_{T}(W) [GeV]", false, &(m_outData -> o_ptwl) );
      m_outMngrHist -> AddStandardTH1( "mll",         5, 0, 500,    ";m_{ll} [GeV]", false, &(m_outData -> o_mll) );
      m_outMngrHist -> AddStandardTH1( "hthad",       25, 0, 3000,    ";H_{T}^{had} [GeV]", false, &(m_outData -> o_hthad) );
      m_outMngrHist -> AddStandardTH1( "hthadRC",     50, 0, 5000,    ";H_{T}^{had} (RC jets) [GeV]", false, &(m_outData -> o_hthadRC) );
      m_outMngrHist -> AddStandardTH1( "hthadRCtag",     50, 0, 5000,  ";H_{T}^{had} (tagged RC jets) [GeV]", false, &(m_outData -> o_hthadRCtag) );
      m_outMngrHist -> AddStandardTH1( "hthadRCM",     50, 0, 5000,  ";H_{T}^{had} (RC jets w/ M>100 GeV) [GeV]", false, &(m_outData -> o_hthadRCM) );

      m_outMngrHist -> AddStandardTH1( "truth_ht_filter",    25, 0, 3000,    ";Truth H_{T} [GeV]", false, &(m_outData -> o_truth_ht_filter ) );
      m_outMngrHist -> AddStandardTH1( "truth_met_filter",   25, 0, 1000,    ";Truth MET [GeV]", false, &(m_outData -> o_truth_met_filter ) );
      m_outMngrHist -> AddStandardTH1( "mtbmin",      25, 0, 500,    ";m_{T}^{min}(b,MET)", false, &(m_outData->o_mTbmin) );
      m_outMngrHist -> AddStandardTH1( "metsig_ev",     0.5, 0, 50,    ";E_{T}^{miss}/#sqrt{H_{T}^{had}} [#sqrt{GeV}]", false, &(m_outData -> o_metsig_ev) );
      m_outMngrHist -> AddStandardTH1( "metsig_obj",    0.5, 0, 50,    "; #sigma(E_{T}^{miss}) [#sqrt{GeV}]", false, &(m_outData -> o_metsig_obj) );
      
      m_outMngrHist -> AddStandardTH2( "meff", "jets_n", 50, 0, 5000, 1, -0.5, 15.5, ";Number of jets", ";m_{eff} [GeV]", true, &(m_outData -> o_meff), &(m_outData -> o_jets_n));
      m_outMngrHist -> AddStandardTH2( "meffred", "jets_n", 50, 0, 5000, 1, -0.5, 15.5, ";Number of jets", ";m_{eff} reduced [GeV]", true, &(m_outData -> o_meffred), &(m_outData -> o_jets_n));

      /*
      m_outMngrHist -> AddStandardTH2( "mu", "fwdjets_n", 10, 0, 80, 1, -0.5, 8.5,"<#mu>", "Number of fwd-jets", false, 
               &(m_outData -> o_pileup_mu),&(m_outData -> o_fwdjets_n) );
      m_outMngrHist -> AddStandardTH2( "mu", "fwdjets30_n", 10, 0, 80, 1, -0.5, 8.5,"<#mu>", "Number of fwd-jets (p_{T}>30 GeV)", false, 
               &(m_outData -> o_pileup_mu),&(m_outData -> o_fwdjets30_n) );
      m_outMngrHist -> AddStandardTH2( "mu", "fwdjets40_n", 10, 0, 80, 1, -0.5, 8.5,"<#mu>", "Number of fwd-jets (p_{T}>40 GeV)", false, 
               &(m_outData -> o_pileup_mu),&(m_outData -> o_fwdjets40_n) );

      m_outMngrHist -> AddStandardTH2( "meff", "mtbmin", 25, 0, 3000, 25, 0., 500,"m_{eff} [GeV]", "m_{T}^{min}(b,MET)", false, 
                   &(m_outData -> o_meff),&(m_outData -> o_mTbmin) );
      */
      //Leptonic top
      m_outMngrHist -> AddStandardTH1( "leptop_n",         1, -0.5, 1.5, ";Number of leptonic tops"      ,  false, &(m_outData -> o_leptop_n) );
      m_outMngrHist -> AddStandardTH1( "leptop_pt",         50, 0, 1000, ";Leptonic top p_{T} [GeV]"      ,  false, &(m_outData -> o_leptop), -1, "Pt", hopt_nouflow);
      m_outMngrHist -> AddStandardTH1( "leptop_eta",        0.2, -3, 3,  ";Leptonic top #eta"             ,  false, &(m_outData -> o_leptop), -1, "Eta", hopt_nouflow);
      m_outMngrHist -> AddStandardTH1( "leptop_m",          10, 0, 500,  ";Leptonic top mass [GeV]"       ,  false, &(m_outData -> o_leptop), -1, "M", hopt_nouflow);
      m_outMngrHist -> AddStandardTH1( "leptop_dR_blep",    0.1, 0, 6,   ";Leptonic top #DeltaR(b,lep)"       ,  false, &(m_outData -> o_leptop), -1, "dR_blep", hopt_nouflow);
      m_outMngrHist -> AddStandardTH1( "leptop_dR_bW",      0.1, 0, 6,   ";Leptonic top #DeltaR(b,W)"       ,  false, &(m_outData -> o_leptop), -1, "dR_bW", hopt_nouflow);
      m_outMngrHist -> AddStandardTH1( "leptop_dPhi_bnu",   0.1, 0, 4,   ";Leptonic top #Delta#phi(b,#nu)"       ,  false, &(m_outData -> o_leptop), -1, "dPhi_bnu", hopt_nouflow);

      //Leptonic W
      m_outMngrHist -> AddStandardTH1( "lepW_pt",         50, 0, 1000, ";Leptonic W p_{T} [GeV]"      ,  false, &(m_outData -> o_lepW), -1, "Pt", hopt_nouflow);
      m_outMngrHist -> AddStandardTH1( "lepW_eta",        0.2, -3, 3,  ";Leptonic W #eta"             ,  false, &(m_outData -> o_lepW), -1, "Eta", hopt_nouflow);
      m_outMngrHist -> AddStandardTH1( "lepW_m",          10, 0, 500,  ";Leptonic W mass [GeV]"       ,  false, &(m_outData -> o_lepW), -1, "M", hopt_nouflow);

      //Reconstructed neutrino
      m_outMngrHist -> AddStandardTH1( "nu_pt",         50, 0, 1000, ";Neutrino p_{T} [GeV]"      ,  false, &(m_outData -> o_nu), -1, "Pt", hopt_nouflow);
      m_outMngrHist -> AddStandardTH1( "nu_eta",        0.2, -3, 3,  ";Neutrino #eta"             ,  false, &(m_outData -> o_nu), -1, "Eta", hopt_nouflow);
      m_outMngrHist -> AddStandardTH1( "nu_m",          10, 0, 500,  ";Neutrino mass [GeV]"       ,  false, &(m_outData -> o_nu), -1, "M", hopt_nouflow);
      m_outMngrHist -> AddStandardTH1( "nu_fracMET",    0.01, 0, 1,  ";Neutrino MET fraction"     ,  false, &(m_outData -> o_nu), -1, "fracMET", hopt_nouflow);
      m_outMngrHist -> AddStandardTH1( "nu_dphi_MET",   0.1, 0, 4,   ";#Delta#phi(Neutrino, MET)" ,  false, &(m_outData -> o_nu), -1, "dphi_MET", hopt_nouflow);
 

      //Semi-boosted hadronic top
      for ( int iJet=-1; iJet<=2; ++iJet ) {
	std::string str_id = "";
	str_id += std::to_string(iJet);
	if(iJet==-1) str_id = "s";
	const bool DrawSyst = (iJet <= 0) && otherVariables;
	m_outMngrHist -> AddStandardTH1( "bW_hadtop"+str_id+"_pt",    25, 0, 500,     ";bW Hadtop"+str_id+" p_{T} [GeV]",   DrawSyst,   &(m_outData -> o_bW_hadtop), iJet, "Pt");
	m_outMngrHist -> AddStandardTH1( "bW_hadtop"+str_id+"_eta",   0.2, -3, 3,     ";bW Hadtop"+str_id+" #eta",          DrawSyst,   &(m_outData -> o_bW_hadtop), iJet, "Eta");
	m_outMngrHist -> AddStandardTH1( "bW_hadtop"+str_id+"_m",     10, 100, 300,     ";bW Hadtop"+str_id+" mass [GeV]",    false,      &(m_outData -> o_bW_hadtop), iJet, "M");
	m_outMngrHist -> AddStandardTH1( "bW_hadtop"+str_id+"_dR_bW", 0.1,0,6,   ";bW Hadtop"+str_id+" MV2c10",        false,      &(m_outData -> o_bW_hadtop), iJet, "dR_bW");
      }
      
      //Jet variables
      m_outMngrHist -> AddStandardTH1( "jets_n",      1, -2.5, 15.5,";Number of jets", true, &(m_outData -> o_jets_n) );
      m_outMngrHist -> AddStandardTH1( "trkjets_n",      1, -2.5, 15.5,";Number of track-jets", otherVariables, &(m_outData -> o_trkjets_n) );
      m_outMngrHist -> AddStandardTH1( "fwdjets_n",   1, -0.5, 8.5,";Number of fwd-jets", otherVariables, &(m_outData -> o_fwdjets_n) );
      m_outMngrHist -> AddStandardTH1( "fwdjets_eta25_30_n",   1, -0.5, 8.5,";Number of fwd-jets 2.5<|#eta|<3.0", otherVariables, 
				       &(m_outData -> o_fwdjets_eta25_30_n) );
      m_outMngrHist -> AddStandardTH1( "fwdjets_eta30_35_n",   1, -0.5, 8.5,";Number of fwd-jets 3.0<|#eta|<3.5", otherVariables, 
				       &(m_outData -> o_fwdjets_eta30_35_n) );
      m_outMngrHist -> AddStandardTH1( "fwdjets_eta35_40_n",   1, -0.5, 8.5,";Number of fwd-jets 3.5<|#eta|<4.0", otherVariables, 
				       &(m_outData -> o_fwdjets_eta35_40_n) );
      m_outMngrHist -> AddStandardTH1( "fwdjets_eta40_45_n",   1, -0.5, 8.5,";Number of fwd-jets 4.0<|#eta|<4.5", otherVariables, 
				       &(m_outData -> o_fwdjets_eta40_45_n) );

      m_outMngrHist -> AddStandardTH1( "bjets_n",     1, -0.5, 8.5, ";Number of b-jets", otherVariables, &(m_outData -> o_bjets_n) );
      m_outMngrHist -> AddStandardTH1( "ljets_n",     1, -0.5, 8.5, ";Number of light-jets", otherVariables, &(m_outData -> o_ljets_n) );
      m_outMngrHist -> AddStandardTH1( "trkbjets_n",     1, -0.5, 8.5, ";Number of track b-jets", otherVariables, &(m_outData -> o_trkbjets_n) );
      m_outMngrHist -> AddStandardTH1( "trkljets_n",     1, -0.5, 8.5, ";Number of track light-jets", otherVariables, &(m_outData -> o_trkljets_n) );
      m_outMngrHist -> AddStandardTH1( "trkjets_nconcentric",      1, -2.5, 15.5,";Number of concentric track-jets", 
				       otherVariables, &(m_outData -> o_trkjets_nconcentric) );
      if(m_opt->VerboseOutput()){
	m_outMngrHist -> AddStandardTH1( "jets_n_truth_b",  1, -2.5, 15.5,";Number of truth b-jets", 
					 otherVariables, &(m_outData -> o_jets_truth_b_n) );
	m_outMngrHist -> AddStandardTH1( "jets_n_truth_c",  1, -2.5, 15.5,";Number of truth c-jets", 
					 otherVariables, &(m_outData -> o_jets_truth_c_n) );
	m_outMngrHist -> AddStandardTH1( "jets_n_truth_tau", 1, -2.5, 15.5,";Number of truth #tau-jets", 
					 otherVariables, &(m_outData -> o_jets_truth_tau_n) );
	m_outMngrHist -> AddStandardTH1( "jets_n_truth_lqg", 1, -2.5, 15.5,";Number of truth light-jets", 
					 otherVariables, &(m_outData -> o_jets_truth_lqg_n) );

	m_outMngrHist -> AddStandardTH1( "bjets_n_truth_b",  1, -2.5, 15.5,";Number of b-tagged truth b-jets", 
					 otherVariables, &(m_outData -> o_bjets_truth_b_n) );
	m_outMngrHist -> AddStandardTH1( "bjets_n_truth_c",  1, -2.5, 15.5,";Number of b-tagged truth c-jets", 
					 otherVariables, &(m_outData -> o_bjets_truth_c_n) );
	m_outMngrHist -> AddStandardTH1( "bjets_n_truth_tau", 1, -2.5, 15.5,";Number of b-tagged truth #tau-jets", 
					 otherVariables, &(m_outData -> o_bjets_truth_tau_n) );
	m_outMngrHist -> AddStandardTH1( "bjets_n_truth_lqg", 1, -2.5, 15.5,";Number of b-tagged truth light-jets", 
					 otherVariables, &(m_outData -> o_bjets_truth_lqg_n) );

	m_outMngrHist -> AddStandardTH1( "trkjets_n_truth_b",  1, -2.5, 15.5,";Number of truth b-trkjets", 
					 otherVariables, &(m_outData -> o_trkjets_truth_b_n) );
	m_outMngrHist -> AddStandardTH1( "trkjets_n_truth_c",  1, -2.5, 15.5,";Number of truth c-trkjets", 
					 otherVariables, &(m_outData -> o_trkjets_truth_c_n) );
	m_outMngrHist -> AddStandardTH1( "trkjets_n_truth_tau", 1, -2.5, 15.5,";Number of truth #tau-trkjets", 
					 otherVariables, &(m_outData -> o_trkjets_truth_tau_n) );
	m_outMngrHist -> AddStandardTH1( "trkjets_n_truth_lqg", 1, -2.5, 15.5,";Number of truth light-trkjets", 
					 otherVariables, &(m_outData -> o_trkjets_truth_lqg_n) );

	m_outMngrHist -> AddStandardTH1( "trkbjets_n_truth_b",  1, -2.5, 15.5,";Number of b-tagged truth b-trkjets", 
					 otherVariables, &(m_outData -> o_trkbjets_truth_b_n) );
	m_outMngrHist -> AddStandardTH1( "trkbjets_n_truth_c",  1, -2.5, 15.5,";Number of b-tagged truth c-trkjets", 
					 otherVariables, &(m_outData -> o_trkbjets_truth_c_n) );
	m_outMngrHist -> AddStandardTH1( "trkbjets_n_truth_tau", 1, -2.5, 15.5,";Number of b-tagged truth #tau-trkjets", 
					 otherVariables, &(m_outData -> o_trkbjets_truth_tau_n) );
	m_outMngrHist -> AddStandardTH1( "trkbjets_n_truth_lqg", 1, -2.5, 15.5,";Number of b-tagged truth light-trkjets", 
					 otherVariables, &(m_outData -> o_trkbjets_truth_lqg_n) );
      }
      //================ CALO JETS ======================================
      for ( int iJet=-1; iJet<=5; ++iJet ) {
	std::string str_id = "";
	str_id += std::to_string(iJet);
	if(iJet==-1) str_id = "s";
	const bool DrawSyst = (iJet <= 0) && otherVariables;
	m_outMngrHist -> AddStandardTH1( "jet"+str_id+"_pt",    10, 0, 1000,     ";Jet"+str_id+" p_{T} [GeV]",  DrawSyst,   
					 &(m_outData -> o_jets), iJet, "Pt");
	m_outMngrHist -> AddStandardTH1( "jet"+str_id+"_eta",   0.2, -3, 3,     ";Jet"+str_id+" #eta",          DrawSyst,   
					 &(m_outData -> o_jets), iJet, "Eta");
	m_outMngrHist -> AddStandardTH1( "jet"+str_id+"_phi",   0.2, -3.5, 3.5, ";Jet"+str_id+" #varphi",       false,      
					 &(m_outData -> o_jets), iJet, "Phi");
	m_outMngrHist -> AddStandardTH1( "jet"+str_id+"_m",     10, 0, 200,     ";Jet"+str_id+" mass [GeV]",    false,      
					 &(m_outData -> o_jets), iJet, "M");
	m_outMngrHist -> AddStandardTH1( "jet"+str_id+"_btagw", 0.1,-1.1,1.1,   ";Jet"+str_id+" MV2c10",        false,      
					 &(m_outData -> o_jets), iJet, "btagw");
	m_outMngrHist -> AddStandardTH1( "jet"+str_id+"_jvt",   0.1,-1.1,1.1,   ";Jet"+str_id+" JVT",           false,      
					 &(m_outData -> o_jets), iJet, "jvt");
	//for MC
	if(m_opt->VerboseOutput() && !m_opt->IsData() && (iJet <= 0) ){
	  m_outMngrHist -> AddStandardTH1( "jet_truth_b"+str_id+"_pt",    10, 0, 1000,     ";Truth B-Jet"+str_id+" p_{T} [GeV]",  DrawSyst,   
					   &(m_outData -> o_jets_truth_b), iJet, "Pt");
	  m_outMngrHist -> AddStandardTH1( "jet_truth_c"+str_id+"_pt",    10, 0, 1000,     ";Truth C-Jet"+str_id+" p_{T} [GeV]",  DrawSyst,   
					   &(m_outData -> o_jets_truth_c), iJet, "Pt");
	  m_outMngrHist -> AddStandardTH1( "jet_truth_tau"+str_id+"_pt",    10, 0, 1000,     ";Truth #tau-Jet"+str_id+" p_{T} [GeV]",  DrawSyst,   
					   &(m_outData -> o_jets_truth_tau), iJet, "Pt");
	  m_outMngrHist -> AddStandardTH1( "jet_truth_lqg"+str_id+"_pt",    10, 0, 1000,     ";Truth Light-Jet"+str_id+" p_{T} [GeV]",  DrawSyst,   
					   &(m_outData -> o_jets_truth_lqg), iJet, "Pt");
	  
	}
	
      }
      for ( int iFwdJet=-1; iFwdJet<=5; ++iFwdJet ) {
	std::string str_id = "";
	str_id += std::to_string(iFwdJet);
	if(iFwdJet==-1) str_id = "s";
	const bool DrawSyst = (iFwdJet <= 0) && otherVariables;
	m_outMngrHist -> AddStandardTH1( "fwdjet"+str_id+"_pt",    5, 0, 500,      ";Fwd-jet"+str_id+" p_{T} [GeV]",   
					 DrawSyst,   &(m_outData -> o_fwdjets), iFwdJet, "Pt");
	m_outMngrHist -> AddStandardTH1( "fwdjet"+str_id+"_eta",   0.2, -5, 5,     ";Fwd-jet"+str_id+" #eta",          
					 DrawSyst,   &(m_outData -> o_fwdjets), iFwdJet, "Eta");
	m_outMngrHist -> AddStandardTH1( "fwdjet"+str_id+"_phi",   0.2, -3.5, 3.5, ";Fwd-jet"+str_id+" #varphi",       
					 false,      &(m_outData -> o_fwdjets), iFwdJet, "Phi");
	m_outMngrHist -> AddStandardTH1( "fwdjet"+str_id+"_m",     4, 0, 100,       ";Fwd-jet"+str_id+" mass [GeV]",    
					 false,      &(m_outData -> o_fwdjets), iFwdJet, "M");
	if(m_opt->VerboseOutput()){
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta25_30"+str_id+"_pt",    5, 0, 500,      ";Fwd-jet"+str_id+" 2.5<|#eta|<3.0 p_{T} [GeV]",   
					   DrawSyst,   &(m_outData -> o_fwdjets_eta25_30), iFwdJet, "Pt");
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta25_30"+str_id+"_eta",   0.2, -5, 5,     ";Fwd-jet"+str_id+" 2.5<|#eta|<3.0 #eta",          
					   DrawSyst,   &(m_outData -> o_fwdjets_eta25_30), iFwdJet, "Eta");
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta25_30"+str_id+"_phi",   0.2, -3.5, 3.5, ";Fwd-jet"+str_id+" 2.5<|#eta|<3.0 #varphi",       
					   false,      &(m_outData -> o_fwdjets_eta25_30), iFwdJet, "Phi");
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta25_30"+str_id+"_m",     4, 0, 100,       ";Fwd-jet"+str_id+" 2.5<|#eta|<3.0 mass [GeV]",    
					   false,      &(m_outData -> o_fwdjets_eta25_30), iFwdJet, "M");
	
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta30_35"+str_id+"_pt",    5, 0, 500,      ";Fwd-jet"+str_id+" 3.0<|#eta|<3.5 p_{T} [GeV]",   
					   DrawSyst,   &(m_outData -> o_fwdjets_eta30_35), iFwdJet, "Pt");
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta30_35"+str_id+"_eta",   0.2, -5, 5,     ";Fwd-jet"+str_id+" 3.0<|#eta|<3.5 #eta",          
					   DrawSyst,   &(m_outData -> o_fwdjets_eta30_35), iFwdJet, "Eta");
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta30_35"+str_id+"_phi",   0.2, -3.5, 3.5, ";Fwd-jet"+str_id+" 3.0<|#eta|<3.5 #varphi",       
					   false,      &(m_outData -> o_fwdjets_eta30_35), iFwdJet, "Phi");
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta30_35"+str_id+"_m",     4, 0, 100,       ";Fwd-jet"+str_id+" 3.0<|#eta|<3.5 mass [GeV]",    
					   false,      &(m_outData -> o_fwdjets_eta30_35), iFwdJet, "M");
	
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta35_40"+str_id+"_pt",    5, 0, 500,      ";Fwd-jet"+str_id+" 3.5<|#eta|<4.0 p_{T} [GeV]",   
					   DrawSyst,   &(m_outData -> o_fwdjets_eta35_40), iFwdJet, "Pt");
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta35_40"+str_id+"_eta",   0.2, -5, 5,     ";Fwd-jet"+str_id+" 3.5<|#eta|<4.0 #eta",          
					   DrawSyst,   &(m_outData -> o_fwdjets_eta35_40), iFwdJet, "Eta");
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta35_40"+str_id+"_phi",   0.2, -3.5, 3.5, ";Fwd-jet"+str_id+" 3.5<|#eta|<4.0 #varphi",       
					   false,      &(m_outData -> o_fwdjets_eta35_40), iFwdJet, "Phi");
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta35_40"+str_id+"_m",     4, 0, 100,       ";Fwd-jet"+str_id+" 3.5<|#eta|<4.0 mass [GeV]",    
					   false,      &(m_outData -> o_fwdjets_eta35_40), iFwdJet, "M");
	
	
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta40_45"+str_id+"_pt",    5, 0, 500,      ";Fwd-jet"+str_id+" 4.0<|#eta|<4.5 p_{T} [GeV]",   
					   DrawSyst,   &(m_outData -> o_fwdjets_eta40_45), iFwdJet, "Pt");
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta40_45"+str_id+"_eta",   0.2, -5, 5,     ";Fwd-jet"+str_id+" 4.0<|#eta|<4.5 #eta",          
					   DrawSyst,   &(m_outData -> o_fwdjets_eta40_45), iFwdJet, "Eta");
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta40_45"+str_id+"_phi",   0.2, -3.5, 3.5, ";Fwd-jet"+str_id+" 4.0<|#eta|<4.5 #varphi",       
					   false,      &(m_outData -> o_fwdjets_eta40_45), iFwdJet, "Phi");
	  m_outMngrHist -> AddStandardTH1( "fwdjet_eta40_45"+str_id+"_m",     4, 0, 100,       ";Fwd-jet"+str_id+" 4.0<|#eta|<4.5 mass [GeV]",    
					   false,      &(m_outData -> o_fwdjets_eta40_45), iFwdJet, "M");
	}
	
      }
      
      for ( int iBJet=-1; iBJet<=3; ++iBJet ) {
	std::string str_id = "";
	str_id += std::to_string(iBJet);
	if(iBJet==-1) str_id = "s";
	const bool DrawSyst = (iBJet <= 0) && otherVariables;
	m_outMngrHist -> AddStandardTH1( "bjet"+str_id+"_pt",    10, 0, 1000,    ";b-jet"+str_id+" p_{T} [GeV]",   
					 false, &(m_outData -> o_bjets),  iBJet, "Pt" );
	m_outMngrHist -> AddStandardTH1( "bjet"+str_id+"_eta",   0.2, -3, 3,    ";b-jet"+str_id+" #eta",           
					 false, &(m_outData -> o_bjets),  iBJet, "Eta" );
	m_outMngrHist -> AddStandardTH1( "bjet"+str_id+"_btagw", 0.1,-1.1,1.1,  ";b-jet"+str_id+" MV2c10",         
					 false, &(m_outData -> o_bjets),  iBJet, "btagw" );
	m_outMngrHist -> AddStandardTH1( "bjet"+str_id+"_jvt",   0.1,-1.1,1.1,  ";b-jet"+str_id+" JVT",            
					 false, &(m_outData -> o_bjets),  iBJet, "jvt" );
	
	//for MC
	if(m_opt->VerboseOutput() && !m_opt->IsData() && (iBJet <= 0) ){
	  m_outMngrHist -> AddStandardTH1( "bjet_truth_b"+str_id+"_pt",    10, 0, 1000,     ";b-tagged Truth B-Jet"+str_id+" p_{T} [GeV]",  DrawSyst,   
					   &(m_outData -> o_bjets_truth_b), iBJet, "Pt");
	  m_outMngrHist -> AddStandardTH1( "bjet_truth_c"+str_id+"_pt",    10, 0, 1000,     ";b-tagged Truth C-Jet"+str_id+" p_{T} [GeV]",  DrawSyst,   
					   &(m_outData -> o_bjets_truth_c), iBJet, "Pt");
	  m_outMngrHist -> AddStandardTH1( "bjet_truth_tau"+str_id+"_pt",    10, 0, 1000,     ";b-tagged Truth #tau-Jet"+str_id+" p_{T} [GeV]",  
					   DrawSyst, &(m_outData -> o_jets_truth_tau), iBJet, "Pt");
	  m_outMngrHist -> AddStandardTH1( "bjet_truth_lqg"+str_id+"_pt",    10, 0, 1000,     ";b-tagged Truth Light-Jet"+str_id+" p_{T} [GeV]",  
					   DrawSyst, &(m_outData -> o_bjets_truth_lqg), iBJet, "Pt");
	}
	
      }
      for ( int iLJet=-1; iLJet<=4; ++iLJet  ) {
	std::string str_id = "";
	str_id += std::to_string(iLJet);
	if(iLJet==-1) str_id = "s";
	m_outMngrHist -> AddStandardTH1( "ljet"+str_id+"_pt",    10, 0, 500,   ";light-jet"+str_id+" p_{T} [GeV]",  
					 false, &(m_outData -> o_ljets), iLJet, "Pt" );
	m_outMngrHist -> AddStandardTH1( "ljet"+str_id+"_eta",   0.2, -3, 3,   ";light-jet"+str_id+" #eta",         
					 false, &(m_outData -> o_ljets), iLJet, "Eta" );
	m_outMngrHist -> AddStandardTH1( "ljet"+str_id+"_btagw", 0.1,-1.1,1.1, ";light-jet"+str_id+" MV2c10", 
					 false, &(m_outData -> o_ljets), iLJet, "btagw" );
	m_outMngrHist -> AddStandardTH1( "ljet"+str_id+"_jvt",   0.1,-1.1,1.1, ";light-jet"+str_id+" JVT",    
					 false, &(m_outData -> o_ljets), iLJet, "jvt" );
      }

      //================ TRACK JETS ======================================
      if(m_opt->BtagCollection() == VLQ_Options::TRACK){
	for ( int iTrkjet=-1; iTrkjet<=5; ++iTrkjet ) {
	  std::string str_id = "";
	  str_id += std::to_string(iTrkjet);
	  if(iTrkjet==-1) str_id = "s";
	  const bool DrawSyst = (iTrkjet <= 0) && otherVariables;
	  m_outMngrHist -> AddStandardTH1( "trkjet"+str_id+"_pt",    10, 0, 1000,     ";Trkjet"+str_id+" p_{T} [GeV]",  DrawSyst,   
					   &(m_outData -> o_trkjets), iTrkjet, "Pt");
	  m_outMngrHist -> AddStandardTH1( "trkjet"+str_id+"_eta",   0.2, -3, 3,     ";Trkjet"+str_id+" #eta",          DrawSyst,   
					   &(m_outData -> o_trkjets), iTrkjet, "Eta");
	  m_outMngrHist -> AddStandardTH1( "trkjet"+str_id+"_phi",   0.2, -3.5, 3.5, ";Trkjet"+str_id+" #varphi",       false,      
					   &(m_outData -> o_trkjets), iTrkjet, "Phi");
	  m_outMngrHist -> AddStandardTH1( "trkjet"+str_id+"_btagw", 0.1,-1.1,1.1,   ";Trkjet"+str_id+" MV2c10",        false,      
					   &(m_outData -> o_trkjets), iTrkjet, "btagw");
	  m_outMngrHist -> AddStandardTH1( "trkjet"+str_id+"_reff", 0.05,0.,1.0,   ";Trkjet"+str_id+" R_{eff}",        false,      
					   &(m_outData -> o_trkjets), iTrkjet, "reff");
	  
	  //for MC
	  if(m_opt->VerboseOutput() && !m_opt->IsData() && (iTrkjet <= 0) ){
	    m_outMngrHist -> AddStandardTH1( "trkjet_truth_b"+str_id+"_pt",    10, 0, 1000,     ";Truth B-Trkjet"+str_id+" p_{T} [GeV]",  DrawSyst,   
					     &(m_outData -> o_trkjets_truth_b), iTrkjet, "Pt");
	    m_outMngrHist -> AddStandardTH1( "trkjet_truth_c"+str_id+"_pt",    10, 0, 1000,     ";Truth C-Trkjet"+str_id+" p_{T} [GeV]",  DrawSyst,   
					     &(m_outData -> o_trkjets_truth_c), iTrkjet, "Pt");
	    m_outMngrHist -> AddStandardTH1( "trkjet_truth_tau"+str_id+"_pt",    10, 0, 1000,     ";b-tagged Truth #tau-Trket"+str_id+" p_{T} [GeV]",  
					     DrawSyst, &(m_outData -> o_trkjets_truth_tau), iTrkjet, "Pt");
	    m_outMngrHist -> AddStandardTH1( "trkjet_truth_lqg"+str_id+"_pt",    10, 0, 1000,     ";Truth Light-Trkjet"+str_id+" p_{T} [GeV]",  DrawSyst,   
					     &(m_outData -> o_trkjets_truth_lqg), iTrkjet, "Pt");
	    
	  }
	  
	}
	
	for ( int iBTrkjet=-1; iBTrkjet<=3; ++iBTrkjet ) {
	  std::string str_id = "";
	  str_id += std::to_string(iBTrkjet);
	  if(iBTrkjet==-1) str_id = "s";
	  const bool DrawSyst = (iBTrkjet <= 0) && otherVariables;
	  m_outMngrHist -> AddStandardTH1( "trkbjet"+str_id+"_pt",    10, 0, 1000,    ";b-trkjet"+str_id+" p_{T} [GeV]",   
					   false, &(m_outData -> o_trkbjets),  iBTrkjet, "Pt" );
	  m_outMngrHist -> AddStandardTH1( "trkbjet"+str_id+"_eta",   0.2, -3, 3,    ";b-trkjet"+str_id+" #eta",           
					   false, &(m_outData -> o_trkbjets),  iBTrkjet, "Eta" );
	  m_outMngrHist -> AddStandardTH1( "trkbjet"+str_id+"_btagw", 0.1,-1.1,1.1,  ";b-trkjet"+str_id+" MV2c10",         
					   false, &(m_outData -> o_trkbjets),  iBTrkjet, "btagw" );
	  m_outMngrHist -> AddStandardTH1( "trkbjet"+str_id+"_reff", 0.05,0.,0.4,   ";b-trkjet"+str_id+" R_{eff}",        false,      
					   &(m_outData -> o_trkjets), iBTrkjet, "reff");
	  
	  //for MC
	  if(m_opt->VerboseOutput() && !m_opt->IsData() && (iBTrkjet <= 0) ){
	    m_outMngrHist -> AddStandardTH1( "trkbjet_truth_b"+str_id+"_pt",    10, 0, 1000,     ";b-tagged Truth B-Trkjet"+str_id+" p_{T} [GeV]", 
					     DrawSyst, &(m_outData -> o_trkbjets_truth_b), iBTrkjet, "Pt");
	    m_outMngrHist -> AddStandardTH1( "trkbjet_truth_c"+str_id+"_pt",    10, 0, 1000,     ";b-tagged Truth C-Trkjet"+str_id+" p_{T} [GeV]", 
					     DrawSyst, &(m_outData -> o_trkbjets_truth_c), iBTrkjet, "Pt");
	    m_outMngrHist -> AddStandardTH1( "trkbjet_truth_tau"+str_id+"_pt",    10, 0, 1000,     ";b-tagged Truth #tau-Trket"+str_id+" p_{T} [GeV]",  
					     DrawSyst, &(m_outData -> o_trkbjets_truth_tau), iBTrkjet, "Pt");
	    m_outMngrHist -> AddStandardTH1( "trkbjet_truth_lqg"+str_id+"_pt",    10, 0, 1000,     ";b-tagged Truth Light-Trkjet"+str_id+" p_{T} [GeV]",  
					     DrawSyst, &(m_outData -> o_trkbjets_truth_lqg), iBTrkjet, "Pt");
	    
	  }
	}
	for ( int iLTrkjet=-1; iLTrkjet<=4; ++iLTrkjet  ) {
	  std::string str_id = "";
	  str_id += std::to_string(iLTrkjet);
	  if(iLTrkjet==-1) str_id = "s";
	  m_outMngrHist -> AddStandardTH1( "trkljet"+str_id+"_pt",    10, 0, 500,   ";light-trkjet"+str_id+" p_{T} [GeV]",  
					   false, &(m_outData -> o_trkljets), iLTrkjet, "Pt" );
	  m_outMngrHist -> AddStandardTH1( "trkljet"+str_id+"_eta",   0.2, -3, 3,   ";light-trkjet"+str_id+" #eta",         
					   false, &(m_outData -> o_trkljets), iLTrkjet, "Eta" );
	  m_outMngrHist -> AddStandardTH1( "trkljet"+str_id+"_btagw", 0.1,-1.1,1.1, ";light-trkjet"+str_id+" MV2c10", 
					   false, &(m_outData -> o_trkljets), iLTrkjet, "btagw" );
	  m_outMngrHist -> AddStandardTH1( "trkljet"+str_id+"_reff", 0.05,0.,0.4,   ";light-trkjet"+str_id+" R_{eff}",        false,      
					   &(m_outData -> o_trkljets), iLTrkjet, "reff");
	}
	
      }//TRACK JETS
      
      //Large-R jets
      if( m_opt -> UseLargeRJets() ){
	m_outMngrHist -> AddStandardTH1( "FatJets_n", 1, -0.5, 5.5,     ";Number of large-R jets",  false, &(m_outData -> o_fjets_n) );
	for ( int iLRJet=-1; iLRJet<=0; ++iLRJet ) {
	  std::string str_id = "";
	  str_id += std::to_string(iLRJet);
	  if(iLRJet==-1) str_id = "s";
	  m_outMngrHist -> AddStandardTH1( "FatJet"+str_id+"_pt",  50, 0, 1000, ";Fat jet"+str_id+"  p_{T} [GeV]"      ,  false, &(m_outData -> o_fjets), iLRJet, "Pt");
	  m_outMngrHist -> AddStandardTH1( "FatJet"+str_id+"_eta", 0.2, -3, 3,  ";Fat jet"+str_id+"  #eta"             ,  false, &(m_outData -> o_fjets), iLRJet, "Eta");
	  m_outMngrHist -> AddStandardTH1( "FatJet"+str_id+"_m",   10, 0, 400,  ";Fat jet"+str_id+"  mass [GeV]"       ,  false, &(m_outData -> o_fjets), iLRJet, "M");
	}
      }
      
    }// DrawReco
    
    //Reclustered jet"+str_id+"  variables
    m_outMngrHist -> AddStandardTH1( "RCjets_n", 1, -0.5, 5.5,     ";Number of signal RC jets", otherVariables, &(m_outData -> o_rcjets_n) );
    for ( int iRCJet=-1; iRCJet<=0; ++iRCJet  ) {
      std::string str_id = "";
      str_id += std::to_string(iRCJet);
      if(iRCJet==-1) str_id = "s";
      //const bool DrawSyst = (iRCJet == 0) && otherVariables;

      if(DrawReco){
	m_outMngrHist -> AddStandardTH1( "RCjet"+str_id+"_pt",         50, 0, 1000, ";RC jet"+str_id+"  p_{T} [GeV]"      ,  false, &(m_outData -> o_rcjets), iRCJet, "Pt");
	m_outMngrHist -> AddStandardTH1( "RCjet"+str_id+"_eta",        0.2, -3, 3,  ";RC jet"+str_id+"  #eta"             ,  false, &(m_outData -> o_rcjets), iRCJet, "Eta");
	m_outMngrHist -> AddStandardTH1( "RCjet"+str_id+"_m",          10, 0, 500,  ";RC jet"+str_id+"  mass [GeV]"       ,  false, &(m_outData -> o_rcjets), iRCJet, "M");
	m_outMngrHist -> AddStandardTH1( "RCjet"+str_id+"_nconsts",    1, -0.5,5.5, ";Number of RC jet"+str_id+"  consts" ,  false,    &(m_outData -> o_rcjets), iRCJet, "nconsts");
	m_outMngrHist -> AddStandardTH1( "RCjet"+str_id+"_nbconsts",    1, -0.5,5.5, ";Number of RC jet"+str_id+" b-tagged consts" ,  false,    &(m_outData -> o_rcjets), iRCJet, "nbconsts");
	m_outMngrHist -> AddStandardTH1( "RCjet"+str_id+"_isRCTTMass", 1, -.5, 1.5, ";Mass Tag"          ,  false,    &(m_outData -> o_rcjets), iRCJet, "isRCTTMass");

	if(m_opt->VerboseOutput()){
	  m_outMngrHist -> AddStandardTH2( "RCjet"+str_id+"_pt", "RCjet"+str_id+"_m", 50, 0, 1200, 10, 0, 500
					   , "RC jet"+str_id+" p_{T} [GeV]; RC jet"+str_id+" p_{T} [GeV]; RC jet"+str_id+" mass [GeV]", "RC jet"+str_id+" mass [GeV]"
					   , false, &(m_outData -> o_rcjets), &(m_outData -> o_rcjets), iRCJet, iRCJet, false, "Pt", "M" );
	  m_outMngrHist -> AddStandardTH2( "RCjet"+str_id+"_pt", "RCjet"+str_id+"_nconsts", 50, 0, 1200, 1, -0.5, 5.5
					   , "RC jet"+str_id+" p_{T} [GeV]; RC jet"+str_id+" p_{T} [GeV]; Number of RC jet"+str_id+" consts", "Number of RC jet"+str_id+" consts"
					   , false, &(m_outData -> o_rcjets), &(m_outData -> o_rcjets), iRCJet, iRCJet, false, "Pt", "nconsts" );
	  m_outMngrHist -> AddStandardTH2( "RCjet"+str_id+"_pt", "RCjet"+str_id+"_nbconsts", 50, 0, 1200, 1, -0.5, 5.5
					   , "RC jet"+str_id+" p_{T} [GeV]; RC jet"+str_id+" p_{T} [GeV]; Number of RC jet"+str_id+" b-tagged consts", "Number of RC jet"+str_id+" b-tagged consts"
					   , false, &(m_outData -> o_rcjets), &(m_outData -> o_rcjets), iRCJet, iRCJet, false, "Pt", "nbconsts" );
	}
      }

      //
      // For truth-match studies
      //

      if(DrawTruth){
	m_outMngrHist -> AddStandardTH1( "RCjet"+str_id + "_pT_truth", 50.,0.,3000., ";RC jet"+str_id+" p_{T}^{truth}"
					 , false, &(m_outData -> o_rcjets), iRCJet, "pT_truth");
	m_outMngrHist -> AddStandardTH1( "RCjet"+str_id + "_fpT_truth", 0.1,0.,2., ";RC jet"+str_id+" p_{T}^{reco} / p_{T}^{truth}"
					 , false, &(m_outData -> o_rcjets), iRCJet, "fpT_reco");
	m_outMngrHist -> AddStandardTH1( "RCjet"+str_id + "_dR_truth", 0.1,0.,6., ";RC jet"+str_id+" #DeltaR^{truth}"
					 , false, &(m_outData -> o_rcjets), iRCJet, "dR_truth");
	m_outMngrHist -> AddStandardTH1( "RCjet"+str_id + "_nmatch_truth", 1.,0.,3., ";RC jet"+str_id+" N_{match}^{truth}"
					 , false, &(m_outData -> o_rcjets), iRCJet, "nmatch_truth");
	m_outMngrHist -> AddStandardTH1( "RCjet"+str_id + "_pdgId_truth", 1.,0.,30., ";RC jet"+str_id+" PDGID^{truth}"
					 , false, &(m_outData -> o_rcjets), iRCJet, "pdgId_truth");
	
	// m_outMngrHist -> AddStandardTH2( "RCjet"+str_id+"_pt", "RCjet"+str_id+"_pdgId_truth", 50, 0, 1000, 1, 0., 26.
	//          , "RC jet"+str_id+" p_{T} [GeV]; RC jet"+str_id+" p_{T} [GeV]; RC jet"+str_id+" PDGID^{truth}", "RC jet"+str_id+" PDGID^{truth}"
	//          , false, &(m_outData -> o_rcjets), &(m_outData -> o_rcjets), iRCJet, iRCJet, false, "Pt", "pdgId_truth" );
	// m_outMngrHist -> AddStandardTH2( "RCjet"+str_id+"_m", "RCjet"+str_id+"_pdgId_truth", 10, 0, 500, 1, 0., 26.
	//          , "RC jet"+str_id+" mass [GeV]; RC jet"+str_id+" mass [GeV]; RC jet"+str_id+" PDGID^{truth}", "RC jet"+str_id+" PDGID^{truth}"
	//          , false, &(m_outData -> o_rcjets), &(m_outData -> o_rcjets), iRCJet, iRCJet, false, "M", "pdgId_truth" );
	// m_outMngrHist -> AddStandardTH2( "RCjet"+str_id+"_nconsts", "RCjet"+str_id+"_pdgId_truth", 1, -0.5, 5.5, 1, 0., 26.
	//          , "Number of RC jet"+str_id+" consts; Number of RC jet"+str_id+" consts; RC jet"+str_id+" PDGID^{truth}", "RC jet"+str_id+" PDGID^{truth}"
	//          , false, &(m_outData -> o_rcjets), &(m_outData -> o_rcjets), iRCJet, iRCJet, false, "nconsts", "pdgId_truth" );
	// m_outMngrHist -> AddStandardTH2( "RCjet"+str_id+"_nbconsts", "RCjet"+str_id+"_pdgId_truth", 1, -0.5, 5.5, 1, 0., 26.
	//          , "Number of RC jet"+str_id+" b-tagged consts; Number of RC jet"+str_id+" b-tagged consts; RC jet"+str_id+" PDGID^{truth}", "RC jet"+str_id+" PDGID^{truth}"
	//          , false, &(m_outData -> o_rcjets), &(m_outData -> o_rcjets), iRCJet, iRCJet, false, "nbconsts", "pdgId_truth" );
      }
    }
    

    if(DrawTruth && m_opt->VerboseOutput()){

      for ( const std::string type : m_truthRCTypes ){

	std::string tagstring = "";
	if(type=="truthTop"){ tagstring = "top"; }
	else if(type=="truthZ"){ tagstring = "Z"; }
	else if(type=="truthW"){ tagstring = "W"; }
	else if(type=="truthHiggs"){ tagstring = "Higgs"; }
	else if(type=="truthOther"){ tagstring = "unmatched"; }

	else if(type=="truthTop_inMassWindow"){ tagstring = "top (in mass window)"; }
	else if(type=="truthZ_inMassWindow"){ tagstring = "Z (in mass window)"; }
	else if(type=="truthW_inMassWindow"){ tagstring = "W (in mass window)"; }
	else if(type=="truthHiggs_inMassWindow"){ tagstring = "Higgs (in mass window)"; }

	for ( int iRCJet =-1; iRCJet <=0; ++iRCJet ) {
	  std::string str_id = "";
	  str_id += std::to_string(iRCJet);
	  if(iRCJet==-1) str_id = "s";    

	  m_outMngrHist -> AddStandardTH2( type+"_RCjet"+str_id+"_pt", type+"_RCjet"+str_id+"_m", 50, 0, 1200, 10, 0, 500
					   , "truth "+tagstring+" RC jet"+str_id+" p_{T} [GeV]; truth "+tagstring+" RC jet"+str_id+" p_{T} [GeV]; truth "+tagstring+" RC jet"+str_id+" mass [GeV]", 
					   "truth "+tagstring+" RC jet"+str_id+" mass [GeV]"
					   , false, &(m_outData -> o_rcjets_truthmatched.at(type)), &(m_outData -> o_rcjets_truthmatched.at(type)), iRCJet, iRCJet, false, "Pt", "M" );
	  m_outMngrHist -> AddStandardTH2( type+"_RCjet"+str_id+"_pt", type+"_RCjet"+str_id+"_nconsts", 50, 0, 1200, 1, -0.5, 5.5
					   , "truth "+tagstring+" RC jet"+str_id+" p_{T} [GeV]; truth "+tagstring+" RC jet"+str_id+" p_{T} [GeV]; Number of truth "+tagstring+" RC jet"+str_id+" consts"
					   , "Number of truth "+tagstring+" RC jet"+str_id+" consts"
					   , false, &(m_outData -> o_rcjets_truthmatched.at(type)), &(m_outData -> o_rcjets_truthmatched.at(type)), iRCJet, iRCJet, false, "Pt", "nconsts" );
	  m_outMngrHist -> AddStandardTH2( type+"_RCjet"+str_id+"_pt", type+"_RCjet"+str_id+"_nbconsts", 50, 0, 1200, 1, -0.5, 5.5
					   , "truth "+tagstring+" RC jet"+str_id+" p_{T} [GeV]; truth "+tagstring+" RC jet"+str_id+" p_{T} [GeV]; Number of truth "+tagstring+" RC jet"+str_id
					   +" b-tagged consts", "Number of truth "+tagstring+" RC jet"+str_id+" b-tagged consts"
					   , false, &(m_outData -> o_rcjets_truthmatched.at(type)), &(m_outData -> o_rcjets_truthmatched.at(type)), iRCJet, iRCJet, false, "Pt", "nbconsts" );
	
	  m_outMngrHist -> AddStandardTH1( type+"_RCjet"+str_id+"_pt", 50, 0, 1200, ";truth "+tagstring+" RC jet"+str_id+" p_{T} [GeV]", false, &(m_outData -> o_rcjets_truthmatched.at(type))
					   , iRCJet, "Pt");
	  m_outMngrHist -> AddStandardTH1( type+"_RCjet"+str_id+"_m", 10, 0, 500, ";truth "+tagstring+" RC jet"+str_id+" mass [GeV]", false, &(m_outData -> o_rcjets_truthmatched.at(type)), iRCJet, "M");
	  m_outMngrHist -> AddStandardTH1( type+"_RCjet"+str_id+"_nconsts", 1, -0.5, 5.5, ";Number of truth "+tagstring+" RC jet"+str_id+" consts", false
					   , &(m_outData -> o_rcjets_truthmatched.at(type)), iRCJet, "nconsts");
	  m_outMngrHist -> AddStandardTH1( type+"_RCjet"+str_id+"_nbconsts", 1, -0.5, 5.5, ";Number of truth "+tagstring+" RC jet"+str_id+" b-tagged consts"
					   , false, &(m_outData -> o_rcjets_truthmatched.at(type)), iRCJet, "nbconsts");
	}

	m_outMngrHist -> AddStandardTH1( type+"_RCjets_n", 1, -0.5, 4.5, ";Number of truth "+tagstring+" RC jets"
					 , false, &(m_outData -> o_rcjets_truthmatched_n.at(type)));
      }//TruthRC types
    }
    if(m_opt->VerboseOutput()){
      for ( const std::string decayType : {"Ht", "Zt", "Wb"} ){
	m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ_n", 1, -0.5, 5.5, ";Number of "+decayType+" reco VLQ",  otherVariables, &(m_outData -> o_recoVLQ_n.at(decayType)) );
	for ( int iTT =-1; iTT <=1; ++iTT ) {
	  std::string str_id = "";
	  str_id += std::to_string(iTT);
	  if(iTT==-1) str_id = "s";
	  const bool DrawSyst = (iTT == 0) && otherVariables;
	  if(DrawReco){
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ" + str_id + "_pt", 50, 0, 5000, ";"+decayType+" reco VLQ"+str_id+" p_{T} [GeV]" ,  
					     DrawSyst, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "Pt" );
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ" + str_id + "_eta", 0.2, -3, 3, ";"+decayType+" reco VLQ"+str_id+" #eta"        ,  
					     DrawSyst, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "Eta" );
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ" + str_id + "_m", 25, 0, 5000, ";"+decayType+" reco VLQ"+str_id+" mass [GeV]"    ,  
					     true, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "M" );
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ" + str_id + "_redm", 25, 0, 4000, ";"+decayType+" reco VLQ"+str_id+" reduced mass [GeV]"    ,  
					     true, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "redM" );
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ" + str_id + "_redm2", 25, 0, 4000, ";"+decayType+" reco VLQ"+str_id+" reduced mass [GeV]"    ,  
					     true, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "redM2" );

	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ" + str_id + "_m1", 10, 0, 500, ";"+decayType+" reco VLQ"+str_id+" m_{1} [GeV]"    ,  
					     DrawSyst, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "m1" );
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ" + str_id + "_m2", 10, 0, 500, ";"+decayType+" reco VLQ"+str_id+" m_{2} [GeV]"    ,  
					     DrawSyst, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "m2" );
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ" + str_id + "_dR12", 0.1, 0, 6, ";"+decayType+" reco VLQ"+str_id+" #DeltaR(1,2)"    ,  
					     DrawSyst, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "dR12" );
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ" + str_id + "_dPhi12", 0.1, 0, 4, ";"+decayType+" reco VLQ"+str_id+" #Delta#phi(1,2)"    ,  
					     DrawSyst, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "dPhi12" );
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ" + str_id + "_dEta12", 0.1, 0, 6, ";"+decayType+" reco VLQ"+str_id+" #Delta#eta(1,2)"    ,  
					     DrawSyst, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "dEta12" );
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ" + str_id + "_fpT12", 0.1, 0., 10., ";"+decayType+" reco VLQ"+str_id+" fpT(1,2)"    ,  
					     DrawSyst, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "fpT12" );
	  }
	
	  if(DrawTruth){
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ"+str_id + "_pT_truth", 50.,0.,3000., "; "+decayType+" reco VLQ"+str_id+" p_{T}^{truth}"
					     , false, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "pT_truth");
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ"+str_id + "_fpT_truth", 0.1,0.,2., "; "+decayType+" reco VLQ"+str_id+" p_{T}^{reco} / p_{T}^{truth}"
					     , false, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "fpT_reco");
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ"+str_id + "_dR_truth", 0.1,0.,6., "; "+decayType+" reco VLQ"+str_id+" #DeltaR^{truth}"
					     , false, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "dR_truth");
	    m_outMngrHist -> AddStandardTH1( decayType + "_recoVLQ"+str_id + "_nmatch_truth", 1.,0.,3., "; "+decayType+" reco VLQ"+str_id+" N_{match}^{truth}"
					     , false, &(m_outData -> o_recoVLQ.at(decayType)), iTT, "nmatch_truth");
	  
	  }
	
	}//index loop
      
      }//decay types
    
    }//Verbose Output

    //
    //Truth-matching of leptonic top and W
    //
    if(DrawTruth){

      //======= Leptonic top ======
      m_outMngrHist -> AddStandardTH1( "leptop_pT_truth",   50, 0, 2500, ";Leptonic top p_{T}^{truth} [GeV]"
				       ,  false, &(m_outData -> o_leptop), -1, "pT_truth");
      m_outMngrHist -> AddStandardTH1( "leptop_fpT_truth",   0.1, 0., 2., ";Leptonic top p_{T}^{reco}/p_{T}^{truth}"
               ,  false, &(m_outData -> o_leptop), -1, "fpT_truth");
      m_outMngrHist -> AddStandardTH1( "leptop_dR_truth",   0.1, 0., 6., ";Leptonic top #DeltaR^{truth}"
               ,  false, &(m_outData -> o_leptop), -1, "dR_truth");
      m_outMngrHist -> AddStandardTH1( "leptop_nmatch_truth",   1, 0, 2, ";Leptonic top isTruthMatched"
               ,  false, &(m_outData -> o_leptop), -1, "nmatch_truth");

      m_outMngrHist -> AddStandardTH2( "leptop_pT_truth", "leptop_fpT_truth"   
				       ,50, 0, 2500, 0.1, 0., 2.
				       ,"Leptonic top p_{T}^{truth} [GeV]", "Leptonic top p_{T}^{reco}/p_{T}^{truth}"
				       ,  false, &(m_outData -> o_leptop), &(m_outData -> o_leptop), -1, -1, true, "pT_truth", "fpT_truth");

      m_outMngrHist -> AddStandardTH2( "leptop_pT_truth", "leptop_dR_truth"
				       ,50, 0, 2500, 0.1, 0., 6.
				       ,"Leptonic top p_{T}^{truth} [GeV]", ";Leptonic top #DeltaR^{truth}"
				       ,  false, &(m_outData -> o_leptop), &(m_outData -> o_leptop), -1, -1, true, "pT_truth", "dR_truth");
      //======= Leptonic W ======
      m_outMngrHist -> AddStandardTH1( "lepW_pT_truth",   50, 0, 2500, ";Leptonic W p_{T}^{truth} [GeV]"
				       ,  false, &(m_outData -> o_lepW), -1, "pT_truth");
      m_outMngrHist -> AddStandardTH1( "lepW_fpT_truth",   0.1, 0., 2., ";Leptonic W p_{T}^{reco}/p_{T}^{truth}"
               ,  false, &(m_outData -> o_lepW), -1, "fpT_truth");
      m_outMngrHist -> AddStandardTH1( "lepW_dR_truth",   0.1, 0., 6., ";Leptonic W #DeltaR^{truth}"
               ,  false, &(m_outData -> o_lepW), -1, "dR_truth");
      m_outMngrHist -> AddStandardTH1( "lepW_nmatch_truth",   1, 0, 2, ";Leptonic W isTruthMatched"
               ,  false, &(m_outData -> o_lepW), -1, "nmatch_truth");


      m_outMngrHist -> AddStandardTH2( "lepW_pT_truth", "lepW_fpT_truth"   
				       ,50, 0, 2500, 0.1, 0., 2.
				       ,"Leptonic W p_{T}^{truth} [GeV]", "Leptonic W p_{T}^{reco}/p_{T}^{truth}"
				       ,  false, &(m_outData -> o_lepW), &(m_outData -> o_lepW), -1, -1, true, "pT_truth", "fpT_truth");

      m_outMngrHist -> AddStandardTH2( "lepW_pT_truth", "lepW_dR_truth"
				       ,50, 0, 2500, 0.1, 0., 6.
				       ,"Leptonic W p_{T}^{truth} [GeV]", ";Leptonic W #DeltaR^{truth}"
				       ,  false, &(m_outData -> o_lepW), &(m_outData -> o_lepW), -1, -1, true, "pT_truth", "dR_truth");

    }

    for ( const std::string type : m_topTaggers ){

      std::string tagstring = "";
      if(type=="RCTTMass"){ tagstring = "Mass-tagged"; }
      else if(type=="RCTTMass"){ tagstring = "Loose Mass-tagged"; }
      else if(type=="RCMTop"){ tagstring = "Top-tagged"; }
      else if(type=="RCMTop0b"){ tagstring = "Top-tagged (0b)"; }
      else if(type=="RCMTop1b"){ tagstring = "Top-tagged (1b)"; }
      else if(type=="RCMTop2bin"){ tagstring = "Top-tagged (#geq2b)"; }
      else if(type=="RCMHiggs"){ tagstring = "Higgs-tagged"; }
      else if(type=="RCMHiggs0b"){ tagstring = "Higgs-tagged (0b)"; }
      else if(type=="RCMHiggs1b"){ tagstring = "Higgs-tagged (1b)"; }
      else if(type=="RCMHiggs2bin"){ tagstring = "Higgs-tagged (#geq2b)"; }
      else if(type=="RCMTopHiggs"){ tagstring = "Top/Higgs-tagged"; }
      else if(type=="RCMV"){ tagstring = "W/Z-tagged"; }
      else if(type=="RCMV0b"){ tagstring = "W/Z-tagged (0b)"; }
      else if(type=="RCMV1bin"){ tagstring = "W/Z-tagged (#geq1b)"; }

      if(DrawReco){
	m_outMngrHist -> AddStandardTH1( type + "_jets_n", 1, -0.5, 5.5, ";Number of "+tagstring+" jets",  otherVariables, &(m_outData -> o_taggedjets_n.at(type)) );
	m_outMngrHist -> AddStandardTH1( type + "_leptop_dRmin",0.1, 0, 6.,";#DeltaR_{min}(Leptonic top, RC-tagged jet)", 
					 false, &(m_outData -> o_leptop), -1, "dRmin_"+type, hopt_nouflow);
	m_outMngrHist -> AddStandardTH1( type + "_leptop_b_dRmin",0.1, 0, 6.,";#DeltaR_{min}(Leptonic top-b, RC-tagged jet)", 
					 false, &(m_outData -> o_leptop), -1, "dRmin_b_"+type, hopt_nouflow);
      }
      for ( int iTT =-1; iTT <=0; ++iTT ) {
        std::string str_id = "";
        str_id += std::to_string(iTT);
        if(iTT==-1) str_id = "s";
        const bool DrawSyst = (iTT == 0) && otherVariables;
	if(DrawReco){
	  m_outMngrHist -> AddStandardTH1( type + "_jet" + str_id + "_pt", 50, 0, 1000, ";"+tagstring+" jet"+str_id+" p_{T} [GeV]" ,  
					   DrawSyst, &(m_outData -> o_taggedjets.at(type)), iTT, "Pt" );
	  m_outMngrHist -> AddStandardTH1( type + "_jet" + str_id + "_eta", 0.2, -3, 3, ";"+tagstring+" jet"+str_id+" #eta"        ,  
					   DrawSyst, &(m_outData -> o_taggedjets.at(type)), iTT, "Eta" );
	  m_outMngrHist -> AddStandardTH1( type + "_jet" + str_id + "_m", 10, 0, 500, ";"+tagstring+" jet"+str_id+" mass [GeV]"    ,  
					   DrawSyst, &(m_outData -> o_taggedjets.at(type)), iTT, "M" );
	  m_outMngrHist -> AddStandardTH1( type + "_jet" +str_id+ "_bconsts_n", 1, -0.5, 5.5, ";"+tagstring+" jet"+str_id+" N_{subjets}^{b-tagged}",  
					   DrawSyst, &(m_outData -> o_taggedjets.at(type)), iTT, "nbconsts" );
	  if(type=="RCTTMass" || type=="LooseRCTTMass"){
	    m_outMngrHist -> AddStandardTH1( type + "_jet" +str_id+ "_consts_n", 1, -0.5, 5.5, ";"+tagstring+" jet"+str_id+" N_{subjets}",  
					     DrawSyst, &(m_outData -> o_taggedjets.at(type)), iTT, "nconsts" );
	  }

	  if(m_opt->VerboseOutput()){	  
	    // 2D variables for boosted object tagging optimization studies
	    m_outMngrHist -> AddStandardTH2( type + "_jet" + str_id + "_pt", type + "_jet" + str_id + "_m", 50, 0, 1200, 10, 0, 500
					     , tagstring+" jet"+str_id+" p_{T} [GeV]; "+tagstring+" jet"+str_id+" p_{T} [GeV]; "+tagstring+" jet"+str_id+" mass [GeV]", 
					     tagstring+" jet"+str_id+" mass [GeV]"
					     , false, &(m_outData -> o_taggedjets.at(type)), &(m_outData -> o_taggedjets.at(type)), iTT, iTT, false, "Pt", "M" );
	    m_outMngrHist -> AddStandardTH2( type + "_jet" + str_id + "_pt", type + "_jet" + str_id + "_nconsts", 50, 0, 1200, 1, -0.5, 5.5
					     , tagstring+" jet"+str_id+" p_{T} [GeV]; "+tagstring+" jet"+str_id+" p_{T} [GeV]; Number of "+tagstring+" jet"+str_id+" consts"
					     , "Number of "+tagstring+" jet"+str_id+" consts"
					     , false, &(m_outData -> o_taggedjets.at(type)), &(m_outData -> o_taggedjets.at(type)), iTT, iTT, false, "Pt", "nconsts" );
	    m_outMngrHist -> AddStandardTH2( type + "_jet" + str_id + "_pt", type + "_jet" + str_id + "_nbconsts", 50, 0, 1200, 1, -0.5, 5.5
					     , tagstring+" jet"+str_id+" p_{T} [GeV]; "+tagstring+" jet"+str_id+" p_{T} [GeV]; Number of "+tagstring+" jet"+str_id+" b-tagged consts"
					     , "Number of "+tagstring+" jet"+str_id+" b-tagged consts"
					     , false, &(m_outData -> o_taggedjets.at(type)), &(m_outData -> o_taggedjets.at(type)), iTT, iTT, false, "Pt", "nbconsts" );
	  }	  
	}//DrawReco
	
	//
	// For truth-match studies
	//
	if(DrawTruth){
	  m_outMngrHist -> AddStandardTH1( type + "_jet"+str_id + "_pT_truth", 50.,0.,3000., "; "+tagstring+" jet"+str_id+" p_{T}^{truth}"
					   , false, &(m_outData -> o_taggedjets.at(type)), iTT, "pT_truth");
	  m_outMngrHist -> AddStandardTH1( type + "_jet"+str_id + "_fpT_truth", 0.1,0.,2., "; "+tagstring+" jet"+str_id+" p_{T}^{reco} / p_{T}^{truth}"
					   , false, &(m_outData -> o_taggedjets.at(type)), iTT, "fpT_truth");
	  m_outMngrHist -> AddStandardTH1( type + "_jet"+str_id + "_dR_truth", 0.1,0.,6., "; "+tagstring+" jet"+str_id+" #DeltaR^{truth}"
					   , false, &(m_outData -> o_taggedjets.at(type)), iTT, "dR_truth");
	  m_outMngrHist -> AddStandardTH1( type + "_jet"+str_id + "_nmatch_truth", 1.,0.,3., "; "+tagstring+" jet"+str_id+" N_{match}^{truth}"
					   , false, &(m_outData -> o_taggedjets.at(type)), iTT, "nmatch_truth");
	  m_outMngrHist -> AddStandardTH1( type + "_jet"+str_id + "_pdgId_truth", 1.,0.,26., "; "+tagstring+" jet"+str_id+" PDGID^{truth}"
					   , false, &(m_outData -> o_taggedjets.at(type)), iTT, "pdgId_truth");
	}
	
      }//index loop
    }
    
    //Electron variables
    m_outMngrHist -> AddStandardTH1( "el_n", 1, -0.5, 5.5, ";Number of signal electrons", false, &(m_outData -> o_el_n) );
    for ( int iEl=-1; iEl<=0; ++iEl ) {
      std::string str_id = "";
      str_id += std::to_string(iEl);
      if(iEl==-1) str_id = "s";
      m_outMngrHist -> AddStandardTH1( "el"+str_id+"_pt",  50, 0, 800,            ";Electron p_{T} [GeV]"        ,  false, &(m_outData -> o_el), iEl, "Pt" );
      m_outMngrHist -> AddStandardTH1( "el"+str_id+"_pt_zoom",  10, 0, 500,       ";Electron p_{T} [GeV]"        ,  false, &(m_outData -> o_el), iEl, "Pt" );
      m_outMngrHist -> AddStandardTH1( "el"+str_id+"_eta",  0.2, -3, 3,           ";Electron #eta"               ,  false, &(m_outData -> o_el), iEl, "Eta" );
      m_outMngrHist -> AddStandardTH1( "el"+str_id+"_phi",  0.2, -4, 4,           ";Electron #phi"               ,  false, &(m_outData -> o_el), iEl, "Phi" );
      //m_outMngrHist -> AddStandardTH1( "el"+str_id+"_d0sig",  0.1, 0, 5,          ";Electron d_{0}^{sig}"        ,  false, &(m_outData -> o_el), iEl, "d0sig" );
      //m_outMngrHist -> AddStandardTH1( "el"+str_id+"_z0",  0.025, -0.5, 0.5,      ";Electron z_{0} [mm]"         ,  false, &(m_outData -> o_el), iEl, "z0" );
      //m_outMngrHist -> AddStandardTH1( "el"+str_id+"_ptvarcone20",  0.01, 0, 0.1, ";Electron ptvarcone20/p_{T}"  ,  false, &(m_outData -> o_el), iEl, "ptvarcone20" );
      //m_outMngrHist -> AddStandardTH1( "el"+str_id+"_topoetcone20",  0.01, 0, 0.1,";Electron topoetcone20/p_{T}" ,  false, &(m_outData -> o_el), iEl, "topoetcone20" );
    }

    //Muon variables
    m_outMngrHist -> AddStandardTH1( "mu_n", 1, -0.5, 5.5, ";Number of signal muons", false, &(m_outData -> o_mu_n) );
    for ( int iMu=-1; iMu<=0; ++iMu ) {
      std::string str_id = "";
      str_id += std::to_string(iMu);
      if(iMu==-1) str_id = "s";
      m_outMngrHist -> AddStandardTH1( "mu"+str_id+"_pt",  50, 0, 800,            ";Muon p_{T} [GeV]"         , false,  &(m_outData -> o_mu), iMu, "Pt" );
      m_outMngrHist -> AddStandardTH1( "mu"+str_id+"_pt_zoom",  10, 0, 500,       ";Muon p_{T} [GeV]"         , false,  &(m_outData -> o_mu), iMu, "Pt" );
      m_outMngrHist -> AddStandardTH1( "mu"+str_id+"_eta",  0.2, -3, 3,           ";Muon #eta"                , false,  &(m_outData -> o_mu), iMu, "Eta" );
      m_outMngrHist -> AddStandardTH1( "mu"+str_id+"_phi",  0.2, -4, 4,           ";Muon #phi"                , false,  &(m_outData -> o_mu), iMu, "Phi" );
      //m_outMngrHist -> AddStandardTH1( "mu"+str_id+"_d0sig",  0.1, 0, 5,          ";Muon d_{0}^{sig}"         , false,  &(m_outData -> o_mu), iMu, "d0sig" );
      //m_outMngrHist -> AddStandardTH1( "mu"+str_id+"_z0",  0.025, -0.5, 0.5,      ";Muon z_{0}"               , false,  &(m_outData -> o_mu), iMu, "z0" );
      //m_outMngrHist -> AddStandardTH1( "mu"+str_id+"_ptvarcone30",  0.01, 0, 0.2, ";Muon ptvarcone30/p_{T}"   ,  false, &(m_outData -> o_mu), iMu, "ptvarcone30" );
      //m_outMngrHist -> AddStandardTH1( "mu"+str_id+"_topoetcone20",  0.01, 0, 0.2,";Muon topoetcone20/p_{T}"  ,  false, &(m_outData -> o_mu), iMu, "topoetcone20" );
    }
   

    //Lepton variables
    if(DrawReco){
      m_outMngrHist -> AddStandardTH1( "lep_n", 1, -0.5, 5.5, ";Number of signal leptons", false, &(m_outData -> o_lep_n) );
      for ( int iLep=-1; iLep<=0; ++iLep ) {
	std::string str_id = "";
	str_id += std::to_string(iLep);
	if(iLep==-1) str_id = "s";
	m_outMngrHist -> AddStandardTH1( "lep"+str_id+"_pt",  50, 0, 800,            ";Lepton p_{T} [GeV]"     ,  otherVariables,           &(m_outData -> o_lep), iLep, "Pt" );
	m_outMngrHist -> AddStandardTH1( "lep"+str_id+"_pt_zoom",  20, 10, 500,       ";Lepton p_{T} [GeV]"     ,  otherVariables,  &(m_outData -> o_lep), iLep, "Pt" );
	m_outMngrHist -> AddStandardTH1( "lep"+str_id+"_eta",  0.2, -3, 3,           ";Lepton #eta"            ,  otherVariables,  &(m_outData -> o_lep), iLep, "Eta" );
	m_outMngrHist -> AddStandardTH1( "lep"+str_id+"_phi",  0.2, -4, 4,           ";Lepton #phi"            ,  false,           &(m_outData -> o_lep), iLep, "Phi" );
	//m_outMngrHist -> AddStandardTH1( "lep"+str_id+"_d0sig",  0.1, 0, 5,          ";Lepton d_{0}^{sig}"     ,  false,           &(m_outData -> o_lep), iLep, "d0sig" );
	//m_outMngrHist -> AddStandardTH1( "lep"+str_id+"_z0",  0.025, -0.5, 0.5,      ";Lepton z_{0} [mm]"      ,  false,           &(m_outData -> o_lep), iLep, "z0" );
      }

      if(m_opt->VerboseOutput()){

	//Kinematic variables
	m_outMngrHist -> AddStandardTH1( "dR_ejet",         0.25,0,5,  ";#DeltaR_{min}(e,jet)"                  , false, &(m_outData -> o_dRmin_ejets)     );
	m_outMngrHist -> AddStandardTH1( "dR_mujet",        0.25,0,5,  ";#DeltaR_{min}(#mu,jet)"                , false, &(m_outData -> o_dRmin_mujets)    );
	m_outMngrHist -> AddStandardTH1( "dR_jetjet",       0.25,0,5,  ";#DeltaR_{min}(jet,jet)"                , false, &(m_outData -> o_dRmin_jetjet)    );
	m_outMngrHist -> AddStandardTH1( "dR_bjetbjet",     0.25,0,5,  ";#DeltaR_{min}(b-jet,b-jet)"            , false, &(m_outData -> o_dRmin_bjetbjet)  );
	m_outMngrHist -> AddStandardTH1( "dR_ebjet",        0.25,0,5,  ";#DeltaR_{min}(e,b-jet)"                , false, &(m_outData -> o_dRmin_ebjets)     );
	m_outMngrHist -> AddStandardTH1( "dR_mubjet",       0.25,0,5,  ";#DeltaR_{min}(#mu,b-jet)"              , false, &(m_outData -> o_dRmin_mubjets)    );
	m_outMngrHist -> AddStandardTH1( "dRmin_TTL_bjets", 0.25,0,5,  ";#DeltaR^{min}(top tagged,b-jets)"      , false, &(m_outData -> o_dR_TTL_bjets)    );
	m_outMngrHist -> AddStandardTH1( "dRmin_TTT_bjets", 0.25,0,5,  ";#DeltaR^{min}(top tagged,b-jets)"      , false, &(m_outData -> o_dR_TTT_bjets)    );
	m_outMngrHist -> AddStandardTH1( "dRmin_TTLooser_bjets", 0.25,0,5,  ";#DeltaR^{min}(top tagged,b-jets)" , false, &(m_outData -> o_dR_TTLooser_bjets));

	m_outMngrHist -> AddStandardTH1( "dPhi_lepmet",     0.1,0.,4,  ";#Delta#Phi(MET,lep)" ,      false, &(m_outData -> o_dPhi_lepmet));
	m_outMngrHist -> AddStandardTH1( "dPhi_jetmet",     0.1,0.,4,  ";#Delta#Phi^{min}(MET,jet)" ,      false, &(m_outData -> o_dPhi_jetmet));
	m_outMngrHist -> AddStandardTH1( "dPhi_jetmet5",     0.1,0,4,  ";#Delta#Phi^{min}(MET,jet)" ,      false, &(m_outData -> o_dPhi_jetmet5));
	m_outMngrHist -> AddStandardTH1( "dPhi_jetmet6",     0.1,0,4,  ";#Delta#Phi^{min}(MET,jet)" ,      false, &(m_outData -> o_dPhi_jetmet6));
	m_outMngrHist -> AddStandardTH1( "dPhi_jetmet7",     0.1,0,4,  ";#Delta#Phi^{min}(MET,jet)" ,      false, &(m_outData -> o_dPhi_jetmet7));
	m_outMngrHist -> AddStandardTH1( "dPhi_lepjet",     0.1,0.,4,  ";#Delta#Phi^{min}(lep,jet)" ,      false, &(m_outData -> o_dPhi_lepjet));
	m_outMngrHist -> AddStandardTH1( "dPhi_lepbjet",    0.1,0.,4,  ";#Delta#Phi^{min}(lep,b-jets)" ,   false, &(m_outData -> o_dPhi_lepbjet));
      }

      m_outMngrHist -> AddStandardTH1( "mbb_mindR",       10,0,1000,  ";m_{bb}^{#DeltaR min} [GeV]"     , false, &(m_outData -> o_mbb_mindR)        );

      m_outMngrHist->AddStandardTH1("jets40_n",                 1, -2.5, 15.5,";Number of jets with p_{T}>40 GeV", false, &(m_outData->o_jets40_n ));
      m_outMngrHist->AddStandardTH1("fwdjets30_n",              1, -0.5, 8.5,";Number of fwd-jets with p_{T}>30 GeV", false, &(m_outData->o_fwdjets30_n ));
      m_outMngrHist->AddStandardTH1("fwdjets40_n",              1, -0.5, 8.5,";Number of fwd-jets with p_{T}>40 GeV", false, &(m_outData->o_fwdjets40_n ));
      m_outMngrHist->AddStandardTH1("centrality",               0.01, 0, 1 ,";Centrality", false, &(m_outData->o_centrality ));
      m_outMngrHist->AddStandardTH1("mbb_leading_bjets",        2, 0, 2000 ,";m(b,b) leading b-jets [GeV]", false, &(m_outData->o_mbb_leading_bjets ));
      m_outMngrHist->AddStandardTH1("mbb_softest_bjets",        2, 0, 2000 ,";m(b,b) softests b-jets [GeV]", false, &(m_outData->o_mbb_softest_bjets ));
      m_outMngrHist->AddStandardTH1("J_lepton_invariant_mass",  2, 0, 2000 ,";m(leading-J,lepton) [GeV]", false, &(m_outData->o_J_lepton_invariant_mass));
      m_outMngrHist->AddStandardTH1("J_leadingb_invariant_mass",2, 0, 2000 ,";m(leading-J,leading-b) [GeV]", false, &(m_outData->o_J_leadingb_invariant_mass));
      m_outMngrHist->AddStandardTH1("J_J_invariant_mass",       2, 0, 2000 ,";m(leading-J,subleading-J) [GeV]", false, &(m_outData->o_J_J_invariant_mass));
      m_outMngrHist->AddStandardTH1("dRaverage_bjetbjet",       0.25,0,5   ,";#DeltaR_{ave.}(b-jet,b-jet)", false, &(m_outData -> o_dRaverage_bjetbjet)  );
      m_outMngrHist->AddStandardTH1("dRaverage_jetjet",         0.25,0,5   ,";#DeltaR_{ave.}(jet,jet)", false, &(m_outData -> o_dRaverage_jetjet)  );

      if(m_opt->DoLowBRegions()){
	m_outMngrHist -> AddStandardTH1( "mtbmin_lowb_3b",      50, 0, 500,    ";m_{T}^{min}(b,MET) (LowB_3b)", false, &(m_outData->o_mTbmin_lowb_3b) );
	m_outMngrHist -> AddStandardTH1( "mtbmin_lowb_3b_zoom", 25, 0, 250,    ";m_{T}^{min}(b,MET) (LowB_3b)", otherVariables, &(m_outData->o_mTbmin_lowb_3b) );
	m_outMngrHist -> AddStandardTH1( "mbb_mindR_lowb_3b",  10,0,300,  ";m_{bb}^{#DeltaR min} (LowB_3b) [GeV]", otherVariables, &(m_outData -> o_mbb_mindR_lowb_3b) );
	m_outMngrHist -> AddStandardTH1( "dR_bjetbjet_lowb_3b", 0.25,0,5,  ";#DeltaR_{min}(b-jet,b-jet) (LowB_3b)", false, &(m_outData -> o_dRmin_bjetbjet_lowb_3b) );
	//---
	m_outMngrHist -> AddStandardTH1( "mtbmin_lowb_4b",      50, 0, 500,    ";m_{T}^{min}(b,MET) (LowB_4b)", false, &(m_outData->o_mTbmin_lowb_4b) );
	m_outMngrHist -> AddStandardTH1( "mtbmin_lowb_4b_zoom", 25, 0, 250,    ";m_{T}^{min}(b,MET) (LowB_4b)", otherVariables, &(m_outData->o_mTbmin_lowb_4b) );
	m_outMngrHist -> AddStandardTH1( "mbb_mindR_lowb_4b",  10,0,300,  ";m_{bb}^{#DeltaR min} (LowB_4b) [GeV]", otherVariables, &(m_outData -> o_mbb_mindR_lowb_4b) );
	m_outMngrHist -> AddStandardTH1( "dR_bjetbjet_lowb_4b", 0.25,0,5,  ";#DeltaR_{min}(b-jet,b-jet) (LowB_4b)", false, &(m_outData -> o_dRmin_bjetbjet_lowb_4b) );
      }
      
    }//DrawReco

    //Truth variables
    if(DrawTruth){
      std::vector<std::string> truthTypes = {};

      if(m_opt -> SampleName() == SampleName::VLQ ){
	truthTypes = {    
	  "VLQ", "VLQ_Ht", "VLQ_Zt", "VLQ_Wb",
	  "VLQ_Hbdect", "VLQ_Wlepb", "VLQ_Whadb", "VLQ_Zhadt",
	  "b1", "b2",
	  "H", "Hbb", "H_b1", "H_b2",
	  "hadtop", "hadtop_b", "hadtop_W", "hadtop_q1", "hadtop_q2",
	  "leptop", "leptop_b", "leptop_W", "leptop_lep", "leptop_nu",
	  "hadZ", "hadZ_q1", "hadZ_q2",
	  "lepZ", "lepZ_lep1", "lepZ_lep2",
	  "invZ",
	  "hadW", "hadW_q1", "hadW_q2",
	  "lepW", "lepW_lep", "lepW_nu"
	};
      }
      else if( m_outData -> o_is_ttbar ){
	truthTypes = {    
	  "hadtop", "hadtop_b", "hadtop_W", "hadtop_q1", "hadtop_q2",
	  "leptop", "leptop_b", "leptop_W", "leptop_lep", "leptop_nu"
	};
      }

      
      for ( const std::string type : truthTypes ){

	m_outMngrHist -> AddStandardTH1( "truth_"+type + "_n", 1, -0.5, 5.5, ";Number of truth "+type,  false, &(m_outData -> o_truth_partons_n.at(type)) );
	for ( int iTT =-1; iTT <=1; ++iTT ) {
	  std::string str_id = "";
	  str_id += std::to_string(iTT);
	  if(iTT==-1) str_id = "s";

	  m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_pt", 50, 0, 2500, "; truth "+type+", "+str_id+" p_{T} [GeV]" ,  
					   false, &(m_outData -> o_truth_partons.at(type)), iTT, "Pt" );
	  m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_eta", 0.2, -4, 4, "; truth "+type+", "+str_id+" #eta"        ,  
					   false, &(m_outData -> o_truth_partons.at(type)), iTT, "Eta" );
	  double  mmax_part = (type.find("VLQ") == std::string::npos) ? 500. : 2000.;
	  m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_m", 10, 0, mmax_part, "; truth "+type+", "+str_id+" mass [GeV]"    ,  
					   false, &(m_outData -> o_truth_partons.at(type)), iTT, "M" );

	  if(type.find("_nu") != std::string::npos || (type.find("invZ") != std::string::npos)){
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dPhi_MET",  0.1,0.,4,  "; truth #Delta#phi("+type+", "+str_id+" ,MET)"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dPhi_MET"/*, hopt_nouflow*/  );
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_frac_MET",  0.1,0.,4,  "; truth MET fraction("+type+", "+str_id+")"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "frac_MET"/*, hopt_nouflow*/  );
	  }
	  if(type.find("VLQ") != std::string::npos){
	  
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_fpT12",  0.25,0,5,  "; truth"+type+", "+str_id+" fpT_{12}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "fpT12"/*, hopt_nouflow*/  );
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dR12",  0.1,0,6,  "; truth"+type+", "+str_id+" #DeltaR_{12}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dR12"/*, hopt_nouflow*/  );
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dPhi12",  0.1,0.,4,  "; truth"+type+", "+str_id+" #Delta#phi_{12}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dPhi12"/*, hopt_nouflow*/  );
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dEta12",  0.1,0.,6,  "; truth"+type+", "+str_id+" #Delta#eta_{12}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dEta12"/*, hopt_nouflow*/  );
	  }
	  if(type=="hadtop" || type=="leptop"){
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dR_bW",  0.25,0,5,  "; truth"+type+", "+str_id+" #DeltaR(b,W)"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dR_bW"/*, hopt_nouflow*/  );
	    if(type=="leptop"){
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dR_blep",  0.25,0,5,  "; truth"+type+", "+str_id+" #DeltaR(b,lep)"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dR_blep"/*, hopt_nouflow*/  );
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dPhi_blep",  0.1,0.,4,  "; truth"+type+", "+str_id+" #Delta#phi(b,lep)"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dPhi_blep"/*, hopt_nouflow*/  );
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dPhi_bnu",  0.1,0.,4,  "; truth"+type+", "+str_id+" #Delta#phi(b,#nu)"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dPhi_bnu"/*, hopt_nouflow*/  );
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_mT_bnu", 25, 0, 3000, "; truth "+type+", "+str_id+" m_{T}(b,#nu) [GeV]"    ,  
					       false, &(m_outData -> o_truth_partons.at(type)), iTT, "mT_bnu"/*, hopt_nouflow*/ );
	    }//leptop
	    if(type=="hadtop"){
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dRmax_bq",  0.25,0,5,  "; truth"+type+", "+str_id+" #DeltaR_{max}(b,q_{1},q_{2})"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dRmax_bq"/*, hopt_nouflow*/  );
	    }//hadtop
	  
	  }//top
	  if(type == "H"){
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dR_bb",  0.25,0,5,  "; truth"+type+", "+str_id+" #DeltaR(b_{1},b_{2})"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dR_bb"/*, hopt_nouflow*/  );
	  }//Higgs
	  if( (type == "lepW") || (type == "leptop") || (type == "leptop_W") ){
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dPhi_lepnu",  0.1,0,4,  "; truth"+type+", "+str_id+" #Delta#phi(lep,#nu)"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dPhi_lepnu"/*, hopt_nouflow*/  );
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dEta_lepnu",  0.1,0,8,  "; truth"+type+", "+str_id+" #Delta#eta(lep,#nu)"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dEta_lepnu"/*, hopt_nouflow*/  );
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_mT_lepnu", 25, 0, 3000, "; truth "+type+", "+str_id+" m_{T}(lep,#nu) [GeV]"    ,  
					     false, &(m_outData -> o_truth_partons.at(type)), iTT, "mT_lepnu"/*, hopt_nouflow*/ );
	    if(type == "leptop"){
	      //Truth-matching
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_pT_reco_leptop", 50.,0.,3000., "; truth"+type+", "+str_id+" p_{T}^{reco}"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "pT_reco_leptop");
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_fpT_reco_leptop", 0.1,0.,2., "; truth"+type+", "+str_id+" p_{T}^{reco}/ p_{T}^{truth}"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "fpT_reco_leptop");
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_m_reco_leptop", 20.,0.,400., "; truth"+type+", "+str_id+" m^{reco}"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "m_reco_leptop");
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dR_reco_leptop", 0.1,0.,6., "; truth"+type+", "+str_id+" #DeltaR^{reco}"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dR_reco_leptop");
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_nmatch_reco_leptop", 1.,0.,2., "; truth"+type+", "+str_id+" IsRecoMatched"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "nmatch_reco_leptop");
	    }
	    else{
	      //Truth-matching
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_pT_reco", 50.,0.,3000., "; truth"+type+", "+str_id+" p_{T}^{reco}"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "pT_reco");
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_fpT_reco", 0.1,0.,2., "; truth"+type+", "+str_id+" p_{T}^{reco}/ p_{T}^{truth}"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "fpT_reco");
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_m_reco", 20.,0.,400., "; truth"+type+", "+str_id+" m^{reco}"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "m_reco");
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dR_reco", 0.1,0.,6., "; truth"+type+", "+str_id+" #DeltaR^{reco}"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dR_reco");
	      m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_nmatch_reco", 1.,0.,2., "; truth"+type+", "+str_id+" IsRecoMatched"
					       , false, &(m_outData -> o_truth_partons.at(type)), iTT, "nmatch_reco");
	    }

	  }//LepW or Leptop
	  if(type == "lepZ"){
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dR_leplep",  0.25,0,5,  "; truth"+type+", "+str_id+" #DeltaR(lep_{1},lep_{2})"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dR_leplep"/*, hopt_nouflow*/  );
	  }//LepZ
	  if( (type == "hadW") || (type == "hadZ") || (type == "hadtop") || (type == "hadtop_W") ){
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dR_qq",  0.25,0,5,  "; truth"+type+", "+str_id+" #DeltaR(q_{1},q_{2})"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dR_qq"/*, hopt_nouflow*/  );
	  }//HadW or HadZ or Hadtop

	  //
	  // Matched fat jet studies
	  //
	  if( (type.find("VLQ") != std::string::npos) ){

	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_pT_reco", 50.,0.,3500., "; truth"+type+", "+str_id+" p_{T}^{reco}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "pT_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_fpT_reco", 0.1,0.,2., "; truth"+type+", "+str_id+" p_{T}^{reco} / p_{T}^{truth}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "fpT_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_m_reco", 20.,0.,3000., "; truth"+type+", "+str_id+" m^{reco}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "m_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_nconsts_reco", 1.,0.,4., "; truth"+type+", "+str_id+" N_{consts}^{reco}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "nconsts_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_nbconsts_reco", 1.,0.,3., "; truth"+type+", "+str_id+" N_{b-consts}^{reco}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "nbconsts_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_isRCMTop", 1.,0.,2., "; truth"+type+", "+str_id+" isTop-tagged"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "isRCMTop_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_isRCMHiggs", 1.,0.,2., "; truth"+type+", "+str_id+" isHiggs-tagged"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "isRCMHiggs_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_isRCMV", 1.,0.,2., "; truth"+type+", "+str_id+" isV-tagged"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "isRCMV_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_nmatch_reco", 1.,0.,3., "; truth"+type+", "+str_id+" N_{match}^{reco}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "nmatch_reco");
	  }


	  if( (type == "leptop") || (type == "hadtop") || (type == "hadtop_W") || 
	      (type == "Hbb") || (type == "Hnonbb") || 
	      (type == "hadW") || (type == "hadZ") ){

	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_pT_reco", 50.,0.,2000., "; truth"+type+", "+str_id+" p_{T}^{RC}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "pT_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_fpT_reco", 0.1,0.,2., "; truth"+type+", "+str_id+" p_{T}^{RC} / p_{T}^{truth}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "fpT_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_m_reco", 20.,0.,3000., "; truth"+type+", "+str_id+" m^{RC}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "m_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_nconsts_reco", 1.,0.,4., "; truth"+type+", "+str_id+" N_{consts}^{reco}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "nconsts_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_nbconsts_reco", 1.,0.,3., "; truth"+type+", "+str_id+" N_{b-consts}^{reco}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "nbconsts_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_isRCMTop", 1.,0.,2., "; truth"+type+", "+str_id+" isTop-tagged"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "isRCMTop_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_isRCMHiggs", 1.,0.,2., "; truth"+type+", "+str_id+" isHiggs-tagged"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "isRCMHiggs_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_isRCMV", 1.,0.,2., "; truth"+type+", "+str_id+" isV-tagged"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "isRCMV_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_dR_reco", 0.1,0.,6., "; truth"+type+", "+str_id+" #DeltaR^{RC}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "dR_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_nmatch_reco", 1.,0.,3., "; truth"+type+", "+str_id+" N_{match}^{RC}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "nmatch_reco");
	    m_outMngrHist -> AddStandardTH1( "truth_" + type + str_id + "_ncontained_reco", 1.,0.,3., "; truth"+type+", "+str_id+" N_{contained}^{RC}"
					     , false, &(m_outData -> o_truth_partons.at(type)), iTT, "ncontained_reco");
	  }

	}//Number of objects of a type
      
      }//Type of truth

      m_outMngrHist -> AddStandardTH1( "truth_dRmin_bb",  0.25,0,5,  "; truth #DeltaR_min(b,b)"
               , false, &(m_outData -> o_truth_dRmin_bb), -1, ""/*, hopt_nouflow*/  );
      m_outMngrHist -> AddStandardTH1( "truth_minv_bb",  10.,0,1000.,  "; truth m_{inv}(b,b) [GeV]"
               , false, &(m_outData -> o_truth_minv_bb)  );
      m_outMngrHist -> AddStandardTH1( "truth_dPhiMin_lepmet", 0.1,0,4,  "; truth #Delta#phi(lep,MET)"
               , false, &(m_outData -> o_truth_dPhiMin_lepmet), -1, ""/*, hopt_nouflow*/  );
      m_outMngrHist -> AddStandardTH1( "truth_dPhiMin_jetmet", 0.1,0,4,  "; truth #Delta#phi_min(jet,MET)"
               , false, &(m_outData -> o_truth_dPhiMin_jetmet), -1, ""/*, hopt_nouflow*/  );
      m_outMngrHist -> AddStandardTH1( "truth_partonMET",  25.,0,2000.,  "; truth (ME) E_{T}^{miss} [GeV]"
               , false, &(m_outData -> o_truth_partonMET), -1, "Pt"/*, hopt_nouflow*/  );
      m_outMngrHist -> AddStandardTH1( "truth_mtw",  25.,0,3000.,  "; truth m_{T}^{W} [GeV]"
               , false, &(m_outData -> o_truth_mtw), -1, ""/*, hopt_nouflow*/  );
      m_outMngrHist -> AddStandardTH1( "truth_ptw",  25.,0,5000.,  "; truth p_{T}^{W} [GeV]"
               , false, &(m_outData -> o_truth_ptw), -1, ""/*, hopt_nouflow*/  );
      m_outMngrHist -> AddStandardTH1( "truth_mtbmin",  10.,0,1000.,  "; truth m_{T,b}^{min} [GeV]"
               , false, &(m_outData -> o_truth_mtbmin), -1, ""/*, hopt_nouflow*/  );
      /*
      //TH2
      m_outMngrHist -> AddStandardTH2( "top_pt", "dR_Wb", 20, 0, 800, 0.1, 0., 5.,"Top p_{T} [GeV]", "#DeltaR(W,b)", false, &(m_outData -> o_truth_top_pt), &(m_outData -> o_truth_dR_Wb) );
      m_outMngrHist -> StoreTProfile( "top_pt", "dR_Wb" );
      */
    }//DrawTruth

  }//if dumpHistos


  //############################################################################
  //
  // Initialisation of the Selector and WeightManager
  //
  //############################################################################
  m_selector->Init(); //This adds all selection regions and books histograms in each of them
  if(m_opt->MsgLevel()==Debug::DEBUG){
    m_selector->PrintSelectionTree();
  }
  m_weightMngr -> Init( m_selector -> GetSelectionTree() );

  //############################################################################
  //
  // Declaration of the OverlapTree
  //
  //############################################################################
  if(m_opt->DumpOverlapTree()){
    for(std::pair<int, Selection*> sel : *(m_selector->GetSelectionTree()) ){
      std::string region_type = "SR";
      if( (sel.second)->Name().find("c-0lep") != std::string::npos ){
        //Validation regions
        if( (sel.second)->Name().find("6jex") != std::string::npos ){
            region_type = "VR";
        }
        //Signal sensitive regions
        else if( (sel.second)->Name().find("0Tex-0Hex") != std::string::npos ){
            region_type = "CR";
        }
        //Signal sensitive regions
        else if( (sel.second)->Name().find("1Tex-0Hex") != std::string::npos ){
            region_type = "CR";
        }
      } else if( (sel.second)->Name().find("c-1lep") != std::string::npos ){
        //Validation regions
        if( (sel.second)->Name().find("5jex") != std::string::npos ){
            region_type = "VR";
        }
        //Signal sensitive regions
        else if( (sel.second)->Name().find("0Tex-0Hex") != std::string::npos ){
            region_type = "CR";
        }
        //Signal sensitive regions
        else if( (sel.second)->Name().find("1Tex-0Hex") != std::string::npos ){
            region_type = "CR";
        }
      }
      std::cout << "HTX " << region_type << " " << (sel.second)->Name() << " : region==" << (sel.second)->SelecInd() << std::endl;
    }
  }

  //############################################################################
  //
  // Declaration of the TruthManager
  //
  //############################################################################
  if( (m_opt -> SampleName() == SampleName::VLQ  ||  m_outData -> o_is_ttbar) && m_opt->InputTree()=="nominal" ) {
    m_truthMngr = new VLQ_TruthManager( m_opt, m_ntupData, m_outData, m_varComputer );
  } else {
    cout<<"VLQ_Analysis_Data2015::Begin: skipping truth part (VLQ-specific)"<<endl;
  }

  //
  // VLQ information
  //
  TH1D* hist = m_outMngrHist -> HistMngr() -> BookTH1D("vlqType","vlqType",1,-1.5,7.5,"","","#events");
  hist -> GetXaxis() -> SetBinLabel( hist -> FindBin(VLQ_Enums::HtHt),  "HtHt" );
  hist -> GetXaxis() -> SetBinLabel( hist -> FindBin(VLQ_Enums::HtWb),  "HtWb" );
  hist -> GetXaxis() -> SetBinLabel( hist -> FindBin(VLQ_Enums::HtZt),  "HtZt" );
  hist -> GetXaxis() -> SetBinLabel( hist -> FindBin(VLQ_Enums::WbWb),  "WbWb" );
  hist -> GetXaxis() -> SetBinLabel( hist -> FindBin(VLQ_Enums::WbZt),  "WbZt" );
  hist -> GetXaxis() -> SetBinLabel( hist -> FindBin(VLQ_Enums::ZtZt),  "ZtZt" );

  //
  // Rejection mask saving
  //
  m_outMngrHist -> HistMngr() -> BookTH1D("rejectionMask","Mask",1,0,10,"","","#events");
  m_outMngrHist -> HistMngr() -> GetTH1D("rejectionMask") -> GetXaxis() -> SetBinLabel( 1,                                    "Kept" );
  m_outMngrHist -> HistMngr() -> GetTH1D("rejectionMask") -> GetXaxis() -> SetBinLabel( VLQ_Enums::TRIGGER_REJECTED + 1,      "Trigger" );
  m_outMngrHist -> HistMngr() -> GetTH1D("rejectionMask") -> GetXaxis() -> SetBinLabel( VLQ_Enums::BADJET_REJECTED + 1,       "Bad jet" );
  m_outMngrHist -> HistMngr() -> GetTH1D("rejectionMask") -> GetXaxis() -> SetBinLabel( VLQ_Enums::LEPTON_REJECTED + 1,       "Lepton" );
  m_outMngrHist -> HistMngr() -> GetTH1D("rejectionMask") -> GetXaxis() -> SetBinLabel( VLQ_Enums::TRIGGERMATCH_REJECTED + 1, "Trigger match" );
  m_outMngrHist -> HistMngr() -> GetTH1D("rejectionMask") -> GetXaxis() -> SetBinLabel( VLQ_Enums::JETS_REJECTED + 1,         "Jets" );
  m_outMngrHist -> HistMngr() -> GetTH1D("rejectionMask") -> GetXaxis() -> SetBinLabel( VLQ_Enums::METMTW_REJECTED + 1,       "Met/Mtw" );
  m_outMngrHist -> HistMngr() -> GetTH1D("rejectionMask") -> GetXaxis() -> SetBinLabel( VLQ_Enums::DPHI_REJECTED + 1,         "#Delta#varphi" );

  m_outMngrHist -> HistMngr() -> BookTH1D("cutFlow_unWeight","Cut",1,0,20,"","","#events");
  m_outMngrHist -> HistMngr() -> BookTH1D("cutFlow_weight_2","Cut",1,0,20,"","","#events");
  m_outMngrHist -> HistMngr() -> BookTH1D("cutFlow_unWeight_e","Cut",1,0,20,"","","#events");
  m_outMngrHist -> HistMngr() -> BookTH1D("cutFlow_weight_2_e","Cut",1,0,20,"","","#events");
  m_outMngrHist -> HistMngr() -> BookTH1D("cutFlow_unWeight_mu","Cut",1,0,20,"","","#events");
  m_outMngrHist -> HistMngr() -> BookTH1D("cutFlow_weight_2_mu","Cut",1,0,20,"","","#events");

  m_outMngrHist -> HistMngr() -> BookTH1D("cutFlow0L_unWeight","Cut",1,0,20,"","","#events");
  m_outMngrHist -> HistMngr() -> BookTH1D("cutFlow0L_weight_2","Cut",1,0,20,"","","#events");

  return true;
}

//____________________________________________________________________________
//
bool VLQ_Analysis_Data2015::Loop(){

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Entering in VLQ_Analysis_Data2015::Loop()" << std::endl;

  int nEntries = m_reader -> ChainNEntries();
  for( int iEntry = 0; iEntry < nEntries; ++iEntry ){

    if(iEntry%10000==1) std::cout << "-> " << iEntry-1 << " / " << nEntries << " entries processed. " << std::endl;

    if(m_opt->SkipEvents()>-1 && iEntry < m_opt->SkipEvents()) continue;
    if(m_opt->NEvents()>-1 && iEntry > m_opt->NEvents()) continue;
    if(m_opt->PickEvent()>-1 && iEntry!=m_opt->PickEvent()) continue;
    this -> Process(iEntry);
  }

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "Leaving VLQ_Analysis_Data2015::Loop()" << std::endl;
  return true;
}

//____________________________________________________________________________
//
bool VLQ_Analysis_Data2015::Process(Long64_t entry)
{
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "== Processing entry : " << entry << std::endl;

  //
  // Put back all output variables to their initial value
  //
  m_outData -> ClearOutputData();
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After clearing OutputData " << entry << std::endl;

  //
  // Get entry in the input tree
  //
  m_reader -> GetChainEntry(entry);
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After getting entry " << entry << std::endl;

  //###########################################################
  //                                                          #
  // C0: If this is ttbar, let's just select what we want     #
  //                                                          #
  //###########################################################
  if( m_outData -> o_is_ttbar ){
    const int &ttbar_HF     = m_ntupData -> d_HF_SimpleClassification;
    const float &ht_truth   = m_ntupData -> d_HT_truth;
    const float &met_truth  = m_ntupData -> d_MET_truth;

    //
    // ttbar+HF splitting
    //
    if( m_opt -> SampleName() == SampleName::TTBARBB ){
      if( ttbar_HF <= 0 ) return false;
      const int classification = ( m_ntupData -> d_HF_Classification - ( m_ntupData -> d_HF_Classification % 100 ) ) / 100;
      //ttb
      if( (m_opt -> StrSampleName() == "TTBARBB_B") && classification != 10 ) return false;
      //ttbb
      if( (m_opt -> StrSampleName() == "TTBARBB_BB") && classification != 20 ) return false;
      //ttB
      if( (m_opt -> StrSampleName() == "TTBARBB_BIGB") && classification != 1 ) return false;
      //others
      if( (m_opt -> StrSampleName() == "TTBARBB_OTHERS") && (classification == 10 || classification == 20 || classification == 1) ) return false;
      //tt no B
      if( (m_opt -> StrSampleName() == "TTBARBB_NOBIGB") && classification == 1 ) return false;
    }
    if( m_opt -> SampleName() == SampleName::TTBARCC ){
      if( ttbar_HF >= 0 ) return false;
    }
    if( m_opt -> SampleName() == SampleName::TTBARLIGHT ){
      if( ttbar_HF != 0 ) return false;
    }

    //
    // ttbar HT/MET slicing
    //
    m_outData -> o_truth_ht_filter = ht_truth;
    m_outData -> o_truth_met_filter = met_truth;
    if( m_opt -> FilterType() == VLQ_Options::APPLYFILTER ){

      if( ( m_opt -> StrSampleID().find("410470.") != std::string::npos ) 
	  || ( m_opt -> StrSampleID().find("410464.") != std::string::npos ) || ( m_opt -> StrSampleID().find("410465.") != std::string::npos ) 
	  || ( m_opt -> StrSampleID().find("410557.") != std::string::npos ) || ( m_opt -> StrSampleID().find("410558.") != std::string::npos ) ){
	if( ht_truth > 600 ) return false;
      } 
      else if( ( m_opt -> StrSampleID().find("407344.") != std::string::npos )
	       || ( m_opt -> StrSampleID().find("407350.") != std::string::npos )
	       || ( m_opt -> StrSampleID().find("407356.") != std::string::npos ) ){
	if( ht_truth < 600 || ht_truth > 1000 ) return false;
      } 
      else if( ( m_opt -> StrSampleID().find("407343.") != std::string::npos )
	       || ( m_opt -> StrSampleID().find("407349.") != std::string::npos )
	       || ( m_opt -> StrSampleID().find("407355.") != std::string::npos ) ){
	if( ht_truth < 1000 || ht_truth > 1500 ) return false;
      } 
      else if( ( m_opt -> StrSampleID().find("407342.") != std::string::npos )
	       || ( m_opt -> StrSampleID().find("407348.") != std::string::npos )
	       || ( m_opt -> StrSampleID().find("407354.") != std::string::npos ) ){
	if( ht_truth < 1500 ) return false;
      }
      
    }//filter
  }//ttbar

  m_outData -> o_event_number = m_ntupData -> d_eventNumber;

  //###################################################
  //                                                  #
  // Trigger requirement                              #
  //                                                  #
  //###################################################
  bool trigMETPass = false;
  bool trigMuonPass = false;
  bool trigElecPass = false;

  m_outData->o_run_number = m_ntupData->d_runNumber;
  m_outData -> o_period = 0;
  if( m_outData->o_run_number >= 276262 && m_outData->o_run_number <= 284484 ){
    m_outData -> o_period = VLQ_Enums::DATA2015;
  } else if (m_outData->o_run_number >= 296939  && m_outData->o_run_number <= 311481 ){
    m_outData -> o_period = VLQ_Enums::DATA2016;
  } else if (m_outData->o_run_number >= 324320 && m_outData->o_run_number <= 341649 ){
    m_outData -> o_period = VLQ_Enums::DATA2017;
  } else if (m_outData->o_run_number >= 348885 && m_outData->o_run_number <= 364292 ){
    m_outData -> o_period = VLQ_Enums::DATA2018;
  }
  else{
    std::cerr << "Unknown run number " << m_outData->o_run_number << " event number ";
    std::cerr << m_ntupData->d_eventNumber << " belonging to neither 2015 nor 2016 nor 2017 nor 2018. Please check" << std::endl;
  }

  bool trigPass = false;
  for(TriggerInfo* trig : m_outData -> o_trigger_list){
    bool _pass = false;
    if( !(trig->Period() & m_outData->o_period) ) continue;
    if( (!m_opt->UseMETTrigger() && !m_opt->UseMETTriggerOneLep() && trig->Type()==VLQ_Enums::TRIGMET)
    || (!m_opt->UseLeptonTrigger() && ((trig->Type()==VLQ_Enums::TRIGELEC)||(trig->Type()==VLQ_Enums::TRIGMUON))) ) continue;
    _pass = m_ntupData -> d_triggers.at(trig->Name());
    trigPass = trigPass || _pass;
    if(trig->Type()==VLQ_Enums::TRIGMET){ trigMETPass = trigMETPass || _pass; }
    else if(trig->Type()==VLQ_Enums::TRIGMUON){ trigMuonPass = trigMuonPass || _pass; }
    else if(trig->Type()==VLQ_Enums::TRIGELEC){ trigElecPass = trigElecPass || _pass; }
    trig->SetPass(_pass);
  }

  if(!trigPass){
    m_outData -> o_rejectEvent |= 1 << VLQ_Enums::TRIGGER_REJECTED;
  }

  //###########################################################
  //                                                          #
  // Build object vectors                                     #
  //                                                          #
  //###########################################################
  const bool OKObjects = m_anaTools -> GetObjectVectors();
  if(!OKObjects){
    std::cerr << "<!> Problem => an error occured during the filling of object vectors" << std::endl;
    abort();
  }
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After building the objects" << std::endl;

  //###########################################################
  //                                                          #
  // C2: Lepton requirement                                   #
  //                                                          #
  //###########################################################
  AnalysisObject *lepton = 0;
  m_outData-> o_channel_type = VLQ_Enums::UNDEFINED;
  if( (!(m_opt->DoOneLeptonAna()) && ((m_outData -> o_mu_n + m_outData -> o_el_n) == 1))
      || (!(m_opt->DoTwoLeptonAna()) && ((m_outData -> o_mu_n + m_outData -> o_el_n) == 2))
      || (!(m_opt->DoZeroLeptonAna()) && ((m_outData -> o_mu_n + m_outData -> o_el_n) == 0)) ){
    m_outData -> o_rejectEvent |= 1 << VLQ_Enums::LEPTON_REJECTED;
  }//lepton number does not match requested channels
  else{
    if ( (m_outData -> o_mu_n + m_outData -> o_el_n) == 1 ) {
      if(m_opt -> StrSampleName().find("QCD") != std::string::npos){
	//This is QCD
	if(m_opt -> SampleName() == SampleName::QCDE){//QCD in electron channel
	  if ( m_outData -> o_el_n == 1 ){
	    lepton = m_outData -> o_el -> at(0);
	    m_outData-> o_channel_type = VLQ_Enums::ELECTRON;
	  }
	} else if(m_opt -> SampleName() == SampleName::QCDMU){//QCD in the muon channel
	  if( m_outData -> o_mu_n == 1 ){
	    lepton = m_outData -> o_mu -> at(0);
	    m_outData-> o_channel_type = VLQ_Enums::MUON;
	  }
	}
      } else {
	//This is not QCD
	if( m_outData -> o_mu_n == 1 ){
	  lepton = m_outData -> o_mu -> at(0);
	  m_outData-> o_channel_type = VLQ_Enums::MUON;
	} else if ( m_outData -> o_el_n == 1 ){
	  lepton = m_outData -> o_el -> at(0);
	  m_outData-> o_channel_type = VLQ_Enums::ELECTRON;
	}
      }
      m_outData -> o_selLep = lepton;
    }//1-lepton 
    else if ( (m_outData -> o_mu_n + m_outData -> o_el_n) == 0 ) {
      m_outData-> o_channel_type = VLQ_Enums::FULLHAD;
    }//0-lepton 
    else if ( (m_outData -> o_mu_n + m_outData -> o_el_n) == 2 ) {
      if( m_outData -> o_el_n == 2){
	m_outData-> o_channel_type = VLQ_Enums::ELEL;
	lepton = m_outData -> o_el -> at(0);
      }//ee
      else if( m_outData -> o_mu_n == 2){
	m_outData-> o_channel_type = VLQ_Enums::MUMU;
	lepton = m_outData -> o_mu -> at(0);
      }//mumu
      else {
	m_outData-> o_channel_type = VLQ_Enums::ELMU;
	if(m_outData -> o_mu -> at(0)->Pt() > m_outData -> o_el -> at(0)->Pt()){
	  lepton = m_outData -> o_mu -> at(0);
	}
	else{
	  lepton = m_outData -> o_el -> at(0);
	}
      }//emu
    }//2-lepton
    else {
      m_outData -> o_rejectEvent |= 1 << VLQ_Enums::LEPTON_REJECTED;
    }

  }// if lepton number matches requested channels
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After lepton selection" << std::endl;

  //###########################################################
  //                                                          #
  // C3: Trigger matching                                     #
  //                                                          #
  //###########################################################
  bool isTriggerMatched = false;
  if( !(m_outData-> o_channel_type == VLQ_Enums::UNDEFINED || m_outData-> o_channel_type == VLQ_Enums::FULLHAD) ){
    bool checkTriggerMatching = true;
    if( !m_opt -> UseLeptonTrigger()) checkTriggerMatching = false;
    if( m_opt -> UseMETTriggerOneLep() && trigMETPass && m_outData -> o_AO_met -> Pt() > 200 ) checkTriggerMatching = false;
    if( checkTriggerMatching ){

      bool ele_ch = (m_outData-> o_channel_type == VLQ_Enums::ELECTRON) || (m_outData-> o_channel_type == VLQ_Enums::ELEL); 
      bool mu_ch = (m_outData-> o_channel_type == VLQ_Enums::MUON) || (m_outData-> o_channel_type == VLQ_Enums::MUMU); 

      if(m_outData-> o_channel_type == VLQ_Enums::ELMU){
	ele_ch = m_outData -> o_el -> at(0)->Pt() > m_outData -> o_mu -> at(0)->Pt();
	mu_ch = !ele_ch;
      }

      for(TriggerInfo* trig : m_outData -> o_trigger_list){
        if( !(trig->Period() & m_outData->o_period) ) continue;
        if( (trig->Type()==VLQ_Enums::TRIGELEC && ele_ch) || (trig->Type()==VLQ_Enums::TRIGMUON && mu_ch) ){
          if( (int)lepton->GetMoment(trig->Name()) ){
            isTriggerMatched = true;
            break;
          }
        }
      }
      if( !isTriggerMatched ){
        m_outData -> o_rejectEvent |= 1 << VLQ_Enums::TRIGGERMATCH_REJECTED;
      }
    }
  }//lepton trigger

  //###########################################################
  //                                                          #
  // Refining channel definition                              #
  //                                                          #
  //###########################################################

  bool isElectronChannel  = false;
  bool isMuonChannel      = false;
  bool isElElChannel  = false;
  bool isMuMuChannel      = false;
  bool isElMuChannel      = false;

  bool isElectronChannelHMET  = false;
  bool isMuonChannelHMET      = false;
  bool isElElChannelHMET  = false;
  bool isMuMuChannelHMET      = false;
  bool isElMuChannelHMET      = false;

  if(m_opt->UseLeptonTrigger()){
    isElectronChannel = ( m_outData-> o_channel_type == VLQ_Enums::ELECTRON ) && trigElecPass && isTriggerMatched;
    isMuonChannel     = ( m_outData-> o_channel_type == VLQ_Enums::MUON )     && trigMuonPass && isTriggerMatched;
    isElElChannel = ( m_outData-> o_channel_type == VLQ_Enums::ELEL ) && trigElecPass && isTriggerMatched;
    isMuMuChannel     = ( m_outData-> o_channel_type == VLQ_Enums::MUMU )     && trigMuonPass && isTriggerMatched;
    isElMuChannel     = ( m_outData-> o_channel_type == VLQ_Enums::ELMU )     && (trigElecPass || trigMuonPass) && isTriggerMatched;
  }
  if(m_opt->UseMETTriggerOneLep()){
    isElectronChannelHMET = ( m_outData-> o_channel_type == VLQ_Enums::ELECTRON ) && ( trigMETPass && m_outData -> o_AO_met -> Pt() > 200 );
    isMuonChannelHMET     = ( m_outData-> o_channel_type == VLQ_Enums::MUON )     && ( trigMETPass && m_outData -> o_AO_met -> Pt() > 200 );
    isElElChannelHMET = ( m_outData-> o_channel_type == VLQ_Enums::ELEL ) && ( trigMETPass && m_outData -> o_AO_met -> Pt() > 200 );
    isMuMuChannelHMET     = ( m_outData-> o_channel_type == VLQ_Enums::MUMU )     && ( trigMETPass && m_outData -> o_AO_met -> Pt() > 200 );
    isElMuChannelHMET     = ( m_outData-> o_channel_type == VLQ_Enums::ELMU )     && ( trigMETPass && m_outData -> o_AO_met -> Pt() > 200 );
  }

  bool is0LeptonChannel   = ( m_outData-> o_channel_type == VLQ_Enums::FULLHAD )  && ( trigMETPass && m_outData -> o_AO_met -> Pt() > 200 );

  if( isElectronChannel || isElectronChannelHMET ){
    m_outData-> o_channel_type = VLQ_Enums::ELECTRON;
  } 
  else if( isMuonChannel || isMuonChannelHMET ){
    m_outData-> o_channel_type = VLQ_Enums::MUON;
  } 
  else if( isElElChannel || isElElChannelHMET ){
    m_outData-> o_channel_type = VLQ_Enums::ELEL;
  } 
  else if( isMuMuChannel || isMuMuChannelHMET ){
    m_outData-> o_channel_type = VLQ_Enums::MUMU;
  } 
  else if( isElMuChannel || isElMuChannelHMET ){
    m_outData-> o_channel_type = VLQ_Enums::ELMU;
  } 
  else if( is0LeptonChannel ){
    m_outData-> o_channel_type = VLQ_Enums::FULLHAD;
  } 
  else {
    m_outData-> o_channel_type = VLQ_Enums::UNDEFINED;
  }

  //###########################################################
  //                                                          #
  // C4: MET/MTW cut to reject QCD in the 1L channel          #
  //                                                          #
  //###########################################################
  if( m_opt -> ApplyMetMtwCuts() && ( m_outData-> o_channel_type == VLQ_Enums::ELECTRON || m_outData-> o_channel_type == VLQ_Enums::MUON ) ){
    bool lowMETMTW = ( m_outData -> o_AO_met -> Pt() < 20 ) || ( (m_outData -> o_AO_met -> Pt() + m_varComputer -> GetMTw( *(m_outData->o_el), *(m_outData->o_mu), m_outData -> o_AO_met )) < 60 );
    if( (!m_opt -> InvertMetMtwCuts() && lowMETMTW) || (m_opt -> InvertMetMtwCuts() && !lowMETMTW) ){
      m_outData -> o_rejectEvent |= 1 << VLQ_Enums::METMTW_REJECTED;
    }
  }

  //###########################################################
  //                                                          #
  // C4': dPhiCut to reject QCD in the 0L channel             #
  //                                                          #
  //###########################################################
  if( m_opt -> ApplyDeltaPhiCut() && m_outData-> o_channel_type == VLQ_Enums::FULLHAD ){

    double dPhiMin = TMath::Abs( m_varComputer -> GetMindPhi( m_outData->o_AO_met, *(m_outData->o_jets), 4 ) );

    if ( !( (dPhiMin > m_opt->MinDeltaPhiCut()) && (  (m_opt->MaxDeltaPhiCut() < 0.) || (dPhiMin <= m_opt->MaxDeltaPhiCut()) ) ) ){
      m_outData -> o_rejectEvent |= 1 << VLQ_Enums::DPHI_REJECTED;
    }

  }

  //###########################################################
  //                                                          #
  // C5: Jet requirement                                      #
  //                                                          #
  //###########################################################
  int min_nj = 0;
  if(m_outData-> o_channel_type==VLQ_Enums::FULLHAD){
    min_nj = m_opt->DoLowJRegions() ? 5 : 6;
  }
  else if ( m_opt->DoSingleVLQRegions() ){
    min_nj = 3;
  }
  else{
    min_nj = m_opt->DoLowJRegions() ? 4 : 5;
  }

  if( (m_outData -> o_jets_n < min_nj) ){
    m_outData -> o_rejectEvent |= 1 << VLQ_Enums::JETS_REJECTED;
  }
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After jets selection" << std::endl;


  //###########################################################
  //                                                          #
  // C6: BJet requirement                                     #
  //                                                          #
  //###########################################################
  int min_nbj = 0;
  if(m_opt->DoTRF() || m_opt->DoLowBRegions()) min_nbj = 0;
  else if(m_opt->DoSingleVLQRegions()) min_nbj = 1;
  else  min_nbj = 2;

  int nbj = (m_opt->BtagCollection() == VLQ_Options::TRACK) ? m_outData -> o_trkbjets_n : m_outData -> o_bjets_n ;
  if( (nbj < min_nbj) ){
    m_outData -> o_rejectEvent |= 1 << VLQ_Enums::BJETS_REJECTED;
  }
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After b-jets selection" << std::endl;


  //###########################################################
  //                                                          #
  // C7: Meff requirement                                     #
  //                                                          #
  //###########################################################
  double temp_meff = m_varComputer->GetMeff(*(m_outData->o_jets),*(m_outData->o_el),*(m_outData->o_mu),m_outData->o_AO_met);
  if( (temp_meff < m_opt->MinMeffCut()) || (m_opt->MaxMeffCut() > 0. && temp_meff > m_opt->MaxMeffCut())  ){
    m_outData -> o_rejectEvent |= 1 << VLQ_Enums::MEFF_REJECTED;
  }

  //###########################################################
  //                                                          #
  // C8: Additional Met cut/veto in 1-lep channel             #
  //                                                          #
  //###########################################################
  if( ( m_outData-> o_channel_type == VLQ_Enums::ELECTRON || m_outData-> o_channel_type == VLQ_Enums::MUON ) && 
      ( (m_outData -> o_AO_met->Pt() < m_opt->MinMetCutOneLep()) 
  || (m_opt->MaxMetCutOneLep() > 0. && m_outData -> o_AO_met->Pt() > m_opt->MaxMetCutOneLep()) ) ){ 
    m_outData -> o_rejectEvent |= 1 << VLQ_Enums::METONELEP_REJECTED;
  }

  //###########################################################
  //                                                          #
  // C9: Additional Met cut/veto in 0-lep channel             #
  //                                                          #
  //###########################################################
  if( (m_outData-> o_channel_type == VLQ_Enums::FULLHAD) &&
      ( (m_outData -> o_AO_met->Pt() < m_opt->MinMetCutZeroLep()) 
  || (m_opt->MaxMetCutZeroLep() > 0. && m_outData -> o_AO_met->Pt() > m_opt->MaxMetCutZeroLep()) ) ){ 
    m_outData -> o_rejectEvent |= 1 << VLQ_Enums::METZEROLEP_REJECTED;
  }


  if( (m_outData-> o_channel_type == VLQ_Enums::ELEL || m_outData-> o_channel_type == VLQ_Enums::MUMU || 
       m_outData-> o_channel_type == VLQ_Enums::ELMU) && 
      (m_opt->MaxMetCutTwoLep() > 0. && m_outData -> o_AO_met->Pt() > m_opt->MaxMetCutTwoLep())){

    m_outData -> o_rejectEvent |= 1 << VLQ_Enums::METTWOLEP_REJECTED;

  }

  //###########################################################
  //                                                          #
  // Reject the events not passing the pre-selection          #
  //                                                          #
  //###########################################################
  if( !m_opt -> DoCutFlow() && m_outData -> o_rejectEvent ){
    return false;
  }

  //###########################################################
  //                                                          #
  // Compute all variables                                    #
  //                                                          #
  //###########################################################
  m_anaTools -> ComputeAllVariables();

  //###########################################################
  //                                                          #
  // Truth information recovery                               #
  //                                                          #
  //###########################################################
  if( !(m_opt -> IsData() || (m_opt -> StrSampleName().find("QCD") != std::string::npos)) && m_truthMngr ){

    //m_outData -> o_VLQtype = m_truthMngr -> GetVLQDecayType();

    //TEMPORARY HACK
    m_truthMngr -> Initialize();
    m_truthMngr -> FillParticlesPartonsVectors();
    if( m_outData -> o_truth_partons_n.at("VLQ_Ht") == 2 ) m_outData -> o_VLQtype = VLQ_Enums::HtHt;
    else if( (m_outData -> o_truth_partons_n.at("VLQ_Ht") == 1) && (m_outData -> o_truth_partons_n.at("VLQ_Wb") == 1) ) m_outData -> o_VLQtype = VLQ_Enums::HtWb;
    else if( m_outData -> o_truth_partons_n.at("VLQ_Wb") == 2 ) m_outData -> o_VLQtype = VLQ_Enums::WbWb;
    else if( (m_outData -> o_truth_partons_n.at("VLQ_Ht") == 1) && (m_outData -> o_truth_partons_n.at("VLQ_Zt") == 1) ) m_outData -> o_VLQtype = VLQ_Enums::HtZt;
    else if( (m_outData -> o_truth_partons_n.at("VLQ_Wb") == 1) && (m_outData -> o_truth_partons_n.at("VLQ_Zt") == 1) ) m_outData -> o_VLQtype = VLQ_Enums::WbZt;
    else if( m_outData -> o_truth_partons_n.at("VLQ_Zt") == 2 ) m_outData -> o_VLQtype = VLQ_Enums::ZtZt;

    m_outMngrHist -> HistMngr() -> FillTH1D( "vlqType", m_outData->o_VLQtype, m_outData -> o_eventWeight_Nom );
    //&& m_truthMngr -> Initialize()

    if(m_opt -> DoTruthAnalysis()){
      //TEMPORARY HACK
      //m_truthMngr -> Initialize();
      //m_truthMngr -> FillParticlesPartonsVectors();
      m_truthMngr -> CalculateTruthVariables();
      
      //
      // Truth-matched fat jet studies
      
      //
      // Match required partons to fat jets
      //
      std::vector<std::string> heavy_list = {};
      if( m_opt -> SampleName() == SampleName::VLQ ){ heavy_list = { "leptop", "hadtop", "hadtop_W", "Hbb", "Hnonbb", "hadW", "hadZ" }; }
      if( m_outData -> o_is_ttbar ){ heavy_list = {"leptop", "hadtop", "hadtop_W"}; }
      
      for( const std::string & restype : heavy_list ){
	AOVector* rescol = (m_outData -> o_truth_partons).at(restype);
	if(!rescol){
	  std::cerr << " WARNING => VLQ_Analysis_Data2015 : could not find parton collection " << restype << std::endl;
	  continue;
	}
	for( AnalysisObject* parton : *rescol ){
	  m_truthMngr -> MatchPartonToFatJets(*(m_outData -> o_rcjets), parton, 0.75 /*maxDR*/, true /*rcCollection*/, false /*tagCollection*/, 
					      true /*containment*/, 1.0 /*drcontained*/, true /*checkPt*/); 
	  
	  //===== Fill truth matched RC jets map =====
	  for( AnalysisObject* rcjet : *(m_outData -> o_rcjets) ){
	    
	    bool isTop = rcjet->M() > 140;
	    bool isHiggs = rcjet->M() > 105 && rcjet->M() < 140;
	    bool isV = rcjet->M() > 70 && rcjet->M() < 105;
	    
	    if( rcjet -> GetMoment("pdgId_truth") == 6 ){
	      m_outData -> o_rcjets_truthmatched.at("truthTop") -> push_back(rcjet);
	      if( isTop ){
		m_outData -> o_rcjets_truthmatched.at("truthTop_inMassWindow") -> push_back(rcjet);
	      }
	    }
	    if( rcjet -> GetMoment("pdgId_truth") == 23 ){
	      m_outData -> o_rcjets_truthmatched.at("truthZ") -> push_back(rcjet);
	      if( isV ){
		m_outData -> o_rcjets_truthmatched.at("truthZ_inMassWindow") -> push_back(rcjet);
	      }
	    }
	    if( rcjet -> GetMoment("pdgId_truth") == 24 ){
	      m_outData -> o_rcjets_truthmatched.at("truthW") -> push_back(rcjet);
	      if( isV ){
		m_outData -> o_rcjets_truthmatched.at("truthW_inMassWindow") -> push_back(rcjet);
	      }
	    }
	    if( rcjet -> GetMoment("pdgId_truth") == 25 ){
	      m_outData -> o_rcjets_truthmatched.at("truthHiggs") -> push_back(rcjet);
	      if( isHiggs ){
		m_outData -> o_rcjets_truthmatched.at("truthHiggs_inMassWindow") -> push_back(rcjet);
	      }
	    }
	    if( rcjet -> GetMoment("pdgId_truth") == 0 ){
	      m_outData -> o_rcjets_truthmatched.at("truthOther") -> push_back(rcjet);
	    }
	  }
	  m_outData -> o_rcjets_truthmatched_n.at("truthTop")   = m_outData -> o_rcjets_truthmatched.at("truthTop")   -> size();
	  m_outData -> o_rcjets_truthmatched_n.at("truthZ")     = m_outData -> o_rcjets_truthmatched.at("truthZ")     -> size();
	  m_outData -> o_rcjets_truthmatched_n.at("truthW")     = m_outData -> o_rcjets_truthmatched.at("truthW")     -> size();
	  m_outData -> o_rcjets_truthmatched_n.at("truthHiggs") = m_outData -> o_rcjets_truthmatched.at("truthHiggs") -> size();
	  m_outData -> o_rcjets_truthmatched_n.at("truthOther") = m_outData -> o_rcjets_truthmatched.at("truthOther") -> size();
	  
	  m_outData -> o_rcjets_truthmatched_n.at("truthTop_inMassWindow")   = m_outData -> o_rcjets_truthmatched.at("truthTop_inMassWindow")   -> size();
	  m_outData -> o_rcjets_truthmatched_n.at("truthZ_inMassWindow")     = m_outData -> o_rcjets_truthmatched.at("truthZ_inMassWindow")     -> size();
	  m_outData -> o_rcjets_truthmatched_n.at("truthW_inMassWindow")     = m_outData -> o_rcjets_truthmatched.at("truthW_inMassWindow")     -> size();
	  m_outData -> o_rcjets_truthmatched_n.at("truthHiggs_inMassWindow") = m_outData -> o_rcjets_truthmatched.at("truthHiggs_inMassWindow") -> size();
	  
	}//each parton in collection
	
      }//each collection of heavy resonance
      
      
      if( m_opt -> SampleName() == SampleName::VLQ ){
	for ( const std::string decayType : {"Ht", "Zt", "Wb"} ){
	  
	  AOVector& recoVLQCollection = *(m_outData -> o_recoVLQ.at(decayType));
	  for( AnalysisObject* partonVLQ : *(m_outData -> o_truth_partons.at("VLQ_"+decayType)) ){
	    m_truthMngr -> MatchPartonToFatJets( recoVLQCollection, partonVLQ, 1. /*maxDR*/, false /*rcCollection*/, false /*tagCollection*/, 
						 false /*containment*/, 1.0 /*drcontained*/, true /*checkPt*/ );
	  }
	  
	}
      }// VLQ samples
      
      
      //
      // Match leptonic W parton to reconstructed leptonic W
      //
      AOVector truth_lepW = {};
      truth_lepW.insert( truth_lepW.end(), m_outData -> o_truth_partons.at("leptop_W")->begin(), m_outData -> o_truth_partons.at("leptop_W")->end());
      if( m_opt -> SampleName() == SampleName::VLQ ){
	truth_lepW.insert( truth_lepW.end(), m_outData -> o_truth_partons.at("lepW")->begin(), m_outData -> o_truth_partons.at("lepW")->end());
      }
      for( AnalysisObject* obj : truth_lepW ){ obj -> SetMoment("nmatch_reco", 0); }
      if( m_outData -> o_lepW ){
	double drmatch_truth_lepW = 100.; //Take all lepW
	AnalysisObject* truth_lepW_match = m_varComputer -> GetClosestAO( m_outData->o_lepW, truth_lepW, drmatch_truth_lepW );
	if(truth_lepW_match){
	  
	  double dr_truthreco_lepW = truth_lepW_match->DeltaR( *(m_outData->o_lepW) ); 
	  if(dr_truthreco_lepW < 0.75){
	    truth_lepW_match->SetMoment("nmatch_reco",1);
	    m_outData->o_lepW->SetMoment("nmatch_truth",1);
	  }
	  else{
	    truth_lepW_match->SetMoment("nmatch_reco",0);
	    m_outData->o_lepW->SetMoment("nmatch_truth",0);
	  }
	  
	  truth_lepW_match->SetMoment("pT_reco", (m_outData->o_lepW)->Pt());
	  truth_lepW_match->SetMoment("fpT_reco", (m_outData->o_lepW)->Pt()/truth_lepW_match->Pt());
	  truth_lepW_match->SetMoment("m_reco", (m_outData->o_lepW)->M());
	  truth_lepW_match->SetMoment("dR_reco", dr_truthreco_lepW);
	  
	  
	  m_outData->o_lepW->SetMoment("pT_truth", truth_lepW_match->Pt());
	  m_outData->o_lepW->SetMoment("fpT_truth", (m_outData->o_lepW)->Pt()/truth_lepW_match->Pt());
	  m_outData->o_lepW->SetMoment("dR_truth", dr_truthreco_lepW);
	}
	else{
	  m_outData->o_lepW->SetMoment("nmatch_truth",0);
	}
      }
      
      //
      // Match leptonic top parton to reconstructed leptonic top
      //
      for( AnalysisObject* obj : *(m_outData -> o_truth_partons.at("leptop")) ){ 
	obj -> SetMoment("nmatch_reco", 0);
	obj -> SetMoment("nmatch_reco_leptop", 0);
      }
      if( m_outData -> o_leptop ){
	double drmatch_truth_leptop = 100.; //Take all leptop
	AnalysisObject* truth_leptop_match = m_varComputer -> 
	  GetClosestAO( m_outData->o_leptop, *(m_outData -> o_truth_partons.at("leptop")), drmatch_truth_leptop );
	if(truth_leptop_match){
	  
	  double dr_truthreco_leptop = truth_leptop_match->DeltaR( *(m_outData->o_leptop) ); 
	  if(dr_truthreco_leptop < 0.75){
	    truth_leptop_match->SetMoment("nmatch_reco_leptop",1);
	    m_outData->o_leptop->SetMoment("nmatch_truth",1);
	  }
	  else{
	    truth_leptop_match->SetMoment("nmatch_reco_leptop",0);
	    m_outData->o_leptop->SetMoment("nmatch_truth",0);
	  }
	  
	  //truth_leptop_match->SetMoment("nmatch_reco_leptop",1);
	  truth_leptop_match->SetMoment("pT_reco_leptop", (m_outData->o_leptop)->Pt());
	  truth_leptop_match->SetMoment("fpT_reco_leptop", (m_outData->o_leptop)->Pt()/truth_leptop_match->Pt());
	  truth_leptop_match->SetMoment("m_reco_leptop", (m_outData->o_leptop)->M());
	  truth_leptop_match->SetMoment("dR_reco_leptop", dr_truthreco_leptop);
	  
	  //m_outData->o_leptop->SetMoment("nmatch_truth",1);
	  m_outData->o_leptop->SetMoment("pT_truth", truth_leptop_match->Pt());
	  m_outData->o_leptop->SetMoment("fpT_truth", (m_outData->o_leptop)->Pt()/truth_leptop_match->Pt());
	  m_outData->o_leptop->SetMoment("dR_truth", dr_truthreco_leptop);
	}
	else{
	  m_outData->o_leptop->SetMoment("nmatch_truth",0);
	}
      }
      

      //
      // Match small-R jets to b-quarks
      //
      m_truthMngr -> MatchJetsToBQuarks( *(m_outData -> o_jets) );



    }// if doing truth analysis

  }//if MC

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After truth handling" << std::endl;

  //###########################################################
  //                                                          #
  // Setting weights in WeightManager                         #
  //                                                          #
  //###########################################################
  if ( !(m_opt -> IsData() || (m_opt -> StrSampleName().find("QCD") != std::string::npos)) ) {
    m_weightMngr -> SetCrossSectionWeight();
    const bool use_met_trigger = isElectronChannelHMET || isMuonChannelHMET || isElElChannelHMET || isMuMuChannelHMET || isElMuChannelHMET;
    m_weightMngr -> SetLeptonSFWeights( !use_met_trigger );

    if( m_opt ->ReweightKinematics() ){
      m_weightMngr -> SetKinReweightings();
    }
    if( m_outData -> o_is_ttbar ){
      if(m_opt->RecomputeTtbarNNLOCorrection() && m_truthMngr){
        m_weightMngr -> SetNNLOWeight(m_truthMngr -> GetTopPt());
      }
      if( m_opt -> ComputeWeightSys() ){
        m_weightMngr -> SetTtccWeights();
        m_weightMngr -> SetTtbarPMGWeights();
        if( m_opt -> ApplyTtbarNNLOCorrection() ){
          m_weightMngr -> SetNNLOSystematic();
        }
      }
      m_weightMngr -> SetTtbarFractionWeight();
    }
    if( m_outData -> o_is_ttbar || (m_opt -> StrSampleName().find("SINGLETOP") != std::string::npos) 
	|| (m_opt -> StrSampleName().find("WJETS") != std::string::npos) || (m_opt -> StrSampleName().find("ZJETS") != std::string::npos) ){
      m_weightMngr -> SetPMGSystWeights();
    if((m_opt -> ReweightKinematics()) && (m_opt -> ComputeWeightSys())){
      m_weightMngr -> UpdateSysReweighting();
    }
  } else if (m_opt -> StrSampleName().find("QCD") != std::string::npos){
    m_weightMngr -> SetQCDWeight();
  }

  //###########################################################
  //                                                          #
  // TRF                                                      #
  //                                                          #
  //###########################################################
  if( !(m_opt -> IsData() || (m_opt -> StrSampleName().find("QCD") != std::string::npos)) && ( m_opt->RecomputeBtagSF() || ( m_opt -> DoTRF() && m_opt->RecomputeTRF() ) ) ){
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> Evaluating TRF with TRF Manager" << std::endl;
    m_TRFMngr->ComputeTRFWeights();
    if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After TRF manager" << std::endl;
  }

  //###########################################################
  //                                                          #
  // Compute all weights                                      #
  //                                                          #
  //###########################################################
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> Before setting the weight " << entry << std::endl;
  m_weightMngr -> ComputeAllWeights();
  if( m_opt->ReweightKinematics() && !m_opt->ComputeWeightSys() ){
    m_weightMngr -> ComputeSystematicWeights();
  }

  if(m_opt -> MsgLevel() == Debug::DEBUG) m_weightMngr -> Print( true ); 

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After setting the weight " << entry << std::endl;

  //###########################################################
  //                                                          #
  // Cut flow procedure                                       #
  //                                                          #
  //###########################################################
  if( m_opt -> DoCutFlow() ){
    struct {
      void operator()(HistManager *hm, const string &hn, const TBits &passBits, const double weight, const unsigned int sequential) {
        for(size_t i=0; i<passBits.GetNbits(); ++i){
          if(passBits.TestBitNumber(i)){
            hm->FillTH1D(hn, double(i), weight);
          } else if (i<=sequential) {
            break;
          }
        }
      };
    } fillCutFlow;
    std::vector < std::string > v_channels = {"", "_e", "_mu"};
    HistManager *hm = m_outMngrHist->HistMngr();
    for ( const std::string ch : v_channels ){
      int step_counter = 0;
      const int n_steps_cutflow_1l = 12;
      const int reject_mask = m_outData->o_rejectEvent;
      TBits pass_bits_1l(n_steps_cutflow_1l);
      pass_bits_1l.SetBitNumber(step_counter++, true); // initial
      pass_bits_1l.SetBitNumber(step_counter++,!(reject_mask & 1<<VLQ_Enums::TRIGGER_REJECTED) &&((ch=="_e" && trigElecPass)||(ch=="_mu" && trigMuonPass)||(ch=="" && (trigElecPass || trigMuonPass))));
      pass_bits_1l.SetBitNumber(step_counter++,!(reject_mask & 1<<VLQ_Enums::LEPTON_REJECTED) &&((ch=="_e" && isElectronChannel)||(ch=="_mu" && isMuonChannel)||(ch=="" && (isElectronChannel || isMuonChannel))));
      pass_bits_1l.SetBitNumber(step_counter++, !(reject_mask & 1<<VLQ_Enums::TRIGGERMATCH_REJECTED));
      pass_bits_1l.SetBitNumber(step_counter++, m_outData->o_jets_n >= 5);
      pass_bits_1l.SetBitNumber(step_counter++, m_anaTools->PassBTagRequirement(2, true));
      pass_bits_1l.SetBitNumber(step_counter++, m_outData->o_AO_met->Pt() > 20);
      pass_bits_1l.SetBitNumber(step_counter++, (m_outData->o_AO_met->Pt()+m_varComputer->GetMTw(*(m_outData->o_el),*(m_outData->o_mu),m_outData->o_AO_met)) > 60);
      //parallel cuts
      pass_bits_1l.SetBitNumber(step_counter++, m_outData->o_taggedjets_n.at("RCMTop")>=1);
      pass_bits_1l.SetBitNumber(step_counter++, m_outData->o_taggedjets_n.at("RCMHiggs")>=1);
      pass_bits_1l.SetBitNumber(step_counter++, (m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_taggedjets_n.at("RCMHiggs")>=2));
      pass_bits_1l.SetBitNumber(step_counter++, m_varComputer->GetMeff(*(m_outData->o_jets),*(m_outData->o_el),*(m_outData->o_mu),m_outData->o_AO_met)>400);
      fillCutFlow(hm, "cutFlow_unWeight"+ch, pass_bits_1l, 1.0, 7);
      fillCutFlow(hm, "cutFlow_weight_2"+ch, pass_bits_1l, m_outData->o_eventWeight_Nom, 7);

      const int n_steps_cutflow_0l = 8;
      step_counter = 0;
      TBits pass_bits_0l(n_steps_cutflow_0l);
      pass_bits_0l.SetBitNumber(step_counter++, true); // initial
      pass_bits_0l.SetBitNumber(step_counter++, !(reject_mask & 1<<VLQ_Enums::TRIGGER_REJECTED) && trigMETPass);
      pass_bits_0l.SetBitNumber(step_counter++, !lepton);
      pass_bits_0l.SetBitNumber(step_counter++, m_outData-> o_channel_type == VLQ_Enums::FULLHAD);
      pass_bits_0l.SetBitNumber(step_counter++,TMath::Abs(m_varComputer->GetMindPhi(m_outData->o_AO_met,*(m_outData->o_jets), 4)) > 0.4);
      pass_bits_0l.SetBitNumber(step_counter++, m_outData->o_jets_n >= 6);
      pass_bits_0l.SetBitNumber(step_counter++, m_anaTools->PassBTagRequirement(2, true));
      pass_bits_0l.SetBitNumber(step_counter++,m_varComputer->GetMeff(*(m_outData->o_jets),*(m_outData->o_el),*(m_outData->o_mu),m_outData->o_AO_met) > 400);
      fillCutFlow(hm, "cutFlow0L_unWeight", pass_bits_0l, 1.0, 7);
      fillCutFlow(hm, "cutFlow0L_weight_2", pass_bits_0l, m_outData->o_eventWeight_Nom, 7);
    } // for(ch)
  } // if(DoCutFlow)


  //////////////////////////////////////////////////////////
  //                                                      //
  // NOW DOING THE ACTUAL SELECTION                       //
  //                                                      //
  //////////////////////////////////////////////////////////

  if( ! m_outData -> o_rejectEvent ){

    //============================================ RUN SELECTION CHAIN HERE ======================================
    m_selector->RunSelectionChain();
    if(m_opt->DumpTree() && ( (m_outData -> o_jets_n >= 4) ) ){ // && (m_outData -> o_bjets_n >= 2) ) ){
      m_outMngrTree->FillStandardTree("tree");
    }

    if(m_opt->DumpOverlapTree()){
      m_outData -> o_region -> clear();
      for( const std::pair<int, Selection*> &sel : *(m_selector->GetSelectionTree()) ){
        if((sel.second)->Decision()){
          m_outData -> o_region -> push_back((sel.second)->SelecInd());
        }
      }
      if(m_outData -> o_region -> size()){
        m_outMngrOvlerlapTree->FillStandardTree("overlap");
      }
    }
  }//selected events

  //
  // Fills the histogram to remember about the rejection causes for events
  //
  if( m_outData -> o_rejectEvent & 1<<VLQ_Enums::TRIGGER_REJECTED ){
    m_outMngrHist -> HistMngr() -> FillTH1D( "rejectionMask", VLQ_Enums::TRIGGER_REJECTED, 1. );
  }
  if( m_outData -> o_rejectEvent & 1<<VLQ_Enums::BADJET_REJECTED ){
    m_outMngrHist -> HistMngr() -> FillTH1D( "rejectionMask", VLQ_Enums::BADJET_REJECTED, 1. );
  }
  if( m_outData -> o_rejectEvent & 1<<VLQ_Enums::LEPTON_REJECTED ){
    m_outMngrHist -> HistMngr() -> FillTH1D( "rejectionMask", VLQ_Enums::LEPTON_REJECTED, 1. );
  }
  if( m_outData -> o_rejectEvent & 1<<VLQ_Enums::TRIGGERMATCH_REJECTED ){
    m_outMngrHist -> HistMngr() -> FillTH1D( "rejectionMask", VLQ_Enums::TRIGGERMATCH_REJECTED, 1. );
  }
  if( m_outData -> o_rejectEvent & 1<<VLQ_Enums::JETS_REJECTED ){
    m_outMngrHist -> HistMngr() -> FillTH1D( "rejectionMask", VLQ_Enums::JETS_REJECTED, 1. );
  }
  if( m_outData -> o_rejectEvent & 1<<VLQ_Enums::METMTW_REJECTED ){
    m_outMngrHist -> HistMngr() -> FillTH1D( "rejectionMask", VLQ_Enums::METMTW_REJECTED, 1. );
  }
  if( m_outData -> o_rejectEvent & 1<<VLQ_Enums::DPHI_REJECTED ){
    m_outMngrHist -> HistMngr() -> FillTH1D( "rejectionMask", VLQ_Enums::DPHI_REJECTED, 1. );
  }
  if( !m_outData -> o_rejectEvent ){
    m_outMngrHist -> HistMngr() -> FillTH1D( "rejectionMask", 0, 1. );
  }
  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "== End of processing of entry : " << entry << std::endl;

  return true;
}

//____________________________________________________________________________
//
bool VLQ_Analysis_Data2015::Terminate()
{

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "-> Entering in Terminate function." << std::endl;
  cout<<"Yield summary: "<<endl
  <<summaryYields()
  <<endl;

  //
  // Reweighting histograms with flat scales
  //
  if( m_outData -> o_is_ttbar ){

    for(auto histname : m_outMngrHist->HistMngr()->GetTH1KeyList()){
      TH1D* histo = m_outMngrHist->HistMngr()->GetTH1D(histname);
    
      // Scaling ttbar HT slices (nominal and PMG weights)
      if( m_opt->ScaleTtbarHtSlices() ){
          double Scale = 1.0;

        if ( m_opt -> StrSampleID().find("407344.") != std::string::npos ){
          Scale = 1./0.99860961239196;
          if ( m_opt -> ComputeWeightSys() ){
            if (histname.find("weight_pmg_Var3cDown") != std::string::npos) Scale = 1./1.00989117643996;
            else if (histname.find("weight_pmg_Var3cUp") != std::string::npos) Scale = 1./0.98630942849818;
            else if (histname.find("weight_pmg_isr_muRfac10__fsr_muRfac05") != std::string::npos) Scale = 1./1.01244857328796;
            else if (histname.find("weight_pmg_isr_muRfac10__fsr_muRfac20") != std::string::npos) Scale = 1./0.98661128202278;
            else if (histname.find("weight_pmg_muR05__muF10") != std::string::npos) Scale = 1./0.96762126478968;
            else if (histname.find("weight_pmg_muR10__muF05") != std::string::npos) Scale = 1./0.97391612230213;
            else if (histname.find("weight_pmg_muR10__muF20") != std::string::npos) Scale = 1./1.02221909345704;
            else if (histname.find("weight_pmg_muR20__muF10") != std::string::npos) Scale = 1./1.02018541245750;
          }
        }
        if ( m_opt -> StrSampleID().find("407343.") != std::string::npos ){
          Scale = 1./1.00220071443736;
          if ( m_opt -> ComputeWeightSys() ){
            if (histname.find("weight_pmg_Var3cDown") != std::string::npos) Scale = 1./1.02075306498765;
            else if (histname.find("weight_pmg_Var3cUp") != std::string::npos) Scale = 1./0.98196927321205;
            else if (histname.find("weight_pmg_isr_muRfac10__fsr_muRfac05") != std::string::npos) Scale = 1./1.03193538833724;
            else if (histname.find("weight_pmg_isr_muRfac10__fsr_muRfac20") != std::string::npos) Scale = 1./0.99087003186616;
            else if (histname.find("weight_pmg_muR05__muF10") != std::string::npos) Scale = 1./0.91951515882506;
            else if (histname.find("weight_pmg_muR10__muF05") != std::string::npos) Scale = 1./0.93819392248974;
            else if (histname.find("weight_pmg_muR10__muF20") != std::string::npos) Scale = 1./1.06487669659290;
            else if (histname.find("weight_pmg_muR20__muF10") != std::string::npos) Scale = 1./1.06891808020899;
          }
        }
        if ( m_opt -> StrSampleID().find("407342.") != std::string::npos ){
          Scale = 1./1.01614066637173;
          if ( m_opt -> ComputeWeightSys() ){
            if (histname.find("weight_pmg_Var3cDown") != std::string::npos) Scale = 1./1.04057257636042;
            else if (histname.find("weight_pmg_Var3cUp") != std::string::npos) Scale = 1./0.99097386133341;
            else if (histname.find("weight_pmg_isr_muRfac10__fsr_muRfac05") != std::string::npos) Scale = 1./1.06280582379557;
            else if (histname.find("weight_pmg_isr_muRfac10__fsr_muRfac20") != std::string::npos) Scale = 1./1.00957616786319;
            else if (histname.find("weight_pmg_muR05__muF10") != std::string::npos) Scale = 1./0.89300718054136;
            else if (histname.find("weight_pmg_muR10__muF05") != std::string::npos) Scale = 1./0.90726044083483;
            else if (histname.find("weight_pmg_muR10__muF20") != std::string::npos) Scale = 1./1.12725897511274;
            else if (histname.find("weight_pmg_muR20__muF10") != std::string::npos) Scale = 1./1.12572488013792;
          }
        }
        histo->Scale(Scale);
      }
    }
  }

  //
  // Writing outputs
  //
  std::string nameHist = m_opt->OutputFile();
  if(m_outMngrHist){

    if(m_opt->DoSumRegions()){
 
      SumAnalysisRegions(true);
      for( const std::pair<int, Selection*> &sel : *(m_selector->GetSelectionTree()) ){
	if(sel.second->PassFlagAtBit(VLQ_Selector::PRESEL)){
	  m_outMngrHist -> SaveStandardTH1(nameHist, false, AnalysisUtils::ReplaceString(sel.second->Name(),"-",""));
	}
      }
      
    }
    else{
      m_outMngrHist -> SaveStandardTH1(nameHist);
      m_outMngrHist -> SaveStandardTH2(nameHist,false);
    }

  }
  if(m_outMngrOvlerlapTree){
    m_outMngrOvlerlapTree -> SaveStandardTree(AnalysisUtils::ReplaceString(nameHist, ".root", "_OVTREE.root"));
  }
  if(m_outMngrTree){
    m_outMngrTree -> SaveStandardTree(AnalysisUtils::ReplaceString(nameHist, ".root", "_TREE.root"));
  }
  m_outData -> EmptyOutputData();
  delete m_outData;

  delete m_outMngrHist;
  delete m_outMngrTree;


  delete m_weightMngr;
  delete m_TRFMngr;
  delete m_selector;

  if(m_opt -> MsgLevel() == Debug::DEBUG) std::cout << "-> End of Terminate function." << std::endl;
  return true;
}

bool VLQ_Analysis_Data2015::SumAnalysisRegions(const bool newFile){
  
  struct{ 
    void operator()(TH1D* hist, TH1D** target, const std::string& name, double scale = 1.){
      if(*target != NULL){ (*target)->Add(hist, scale); }
      else{ 
	*target = (TH1D*)(hist->Clone(name.c_str()));
	(*target)->SetDirectory(0);
	(*target)->Scale(scale);
      } 
      
      return;
    };
  } AddHistogramToTarget;


  std::vector<std::string> regList{};
  std::map<std::string, std::vector<std::string> > targetPrintList{};
  
  if( m_opt->DoOneLeptonAna() ){
    
    if( m_opt->DoFitRegions() ){
      std::vector<std::string> regList_1L_fit = {
	"c1lep0Tex0Hex6jin2bex",
	"c1lep0Tex0Hex6jin3bex",
	"c1lep0Tex0Hex6jin4bin",
	"c1lep0Tex1Hex6jin2bex",
	"c1lep0Tex1Hex6jin3bex",
	"c1lep0Tex1Hex6jin4bin",
	"c1lep1Tex0Hex6jin2bex",
	"c1lep1Tex0Hex6jin3bex",
	"c1lep1Tex0Hex6jin4bin",
	"c1lep1Tex1Hex6jin2bex",
	"c1lep1Tex1Hex6jin3bex",
	"c1lep1Tex1Hex6jin4bin",
	"c1lep2Tin0_1Hwin6jin2bex",
	"c1lep2Tin0_1Hwin6jin3bex",
	"c1lep2Tin0_1Hwin6jin4bin",
	"c1lep0Tin2Hin6jin2bex",
	"c1lep0Tin2Hin6jin3bex",
	"c1lep0Tin2Hin6jin4bin"
      };
      regList.insert(regList.end(), regList_1L_fit.begin(), regList_1L_fit.end());
    }//fit
    
    if( m_opt->DoValidnRegions() ){
      std::vector<std::string> regList_1L_valid = {
	"c1lep0Tex0Hex5jex2bex",
	"c1lep0Tex0Hex5jex3bex",
	"c1lep0Tex0Hex5jex4bin",
	"c1lep0Tex1Hex5jex2bex",
	"c1lep0Tex1Hex5jex3bex",
	"c1lep0Tex1Hex5jex4bin",
	"c1lep1Tex0Hex5jex2bex",
	"c1lep1Tex0Hex5jex3bex",
	"c1lep1Tex0Hex5jex4bin",
	"c1lep1Tex1Hex5jex2bex",
	"c1lep1Tex1Hex5jex3bex",
	"c1lep2Tin0_1Hwin5jex2bex",
	"c1lep2Tin0_1Hwin5jex3bex",
	"c1lep0Tin2Hin5jex2bex",
	"c1lep0Tin2Hin5jex3bex",
	"c1lep2THin5jex4bin"
      };
      regList.insert(regList.end(), regList_1L_valid.begin(), regList_1L_valid.end());
    }//valid
    

    std::map<std::string, std::vector<std::string> > targetPrintList_1L = {
      {"sum1lep5jin2bin", {}}, 
      {"sum1lep5jin3bin", {}}, 
      {"sum1lep6jin2bin", {}},
      {"sum1lep6jin3bin", {}},
      {"sum1lep1THin6jin3bin", {}}
    };
    
    targetPrintList.insert(targetPrintList_1L.begin(), targetPrintList_1L.end());
    
    
  }//1-lepton

  if( m_opt->DoZeroLeptonAna() ){

    if( m_opt->DoFitRegions() ){
      std::vector<std::string> regList_0L_fit = {
	"c0lep0Tex0Hex7jin2bexLowMtbmin",
	"c0lep0Tex0Hex7jin2bexHighMtbmin",
	"c0lep0Tex1Hex7jin2bexLowMtbmin",
	"c0lep0Tex1Hex7jin2bexHighMtbmin",
	"c0lep1Tex0Hex7jin2bexLowMtbmin",
	"c0lep1Tex0Hex7jin2bexHighMtbmin",
	"c0lep2THin7jin2bexLowMtbmin",
	"c0lep2THin7jin2bexHighMtbmin",
	"c0lep0Tex0Hex7jin3bexLowMtbmin",
	"c0lep0Tex0Hex7jin3bexHighMtbmin",
	"c0lep0Tex1Hex7jin3bexLowMtbmin",
	"c0lep0Tex1Hex7jin3bexHighMtbmin",
	"c0lep1Tex0Hex7jin3bexLowMtbmin",
	"c0lep1Tex0Hex7jin3bexHighMtbmin",
	"c0lep1Tex1Hex7jin3bexLowMtbmin",
	"c0lep1Tex1Hex7jin3bexHighMtbmin",
	"c0lep2Tin0_1Hwin7jin3bexLowMtbmin",
	"c0lep2Tin0_1Hwin7jin3bexHighMtbmin",
	"c0lep0Tin2Hin7jin3bex",
	"c0lep0Tex0Hex7jin4binLowMtbmin",
	"c0lep0Tex0Hex7jin4binHighMtbmin",
	"c0lep0Tex1Hex7jin4binLowMtbmin",
	"c0lep0Tex1Hex7jin4binHighMtbmin",
	"c0lep1Tex0Hex7jin4binLowMtbmin",
	"c0lep1Tex0Hex7jin4binHighMtbmin",
	"c0lep2THin7jin4bin"
      };
      regList.insert(regList.end(), regList_0L_fit.begin(), regList_0L_fit.end());
    }//fit

    if( m_opt->DoValidnRegions() ){
      std::vector<std::string> regList_0L_valid = {
	"c0lep0Tex0Hex6jex2bexLowMtbmin",
	"c0lep0Tex0Hex6jex2bexHighMtbmin",
	"c0lep0Tex1Hex6jex2bexLowMtbmin",
	"c0lep0Tex1Hex6jex2bexHighMtbmin",
	"c0lep1Tex0Hex6jex2bexLowMtbmin",
	"c0lep1Tex0Hex6jex2bexHighMtbmin",
	"c0lep2THin6jex2bexLowMtbmin",
	"c0lep2THin6jex2bexHighMtbmin",
	"c0lep0Tex0Hex6jex3bexLowMtbmin",
	"c0lep0Tex0Hex6jex3bexHighMtbmin",
	"c0lep0Tex1Hex6jex3bexLowMtbmin",
	"c0lep0Tex1Hex6jex3bexHighMtbmin",
	"c0lep1Tex0Hex6jex3bexLowMtbmin",
	"c0lep1Tex0Hex6jex3bexHighMtbmin",
	"c0lep2THin6jex3bex",
	"c0lep0Tex0Hex6jex4binLowMtbmin",
	"c0lep0Tex0Hex6jex4binHighMtbmin",
	"c0lep0Tex1Hex6jex4bin",
	"c0lep1Tex0Hex6jex4bin",
	"c0lep2THin6jex4bin"
      };
      regList.insert(regList.end(), regList_0L_valid.begin(), regList_0L_valid.end());
    }//valid

    std::map<std::string, std::vector<std::string> > targetPrintList_0L = {
      {"sum0lep6jin2bin", {}},  
      {"sum0lep7jin2bin", {}},  
      {"sum0lep6jin3bin", {}},  
      {"sum0lep7jin3bin", {}},  
      
      {"sum0lep1THin7jin2bin", {}}
    };
    
    targetPrintList.insert(targetPrintList_0L.begin(), targetPrintList_0L.end());

  }//0-lep
  
  /* 
     {"sum0lep6jin2bin", {}}, 
    {"sum0lep7jin2bin", {}},  
    {"sum0lep6jin3bin", {}},  
    {"sum0lep7jin3bin", {}},  
  */
    //{"sum0lep0Tex0Hex7jin2bin", {}},
    //{"sum0lep1THin7jin2bin", {}},
    //{"sum0lep2THin7jin2bin", {}},

    /*
    {"sum0lep0Tex0Hex6jex2bex", {}},
    {"sum0lep1THex6jex2bex", {}},
    {"sum0lep2THin6jex2bex", {}},

    {"sum0lep0Tex0Hex6jex3bex", {}},
    {"sum0lep1THex6jex3bex", {}},
    {"sum0lep2THin6jex3bex", {}},

    {"sum0lep0Tex0Hex6jex4bin", {}},
    {"sum0lep1THex6jex4bin", {}},
    {"sum0lep2THin6jex4bin", {}},

    {"sum0lep0Tex0Hex7jin2bex", {}},
    {"sum0lep1THex7jin2bex", {}},
    {"sum0lep2THin7jin2bex", {}},

    {"sum0lep0Tex0Hex7jin3bex", {}},
    {"sum0lep1THex7jin3bex", {}},
    {"sum0lep2THin7jin3bex", {}},

    {"sum0lep0Tex0Hex7jin4bin", {}},
    {"sum0lep1THex7jin4bin", {}},
    {"sum0lep2THin7jin4bin", {}},
    */
    //=====================================
  /*
    {"sum1lep5jin2bin", {}}, 
    {"sum1lep5jin3bin", {}}, 
    {"sum1lep6jin2bin", {}},
    {"sum1lep6jin3bin", {}},
  */
    //{"sum1lep0Tex0Hex6jin3bin", {}},
    //{"sum1lep1THin6jin3bin", {}}
    //{"sum1lep2THin6jin3bin", {}},
    /*
    {"sum1lep0Tex0Hex5jex3bex", {}},
    {"sum1lep1THex5jex3bex", {}},
    {"sum1lep2THin5jex3bex", {}},

    {"sum1lep0Tex0Hex5jex4bin", {}},
    {"sum1lep1THex5jex4bin", {}},
    {"sum1lep2THin5jex4bin", {}},

    {"sum1lep0Tex0Hex6jin3bex", {}},
    {"sum1lep1THex6jin3bex", {}},
    {"sum1lep2THin6jin3bex", {}},

    {"sum1lep0Tex0Hex6jin4bin", {}},
    {"sum1lep1THex6jin4bin", {}},
    {"sum1lep2THin6jin4bin", {}}
    */
  //};

  for(const std::string& region : regList){

    std::string boostcat = "";
    if(region.find("0Tex0Hex") != std::string::npos){ boostcat = "0Tex0Hex"; }
    else if( (region.find("1Tex0Hex") != std::string::npos) || (region.find("0Tex1Hex") != std::string::npos) ){ boostcat = "1THex"; }
    else if( (region.find("2THin") != std::string::npos) || (region.find("2Tin0_1Hwin") != std::string::npos) 
	     || (region.find("1Tex1Hex") != std::string::npos) || (region.find("0Tin2Hin") != std::string::npos) ){ boostcat = "2THin"; }


    //else 
    /*
      if( (region.find("1Tex0Hex") != std::string::npos) || (region.find("0Tex1Hex") != std::string::npos)
      || (region.find("2THin") != std::string::npos) || (region.find("2Tin0_1Hwin") != std::string::npos) 
      || (region.find("1Tex1Hex") != std::string::npos) || (region.find("0Tin2Hin") != std::string::npos) )
      { boostcat = "1THin"; }
    */

    std::string jetcat = "";
    if(region.find("5jex") != std::string::npos){ jetcat = "5jex"; }
    else if(region.find("6jex") != std::string::npos){ jetcat = "6jex"; }
    else if(region.find("5jin") != std::string::npos){ jetcat = "5jin"; }
    else if(region.find("6jin") != std::string::npos){ jetcat = "6jin"; }
    else if(region.find("7jin") != std::string::npos){ jetcat = "7jin"; }
    
    std::string bcat = "";
    if(region.find("2bex") != std::string::npos){ bcat = "2bex"; }
    else if(region.find("2bin") != std::string::npos){ bcat = "2bin"; }
    else if(region.find("3bex") != std::string::npos){ bcat = "3bex"; }
    else if(region.find("3bin") != std::string::npos){ bcat = "3bin"; }
    else if(region.find("4bin") != std::string::npos){ bcat = "4bin"; }

    if(region.find("0lep") != std::string::npos){
      //=========================== Preselections ==============================================
      targetPrintList.at("sum0lep6jin2bin").push_back(region);
      if(region.find("7jin") != std::string::npos){
	targetPrintList.at("sum0lep7jin2bin").push_back(region);
      }
      
      if( (region.find("3bex") != std::string::npos) || (region.find("4bin") != std::string::npos) ){
	targetPrintList.at("sum0lep6jin3bin").push_back(region);
	
	if(region.find("7jin") != std::string::npos){
	  targetPrintList.at("sum0lep7jin3bin").push_back(region);
	}//7jin3bin
	
      }//3bin

      //targetPrintList.at("sum0lep"+boostcat+"6jin2bin").push_back(region);
      if(region.find("7jin") != std::string::npos){
	if( !boostcat.empty() && (boostcat.find("0Tex0Hex") == std::string::npos) ){
	  targetPrintList.at("sum0lep1THin7jin2bin").push_back(region);
	}
      }
      //targetPrintList.at("sum0lep"+boostcat+jetcat+bcat).push_back(region);
      
    }//0L regions

    //===================== 1L REGIONS ========================================
    else if(region.find("1lep") != std::string::npos){
      targetPrintList.at("sum1lep5jin2bin").push_back(region);
      if(region.find("6jin") != std::string::npos){
	targetPrintList.at("sum1lep6jin2bin").push_back(region);
      }
      
      if( (region.find("3bex") != std::string::npos) || (region.find("4bin") != std::string::npos) ){
	targetPrintList.at("sum1lep5jin3bin").push_back(region);
	
	if(region.find("6jin") != std::string::npos){
	  targetPrintList.at("sum1lep6jin3bin").push_back(region);
	  if( !boostcat.empty() && (boostcat.find("0Tex0Hex") == std::string::npos) ){
	    targetPrintList.at("sum1lep1THin6jin3bin").push_back(region);
	  }
	  //targetPrintList.at("sum1lep"+boostcat+"6jin3bin").push_back(region);
	}//6jin3bin
	
	//targetPrintList.at("sum1lep"+boostcat+jetcat+bcat).push_back(region);
	
      }//3bin
      
    }//1L regions

  }

  if( m_opt->MsgLevel() == Debug::DEBUG ){
    //============== PRINT regions lists ===================
    for(std::pair<std::string, std::vector<std::string> > targetPair : targetPrintList){
      std::cout << " TARGET REGION : " <<  targetPair.first << " built from :: " <<std::endl;
      for(std::string source_reg : targetPair.second){
	std::cout<<source_reg<<std::endl;
      }
      std::cout<<std::endl<<std::endl;
    }
  }

  std::vector<std::string> lepchannels{""}; 
  if(m_opt->DoSplitEMu()){
    lepchannels.push_back("_el"); 
    lepchannels.push_back("_mu");
  }
  
  std::vector<std::string> wgtList{""};
  for(std::pair<std::string, WeightObject*> sysPair : *(m_weightMngr->SystMap()) ){
    //with the exception of QCD1L
    wgtList.push_back("_"+sysPair.first);
  }

  std::map<std::string, std::string> boostcat_systs = { 
    {"0T0H","0Tex0Hex"},{"1T0H","1Tex0Hex"},{"0T1H","0Tex1Hex"},
    {"1T1H","1Tex1Hex"},{"2T0_1H","2Tin0_1Hwin"},{"0T2H","0Tin2Hin"},{"2TH","2THin"}
  };


  bool is_Vjets = ( (m_opt -> StrSampleName().find("WJETS") != std::string::npos) || (m_opt -> StrSampleName().find("ZJETS") != std::string::npos) );
  bool is_VjetsHF = is_Vjets && ( (m_opt -> StrSampleName().find("CHARM") != std::string::npos) || (m_opt -> StrSampleName().find("BEAUTY") != std::string::npos) );

  bool is_Diboson = m_opt->StrSampleName().find("DIBOSONS") != std::string::npos;
  bool is_singletop = m_opt->StrSampleName().find("SINGLETOP") != std::string::npos;
  bool is_QCD0L = m_opt->MakeQCD0LSystematics(); //StrSampleName().find("QCD0L") != std::string::npos;
  bool is_QCD = m_opt->StrSampleName().find("QCD") != std::string::npos;

  //================ If single-top Wt then make DR/DS uncertainty ================
  if(is_singletop){
    VLQ_PropagateSingleTopSystematics* DRDSPropagator = new VLQ_PropagateSingleTopSystematics(m_opt, m_outMngrHist);
    DRDSPropagator->Init(m_selector -> GetSelectionTree(), std::getenv("VLQAnalysisFramework_DIR")+std::string("/data/VLQAnalysis/single_top_syst.root"));
    DRDSPropagator->WriteAllSystHistograms(false,false);
    delete DRDSPropagator;
  }
      
  std::string fileMode = newFile ? "RECREATE" : "UPDATE";
  TFile* outfile = TFile::Open( m_opt->OutputFile().c_str(), fileMode.c_str());
  
  for( std::pair< std::string, OutputHistManager::h1Def* > hpair : *(m_outMngrHist->StdTH1Def()) ){

    if( !hpair.second->hasSyst ){ continue; }

    const std::string& variable_base = hpair.first;

    for ( unsigned int i = 0 ; i <= ( ( m_opt -> SampleName() == SampleName::VLQ  && m_opt -> SplitVLQDecays() /* && split*/ ) ? 6 : 0 ) ; ++i) {
      std::string variable = "";
      if( i > 0 ){
	variable += "vlq";
	variable += std::to_string(i);
	variable += "_";
      }
      variable +=  variable_base;
      
      for(std::pair<std::string, std::vector<std::string> > targetPair : targetPrintList){
      
	if(is_QCD && targetPair.first.find("0lep") != std::string::npos){continue;}
	if(is_QCD0L && targetPair.first.find("1lep") != std::string::npos){continue;}

	for(const std::string& channel : lepchannels){
  
	  if(!channel.empty() && targetPair.first.find("1lep") == std::string::npos){ continue; }   
  
	  //Regular weight systematics
	  for(std::string sys : wgtList){
      
	    //std::cout<<"============================ TARGET : "<<targetPair.first<<" ========================="<<std::endl;   
	    TH1D* targethist = NULL;
	    double sumint = 0.;
	    for(std::string source_reg : targetPair.second){
	      TH1D* sourcehist = m_outMngrHist->HistMngr()->GetTH1D( (source_reg + channel + "_" + variable + sys).c_str() );
	      AddHistogramToTarget(sourcehist, &targethist, targetPair.first + channel + "_" + variable + sys);
	      sumint += sourcehist->GetEntries();
	    }//source regions

	    outfile->cd();
	    if(targethist){
	      HistManager::FinaliseTH1Bins(targethist);
	      targethist->Write();
	      delete targethist;
	    }
	    //std::cout<<"======================================================================================"<<std::endl;
	  }//systematics + nominal

	  //Region dependent weight systematics, including proxies for normalisation uncertainties 
	  for(std::pair<std::string, std::string> boostcat : boostcat_systs){

	    //============================ V+jets samples ===========================================================================
	    if(is_Vjets){

	      //only XS uncertainty
	      TH1D* targethist_Vjets_XS_up = NULL;
	      TH1D* targethist_Vjets_XS_down = NULL;
	      for(std::string source_reg : targetPair.second){
		TH1D* sourcehist = m_outMngrHist->HistMngr()->GetTH1D( (source_reg + channel + "_" + variable).c_str() );
	  
		//If source region is in the  boost category of the systematic, then scale it up and down
		double sc_up = (source_reg.find(boostcat.second) != std::string::npos) ? 1.3 : 1.;
		double sc_down = (source_reg.find(boostcat.second) != std::string::npos) ? 0.7 : 1.;
	  
		AddHistogramToTarget(sourcehist, &targethist_Vjets_XS_up, targetPair.first + channel + "_" + variable + "_weight_Vjets_XS_"+boostcat.first+"_up", sc_up);
		AddHistogramToTarget(sourcehist, &targethist_Vjets_XS_down, targetPair.first + channel + "_" + variable + "_weight_Vjets_XS_"+boostcat.first+"_down", sc_down);
	      }//source regions
	
	      if(is_VjetsHF){
		//only XS uncertainty
		TH1D* targethist_VjetsHF_XS_up = NULL;
		TH1D* targethist_VjetsHF_XS_down = NULL;
		for(std::string source_reg : targetPair.second){
		  TH1D* sourcehist = m_outMngrHist->HistMngr()->GetTH1D( (source_reg + channel + "_" + variable).c_str() );
	    
		  //If source region is in the  boost category of the systematic, then scale it up and down
		  double sc_up = (source_reg.find(boostcat.second) != std::string::npos) ? 1.3 : 1.;
		  double sc_down = (source_reg.find(boostcat.second) != std::string::npos) ? 0.7 : 1.;
	    
		  AddHistogramToTarget(sourcehist, &targethist_VjetsHF_XS_up, targetPair.first + channel + "_" + variable + "_weight_VjetsHF_XS_"+boostcat.first+"_up", sc_up);
		  AddHistogramToTarget(sourcehist, &targethist_VjetsHF_XS_down, targetPair.first + channel + "_" + variable + "_weight_VjetsHF_XS_"+boostcat.first+"_down", sc_down);
		}//source regions
	  
		outfile->cd();
		if(targethist_VjetsHF_XS_up){
		  HistManager::FinaliseTH1Bins(targethist_VjetsHF_XS_up);
		  targethist_VjetsHF_XS_up->Write();
		  delete targethist_VjetsHF_XS_up;
		}
		if(targethist_VjetsHF_XS_down){
		  HistManager::FinaliseTH1Bins(targethist_VjetsHF_XS_down);
		  targethist_VjetsHF_XS_down->Write();
		  delete targethist_VjetsHF_XS_down;
		}

	      }//Vjets+HF sample

	      outfile->cd();
	      if(targethist_Vjets_XS_up){
		HistManager::FinaliseTH1Bins(targethist_Vjets_XS_up);
		targethist_Vjets_XS_up->Write();
		delete targethist_Vjets_XS_up;
	      }
	      if(targethist_Vjets_XS_down){
		HistManager::FinaliseTH1Bins(targethist_Vjets_XS_down);
		targethist_Vjets_XS_down->Write();
		delete targethist_Vjets_XS_down;
	      }
	
	    }//Vjets sample
	    //================================================= V+jets samples ===========================================================

	    //============================ Diboson samples ===========================================================================
	    if(is_Diboson){

	      //only XS uncertainty
	      TH1D* targethist_Diboson_XS_up = NULL;
	      TH1D* targethist_Diboson_XS_down = NULL;
	      for(std::string source_reg : targetPair.second){
		TH1D* sourcehist = m_outMngrHist->HistMngr()->GetTH1D( (source_reg + channel + "_" + variable).c_str() );
	  
		//If source region is in the  boost category of the systematic, then scale it up and down
		double sc_up = (source_reg.find(boostcat.second) != std::string::npos) ? 1.48 : 1.;
		double sc_down = (source_reg.find(boostcat.second) != std::string::npos) ? 0.52 : 1.;
	  
		AddHistogramToTarget(sourcehist, &targethist_Diboson_XS_up, targetPair.first + channel + "_" + variable + "_weight_Dibosons_XS_"+boostcat.first+"_up", sc_up);
		AddHistogramToTarget(sourcehist, &targethist_Diboson_XS_down, targetPair.first + channel + "_" + variable + "_weight_Dibosons_XS_"+boostcat.first+"_down", sc_down);
	      }//source regions
	
	      outfile->cd();
	      if(targethist_Diboson_XS_up){
		HistManager::FinaliseTH1Bins(targethist_Diboson_XS_up);
		targethist_Diboson_XS_up->Write();
		delete targethist_Diboson_XS_up;
	      }
	      if(targethist_Diboson_XS_down){
		HistManager::FinaliseTH1Bins(targethist_Diboson_XS_down);
		targethist_Diboson_XS_down->Write();
		delete targethist_Diboson_XS_down;
	      }
	
	    }//Diboson sample
	    //================================================= Diboson samples ===========================================================

	    //============================ Single top samples ===========================================================================
	    if(is_singletop){

	      std::vector<std::string> mtblist{};
	      if(targetPair.first.find("0lep") != std::string::npos){ 
		mtblist =  {"","LowMtbmin","HighMtbmin"};
	      } 
	      else{ 
		mtblist = {""}; 
	      }
	
	      for(const std::string& mtbcat : mtblist){

		std::string mtbsuffix = (mtbcat.empty()) ? mtbcat : "_"+mtbcat;
		double sc_const = (mtbcat == "HighMtbmin") ? 0.25 : 0.20;
		//XS uncertainty
		TH1D* targethist_Singletop_XS_up = NULL;
		TH1D* targethist_Singletop_DRDS_up = NULL;
		TH1D* targethist_Singletop_XS_down = NULL;
	  
		//std::cout << std::endl <<" STARTING Target region: " << targetPair.first << " BOOSTCAT : " << boostcat.second << std::endl;
	  
		for(std::string source_reg : targetPair.second){
		  TH1D* sourcehist = m_outMngrHist->HistMngr()->GetTH1D( (source_reg + channel + "_" + variable).c_str() );
	    
		  bool is_regmatch = ( (source_reg.find(boostcat.second) != std::string::npos) 
				       && ( (mtbcat.empty() && (source_reg.find("Mtbmin") == std::string::npos)) 
					    || (!mtbcat.empty() && (source_reg.find(mtbcat) != std::string::npos)) ) ); 
	    
		  //If source region is in the  boost category of the systematic, then scale it up and down
		  //also add DRDS variation
		  double sc_up = 1.;
		  double sc_down = 1.;
		  if(is_regmatch){
		    sc_up = 1.+sc_const;
		    sc_down = 1.-sc_const;
		    TH1D* sourcehist_DRDS = m_outMngrHist->HistMngr()->GetTH1D( (source_reg + channel + "_" + variable + "_SingletopDRDS").c_str() );
		    if(sourcehist_DRDS){
		      //std::cout<<" DRDS Source region : "<<source_reg<<" --> "<<sourcehist_DRDS->Integral() << std::endl;
		      AddHistogramToTarget(sourcehist_DRDS, &targethist_Singletop_DRDS_up
					   , targetPair.first + channel + "_" + variable + "_SingletopDRDS_"+boostcat.first+mtbsuffix);
		    }
		  }
		  else{
		    //std::cout<<" Source region : "<<source_reg<<" --> "<<sourcehist->Integral() << std::endl;
		    AddHistogramToTarget(sourcehist, &targethist_Singletop_DRDS_up
					 , targetPair.first + channel + "_" + variable + "_SingletopDRDS_"+boostcat.first+mtbsuffix);
		  }
	    
		  AddHistogramToTarget(sourcehist, &targethist_Singletop_XS_up, 
				       targetPair.first + channel + "_" + variable + "_weight_Singletop_XS_"+boostcat.first+mtbsuffix+"_up", sc_up);
		  AddHistogramToTarget(sourcehist, &targethist_Singletop_XS_down, 
				       targetPair.first + channel + "_" + variable + "_weight_Singletop_XS_"+boostcat.first+mtbsuffix+"_down", sc_down);
		}//source regions
	  
		outfile->cd();
		if(targethist_Singletop_XS_up){
		  HistManager::FinaliseTH1Bins(targethist_Singletop_XS_up);
		  targethist_Singletop_XS_up->Write();
		  delete targethist_Singletop_XS_up;
		}
		if(targethist_Singletop_XS_down){
		  HistManager::FinaliseTH1Bins(targethist_Singletop_XS_down);
		  targethist_Singletop_XS_down->Write();
		  delete targethist_Singletop_XS_down;
		}
		if(targethist_Singletop_DRDS_up){
		  //std::cout<<" Target region: " << targetPair.first << " --> " << targethist_Singletop_DRDS_up->Integral() << std::endl;
		  HistManager::FinaliseTH1Bins(targethist_Singletop_DRDS_up);
		  targethist_Singletop_DRDS_up->Write();
		  delete targethist_Singletop_DRDS_up;
		}
	  
	      }//mtbmin categories

	    }//single top samples

	    //================================================= Single top samples ===========================================================


	    //============================ QCD samples ===========================================================================
	    if(is_QCD0L){
	      std::vector<std::string> mtblist =  {"","LowMtbmin","HighMtbmin"};

	      for(const std::string& mtbcat : mtblist){
	  
		//XS uncertainty
		TH1D* targethist_QCD0L_XS_up = NULL;
		TH1D* targethist_QCD0L_XS_down = NULL;
		for(std::string source_reg : targetPair.second){
		  TH1D* sourcehist = m_outMngrHist->HistMngr()->GetTH1D( (source_reg + channel + "_" + variable).c_str() );
	    
		  bool is_regmatch = ( (source_reg.find(boostcat.second) != std::string::npos) 
				       && ( (mtbcat.empty() && (source_reg.find("Mtbmin") == std::string::npos)) 
					    || (!mtbcat.empty() && (source_reg.find(mtbcat) != std::string::npos)) ) ); 
	    
		  //If source region is in the  boost category of the systematic, then scale it up and down
		  double sc_up = is_regmatch ? 2. : 1.;
		  double sc_down = is_regmatch ? 0. : 1.;
		  /*
		    if(is_regmatch){
		    std::cout << " is_regmatch = " << is_regmatch << " source_reg = "<<source_reg 
		    << " mtbcat = " << mtbcat << " boostcat = " << boostcat.second 
		    << " sc_up " << sc_up << std::endl;
		    }
		  */
		  std::string mtbsuffix = (mtbcat.empty()) ? mtbcat : "_"+mtbcat;
		  AddHistogramToTarget(sourcehist, &targethist_QCD0L_XS_up, targetPair.first + channel + "_" + variable + "_weight_QCD0L_XS_"+boostcat.first+mtbsuffix+"_up", sc_up);
		  AddHistogramToTarget(sourcehist, &targethist_QCD0L_XS_down, targetPair.first + channel + "_" + variable + "_weight_QCD0L_XS_"+boostcat.first+mtbsuffix+"_down", sc_down);
		}//source regions
	  
		//DR/DS uncertainty (TO DO)
	  
		outfile->cd();
		if(targethist_QCD0L_XS_up){
		  HistManager::FinaliseTH1Bins(targethist_QCD0L_XS_up);
		  targethist_QCD0L_XS_up->Write();
		  delete targethist_QCD0L_XS_up;
		}
		if(targethist_QCD0L_XS_down){
		  HistManager::FinaliseTH1Bins(targethist_QCD0L_XS_down);
		  targethist_QCD0L_XS_down->Write();
		  delete targethist_QCD0L_XS_down;
		}
	  
	      }//mtbmin categories
	
	    }//QCD0L samples 


	    if(is_QCD){

	      //weight uncertainty
	      TH1D* targethist_QCD_el_up = NULL;
	      TH1D* targethist_QCD_mu_up = NULL;

	      for(std::string source_reg : targetPair.second){
	  
		if(source_reg.find(boostcat.second) != std::string::npos){
		  //If source region is in the  boost category of the systematic, add the systematically varied histograms
		  TH1D* sourcehist_el_up = m_outMngrHist->HistMngr()->GetTH1D( (source_reg + channel + "_" + variable + "_weight_qcd_el_up").c_str() );
		  //TH1D* sourcehist_el_down = m_outMngrHist->HistMngr()->GetTH1D( (source_reg + channel + "_" + variable + "_weight_qcd_el_down").c_str() );
		  TH1D* sourcehist_mu_up = m_outMngrHist->HistMngr()->GetTH1D( (source_reg + channel + "_" + variable + "_weight_qcd_mu_up").c_str() );
		  //TH1D* sourcehist_mu_down = m_outMngrHist->HistMngr()->GetTH1D( (source_reg + channel + "_" + variable + "_weight_qcd_mu_down").c_str() );
	    
		  AddHistogramToTarget(sourcehist_el_up, &targethist_QCD_el_up, targetPair.first + channel + "_" + variable + "_weight_qcd_el_"+boostcat.first+"_up");
		  //AddHistogramToTarget(sourcehist_el_down, &targethist_QCD_el_down, targetPair.first + channel + "_" + variable + "_weight_qcd_el_"+boostcat.first+"_down");
		  AddHistogramToTarget(sourcehist_mu_up, &targethist_QCD_mu_up, targetPair.first + channel + "_" + variable + "_weight_qcd_mu_"+boostcat.first+"_up");
		  //AddHistogramToTarget(sourcehist_mu_down, &targethist_QCD_mu_down, targetPair.first + channel + "_" + variable + "_weight_qcd_mu_"+boostcat.first+"_down");
		}
		else{
		  TH1D* sourcehist = m_outMngrHist->HistMngr()->GetTH1D( (source_reg + channel + "_" + variable).c_str() );
		  AddHistogramToTarget(sourcehist, &targethist_QCD_el_up, targetPair.first + channel + "_" + variable + "_weight_qcd_el_"+boostcat.first+"_up");
		  //AddHistogramToTarget(sourcehist, &targethist_QCD_el_down, targetPair.first + channel + "_" + variable + "_weight_qcd_el_"+boostcat.first+"_down");
		  AddHistogramToTarget(sourcehist, &targethist_QCD_mu_up, targetPair.first + channel + "_" + variable + "_weight_qcd_mu_"+boostcat.first+"_up");
		  //AddHistogramToTarget(sourcehist, &targethist_QCD_mu_down, targetPair.first + channel + "_" + variable + "_weight_qcd_mu_"+boostcat.first+"_down");
		}
	      }//source regions
	
	      outfile->cd();
	      if(targethist_QCD_el_up){
		HistManager::FinaliseTH1Bins(targethist_QCD_el_up);
		targethist_QCD_el_up->Write();
		delete targethist_QCD_el_up;
	      }
	      /*      if(targethist_QCD_el_down){
		      targethist_QCD_el_down->Write();
		      delete targethist_QCD_el_down;
		      }*/
	      if(targethist_QCD_mu_up){
		HistManager::FinaliseTH1Bins(targethist_QCD_mu_up);
		targethist_QCD_mu_up->Write();
		delete targethist_QCD_mu_up;
	      }
	      /*if(targethist_QCD_mu_down){
		targethist_QCD_mu_down->Write();
		delete targethist_QCD_mu_down;
		}*/
	
	    }//QCD samples
      
	    //================================================= QCD samples ===========================================================
      
	  }//Boosted categories
    
	}//lepton channels

      }//target regions
      
    }//vlq decays
    
  }//variables
  
  //==== Normalisation ===
  //Vjets_XS -- split by lepton channel and boosted category 30% (additional histograms for charm and beauty)
  //Dibosons_XS -- split by lepton channel and boosted category (48%)
  //Singletop_XS -- split by lepton channel, boosted category and mtbmin category (20% 1lep and low-mTbmin or non-split 0L, 25% high-mTbmin)
  //QCD_0L -- split by boosted category and mtbmin category (100% everywhere)

  //==== Existing weight ====
  //QCD_1L -- split by boosted category and mtbmin category and lepton flavour category
  //Singletop_DRDS -- split by lepton channel, boosted category and mtbmin category

  
  wgtList.clear();
  regList.clear();
  targetPrintList.clear();
  
  outfile->Close();
  return true;
}

//____________________________________________________________________________
//
std::string VLQ_Analysis_Data2015::summaryYields()
{
  struct {
    bool operator()(const string &histoname) {
      return (vlq::utils::endswith(histoname, "meff") and not
      vlq::utils::endswith(histoname, "_vs_meff"));
    }
  } isInterestingHistogram; // what I think is useful; this definition is to be discussed

  std::ostringstream oss;
  if(HistManager *hm = m_outMngrHist->HistMngr()) {
    const string cutflow_histoname = "cutFlow_unWeight";
    if(TH1D *h = hm->GetTH1D(cutflow_histoname)){
      const vector<double> bin_centers = {1, 2, 3, 4, 5, 6, 7, 8}; // avoid unused bins, see VLQ_Analysis_Data2015::Loop
      oss<<cutflow_histoname
      <<" (docutflow = "<<m_opt -> DoCutFlow()<<"): ";
      for(auto v : bin_centers)
      oss<<" -> "<<h->GetBinContent(h->GetXaxis()->FindFixBin(v));
      oss<<"\n";
    } else {
      cout<<"VLQ_Analysis_Data2015::summaryYields: Cannot retrieve '"<<cutflow_histoname<<"'"<<endl;
    }
    vector<string> all_th1_names = hm->GetTH1KeyList();
    vector<string> relevant_th1_names;
    relevant_th1_names.reserve(all_th1_names.size());
    copy_if(all_th1_names.begin(), all_th1_names.end(), std::back_inserter(relevant_th1_names), isInterestingHistogram);
    for(const string &hn : relevant_th1_names)
    if(TH1D *h = hm->GetTH1D(hn))
    oss<<hn<<": "<<h->Integral()<<"\n";
  } else {
    cout<<"VLQ_Analysis_Data2015::summaryYields: cannot access hist manager."<<endl;
  }
  return oss.str();
}
