#ifndef VLQ_SELECTOR_H
#define VLQ_SELECTOR_H

#include "IFAETopFramework/SelectorBase.h"
#include <fstream>

class VLQ_Options;
class VLQ_AnalysisTools;
class VLQ_NtupleData;
class VLQ_OutputData;

class OutputHistManager;
class OutputTreeManager;

class VLQ_Selector : public SelectorBase{

public:

  struct SelProp{
    int index;
    std::string name;
    std::string primanc_name;
    int type;
  };

  //=============================================================================================

  enum VLQSelFlags{DOSYST=DOTREE+1,PRESEL,SINGLEVLQ,PAIRVLQ,VALIDATION,FIT,ONELEP,ZEROLEP};

    //=========== Top selections =========================
  enum VLQTopSels{ c_all=1,
		   c_1l_chan, c_1el_chan, c_1mu_chan, 
		   c_2l_chan, c_ee_chan, c_mumu_chan, c_emu_chan, c_sf_chan,
		   c_0l_chan,
		   /*jet multiplicities*/
		   c_2jin, c_3jin, c_3jex, c_3_5jwin,
		   c_4jex, c_5jex, c_6jex, c_7jex, c_8jex, c_9jex, 
		   c_10jex, c_11jex, c_6_8jwin,
		   c_4jin, c_5jin, c_6jin, c_7jin, c_8jin, c_9jin,
		   c_10jin, c_11jin, c_12jin,
		   /*bjet multiplicities*/
		   c_0bex, c_1bex, c_2bex, c_3bex,
		   c_0bin, c_1bin, c_2bin, c_3bin, c_4bin, c_1_2bwin,
		   /*RC jet multiplicities*/
		   c_0Jex, c_1Jex, c_1Jin, c_2Jex, c_2Jin, c_3Jin,
		   c_0_2Jwin0Hex1Tex,
		   /*Mass-tagged jet multiplicities*/
		   c_0Mex, c_1Mex, c_1Min, c_2Min, 
		   c_1Mex3Jin, c_1Mex3Jin0Hex, c_1Mex3Jin1Hin,
		   c_2Min3Jin, c_2Min3Jin0Hex, c_2Min3Jin1Hin,
		   c_1Mex0Hex1Tex,
		   /*Hadtop-tagged jet multiplicities*/
		   c_0Tex, c_0Tin, c_1Tex, c_0_1Twin, c_1Tin, c_2Tin,
		   /*Leptop multiplicities*/
		   c_0Lex, c_0Lin, c_1Lex, c_0_1Lwin, c_1Lin, c_2Lin,
		   /*Higgs-tagged jet multiplicities*/
		   c_0Hex, c_0Hin, c_1Hex, c_1Hin, c_2Hin, c_0_1Hwin,
		   /*V-tagged jet multiplicities*/
		   c_0Vex, c_0Vin, c_1Vex, c_0_1Vwin, c_1Vin, c_2Vin, c_3Vin,
		   /*Forward jet multiplicities*/
		   c_0fjex, c_0fjin, c_1fjex, c_1fjin, c_2fjin,
		   /*T+H multiplicities*/
		   c_0THex, c_1THex, c_1THin, c_2THex, c_2THin, c_3THin,
		   /*L+T multiplicities*/
		   c_0LTex, c_1LTex, c_1LTin, c_2LTin,
		   /*V+T multiplicities*/
		   c_0VTex, c_1VTex, c_0_1VTwin, c_1VTin, c_2VTin, c_2VTor, 
		   /*V+L+T multiplicities*/
		   c_0VLTex, c_1VLTex, c_1VLTin, c_2VLTin,
		   /*V+L+T+H multiplicities*/
		   c_0VLTHex, c_1VLTHin,
		   /*Kinematic cuts*/
		   c_LowMtbmin, c_HighMtbmin, c_LowMbb, c_HighMbb, c_HighMetSig, c_LowMetSig, c_HighMLL, c_ZwinMLL, 
		   /*MVAScore cuts*/
		   c_LowMVAScore, c_MidMVAScore, c_HighMVAScore, TOPSEL_MAX
  };


    VLQ_Selector( VLQ_Options* opt, const VLQ_NtupleData *ntupData, VLQ_OutputData *outData, VLQ_AnalysisTools *anaTools
		   , const bool useDecisions=true, const bool add_primaries=false );
    VLQ_Selector( const VLQ_Selector & );
    ~VLQ_Selector();

    inline void SetOutputHistManager(OutputHistManager* outMngrHist){ m_outMngrHist = outMngrHist; }

    bool Init();

    Selection* AddVLQSelection( const std::string &name, const bool do_runop=true, const bool do_syst=false, const int reg_type=0);

    int GetSelectionIndex( const std::string& sel_name);
    std::string GetSelectionName( const int sel_index);

    virtual bool PassSelection( const int index );
    virtual bool RunOperations(const Selection& sel) const;

 protected:
    virtual Selection* MakeSelection( const int index, const std::string& name="" );

    bool AddPrimary( Selection& sel, const std::string& anc_name);
    bool AddAncestor( Selection& sel, const std::string& anc_name, bool is_primary=false);
    bool AddAncestors( Selection& sel, const std::vector<std::string> &anc_list, const std::string& primary);
    bool AddBlindingCut(Selection& sel);

    int AddSelectionIndex( const std::string& sel_name, int index=-1, bool newentry=false);
    SelProp MakeSelProp(const std::string& _name, int _type, const std::string& _primanc_name="");

 private:
    VLQ_Options *m_opt;
    const VLQ_NtupleData *m_ntupData;
    VLQ_OutputData *m_outData;
    VLQ_AnalysisTools *m_anaTools;

    OutputHistManager* m_outMngrHist;
    //OutputTreeManager* m_outMngrTree;

    std::map<std::string, int>* m_sel_indices;
    std::map<int, std::string>* m_sel_names;
    std::vector<SelProp>* m_sel_lep_prop;
    std::vector<SelProp>* m_sel_lepflav_prop;
    std::vector<SelProp>* m_sel_jet_prop;
    std::vector<SelProp>* m_sel_fwdjet_prop;
    std::vector<SelProp>* m_sel_bjet_prop;
    std::vector<SelProp>* m_sel_J_prop;
    std::vector<SelProp>* m_sel_M_prop;
    std::vector<SelProp>* m_sel_T_prop;
    std::vector<SelProp>* m_sel_L_prop;
    std::vector<SelProp>* m_sel_H_prop;
    std::vector<SelProp>* m_sel_V_prop;
    std::vector<SelProp>* m_sel_TH_prop;
    std::vector<SelProp>* m_sel_LT_prop;
    std::vector<SelProp>* m_sel_VT_prop;
    std::vector<SelProp>* m_sel_VLT_prop;
    std::vector<SelProp>* m_sel_VLTH_prop;

    std::vector<SelProp>* m_sel_Mbb_prop;
    std::vector<SelProp>* m_sel_Mtb_prop;
    std::vector<SelProp>* m_sel_MetSig_prop;
    std::vector<SelProp>* m_sel_MLL_prop;
    std::vector<SelProp>* m_sel_MVAScore_prop;
    std::ifstream m_blinding_config;

    //int m_nsel;


};

#endif
