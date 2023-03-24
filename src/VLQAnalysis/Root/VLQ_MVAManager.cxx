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

  if(m_opt->AddMVAInputsFromXml() || m_opt->ApplyMVA()){
    status += ReadXmlWeightFile();
  }
  
  if(m_opt->ApplyMVA()) { status += InitMVAReader(); }

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

  m_metadataStore = {};
  //======================================
  //Metadata
  m_metadataStore["run_number"] = new VariableDef("run_number", ";Run Number", &(m_outputData->o_run_number));
  m_metadataStore["event_number"] = new VariableDef("event_number", ";Event Number", &(m_outputData->o_event_number));
  m_metadataStore["VLQtype"] = new VariableDef("VLQtype", ";VLQ decay type",  &(m_outputData->o_VLQtype));
  m_metadataStore["channel"] = new VariableDef("channel", ";Channel Type",  &(m_outputData->o_channel_type));
  m_metadataStore["sampleID"] = new VariableDef("sampleID", ";DSID", &(m_outputData->o_sampleID));
  m_metadataStore["sampleName"] = new VariableDef("sampleName", ";Sample name", &(m_outputData->o_sampleName));
  m_metadataStore["mc_campaign"] = new VariableDef("mc_campaign", ";MC campaign", &(m_outputData->o_mc_campaign));


  m_varStore = {};
  //======================================
  //Global meff
  m_varStore["meff"] = new VariableDef("meff","m_{eff} [GeV]",&(m_outputData -> o_meff));
  m_varStore["hthad"] = new VariableDef("hthad","H_{T}^{had} [GeV]",&(m_outputData -> o_hthad));
  //======================================

  //======================================
  //Angular variables between RC/RCTTM jets 
  m_varStore["leadingdR_RCjets"] = new VariableDef("leadingdR_RCjets", ";Leading #DeltaR(RC,RC)", 
							&(m_outputData -> o_leadingdR_RCjets));
  m_varStore["leadingdEta_RCjets"] = new VariableDef("leadingdEta_RCjets","Leading #Delta#eta(RC,RC)",
							  &(m_outputData -> o_leadingdEta_RCjets));
  m_varStore.at("leadingdEta_RCjets")->SetDefault(-10.);
  m_varStore["leadingdPhi_RCjets"] = new VariableDef("leadingdPhi_RCjets", ";Leading #Delta#phi(RC,RC)", 
							  &(m_outputData -> o_leadingdPhi_RCjets));
  m_varStore["leadingdR_RCMTT"] = new VariableDef("leadingdR_RCMTT", ";Leading #DeltaR(RCMTT,RCMTT)", 
							&(m_outputData -> o_leadingdR_RCTTMassRCTTMass));
  m_varStore["leadingdEta_RCMTT"] = new VariableDef("leadingdEta_RCMTT", ";Leading #Delta#eta(RCMTT,RCMTT)", 
							  &(m_outputData -> o_leadingdEta_RCTTMassRCTTMass));
  m_varStore.at("leadingdEta_RCMTT")->SetDefault(-10.);
  m_varStore["leadingdPhi_RCMTT"] = new VariableDef("leadingdPhi_RCMTT", ";Leading #Delta#phi(RCMTT,RCMTT)", 
							&(m_outputData -> o_leadingdPhi_RCTTMassRCTTMass));
  m_varStore["dRmin_RCjets"] = new VariableDef("dRmin_RCjets", ";#DeltaR_{min}(RC,RC)", 
						    &(m_outputData -> o_dRmin_RCjets));
  m_varStore["dEtamin_RCjets"] = new VariableDef("dEtamin_RCjets", ";#Delta#eta_{min}(RC,RC)", 
						      &(m_outputData -> o_dEtamin_RCjets));
  m_varStore.at("dEtamin_RCjets")->SetDefault(-10.);
  m_varStore["dPhimin_RCjets"] = new VariableDef("dPhimin_RCjets", ";#Delta#phi_{min}(RC,RC)", 
						      &(m_outputData -> o_dPhimin_RCjets));
  m_varStore["dRmin_RCMTT"] = new VariableDef("dRmin_RCMTT", ";#DeltaR_{min}(RCMTT,RCMTT)", 
						   &(m_outputData -> o_dRmin_RCTTMassRCTTMass));
  m_varStore["dEtamin_RCMTT"] = new VariableDef("dEtamin_RCMTT", ";#Delta#eta_{min}(RCMTT,RCMTT)", 
						     &(m_outputData -> o_dEtamin_RCTTMassRCTTMass));
  m_varStore.at("dEtamin_RCMTT")->SetDefault(-10.);
  m_varStore["dPhimin_RCMTT"] = new VariableDef("dPhimin_RCMTT", ";#Delta#phi_{min}(RCMTT,RCMTT)", 
						     &(m_outputData -> o_dPhimin_RCTTMassRCTTMass));

  m_varStore["dRavg_RCjets"] = new VariableDef("dRavg_RCjets", ";#DeltaR_{avg}(RC,RC)", 
						    &(m_outputData -> o_dRaverage_RCjets));
  m_varStore["dEtaavg_RCjets"] = new VariableDef("dEtaavg_RCjets", ";#Delta#eta_{avg}(RC,RC)", 
						      &(m_outputData -> o_dEtaaverage_RCjets));
  m_varStore.at("dEtaavg_RCjets")->SetDefault(-10.);
  m_varStore["dPhiavg_RCjets"] = new VariableDef("dPhiavg_RCjets", ";#Delta#phi_{avg}(RC,RC)", 
						      &(m_outputData -> o_dPhiaverage_RCjets));
  m_varStore["dRavg_RCMTT"] = new VariableDef("dRavg_RCMTT", ";#DeltaR_{avg}(RCMTT,RCMTT)", 
						   &(m_outputData -> o_dRaverage_RCTTMassRCTTMass));
  m_varStore["dEtaavg_RCMTT"] = new VariableDef("dEtaavg_RCMTT", ";#Delta#eta_{avg}(RCMTT,RCMTT)", 
						     &(m_outputData -> o_dEtaaverage_RCTTMassRCTTMass));
  m_varStore.at("dEtaavg_RCMTT")->SetDefault(-10.);
  m_varStore["dPhiavg_RCMTT"] = new VariableDef("dPhiavg_RCMTT", ";#Delta#phi_{avg}(RCMTT,RCMTT)", 
						     &(m_outputData -> o_dPhiaverage_RCTTMassRCTTMass));
  //======================================

  //======================================
  //Angular variables between RC/RCTTM jets and MET 

  m_varStore["dPhimin_RCMET"] = new VariableDef("dPhimin_RCMET", ";#Delta#phi_{min}(RC,MET)", 
						     &(m_outputData -> o_dPhimin_RCjetsMET ));
  m_varStore["dPhimin_RCMTTMET"] = new VariableDef("dPhimin_RCMTTMET", ";#Delta#phi_{min}(RCMTT,MET)", 
						       &(m_outputData -> o_dPhimin_RCjetsMET ));

  m_varStore["dPhiavg_RCMET"] = new VariableDef("dPhiavg_RCMET", ";#Delta#phi_{avg}(RC,MET)", 
						     &(m_outputData -> o_dPhiaverage_RCjetsMET));
  m_varStore["dPhiavg_RCMTTMET"] = new VariableDef("dPhiavg_RCMTTMET", ";#Delta#phi_{avg}(RCMTT,MET)", 
							&(m_outputData -> o_dPhiaverage_RCTTMassMET));

  m_varStore["leadingdPhi_RCMET"] = new VariableDef("leadingdPhi_RCMET", ";Leading #Delta#phi(RC,MET)", 
						  &(m_outputData -> o_leadingdPhi_RCjetsMET));
  m_varStore["leadingdPhi_RCMTTMET"] = new VariableDef("leadingdPhi_RCMTTMET", ";Leading #Delta#phi(RCMTT,MET)", 
						  &(m_outputData -> o_leadingdPhi_RCTTMassMET));
  //======================================


  //======================================
  //RC jet kinematics
  m_varStore["RCjet0_pt"] = new VariableDef("RCjet0_pt", ";p_{T}(RCjet0) [GeV]", &(m_outputData->o_rcjets), 0, "Pt");
  m_varStore["RCjet1_pt"] = new VariableDef("RCjet1_pt", ";p_{T}(RCjet0) [GeV]", &(m_outputData->o_rcjets), 1, "Pt");
  m_varStore["RCjet2_pt"] = new VariableDef("RCjet2_pt", ";p_{T}(RCjet0) [GeV]", &(m_outputData->o_rcjets), 2, "Pt");
  //======================================

  //======================================
  //Boosted object properties
  m_varStore["RCMTop0_pt"] = new VariableDef("RCMTop0_pt", ";p_{T}(RCMTop0) [GeV]", 
						  &(m_outputData -> o_taggedjets.at("RCMTop")), 0, "Pt");
  m_varStore["RCMHiggs0_pt"] = new VariableDef("RCMHiggs0_pt", ";p_{T}(RCMHiggs0) [GeV]", 
						   &(m_outputData -> o_taggedjets.at("RCMHiggs")), 0, "Pt");
  m_varStore["RCMV0_pt"] = new VariableDef("RCMV0_pt", ";p_{T}(RCMV0) [GeV]", 
						   &(m_outputData -> o_taggedjets.at("RCMV")), 0, "Pt");

  m_varStore["RCMTop1_pt"] = new VariableDef("RCMTop1_pt", ";p_{T}(RCMTop1) [GeV]", 
						  &(m_outputData -> o_taggedjets.at("RCMTop")), 1, "Pt");
  m_varStore["RCMHiggs1_pt"] = new VariableDef("RCMHiggs1_pt", ";p_{T}(RCMHiggs1) [GeV]", 
						  &(m_outputData -> o_taggedjets.at("RCMHiggs")), 1, "Pt");
  m_varStore["RCMV1_pt"] = new VariableDef("RCMV1_pt", ";p_{T}(RCMV1) [GeV]", 
						  &(m_outputData -> o_taggedjets.at("RCMV")), 1, "Pt");

  m_varStore["RCMTop0_eta"] = new VariableDef("RCMTop0_eta", ";#eta(RCMTop0)", 
					 &(m_outputData -> o_taggedjets.at("RCMTop")), 0, "Eta"); //default -10
  m_varStore.at("RCMTop0_eta")->SetDefault(-10.);
  m_varStore["RCMHiggs0_eta"] = new VariableDef("RCMHiggs0_eta", ";#eta(RCMHiggs0)", 
					   &(m_outputData -> o_taggedjets.at("RCMHiggs")), 0, "Eta"); //default -10
  m_varStore.at("RCMHiggs0_eta")->SetDefault(-10.);
  m_varStore["RCMV0_eta"] = new VariableDef("RCMV0_eta", ";#eta(RCMV0)", 
				       &(m_outputData -> o_taggedjets.at("RCMV")), 0, "Eta"); //default -10
  m_varStore.at("RCMV0_eta")->SetDefault(-10.);

  m_varStore["RCMTop0_nconsts"] = new VariableDef("RCMTop0_nconsts", ";N_{consts}(RCMTop0)", 
					       &(m_outputData -> o_taggedjets.at("RCMTop")), 0, "nconsts");
  m_varStore["RCMHiggs0_nconsts"] = new VariableDef("RCMHiggs0_nconsts", ";N_{consts}(RCMHiggs0)", 
					       &(m_outputData -> o_taggedjets.at("RCMHiggs")), 0, "nconsts");
  m_varStore["RCMV0_nconsts"] = new VariableDef("RCMV0_nconsts", ";N_{consts}(RCMV0)", 
					       &(m_outputData -> o_taggedjets.at("RCMV")), 0, "nconsts");

  m_varStore["RCMTop0_nbconsts"] = new VariableDef("RCMTop0_nbconsts", ";N_{bconsts}(RCMTop0)", 
						&(m_outputData -> o_taggedjets.at("RCMTop")), 0, "nbconsts");
  m_varStore["RCMHiggs0_nbconsts"] = new VariableDef("RCMHiggs0_nbconsts", ";N_{bconsts}(RCMHiggs0)", 
						&(m_outputData -> o_taggedjets.at("RCMHiggs")), 0, "nbconsts");
  m_varStore["RCMV0_nbconsts"] = new VariableDef("RCMV0_nbconsts", ";N_{bconsts}(RCMV0)", 
					    &(m_outputData -> o_taggedjets.at("RCMV")), 0, "nbconsts");
  //======================================

  //======================================
  //Jet multiplicities
  
  m_varStore["jets_n"] = new VariableDef("jets_n", ";N_{jets}", &(m_outputData -> o_jets_n));
  if( m_opt->BtagCollection()==VLQ_Options::TRACK ){
    m_varStore["bjets_n"] = new VariableDef("bjets_n", ";N_{bjets}^{trk}",  &(m_outputData -> o_trkbjets_n));
  }
  else{
    m_varStore["bjets_n"] = new VariableDef("bjets_n", ";N_{bjets}^{PF}", &(m_outputData -> o_bjets_n));  
  }

  m_varStore["RCjets_n"] = new VariableDef("RCjets_n", ";N_{RC}", &(m_outputData -> o_rcjets_n));
  m_varStore["RCMTT_jets_n"] = new VariableDef("RCMTT_jets_n", ";N_{RCMTT}", 
						    &(m_outputData -> o_taggedjets_n.at("RCTTMass")));
  m_varStore["RCMTop_jets_n"] = new VariableDef("RCMTop_jets_n", ";N_{RCMTop}", 
						     &(m_outputData -> o_taggedjets_n.at("RCMTop")));
  m_varStore["RCMHiggs_jets_n"] = new VariableDef("RCMHiggs_jets_n", ";N_{RCMHiggs}", 
						       &(m_outputData -> o_taggedjets_n.at("RCMHiggs")));
  m_varStore["RCMV_jets_n"] = new VariableDef("RCMV_jets_n", ";N_{RCMV}", &(m_outputData -> o_taggedjets_n.at("RCMV")));
  //======================================

  //======================================
  //Lepton and MET-related

  m_varStore["leptop_pt"] = new VariableDef("leptop_pt","Leptop p_{T} [GeV]", &(m_outputData -> o_leptop), -1, "Pt");
  m_varStore.at("leptop_pt")->SetDefault(-100.);
  m_varStore["ptw"] = new VariableDef("ptw", ";p_{T}^{W}", &(m_outputData -> o_ptwl));
  m_varStore["mtw"] = new VariableDef("mtw", ";m_{T}^{W}", &(m_outputData -> o_mtwl));
  m_varStore["residualMET"] = new VariableDef("residualMET", ";Residual MET", 
						      &(m_outputData -> o_residualMET), -1, "Pt");
  m_varStore["met"] = new VariableDef("met", ";E_{T}^{miss}", &(m_outputData -> o_met));
  m_varStore["mtbmin"] = new VariableDef("mtbmin", ";m_{T,b}^{min}", &(m_outputData -> o_mTbmin));
  //======================================

  //======================================
  //Reconstructed VLQ mass
  m_varStore["mvlq0_RCTTM_drmax"] = new VariableDef("mvlq0_RCTTM_drmax", ";m_{vlq}^{0}(RCTTM, #DeltaR_{max})", 
							 &(m_outputData -> o_m_vlq_rcttmass_drmax),0);
  m_varStore["mvlq1_RCTTM_drmax"] = new VariableDef("mvlq1_RCTTM_drmax", ";m_{vlq}^{1}(RCTTM, #DeltaR_{max})", 
							 &(m_outputData -> o_m_vlq_rcttmass_drmax),1);
  m_varStore["mvlq0_rcj_drmax"] = new VariableDef("mvlq0_rcj_drmax", ";m_{vlq}^{0}(RC, #DeltaR_{max})", 
						       &(m_outputData -> o_m_vlq_rcjets_drmax),0);
  m_varStore["mvlq1_rcj_drmax"] = new VariableDef("mvlq1_rcj_drmax", ";m_{vlq}^{1}(RC, #DeltaR_{max})", 
						       &(m_outputData -> o_m_vlq_rcjets_drmax),1);
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
      xmax=5000.;
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

  
  std::ifstream file(std::getenv("VLQAnalysisFramework_DIR")+std::string("/data/VLQAnalysis/")+m_opt->MVAWeightFile());
  if(!file.is_open()){
    std::cerr << " [Error] VLQ_VariableComputer::ReadXmlWeightFile() --> File " << m_opt->MVAWeightFile() 
	      << " not found." << std::endl;
    abort();
  }

  std::string line;
  while(getline(file, line)){

    bool foundVar = (line.find("Variable VarIndex") != std::string::npos);
    bool foundSpec = (line.find("Spectator SpecIndex") != std::string::npos);

    if(foundVar || foundSpec){

      size_t varNamePos = line.find("Expression");
      std::string lineTrimmed = line.substr(line.find("=", varNamePos)+1);
      std::string varName = lineTrimmed.substr(1, lineTrimmed.find(" ")-2);

      //Now look for it in the mva variable store
      if(m_varStore.find(varName) != m_varStore.end()){
	if(foundVar) m_readerVarList[varName] = m_varStore.at(varName);
	else if(foundSpec) m_readerSpecList[varName] =  m_varStore.at(varName);
	
      }
      else{
	std::cerr << " [Error] VLQ_VariableComputer::ReadXmlWeightFile() --> Variable " << varName 
		  << " required in MVA weight file is not found in m_varStore." << std::endl;
	abort();
      }

    }//adding a variable or spectator

  }//looping over weight file

  file.close();


  return 0;

}

int VLQ_MVAManager::InitMVAReader(){

  m_mvaReader = new TMVA::Reader("!Color:Silent");

  for(const std::pair<std::string, VariableDef*> varPair : m_readerVarList){
    m_mvaReader->AddVariable(varPair.first, varPair.second->FloatValStore());
  }

  for(const std::pair<std::string, VariableDef*> specPair : m_readerSpecList){
    m_mvaReader->AddSpectator(specPair.first, specPair.second->FloatValStore());
  }

  std::cout << "Booking MVA" << std::endl;
  m_mvaReader->BookMVA("MLP", m_opt->MVAWeightFile()); //find architecture name from weight file in future
  std::cout << "==================================================================================" << std::endl;


  return 0;

}

