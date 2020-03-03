#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "TFile.h"
#include "TH1.h"

int main(int argc, char** argv){


  std::string inDir="";
  std::string outName="";
  bool onlyRwTt=false;
  bool splitEMu=false;

  int optind = 1;
  std::stringstream ss;
  std::string _switch = "";

  while ((optind < argc) && (argv[optind][0]=='-')) {
    _switch = std::string(argv[optind]);
    optind++;  ss << argv[optind];

    if       (_switch == "--inDir")     { ss >> inDir; } //Name of input directory containing files
    else if  (_switch == "--outName")   { ss >> outName; }   //Name of output reweighting file
    else if  (_switch == "--onlyRwTt")  { ss >> onlyRwTt; }    //Only reweight ttbar background
    else if  (_switch == "--splitEMu")  { ss >> splitEMu; }  //Split into electron and muon channels

    else std::cout<<"Unknown switch "<<_switch<<std::endl;

    optind++; ss.clear(); _switch.clear();
  }

  std::cout << " inDir = " << inDir << std::endl;
  std::cout << " outName = " << outName << std::endl;
  std::cout << " onlyRwTt = " << onlyRwTt << std::endl;
  std::cout << " splitEMu = " << splitEMu << std::endl;

  TFile* f_out = TFile::Open( outName.c_str(), "UPDATE" );

  std::vector<std::string> list_tt = {"ttbarlight", "ttbarcc", "ttbarbb"};
  //std::vector<std::string> list_nontt = {"Singletop", "topEW", "ttH", "Wjetslight", "Wjetscharm", "Wjetsbeauty", "Zjetslight", "Zjetscharm", "Zjetsbeauty", "Dibosons", "SM4tops", "QCD", "QCD0L"};
  std::vector<std::string> list_nontt = {"Singletop", "topEW", "Wjets", "Zjets", "others"};

  std::vector<std::string> list_reg = {"c1lep5jin2bin", "c0lep6jin2bin", "c1lep5jin2bex", "c1lep5jin3bin", "c0lep6jin3bin"};
  if(splitEMu){
    list_reg.push_back("c1lep5jin2bin_el");
    list_reg.push_back("c1lep5jin2bin_mu");
  }

  std::vector<std::string> list_kin = {"met", "jet0_pt", "lep0_pt"};

  double lumi = 36074.56;

  double binedges_met[20] = {0,20,40,60,80,100,120,140,160,180,200,240,280,320,360,400,500,600,700,1000}; 
  double *rebin_met = &binedges_met[0];
  int nbins_met = 19;

  double binedges_jetpt[16] = {0,50,100,150,200,250,300,350,400,450,500,550,600,700,800,1000};
  double* rebin_jetpt = &binedges_jetpt[0];
  int nbins_jetpt = 15;
  
  double binedges_leppt[15]= {0,50,100,150,200,250,300,350,400,450,500,550,600,700,800};
  double* rebin_leppt = &binedges_leppt[0];
  int nbins_leppt = 14;

  for(const std::string& kinematic : list_kin){

    //Set rebinning to the same values as that for preselection plots
    double *rebin = 0;
    int nbins = 0;
    if(kinematic == "met"){
      rebin = rebin_met;
      nbins = nbins_met;
    }
    else if(kinematic == "jet0_pt"){
      rebin = rebin_jetpt;
      nbins = nbins_jetpt;
    }
    else if(kinematic == "lep0_pt"){
      rebin = rebin_leppt;
      nbins = nbins_leppt;
    }

    for(const std::string& region : list_reg){
      std::string subdir = "";
      if(region.find("c1lep") != std::string::npos){ subdir = "1lep"; }
      else if(region.find("c0lep") != std::string::npos){ subdir = "0lep"; }

      TFile* f_data = TFile::Open( (inDir + "/" + subdir + "/Data.root").c_str(), "READ" );
      TH1D* data_hist = (TH1D*)(f_data -> Get( (region + "_" + kinematic).c_str() ) );
      data_hist->SetDirectory(0);
      f_data->Close();
      data_hist = (TH1D*)(data_hist->Rebin(nbins, data_hist->GetName(), rebin));

      TFile* f_qcd = TFile::Open( (inDir + "/" + subdir + "/QCD.root").c_str(), "READ" );
      TH1D* qcd_hist = (TH1D*)(f_qcd -> Get ( (region + "_" + kinematic).c_str() ) );
      qcd_hist->SetDirectory(0);
      f_qcd->Close();
      qcd_hist = (TH1D*)(qcd_hist->Rebin(nbins, qcd_hist->GetName(), rebin));


      TH1D* nontt_hist = (TH1D*)(data_hist->Clone( Form("%s_nontt",data_hist->GetName()) ));
      nontt_hist->Reset();


      for(const std::string& bkg : list_nontt){
	/*
	if( (region.find("c1lep") != std::string::npos && bkg == "QCD0L") 
	    || (region.find("c0lep") != std::string::npos && bkg == "QCD") ){ continue; }
	*/
	TFile* f_nontt = TFile::Open( (inDir + "/" + subdir + "/" + bkg + ".root").c_str(), "READ" );
	TH1D* h_nontt_i = ((TH1D*)(f_nontt -> Get( (region + "_" + kinematic).c_str() )) );

	h_nontt_i->SetDirectory(0);
	h_nontt_i = (TH1D*)(h_nontt_i->Rebin(nbins, h_nontt_i->GetName(), rebin));

	h_nontt_i->Scale(lumi);
	nontt_hist -> Add ( h_nontt_i );
	delete h_nontt_i;
	f_nontt->Close();
      }

      TH1D* tt_hist = (TH1D*)(data_hist->Clone( Form("%s_tt",data_hist->GetName()) ));
      tt_hist->Reset();
      for(const std::string& bkg : list_tt){
	TFile* f_tt = TFile::Open( (inDir + "/" + subdir + "/"  + bkg + ".root").c_str() );
	TH1D* h_tt_i = (TH1D*)(f_tt -> Get( (region + "_" + kinematic).c_str() ));
	h_tt_i->SetDirectory(0);
	h_tt_i = (TH1D*)(h_tt_i->Rebin(nbins, h_tt_i->GetName(), rebin));
	h_tt_i->Scale(lumi);
	tt_hist -> Add ( h_tt_i );
	delete h_tt_i;
	f_tt->Close();
      }

      data_hist -> Add ( qcd_hist, -1.);
      if(onlyRwTt){ // (Data - nontt)/tt
	data_hist -> Add ( nontt_hist, -1.);
	data_hist -> Divide ( tt_hist );
      }
      else{ // (Data - QCD)/nonQCD
	tt_hist -> Add (nontt_hist);
	data_hist -> Divide( tt_hist );
      }

      f_out->cd();
      data_hist->Write();

    }//region loop

  }//kinematics loop


  f_out->Close();

  return 0;

}
