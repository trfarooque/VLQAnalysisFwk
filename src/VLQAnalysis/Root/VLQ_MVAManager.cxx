#include "IFAETopFramework/OutputHistManager.h"
#include "IFAETopFramework/OutputTreeManager.h"
#include "IFAETopFramework/VariableDef.h"

#include "VLQAnalysis/VLQ_Options.h"
#include "VLQAnalysis/VLQ_OutputData.h"
#include "VLQAnalysis/VLQ_WeightManager.h"
#include "VLQAnalysis/VLQ_MVAManager.h"


//================== public ============================
VLQ_MVAManager::VLQ_MVAManager(VLQ_Options* opt, VLQ_OutputData* outData, VLQ_WeightManager* weightMngr,
			       OutputHistManager* outHistMngr, OutputTreeManager* outTreeMngr) : 
  m_opt(opt),
  m_outputData(outData),
  m_weightMngr(weightMngr),
  m_outputHistMngr(outHistMngr),
  m_outputTreeMngr(outTreeMngr),
  m_mvaReader(NULL),
  m_varStore{},
  m_metadataStore{},
  m_inputVarList{},
  m_readerVarList{},
  m_readerSpecList{}

{}

VLQ_MVAManager::VLQ_MVAManager() : 
  m_opt(NULL),
  m_outputData(NULL),
  m_weightMngr(NULL),
  m_outputHistMngr(NULL),
  m_outputTreeMngr(NULL),
  m_mvaReader(NULL),
  m_varStore{},
  m_metadataStore{},
  m_inputVarList{},
  m_readerVarList{},
  m_readerSpecList{}

{}

VLQ_MVAManager::~VLQ_MVAManager(){
  
  
  delete m_mvaReader;
  m_inputVarList.clear();
  m_readerVarList.clear();
  m_readerSpecList.clear();

  for( const std::pair<std::string, VariableDef*>& varPair : m_varStore ){ delete m_varStore.at(varPair.first); }
  m_varStore.clear();
  for( const std::pair<std::string, VariableDef* >& varPair : m_metadataStore ){ delete m_metadataStore.at(varPair.first); }
  m_metadataStore.clear();

}

int VLQ_MVAManager::Init(const std::vector<std::string>& inputList){

  int status = 0;

  status += MakeVariableStore();

  if(!m_opt) return status;

  
  if(m_opt->ApplyMVA()) {
    m_mvaReader = new TMVA::Reader("!Color");
  }

  if(m_opt->AddMVAInputsFromXml() || m_opt->ApplyMVA()){
    status += ReadXmlWeightFile();
  }

  if(m_opt->ApplyMVA()) { 
    std::cout << "Booking MVA" << std::endl;
    //find architecture name from weight file in future
    std::string xmlPath = std::getenv("VLQAnalysisFramework_DIR")+std::string("/data/VLQAnalysis/")+m_opt->MVAWeightFile();

    m_mvaReader->BookMVA("MLP", xmlPath.c_str());
    std::cout << "==================================================================================" << std::endl;
  }

  if(m_opt->MakeMVAInputHists() || m_opt->MakeMVAInputTree()){

    status += MakeInputVarList(inputList);

    if(m_opt->MakeMVAInputHists()) { status += AddMVAVarHistograms(); }
    if(m_opt->MakeMVAInputTree())  { status += AddMVAVarTreeBranches(); }

  }

  return status;

}


int VLQ_MVAManager::EvaluateMVA(){

  //First, update the variable stores
  for(const std::pair<std::string, VariableDef*> specPair : m_readerSpecList){
    specPair.second->CalcFloatValue();
  }
  for(const std::pair<std::string, VariableDef*> varPair : m_readerVarList){
    varPair.second->CalcFloatValue();
  }

  m_outputData->o_MVAScore = m_mvaReader->EvaluateMVA("MLP");

  return 0;

}

//================== protected ============================


int VLQ_MVAManager::MakeVariableStore(){
  void* dummy = NULL;

  m_metadataStore = {};
  //======================================
  //Metadata
  m_metadataStore["run_number"] = new VariableDef("run_number", ";Run Number", &(dummy));
  if(m_outputData){ m_metadataStore.at("run_number")->SetAddress(&(m_outputData->o_run_number)); }

  m_metadataStore["event_number"] = new VariableDef("event_number", ";Event Number", &(dummy));
  if(m_outputData){ m_metadataStore.at("event_number")->SetAddress(&(m_outputData->o_event_number));}

  m_metadataStore["VLQtype"] = new VariableDef("VLQtype", ";VLQ decay type", &(dummy));
  if(m_outputData){ m_metadataStore.at("VLQtype")->SetAddress(&(m_outputData->o_VLQtype));}

  m_metadataStore["channel"] = new VariableDef("channel", ";Channel Type", &(dummy));
  if(m_outputData){ m_metadataStore.at("channel")->SetAddress(&(m_outputData->o_channel_type));}

  m_metadataStore["sampleID"] = new VariableDef("sampleID", ";DSID", &(dummy));
  if(m_outputData){ m_metadataStore.at("sampleID")->SetAddress(&(m_outputData->o_sampleID)); }

  m_metadataStore["sampleName"] = new VariableDef("sampleName", ";Sample name", &(dummy));
  if(m_outputData){ m_metadataStore.at("sampleName")->SetAddress(&(m_outputData->o_sampleName)); }

  m_metadataStore["mc_campaign"] = new VariableDef("mc_campaign", ";MC campaign", &(dummy));
  if(m_outputData){ m_metadataStore.at("mc_campaign")->SetAddress(&(m_outputData->o_mc_campaign)); }

  m_varStore = {};
  //======================================
  //Global meff
  m_varStore["meff"] = new VariableDef("meff",";m_{eff} [GeV]", &(dummy));
  if(m_outputData){ m_varStore.at("meff")->SetAddress(&(m_outputData -> o_meff));}

  m_varStore["hthad"] = new VariableDef("hthad",";H_{T}^{had} [GeV]", &(dummy));
  if(m_outputData){ m_varStore.at("hthad")->SetAddress(&(m_outputData -> o_hthad));}
  //======================================

  //======================================
  //Angular variables between RC/RCTTM jets 
  m_varStore["leadingdR_RCjets"] = new VariableDef("leadingdR_RCjets", ";Leading #DeltaR(RC,RC)", &(dummy)); 
  if(m_outputData){ m_varStore.at("leadingdR_RCjets")->SetAddress(&(m_outputData -> o_leadingdR_RCjets)); }

  m_varStore["leadingdEta_RCjets"] = new VariableDef("leadingdEta_RCjets",";Leading #Delta#eta(RC,RC)", &(dummy));
  if(m_outputData){ m_varStore.at("leadingdEta_RCjets")->SetAddress(&(m_outputData -> o_leadingdEta_RCjets)); }
  m_varStore.at("leadingdEta_RCjets")->SetDefault(-10.);

  m_varStore["leadingdPhi_RCjets"] = new VariableDef("leadingdPhi_RCjets", ";Leading #Delta#phi(RC,RC)", &(dummy));
  if(m_outputData){ m_varStore.at("leadingdPhi_RCjets")->SetAddress(&(m_outputData -> o_leadingdPhi_RCjets)); }

  m_varStore["leadingdR_RCMTT"] = new VariableDef("leadingdR_RCMTT", ";Leading #DeltaR(RCMTT,RCMTT)", &(dummy)); 
  if(m_outputData){ m_varStore.at("leadingdR_RCMTT")->SetAddress(&(m_outputData -> o_leadingdR_RCTTMassRCTTMass)); }

  m_varStore["leadingdEta_RCMTT"] = new VariableDef("leadingdEta_RCMTT", ";Leading #Delta#eta(RCMTT,RCMTT)", &(dummy)); 
  if(m_outputData){ m_varStore.at("leadingdEta_RCMTT")->SetAddress(&(m_outputData -> o_leadingdEta_RCTTMassRCTTMass)); }
  m_varStore.at("leadingdEta_RCMTT")->SetDefault(-10.);

  m_varStore["leadingdPhi_RCMTT"] = new VariableDef("leadingdPhi_RCMTT", ";Leading #Delta#phi(RCMTT,RCMTT)", &(dummy)); 
  if(m_outputData){ m_varStore.at("leadingdPhi_RCMTT")->SetAddress(&(m_outputData -> o_leadingdPhi_RCTTMassRCTTMass)); }

  m_varStore["dRmin_RCjets"] = new VariableDef("dRmin_RCjets", ";#DeltaR_{min}(RC,RC)", &(dummy)); 
  if(m_outputData){ m_varStore.at("dRmin_RCjets")->SetAddress(&(m_outputData -> o_dRmin_RCjets)); }

  m_varStore["dEtamin_RCjets"] = new VariableDef("dEtamin_RCjets", ";#Delta#eta_{min}(RC,RC)", &(dummy)); 
  if(m_outputData){ m_varStore.at("dEtamin_RCjets")->SetAddress(&(m_outputData -> o_dEtamin_RCjets)); }
  m_varStore.at("dEtamin_RCjets")->SetDefault(-10.);

  m_varStore["dPhimin_RCjets"] = new VariableDef("dPhimin_RCjets", ";#Delta#phi_{min}(RC,RC)", &(dummy)); 
  if(m_outputData){ m_varStore.at("dPhimin_RCjets")->SetAddress(&(m_outputData -> o_dPhimin_RCjets)); }

  m_varStore["dRmin_RCMTT"] = new VariableDef("dRmin_RCMTT", ";#DeltaR_{min}(RCMTT,RCMTT)", &(dummy)); 
  if(m_outputData){ m_varStore.at("dRmin_RCMTT")->SetAddress(&(m_outputData -> o_dRmin_RCTTMassRCTTMass)); }

  m_varStore["dEtamin_RCMTT"] = new VariableDef("dEtamin_RCMTT", ";#Delta#eta_{min}(RCMTT,RCMTT)", &(dummy)); 
  if(m_outputData){ m_varStore.at("dEtamin_RCMTT")->SetAddress(&(m_outputData -> o_dEtamin_RCTTMassRCTTMass)); }
  m_varStore.at("dEtamin_RCMTT")->SetDefault(-10.);

  m_varStore["dPhimin_RCMTT"] = new VariableDef("dPhimin_RCMTT", ";#Delta#phi_{min}(RCMTT,RCMTT)", &(dummy)); 
  if(m_outputData){ m_varStore.at("dPhimin_RCMTT")->SetAddress(&(m_outputData -> o_dPhimin_RCTTMassRCTTMass)); }

  m_varStore["dRavg_RCjets"] = new VariableDef("dRavg_RCjets", ";#DeltaR_{avg}(RC,RC)", &(dummy)); 
  if(m_outputData){ m_varStore.at("dRavg_RCjets")->SetAddress(&(m_outputData -> o_dRaverage_RCjets)); }

  m_varStore["dEtaavg_RCjets"] = new VariableDef("dEtaavg_RCjets", ";#Delta#eta_{avg}(RC,RC)", &(dummy)); 
  if(m_outputData){ m_varStore.at("dEtaavg_RCjets")->SetAddress(&(m_outputData -> o_dEtaaverage_RCjets)); }
  m_varStore.at("dEtaavg_RCjets")->SetDefault(-10.);

  m_varStore["dPhiavg_RCjets"] = new VariableDef("dPhiavg_RCjets", ";#Delta#phi_{avg}(RC,RC)", &(dummy)); 
  if(m_outputData){ m_varStore.at("dPhiavg_RCjets")->SetAddress(&(m_outputData -> o_dPhiaverage_RCjets)); }

  m_varStore["dRavg_RCMTT"] = new VariableDef("dRavg_RCMTT", ";#DeltaR_{avg}(RCMTT,RCMTT)", &(dummy)); 
  if(m_outputData){ m_varStore.at("dRavg_RCMTT")->SetAddress(&(m_outputData -> o_dRaverage_RCTTMassRCTTMass)); }

  m_varStore["dEtaavg_RCMTT"] = new VariableDef("dEtaavg_RCMTT", ";#Delta#eta_{avg}(RCMTT,RCMTT)", &(dummy)); 
  if(m_outputData){ m_varStore.at("dEtaavg_RCMTT")->SetAddress(&(m_outputData -> o_dEtaaverage_RCTTMassRCTTMass)); }
  m_varStore.at("dEtaavg_RCMTT")->SetDefault(-10.);

  m_varStore["dPhiavg_RCMTT"] = new VariableDef("dPhiavg_RCMTT", ";#Delta#phi_{avg}(RCMTT,RCMTT)", &(dummy)); 
  if(m_outputData){ m_varStore.at("dPhiavg_RCMTT")->SetAddress(&(m_outputData -> o_dPhiaverage_RCTTMassRCTTMass)); }
  //======================================

  //======================================
  //Angular variables between RC/RCTTM jets and MET 

  m_varStore["dPhimin_RCMET"] = new VariableDef("dPhimin_RCMET", ";#Delta#phi_{min}(RC,MET)", &(dummy));
  if(m_outputData){ m_varStore.at("dPhimin_RCMET")->SetAddress(&(m_outputData -> o_dPhimin_RCjetsMET )); }

  m_varStore["dPhimin_RCMTTMET"] = new VariableDef("dPhimin_RCMTTMET", ";#Delta#phi_{min}(RCMTT,MET)", &(dummy));
  if(m_outputData){ m_varStore.at("dPhimin_RCMTTMET")->SetAddress(&(m_outputData -> o_dPhimin_RCTTMassMET )); }
  
  m_varStore["dPhiavg_RCMET"] = new VariableDef("dPhiavg_RCMET", ";#Delta#phi_{avg}(RC,MET)", &(dummy));			
  if(m_outputData){ m_varStore.at("dPhiavg_RCMET")->SetAddress(&(m_outputData -> o_dPhiaverage_RCjetsMET));}

  m_varStore["dPhiavg_RCMTTMET"] = new VariableDef("dPhiavg_RCMTTMET", ";#Delta#phi_{avg}(RCMTT,MET)", &(dummy));		
  if(m_outputData){ m_varStore.at("dPhiavg_RCMTTMET")->SetAddress(&(m_outputData -> o_dPhiaverage_RCTTMassMET)); }

  m_varStore["leadingdPhi_RCMET"] = new VariableDef("leadingdPhi_RCMET", ";Leading #Delta#phi(RC,MET)", &(dummy));
  if(m_outputData){ m_varStore.at("leadingdPhi_RCMET")->SetAddress(&(m_outputData -> o_leadingdPhi_RCjetsMET)); }

  m_varStore["leadingdPhi_RCMTTMET"] = new VariableDef("leadingdPhi_RCMTTMET", ";Leading #Delta#phi(RCMTT,MET)", &(dummy));
  if(m_outputData){ m_varStore.at("leadingdPhi_RCMTTMET")->SetAddress(&(m_outputData -> o_leadingdPhi_RCTTMassMET)); }
  //======================================

  //======================================
  //QCD cleaning variable - technically not part of the MVA set but useful

  m_varStore["dPhi_jetmet"] = new VariableDef("dPhi_jetmet", ";#Delta#Phi^{min}(MET,jet)", &(dummy));
  if(m_outputData){ m_varStore.at("dPhi_jetmet")->SetAddress(&(m_outputData -> o_dPhi_jetmet)); }

  //======================================
  //RC jet kinematics
  m_varStore["RCjet0_pt"] = new VariableDef("RCjet0_pt", ";p_{T}(RCjet0) [GeV]", &(dummy), 0, "Pt");
  if(m_outputData){ m_varStore.at("RCjet0_pt")->SetAddress(&(m_outputData->o_rcjets)); }
  m_varStore["RCjet1_pt"] = new VariableDef("RCjet1_pt", ";p_{T}(RCjet0) [GeV]", &(dummy), 1, "Pt");
  if(m_outputData){ m_varStore.at("RCjet1_pt")->SetAddress(&(m_outputData->o_rcjets)); }
  m_varStore["RCjet2_pt"] = new VariableDef("RCjet2_pt", ";p_{T}(RCjet0) [GeV]", &(dummy), 2, "Pt");
  if(m_outputData){ m_varStore.at("RCjet2_pt")->SetAddress(&(m_outputData->o_rcjets)); }
  //======================================

  //======================================
  //Boosted object properties
  m_varStore["RCMTop0_pt"] = new VariableDef("RCMTop0_pt", ";p_{T}(RCMTop0) [GeV]", &(dummy), 0, "Pt");
  if(m_outputData){ m_varStore.at("RCMTop0_pt")->SetAddress(&(m_outputData->o_taggedjets.at("RCMTop"))); }

  m_varStore["RCMHiggs0_pt"] = new VariableDef("RCMHiggs0_pt", ";p_{T}(RCMHiggs0) [GeV]", &(dummy), 0, "Pt");
  if(m_outputData){ m_varStore.at("RCMHiggs0_pt")->SetAddress(&(m_outputData->o_taggedjets.at("RCMHiggs"))); }

  m_varStore["RCMV0_pt"] = new VariableDef("RCMV0_pt", ";p_{T}(RCMV0) [GeV]", &(dummy), 0, "Pt");
  if(m_outputData){ m_varStore.at("RCMV0_pt")->SetAddress(&(m_outputData->o_taggedjets.at("RCMV"))); }

  m_varStore["RCMTop1_pt"] = new VariableDef("RCMTop1_pt", ";p_{T}(RCMTop1) [GeV]", &(dummy), 1, "Pt");
  if(m_outputData){ m_varStore.at("RCMTop1_pt")->SetAddress(&(m_outputData->o_taggedjets.at("RCMTop"))); }

  m_varStore["RCMHiggs1_pt"] = new VariableDef("RCMHiggs1_pt", ";p_{T}(RCMHiggs1) [GeV]", &(dummy), 1, "Pt");
  if(m_outputData){ m_varStore.at("RCMHiggs1_pt")->SetAddress(&(m_outputData->o_taggedjets.at("RCMHiggs"))); }

  m_varStore["RCMV1_pt"] = new VariableDef("RCMV1_pt", ";p_{T}(RCMV1) [GeV]", &(dummy), 1, "Pt");
  if(m_outputData){ m_varStore.at("RCMV1_pt")->SetAddress(&(m_outputData->o_taggedjets.at("RCMV"))); }

  m_varStore["RCMTop0_eta"] = new VariableDef("RCMTop0_eta", ";#eta(RCMTop0)", &(dummy), 0, "Eta"); //default -10
  if(m_outputData){ m_varStore.at("RCMTop0_eta")->SetAddress(&(m_outputData->o_taggedjets.at("RCMTop"))); }
  m_varStore.at("RCMTop0_eta")->SetDefault(-10.);

  m_varStore["RCMHiggs0_eta"] = new VariableDef("RCMHiggs0_eta", ";#eta(RCMHiggs0)", &(dummy), 0, "Eta"); //default -10
  if(m_outputData){ m_varStore.at("RCMHiggs0_eta")->SetAddress(&(m_outputData->o_taggedjets.at("RCMHiggs"))); }
  m_varStore.at("RCMHiggs0_eta")->SetDefault(-10.);

  m_varStore["RCMV0_eta"] = new VariableDef("RCMV0_eta", ";#eta(RCMV0)", &(dummy), 0, "Eta"); //default -10
  if(m_outputData){ m_varStore.at("RCMV0_eta")->SetAddress(&(m_outputData->o_taggedjets.at("RCMV"))); }
  m_varStore.at("RCMV0_eta")->SetDefault(-10.);

  m_varStore["RCMTop0_nconsts"] = new VariableDef("RCMTop0_nconsts", ";N_{consts}(RCMTop0)", &(dummy), 0, "nconsts");
  if(m_outputData){ m_varStore.at("RCMTop0_nconsts")->SetAddress(&(m_outputData->o_taggedjets.at("RCMTop"))); }

  m_varStore["RCMHiggs0_nconsts"] = new VariableDef("RCMHiggs0_nconsts", ";N_{consts}(RCMHiggs0)", &(dummy), 0, "nconsts");
  if(m_outputData){ m_varStore.at("RCMHiggs0_nconsts")->SetAddress(&(m_outputData->o_taggedjets.at("RCMHiggs"))); }

  m_varStore["RCMV0_nconsts"] = new VariableDef("RCMV0_nconsts", ";N_{consts}(RCMV0)", &(dummy), 0, "nconsts");
  if(m_outputData){ m_varStore.at("RCMV0_nconsts")->SetAddress(&(m_outputData->o_taggedjets.at("RCMV"))); }

  m_varStore["RCMTop0_nbconsts"] = new VariableDef("RCMTop0_nbconsts", ";N_{bconsts}(RCMTop0)", &(dummy), 0, "nbconsts");
  if(m_outputData){ m_varStore.at("RCMTop0_nbconsts")->SetAddress(&(m_outputData->o_taggedjets.at("RCMTop"))); }

  m_varStore["RCMHiggs0_nbconsts"] = new VariableDef("RCMHiggs0_nbconsts", ";N_{bconsts}(RCMHiggs0)", &(dummy), 0, "nbconsts");
  if(m_outputData){ m_varStore.at("RCMHiggs0_nbconsts")->SetAddress(&(m_outputData->o_taggedjets.at("RCMHiggs"))); }

  m_varStore["RCMV0_nbconsts"] = new VariableDef("RCMV0_nbconsts", ";N_{bconsts}(RCMV0)", &(dummy), 0, "nbconsts");
  if(m_outputData){ m_varStore.at("RCMV0_nbconsts")->SetAddress(&(m_outputData->o_taggedjets.at("RCMV"))); }
  //======================================

  //======================================
  //Jet multiplicities
  
  m_varStore["jets_n"] = new VariableDef("jets_n", ";N_{jets}", &(dummy)); 
  if(m_outputData){ m_varStore.at("jets_n")->SetAddress(&(m_outputData -> o_jets_n)); }

  m_varStore["bjets_n"] = new VariableDef("bjets_n", ";N_{bjets}^{trk}",  &(dummy));
  if(m_outputData){ 
    if( m_opt->BtagCollection()==VLQ_Options::TRACK ){
      m_varStore.at("bjets_n")->SetAddress(&(m_outputData -> o_trkbjets_n));
    }
    else{
      m_varStore.at("bjets_n")->SetAddress(&(m_outputData -> o_bjets_n));
    }
  }

  m_varStore["RCjets_n"] = new VariableDef("RCjets_n", ";N_{RC}", &(dummy));
  if(m_outputData){ m_varStore.at("RCjets_n")->SetAddress(&(m_outputData -> o_rcjets_n)); }

  m_varStore["RCMTT_jets_n"] = new VariableDef("RCMTT_jets_n", ";N_{RCMTT}", &(dummy)); 
  if(m_outputData){ m_varStore.at("RCMTT_jets_n")->SetAddress(&(m_outputData -> o_taggedjets_n.at("RCTTMass"))); }

  m_varStore["RCMTop_jets_n"] = new VariableDef("RCMTop_jets_n", ";N_{RCMTop}", &(dummy)); 
  if(m_outputData){ m_varStore.at("RCMTop_jets_n")->SetAddress(&(m_outputData -> o_taggedjets_n.at("RCMTop"))); }

  m_varStore["RCMHiggs_jets_n"] = new VariableDef("RCMHiggs_jets_n", ";N_{RCMHiggs}", &(dummy)); 
  if(m_outputData){ m_varStore.at("RCMHiggs_jets_n")->SetAddress(&(m_outputData -> o_taggedjets_n.at("RCMHiggs"))); }

  m_varStore["RCMV_jets_n"] = new VariableDef("RCMV_jets_n", ";N_{RCMV}", &(dummy)); 
  if(m_outputData){ m_varStore.at("RCMV_jets_n")->SetAddress(&(m_outputData -> o_taggedjets_n.at("RCMV"))); }
  //======================================

  //======================================
  //Lepton and MET-related

  m_varStore["leptop_n"] = new VariableDef("leptop_n",";N_{leptonic top}", &(dummy));
  if(m_outputData){ m_varStore.at("leptop_n")->SetAddress(&(m_outputData -> o_leptop_n)); }

  m_varStore["leptop_pt"] = new VariableDef("leptop_pt",";p_{T}(leptonic top) [GeV]", &(dummy), -1, "Pt");
  if(m_outputData){ m_varStore.at("leptop_pt")->SetAddress(&(m_outputData -> o_leptop)); }
  m_varStore.at("leptop_pt")->SetDefault(-100.);

  m_varStore["ptw"] = new VariableDef("ptw", ";p_{T}^{W}", &(dummy));
  if(m_outputData){ m_varStore.at("ptw")->SetAddress(&(m_outputData -> o_ptwl)); }

  m_varStore["mtw"] = new VariableDef("mtw", ";m_{T}^{W}", &(dummy));
  if(m_outputData){ m_varStore.at("mtw")->SetAddress(&(m_outputData -> o_mtwl)); }

  m_varStore["residualMET"] = new VariableDef("residualMET", ";Residual MET", &(dummy), -1, "Pt");
  if(m_outputData){ m_varStore.at("residualMET")->SetAddress(&(m_outputData -> o_residualMET)); }

  m_varStore["met"] = new VariableDef("met", ";E_{T}^{miss}", &(dummy));
  if(m_outputData){ m_varStore.at("met")->SetAddress(&(m_outputData -> o_met)); }

  m_varStore["mtbmin"] = new VariableDef("mtbmin", ";m_{T,b}^{min}", &(dummy));
  if(m_outputData){ m_varStore.at("mtbmin")->SetAddress(&(m_outputData -> o_mTbmin)); }
  //======================================

  //======================================
  //Reconstructed VLQ mass
  m_varStore["mvlq0_RCTTM_drmax"] = new VariableDef("mvlq0_RCTTM_drmax", ";m_{vlq}^{0}(RCTTM, #DeltaR_{max})", &(dummy), 0); 
  if(m_outputData){ m_varStore.at("mvlq0_RCTTM_drmax")->SetAddress(&(m_outputData -> o_m_vlq_rcttmass_drmax)); }

  m_varStore["mvlq1_RCTTM_drmax"] = new VariableDef("mvlq1_RCTTM_drmax", ";m_{vlq}^{1}(RCTTM, #DeltaR_{max})", &(dummy), 1);
  if(m_outputData){ m_varStore.at("mvlq1_RCTTM_drmax")->SetAddress(&(m_outputData -> o_m_vlq_rcttmass_drmax)); }

  m_varStore["mvlq0_rcj_drmax"] = new VariableDef("mvlq0_rcj_drmax", ";m_{vlq}^{0}(RC, #DeltaR_{max})",&(dummy), 0); 
  if(m_outputData){ m_varStore.at("mvlq0_rcj_drmax")->SetAddress(&(m_outputData -> o_m_vlq_rcjets_drmax)); }

  m_varStore["mvlq1_rcj_drmax"] = new VariableDef("mvlq1_rcj_drmax", ";m_{vlq}^{1}(RC, #DeltaR_{max})",&(dummy), 1); 
  if(m_outputData){ m_varStore.at("mvlq1_rcj_drmax")->SetAddress(&(m_outputData -> o_m_vlq_rcjets_drmax)); }
//======================================
  
  return 0;

}

int VLQ_MVAManager::MakeInputVarList(const std::vector<std::string>& inputList){


  if(inputList.empty() && !m_opt->AddMVAInputsFromXml()){

    //add everything from the variable store
    m_inputVarList.insert(m_varStore.begin(), m_varStore.end());

  }
  else{

    if(!inputList.empty()){

      for( const std::string& inputName : inputList){

	if(m_varStore.find(inputName) != m_varStore.end()){
	  m_inputVarList[inputName] = m_varStore.at(inputName);
	}
	else{
	  std::cout<< "[WARNING] VLQ_MVAManager::MakeInputVarList --> Variable " 
		   << inputName << "not found in list of MVA variables. Skipping. " << std::endl;
	}
      }

    }//first add variables from the inputList

    if(m_opt->AddMVAInputsFromXml()){
      if(m_inputVarList.empty()){ 
	m_inputVarList.insert(m_readerVarList.begin(), m_readerVarList.end());
      }
      else{

	for(const std::pair<std::string, VariableDef*> xmlVar : m_readerVarList ){
	  if(m_inputVarList.find(xmlVar.first) == m_inputVarList.end()){
	    m_inputVarList[xmlVar.first] = xmlVar.second;
	  }

	}//all reader variables

      }//if input list is non-empty

    }//adding xml variables

  }//if input list given or reading from xml (or both)


  return 0;

}


int VLQ_MVAManager::AddMVAVarTreeBranches(){

  //Always add all the metadata branches
  for(const std::pair<std::string, VariableDef*> metadataVar : m_metadataStore ){
    m_outputTreeMngr->AddStandardBranch(metadataVar.second, "", "");
  }

  //Add the event weights
  m_outputTreeMngr->AddAllWeightBranches("nomWeight", m_weightMngr, true);

  for(const std::pair<std::string, VariableDef*> inputVar : m_inputVarList ){

    m_outputTreeMngr->AddStandardBranch(inputVar.second, "", "");
  }
  if(m_opt->ApplyMVA()) m_outputTreeMngr->AddStandardBranch("MVAScore", "MVA Score", &(m_outputData -> o_MVAScore));

  m_outputTreeMngr->BookStandardTree("mvaTree", "tree for MVA training");
  return 0;

}
//EDIT - histograms
int VLQ_MVAManager::AddMVAVarHistograms(){
  double xmin = 0.;
  double xmax = 0.;
  double xwidth = 0.;

  bool addSyst = m_opt -> MultipleVariables();

  for(const std::pair<std::string, VariableDef*> inputVar : m_inputVarList ){

    const std::string& name = inputVar.first;
    VariableDef* var = inputVar.second;



    //==============Define binning=================
    if( (name == "meff") || (name == "hthad") 
	|| (name.find("mvlq") != std::string::npos) ){
      xmin=0.;
      xmax=7000.;
      xwidth=25.;
    }
    else if( (name=="mtw") || (name=="ptw") || (name=="residualMET") || (name=="mtbmin") ){
      xmin=0.;
      xmax=1000.;
      xwidth=25.;
    }
    else if( (name=="met") || (name.find("_pt") != std::string::npos) ){
      xmin=0.;
      xmax=2000.;
      xwidth=10.;
    }
    else if( (name.find("_eta") != std::string::npos) ){
      xmin=-3.;
      xmax=3.;
      xwidth=0.2;
    }
    else if( (name.find("dR") != std::string::npos) || (name.find("dEta") != std::string::npos) 
	     || (name.find("dPhi") != std::string::npos) ){
      xmin=0.;
      xmax=5.;
      xwidth=0.1;
    }
    else if( (name == "leptop_n") ){
      xmin=-0.5;
      xmax=1.5;
      xwidth=1.;
    }
    else if( (name == "jets_n") ){
      xmin=-0.5;
      xmax=15.5;
      xwidth=1.;
    } 
    else if( (name=="trkbjets_n") || (name=="bjets_n") ){
      xmin=-0.5;
      xmax=7.5;
      xwidth=1.;
    }
    else if( (name.find("jets_n") != std::string::npos) && (name.find("RC") != std::string::npos) ){
      xmin=-0.5;
      xmax=5.5;
      xwidth=1.;
    }
    else if( (name.find("_nconsts") != std::string::npos) || (name.find("_nbconsts") != std::string::npos) ){
      xmin=-0.5;
      xmax=5.5;
      xwidth=1.;
    }
    else{
      std::cout << "[ERROR] VLQ_MVAManager::AddTrainingVarHistograms: Binning for variable " 
		<< name << " not defined. Skipping." << std::endl;
    }
    //========================================

    m_outputHistMngr->AddStandardTH1( var, xwidth, xmin, xmax, addSyst ); 

  }

  if(m_opt->ApplyMVA()){
    m_outputHistMngr -> AddStandardTH1( "MVAScore", 0.050, -0.1, 1.05, "; MVA Score", true, &(m_outputData -> o_MVAScore) );
  }

  return 0;

}

int VLQ_MVAManager::ReadXmlWeightFile(){

  std::string xmlPath = std::getenv("VLQAnalysisFramework_DIR")+std::string("/data/VLQAnalysis/")+m_opt->MVAWeightFile();

  std::ifstream file(xmlPath.c_str());
  if(!file.is_open()){
    std::cerr << " [Error] VLQ_VariableComputer::ReadXmlWeightFile() --> File " << m_opt->MVAWeightFile() 
	      << " not found." << std::endl;
    abort();
  }

  std::string line;
  int ind = 0; 
  while(getline(file, line)){

    bool foundVar = (line.find("Variable VarIndex") != std::string::npos);
    bool foundSpec = (line.find("Spectator SpecIndex") != std::string::npos);

    if(foundVar || foundSpec){

      size_t varNamePos = line.find("Expression");
      std::string lineTrimmed = line.substr(line.find("=", varNamePos)+1);
      std::string varName = lineTrimmed.substr(1, lineTrimmed.find(" ")-2);
      //Now look for it in the mva variable store
      if(foundVar){
	if(m_varStore.find(varName) != m_varStore.end()){
	  std::cout << "Adding " << varName << " index : " << ind << std::endl;
	  m_readerVarList[varName] = m_varStore.at(varName);
	  m_mvaReader->AddVariable(varName, m_varStore.at(varName)->FloatValStore());
	  ind++;
	}
	else{
	  std::cerr << " [Error] VLQ_VariableComputer::ReadXmlWeightFile() --> Variable " << varName 
		    << " required in MVA weight file is not found in m_varStore." << std::endl;
	  abort();
	}
      }
      else if(foundSpec){
	if(m_metadataStore.find(varName) != m_metadataStore.end()){
	  m_readerSpecList[varName] =  m_metadataStore.at(varName);
	  m_mvaReader->AddSpectator(varName, m_metadataStore.at(varName)->FloatValStore());
	}
	else if(m_varStore.find(varName) != m_varStore.end()){
	  m_readerSpecList[varName] =  m_varStore.at(varName);
	  m_mvaReader->AddSpectator(varName, m_varStore.at(varName)->FloatValStore());
	}
	else{
	  std::cerr << " [Error] VLQ_VariableComputer::ReadXmlWeightFile() --> Spectator " << varName 
		    << " required in MVA weight file is not found in m_metadataStore of in m_varStore." << std::endl;
	  abort();
	}

      }

    }//adding a variable or spectator

  }//looping over weight file

  file.close();


  return 0;

}
