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
  m_sel_MVAScore_prop(NULL)
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
  for(auto selprop : *m_sel_MLL_prop){ AddSelectionIndex(selprop.name, selprop.index); }
  for(auto selprop : *m_sel_MVAScore_prop){ AddSelectionIndex(selprop.name, selprop.index); }

  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<"Added all top selections "<<std::endl; }
  //========================== Add desired regions ================================================

  bool do_runop = true;

  std::map<std::string, std::vector<std::string> > ch_lep;
  if(m_opt->DoOneLeptonAna()){
    if(m_opt->DoSplitEMu()) ch_lep["c-1lep-"] = {"", "_el", "_mu"};
    else ch_lep["c-1lep-"] = {""};
  }
  if(m_opt->DoTwoLeptonAna()){
    if(m_opt->DoSplitEMu()) ch_lep["c-2lep-"] = {"", "_ee", "_mumu", "_emu", "_sf"};
    else ch_lep["c-2lep-"] = {"_sf"};
  }
  if(m_opt->DoZeroLeptonAna()){
    ch_lep["c-0lep-"] = {""};
  }
  std::string nolep_prefix = "c-0lep-";

  std::map<std::string, std::vector<std::string> > ch_mll;

  if(m_opt -> DoOneLeptonAna()) ch_mll["c-1lep-"] = {""};

  if(m_opt -> DoTwoLeptonAna()){

    if(m_opt->VerboseOutput()) ch_mll["c-2lep-"] = {"", "-HighMLL", "-ZwinMLL"};

    else ch_mll["c-2lep-"] = {"-ZwinMLL"};

  }

  if(m_opt->DoZeroLeptonAna()){
    ch_mll["c-0lep-"] = {""};
  }

  std::vector<std::string> ch_metsig; ch_metsig.clear();
  if(m_opt->ApplyMetSignificanceCut()) ch_metsig = {"", "-LowMetSig", "-HighMetSig"};
  else ch_metsig = {""};


  //======== PRESELECTION=========
  if(m_opt->DoPreselection()){

    AddVLQSelection("c-all",do_runop, false, PRESEL);

    std::vector<std::string> v_bjet_presel = {"1bin","2bin","2bex","3bin","3bex","4bin"};
    if(m_opt->DoLowBRegions()){
      v_bjet_presel.push_back("0bin");
      v_bjet_presel.push_back("0bex");
      v_bjet_presel.push_back("1bex");
    }

    std::vector<std::string> v_jet_presel = {};

    std::vector<std::string> ch_mtb_presel = {""};

    if(m_opt->DoSingleVLQRegions()){
      v_jet_presel = {"3_5jwin","3jin","4jin","5jin","6jin"};

      if(m_opt->DeriveReweighting() ||  m_opt->DoExclusiveJetRegions()){
	std::vector<std::string> v_jet_presel_ex = {"3jex","4jex","5jex","6jex","7jex","8jex","9jex","10jex","11jex"};
	v_jet_presel.insert(v_jet_presel.end(),v_jet_presel_ex.begin(),v_jet_presel_ex.end());

	if(m_opt->DeriveReweighting()){
	  std::vector<std::string> v_jet_presel_in = {"7jin","8jin","9jin"};
	  v_jet_presel.insert(v_jet_presel.end(),v_jet_presel_in.begin(),v_jet_presel_in.end());
	}
      }

    }//sVLQ
    if(m_opt->DoPairVLQRegions()){
      if(m_opt->DoZeroLeptonAna()){
	v_jet_presel.push_back("6jin");
	v_jet_presel.push_back("7jin");
	if(m_opt->DoLowJRegions()){
	  v_jet_presel.push_back("5jex");
	  v_jet_presel.push_back("5jin");
	}

	if(m_opt->DoExclusiveJetRegions()){
	  std::vector<std::string> v_jet_presel_ex = {"6jex","7jex","8jex","9jin"};
	  v_jet_presel.insert(v_jet_presel.end(),v_jet_presel_ex.begin(),v_jet_presel_ex.end());
	}

      }
      if(m_opt->DoOneLeptonAna() || m_opt->DoTwoLeptonAna()){
	v_jet_presel.push_back("6jin");
	v_jet_presel.push_back("5jin");
	if(m_opt->DoLowJRegions()){
	  v_jet_presel.push_back("4jex");
	  v_jet_presel.push_back("4jin");
	}

	if(m_opt->DoExclusiveJetRegions()){
	  std::vector<std::string> v_jet_presel_ex = {"5jex","6jex","7jex","8jex","9jin"};
	  v_jet_presel.insert(v_jet_presel.end(),v_jet_presel_ex.begin(),v_jet_presel_ex.end());
	}

      }
      if(m_opt->DoSplitMtb()){
	ch_mtb_presel.push_back("-LowMtbmin");
	ch_mtb_presel.push_back("-HighMtbmin");
      }
    }//pVLQ

    std::vector<std::string> ch_fjet = {"",/*"-0fjex","-1fjin"*/};

    std::set<std::string> set_jet_presel(v_jet_presel.begin(), v_jet_presel.end());
    for( std::pair<std::string, std::vector<std::string> > lep_ch_pair : ch_lep ){

      const std::string& lep_prefix = lep_ch_pair.first;
      for(const std::string& jet : set_jet_presel){
	for(const std::string& bjet : v_bjet_presel){
	  for(const std::string& mtbsuf : ch_mtb_presel){
	    for(const std::string& lepsuf : lep_ch_pair.second){
	      for(const std::string& mllsuf : ch_mll[lep_prefix]){
		for(const std::string& metsuf : ch_metsig){
		  for(const std::string& fjet : ch_fjet){
		    AddVLQSelection(lep_prefix+jet+"-"+bjet+mtbsuf+fjet+metsuf+mllsuf+lepsuf, do_runop, m_opt->DoPreselSys(), PRESEL);
		  }
		}//metsig channels
	      }//mll channels
	    }//lepflav channels
	  } //mtb channels
	}//bjet
      }//jet

      if(m_opt->DoSingleVLQRegions() && m_opt->DoExtendedPreselection()){
	std::vector<std::string> v_supr_svlq = {"0fjex", "1fjin"}; //, "1fjin-0Hex-0Vex"};
	for(const std::string& supr : v_supr_svlq){
	  for(const std::string& lepsuf : lep_ch_pair.second){
	    AddVLQSelection(lep_prefix+"3_5jwin-1bin-"+supr+lepsuf, do_runop, m_opt->DoPreselSys(), PRESEL);
	    AddVLQSelection(lep_prefix+"6jin-1bin-"+supr+lepsuf, do_runop, m_opt->DoPreselSys(), PRESEL);
	  }//lepflav channels
	}//signal suppressing
      }//Extended preselection
    }//Lepton channels

    // MVA Training region                                                                                                                                                
    if(m_opt->ApplyMVA() && m_opt->DoOneLeptonAna() && m_opt->DoPairVLQRegions()){
      std::vector<std::string> v_mva_jet_presel   = {"5jin", "5jex", "6jin"};
      std::vector<std::string> v_mva_bjet_presel  = {"3bin", "3bex", "4bin"};
      std::vector<std::string> v_mva_boost_presel = {"2Min3Jin", "2Min3Jin0Hex", "2Min3Jin1Hin"};
      std::vector<std::string> v_mva_score_presel = {"", "-HighMVAScore", "-MidMVAScore", "-LowMVAScore"};

      // ttbar CRs
      AddVLQSelection("c-1lep-5jin-2bex-0Hex-1Tex-1Lex", do_runop, m_opt->DoPreselSys(), PRESEL);
      AddVLQSelection("c-1lep-5jin-3bex-0Hex-1Tex-1Lex", do_runop, m_opt->DoPreselSys(), PRESEL);
      AddVLQSelection("c-1lep-5jin-4bin-0Hex-1Tex-1Lex", do_runop, m_opt->DoPreselSys(), PRESEL);
      
      AddVLQSelection("c-1lep-5jin-2bex-1Mex0Hex1Tex-1Lex", do_runop, m_opt->DoPreselSys(), PRESEL);
      AddVLQSelection("c-1lep-5jin-3bex-1Mex0Hex1Tex-1Lex", do_runop, m_opt->DoPreselSys(), PRESEL);
      AddVLQSelection("c-1lep-5jin-4bin-1Mex0Hex1Tex-1Lex", do_runop, m_opt->DoPreselSys(), PRESEL);

      AddVLQSelection("c-1lep-5jin-2bex-0_2Jwin0Hex1Tex-1Lex", do_runop, m_opt->DoPreselSys(), PRESEL);
      AddVLQSelection("c-1lep-5jin-3bex-0_2Jwin0Hex1Tex-1Lex", do_runop, m_opt->DoPreselSys(), PRESEL);
      AddVLQSelection("c-1lep-5jin-4bin-0_2Jwin0Hex1Tex-1Lex", do_runop, m_opt->DoPreselSys(), PRESEL);

      // background CRs
      AddVLQSelection("c-1lep-5jin-2bex-0Hex-1VTex-0_1Lwin", do_runop, m_opt->DoPreselSys(), PRESEL);
      AddVLQSelection("c-1lep-5jin-3bex-0Hex-1VTex-0_1Lwin", do_runop, m_opt->DoPreselSys(), PRESEL);
      AddVLQSelection("c-1lep-5jin-4bin-0Hex-1VTex-0_1Lwin", do_runop, m_opt->DoPreselSys(), PRESEL);

      AddVLQSelection("c-1lep-5jin-2bex-0Hex-1VTex-0Lex", do_runop, m_opt->DoPreselSys(), PRESEL);
      AddVLQSelection("c-1lep-5jin-3bex-0Hex-1VTex-0Lex", do_runop, m_opt->DoPreselSys(), PRESEL);
      AddVLQSelection("c-1lep-5jin-4bin-0Hex-1VTex-0Lex", do_runop, m_opt->DoPreselSys(), PRESEL);

      AddVLQSelection("c-1lep-5jin-2bex-0Hex-1VTex-1Lex", do_runop, m_opt->DoPreselSys(), PRESEL);
      AddVLQSelection("c-1lep-5jin-3bex-0Hex-1VTex-1Lex", do_runop, m_opt->DoPreselSys(), PRESEL);
      AddVLQSelection("c-1lep-5jin-4bin-0Hex-1VTex-1Lex", do_runop, m_opt->DoPreselSys(), PRESEL);

      for(const std::string &mva_jet_sel : v_mva_jet_presel){
	for(const std::string &mva_bjet_sel : v_mva_bjet_presel){
	  for(const std::string &mva_boost_presel : v_mva_boost_presel){
	    for(const std::string &mva_score_presel : v_mva_score_presel){
	      AddVLQSelection("c-1lep-"+mva_jet_sel+"-"+mva_bjet_sel+"-"+mva_boost_presel+mva_score_presel, do_runop, m_opt->DoPreselSys(), PRESEL);
	    }
	  }
	}
      }
    }

  }//Do Preselection

  //================================ FIT, VALIDATION, AND LOOSE SYST REGIONS ===================================
  if(m_opt->DoFitRegions() || m_opt->DoValidnRegions() || m_opt->DoLooseSystRegions()){
    bool do_syst = true;

    //======== SINGLE VLQ REGIONS =========
    if(m_opt->DoSingleVLQRegions()){

      for( std::pair<std::string, std::vector<std::string> > lep_ch_pair : ch_lep ){

	const std::string& lep_prefix = lep_ch_pair.first;

	for(const std::string& lepsuf : lep_ch_pair.second){

	  if(m_opt->DoFitRegions()){

	    // Signal regions
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

	  }//FIT REGIONS

	  if(m_opt->DoValidnRegions()){

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

	  }

	  // Loose regions for syst extrapolation
	  if(m_opt->DoLooseSystRegions()){

	    /*
	    // Signal regions
	    AddVLQSelection(lep_prefix+"3_5jwin-1bin-1fjin-0LTex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR1-a
	    AddVLQSelection(lep_prefix+"3_5jwin-1bin-1fjin-0Tex-1Lin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ); //SR1-b ORIG

	    AddVLQSelection(lep_prefix+"3_5jwin-1bin-1fjin-0LTex-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);    //SR2-a
	    AddVLQSelection(lep_prefix+"3_5jwin-1bin-1fjin-0Tex-1Lin-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);    //SR2-b ORIG
	    AddVLQSelection(lep_prefix+"3_5jwin-1bin-1fjin-1Tin-0Lex-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	    AddVLQSelection(lep_prefix+"6jin-1bin-1fjin-1Lex-0Tex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR3-a
	    AddVLQSelection(lep_prefix+"6jin-1bin-1fjin-0Lex-1Tex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR3-b
	    AddVLQSelection(lep_prefix+"6jin-1bin-1fjin-2LTin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);       //SR3-c

	    AddVLQSelection(lep_prefix+"6jin-1bin-1fjin-1Lex-0VTex-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);        //SR4-a
	    AddVLQSelection(lep_prefix+"6jin-1bin-1fjin-0Lex-1VTex-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);        //SR4-b
	    AddVLQSelection(lep_prefix+"6jin-1bin-1fjin-2VLTin-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);        //SR4-c

	    // Ttbar+HF control regions
	    AddVLQSelection(lep_prefix+"3_5jwin-1bin-0fjex-1Lin-0VTex-0Hex"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	    AddVLQSelection(lep_prefix+"6jin-1bin-0fjex-1Lin-0VTex-0Hex"+lepsuf, do_runop, do_syst, SINGLEVLQ);



	    // CONVENTIONAL VALIDATION REGIONS
	    AddVLQSelection(lep_prefix+"3_5jwin-1bin-0fjex-0Tex-0Lex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	    AddVLQSelection(lep_prefix+"3_5jwin-1bin-0fjex-0Tex-1Lin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	    AddVLQSelection(lep_prefix+"3_5jwin-1bin-1fjin-1LTin-0Hex-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	    AddVLQSelection(lep_prefix+"3_5jwin-1bin-1fjin-1Tin-0Lex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	    AddVLQSelection(lep_prefix+"3_5jwin-1bin-0fjex-0Tex-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	    AddVLQSelection(lep_prefix+"3_5jwin-1bin-1fjin-1VLTin-0Hex"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	    AddVLQSelection(lep_prefix+"6jin-1bin-0fjex-1LTex-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	    AddVLQSelection(lep_prefix+"6jin-1bin-0fjex-2LTin-0Hex-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	    AddVLQSelection(lep_prefix+"6jin-1bin-1fjin-0Tex-0Lex-1Hin-1Vin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	    AddVLQSelection(lep_prefix+"6jin-1bin-1fjin-2LTin-1Hin-0Vex"+lepsuf, do_runop, do_syst, SINGLEVLQ);

	    AddVLQSelection(lep_prefix+"6jin-1bin-0fjex-1VLTin-1Hin"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	    AddVLQSelection(lep_prefix+"6jin-1bin-1fjin-1VLTin-0Hex"+lepsuf, do_runop, do_syst, SINGLEVLQ);
	    */

	    //========== FWDJet INCLUSIVE ===================
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



	    // CONVENTIONAL VALIDATION REGIONS
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

	}//lepflav channels
      }//Lepton channels

    }//Single VLQ regions

    //======== PAIR VLQ REGIONS =========
    if(m_opt->DoPairVLQRegions()){

      bool do_syst = true;

      std::vector<std::string> bjet_analist = { "2bex", "3bex" ,"4bin"};
      if(!(m_opt->DoZeroLeptonAna())) bjet_analist.push_back("3bin"); 
      

      std::vector<std::string> boostlist_1L_default{};
      std::vector<std::string> boostlist_1L_valid_4b{};

      std::vector<std::string> boostlist_0L_valid_lowb{};
      std::vector<std::string> boostlist_0L_lowb{};
      std::vector<std::string> boostlist_0L_highb{};


      std::vector<std::string> boostlist_highstat = {
	"2Tin-0_1Hwin",
	"1Tex-1Hex", "1Tex-0Hex",
	"0Tex-1Hex", "0Tex-0Hex",
	"0Tin-2Hin"
      };

      std::vector<std::string> boostlist_lowstat = {
	"2THin", "1Tex-0Hex",
	"0Tex-1Hex", "0Tex-0Hex"
      };

      if(m_opt->DoZeroLeptonAna()){
	boostlist_0L_lowb.insert(boostlist_0L_lowb.end(), boostlist_highstat.begin(), boostlist_highstat.end());
	boostlist_0L_valid_lowb.insert(boostlist_0L_valid_lowb.end(), boostlist_highstat.begin(), boostlist_highstat.end());
	boostlist_0L_valid_lowb.push_back("2THin");
	boostlist_0L_highb.insert(boostlist_0L_highb.end(), boostlist_lowstat.begin(), boostlist_lowstat.end());
      }

      if(m_opt->DoOneLeptonAna() || m_opt->DoTwoLeptonAna()){
	boostlist_1L_default.insert(boostlist_1L_default.end(), boostlist_highstat.begin(), boostlist_highstat.end());
	boostlist_1L_valid_4b.insert(boostlist_1L_valid_4b.end(), boostlist_lowstat.begin(), boostlist_lowstat.end());
      }

      //Remove overlaps between different boost options so that any region is only added once
      std::set<std::string> boostset_1L_default(boostlist_1L_default.begin(), boostlist_1L_default.end());
      std::set<std::string> boostset_1L_valid_4b(boostlist_1L_valid_4b.begin(), boostlist_1L_valid_4b.end());

      std::set<std::string> boostset_0L_lowb(boostlist_0L_lowb.begin(), boostlist_0L_lowb.end());
      std::set<std::string> boostset_0L_valid_lowb(boostlist_0L_valid_lowb.begin(), boostlist_0L_valid_lowb.end());
      std::set<std::string> boostset_0L_highb(boostlist_0L_highb.begin(), boostlist_0L_highb.end());

      //0-lepton old analysis regions
      if(m_opt->DoZeroLeptonAna() && m_opt->DoOldPairProdRegions()){
	std::vector<std::string> jet_analist_0L{};
	if(m_opt->DoFitRegions()){ jet_analist_0L.push_back("7jin"); }
	if(m_opt->DoLowJRegions())  {
	  jet_analist_0L.push_back("5jex");
	}
	if(m_opt->DoValidnRegions()){
	  jet_analist_0L.push_back("6jex");
	}

	for( const std::string& jet : jet_analist_0L ){

	  const int reg_type = (jet=="6jex") ? VALIDATION : FIT;

	  std::vector<std::string> ch_mtb; ch_mtb.clear();
	  if(m_opt->DoSplitMtb()) ch_mtb = {"", "-LowMtbmin", "-HighMtbmin"};
	  else ch_mtb = {""};

	  for( const std::string& bjet : bjet_analist ){
	    const std::set<std::string>* boostset_0L = NULL;

	    if(bjet=="2bex"){ boostset_0L = &boostset_0L_valid_lowb; }
	    else if( (bjet=="3bex") && (reg_type == FIT) ){ boostset_0L = &boostset_0L_lowb; }
	    else if( (bjet=="4bin") || ((reg_type == VALIDATION) && (bjet == "3bex")) ){ boostset_0L = &boostset_0L_highb; }

	    for( const std::string& boost : *boostset_0L ){
	      for(const std::string& mtb : ch_mtb ){
	
		AddVLQSelection(nolep_prefix+boost+"-"+jet+"-"+bjet+mtb, do_runop, do_syst, reg_type);
		if(m_opt->ApplyMetSignificanceCut()){
		  AddVLQSelection(nolep_prefix+boost+"-"+jet+"-"+bjet+mtb+"-HighMetSig", do_runop, do_syst, reg_type);
		  AddVLQSelection(nolep_prefix+boost+"-"+jet+"-"+bjet+mtb+"-LowMetSig", do_runop, do_syst, reg_type);
		}
		/*if(m_opt->ApplyMetSignificanceCut() && (bjet=="2bex") ){
		  AddVLQSelection(nolep_prefix+boost+"-"+jet+"-"+bjet+mtb+"-HighMetSig", do_runop, do_syst, reg_type);
		  }*///metsig

	      }//mtb split
	    }//boost list
	  }//ana bjet channels
	}//jet
      }//0-lepton

      //0-lepton new analysis regions
      if(m_opt->DoZeroLeptonAna()){

	std::vector<std::string> ch_mtb; ch_mtb.clear();
	if(m_opt->DoSplitMtb()) ch_mtb = {"", "-LowMtbmin", "-HighMtbmin"};
	else ch_mtb = {""};

	// ZtZt optimized regions
	AddVLQSelection("c-0lep-7jin-2bex-0Hex-1VTex-HighMtbmin", do_runop, do_syst, FIT); 
	AddVLQSelection("c-0lep-7jin-2bex-0Hex-1Vex-1Tin-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-2bex-0Hex-0Vex-2Tin-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-2bex-0Hex-2Vin-0Tin-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-1VTex", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-1Vex-1Tin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-0Vex-2Tin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-2Vin-0Tin", do_runop, do_syst, FIT);
	// ZtZt splits
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-1VTex-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bin-0Hex-1Vex-1Tin-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bin-0Hex-0Vex-2Tin-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bin-0Hex-1VTex-LowMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-0Hex-1VTex-LowMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-0Hex-1VTex-LowMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bin-0Hex-1Vex-1Tin-LowMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bin-0Hex-0Vex-2Tin-LowMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-0Hex-0Vex-2Tin-LowMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-0Hex-0Vex-2Tin-LowMtbmin", do_runop, do_syst, FIT);
	// HtHt optimized regions
	AddVLQSelection("c-0lep-7jin-3bin-2Hin-0Vin-0Tin", do_runop, do_syst, FIT);
	// HtZt optimized regions
	AddVLQSelection("c-0lep-7jin-2bex-1Hin-0VTex-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-2bex-1Hin-1VTex-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-2bex-1Hin-2VTin-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-0VTex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-1VTex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-2VTin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-0VTex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-1VTex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-2VTin", do_runop, do_syst, FIT);
	// HtZt option 2
	AddVLQSelection("c-0lep-7jin-2bex-1Hin-0_1VTwin-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-0_1VTwin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-0_1VTwin", do_runop, do_syst, FIT);
	// HtZt option 3
	AddVLQSelection("c-0lep-7jin-2bex-1Hin-2VTor-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-2VTor", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-2VTor", do_runop, do_syst, FIT);
	// HtZt splits
	AddVLQSelection("c-0lep-7jin-3bex-1Hex-0VTex-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-0VTex-LowMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-1VTex-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-1VTex-LowMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-2VTin-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-2VTin-LowMtbmin", do_runop, do_syst, FIT);

        AddVLQSelection("c-0lep-7jin-4bin-1Hex-0VTex-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-0VTex-LowMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-1VTex-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-1VTex-LowMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-2VTin-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-2VTin-LowMtbmin", do_runop, do_syst, FIT);

        AddVLQSelection("c-0lep-7jin-2bex-1Hin-0_1Vwin-2Tin-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-2bex-1Hin-2Vin-0_1Twin-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-2bex-1Hin-1Vex-1Tex-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-0_1Vwin-2Tin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-2Vin-0_1Twin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-1Vex-1Tex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-0_1Vwin-2Tin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-2Vin-0_1Twin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-1Vex-1Tex", do_runop, do_syst, FIT);
	
	//v-------------------------------Second Iteration------------------------------------v//
	// ZtZt optimized regions
	/*AddVLQSelection("c-0lep-7jin-2bex-0Hex-1VTex-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-2bex-0Hex-1Vex-1Tin-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-2bex-0Hex-0Vex-2Tex-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-2bex-0Hex-2Vin-0Tin-HighMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bin-0Hex-1VTex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bin-0Hex-1Vex-1Tin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bin-0Hex-0Vex-2Tex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bin-0Hex-2Vin-0Tin", do_runop, do_syst, FIT);
	// HtHt optimized regions
        AddVLQSelection("c-0lep-7jin-2bex-2Hin-0Vin-0Tin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bin-2Hin-0Vin-0Tin", do_runop, do_syst, FIT);
	// HtZt optimied regions
        AddVLQSelection("c-0lep-7jin-2bex-1Hex-0VTex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-2bex-1Hex-1Vex-0Tex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-2bex-1Hex-2Vin-0Tex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-2bex-1Hex-1Vin-1Tex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-2bex-1Hex-0Vin-2Tin", do_runop, do_syst, FIT);

        AddVLQSelection("c-0lep-7jin-3bex-1Hex-0VTex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-1Vex-0Tex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-2Vin-0Tex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-1Vin-1Tex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bex-1Hex-0Vin-2Tin", do_runop, do_syst, FIT);

        AddVLQSelection("c-0lep-7jin-4bin-1Hex-0VTex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-1Vex-0Tex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-2Vin-0Tex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-1Vin-1Tex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-0Vin-2Tin", do_runop, do_syst, FIT);

	//v--------------------------Merges and splits--------------------------v
	// 0Hex Mtbmin split
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-1VTex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-1VTex-LowMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-1Vex-1Tin-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-1Vex-1Tin-LowMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bin-0Hex-0Vex-2Tex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-3bin-0Hex-0Vex-2Tex-LowMtbmin", do_runop, do_syst, FIT);
	// 1Hex BO merge
	AddVLQSelection("c-0lep-7jin-3bex-1Hex-1Vin-0Tex", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-4bin-1Hex-1Vin-0Tex", do_runop, do_syst, FIT);
	// 1Hex b-jet merge
	AddVLQSelection("c-0lep-7jin-3bin-1Hex-1Vin-1Tex", do_runop, do_syst, FIT);
	// 1Hex Mtbmin split
	AddVLQSelection("c-0lep-7jin-2bex-1Hex-0VTex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-2bex-1Hex-0VTex-LowMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-2bex-1Hex-1Vin-1Tex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-2bex-1Hex-1Vin-1Tex-LowMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-2bex-1Hex-0Vin-2Tin-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-2bex-1Hex-0Vin-2Tin-LowMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-3bex-1Hex-0VTex-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-3bex-1Hex-0VTex-LowMtbmin", do_runop, do_syst, FIT);
	// 2Hin Mtbmin split
	AddVLQSelection("c-0lep-7jin-2bex-2Hin-0Vin-0Tin-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-2bex-2Hin-0Vin-0Tin-LowMtbmin", do_runop, do_syst, FIT);
        AddVLQSelection("c-0lep-7jin-3bin-2Hin-0Vin-0Tin-HighMtbmin", do_runop, do_syst, FIT);
	AddVLQSelection("c-0lep-7jin-3bin-2Hin-0Vin-0Tin-LowMtbmin", do_runop, do_syst, FIT);*/
	//^--------------------------Merges and splits--------------------------^
	//^-------------------------------Second Iteration------------------------------------^//

	//v------------------------------- First Iteration------------------------------------v//
	/*for( const std::string& bjet : bjet_analist ){
	  
	  for(const std::string& mtbsuf : ch_mtb){

	    AddVLQSelection("c-0lep-7jin-"+bjet+"-0Hex-0Vin-1Tin"+mtbsuf, do_runop, do_syst, FIT);
	    AddVLQSelection("c-0lep-7jin-"+bjet+"-0Hex-1Vin-1Tex"+mtbsuf, do_runop, do_syst, FIT);
	    AddVLQSelection("c-0lep-7jin-"+bjet+"-0Hex-1Vin-2Tin"+mtbsuf, do_runop, do_syst, FIT);
	    AddVLQSelection("c-0lep-7jin-"+bjet+"-0Hex-2Vin-1Tex"+mtbsuf, do_runop, do_syst, FIT);
	    AddVLQSelection("c-0lep-7jin-"+bjet+"-0Hex-2Vin-2Tin"+mtbsuf, do_runop, do_syst, FIT);
	    AddVLQSelection("c-0lep-7jin-"+bjet+"-1Hex-0Vex-0Tex"+mtbsuf, do_runop, do_syst, FIT);
	    AddVLQSelection("c-0lep-7jin-"+bjet+"-1Hex-1Vin-0Tex"+mtbsuf, do_runop, do_syst, FIT);
	    AddVLQSelection("c-0lep-7jin-"+bjet+"-1Hex-0Vex-1Tex"+mtbsuf, do_runop, do_syst, FIT);
	    AddVLQSelection("c-0lep-7jin-"+bjet+"-1Hex-1Vin-1Tex"+mtbsuf, do_runop, do_syst, FIT);
	    AddVLQSelection("c-0lep-7jin-"+bjet+"-1Hex-0Vin-2Tin"+mtbsuf, do_runop, do_syst, FIT);
	    AddVLQSelection("c-0lep-7jin-"+bjet+"-2Hin-0Vin-0Tex"+mtbsuf, do_runop, do_syst, FIT);
	    AddVLQSelection("c-0lep-7jin-"+bjet+"-2Hin-0Vin-1Tin"+mtbsuf, do_runop, do_syst, FIT);

	    // Merged regions
	    AddVLQSelection("c-0lep-7jin-"+bjet+"-0Hex-2Vin-1Tin"+mtbsuf, do_runop, do_syst, FIT);
	    if(bjet == "3bex"){
	      AddVLQSelection("c-0lep-7jin-3bin-0Hex-1Vin-1Tex"+mtbsuf, do_runop, do_syst, FIT);
	      AddVLQSelection("c-0lep-7jin-3bin-0Hex-1Vin-2Tin"+mtbsuf, do_runop, do_syst, FIT);
	      AddVLQSelection("c-0lep-7jin-3bin-0Hex-2Vin-1Tex"+mtbsuf, do_runop, do_syst, FIT);
	      AddVLQSelection("c-0lep-7jin-3bin-0Hex-2Vin-2Tin"+mtbsuf, do_runop, do_syst, FIT);
	      AddVLQSelection("c-0lep-7jin-3bin-0Hex-2Vin-1Tin"+mtbsuf, do_runop, do_syst, FIT);
	    }
	    AddVLQSelection("c-0lep-7jin-"+bjet+"-1Hex-0Vex-0_1Twin"+mtbsuf, do_runop, do_syst, FIT);

	  }
	    
	  }*/
	//^------------------------------- First Iteration------------------------------------^//
      }
      
      if(m_opt->DoOneLeptonAna()){
	
	for(std::pair<std::string, std::vector<std::string> > lep_ch_pair : ch_lep ){

	  const std::string& lep_prefix = lep_ch_pair.first;
	  
          for(const std::string& lepsuf : lep_ch_pair.second){

	    // Third iteration of regions
	    AddVLQSelection(lep_prefix+"6jin-3bin-0Hex-0Vin-0LTex"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-3bin-0Hex-0Vin-1LTin"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-3bin-1Hex-0Vex-0LTex"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-3bin-1Hex-0Vex-1Lin-0Tex"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-3bin-1Hex-1Vin-1Lin-0Tex"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-3bin-1Hex-1Vin-0LTex"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-3bin-1Hex-0Vin-0Lex-1Tin"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-3bin-1Hex-0Vin-1Lin-1Tin"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-3bin-2Hin-0Vin-1Lin-1Tin"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-3bin-2Hin-0Vin-0Lex-1Tin"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-3bin-2Hin-0Vin-1Lin-0Tex"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-3bin-2Hin-0Vin-0LTex"+lepsuf, do_runop, do_syst, FIT);

	    // 3bex regions
	    AddVLQSelection(lep_prefix+"6jin-3bex-0Hex-0Vin-0LTex"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-3bex-0Hex-0Vin-1LTin"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-3bex-1Hex-0Vex-0LTex"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-3bex-1Hex-0Vex-1Lin-0Tex"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-3bex-1Hex-1Vin-1Lin-0Tex"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-3bex-1Hex-1Vin-0LTex"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-3bex-1Hex-0Vin-0Lex-1Tin"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-3bex-1Hex-0Vin-1Lin-1Tin"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-3bex-2Hin-0Vin-1Lin-1Tin"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-3bex-2Hin-0Vin-0Lex-1Tin"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-3bex-2Hin-0Vin-1Lin-0Tex"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-3bex-2Hin-0Vin-0LTex"+lepsuf, do_runop, do_syst, FIT);

	    // 4bin regions
	    AddVLQSelection(lep_prefix+"6jin-4bin-0Hex-0Vin-0LTex"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-4bin-0Hex-0Vin-1LTin"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-4bin-1Hex-0Vex-0LTex"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-4bin-1Hex-0Vex-1Lin-0Tex"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-4bin-1Hex-1Vin-1Lin-0Tex"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-4bin-1Hex-1Vin-0LTex"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-4bin-1Hex-0Vin-0Lex-1Tin"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-4bin-1Hex-0Vin-1Lin-1Tin"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-4bin-2Hin-0Vin-1Lin-1Tin"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-4bin-2Hin-0Vin-0Lex-1Tin"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-4bin-2Hin-0Vin-1Lin-0Tex"+lepsuf, do_runop, do_syst, FIT);
            AddVLQSelection(lep_prefix+"6jin-4bin-2Hin-0Vin-0LTex"+lepsuf, do_runop, do_syst, FIT);
	    
	    // merged 2Hin regions
	    AddVLQSelection(lep_prefix+"6jin-3bex-2Hin-0Vin-1LTin"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-3bex-2Hin-0Vin-0Lin-0Tin"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-4bin-2Hin-0Vin-1LTin"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-4bin-2Hin-0Vin-0Lin-0Tin"+lepsuf, do_runop, do_syst, FIT);
	    
	    // merged 1Hex1Vin regions
	    AddVLQSelection(lep_prefix+"6jin-3bin-1Hex-1Vin-0Lin-0Tex"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-3bex-1Hex-1Vin-0Lin-0Tex"+lepsuf, do_runop, do_syst, FIT);
	    AddVLQSelection(lep_prefix+"6jin-4bin-1Hex-1Vin-0Lin-0Tex"+lepsuf, do_runop, do_syst, FIT);

	  }

	}

      }

      //One/Two-lepton analysis regions
      if((m_opt->DoOneLeptonAna() || m_opt->DoTwoLeptonAna()) && m_opt->DoOldPairProdRegions()){

	for( std::pair<std::string, std::vector<std::string> > lep_ch_pair : ch_lep ){
	  const std::string& lep_prefix = lep_ch_pair.first;

	  for(const std::string& lepsuf : lep_ch_pair.second){

	    for( const std::string& bjet : bjet_analist ){

	      for( const std::string& boost : boostset_1L_default){
		if(m_opt->DoFitRegions()){
		  //Fit regions
		  AddVLQSelection(lep_prefix+boost+"-6jin-"+bjet+lepsuf, true, true, FIT);
		}
		if(m_opt->DoValidnRegions()){
		  if(bjet != "4bin"){
		    //Validation regions
		    AddVLQSelection(lep_prefix+boost+"-5jex-"+bjet+lepsuf, true, true, VALIDATION);
		  }
		}
	      }//Default boost merging

	      if(m_opt->DoValidnRegions()){
		if(bjet == "4bin"){
		  //Validation regions
		  for( const std::string& boost : boostset_1L_valid_4b){
		    AddVLQSelection(lep_prefix+boost+"-5jex-"+bjet+lepsuf, true, true, VALIDATION);
		  }
		}//Special merging for 4b validation regions
	      }

	    }//ana bjet channels
	  }//e/mu splitting

	}//lepton channels

      }//1/2-lepton

    }//Pair VLQ regions

  }//Fit/validation regions

  m_blinding_config.close();

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
	  || sprop_Mbb || sprop_Mtb || sprop_MetSig || sprop_MLL || sprop_MVA)  ){

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
	       || sprop_Mbb || sprop_Mtb || sprop_MetSig || sprop_MLL || sprop_MVA) ){

      if(sprop_bjet->primanc_name.empty()){
        AddAncestor(*sel, "c-"+sprop_lep->name+"-"+sprop_jet->name, true);
	SelectorBase::AddAncestor(*sel, sprop_bjet->index);
      }
      else{
        AddPrimary(*sel, "c-"+sprop_lep->name+"-"+sprop_jet->name+"-"+sprop_bjet->primanc_name);
        SelectorBase::AddAncestors(*sel, {sprop_lep->index, sprop_jet->index, sprop_bjet->index});
      }
    } //Lep + jet + bjet
    else if( !(sprop_Mbb || sprop_Mtb || sprop_MetSig || sprop_MLL || sprop_MVA) ){
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
      if(!(sprop_MetSig || sprop_MLL || sprop_MVA) ){
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
	if(sprop_MLL){
	  SelectorBase::AddAncestor(*sel, sprop_MLL->index);
	}
	if(sprop_MVA){
	  SelectorBase::AddAncestor(*sel, sprop_MVA->index);
	}
      }//Lep-jet-bjet-boost-mt[b]b-metsig-mll

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

  //==== MLL ======
  else if(index == c_HighMLL){
    pass = (m_outData->o_mll > 15.);
  }
  else if(index == c_ZwinMLL){
    pass = (fabs(m_outData->o_mll-91.) < 10.);
  }

  //==== MVAScore ====
  else if(index == c_LowMVAScore){
    pass = (m_outData->o_MVAScore < 0.50);
    //pass = (m_outData->o_MVAScore < 0.25);
    //pass = (m_outData->o_MVAScore < 0.35);
    //pass = (m_outData->o_MVAScore < 0.15);
  }
  else if(index == c_MidMVAScore){
    pass = (m_outData->o_MVAScore >= 0.50) && (m_outData->o_MVAScore < 0.90);
    //pass = (m_outData->o_MVAScore >= 0.25) && (m_outData->o_MVAScore < 0.90);
    //pass = (m_outData->o_MVAScore >= 0.35) && (m_outData->o_MVAScore < 0.90);
    //pass = (m_outData->o_MVAScore >= 0.15) && (m_outData->o_MVAScore < 0.90);
  }
  else if(index == c_HighMVAScore){
    pass = (m_outData->o_MVAScore >= 0.90);
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
  else if(index == c_2Min3Jin){ pass = (m_outData->o_taggedjets_n.at("RCTTMass") >= 2) && (m_outData->o_rcjets_n >= 3); }
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
