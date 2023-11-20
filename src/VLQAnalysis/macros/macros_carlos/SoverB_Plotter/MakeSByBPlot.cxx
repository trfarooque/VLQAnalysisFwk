#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <boost/algorithm/string/replace.hpp>
#include <regex>

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

std::vector<std::string> bkg_list = {"Dibosons","Dijet","SM4tops","Singletop","SingletopWtprod","Singletopschan","Singletoptchan","Wjets","Zjets","topEW","ttH","ttbarbb","ttbarcc","ttbarlight"};

double mc16a_scale = 36207.66; 
double mc16d_scale = 44037.4; 
double mc16e_scale = 58450.1;

//std::string inDir = "";
// Structure to store the information for each signal and background sample
//__________________________________________________________________________________________________
//
struct SigInfo{
  
  std::string key;
  std::string label;
  Color_t color;
  std::string suffix;

  SigInfo(const std::string& _key, const std::string& _label, std::string& _color, 
	  std::string& _suffix){ 

    key = _key;
    label = _label;
    suffix = _suffix;
    
    std::transform(_color.begin(), _color.end(), _color.begin(), toupper);

    if(_color == "RED") color = kRed+1;
    else if(_color == "BLUE") color = kBlue+1;
    else if(_color == "GREEN") color = kGreen+2;
    else color =kBlack;

  }

};

// Structure to store the information for a given search region
//__________________________________________________________________________________________________
//
struct RegInfo{
  
  std::string name;
  std::string binlabel;
  std::map<std::string, double> list_sbyb; // store yield ratio for a given signal
  std::map<std::string, double> list_sbysqb; // store significance for a given signal
  std::map<std::string, double> list_s; // store signal yield
  std::map<std::string, double> list_b; // store background yield
  std::map<std::string, double> list_sigStatUnc; // store stat uncertainty for a given signal
  std::map<std::string, double> list_bkgStatUnc; // store stat uncertainty for background

  RegInfo( const std::string& _name, const std::string& _binlabel,
           const std::map<std::string, double>& _list_sbyb,
           const std::map<std::string, double>& _list_sbysqb,
	   const std::map<std::string, double>& _list_s,
	   const std::map<std::string, double>& _list_b,
           const std::map<std::string, double>& _list_sigStatUnc,
           const std::map<std::string, double>& _list_bkgStatUnc){

    name = _name;
    binlabel = _binlabel;
    list_sbyb = _list_sbyb;
    list_sbysqb = _list_sbysqb;
    list_s = _list_s;
    list_b = _list_b;
    list_sigStatUnc = _list_sigStatUnc;
    list_bkgStatUnc = _list_bkgStatUnc;
  }

  std::map< std::string, double > getSByB() const{ return list_sbyb; }
  std::map< std::string, double > getSBySqrtB() const { return list_sbysqb; }
  std::map< std::string, double > getS() const { return list_s; }
  std::map< std::string, double > getB() const { return list_b; }
  std::map< std::string, double > getSStatUnc() const { return list_sigStatUnc; }
  std::map< std::string, double > getBStatUnc() const { return list_bkgStatUnc; }


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
    std::string suffix = "";

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
	else if(opt == "SUFFIX") suffix = val;
	else if(opt == "COLOR") color = val;
      }

      siglist.push_back( new SigInfo(key, label, color, suffix));
      
    }
   
  }

  std::cout << "#####################################" << std::endl;
  std::cout << "Closing file "  << fileName << std::endl;
  std::cout << "#####################################" << std::endl;
  
  file.close();

}

// Function that determines the region label to be used
//__________________________________________________________________________________________________
//
std::string getRegionLabel(const std::string &region){

  std::string tmp_region = region;
  
  std::vector< std::string > labelComponents;

  std::smatch match;
  
  std::regex JBOMR("[[:d:]_[:d:]]{1,3}[jfbMJHVLT]+[a-z]{2,3}");

  while(std::regex_search(tmp_region, match, JBOMR)){

    for(std::string comp : match){

      std::string tmp_label = "";

      if(comp.find("win") != std::string::npos){
	tmp_label += std::string(1,comp[0]) + "-" + std::string(1,comp[2]);
	comp = comp.substr(3, comp.find("win")-3);
      }
      else if(comp.find("in") != std::string::npos){
	tmp_label += "#geq" + std::string(1,comp[0]);
	comp = comp.substr(1, comp.find("in")-1);
      }
      else if(comp.find("ex") != std::string::npos){
	tmp_label += comp[0];
	comp = comp.substr(1, comp.find("ex")-1);
      }
      else{
	tmp_label += "#oplus" + std::string(1,comp[0]);
	comp = comp.substr(1, comp.find("or")-1);
      }
      
      // LT, VT, HT, VLT
      if(comp.size() > 1 && comp != "fj"){
	
	tmp_label += "(";

	std::string::iterator it = comp.begin();

	while(it != comp.end()){

	  tmp_label += (*it != 'L' && *it != 'T') ? std::string(1,*it) : ( (*it == 'L') ? "$t_{l}$" : "$t_{h}$" );

	  if(next(it) != comp.end()) tmp_label+="+";

	  ++it;

	}

	tmp_label += ")";

      }
      // j, b, fj, H, V, T, L
      else{
	
	tmp_label += (comp != "L" && comp != "T") ? comp : ( (comp == "L") ? "$t_{l}$" : "$t_{h}$");

      }

      //label += ", ";

      labelComponents.push_back(tmp_label);
      
    }

    tmp_region = match.suffix().str();
    
  }

  if(region.find("HighMtbmin") != std::string::npos) labelComponents.push_back("HM");
  else if(region.find("LowMtbmin") != std::string::npos) labelComponents.push_back("LM");

  if(region.find("HighMVAScore") != std::string::npos) labelComponents.push_back("HMVA");
  else if(region.find("MidMVAScore") != std::string::npos) labelComponents.push_back("MMVA");
  else if(region.find("LowMVAScore") != std::string::npos) labelComponents.push_back("LMVA");

  if(region.find("HighMetCut") != std::string::npos) labelComponents.push_back("HMET");
  else if(region.find("LowMetCut") != std::string::npos) labelComponents.push_back("LMET");

  std::string label = "";

  for(unsigned int i = 0; i < labelComponents.size()-1; i++) label += labelComponents[i]+", ";

  label += labelComponents[labelComponents.size()-1];

  return label;

}

// Function that fills a vector of RegInfo with the relevant information
//__________________________________________________________________________________________________
//
void fillRegInfo(std::string inDir, std::vector< RegInfo* >& reglist, std::vector< SigInfo* >& siglist, SigInfo* &bkginfo, std::vector< std::string >& regions, std::string& variable, double varBin){

  for(unsigned int j = 0; j < regions.size(); j++){
    
    std::string reg_name = regions[j]+"_"+variable;
    std::string reg_label = getRegionLabel(regions[j]);

    std::map< std::string, double > reg_sbyb = {};
    std::map< std::string, double > reg_sbysqb = {};
    std::map< std::string, double > reg_s = {};
    std::map< std::string, double > reg_b = {};
    std::map< std::string, double > reg_sigStatUnc = {};
    std::map< std::string, double > reg_bkgStatUnc = {};
    
    reglist.push_back(new RegInfo(reg_name, reg_label, reg_sbyb, reg_sbysqb, reg_s, reg_b, reg_sigStatUnc, reg_bkgStatUnc));

  }

  std::map<std::string,TFile*> bkg_files_mc16a;
  std::map<std::string,TFile*> bkg_files_mc16d;
  std::map<std::string,TFile*> bkg_files_mc16e;

  for(auto bkg : bkg_list ){

    bkg_files_mc16a[bkg] = TFile::Open((inDir+"/"+bkg+".mc16a.root").c_str(),"READ");
    bkg_files_mc16d[bkg] = TFile::Open((inDir+"/"+bkg+".mc16d.root").c_str(),"READ");
    bkg_files_mc16e[bkg] = TFile::Open((inDir+"/"+bkg+".mc16e.root").c_str(),"READ");

  }


  for(unsigned int i = 0; i < siglist.size(); i++){
   
    SigInfo* siginfo = siglist.at(i);

    TFile* sig_mc16a = TFile::Open( (inDir+"/VLQ_TT_"+siginfo->key+".mc16a.root").c_str(), "READ");
    TFile* sig_mc16d = TFile::Open( (inDir+"/VLQ_TT_"+siginfo->key+".mc16d.root").c_str(), "READ");
    TFile* sig_mc16e = TFile::Open( (inDir+"/VLQ_TT_"+siginfo->key+".mc16e.root").c_str(), "READ");
 
    for(unsigned int j = 0; j < regions.size(); j++){

      std::cout << "Processing region " << regions[j] << std::endl;

      std::string hist_name = regions[j] + "_" + variable;

      TH1D* sig_mc16a_hist = (TH1D*)(sig_mc16a->Get(hist_name.c_str()));
      TH1D* sig_mc16d_hist = (TH1D*)(sig_mc16d->Get(hist_name.c_str()));
      TH1D* sig_mc16e_hist = (TH1D*)(sig_mc16e->Get(hist_name.c_str()));

      sig_mc16a_hist->Scale(mc16a_scale);
      sig_mc16d_hist->Scale(mc16d_scale);
      sig_mc16e_hist->Scale(mc16e_scale);

      TH1D* bkg_mc16a_hist = (TH1D*)(sig_mc16a_hist->Clone());
      bkg_mc16a_hist->Reset();
      TH1D* bkg_mc16d_hist = (TH1D*)(sig_mc16d_hist->Clone());
      bkg_mc16d_hist->Reset();
      TH1D* bkg_mc16e_hist = (TH1D*)(sig_mc16d_hist->Clone());
      bkg_mc16e_hist->Reset();

      for( auto bkg : bkg_list ){ 

	TH1D* h_bkg_mc16a =  (TH1D*)(bkg_files_mc16a[bkg]->Get(hist_name.c_str()));
	TH1D* h_bkg_mc16d =  (TH1D*)(bkg_files_mc16d[bkg]->Get(hist_name.c_str()));
	TH1D* h_bkg_mc16e =  (TH1D*)(bkg_files_mc16e[bkg]->Get(hist_name.c_str()));

	if(bkg == "Dijet"){
	  h_bkg_mc16a->Scale(0.8);
	  h_bkg_mc16d->Scale(0.8);
	  h_bkg_mc16e->Scale(0.8);
	}
	bkg_mc16a_hist -> Add( h_bkg_mc16a );
	bkg_mc16d_hist -> Add( h_bkg_mc16d );
	bkg_mc16e_hist -> Add( h_bkg_mc16e );

	//bkg_mc16a_hist -> Add( (TH1D*)(bkg_files_mc16a[bkg]->Get(hist_name.c_str())) );
	//bkg_mc16d_hist -> Add( (TH1D*)(bkg_files_mc16d[bkg]->Get(hist_name.c_str())) );
	//bkg_mc16e_hist -> Add( (TH1D*)(bkg_files_mc16e[bkg]->Get(hist_name.c_str())) );

      }
      
      bkg_mc16a_hist->Scale(mc16a_scale);
      bkg_mc16d_hist->Scale(mc16d_scale);
      bkg_mc16e_hist->Scale(mc16e_scale);


      bkg_mc16a_hist->Add(bkg_mc16d_hist);
      bkg_mc16a_hist->Add(bkg_mc16e_hist);
      
      sig_mc16a_hist->Add(sig_mc16d_hist);
      sig_mc16a_hist->Add(sig_mc16e_hist);
      std::cout << ":TF 4" << std::endl;

      int bin_1TeV = bkg_mc16a_hist->FindBin(varBin);
      double bkgStatUnc = 0;
      double bkgYield = bkg_mc16a_hist->IntegralAndError(bin_1TeV, -1, bkgStatUnc);

      double sigStatUnc = 0;
      double sigYield = sig_mc16a_hist->IntegralAndError(bin_1TeV, -1, sigStatUnc);

      double SbyB = (sigYield > 0.) ? sigYield/bkgYield : 0.;
      double SbySqB = (sigYield > 0.) ? sigYield/sqrt(bkgYield): 0.;

      (reglist[j]->list_sbyb).insert(std::pair<std::string, double>(siginfo->key, SbyB));
      (reglist[j]->list_sbysqb).insert(std::pair<std::string, double>(siginfo->key, SbySqB));
      (reglist[j]->list_s).insert(std::pair<std::string, double>(siginfo->key, sigYield));
      (reglist[j]->list_b).insert(std::pair<std::string, double>(siginfo->key, bkgYield));
      (reglist[j]->list_sigStatUnc).insert(std::pair<std::string, double>(siginfo->key, sigStatUnc));
      (reglist[j]->list_bkgStatUnc).insert(std::pair<std::string, double>(siginfo->key, bkgStatUnc));

    }

    sig_mc16a->Close();
    sig_mc16d->Close();
    sig_mc16e->Close();

    //bkg_mc16a->Close();
    //bkg_mc16d->Close();
    //bkg_mc16e->Close();

  }

}

// Function that generates a LaTeX table with all the information of the signal that maximizes
// the S/B of a given search region
//__________________________________________________________________________________________________ 
//
void MakeLaTeXTable(std::vector< RegInfo* >& reglist, const std::string &fName){

  std::ofstream file(fName);
  file << "\\documentclass{article}\n";
  file << "\\begin{document}\n";
  file << "\\begin{table}[h!]\n";
  file << "\\centering\n";
  file << "\\hspace*{-3cm}\n";
  file << "\\begin{tabular}{| c || c | c | c | c | c | c |}\n";
  file << "\\hline\\hline\n";
  file << "\\hline\n";
  file << "Region & Signal with max S/B & S/B & S/$\\sqrt{B}$ & S & B & B stat. unc.\\\\\n";
  file << "\\hline\n";
  
  for(unsigned int i = 0; i < reglist.size(); i++){

    std::string key_max_sbyb;
    double max_sbyb = -1;

    std::map< std::string, double >::iterator it = (reglist[i]->list_sbyb).begin();
  
    while(it != (reglist[i]->list_sbyb).end()){

      if(it->second > max_sbyb){
	
	key_max_sbyb = it->first;
	max_sbyb = it->second;

      }

      ++it;

    }
    
    std::string bin_label_latex = boost::replace_all_copy(reglist[i]->binlabel, "#geq", "$\\geq$");
    bin_label_latex = boost::replace_all_copy(bin_label_latex, "#oplus", "$\\oplus$");
    std::string sig_name_latex = boost::replace_all_copy(key_max_sbyb, "_", " ");
    
    file << bin_label_latex << " & ";
    file << sig_name_latex << " & " << std::setprecision(3) << max_sbyb << " & ";
    file << reglist[i]->list_sbysqb[key_max_sbyb] << " & ";
    file << reglist[i]->list_s[key_max_sbyb] << " & ";
    file << reglist[i]->list_b[key_max_sbyb] << " & ";
    file << reglist[i]->list_bkgStatUnc[key_max_sbyb] << " \\\\\n";
    file << "\\hline\n";
 
  }

  file << "\\hline\\hline\n";

  file << "\\end{tabular}\n";
  file << "\\end{table}\n";
  file << "\\end{document}\n";

  file.close();

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
void MakePlotPanel(std::vector< RegInfo* >& list_reg, std::vector< SigInfo* >& list_sig, const std::string &option,
		   TLegend* &leg, const std::string &out_name, const std::string &yTitle = "",
		   bool doBinLabel = false, bool doLegendEntry = false, bool doLogY = false, bool doErrorBar = false,
		   double textSizeSF = 1.0){

  unsigned int nbins = list_reg.size();

  THStack *histo_stack = new THStack(("hstack_"+out_name+"_"+option).c_str(), "");

  for(unsigned int isig = 0; isig < list_sig.size(); isig++){
    
    TH1D* histo = new TH1D(("h_"+out_name+"_"+option+"_"+list_sig[isig]->key).c_str(), "", nbins, 0, nbins);

    for(unsigned int ibin = 1; ibin <= nbins; ibin++){
      
      if(option == "SByB") histo->SetBinContent(ibin, (list_reg[ibin-1]->getSByB())[list_sig[isig]->key]);
      else histo->SetBinContent(ibin, (list_reg[ibin-1]->getSBySqrtB())[list_sig[isig]->key]);
     
      if(doErrorBar && !doLogY){
	if(option == "SByB") histo->SetBinError(ibin, ( (list_reg[ibin-1]->getSStatUnc())[list_sig[isig]->key] )/ (list_reg[ibin-1]->getBStatUnc())[list_sig[isig]->key] );
	else histo->SetBinError(ibin, ( (list_reg[ibin-1]->getSStatUnc())[list_sig[isig]->key] )/sqrt( (list_reg[ibin-1]->getBStatUnc())[list_sig[isig]->key] ) );
      }

      if(doBinLabel) histo->GetXaxis()->SetBinLabel(ibin, (list_reg[ibin-1]->binlabel).c_str());
 
    }

    histo->SetLineColor(list_sig[isig]->color);
    histo->SetLineWidth(2);
    histo->SetLineStyle(2);

    if(doErrorBar && !doLogY) histo_stack->Add(histo, "E0");
    else histo_stack->Add( histo );

    if(doLegendEntry) leg->AddEntry(histo, (list_sig[isig]->label).c_str(), "f");
    
  }

  double stretch = (doLogY) ? 1.E1 : 1.37;
  double yLabelSize = (doLogY) ? 0.05*textSizeSF : 0.07*textSizeSF;
  double yTitleSize = (doLogY) ? 0.06*textSizeSF : 0.09*textSizeSF;
  double yTitleOffset = (doLogY) ? 0.7 : 0.5;
  double yTickLength = (doLogY) ? 0.015 : 0.009;

  histo_stack->SetMaximum(stretch*histo_stack->GetMaximum("nostack"));

  histo_stack->Draw("nostack");

  histo_stack->GetHistogram()->GetYaxis()->SetTitle(yTitle.c_str());
  histo_stack->GetHistogram()->GetYaxis()->SetLabelSize(yLabelSize);
  histo_stack->GetHistogram()->GetYaxis()->SetTitleSize(yTitleSize);
  histo_stack->GetHistogram()->GetYaxis()->SetTitleOffset(yTitleOffset);
  histo_stack->GetHistogram()->GetYaxis()->SetTickLength(yTickLength);
  histo_stack->GetHistogram()->GetYaxis()->CenterTitle();

  histo_stack->GetXaxis()->SetTickLength(0.015);
  if(doBinLabel){
    histo_stack->GetXaxis()->SetLabelSize(0.07*textSizeSF);
    histo_stack->GetXaxis()->SetLabelOffset(0.02);
    for(unsigned int ibin = 1; ibin <= nbins; ibin++) histo_stack->GetXaxis()->SetBinLabel(ibin, (list_reg[ibin-1]->binlabel).c_str());
    
  }

}

//__________________________________________________________________________________________________ 
//
void MakePlot(std::vector< RegInfo* > &list_reg, std::vector< SigInfo* > &list_sig, const std::string &out_name,
	      const std::string &feature1, const std::string &yTitle1, bool doBinLabel1 = false, bool doLegendEntry1 = false, bool doLogY1 = false, bool doErrorBar1 = false,
	      const std::string &feature2 = "", const std::string &yTitle2 = "", bool doBinLabel2 = false, bool doLegendEntry2 = false, bool doLogY2 = false, bool doErrorBar2 = false){

  double textSizeSF = (feature2 == "") ? 0.5 : 1.0;
  
  TCanvas* canv = new TCanvas(out_name.c_str(), "", 1000, 700);
  
  canv->SetTicks();

  TLegend* leg = new TLegend(0.65, 0.6, 0.85, 0.85);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetLineColor(0);
  leg->SetLineStyle(0);
  leg->SetLineWidth(0);
  leg->SetBorderSize(0);
  // Information encoded in TextFont Number
  // Font code = 10*fontnumber + precision
  // If precision is > 2 then the text size is set to the number of pixels.
  // Otherwise the text size is computed as a percent of the pad size      
  leg->SetTextFont(42);
  leg->SetTextSize(0.060*textSizeSF);
  leg->SetShadowColor(0);

  TPaveText* ttlbox = new TPaveText(0.18, 0.71, 0.38, 0.82, "NBNDC");
  ttlbox->SetTextAlign(12); //horizontally centred; top-aligned   
  ttlbox->SetFillColor(0);
  ttlbox->SetFillStyle(0);
  ttlbox->SetLineColor(0);
  ttlbox->SetLineStyle(0);
  ttlbox->SetLineWidth(0);
  ttlbox->SetBorderSize(0);
  ttlbox->SetTextSize(0.09*textSizeSF);
  ttlbox->SetTextFont(42);
  ttlbox->SetShadowColor(0);
  
  ttlbox->AddText("#splitline{#scale[1.2]{#bf{#it{ATLAS}} Simulation}}{#sqrt{s} = 13 TeV, 139 fb^{-1}}");

  if(feature2 == ""){
    canv->SetBottomMargin(0.18);
    canv->SetLeftMargin(0.17);
    canv->SetRightMargin(0.17);
    canv->SetFrameBorderMode(0);
    canv->SetFillStyle(4000);

    MakePlotPanel(list_reg, list_sig, feature1, leg, out_name, yTitle1, doBinLabel1, doLegendEntry1, doLogY1, doErrorBar1, textSizeSF);

    if(doLegendEntry1) leg->Draw();
    if(doLogY1) canv->SetLogy();

    ttlbox->Draw();

  }
  else{

    canv->Divide(1,2);

    canv->cd(1)->SetPad(0,0.6,1,1);
    canv->cd(1)->SetTicks();
    canv->cd(1)->SetTopMargin(0.05);
    canv->cd(1)->SetBottomMargin(0.);
    canv->cd(1)->SetLeftMargin(0.17);
    canv->cd(1)->SetRightMargin(0.17);
    canv->cd(1)->SetFrameBorderMode(0);
    canv->cd(1)->SetFillStyle(4000);

    canv->cd(2)->SetPad(0,0,1,0.6);
    canv->cd(2)->SetTicks();
    canv->cd(2)->SetTopMargin(0.0);
    canv->cd(2)->SetBottomMargin(0.48);
    canv->cd(2)->SetLeftMargin(0.17);
    canv->cd(2)->SetRightMargin(0.17);
    canv->cd(2)->SetFrameFillStyle(4000);
    canv->cd(2)->SetFrameBorderMode(0);
    canv->cd(2)->SetFillStyle(4000);

    canv->cd(1);
    
    MakePlotPanel(list_reg, list_sig, feature1, leg, out_name, yTitle1, doBinLabel1, doLegendEntry1, doLogY1, doErrorBar1, textSizeSF);

    if(doLegendEntry1) leg->Draw();
    if(doLogY1) canv->cd(1)->SetLogy();

    ttlbox->Draw();

    canv->cd(1)->Update();
    canv->cd(1)->Modified();

    canv->cd(2);

    MakePlotPanel(list_reg, list_sig, feature2, leg, out_name, yTitle2, doBinLabel2, doLegendEntry2, doLogY2, doErrorBar2, textSizeSF);

    if(doLegendEntry2) leg->Draw();
    if(doLogY2) canv->cd(2)->SetLogy();

    canv->cd(2)->Update();
    canv->cd(2)->Modified();
    canv->cd();

  }

  canv->Update();
  canv->Modified();
  canv->SaveAs(Form("%s.png", canv->GetName()));
  canv->SaveAs(Form("%s.pdf", canv->GetName()));
  canv->SaveAs(Form("%s.eps", canv->GetName()));

}

//__________________________________________________________________________________________________
//
int main(int argc, char** argv){
  std::string m_inDir = "";
  std::string m_variable = "meff";
  double m_bin = 1000.;
  std::string m_sigSampleFile = "";
  std::string m_bkgSampleFile = "";
  std::string m_outName = "test.root";
  bool m_doOldAnalysis = false;
  bool m_do0Lepton = false;
  bool m_do1Lepton = false;
  bool m_doLatexTable = false;

  for(int i = 1; i < argc; i++){

    std::string opt(argv[i]);

    std::string argument;

    std::string value;

    size_t pos = opt.find("=");

    argument = opt.substr(0, pos);

    value = opt.erase(0, pos + 1);

    std::transform(argument.begin(), argument.end(), argument.begin(), toupper);

    if(argument == "--INDIR") m_inDir = value;
    else if(argument == "--SIGSAMPLEFILE") m_sigSampleFile = value;
    else if(argument == "--BKGSAMPLEFILE") m_bkgSampleFile = value;
    else if(argument == "--OUTNAME") m_outName = value;
    else if(argument == "--VARIABLE") m_variable = value;
    else if(argument == "--VARBIN") m_bin = std::stod(value);
    else if(argument == "--DOOLDREGIONS"){
      std::transform(value.begin(), value.end(), value.begin(), toupper);
      m_doOldAnalysis = (value == "TRUE");
    }
    else if(argument == "--DO0LEPTON"){
      std::transform(value.begin(), value.end(), value.begin(), toupper);
      m_do0Lepton = (value == "TRUE");
    }
    else if(argument == "--DO1LEPTON"){
      std::transform(value.begin(), value.end(), value.begin(), toupper);
      m_do1Lepton = (value == "TRUE");
    }
    else if(argument == "--DOLATEXTABLE"){
      std::transform(value.begin(), value.end(), value.begin(), toupper);
      m_doLatexTable = (value == "TRUE");
    }
    else{ std::cout << "WARNING: Option " << argument << " not recognized." << std::endl; }

  }

  std::vector< SigInfo* > m_siglist = {};
  std::vector< SigInfo* > m_bkglist = {};

  fillSigInfoVector(m_siglist, m_sigSampleFile, ':');
  fillSigInfoVector(m_bkglist, m_bkgSampleFile, ':');
  
  // Old Pair VLQ Analysis 0-lepton Regions
  std::vector< std::string > old_pvlq_ana_0l_2bex = 
    {
      "c0lep0Tex0Hex7jin2bexHighMtbmin", "c0lep1Tex0Hex7jin2bexHighMtbmin", "c0lep0Tex1Hex7jin2bexHighMtbmin", "c0lep2THin7jin2bexHighMtbmin"
    };

  std::vector< std::string > old_pvlq_ana_0l_3bex =
    {
      "c0lep0Tex0Hex7jin3bexHighMtbmin", "c0lep1Tex0Hex7jin3bexHighMtbmin", "c0lep0Tex1Hex7jin3bexHighMtbmin", "c0lep1Tex1Hex7jin3bexHighMtbmin", "c0lep2Tin0_1Hwin7jin3bexHighMtbmin",
      "c0lep0Tex0Hex7jin3bexLowMtbmin",  "c0lep1Tex0Hex7jin3bexLowMtbmin",  "c0lep0Tex1Hex7jin3bexLowMtbmin",  "c0lep1Tex1Hex7jin3bexLowMtbmin",  "c0lep2Tin0_1Hwin7jin3bexLowMtbmin",
      "c0lep0Tin2Hin7jin3bex"
    };
  
  std::vector< std::string > old_pvlq_ana_0l_4bin = 
    {
      "c0lep0Tex0Hex7jin4binHighMtbmin", "c0lep1Tex0Hex7jin4binHighMtbmin", "c0lep0Tex1Hex7jin4binHighMtbmin",
      "c0lep0Tex0Hex7jin4binLowMtbmin",  "c0lep1Tex0Hex7jin4binLowMtbmin",  "c0lep0Tex1Hex7jin4binLowMtbmin",
      "c0lep2THin7jin2bin"
    };
 
  // Old Pair VLQ Analysis 1-lepton Regions
  std::vector< std::string > old_pvlq_ana_1l_3bex = 
    {
      "c1lep0Tex0Hex6jin3bex", "c1lep0Tex1Hex6jin3bex", "c1lep1Tex0Hex6jin3bex", "c1lep1Tex1Hex6jin3bex", "c1lep2Tin0_1Hwin6jin3bex", "c1lep0Tin2Hin6jin3bex"
    };

  std::vector< std::string > old_pvlq_ana_1l_4bin =
    {
      "c1lep0Tex0Hex6jin4bin", "c1lep0Tex1Hex6jin4bin", "c1lep1Tex0Hex6jin4bin", "c1lep1Tex1Hex6jin4bin", "c1lep2Tin0_1Hwin6jin4bin", "c1lep0Tin2Hin6jin4bin"
    };

  // New Pair VLQ Analysis 0-lepton Regions
  std::vector<std::string> new_pvlq_ana_0l_2bex =
    {
      "c0lep7jin2bex0Hex0Vin1Tin", "c0lep7jin2bex0Hex1Vin1Tex", "c0lep7jin2bex0Hex1Vin2Tin", "c0lep7jin2bex0Hex2Vin1Tex", "c0lep7jin2bex0Hex2Vin2Tin",
      "c0lep7jin2bex1Hex0Vex0Tex", "c0lep7jin2bex1Hex1Vin0Tex", "c0lep7jin2bex1Hex0Vex1Tex", "c0lep7jin2bex1Hex1Vin1Tex", "c0lep7jin2bex1Hex0Vin2Tin",
      "c0lep7jin2bex2Hin0Vin0Tex", "c0lep7jin2bex2Hin0Vin1Tin"
    };

  std::vector<std::string> new_pvlq_ana_0l_3bex =
    {
      "c0lep7jin3bex0Hex0Vin1Tin", "c0lep7jin3bex0Hex1Vin1Tex", "c0lep7jin3bex0Hex1Vin2Tin", "c0lep7jin3bex0Hex2Vin1Tex", "c0lep7jin3bex0Hex2Vin2Tin",
      "c0lep7jin3bex1Hex0Vex0Tex", "c0lep7jin3bex1Hex1Vin0Tex", "c0lep7jin3bex1Hex0Vex1Tex", "c0lep7jin3bex1Hex1Vin1Tex", "c0lep7jin3bex1Hex0Vin2Tin",
      "c0lep7jin3bex2Hin0Vin0Tex", "c0lep7jin3bex2Hin0Vin1Tin"
    };

  std::vector<std::string> new_pvlq_ana_0l_4bin =
    {
      "c0lep7jin4bin0Hex0Vin1Tin", "c0lep7jin4bin0Hex1Vin1Tex", "c0lep7jin4bin0Hex1Vin2Tin", "c0lep7jin4bin0Hex2Vin1Tex", "c0lep7jin4bin0Hex2Vin2Tin",
      "c0lep7jin4bin1Hex0Vex0Tex", "c0lep7jin4bin1Hex1Vin0Tex", "c0lep7jin4bin1Hex0Vex1Tex", "c0lep7jin4bin1Hex1Vin1Tex", "c0lep7jin4bin1Hex0Vin2Tin",
      "c0lep7jin4bin2Hin0Vin0Tex", "c0lep7jin4bin2Hin0Vin1Tin"
    };

  // New Pair VLQ Analysis 0-lepton Regions Ordered by Higgs Tag Multiplicity

  std::vector< std::string > new_pvlq_ana_0l_0Hex = 
    {
      "c0lep7jin2bex0Hex1VTexHighMtbmin", "c0lep7jin2bex0Hex1Vex1TinHighMtbmin", "c0lep7jin2bex0Hex0Vex2TinHighMtbmin", "c0lep7jin2bex0Hex2Vin0TinHighMtbmin",
      "c0lep7jin3bin0Hex1VTexHighMtbmin", "c0lep7jin3bex0Hex1VTexLowMtbmin", "c0lep7jin4bin0Hex1VTexLowMtbmin",
      "c0lep7jin3bin0Hex1Vex1TinHighMtbmin", "c0lep7jin3bin0Hex1Vex1TinLowMtbmin",
      "c0lep7jin3bin0Hex0Vex2TinHighMtbmin", "c0lep7jin3bex0Hex0Vex2TinLowMtbmin", "c0lep7jin4bin0Hex0Vex2TinLowMtbmin",
      "c0lep7jin3bin0Hex2Vin0Tin"
   };

  std::vector< std::string > new_pvlq_ana_0l_1Hex =
    {
      "c0lep7jin2bex1Hin0VTexHighMtbmin", "c0lep7jin2bex1Hin1VTexHighMtbmin", "c0lep7jin2bex1Hin2VTinHighMtbmin",
      "c0lep7jin3bex1Hex0VTex", "c0lep7jin3bex1Hex1VTex", "c0lep7jin3bex1Hex2VTin", 
      "c0lep7jin4bin1Hex0VTex", "c0lep7jin4bin1Hex1VTex", "c0lep7jin4bin1Hex2VTin"
    };

  std::vector< std::string > new_pvlq_ana_0l_1Hex_mtbmin_split =
    {
      "c0lep7jin3bex1Hex0VTexHighMtbmin", "c0lep7jin3bex1Hex0VTexLowMtbmin",
      "c0lep7jin3bex1Hex1VTexHighMtbmin", "c0lep7jin3bex1Hex1VTexLowMtbmin",
      "c0lep7jin3bex1Hex2VTinHighMtbmin", "c0lep7jin3bex1Hex2VTinLowMtbmin",
      "c0lep7jin4bin1Hex0VTexHighMtbmin", "c0lep7jin4bin1Hex0VTexLowMtbmin",
      "c0lep7jin4bin1Hex1VTexHighMtbmin", "c0lep7jin4bin1Hex1VTexLowMtbmin",
      "c0lep7jin4bin1Hex2VTinHighMtbmin", "c0lep7jin4bin1Hex2VTinLowMtbmin"
    };

  std::vector< std::string > new_pvlq_ana_0l_1Hex_2VTin_split =
    {
      "c0lep7jin2bex1Hin0_1Vwin2TinHighMtbmin", "c0lep7jin2bex1Hin2Vin0_1TwinHighMtbmin", "c0lep7jin2bex1Hin1Vex1TexHighMtbmin",
      "c0lep7jin3bex1Hex0_1Vwin2Tin", "c0lep7jin3bex1Hex2Vin0_1Twin", "c0lep7jin3bex1Hex1Vex1Tex",
      "c0lep7jin4bin1Hex0_1Vwin2Tin", "c0lep7jin4bin1Hex2Vin0_1Twin", "c0lep7jin4bin1Hex1Vex1Tex",
    };

  std::vector< std::string > new_pvlq_ana_0l_1Hex_opt2 = 
    {
      "c0lep7jin2bex1Hin0_1VTwinHighMtbmin", "c0lep7jin2bex1Hin2VTinHighMtbmin",
      "c0lep7jin3bex1Hex0_1VTwin", "c0lep7jin3bex1Hex2VTin",
      "c0lep7jin4bin1Hex0VTex", "c0lep7jin4bin1Hex1VTex", "c0lep7jin4bin1Hex2VTin"
    };

  std::vector< std::string > new_pvlq_ana_0l_1Hex_opt3 =
    {
      "c0lep7jin2bex1Hin0_1VTwinHighMtbmin", "c0lep7jin2bex1Hin1Vex1TexHighMtbmin", "c0lep7jin2bex1Hin2VTorHighMtbmin",
      "c0lep7jin3bex1Hex0_1VTwin", "c0lep7jin3bex1Hex1Vex1Tex", "c0lep7jin3bex1Hex2VTor",
      "c0lep7jin4bin1Hex0VTex", "c0lep7jin4bin1Hex1VTex", "c0lep7jin4bin1Hex1Vex1Tex", "c0lep7jin4bin1Hex2VTor"
    };

  std::vector< std::string > new_pvlq_ana_0l_2Hin = 
    {
    "c0lep7jin3bin2Hin0Vin0Tin"
    };
  
  //^----------------------Second Iteration-----------------------^


  // New Pair VLQ Analysis 1-lepton Regions
  std::vector< std::string > new_pvlq_ana_1l_3bex = 
    {
      "c1lep6jin3bex0Hex0Vin0LTex", "c1lep6jin3bex0Hex0Vin1LTin", "c1lep6jin3bex1Hex0Vex0LTex", "c1lep6jin3bex1Hex0Vex1Lin0Tex", 
      "c1lep6jin3bex1Hex1Vin0Lin0Tex", "c1lep6jin3bex1Hex0Vin0Lex1Tin", "c1lep6jin3bex1Hex0Vin1Lin1Tin", 
      "c1lep6jin3bex2Hin0Vin1LTin", "c1lep6jin3bex2Hin0Vin0LTex"
    };

  std::vector< std::string > new_pvlq_ana_1l_4bin =
    {
      "c1lep6jin4bin0Hex0Vin0LTex", "c1lep6jin4bin0Hex0Vin1LTin", "c1lep6jin4bin1Hex0Vex0LTex", "c1lep6jin4bin1Hex0Vex1Lin0Tex",
      "c1lep6jin4bin1Hex1Vin0Lin0Tex", "c1lep6jin4bin1Hex0Vin0Lex1Tin", "c1lep6jin4bin1Hex0Vin1Lin1Tin", "c1lep6jin4bin2Hin0Vin0Lin0Tin"
    };

  std::vector< std::string > new_pvlq_ana_1l_MVA_5jex_3bex =
    {
      "c1lep5jex3bex2Min3JinHighMVAScore", "c1lep5jex3bex2Min3Jin0HexHighMVAScore", "c1lep5jex3bex2Min3Jin1HinHighMVAScore",
      "c1lep5jex3bex2Min3JinMidMVAScore",  "c1lep5jex3bex2Min3Jin0HexMidMVAScore",  "c1lep5jex3bex2Min3Jin1HinMidMVAScore",
      "c1lep5jex3bex2Min3JinLowMVAScore",  "c1lep5jex3bex2Min3Jin0HexLowMVAScore",  "c1lep5jex3bex2Min3Jin1HinLowMVAScore"
    };

  std::vector< std::string > new_pvlq_ana_1l_MVA_6jin_3bex = 
    {
      "c1lep6jin3bex2Min3JinHighMVAScore", "c1lep6jin3bex2Min3Jin0HexHighMVAScore", "c1lep6jin3bex2Min3Jin1HinHighMVAScore",
      "c1lep6jin3bex2Min3JinMidMVAScore",  "c1lep6jin3bex2Min3Jin0HexMidMVAScore",  "c1lep6jin3bex2Min3Jin1HinMidMVAScore",
      "c1lep6jin3bex2Min3JinLowMVAScore",  "c1lep6jin3bex2Min3Jin0HexLowMVAScore",  "c1lep6jin3bex2Min3Jin1HinLowMVAScore"
    };

  std::vector< std::string > new_pvlq_ana_1l_MVA_5jex_4bin =
    {
      "c1lep5jex4bin2Min3JinHighMVAScore", "c1lep5jex4bin2Min3Jin0HexHighMVAScore", "c1lep5jex4bin2Min3Jin1HinHighMVAScore",
      "c1lep5jex4bin2Min3JinMidMVAScore",  "c1lep5jex4bin2Min3Jin0HexMidMVAScore",  "c1lep5jex4bin2Min3Jin1HinMidMVAScore",
      "c1lep5jex4bin2Min3JinLowMVAScore",  "c1lep5jex4bin2Min3Jin0HexLowMVAScore",  "c1lep5jex4bin2Min3Jin1HinLowMVAScore"
    };
  
  std::vector< std::string > new_pvlq_ana_1l_MVA_6jin_4bin =
    {
      "c1lep6jin4bin2Min3JinHighMVAScore", "c1lep6jin4bin2Min3Jin0HexHighMVAScore", "c1lep6jin4bin2Min3Jin1HinHighMVAScore",
      "c1lep6jin4bin2Min3JinMidMVAScore",  "c1lep6jin4bin2Min3Jin0HexMidMVAScore",  "c1lep6jin4bin2Min3Jin1HinMidMVAScore",
      "c1lep6jin4bin2Min3JinLowMVAScore",  "c1lep6jin4bin2Min3Jin0HexLowMVAScore",  "c1lep6jin4bin2Min3Jin1HinLowMVAScore"
    };


  std::vector< std::string > new_pvlq_ana_1l_MVA_5jin = 
    {
      "c1lep5jin2bex0Hex1VTex1Lex",
      "c1lep5jin3bex0Hex1VTex0_1Lwin",
      "c1lep5jin4bin0Hex1VTex0_1Lwin",
      "c1lep5jin3bex2Min3Jin0HexHighMVAScore",
      "c1lep5jin3bex2Min3Jin0HexMidMVAScore",
      "c1lep5jin3bex2Min3Jin0HexLowMVAScore",
      "c1lep5jin4bin2Min3Jin0HexHighMVAScore",
      "c1lep5jin4bin2Min3Jin0HexMidMVAScore",
      "c1lep5jin4bin2Min3Jin0HexLowMVAScore",
      "c1lep5jin3bex2Min3Jin1HinHighMVAScore",
      "c1lep5jin3bex2Min3Jin1HinMidMVAScore",
      "c1lep5jin3bex2Min3Jin1HinLowMVAScore",
      "c1lep5jin4bin2Min3Jin1HinHighMVAScore",
      "c1lep5jin4bin2Min3Jin1HinMidMVAScore",
      "c1lep5jin4bin2Min3Jin1HinLowMVAScore"
    };


  std::vector< std::string > new_pvlq_ana_0l_MVA_6jin = 
    {
      "c0lep6jin2bex2MinHighMetCutLowMVAScore",
      "c0lep6jin3bex2MinLowMVAScore",
      "c0lep6jin4bin2MinLowMVAScore",
      "c0lep6jin2bex2MinHighMetCutMidMVAScore",
      "c0lep6jin3bin2MinMidMVAScore",
      "c0lep6jin2bex2MinHighMetCutHighMVAScore",
      "c0lep6jin3bex2MinLowMetCutHighMVAScore",
      "c0lep6jin3bex2MinHighMetCutHighMVAScore",
      "c0lep6jin4bin2Min0HexHighMVAScore",
      "c0lep6jin4bin2Min1HinHighMVAScore"
    };

  std::map<std::string, std::vector<std::string> > regions_1bex = {};
  std::map<std::string, std::vector<std::string> > regions_2bex = {};
  std::map<std::string, std::vector<std::string> > regions_3bex = {};
  std::map<std::string, std::vector<std::string> > regions_4bin = {};

  std::map<std::string, std::vector<std::string> > regions_MVA = {};

  //  else{

    if(m_doOldAnalysis){
      if(m_do0Lepton){

	regions_2bex.insert( std::pair<std::string, std::vector<std::string > >("2bex", old_pvlq_ana_0l_2bex));
	regions_3bex.insert( std::pair<std::string, std::vector<std::string > >("3bex", old_pvlq_ana_0l_3bex));
	regions_4bin.insert( std::pair<std::string, std::vector<std::string > >("4bin", old_pvlq_ana_0l_4bin));
	
	std::vector<RegInfo*> list_reg_2bex{};
	std::vector<RegInfo*> list_reg_3bex{};
	std::vector<RegInfo*> list_reg_4bin{};
	
	fillRegInfo(m_inDir, list_reg_2bex, m_siglist, m_bkglist[0],regions_2bex.at("2bex"), m_variable, m_bin);
	fillRegInfo(m_inDir, list_reg_3bex, m_siglist, m_bkglist[0],regions_3bex.at("3bex"), m_variable, m_bin);
	fillRegInfo(m_inDir, list_reg_4bin, m_siglist, m_bkglist[0],regions_4bin.at("4bin"), m_variable, m_bin);

	MakePlot(list_reg_2bex, m_siglist, m_outName+"_yield_pvlq_0L_2bex", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
		 "SByB", "S/B", true, false, true, false);
	MakePlot(list_reg_3bex, m_siglist, m_outName+"_yield_pvlq_0L_3bex", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
		 "SByB", "S/B", true, false, true, false);
	MakePlot(list_reg_4bin, m_siglist, m_outName+"_yield_pvlq_0L_4bin", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);
      }
      else{
	regions_3bex.insert( std::pair<std::string, std::vector<std::string> >("3bex", old_pvlq_ana_1l_3bex));
	regions_4bin.insert( std::pair<std::string, std::vector<std::string> >("4bin", old_pvlq_ana_1l_4bin));
	
	std::vector<RegInfo*> list_reg_3bex{};
	std::vector<RegInfo*> list_reg_4bin{};
	
	fillRegInfo(m_inDir, list_reg_3bex, m_siglist, m_bkglist[0], regions_3bex.at("3bex"), m_variable, m_bin);
	fillRegInfo(m_inDir, list_reg_4bin, m_siglist, m_bkglist[0], regions_4bin.at("4bin"), m_variable, m_bin);
	
	MakePlot(list_reg_3bex, m_siglist, m_outName+"_yield_pvlq_1L_3bex", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);
	MakePlot(list_reg_4bin, m_siglist, m_outName+"_yield_pvlq_1L_4bin", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);

      }
    }
    else{
      if(m_do0Lepton){

	regions_MVA.insert( std::pair<std::string, std::vector<std::string> >("ALL", new_pvlq_ana_0l_MVA_6jin));
		
	std::vector<RegInfo*> list_reg_MVA{};

	fillRegInfo(m_inDir, list_reg_MVA, m_siglist, m_bkglist[0], regions_MVA.at("ALL"), m_variable, m_bin);

	MakePlot(list_reg_MVA, m_siglist, m_outName+"_yield_pvlq_0L_MVA", "SBySqrtB", "S/#sqrt{B}", 
		 false, true, false, true,
                 "SByB", "S/B", true, false, true, false);

	if(m_doLatexTable){
          MakeLaTeXTable(list_reg_MVA, m_outName+"_table_pvlq_0L_MVA.tex");
	}
      }
      else{

	regions_MVA.insert( std::pair<std::string, std::vector<std::string> >("ALL", new_pvlq_ana_1l_MVA_5jin));
		
	std::vector<RegInfo*> list_reg_MVA{};

	fillRegInfo(m_inDir, list_reg_MVA, m_siglist, m_bkglist[0], regions_MVA.at("ALL"), m_variable, m_bin);

	MakePlot(list_reg_MVA, m_siglist, m_outName+"_yield_pvlq_1L_MVA", "SBySqrtB", "S/#sqrt{B}", 
		 false, true, false, true,
                 "SByB", "S/B", true, false, true, false);

	if(m_doLatexTable){
          MakeLaTeXTable(list_reg_MVA, m_outName+"_table_pvlq_1L_MVA.tex");
	}


      }

      //    }

  }

  return 0;

}
