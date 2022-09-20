#ifndef VLQ_OUTPUTDATA_H
#define VLQ_OUTPUTDATA_H

#include "IFAETopFramework/OutputData.h"
#include <vector>
#include <map>

class TriggerInfo;
class AnalysisObject;
class VLQ_Options;

class VLQ_OutputData : public OutputData {

public:

    //
    // Standard C++ functions
    //
    VLQ_OutputData( VLQ_Options* opt );
    VLQ_OutputData( const VLQ_OutputData & );
    ~VLQ_OutputData();

    //
    // Class specific functions
    //
    virtual void ClearOutputData();

private:
    VLQ_Options* m_opt;

public:

    //
    // Declare here the output variables of your code. The naming convention
    // is o_*
    //

    //List of triggers to use. Remains constant for all events
    std::vector<TriggerInfo*> o_trigger_list;

    //
    // Metadata
    //
    int o_sampleID;
    int o_sampleName;
    int o_mc_campaign;



    //
    // Event variables
    //
    int o_channel_type;
    int o_period;
    int o_run_number;
    double o_pileup_mu;
    int o_npv;
    double o_meff;
    double o_meffred;
    double o_met;
    double o_mtwl;
    double o_ptwl;
    double o_mll;
    double o_hthad;
    double o_hthadRC;
    double o_hthadRCtag;
    double o_hthadRCM;
    double o_mJsum;
    double o_metsig_ev;
    double o_metsig_obj;
    float o_MVAScore;
    unsigned long long int o_event_number;
    std::vector < int > *o_region;

    //
    // Jet-related quantities
    //
    int o_jets_n;
    int o_bjets_n;
    int o_ljets_n;

    int o_jets_truth_b_n;
    int o_jets_truth_c_n;
    int o_jets_truth_tau_n;
    int o_jets_truth_lqg_n;

    int o_bjets_truth_b_n;
    int o_bjets_truth_c_n;
    int o_bjets_truth_tau_n;
    int o_bjets_truth_lqg_n;

    int o_trkjets_n;
    int o_trkbjets_n;
    int o_trkljets_n;
    int o_trkjets_nconcentric;

    int o_trkjets_truth_b_n;
    int o_trkjets_truth_c_n;
    int o_trkjets_truth_tau_n;
    int o_trkjets_truth_lqg_n;

    int o_trkbjets_truth_b_n;
    int o_trkbjets_truth_c_n;
    int o_trkbjets_truth_tau_n;
    int o_trkbjets_truth_lqg_n;

    int o_fwdjets_n;
    int o_fwdjets_eta25_30_n;
    int o_fwdjets_eta30_35_n;
    int o_fwdjets_eta35_40_n;
    int o_fwdjets_eta40_45_n;

    int o_fwdjets30_n;
    int o_fwdjets40_n;

    int o_fjets_n;
    int o_rcjets_n;
    std::map< std::string, int > o_taggedjets_n;

    std::vector< AnalysisObject* >* o_jets;
    std::vector< AnalysisObject* >* o_bjets;
    std::vector< AnalysisObject* >* o_ljets;

    std::vector< AnalysisObject* >* o_jets_truth_b;
    std::vector< AnalysisObject* >* o_jets_truth_c;
    std::vector< AnalysisObject* >* o_jets_truth_tau;
    std::vector< AnalysisObject* >* o_jets_truth_lqg;

    std::vector< AnalysisObject* >* o_bjets_truth_b;
    std::vector< AnalysisObject* >* o_bjets_truth_c;
    std::vector< AnalysisObject* >* o_bjets_truth_tau;
    std::vector< AnalysisObject* >* o_bjets_truth_lqg;

    std::vector< AnalysisObject* >* o_trkjets;
    std::vector< AnalysisObject* >* o_trkbjets;
    std::vector< AnalysisObject* >* o_trkljets;

    std::vector< AnalysisObject* >* o_trkjets_truth_b;
    std::vector< AnalysisObject* >* o_trkjets_truth_c;
    std::vector< AnalysisObject* >* o_trkjets_truth_tau;
    std::vector< AnalysisObject* >* o_trkjets_truth_lqg;

    std::vector< AnalysisObject* >* o_trkbjets_truth_b;
    std::vector< AnalysisObject* >* o_trkbjets_truth_c;
    std::vector< AnalysisObject* >* o_trkbjets_truth_tau;
    std::vector< AnalysisObject* >* o_trkbjets_truth_lqg;

    std::vector< AnalysisObject* >* o_fwdjets;
    std::vector< AnalysisObject* >* o_fwdjets_eta25_30;
    std::vector< AnalysisObject* >* o_fwdjets_eta30_35;
    std::vector< AnalysisObject* >* o_fwdjets_eta35_40;
    std::vector< AnalysisObject* >* o_fwdjets_eta40_45;


    std::vector< AnalysisObject* >* o_rcjets;
    std::vector< AnalysisObject* >* o_fjets;
    std::map< std::string, std::vector< AnalysisObject* >*  > o_taggedjets;

    std::vector< AnalysisObject* >* o_bjets_lowb_3b;
    std::vector< AnalysisObject* >* o_bjets_lowb_4b;

    std::map< std::string, int > o_rcjets_truthmatched_n;
    std::map< std::string, std::vector< AnalysisObject* >*  > o_rcjets_truthmatched;

    //
    // Lepton variables
    //
    int o_el_n;
    int o_mu_n;
    int o_lep_n;
    int o_lepForVeto_n;
    int o_el_loose_n;
    int o_mu_loose_n;
    int o_lep_loose_n;
    std::vector< AnalysisObject* >* o_el;
    std::vector< AnalysisObject* >* o_mu;
    std::vector< AnalysisObject* >* o_lep;
    AnalysisObject *o_selLep;

    //
    // Missing ET
    //
    AnalysisObject *o_AO_met;
    AnalysisObject *o_residualMET;
    //    double o_residualMET;

    //
    // Leptonic top and W
    //
    int o_lepW_n;
    int o_leptop_n;
    AnalysisObject *o_leptop;
    AnalysisObject *o_leptop_b;
    AnalysisObject *o_lepW;
    AnalysisObject *o_nu;

    //
    // Different categories of leptop, for optimisation studies
    //

    std::map<std::string, AnalysisObject*> o_catLeptop;
    std::map<std::string, AnalysisObject*> o_catLeptop_b;
    std::map<std::string,  int > o_catLeptop_n;


    //
    // Semi-boosted hadronic top quark
    //
    std::vector< AnalysisObject* >* o_bW_hadtop;

    //
    // Reconstructed VLQs
    //
    std::map< std::string, int > o_recoVLQ_n;
    std::map< std::string, std::vector< AnalysisObject* >*  > o_recoVLQ;


    //
    // Variables for top tagging truth studies
    //
    float o_fjets_topmatched;
    float o_fjets_topmatchboost_TTL;
    float o_fjets_topmatchboost_TTT;
    float o_nhadtops;
    float o_nboosthadtops;
    float o_matchingeff;
    float o_tageff_TTL;
    float o_tageff_TTT;

    //
    // Other kinematic variables
    //
    double o_dRmin_lepjet;
    double o_dRmin_ejets;
    double o_dRmin_mujets;
    double o_dRmin_lepbjet;
    double o_dRmin_jetjet;
    double o_dRmin_RCjets;
    double o_dRmin_RCTTMassRCTTMass;
    double o_dEtamin_lepjet;
    double o_dEtamin_lepbjet;
    double o_dEtamin_jetjet;
    double o_dEtamin_RCjets;
    double o_dEtamin_RCTTMassRCTTMass;
    double o_dPhimin_lepjet;
    double o_dPhimin_lepbjet;
    double o_dPhimin_jetjet;
    double o_dPhimin_RCjets;
    double o_dPhimin_RCjetsMET;
    double o_dPhimin_RCTTMassRCTTMass;
    double o_dPhimin_RCTTMassMET;
    double o_dRmin_RCMHiggsRCMHiggs;
    double o_dRmin_RCMHiggsRCMV;
    double o_dRmin_RCMHiggsRCMTop;
    double o_dRmin_RCMVRCMV;
    double o_dRmin_RCMVRCMTop;
    double o_dRmin_RCMTopRCMTop;
    double o_dEtamin_RCMHiggsRCMHiggs;
    double o_dEtamin_RCMHiggsRCMV;
    double o_dEtamin_RCMHiggsRCMTop;
    double o_dEtamin_RCMVRCMV;
    double o_dEtamin_RCMVRCMTop;
    double o_dEtamin_RCMTopRCMTop;
    double o_dPhimin_RCMHiggsRCMHiggs;
    double o_dPhimin_RCMHiggsMET;
    double o_dPhimin_RCMHiggsRCMV;
    double o_dPhimin_RCMHiggsRCMTop;
    double o_dPhimin_RCMVRCMV;
    double o_dPhimin_RCMVRCMTop;
    double o_dPhimin_RCMTopRCMTop;
    double o_dPhimin_RCMTopMET;
    double o_leadingdR_lepjet;
    double o_leadingdR_lepbjet;
    double o_leadingdR_jetjet;
    double o_leadingdR_RCjets;
    double o_leadingdEta_RCjets;
    double o_leadingdEta_lepjet;
    double o_leadingdEta_lepbjet;
    double o_leadingdEta_jetjet;
    double o_leadingdPhi_RCjets;
    double o_leadingdPhi_RCjetsMET;
    double o_leadingdPhi_lepjet;
    double o_leadingdPhi_lepbjet;
    double o_leadingdPhi_jetjet;
    double o_leadingdR_RCMHiggsRCMHiggs;
    double o_leadingdR_RCMHiggsRCMV;
    double o_leadingdR_RCMHiggsRCMTop;
    double o_leadingdR_RCMVRCMV;
    double o_leadingdR_RCMVRCMTop;
    double o_leadingdR_RCMTopRCMTop;
   
    double o_leadingdR_RCTTMassRCTTMass;
    double o_leadingdEta_RCTTMassRCTTMass;
    double o_leadingdPhi_RCTTMassRCTTMass;
    double o_leadingdPhi_RCTTMassMET;
    double o_leadingdEta_RCMHiggsRCMHiggs;
    double o_leadingdEta_RCMHiggsRCMV;
    double o_leadingdEta_RCMHiggsRCMTop;
    double o_leadingdEta_RCMVRCMV;
    double o_leadingdEta_RCMVRCMTop;
    double o_leadingdEta_RCMTopRCMTop;
    double o_leadingdPhi_RCMHiggsRCMHiggs;
    double o_leadingdPhi_RCMHiggsMET;
    double o_leadingdPhi_RCMHiggsRCMV;
    double o_leadingdPhi_RCMHiggsRCMTop;
    double o_leadingdPhi_RCMVRCMV;
    double o_leadingdPhi_RCMVRCMTop;
    double o_leadingdPhi_RCMTopRCMTop;
    double o_leadingdPhi_RCMTopMET;
    double o_minRecoVLQMAsymm_HtHt;
    double o_minRecoVLQMAsymm_HtZt;
    double o_minRecoVLQMAsymm_HtWb;
    double o_dRmin_bjetbjet;
    double o_dRmin_bjetbjet_lowb_3b;
    double o_dRmin_bjetbjet_lowb_4b;
    double o_dR_TTL_bjets;
    double o_dR_TTT_bjets;
    double o_dR_TTLooser_bjets;
    double o_mbb_mindR;
    double o_mbb_mindR_lowb_3b;
    double o_mbb_mindR_lowb_4b;

    double o_dPhi_lepmet;
    double o_dPhi_jetmet;
    double o_dPhi_jetmet5;
    double o_dPhi_jetmet6;
    double o_dPhi_jetmet7;
    double o_dPhi_lepjet;
    double o_dPhi_lepbjet;
    double o_dRmin_ebjets;
    double o_dRmin_mubjets;
    double o_mTbmin;
    double o_mTbmin_lowb_3b;
    double o_mTbmin_lowb_4b;

    //HBSM related variables
    int o_jets40_n;
    double o_centrality;
    double o_mbb_leading_bjets;
    double o_mbb_softest_bjets;
    double o_J_lepton_invariant_mass;
    double o_J_leadingb_invariant_mass;
    double o_J_J_invariant_mass;

    double o_invariant_mass_RCTTMassRCTTMass;
    double o_invariant_mass_RCjets;
    double o_dRmaxM_RCTTMassRCTTMass;
    double o_dRmaxM_RCjets;
    double o_dPhimaxM_RCTTMassRCTTMass;
    double o_dPhimaxM_RCjets;
    double o_dEtaminM_RCTTMassRCTTMass;
    double o_dEtaminM_RCjets;

    double o_dRaverage_bjetbjet;
    double o_dRaverage_jetjet;
    double o_dRaverage_lepjet;
    double o_dRaverage_lepbjet;
    double o_dRaverage_RCjets;
    double o_dRaverage_RCTTMassRCTTMass;

    double o_dEtaaverage_RCjets;
    double o_dEtaaverage_jetjet;
    double o_dEtaaverage_lepjet;
    double o_dEtaaverage_lepbjet;
    double o_dEtaaverage_RCTTMassRCTTMass;

    double o_dPhiaverage_RCjets;
    double o_dPhiaverage_RCjetsMET;
    double o_dPhiaverage_jetjet;
    double o_dPhiaverage_lepjet;
    double o_dPhiaverage_lepbjet;
    double o_dPhiaverage_RCTTMassRCTTMass;
    double o_dPhiaverage_RCTTMassMET;
    double o_dPhiaverage_RCMTopMET;
    double o_dPhiaverage_RCMHiggsMET;
    //
    //low mass Hplus variables
    double o_mbb_maxdR;
    double o_dPhibb_leading_bjets;
    double o_dPhibb_mindR;
    double o_dPhibb_maxdR;
    double o_dEtabb_leading_bjets;
    double o_dEtabb_mindR;
    double o_dEtabb_maxdR;
    double o_mjj_leading_jets;
    double o_mjj_mindR;
    double o_mjj_maxdR;
    double o_dPhijj_leading_jets;
    double o_dPhijj_mindR;
    double o_dPhijj_maxdR;
    double o_dEtajj_leading_jets;
    double o_dEtajj_mindR;
    double o_dEtajj_maxdR;
    //
    // Event variables for selection
    //
    int o_rejectEvent;
    int o_VLQtype;

    //
    // Truth variables
    //

    //ttbar sample
    double o_truth_dR_Wb;
    double o_truth_top_pt;
    double o_truth_ht_filter;
    double o_truth_met_filter;

    //Invariant VLQ masses reconstruction 
 
    std::vector < double > o_m_vlq_rcjets_pt;
    double o_m_vlq_rcjets_pt_n;
    double o_averagem_vlq_rcjets_pt;
    double o_masymm_vlq_rcjets_pt;
    double o_fmasymm_vlq_rcjets_pt; 

    std::vector < double > o_m_vlq_rcjets_detamin;
    double o_m_vlq_rcjets_detamin_n;
    double o_averagem_vlq_rcjets_detamin;
    double o_masymm_vlq_rcjets_detamin;
    double o_fmasymm_vlq_rcjets_detamin; 

    std::vector < double > o_m_vlq_rcjets_drmax;
    double o_m_vlq_rcjets_drmax_n;
    double o_averagem_vlq_rcjets_drmax;
    double o_masymm_vlq_rcjets_drmax;
    double o_fmasymm_vlq_rcjets_drmax; 

    std::vector < double > o_m_vlq_rcjets_dphimax;
    double o_m_vlq_rcjets_dphimax_n;
    double o_averagem_vlq_rcjets_dphimax;
    double o_masymm_vlq_rcjets_dphimax;
    double o_fmasymm_vlq_rcjets_dphimax; 

    std::vector < double > o_m_vlq_rcttmass_pt;
    double o_m_vlq_rcttmass_pt_n;
    double o_averagem_vlq_rcttmass_pt;
    double o_masymm_vlq_rcttmass_pt;
    double o_fmasymm_vlq_rcttmass_pt; 

    std::vector < double > o_m_vlq_rcttmass_detamin;
    double o_m_vlq_rcttmass_detamin_n;
    double o_averagem_vlq_rcttmass_detamin;
    double o_masymm_vlq_rcttmass_detamin;
    double o_fmasymm_vlq_rcttmass_detamin; 

    std::vector < double > o_m_vlq_rcttmass_drmax;
    double o_m_vlq_rcttmass_drmax_n;
    double o_averagem_vlq_rcttmass_drmax;
    double o_masymm_vlq_rcttmass_drmax;
    double o_fmasymm_vlq_rcttmass_drmax; 

    std::vector < double > o_m_vlq_rcttmass_dphimax;
    double o_m_vlq_rcttmass_dphimax_n;
    double o_averagem_vlq_rcttmass_dphimax;
    double o_masymm_vlq_rcttmass_dphimax;
    double o_fmasymm_vlq_rcttmass_dphimax; 

    //VLQ sample
    int o_truth_dRmin_isHiggs_b;

    std::vector< AnalysisObject* > *o_truth_all_particles;
    std::vector< AnalysisObject* > *o_truth_all_partons;
    std::vector< AnalysisObject* > *o_truth_all_MEParticles;

    std::vector< AnalysisObject* > *o_truth_all_bpartons;
    std::vector< AnalysisObject* > *o_truth_all_b;
    std::vector< AnalysisObject* > *o_truth_all_q;
    std::vector< AnalysisObject* > *o_truth_all_lep;

    AnalysisObject* o_truth_partonMET;

    std::map< std::string, int > o_truth_partons_n;
    std::map< std::string, std::vector< AnalysisObject* >* > o_truth_partons;

    double o_truth_dRmin_bb;
    double o_truth_minv_bb;
    double o_truth_dPhiMin_lepmet;
    double o_truth_dPhiMin_jetmet;
    double o_truth_mtw;
    double o_truth_ptw;
    double o_truth_mtbmin;

    //
    // TRF-related variables
    //
    std::vector < double > *o_TRFweight_in;
    std::vector < double > *o_TRFweight_ex;
    std::vector < std::vector< bool > > *o_TRFPerm_in;
    std::vector < std::vector< bool > > *o_TRFPerm_ex;

    std::vector < std::vector < double > > *o_LTag_BreakUp_TRF_in;
    std::vector < std::vector < double > > *o_LTag_BreakDown_TRF_in;
    std::vector < std::vector < double > > *o_LTag_BreakUp_TRF_ex;
    std::vector < std::vector < double > > *o_LTag_BreakDown_TRF_ex;

    std::vector < std::vector < double > > *o_CTag_BreakUp_TRF_in;
    std::vector < std::vector < double > > *o_CTag_BreakDown_TRF_in;
    std::vector < std::vector < double > > *o_CTag_BreakUp_TRF_ex;
    std::vector < std::vector < double > > *o_CTag_BreakDown_TRF_ex;

    std::vector < std::vector < double > > *o_BTag_BreakUp_TRF_in;
    std::vector < std::vector < double > > *o_BTag_BreakDown_TRF_in;
    std::vector < std::vector < double > > *o_BTag_BreakUp_TRF_ex;
    std::vector < std::vector < double > > *o_BTag_BreakDown_TRF_ex;

    std::vector < std::vector < double > > *o_BTagExtrapUp_TRF_in;
    std::vector < std::vector < double > > *o_BTagExtrapDown_TRF_in;
    std::vector < std::vector < double > > *o_BTagExtrapUp_TRF_ex;
    std::vector < std::vector < double > > *o_BTagExtrapDown_TRF_ex;
    std::vector < std::vector < double > > *o_BTagExtrapFromCharmUp_TRF_in;
    std::vector < std::vector < double > > *o_BTagExtrapFromCharmDown_TRF_in;
    std::vector < std::vector < double > > *o_BTagExtrapFromCharmUp_TRF_ex;
    std::vector < std::vector < double > > *o_BTagExtrapFromCharmDown_TRF_ex;

    //
    // Some booleans
    //
    bool o_is_ttbar;
};

#endif //VLQ_OUTPUTDATA_H
