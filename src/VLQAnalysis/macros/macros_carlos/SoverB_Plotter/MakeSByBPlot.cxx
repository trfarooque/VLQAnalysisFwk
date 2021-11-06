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

// Structure to store the information for each signal and background sample
//__________________________________________________________________________________________________
//
struct SigInfo{
  
  std::string key;
  std::string label;
  Color_t color;
  std::string infile_mc16a;
  std::string infile_mc16d;
  std::string infile_mc16e;
  double mc16a_scale;
  double mc16d_scale;
  double mc16e_scale;
  std::string suffix;

  SigInfo(const std::string& _key, const std::string& _label, std::string& _color, 
	  std::string& _infile_mc16a, std::string& _infile_mc16d, std::string& _infile_mc16e, 
	  std::string& _suffix, double _mc16a_scale = 36207.66, double _mc16d_scale = 44037.4, double _mc16e_scale = 58450.1){

    key = _key;
    label = _label;
    infile_mc16a = _infile_mc16a;
    infile_mc16d = _infile_mc16d;
    infile_mc16e = _infile_mc16e;
    mc16a_scale = _mc16a_scale;
    mc16d_scale = _mc16d_scale;
    mc16e_scale = _mc16e_scale;
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
    std::string infile_mc16a= "";
    std::string infile_mc16d = "";
    std::string infile_mc16e = "";
    std::string suffix = "";
    double mc16a_scale = 36207.66;
    double mc16d_scale = 44037.4;
    double mc16e_scale = 58450.1;

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
	else if(opt == "SUFFIX") suffix = val;
	else if(opt == "COLOR") color = val;
	else if(opt == "MC16A_SCALE") mc16a_scale = std::stod(val);
	else if(opt == "MC16D_SCALE") mc16d_scale = std::stod(val);
	else if(opt == "MC16E_SCALE") mc16e_scale = std::stod(val);
      }

      siglist.push_back( new SigInfo(key, label, color, infile_mc16a, infile_mc16d, infile_mc16e, suffix, mc16a_scale, mc16d_scale, mc16e_scale));
      
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

	  tmp_label += (*it != 'L' && *it != 'T') ? std::string(1,*it) : ( (*it == 'L') ? "t_{l}" : "t_{h}" );

	  if(next(it) != comp.end()) tmp_label+="+";

	  ++it;

	}

	tmp_label += ")";

      }
      // j, b, fj, H, V, T, L
      else{
	
	tmp_label += (comp != "L" && comp != "T") ? comp : ( (comp == "L") ? "t_{l}" : "t_{h}");

      }

      //label += ", ";

      labelComponents.push_back(tmp_label);
      
    }

    tmp_region = match.suffix().str();
    
  }

  if(region.find("HighMtbmin") != std::string::npos) labelComponents.push_back("HM");
  else if(region.find("LowMtbmin") != std::string::npos) labelComponents.push_back("LM");
  else if(region.find("HighMVAScore") != std::string::npos) labelComponents.push_back("HMVA");
  else if(region.find("MidMVAScore") != std::string::npos) labelComponents.push_back("MMVA");
  else if(region.find("LowMVAScore") != std::string::npos) labelComponents.push_back("LMVA");

  std::string label = "";

  for(unsigned int i = 0; i < labelComponents.size()-1; i++) label += labelComponents[i]+", ";

  label += labelComponents[labelComponents.size()-1];

  return label;

}

// Function that fills a vector of RegInfo with the relevant information
//__________________________________________________________________________________________________
//
void fillRegInfo(std::vector< RegInfo* >& reglist, std::vector< SigInfo* >& siglist, SigInfo* &bkginfo, std::vector< std::string >& regions, std::string& variable, double varBin){

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

  for(unsigned int i = 0; i < siglist.size(); i++){
   
    SigInfo* siginfo = siglist.at(i);
 
    TFile* sig_mc16a = TFile::Open((siginfo->infile_mc16a).c_str(), "READ");
    TFile* sig_mc16d = TFile::Open((siginfo->infile_mc16d).c_str(), "READ");
    TFile* sig_mc16e = TFile::Open((siginfo->infile_mc16e).c_str(), "READ");

    TFile* bkg_mc16a = TFile::Open((bkginfo->infile_mc16a).c_str(), "READ");
    TFile* bkg_mc16d = TFile::Open((bkginfo->infile_mc16d).c_str(), "READ");
    TFile* bkg_mc16e = TFile::Open((bkginfo->infile_mc16e).c_str(), "READ");
    
    for(unsigned int j = 0; j < regions.size(); j++){

      std::cout << "Processing region " << regions[j] << std::endl;

      std::string bkg_hist_name = regions[j] + "_" + variable;

      std::string sig_hist_name = (siginfo->suffix) == "" ? regions[j] + "_" + variable : regions[j] + "_" + (siginfo->suffix) + "_" + variable;

      TH1D* bkg_mc16a_hist = (TH1D*)(bkg_mc16a->Get(bkg_hist_name.c_str()));
      TH1D* bkg_mc16d_hist = (TH1D*)(bkg_mc16d->Get(bkg_hist_name.c_str()));
      TH1D* bkg_mc16e_hist = (TH1D*)(bkg_mc16e->Get(bkg_hist_name.c_str()));

      TH1D* sig_mc16a_hist = (TH1D*)(sig_mc16a->Get(sig_hist_name.c_str()));
      TH1D* sig_mc16d_hist = (TH1D*)(sig_mc16d->Get(sig_hist_name.c_str()));
      TH1D* sig_mc16e_hist = (TH1D*)(sig_mc16e->Get(sig_hist_name.c_str()));
      
      bkg_mc16a_hist->Scale(bkginfo->mc16a_scale);
      bkg_mc16d_hist->Scale(bkginfo->mc16d_scale);
      bkg_mc16e_hist->Scale(bkginfo->mc16e_scale);

      sig_mc16a_hist->Scale(siginfo->mc16a_scale);
      sig_mc16d_hist->Scale(siginfo->mc16d_scale);
      sig_mc16e_hist->Scale(siginfo->mc16e_scale);

      bkg_mc16a_hist->Add(bkg_mc16d_hist);
      bkg_mc16a_hist->Add(bkg_mc16e_hist);
      
      sig_mc16a_hist->Add(sig_mc16d_hist);
      sig_mc16a_hist->Add(sig_mc16e_hist);
      
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

    bkg_mc16a->Close();
    bkg_mc16d->Close();
    bkg_mc16e->Close();

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

  std::string m_variable = "meff";
  double m_bin = 1000.;
  std::string m_sigSampleFile = "";
  std::string m_bkgSampleFile = "";
  std::string m_outName = "test.root";
  bool m_doOldAnalysis = false;
  bool m_do0Lepton = false;
  bool m_doSVLQAnalysis = false;
  bool m_doLatexTable = false;

  for(int i = 1; i < argc; i++){

    std::string opt(argv[i]);

    std::string argument;

    std::string value;

    size_t pos = opt.find("=");

    argument = opt.substr(0, pos);

    value = opt.erase(0, pos + 1);

    std::transform(argument.begin(), argument.end(), argument.begin(), toupper);

    if(argument == "--SIGSAMPLEFILE") m_sigSampleFile = value;
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
    else if(argument == "--DOSVLQ"){
      std::transform(value.begin(), value.end(), value.begin(), toupper);
      m_doSVLQAnalysis = (value == "TRUE");
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
  
  //v----------------------Second Iteration-----------------------v
  /*std::vector<std::string> new_pvlq_ana_0l_0Hex =
    {
      "c0lep7jin2bex0Hex1VTexHighMtbmin", "c0lep7jin2bex0Hex1Vex1TinHighMtbmin", "c0lep7jin2bex0Hex0Vex2TexHighMtbmin", "c0lep7jin2bex0Hex2Vin0TinHighMtbmin",
      "c0lep7jin3bin0Hex1VTexHighMtbmin", "c0lep7jin3bin0Hex1VTexLowMtbmin", "c0lep7jin3bin0Hex1Vex1TinHighMtbmin", "c0lep7jin3bin0Hex1Vex1TinLowMtbmin", 
      "c0lep7jin3bin0Hex0Vex2TexHighMtbmin", "c0lep7jin3bin0Hex0Vex2TexLowMtbmin", "c0lep7jin3bin0Hex2Vin0Tin"
    };
  
  std::vector<std::string> new_pvlq_ana_0l_1Hex =
    {
      "c0lep7jin2bex1Hex0VTexHighMtbmin", "c0lep7jin2bex1Hex0VTexLowMtbmin", "c0lep7jin2bex1Hex1Vex0Tex", "c0lep7jin2bex1Hex2Vin0Tex", 
      "c0lep7jin2bex1Hex1Vin1TexHighMtbmin", "c0lep7jin2bex1Hex1Vin1TexLowMtbmin", "c0lep7jin2bex1Hex0Vin2TinHighMtbmin", "c0lep7jin2bex1Hex0Vin2TinLowMtbmin",
      "c0lep7jin3bex1Hex0VTexHighMtbmin", "c0lep7jin3bex1Hex0VTexLowMtbmin", "c0lep7jin3bex1Hex1Vin0Tex", "c0lep7jin3bin1Hex1Vin1Tex", "c0lep7jin3bex1Hex0Vin2Tin",
      "c0lep7jin4bin1Hex0VTex", "c0lep7jin4bin1Hex1Vin0Tex", "c0lep7jin4bin1Hex0Vin2Tin"
    };

  std::vector<std::string> new_pvlq_ana_0l_2Hin =
    {
      "c0lep7jin2bex2Hin0Vin0TinHighMtbmin", "c0lep7jin2bex2Hin0Vin0TinLowMtbmin", "c0lep7jin3bin2Hin0Vin0TinHighMtbmin", "c0lep7jin3bin2Hin0Vin0TinLowMtbmin"
    };*/
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

  std::vector< std::string > new_pvlq_ana_1l_MVA_3bex = 
    {
      "c1lep6jin3bex2Min3JinHighMVAScore", "c1lep6jin3bex2Min3Jin0HexHighMVAScore", "c1lep6jin3bex2Min3Jin1HinHighMVAScore",
      "c1lep6jin3bex2Min3JinMidMVAScore",  "c1lep6jin3bex2Min3Jin0HexMidMVAScore",  "c1lep6jin3bex2Min3Jin1HinMidMVAScore",
      "c1lep6jin3bex2Min3JinLowMVAScore",  "c1lep6jin3bex2Min3Jin0HexLowMVAScore",  "c1lep6jin3bex2Min3Jin1HinLowMVAScore"
    };
  
  std::vector< std::string > new_pvlq_ana_1l_MVA_4bin =
    {
      "c1lep6jin4bin2Min3JinHighMVAScore", "c1lep6jin4bin2Min3Jin0HexHighMVAScore", "c1lep6jin4bin2Min3Jin1HinHighMVAScore",
      "c1lep6jin4bin2Min3JinMidMVAScore",  "c1lep6jin4bin2Min3Jin0HexMidMVAScore",  "c1lep6jin4bin2Min3Jin1HinMidMVAScore",
      "c1lep6jin4bin2Min3JinLowMVAScore",  "c1lep6jin4bin2Min3Jin0HexLowMVAScore",  "c1lep6jin4bin2Min3Jin1HinLowMVAScore"
    };

  /*std::vector<std::string> new_pvlq_ana_1l_unmerged =
    {
      "0Hex0Vin0LTex", "0Hex0Vin1LTin", "1Hex0Vex0LTex", "1Hex0Vex1Lin0Tex", "1Hex1Vin1Lin0Tex", "1Hex1Vin0LTex",
      "1Hex0Vin0Lex1Tin", "1Hex0Vin1Lin1Tin", "2Hin0Vin1Lin1Tin", "2Hin0Vin0Lex1Tin", "2Hin0Vin1Lin0Tex", "2Hin0Vin0LTex"
    };

  std::vector<std::string> new_pvlq_ana_1l__merged_v1 =
    {
      "0Hex0Vin0LTex", "0Hex0Vin1LTin", "1Hex0Vex0LTex", "1Hex0Vex1Lin0Tex", "1Hex1Vin0Lin0Tex",
      "1Hex0Vin0Lex1Tin", "1Hex0Vin1Lin1Tin", "2Hin0Vin1LTin", "2Hin0Vin0LTex"
      };*/

  // Single VLQ Analysis 1-lepton Regions
  std::vector< std::string > svlq_ana_1l_1bex = 
    {
      "c1lep3_5jwin1bex1fjin0LTex0Hex1Vin", "c1lep3_5jwin1bex1fjin0Tex1Lin0Hex1Vin",
      "c1lep6jin1bex1fjin1Lex0Tex0Hex1Vin", "c1lep6jin1bex1fjin0Lex1Tex0Hex1Vin", "c1lep6jin1bex1fjin2LTin0Hex1Vin"
    };
  
  std::vector< std::string > svlq_ana_1l_2bex =
    {
      "c1lep3_5jwin2bex1fjin0LTex0Hex1Vin", "c1lep3_5jwin2bex1fjin0Tex1Lin0Hex1Vin",
      "c1lep6jin2bex1fjin1Lex0Tex0Hex1Vin", "c1lep6jin2bex1fjin0Lex1Tex0Hex1Vin", "c1lep6jin2bex1fjin2LTin0Hex1Vin"
    };

  std::vector< std::string > svlq_ana_1l_3bex =
    {
      "c1lep3_5jwin3bex1fjin0LTex1Hin0Vex", "c1lep3_5jwin3bex1fjin0Tex1Lin1Hin0Vex", "c1lep3_5jwin3bex1fjin1Tin0Lex1Hin0Vex",
      "c1lep6jin3bex1fjin1Lex0VTex1Hin", "c1lep6jin3bex1fjin0Lex1VTex1Hin", "c1lep6jin3bex1fjin2VLTin1Hin"
    };

  std::vector<std::string> svlq_ana_1l_4bin =
    {
      "c1lep3_5jwin4bin1fjin0LTex1Hin0Vex", "c1lep3_5jwin4bin1fjin0Tex1Lin1Hin0Vex", 
      "c1lep3_5jwin4bin1fjin1Tin0Lex1Hin0Vex", "c1lep3_5jwin4bin0fjex1Lin0VTex0Hex",
      "c1lep6jin4bin1fjin1Lex0VTex1Hin", "c1lep6jin4bin1fjin0Lex1VTex1Hin",
      "c1lep6jin4bin1fjin2VLTin1Hin", "c1lep6jin4bin0fjex1Lin0VTex0Hex"
    };
    

  std::vector<std::string> test_reg =
    {
      "c0lep7jin4bin0Hex0Vin1TinHighMtbmin", "c0lep7jin4bin1Hex0Vex0TexHighMtbmin",
      "c0lep7jin4bin1Hex0Vex1TexHighMtbmin", "c0lep7jin4bin1Hex1Vin1TexHighMtbmin",
      "c0lep7jin4bin1Hex0Vin2TinHighMtbmin"
    };


  std::map<std::string, std::vector<std::string> > regions_1bex = {};
  std::map<std::string, std::vector<std::string> > regions_2bex = {};
  std::map<std::string, std::vector<std::string> > regions_3bex = {};
  std::map<std::string, std::vector<std::string> > regions_4bin = {};

  if(m_doSVLQAnalysis){

    regions_1bex.insert(std::pair<std::string, std::vector<std::string > >("1bex", svlq_ana_1l_1bex));
    regions_2bex.insert(std::pair<std::string, std::vector<std::string > >("2bex", svlq_ana_1l_2bex));
    regions_3bex.insert(std::pair<std::string, std::vector<std::string > >("3bex", svlq_ana_1l_3bex));
    regions_4bin.insert(std::pair<std::string, std::vector<std::string > >("4bin", svlq_ana_1l_4bin));

    std::vector<RegInfo*> list_reg_1bex{};
    std::vector<RegInfo*> list_reg_2bex{};
    std::vector<RegInfo*> list_reg_3bex{};
    std::vector<RegInfo*> list_reg_4bin{};
    
    fillRegInfo(list_reg_1bex, m_siglist, m_bkglist[0], regions_1bex.at("1bex"), m_variable, m_bin);
    fillRegInfo(list_reg_2bex, m_siglist, m_bkglist[0], regions_2bex.at("2bex"), m_variable, m_bin);
    fillRegInfo(list_reg_3bex, m_siglist, m_bkglist[0], regions_3bex.at("3bex"), m_variable, m_bin);
    fillRegInfo(list_reg_4bin, m_siglist, m_bkglist[0], regions_4bin.at("4bin"), m_variable, m_bin);

    MakePlot(list_reg_1bex, m_siglist, m_outName+"_yield_svlq_1bex", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
	     "SByB", "S/B", true, false, true, false);
    MakePlot(list_reg_2bex, m_siglist, m_outName+"_yield_svlq_2bex", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
             "SByB", "S/B", true, false, true, false);
    MakePlot(list_reg_3bex, m_siglist, m_outName+"_yield_svlq_3bex", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
             "SByB", "S/B", true, false, true, false);
    MakePlot(list_reg_4bin, m_siglist, m_outName+"_yield_svlq_4bin", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
             "SByB", "S/B", true, false, true, false);

  }
  else{

    if(m_doOldAnalysis){
      if(m_do0Lepton){

	regions_2bex.insert( std::pair<std::string, std::vector<std::string > >("2bex", old_pvlq_ana_0l_2bex));
	regions_3bex.insert( std::pair<std::string, std::vector<std::string > >("3bex", old_pvlq_ana_0l_3bex));
	regions_4bin.insert( std::pair<std::string, std::vector<std::string > >("4bin", old_pvlq_ana_0l_4bin));
	
	std::vector<RegInfo*> list_reg_2bex{};
	std::vector<RegInfo*> list_reg_3bex{};
	std::vector<RegInfo*> list_reg_4bin{};
	
	fillRegInfo(list_reg_2bex, m_siglist, m_bkglist[0],regions_2bex.at("2bex"), m_variable, m_bin);
	fillRegInfo(list_reg_3bex, m_siglist, m_bkglist[0],regions_3bex.at("3bex"), m_variable, m_bin);
	fillRegInfo(list_reg_4bin, m_siglist, m_bkglist[0],regions_4bin.at("4bin"), m_variable, m_bin);

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
	
	fillRegInfo(list_reg_3bex, m_siglist, m_bkglist[0], regions_3bex.at("3bex"), m_variable, m_bin);
	fillRegInfo(list_reg_4bin, m_siglist, m_bkglist[0], regions_4bin.at("4bin"), m_variable, m_bin);
	
	MakePlot(list_reg_3bex, m_siglist, m_outName+"_yield_pvlq_1L_3bex", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);
	MakePlot(list_reg_4bin, m_siglist, m_outName+"_yield_pvlq_1L_4bin", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);

      }
    }
    else{
      if(m_do0Lepton){
	regions_2bex.insert( std::pair<std::string, std::vector<std::string > >("0Hex",  new_pvlq_ana_0l_0Hex));
	regions_3bex.insert( std::pair<std::string, std::vector<std::string > >("1Hex",  new_pvlq_ana_0l_1Hex));
	regions_3bex.insert( std::pair<std::string, std::vector<std::string > >("1Hex_mtbmin_split", new_pvlq_ana_0l_1Hex_mtbmin_split));
	regions_3bex.insert( std::pair<std::string, std::vector<std::string > >("1Hex_2VTin_split", new_pvlq_ana_0l_1Hex_2VTin_split));
	regions_3bex.insert( std::pair<std::string, std::vector<std::string > >("1Hex_opt2", new_pvlq_ana_0l_1Hex_opt2));
	regions_3bex.insert( std::pair<std::string, std::vector<std::string > >("1Hex_opt3", new_pvlq_ana_0l_1Hex_opt3));
	regions_4bin.insert( std::pair<std::string, std::vector<std::string > >("2Hin",  new_pvlq_ana_0l_2Hin));

	std::vector<RegInfo*> list_reg_0Hex{};
	std::vector<RegInfo*> list_reg_1Hex{};
	std::vector<RegInfo*> list_reg_1Hex_mtbmin_split{};
	std::vector<RegInfo*> list_reg_1Hex_2VTin_split{};
	std::vector<RegInfo*> list_reg_1Hex_opt2{};
	std::vector<RegInfo*> list_reg_1Hex_opt3{};
	std::vector<RegInfo*> list_reg_2Hin{};

	fillRegInfo(list_reg_0Hex, m_siglist, m_bkglist[0], regions_2bex.at("0Hex"), m_variable, m_bin);
	fillRegInfo(list_reg_1Hex, m_siglist, m_bkglist[0], regions_3bex.at("1Hex"), m_variable, m_bin);
	fillRegInfo(list_reg_1Hex_mtbmin_split, m_siglist, m_bkglist[0], regions_3bex.at("1Hex_mtbmin_split"), m_variable, m_bin);
	fillRegInfo(list_reg_1Hex_2VTin_split, m_siglist, m_bkglist[0], regions_3bex.at("1Hex_2VTin_split"), m_variable, m_bin);
	fillRegInfo(list_reg_1Hex_opt2, m_siglist, m_bkglist[0], regions_3bex.at("1Hex_opt2"), m_variable, m_bin);
	fillRegInfo(list_reg_1Hex_opt3, m_siglist, m_bkglist[0], regions_3bex.at("1Hex_opt3"), m_variable, m_bin);
	fillRegInfo(list_reg_2Hin, m_siglist, m_bkglist[0], regions_4bin.at("2Hin"), m_variable, m_bin);
	
	MakePlot(list_reg_0Hex, m_siglist, m_outName+"_yield_0Hex", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);
        MakePlot(list_reg_1Hex, m_siglist, m_outName+"_yield_1Hex", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);
	MakePlot(list_reg_1Hex_mtbmin_split, m_siglist, m_outName+"_yield_1Hex_mtbmin_split", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);
	MakePlot(list_reg_1Hex_2VTin_split, m_siglist, m_outName+"_yield_1Hex_2VTin_split", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);
	MakePlot(list_reg_1Hex_opt2, m_siglist, m_outName+"_yield_1Hex_opt2", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);
	MakePlot(list_reg_1Hex_opt3, m_siglist, m_outName+"_yield_1Hex_opt3", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);
        MakePlot(list_reg_2Hin, m_siglist, m_outName+"_yield_2Hin", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);

	if(m_doLatexTable){
	  MakeLaTeXTable(list_reg_0Hex, m_outName+"_table_0Hex.tex");
	  MakeLaTeXTable(list_reg_1Hex, m_outName+"_table_1Hex.tex");
	  MakeLaTeXTable(list_reg_1Hex_mtbmin_split, m_outName+"_table_1Hex_mtbmin_split.tex");
	  MakeLaTeXTable(list_reg_1Hex_2VTin_split, m_outName+"_table_1Hex_2VTin_split.tex");
	  MakeLaTeXTable(list_reg_1Hex_opt2, m_outName+"_table_1Hex_opt2.tex");
	  MakeLaTeXTable(list_reg_1Hex_opt3, m_outName+"_table_1Hex_opt3.tex");
	  MakeLaTeXTable(list_reg_2Hin, m_outName+"_table_2Hin.tex");
	}

      }
      else{
	regions_3bex.insert( std::pair<std::string, std::vector<std::string> >("3bex", new_pvlq_ana_1l_3bex));
	regions_4bin.insert( std::pair<std::string, std::vector<std::string> >("4bin", new_pvlq_ana_1l_4bin));
	//regions_3bex.insert( std::pair<std::string, std::vector<std::string> >("3bex",fullboost_merged_v1)); 
	//regions_4bin.insert( std::pair<std::string, std::vector<std::string> >("4bin",fullboost_merged_v1));
	regions_3bex.insert( std::pair<std::string, std::vector<std::string> >("MVA_3bex", new_pvlq_ana_1l_MVA_3bex));
	regions_4bin.insert( std::pair<std::string, std::vector<std::string> >("MVA_4bin", new_pvlq_ana_1l_MVA_4bin));
	
	
	std::vector<RegInfo*> list_reg_3bex{};
	std::vector<RegInfo*> list_reg_4bin{};

	std::vector<RegInfo*> list_reg_MVA_3bex{};
	std::vector<RegInfo*> list_reg_MVA_4bin{};
	
	fillRegInfo(list_reg_3bex, m_siglist, m_bkglist[0], regions_3bex.at("3bex"), m_variable, m_bin);
	fillRegInfo(list_reg_4bin, m_siglist, m_bkglist[0], regions_4bin.at("4bin"), m_variable, m_bin);

	fillRegInfo(list_reg_MVA_3bex, m_siglist, m_bkglist[0], regions_3bex.at("MVA_3bex"), m_variable, m_bin);
        fillRegInfo(list_reg_MVA_4bin, m_siglist, m_bkglist[0], regions_4bin.at("MVA_4bin"), m_variable, m_bin);

	MakePlot(list_reg_3bex, m_siglist, m_outName+"_yield_pvlq_1L_3bex", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);
	MakePlot(list_reg_4bin, m_siglist, m_outName+"_yield_pvlq_1L_4bin", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);

	MakePlot(list_reg_MVA_3bex, m_siglist, m_outName+"_yield_pvlq_1L_MVA_3bex", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);
        MakePlot(list_reg_MVA_4bin, m_siglist, m_outName+"_yield_pvlq_1L_MVA_4bin", "SBySqrtB", "S/#sqrt{B}", false, true, false, true,
                 "SByB", "S/B", true, false, true, false);

	if(m_doLatexTable){
          MakeLaTeXTable(list_reg_3bex, m_outName+"_table_3bex.tex");
          MakeLaTeXTable(list_reg_4bin, m_outName+"_table_4bin.tex");
	  
	  MakeLaTeXTable(list_reg_MVA_3bex, m_outName+"_table_MVA_3bex.tex");
          MakeLaTeXTable(list_reg_MVA_4bin, m_outName+"_table_MVA_4bin.tex");
        }

      }

    }

  }

  return 0;

}
