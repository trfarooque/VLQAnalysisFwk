
#include "VLQAnalysis/VLQ_NtupleData.h"

#include <iostream>

//____________________________________________________________
//
VLQ_NtupleData::VLQ_NtupleData():
NtupleData(),
// electrons
d_el_pt(0),
d_el_eta(0),
d_el_phi(0),
d_el_e(0),
d_el_passOR(0),
d_el_isSignal(0),
d_el_ptvarcone20(0),
d_el_topoetcone20(0),
d_el_d0sig(0),
d_el_z0(0),
// muons
d_mu_pt(0),
d_mu_eta(0),
d_mu_phi(0),
d_mu_e(0),
d_mu_ptvarcone30(0),
d_mu_topoetcone20(0),
d_mu_d0sig(0),
d_mu_z0(0),
d_mu_isSignal(0),
d_mu_isBad(0),
d_mu_isCosmic(0),
d_mu_passOR(0),
// small jets
d_jet_pt(0),
d_jet_eta(0),
d_jet_phi(0),
d_jet_E(0),
d_jet_btag_weight(0),
d_jet_isb(0),
d_jet_jvt(0),
d_jet_truthLabel(0),
d_jet_isSignal(0),
d_jet_passOR(0),
// forward jets
d_fwdjet_pt(0),
d_fwdjet_eta(0),
d_fwdjet_phi(0),
d_fwdjet_E(0),
d_fwdjet_btag_weight(0),
d_fwdjet_jvt(0),
d_fwdjet_fJvt(0),
d_fwdjet_truthLabel(0),
d_fwdjet_isSignal(0),
d_fwdjet_passOR(0),
// small jets
d_trkjet_pt(0),
d_trkjet_eta(0),
d_trkjet_phi(0),
d_trkjet_btag_weight(0),
d_trkjet_isb(0),
d_trkjet_truthLabel(0),
// standard large-R jets
d_ljet_pt(0),
d_ljet_eta(0),
d_ljet_phi(0),
d_ljet_e(0),
d_ljet_m(0),
d_ljet_sd12(0),
d_ljet_sd23(0),
d_ljet_tau21_wta(0),
d_ljet_tau32_wta(0),
d_ljet_D2(0),
d_ljet_C2(0),
d_ljet_topTag(0),
d_ljet_bosonTag(0),
d_ljet_topTag_loose(0),
d_ljet_bosonTag_loose(0),
d_ljet_DNNC50_pass(0),
d_ljet_DNNC80_pass(0),
d_ljet_DNNI50_pass(0),
d_ljet_DNNI80_pass(0),
d_ljet_SW50_pass(0),
d_ljet_SW80_pass(0),
d_ljet_SZ50_pass(0),
d_ljet_SZ80_pass(0),
d_ljet_XbbHiggs_score(0),
d_ljet_XbbQCD_score(0),
d_ljet_XbbTop_score(0),
d_ljet_DNNC50_SF(0),
d_ljet_DNNC80_SF(0),
d_ljet_DNNI50_SF(0),
d_ljet_DNNI80_SF(0),
d_ljet_SW50_SF(0),
d_ljet_SW80_SF(0),
d_ljet_SZ50_SF(0),
d_ljet_SZ80_SF(0),
// reclustered jets
d_rcjets_pt(0),
d_rcjets_m(0),
d_rcjets_eta(0),
d_rcjets_phi(0),
d_rcjets_nconsts(0),
// truth information
d_mc_pt(0),
d_mc_eta(0),
d_mc_phi(0),
d_mc_m(0),
d_mc_pdgId(0),
d_mc_children_index(0),
//trf weight vectors
d_trf_weight_77_2ex_eigenvars_B_up(0),
d_trf_weight_77_2ex_eigenvars_B_down(0),
d_trf_weight_77_2ex_eigenvars_C_up(0),
d_trf_weight_77_2ex_eigenvars_C_down(0),
d_trf_weight_77_2ex_eigenvars_Light_up(0),
d_trf_weight_77_2ex_eigenvars_Light_down(0),
d_trf_weight_77_3ex_eigenvars_B_up(0),
d_trf_weight_77_3ex_eigenvars_B_down(0),
d_trf_weight_77_3ex_eigenvars_C_up(0),
d_trf_weight_77_3ex_eigenvars_C_down(0),
d_trf_weight_77_3ex_eigenvars_Light_up(0),
d_trf_weight_77_3ex_eigenvars_Light_down(0),
d_trf_weight_77_4in_eigenvars_B_up(0),
d_trf_weight_77_4in_eigenvars_B_down(0),
d_trf_weight_77_4in_eigenvars_C_up(0),
d_trf_weight_77_4in_eigenvars_C_down(0),
d_trf_weight_77_4in_eigenvars_Light_up(0),
d_trf_weight_77_4in_eigenvars_Light_down(0),
d_trf_tagged_77_2ex(0),
d_trf_tagged_77_3ex(0),
d_trf_tagged_77_4in(0)
{
  d_el_trigMatch.clear();
  d_mu_trigMatch.clear();
  d_triggers.clear();
  /*
  d_vrcjets_pt.clear();
  d_vrcjets_m.clear();
  d_vrcjets_eta.clear();
  d_vrcjets_phi.clear();
  d_vrcjets_nconsts.clear();
  d_vrcjets_Reff.clear();
  */
  d_trf_weight_77_2ex_eigenvars_B_up = new std::vector<double>(6);
  d_trf_weight_77_2ex_eigenvars_B_down = new std::vector<double>(6);
  d_trf_weight_77_2ex_eigenvars_C_up = new std::vector<double>(4);
  d_trf_weight_77_2ex_eigenvars_C_down = new std::vector<double>(4);
  d_trf_weight_77_2ex_eigenvars_Light_up = new std::vector<double>(17);
  d_trf_weight_77_2ex_eigenvars_Light_down = new std::vector<double>(17);
  d_trf_weight_77_3ex_eigenvars_B_up = new std::vector<double>(6);
  d_trf_weight_77_3ex_eigenvars_B_down = new std::vector<double>(6);
  d_trf_weight_77_3ex_eigenvars_C_up = new std::vector<double>(4);
  d_trf_weight_77_3ex_eigenvars_C_down = new std::vector<double>(4);
  d_trf_weight_77_3ex_eigenvars_Light_up = new std::vector<double>(17);
  d_trf_weight_77_3ex_eigenvars_Light_down = new std::vector<double>(17);
  d_trf_weight_77_4in_eigenvars_B_up = new std::vector<double>(6);
  d_trf_weight_77_4in_eigenvars_B_down = new std::vector<double>(6);
  d_trf_weight_77_4in_eigenvars_C_up = new std::vector<double>(4);
  d_trf_weight_77_4in_eigenvars_C_down = new std::vector<double>(4);
  d_trf_weight_77_4in_eigenvars_Light_up = new std::vector<double>(17);
  d_trf_weight_77_4in_eigenvars_Light_down = new std::vector<double>(17);
}

//____________________________________________________________
//
VLQ_NtupleData::~VLQ_NtupleData()
{}
