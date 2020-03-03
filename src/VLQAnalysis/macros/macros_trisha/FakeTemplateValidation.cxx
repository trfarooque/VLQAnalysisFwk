#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include "IFAETopFramework/HistManager.h"
#include <vector>
#include <string>
#include <iostream>
 
int main(int argc, char** argv){

  bool isdata = ( atoi(argv[3]) > 0 );
  bool isttlight = ( atoi(argv[4]) == 1 );
  bool isttbb = ( atoi(argv[4]) == 2 );
  bool isvjets22 = ( atoi(argv[4]) == 3 );
  bool isqcd = ( atoi(argv[4]) == 4 );
  bool dolowb = ( atoi(argv[6]) > 0 );
  std::string qcdconfig = Form("config%i", atoi(argv[5]));

  std::cout<<" isdata =  "<<isdata <<std::endl;
  std::cout<<" isttlight =  "<<isttlight <<std::endl;
  std::cout<<" isttbb =  "<<isttbb <<std::endl;
  std::cout<<" isvjets22 =  "<<isvjets22 <<std::endl;
  std::cout<<" isqcd =  "<<isqcd <<std::endl;
  std::cout<<" qcdconfig =  "<<qcdconfig <<std::endl;
  std::cout<<" dolowb =  "<<dolowb <<std::endl;


  if(argc<6){ std::cout<<"Provide name of file from which to read chain; name of file to write to; isdata; istt"<<std::endl; }


  //_______________________ SET UP THE CHAIN ________________________

  TChain          *fChain = new TChain("tree");   //!pointer to the analyzed TTree or TChain
  fChain->Add(argv[1]);

  // Declaration of leaf types
  Double_t        nomWeight;

  Float_t         nomWeight_weight_btag;
  Float_t         nomWeight_weight_leptonSF;
  Float_t         nomWeight_weight_jvt;
  Float_t         nomWeight_weight_mc;
  Double_t        nomWeight_weight_norm;
  Float_t         nomWeight_weight_pileup;
  //Double_t        nomWeight_weight_ttbar_fractions_rw;
  Float_t        nomWeight_weight_ttbar_nnlo;
  Double_t        nomWeight_weight_ttbb;
  Float_t         nomWeight_weight_sherpa_22_vjets;

  Float_t        nomWeight_fakesMM_weight_ejets_2015;
  //Float_t        nomWeight_fakesMM_weight_ejets_2016;
  Float_t        nomWeight_fakesMM_weight_mujets_2015;
  Float_t        nomWeight_fakesMM_weight_mujets_2016;

  Int_t           channel;
  Int_t           run_number;
  Int_t           data_period;

  Int_t           npv;
  Double_t        pileup_mu;

  Int_t           bjets_n;
  Double_t        dPhi_jetmet;
  Double_t        dPhi_lepbjet;
  Double_t        dPhi_lepjet;
  Double_t        dPhi_lepmet;
  Double_t        dRmin_bb;
  Double_t        dRmin_ebjets;
  Double_t        dRmin_ejets;
  Double_t        dRmin_jetjet;
  Double_t        dRmin_mubjets;
  Double_t        dRmin_mujets;
  Double_t        el1_eta;
  Double_t        el1_phi;
  Double_t        el1_pt;
  Int_t           el_n;

  Int_t           el_loose_n;
  Int_t           mu_loose_n;
  Int_t           lep_loose_n;

  std::vector<double>*        fjets_eta;
  std::vector<double>*        fjets_m;
  std::vector<double>*        fjets_phi;
  std::vector<double>*        fjets_pt;
  Int_t           fjets_n;

  std::vector<double>*        jets_btagw;
  std::vector<double>*        jets_eta;
  std::vector<double>*        jets_jvt;
  std::vector<double>*        jets_m;
  std::vector<double>*        jets_phi;
  std::vector<double>*        jets_pt;
  std::vector<double>*        jets_drmin_rc = new std::vector<double>; //input : 0

  Int_t           jets_n;
  Int_t           lep_n;
  Double_t        mT_bmin;
  Double_t        mbb_mindr;

  Double_t        mT_bmin_lowb_3b;
  Double_t        mbb_mindr_lowb_3b;
  Double_t        dRmin_bb_lowb_3b;

  Double_t        mT_bmin_lowb_4b;
  Double_t        mbb_mindr_lowb_4b;
  Double_t        dRmin_bb_lowb_4b;


  Double_t        meff;
  Double_t        met;
  Double_t        met_phi;
  Double_t        mtw;
  //Double_t        ptw;
  Double_t        mu1_eta;
  Double_t        mu1_phi;
  Double_t        mu1_pt;
  Int_t           mu_n;
  Double_t        hthad;
  std::vector<double>*        rcjets_eta;
  std::vector<double>*        rcjets_m;
  std::vector<double>*        rcjets_nconsts;
  std::vector<double>*        rcjets_phi;
  std::vector<double>*        rcjets_pt;
  Int_t           rcjets_n;
  Int_t           ttjets_n;

  // List of branches
  //TBranch        *b_nomWeight; //!

  TBranch        *b_channel;   //!
  TBranch        *b_data_period;   //!
  TBranch        *b_run_number;   //!
  TBranch        *b_npv;   //!
  TBranch        *b_pileup_mu;   //!
  
  TBranch        *b_nomWeight_weight_btag;   //!
  TBranch        *b_nomWeight_weight_leptonSF;   //!
  TBranch        *b_nomWeight_weight_jvt;   //!
  TBranch        *b_nomWeight_weight_mc;   //!
  TBranch        *b_nomWeight_weight_norm;   //!
  TBranch        *b_nomWeight_weight_pileup;   //!
  TBranch        *b_nomWeight_weight_ttbb;   //!
  TBranch        *b_nomWeight_weight_ttbar_nnlo;   //!
  TBranch        *b_nomWeight_weight_sherpa_22_vjets; //!

  TBranch        *b_nomWeight_fakesMM_weight_ejets_2015;
  //TBranch        *b_nomWeight_fakesMM_weight_ejets_2016;
  TBranch        *b_nomWeight_fakesMM_weight_mujets_2015;
  TBranch        *b_nomWeight_fakesMM_weight_mujets_2016;
  
  TBranch        *b_bjets_n;   //!
  TBranch        *b_dPhi_jetmet;   //!
  TBranch        *b_dPhi_lepbjet;   //!
  TBranch        *b_dPhi_lepjet;   //!
  TBranch        *b_dPhi_lepmet;   //!
  TBranch        *b_dRmin_bb;   //!
  TBranch        *b_dRmin_ebjets;   //!
  TBranch        *b_dRmin_ejets;   //!
  TBranch        *b_dRmin_jetjet;   //!
  TBranch        *b_dRmin_mubjets;   //!
  TBranch        *b_dRmin_mujets;   //!
  TBranch        *b_el1_eta;   //!
  TBranch        *b_el1_phi;   //!
  TBranch        *b_el1_pt;   //!
  TBranch        *b_el_n;   //!
  TBranch        *b_fjets_eta;   //!
  TBranch        *b_fjets_m;   //!
  TBranch        *b_fjets_phi;   //!
  TBranch        *b_fjets_pt;   //!
  TBranch        *b_fjets_n;   //!
  TBranch        *b_jets_btagw;   //!
  TBranch        *b_jets_eta;   //!
  TBranch        *b_jets_jvt;   //!
  TBranch        *b_jets_m;   //!
  TBranch        *b_jets_phi;   //!
  TBranch        *b_jets_pt;   //!
  TBranch        *b_jets_n;   //!
  TBranch        *b_lep_n;   //!
  TBranch        *b_mT_bmin;   //!
  TBranch        *b_mbb_mindr;   //!

  TBranch        *b_mT_bmin_lowb_3b; //!
  TBranch        *b_mbb_mindr_lowb_3b; //!
  TBranch        *b_dRmin_bb_lowb_3b; //!

  TBranch        *b_mT_bmin_lowb_4b; //!
  TBranch        *b_mbb_mindr_lowb_4b; //!
  TBranch        *b_dRmin_bb_lowb_4b; //!

  TBranch        *b_meff;   //!
  TBranch        *b_met;   //!
  TBranch        *b_met_phi;   //!
  TBranch        *b_mtw;   //!
  TBranch        *b_mu1_eta;   //!
  TBranch        *b_mu1_phi;   //!
  TBranch        *b_mu1_pt;   //!
  TBranch        *b_mu_n;   //!

  TBranch        *b_el_loose_n; //!
  TBranch        *b_mu_loose_n; //!
  TBranch        *b_lep_loose_n; //!
 
  TBranch        *b_hthad;   //!
  TBranch        *b_rcjets_eta;   //!
  TBranch        *b_rcjets_m;   //!
  TBranch        *b_rcjets_nconsts;   //!
  TBranch        *b_rcjets_phi;   //!
  TBranch        *b_rcjets_pt;   //!
  TBranch        *b_rcjets_n;   //!
  TBranch        *b_ttjets_n;   //!

  fjets_eta = 0;
  fjets_m = 0;
  fjets_phi = 0;
  fjets_pt = 0;
  jets_btagw = 0;
  jets_eta = 0;
  jets_jvt = 0;
  jets_m = 0;
  jets_phi = 0;
  jets_pt = 0;
  rcjets_eta = 0;
  rcjets_m = 0;
  rcjets_nconsts = 0;
  rcjets_phi = 0;
  rcjets_pt = 0;

  //fChain->SetBranchAddress("nomWeight", &nomWeight, &b_nomWeight);
  if(!isdata){
    if(isqcd){
      fChain->SetBranchAddress(Form("nomWeight_fakesMM_weight_ejets_2015_Loose_test2015%s", qcdconfig.c_str()), &nomWeight_fakesMM_weight_ejets_2015
			       , &b_nomWeight_fakesMM_weight_ejets_2015);
      //fChain->SetBranchAddress(Form("nomWeight_fakesMM_weight_ejets_2016_%s", qcdconfig.c_str()), &nomWeight_fakesMM_weight_ejets_2016
      //		       , &b_nomWeight_fakesMM_weight_ejets_2016);
      fChain->SetBranchAddress(Form("nomWeight_fakesMM_weight_mujets_2015_Loose_test2015%s", qcdconfig.c_str()), &nomWeight_fakesMM_weight_mujets_2015
			       , &b_nomWeight_fakesMM_weight_mujets_2015);
      fChain->SetBranchAddress(Form("nomWeight_fakesMM_weight_mujets_2016_Loose_test2016%s", qcdconfig.c_str()), &nomWeight_fakesMM_weight_mujets_2016
			       , &b_nomWeight_fakesMM_weight_mujets_2016);
    }
    else{
      fChain->SetBranchAddress("nomWeight_weight_btag", &nomWeight_weight_btag, &b_nomWeight_weight_btag);
      fChain->SetBranchAddress("nomWeight_weight_leptonSF", &nomWeight_weight_leptonSF, &b_nomWeight_weight_leptonSF);
      fChain->SetBranchAddress("nomWeight_weight_jvt", &nomWeight_weight_jvt, &b_nomWeight_weight_jvt);
      fChain->SetBranchAddress("nomWeight_weight_mc", &nomWeight_weight_mc, &b_nomWeight_weight_mc);
      fChain->SetBranchAddress("nomWeight_weight_norm", &nomWeight_weight_norm, &b_nomWeight_weight_norm);
      fChain->SetBranchAddress("nomWeight_weight_pileup", &nomWeight_weight_pileup, &b_nomWeight_weight_pileup);
      if(isttbb){
	fChain->SetBranchAddress("nomWeight_weight_ttbb", &nomWeight_weight_ttbb, &b_nomWeight_weight_ttbb);
      }
      if(isttlight){
	fChain->SetBranchAddress("nomWeight_weight_ttbar_nnlo", &nomWeight_weight_ttbar_nnlo, &b_nomWeight_weight_ttbar_nnlo);
      }
      else if(isvjets22){
	fChain->SetBranchAddress("nomWeight_weight_sherpa_22_vjets", &nomWeight_weight_sherpa_22_vjets, &b_nomWeight_weight_sherpa_22_vjets);
      }//vjets_22
    }//!qcd
  }//!data

  fChain->SetBranchAddress("channel", &channel, &b_channel);
  fChain->SetBranchAddress("run_number", &run_number, &b_run_number);
  fChain->SetBranchAddress("data_period", &data_period, &b_data_period);
  fChain->SetBranchAddress("npv", &npv, &b_npv);
  fChain->SetBranchAddress("pileup_mu", &pileup_mu, &b_pileup_mu);

  fChain->SetBranchAddress("bjets_n", &bjets_n, &b_bjets_n);
  fChain->SetBranchAddress("dPhi_jetmet", &dPhi_jetmet, &b_dPhi_jetmet);
  fChain->SetBranchAddress("dPhi_lepbjet", &dPhi_lepbjet, &b_dPhi_lepbjet);
  fChain->SetBranchAddress("dPhi_lepjet", &dPhi_lepjet, &b_dPhi_lepjet);
  fChain->SetBranchAddress("dPhi_lepmet", &dPhi_lepmet, &b_dPhi_lepmet);
  fChain->SetBranchAddress("dRmin_bb", &dRmin_bb, &b_dRmin_bb);
  fChain->SetBranchAddress("dRmin_ebjets", &dRmin_ebjets, &b_dRmin_ebjets);
  fChain->SetBranchAddress("dRmin_ejets", &dRmin_ejets, &b_dRmin_ejets);
  fChain->SetBranchAddress("dRmin_jetjet", &dRmin_jetjet, &b_dRmin_jetjet);
  fChain->SetBranchAddress("dRmin_mubjets", &dRmin_mubjets, &b_dRmin_mubjets);
  fChain->SetBranchAddress("dRmin_mujets", &dRmin_mujets, &b_dRmin_mujets);
  fChain->SetBranchAddress("el1_eta", &el1_eta, &b_el1_eta);
  fChain->SetBranchAddress("el1_phi", &el1_phi, &b_el1_phi);
  fChain->SetBranchAddress("el1_pt", &el1_pt, &b_el1_pt);
  fChain->SetBranchAddress("el_n", &el_n, &b_el_n);

  fChain->SetBranchAddress("el_loose_n", &el_loose_n, &b_el_loose_n);
  fChain->SetBranchAddress("mu_loose_n", &mu_loose_n, &b_mu_loose_n);
  fChain->SetBranchAddress("lep_loose_n", &lep_loose_n, &b_lep_loose_n);

  fChain->SetBranchAddress("fjets_eta", &fjets_eta, &b_fjets_eta);
  fChain->SetBranchAddress("fjets_m", &fjets_m, &b_fjets_m);
  fChain->SetBranchAddress("fjets_phi", &fjets_phi, &b_fjets_phi);
  fChain->SetBranchAddress("fjets_pt", &fjets_pt, &b_fjets_pt);
  fChain->SetBranchAddress("fjets_n", &fjets_n, &b_fjets_n);
  fChain->SetBranchAddress("jets_btagw", &jets_btagw, &b_jets_btagw);
  fChain->SetBranchAddress("jets_eta", &jets_eta, &b_jets_eta);
  fChain->SetBranchAddress("jets_jvt", &jets_jvt, &b_jets_jvt);
  fChain->SetBranchAddress("jets_m", &jets_m, &b_jets_m);
  fChain->SetBranchAddress("jets_phi", &jets_phi, &b_jets_phi);
  fChain->SetBranchAddress("jets_pt", &jets_pt, &b_jets_pt);
  fChain->SetBranchAddress("jets_n", &jets_n, &b_jets_n);
  fChain->SetBranchAddress("lep_n", &lep_n, &b_lep_n);
  fChain->SetBranchAddress("mT_bmin", &mT_bmin, &b_mT_bmin);
  fChain->SetBranchAddress("mbb_mindr", &mbb_mindr, &b_mbb_mindr);
  if(dolowb){
    fChain->SetBranchAddress("mT_bmin_lowb_3b", &mT_bmin_lowb_3b, &b_mT_bmin_lowb_3b);
    fChain->SetBranchAddress("mbb_mindr_lowb_3b", &mbb_mindr_lowb_3b, &b_mbb_mindr_lowb_3b);
    fChain->SetBranchAddress("dRmin_bb_lowb_3b", &dRmin_bb_lowb_3b, &b_dRmin_bb_lowb_3b);

    fChain->SetBranchAddress("mT_bmin_lowb_4b", &mT_bmin_lowb_4b, &b_mT_bmin_lowb_4b);
    fChain->SetBranchAddress("mbb_mindr_lowb_4b", &mbb_mindr_lowb_4b, &b_mbb_mindr_lowb_4b);
    fChain->SetBranchAddress("dRmin_bb_lowb_4b", &dRmin_bb_lowb_4b, &b_dRmin_bb_lowb_4b);
  }
  fChain->SetBranchAddress("meff", &meff, &b_meff);
  fChain->SetBranchAddress("met", &met, &b_met);
  fChain->SetBranchAddress("met_phi", &met_phi, &b_met_phi);
  fChain->SetBranchAddress("mtw", &mtw, &b_mtw);
  fChain->SetBranchAddress("mu1_eta", &mu1_eta, &b_mu1_eta);
  fChain->SetBranchAddress("mu1_phi", &mu1_phi, &b_mu1_phi);
  fChain->SetBranchAddress("mu1_pt", &mu1_pt, &b_mu1_pt);
  fChain->SetBranchAddress("mu_n", &mu_n, &b_mu_n);
  fChain->SetBranchAddress("hthad", &hthad, &b_hthad);
  fChain->SetBranchAddress("rcjets_eta", &rcjets_eta, &b_rcjets_eta);
  fChain->SetBranchAddress("rcjets_m", &rcjets_m, &b_rcjets_m);
  fChain->SetBranchAddress("rcjets_nconsts", &rcjets_nconsts, &b_rcjets_nconsts);
  fChain->SetBranchAddress("rcjets_phi", &rcjets_phi, &b_rcjets_phi);
  fChain->SetBranchAddress("rcjets_pt", &rcjets_pt, &b_rcjets_pt);
  fChain->SetBranchAddress("rcjets_n", &rcjets_n, &b_rcjets_n);
  fChain->SetBranchAddress("ttjets_n", &ttjets_n, &b_ttjets_n);

  //_________________________________________________________________________________________________

  std::vector<std::string> regions; regions.clear();

  //regions.push_back("0lep"); 

  //regions.push_back("0lep_period2015"); 
  //regions.push_back("0lep_period2016"); 
  /*
  regions.push_back("0lep_lowmu"); 
  regions.push_back("0lep_highmu"); 
  */
  //regions.push_back("1lep"); 
  //regions.push_back("1elmu"); 
  /*
  regions.push_back("1elmu"); 
  regions.push_back("1el"); 
  regions.push_back("1mu"); 
  regions.push_back("1elmu_period2015"); 
  regions.push_back("1elmu_period2016"); 
  regions.push_back("1el_period2015"); 
  regions.push_back("1el_period2016"); 
  regions.push_back("1mu_period2015"); 
  regions.push_back("1mu_period2016"); 
  */

  regions.push_back("metmtwinc_1el_0bex");
  regions.push_back("metmtwinc_1mu_0bex");

  regions.push_back("metmtwinc_1el_1bex");
  regions.push_back("metmtwinc_1mu_1bex");

  regions.push_back("metmtwinc_1el_2bin");
  regions.push_back("metmtwinc_1mu_2bin");

  regions.push_back("1el_0bex");
  regions.push_back("1mu_0bex");

  regions.push_back("1el_1bex");
  regions.push_back("1mu_1bex");

  regions.push_back("1el_2bin");
  regions.push_back("1mu_2bin");

  /*
  regions.push_back("1elmu_lowmu"); 
  regions.push_back("1elmu_highmu"); 
  regions.push_back("1el_lowmu"); 
  regions.push_back("1el_highmu"); 
  regions.push_back("1mu_lowmu"); 
  regions.push_back("1mu_highmu"); 
  */
  //regions.push_back("0lep_RC1M_nsj1"); 
  //regions.push_back("0lep_RC1M_mwin"); 
  //regions.push_back("0lep_RC1M_moutwin"); 

  /*
  regions.push_back("0lep_RC1M_nsj2"); 
  regions.push_back("0lep_RC1M_nsjge3"); 


  regions.push_back("0lep_RC1M_nsj2_mwin"); 
  regions.push_back("0lep_RC1M_nsj2_moutwin"); 
  */
  HistManager* m_hstMngr = new HistManager(true, true);
  

  std::vector<std::string> variables;

  variables.push_back("npv");
  variables.push_back("pileup_mu");

  variables.push_back("fjets_n");
  variables.push_back("rcjets_n");
  variables.push_back("jets_n");
  variables.push_back("bjets_n");

  //variables.push_back("el_loose_n");
  //variables.push_back("mu_loose_n");
  //variables.push_back("lep_loose_n");

  variables.push_back("dPhi_jetmet");

  variables.push_back("dPhi_lepbjet");
  variables.push_back("dPhi_lepjet");
  variables.push_back("dPhi_lepmet");

  variables.push_back("dRmin_ebjets");
  variables.push_back("dRmin_ejets");
  variables.push_back("dRmin_mubjets");
  variables.push_back("dRmin_mujets");

  variables.push_back("dRmin_bb");
  variables.push_back("dRmin_jetjet");

  variables.push_back("mT_bmin");
  variables.push_back("mbb_mindr");
  variables.push_back("ptbb_mindr");
  variables.push_back("etabb_mindr");
  variables.push_back("phibb_mindr");

  if(dolowb){
    variables.push_back("dRmin_bb_lowb_3b");
    variables.push_back("mT_bmin_lowb_3b");
    variables.push_back("mbb_mindr_lowb_3b");

    variables.push_back("dRmin_bb_lowb_4b");
    variables.push_back("mT_bmin_lowb_4b");
    variables.push_back("mbb_mindr_lowb_4b");
  }

  variables.push_back("bjet1_mindr_pt");
  variables.push_back("bjet1_mindr_mass");
  variables.push_back("bjet1_mindr_eta");
  variables.push_back("bjet1_mindr_phi");
  variables.push_back("bjet1_mindr_dR_rc");

  variables.push_back("bjet2_mindr_pt");
  variables.push_back("bjet2_mindr_mass");
  variables.push_back("bjet2_mindr_eta");
  variables.push_back("bjet2_mindr_phi");
  variables.push_back("bjet2_mindr_dR_rc");

  variables.push_back("jet1_mindr_pt");
  variables.push_back("jet1_mindr_mass");
  variables.push_back("jet1_mindr_eta");
  variables.push_back("jet1_mindr_phi");
  variables.push_back("jet1_mindr_dR_rc");

  variables.push_back("jet2_mindr_pt");
  variables.push_back("jet2_mindr_mass");
  variables.push_back("jet2_mindr_eta");
  variables.push_back("jet2_mindr_phi");
  variables.push_back("jet2_mindr_dR_rc");


  variables.push_back("hthad");
  variables.push_back("meff");
  variables.push_back("met");
  variables.push_back("met_phi");
  variables.push_back("mtw");

  variables.push_back("fjet1_pt");
  variables.push_back("fjet1_mass");
  variables.push_back("fjet1_eta");
  variables.push_back("fjet1_phi");

  variables.push_back("fjets_pt");
  variables.push_back("fjets_mass");
  variables.push_back("fjets_eta");
  variables.push_back("fjets_phi");

  variables.push_back("rcjet1_pt");
  variables.push_back("rcjet1_eta");
  variables.push_back("rcjet1_phi");
  variables.push_back("rcjet1_mass");
  variables.push_back("rcjet1_nconsts");

  variables.push_back("rcjet2_pt");
  variables.push_back("rcjet2_eta");
  variables.push_back("rcjet2_phi");
  variables.push_back("rcjet2_mass");
  variables.push_back("rcjet2_nconsts");

  variables.push_back("rcjets_pt");
  variables.push_back("rcjets_eta");
  variables.push_back("rcjets_phi");
  variables.push_back("rcjets_mass");
  variables.push_back("rcjets_nconsts");

  variables.push_back("jets_pt");
  variables.push_back("jets_mass");
  variables.push_back("jets_eta");
  variables.push_back("jets_phi");
  variables.push_back("jets_btagw");
  variables.push_back("jets_jvt");
  variables.push_back("jets_dRmin_j");
  
  variables.push_back("bjets_pt");
  variables.push_back("bjets_mass");
  variables.push_back("bjets_eta");
  variables.push_back("bjets_phi");
  variables.push_back("bjets_btagw");
  variables.push_back("bjets_jvt");
  variables.push_back("bjets_dRmin_j");

  variables.push_back("ljets_pt");
  variables.push_back("ljets_mass");
  variables.push_back("ljets_eta");
  variables.push_back("ljets_phi");
  variables.push_back("ljets_btagw");
  variables.push_back("ljets_jvt");
  variables.push_back("ljets_dRmin_j");

  //
  variables.push_back("jet1_pt");
  variables.push_back("jet1_mass");
  variables.push_back("jet1_eta");
  variables.push_back("jet1_phi");
  variables.push_back("jet1_btagw");
  variables.push_back("jet1_jvt");
  variables.push_back("jet1_dRmin_j");
  
  variables.push_back("bjet1_pt");
  variables.push_back("bjet1_mass");
  variables.push_back("bjet1_eta");
  variables.push_back("bjet1_phi");
  variables.push_back("bjet1_btagw");
  variables.push_back("bjet1_jvt");
  variables.push_back("bjet1_dRmin_j");

  variables.push_back("ljet1_pt");
  variables.push_back("ljet1_mass");
  variables.push_back("ljet1_eta");
  variables.push_back("ljet1_phi");
  variables.push_back("ljet1_btagw");
  variables.push_back("ljet1_jvt");
  variables.push_back("ljet1_dRmin_j");

  variables.push_back("el1_pt");
  variables.push_back("el1_eta");
  variables.push_back("el1_phi");

  variables.push_back("mu1_pt");
  variables.push_back("mu1_eta");
  variables.push_back("mu1_phi");


  for(const std::string region : regions){

    for(const std::string variable : variables){
      double binsize = 0.; double binmin = 0.; double binmax = 0.;
      if( (variable == "fjets_n") || (variable == "rcjets_n") ){ binsize = 1.; binmin = 0.; binmax = 5.; }
      else if( variable == "jets_n" ){ binsize = 1.; binmin = 0.; binmax = 15.; }
      else if( variable == "bjets_n" ){ binsize = 1.; binmin = 0.; binmax = 8.; }
      else if( variable == "ljets_n" ){ binsize = 1.; binmin = 0.; binmax = 15.; }
      else if( (variable == "el_loose_n") || (variable == "mu_loose_n") || (variable == "lep_loose_n") ){ binsize = 1.; binmin = 0.; binmax = 4.; }
      else if( (variable.find("_njets") != std::string::npos) || (variable.find("_nconsts") != std::string::npos) ){
	binsize = 1.; binmin = 0.; binmax = 8.;
      }
      else if(variable.find("_nbjets") != std::string::npos){ binsize = 1.; binmin = 0.; binmax = 4.; }
      else if(variable.find("_nljets") != std::string::npos){ binsize = 1.; binmin = 0.; binmax = 4.; }
      
      else if( (variable.find("dPhi") != std::string::npos) || (variable.find("phibb") != std::string::npos) 
	       || (variable.find("_phi") != std::string::npos) ){ binsize = 0.2; binmin = -4.; binmax = 4.; }
      else if( (variable.find("_eta") != std::string::npos) || (variable.find("etabb") != std::string::npos) ){ binsize = 0.2; binmin = -3.; binmax = 3.; }
      else if( variable.find("dR") != std::string::npos ){ binsize = 0.25; binmin = 0.; binmax = 5.; }
      else if( (variable == "meff") || (variable == "hthad") ){ binsize = 50.; binmin = 0.; binmax = 2000.; }
      else if(variable == "met"){ binsize = 20.; binmin = 0.; binmax = 500.; }
      else if(variable == "mtw"){ binsize = 20.; binmin = 0.; binmax = 400.; }
      else if( (variable.find("mT_bmin") != std::string::npos) || (variable.find("mbb_mindr") != std::string::npos) ){ binsize = 25.; binmin = 0.; binmax = 400.; }
      else if( (variable.find("_mass") != std::string::npos) 
	       || (variable.find("_mjsum") != std::string::npos) 
	       || (variable.find("_m_sumj") != std::string::npos) ){
	if( (variable.find("rcjet") != std::string::npos) || (variable.find("fjet") != std::string::npos) ){
	  binsize = 20.; binmin = 0.; binmax = 400.;
	}
	else{ binsize = 10.; binmin = 0.; binmax = 150.; }
      }
      else if( (variable.find("ptbb") != std::string::npos) || (variable.find("_pt") != std::string::npos) ){
	if(variable.find("_ptfrac") != std::string::npos){ binsize = 0.05; binmin = 0.; binmax = 1.; }
	else if( (variable.find("rcjet") != std::string::npos) || (variable.find("fjet") != std::string::npos) ){
	  binsize = 20.; binmin = 0.; binmax = 1000.;
	}
	else if( (variable == "el1_pt") || (variable == "mu1_pt") ){ binsize = 5.; binmin = 0.; binmax = 500.; }
	else{ binsize = 20.; binmin = 0.; binmax = 500.; }
      }
      else if( (variable.find("btagw") != std::string::npos) || (variable.find("jvt") != std::string::npos) ){ binsize = 0.1; binmin = -1.1; binmax = 1.1; }
      else if( variable == "npv" ){ binsize = 1.; binmin = 0.; binmax = 50.;}
      else if( variable == "pileup_mu" ){ binsize = 1.; binmin = 0.; binmax = 50.;}
      //---------------------------------
      //std::cout<<" Booking histogram "<<region+"_"+variable<<std::endl;
      m_hstMngr->BookTH1D(region+"_"+variable,  "", binsize, binmin, binmax);	
	
    }//variables

  }//regions

    //
    //m_hstMngr->BookTH1D(region+"_"+"jets_rc1_trim05_pt", "", 10., 0., 500.);
    //____________________________________________________

  //__________________________ EVENT LOOP _____________________________________

  for(long int entry = 0; entry < fChain->GetEntries(); ++entry){

    jets_drmin_rc->clear();

    if(entry % 10000 == 0) std::cout<<"Processing entry "<<entry<<std::endl;
    fChain->GetEntry(entry);

    nomWeight = 1;
    if(!isdata){
      if(!isqcd){
	nomWeight = nomWeight_weight_btag * nomWeight_weight_mc * nomWeight_weight_norm * nomWeight_weight_pileup;// * nomWeight_weight_jvt;
	if( el_n + mu_n > 0 ){
	  nomWeight *= nomWeight_weight_leptonSF;
	}
	if(isttlight){
	  nomWeight *= nomWeight_weight_ttbar_nnlo;
	} 
	else if(isttbb){
	  nomWeight *= nomWeight_weight_ttbb;
	} 
	else if(isvjets22){
	  nomWeight *= nomWeight_weight_sherpa_22_vjets;
	}
      }
      else{
	if(channel==1 && data_period==1){
	  nomWeight = nomWeight_fakesMM_weight_ejets_2015;
	}
	else if(channel==1 && data_period==2){ continue;
	  //nomWeight = nomWeight_fakesMM_weight_ejets_2016;
	}
	else if(channel==2 && data_period==1){
	  nomWeight = nomWeight_fakesMM_weight_mujets_2015;
	}
	else if(channel==2 && data_period==2){
	  nomWeight = nomWeight_fakesMM_weight_mujets_2016;
	}
      }
    }

    if(!dolowb && (bjets_n < 2)) continue;
    if(jets_n < 5) continue;


    for(const std::string region:regions){
      if( (region.find("noRC") != std::string::npos) && (rcjets_n > 0) ) continue;
      else if( (region.find("RC1M") != std::string::npos) && (rcjets_n != 1) ) continue;
      //-------------------------------------------------------------
      if(region.find("_low150") != std::string::npos){
	if(rcjets_n>0 && rcjets_m->at(0) > 150) continue;
      }
      if(region.find("_high150") != std::string::npos){
	if(rcjets_n>0 && rcjets_m->at(0) < 150) continue;
      }

      if(region.find("_mwin") != std::string::npos){
	if(rcjets_n>0 && (rcjets_m->at(0) < 90 || rcjets_m->at(0) > 140) ) continue;
      }

      if(region.find("_moutwin") != std::string::npos){
	if(rcjets_n>0 && (rcjets_m->at(0) >= 90 && rcjets_m->at(0) <= 140) ) continue;
      }

      if(region.find("_nsj1") != std::string::npos){
	if(rcjets_n>0 && rcjets_nconsts->at(0) != 1) continue;
      }
      if(region.find("_nsj2") != std::string::npos){
	if(rcjets_n>0 && rcjets_nconsts->at(0) != 2) continue;
      }
      if(region.find("_nsjge3") != std::string::npos){
	if(rcjets_n>0 && rcjets_nconsts->at(0) < 3) continue;
      }


      //---------------------------------------------------------------

      if(region.find("0lep") != std::string::npos){
	if ( (el_n+mu_n > 0) || (met < 200) || (fabs(dPhi_jetmet) < 0.4) || (jets_n < 6) || (channel != 3) ){continue;}
      }
      else if(region.find("1elmu") != std::string::npos){ 
	if  (el_n+mu_n <= 0  || (channel != 1 && channel != 2) ){continue;}
      }
      else if(region.find("1el") != std::string::npos){
	if  (el_n != 1 || mu_n > 0 || channel != 1){continue;}
      }
      else if(region.find("1mu") != std::string::npos){
	if (el_n > 0 || mu_n != 1 || channel != 2){continue;}
      }

      //--------------------------------------------------------------------------------------------

      if( (region.find("0bex") != std::string::npos) && (bjets_n != 0) ){continue;} 
      else if( (region.find("1bex") != std::string::npos) && (bjets_n != 1) ){continue;} 
      else if( (region.find("2bex") != std::string::npos) && (bjets_n != 2) ){continue;} 
      else if( (region.find("3bex") != std::string::npos) && (bjets_n != 3) ){continue;} 

      else if( (region.find("0bin") != std::string::npos) && (bjets_n < 0) ){continue;} 
      else if( (region.find("1bin") != std::string::npos) && (bjets_n < 1) ){continue;} 
      else if( (region.find("2bin") != std::string::npos) && (bjets_n < 2) ){continue;} 
      else if( (region.find("3bin") != std::string::npos) && (bjets_n < 3) ){continue;} 
      else if( (region.find("4bin") != std::string::npos) && (bjets_n < 4) ){continue;} 
 
      //------------------------------------------------------------------------------------------
      if(region.find("metmtwinc") != std::string::npos){ 
	if( (met<20.) || (met+mtw < 60.) ){continue;}
      }

     //--------------------------------------------------------------------------------------------
      if( region.find("period2015") != std::string::npos ){ 
	if(data_period != 1){ continue; }
      }
      else if( region.find("period2016") != std::string::npos ){ 
	if(data_period != 2){ continue; }
      }
      //---------------------------------------------------------

      if( region.find("lowmu") != std::string::npos ){ 
	if(pileup_mu >= 22.){ continue; }
      }
      if( region.find("highmu") != std::string::npos ){ 
	if(pileup_mu < 22.){ continue; }
      }
      /*
      m_hstMngr->FillTH1D(region+"_"+"el_loose_n", el_loose_n, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"mu_loose_n", mu_loose_n, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"el_loose_n", el_loose_n+mu_loose_n, nomWeight);
      */
      for( int ifj = 0; ifj < fjets_n; ifj++){
	if(ifj == 0){
	  m_hstMngr->FillTH1D(region+"_"+"fjet1_mass", fjets_m->at(ifj), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"fjet1_pt", fjets_pt->at(ifj), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"fjet1_eta", fjets_eta->at(ifj), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"fjet1_phi", fjets_phi->at(ifj), nomWeight);
	}
	m_hstMngr->FillTH1D(region+"_"+"fjets_mass", fjets_m->at(ifj), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"fjets_pt", fjets_pt->at(ifj), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"fjets_eta", fjets_eta->at(ifj), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"fjets_phi", fjets_phi->at(ifj), nomWeight);
      }

      for( int irc = 0; irc < rcjets_n; irc++){
	if(irc == 0){
	  m_hstMngr->FillTH1D(region+"_"+"rcjet1_mass", rcjets_m->at(irc), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"rcjet1_pt", rcjets_pt->at(irc), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"rcjet1_eta", rcjets_eta->at(irc), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"rcjet1_phi", rcjets_phi->at(irc), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"rcjet1_nconsts", rcjets_nconsts->at(irc), nomWeight);
	}
	else if(irc == 1){
	  m_hstMngr->FillTH1D(region+"_"+"rcjet2_mass", rcjets_m->at(irc), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"rcjet2_pt", rcjets_pt->at(irc), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"rcjet2_eta", rcjets_eta->at(irc), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"rcjet2_phi", rcjets_phi->at(irc), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"rcjet2_nconsts", rcjets_nconsts->at(irc), nomWeight);
	}

	m_hstMngr->FillTH1D(region+"_"+"rcjets_mass", rcjets_m->at(irc), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"rcjets_pt", rcjets_pt->at(irc), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"rcjets_eta", rcjets_eta->at(irc), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"rcjets_phi", rcjets_phi->at(irc), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"rcjets_nconsts", rcjets_nconsts->at(irc), nomWeight);
	
      }//reclustered jet loop
      
      int nbj = 0; int nlj = 0; 

      int i1_mindr_bb = -1; int i2_mindr_bb = -1; double mindr_bb = 10.;
      int i1_mindr_jj = -1; int i2_mindr_jj = -1; double mindr_jj = 10.;

      for( int ij = 0; ij < jets_n; ij++){

	TLorentzVector tlv_j;
	tlv_j.SetPtEtaPhiM(jets_pt->at(ij), jets_eta->at(ij), jets_phi->at(ij), jets_m->at(ij));
	double dRmin_jj = 10.; double dRmin_rc = 10.;

	for(int irc = 0; irc < rcjets_n; irc++){
	  TLorentzVector tlv_rc;
	  tlv_rc.SetPtEtaPhiM(rcjets_pt->at(irc), rcjets_eta->at(irc), rcjets_phi->at(irc), rcjets_m->at(irc));
	  double dr_jrc = tlv_rc.DeltaR(tlv_j);
	  if(dr_jrc < dRmin_rc) dRmin_rc = dr_jrc;
	}
	jets_drmin_rc->push_back(dRmin_rc);

	for( int jj = 0; jj < jets_n; jj++){
	  if(ij == jj) continue;
	  TLorentzVector tlv_jalt;
	  tlv_jalt.SetPtEtaPhiM(jets_pt->at(jj), jets_eta->at(jj), jets_phi->at(jj), jets_m->at(jj));
	  double dr_jj = tlv_jalt.DeltaR(tlv_j);
	  if(dr_jj < dRmin_jj) dRmin_jj = dr_jj;
	  if(dr_jj < mindr_jj){
	    mindr_jj = dr_jj;
	    i1_mindr_jj = ij; i2_mindr_jj = jj;
	  }

	  if( (jets_btagw->at(ij) > 0.645925) && (jets_btagw->at(jj) > 0.645925) ){
	    if(dr_jj < mindr_bb){
	      mindr_bb = dr_jj;
	      i1_mindr_bb = ij; i2_mindr_bb = jj;
	    }
	  }
	}//second jet loop

	m_hstMngr->FillTH1D(region+"_"+"jets_pt", jets_pt->at(ij), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jets_mass", jets_m->at(ij), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jets_eta", jets_eta->at(ij), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jets_phi", jets_phi->at(ij), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jets_btagw", jets_btagw->at(ij), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jets_jvt", jets_jvt->at(ij), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jets_dRmin_j", dRmin_jj, nomWeight);

	if(ij == 0){
	  m_hstMngr->FillTH1D(region+"_"+"jet1_pt", jets_pt->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"jet1_mass", jets_m->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"jet1_eta", jets_eta->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"jet1_phi", jets_phi->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"jet1_btagw", jets_btagw->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"jet1_jvt", jets_jvt->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"jet1_dRmin_j", dRmin_jj, nomWeight);
	}

	if(jets_btagw->at(ij) > 0.645925){
	  m_hstMngr->FillTH1D(region+"_"+"bjets_pt", jets_pt->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"bjets_mass", jets_m->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"bjets_eta", jets_eta->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"bjets_phi", jets_phi->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"bjets_btagw", jets_btagw->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"bjets_jvt", jets_jvt->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"bjets_dRmin_j", dRmin_jj, nomWeight);

	  if(nbj==0){
	    m_hstMngr->FillTH1D(region+"_"+"bjet1_pt", jets_pt->at(ij), nomWeight);
	    m_hstMngr->FillTH1D(region+"_"+"bjet1_mass", jets_m->at(ij), nomWeight);
	    m_hstMngr->FillTH1D(region+"_"+"bjet1_eta", jets_eta->at(ij), nomWeight);
	    m_hstMngr->FillTH1D(region+"_"+"bjet1_phi", jets_phi->at(ij), nomWeight);
	    m_hstMngr->FillTH1D(region+"_"+"bjet1_btagw", jets_btagw->at(ij), nomWeight);
	    m_hstMngr->FillTH1D(region+"_"+"bjet1_jvt", jets_jvt->at(ij), nomWeight);
	    m_hstMngr->FillTH1D(region+"_"+"bjet1_dRmin_j", dRmin_jj, nomWeight);
	  }
	  nbj++;
	}//bjet
	else{
	  m_hstMngr->FillTH1D(region+"_"+"ljets_pt", jets_pt->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"ljets_mass", jets_m->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"ljets_eta", jets_eta->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"ljets_phi", jets_phi->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"ljets_btagw", jets_btagw->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"ljets_jvt", jets_jvt->at(ij), nomWeight);
	  m_hstMngr->FillTH1D(region+"_"+"ljets_dRmin_j", dRmin_jj, nomWeight);

	  if(nbj==0){
	    m_hstMngr->FillTH1D(region+"_"+"ljet1_pt", jets_pt->at(ij), nomWeight);
	    m_hstMngr->FillTH1D(region+"_"+"ljet1_mass", jets_m->at(ij), nomWeight);
	    m_hstMngr->FillTH1D(region+"_"+"ljet1_eta", jets_eta->at(ij), nomWeight);
	    m_hstMngr->FillTH1D(region+"_"+"ljet1_phi", jets_phi->at(ij), nomWeight);
	    m_hstMngr->FillTH1D(region+"_"+"ljet1_btagw", jets_btagw->at(ij), nomWeight);
	    m_hstMngr->FillTH1D(region+"_"+"ljet1_jvt", jets_jvt->at(ij), nomWeight);
	    m_hstMngr->FillTH1D(region+"_"+"ljet1_dRmin_j", dRmin_jj, nomWeight);
	  }
	  nlj++;
	}//ljet

      }//jet loop
      m_hstMngr->FillTH1D(region+"_"+"dPhi_jetmet", dPhi_jetmet, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"dRmin_jetjet", dRmin_jetjet, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"dRmin_bb", dRmin_bb, nomWeight);

      if( (i1_mindr_bb >= 0) && (i2_mindr_bb >= 0) ){
	TLorentzVector tlv_b1;
	tlv_b1.SetPtEtaPhiM(jets_pt->at(i1_mindr_bb), jets_eta->at(i1_mindr_bb), jets_phi->at(i1_mindr_bb), jets_m->at(i1_mindr_bb));
	TLorentzVector tlv_b2;
	tlv_b2.SetPtEtaPhiM(jets_pt->at(i2_mindr_bb), jets_eta->at(i2_mindr_bb), jets_phi->at(i2_mindr_bb), jets_m->at(i2_mindr_bb));
	TLorentzVector tlv_bb_sys = tlv_b1 + tlv_b2;

	m_hstMngr->FillTH1D(region+"_"+"ptbb_mindr", tlv_bb_sys.Pt(), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"etabb_mindr", tlv_bb_sys.Eta(), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"phibb_mindr", tlv_bb_sys.Phi(), nomWeight);
      }

      if(i1_mindr_bb >= 0){
	m_hstMngr->FillTH1D(region+"_"+"bjet1_mindr_pt", jets_pt->at(i1_mindr_bb), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"bjet1_mindr_mass", jets_m->at(i1_mindr_bb), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"bjet1_mindr_eta", jets_eta->at(i1_mindr_bb), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"bjet1_mindr_phi", jets_phi->at(i1_mindr_bb), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"bjet1_mindr_dR_rc", jets_drmin_rc->at(i1_mindr_bb), nomWeight);
      }
      if(i2_mindr_bb >= 0){
	m_hstMngr->FillTH1D(region+"_"+"bjet2_mindr_pt", jets_pt->at(i2_mindr_bb), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"bjet2_mindr_mass", jets_m->at(i2_mindr_bb), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"bjet2_mindr_eta", jets_eta->at(i2_mindr_bb), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"bjet2_mindr_phi", jets_phi->at(i2_mindr_bb), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"bjet2_mindr_dR_rc", jets_drmin_rc->at(i2_mindr_bb), nomWeight);
      }
      if(i1_mindr_jj >= 0){
	m_hstMngr->FillTH1D(region+"_"+"jet1_mindr_pt", jets_pt->at(i1_mindr_jj), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jet1_mindr_mass", jets_m->at(i1_mindr_jj), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jet1_mindr_eta", jets_eta->at(i1_mindr_jj), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jet1_mindr_phi", jets_phi->at(i1_mindr_jj), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jet1_mindr_dR_rc", jets_drmin_rc->at(i1_mindr_jj), nomWeight);
      }
      if(i2_mindr_jj >= 0){
	m_hstMngr->FillTH1D(region+"_"+"jet2_mindr_pt", jets_pt->at(i2_mindr_jj), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jet2_mindr_mass", jets_m->at(i2_mindr_jj), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jet2_mindr_eta", jets_eta->at(i2_mindr_jj), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jet2_mindr_phi", jets_phi->at(i2_mindr_jj), nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"jet2_mindr_dR_rc", jets_drmin_rc->at(i2_mindr_jj), nomWeight);
      }

      if(el_n + mu_n > 0){
	m_hstMngr->FillTH1D(region+"_"+"dPhi_lepbjet", dPhi_lepbjet, nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"dPhi_lepjet", dPhi_lepjet, nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"dPhi_lepmet", dPhi_lepmet, nomWeight);
      }
      if(el_n > 0){
	m_hstMngr->FillTH1D(region+"_"+"dRmin_ebjets", dRmin_ebjets, nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"dRmin_ejets", dRmin_ejets, nomWeight);
      }

      if(mu_n > 0){
	m_hstMngr->FillTH1D(region+"_"+"dRmin_mubjets", dRmin_mubjets, nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"dRmin_mujets", dRmin_mujets, nomWeight);

      }

      m_hstMngr->FillTH1D(region+"_"+"mT_bmin", mT_bmin, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"mbb_mindr", mbb_mindr, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"meff", meff, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"hthad", hthad, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"met", met, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"met_phi", met_phi, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"mtw", mtw, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"el1_pt", el1_pt, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"el1_eta", el1_eta, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"el1_phi", el1_phi, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"mu1_pt", mu1_pt, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"mu1_eta", mu1_eta, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"mu1_phi", mu1_phi, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"npv", npv, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"pileup_mu", pileup_mu, nomWeight);

      m_hstMngr->FillTH1D(region+"_"+"fjets_n", fjets_n, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"rcjets_n", rcjets_n, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"jets_n", jets_n, nomWeight);
      m_hstMngr->FillTH1D(region+"_"+"bjets_n", bjets_n, nomWeight);

      if(dolowb){
	m_hstMngr->FillTH1D(region+"_"+"dRmin_bb_lowb_3b", dRmin_bb_lowb_3b, nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"mT_bmin_lowb_3b", mT_bmin_lowb_3b, nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"mbb_mindr_lowb_3b", mbb_mindr_lowb_3b, nomWeight);

	m_hstMngr->FillTH1D(region+"_"+"dRmin_bb_lowb_4b", dRmin_bb_lowb_4b, nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"mT_bmin_lowb_4b", mT_bmin_lowb_4b, nomWeight);
	m_hstMngr->FillTH1D(region+"_"+"mbb_mindr_lowb_4b", mbb_mindr_lowb_4b, nomWeight);
      }
    }// Regions
    
  }//Event loop

  TFile* outfile = TFile::Open(argv[2], "RECREATE");
  outfile->cd(); 
  for( const std::string hist : m_hstMngr->GetTH1KeyList() ){
    m_hstMngr->FinaliseTH1Bins(hist); 
    m_hstMngr->GetTH1D(hist)->Write();
  }

  outfile->Close();
  delete outfile;
  
  m_hstMngr->ClearAllTH1();
  m_hstMngr->ClearAllTH2();
  delete m_hstMngr;
  delete fChain; 
  
  return 0;
  
}
  
  
