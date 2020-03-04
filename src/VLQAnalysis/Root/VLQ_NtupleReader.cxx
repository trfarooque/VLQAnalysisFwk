#include "IFAETopFramework/TriggerInfo.h"

#include "VLQAnalysis/VLQ_Options.h"
#include "VLQAnalysis/VLQ_Enums.h"
#include "VLQAnalysis/VLQ_NtupleReader.h"
#include "VLQAnalysis/VLQ_NtupleData.h"

#include <iostream>

using namespace std;

//_________________________________________________________________________________
//
VLQ_NtupleReader::VLQ_NtupleReader(VLQ_Options* opt):
NtupleReader(opt),
m_opt(opt),
m_trigger_list(NULL)
{
  m_VLQ_ntupData = new VLQ_NtupleData();
  m_ntupData = dynamic_cast<NtupleData*>(m_VLQ_ntupData);
}

//_________________________________________________________________________________
//
VLQ_NtupleReader::~VLQ_NtupleReader()
{
  delete m_VLQ_ntupData;
}

//_________________________________________________________________________________
//
int VLQ_NtupleReader::SetAllBranchAddresses()
{
  int stat = 0;
  stat += NtupleReader::SetAllBranchAddresses(); if(stat != 0){ std::cout << "Failed in " << std::endl; return stat; }
  stat += SetEventBranchAddresses(); if(stat != 0){ std::cout << "Failed in SetEventBranchAddresses" << std::endl; return stat; }
  stat += SetJetBranchAddresses(); if(stat != 0){ std::cout << "Failed in SetJetBranchAddresses" << std::endl; return stat; }
  stat += SetLeptonBranchAddresses(); if(stat != 0){ std::cout << "Failed in SetLeptonBranchAddresses" << std::endl; return stat; }
  stat += SetMETBranchAddresses(); if(stat != 0){ std::cout << "Failed in SetMETBranchAddresses" << std::endl; return stat; }
  if( m_opt -> InputTree() == "nominal" ){
    stat += SetTruthParticleBranchAddresses(); if(stat != 0){ std::cout << "Failed in SetTruthParticleBranchAddresses" << std::endl; return stat; }
  }
  return stat;
}

//_________________________________________________________________________________
//
int VLQ_NtupleReader::SetEventBranchAddresses(){

  int stat = 0;

  if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in VLQ_NtupleReader::SetEventBranchAddresses()" << std::endl;

  //
  // Event variables (PU, ...)
  //
  stat =  SetVariableToChain("event_number",     &(m_VLQ_ntupData->d_eventNumber) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("run_number",       &(m_VLQ_ntupData->d_runNumber) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("average_interactions_per_crossing",     &(m_VLQ_ntupData->d_mu) ); if(stat != 0){ return stat; }

  if( m_opt -> SampleName() == SampleName::TTBAR || m_opt -> SampleName() == SampleName::TTBARBB ||
  m_opt -> SampleName() == SampleName::TTBARCC || m_opt -> SampleName() == SampleName::TTBARLIGHT ){
    //
    // Ttbar+HF variables
    //
    stat =  SetVariableToChain("ttbar_class", &(m_VLQ_ntupData->d_HF_SimpleClassification) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ttbar_class_ext", &(m_VLQ_ntupData->d_HF_Classification) ); if(stat != 0){ return stat; }

    //
    // Truth information
    //
    stat =  SetVariableToChain("gen_filt_ht", &(m_VLQ_ntupData->d_HT_truth) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("gen_filt_met", &(m_VLQ_ntupData->d_MET_truth) ); if(stat != 0){ return stat; }
  }

  //
  // Trigger and trigger matching
  //
  for( TriggerInfo* trig : *m_trigger_list ){
    const std::string& trigname = trig->Name();
    m_VLQ_ntupData->d_triggers.insert(std::pair<std::string, int>(trigname, 0) );
    stat = SetVariableToChain("pass_"+trigname, &(m_VLQ_ntupData->d_triggers.at(trigname)) );
    if(trig->Type() == VLQ_Enums::TRIGELEC){
      m_VLQ_ntupData->d_el_trigMatch.insert(std::pair<std::string, std::vector<int>* >(trigname, NULL) );
      stat = SetVariableToChain("electrons_trgMatch_"+trigname, &(m_VLQ_ntupData->d_el_trigMatch.at(trigname)) );
    }
    else if(trig->Type() == VLQ_Enums::TRIGMUON){
      m_VLQ_ntupData->d_mu_trigMatch.insert( std::pair<std::string, std::vector<int>* >(trigname, NULL) );
      stat = SetVariableToChain("muons_trgMatch_"+trigname, &(m_VLQ_ntupData->d_mu_trigMatch.at(trigname)) );
    }
  }

  //
  // VLQ-specific truth information
  //
  if( m_opt -> SampleName() == SampleName::VLQ ){
    stat =  SetVariableToChain("top_decay_type", &(m_VLQ_ntupData->d_truth_top_decay) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("antitop_decay_type", &(m_VLQ_ntupData->d_truth_antitop_decay) ); if(stat != 0){ return stat; }
  }

  if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving VLQ_NtupleReader::setEventBranchAddresses()" << std::endl;

  return 0;
}

//_________________________________________________________________________________
//
int VLQ_NtupleReader::SetJetBranchAddresses(const string &/*sj*/){

  int stat = 0;

  if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in VLQ_NtupleReader::SetJetBranchAddresses()" << std::endl;

  //
  // Small-radius jets
  //
  stat =  SetVariableToChain("jets_pt", &(m_VLQ_ntupData->d_jet_pt) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("jets_phi", &(m_VLQ_ntupData->d_jet_phi) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("jets_eta", &(m_VLQ_ntupData->d_jet_eta) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("jets_e", &(m_VLQ_ntupData->d_jet_E) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("jets_btag_weight", &(m_VLQ_ntupData->d_jet_btag_weight) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("jets_jvt", &(m_VLQ_ntupData->d_jet_jvt) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("jets_isb_"+m_opt->BtagOP(), &(m_VLQ_ntupData->d_jet_isb) ); if(stat != 0){ return stat; }

  if(!(m_opt->IsData() || m_opt -> StrSampleName().find("QCD") != std::string::npos)){
    stat =  SetVariableToChain("jets_truthLabel", &(m_VLQ_ntupData->d_jet_truthLabel) ); if(stat != 0){ return stat; }
  }

  //
  // Forward-radius jets
  //
  stat =  SetVariableToChain("fwjets_pt", &(m_VLQ_ntupData->d_fwdjet_pt) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("fwjets_phi", &(m_VLQ_ntupData->d_fwdjet_phi) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("fwjets_eta", &(m_VLQ_ntupData->d_fwdjet_eta) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("fwjets_e", &(m_VLQ_ntupData->d_fwdjet_E) ); if(stat != 0){ return stat; }
  // stat =  SetVariableToChain("fwjets_btag_weight", &(m_VLQ_ntupData->d_fwdjet_btag_weight) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("fwjets_jvt", &(m_VLQ_ntupData->d_fwdjet_jvt) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("fwjets_fJvt", &(m_VLQ_ntupData->d_fwdjet_fJvt) ); if(stat != 0){ return stat; }
  // stat =  SetVariableToChain("jets_isSignal", &(m_VLQ_ntupData->d_fwdjet_isSignal) ); if(stat != 0){ return stat; }
  // stat =  SetVariableToChain("jets_passOR", &(m_VLQ_ntupData->d_fwdjet_passOR) ); if(stat != 0){ return stat; }
  if(!(m_opt->IsData() || m_opt -> StrSampleName().find("QCD") != std::string::npos)){
    stat =  SetVariableToChain("fwjets_truthLabel", &(m_VLQ_ntupData->d_fwdjet_truthLabel) ); if(stat != 0){ return stat; }
  }

  //
  // Track jets - read only if trkjet b-tagging is used
  //
  if(m_opt->BtagCollection()==VLQ_Options::TRACK){
    stat =  SetVariableToChain("trkjets_pt", &(m_VLQ_ntupData->d_trkjet_pt) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("trkjets_phi", &(m_VLQ_ntupData->d_trkjet_phi) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("trkjets_eta", &(m_VLQ_ntupData->d_trkjet_eta) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("trkjets_btag_weight", &(m_VLQ_ntupData->d_trkjet_btag_weight) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("trkjets_isb_"+m_opt->BtagOP(), &(m_VLQ_ntupData->d_trkjet_isb) ); if(stat != 0){ return stat; }
    if(!(m_opt->IsData() || m_opt -> StrSampleName().find("QCD") != std::string::npos)){
      stat =  SetVariableToChain("trkjets_truthLabel", &(m_VLQ_ntupData->d_trkjet_truthLabel) ); if(stat != 0){ return stat; }
    }
  }

  //
  // Reclustered jets
  //
  //if(m_opt->RCCollection() == m_opt->RCCollections::RC){
    std::cout << "Using standard reclustering jet collection" << std::endl;
    stat =  SetVariableToChain("rc_"+m_opt->RCCollection()+"_jets_pt", &(m_VLQ_ntupData->d_rcjets_pt) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("rc_"+m_opt->RCCollection()+"_jets_eta", &(m_VLQ_ntupData->d_rcjets_eta) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("rc_"+m_opt->RCCollection()+"_jets_phi", &(m_VLQ_ntupData->d_rcjets_phi) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("rc_"+m_opt->RCCollection()+"_jets_m", &(m_VLQ_ntupData->d_rcjets_m) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("rc_"+m_opt->RCCollection()+"_jets_nconst", &(m_VLQ_ntupData->d_rcjets_nconsts) ); if(stat != 0){ return stat; }
    //}
    /*
  else{
    //
    // Varaible-R reclustered jets
    //
    //Replace the local RC collection with a VRRC collection according to the passed option
    std::string vrc_par="";
    if     (m_opt->RCCollection() == m_opt->RCCollections::R12_RHO_300)vrc_par="r12rho300";
    else if(m_opt->RCCollection() == m_opt->RCCollections::R10_RHO_300)vrc_par="r10rho300";
    else if(m_opt->RCCollection() == m_opt->RCCollections::R10_RHO_350)vrc_par="r10rho350";
    else if(m_opt->RCCollection() == m_opt->RCCollections::R10_RHO_250)vrc_par="r10rho250";
    else {std::cout<<"Unknown RCCollection option " << std::endl;}

    std::cout << "Using variable-R reclustering jet collection " << vrc_par <<std::endl;

    m_VLQ_ntupData->d_vrcjets_pt.insert( std::pair < std::string, std::vector <float>* > ( "vrc_"+vrc_par, NULL ) );
    m_VLQ_ntupData->d_vrcjets_eta.insert( std::pair < std::string, std::vector <float>* > ( "vrc_"+vrc_par, NULL ) );
    m_VLQ_ntupData->d_vrcjets_phi.insert( std::pair < std::string, std::vector <float>* > ( "vrc_"+vrc_par, NULL ) );
    m_VLQ_ntupData->d_vrcjets_m.insert( std::pair < std::string, std::vector <float>* > ( "vrc_"+vrc_par, NULL ) );
    m_VLQ_ntupData->d_vrcjets_nconsts.insert( std::pair < std::string, std::vector <int>* > ( "vrc_"+vrc_par, NULL ) );
    stat =  SetVariableToChain("vrc_" + vrc_par + "_jets_pt",       &(m_VLQ_ntupData->d_rcjets_pt     ) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("vrc_" + vrc_par + "_jets_eta",      &(m_VLQ_ntupData->d_rcjets_eta    ) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("vrc_" + vrc_par + "_jets_phi",      &(m_VLQ_ntupData->d_rcjets_phi    ) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("vrc_" + vrc_par + "_jets_m",        &(m_VLQ_ntupData->d_rcjets_m      ) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("vrc_" + vrc_par + "_jets_nconst",   &(m_VLQ_ntupData->d_rcjets_nconsts) ); if(stat != 0){ return stat; }
  }
    */
  //
  // Large-R jets
  //
  if(m_opt->UseLargeRJets()){
    stat =  SetVariableToChain("ljet_pt", &(m_VLQ_ntupData->d_ljet_pt) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_eta", &(m_VLQ_ntupData->d_ljet_eta) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_phi", &(m_VLQ_ntupData->d_ljet_phi) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_e", &(m_VLQ_ntupData->d_ljet_e) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_m", &(m_VLQ_ntupData->d_ljet_m) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_sd12", &(m_VLQ_ntupData->d_ljet_sd12) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_sd23", &(m_VLQ_ntupData->d_ljet_sd23) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_tau21_wta", &(m_VLQ_ntupData->d_ljet_tau21_wta) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_tau32_wta", &(m_VLQ_ntupData->d_ljet_tau32_wta) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_D2", &(m_VLQ_ntupData->d_ljet_D2) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_C2", &(m_VLQ_ntupData->d_ljet_C2) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_topTag", &(m_VLQ_ntupData->d_ljet_topTag) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_bosonTag", &(m_VLQ_ntupData->d_ljet_bosonTag) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_topTag_loose", &(m_VLQ_ntupData->d_ljet_topTag_loose) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("ljet_bosonTag_loose", &(m_VLQ_ntupData->d_ljet_bosonTag_loose) ); if(stat != 0){ return stat; }
  }
  if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving VLQ_NtupleReader::setJetBranchAddresses()" << std::endl;

  return 0;
}

//_________________________________________________________________________________
//
int VLQ_NtupleReader::SetLeptonBranchAddresses(){

  int stat = 0;

  if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in VLQ_NtupleReader::SetLeptonBranchAddresses()" << std::endl;

  //
  // Electron-related branches
  //
  stat =  SetVariableToChain("electrons_pt", &(m_VLQ_ntupData->d_el_pt) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("electrons_phi", &(m_VLQ_ntupData->d_el_phi) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("electrons_eta", &(m_VLQ_ntupData->d_el_eta) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("electrons_e", &(m_VLQ_ntupData->d_el_e) ); if(stat != 0){ return stat; }
  //if(!m_opt->IsR21()){
  /*
    stat =  SetVariableToChain("electrons_ptvarcone20", &(m_VLQ_ntupData->d_el_ptvarcone20) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("electrons_topoetcone20", &(m_VLQ_ntupData->d_el_topoetcone20) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("electrons_d0sig", &(m_VLQ_ntupData->d_el_d0sig) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("electrons_z0", &(m_VLQ_ntupData->d_el_z0) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("electrons_isSignal", &(m_VLQ_ntupData->d_el_isSignal) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("electrons_passOR", &(m_VLQ_ntupData->d_el_passOR) ); if(stat != 0){ return stat; }
  */
    //}
  //
  // Muon-related branches
  //
  stat =  SetVariableToChain("muons_pt",      &(m_VLQ_ntupData->d_mu_pt) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("muons_phi",     &(m_VLQ_ntupData->d_mu_phi) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("muons_eta",     &(m_VLQ_ntupData->d_mu_eta) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("muons_e",       &(m_VLQ_ntupData->d_mu_e) ); if(stat != 0){ return stat; }
  //if(!m_opt->IsR21()){
  /*
    stat =  SetVariableToChain("muons_ptvarcone30",   &(m_VLQ_ntupData->d_mu_ptvarcone30) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("muons_topoetcone20",  &(m_VLQ_ntupData->d_mu_topoetcone20) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("muons_d0sig",         &(m_VLQ_ntupData->d_mu_d0sig) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("muons_z0",            &(m_VLQ_ntupData->d_mu_z0) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("muons_isSignal",      &(m_VLQ_ntupData->d_mu_isSignal) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("muons_passOR",        &(m_VLQ_ntupData->d_mu_passOR) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("muons_isCosmic",      &(m_VLQ_ntupData->d_mu_isCosmic) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("muons_isBad",         &(m_VLQ_ntupData->d_mu_isBad) ); if(stat != 0){ return stat; }
  */
  //}

  if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving VLQ_NtupleReader::setLeptonBranchAddresses()" << std::endl;

  return 0;
}

//_________________________________________________________________________________
//
int VLQ_NtupleReader::SetMETBranchAddresses(){

  int stat = 0;

  if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in VLQ_NtupleReader::SetMETBranchAddresses()" << std::endl;

  stat =  SetVariableToChain("mettst", &(m_VLQ_ntupData->d_met_met) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("mettst_phi", &(m_VLQ_ntupData->d_met_phi) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("mettst_sig", &(m_VLQ_ntupData->d_met_sig) ); if(stat != 0){ return stat; }

  if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving VLQ_NtupleReader::SetMETBranchAddresses()" << std::endl;

  return 0;
}

//_________________________________________________________________________________
//
int VLQ_NtupleReader::SetTruthParticleBranchAddresses(){

  if( m_opt -> SampleName() != SampleName::TTBAR && m_opt -> SampleName() != SampleName::TTBARBB &&
      m_opt -> SampleName() != SampleName::TTBARCC && m_opt -> SampleName() != SampleName::TTBARLIGHT  && m_opt -> SampleName() != SampleName::VLQ){
    return 0;
  }

  int stat = 0;

  if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in VLQ_NtupleReader::SetTruthParticleBranchAddresses()" << std::endl;

  stat =  SetVariableToChain("mc_pt", &(m_VLQ_ntupData->d_mc_pt) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("mc_eta", &(m_VLQ_ntupData->d_mc_eta) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("mc_phi", &(m_VLQ_ntupData->d_mc_phi) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("mc_m", &(m_VLQ_ntupData->d_mc_m) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("mc_pdgId", &(m_VLQ_ntupData->d_mc_pdgId) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("mc_children_index", &(m_VLQ_ntupData->d_mc_children_index) ); if(stat != 0){ return stat; }

  if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving VLQ_NtupleReader::SetTruthParticleBranchAddresses()" << std::endl;

  return 0;
}

//_________________________________________________________________________________
//
int VLQ_NtupleReader::SetTRFBranchAddresses(){

  int stat = 0;

  if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Entering in VLQ_NtupleReader::SetTRFBranchAddresses()" << std::endl;

  //Nominal weights
  stat =  SetVariableToChain("truthTagisB_2bex", &(m_VLQ_ntupData->d_trf_tagged_77_2ex) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("truthTagisB_3bex", &(m_VLQ_ntupData->d_trf_tagged_77_3ex) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("truthTagisB_4bin", &(m_VLQ_ntupData->d_trf_tagged_77_4in) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("truthTagWei_Nominal_2bex", &(m_VLQ_ntupData->d_trf_weight_77_2ex) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("truthTagWei_Nominal_3bex", &(m_VLQ_ntupData->d_trf_weight_77_3ex) ); if(stat != 0){ return stat; }
  stat =  SetVariableToChain("truthTagWei_Nominal_4bin", &(m_VLQ_ntupData->d_trf_weight_77_4in) ); if(stat != 0){ return stat; }

  //Systematic weights
  if(m_opt -> ComputeWeightSys()){
    //B-flavour
    for ( unsigned int iB = 0; iB <=5; ++iB ){
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_B_%i__1up_2bex", iB), &(m_VLQ_ntupData->d_trf_weight_77_2ex_eigenvars_B_up->at(iB)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_B_%i__1up_3bex", iB ), &(m_VLQ_ntupData->d_trf_weight_77_3ex_eigenvars_B_up->at(iB)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_B_%i__1up_4bin", iB ), &(m_VLQ_ntupData->d_trf_weight_77_4in_eigenvars_B_up->at(iB)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_B_%i__1down_2bex", iB ), &(m_VLQ_ntupData->d_trf_weight_77_2ex_eigenvars_B_down->at(iB)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_B_%i__1down_3bex", iB ), &(m_VLQ_ntupData->d_trf_weight_77_3ex_eigenvars_B_down->at(iB)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_B_%i__1down_4bin", iB ), &(m_VLQ_ntupData->d_trf_weight_77_4in_eigenvars_B_down->at(iB)) ); if(stat != 0){ return stat; }
    }//B-eigenvectors
    //C-flavour
    for ( unsigned int iC = 0; iC <=3; ++iC ){
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_C_%i__1up_2bex", iC ), &(m_VLQ_ntupData->d_trf_weight_77_2ex_eigenvars_C_up->at(iC)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_C_%i__1up_3bex", iC ), &(m_VLQ_ntupData->d_trf_weight_77_3ex_eigenvars_C_up->at(iC)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_C_%i__1up_4bin", iC ), &(m_VLQ_ntupData->d_trf_weight_77_4in_eigenvars_C_up->at(iC)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_C_%i__1down_2bex", iC ), &(m_VLQ_ntupData->d_trf_weight_77_2ex_eigenvars_C_down->at(iC)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_C_%i__1down_3bex", iC ), &(m_VLQ_ntupData->d_trf_weight_77_3ex_eigenvars_C_down->at(iC)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_C_%i__1down_4bin", iC ), &(m_VLQ_ntupData->d_trf_weight_77_4in_eigenvars_C_down->at(iC)) ); if(stat != 0){ return stat; }
    }//C-eigenvectors
    //Light-flavour
    for ( unsigned int iL = 0; iL <=16; ++iL ){
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_Light_%i__1up_2bex", iL ), &(m_VLQ_ntupData->d_trf_weight_77_2ex_eigenvars_Light_up->at(iL)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_Light_%i__1up_3bex", iL ), &(m_VLQ_ntupData->d_trf_weight_77_3ex_eigenvars_Light_up->at(iL)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_Light_%i__1up_4bin", iL ), &(m_VLQ_ntupData->d_trf_weight_77_4in_eigenvars_Light_up->at(iL)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_Light_%i__1down_2bex", iL ), &(m_VLQ_ntupData->d_trf_weight_77_2ex_eigenvars_Light_down->at(iL)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_Light_%i__1down_3bex", iL ), &(m_VLQ_ntupData->d_trf_weight_77_3ex_eigenvars_Light_down->at(iL)) ); if(stat != 0){ return stat; }
      stat =  SetVariableToChain(Form("truthTagWei_FT_EFF_Eigen_Light_%i__1down_4bin", iL ), &(m_VLQ_ntupData->d_trf_weight_77_4in_eigenvars_Light_down->at(iL)) ); if(stat != 0){ return stat; }
    }//Light-eigenvectors

    stat =  SetVariableToChain("truthTagWei_FT_EFF_extrapolation__1up_2bex", &(m_VLQ_ntupData->d_trf_weight_77_2ex_extrapolation_up) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("truthTagWei_FT_EFF_extrapolation__1down_2bex", &(m_VLQ_ntupData->d_trf_weight_77_2ex_extrapolation_down) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("truthTagWei_FT_EFF_extrapolation_from_charm__1up_2bex", &(m_VLQ_ntupData->d_trf_weight_77_2ex_extrapolation_from_charm_up) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("truthTagWei_FT_EFF_extrapolation_from_charm__1down_2bex", &(m_VLQ_ntupData->d_trf_weight_77_2ex_extrapolation_from_charm_down) ); if(stat != 0){ return stat; }

    stat =  SetVariableToChain("truthTagWei_FT_EFF_extrapolation__1up_3bex", &(m_VLQ_ntupData->d_trf_weight_77_3ex_extrapolation_up) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("truthTagWei_FT_EFF_extrapolation__1down_3bex", &(m_VLQ_ntupData->d_trf_weight_77_3ex_extrapolation_down) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("truthTagWei_FT_EFF_extrapolation_from_charm__1up_3bex", &(m_VLQ_ntupData->d_trf_weight_77_3ex_extrapolation_from_charm_up) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("truthTagWei_FT_EFF_extrapolation_from_charm__1down_3bex", &(m_VLQ_ntupData->d_trf_weight_77_3ex_extrapolation_from_charm_down) ); if(stat != 0){ return stat; }

    stat =  SetVariableToChain("truthTagWei_FT_EFF_extrapolation__1up_4bin", &(m_VLQ_ntupData->d_trf_weight_77_4in_extrapolation_up) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("truthTagWei_FT_EFF_extrapolation__1down_4bin", &(m_VLQ_ntupData->d_trf_weight_77_4in_extrapolation_down) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("truthTagWei_FT_EFF_extrapolation_from_charm__1up_4bin", &(m_VLQ_ntupData->d_trf_weight_77_4in_extrapolation_from_charm_up) ); if(stat != 0){ return stat; }
    stat =  SetVariableToChain("truthTagWei_FT_EFF_extrapolation_from_charm__1down_4bin", &(m_VLQ_ntupData->d_trf_weight_77_4in_extrapolation_from_charm_down) ); if(stat != 0){ return stat; }

  }

  if(m_opt->MsgLevel()==Debug::DEBUG) std::cout << "Leaving VLQ_NtupleReader::SetTRFBranchAddresses()" << std::endl;

  return 0;
}
