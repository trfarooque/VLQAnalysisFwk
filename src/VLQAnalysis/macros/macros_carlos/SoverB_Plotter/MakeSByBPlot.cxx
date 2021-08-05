#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TH1D.h"
#include "THStack.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "Rtypes.h"
#include "TPaveText.h"
#include "TLine.h"

#include "IFAETopFramework/AnalysisUtils.h"

// Structure to store the information for each signal sample
//__________________________________________________________________________________________________
//
struct SigInfo{
  
  std::string key;
  std::string label;
  Color_t color;
  std::string infile_mc16a;
  std::string infile_mc16d;
  std::string infile_mc16e;
  std::string bkg_file_mc16a;
  std::string bkg_file_mc16d;
  std::string bkg_file_mc16e;
  std::string sig_suffix;

  SigInfo(const std::string& _key, const std::string& _label, std::string& _color, 
	  std::string& _infile_mc16a, std::string& _infile_mc16d, std::string& _infile_mc16e, 
	  std::string& _bkg_file_mc16a, std::string& _bkg_file_mc16d, std::string& _bkg_file_mc16e,
	  std::string& _sig_suffix){

    key = _key;
    label = _label;
    infile_mc16a = _infile_mc16a;
    infile_mc16d = _infile_mc16d;
    infile_mc16e = _infile_mc16e;
    bkg_file_mc16a = _bkg_file_mc16a;
    bkg_file_mc16d = _bkg_file_mc16d;
    bkg_file_mc16e = _bkg_file_mc16e;
    sig_suffix = _sig_suffix;
    
    std::transform(_color.begin(), _color.end(), _color.begin(), toupper);

    if(_color == "RED") color = kRed+1;
    else if(_color == "BLUE") color = kBlue+1;
    else if(_color == "GREEN") color = kGreen+2;

  }

};

// Structure to store the information for a given search region
//__________________________________________________________________________________________________
//
struct RegInfo{

  std::string name;
  std::string binlabel;
  std::map<std::string, double> list_sbyb;
  std::map<std::string, double> list_sbysqb;
  std::map<std::string, double> list_sigStatUnc;
  std::map<std::string, double> list_bkgdStatUnc;

  RegInfo( const std::string& _name, const std::string& _binlabel,
           const std::map<std::string, double>& _list_sbyb,
           const std::map<std::string, double>& _list_sbysqb,
           const std::map<std::string, double>& _list_sigStatUnc,
           const std::map<std::string, double>& _list_bkgdStatUnc){
    name = _name;
    binlabel = _binlabel;
    list_sbyb = _list_sbyb;
    list_sbysqb = _list_sbysqb;
    list_sigStatUnc = _list_sigStatUnc;
    list_bkgdStatUnc = _list_bkgdStatUnc;
  }

};

// Function to fill a SigInfo vector from an input configuration file
//__________________________________________________________________________________________________
//
void fillSigInfoVector(std::vector< SigInfo* >& siglist, std::string fileName, char delim){

  std::cout << "Opening file " << fileName << std::endl;

  std::ifstream file(fileName);

  if(!file.is_open()){

    std::cout << "< ERROR > : The file " << fileName << "does not exist! Exiting from program." << std::endl;

    exit(EXIT_FAILURE);

  }

  std::string line;

  while(getline(file, line)){

    std::string key = "";
    std::string label = "";
    std::string color = "";
    std::string infile_mc16a= "";
    std::string infile_mc16d = "";
    std::string infile_mc16e = "";
    std::string bkg_file_mc16a = "";
    std::string bkg_file_mc16d = "";
    std::string bkg_file_mc16e = "";
    std::string sig_suffix = "";

    if(line == "NEW"){

      std::cout << "#####################################" << std::endl;
      std::cout << "#     Found signal sample info      #" << std::endl;
      std::cout << "#####################################" << std::endl;
            
      while(getline(file,line) && line != "END"){

	std::string opt = line.substr(0, line.find(delim));

	std::string val = line.substr(line.find(delim)+1);

	std::transform(opt.begin(), opt.end(), opt.begin(), toupper);

	std::cout << opt << " : " << val <<std::endl;

	if(opt == "NAME") key = val;
	else if(opt == "LABEL") label = val;
	else if(opt == "MC16A_FILE") infile_mc16a = val;
	else if(opt == "MC16D_FILE") infile_mc16d = val;
	else if(opt == "MC16E_FILE") infile_mc16e = val;
	else if(opt == "BKG_MC16A_FILE") bkg_file_mc16a = val;
        else if(opt == "BKG_MC16D_FILE") bkg_file_mc16d = val;
        else if(opt == "BKG_MC16E_FILE") bkg_file_mc16e = val;
	else if(opt == "SIG_SUFFIX") sig_suffix = val;
	else if(opt == "COLOR") color = val;
      }

      siglist.push_back( new SigInfo(key, label, color, infile_mc16a, infile_mc16d, infile_mc16e, bkg_file_mc16a, bkg_file_mc16d, bkg_file_mc16e, sig_suffix));
      
    }
   
  }

  std::cout << "#####################################" << std::endl;
  std::cout << "Closing file "  << fileName << std::endl;
  std::cout << "#####################################" << std::endl;
  
  file.close();

}

// Function that determines the b-tagging label to be used for a given search region
//__________________________________________________________________________________________________
//
std::string getBTagLabel(const std::string& btitle){

  std::string blabel = "";

  if(btitle == "2bex"){ blabel = "2b"; }
  else if(btitle == "2bexHighMtbmin"){ blabel = "2b,HM"; }
  else if(btitle == "3bex"){ blabel = "3b"; }
  else if(btitle == "3bin"){ blabel = "#geq3b";}
  else if(btitle == "3bexLowMtbmin"){ blabel = "3b,LM"; }
  else if(btitle == "3bexHighMtbmin"){ blabel = "3b,HM"; }
  else if(btitle == "4bin"){ blabel = "#geq4b"; }
  else if(btitle == "4binLowMtbmin"){ blabel = "#geq4b,LM"; }
  else if(btitle == "4binHighMtbmin"){ blabel = "#geq4b,HM"; }
  else{ std::cout << "WARNING: b-tagging title " << btitle << " not recognized." << std::endl; }

  return blabel;

}

// Function that determines the tagged boosted objet label to be used for a given search region
//__________________________________________________________________________________________________
//
std::string getBoostLabel(const std::string& boosttitle){

  std::string boostlabel = "";
  
  // New analysis regions
  if( boosttitle == "0Hex0Vin0LTex") {boostlabel = "0H, #geq0V, 0LT, 0T"; }
  else if( boosttitle == "0Hex0Vin1LTin") {boostlabel = "0H, #geq0V, #geq1(LT+T)"; }
  else if( boosttitle == "1Hex0Vex0LTex") {boostlabel = "1H, 0V, 0LT, 0T"; }
  else if( boosttitle == "1Hex0Vex1Lin0Tex") {boostlabel = "1H, 0V, #geq1LT, 0T"; }
  else if( boosttitle == "1Hex1Vin1Lin0Tex") {boostlabel = "1H, #geq1V, #geq1LT, 0T"; }
  else if( boosttitle == "1Hex1Vin0LTex") {boostlabel = "1H, #geq1V, 0LT, 0T"; }
  else if( boosttitle == "1Hex0Vin0Lex1Tin") {boostlabel = "1H, #geq0V, 0LT, #geq1T"; }
  else if( boosttitle == "1Hex0Vin1Lin1Tin") {boostlabel = "1H, #geq0V, #geq1LT, #geq1T"; }
  else if( boosttitle == "2Hin0Vin1Lin1Tin") {boostlabel = "#geq2H, #geq0V, #geq1LT, #geq1T"; }
  else if( boosttitle == "2Hin0Vin0Lex1Tin") {boostlabel = "#geq2H, #geq0V, 0LT, #geq1T"; }
  else if( boosttitle == "2Hin0Vin1Lin0Tex") {boostlabel = "#geq2H, #geq0V, #geq1LT, 0T"; }
  else if( boosttitle == "2Hin0Vin0LTex") {boostlabel = "#geq2H, #geq0V, 0LT, 0T"; }
  // New analysis merged regions                             
  else if( boosttitle == "1Hex1Vin0Lin0Tex") {boostlabel = "1H, #geq1V, #geq0LT, 0T";}
  else if( boosttitle == "2Hin0Vin1LTin") {boostlabel = "#geq2H, #geq0V, #geq1(LT+T)";}
  else if( boosttitle == "2Hin0Vin0Lin0Tin") {boostlabel = "#geq2H, #geq0V, #geq0LT, #geq0T";}
  // Old analysis regions                                                                                                                                                     
  else if( boosttitle == "0Tex0Hex") {boostlabel = "0T, 0H"; }
  else if( boosttitle == "0Tex1Hex") {boostlabel = "0T, 1H"; }
  else if( boosttitle == "1Tex0Hex") {boostlabel = "1T, 0H"; }
  else if( boosttitle == "1Tex1Hex") {boostlabel = "1T, 1H"; }
  else if( boosttitle == "2Tin0_1Hwin") {boostlabel = "#geq2T, 0-1H"; }
  else if( boosttitle == "0Tin2Hin") {boostlabel = "#geq0T, #geq2H"; }
  else{ std::cout << "WARNING: boosted object title " << boosttitle << " not recognized." << std::endl; }

  return boostlabel;

}

// Function that fills a vector of RegInfo
//__________________________________________________________________________________________________
//
void fillRegInfo(std::vector< RegInfo* >& reglist, std::vector< SigInfo* >& siglist, const std::string& lepTitle, const std::string& bTagTitle, std::vector< std::string >& boostTitles, std::string& variable, double varBin){

  std::string jetTitle = (lepTitle == "c1lep") ? "6jin" : "7jin";
  std::string jetLabel = (lepTitle == "c1lep") ? "#geq6j" : "#geq7j";

  std::string bTagLabel = getBTagLabel(bTagTitle);
  
  for(const std::string& boostTitle : boostTitles){

    std::string boostLabel = getBoostLabel(boostTitle);

    std::string name = "";

    if( (boostTitle == "0Tex0Hex") || (boostTitle == "0Tex1Hex") || (boostTitle == "1Tex0Hex") ||
	(boostTitle == "1Tex1Hex") || (boostTitle == "2Tin0_1Hwin") || (boostTitle == "0Tin2Hin")){
      name = lepTitle + boostTitle + jetTitle + bTagTitle + "_"  + variable;
    }
    else{
      name = lepTitle + jetTitle + bTagTitle + boostTitle + "_" + variable;
    }

    std::string binLabel = boostLabel;// + ", " + jetLabel + ", " + bTagLabel;

    std::map< std::string, double > reg_sbyb = {};
    std::map< std::string, double > reg_sbysqb = {};
    std::map< std::string, double > reg_sigStatUnc = {};
    std::map< std::string, double > reg_bkgStatUnc = {};

    std::cout << "Filling information for the region " << binLabel << std::endl;

    for(unsigned int i = 0; i < siglist.size(); i++){

      SigInfo* siginfo = siglist.at(i);

      TH1D* h_bkg_mc16a = NULL;
      TH1D* h_bkg_mc16d = NULL;
      TH1D* h_bkg_mc16e = NULL;
      
      if(siginfo->bkg_file_mc16a != ""){

	TFile* tmp_mc16a = TFile::Open((siginfo->bkg_file_mc16a).c_str(), "READ");
	
	h_bkg_mc16a = (TH1D*)(tmp_mc16a->Get(name.c_str()));
	
	h_bkg_mc16a->Scale(36207.66);

      }
      if(siginfo->bkg_file_mc16d != ""){

	TFile* tmp_mc16d = TFile::Open((siginfo->bkg_file_mc16d).c_str(), "READ");
	
	h_bkg_mc16d = (TH1D*)(tmp_mc16d->Get(name.c_str()));
	
	h_bkg_mc16d->Scale(44037.4);

	h_bkg_mc16a->Add(h_bkg_mc16d);

      }
      if(siginfo->bkg_file_mc16e != ""){

	TFile* tmp_mc16e = TFile::Open((siginfo->bkg_file_mc16e).c_str(), "READ");

        h_bkg_mc16e = (TH1D*)(tmp_mc16e->Get(name.c_str()));

        h_bkg_mc16e->Scale(58450.1);

	h_bkg_mc16a->Add(h_bkg_mc16e);

      }
      
      std::string sigName = name;

      if(siginfo->sig_suffix != ""){
	
	size_t pos = sigName.find(variable);
     
	std::string prefix = sigName.substr(0, pos);

	sigName = prefix+(siginfo->sig_suffix)+"_"+variable;

      }

      TH1D* h_sig_mc16a = NULL;
      TH1D* h_sig_mc16d = NULL;
      TH1D* h_sig_mc16e = NULL;

      if(siginfo->infile_mc16a != ""){

        TFile* tmp_mc16a = TFile::Open((siginfo->infile_mc16a).c_str(), "READ");

        h_sig_mc16a = (TH1D*)(tmp_mc16a->Get(sigName.c_str()));

        h_sig_mc16a->Scale(36207.66);

      }
      if(siginfo->infile_mc16d != ""){

	TFile* tmp_mc16d = TFile::Open((siginfo->infile_mc16d).c_str(), "READ");

        h_sig_mc16d = (TH1D*)(tmp_mc16d->Get(sigName.c_str()));

        h_sig_mc16d->Scale(44037.4);

	h_sig_mc16a->Add(h_sig_mc16d);

      }
      if(siginfo->infile_mc16e != ""){

	TFile* tmp_mc16e = TFile::Open((siginfo->infile_mc16e).c_str(), "READ");

        h_sig_mc16e = (TH1D*)(tmp_mc16e->Get(sigName.c_str()));

        h_sig_mc16e->Scale(58450.1);
	
	h_sig_mc16a->Add(h_sig_mc16e);

      }

      int bin_1TeV = h_bkg_mc16a->FindBin(varBin);
      double bkgStatUnc = 0;
      double bkgYield = h_bkg_mc16a->IntegralAndError(bin_1TeV, -1, bkgStatUnc);

      double sigStatUnc = 0;
      double sigYield = h_sig_mc16a->IntegralAndError(bin_1TeV, -1, sigStatUnc);

      double SbyB = (sigYield > 0.) ? sigYield/bkgYield : 0.;
      double SbySqB = (sigYield > 0.) ? sigYield/sqrt(bkgYield): 0.;

      reg_sbyb.insert(std::pair<std::string, double>(siginfo->key, SbyB));
      reg_sbysqb.insert(std::pair<std::string, double>(siginfo->key, SbySqB));
      reg_sigStatUnc.insert(std::pair<std::string, double>(siginfo->key, sigStatUnc));
      reg_bkgStatUnc.insert(std::pair<std::string, double>(siginfo->key, bkgStatUnc));


    }

    reglist.push_back(new RegInfo(name, binLabel, reg_sbyb, reg_sbysqb, reg_sigStatUnc, reg_bkgStatUnc));

  }

}

//__________________________________________________________________________________________________
//
TStyle* AtlasStyle(){

  TStyle *atlasStyle = new TStyle("ATLAS","Atlas style");

  Int_t icol=0; // WHITE                                                                                                                                                  
  atlasStyle->SetFrameBorderMode(icol);
  atlasStyle->SetFrameFillColor(icol);
  atlasStyle->SetCanvasBorderMode(icol);
  atlasStyle->SetCanvasColor(icol);
  atlasStyle->SetPadBorderMode(icol);
  atlasStyle->SetPadColor(icol);
  atlasStyle->SetStatColor(icol);
  atlasStyle->SetHistLineWidth(2.);
  atlasStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes                                                                    
  atlasStyle->SetEndErrorSize(0.);

  return atlasStyle;

}

//__________________________________________________________________________________________________
//
void SetAtlasStyle(){

  static TStyle* atlasStyle = 0;
  std::cout << "\nApplying ATLAS style settings...\n" << std::endl ;
  if ( atlasStyle==0 ) atlasStyle = AtlasStyle();
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();

}

//__________________________________________________________________________________________________ 
//
void MakePlot(std::vector< RegInfo* >& list_reg, std::vector< SigInfo* >& list_sig, std::string out_name, std::string yTitleUp, std::string yTitleDown, std::string preselection){

  int nbins = list_reg.size();
  double stretch = 1.37;
  double stretch_log = 1.E3;
  
  THStack hs_sbyb_hist;
  THStack hs_sbysqb_hist;

  std::vector< TH1D* > h_sbyb_err;
  std::vector< TH1D* > h_sbysqb_err;
  

  TLegend* leg_sbysqb = new TLegend(0.65, 0.6, 0.85, 0.85);
  
  leg_sbysqb->SetFillColor(0);
  leg_sbysqb->SetFillStyle(0);
  leg_sbysqb->SetLineColor(0);
  leg_sbysqb->SetLineStyle(0);
  leg_sbysqb->SetLineWidth(0);                             
  leg_sbysqb->SetBorderSize(0);
  leg_sbysqb->SetTextSize(0.060);
  leg_sbysqb->SetTextFont(42);
  leg_sbysqb->SetShadowColor(0);

  int indexCounter = 0;

  for(SigInfo* siginfo : list_sig){

    TH1D* h_sbyb = new TH1D(("h_sbyb_"+out_name+"_"+siginfo->key).c_str(), "", nbins, 0, nbins);
    TH1D* h_sbysqb = new TH1D(("h_sbysqb_"+out_name+"_"+siginfo->key).c_str(), "", nbins, 0, nbins);

    h_sbyb_err.push_back(new TH1D(("h_sbyb_err_"+out_name+"_"+siginfo->key).c_str(), "", nbins, 0, nbins));
    h_sbysqb_err.push_back(new TH1D(("h_sbysqb_err_"+out_name+"_"+siginfo->key).c_str(), "", nbins, 0, nbins));
    
    for(int ibin = 1; ibin <= nbins; ibin++){

      RegInfo* reg = list_reg.at(ibin-1);

      h_sbyb->SetBinContent(ibin, reg->list_sbyb[siginfo->key]);
      h_sbyb->SetBinError(ibin, (reg->list_sigStatUnc[siginfo->key])/(reg->list_bkgdStatUnc[siginfo->key]));

      h_sbysqb->SetBinContent(ibin, reg->list_sbysqb[siginfo->key]);
      h_sbysqb->SetBinError(ibin, (reg->list_sigStatUnc[siginfo->key])/sqrt(reg->list_bkgdStatUnc[siginfo->key]));

      h_sbyb->GetXaxis()->SetBinLabel(ibin, (reg->binlabel).c_str());
      //h_sbyb->GetXaxis()->ChangeLabel(ibin, 90., -1, -1, -1, -1, (reg->binlabel));

    }

    h_sbyb->SetLineColor(siginfo->color);
    h_sbysqb->SetLineColor(siginfo->color);

    h_sbyb->SetLineWidth(2);
    h_sbysqb->SetLineWidth(2);

    h_sbyb->SetLineStyle(2);
    h_sbysqb->SetLineStyle(2);

    h_sbyb_err.at(indexCounter)->Add(h_sbyb);
    h_sbysqb_err.at(indexCounter)->Add(h_sbysqb);

    indexCounter++;

    hs_sbyb_hist.Add( h_sbyb, "E0" );
    hs_sbysqb_hist.Add( h_sbysqb, "E0" );

    leg_sbysqb->AddEntry(h_sbysqb, (siginfo->label).c_str(), "f");

  }

  double orig_max = hs_sbysqb_hist.GetMaximum("nostack");
  //hs_sbyb_hist.GetHistogram()->GetXaxis()->LabelsOption("v");
  hs_sbyb_hist.SetMaximum(stretch*hs_sbyb_hist.GetMaximum("nostack") );
  hs_sbysqb_hist.SetMaximum(stretch*orig_max);

  TCanvas* canv = new TCanvas(out_name.c_str(), "", 1000, 700);
  canv->Divide(1,2);

  canv->cd(1)->SetPad(0,0.6,1,1);
  canv->cd(1)->SetTicks();
  canv->cd(1)->SetTopMargin(0.05);
  canv->cd(1)->SetBottomMargin(0.);
  canv->cd(1)->SetLeftMargin(0.14);
  //canv->cd(1)->SetRightMargin(0.05);
  canv->cd(1)->SetRightMargin(0.14);
  canv->cd(1)->SetFrameBorderMode(0);
  canv->cd(1)->SetFillStyle(4000);

  canv->cd(2)->SetPad(0,0,1,0.6);
  canv->cd(2)->SetTicks();
  canv->cd(2)->SetTopMargin(0.0);
  canv->cd(2)->SetBottomMargin(0.48);
  canv->cd(2)->SetLeftMargin(0.14);
  //canv->cd(2)->SetRightMargin(0.05);
  canv->cd(2)->SetRightMargin(0.14);
  canv->cd(2)->SetFrameFillStyle(4000);
  canv->cd(2)->SetFrameBorderMode(0);
  canv->cd(2)->SetFillStyle(4000);

  canv->cd(1);

  double ttl_xmin=0.18;
  double ttl_ymin=0.71;
  double ttl_xmax=0.38;
  double ttl_ymax=0.82;

  TPaveText* ttlbox = new TPaveText(ttl_xmin, ttl_ymin, ttl_xmax, ttl_ymax, "NBNDC");
  ttlbox->SetTextAlign(12); //horizontally centred; top-aligned                                                                                                               
  ttlbox->SetFillColor(0);
  ttlbox->SetFillStyle(0);
  ttlbox->SetLineColor(0);
  ttlbox->SetLineStyle(0);
  ttlbox->SetLineWidth(0);
  ttlbox->SetBorderSize(0);
  ttlbox->SetTextSize(0.09);
  ttlbox->SetTextFont(42);
  ttlbox->SetShadowColor(0);

  //ttlbox->AddText("#splitline{#scale[1.2]{#bf{#it{ATLAS}} Simulation}}{#sqrt{s} = 13 TeV, 139 fb^{-1}}");

  std::string textToAdd = "#splitline{#scale[1.2]{#bf{#it{ATLAS}} Simulation}}{#splitline{#sqrt{s} = 13 TeV, 139 fb^{-1}}{Preselection " + preselection + "}}";
  ttlbox->AddText(textToAdd.c_str());

  hs_sbysqb_hist.Draw("nostack");

  /*for(int nErr = 0; nErr < h_sbysqb_err.size(); nErr++){

    h_sbysqb_err.at(nErr)->SetLineWidth(2);
    h_sbysqb_err.at(nErr)->SetLineStyle(2);

    h_sbysqb_err.at(nErr)->SetLineColor(list_sig.at(nErr)->color);

    h_sbysqb_err.at(nErr)->Draw("same E");

    }*/

  leg_sbysqb->Draw();
  ttlbox->Draw();

  hs_sbysqb_hist.GetHistogram()->GetYaxis()->SetTitle(yTitleUp.c_str());
  hs_sbysqb_hist.GetHistogram()->GetYaxis()->SetLabelSize(0.07);
  hs_sbysqb_hist.GetHistogram()->GetYaxis()->SetTitleSize(0.09);
  hs_sbysqb_hist.GetHistogram()->GetYaxis()->SetTitleOffset(0.5);
  hs_sbysqb_hist.GetHistogram()->GetYaxis()->SetTickLength(0.009);

  hs_sbysqb_hist.GetHistogram()->GetYaxis()->CenterTitle();

  hs_sbysqb_hist.GetHistogram()->GetXaxis()->SetLabelSize(0.0);
  hs_sbysqb_hist.GetHistogram()->GetXaxis()->SetTitleSize(0.0);
  hs_sbysqb_hist.GetHistogram()->GetXaxis()->SetTickLength(0.009);

  TLine* lnref_sbysqb = new TLine(nbins, hs_sbysqb_hist.GetHistogram()->GetMinimum() , nbins, orig_max-0.5);
  lnref_sbysqb->Draw();

  double fracmax = 1. / stretch;

  //----------------------------

  canv->cd(1)->Update();
  canv->cd(1)->Modified();
  canv->cd(2);

  hs_sbyb_hist.Draw("nostack");

  /*for(int nErr = 0; nErr < h_sbyb_err.size(); nErr++){

    h_sbyb_err.at(nErr)->SetLineWidth(2);
    h_sbyb_err.at(nErr)->SetLineStyle(2);

    h_sbyb_err.at(nErr)->SetLineColor(list_sig.at(nErr)->color);

    h_sbyb_err.at(nErr)->Draw("same E");

    }*/

  hs_sbyb_hist.GetHistogram()->GetYaxis()->SetTitle(yTitleDown.c_str());
  hs_sbyb_hist.GetHistogram()->GetXaxis()->SetLabelSize(0.07);
  hs_sbyb_hist.GetHistogram()->GetXaxis()->SetLabelOffset(0.02);
  hs_sbyb_hist.GetHistogram()->GetYaxis()->SetLabelSize(0.05);
  hs_sbyb_hist.GetHistogram()->GetYaxis()->SetTitleSize(0.06);
  hs_sbyb_hist.GetHistogram()->GetYaxis()->SetTitleOffset(0.7);
  hs_sbyb_hist.GetHistogram()->GetYaxis()->CenterTitle();
  hs_sbyb_hist.GetHistogram()->GetXaxis()->SetTickLength(0.015);
  hs_sbyb_hist.GetHistogram()->GetYaxis()->SetTickLength(0.015);

  TLine* lnref_sbyb = new TLine(nbins, hs_sbyb_hist.GetHistogram()->GetMinimum() , nbins, hs_sbyb_hist.GetHistogram()->GetMaximum());
  lnref_sbyb->Draw();

  TLine line(0.01,1,0.2,1);
  line.SetLineColor(kWhite);
  line.SetLineWidth(25);
  line.DrawLineNDC(0.07,1,0.139,1);

  canv->cd(2)->SetLogy();

  canv->cd(2)->Update();
  canv->cd(2)->Modified();

  canv->cd();
  TPaveText* lowlabel = new TPaveText(0.124, 0.58, 0.139, 0.62, "NBNDC");
  lowlabel->SetTextAlign(12); //horizontally centred; top-aligned                                                                                                         
  lowlabel->SetFillColor(kWhite);
  lowlabel->SetFillStyle(1);
  lowlabel->SetLineColor(0);
  lowlabel->SetLineStyle(0);
  lowlabel->SetLineWidth(0);
  lowlabel->SetBorderSize(0);
  lowlabel->SetTextSize(0.032);
  lowlabel->SetTextFont(42);
  lowlabel->SetShadowColor(0);

  lowlabel->AddText("0");
  lowlabel->Draw();
  canv->cd()->Update();
  canv->cd()->Modified();

  canv->SaveAs(Form("%s.png", canv->GetName()));
  canv->SaveAs(Form("%s.pdf", canv->GetName()));
  canv->SaveAs(Form("%s.eps", canv->GetName()));



}

//__________________________________________________________________________________________________
//
int main(int argc, char** argv){

  std::string m_variable = "meff";
  std::string m_doOldAnalysis = "";
  double m_bin = 1000.;
  std::string m_sampleFile = "";
  std::string m_outName = "test.root";

  for(int i = 1; i < argc; i++){

    std::string opt(argv[i]);

    std::string argument;

    std::string value;

    size_t pos = opt.find("=");

    argument = opt.substr(0, pos);

    value = opt.erase(0, pos + 1);

    std::transform(argument.begin(), argument.end(), argument.begin(), toupper);

    if(argument == "--SAMPLEFILE") m_sampleFile = value;
    else if(argument == "--OUTNAME") m_outName = value;
    else if(argument == "--VARIABLE") m_variable = value;
    else if(argument == "--VARBIN") m_bin = std::stod(value);
    else if(argument == "--DOOLDREGIONS") m_doOldAnalysis = value;
    else{ std::cout << "WARNING: Option " << argument << " not recognized." << std::endl; }

  }

  std::vector< SigInfo* > m_siglist = {};

  fillSigInfoVector(m_siglist, m_sampleFile, ':');
  
  std::vector<std::string> oldAnalysis =
    {
      "0Tex0Hex", "0Tex1Hex", "1Tex0Hex", "1Tex1Hex", "2Tin0_1Hwin", "0Tin2Hin"
    };

  std::vector<std::string> fullboost =
    {
      "0Hex0Vin0LTex", "0Hex0Vin1LTin", "1Hex0Vex0LTex", "1Hex0Vex1Lin0Tex", "1Hex1Vin1Lin0Tex", "1Hex1Vin0LTex",
      "1Hex0Vin0Lex1Tin", "1Hex0Vin1Lin1Tin", "2Hin0Vin1Lin1Tin", "2Hin0Vin0Lex1Tin", "2Hin0Vin1Lin0Tex", "2Hin0Vin0LTex"
    };

  std::vector<std::string> fullboost_merged_v1 =
    {
      "0Hex0Vin0LTex", "0Hex0Vin1LTin", "1Hex0Vex0LTex", "1Hex0Vex1Lin0Tex", "1Hex1Vin0Lin0Tex",
      "1Hex0Vin0Lex1Tin", "1Hex0Vin1Lin1Tin", "2Hin0Vin1LTin", "2Hin0Vin0LTex"
    };

  std::vector<std::string> fullboost_merged_v2 =
    {
      "0Hex0Vin0LTex", "0Hex0Vin1LTin", "1Hex0Vex0LTex", "1Hex0Vex1Lin0Tex", "1Hex1Vin0Lin0Tex",
      "1Hex0Vin0Lex1Tin", "1Hex0Vin1Lin1Tin", "2Hin0Vin0Lin0Tin"
    };

  std::map<std::string, std::vector<std::string> > regions_1L_3bex ={};
  std::map<std::string, std::vector<std::string> > regions_1L_4bin ={};

  std::transform(m_doOldAnalysis.begin(), m_doOldAnalysis.end(), m_doOldAnalysis.begin(), toupper);

  if(m_doOldAnalysis  == "TRUE"){
    regions_1L_3bex.insert( std::pair<std::string, std::vector<std::string> >("3bex",oldAnalysis));
    regions_1L_4bin.insert( std::pair<std::string, std::vector<std::string> >("4bin",oldAnalysis));
  }
  else{
    regions_1L_3bex.insert( std::pair<std::string, std::vector<std::string> >("3bex",fullboost_merged_v1));
    regions_1L_4bin.insert( std::pair<std::string, std::vector<std::string> >("4bin",fullboost_merged_v2));
    //regions_1L_3bex.insert( std::pair<std::string, std::vector<std::string> >("3bex",fullboost_merged_v1)); 
    //regions_1L_4bin.insert( std::pair<std::string, std::vector<std::string> >("4bin",fullboost_merged_v1));                                                               
  }

  std::vector<RegInfo*> list_reg1L_3bex{};
  std::vector<RegInfo*> list_reg1L_4bin{};

  fillRegInfo(list_reg1L_3bex, m_siglist, "c1lep", "3bex", regions_1L_3bex.at("3bex"), m_variable, m_bin);

  fillRegInfo(list_reg1L_4bin, m_siglist, "c1lep", "4bin", regions_1L_4bin.at("4bin"), m_variable, m_bin);

  //SetAtlasStyle();

  // make plots
  MakePlot(list_reg1L_3bex, m_siglist, m_outName+"_yield_3bex", "S/#sqrt{B}", "S/B", "#geq6j, 3b");
  MakePlot(list_reg1L_4bin, m_siglist, m_outName+"_yield_4bin", "S/#sqrt{B}", "S/B", "#geq6j, #geq4b");

  return 0;

}
