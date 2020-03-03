#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "TFile.h"
#include "TH1.h"
#include "TKey.h"

/*
  The command line options are described below individually. An example command line is:
  MakeQCDFile --sourceDir ${sourceDir} --outName "QCD0L.root" --splitMtb 1 \
  --slope -8.62553 --slopeLM -8.59532 --slopeHM -9.02948 --minDphi 0.4 --boundedUp 0 --oneVar meff 

  where ${sourceDir} is the directory containing the QCD templates
*/

int main(int argc, char** argv){

  std::string sourceDir = "";
  std::string outName  = "";
  std::string oneVar = "";

  bool splitMtb = true;
  double slope = 1.;
  double slopeLM = 1.;
  double slopeHM = 1.;
  double slope_0bex = 1.;
  double slope_1bex = 1.;

  double minDphi = 0.;
  bool boundedUp = false;

  int optind = 1;
  std::stringstream ss;
  std::string _switch = "";

  while ((optind < argc) && (argv[optind][0]=='-')) {
    _switch = std::string(argv[optind]);
    optind++;  ss << argv[optind];
    //std::cout<<" _switch = "<<_switch<<" ss = "<<ss.str()<<std::endl;
    if       (_switch == "--sourceDir")   { ss >> sourceDir; } //Name of source directory containing QCD template
    else if  (_switch == "--outName")     { ss >> outName; }   //Name of output QCD file
    else if  (_switch == "--oneVar")      { ss >> oneVar; }    //Name of a single variable to reweight. All histograms are reweighted if this is empty
    else if  (_switch == "--splitMtb")    { ss >> splitMtb; }  //use different scales for high- and low-mtb regions
    else if  (_switch == "--slope")       { ss >> slope; }     //Common slope parameter
    else if  (_switch == "--slopeLM")     { ss >> slopeLM; }   //Slope parameter for LM regions
    else if  (_switch == "--slopeHM")     { ss >> slopeHM; }   //Slope parameter for HM regions
    else if  (_switch == "--slope_0bex")  { ss >> slope_0bex; }     //Slope parameter in 0bex regions
    else if  (_switch == "--slope_1bex")  { ss >> slope_1bex; }     //Slope parameter in 1bex regions
    else if  (_switch == "--minDphi")     { ss >> minDphi; }   //Minimum dPhimin in target region
    else if  (_switch == "--boundedUp")   { ss >> boundedUp; } //dPhimin interval is bounded from above (by 0.1)

    else std::cout<<"Unknown switch "<<_switch<<std::endl;

    optind++; ss.clear(); _switch.clear();
  }

  std::cout<<" sourceDir = "<<sourceDir<<std::endl;
  std::cout<<" outName = "<<outName<<std::endl;
  std::cout<<" oneVar = "<<oneVar<<std::endl;
  std::cout<<" splitMtb = "<<splitMtb<<std::endl;
  std::cout<<" slope = "<<slope<<std::endl;
  std::cout<<" slopeLM = "<<slopeLM<<std::endl;
  std::cout<<" slopeHM = "<<slopeHM<<std::endl;
  std::cout<<" slope_0bex = "<<slope_0bex<<std::endl;
  std::cout<<" slope_1bex = "<<slope_1bex<<std::endl;
  std::cout<<" minDphi = "<<minDphi<<std::endl;
  std::cout<<" boundedUp = "<<boundedUp<<std::endl;

  bool fail = false;
  if(sourceDir.empty()){ std::cerr << "Source directory containing QCD template must be provided (--sourceDir)" << std::endl; fail=true; }
  if(outName.empty()){ outName = "QCD.root"; }
  if(slope >= 1.){  std::cerr << " No slope parameter provided " << std::endl; fail = true; }
  if(splitMtb){
    if(slopeLM >= 1.){  std::cerr << " No slope parameter provided for LM regions" << std::endl; fail = true; }
    if(slopeHM >= 1.){  std::cerr << " No slope parameter provided for HM regions" << std::endl; fail = true; }
  }
  else{
    slopeLM = slope;
    slopeHM = slope; 
  }


  if(fail) { return -1; }

  TFile* data_file = TFile::Open( (sourceDir+"outVLQAnalysis_Data_TOPQ4_nominal_HIST.root").c_str(), "READ");
  TFile* bkg_file  = TFile::Open( (sourceDir+"outVLQAnalysis_Backgrounds_nominal_HIST.root").c_str(), "READ");
  TFile* out_file  = TFile::Open( outName.c_str(), "RECREATE");

  double lumi = 36074.56;

  double sc_QCD = exp(slope*minDphi);
  double sc_QCD_0bex = exp(slope_0bex*minDphi);
  double sc_QCD_1bex = exp(slope_1bex*minDphi);
  double sc_QCD_LM = exp(slopeLM*minDphi);
  double sc_QCD_HM = exp(slopeHM*minDphi);

  if(!boundedUp){ 
    sc_QCD /= 1 - exp(slope*0.1);
    sc_QCD_0bex /= 1 - exp(slope_0bex*0.1);
    sc_QCD_1bex /= 1 - exp(slope_1bex*0.1);
    sc_QCD_LM /= 1 - exp(slopeLM*0.1);
    sc_QCD_HM /= 1 - exp(slopeHM*0.1);
  }

  TIter next(data_file->GetListOfKeys());
  TKey* key = NULL;
  
  std::string className = "";
  std::string histName = "";

  while( (key = (TKey*)next()) ){

    className = (std::string)(key->GetClassName());
    if(className.find("TH1D") == std::string::npos){ className.clear(); continue; }

    histName = (std::string)(key->GetName());
    if( (histName.find("cutFlow") != std::string::npos) || (histName.find("rejectionMask") != std::string::npos) || (histName.find("vlqType") != std::string::npos) ){continue;}

    if( !oneVar.empty() && (histName.find(oneVar)==std::string::npos) ){continue;}
    //if( !oneVar.empty() && histName.find("meff") == std::string::npos){continue;}

    className.clear();

    //std::cout<<histName<<std::endl;
    TH1D* outhist = (TH1D*)(((key->ReadObj()))->Clone( (histName).c_str()));
    outhist->SetDirectory(0);
    outhist->Add( (TH1D*)(bkg_file->Get(histName.c_str())), -1.*lumi);
    double sc = 1.;
    if(histName.find("0bex") != std::string::npos)         { sc = sc_QCD_0bex; }
    else if(histName.find("1bex") != std::string::npos)    { sc = sc_QCD_1bex; }
    else if(histName.find("LowMtbin") != std::string::npos){ sc = sc_QCD_LM; }
    else if(histName.find("HighMtbin") != std::string::npos){ sc = sc_QCD_HM; }
    else { sc = sc_QCD; }
    outhist->Scale(sc);

    out_file->cd();
    outhist->Write();
    delete outhist;

  }//Loop over keys

  data_file->Close();
  bkg_file->Close();
  out_file->Close();

  return 0;

}
