// Dear emacs, this is -*- c++ -*-
#ifndef VLQ_NTUPLEDATA_H
#define VLQ_NTUPLEDATA_H

#include "IFAETopFramework/NtupleData.h"

#include <vector>

class VLQ_NtupleData: public NtupleData {

public:

    //
    // Standard C++ functions
    //
    VLQ_NtupleData();
    ~VLQ_NtupleData();

public:

    // event variables (run/event numbers)
    unsigned long int d_runNumber;
    unsigned long int d_randomRunNumber;
    unsigned long long int d_eventNumber;
    float d_mu;

    // event truth variables
    float d_HT_truth;
    float d_MET_truth;

    // electrons
    std::vector<float> *d_el_pt;
    std::vector<float> *d_el_eta;
    std::vector<float> *d_el_phi;
    std::vector<float> *d_el_e;
    std::vector<int>   *d_el_passOR;
    std::vector<int>   *d_el_isSignal;
    std::vector<float> *d_el_ptvarcone20;
    std::vector<float> *d_el_topoetcone20;
    std::vector<double> *d_el_d0sig;
    std::vector<double> *d_el_z0;
    std::map<std::string, std::vector<int>* > d_el_trigMatch;

    // muons
    std::vector<float> *d_mu_pt;
    std::vector<float> *d_mu_eta;
    std::vector<float> *d_mu_phi;
    std::vector<float> *d_mu_e;
    std::vector<float> *d_mu_ptvarcone30;
    std::vector<float> *d_mu_topoetcone20;
    std::vector<double> *d_mu_d0sig;
    std::vector<double> *d_mu_z0;
    std::vector<int> *d_mu_isSignal;
    std::vector<int> *d_mu_isBad;
    std::vector<int> *d_mu_isCosmic;
    std::vector<int> *d_mu_passOR;
    std::map< std::string, std::vector<int>* > d_mu_trigMatch;

    // small jets
    std::vector<float> *d_jet_pt;
    std::vector<float> *d_jet_eta;
    std::vector<float> *d_jet_phi;
    std::vector<float> *d_jet_E;
    std::vector<double> *d_jet_btag_weight;
    std::vector<int>   *d_jet_isb;
    std::vector<float> *d_jet_jvt;
    std::vector<int>   *d_jet_truthLabel;
    std::vector<int>   *d_jet_isSignal;
    std::vector<int>   *d_jet_passOR;

    // forward jets
    std::vector<float> *d_fwdjet_pt;
    std::vector<float> *d_fwdjet_eta;
    std::vector<float> *d_fwdjet_phi;
    std::vector<float> *d_fwdjet_E;
    std::vector<double> *d_fwdjet_btag_weight;
    std::vector<float> *d_fwdjet_jvt;
    std::vector<float> *d_fwdjet_fJvt;
    std::vector<int>   *d_fwdjet_truthLabel;
    std::vector<int>   *d_fwdjet_isSignal;
    std::vector<int>   *d_fwdjet_passOR;

    //track jets
    std::vector<float> *d_trkjet_pt;
    std::vector<float> *d_trkjet_eta;
    std::vector<float> *d_trkjet_phi;
    std::vector<double> *d_trkjet_btag_weight;
    std::vector<int>   *d_trkjet_isb;
    std::vector<int>   *d_trkjet_truthLabel;

    // standard large-R jets
    std::vector<float> *d_ljet_pt;
    std::vector<float> *d_ljet_eta;
    std::vector<float> *d_ljet_phi;
    std::vector<float> *d_ljet_e;
    std::vector<float> *d_ljet_m;
    std::vector<float> *d_ljet_sd12;
    std::vector<float> *d_ljet_sd23;
    std::vector<float> *d_ljet_tau21_wta;
    std::vector<float> *d_ljet_tau32_wta;
    std::vector<float> *d_ljet_D2;
    std::vector<float> *d_ljet_C2;
    std::vector<bool>  *d_ljet_topTag;
    std::vector<bool>  *d_ljet_bosonTag;
    std::vector<bool>  *d_ljet_topTag_loose;
    std::vector<bool>  *d_ljet_bosonTag_loose;
    std::vector<int>   *d_ljet_DNNC50_pass;
    std::vector<int>   *d_ljet_DNNC80_pass;
    std::vector<int>   *d_ljet_DNNI50_pass;
    std::vector<int>   *d_ljet_DNNI80_pass;
    std::vector<int>   *d_ljet_SW50_pass;
    std::vector<int>   *d_ljet_SW80_pass;
    std::vector<int>   *d_ljet_SZ50_pass;
    std::vector<int>   *d_ljet_SZ80_pass;
    std::vector<float> *d_ljet_XbbHiggs_score;
    std::vector<float> *d_ljet_XbbQCD_score;
    std::vector<float> *d_ljet_XbbTop_score;

    std::vector<float> *d_ljet_DNNC50_SF;
    std::vector<float> *d_ljet_DNNC80_SF;
    std::vector<float> *d_ljet_DNNI50_SF;
    std::vector<float> *d_ljet_DNNI80_SF;
    std::vector<float> *d_ljet_SW50_SF;
    std::vector<float> *d_ljet_SW80_SF;
    std::vector<float> *d_ljet_SZ50_SF;
    std::vector<float> *d_ljet_SZ80_SF;

    // reclustered jets
    std::vector<float> *d_rcjets_pt;
    std::vector<float> *d_rcjets_m;
    std::vector<float> *d_rcjets_eta;
    std::vector<float> *d_rcjets_phi;
    std::vector<int>   *d_rcjets_nconsts;
  /*
    // variable-R reclustered jets
    std::map < std::string, std::vector<float>* > d_vrcjets_pt;
    std::map < std::string, std::vector<float>* > d_vrcjets_m;
    std::map < std::string, std::vector<float>* > d_vrcjets_eta;
    std::map < std::string, std::vector<float>* > d_vrcjets_phi;
    std::map < std::string, std::vector<int>* >   d_vrcjets_nconsts;
  //std::map < std::string, std::vector<float>* > d_vrcjets_Reff;
  */
    // met
    float d_met_met;
    float d_met_phi;
    float d_met_sig;

    // truth
    std::vector < float > *d_mc_pt;
    std::vector < float > *d_mc_eta;
    std::vector < float > *d_mc_phi;
    std::vector < float > *d_mc_m;
    std::vector < float > *d_mc_pdgId;
    std::vector < std::vector < int > > *d_mc_children_index;

    int d_truth_top_decay;
    int d_truth_antitop_decay;
    float d_top_pt;
    float d_ttbar_pt;

    // trigger decisions
    std::map<std::string, int> d_triggers;

    // ttbar+HF classification
    int d_HF_Classification;
    int d_HF_SimpleClassification;
    float d_q1_pt;
    float d_q1_eta;
    float d_q2_pt;
    float d_q2_eta;
    float d_qq_pt;
    float d_qq_dr;

    // trf pre-computed weights
    double d_trf_weight_77_2ex;
    double d_trf_weight_77_3ex;
    double d_trf_weight_77_4in;
    std::vector<double> *d_trf_weight_77_2ex_eigenvars_B_up;
    std::vector<double> *d_trf_weight_77_2ex_eigenvars_B_down;
    std::vector<double> *d_trf_weight_77_2ex_eigenvars_C_up;
    std::vector<double> *d_trf_weight_77_2ex_eigenvars_C_down;
    std::vector<double> *d_trf_weight_77_2ex_eigenvars_Light_up;
    std::vector<double> *d_trf_weight_77_2ex_eigenvars_Light_down;
    double d_trf_weight_77_2ex_extrapolation_up;
    double d_trf_weight_77_2ex_extrapolation_down;
    double d_trf_weight_77_2ex_extrapolation_from_charm_up;
    double d_trf_weight_77_2ex_extrapolation_from_charm_down;
    std::vector<double> *d_trf_weight_77_3ex_eigenvars_B_up;
    std::vector<double> *d_trf_weight_77_3ex_eigenvars_B_down;
    std::vector<double> *d_trf_weight_77_3ex_eigenvars_C_up;
    std::vector<double> *d_trf_weight_77_3ex_eigenvars_C_down;
    std::vector<double> *d_trf_weight_77_3ex_eigenvars_Light_up;
    std::vector<double> *d_trf_weight_77_3ex_eigenvars_Light_down;
    double d_trf_weight_77_3ex_extrapolation_up;
    double d_trf_weight_77_3ex_extrapolation_down;
    double d_trf_weight_77_3ex_extrapolation_from_charm_up;
    double d_trf_weight_77_3ex_extrapolation_from_charm_down;
    std::vector<double> *d_trf_weight_77_4in_eigenvars_B_up;
    std::vector<double> *d_trf_weight_77_4in_eigenvars_B_down;
    std::vector<double> *d_trf_weight_77_4in_eigenvars_C_up;
    std::vector<double> *d_trf_weight_77_4in_eigenvars_C_down;
    std::vector<double> *d_trf_weight_77_4in_eigenvars_Light_up;
    std::vector<double> *d_trf_weight_77_4in_eigenvars_Light_down;
    double d_trf_weight_77_4in_extrapolation_up;
    double d_trf_weight_77_4in_extrapolation_down;
    double d_trf_weight_77_4in_extrapolation_from_charm_up;
    double d_trf_weight_77_4in_extrapolation_from_charm_down;
    std::vector<bool> *d_trf_tagged_77_2ex;
    std::vector<bool> *d_trf_tagged_77_3ex;
    std::vector<bool> *d_trf_tagged_77_4in;

};

#endif //VLQ_NTUPLEDATA_H
