#include <iostream>
#include <algorithm>
#include <ctype.h>
//#include <locale>
#include "IFAETopFramework/AnalysisUtils.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/OutputHistManager.h"
#include "IFAETopFramework/OutputTreeManager.h"
#include "IFAETopFramework/Selection.h"

#include "VLQAnalysis/VLQ_NtupleData.h"
#include "VLQAnalysis/VLQ_OutputData.h"
#include "VLQAnalysis/VLQ_Options.h"
#include "VLQAnalysis/VLQ_AnalysisTools.h"

#include "VLQAnalysis/VLQ_Selector.h"
#include "VLQAnalysis/VLQ_Enums.h"

//______________________________________________________________________________
//
VLQ_Selector::VLQ_Selector( VLQ_Options *opt, const VLQ_NtupleData *ntupData,
                            VLQ_OutputData *outData, VLQ_AnalysisTools *anaTools,
                            const bool useDecisions, const bool add_primaries ):
  SelectorBase((OptionsBase*)opt, (OutputData*)outData, useDecisions, add_primaries),
  m_opt(opt),
  m_ntupData(ntupData),
  m_outData(outData),
  m_anaTools(anaTools),
  m_outMngrHist(0),
  m_sel_indices(NULL),
  m_sel_names(NULL),
  m_sel_lep_prop(NULL),
  m_sel_lepflav_prop(NULL),
  m_sel_jet_prop(NULL),
  m_sel_fwdjet_prop(NULL),
  m_sel_bjet_prop(NULL),
  m_sel_J_prop(NULL),
  m_sel_M_prop(NULL),
  m_sel_T_prop(NULL),
  m_sel_L_prop(NULL),
  m_sel_H_prop(NULL),
  m_sel_V_prop(NULL),
  m_sel_TH_prop(NULL),
  m_sel_LT_prop(NULL),
  m_sel_VT_prop(NULL),
  m_sel_VLT_prop(NULL),
  m_sel_VLTH_prop(NULL),
  m_sel_Mbb_prop(NULL),
  m_sel_Mtb_prop(NULL),
  m_sel_MetSig_prop(NULL),
  m_sel_MLL_prop(NULL),
  m_sel_MVAScore_prop(NULL),
  m_sel_MetCut_prop(NULL)
{

}

//______________________________________________________________________________
//
VLQ_Selector::VLQ_Selector( const VLQ_Selector &q ):
  SelectorBase(q)
{
  m_opt                 = q.m_opt;
  m_ntupData            = q.m_ntupData;
  m_outData             = q.m_outData;
  m_anaTools            = q.m_anaTools;
  m_outMngrHist         = q.m_outMngrHist;
  m_sel_indices         = new std::map<std::string, int>(*(q.m_sel_indices));
  m_sel_names           = new std::map<int, std::string>(*(q.m_sel_names));
  m_sel_lep_prop        = new std::vector<SelProp>(*(q.m_sel_lep_prop));
  m_sel_lepflav_prop    = new std::vector<SelProp>(*(q.m_sel_lepflav_prop));
  m_sel_jet_prop        = new std::vector<SelProp>(*(q.m_sel_jet_prop));
  m_sel_fwdjet_prop     = new std::vector<SelProp>(*(q.m_sel_fwdjet_prop));
  m_sel_bjet_prop       = new std::vector<SelProp>(*(q.m_sel_bjet_prop));
  m_sel_J_prop          = new std::vector<SelProp>(*(q.m_sel_J_prop));
  m_sel_M_prop          = new std::vector<SelProp>(*(q.m_sel_M_prop));
  m_sel_T_prop          = new std::vector<SelProp>(*(q.m_sel_T_prop));
  m_sel_L_prop          = new std::vector<SelProp>(*(q.m_sel_L_prop));
  m_sel_H_prop          = new std::vector<SelProp>(*(q.m_sel_H_prop));
  m_sel_V_prop          = new std::vector<SelProp>(*(q.m_sel_V_prop));
  m_sel_TH_prop         = new std::vector<SelProp>(*(q.m_sel_TH_prop));
  m_sel_LT_prop         = new std::vector<SelProp>(*(q.m_sel_LT_prop));
  m_sel_VT_prop         = new std::vector<SelProp>(*(q.m_sel_VT_prop));
  m_sel_VLT_prop        = new std::vector<SelProp>(*(q.m_sel_VLT_prop));
  m_sel_VLTH_prop        = new std::vector<SelProp>(*(q.m_sel_VLTH_prop));
  m_sel_Mbb_prop        = new std::vector<SelProp>(*(q.m_sel_Mbb_prop));
  m_sel_Mtb_prop        = new std::vector<SelProp>(*(q.m_sel_Mtb_prop));
  m_sel_MetSig_prop     = new std::vector<SelProp>(*(q.m_sel_MetSig_prop));
  m_sel_MetCut_prop     = new std::vector<SelProp>(*(q.m_sel_MetCut_prop));
  m_sel_MLL_prop        = new std::vector<SelProp>(*(q.m_sel_MLL_prop));
  m_sel_MVAScore_prop   = new std::vector<SelProp>(*(q.m_sel_MVAScore_prop));
}

//______________________________________________________________________________
//
VLQ_Selector::~VLQ_Selector(){
  delete m_sel_indices;
}

//______________________________________________________________________________
//
bool VLQ_Selector::Init(){
  if(m_opt->MsgLevel() == Debug::DEBUG){
    std::cout<<" Inside VLQ_Selector::Init()"<<std::endl;
  }

  m_blinding_config.open( Form("%s/python/VLQAnalysis/regions_dictionary.py", std::getenv("VLQAnalysisFramework_DIR")) );

  //============================  Initialise top selections and add rules for primary ancestors ==================================
  m_sel_indices = new std::map<std::string, int>();
  m_sel_names = new std::map<int, std::string>();

  m_sel_lep_prop = new std::vector<SelProp>({
      MakeSelProp("all", c_all),
	MakeSelProp("1lep", c_1l_chan) , MakeSelProp("0lep", c_0l_chan), MakeSelProp("2lep", c_2l_chan) });

  m_sel_lepflav_prop = new std::vector<SelProp>({
      MakeSelProp("1el", c_1el_chan) , MakeSelProp("1mu", c_1mu_chan)
	, MakeSelProp("ee", c_ee_chan), MakeSelProp("mumu", c_mumu_chan), MakeSelProp("emu", c_emu_chan), MakeSelProp("sf", c_sf_chan) });

  m_sel_jet_prop = new std::vector<SelProp>({
      MakeSelProp("2jin", c_2jin, ""),MakeSelProp("3jin", c_3jin, "2jin")
	,MakeSelProp("3jex", c_3jex, "3jin"),MakeSelProp("4jin", c_4jin, "3jin")
	,MakeSelProp("4jex", c_4jex, "4jin"), MakeSelProp("5jin", c_5jin, "4jin")
	,MakeSelProp("5jex", c_5jex, "5jin"), MakeSelProp("6jin", c_6jin, "5jin")
	,MakeSelProp("6_8jwin", c_6_8jwin, "6jin"), MakeSelProp("7jin", c_7jin, "6jin")
	,MakeSelProp("6jex", c_6jex, "6jin")
	,MakeSelProp("3_5jwin", c_3_5jwin, "3jin")
	,MakeSelProp("7jex", c_7jex, "7jin"), MakeSelProp("8jin", c_8jin, "7jin")
	,MakeSelProp("8jex", c_8jex, "8jin"), MakeSelProp("9jin", c_9jin, "8jin")
	,MakeSelProp("9jex", c_9jex, "9jin"), MakeSelProp("10jin", c_10jin, "9jin")
	,MakeSelProp("10jex", c_10jex, "10jin"), MakeSelProp("11jin", c_11jin, "10jin")
	,MakeSelProp("11jex", c_11jex, "11jin"), MakeSelProp("12jin", c_12jin, "11jin") });

  m_sel_fwdjet_prop = new std::vector<SelProp>({
      MakeSelProp("0fjex",c_0fjex), MakeSelProp("0fjin",c_0fjin), MakeSelProp("1fjex", c_1fjex),
	MakeSelProp("1fjin", c_1fjin), MakeSelProp("2fjin", c_2fjin) });

  m_sel_bjet_prop = new std::vector<SelProp>({
      MakeSelProp("0bex", c_0bex, ""), MakeSelProp("0bin", c_0bin, ""), MakeSelProp("1bin", c_1bin, "")
	,MakeSelProp("1bex", c_1bex, "1bin"), MakeSelProp("2bin", c_2bin, "1bin")
	,MakeSelProp("2bex", c_2bex, "2bin"), MakeSelProp("3bin", c_3bin, "2bin")
	,MakeSelProp("3bex", c_3bex, "3bin"), MakeSelProp("4bin", c_4bin, "3bin")
	,MakeSelProp("1_2bwin", c_1_2bwin, "1bin") });

  //**** NOTE : Ancestors are not currently implemented for boosted categories ******

  m_sel_J_prop = new std::vector<SelProp>({
      MakeSelProp("0Jex",c_0Jex), MakeSelProp("1Jex", c_1Jex), MakeSelProp("1Jin", c_1Jin),
	MakeSelProp("2Jex", c_2Jex), MakeSelProp("2Jin", c_2Jin), MakeSelProp("3Jin", c_3Jin),
	MakeSelProp("0_2Jwin0Hex1Tex",c_0_2Jwin0Hex1Tex)});

  m_sel_M_prop = new std::vector<SelProp>({
      MakeSelProp("0Mex",c_0Mex), MakeSelProp("1Mex", c_1Mex), MakeSelProp("1Min", c_1Min), MakeSelProp("2Min", c_2Min), 
	MakeSelProp("2Min3Jin", c_2Min3Jin), MakeSelProp("2Min3Jin0Hex", c_2Min3Jin0Hex), MakeSelProp("2Min3Jin1Hin", c_2Min3Jin1Hin),
	MakeSelProp("2Min0Hex", c_2Min0Hex), MakeSelProp("2Min1Hin",c_2Min1Hin),
	MakeSelProp("1Mex3Jin", c_1Mex3Jin), MakeSelProp("1Mex3Jin0Hex", c_1Mex3Jin0Hex), MakeSelProp("1Mex3Jin1Hin", c_1Mex3Jin1Hin),
	MakeSelProp("1Mex0Hex1Tex", c_1Mex0Hex1Tex)});

  m_sel_T_prop = new std::vector<SelProp>({
      MakeSelProp("0Tin",c_0Tin), MakeSelProp("0Tex",c_0Tex), MakeSelProp("0_1Twin",c_0_1Twin), MakeSelProp("1Tin",c_1Tin), MakeSelProp("1Tex", c_1Tex), MakeSelProp("2Tin", c_2Tin) });

  m_sel_L_prop = new std::vector<SelProp>({
      MakeSelProp("0Lin",c_0Lin), MakeSelProp("0Lex",c_0Lex), MakeSelProp("1Lin",c_1Lin), MakeSelProp("1Lex", c_1Lex), MakeSelProp("0_1Lwin",  c_0_1Lwin),
	MakeSelProp("2Lin", c_2Lin) });

  m_sel_H_prop = new std::vector<SelProp>({
      MakeSelProp("0Hex",c_0Hex), MakeSelProp("1Hex", c_1Hex), MakeSelProp("1Hin", c_1Hin), MakeSelProp("2Hin", c_2Hin), MakeSelProp("0_1Hwin", c_0_1Hwin) });

  m_sel_V_prop = new std::vector<SelProp>({
      MakeSelProp("0Vex",c_0Vex), MakeSelProp("1Vex", c_1Vex), MakeSelProp("0_1Vwin", c_0_1Vwin), MakeSelProp("1Vin", c_1Vin), MakeSelProp("2Vin", c_2Vin), MakeSelProp("3Vin", c_3Vin) });

  m_sel_TH_prop = new std::vector<SelProp>({
      MakeSelProp("0THex",c_0THex), MakeSelProp("1THex", c_1THex), MakeSelProp("1THin", c_1THin), MakeSelProp("2THex", c_2THex), MakeSelProp("2THin", c_2THin), MakeSelProp("3THin", c_3THin) });

  m_sel_LT_prop = new std::vector<SelProp>({
      MakeSelProp("0LTex",c_0LTex), MakeSelProp("1LTex", c_1LTex), MakeSelProp("1LTin", c_1LTin), MakeSelProp("2LTin", c_2LTin) });

  m_sel_VT_prop = new std::vector<SelProp>({
      MakeSelProp("0VTex",c_0VTex), MakeSelProp("1VTex", c_1VTex), MakeSelProp("0_1VTwin", c_0_1VTwin), MakeSelProp("1VTin", c_1VTin), MakeSelProp("2VTin", c_2VTin), MakeSelProp("2VTor", c_2VTor) });

  m_sel_VLT_prop = new std::vector<SelProp>({
      MakeSelProp("0VLTex",c_0VLTex), MakeSelProp("1VLTex", c_1VLTex), MakeSelProp("1VLTin", c_1VLTin), MakeSelProp("2VLTin", c_2VLTin) });

  m_sel_VLTH_prop = new std::vector<SelProp>({
      MakeSelProp("0VLTHex",c_0VLTHex), MakeSelProp("1VLTHin", c_1VLTHin) });

  m_sel_Mbb_prop = new std::vector<SelProp>({
      MakeSelProp("LowMbb",c_LowMbb), MakeSelProp("HighMbb", c_HighMbb) });

  m_sel_Mtb_prop = new std::vector<SelProp>({
      MakeSelProp("LowMtbmin",c_LowMtbmin), MakeSelProp("HighMtbmin", c_HighMtbmin) });

  m_sel_MetSig_prop = new std::vector<SelProp>({
      MakeSelProp("LowMetSig",c_LowMetSig), MakeSelProp("HighMetSig", c_HighMetSig) });
  m_sel_MetCut_prop = new std::vector<SelProp>({
      MakeSelProp("LowMetCut",c_LowMetCut), MakeSelProp("HighMetCut", c_HighMetCut) });
  m_sel_MLL_prop = new std::vector<SelProp>({
      MakeSelProp("HighMLL",c_HighMLL), MakeSelProp("ZwinMLL", c_ZwinMLL) });
  
  m_sel_MVAScore_prop = new std::vector<SelProp>({
      MakeSelProp("LowMVAScore", c_LowMVAScore), MakeSelProp("MidMVAScore", c_MidMVAScore), MakeSelProp("HighMVAScore", c_HighMVAScore) });

  for(auto selprop : *m_sel_lep_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_lepflav_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_jet_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_fwdjet_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_bjet_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_J_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_M_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_T_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_L_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_H_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_V_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_TH_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_LT_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_VT_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_VLT_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_VLTH_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_Mbb_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_Mtb_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_MetSig_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_MetCut_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_MLL_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_MVAScore_prop){ AddSelectionIndex(selprop.name, selprop.index); }

  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"Added all top selections "<<std::endl; }
  //========================== Add desired regions ================================================


  //========================== Get desired channels ===============================================
  GetVLQChannels();

  //========================== Add preselection regions ===========================================
  if(m_opt->DoPreselection()){
    AddPreselectionRegions();
    
    if(m_opt->ApplyMVA()) AddMVATrainingRegions();
  }
  //========================== Add fit regions ====================================================
  if(m_opt->DoFitRegions()) AddFitRegions();
  
  //========================== Add validation regions =============================================
  if(m_opt->DoValidnRegions()) AddValidationRegions();

  m_blinding_config.close();
  
  m_ch_metsig.clear();
  m_ch_metcut.clear();
  m_ch_mtb_presel.clear();
  m_ch_fjet.clear();
  m_v_bjet_presel.clear();
  m_ch_lep.clear();
  m_ch_mll.clear();
  m_v_jet_presel.clear();

  return true;

}

//______________________________________________________________________________
//
bool VLQ_Selector::GetVLQChannels(){

  //v------------------General cuts------------------v//
  // met significance cut channels
  if(m_opt->ApplyMetSignificanceCut()) m_ch_metsig = {"", "-LowMetSig", "-HighMetSig"};
  else m_ch_metsig = {""};

  // met cut channels
  if(m_opt->ApplyMetRegionsCut()) m_ch_metcut = {"", "-LowMetCut", "-HighMetCut"};
  else m_ch_metcut = {""};

  // mtbmin cut channels
  m_ch_mtb_presel["sVLQ"] = {""};
  m_ch_mtb_presel["pVLQ"] = {""};

  // forward jet multiplicity cut channels
  m_ch_fjet["sVLQ"] = {""};
  m_ch_fjet["pVLQ"] = {""};

  // b-tagged jet multiplicity cut channels
  m_v_bjet_presel = {"1bin","2bin","2bex","3bin","3bex","4bin"};
  if(m_opt->DoLowBRegions()){
    m_v_bjet_presel.push_back("0bin");
    m_v_bjet_presel.push_back("0bex");
    m_v_bjet_presel.push_back("1bex");
  }
  //^------------------General cuts------------------^//

  //v----------Lepton channel specific cuts----------v//
  // 0-lepton
  if(m_opt->DoZeroLeptonAna()){
    m_ch_lep["c-0lep-"] = {""};
    m_ch_mll["c-0lep-"] = {""};
  }

  // 1-lepton
  if(m_opt->DoOneLeptonAna()){
    if(m_opt->DoSplitEMu()) m_ch_lep["c-1lep-"] = {"", "_el", "_mu"};
    else m_ch_lep["c-1lep-"] = {""};

    m_ch_mll["c-1lep-"] = {""};
  }

  // 2-lepton
  if(m_opt->DoTwoLeptonAna()){
    if(m_opt->DoSplitEMu()) m_ch_lep["c-2lep-"] = {"", "_ee", "_mumu", "_emu", "_sf"};
    m_ch_lep["c-2lep-"] = {"_sf"};

    if(m_opt->VerboseOutput()) m_ch_mll["c-2lep-"] = {"", "-HighMLL", "-ZwinMLL"};
    else m_ch_mll["c-2lep-"] = {"-ZwinMLL"};
  }
  //^----------Lepton channel specific cuts----------^//

  //v-------------Single VLQ analysis cuts-----------v//
  if(m_opt->DoSingleVLQRegions()){
    
    if(m_opt->DoExtendedPreselection()){
      m_ch_fjet["sVLQ"].push_back("-0fjex");
      m_ch_fjet["sVLQ"].push_back("-1fjin");
    }
    
    if(m_opt->DoOneLeptonAna() || m_opt->DoTwoLeptonAna()){
      m_v_jet_presel["c-1lep-sVLQ"] = {"3_5jwin","3jin","4jin","5jin","6jin"};
      m_v_jet_presel["c-2lep-sVLQ"] = {"3_5jwin","3jin","4jin","5jin","6jin"};

      if(m_opt->DeriveReweighting() ||  m_opt->DoExclusiveJetRegions()){
	std::vector<std::string> v_jet_presel_ex = {"3jex","4jex","5jex","6jex","7jex","8jex","9jex","10jex","11jex"};
        m_v_jet_presel["c-1lep-sVLQ"].insert(m_v_jet_presel["c-1lep-sVLQ"].end(),v_jet_presel_ex.begin(),v_jet_presel_ex.end());
	m_v_jet_presel["c-2lep-sVLQ"].insert(m_v_jet_presel["c-1lep-sVLQ"].end(),v_jet_presel_ex.begin(),v_jet_presel_ex.end());

        if(m_opt->DeriveReweighting()){
	  std::vector<std::string> v_jet_presel_in = {"7jin","8jin","9jin"};
          m_v_jet_presel["c-1lep-sVLQ"].insert(m_v_jet_presel["c-1lep-sVLQ"].end(),v_jet_presel_in.begin(),v_jet_presel_in.end());
	  m_v_jet_presel["c-2lep-sVLQ"].insert(m_v_jet_presel["c-1lep-sVLQ"].end(),v_jet_presel_in.begin(),v_jet_presel_in.end());
        }

      }

    }

  }
  //^-------------Single VLQ analysis cuts-----------^//
  
  //v--------------Pair VLQ analysis cuts------------v//
  if(m_opt->DoPairVLQRegions()){

    if(m_opt->DoSplitMtb()){
      m_ch_mtb_presel["pVLQ"].push_back("-LowMtbmin");
      m_ch_mtb_presel["pVLQ"].push_back("-HighMtbmin");
    }

    if(m_opt->DoZeroLeptonAna()){
      m_v_jet_presel["c-0lep-pVLQ"] = {"6jin", "7jin"};

      if(m_opt->DoLowJRegions()){
	std::vector<std::string> v_jet_presel_low = {"5jex","5jin"};
        m_v_jet_presel["c-0lep-pVLQ"].insert(m_v_jet_presel["c-0lep-pVLQ"].end(), v_jet_presel_low.begin(), v_jet_presel_low.end());
      }

    }
    
    if(m_opt->DoOneLeptonAna() || m_opt->DoTwoLeptonAna()){

      m_v_jet_presel["c-1lep-pVLQ"] = {"5jin", "6jin"};
      m_v_jet_presel["c-2lep-pVLQ"] = {"5jin", "6jin"};

      if(m_opt->DoLowJRegions()){
	std::vector<std::string> v_jet_presel_low = {"4jex","4jin"};
        m_v_jet_presel["c-1lep-pVLQ"].insert(m_v_jet_presel["c-1lep-pVLQ"].end(), v_jet_presel_low.begin(), v_jet_presel_low.end());
	m_v_jet_presel["c-2lep-pVLQ"].insert(m_v_jet_presel["c-1lep-pVLQ"].end(), v_jet_presel_low.begin(), v_jet_presel_low.end());
      }

      if(m_opt->DoExclusiveJetRegions()){
	std::vector<std::string> v_jet_presel_ex = {"5jex","6jex","7jex","8jex","9jin"};
        m_v_jet_presel["c-1lep-pVLQ"].insert(m_v_jet_presel["c-1lep-pVLQ"].end(), v_jet_presel_ex.begin(), v_jet_presel_ex.end());
	m_v_jet_presel["c-2lep-pVLQ"].insert(m_v_jet_presel["c-1lep-pVLQ"].end(), v_jet_presel_ex.begin(), v_jet_presel_ex.end());
      }

    }

  }
  //^--------------Pair VLQ analysis cuts------------^//

  return true;
}

//______________________________________________________________________________
//
bool VLQ_Selector::AddPreselectionRegions(){

  bool do_runop = true;

  std::vector< std::string > vlq_ana_type = {};
  if(m_opt->DoSingleVLQRegions()) vlq_ana_type.push_back("sVLQ");
  if(m_opt->DoPairVLQRegions()) vlq_ana_type.push_back("pVLQ");

  for(const std::string& ana_type : vlq_ana_type){

    for(std::pair<std::string, std::vector<std::string> > lep_ch_pair : m_ch_lep){
    
      const std::string& lep_prefix = lep_ch_pair.first;
      
      for(const std::string& jet : m_v_jet_presel[lep_prefix+ana_type]){
	for(const std::string& bjet : m_v_bjet_presel){
	  for(const std::string& mtbsuf : m_ch_mtb_presel[ana_type]){
	    for(const std::string& lepsuf : lep_ch_pair.second){
	      for(const std::string& mllsuf : m_ch_mll[lep_prefix]){
		for(const std::string& metcut : m_ch_metcut){
		  for(const std::string& metsuf : m_ch_metsig){
		    for(const std::string& fjet : m_ch_fjet[ana_type]){
		      AddVLQSelection(lep_prefix+jet+"-"+bjet+mtbsuf+fjet+metsuf+metcut+mllsuf+lepsuf, do_runop, m_opt->DoPreselSys(), PRESEL);
		    } // forward jet channel
		  } // MET significance channel
		} // MET cut channel
	      } // dilepton mass channel
	    } // lepton channel type
	  } // Mtbmin cut channel
	} // b-tagged jet multiplicity channel
      } // jet multiplicity channel

    } // lepton channel

  } // VLQ analysis type

  return true;

}

//______________________________________________________________________________
//
bool VLQ_Selector::AddFitRegions(){

  bool do_syst = true;
  bool do_runop = true;
  
  if(m_opt->DoSingleVLQRegions()){

    for( std::pair<std::string, std::vector<std::string> > lep_ch_pair : m_ch_lep ){

      const std::string& lep_prefix = lep_ch_pair.first;

      for(const std::string& lepsuf : lep_ch_pair.second){

	AddVLQSelection(lep_prefix+"3_5jwin-1bex-1fjin-0LTex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR1-a                                            
	AddVLQSelection(lep_prefix+"3_5jwin-1bex-1fjin-0Tex-1Lin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ); //SR1-b ORIG                                         

	AddVLQSelection(lep_prefix+"3_5jwin-2bex-1fjin-0LTex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR1-a                                            
	AddVLQSelection(lep_prefix+"3_5jwin-2bex-1fjin-0Tex-1Lin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ); //SR1-b ORIG                                         

	AddVLQSelection(lep_prefix+"3_5jwin-3bex-1fjin-0LTex-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);    //SR2-a                                               
	AddVLQSelection(lep_prefix+"3_5jwin-3bex-1fjin-0Tex-1Lin-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);    //SR2-b ORIG                                      
	AddVLQSelection(lep_prefix+"3_5jwin-3bex-1fjin-1Tin-0Lex-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	AddVLQSelection(lep_prefix+"3_5jwin-4bin-1fjin-0LTex-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);    //SR2-a                                               
	AddVLQSelection(lep_prefix+"3_5jwin-4bin-1fjin-0Tex-1Lin-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);    //SR2-b ORIG                                      
	AddVLQSelection(lep_prefix+"3_5jwin-4bin-1fjin-1Tin-0Lex-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	AddVLQSelection(lep_prefix+"6jin-1bex-1fjin-1Lex-0Tex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR3-a                                           
	AddVLQSelection(lep_prefix+"6jin-1bex-1fjin-0Lex-1Tex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR3-b                                           
	AddVLQSelection(lep_prefix+"6jin-1bex-1fjin-2LTin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR3-c                                               

	AddVLQSelection(lep_prefix+"6jin-2bex-1fjin-1Lex-0Tex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR3-a                                           
	AddVLQSelection(lep_prefix+"6jin-2bex-1fjin-0Lex-1Tex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR3-b                                           
	AddVLQSelection(lep_prefix+"6jin-2bex-1fjin-2LTin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR3-c                                               

	AddVLQSelection(lep_prefix+"6jin-3bex-1fjin-1Lex-0VTex-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);        //SR4-a                                              
	AddVLQSelection(lep_prefix+"6jin-3bex-1fjin-0Lex-1VTex-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);        //SR4-b                                              
	AddVLQSelection(lep_prefix+"6jin-3bex-1fjin-2VLTin-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);        //SR4-c                                                  

	AddVLQSelection(lep_prefix+"6jin-4bin-1fjin-1Lex-0VTex-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);        //SR4-a                                              
	AddVLQSelection(lep_prefix+"6jin-4bin-1fjin-0Lex-1VTex-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);        //SR4-b                                              
	AddVLQSelection(lep_prefix+"6jin-4bin-1fjin-2VLTin-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);        //SR4-c 

	// Ttbar+HF control regions                                                                                                                                     
	AddVLQSelection(lep_prefix+"3_5jwin-4bin-0fjex-1Lin-0VTex-0Hex"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"6jin-4bin-0fjex-1Lin-0VTex-0Hex"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	// Loose regions for syst extrapolation                                                                                                                           
	if(m_opt->DoLooseSystRegions()){

	  // Signal regions                                                                                                                                               
	  AddVLQSelection(lep_prefix+"3_5jwin-1bin-0LTex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR1-a                                                  
	  AddVLQSelection(lep_prefix+"3_5jwin-1bin-0Tex-1Lin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ); //SR1-b ORIG                                               

	  AddVLQSelection(lep_prefix+"3_5jwin-1bin-0LTex-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);    //SR2-a                                                     
	  AddVLQSelection(lep_prefix+"3_5jwin-1bin-0Tex-1Lin-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);    //SR2-b ORIG                                            
	  AddVLQSelection(lep_prefix+"3_5jwin-1bin-1Tin-0Lex-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	  AddVLQSelection(lep_prefix+"6jin-1bin-1Lex-0Tex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR3-a                                                 
	  AddVLQSelection(lep_prefix+"6jin-1bin-0Lex-1Tex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR3-b                                                 
	  AddVLQSelection(lep_prefix+"6jin-1bin-2LTin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR3-c                                                     

	  AddVLQSelection(lep_prefix+"6jin-1bin-1Lex-0VTex-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);        //SR4-a                                                    
	  AddVLQSelection(lep_prefix+"6jin-1bin-0Lex-1VTex-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);        //SR4-b                                                    
	  AddVLQSelection(lep_prefix+"6jin-1bin-2VLTin-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);        //SR4-c                                                        

	  // Ttbar+HF control regions                                                                                                                                     
	  AddVLQSelection(lep_prefix+"3_5jwin-1bin-1Lin-0VTex-0Hex"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	  AddVLQSelection(lep_prefix+"6jin-1bin-1Lin-0VTex-0Hex"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	}

      }

    }

  }
  
  if(m_opt->DoPairVLQRegions()){

    if(m_opt->DoZeroLeptonAna()){

      if(m_opt->DoOldPairProdRegions()){
	AddVLQSelection("c-0lep-0Tex-0Hex-7jin-2bex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-1Tex-0Hex-7jin-2bex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-0Tex-1Hex-7jin-2bex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-2THin-7jin-2bex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-0Tex-0Hex-7jin-3bex-LowMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-1Tex-0Hex-7jin-3bex-LowMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-0Tex-1Hex-7jin-3bex-LowMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-1Tex-1Hex-7jin-3bex-LowMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-2Tin-0_1Hwin-7jin-3bex-LowMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-0Tex-0Hex-7jin-3bex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-1Tex-0Hex-7jin-3bex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-0Tex-1Hex-7jin-3bex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-1Tex-1Hex-7jin-3bex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-2Tin0_1Hwin-7jin-3bex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-0Tin-2Hin-7jin-3bex", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-0Tex-0Hex-7jin-4bin-LowMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-1Tex-0Hex-7jin-4bin-LowMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-0Tex-1Hex-7jin-4bin-LowMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-0Tex-0Hex-7jin-4bin-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-1Tex-0Hex-7jin-4bin-HighMtbmin,", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-0Tex-1Hex-7jin-4bin-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-2THin-7jin-4bin", do_runop, do_syst, FIT);
      }

      if(m_opt->DoBaselineFitRegions()){
	// 0Hex (ZtZt) optimized regions                                                                                                                                  
	AddVLQSelection("c-0lep-7jin-2bex-0Hex-1VTex-HighMtbmin", do_runop, do_syst, FIT);     //0lep7jin2bex0Hex1VTexHighMtbmin                                          
	AddVLQSelection("c-0lep-7jin-2bex-0Hex-1Vex-1Tin-HighMtbmin", do_runop, do_syst, FIT); //0lep7jin2bex0Hex1Vex1TinHighMtbmin                                       
	AddVLQSelection("c-0lep-7jin-2bex-0Hex-0Vex-2Tin-HighMtbmin", do_runop, do_syst, FIT); //0lep7jin2bex0Hex0Vex2TinHighMtbmin                                       
	AddVLQSelection("c-0lep-7jin-2bex-0Hex-2Vin-0Tin-HighMtbmin", do_runop, do_syst, FIT); //0lep7jin2bex0Hex2Vin0TinHighMtbmin                                       
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-1VTex", do_runop, do_syst, FIT);                //0lep7jin3bin0Hex1VTex                                                    
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-1Vex-1Tin", do_runop, do_syst, FIT);            //0lep7jin3bin0Hex1Vex1Tin                                                 
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-0Vex-2Tin", do_runop, do_syst, FIT);            //0lep7jin3bin0Hex0Vex2Tin                                                 
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-2Vin-0Tin", do_runop, do_syst, FIT);            //0lep7jin3bin0Hex2Vin0Tin                                                 
	// 2Hin (HtHt) optimized regions                                                                                                                                  
	AddVLQSelection("c-0lep-7jin-3bin-2Hin-0Vin-0Tin", do_runop, do_syst, FIT);            //0lep7jin3bin2Hin0Vin0Tin                                                 
	// 1Hex (HtZt) optimized regions                                                                                                                                  
	AddVLQSelection("c-0lep-7jin-2bex-1Hin-0VTex-HighMtbmin", do_runop, do_syst, FIT);     //0lep7jin2bex1Hin0VTexHighMtbmin                                          
	AddVLQSelection("c-0lep-7jin-2bex-1Hin-1VTex-HighMtbmin", do_runop, do_syst, FIT);     //0lep7jin2bex1Hin1VTexHighMtbmin                                          
	AddVLQSelection("c-0lep-7jin-2bex-1Hin-2VTin-HighMtbmin", do_runop, do_syst, FIT);     //0lep7jin2bex1Hin2VTinHighMtbmin                                          
	AddVLQSelection("c-0lep-7jin-3bex-1Hex-0VTex", do_runop, do_syst, FIT);                //0lep7jin3bex1Hex0VTex                                                    
	AddVLQSelection("c-0lep-7jin-3bex-1Hex-1VTex", do_runop, do_syst, FIT);                //0lep7jin3bex1Hex1VTex                                                    
	AddVLQSelection("c-0lep-7jin-3bex-1Hex-2VTin", do_runop, do_syst, FIT);                //0lep7jin3bex1Hex2VTin                                                    
	AddVLQSelection("c-0lep-7jin-4bin-1Hex-0VTex", do_runop, do_syst, FIT);                //0lep7jin4bin1Hex0VTex                                                    
	AddVLQSelection("c-0lep-7jin-4bin-1Hex-1VTex", do_runop, do_syst, FIT);                //0lep7jin4bin1Hex1VTex                                                    
	AddVLQSelection("c-0lep-7jin-4bin-1Hex-2VTin", do_runop, do_syst, FIT);                //0lep7jin4bin1Hex2VTin                                                    
      }

      if(m_opt->ApplyMVA()){
	// Optimized regions                                                                                                                                              
	// High MVA score regions, 400 GeV MET cut                                                                                                                        
	AddVLQSelection("c-0lep-6jin-2bex-2Min-HighMetCut-HighMVAScore",    do_runop, do_syst, FIT); //0lep6jin2bex2MinHighMetCutHighMVAScore                             
	AddVLQSelection("c-0lep-6jin-3bex-2Min-HighMetCut-HighMVAScore",    do_runop, do_syst, FIT); //0lep6jin3bex2MinHighMetCutHighMVAScore                             
	AddVLQSelection("c-0lep-6jin-3bex-2Min-LowMetCut-HighMVAScore",     do_runop, do_syst, FIT); //0lep6jin3bex2MinLowMetCutHighMVAScore                              
	AddVLQSelection("c-0lep-6jin-4bin-2Min0Hex-HighMVAScore",           do_runop, do_syst, FIT); //0lep6jin4bin2Min0HexHighMVAScore                                   
	AddVLQSelection("c-0lep-6jin-4bin-2Min1Hin-HighMVAScore",           do_runop, do_syst, FIT); //0lep6jin4bin2Min1HinHighMVAScore                                   
	// Mid MVA score regions, 400 GeV MET cut                                                                                                                         
	AddVLQSelection("c-0lep-6jin-2bex-2Min-HighMetCut-MidMVAScore",     do_runop, do_syst, FIT); //0lep6jin2bex2MinHighMetCutMidMVAScore                              
	AddVLQSelection("c-0lep-6jin-3bin-2Min-MidMVAScore",                do_runop, do_syst, FIT); //0lep6jin3bin2MinMidMVAScore // reconsider 3bin merge               
	// Low MVA score regions, 400 GeV MET cut                                                                                                                         
	AddVLQSelection("c-0lep-6jin-2bex-2Min-HighMetCut-LowMVAScore",     do_runop, do_syst, FIT); //0lep6jin2bex2MinHighMetCutLowMVAScore                              
	AddVLQSelection("c-0lep-6jin-3bex-2Min-LowMVAScore",                do_runop, do_syst, FIT); //0lep6jin3bex2MinLowMVAScore                                        
	AddVLQSelection("c-0lep-6jin-4bin-2Min-LowMVAScore",                do_runop, do_syst, FIT); //0lep6jin4bin2MinLowMVAScore                                        

	// Regions that were either dropped or merged to obtained the optimized regions. Will keep in case of reoptimization                                              
	// High MVA score regions, 400 GeV MET cut                                                                                                                        
	AddVLQSelection("c-0lep-6jin-2bex-2Min-LowMetCut-HighMVAScore",     do_runop, do_syst, FIT); //0lep6jin2bex2MinLowMetCutHighMVAScore                              
	// Mid MVA score regions, 400 GeV MET cut                                                                                                                         
	AddVLQSelection("c-0lep-6jin-2bin-2Min-LowMetCut-MidMVAScore",      do_runop, do_syst, FIT); //0lep6jin2bin2MinLowMetCutMidMVAScore                               
	// Low MVA score regions, 400 GeV MET cut                                                                                                                         
	AddVLQSelection("c-0lep-6jin-2bin-2Min-HighMetCut-LowMVAScore",     do_runop, do_syst, FIT); //0lep6jin2bin2MinHighMetCutLowMVAScore                              
	AddVLQSelection("c-0lep-6jin-2bin-2Min-LowMetCut-LowMVAScore",      do_runop, do_syst, FIT); //0lep6jin2bin2MinLowMetCutLowMVAScore                               
	// Mid MVA score regions, 400 GeV MET cut with bjet split                                                                                                         
	AddVLQSelection("c-0lep-6jin-2bin-2Min-HighMetCut-MidMVAScore",     do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-6jin-2bex-2Min-LowMetCut-MidMVAScore",      do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-6jin-3bex-2Min-HighMetCut-MidMVAScore",     do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-6jin-3bex-2Min-LowMetCut-MidMVAScore",      do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-6jin-4bin-2Min-HighMetCut-MidMVAScore",     do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-6jin-4bin-2Min-LowMetCut-MidMVAScore",      do_runop, do_syst, FIT);
	// Low MVA score regions, 400 GeV MET cut with bjet split                                                                                                         
	AddVLQSelection("c-0lep-6jin-2bex-2Min-LowMetCut-LowMVAScore",      do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-6jin-3bex-2Min-HighMetCut-LowMVAScore",     do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-6jin-3bex-2Min-LowMetCut-LowMVAScore",      do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-6jin-4bin-2Min-HighMetCut-LowMVAScore",     do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-6jin-4bin-2Min-LowMetCut-LowMVAScore",      do_runop, do_syst, FIT);
	// Merge proposals                                                                                                                                                
	// Mid MVA score regions, MET merge with bjet split                                                                                                               
	AddVLQSelection("c-0lep-6jin-3bex-2Min-MidMVAScore",     do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-6jin-4bin-2Min-MidMVAScore",     do_runop, do_syst, FIT);
      }
    }

    if(m_opt->DoOneLeptonAna()){

      if(m_opt->DoOldPairProdRegions()){
	AddVLQSelection("c-1lep-0Tex-0Hex-6jin-3bex", do_runop, do_syst, FIT);
	AddVLQSelection("c-1lep-1Tex-0Hex-6jin-3bex", do_runop, do_syst, FIT);
	AddVLQSelection("c-1lep-0Tex-1Hex-6jin-3bex", do_runop, do_syst, FIT);
	AddVLQSelection("c-1lep-1Tex-1Hex-6jin-3bex", do_runop, do_syst, FIT);
	AddVLQSelection("c-1lep-2Tin-0_1Hwin-6jin-3bex", do_runop, do_syst, FIT);
	AddVLQSelection("c-1lep-0Tin-2Hin-6jin-3bex", do_runop, do_syst, FIT);
	AddVLQSelection("c-1lep-0Tex-0Hex-6jin-4bin", do_runop, do_syst, FIT);
	AddVLQSelection("c-1lep-1Tex-0Hex-6jin-4bin", do_runop, do_syst, FIT);
	AddVLQSelection("c-1lep-0Tex-1Hex-6jin-4bin", do_runop, do_syst, FIT);
	AddVLQSelection("c-1lep-1Tex-1Hex-6jin-4bin", do_runop, do_syst, FIT);
	AddVLQSelection("c-1lep-2Tin-0_1Hwin-6jin-4bin", do_runop, do_syst, FIT);
	AddVLQSelection("c-1lep-0Tin-2Hin-6jin-4bin", do_runop, do_syst, FIT);
      }

      if(m_opt->DoBaselineFitRegions()){

	// 3bex regions                                                                                                                                               
	AddVLQSelection("c-1lep-6jin-3bex-0Hex-0Vin-0LTex", do_runop, do_syst, FIT);     //1lep6jin3bex0Hex0Vin0LTex                                       
	AddVLQSelection("c-1lep-6jin-3bex-0Hex-0Vin-1LTin", do_runop, do_syst, FIT);     //1lep6jin3bex0Hex0Vin1LTin                                       
	AddVLQSelection("c-1lep-6jin-3bex-1Hex-0Vex-0LTex", do_runop, do_syst, FIT);     //1lep6jin3bex1Hex0Vex0LTex                                       
	AddVLQSelection("c-1lep-6jin-3bex-1Hex-0Vex-1Lin-0Tex", do_runop, do_syst, FIT); //1lep6jin3bex1Hex0Vex1Lin0Tex                                    
	AddVLQSelection("c-1lep-6jin-3bex-1Hex-1Vin-0Lin-0Tex", do_runop, do_syst, FIT); //1lep6jin3bex1Hex1Vin0Lin0Tex                                    
	AddVLQSelection("c-1lep-6jin-3bex-1Hex-0Vin-0Lex-1Tin", do_runop, do_syst, FIT); //1lep6jin3bex1Hex0Vin0Lex1Tin                                    
	AddVLQSelection("c-1lep-6jin-3bex-1Hex-0Vin-1Lin-1Tin", do_runop, do_syst, FIT); //1lep6jin3bex1Hex0Vin1Lin1Tin                                    
	AddVLQSelection("c-1lep-6jin-3bex-2Hin-0Vin-0LTex", do_runop, do_syst, FIT);     //1lep6jin3bex2Hin0Vin0LTex                                       
	AddVLQSelection("c-1lep-6jin-3bex-2Hin-0Vin-1LTin", do_runop, do_syst, FIT);     //1lep6jin3bex2Hin0Vin1LTin                                       
	// 4bin regions                                                                                                                                               
	AddVLQSelection("c-1lep-6jin-4bin-0Hex-0Vin-0LTex", do_runop, do_syst, FIT);     //1lep6jin4bin0Hex0Vin0LTex                                       
	AddVLQSelection("c-1lep-6jin-4bin-0Hex-0Vin-1LTin", do_runop, do_syst, FIT);     //1lep6jin4bin0Hex0Vin1LTin                                       
	AddVLQSelection("c-1lep-6jin-4bin-1Hex-0Vex-0LTex", do_runop, do_syst, FIT);     //1lep6jin4bin1Hex0Vex0LTex                                       
	AddVLQSelection("c-1lep-6jin-4bin-1Hex-0Vex-1Lin-0Tex", do_runop, do_syst, FIT); //1lep6jin4bin1Hex0Vex1Lin0Tex                                    
	AddVLQSelection("c-1lep-6jin-4bin-1Hex-0Vin-0Lex-1Tin", do_runop, do_syst, FIT); //1lep6jin4bin1Hex0Vin0Lex1Tin                                    
	AddVLQSelection("c-1lep-6jin-4bin-1Hex-0Vin-1Lin-1Tin", do_runop, do_syst, FIT); //1lep6jin4bin1Hex0Vin1Lin1Tin                                    
	AddVLQSelection("c-1lep-6jin-4bin-1Hex-1Vin-0Lin-0Tex", do_runop, do_syst, FIT); //1lep6jin4bin1Hex1Vin0Lin0Tex                                    
	AddVLQSelection("c-1lep-6jin-4bin-2Hin-0Vin-0Lin-0Tin", do_runop, do_syst, FIT); //1lep6jin4bin2Hin0Vin0Lin0Tin                                    
	
      }
      
      if(m_opt->ApplyMVA()){

	//MVA regions                                                                                                                                                     
	// background control regions                                                                                                                                     
	AddVLQSelection("c-1lep-5jin-2bex-0Hex-1VTex-1Lex", do_runop, do_syst, FIT);            //1lep5jin2bex0Hex1VTex1Lex                                               
	AddVLQSelection("c-1lep-5jin-3bex-0Hex-1VTex-0_1Lwin", do_runop, do_syst, FIT);         //1lep5jin3bex0Hex1VTex0_1Lwin                                            
	AddVLQSelection("c-1lep-5jin-4bin-0Hex-1VTex-0_1Lwin", do_runop, do_syst, FIT);         //1lep5jin4bin0Hex1VTex0_1Lwin                                            
	// 3bex-0Hex regions                                                                                                                                              
	AddVLQSelection("c-1lep-5jin-3bex-2Min3Jin0Hex-HighMVAScore", do_runop, do_syst, FIT); //1lep5jin3bex2Min3Jin0HexHighMVAScore                                     
	AddVLQSelection("c-1lep-5jin-3bex-2Min3Jin0Hex-MidMVAScore", do_runop, do_syst, FIT);  //1lep5jin3bex2Min3Jin0HexMidMVAScore                                      
	AddVLQSelection("c-1lep-5jin-3bex-2Min3Jin0Hex-LowMVAScore", do_runop, do_syst, FIT);  //1lep5jin3bex2Min3Jin0HexLowMVAScore                                      
	// 3bex-1Hin regions                                                                                                                                              
	AddVLQSelection("c-1lep-5jin-3bex-2Min3Jin1Hin-HighMVAScore", do_runop, do_syst, FIT); //1lep5jin3bex2Min3Jin1HinHighMVAScore                                     
	AddVLQSelection("c-1lep-5jin-3bex-2Min3Jin1Hin-MidMVAScore", do_runop, do_syst, FIT);  //1lep5jin3bex2Min3Jin1HinMidMVAScore                                      
	AddVLQSelection("c-1lep-5jin-3bex-2Min3Jin1Hin-LowMVAScore", do_runop, do_syst, FIT);  //1lep5jin3bex2Min3Jin1HinLowMVAScore                                      
	// 4bin-0Hex regions                                                                                                                                              
	AddVLQSelection("c-1lep-5jin-4bin-2Min3Jin0Hex-HighMVAScore", do_runop, do_syst, FIT); //1lep5jin4bin2Min3Jin0HexHighMVAScore                                     
	AddVLQSelection("c-1lep-5jin-4bin-2Min3Jin0Hex-MidMVAScore", do_runop, do_syst, FIT);  //1lep5jin4bin2Min3Jin0HexMidMVAScore                                      
	AddVLQSelection("c-1lep-5jin-4bin-2Min3Jin0Hex-LowMVAScore", do_runop, do_syst, FIT);  //1lep5jin4bin2Min3Jin0HexLowMVAScore                                      
	// 4bin-1Hin regions                                                                                                                                              
	AddVLQSelection("c-1lep-5jin-4bin-2Min3Jin1Hin-HighMVAScore", do_runop, do_syst, FIT); //1lep5jin4bin2Min3Jin1HinHighMVAScore                                     
	AddVLQSelection("c-1lep-5jin-4bin-2Min3Jin1Hin-MidMVAScore", do_runop, do_syst, FIT);  //1lep5jin4bin2Min3Jin1HinMidMVAScore                                      
	AddVLQSelection("c-1lep-5jin-4bin-2Min3Jin1Hin-LowMVAScore", do_runop, do_syst, FIT);  //1lep5jin4bin2Min3Jin1HinLowMVAScore                                      

	// Uncorrelation schemes for ttbar modeling/pmg weight systematics                                                                                                
	// Njets only : split 5jin regions into 5jex and 6jin for ttbar modeling and pmg weight systematics only                                                          
	// MTag only  : split between background control (1Mex) and MVA search regions (2Min) for ttbar modeling and pmg weight systematics only                          
	// Full decorrelation : split into 5jex-1Mex, 6jin-1Mex, 5jex-2Min, 6jin-2Min for ttbar modeling and pmg weight systematics                                       
	if(m_opt->DoUncorrelatedMVARegions()){
	  // background control regions                                                                                                                                   
	  AddVLQSelection("c-1lep-5jex-2bex-0Hex-1VTex-1Lex", do_runop, do_syst, FIT);
	  AddVLQSelection("c-1lep-5jex-3bex-0Hex-1VTex-0_1Lwin", do_runop, do_syst, FIT);
	  AddVLQSelection("c-1lep-5jex-4bin-0Hex-1VTex-0_1Lwin", do_runop, do_syst, FIT);
	  AddVLQSelection("c-1lep-6jin-2bex-0Hex-1VTex-1Lex", do_runop, do_syst, FIT);
	  AddVLQSelection("c-1lep-6jin-3bex-0Hex-1VTex-0_1Lwin", do_runop, do_syst, FIT);
	  AddVLQSelection("c-1lep-6jin-4bin-0Hex-1VTex-0_1Lwin", do_runop, do_syst, FIT);
	  // MVA fit regions                                                                                                                                              
	  std::vector<std::string> v_mva_uncor_jet_presel   = {"-5jex", "-6jin"};
	  std::vector<std::string> v_mva_uncor_bjet_presel  = {"-3bex", "-4bin"};
	  std::vector<std::string> v_mva_uncor_boost_presel = {"-2Min3Jin0Hex", "-2Min3Jin1Hin"};
	  std::vector<std::string> v_mva_uncor_score_presel = {"-HighMVAScore", "-MidMVAScore", "-LowMVAScore"};
	  for(const std::string &mva_jet_sel : v_mva_uncor_jet_presel){
	    for(const std::string &mva_bjet_sel : v_mva_uncor_bjet_presel){
	      for(const std::string &mva_boost_presel : v_mva_uncor_boost_presel){
		for(const std::string &mva_score_presel : v_mva_uncor_score_presel){
		  AddVLQSelection("c-1lep"+mva_jet_sel+mva_bjet_sel+mva_boost_presel+mva_score_presel, do_runop, do_syst, FIT);
		}
	      }
	    }
	  }
	}
      }
    }
  }

  return true;
}

//______________________________________________________________________________
//
bool VLQ_Selector::AddValidationRegions(){
  
  bool do_syst = true;
  bool do_runop = true;

  if(m_opt->DoSingleVLQRegions()){

    for( std::pair<std::string, std::vector<std::string> > lep_ch_pair : m_ch_lep ){

      const std::string& lep_prefix = lep_ch_pair.first;

      for(const std::string& lepsuf : lep_ch_pair.second){

	// CONVENTIONAL VALIDATION REGIONS                                                                                                                              
	AddVLQSelection(lep_prefix+"3_5jwin-1bex-0fjex-0Tex-0Lex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"3_5jwin-1bex-0fjex-0Tex-1Lin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"3_5jwin-1bex-1fjin-1LTin-0Hex-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"3_5jwin-1bex-1fjin-1Tin-0Lex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	AddVLQSelection(lep_prefix+"3_5jwin-2bex-0fjex-0Tex-0Lex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"3_5jwin-2bex-0fjex-0Tex-1Lin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"3_5jwin-2bex-1fjin-1LTin-0Hex-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"3_5jwin-2bex-1fjin-1Tin-0Lex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	AddVLQSelection(lep_prefix+"3_5jwin-3bin-0fjex-0Tex-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"3_5jwin-3bin-1fjin-1VLTin-0Hex"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	AddVLQSelection(lep_prefix+"6jin-1bex-0fjex-1LTex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"6jin-1bex-0fjex-2LTin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"6jin-1bex-1fjin-0Tex-0Lex-1Hin-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"6jin-1bex-1fjin-2LTin-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	AddVLQSelection(lep_prefix+"6jin-2bex-0fjex-1LTex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"6jin-2bex-0fjex-2LTin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"6jin-2bex-1fjin-0Tex-0Lex-1Hin-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"6jin-2bex-1fjin-2LTin-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	AddVLQSelection(lep_prefix+"6jin-3bin-0fjex-1VLTin-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	AddVLQSelection(lep_prefix+"6jin-3bin-1fjin-1VLTin-0Hex"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	if(m_opt->DoLooseSystRegions()){
	  //AddVLQSelection(lep_prefix+"3_5jwin-1bin-0Tex-0Lex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);                                                          
	  //AddVLQSelection(lep_prefix+"3_5jwin-1bin-0Tex-1Lin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);                                                          
	  AddVLQSelection(lep_prefix+"3_5jwin-1bin-1LTin-0Hex-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	  AddVLQSelection(lep_prefix+"3_5jwin-1bin-1Tin-0Lex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	  AddVLQSelection(lep_prefix+"3_5jwin-1bin-0Tex-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	  AddVLQSelection(lep_prefix+"3_5jwin-1bin-1VLTin-0Hex"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	  AddVLQSelection(lep_prefix+"6jin-1bin-1LTex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	  //AddVLQSelection(lep_prefix+"6jin-1bin-2LTin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);                                                                 
	  AddVLQSelection(lep_prefix+"6jin-1bin-0Tex-0Lex-1Hin-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	  AddVLQSelection(lep_prefix+"6jin-1bin-2LTin-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	  AddVLQSelection(lep_prefix+"6jin-1bin-1VLTin-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	  AddVLQSelection(lep_prefix+"6jin-1bin-1VLTin-0Hex"+lepsuf, do_runop, do_syst, SINGLEVLQ);


	  //AddVLQSelection(lep_prefix+"3_5jwin-1_2bwin-1VLTHin"+lepsuf, do_runop, do_syst, SINGLEVLQ);                                                                   
	  //AddVLQSelection(lep_prefix+"3_5jwin-3bin-1VLTHin"+lepsuf, do_runop, do_syst, SINGLEVLQ);                                                                      
	  //AddVLQSelection(lep_prefix+"6jin-1_2bwin-1VLTHin"+lepsuf, do_runop, do_syst, SINGLEVLQ);                                                                      
	  //AddVLQSelection(lep_prefix+"6jin-3bin-1VLTHin"+lepsuf, do_runop, do_syst, SINGLEVLQ);                                                                         
	}
      }
    }
  }

  return true;

}

//______________________________________________________________________________
//
bool VLQ_Selector::AddMVATrainingRegions(){
  
  bool do_runop = true;

  std::map<std::string, std::vector< std::string > > mva_jet_presel;
  std::map<std::string, std::vector< std::string > > mva_bjet_presel;
  std::map<std::string, std::vector< std::string > > mva_boost_presel;
  std::vector< std::string > mva_cut_presel = {"", "-LowMVAScore", "-MidMVAScore", "-HighMVAScore"};

  if(m_opt->DoZeroLeptonAna()){
    mva_jet_presel["c-0lep-"] = {"6jin"};
    mva_bjet_presel["c-0lep-"] = {"-2bin"};
    mva_boost_presel["c-0lep-"] = {"-2Min"};
  }
  if(m_opt->DoOneLeptonAna()){
    mva_jet_presel["c-1lep-"] = {"5jin","6jin"};
    mva_bjet_presel["c-1lep-"] = {"-3bin"};
    mva_boost_presel["c-1lep-"]= {"-2Min3Jin"};
  }
 
  for(std::pair<std::string, std::vector<std::string> > lep_jet_ch_pair : mva_jet_presel){

    const std::string& lep_ch = lep_jet_ch_pair.first;
    
    for(const std::string& jet : mva_jet_presel[lep_ch]){
      for(const std::string& bjet : mva_bjet_presel[lep_ch]){
	for(const std::string& boost : mva_boost_presel[lep_ch]){
	  for(const std::string& mva_cut : mva_cut_presel){
	    AddVLQSelection(lep_ch+jet+bjet+boost+mva_cut, do_runop, m_opt->DoPreselSys(), PRESEL);
	  }
	}
      }
    }
  }

  return true;

}

//______________________________________________________________________________
//
Selection* VLQ_Selector::AddVLQSelection(const std::string& name, bool do_runop, bool do_syst, const int reg_type){

  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" AddVLQSelection "<<name<<" do_runop = "<<do_runop<<" do_syst = "<<do_syst<<" reg_type = "<<reg_type<<std::endl; }
  Selection* sel = AddSelection( AddSelectionIndex(name), name, do_runop );
  if(!sel){ std::cerr << "Failed to add selection with name "<<name<<std::endl; return NULL; }
  sel->AddFlagAtBit(DOSYST, do_syst);

  if     (reg_type == PRESEL){ sel->AddFlagAtBit(PRESEL, true); }
  else if(reg_type == VALIDATION) { sel->AddFlagAtBit(VALIDATION, true); }
  else if(reg_type == FIT){ sel->AddFlagAtBit(FIT, true); }
  else if(reg_type == SINGLEVLQ){ sel->AddFlagAtBit(SINGLEVLQ, true); }

  if     ( name.find("c-1lep") != std::string::npos ){ sel->AddFlagAtBit(ONELEP, true); }
  else if( name.find("c-0lep") != std::string::npos ){ sel->AddFlagAtBit(ZEROLEP, true); }


  if(m_opt->DoBlind()){ AddBlindingCut(*sel); }


  std::string hist_name = AnalysisUtils::ReplaceString(name, "-", "");
  //=================================== Book histogram in region ===============================
  if(m_opt->DumpHistos() && do_runop){
    m_anaTools -> BookAllHistograms(hist_name/*, true*/, do_syst);
  }

  return sel;

}

//______________________________________________________________________________
//
Selection* VLQ_Selector::MakeSelection(const int index, const std::string& name){
  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" VLQ_Selector::MakeSelection with name "<<name<<" and index "<<index<<std::endl; }

  if( (index < 0) && name.empty() ){
    std::cerr << "VLQ_Selector::MakeSelection --> No rule available to make selection with empty name and negative index. Exiting."<<std::endl;
    return NULL;
  }

  const std::string& sel_name = (name.empty()) ? GetSelectionName(index) : name;
  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" Selection index = "<<index<<" name = "<<name<<" sel_name = "<<sel_name<<std::endl; }

  //================ Build selection hierarchies -=========

  //Parse the name into its components
  std::string _name_ = sel_name;
  std::string _parts_= "";
  std::string::size_type pos = 0;

  int n_nodes = 0;

  SelProp* sprop_lep = NULL;
  SelProp* sprop_jet = NULL;
  SelProp* sprop_fwdjet = NULL;
  SelProp* sprop_bjet = NULL;
  SelProp* sprop_J = NULL;
  SelProp* sprop_M = NULL;
  SelProp* sprop_T = NULL;
  SelProp* sprop_L = NULL;
  SelProp* sprop_H = NULL;
  SelProp* sprop_V = NULL;
  SelProp* sprop_TH = NULL;
  SelProp* sprop_LT = NULL;
  SelProp* sprop_VT = NULL;
  SelProp* sprop_VLT = NULL;
  SelProp* sprop_VLTH = NULL;
  SelProp* sprop_Mbb = NULL;
  SelProp* sprop_Mtb = NULL;
  SelProp* sprop_MetSig = NULL;
  SelProp* sprop_MetCut = NULL;
  SelProp* sprop_MLL = NULL;
  SelProp* sprop_MVA = NULL;
  do{     pos = AnalysisUtils::ParseString(_name_, _parts_, "-");
    AnalysisUtils::TrimString(_parts_);
    bool found = false;

    //=============== Lepton part =====================
    for(SelProp& lepprop : *m_sel_lep_prop){
      if(lepprop.name == _parts_){
	sprop_lep = &lepprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== Jet part ========================
    for(SelProp& jetprop : *m_sel_jet_prop){
      if(jetprop.name == _parts_){
	sprop_jet = &jetprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== Fwd-jet part ========================
    for(SelProp& fwdjetprop : *m_sel_fwdjet_prop){
      if(fwdjetprop.name == _parts_){
	sprop_fwdjet = &fwdjetprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== B-jet part ========================
    for(SelProp& bjetprop : *m_sel_bjet_prop){
      if(bjetprop.name == _parts_){
	sprop_bjet = &bjetprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== J-tag part ========================
    for(SelProp& Jprop : *m_sel_J_prop){
      if(Jprop.name == _parts_){
	sprop_J = &Jprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== M-tag part ========================
    for(SelProp& Mprop : *m_sel_M_prop){
      if(Mprop.name == _parts_){
	sprop_M = &Mprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== T-tag part ========================
    for(SelProp& Tprop : *m_sel_T_prop){
      if(Tprop.name == _parts_){
	sprop_T = &Tprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== Leptonic T-tag part ===============
    for(SelProp& Lprop : *m_sel_L_prop){
      if(Lprop.name == _parts_){
	sprop_L = &Lprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== H-tag part ========================
    for(SelProp& Hprop : *m_sel_H_prop){
      if(Hprop.name == _parts_){
	sprop_H = &Hprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== V-tag part ========================
    for(SelProp& Vprop : *m_sel_V_prop){
      if(Vprop.name == _parts_){
	sprop_V = &Vprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== TH-tag part ========================
    for(SelProp& THprop : *m_sel_TH_prop){
      if(THprop.name == _parts_){
	sprop_TH = &THprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;


    //=============== LT-tag part ========================
    for(SelProp& LTprop : *m_sel_LT_prop){
      if(LTprop.name == _parts_){
	sprop_LT = &LTprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== VT-tag part ========================
    for(SelProp& VTprop : *m_sel_VT_prop){
      if(VTprop.name == _parts_){
	sprop_VT = &VTprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== VLT-tag part ========================
    for(SelProp& VLTprop : *m_sel_VLT_prop){
      if(VLTprop.name == _parts_){
	sprop_VLT = &VLTprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== VLTH-tag part ========================
    for(SelProp& VLTHprop : *m_sel_VLTH_prop){
      if(VLTHprop.name == _parts_){
	sprop_VLTH = &VLTHprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== Mbb-split part ========================
    for(SelProp& Mbbprop : *m_sel_Mbb_prop){
      if(Mbbprop.name == _parts_){
	sprop_Mbb = &Mbbprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

    //=============== Mtb-split part ========================
    for(SelProp& Mtbprop : *m_sel_Mtb_prop){
      if(Mtbprop.name == _parts_){
	sprop_Mtb = &Mtbprop;
	found = true; n_nodes++; break;
      }
    }
    //=============== Metsig-split part ========================
    for(SelProp& MetSigprop : *m_sel_MetSig_prop){
      if(MetSigprop.name == _parts_){
	sprop_MetSig = &MetSigprop;
	found = true; n_nodes++; break;
      }
    }
    //=============== MetCut-split part ========================
    for(SelProp& MetCutprop : *m_sel_MetCut_prop){
      if(MetCutprop.name == _parts_){
        sprop_MetCut = &MetCutprop;
        found = true; n_nodes++; break;
      }
    }
    //=============== MLL selection part ========================
    for(SelProp& MLLprop : *m_sel_MLL_prop){
      if(MLLprop.name == _parts_){
	sprop_MLL = &MLLprop;
	found = true; n_nodes++; break;
      }
    }
    //=============== MVAScore selection part ===================
    for(SelProp& MVAprop : *m_sel_MVAScore_prop){
      if(MVAprop.name == _parts_){
	sprop_MVA = &MVAprop;
	found = true; n_nodes++; break;
      }
    }
    if(found) continue;

  }while(pos != std::string::npos);


  //============================= Only certain selection patterns are currently allowed =========================
  //==== All regions are required to have, at minimum, defined lepton and jet multiplicity requirements.
  //==== Other requirements are optional
  if( (n_nodes > 1) && !sprop_lep ){ //If no lepton channel found for a compound selection
    std::cerr << " No lepton channel could be identified for selection " << name << ". Exiting."  << std::endl;
    return NULL;
  }
  if( (n_nodes > 1) && !sprop_lep ){ //If no jet channel found for a compound selection
    std::cerr << " No allowed jet multiplicity selection found for selection " << name << ". Exiting."  << std::endl;
    return NULL;
  }

  if( !( (sprop_M==NULL) || ( (sprop_T==NULL) &&  (sprop_H==NULL) &&  (sprop_TH==NULL) ) ) ){
    std::cerr << " It is inadvisable to apply M-tagged jet cut together ";
    std::cerr << "with a cut on T-tagging or H-tagging. Please check selection pattern ";
    std::cerr << name << ". Exiting. "<<std::endl;
    return NULL;
  }

  //==================== MAKE SELECTION ==================================
  Selection* sel = SelectorBase::MakeSelection(index, sel_name);

  //============================= ADD ANCESTORS ======================================================================
  if(n_nodes <= 1){
    ;//no ancestors added for a top selection
  }

  else if( (sel_name.find("_el") == sel_name.size()-3) || (sel_name.find("_mu") == sel_name.size()-3) ){
    if(sel_name.find("_el") != std::string::npos){
      AddAncestor(*sel, sel_name.substr(0, sel_name.find("_el")), true);
      SelectorBase::AddAncestor(*sel, c_1el_chan);
    }
    else if(sel_name.find("_mu") != std::string::npos){
      AddAncestor(*sel, sel_name.substr(0, sel_name.find("_mu")), true);
      SelectorBase::AddAncestor(*sel, c_1mu_chan);
    }
  }
  else if( (sel_name.find("_ee") == sel_name.size()-3) || (sel_name.find("_mumu") == sel_name.size()-5)
	   || (sel_name.find("_emu") == sel_name.size()-4) || (sel_name.find("_sf") == sel_name.size()-3) ){
    if(sel_name.find("_ee") != std::string::npos){
      AddAncestor(*sel, sel_name.substr(0, sel_name.find("_ee")), true);
      SelectorBase::AddAncestor(*sel, c_ee_chan);
    }
    else if(sel_name.find("_mumu") != std::string::npos){
      AddAncestor(*sel, sel_name.substr(0, sel_name.find("_mumu")), true);
      SelectorBase::AddAncestor(*sel, c_mumu_chan);
    }
    else if(sel_name.find("_emu") != std::string::npos){
      AddAncestor(*sel, sel_name.substr(0, sel_name.find("_emu")), true);
      SelectorBase::AddAncestor(*sel, c_emu_chan);
    }
    else if(sel_name.find("_sf") != std::string::npos){
      AddAncestor(*sel, sel_name.substr(0, sel_name.find("_sf")), true);
      SelectorBase::AddAncestor(*sel, c_sf_chan);
    }

  }//el/mu channel splitting is done last

  else{
    if( !(sprop_fwdjet || sprop_bjet || sprop_J || sprop_M || sprop_T || sprop_L || sprop_H || sprop_V
	  || sprop_TH || sprop_LT || sprop_VT || sprop_VLT || sprop_VLTH
	  || sprop_Mbb || sprop_Mtb || sprop_MetSig || sprop_MetCut ||  sprop_MLL || sprop_MVA)  ){

      if(sprop_jet->primanc_name.empty()){
        SelectorBase::AddAncestors(*sel, {sprop_lep->index, sprop_jet->index}, sprop_lep->index);
      }
      else{
        AddPrimary(*sel, "c-"+sprop_lep->name+"-"+sprop_jet->primanc_name);
        SelectorBase::AddAncestors(*sel, {sprop_lep->index, sprop_jet->index});
      }

    }//Lep + jet
    else if( !(sprop_fwdjet || sprop_J || sprop_M || sprop_T || sprop_L || sprop_H || sprop_V
	       || sprop_TH || sprop_LT || sprop_VT || sprop_VLT || sprop_VLTH
	       || sprop_Mbb || sprop_Mtb || sprop_MetSig || sprop_MetCut || sprop_MLL || sprop_MVA) ){

      if(sprop_bjet->primanc_name.empty()){
        AddAncestor(*sel, "c-"+sprop_lep->name+"-"+sprop_jet->name, true);
	SelectorBase::AddAncestor(*sel, sprop_bjet->index);
      }
      else{
        AddPrimary(*sel, "c-"+sprop_lep->name+"-"+sprop_jet->name+"-"+sprop_bjet->primanc_name);
        SelectorBase::AddAncestors(*sel, {sprop_lep->index, sprop_jet->index, sprop_bjet->index});
      }
    } //Lep + jet + bjet
    else if( !(sprop_Mbb || sprop_Mtb || sprop_MetSig || sprop_MetCut || sprop_MLL || sprop_MVA) ){
      AddAncestor(*sel, "c-"+sprop_lep->name+"-"+sprop_jet->name+"-"+sprop_bjet->name, true);

      if(sprop_J){ SelectorBase::AddAncestor(*sel, sprop_J->index); }
      else if(sprop_M){ SelectorBase::AddAncestor(*sel, sprop_M->index); }
      else{
        if(sprop_fwdjet ) { SelectorBase::AddAncestor(*sel, sprop_fwdjet->index); }
        if(sprop_T )      { SelectorBase::AddAncestor(*sel, sprop_T->index); }
        if(sprop_L )     { SelectorBase::AddAncestor(*sel, sprop_L->index); }
        if(sprop_H )      { SelectorBase::AddAncestor(*sel, sprop_H->index); }
        if(sprop_V )      { SelectorBase::AddAncestor(*sel, sprop_V->index); }
        if(sprop_TH )     { SelectorBase::AddAncestor(*sel, sprop_TH->index); }
        if(sprop_LT )     { SelectorBase::AddAncestor(*sel, sprop_LT->index); }
        if(sprop_VT )     { SelectorBase::AddAncestor(*sel, sprop_VT->index); }
        if(sprop_VLT )     { SelectorBase::AddAncestor(*sel, sprop_VLT->index); }
        if(sprop_VLTH )    { SelectorBase::AddAncestor(*sel, sprop_VLTH->index); }
      }
    }//Lep-jet-bjet + boost
    else{
      std::string s_boost = "";
      if(sprop_J){s_boost  = sprop_J->name; }
      if(sprop_M){s_boost  = sprop_M->name; }
      if(sprop_TH){s_boost = sprop_TH->name; }
      else{

	if(sprop_T){
	  if(!s_boost.empty()){ s_boost += "-"; }
	  s_boost += sprop_T->name;
	}

	if(sprop_L){
	  if(!s_boost.empty()){ s_boost += "-"; }
	  s_boost += sprop_T->name;
	}

	if(sprop_H){
	  if(!s_boost.empty()){ s_boost += "-"; }
	  s_boost += sprop_H->name;
	}

	if(sprop_V){
	  if(!s_boost.empty()){ s_boost += "-"; }
	  s_boost += sprop_V->name;
	}

	if(sprop_LT){
	  if(!s_boost.empty()){ s_boost += "-"; }
	  s_boost += sprop_LT->name;
	}

	if(sprop_VT){
	  if(!s_boost.empty()){ s_boost += "-"; }
	  s_boost += sprop_VT->name;
	}

	if(sprop_VLT){
	  if(!s_boost.empty()){ s_boost += "-"; }
	  s_boost += sprop_VLT->name;
	}

	if(sprop_VLTH){
	  if(!s_boost.empty()){ s_boost += "-"; }
	  s_boost += sprop_VLTH->name;
	}

      }
      if(!(sprop_MetSig || sprop_MetCut || sprop_MLL || sprop_MVA) ){
	AddAncestor(*sel, "c-" + sprop_lep->name + "-" + s_boost + "-" + sprop_jet->name + "-" + sprop_bjet->name, true);
	if(sprop_Mbb){
	  SelectorBase::AddAncestor(*sel, sprop_Mbb->index);
	}
	else if(sprop_Mtb){
	  SelectorBase::AddAncestor(*sel, sprop_Mtb->index);
	}
      }//Lep-jet-bjet-boost-mt[b]b
      else{
	std::string msplit = "";
	if(sprop_Mbb){ msplit = "-"+sprop_Mbb->name; }
	else if(sprop_Mtb){ msplit = "-"+sprop_Mtb->name; }
	AddAncestor(*sel, "c-" + sprop_lep->name + "-" + s_boost + "-" + sprop_jet->name + "-" + sprop_bjet->name + msplit, true);
	if(sprop_MetSig){
	  SelectorBase::AddAncestor(*sel, sprop_MetSig->index);
	}
	if(sprop_MetCut){
	  SelectorBase::AddAncestor(*sel, sprop_MetCut->index);
        }
	if(sprop_MLL){
	  SelectorBase::AddAncestor(*sel, sprop_MLL->index);
	}
	if(sprop_MVA){
	  SelectorBase::AddAncestor(*sel, sprop_MVA->index);
	}
      }//Lep-jet-bjet-boost-mt[b]b-metsig-metcut-mll

    }//post-boost

    if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" Selection "<<sel_name<<" successfully added"<<std::endl; }
  }//if not el or mu channel

  return sel;
}

//______________________________________________________________________________
//
bool VLQ_Selector::PassSelection(const int index){

  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" Calling PassSelection for index "<<index<<std::endl; }
  bool pass = true;

  //== Lepton channels ==
  if(index == c_all){
    pass = true;
  }
  else if(index == c_1l_chan){
    pass = (m_outData->o_channel_type == VLQ_Enums::ELECTRON) || (m_outData->o_channel_type == VLQ_Enums::MUON);
  }
  else if(index == c_1el_chan){
    pass = (m_outData->o_channel_type == VLQ_Enums::ELECTRON);
  }
  else if(index == c_1mu_chan){
    pass = (m_outData->o_channel_type == VLQ_Enums::MUON);
  }

  else if(index == c_2l_chan){
    pass = (m_outData->o_channel_type == VLQ_Enums::ELEL) || (m_outData->o_channel_type == VLQ_Enums::MUMU)
      || (m_outData->o_channel_type == VLQ_Enums::ELMU) ;
  }
  else if(index == c_ee_chan){
    pass = (m_outData->o_channel_type == VLQ_Enums::ELEL);
  }
  else if(index == c_mumu_chan){
    pass = (m_outData->o_channel_type == VLQ_Enums::MUMU);
  }
  else if(index == c_emu_chan){
    pass = (m_outData->o_channel_type == VLQ_Enums::ELMU);
  }
  else if(index == c_sf_chan){
    pass = (m_outData->o_channel_type == VLQ_Enums::ELEL) || (m_outData->o_channel_type == VLQ_Enums::MUMU);
  }

  else if(index == c_0l_chan){
    pass = (m_outData->o_channel_type == VLQ_Enums::FULLHAD);
  }

  //== MtbMin ===
  else if(index == c_LowMtbmin){
    pass = m_anaTools->PassBTagRequirement(2, false) ? (m_outData->o_mTbmin < m_opt->MtbminCut()) : (m_outData->o_mTbmin < 120.);
  }
  else if(index == c_HighMtbmin){
    pass = m_anaTools->PassBTagRequirement(2, false) ? (m_outData->o_mTbmin >= m_opt->MtbminCut()) : (m_outData->o_mTbmin >= 120.);
  }

  //== Mbb ===
  else if(index == c_LowMbb){
    pass = (m_outData->o_mbb_mindR < 100);
  }
  else if(index == c_HighMbb){
    pass = (m_outData->o_mbb_mindR >= 100);
  }

  //=== Metsig ====
  else if(index == c_LowMetSig){
    pass = m_opt->ApplyMetSigObjCut() ? (m_outData->o_metsig_obj < 5.) : (m_outData->o_metsig_ev < 7.);
  }
  else if(index == c_HighMetSig){
    pass = m_opt->ApplyMetSigObjCut() ? (m_outData->o_metsig_obj > 5.) : (m_outData->o_metsig_ev > 7.);
  }
  
  //=== Metcut ====
  else if(index == c_LowMetCut && m_opt->ApplyMetRegionsCut()){
    pass = m_outData->o_AO_met->Pt() < m_opt->MetRegionsCut();
  }
  else if(index == c_HighMetCut && m_opt->ApplyMetRegionsCut()){
    pass = m_outData->o_AO_met->Pt() > m_opt->MetRegionsCut();
  }

  //==== MLL ======
  else if(index == c_HighMLL){
    pass = (m_outData->o_mll > 15.);
  }
  else if(index == c_ZwinMLL){
    pass = (fabs(m_outData->o_mll-91.) < 10.);
  }

  //==== MVAScore ====
  else if(index == c_LowMVAScore){
    if(m_opt->DoOneLeptonAna()){
      pass = (m_outData->o_MVAScore < m_opt->LowMVACut1Lep());
      //pass = (m_outData->o_MVAScore < 0.50);
    }
    else if(m_opt->DoZeroLeptonAna()){
      pass = (m_outData->o_MVAScore < m_opt->LowMVACut0Lep());
    }
    else{
      std::cout << "< WARNING! > : Trying to use an MVA region for a channel that is not the 0 lepton or 1 lepton channel." << std::endl;
      pass = false;
    }
  }
  else if(index == c_MidMVAScore){
    if(m_opt->DoOneLeptonAna()){
      pass = (m_outData->o_MVAScore >= m_opt->LowMVACut1Lep()) && (m_outData->o_MVAScore < m_opt->HighMVACut1Lep());
      //pass = (m_outData->o_MVAScore >= 0.50) && (m_outData->o_MVAScore < 0.90);
    }
    else if(m_opt->DoZeroLeptonAna()){
      pass = (m_outData->o_MVAScore >= m_opt->LowMVACut0Lep()) && (m_outData->o_MVAScore < m_opt->HighMVACut0Lep());
    }
    else{
      std::cout << "< WARNING! > : Trying to use an MVA region for a channel that is not the 0 lepton or 1 lepton channel." << std::endl;
      pass = false;
    }
  }
  else if(index == c_HighMVAScore){
    if(m_opt->DoOneLeptonAna()){
      pass = (m_outData->o_MVAScore >= m_opt->HighMVACut1Lep());
      //pass = (m_outData->o_MVAScore >= 0.90);
    }
    else if(m_opt->DoZeroLeptonAna()){
      pass = (m_outData->o_MVAScore >= m_opt->HighMVACut0Lep());
    }
    else{
      std::cout << "< WARNING! > : Trying to use an MVA region for a channel that is not the 0 lepton or 1 lepton channel." << std::endl;
      pass = false;
    }
  }

  //=========== Jet multiplicities ====================
  else if(index == c_3jex){ pass = (m_outData->o_jets_n == 3); }
  else if(index == c_4jex){ pass = (m_outData->o_jets_n == 4); }
  else if(index == c_5jex){ pass = (m_outData->o_jets_n == 5); }
  else if(index == c_6jex){ pass = (m_outData->o_jets_n == 6); }
  else if(index == c_7jex){ pass = (m_outData->o_jets_n == 7); }
  else if(index == c_8jex){ pass = (m_outData->o_jets_n == 8); }
  else if(index == c_9jex){ pass = (m_outData->o_jets_n == 9); }
  else if(index == c_10jex){ pass = (m_outData->o_jets_n == 10); }
  else if(index == c_11jex){ pass = (m_outData->o_jets_n == 11); }

  else if(index == c_3_5jwin){ pass = (m_outData->o_jets_n >= 3) && (m_outData->o_jets_n <= 5); }
  else if(index == c_6_8jwin){ pass = (m_outData->o_jets_n >= 6) && (m_outData->o_jets_n <= 8); }

  else if(index == c_2jin){ pass = (m_outData->o_jets_n >= 2); }
  else if(index == c_3jin){ pass = (m_outData->o_jets_n >= 3); }
  else if(index == c_4jin){ pass = (m_outData->o_jets_n >= 4); }
  else if(index == c_5jin){ pass = (m_outData->o_jets_n >= 5); }
  else if(index == c_6jin){ pass = (m_outData->o_jets_n >= 6); }
  else if(index == c_7jin){ pass = (m_outData->o_jets_n >= 7); }
  else if(index == c_8jin){ pass = (m_outData->o_jets_n >= 8); }
  else if(index == c_9jin){ pass = (m_outData->o_jets_n >= 9); }
  else if(index == c_10jin){ pass = (m_outData->o_jets_n >= 10); }
  else if(index == c_11jin){ pass = (m_outData->o_jets_n >= 11); }
  else if(index == c_12jin){ pass = (m_outData->o_jets_n >= 12); }

  else if(index == c_0fjin){ pass = (m_outData->o_fwdjets_n >= 0); }
  else if(index == c_0fjex){ pass = (m_outData->o_fwdjets_n == 0); }
  else if(index == c_1fjin){ pass = (m_outData->o_fwdjets_n >= 1); }
  else if(index == c_1fjex){ pass = (m_outData->o_fwdjets_n == 1); }
  else if(index == c_2fjin){ pass = (m_outData->o_fwdjets_n >= 2); }

  //=========== RC Tag multiplicities ====================
  else if(index == c_0Jex){ pass = (m_outData->o_rcjets_n == 0); }
  else if(index == c_1Jex){ pass = (m_outData->o_rcjets_n == 1); }
  else if(index == c_1Jin){ pass = (m_outData->o_rcjets_n >= 1); }
  else if(index == c_2Jex){ pass = (m_outData->o_rcjets_n == 2); }
  else if(index == c_2Jin){ pass = (m_outData->o_rcjets_n >= 2); }
  else if(index == c_3Jin){ pass = (m_outData->o_rcjets_n >= 3); }
  else if(index == c_0_2Jwin0Hex1Tex){ pass = (m_outData->o_rcjets_n < 3) && (m_outData->o_taggedjets_n.at("RCMHiggs") == 0) &&
                                              (m_outData->o_taggedjets_n.at("RCMTop") == 1);}

  else if(index == c_0Mex){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") == 0); }
  else if(index == c_1Mex){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") == 1); }
  else if(index == c_1Min){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") >= 1); }
  else if(index == c_2Min){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") >= 2); }
  else if(index == c_1Mex3Jin){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") == 1) && (m_outData->o_rcjets_n >= 3); }
  else if(index == c_1Mex3Jin0Hex){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") == 1) && (m_outData->o_rcjets_n >= 3) &&
                                           (m_outData->o_taggedjets_n.at("RCMHiggs") == 0); }
  else if(index == c_1Mex3Jin1Hin){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") == 1) && (m_outData->o_rcjets_n >= 3) &&
                                           (m_outData->o_taggedjets_n.at("RCMHiggs") >= 1); }
  else if(index == c_2Min3Jin){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") >= 2) && (m_outData->o_rcjets_n >= 3); }
  else if(index == c_2Min0Hex){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") >= 2) && (m_outData->o_taggedjets_n.at("RCMHiggs") == 0); }
  else if(index == c_2Min1Hin){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") >= 2) && (m_outData->o_taggedjets_n.at("RCMHiggs") >= 1); }
  else if(index == c_2Min3Jin0Hex){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") >= 2) && (m_outData->o_rcjets_n >= 3) && 
                                           (m_outData->o_taggedjets_n.at("RCMHiggs") == 0); }
  else if(index == c_2Min3Jin1Hin){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") >= 2) && (m_outData->o_rcjets_n >= 3) &&
                                           (m_outData->o_taggedjets_n.at("RCMHiggs") >= 1); }
  else if(index == c_1Mex0Hex1Tex){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") == 1) && (m_outData->o_taggedjets_n.at("RCMHiggs") == 0) &&
                                           (m_outData->o_taggedjets_n.at("RCMTop") == 1);}
  else if(index == c_0Tex){ pass = (m_outData->o_taggedjets_n.at("RCMTop") == 0); }
  else if(index == c_0Tin){ pass = (m_outData->o_taggedjets_n.at("RCMTop") >= 0); }
  else if(index == c_1Tex){ pass = (m_outData->o_taggedjets_n.at("RCMTop") == 1); }
  else if(index == c_0_1Twin){ pass = (m_outData->o_taggedjets_n.at("RCMTop") == 0 || m_outData->o_taggedjets_n.at("RCMTop") == 1); }
  else if(index == c_1Tin){ pass = (m_outData->o_taggedjets_n.at("RCMTop") >= 1); }
  else if(index == c_2Tin){ pass = (m_outData->o_taggedjets_n.at("RCMTop") >= 2); }

  else if(index == c_0Lex){ pass = (m_outData->o_leptop_n == 0); }
  else if(index == c_0Lin){ pass = (m_outData->o_leptop_n >= 0); }
  else if(index == c_1Lex){ pass = (m_outData->o_leptop_n == 1); }
  else if(index == c_0_1Lwin){ pass = (m_outData->o_leptop_n == 0 || m_outData->o_leptop_n == 1); }
  else if(index == c_1Lin){ pass = (m_outData->o_leptop_n >= 1); }
  else if(index == c_2Lin){ pass = (m_outData->o_leptop_n >= 2); }
  else if(index == c_1LTin){ pass = (m_outData->o_leptop_n+m_outData->o_taggedjets_n.at("RCMTop") >= 1); }
  else if(index == c_2LTin){ pass = (m_outData->o_leptop_n+m_outData->o_taggedjets_n.at("RCMTop") >= 2); }

  else if(index == c_0Hex){ pass = (m_outData->o_taggedjets_n.at("RCMHiggs") == 0); }
  else if(index == c_0Hin){ pass = (m_outData->o_taggedjets_n.at("RCMHiggs") >= 0); }
  else if(index == c_1Hex){ pass = (m_outData->o_taggedjets_n.at("RCMHiggs") == 1); }
  else if(index == c_1Hin){ pass = (m_outData->o_taggedjets_n.at("RCMHiggs") >= 1); }
  else if(index == c_2Hin){ pass = (m_outData->o_taggedjets_n.at("RCMHiggs") >= 2); }
  else if(index == c_0_1Hwin){ pass = (m_outData->o_taggedjets_n.at("RCMHiggs") == 0 || m_outData->o_taggedjets_n.at("RCMHiggs") == 1); }

  else if(index == c_0Vex){ pass = (m_outData->o_taggedjets_n.at("RCMV") == 0); }
  else if(index == c_0Vin){ pass = (m_outData->o_taggedjets_n.at("RCMV") >= 0); }
  else if(index == c_1Vex){ pass = (m_outData->o_taggedjets_n.at("RCMV") == 1); }
  else if(index == c_0_1Vwin){ pass = (m_outData->o_taggedjets_n.at("RCMV") == 0 || m_outData->o_taggedjets_n.at("RCMV") == 1); }
  else if(index == c_1Vin){ pass = (m_outData->o_taggedjets_n.at("RCMV") >= 1); }
  else if(index == c_2Vin){ pass = (m_outData->o_taggedjets_n.at("RCMV") >= 2); }
  else if(index == c_3Vin){ pass = (m_outData->o_taggedjets_n.at("RCMV") >= 3); }

  else if(index == c_0THex){ pass = (m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_taggedjets_n.at("RCMHiggs") == 0); }
  else if(index == c_1THex){ pass = (m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_taggedjets_n.at("RCMHiggs") == 1); }
  else if(index == c_1THin){ pass = (m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_taggedjets_n.at("RCMHiggs") >= 1); }
  else if(index == c_2THex){ pass = (m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_taggedjets_n.at("RCMHiggs") == 2); }
  else if(index == c_2THin){ pass = (m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_taggedjets_n.at("RCMHiggs") >= 2); }
  else if(index == c_3THin){ pass = (m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_taggedjets_n.at("RCMHiggs") >= 3); }

  else if(index == c_0LTex){ pass = (m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_leptop_n == 0); }
  else if(index == c_1LTex){ pass = (m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_leptop_n == 1); }
  else if(index == c_1LTin){ pass = (m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_leptop_n >= 1); }
  else if(index == c_2LTin){ pass = (m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_leptop_n >= 2); }

  else if(index == c_0VTex){ pass = (m_outData->o_taggedjets_n.at("RCMV")+m_outData->o_taggedjets_n.at("RCMTop") == 0); }
  else if(index == c_1VTex){ pass = (m_outData->o_taggedjets_n.at("RCMV")+m_outData->o_taggedjets_n.at("RCMTop") == 1); }
  else if(index == c_0_1VTwin){pass = ( (m_outData->o_taggedjets_n.at("RCMV")+m_outData->o_taggedjets_n.at("RCMTop") == 0) ||
                                        (m_outData->o_taggedjets_n.at("RCMV")+m_outData->o_taggedjets_n.at("RCMTop") == 1) ); }
  else if(index == c_1VTin){ pass = (m_outData->o_taggedjets_n.at("RCMV")+m_outData->o_taggedjets_n.at("RCMTop") >= 1); }
  else if(index == c_2VTin){ pass = (m_outData->o_taggedjets_n.at("RCMV")+m_outData->o_taggedjets_n.at("RCMTop") >= 2); }
  else if(index == c_2VTor){ pass = ((m_outData->o_taggedjets_n.at("RCMV") >= 2) || (m_outData->o_taggedjets_n.at("RCMTop") >= 2)); }

  else if(index == c_0VLTex){ pass = (m_outData->o_taggedjets_n.at("RCMV")+m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_leptop_n == 0); }
  else if(index == c_1VLTex){ pass = (m_outData->o_taggedjets_n.at("RCMV")+m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_leptop_n == 1); }
  else if(index == c_1VLTin){ pass = (m_outData->o_taggedjets_n.at("RCMV")+m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_leptop_n >= 1); }
  else if(index == c_2VLTin){ pass = (m_outData->o_taggedjets_n.at("RCMV")+m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_leptop_n >= 2); }

  else if(index == c_0VLTHex){ pass = (m_outData->o_taggedjets_n.at("RCMV")+m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_leptop_n+m_outData->o_taggedjets_n.at("RCMHiggs") == 0); }
  else if(index == c_1VLTHin){ pass = (m_outData->o_taggedjets_n.at("RCMV")+m_outData->o_taggedjets_n.at("RCMTop")+m_outData->o_leptop_n+m_outData->o_taggedjets_n.at("RCMHiggs") >= 1); }

  //=========== B-tag multiplicities ====================
  else if(index == c_0bex){ pass = m_anaTools->PassBTagRequirement(0, false); }
  else if(index == c_1bex){ pass = m_anaTools->PassBTagRequirement(1, false); }
  else if(index == c_2bex){ pass = m_anaTools->PassBTagRequirement(2, false); }
  else if(index == c_3bex){ pass = m_anaTools->PassBTagRequirement(3, false); }

  else if(index == c_0bin){ pass = m_anaTools->PassBTagRequirement(0, true); }
  else if(index == c_1bin){ pass = m_anaTools->PassBTagRequirement(1, true); }
  else if(index == c_2bin){ pass = m_anaTools->PassBTagRequirement(2, true); }
  else if(index == c_3bin){ pass = m_anaTools->PassBTagRequirement(3, true); }
  else if(index == c_4bin){ pass = m_anaTools->PassBTagRequirement(4, true); }
  else if(index == c_1_2bwin){ pass = (m_anaTools->PassBTagRequirement(1, false)) || (m_anaTools->PassBTagRequirement(2, false)); }

  return pass;

}

//______________________________________________________________________________
//
int VLQ_Selector::GetSelectionIndex( const std::string& sel_name ){
  std::map<std::string, int>::iterator selit = m_sel_indices -> find( sel_name );

  if(selit == m_sel_indices->end()){
    std::cout<<"VLQ_Selector::GetSelectionIndex --> Cannot find index for selection with name "<<sel_name<<std::endl;
    return -1;
  }
  return selit->second;

}

//______________________________________________________________________________
//
std::string VLQ_Selector::GetSelectionName( const int sel_ind){
  std::map<int, std::string>::iterator selit = m_sel_names -> find( sel_ind );
  if(selit == m_sel_names->end()){
    std::cout<<"VLQ_Selector::GetSelectionName --> Cannot find name for selection with index "<<sel_ind<<std::endl;
    return "";
  }

  return selit->second;

}

//______________________________________________________________________________
//Adds a new selection index to the map. If !newentry
int VLQ_Selector::AddSelectionIndex( const std::string& sel_name, int index, bool newentry){

  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" Calling AddSelectionIndex("<<sel_name<<", "<<index<<","<<newentry<<")"<<std::endl; }
  int sel_index = -1;
  std::pair< std::map<string, int>::iterator, bool > selname_pair = m_sel_indices -> insert( std::pair< string, int >(sel_name, sel_index) );
  if(!selname_pair.second){
    if(newentry){
      std::cerr << " Selection at index "<<selname_pair.first->second<<" already has name "<<sel_name<<". Please check"<<std::endl;
      return -1;
    }
    else{ sel_index = selname_pair.first->second; }
  }
  else{
    sel_index = (index >= 0) ? index : std::max(m_sel_names->rbegin()->first,(int)TOPSEL_MAX)+1;
  }
  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" Selection name "<<sel_name<<" assigned to index "<<sel_index<<std::endl; }
  selname_pair.first->second = sel_index;
  m_sel_names->insert(std::pair<int, std::string>(sel_index, sel_name));
  return sel_index;

}

//______________________________________________________________________________
//
bool VLQ_Selector::AddAncestor( Selection& sel, const std::string& anc_name, bool is_primary){

  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" Adding "<<anc_name<<" as ancestor of "<<sel.Name()<<" is_primary = "<<is_primary<<std::endl; }
  bool stat = SelectorBase::AddAncestor(sel, AddSelectionIndex( anc_name ), is_primary);

  return stat;

}

//______________________________________________________________________________
//
bool VLQ_Selector::AddAncestors(Selection& sel, const std::vector<std::string> &anclist, const std::string& primary){

  bool found_primary = false;
  for(const std::string& anc : anclist){
    bool is_primary = (!found_primary) && (anc==primary);
    if(is_primary){ found_primary = true; }
    AddAncestor(sel, anc, is_primary);
  }
  if(!found_primary && !primary.empty()){ AddPrimary(sel, primary); }

  return true;
}

//______________________________________________________________________________
//
bool VLQ_Selector::AddPrimary( Selection& sel, const std::string& anc_name){

  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" Adding "<<anc_name<<" as primary of "<<sel.Name()<<std::endl; }
  bool stat = SelectorBase::AddPrimary(sel, AddSelectionIndex( anc_name));
  return stat;

}

//______________________________________________________________________________
//
bool VLQ_Selector::RunOperations(const Selection& sel) const{

  m_anaTools -> UpdateRegionDependentWeight( AnalysisUtils::ReplaceString(sel.Name(),"-","" ) );

  // std::cout << "Current selection " << sel.Name() << std::endl;

  if( m_opt->DoBlind() && !(sel.PassBlindingCuts()) ){ return true; } //blind if needed

  std::string histName = AnalysisUtils::ReplaceString(sel.Name(),"-", "");
  m_anaTools -> FillAllHistograms(histName,m_outData->o_VLQtype/*,sel.PassFlagAtBit(DOSYST)*/);
  return true;

}

//______________________________________________________________________________
//
bool VLQ_Selector::AddBlindingCut(Selection& sel){

  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"VLQ_Selector::AddBlindingCut FOR "<<sel.Name()<<std::endl; }
  std::string line="";
  std::string region="";
  std::string blind_bin="";
  bool found_reg=false;
  std::string delim = ":\"";
  double blind_cut = 0.;

  std::string selName = AnalysisUtils::ReplaceString(sel.Name(),"-","");

  m_blinding_config.clear();
  m_blinding_config.seekg(0, ios::beg);

  while(std::getline(m_blinding_config, line)){
    AnalysisUtils::TrimString(line); //remove all whitespaces
    if(line.find("#") != std::string::npos){ line = line.substr(0,line.find("#")); } //remove comments

    if(found_reg){
      if(line.find("binning_blind") != std::string::npos){
        if(line.find("\"\"") != std::string::npos){blind_cut = 0.;} //empty bins
        else{
          line=AnalysisUtils::ReplaceString(line,"\",",""); //remove the comma and quotation mark at the end of the line
          blind_bin=line.substr(line.find_last_of(",")+1);
          blind_cut=atof(blind_bin.c_str());
        }
        break;
      }
      else{ continue; }
    }
    else{
      if( line.find("name") != std::string::npos ){
        region = line.substr( line.find(delim) + delim.size() );
        region=AnalysisUtils::ReplaceString(region,"\",","");  //remove the comma and quotation mark at the end of the line
        if(region == "HTX_"+selName){ found_reg = true; }
      }
      else{ continue; }
    }

  }

  if( (sel.PassFlagAtBit(FIT) || sel.PassFlagAtBit(VALIDATION)) || found_reg ){ //Blind fit and validation regions completely if not found in dictionary
    if(!found_reg){
      std::cout<<" VLQ_Selector::WARNING "<<" region "<<selName<<" was not found in the blinding configuration file. Blinding fully"<<std::endl;
    }
    else if(blind_bin.empty()){
      std::cout<<" VLQ_Selector::WARNING "<<" region "<<selName<<" is fully blind "<<std::endl;
    }
    sel.AddCut("blind_meff", &(m_outData->o_meff), blind_cut, "<", -1, "", true);
  }
  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"VLQ_Selector::AddBlindingCut Selection "<<sel.Name()<<" blinding cut on meff = "<<blind_cut<<std::endl; }

  return true;
}

//______________________________________________________________________________
//
VLQ_Selector::SelProp VLQ_Selector::MakeSelProp(const std::string& _name, int _index, const std::string& _primanc_name){

  SelProp selProp;
  selProp.index = (_index<0) ? AddSelectionIndex(_name) : _index;
  selProp.name = _name;
  selProp.primanc_name = _primanc_name;

  return selProp;
}
