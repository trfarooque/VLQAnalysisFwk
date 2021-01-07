#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

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

std::string m_variable;
std::string m_massPointLabel;
std::string m_doOldAnalysis;
double m_lumi;
double m_siglumi;
double m_bin;

//__________________________________________________________________________________________________
//
struct SigInfo{
  
  std::string key;
  std::string label;
  Color_t color;
  TFile* infile;

  SigInfo(const std::string& _key, const std::string& _label, Color_t _color, TFile* _infile){
    key = _key;
    label = _label;
    color = _color;
    infile = _infile;
  }

};

std::vector<TFile*> m_fbkg;
std::vector<SigInfo*> m_siglist;

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

//__________________________________________________________________________________________________
//
std::string get_blabel(const std::string& btitle){

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

//__________________________________________________________________________________________________
//
std::string get_boostlabel(const std::string& boosttitle){

  std::string boostlabel = "";

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
  else if( boosttitle ==  "2Hin0Vin0LTex") {boostlabel = "#geq2H, #geq0V, 0LT, 0T"; }
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

//__________________________________________________________________________________________________
//
RegInfo* FillRegionInfo(const std::string& leptitle, const std::string& btitle, const std::string& boosttitle ){
  
  std::string jettitle = (leptitle == "c1lep") ? "6jin" : "7jin";
  std::string jetlabel = (leptitle == "c1lep") ? "#geq6j" : "#geq7j";

  std::string blabel = get_blabel(btitle);
  std::string boostlabel = get_boostlabel(boosttitle);

  std::string _name = "";

  if( (boosttitle == "0Tex0Hex") || ( boosttitle == "0Tex1Hex") || ( boosttitle == "1Tex0Hex") || 
      ( boosttitle == "1Tex1Hex") || ( boosttitle == "2Tin0_1Hwin") || ( boosttitle == "0Tin2Hin")){
    _name = leptitle + boosttitle + jettitle + btitle + "_"  + m_variable;
  }
  else{
    _name = leptitle + jettitle + btitle + boosttitle + "_" + m_variable;
  }

  std::string _binlabel = boostlabel + ", " + jetlabel + ", " + blabel;

  std::map<std::string, double> _list_sbyb = {};
  std::map<std::string, double> _list_sbysqb = {};

  std::map<std::string, double> _list_sigStatUnc = {};
  std::map<std::string, double> _list_bkgdStatUnc = {};

  TFile* f_bkgd = m_fbkg.at(0);
  TH1D* h_bkgd = NULL;

  std::cout << "Opening background histogram " << _name << std::endl;

  h_bkgd = (TH1D*)(f_bkgd->Get(_name.c_str()));

  // make the bin value an input
  int bin_1TeV = h_bkgd->FindBin(m_bin);
  double bkgdStatUnc = 0;
  double yield_bkgd = h_bkgd->IntegralAndError(bin_1TeV, -1, bkgdStatUnc)*m_lumi;

  for(unsigned int i = 0; i < m_siglist.size(); i++){ // signal loop

    SigInfo* siginfo = m_siglist.at(i);

    std::string _sName = "";

    if( (boosttitle == "0Tex0Hex") || ( boosttitle == "0Tex1Hex") || ( boosttitle == "1Tex0Hex") ||
	( boosttitle == "1Tex1Hex") || ( boosttitle == "2Tin0_1Hwin") || ( boosttitle == "0Tin2Hin")){
      _sName = leptitle + boosttitle + jettitle + btitle + "_vlq" + std::to_string(i+1) + "_" + m_variable;
    }
    else{
      _sName = leptitle + jettitle + btitle + boosttitle + "_vlq" + std::to_string(i+1) + "_" + m_variable;
    }

    TH1D* h_sig = NULL;

    std::cout << "Opening signal histogram " << _sName << std::endl;

    h_sig = (TH1D*)((siginfo->infile)->Get(_sName.c_str()));

    double sigStatUnc = 0;
    // make signal luminosity an input. Default should be the same as background m_lumi
    double yield_sig = h_sig->IntegralAndError(bin_1TeV, -1, sigStatUnc)*m_siglumi;
    
    double SbyB_sig    = (yield_bkgd > 0.) ? yield_sig/yield_bkgd : 0.;
    double SbySqB_sig  = (yield_bkgd > 0.) ? yield_sig/sqrt(yield_bkgd) : 0.;

    _list_sbyb.insert( std::pair<std::string, double>(siginfo->key, SbyB_sig) );
    _list_sbysqb.insert( std::pair<std::string, double>(siginfo->key, SbySqB_sig) );

    _list_sigStatUnc.insert( std::pair<std::string, double>(siginfo->key, sigStatUnc) );
    _list_bkgdStatUnc.insert( std::pair<std::string, double>(siginfo->key, bkgdStatUnc) );

  } // signal loop
 
  RegInfo* regSet = new RegInfo( _name, _binlabel, _list_sbyb, _list_sbysqb, _list_sigStatUnc, _list_bkgdStatUnc );

  return regSet;

}

//__________________________________________________________________________________________________
//
TStyle* AtlasStyle()
{

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
void SetAtlasStyle ()
{

  static TStyle* atlasStyle = 0;
  std::cout << "\nApplying ATLAS style settings...\n" << std::endl ;
  if ( atlasStyle==0 ) atlasStyle = AtlasStyle();
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();

}

//__________________________________________________________________________________________________
//
void MakePlot(std::vector<RegInfo*> list_reg, std::vector<RegInfo*> list_reg1L, std::string out_name, std::string yTitle1, std::string yTitle2, bool DOYIELD){
  
  int nbins = list_reg.size();
  int nbins_1L = list_reg1L.size();

  double frac1L = ((double)nbins_1L)/((double)nbins);
  double stretch = 1.37;
  double stretch_log = 1.E3;
  
  THStack hs_sbyb_hist;
  THStack hs_sbysqb_hist;

  TLegend* leg_sbysqb = new TLegend(0.65, 0.6, 0.85, 0.85);

  leg_sbysqb->SetFillColor(0);
  leg_sbysqb->SetFillStyle(0);
  leg_sbysqb->SetLineColor(0);
  leg_sbysqb->SetLineStyle(0);
  leg_sbysqb->SetLineWidth(0);
  //leg_sbysqb->SetMargin(margin);                                                                                                                             
  leg_sbysqb->SetBorderSize(0);
  leg_sbysqb->SetTextSize(0.060);
  leg_sbysqb->SetTextFont(42);
  leg_sbysqb->SetShadowColor(0);
  
  int i_sig=0;
  for(SigInfo* siginfo : m_siglist){
    
    TH1D* h_sbyb = new TH1D( ("h_sbyb_"+out_name+"_"+siginfo->key).c_str(), "", nbins, 0, nbins);
    TH1D* h_sbysqb = new TH1D( ("h_sbysqb_"+out_name+"_"+siginfo->key).c_str(), "", nbins, 0, nbins);

    for(int ibin = 1; ibin <= nbins; ibin++){

      RegInfo* reg = list_reg.at(ibin-1);

      if(DOYIELD){
	h_sbyb->SetBinContent(ibin, reg->list_sbyb[siginfo->key]);
	h_sbysqb->SetBinContent(ibin, reg->list_sbysqb[siginfo->key]);
      }
      else{
	h_sbyb->SetBinContent(ibin, reg->list_bkgdStatUnc[siginfo->key]);
        h_sbysqb->SetBinContent(ibin, reg->list_sigStatUnc[siginfo->key]);
      }

      h_sbyb->GetXaxis()->SetBinLabel(ibin, (reg->binlabel).c_str());
      h_sbyb->GetXaxis()->ChangeLabel(ibin, 120., -1., -1, -1, -1, (reg->binlabel).c_str());
      //h_sbysqb->GetXaxis()->SetBinLabel(ibin, (reg->binlabel).c_str());                                                                                               

    }

    h_sbyb->SetLineColor(siginfo->color);
    h_sbysqb->SetLineColor(siginfo->color);

    h_sbyb->SetLineWidth(2);
    h_sbysqb->SetLineWidth(2);

    if(i_sig==0){
      h_sbyb->SetLineStyle(1);
      h_sbysqb->SetLineStyle(1);
    }
    else{
      h_sbyb->SetLineStyle(2);
      h_sbysqb->SetLineStyle(2);
    }

    hs_sbyb_hist.Add( h_sbyb );
    hs_sbysqb_hist.Add( h_sbysqb );

    leg_sbysqb->AddEntry(h_sbysqb, (siginfo->label).c_str(), "f");

    i_sig++;

  }

  double orig_max = hs_sbysqb_hist.GetMaximum("nostack");
  hs_sbyb_hist.SetMaximum(stretch*hs_sbyb_hist.GetMaximum("nostack") );
  hs_sbysqb_hist.SetMaximum(stretch*orig_max);

  TCanvas* canv = new TCanvas(out_name.c_str(), "", 1000, 700);
  canv->Divide(1,2);

  canv->cd(1)->SetPad(0,0.6,1,1);
  canv->cd(1)->SetTicks();
  canv->cd(1)->SetTopMargin(0.05);
  canv->cd(1)->SetBottomMargin(0.);
  canv->cd(1)->SetLeftMargin(0.14);
  canv->cd(1)->SetRightMargin(0.05);
  canv->cd(1)->SetFrameBorderMode(0);
  canv->cd(1)->SetFillStyle(4000);

  canv->cd(2)->SetPad(0,0,1,0.6);
  canv->cd(2)->SetTicks();
  canv->cd(2)->SetTopMargin(0.0);
  canv->cd(2)->SetBottomMargin(0.48);
  canv->cd(2)->SetLeftMargin(0.14);
  canv->cd(2)->SetRightMargin(0.05);
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

  ttlbox->AddText("#splitline{#scale[1.2]{#bf{#it{ATLAS}} Simulation}}{#sqrt{s} = 13 TeV, 139.0 fb^{-1}}");

  hs_sbysqb_hist.Draw("nostack");
  leg_sbysqb->Draw();
  ttlbox->Draw();

  hs_sbysqb_hist.GetHistogram()->GetYaxis()->SetTitle(yTitle1.c_str());
  hs_sbysqb_hist.GetHistogram()->GetYaxis()->SetLabelSize(0.07);
  hs_sbysqb_hist.GetHistogram()->GetYaxis()->SetTitleSize(0.09);
  hs_sbysqb_hist.GetHistogram()->GetYaxis()->SetTitleOffset(0.5);
  hs_sbysqb_hist.GetHistogram()->GetYaxis()->SetTickLength(0.009);

  hs_sbysqb_hist.GetHistogram()->GetYaxis()->CenterTitle();

  hs_sbysqb_hist.GetHistogram()->GetXaxis()->SetLabelSize(0.0);
  hs_sbysqb_hist.GetHistogram()->GetXaxis()->SetTitleSize(0.0);
  hs_sbysqb_hist.GetHistogram()->GetXaxis()->SetTickLength(0.009);

  TLine* lnref_sbysqb = new TLine(nbins_1L, hs_sbysqb_hist.GetHistogram()->GetMinimum() , nbins_1L, orig_max-0.5);
  lnref_sbysqb->Draw();

  double fracmax = 1. / stretch;

  TPaveText* box1L = new TPaveText(nbins_1L-3.5, orig_max-0.75, nbins_1L-0.5, orig_max-0.5, "NB");
  box1L->SetTextAlign(12); //horizontally centred; top-aligned                                                                                                          
  box1L->SetFillColor(0);
  box1L->SetFillStyle(0);
  box1L->SetLineColor(0);
  box1L->SetLineStyle(0);
  box1L->SetLineWidth(0);
  box1L->SetBorderSize(0);
  box1L->SetTextSize(0.07);
  box1L->SetTextFont(42);
  box1L->SetShadowColor(0);

  box1L->AddText("1-lepton");
  box1L->Draw();

  canv->cd(1)->Update();
  canv->cd(1)->Modified();
  canv->cd(2);

  hs_sbyb_hist.Draw("nostack");

  hs_sbyb_hist.GetHistogram()->GetYaxis()->SetTitle(yTitle2.c_str());
  hs_sbyb_hist.GetHistogram()->GetXaxis()->SetLabelSize(0.07);
  hs_sbyb_hist.GetHistogram()->GetXaxis()->SetLabelOffset(0.02);
  hs_sbyb_hist.GetHistogram()->GetYaxis()->SetLabelSize(0.05);
  hs_sbyb_hist.GetHistogram()->GetYaxis()->SetTitleSize(0.06);
  hs_sbyb_hist.GetHistogram()->GetYaxis()->SetTitleOffset(0.7);
  hs_sbyb_hist.GetHistogram()->GetYaxis()->CenterTitle();
  hs_sbyb_hist.GetHistogram()->GetXaxis()->SetTickLength(0.015);
  hs_sbyb_hist.GetHistogram()->GetYaxis()->SetTickLength(0.015);

  TLine* lnref_sbyb = new TLine(nbins_1L, hs_sbyb_hist.GetHistogram()->GetMinimum() , nbins_1L, hs_sbyb_hist.GetHistogram()->GetMaximum());
  lnref_sbyb->Draw();

  TLine line(0.01,1,0.2,1);
  line.SetLineColor(kWhite);
  line.SetLineWidth(25);                                                             
  line.DrawLineNDC(0.07,1,0.139,1);

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

  SetAtlasStyle();

  m_variable = "meff";
  m_massPointLabel = "";
  m_doOldAnalysis = "";
  m_lumi = 139000;
  m_siglumi = 139000;
  m_bin = 1000.;

  std::string sig_file = "";
  std::string bkgd_file = "";
  std::string out_name = "";

  for(int i = 1; i < argc; i++){

    std::string opt(argv[i]);

    std::string argument;

    std::string value;

    size_t pos = opt.find("=");

    argument = opt.substr(0, pos);

    value = opt.erase(0, pos + 1);

    std::transform(argument.begin(), argument.end(), argument.begin(), toupper);

    if(argument == "--SIGFILE") sig_file = value;
    else if(argument == "--BKGFILE") bkgd_file = value;
    else if(argument == "--OUTNAME") out_name = value;
    else if(argument == "--SIGLUMI") m_siglumi = std::stod(value);
    else if(argument == "--VARIABLE") m_variable = value;
    else if(argument == "--VARBIN") m_bin = std::stod(value);
    else if(argument == "--MASSPOINT") m_massPointLabel = value;
    else if(argument == "--DOOLDREGIONS") m_doOldAnalysis = value;
    else{ std::cout << "WARNING: Option " << argument << " not recognized." << std::endl; }

  }

  m_fbkg = {};
  std::cout << "Opening background file: " << std::endl;
  std::cout << bkgd_file << std::endl;
  m_fbkg.push_back(TFile::Open(bkgd_file.c_str(), "READ"));
  
  m_siglist = {};
  
  std::cout << "Opening signal file: " << std::endl;
  std::cout << sig_file << std::endl;

  m_siglist.push_back( new SigInfo("HtHt", "TT#rightarrowHtHt "+m_massPointLabel, kGreen+2, TFile::Open(sig_file.c_str(), "READ") ) );
  m_siglist.push_back( new SigInfo("HtWb", "TT#rightarrowHtWb "+m_massPointLabel, kBlue+1, TFile::Open(sig_file.c_str(), "READ") ) );
  m_siglist.push_back( new SigInfo("HtZt", "TT#rightarrowHtZt "+m_massPointLabel, kOrange+7, TFile::Open(sig_file.c_str(), "READ") ) );
  m_siglist.push_back( new SigInfo("WbWb", "TT#rightarrowWbWb "+m_massPointLabel, kRed+1, TFile::Open(sig_file.c_str(), "READ") ) );
  m_siglist.push_back( new SigInfo("WbZt", "TT#rightarrowWbZt "+m_massPointLabel, kMagenta+3, TFile::Open(sig_file.c_str(), "READ") ) );
  m_siglist.push_back( new SigInfo("ZtZt", "TT#rightarrowZtZt "+m_massPointLabel, kCyan-3, TFile::Open(sig_file.c_str(), "READ") ) );

  std::vector<std::string> oldAnalysis =
    {
      "0Tex0Hex", "0Tex1Hex", "1Tex0Hex", "1Tex1Hex", "2Tin0_1Hwin", "0Tin2Hin"
    };

  std::vector<std::string> fullboost =
    {
      "0Hex0Vin0LTex", "0Hex0Vin1LTin", "1Hex0Vex0LTex", "1Hex0Vex1Lin0Tex", "1Hex1Vin1Lin0Tex", "1Hex1Vin0LTex",
      "1Hex0Vin0Lex1Tin", "1Hex0Vin1Lin1Tin", "2Hin0Vin1Lin1Tin", "2Hin0Vin0Lex1Tin", "2Hin0Vin1Lin0Tex", "2Hin0Vin0LTex"
    };

  std::map<std::string, std::vector<std::string> > regions_1L_3bex ={};
  std::map<std::string, std::vector<std::string> > regions_1L_4bin ={};

  std::transform(m_doOldAnalysis.begin(), m_doOldAnalysis.end(), m_doOldAnalysis.begin(), toupper);

  if(m_doOldAnalysis  == "TRUE"){
    regions_1L_3bex.insert( std::pair<std::string, std::vector<std::string> >("3bex",oldAnalysis));
    regions_1L_4bin.insert( std::pair<std::string, std::vector<std::string> >("4bin",oldAnalysis));
  }
  else{
    regions_1L_3bex.insert( std::pair<std::string, std::vector<std::string> >("3bex",fullboost));
    regions_1L_4bin.insert( std::pair<std::string, std::vector<std::string> >("4bin",fullboost));
  }

  std::vector<RegInfo*> list_reg1L_3bex{};
  std::vector<RegInfo*> list_reg1L_4bin{};


  for(const std::string& region : regions_1L_3bex.at("3bex")){
    list_reg1L_3bex.push_back ( FillRegionInfo("c1lep","3bex",region) );
  }

  for(const std::string& region : regions_1L_4bin.at("4bin")){
    list_reg1L_4bin.push_back ( FillRegionInfo("c1lep","4bin",region) );
  }

  std::vector<RegInfo*> list_reg_3bex{};
  list_reg_3bex.insert(list_reg_3bex.end(), list_reg1L_3bex.begin(), list_reg1L_3bex.end());

  std::vector<RegInfo*> list_reg_4bin{};
  list_reg_4bin.insert(list_reg_4bin.end(), list_reg1L_4bin.begin(), list_reg1L_4bin.end());

  MakePlot(list_reg_3bex, list_reg1L_3bex, out_name+"_yield_3bex", "S/#sqrt{B}", "S/B", true);

  MakePlot(list_reg_3bex, list_reg1L_3bex, out_name+"_statUncertainty_3bex", "S stat. unc.", "B stat. unc.", false);
  
  MakePlot(list_reg_4bin, list_reg1L_4bin, out_name+"_yield_4bin", "S/#sqrt{B}", "S/B", true);

  MakePlot(list_reg_4bin, list_reg1L_4bin, out_name+"_statUncertainty_4bin", "S stat. unc.", "B stat. unc.", false);

  return 0;

}
