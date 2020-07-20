#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "TFile.h"
#include "TMath.h"
#include "TH1.h"
#include "TH2D.h"
#include "TF1.h"

std::string inDir;
std::string outName;
std::string signalSamples;
std::string backgroundSamples;
std::string systematicsList;
bool doSmoothing = false;
bool debug = false;

// Utility function for splitting input string with a given delimiter. Each substring
// is stored in an output vector
//______________________________________________________________________________
//
std::vector<std::string> splitString(std::string input, std::string delimiter){

  std::vector<std::string> stringCollection = {};

  size_t pos = 0;
  
  std::string token;

  while( (pos = input.find(delimiter)) != std::string::npos){

    token = input.substr(0,pos);
    input.erase(0, pos + delimiter.length());
    stringCollection.push_back(token);
    //std::cout << token << std::endl;


  }

  for(unsigned i = 0; i < stringCollection.size(); i++){
    
    std::cout << "Sample i = " << i << " " << stringCollection.at(i) << std::endl;

  }

  return stringCollection;

}

// Function that adds the 2D histograms from a collection of samples and scales them 
// with the corresponding luminosity of the MC campaign
//______________________________________________________________________________
//
TH2D* getTotalKinHisto2D( std::vector<std::string>* samples, std::string inputDir, std::string region, 
			  std::vector<std::string>* campaigns, std::vector<std::string>* kinematics, TH2D* histTemp,
			  std::string nameHisto, std::string systematicPrefix){

  TH2D* totalHist = (TH2D*)(histTemp -> Clone ( ( std::string(histTemp -> GetName()) + "_" + std::string(nameHisto) ).c_str() ) );

  totalHist -> Reset();

  for(const std::string& sample : *samples){
    
    std::string sampleName = sample;

    if(sample == "ttbarbb" || sample == "ttbarcc" || sample == "ttbarlight" || sample == "SingletopWtprod"){

      if(systematicPrefix == "_aMCPy" || systematicPrefix == "_PowHer" || systematicPrefix == "_AFII"){
	
	sampleName += systematicPrefix;

      }
      else if(systematicPrefix == "_DiagSub"){
	
	sampleName += systematicPrefix;

      } 
      else if(systematicPrefix != ""){

	sampleName += "_pmg";

      }
      
    }
    else if(sample == "Zjets"){
      
      if(systematicPrefix != ""){

	sampleName += "_pmg";

      }

    }


    std::string systPrex = systematicPrefix;

    for(const std::string& campaign : *campaigns){

      if(sample == "ttH" && campaign == "merged_mc16e") continue;

      if(sampleName == "SingletopWtprod_pmg" && systematicPrefix == "_weight_pmg_muR10__muF20") systPrex = "_weight_pmg_muR100__muF200";
      if(sampleName == "SingletopWtprod_pmg" && systematicPrefix == "_weight_pmg_muR10__muF05") systPrex = "_weight_pmg_muR100__muF050";
      if(sampleName == "SingletopWtprod_pmg" && systematicPrefix == "_weight_pmg_muR20__muF10") systPrex = "_weight_pmg_muR200__muF100";
      if(sampleName == "SingletopWtprod_pmg" && systematicPrefix == "_weight_pmg_muR05__muF10") systPrex = "_weight_pmg_muR050__muF100";

      if(debug){
	std::cout << "Opening: " << inputDir + "/" + campaign + "/outVLQAnalysis_" + sampleName + "_nominal_HIST.root" << std::endl;
	std::cout << "Retrieving: " << region + "_" + (kinematics -> at(0)) + "_vs_" + (kinematics -> at(1)) + systPrex << std::endl;
      }

      TFile* f = TFile::Open( (inputDir + "/" + campaign + "/outVLQAnalysis_" + sampleName + "_nominal_HIST.root").c_str(), "READ" );

      TH2D* h_i = ((TH2D*)(f -> Get( ( std::string(region) + "_" + (kinematics -> at(0)) + "_vs_" + (kinematics -> at(1)) + systPrex ).c_str() ) ) );

      if(!h_i){
	h_i = ((TH2D*)(f -> Get( ( std::string(region) + "_" + (kinematics -> at(0)) + "_vs_" + (kinematics -> at(1)) ).c_str() ) ) );
	if(debug) std::cout << region + "_" + (kinematics -> at(0)) + "_vs_" + (kinematics -> at(1)) + systPrex << " not found." << std::endl;
      }

      h_i -> SetDirectory(0);

      if(campaign == "merged_mc16a"){
	h_i -> Scale(36207.66);
      }
      else if(campaign == "merged_mc16d"){
	h_i -> Scale(44307.4);
      }
      else if(campaign == "merged_mc16e"){
	h_i -> Scale(58450.1);
      }
      else{
	std::cout << "MC campaign unrecognized, unable to scale histogram with campaign luminosity" << std::endl;
      }

      totalHist -> Add(h_i);
      delete h_i;
      f -> Close();
    }

  }

  return totalHist;


}

//______________________________________________________________________________
//
TH1D* getTotalKinHisto1D( std::vector<std::string>* samples, std::string inputDir, std::string region,
                          std::vector<std::string>* campaigns, std::string kinematics, TH1D* histTemp,
                          std::string nameHisto, std::string systematicPrefix){

  TH1D* totalHist = (TH1D*)(histTemp -> Clone ( ( std::string(histTemp -> GetName()) + "_" + std::string(nameHisto) ).c_str() ) );

  totalHist -> Reset();

  for(const std::string& sample : *samples){

    std::string sampleName = sample;

    if(sample == "ttbarbb" || sample == "ttbarcc" || sample == "ttbarlight" || sample == "SingletopWtprod"){


      if(systematicPrefix == "_aMCPy" || systematicPrefix == "_PowHer" || systematicPrefix == "_AFII"){
	
	sampleName += systematicPrefix;

      }
      else if(systematicPrefix == "_DiagSub"){
	
	sampleName += systematicPrefix;

      }
      else if(systematicPrefix != ""){

	sampleName += "_pmg";

      }

    }
    else if(sample == "Zjets"){
      
      if(systematicPrefix != ""){

	sampleName += "_pmg";

      }

    }

    for(const std::string& campaign : *campaigns){

      if(sample == "ttH" && campaign == "merged_mc16e") continue;

      if(sample == "SingletopWtprod_pmg" && systematicPrefix == "_weight_pmg_muR10__muF20") systematicPrefix = "_weight_pmg_muR100__muF200";
      if(sample == "SingletopWtprod_pmg" && systematicPrefix == "_weight_pmg_muR10__muF05") systematicPrefix = "_weight_pmg_muR100__muF050";
      if(sample == "SingletopWtprod_pmg" && systematicPrefix == "_weight_pmg_muR20__muF10") systematicPrefix = "_weight_pmg_muR200__muF100";
      if(sample == "SingletopWtprod_pmg" && systematicPrefix == "_weight_pmg_muR05__muF10") systematicPrefix = "_weight_pmg_muR050__muF100";

      TFile* f = TFile::Open( (inputDir + "/" + campaign + "/outVLQAnalysis_" + sampleName + "_nominal_HIST.root").c_str(), "READ" );

      TH1D* h_i = ((TH1D*)(f -> Get( ( std::string(region) + "_" + kinematics + systematicPrefix ).c_str() ) ) );

      /*if(debug){
	std::cout << inputDir + "/" + campaign + "/outVLQAnalysis_" + sampleName + "_nominal_HIST.root" << std::endl;
	std::cout << region + "_" + kinematics + systematicPrefix << std::endl;
      }*/

      if(!h_i){
	h_i = ((TH1D*)(f -> Get( ( std::string(region) + "_" + kinematics ).c_str() ) ) );
      }

      h_i -> SetDirectory(0);
                                                                                                                                                                                                         
      if(campaign == "merged_mc16a"){
        h_i -> Scale(36207.66);
      }
      else if(campaign == "merged_mc16d"){
        h_i -> Scale(44307.4);
      }
      else if(campaign == "merged_mc16e"){
        h_i -> Scale(58450.1);
      }
      else{
	std::cout << "MC campaign unrecognized, unable to scale histogram with campaign luminosity" << std::endl;
      }

      totalHist -> Add(h_i);
      delete h_i;
      f -> Close();
    }

  }

  return totalHist;

}

//______________________________________________________________________________ 
//
void parseUserOptions(int arc, char** argv){
  
  for(int i = 1; i < arc; i++){

    std::string opt(argv[i]);

    std::string argument;

    std::string value;

    size_t pos = opt.find("=");

    argument = opt.substr(0, pos);

    value=opt.erase(0, pos + 1);

    if(argument == "--INDIR") inDir = value;
    else if(argument == "--OUTPUT") outName = value;
    else if(argument == "--SIGNAL") signalSamples = value;
    else if(argument == "--BACKGROUND") backgroundSamples = value;
    else if(argument == "--SYSTEMATICS") systematicsList = value;
    else if(argument == "--DOSMOOTHING"){
      std::transform(value.begin(), value.end(), value.begin(), toupper);
      doSmoothing = (value == "TRUE") ? true : false;
    }
    else if(argument == "--DEBUG"){

      std::transform(value.begin(), value.end(), value.begin(), toupper);
      debug = (value == "TRUE") ? true : false;
    }
  }
  
}

//______________________________________________________________________________
//
Double_t hyperbolaFitFunction(Double_t *x, Double_t *par){
  
  Double_t fitVal = par[0] + par[1]/(TMath::Power(x[0],par[2]));

  return fitVal;

}

//______________________________________________________________________________
//
Double_t sigmoidFitFunction(Double_t *x, Double_t *par){

  //Double_t fitVal = par[0]/(par[1] + TMath::Exp( par[2]*x[0]) );

  Double_t fitVal = par[0] - (par[1]/(1.0 + TMath::Exp( par[2]*x[0] ) ) );

  return fitVal;

}

Double_t sechFitFunction(Double_t *x, Double_t *par){

  Double_t fitVal = par[0] + par[1]/(TMath::CosH(par[2]*x[0]) );
  //Double_t fitVal = par[0]/(TMath::CosH( TMath::Power(par[1]*x[0],par[2]) ) );


  return fitVal;

}

Double_t asinhFitFunction(Double_t *x, Double_t*par){

  //Double_t fitVal = par[0] + par[2]*TMath::ASinH(-par[1]*x[0] );
  
  //Double_t fitVal = -par[0]*TMath::Power(x[0],2.0) - TMath::ASinH(par[1]*x[0] );

  Double_t fitVal = -(par[0] + TMath::Power(par[1]*x[0],3.0) + TMath::ASinH(par[1]*x[0]));

  return fitVal;

}


// Main function that calculates the jets_n reweighting for the given samples to be 
// reweighted, applies it to meff and then calculates the meff reweighting for the samples
// to be reweighted
//______________________________________________________________________________
//
int main(int argc, char** argv){

  parseUserOptions(argc, argv);

  std::cout << " inDir = " << inDir << std::endl;
  std::cout << " outName = " << outName << std::endl;
  std::cout << " signalSamples = " << signalSamples << std::endl;
  std::cout << " backgroundSamples = " << backgroundSamples << std::endl;
  std::cout << " systematicsList = " << systematicsList << std::endl;
  std::cout << " doSmoothing = " << doSmoothing << std::endl;
  std::cout << " debug = "<< debug << std::endl;

  TFile* f_out = TFile::Open( outName.c_str(), "UPDATE" );


  // Store all the samples to be reweighted
  std::vector<std::string> list_signal = splitString(signalSamples, ",");
  
  // Store all the background samples
  std::vector<std::string> list_background = splitString(backgroundSamples, ",");
  
  std::vector<std::string> list_campaign = {"merged_mc16a","merged_mc16d","merged_mc16e"};
  
  // Vector containing the kinematics to be reweighted
  std::vector<std::string> list_kin = {"jets_n","meffred"};
  //std::vector<std::string> list_kin = {"jets_n","meff"};

  std::vector<std::string> list_reg{};

  std::ifstream systFile(systematicsList);

  std::vector<std::string> systematics;

  if(!systFile.is_open()){
    std::cout << "Couldn't find the systematics file: " << systematicsList << std::endl;
    std::cout << "Will continue running the program using nominal systematics only." << std::endl;
    systematics.push_back("nominal");
  }
  else{
    std::string systematic;

    while( getline(systFile, systematic) ){
      
      std::cout << "Found systematic: " << systematic << std::endl;

      systematics.push_back(systematic);
      
    }

    systFile.close();

  }
 

  // If we are reweighting any ttbar sample or singletopWt use the following regions

  if(std::find(list_signal.begin(), list_signal.end(), "ttbarbb") != list_signal.end() || 
     std::find(list_signal.begin(), list_signal.end(), "ttbarbb_pmg") != list_signal.end()){

    list_reg.push_back("c1lep3jin2bex");

  }

  // If we are reweighting Zjets use the following regions
  if(std::find(list_signal.begin(), list_signal.end(), "Zjets") != list_signal.end()){    

    list_reg.push_back("c2lep3jin1bexZwinMLL_sf");

  }


  // Define rebinning for meff

  double binedges_meff[14] = {0,100,400,500,600,700,800,900,1000,1200,1400,1600,2000,5000};
  //double binedges_meff[15] = {-200,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,5000};
  double* rebin_meff = &binedges_meff[0];
  int nbins_meff = 13;

  //double binedges_meffred[15] = {-200,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,5000};
  double binedges_meffred[14] = {0,100,400,500,600,700,800,900,1000,1200,1400,1600,2000,5000}; 
  double* rebin_meffred = &binedges_meffred[0];
  int nbins_meffred = 13;

  //double binedges_jets_n[10] = {-0.5, 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 15.5};
  double binedges_jets_n[15] = {-0.5, 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5, 11.5, 12.5, 15.5};
  double* rebin_jets_n = &binedges_jets_n[0];
  int nbins_jets_n = 14;

  for(const std::string& region : list_reg){ // Start loop through regions 

    for(const std::string& systematic : systematics){ // Start loop through systematics

      std::cout << "########################################################################" << std::endl;
      std::cout << "Running systematic: " << systematic << std::endl;
      std::cout << "########################################################################" << std::endl;
      
      std::string systematicPrefix = (systematic == "nominal") ? "" : "_" + systematic;


      TFile* f_data = TFile::Open( (inDir + "/mergedData/outVLQAnalysis_Data_nominal_HIST.root").c_str(), "READ" );

      // Retrieve jets_n and meff 2D histogram                                                                                                                                                          
      TH2D* data_hist = (TH2D*)(f_data -> Get( ( (region + "_" + list_kin[0] + "_vs_" + list_kin[1]).c_str() ) ) );

      data_hist -> SetDirectory(0);

      f_data -> Close();

      TH2D* signal_hist = new TH2D();

      TH2D* background_hist = new TH2D();

      TH1D* jets_n_rw = ( (TH2D*)(data_hist -> Clone(data_hist -> GetName() ) ) ) -> ProjectionY();

      TH1D* jets_n_MC_ratio = ( (TH2D*)(data_hist -> Clone(data_hist -> GetName() ) ) ) -> ProjectionY();

      jets_n_rw -> SetName( (region + "_jets_n"+systematicPrefix).c_str() );

      jets_n_rw = (TH1D*) jets_n_rw->Rebin(nbins_jets_n, jets_n_rw   -> GetName(), rebin_jets_n);

      jets_n_MC_ratio -> SetName( (region + "_jets_n"+systematicPrefix).c_str() );

      jets_n_MC_ratio = (TH1D*) jets_n_MC_ratio->Rebin(nbins_jets_n, jets_n_MC_ratio   -> GetName(), rebin_jets_n);

      *signal_hist = *getTotalKinHisto2D(&list_signal, inDir, region, &list_campaign, &list_kin, data_hist, "signal", "");

      *background_hist = *getTotalKinHisto2D(&list_background, inDir, region, &list_campaign, &list_kin, data_hist, "background", "");

      TH2D* numeratorHist = (TH2D*)(data_hist -> Clone ( "numerator" ) );

      numeratorHist->Reset();

      numeratorHist -> Add(data_hist, background_hist, 1.0, -1.0);

      TH1D* numerator_jets_n = numeratorHist -> ProjectionY();

      TH1D* denominator_jets_n = signal_hist -> ProjectionY();

      numerator_jets_n   = (TH1D*) numerator_jets_n   -> Rebin(nbins_jets_n, numerator_jets_n   -> GetName(), rebin_jets_n);

      denominator_jets_n = (TH1D*) denominator_jets_n -> Rebin(nbins_jets_n, denominator_jets_n -> GetName(), rebin_jets_n);

      jets_n_rw -> Divide(numerator_jets_n, denominator_jets_n);

      if(systematic != "nominal"){

	signal_hist->Reset();
	
	background_hist->Reset();

	*signal_hist = *getTotalKinHisto2D(&list_signal, inDir, region, &list_campaign, &list_kin, data_hist, "signal", systematicPrefix);

	*background_hist = *getTotalKinHisto2D(&list_signal, inDir, region, &list_campaign, &list_kin, data_hist, "background", "");
	
	TH1D* numerator_jets_n_MC_ratio = background_hist -> ProjectionY();

	TH1D* denominator_jets_n_MC_ratio = signal_hist -> ProjectionY();

	numerator_jets_n_MC_ratio   = (TH1D*) numerator_jets_n_MC_ratio   -> Rebin(nbins_jets_n, numerator_jets_n_MC_ratio   -> GetName(), rebin_jets_n);

	denominator_jets_n_MC_ratio = (TH1D*) denominator_jets_n_MC_ratio -> Rebin(nbins_jets_n, denominator_jets_n_MC_ratio -> GetName(), rebin_jets_n);
	
	jets_n_MC_ratio -> Divide(numerator_jets_n_MC_ratio, denominator_jets_n_MC_ratio);

	f_out->cd();

	jets_n_MC_ratio -> Write();

      }
      else{

	f_out->cd();

	jets_n_rw -> Write();

      }

      if(list_kin[1] == "meffred" && region == "c2lep3jin1bexZwinMLL_sf"){

	TH1D* meffred_rw = new TH1D( (region + "_" + list_kin[1] + systematicPrefix).c_str(), "", nbins_meffred, binedges_meffred);
	
	if(systematic != "nominal"){

	  signal_hist->Reset();

	  *signal_hist = *getTotalKinHisto2D(&list_signal, inDir, region, &list_campaign, &list_kin, data_hist, "signal", systematicPrefix);

	  background_hist->Reset();

          *background_hist = *getTotalKinHisto2D(&list_signal, inDir, region, &list_campaign, &list_kin, data_hist, "background", "");

	  for(int ybin = 4; ybin <= signal_hist -> GetNbinsY(); ybin++){

            for(int xbin = 1; xbin <= signal_hist -> GetNbinsX(); xbin++){

              signal_hist->SetBinContent(xbin, ybin, (jets_n_rw -> GetBinContent(ybin)) * (jets_n_MC_ratio -> GetBinContent(ybin)) * (signal_hist->GetBinContent(xbin, ybin)) );

	      background_hist->SetBinContent(xbin, ybin, (jets_n_rw -> GetBinContent(ybin)) * (background_hist->GetBinContent(xbin, ybin)) );

            }

	  }

	  TH1D* background_hist_meffred = ( (TH2D*)(background_hist -> Clone(background_hist -> GetName() ) ) ) -> ProjectionX();

	  TH1D* signal_hist_meffred = ( (TH2D*)(signal_hist -> Clone(signal_hist -> GetName() ) ) ) -> ProjectionX();

	  background_hist_meffred = (TH1D*) background_hist_meffred -> Rebin( nbins_meffred, background_hist_meffred -> GetName(), rebin_meffred);

          signal_hist_meffred = (TH1D*) signal_hist_meffred -> Rebin( nbins_meffred, signal_hist_meffred -> GetName(), rebin_meffred);

	  background_hist_meffred->Sumw2(kFALSE);

	  meffred_rw->Add(background_hist_meffred, 1.0);
	  
	  meffred_rw->Divide(signal_hist_meffred);

	}
	else{

	  for(int ybin = 4; ybin <= signal_hist -> GetNbinsY(); ybin++){

	    for(int xbin = 1; xbin <= signal_hist -> GetNbinsX(); xbin++){

	      signal_hist->SetBinContent(xbin, ybin, (jets_n_rw -> GetBinContent(ybin)) * (signal_hist->GetBinContent(xbin, ybin)) );

	    }

	  }

	  TH1D* data_hist_meffred = ( (TH2D*)(data_hist -> Clone(data_hist -> GetName() ) ) ) -> ProjectionX();

	  TH1D* background_hist_meffred = ( (TH2D*)(background_hist -> Clone(background_hist -> GetName() ) ) ) -> ProjectionX();

	  TH1D* signal_hist_meffred = ( (TH2D*)(signal_hist -> Clone(signal_hist -> GetName() ) ) ) -> ProjectionX();

	  data_hist_meffred = (TH1D*) data_hist_meffred -> Rebin( nbins_meffred, data_hist_meffred -> GetName(), rebin_meffred);

	  data_hist_meffred->Sumw2(kFALSE);

	  background_hist_meffred = (TH1D*) background_hist_meffred -> Rebin( nbins_meffred, background_hist_meffred -> GetName(), rebin_meffred);

	  signal_hist_meffred = (TH1D*) signal_hist_meffred -> Rebin( nbins_meffred, signal_hist_meffred -> GetName(), rebin_meffred);

	  meffred_rw -> Add(data_hist_meffred, background_hist_meffred, 1.0, -1.0);

	  meffred_rw -> Divide(signal_hist_meffred);

	  if(doSmoothing){

	    std::cout << "#######################################################################" << std::endl;
	    std::cout << "Fitting meffred reweighting histogram" << std::endl;

	    TF1* funcHyperbola = new TF1((region + "_" + list_kin[1] + "_Hyperbolafit" + systematicPrefix).c_str(), hyperbolaFitFunction, 0, 5000, 3);
	    TF1* funcSigmoid = new TF1((region + "_" + list_kin[1] + "_fit" + systematicPrefix).c_str(), sigmoidFitFunction, 0, 5000, 3);
	    TF1* funcSech = new TF1((region + "_" + list_kin[1] + "_Sechfit" + systematicPrefix).c_str(), sechFitFunction, 0, 5000, 3);
	    TF1* funcArcSinh = new TF1((region + "_" + list_kin[1] + "_ArcSinhfit" + systematicPrefix).c_str(), asinhFitFunction, -200, 0, 2);

	    //funcSigmoid->SetParameters(1.0,1.0,0.001);
	    funcSech->SetParameters(1.0,1.0,0.001);

	    funcSigmoid->SetParameters(1.15, 0.80, 0.001);

	    //funcArcSinh->SetParameters(1.0,0.01,0.8);
	    funcArcSinh->SetParameters(0.8,0.001);


	    //funcSech->SetParameters(1.15,0.001,2.5);

	    //meffred_rw->Fit(funcHyperbola, "M WW");
	    meffred_rw->Fit(funcSigmoid, "M WW R");
	    //meffred_rw->Fit(funcSech, "M WW");
	    //meffred_rw->Fit(funcArcSinh, "M WW");

	    //f_out->cd();
	    //funcHyperbola->Write();

	    f_out->cd();
	    funcSigmoid->Write();

	    //f_out->cd();
	    //funcSech->Write();

	    //f_out->cd();
	    //funcArcSinh->Write();

	  }

	}

        meffred_rw -> SetName( (region + "_" + list_kin[1] + systematicPrefix).c_str() );

        f_out -> cd();

        meffred_rw -> Write();

      }
      else if(list_kin[1] == "meffred" && region == "c1lep3jin2bex"){

	int nbins_jets_n_new = 8;
	double binedges_jets_n_new[9] = {-0.5, 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 15.5};
	
	TH2D* meffred_rw = new TH2D( (region + "_" + list_kin[1] + systematicPrefix).c_str(), "", nbins_meff, binedges_meff, nbins_jets_n_new, binedges_jets_n_new);

	if(systematic != "nominal"){

          signal_hist->Reset();

          *signal_hist = *getTotalKinHisto2D(&list_signal, inDir, region, &list_campaign, &list_kin, data_hist, "signal", systematicPrefix);

          background_hist->Reset();

          *background_hist = *getTotalKinHisto2D(&list_signal, inDir, region, &list_campaign, &list_kin, data_hist, "background", "");

	  for(int ybin = 4; ybin <= signal_hist -> GetNbinsY(); ybin++){

            for(int xbin = 1; xbin <= signal_hist -> GetNbinsX(); xbin++){

	      signal_hist->SetBinContent(xbin, ybin, (jets_n_rw -> GetBinContent(ybin)) * (jets_n_MC_ratio -> GetBinContent(ybin)) * (signal_hist->GetBinContent(xbin, ybin)) );

              background_hist->SetBinContent(xbin, ybin, (jets_n_rw -> GetBinContent(ybin)) * (background_hist->GetBinContent(xbin, ybin)) );

            }

          }

	  for(int ybin = 4; ybin <= meffred_rw -> GetNbinsY(); ybin++){

	    TH1D* signal_hist_meffred = ((TH2D*)(signal_hist -> Clone(signal_hist->GetName() ) ) ) -> ProjectionX();
	    signal_hist_meffred -> Reset();

	    TH1D* background_hist_meffred = ((TH2D*)(background_hist -> Clone(background_hist->GetName() ) ) ) -> ProjectionX();
	    background_hist_meffred -> Reset();

	    background_hist_meffred -> Sumw2(kFALSE);

	    if(ybin < meffred_rw -> GetNbinsY()){

	      for(int xbin = 1; xbin <= data_hist -> GetNbinsX(); xbin++){

		signal_hist_meffred -> SetBinContent(xbin, signal_hist -> GetBinContent(xbin,ybin));

		signal_hist -> SetBinContent(xbin,ybin,0);

		background_hist_meffred -> SetBinContent(xbin, background_hist -> GetBinContent(xbin,ybin));

		background_hist ->SetBinContent(xbin,ybin,0);

	      }

	    }
	    else{

	      signal_hist_meffred = ((TH2D*)(signal_hist -> Clone(signal_hist->GetName() ) ) ) -> ProjectionX();
	      
	      background_hist_meffred = ((TH2D*)(background_hist -> Clone(background_hist->GetName() ) ) ) -> ProjectionX();

	    }


	    signal_hist_meffred = (TH1D*) signal_hist_meffred -> Rebin( nbins_meff, signal_hist_meffred -> GetName(), rebin_meff);

	    background_hist_meffred = (TH1D*) background_hist_meffred -> Rebin( nbins_meff, background_hist_meffred -> GetName(), rebin_meff);

	    background_hist_meffred -> Divide(signal_hist_meffred);

	    for(int xbin = 1; xbin <= meffred_rw -> GetNbinsX(); xbin++){

	      meffred_rw -> SetBinContent(xbin, ybin, background_hist_meffred -> GetBinContent(xbin));

	    }

	  }

        }
	else{


	  for(int ybin = 4; ybin <= signal_hist -> GetNbinsY(); ybin++){
	    
	    for(int xbin = 1; xbin <= signal_hist -> GetNbinsX(); xbin++){
	      
	      signal_hist->SetBinContent(xbin, ybin, (jets_n_rw -> GetBinContent(ybin)) * (signal_hist->GetBinContent(xbin, ybin)) );
	      
	    }
	    
	  }
	  
	  for(int ybin = 4; ybin <= meffred_rw -> GetNbinsY(); ybin++){
	  
	    TH1D* data_hist_meffred = ((TH2D*)(data_hist -> Clone(data_hist->GetName() ) ) ) -> ProjectionX();
	    data_hist_meffred -> Reset();
	    
	    data_hist_meffred->Sumw2(kFALSE);
	    
	    TH1D* signal_hist_meffred = ((TH2D*)(signal_hist -> Clone(signal_hist->GetName() ) ) ) -> ProjectionX();
	    signal_hist_meffred -> Reset();
	    
	    TH1D* background_hist_meffred = ((TH2D*)(background_hist -> Clone(background_hist->GetName() ) ) ) -> ProjectionX();
	    background_hist_meffred -> Reset();	  
	    
	    if(ybin < meffred_rw -> GetNbinsY()){
	      
	      for(int xbin = 1; xbin <= data_hist -> GetNbinsX(); xbin++){ 
		
		data_hist_meffred -> SetBinContent(xbin, data_hist -> GetBinContent(xbin,ybin));
		
		data_hist -> SetBinContent(xbin,ybin,0);
		
		signal_hist_meffred -> SetBinContent(xbin, signal_hist -> GetBinContent(xbin,ybin));
		
		signal_hist -> SetBinContent(xbin,ybin,0);
	      
		background_hist_meffred -> SetBinContent(xbin, background_hist -> GetBinContent(xbin,ybin));
		
		background_hist ->SetBinContent(xbin,ybin,0);
	      
	      } 

	    }
	    else{
	      
	      data_hist_meffred = ((TH2D*)(data_hist -> Clone(data_hist->GetName() ) )) -> ProjectionX();
	      
	      data_hist_meffred->Sumw2(kFALSE);
	      
	      signal_hist_meffred = ((TH2D*)(signal_hist -> Clone(signal_hist->GetName() ) ) ) -> ProjectionX();
	      
	      background_hist_meffred = ((TH2D*)(background_hist -> Clone(background_hist->GetName() ) ) ) -> ProjectionX();
	      
	    }

	    data_hist_meffred->Sumw2(kTRUE);
	    
	    data_hist_meffred = (TH1D*) data_hist_meffred -> Rebin( nbins_meff, data_hist_meffred -> GetName(), rebin_meff);
	    
	    signal_hist_meffred = (TH1D*) signal_hist_meffred -> Rebin( nbins_meff, signal_hist_meffred -> GetName(), rebin_meff);
	    
	    background_hist_meffred = (TH1D*) background_hist_meffred -> Rebin( nbins_meff, background_hist_meffred -> GetName(), rebin_meff);
	    
	    data_hist_meffred -> Add(background_hist_meffred, -1.0);
	    
	    data_hist_meffred -> Divide(signal_hist_meffred);
	    
	    f_out->cd();

	    data_hist_meffred->SetName( (region + "_" + list_kin[1] + "_jet_" + std::to_string(ybin-1) + "_" + systematicPrefix).c_str() );
	    
	    data_hist_meffred->Write();
	    
	    if(doSmoothing){
	      
	      std::cout << "#######################################################################" << std::endl;
	      if(ybin < meffred_rw -> GetNbinsY()) std::cout << "Fitting meff reweighting histogram for njets = " << ybin-1 << std::endl;
	      else std::cout << "Fitting meffred reweighting histogram for njets >= " << ybin-1 << std::endl;
	      
	      TF1* funcHyperbola = new TF1((region + "_" + list_kin[1] + "_Hyperbolafit_jet_"+ std::to_string(ybin-1) + systematicPrefix).c_str(), hyperbolaFitFunction, 0, 5000, 3);
	      
	      TF1* funcSigmoid = new TF1((region + "_" + list_kin[1] + "_fit_jet_"+ std::to_string(ybin-1) + systematicPrefix).c_str(), sigmoidFitFunction, 0, 5000, 3);
	    
	      TF1* funcSech = new TF1((region + "_" + list_kin[1] + "_Sechfit_jet_" + std::to_string(ybin-1) + systematicPrefix).c_str(), sechFitFunction, 0, 5000, 3);

	      TF1* funcArcSinh = new TF1((region + "_" + list_kin[1] + "_ArcSinhfit_jet_" + std::to_string(ybin-1) + systematicPrefix).c_str(), asinhFitFunction, 0, 5000, 2);


	      funcSigmoid->SetParameters(1.15, 0.80, 0.001);
	      

	      funcSech->SetParameters(1.0,1.0,0.001);

	      //data_hist_meffred->Fit(funcHyperbola, "M WW");
	      
	      data_hist_meffred->Fit(funcSigmoid, "M WW R");
	      
	      //data_hist_meffred->Fit(funcSech, "M WW");
	      
	      //data_hist_meffred->Fit(funcArcSinh, "M WW");

	      //f_out->cd();
	      //funcHyperbola->Write();
	      
	      f_out->cd();
	      funcSigmoid->Write();

	      //f_out->cd();
              //funcSech->Write();

	      //f_out->cd();
	      //funcArcSinh->Write();

	    }
	    
	    for(int xbin = 1; xbin <= meffred_rw -> GetNbinsX(); xbin++){

	      meffred_rw -> SetBinContent(xbin, ybin, data_hist_meffred -> GetBinContent(xbin));
	      
	    } 

	  }

	}

        f_out -> cd();

        meffred_rw -> SetName( (region + "_" + list_kin[1] + systematicPrefix).c_str() );

        meffred_rw -> Write();
	
	if(debug){

          for(int ybin = 4; ybin <= meffred_rw -> GetNbinsY(); ybin++){

	    std::string test_region = Form("c1lep%ijex2bex", ybin-1);
	    if(ybin == meffred_rw -> GetNbinsY()) test_region = Form("c1lep%ijin2bex", ybin-1);

            TFile* f_data_test = TFile::Open( (inDir + "/mergedData/outVLQAnalysis_Data_nominal_HIST.root").c_str(), "READ" );
            TH1D* data_hist_test = (TH1D*)(f_data_test -> Get( ( (test_region + "_" + list_kin[1]).c_str() ) ) );
            data_hist_test -> SetDirectory(0);
            f_data_test -> Close();

	    TH1D* signal_hist_test = new TH1D();

	    TH1D* background_hist_test = new TH1D();

	    if(systematic != "nominal"){

	      *signal_hist_test = *getTotalKinHisto1D(&list_signal, inDir, test_region, &list_campaign, list_kin[1], data_hist_test, "signal", systematicPrefix);

	      *background_hist_test = *getTotalKinHisto1D(&list_signal, inDir, test_region, &list_campaign, list_kin[1], data_hist_test, "background", "");

	    }
	    
	    else{

	      *signal_hist_test = *getTotalKinHisto1D(&list_signal, inDir, test_region, &list_campaign, list_kin[1], data_hist_test, "signal", systematicPrefix);

	      *background_hist_test = *getTotalKinHisto1D(&list_background, inDir, test_region, &list_campaign, list_kin[1], data_hist_test, "background", systematicPrefix);

	    }

            data_hist_test = (TH1D*) data_hist_test -> Rebin( nbins_meff, data_hist_test -> GetName(), rebin_meff);
            signal_hist_test = (TH1D*) signal_hist_test -> Rebin( nbins_meff, signal_hist_test -> GetName(), rebin_meff);
            background_hist_test = (TH1D*) background_hist_test -> Rebin( nbins_meff, background_hist_test -> GetName(), rebin_meff);

            for(int xbin =1; xbin < meffred_rw -> GetNbinsX(); xbin++){

	      double weight = (systematic != "nominal") ? meffred_rw -> GetBinContent(xbin, ybin) : meffred_rw -> GetBinContent(xbin, ybin)*jets_n_rw -> GetBinContent(ybin);

              signal_hist_test -> SetBinContent(xbin, signal_hist_test -> GetBinContent(xbin)*weight);

            }

	    std::cout << "For jets_n = " << ybin -1 << std::endl;

	    if(systematic != "nominal"){

	      background_hist_test -> Divide(signal_hist_test);
	      
	      for(int xbin =1; xbin < meffred_rw -> GetNbinsX(); xbin++){

		std::cout << "Closure " << list_kin[1] << " bin " << xbin << " value: " << background_hist_test -> GetBinContent(xbin) << std::endl;

	      }
		
	    }
	    else{

	      data_hist_test -> Add(background_hist_test,-1);
	      data_hist_test -> Divide(signal_hist_test);
	    
	      for(int xbin =1; xbin < meffred_rw -> GetNbinsX(); xbin++){
		
		std::cout << "Closure " << list_kin[1] << " bin " << xbin << " value: " << data_hist_test -> GetBinContent(xbin) << std::endl;
		
	      }

	    }
	      
          }

        }

      }
      else if(list_kin[1] == "meff"){
	
	// Define histogram that will contain the meff reweighting after applying the jets_n reweighting
	TH2D* meff_rw = new TH2D( (region + "_" + list_kin[1] + systematicPrefix).c_str(), "", nbins_meff, binedges_meff, nbins_jets_n, binedges_jets_n);
	
	for(int ybin =4; ybin <= meff_rw -> GetNbinsY(); ybin++){ // Start loop through jets_n bins
	  
	  // Define 1D histograms that will contain the meff values for data, samples to be reweighted, and
	  // background samples
	  
	  TH1D* data_hist_meff = ((TH2D*)(data_hist -> Clone(data_hist->GetName() ) ) ) -> ProjectionX();
	  data_hist_meff -> Reset();
	  
	  data_hist_meff->Sumw2(kFALSE);
	  
	  TH1D* signal_hist_meff = ((TH2D*)(signal_hist -> Clone(signal_hist->GetName() ) ) ) -> ProjectionX();
	  signal_hist_meff -> Reset();
	  
	  TH1D* background_hist_meff = ((TH2D*)(background_hist -> Clone(background_hist->GetName() ) ) ) -> ProjectionX();
	  background_hist_meff -> Reset();
	  
	  // std::cout << "At ybin: " << ybin << std::endl;
	  
	  if(ybin < meff_rw -> GetNbinsY()){ // Start if ybin is lower than jets_n = 8 
	    
	    for(int xbin = 1; xbin <= data_hist -> GetNbinsX(); xbin++){ // Start loop through meff bins
	      
	      // Get the meff values from the 2D histograms for the current ybin and store them in the 1D histos
	      
	      data_hist_meff -> SetBinContent(xbin, data_hist -> GetBinContent(xbin,ybin));
	      
	      // Apply the jets_n reweighting to the samples to be reweighted for the current jets_n bin
	      signal_hist_meff -> SetBinContent(xbin, signal_hist -> GetBinContent(xbin,ybin) * jets_n_rw -> GetBinContent( ybin ));
	      
	      background_hist_meff -> SetBinContent(xbin, background_hist -> GetBinContent(xbin,ybin));
	      
	    } // End loop through meff bns
	    
	  } // End if ybin is lower than jets_n = 8
	  else{ // Start if ybin is greater than jets_n = 8
	    
	    for(int ybin_tail = ybin; ybin_tail <= data_hist -> GetNbinsY(); ybin_tail++){ // Start loop through jets_n bins greater than jets_n = 8 bin
	      
	      // Define temporary histograms that will contain the meff values for jets_n bins greater than jets_n = 8
	      // This value will be added to data_hist_meff, signal_hist_meff and background_hist_meff so that they
	      // contain the total meff for the merged jets_n bin
	      
	      TH1D* hd = (TH1D*) data_hist_meff -> Clone(data_hist_meff -> GetName() );
	      hd -> Reset();
	      
	      TH1D* hs = (TH1D*) signal_hist_meff -> Clone(signal_hist_meff -> GetName() );
	      hs -> Reset();
	      
	      TH1D* hb = (TH1D*) background_hist_meff -> Clone(background_hist_meff -> GetName() );
	      hb -> Reset();
	      
	      for(int xbin = 1; xbin <= data_hist -> GetNbinsX(); xbin++){ // Start loop through meff bins
		
		hd -> SetBinContent(xbin, data_hist -> GetBinContent(xbin, ybin_tail));
		
		// Apply the jets_n reweighting from the last jets_n bin
		hs -> SetBinContent(xbin, signal_hist -> GetBinContent(xbin,ybin_tail) * jets_n_rw -> GetBinContent(ybin) );
		
		hb -> SetBinContent(xbin, background_hist -> GetBinContent(xbin,ybin_tail) );
		
	      } // End loop through meff bins
	      
	      // Add the meff histograms for the current jets_n bin
	      
	      data_hist_meff -> Add( hd );
	      
	      signal_hist_meff -> Add( hs );
	      
	      background_hist_meff -> Add( hb );
	      
	    } // End loop through jets_n bins greater than jets_n = 8 bin
	    
	  
	  } // End if ybin is greater than jets_n = 8
	  
	  data_hist_meff->Sumw2(kTRUE);
	  
	  // Apply meff rebinning
	  
	  data_hist_meff = (TH1D*) data_hist_meff -> Rebin( nbins_meff, data_hist_meff -> GetName(), rebin_meff);
	  
	  signal_hist_meff = (TH1D*) signal_hist_meff -> Rebin( nbins_meff, signal_hist_meff -> GetName(), rebin_meff);
	  
	  background_hist_meff = (TH1D*) background_hist_meff -> Rebin( nbins_meff, background_hist_meff -> GetName(), rebin_meff);
	  
	  // Define histogram containing numerator for meff reweighting
	  data_hist_meff -> Add(background_hist_meff, -1.0);
	  
	  // Calculate meff reweighting by dividing numerator by samples to be reweighted
	  data_hist_meff -> Divide(signal_hist_meff);
	  
	  f_out->cd();
	  
	  data_hist_meff->SetName( (region + "_" + list_kin[1] + "_jet_" + std::to_string(ybin-1) + "_" + systematicPrefix).c_str() );
	  
	  // Uncomment this to store individual TH1 meff reweighting histograms into output file
	  data_hist_meff->Write();
	  
	  if(doSmoothing){
	  
	    std::cout << "#######################################################################" << std::endl;
	    if(ybin < meff_rw -> GetNbinsY()) std::cout << "Fitting meff reweighting histogram for njets = " << ybin-1 << std::endl;
	    else std::cout << "Fitting meff reweighting histogram for njets >= " << ybin-1 << std::endl;
	    
	    TF1* funcHyperbola = new TF1(("meffFitFuncHyperbola_jet_"+ std::to_string(ybin-1)).c_str(), hyperbolaFitFunction, 600, 5000, 3);
	    
	    TF1* funcSigmoid = new TF1(("meffFitFuncSigmoid_jet_"+ std::to_string(ybin-1)).c_str(), sigmoidFitFunction, 600, 5000, 3);
	    
	    data_hist_meff->Fit(funcHyperbola, "M WW");

	    data_hist_meff->Fit(funcSigmoid, "M WW");
	    
	    f_out->cd();
	    funcHyperbola->Write();

	    f_out->cd();
	    funcSigmoid->Write();
	    
	  }
	  
	  // Store meff reweighting values for the current jets_n bin
	  for(int xbin = 1; xbin <= meff_rw -> GetNbinsX(); xbin++){ // Start loop through meff bins
	    
	    meff_rw -> SetBinContent(xbin, ybin, data_hist_meff -> GetBinContent(xbin));
	    
	    // std::cout << data_hist_meff -> GetBinContent(xbin) << std::endl;
	    
	  } // End loop through meff bins
	  
	} // End loop through jets_n bins
	
	//meff_rw -> Draw("COLZ");  
	
	f_out -> cd();
	
	meff_rw -> SetName( (region + "_" + list_kin[1] + systematicPrefix).c_str() );
	
	meff_rw -> Write();
      
      }
      
    } // End loop through regions

  } // End loop through systematics

  f_out->Close();

  return 0;

}

