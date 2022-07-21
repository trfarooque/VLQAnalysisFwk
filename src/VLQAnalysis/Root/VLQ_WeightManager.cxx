#include "VLQAnalysis/VLQ_WeightManager.h"
#include "IFAETopFramework/SampleInfo.h"
#include "IFAETopFramework/AnalysisObject.h"
#include "IFAETopFramework/Selection.h"
#include "IFAETopFramework/AnalysisUtils.h"
#include "IFAETopFramework/OutputHistManager.h"
#include "VLQAnalysis/VLQ_Options.h"
#include "VLQAnalysis/VLQ_NtupleData.h"
#include "VLQAnalysis/VLQ_OutputData.h"
#include "VLQAnalysis/VLQ_Enums.h"
#include "VLQAnalysis/VLQ_TtbarSystematicsManager.h"
#include "VLQAnalysis/VLQ_VariableComputer.h"
#include "VLQAnalysis/VLQ_KinReweighter.h"
#include "VLQAnalysis/VLQ_FJVTCalibTool.h"

#include "IFAEReweightingTools/HFSystDataMembers.h"
#include "IFAEReweightingTools/ttbbNLO_syst.h"
#include "IFAEReweightingTools/ttccNLO_syst.h"
#include "IFAEReweightingTools/TtbarFractionReweighter.h"

//#include "NNLOReweighter/NNLOReweighter.h"

#include "FakeLeptonEstimation/FakeLeptonEstimation.h"

#include <iostream>
#include <stdexcept> // invalid_argument
#include <string>
#include <map>

using std::string;


//______________________________________________________________________________
//
VLQ_WeightManager::VLQ_WeightManager( VLQ_Options *opt, const VLQ_NtupleData* ntupleData,
                                      VLQ_OutputData* outputData, OutputHistManager* outHistMngr ):
  WeightManager(opt, ntupleData, outputData),
  m_vlq_opt(opt),
  m_vlq_ntupData(ntupleData),
  m_vlq_outData(outputData),
  m_outHistMngr(outHistMngr),
  m_sampleInfo(0),
  m_qcdWeight(0),
  m_varComputer(0),
  m_tool_HFsyst(0),
  m_tool_ttFractionRw(0),
  m_ttbar_syst_weight(0),
  m_kinRw(0),
  m_fJvt_calibTool(0),
  m_syst_regions(0)
{

  if( m_vlq_opt -> ReweightKinematics()
      && ( (m_vlq_opt->SampleName() == SampleName::WJETS) || (m_vlq_opt->SampleName() == SampleName::ZJETS)
	   || (m_vlq_opt->StrSampleName().find("TTBAR") != std::string::npos)
	   || (m_vlq_opt->StrSampleName().find("SINGLETOPWTPROD") != std::string::npos) )
      ){

    m_kinRw = new VLQ_KinReweighter(m_vlq_opt, m_vlq_outData /*, m_vlq_ntupData*/);

    if((m_vlq_opt -> SampleName() == SampleName::ZJETS) || (m_vlq_opt -> SampleName() == SampleName::WJETS)){
      m_kinRw->Init(std::getenv("VLQAnalysisFramework_DIR")+std::string("/data/VLQAnalysis/kinReweightings_OnlyZjets.root"));
    }
    else{
      m_kinRw->Init(std::getenv("VLQAnalysisFramework_DIR")+std::string("/data/VLQAnalysis/kinReweightings_OnlyWtTtbar.root"));
    }

  }

}

//______________________________________________________________________________
//
VLQ_WeightManager::VLQ_WeightManager( const VLQ_WeightManager &q ) : WeightManager(q) {
  m_vlq_opt           = q.m_vlq_opt;
  m_vlq_ntupData      = q.m_vlq_ntupData;
  m_vlq_outData       = q.m_vlq_outData;
  m_outHistMngr       = q.m_outHistMngr;
  m_sampleInfo        = q.m_sampleInfo;
  m_qcdWeight         = q.m_qcdWeight;
  m_varComputer       = q.m_varComputer;
  m_tool_HFsyst       = q.m_tool_HFsyst;
  m_tool_ttFractionRw = q.m_tool_ttFractionRw;
  m_ttbar_syst_weight = q.m_ttbar_syst_weight;
  m_kinRw             = q.m_kinRw;
  m_syst_regions      = q.m_syst_regions;
}

//______________________________________________________________________________
//
VLQ_WeightManager::~VLQ_WeightManager(){
  if(m_sampleInfo) delete m_sampleInfo;
  if(m_qcdWeight) delete m_qcdWeight;
  if(m_varComputer) delete m_varComputer;
  if(m_tool_HFsyst) delete m_tool_HFsyst;
  if(m_tool_ttFractionRw) delete m_tool_ttFractionRw;
  if(m_ttbar_syst_weight) delete m_ttbar_syst_weight;
  if(m_kinRw) delete m_kinRw;
  if(m_syst_regions) delete m_syst_regions;
}

//______________________________________________________________________________
//
void VLQ_WeightManager::Init( std::map < int, Selection* >* selection_tree ){

  ///////////////////////////////////////
  // Cross-section weight
  ///////////////////////////////////////
  if ( !(m_vlq_opt -> IsData() || (m_opt -> StrSampleName().find("QCD") != std::string::npos)) ) {
    std::string path = "";
    path += std::getenv("VLQAnalysisFramework_DIR") + std::string("/data/VLQAnalysis/") + m_vlq_opt -> SampleDat();
    string sampleId = m_vlq_opt -> StrSampleID();
    m_sampleInfo = new SampleInfo( sampleId, path );
    if(!m_sampleInfo->Ready()){
      throw std::invalid_argument(string(__FILE__)+"\n"+
				  " Could not identify sample '"+sampleId+"'"
				  " in the provided config file '"+path+"'. \n"
				  " Please check !"
				  " Normalisation will be crap !");
    }
  }

  //////////////////////////////////////
  // QCD weight tool
  //////////////////////////////////////
  m_qcdWeight = new FakeLeptonEstimation();
  m_qcdWeight -> Init();
  m_varComputer = new VLQ_VariableComputer(m_vlq_opt);

  //////////////////////////////////////
  // FJVT calibration tool
  //////////////////////////////////////
  m_fJvt_calibTool = new VLQ_FJVTCalibTool(m_vlq_ntupData, m_vlq_opt);
  m_fJvt_calibTool -> Init("/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/JetJvtEfficiency/May2020/fJvtSFFile.EMPFlow.root");

  //
  // ttbar-related
  //
  if( m_vlq_outData -> o_is_ttbar ){
    /*
    //////////////////////////////////////
    // ttbar NNLO reweighting
    //////////////////////////////////////
    if( m_vlq_opt -> ApplyTtbarNNLOCorrection() && m_vlq_opt -> RecomputeTtbarNNLOCorrection() ){
      int dsid = 410000;
      //radHi
      if( m_vlq_opt -> InputFile() . find(".407029.") != std::string::npos ) dsid = 410001;
      if( m_vlq_opt -> InputFile() . find(".407030.") != std::string::npos ) dsid = 410001;
      if( m_vlq_opt -> InputFile() . find(".407031.") != std::string::npos ) dsid = 410001;
      if( m_vlq_opt -> InputFile() . find(".407032.") != std::string::npos ) dsid = 410001;
      //radLo
      if( m_vlq_opt -> InputFile() . find(".407033.") != std::string::npos ) dsid = 410002;
      if( m_vlq_opt -> InputFile() . find(".407034.") != std::string::npos ) dsid = 410002;
      if( m_vlq_opt -> InputFile() . find(".407035.") != std::string::npos ) dsid = 410002;
      if( m_vlq_opt -> InputFile() . find(".407036.") != std::string::npos ) dsid = 410002;
      //PowHpp
      if( m_vlq_opt -> InputFile() . find(".407037.") != std::string::npos ) dsid = 410004;
      if( m_vlq_opt -> InputFile() . find(".407038.") != std::string::npos ) dsid = 410004;
      if( m_vlq_opt -> InputFile() . find(".407039.") != std::string::npos ) dsid = 410004;
      if( m_vlq_opt -> InputFile() . find(".407040.") != std::string::npos ) dsid = 410004;
      m_nnlo_rw = new NNLOReweighter( dsid, (std::string(getenv("VLQAnalysisFramework_DIR"))+"/data/NNLOReweighter/") );
      m_nnlo_rw -> Init();
    }
>>>>>>> Initial commit br_VLQ_Reweighting.
    */
    //////////////////////////////////////
    // tt+bb reweighting tool
    //////////////////////////////////////
    // Declaration of ttbb_syst class
    if( m_vlq_opt -> RecomputeTtBbRw () ){
      m_tool_HFsyst = new ttbbNLO_syst( m_vlq_opt -> StrSampleID(),std::getenv("VLQAnalysisFramework_DIR")+std::string("/data/IFAEReweightingTools/ttbbNormRw.root"),
					std::getenv("VLQAnalysisFramework_DIR")+std::string("/data/IFAEReweightingTools/ttbbShapeRw.root"));
      m_tool_HFsyst -> Init();
    }

    // Declaration of Ttbar Fraction Rw tool
    if( m_vlq_opt -> ReweightTtbarFractions () ){
      m_tool_ttFractionRw = new TtbarFractionReweighter(m_vlq_opt -> StrSampleID());
      m_tool_ttFractionRw -> Init();
    }

    //////////////////////////////////////
    // ttbar systematic uncertainties (generator comparisons)
    //////////////////////////////////////
    if( m_opt->ComputeWeightSys() ){
      //m_ttbar_syst_weight = new VLQ_TtbarSystematicsManager( m_vlq_opt, m_vlq_outData, m_vlq_ntupData );
      //m_ttbar_syst_weight -> Init(selection_tree, std::getenv("VLQAnalysisFramework_DIR")+std::string("/data/VLQAnalysis/ttbarSystematics.root"));
    }


  }//ttbar samples

}

//______________________________________________________________________________
//
bool VLQ_WeightManager::AddVLQNominalWeights(){

  if( m_opt -> StrSampleName().find("QCD") != std::string::npos ){
    AddAndInitWeight("weight_qcd", "", true/*in nominal*/, false/*is input*/);
  }
  else{
    AddAndInitWeight("weight_mc");
    AddAndInitWeight("weight_jvt");
    AddAndInitWeight("weight_norm", "", true, false);

    if(m_vlq_opt->UsePileUpWeight()){
      AddAndInitWeight("weight_pu");
    }

    if(m_vlq_opt->UseLeptonsSF()){
      AddAndInitWeight("weight_elec");
      AddAndInitWeight("weight_muon");
      if(m_vlq_opt->UseLeptonTrigger()){
        AddAndInitWeight("weight_elec_trigger");
        AddAndInitWeight("weight_muon_trigger");
      }
    }

    std::string btag_name = (m_vlq_opt->BtagCollection() == VLQ_Options::TRACK) ? "weight_trkbtag" : "weight_btag";
    bool btag_isinput = !((m_vlq_opt -> DoTRF() && m_vlq_opt -> RecomputeTRF()) || m_vlq_opt -> RecomputeBtagSF());
    AddAndInitWeight(btag_name, "", true/*is nominal*/, btag_isinput);

    if( m_vlq_outData -> o_is_ttbar ){
      //ttbb correction weight
      if(m_vlq_opt->ApplyTtbbCorrection() && m_vlq_opt->SampleName()==SampleName::TTBARBB){
        AddAndInitWeight("weight_ttbb", "", true, true, "weight_ttbb_ttbb_Nominal_weight");
      }
      //ttbar fraction reweighting
      if(m_vlq_opt->ReweightTtbarFractions()){
        AddAndInitWeight("weight_ttbar_fractions_rw", "", true, false);
      }
      //top pT reweighting
      if(m_vlq_opt->ApplyTtbarNNLOCorrection() && m_vlq_opt->SampleName()!=SampleName::TTBARBB){
        if( !m_vlq_opt -> RecomputeTtbarNNLOCorrection() ){
          AddAndInitWeight("weight_ttbar_NNLO_1L");//input tree
        } else {
          AddAndInitWeight("weight_ttbar_NNLO_1L", "", true, false);
        }
      }
    }//ttbar

    if( ((m_vlq_opt -> StrSampleName().find("W+JETS22") != std::string::npos)
	 || (m_vlq_opt -> StrSampleName().find("Z+JETS22") != std::string::npos))

	&& ((m_vlq_opt -> StrSampleName().find("W+JETS221") == std::string::npos)
	    || (m_vlq_opt -> StrSampleName().find("Z+JETS221") == std::string::npos))
	){
      AddAndInitWeight("weight_WZ_2_2");
    }

  }//!QCD
  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::AddFJvtSFWeights(  ){

  //if(!m_fJvt_calibTool) return true;
  AddAndInitWeight("weight_fJvt", "", true, false);
  AddAndInitWeight("weight_fJvt_UP", "", false, false, "", "weight_fJvt");
  AddAndInitWeight("weight_fJvt_DOWN", "", false, false, "", "weight_fJvt");
  
  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::AddKinReweightings(  ){

  if(!m_kinRw) return true;

  for(const std::pair<std::string, int> kinpair : *(m_kinRw->GetReweightingList())){

    AddAndInitWeight("weight_RW_"+kinpair.first, "", true /*isNominal*/, false /*isInput*/);

  }
  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::AddKinRwSyst(){
  
  if( m_vlq_opt -> ReweightKinematics() && m_vlq_opt->DoKinRwSyst() ){

    if( (m_vlq_outData -> o_is_ttbar) || (m_opt -> SampleName() == SampleName::SINGLETOP) ){

      std::string replace = ( (m_vlq_opt->StrSampleName().find("SINGLETOPSCHAN") != std::string::npos) 
			      || (m_vlq_opt->StrSampleName().find("SINGLETOPTCHAN") != std::string::npos) ) 
	? "" : "weight_RW_MEFFRED";
 
      AddAndInitWeight("weight_RW_MEFFRED_3JEX_UP", "", false, false, "", replace);
      AddAndInitWeight("weight_RW_MEFFRED_3JEX_DOWN", "", false, false, "", replace);

      AddAndInitWeight("weight_RW_MEFFRED_4JEX_UP", "", false, false, "", replace);
      AddAndInitWeight("weight_RW_MEFFRED_4JEX_DOWN", "", false, false, "", replace);

      AddAndInitWeight("weight_RW_MEFFRED_5JEX_UP", "", false, false, "", replace);
      AddAndInitWeight("weight_RW_MEFFRED_5JEX_DOWN", "", false, false, "", replace);

      AddAndInitWeight("weight_RW_MEFFRED_6JEX_UP", "", false, false, "", replace);
      AddAndInitWeight("weight_RW_MEFFRED_6JEX_DOWN", "", false, false, "", replace);

      AddAndInitWeight("weight_RW_MEFFRED_7JIN_UP", "", false, false, "", replace);
      AddAndInitWeight("weight_RW_MEFFRED_7JIN_DOWN", "", false, false, "", replace);

    }
    else{
      AddAndInitWeight("weight_RW_MEFFRED_UP", "", false, false, "", "weight_RW_MEFFRED");
      AddAndInitWeight("weight_RW_MEFFRED_DOWN", "", false, false, "", "weight_RW_MEFFRED");
    }


  } // background reweighting uncertainty  
  
  return true;

}

//______________________________________________________________________________
//
bool VLQ_WeightManager::AddVLQSystematicWeights( bool dump_config ){

  //QCD systematics
  if( dump_config || (m_opt -> StrSampleName().find("QCD") != std::string::npos) ){
    AddAndInitWeight("weight_qcd_el_up","",false, false, "", "weight_qcd");
    AddAndInitWeight("weight_qcd_mu_up","",false, false, "", "weight_qcd");
    return true;
  }
  if(m_vlq_opt->DoExpSys()){
    //JVT systematics
    AddAndInitWeight("weight_jvt_UP", "", false, true, "weight_jvt_JET_JvtEfficiency__1up", "weight_jvt");
    AddAndInitWeight("weight_jvt_DOWN", "", false, true, "weight_jvt_JET_JvtEfficiency__1down", "weight_jvt");

    //PU systematics
    //if(m_vlq_opt->UsePileUpWeight()){
    //AddAndInitWeight("weight_pu_UP", "", false, true, "weight_pu_PRW_DATASF__1up", "weight_pu");
    //AddAndInitWeight("weight_pu_DOWN", "", false, true, "weight_pu_PRW_DATASF__1down", "weight_pu");
    //}

    //Lepton SF systematics
    if(m_vlq_opt->UseLeptonsSF()){
      std::vector<std::string> el_sys_comp = {"Reco", "ID", "Iso" };
      for(const std::string& el_sys : el_sys_comp){
	AddAndInitWeight("weight_elec_"+el_sys+"_UP", "", false, true, "weight_elec_EL_EFF_"+el_sys+"_TOTAL_1NPCOR_PLUS_UNCOR__1up", "weight_elec");
	AddAndInitWeight("weight_elec_"+el_sys+"_DOWN", "", false, true, "weight_elec_EL_EFF_"+el_sys+"_TOTAL_1NPCOR_PLUS_UNCOR__1down", "weight_elec");
      }

      std::map<std::string, std::vector<std::string> > mu_sys_comp = {
	{"RECO", {"STAT", "SYS", "STAT_LOWPT", "SYS_LOWPT"}},
	{"ISO",{"STAT", "SYS"}},
	{"TTVA", {"STAT", "SYS"}}
	//{"BADMUON", {"STAT", "SYS"}}
      };
      for(std::pair<std::string, std::vector<std::string> > mu_sys_pair : mu_sys_comp){
	for(const std::string& mu_sys : mu_sys_pair.second){
	  AddAndInitWeight("weight_muon_"+mu_sys_pair.first+"_"+mu_sys+"_UP","",false, true, "weight_muon_MUON_EFF_"+mu_sys_pair.first+"_"+mu_sys+"__1up", "weight_muon");
	  AddAndInitWeight("weight_muon_"+mu_sys_pair.first+"_"+mu_sys+"_DOWN","",false, true, "weight_muon_MUON_EFF_"+mu_sys_pair.first+"_"+mu_sys+"__1down", "weight_muon");
	}
      }

      if(m_vlq_opt->UseLeptonTrigger()){
	std::vector<std::string> el_trig_sys_comp = {"TriggerEff", "Trigger"};
	for(const std::string& el_trig_sys : el_trig_sys_comp){
	  AddAndInitWeight("weight_elec_"+el_trig_sys+"_UP", "", false, true, "weight_elec_trigger_EL_EFF_"+el_trig_sys+"_TOTAL_1NPCOR_PLUS_UNCOR__1up", "weight_elec_trigger");
	  AddAndInitWeight("weight_elec_"+el_trig_sys+"_DOWN", "", false, true, "weight_elec_trigger_EL_EFF_"+el_trig_sys+"_TOTAL_1NPCOR_PLUS_UNCOR__1down", "weight_elec_trigger");
	}
	std::vector<std::string> mu_trig_sys_comp = {"TrigStatUncertainty", "TrigSystUncertainty"};
	for(const std::string& mu_trig_sys : mu_trig_sys_comp){
	  AddAndInitWeight("weight_muon_"+mu_trig_sys+"_UP", "", false, true, "weight_muon_trigger_MUON_EFF_"+mu_trig_sys+"__1up", "weight_muon_trigger");
	  AddAndInitWeight("weight_muon_"+mu_trig_sys+"_DOWN", "", false, true, "weight_muon_trigger_MUON_EFF_"+mu_trig_sys+"__1down", "weight_muon_trigger");
	}
      }//trigger
    }//lepton SF

    //Btag systematics
    std::string btag_name = (m_vlq_opt->BtagCollection() == VLQ_Options::TRACK) ? "weight_trkbtag" : "weight_btag";
    std::string btag_vartype = "";
    bool btag_isinput = true;

    if((m_vlq_opt -> DoTRF() && m_vlq_opt -> RecomputeTRF()) || m_vlq_opt -> RecomputeBtagSF()){
      btag_isinput = false;
    }

    for(int i = 0; i <=8; i++ ){
      AddAndInitWeight(Form("%s_B_EV_Up_%i",btag_name.c_str(),i), "", false, btag_isinput, Form("%s_FT_EFF_Eigen_B_%i__1up",btag_name.c_str(),i), btag_name);
      AddAndInitWeight(Form("%s_B_EV_Down_%i",btag_name.c_str(),i), "", false, btag_isinput, Form("%s_FT_EFF_Eigen_B_%i__1down",btag_name.c_str(),i), btag_name);
    }

    for(int i = 0; i <=4; i++ ){
      AddAndInitWeight(Form("%s_C_EV_Up_%i",btag_name.c_str(),i), "", false, btag_isinput, Form("%s_FT_EFF_Eigen_C_%i__1up",btag_name.c_str(),i), btag_name);
      AddAndInitWeight(Form("%s_C_EV_Down_%i",btag_name.c_str(),i), "", false, btag_isinput, Form("%s_FT_EFF_Eigen_C_%i__1down",btag_name.c_str(),i), btag_name);
    }

    for(int i = 0; i <=5; i++ ){
      AddAndInitWeight(Form("%s_Light_EV_Up_%i",btag_name.c_str(),i), "", false, btag_isinput, Form("%s_FT_EFF_Eigen_Light_%i__1up",btag_name.c_str(),i), btag_name);
      AddAndInitWeight(Form("%s_Light_EV_Down_%i",btag_name.c_str(),i), "", false, btag_isinput, Form("%s_FT_EFF_Eigen_Light_%i__1down",btag_name.c_str(),i), btag_name);
    }

    AddAndInitWeight(btag_name+"_extrapolation_Up", "", false, btag_isinput, btag_name+"_FT_EFF_extrapolation__1up", btag_name);
    AddAndInitWeight(btag_name+"_extrapolation_Down", "", false, btag_isinput, btag_name+"_FT_EFF_extrapolation__1down", btag_name);

    AddAndInitWeight(btag_name+"_extrapolation_from_charm_Up", "", false, btag_isinput, btag_name+"_FT_EFF_extrapolation_from_charm__1up", btag_name);
    AddAndInitWeight(btag_name+"_extrapolation_from_charm_Down", "", false, btag_isinput, btag_name+"_FT_EFF_extrapolation_from_charm__1down", btag_name);

  }//INSTRUMENTAL UNCERTAINTIES

  if(m_vlq_opt->DoPDFSys()){

    //Using LHAPDF sets
    int baseIdx = 90900; 
    for( int i =1; i<=30; i++ ){
      std::string wname_PDF = Form("weight_pmg_PDFset%i", baseIdx+i);
      AddAndInitWeight(wname_PDF,"",false, true, wname_PDF, "weight_mc")->AddFlagAtBit(REWEIGHT, true);
    }

  }

  if(m_vlq_opt->DoTheorySys()){

    //ttbar and singletop systematics
     if( (m_vlq_outData -> o_is_ttbar) || (m_opt -> SampleName() == SampleName::SINGLETOP) ){
      // PMG weights
      if(m_vlq_outData -> o_is_ttbar){
        AddAndInitWeight("weight_pmg_muR10__muF20","",false, true, "weight_pmg_muR10__muF20", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
        AddAndInitWeight("weight_pmg_muR10__muF05","",false, true, "weight_pmg_muR10__muF05", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
        AddAndInitWeight("weight_pmg_muR20__muF10","",false, true, "weight_pmg_muR20__muF10", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
        AddAndInitWeight("weight_pmg_muR05__muF10","",false, true, "weight_pmg_muR05__muF10", "weight_mc")->AddFlagAtBit(REWEIGHT, true);

      }
      else{
	//Reweightings not yet available
        AddAndInitWeight("weight_pmg_muR10__muF20","",false, true, "weight_pmg_muR100__muF200", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
	AddAndInitWeight("weight_pmg_muR10__muF05","",false, true, "weight_pmg_muR100__muF050", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
	AddAndInitWeight("weight_pmg_muR20__muF10","",false, true, "weight_pmg_muR200__muF100", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
	AddAndInitWeight("weight_pmg_muR05__muF10","",false, true, "weight_pmg_muR050__muF100", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
      }

      AddAndInitWeight("weight_pmg_Var3cUp","",false, true, "weight_pmg_Var3cUp", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
      AddAndInitWeight("weight_pmg_Var3cDown","",false, true, "weight_pmg_Var3cDown", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
      AddAndInitWeight("weight_pmg_isr_muRfac10__fsr_muRfac20","",false, true, "weight_pmg_isr_muRfac10__fsr_muRfac20", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
      AddAndInitWeight("weight_pmg_isr_muRfac10__fsr_muRfac05","",false, true, "weight_pmg_isr_muRfac10__fsr_muRfac05", "weight_mc")->AddFlagAtBit(REWEIGHT, true);

	/*
      //ttbar systematics
      if( m_vlq_outData -> o_is_ttbar ){

	//ttbar NNLO systematic
	if(m_vlq_opt->SampleName()!=SampleName::TTBARBB){
	  if(m_vlq_opt->ApplyTtbarNNLOCorrection()){
	    AddAndInitWeight("weight_ttbar_NNLO_OFF", "", false, false, "", "weight_ttbar_NNLO_1L");
	  } else {
	    AddAndInitWeight("weight_ttbar_NNLO_ON", "", false, true, "weight_ttbar_NNLO_1L", "");
	  }
	}

	//ttbb uncertainties
	if(m_vlq_opt->ApplyTtbbCorrection() && m_vlq_opt->SampleName()==SampleName::TTBARBB){
	  std::vector<std::string> ttbb_sys_comp = {"CSS_KIN", "MSTW", "NNPDF", "Q_CMMPS", "glosoft", "defaultX05", "defaultX2", "MPIup", "MPIdown", "MPIfactor", "aMcAtNloHpp", "aMcAtNloPy8"};
	  for(const std::string& ttbb_sys : ttbb_sys_comp){
	    AddAndInitWeight("weight_ttbb_"+ttbb_sys, "", false, true, "weight_ttbb_ttbb_"+ttbb_sys+"_weight", "weight_ttbb");
	  }
	}//ttbb correction

	//ttcc uncertainties
	if( m_vlq_opt->ComputeWeightSys() && m_vlq_opt -> ComputeTtccNLO()){
	  AddAndInitWeight("weight_ttcc_NLO", "", false, false);
	}

      }//ttbar samples
	*/ //IF ANYONE EVER WANTS TO REVIVE

    }//ttbar or singletop samples

    //V+jets systematics
    if( (m_opt -> SampleName() == SampleName::WJETS) || (m_opt -> SampleName() == SampleName::ZJETS) ){
      //V+jets PMG weights
      AddAndInitWeight("weight_pmg_muR05__muF05","",false, true, "weight_pmg_MUR05__MUF05__PDF261000", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
      AddAndInitWeight("weight_pmg_muR05__muF10","",false, true, "weight_pmg_MUR05__MUF1__PDF261000", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
      AddAndInitWeight("weight_pmg_muR10__muF05","",false, true, "weight_pmg_MUR1__MUF05__PDF261000", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
      AddAndInitWeight("weight_pmg_muR10__muF10","",false, true, "weight_pmg_MUR1__MUF1__PDF261000", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
      AddAndInitWeight("weight_pmg_muR10__muF20","",false, true, "weight_pmg_MUR1__MUF2__PDF261000", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
      AddAndInitWeight("weight_pmg_muR20__muF10","",false, true, "weight_pmg_MUR2__MUF1__PDF261000", "weight_mc")->AddFlagAtBit(REWEIGHT, true);
      AddAndInitWeight("weight_pmg_muR20__muF20","",false, true, "weight_pmg_MUR2__MUF2__PDF261000", "weight_mc")->AddFlagAtBit(REWEIGHT, true);

      //parametrised weights
      AddAndInitWeight("weight_pmg_ckkw15","",false, true, "ckkw15_Weight", "weight_mc","F")->AddFlagAtBit(REWEIGHT, true);
      AddAndInitWeight("weight_pmg_ckkw30","",false, true, "ckkw30_Weight", "weight_mc","F")->AddFlagAtBit(REWEIGHT, true);
      AddAndInitWeight("weight_pmg_qsf025","",false, true, "qsf025_Weight", "weight_mc","F")->AddFlagAtBit(REWEIGHT, true);
      AddAndInitWeight("weight_pmg_qsf4","",false, true, "qsf4_Weight", "weight_mc","F")->AddFlagAtBit(REWEIGHT, true);

    }

  }//theory sys

  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::SetLeptonSFWeights( const bool apply_trigger_weights ){
  bool apply_weights = true;
  if ( m_vlq_outData-> o_channel_type != VLQ_Enums::ELECTRON && m_vlq_outData-> o_channel_type != VLQ_Enums::MUON ){
    apply_weights = false;
  }

  //Setting the nominal lepton weights to 1
  if(!apply_weights){
    for( auto& weight : *m_nomMap ){
      if( weight.first.find("weight_elec") != std::string::npos ||
          weight.first.find("weight_muon") != std::string::npos
	  ){
        UpdateNominalComponent(weight.first, 1.);
      }
    }
  }
  //Setting the nominal trigger weight to 1 in case the event is trigger by met trigger
  else if(!apply_trigger_weights){
    for( auto& weight : *m_nomMap ){
      if( weight.first.find("weight_elec_trigger") != std::string::npos ||
          weight.first.find("weight_muon_trigger") != std::string::npos
	  ){
        UpdateNominalComponent(weight.first, 1.);
      }
    }
  }

  //Setting the systematics lepton weights to 1
  if(!apply_weights){
    for( auto& weight : *m_systMap ){
      if( weight.first.find("weight_elec") != std::string::npos ||
          weight.first.find("weight_muon") != std::string::npos
	  ){
        UpdateSystematicComponent(weight.first, 1.);
      }
    }
  }
  //Setting the nominal trigger weight to 1 in case the event is trigger by met trigger
  else if(!apply_trigger_weights){
    for( auto& weight : *m_systMap ){
      if( weight.first.find("weight_elec_trigger") != std::string::npos ||
          weight.first.find("weight_muon_trigger") != std::string::npos
	  ){
        UpdateSystematicComponent(weight.first, 1.);
      }
    }
  }
  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::SetTtbarHtSliceScale(){

  // CURRENTLY NOT IN USE -- Ttbar Ht scaling happening in VLQ_Analysis_Data2015 in Terminate() function
  /*
    if ( m_opt -> StrSampleID().find("407344.") != std::string::npos ){
    UpdateNominalComponent("weight_mc", (*m_nomMap)["weight_mc"]->GetComponentValue()/0.99860961239196);
    if(m_vlq_opt->ComputeWeightSys() && m_vlq_opt->DoTheorySys()){
    UpdateSystematicComponent("weight_pmg_Var3cDown", (*m_systMap)["weight_pmg_Var3cDown"]->GetComponentValue()/1.00989117643996);
    UpdateSystematicComponent("weight_pmg_Var3cUp", (*m_systMap)["weight_pmg_Var3cUp"]->GetComponentValue()/0.98630942849818);
    UpdateSystematicComponent("weight_pmg_isr_muRfac10__fsr_muRfac05", (*m_systMap)["weight_pmg_isr_muRfac10__fsr_muRfac05"]->GetComponentValue()/1.01244857328796);
    UpdateSystematicComponent("weight_pmg_isr_muRfac10__fsr_muRfac20", (*m_systMap)["weight_pmg_isr_muRfac10__fsr_muRfac20"]->GetComponentValue()/0.98661128202278);
    UpdateSystematicComponent("weight_pmg_muR05__muF10", (*m_systMap)["weight_pmg_muR05__muF10"]->GetComponentValue()/0.96762126478968);
    UpdateSystematicComponent("weight_pmg_muR10__muF05", (*m_systMap)["weight_pmg_muR10__muF05"]->GetComponentValue()/0.97391612230213);
    UpdateSystematicComponent("weight_pmg_muR10__muF20", (*m_systMap)["weight_pmg_muR10__muF20"]->GetComponentValue()/1.02221909345704);
    UpdateSystematicComponent("weight_pmg_muR20__muF10", (*m_systMap)["weight_pmg_muR20__muF10"]->GetComponentValue()/1.02018541245750);
    }
    }
    if ( m_opt -> StrSampleID().find("407343.") != std::string::npos ){
    UpdateNominalComponent("weight_mc", (*m_nomMap)["weight_mc"]->GetComponentValue()/1.00220071443736);
    if(m_vlq_opt->ComputeWeightSys() && m_vlq_opt->DoTheorySys()){
    UpdateSystematicComponent("weight_pmg_Var3cDown", (*m_systMap)["weight_pmg_Var3cDown"]->GetComponentValue()/1.02075306498765);
    UpdateSystematicComponent("weight_pmg_Var3cUp", (*m_systMap)["weight_pmg_Var3cUp"]->GetComponentValue()/0.98196927321205);
    UpdateSystematicComponent("weight_pmg_isr_muRfac10__fsr_muRfac05", (*m_systMap)["weight_pmg_isr_muRfac10__fsr_muRfac05"]->GetComponentValue()/1.03193538833724);
    UpdateSystematicComponent("weight_pmg_isr_muRfac10__fsr_muRfac20", (*m_systMap)["weight_pmg_isr_muRfac10__fsr_muRfac20"]->GetComponentValue()/0.99087003186616);
    UpdateSystematicComponent("weight_pmg_muR05__muF10", (*m_systMap)["weight_pmg_muR05__muF10"]->GetComponentValue()/0.91951515882506);
    UpdateSystematicComponent("weight_pmg_muR10__muF05", (*m_systMap)["weight_pmg_muR10__muF05"]->GetComponentValue()/0.93819392248974);
    UpdateSystematicComponent("weight_pmg_muR10__muF20", (*m_systMap)["weight_pmg_muR10__muF20"]->GetComponentValue()/1.06487669659290);
    UpdateSystematicComponent("weight_pmg_muR20__muF10", (*m_systMap)["weight_pmg_muR20__muF10"]->GetComponentValue()/1.06891808020899);
    }
    }
    if ( m_opt -> StrSampleID().find("407342.") != std::string::npos ){
    UpdateNominalComponent("weight_mc", (*m_nomMap)["weight_mc"]->GetComponentValue()/1.01614066637173);
    if(m_vlq_opt->ComputeWeightSys() && m_vlq_opt->DoTheorySys()){
    UpdateSystematicComponent("weight_pmg_Var3cDown", (*m_systMap)["weight_pmg_Var3cDown"]->GetComponentValue()/1.04057257636042);
    UpdateSystematicComponent("weight_pmg_Var3cUp", (*m_systMap)["weight_pmg_Var3cUp"]->GetComponentValue()/0.99097386133341);
    UpdateSystematicComponent("weight_pmg_isr_muRfac10__fsr_muRfac05", (*m_systMap)["weight_pmg_isr_muRfac10__fsr_muRfac05"]->GetComponentValue()/1.06280582379557);
    UpdateSystematicComponent("weight_pmg_isr_muRfac10__fsr_muRfac20", (*m_systMap)["weight_pmg_isr_muRfac10__fsr_muRfac20"]->GetComponentValue()/1.00957616786319);
    UpdateSystematicComponent("weight_pmg_muR05__muF10", (*m_systMap)["weight_pmg_muR05__muF10"]->GetComponentValue()/0.89300718054136);
    UpdateSystematicComponent("weight_pmg_muR10__muF05", (*m_systMap)["weight_pmg_muR10__muF05"]->GetComponentValue()/0.90726044083483);
    UpdateSystematicComponent("weight_pmg_muR10__muF20", (*m_systMap)["weight_pmg_muR10__muF20"]->GetComponentValue()/1.12725897511274);
    UpdateSystematicComponent("weight_pmg_muR20__muF10", (*m_systMap)["weight_pmg_muR20__muF10"]->GetComponentValue()/1.12572488013792);
    }
    }
  */

  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::SetCrossSectionWeight(){
  if(!m_sampleInfo){
    std::cerr << "<!> Error in VLQ_WeightManager::SetCrossSectionWeight(): m_sampleInfo is null ... Please check !" << std::endl;
    abort();
  }
  if( m_vlq_opt -> IsData() || m_opt -> StrSampleName().find("QCD") != std::string::npos ){
    return false;
  }
  SetNominalComponent( "weight_norm", m_sampleInfo -> NormFactor("" ,1.0) );
  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::SetFJvtSFWeights(){

  if( m_vlq_opt -> IsData() || m_opt -> StrSampleName().find("QCD") != std::string::npos ){
    return false;
  }
  if( !m_vlq_opt->DoFJvtSFWeights() || !m_fJvt_calibTool ) return true;

  SetNominalComponent( "weight_fJvt", m_fJvt_calibTool -> GetFJVTEventWeight() );
  SetSystematicComponent( "weight_fJvt_UP", m_fJvt_calibTool -> GetFJVTEventWeight(1.) );
  SetSystematicComponent( "weight_fJvt_DOWN", m_fJvt_calibTool -> GetFJVTEventWeight(-1.) );
  

  return true;

}


//______________________________________________________________________________
//
bool VLQ_WeightManager::SetQCDWeight(){

  if( m_opt -> StrSampleName().find("QCD") == std::string::npos ){
    return false;
  }
  bool isElectronChannel = ( m_vlq_outData -> o_channel_type == VLQ_Enums::ELECTRON );
  bool isMuonChannel     = ( m_vlq_outData -> o_channel_type == VLQ_Enums::MUON );
  if( !isElectronChannel && !isMuonChannel ){
    SetNominalComponent("weight_qcd", 0. );
    return false;
  }
  if(!m_qcdWeight){
    std::cerr << "<!> Error in VLQ_WeightManager::SetQCDWeight(): m_qcdWeight is null ... Please check !" << std::endl;
    abort();
  }

  std::vector < TLorentzVector > v_ele, v_mu, v_ele_loose, v_mu_loose, v_jets;
  std::vector < float > v_topo_el, v_topo_mu;
  for ( const AnalysisObject* ele : *(m_vlq_outData->o_el) ) {
    TLorentzVector temp_el;
    temp_el.SetPtEtaPhiE( ele->Pt(), ele->Eta(), ele->Phi(), ele->E() );
    v_topo_el.push_back((float)ele->GetMoment("topoetcone20"));
    v_ele_loose.push_back(temp_el);
    if((int)ele->GetMoment("isSignal")) v_ele.push_back(temp_el);
  }
  for ( const AnalysisObject* mu : *(m_vlq_outData->o_mu) ) {
    TLorentzVector temp_mu;
    temp_mu.SetPtEtaPhiE( mu->Pt(), mu->Eta(), mu->Phi(), mu->E() );
    v_topo_mu.push_back((float)mu->GetMoment("topoetcone20"));
    v_mu_loose.push_back(temp_mu);
    if((int)mu->GetMoment("isSignal")) v_mu.push_back(temp_mu);
  }
  for ( const AnalysisObject* jet : *(m_vlq_outData->o_jets) ) {
    TLorentzVector temp_jet;
    temp_jet.SetPtEtaPhiE( jet->Pt(), jet->Eta(), jet->Phi(), jet->E() );
    v_jets.push_back(temp_jet);
  }
  const double qcdWeight = m_qcdWeight->RetrieveWeightFromVectors( v_ele, v_mu, v_ele_loose, v_mu_loose, v_topo_el, v_topo_mu, v_jets, isElectronChannel, m_vlq_outData -> o_run_number);
  SetNominalComponent("weight_qcd", qcdWeight );

  if(m_vlq_opt->ComputeWeightSys()){
    const double qdcUnc = m_qcdWeight->GetSystematicWeight();
    if(isElectronChannel){
      SetSystematicComponent( "weight_qcd_el_up", qcdWeight*qdcUnc );
      SetSystematicComponent( "weight_qcd_mu_up", qcdWeight );
    } else {
      SetSystematicComponent( "weight_qcd_el_up", qcdWeight );
      SetSystematicComponent( "weight_qcd_mu_up", qcdWeight*qdcUnc );
    }
  }

  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::SetPMGSystNorm(){

  if(!m_vlq_opt->ComputeWeightSys()){
    return true;
  }
  double nev_nom = m_sampleInfo->NWeightedEvents("nominal");
  for( auto& sysweight :  *m_systMap ){

    if(sysweight.first.find("pmg") == std::string::npos) continue;

    const string& branchName = (sysweight.second)->BranchName();
    double nev_sys = m_sampleInfo->NWeightedEvents("sumOfWeights_"+branchName, true /*ignore branch if missing*/ );

    double sys_factor = (nev_sys > 0.) ? nev_nom/nev_sys : 1.;

    if((sysweight.second)->GetComponentValue() > m_sampleInfo->WeightThreshold(branchName, false)){
      if(m_vlq_opt -> MsgLevel() == Debug::DEBUG){
	std::cout << "Systematic component " << sysweight.first << " = " << (sysweight.second)->GetComponentValue() 
		  << " > " << m_sampleInfo->WeightThreshold(branchName, false) << ". Setting to 0." << std::endl;
      }
      UpdateSystematicComponent(sysweight.first, 0);
    }
    else{
      UpdateSystematicComponent(sysweight.first, (sysweight.second)->GetComponentValue()*sys_factor);
    }
    
  }

  if(m_vlq_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After SetPMGSystNorm weights" << std::endl;

  return true;

}

//______________________________________________________________________________
//
bool VLQ_WeightManager::SetTtccWeights(){

  if(!m_vlq_opt->ComputeWeightSys() || !m_vlq_opt -> ComputeTtccNLO()){
    return true;
  }

  if( m_vlq_outData -> o_is_ttbar ){

    //
    // NLO tt+cc
    //
    float ttccNLO_weight=ttccNLO_syst::CharmRW(m_vlq_ntupData -> d_HF_Classification, m_vlq_ntupData->d_q1_pt, m_vlq_ntupData->d_q1_eta, m_vlq_ntupData->d_qq_pt, m_vlq_ntupData->d_qq_dr);

    SetSystematicComponent("weight_ttcc_NLO", ttccNLO_weight);

    if(m_vlq_opt -> MsgLevel() == Debug::DEBUG) std::cout << "==> After TtCc weights" << std::endl;

  } else{
    return false;
  }
  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::SetTtbarGeneratorSystematics( const std::string &region ){
  if(!m_vlq_opt->ComputeWeightSys()){
    return true;
  }
  if( m_vlq_outData -> o_is_ttbar ){

    if(!m_ttbar_syst_weight){
      std::cerr << "<!> Error in VLQ_WeightManager::SetTtbarGeneratorSystematics(): m_ttbar_syst_weight is null ... Please check !" << std::endl;
      abort();
    }

    SetSystematicComponent( "weight_ttbar_PS",    m_ttbar_syst_weight -> GetTtbarSystWeight( region, VLQ_TtbarSystematicsManager::PS)      );
    SetSystematicComponent( "weight_ttbar_GEN",   m_ttbar_syst_weight -> GetTtbarSystWeight( region, VLQ_TtbarSystematicsManager::GEN)     );
    SetSystematicComponent( "weight_ttbar_GENPS", m_ttbar_syst_weight -> GetTtbarSystWeight( region, VLQ_TtbarSystematicsManager::GENPS)   );
    SetSystematicComponent( "weight_ttbar_RADHI", m_ttbar_syst_weight -> GetTtbarSystWeight( region, VLQ_TtbarSystematicsManager::RADHI)   );
    SetSystematicComponent( "weight_ttbar_RADLOW",m_ttbar_syst_weight -> GetTtbarSystWeight( region, VLQ_TtbarSystematicsManager::RADLOW)  );
  } else {
    return false;
  }
  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::SetNNLOSystematic(){
  if(!m_vlq_opt->ComputeWeightSys()){
    return true;
  }
  if( m_vlq_outData -> o_is_ttbar && m_vlq_opt->SampleName()!=SampleName::TTBARBB ){
    SetSystematicComponent( "weight_ttbar_NNLO_OFF", 1. );
  } else {
    return false;
  }
  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::SetTtbarFractionWeight(){
  if(m_vlq_opt->ReweightTtbarFractions()){
    double weight = m_tool_ttFractionRw->GetTtbarFraction_Rw(m_vlq_ntupData -> d_HF_SimpleClassification);
    SetNominalComponent("weight_ttbar_fractions_rw", weight );
  }
  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::SetNNLOWeight( const double topPt ){

  if( m_vlq_outData -> o_is_ttbar && m_vlq_opt->RecomputeTtbarNNLOCorrection()){
    const double result = 1.; //m_nnlo_rw->GetExtendedTopPtWeight( topPt * 1000.) ;
    //std::cout << "result = " << result << std::endl;

    SetNominalComponent("weight_ttbar_NNLO_1L", result) ;
  }
  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::SetKinReweightings(  ){

  //if(!m_vlq_opt->ComputeWeightSys()){

  if( !m_vlq_opt -> ReweightKinematics() || !m_kinRw ){
    return true;
  }

  //if(!m_kinRw){
  //std::cerr << "<!> Error in VLQ_WeightManager::SetKinReweightings(): m_kinRw is null ... Please check !" << std::endl;
  //abort();
  //}

  for(const std::pair<std::string, int> kinpair : *(m_kinRw->GetReweightingList())){

    double weight = m_kinRw -> GetKinReweight( kinpair.second );

    if((m_vlq_opt -> StrSampleName().find("POWHER") != std::string::npos)){
      weight *= m_kinRw->GetKinReweight(kinpair.second, "_PowHer");
    }
    else if((m_vlq_opt -> StrSampleName().find("AMCPY") != std::string::npos)){
      weight *= m_kinRw->GetKinReweight(kinpair.second, "_aMCPy");
    }
    else if(m_vlq_opt -> ISAFII()){
      weight *= m_kinRw->GetKinReweight(kinpair.second, "_AFII");
    }
    else if(m_vlq_opt -> ISDIAGSUB()){
      weight *= m_kinRw->GetKinReweight(kinpair.second, "_DiagSub");
    }


    SetNominalComponent( "weight_RW_"+kinpair.first, weight );

  }

  return true;
}

//______________________________________________________________________________
//
bool VLQ_WeightManager::SetKinRwSyst(){

  double weight_up = 1.;
  double weight_dn = 1.;
  double weight_nom = 1.;

  if(m_kinRw){
    weight_nom = GetWeightObject("weight_RW_MEFFRED")->GetComponentValue();
    weight_up = m_kinRw->GetKinRwSyst("_upBand2S");
    weight_dn = m_kinRw->GetKinRwSyst("_dnBand2S");
  }

  std::string wname = "weight_RW_MEFFRED";
  if( (m_vlq_outData -> o_is_ttbar) || (m_opt -> SampleName() == SampleName::SINGLETOP) ){

    SetSystematicComponent("weight_RW_MEFFRED_3JEX_UP", weight_nom);
    SetSystematicComponent("weight_RW_MEFFRED_3JEX_DOWN", weight_nom);

    SetSystematicComponent("weight_RW_MEFFRED_4JEX_UP", weight_nom);
    SetSystematicComponent("weight_RW_MEFFRED_4JEX_DOWN", weight_nom);

    SetSystematicComponent("weight_RW_MEFFRED_5JEX_UP", weight_nom);
    SetSystematicComponent("weight_RW_MEFFRED_5JEX_DOWN", weight_nom);

    SetSystematicComponent("weight_RW_MEFFRED_6JEX_UP", weight_nom);
    SetSystematicComponent("weight_RW_MEFFRED_6JEX_DOWN", weight_nom);

    SetSystematicComponent("weight_RW_MEFFRED_7JIN_UP", weight_nom);
    SetSystematicComponent("weight_RW_MEFFRED_7JIN_DOWN", weight_nom);


    if( m_vlq_outData -> o_jets_n == 3 ) wname += "_3JEX";
    else if( m_vlq_outData -> o_jets_n == 4 ) wname += "_4JEX";
    else if( m_vlq_outData -> o_jets_n == 5 ) wname += "_5JEX";
    else if( m_vlq_outData -> o_jets_n == 6 ) wname += "_6JEX";
    else if( m_vlq_outData -> o_jets_n >= 7 ) wname += "_7JIN";

    UpdateSystematicComponent(wname+"_UP", weight_up);
    UpdateSystematicComponent(wname+"_DOWN", weight_dn);
    
  }
  else{
    SetSystematicComponent(wname+"_UP", weight_up);
    SetSystematicComponent(wname+"_DOWN", weight_dn);    
  }


  return true;

}

//______________________________________________________________________________
//
bool VLQ_WeightManager::UpdateSysReweighting(){

  if(!m_kinRw) return true;

  for( auto& weight : *m_systMap){

    if( (weight.second)->PassFlagAtBit(REWEIGHT) ){

      double oldWeight = weight.second->GetComponentValue();
      for(const std::pair<std::string, int> kinpair : *(m_kinRw->GetReweightingList())){
	oldWeight *= m_kinRw->GetKinReweight(kinpair.second, "_"+weight.first);
      }

      UpdateSystematicComponent(weight.first, oldWeight);

    }

  }

  return true;
}
