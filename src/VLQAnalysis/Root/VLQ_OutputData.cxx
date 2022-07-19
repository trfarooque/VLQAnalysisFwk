#include <iostream>
#include "IFAETopFramework/AnalysisObject.h"
#include "IFAETopFramework/AnalysisUtils.h"
#include "IFAETopFramework/TriggerInfo.h"

#include "VLQAnalysis/VLQ_Options.h"
#include "VLQAnalysis/VLQ_OutputData.h"


//______________________________________________________
//
VLQ_OutputData::VLQ_OutputData( VLQ_Options* opt ):
OutputData(),
m_opt(opt),
//Event variables
o_channel_type(0),o_period(0),o_run_number(0),o_pileup_mu(0),o_npv(0), o_meff(0), o_meffred(0), o_met(0), o_mtwl(0), o_ptwl(0), o_mll(0), 
o_hthad(0), o_hthadRC(0), o_hthadRCtag(0), o_hthadRCM(0), o_mJsum(0), o_metsig_ev(0), o_metsig_obj(0),o_residualMET(0),
o_MVAScore(-1),
o_weight_pmg_isr_muRfac10__fsr_muRfac20(0), o_weight_pmg_isr_muRfac10__fsr_muRfac05(0),
//Jet type numbers
o_jets_n(0),o_bjets_n(0),o_ljets_n(0),
o_jets_truth_b_n(0),o_jets_truth_c_n(0),o_jets_truth_tau_n(0),o_jets_truth_lqg_n(0),
o_bjets_truth_b_n(0),o_bjets_truth_c_n(0),o_bjets_truth_tau_n(0),o_bjets_truth_lqg_n(0),
o_trkjets_n(0),o_trkbjets_n(0),o_trkljets_n(0),o_trkjets_nconcentric(0),
o_trkjets_truth_b_n(0),o_trkjets_truth_c_n(0),o_trkjets_truth_tau_n(0),o_trkjets_truth_lqg_n(0),
o_trkbjets_truth_b_n(0),o_trkbjets_truth_c_n(0),o_trkbjets_truth_tau_n(0),o_trkbjets_truth_lqg_n(0),
o_fwdjets_n(0),o_fwdjets_eta25_30_n(0),o_fwdjets_eta30_35_n(0),o_fwdjets_eta35_40_n(0),o_fwdjets_eta40_45_n(0),
o_fwdjets30_n(0),o_fwdjets40_n(0),
o_fjets_n(0),o_rcjets_n(0),
//Jet type containers
o_jets(0),o_bjets(0),o_ljets(0),
o_jets_truth_b(0),o_jets_truth_c(0),o_jets_truth_tau(0),o_jets_truth_lqg(0),
o_bjets_truth_b(0),o_bjets_truth_c(0),o_bjets_truth_tau(0),o_bjets_truth_lqg(0),
o_trkjets(0),o_trkbjets(0),o_trkljets(0),
o_trkjets_truth_b(0),o_trkjets_truth_c(0),o_trkjets_truth_tau(0),o_trkjets_truth_lqg(0),
o_trkbjets_truth_b(0),o_trkbjets_truth_c(0),o_trkbjets_truth_tau(0),o_trkbjets_truth_lqg(0),
o_fwdjets(0),o_fwdjets_eta25_30(0),o_fwdjets_eta30_35(0),o_fwdjets_eta35_40(0),o_fwdjets_eta40_45(0),
o_rcjets(0),o_fjets(0),
o_bjets_lowb_3b(0), o_bjets_lowb_4b(0),
//Lepton variables
o_el_n(0),o_mu_n(0),o_lep_n(0),o_lepForVeto_n(0),
o_el_loose_n(0),o_mu_loose_n(0),o_lep_loose_n(0),
o_el(0),o_mu(0),o_lep(0),o_selLep(0),
//MET
o_AO_met(0),
//Leptonic top and leptonic W
o_leptop_n(0), o_leptop(0), o_leptop_b(0), o_lepW(0), o_lepW_n(0), o_nu(0),
//Semi-boosted hadronic top
o_bW_hadtop(0),
//Top-tagging truth studies variables
o_fjets_topmatched(0),o_fjets_topmatchboost_TTL(0),o_fjets_topmatchboost_TTT(0),
o_nhadtops(0),o_nboosthadtops(0),o_matchingeff(0),o_tageff_TTL(0),o_tageff_TTT(0),
//Kinematic variables
o_dRmin_lepjet(0),o_dRmin_ejets(0), o_dRmin_mujets(0),o_dRmin_lepbjet(0), o_dRmin_jetjet(0),  o_dRmin_RCjets(0),
  o_dEtamin_lepjet(0),o_dEtamin_lepbjet(0), o_dEtamin_jetjet(0), o_dEtamin_RCjets(0), o_dPhimin_lepjet(0),o_dPhimin_lepbjet(0), o_dPhimin_jetjet(0) , o_dPhimin_RCjets(0),
o_dRmin_RCMHiggsRCMHiggs(0), o_dRmin_RCMHiggsRCMV(0), o_dRmin_RCMHiggsRCMTop(0), o_dRmin_RCMVRCMV(0), o_dRmin_RCTTMassRCTTMass(0),
o_dRmin_RCMVRCMTop(0), o_dRmin_RCMTopRCMTop(0),o_dPhimin_RCMHiggsRCMHiggs(0), o_dPhimin_RCMHiggsRCMV(0), o_dPhimin_RCMHiggsRCMTop(0), o_dPhimin_RCMVRCMV(0), o_dPhimin_RCTTMassRCTTMass(0),
o_dPhimin_RCMVRCMTop(0), o_dPhimin_RCMTopRCMTop(0),o_dEtamin_RCMHiggsRCMHiggs(0), o_dEtamin_RCMHiggsRCMV(0), o_dEtamin_RCMHiggsRCMTop(0), o_dEtamin_RCMVRCMV(0),
o_dEtamin_RCMVRCMTop(0), o_dEtamin_RCMTopRCMTop(0), o_dEtamin_RCTTMassRCTTMass(0),
o_leadingdR_lepjet(0), o_leadingdR_lepbjet(0),o_leadingdR_jetjet(0),o_leadingdR_RCjets(0),
o_leadingdEta_lepjet(0), o_leadingdEta_lepbjet(0),o_leadingdEta_jetjet(0),o_leadingdEta_RCjets(0),
o_leadingdPhi_lepjet(0), o_leadingdPhi_lepbjet(0),o_leadingdPhi_jetjet(0),o_leadingdPhi_RCjets(0),
o_leadingdR_RCMHiggsRCMHiggs(0), o_leadingdR_RCMHiggsRCMV(0),
  o_leadingdR_RCMHiggsRCMTop(0), o_leadingdR_RCMVRCMV(0), o_leadingdR_RCMVRCMTop(0), o_leadingdR_RCMTopRCMTop(0),  o_leadingdR_RCTTMassRCTTMass(0),
o_leadingdEta_RCMHiggsRCMHiggs(0), o_leadingdEta_RCMHiggsRCMV(0),
  o_leadingdEta_RCMHiggsRCMTop(0), o_leadingdEta_RCMVRCMV(0), o_leadingdEta_RCMVRCMTop(0), o_leadingdEta_RCMTopRCMTop(0), o_leadingdEta_RCTTMassRCTTMass(0),
o_leadingdPhi_RCMHiggsRCMHiggs(0), o_leadingdPhi_RCMHiggsRCMV(0),
  o_leadingdPhi_RCMHiggsRCMTop(0), o_leadingdPhi_RCMVRCMV(0), o_leadingdPhi_RCMVRCMTop(0), o_leadingdPhi_RCMTopRCMTop(0),o_leadingdPhi_RCTTMassRCTTMass(0),
o_minRecoVLQMAsymm_HtHt(0), o_minRecoVLQMAsymm_HtZt(0), o_minRecoVLQMAsymm_HtWb(0),
o_dRmin_bjetbjet(0), o_dRmin_bjetbjet_lowb_3b(0), o_dRmin_bjetbjet_lowb_4b(0),
o_dR_TTL_bjets(0),o_dR_TTT_bjets(0), o_dR_TTLooser_bjets(0), o_mbb_mindR(0), o_mbb_mindR_lowb_3b(0), o_mbb_mindR_lowb_4b(0),
o_dPhi_lepmet(0), o_dPhi_jetmet(0), o_dPhi_jetmet5(0), o_dPhi_jetmet6(0), o_dPhi_jetmet7(0),o_dPhi_lepjet(0),o_dPhi_lepbjet(0),
o_dRmin_ebjets(0), o_dRmin_mubjets(0), o_mTbmin(0), o_mTbmin_lowb_3b(0), o_mTbmin_lowb_4b(0),
//HBSM variables
o_jets40_n(0), o_centrality(0), o_mbb_leading_bjets(0), o_mbb_softest_bjets(0), o_J_lepton_invariant_mass(0), o_J_leadingb_invariant_mass(0), o_J_J_invariant_mass(0),
o_dRaverage_bjetbjet(0),o_dRaverage_jetjet(0),o_dRaverage_lepjet(0),o_dRaverage_lepbjet(0),o_dRaverage_RCjets(0),o_dRaverage_RCTTMassRCTTMass(0),
o_dEtaaverage_jetjet(0),o_dEtaaverage_lepjet(0),o_dEtaaverage_lepbjet(0),o_dEtaaverage_RCjets(0),o_dEtaaverage_RCTTMassRCTTMass(0),
o_dPhiaverage_jetjet(0),o_dPhiaverage_lepjet(0),o_dPhiaverage_lepbjet(0), o_dPhiaverage_RCjets(0),o_dPhiaverage_RCTTMassRCTTMass(0),
o_invariant_mass_RCTTMassRCTTMass(0), o_invariant_mass_RCjets(0),o_dRmaxM_RCTTMassRCTTMass(0),o_dRmaxM_RCjets(0),o_dPhimaxM_RCTTMassRCTTMass(0),o_dPhimaxM_RCjets(0),o_dEtaminM_RCTTMassRCTTMass(0),o_dEtaminM_RCjets(0),
//Low mass Hplus Variable
o_mbb_maxdR(0), o_dPhibb_leading_bjets(0), o_dPhibb_mindR(0), o_dPhibb_maxdR(0), o_dEtabb_leading_bjets(0), o_dEtabb_mindR(0), o_dEtabb_maxdR(0), o_mjj_leading_jets(0), 
o_mjj_mindR(0), o_mjj_maxdR(0), o_dPhijj_leading_jets(0), o_dPhijj_mindR(0), o_dPhijj_maxdR(0), o_dEtajj_leading_jets(0), o_dEtajj_mindR(0), o_dEtajj_maxdR(0),
//Event variable for selection
o_rejectEvent(0),
//VLQ type
o_VLQtype(-1),
//VLQ_invariant_masses reconstruction
o_m_vlq_rcjets_pt_n(0),o_m_vlq_rcjets_detamin_n(0), o_m_vlq_rcjets_drmax_n(0), o_m_vlq_rcjets_dphimax_n(0),  
o_averagem_vlq_rcjets_pt(0),o_masymm_vlq_rcjets_pt(0),o_fmasymm_vlq_rcjets_pt(0),
o_averagem_vlq_rcjets_detamin(0),o_masymm_vlq_rcjets_detamin(0),o_fmasymm_vlq_rcjets_detamin(0),
o_averagem_vlq_rcjets_drmax(0),o_masymm_vlq_rcjets_drmax(0),o_fmasymm_vlq_rcjets_drmax(0),
o_averagem_vlq_rcjets_dphimax(0),o_masymm_vlq_rcjets_dphimax(0),o_fmasymm_vlq_rcjets_dphimax(0),

o_m_vlq_rcttmass_pt_n(0),o_m_vlq_rcttmass_detamin_n(0), o_m_vlq_rcttmass_drmax_n(0), o_m_vlq_rcttmass_dphimax_n(0),  
o_averagem_vlq_rcttmass_pt(0),o_masymm_vlq_rcttmass_pt(0),o_fmasymm_vlq_rcttmass_pt(0),
o_averagem_vlq_rcttmass_detamin(0),o_masymm_vlq_rcttmass_detamin(0),o_fmasymm_vlq_rcttmass_detamin(0),
o_averagem_vlq_rcttmass_drmax(0),o_masymm_vlq_rcttmass_drmax(0),o_fmasymm_vlq_rcttmass_drmax(0),
o_averagem_vlq_rcttmass_dphimax(0),o_masymm_vlq_rcttmass_dphimax(0),o_fmasymm_vlq_rcttmass_dphimax(0),
//Truth variables
o_truth_dR_Wb(0), o_truth_top_pt(0),o_truth_ht_filter(0),o_truth_met_filter(0),
//Truth VLQ variables
o_truth_dRmin_isHiggs_b(0),
o_truth_all_particles(0), o_truth_all_partons(0), o_truth_all_MEParticles(0),
o_truth_all_bpartons(0), o_truth_all_b(0), o_truth_all_q(0), o_truth_all_lep(0), o_truth_partonMET(0),
o_truth_dRmin_bb(0.),o_truth_minv_bb(0.),
o_truth_dPhiMin_lepmet(0.), o_truth_dPhiMin_jetmet(0.), o_truth_mtw(0.), o_truth_ptw(0.), o_truth_mtbmin(0.),
//TRF-weights storage
o_TRFweight_in(0),o_TRFweight_ex(0),o_TRFPerm_in(0),o_TRFPerm_ex(0),
o_LTag_BreakUp_TRF_in(0), o_LTag_BreakDown_TRF_in(0), o_LTag_BreakUp_TRF_ex(0), o_LTag_BreakDown_TRF_ex(0),//light-tagging
o_CTag_BreakUp_TRF_in(0), o_CTag_BreakDown_TRF_in(0), o_CTag_BreakUp_TRF_ex(0), o_CTag_BreakDown_TRF_ex(0),//c-tagging
o_BTag_BreakUp_TRF_in(0), o_BTag_BreakDown_TRF_in(0), o_BTag_BreakUp_TRF_ex(0), o_BTag_BreakDown_TRF_ex(0),//b-tagging
o_BTagExtrapUp_TRF_in(0), o_BTagExtrapDown_TRF_in(0), o_BTagExtrapUp_TRF_ex(0), o_BTagExtrapDown_TRF_ex(0),//extrapolation
o_BTagExtrapFromCharmUp_TRF_in(0), o_BTagExtrapFromCharmDown_TRF_in(0), o_BTagExtrapFromCharmUp_TRF_ex(0), o_BTagExtrapFromCharmDown_TRF_ex(0),//extrapolation from charm
o_is_ttbar(false)
{

  o_trigger_list.clear();

  o_region = new std::vector < int >;

  o_pmg_weight_threshold.clear();

  o_jets    = new AOVector();
  o_fwdjets = new AOVector();
  o_fwdjets_eta25_30 = new AOVector();
  o_fwdjets_eta30_35 = new AOVector();
  o_fwdjets_eta35_40 = new AOVector();
  o_fwdjets_eta40_45 = new AOVector();

  o_bjets   = new AOVector();

  o_jets_truth_b = new AOVector();
  o_jets_truth_c = new AOVector();
  o_jets_truth_tau = new AOVector();
  o_jets_truth_lqg = new AOVector();

  o_bjets_truth_b = new AOVector();
  o_bjets_truth_c = new AOVector();
  o_bjets_truth_tau = new AOVector();
  o_bjets_truth_lqg = new AOVector();

  if(m_opt->DoLowBRegions()){
    o_bjets_lowb_3b = new AOVector();
    o_bjets_lowb_4b = new AOVector();
  }
  o_fjets   = new AOVector();
  o_ljets   = new AOVector();
  if(m_opt->BtagCollection() == VLQ_Options::TRACK){
    o_trkjets = new AOVector();
    o_trkbjets = new AOVector();
    o_trkljets = new AOVector();

    o_trkjets_truth_b = new AOVector();
    o_trkjets_truth_c = new AOVector();
    o_trkjets_truth_tau = new AOVector();
    o_trkjets_truth_lqg = new AOVector();

    o_trkbjets_truth_b = new AOVector();
    o_trkbjets_truth_c = new AOVector();
    o_trkbjets_truth_tau = new AOVector();
    o_trkbjets_truth_lqg = new AOVector();

  }
  o_rcjets  = new AOVector();

  o_taggedjets_n.clear();
  o_taggedjets.clear();

  std::vector < std::string > jetTaggedType;
  jetTaggedType.push_back("RCTTMass");
  jetTaggedType.push_back("RCMTop");
  jetTaggedType.push_back("RCMTop0b");
  jetTaggedType.push_back("RCMTop1b");
  jetTaggedType.push_back("RCMTop2bin");
  jetTaggedType.push_back("RCMHiggs");
  jetTaggedType.push_back("RCMHiggs0b");
  jetTaggedType.push_back("RCMHiggs1b");
  jetTaggedType.push_back("RCMHiggs2bin");
  jetTaggedType.push_back("RCMTopHiggs");
  jetTaggedType.push_back("RCMV");
  jetTaggedType.push_back("RCMV0b");
  jetTaggedType.push_back("RCMV1bin");
  jetTaggedType.push_back("LooseRCTTMass");
  for ( const std::string type : jetTaggedType ){
    o_taggedjets.insert( std::pair<std::string, AOVector*>( type, new AOVector() ) );
    o_taggedjets_n.insert( std::pair<std::string, int>( type, 0 ) );
  }
  jetTaggedType.clear();

  o_rcjets_truthmatched_n.clear();
  o_rcjets_truthmatched.clear();

  std::vector < std::string > rcjetTruthType;
  rcjetTruthType.push_back("truthTop");
  rcjetTruthType.push_back("truthZ");
  rcjetTruthType.push_back("truthW");
  rcjetTruthType.push_back("truthHiggs");
  rcjetTruthType.push_back("truthOther");

  rcjetTruthType.push_back("truthTop_inMassWindow");
  rcjetTruthType.push_back("truthZ_inMassWindow");
  rcjetTruthType.push_back("truthW_inMassWindow");
  rcjetTruthType.push_back("truthHiggs_inMassWindow");
  for ( const std::string type : rcjetTruthType ){
    o_rcjets_truthmatched.insert( std::pair<std::string, AOVector*>( type, new AOVector() ) );
    o_rcjets_truthmatched_n.insert( std::pair<std::string, int>( type, 0 ) );
  }
  rcjetTruthType.clear();


  //All leptop and leptop-b categories
  o_catLeptop.clear();
  o_catLeptop_b.clear();
  o_catLeptop_n.clear();

  std::vector<std::string> leptopTypes = {"highM", "winM", "BoutRCtag", "BinRCtag",
					 "BinRCMTop","BinRCMHiggs","BinRCMV",
					  "BinRCtagNconst1","BinRCMTopNconst1","BinRCMHiggsNconst1","BinRCMVNconst1",
					  "truthMatch", "nontruthMatch", "lowdRbW", "highdRbW"};

  for(const std::string& lptype : leptopTypes){
    o_catLeptop.insert(std::pair<std::string, AnalysisObject*>(lptype, nullptr));
    o_catLeptop_b.insert(std::pair<std::string, AnalysisObject*>(lptype, nullptr));
    o_catLeptop_n.insert(std::pair<std::string, int>(lptype, 0));
  }
  leptopTypes.clear();



  o_el = new AOVector();
  o_mu = new AOVector();
  o_lep = new AOVector();

  o_nu = new AnalysisObject();
  o_lepW = new AnalysisObject();
  o_bW_hadtop = new AOVector();


  //Reco VLQ
  o_recoVLQ_n.clear();
  o_recoVLQ.clear();

  std::vector < std::string > decayType;
  decayType.push_back("Ht");
  decayType.push_back("Zt");
  decayType.push_back("Wb");
  for ( const std::string type : decayType ){
    o_recoVLQ.insert( std::pair<std::string, AOVector*>( type, new AOVector() ) );
    o_recoVLQ_n.insert( std::pair<std::string, int>( type, 0 ) );
  }
  decayType.clear();
  //VLq mass invariant reconstruction 
  // moved to line 412
  
 
  //Truth
  o_truth_all_particles = new AOVector();
  o_truth_all_partons = new AOVector();
  o_truth_all_MEParticles = new AOVector();

  o_truth_all_bpartons = new AOVector();
  o_truth_all_b = new AOVector();
  o_truth_all_q = new AOVector();
  o_truth_all_lep = new AOVector();
  o_truth_partonMET = new AnalysisObject();

  o_truth_partons.clear();
  o_truth_partons_n.clear();

  std::vector< std::string > truthType = {
    "VLQ", "VLQ_Ht", "VLQ_Zt", "VLQ_Wb",
    "VLQ_Hbdect","VLQ_Whadb","VLQ_Wlepb","VLQ_Zhadt",
    "b1", "b2",
    "H", "Hbb", "Hnonbb", "H_b1", "H_b2",
    "hadtop", "hadtop_b", "hadtop_W", "hadtop_q1", "hadtop_q2",
    "leptop", "leptop_b", "leptop_W", "leptop_lep", "leptop_nu",
    "hadZ", "hadZ_q1", "hadZ_q2",
    "lepZ", "lepZ_lep1", "lepZ_lep2",
    "invZ",
    "hadW", "hadW_q1", "hadW_q2",
    "lepW", "lepW_lep", "lepW_nu"
  };

  for( const std::string& type : truthType ){
    o_truth_partons.insert( std::pair<std::string, AOVector*>(type, new AOVector() ) );
    o_truth_partons_n.insert( std::pair<std::string, int>(type, 0. ) );

  }

  //====================================================================

  o_TRFweight_in      = new std::vector < double >;
  o_TRFweight_ex      = new std::vector < double >;
  o_TRFPerm_in        = new std::vector < std::vector< bool > >;
  o_TRFPerm_ex        = new std::vector < std::vector< bool > >;

  o_BTag_BreakUp_TRF_in   = new std::vector < std::vector < double > >;
  o_BTag_BreakDown_TRF_in = new std::vector < std::vector < double > >;
  o_CTag_BreakUp_TRF_in   = new std::vector < std::vector < double > >;
  o_CTag_BreakDown_TRF_in = new std::vector < std::vector < double > >;
  o_LTag_BreakUp_TRF_in   = new std::vector < std::vector < double > >;
  o_LTag_BreakDown_TRF_in = new std::vector < std::vector < double > >;
  o_BTag_BreakUp_TRF_ex   = new std::vector < std::vector < double > >;
  o_BTag_BreakDown_TRF_ex = new std::vector < std::vector < double > >;
  o_CTag_BreakUp_TRF_ex   = new std::vector < std::vector < double > >;
  o_CTag_BreakDown_TRF_ex = new std::vector < std::vector < double > >;
  o_LTag_BreakUp_TRF_ex   = new std::vector < std::vector < double > >;
  o_LTag_BreakDown_TRF_ex = new std::vector < std::vector < double > >;
  o_BTagExtrapUp_TRF_ex   = new std::vector < std::vector < double > >;
  o_BTagExtrapDown_TRF_ex = new std::vector < std::vector < double > >;
  o_BTagExtrapUp_TRF_in   = new std::vector < std::vector < double > >;
  o_BTagExtrapDown_TRF_in = new std::vector < std::vector < double > >;
  o_BTagExtrapFromCharmUp_TRF_in    = new std::vector < std::vector < double > >;
  o_BTagExtrapFromCharmDown_TRF_in  = new std::vector < std::vector < double > >;
  o_BTagExtrapFromCharmUp_TRF_ex    = new std::vector < std::vector < double > >;
  o_BTagExtrapFromCharmDown_TRF_ex  = new std::vector < std::vector < double > >;
}

//______________________________________________________
//
VLQ_OutputData::VLQ_OutputData( const VLQ_OutputData & q ):
OutputData(q)
{
  std::cout << "<!> WARNING: You're using an empty copy-constructor (VLQ_OutputData) ... Please check !!" << std::endl;
}

//______________________________________________________
//
VLQ_OutputData::~VLQ_OutputData()
{

  ClearOutputData();
  o_pmg_weight_threshold.clear();
  o_taggedjets_n.clear();
  o_rcjets_truthmatched_n.clear();

  for(TriggerInfo* trig : o_trigger_list){ delete trig; }
  o_trigger_list.clear();

  delete o_jets;
  delete o_bjets;
  delete o_ljets;

  delete o_jets_truth_b;
  delete o_jets_truth_c;
  delete o_jets_truth_tau;
  delete o_jets_truth_lqg;

  delete o_bjets_truth_b;
  delete o_bjets_truth_c;
  delete o_bjets_truth_tau;
  delete o_bjets_truth_lqg;

  delete o_trkjets;
  delete o_trkbjets;
  delete o_trkljets;

  delete o_trkjets_truth_b;
  delete o_trkjets_truth_c;
  delete o_trkjets_truth_tau;
  delete o_trkjets_truth_lqg;

  delete o_trkbjets_truth_b;
  delete o_trkbjets_truth_c;
  delete o_trkbjets_truth_tau;
  delete o_trkbjets_truth_lqg;

  delete o_fwdjets;
  delete o_fwdjets_eta25_30;
  delete o_fwdjets_eta30_35;
  delete o_fwdjets_eta35_40;
  delete o_fwdjets_eta40_45;
  delete o_fjets;
  delete o_rcjets;

  delete o_bjets_lowb_3b;
  delete o_bjets_lowb_4b;

  delete o_el;
  delete o_mu;
  delete o_lep;

  delete o_bW_hadtop;

  o_recoVLQ_n.clear();

  delete o_truth_all_particles; 
  delete o_truth_all_partons; 
  delete o_truth_all_MEParticles;

  delete o_truth_all_bpartons;
  delete o_truth_all_b; 
  delete o_truth_all_q; 
  delete o_truth_all_lep; 
  delete o_truth_partonMET; 

  for(std::pair<std::string, AOVector*> truthPair : o_truth_partons){
    delete truthPair.second;
  }

}

//______________________________________________________
//
void VLQ_OutputData::ClearOutputData()
{
  //
  // The ClearOutputData function has to be call at each event in
  // order to restore the original value of all variables, keeping
  // constant the address.
  //
  OutputData::ClearOutputData();
  for(TriggerInfo* trig : o_trigger_list){
    trig->SetPass(false);
  }
  //VLQ invariant mass reconstruction

  o_m_vlq_rcjets_pt.clear();
  o_m_vlq_rcjets_detamin.clear();
  o_m_vlq_rcjets_drmax.clear();
  o_m_vlq_rcjets_dphimax.clear();

  o_m_vlq_rcttmass_pt.clear();
  o_m_vlq_rcttmass_detamin.clear();
  o_m_vlq_rcttmass_drmax.clear();
  o_m_vlq_rcttmass_dphimax.clear();

  
  //
  // Flat variables
  //
  o_dRmin_lepjet = -99.;
  o_dRmin_ejets = -99.;
  o_dRmin_mujets = -99.;
  o_dRmin_lepbjet = -99.;
  o_dRmin_jetjet = -99.;
  o_dEtamin_lepjet = -99.;
  o_dEtamin_lepbjet = -99.;
  o_dEtamin_jetjet = -99.;
  o_dPhimin_lepjet = -99.;
  o_dPhimin_lepbjet = -99.;
  o_dPhimin_jetjet = -99.;
  o_dRmin_RCjets = -99.;
  o_dEtamin_RCjets = -99.;
  o_dPhimin_RCjets = -99.;
  o_dRmin_RCMHiggsRCMHiggs = -100.;
  o_dRmin_RCMHiggsRCMV = -100.;
  o_dRmin_RCMHiggsRCMTop = -100.; 
  o_dRmin_RCMVRCMV = -100.;
  o_dRmin_RCMVRCMTop = -100.; 
  o_dRmin_RCMTopRCMTop = -100.;
  o_leadingdR_lepjet = -100.;
  o_leadingdR_lepbjet = -100.;
  o_leadingdR_jetjet = -100.;
  o_leadingdEta_lepjet = -100.;
  o_leadingdEta_lepbjet = -100.;
  o_leadingdEta_jetjet = -100.;
  o_leadingdPhi_lepjet = -100.;
  o_leadingdPhi_lepbjet = -100.;
  o_leadingdPhi_jetjet = -100.;
  o_leadingdR_RCMHiggsRCMHiggs = -100.;
  o_leadingdR_RCMHiggsRCMV = -100.;
  o_leadingdR_RCMHiggsRCMTop = -100.;
  o_leadingdR_RCMVRCMV = -100.;
  o_leadingdR_RCMVRCMTop = -100.;
  o_leadingdR_RCMTopRCMTop = -100.;

  o_leadingdR_RCTTMassRCTTMass = -100.;
  o_leadingdEta_RCTTMassRCTTMass = -100.;
  o_leadingdPhi_RCTTMassRCTTMass = -100.;

  o_leadingdEta_RCMHiggsRCMHiggs = -100.;
  o_leadingdEta_RCMHiggsRCMV = -100.;
  o_leadingdEta_RCMHiggsRCMTop = -100.;
  o_leadingdEta_RCMVRCMV = -100.;
  o_leadingdEta_RCMVRCMTop = -100.;
  o_leadingdEta_RCMTopRCMTop = -100.;
  o_leadingdPhi_RCMHiggsRCMHiggs = -100.;
  o_leadingdPhi_RCMHiggsRCMV = -100.;
  o_leadingdPhi_RCMHiggsRCMTop = -100.;
  o_leadingdPhi_RCMVRCMV = -100.;
  o_leadingdPhi_RCMVRCMTop = -100.;
  o_leadingdPhi_RCMTopRCMTop = -100.;
  o_dPhimin_RCMHiggsRCMHiggs = -100.;
  o_dPhimin_RCMHiggsRCMV = -100.;
  o_dPhimin_RCMHiggsRCMTop = -100.;
  o_dPhimin_RCMVRCMV = -100.;
  o_dPhimin_RCMVRCMTop = -100.;
  o_dPhimin_RCMTopRCMTop = -100.;
  o_dPhimin_RCTTMassRCTTMass = -100.;
  o_dEtamin_RCTTMassRCTTMass = -100.;
  o_dRmin_RCTTMassRCTTMass = -100.;
  o_dEtamin_RCMHiggsRCMHiggs = -100.;
  o_dEtamin_RCMHiggsRCMV = -100.;
  o_dEtamin_RCMHiggsRCMTop = -100.;
  o_dEtamin_RCMVRCMV = -100.;
  o_dEtamin_RCMVRCMTop = -100.;
  o_dEtamin_RCMTopRCMTop = -100.;
  o_minRecoVLQMAsymm_HtHt = -100.;
  o_minRecoVLQMAsymm_HtZt = -100.;
  o_minRecoVLQMAsymm_HtWb = -100.;
  o_dRmin_bjetbjet = 99.;
  o_dRmin_bjetbjet_lowb_3b = 99.;
  o_dRmin_bjetbjet_lowb_4b = 99.;
  o_dR_TTL_bjets = 99.;
  o_dR_TTT_bjets = 99.;
  o_dR_TTLooser_bjets = 99.;
  o_mbb_mindR = -1.;
  o_mbb_mindR_lowb_3b = -1.;
  o_mbb_mindR_lowb_4b = -1.;
  o_dPhi_lepmet = 99.;
  o_dPhi_jetmet = 99.;
  o_dPhi_jetmet5 = 99.;
  o_dPhi_jetmet6 = 99.;
  o_dPhi_jetmet7 = 99.;
  o_dPhi_lepjet = 99.;
  o_dPhi_lepbjet = 99.;
  o_dRmin_ebjets = 99.;
  o_dRmin_mubjets = 99.;
  o_mTbmin = 99.;
  o_mTbmin_lowb_3b = 99.;
  o_mTbmin_lowb_4b = 99.;

  o_mu_n = 0;
  o_el_n = 0;
  o_lep_n = 0;
  o_lepForVeto_n = 0;
  o_mu_loose_n = 0;
  o_el_loose_n = 0;
  o_lep_loose_n = 0;
  o_jets_n = 0;
  o_bjets_n = 0;
  o_ljets_n = 0;

  o_jets_truth_b_n = 0;
  o_jets_truth_c_n = 0;
  o_jets_truth_tau_n = 0;
  o_jets_truth_lqg_n = 0;

  o_bjets_truth_b_n = 0;
  o_bjets_truth_c_n = 0;
  o_bjets_truth_tau_n = 0;
  o_bjets_truth_lqg_n = 0;

  o_trkjets_n = 0;
  o_trkbjets_n = 0;
  o_trkljets_n = 0;
  o_trkjets_nconcentric = 0;

  o_trkjets_truth_b_n = 0;
  o_trkjets_truth_c_n = 0;
  o_trkjets_truth_tau_n = 0;
  o_trkjets_truth_lqg_n = 0;

  o_trkbjets_truth_b_n = 0;
  o_trkbjets_truth_c_n = 0;
  o_trkbjets_truth_tau_n = 0;
  o_trkbjets_truth_lqg_n = 0;

  o_fwdjets_n = 0;
  o_fwdjets_eta25_30_n = 0;
  o_fwdjets_eta30_35_n = 0;
  o_fwdjets_eta35_40_n = 0;
  o_fwdjets_eta40_45_n = 0;
  o_fwdjets30_n = 0;
  o_fwdjets40_n = 0;

  o_rcjets_n = 0;
  o_fjets_n = 0;
  o_channel_type = 0;
  o_period = 0;
  o_run_number = 0;
  o_pileup_mu = 0;
  o_npv = 0;
  o_meff = 0;
  o_meffred = 0;
  o_met = 0;
  // o_residualMET = 0; corresponds when this is a double instead of an analysis object
  o_mtwl = 0;
  o_ptwl = 0;
  o_mll = 0;
  o_hthad = 0;
  o_metsig_ev = 0;
  o_metsig_obj = 0;
  o_MVAScore = -1;

  o_weight_pmg_isr_muRfac10__fsr_muRfac20 = 0;
  o_weight_pmg_isr_muRfac10__fsr_muRfac05 = 0;

  o_jets40_n = -100.;
  o_centrality = -100.;
  o_mbb_leading_bjets = -100.;
  o_mbb_softest_bjets = -100.;
  o_J_lepton_invariant_mass = -100.;
  o_J_leadingb_invariant_mass = -100.;
  o_J_J_invariant_mass = -100.;

  //VLQ Invariant mass recosntruction
  o_m_vlq_rcjets_pt_n = 0;
  o_m_vlq_rcjets_detamin_n = 0;
  o_m_vlq_rcjets_drmax_n = 0;
  o_m_vlq_rcjets_dphimax_n = 0;
  
  o_averagem_vlq_rcjets_pt= -1. ;
  o_masymm_vlq_rcjets_pt= -10000. ;
  o_fmasymm_vlq_rcjets_pt= -100. ;

  o_averagem_vlq_rcjets_detamin= -1. ;
  o_masymm_vlq_rcjets_detamin= -10000. ;
  o_fmasymm_vlq_rcjets_detamin= -100. ;
 
  o_averagem_vlq_rcjets_drmax= -1. ;
  o_masymm_vlq_rcjets_drmax= -10000. ;
  o_fmasymm_vlq_rcjets_drmax= -100. ;
 
  o_averagem_vlq_rcjets_dphimax= -1. ;
  o_masymm_vlq_rcjets_dphimax= -10000. ;
  o_fmasymm_vlq_rcjets_dphimax= -100. ;

  o_m_vlq_rcttmass_pt_n = 0;
  o_m_vlq_rcttmass_detamin_n = 0;
  o_m_vlq_rcttmass_drmax_n = 0;
  o_m_vlq_rcttmass_dphimax_n = 0;
  
  o_averagem_vlq_rcttmass_pt= -1. ;
  o_masymm_vlq_rcttmass_pt= -10000. ;
  o_fmasymm_vlq_rcttmass_pt= -100. ;

  o_averagem_vlq_rcttmass_detamin= -1. ;
  o_masymm_vlq_rcttmass_detamin= -10000. ;
  o_fmasymm_vlq_rcttmass_detamin= -100. ;
 
  o_averagem_vlq_rcttmass_drmax= -1. ;
  o_masymm_vlq_rcttmass_drmax= -10000. ;
  o_fmasymm_vlq_rcttmass_drmax= -100. ;

  o_averagem_vlq_rcttmass_dphimax= -1. ;
  o_masymm_vlq_rcttmass_dphimax= -10000. ;
  o_fmasymm_vlq_rcttmass_dphimax= -100. ;
  //
  o_invariant_mass_RCTTMassRCTTMass = -100.;
  o_invariant_mass_RCjets= -100.;
  o_dRmaxM_RCTTMassRCTTMass= -100.;
  o_dRmaxM_RCjets= -100.;
  o_dPhimaxM_RCTTMassRCTTMass= -100.;
  o_dPhimaxM_RCjets= -100.;
  o_dEtaminM_RCTTMassRCTTMass= -100.;
  o_dEtaminM_RCjets= -100.;

  o_dRaverage_bjetbjet = -100; 
  o_dRaverage_jetjet = -100;
  o_dRaverage_lepbjet = -100;
  o_dRaverage_lepjet = -100;

  o_dRaverage_RCjets = -100;
  o_dEtaaverage_RCjets = -100;
  o_dPhiaverage_RCjets = -100;

  o_dRaverage_RCTTMassRCTTMass = -100;
  o_dEtaaverage_RCTTMassRCTTMass = -100;
  o_dPhiaverage_RCTTMassRCTTMass = -100;

  o_dEtaaverage_jetjet = -100;
  o_dEtaaverage_lepbjet = -100;
  o_dEtaaverage_lepjet = -100;

  o_dPhiaverage_jetjet = -100;
  o_dPhiaverage_lepbjet = -100;
  o_dPhiaverage_lepjet = -100;

  o_fjets_topmatched=-99;
  o_fjets_topmatchboost_TTL=-99;
  o_fjets_topmatchboost_TTT=-99;
  o_nhadtops=-99;
  o_nboosthadtops=-99;
  o_matchingeff=-99;
  o_tageff_TTL=-99;
  o_tageff_TTT=-99;
  
  o_mbb_maxdR = -1.;
  o_dPhibb_leading_bjets=99.;
  o_dPhibb_mindR=99.;
  o_dPhibb_maxdR=99.;
  o_dEtabb_leading_bjets=99.;
  o_dEtabb_mindR=99.;
  o_dEtabb_maxdR=99.;

  o_mjj_leading_jets=-1.;
  o_mjj_mindR=-1.;
  o_mjj_maxdR=-1.;

  o_dPhijj_leading_jets=99.;
  o_dPhijj_mindR=99.;
  o_dPhijj_maxdR=99.;

  o_dEtajj_leading_jets=99.;
  o_dEtajj_mindR=99.;
  o_dEtajj_maxdR=99.;
  //
  // Selected lepton
  //
  o_selLep = 0;
  //
  // Vectors
  //
  //delete electrons
  for ( const AnalysisObject* ele : *(o_el) ) {
    delete ele;
  }
  o_el -> clear();

  //delete muons
  for ( const AnalysisObject* muon : *(o_mu) ) {
    delete muon;
  }
  o_mu -> clear();
  o_lep -> clear();

  //delete jets
  for ( const AnalysisObject* jet : *(o_jets) ) {
    delete jet;
  }
  o_jets -> clear();
  o_bjets -> clear();
  o_ljets->clear();

  o_jets_truth_b -> clear();
  o_jets_truth_c -> clear();
  o_jets_truth_tau -> clear();
  o_jets_truth_lqg -> clear();

  o_bjets_truth_b -> clear();
  o_bjets_truth_c -> clear();
  o_bjets_truth_tau -> clear();
  o_bjets_truth_lqg -> clear();

  //delete track jets
  if(o_trkjets){
    for ( const AnalysisObject* trkjet : *(o_trkjets) ) {
      delete trkjet;
    }
    o_trkjets->clear();
    o_trkbjets->clear();
    o_trkljets->clear();

    o_trkjets_truth_b -> clear();
    o_trkjets_truth_c -> clear();
    o_trkjets_truth_tau -> clear();
    o_trkjets_truth_lqg -> clear();

    o_trkbjets_truth_b -> clear();
    o_trkbjets_truth_c -> clear();
    o_trkbjets_truth_tau -> clear();
    o_trkbjets_truth_lqg -> clear();
  }

  if(o_bjets_lowb_3b) o_bjets_lowb_3b -> clear();
  if(o_bjets_lowb_4b) o_bjets_lowb_4b -> clear();

  //delete fwd-jets
  for ( const AnalysisObject* fwdjet : *(o_fwdjets) ) {
    delete fwdjet;
  }
  o_fwdjets -> clear();
  o_fwdjets_eta25_30 -> clear();
  o_fwdjets_eta30_35 -> clear();
  o_fwdjets_eta35_40 -> clear();
  o_fwdjets_eta40_45 -> clear();

  //delete RC jets
  for ( const AnalysisObject* RCjet : *(o_rcjets) ) {
    delete RCjet;
  }
  o_rcjets -> clear();

  //delete large-R jets
  for ( const AnalysisObject* LRJet : *(o_fjets) ) {
    delete LRJet;
  }
  o_fjets -> clear();

  //delete met
  delete o_AO_met;
  o_AO_met = nullptr;
  delete o_residualMET;
  o_residualMET = nullptr;
  //Reset neutrino 
  if(o_nu){
    delete o_nu;
    o_nu = nullptr;
  }

  //Reset lepW 
  if(o_lepW){
    delete o_lepW;
    o_lepW = nullptr;
  }

  //Reset leptop 
  if(o_leptop){
    delete o_leptop;
    o_leptop = nullptr;
  }

  //Reset b-jet from leptop
  o_leptop_b = nullptr;
  o_leptop_n = 0;
  o_lepW_n = 0;//not sure if it should go inside the if 
  //delete semi-boosted hadtops
  for ( const AnalysisObject* hadtop : *(o_bW_hadtop) ) {
    delete hadtop;
  }
  o_bW_hadtop -> clear();

  o_region -> clear();

  //
  // Map clearing
  //
  for(std::pair < std::string, float > pmg_weight_thr : o_pmg_weight_threshold){
    o_pmg_weight_threshold.at(pmg_weight_thr.first) = -1;
  }

  for ( std::pair < std::string, int > component : o_taggedjets_n ){
    o_taggedjets_n.at(component.first) = 0;
  }

  for(std::pair<std::string, AOVector*> obj_pair : o_taggedjets){
    obj_pair.second->clear();
  }

  for ( std::pair < std::string, int > component : o_rcjets_truthmatched_n ){
    o_rcjets_truthmatched_n.at(component.first) = 0;
  }

  for(std::pair<std::string, AOVector*> obj_pair : o_rcjets_truthmatched){
    obj_pair.second->clear();
  }

  for ( std::pair < std::string, int > component : o_recoVLQ_n ){
    o_recoVLQ_n.at(component.first) = 0;
  }

  for(std::pair<std::string, AOVector*> obj_pair : o_recoVLQ){
    AnalysisUtils::CleanContainer(*(obj_pair.second));
  }

  //
  // Leptop categories
  //
  for(std::pair<std::string, AnalysisObject*> lp_pair : o_catLeptop ){
    o_catLeptop.at(lp_pair.first) = nullptr;
  }
  for(std::pair<std::string, AnalysisObject*> lpb_pair : o_catLeptop_b ){
    o_catLeptop_b.at(lpb_pair.first) = nullptr;
  }
  for(std::pair<std::string, int> lpb_pair : o_catLeptop_n ){    //probably this line is not correct
    o_catLeptop_n.at(lpb_pair.first) = 0;
  }


  //
  // Event variable for selection
  //
  o_rejectEvent = 0;

  //
  // VLQ classification
  //
  o_VLQtype = -1;

  //
  // Truth variables
  //

  for(std::pair<std::string, AOVector*> truth_pair : o_truth_partons){
    truth_pair.second->clear();
    o_truth_partons_n.at(truth_pair.first) = 0;
  }

  o_truth_all_partons->clear();
  o_truth_all_MEParticles->clear();

  o_truth_all_bpartons->clear();
  o_truth_all_b->clear();
  o_truth_all_q->clear();
  o_truth_all_lep->clear();

  AnalysisUtils::CleanContainer(*o_truth_all_particles);

  o_truth_partonMET->SetPtEtaPhiM(0.,0.,0.,0.);


  o_truth_dR_Wb = -1;
  o_truth_top_pt = 0;
  o_truth_ht_filter = 0.;
  o_truth_met_filter = 0.;

  //VLQ sample
  o_truth_dRmin_isHiggs_b = 0;

  o_truth_dRmin_bb = -1.;
  o_truth_minv_bb = 0.;
  o_truth_dPhiMin_lepmet = -10.;
  o_truth_dPhiMin_jetmet = -10.;
  o_truth_mtw = 0.;
  o_truth_ptw = 0.;
  o_truth_mtbmin = 0.;
     
}
