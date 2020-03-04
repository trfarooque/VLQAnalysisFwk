#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "TFile.h"
#include "TH1.h"
#include "TH2D.h"


std::string inDir;
std::string outName;
std::string signalSamples;
std::string backgroundSamples;

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

  for(int i = 0; i < stringCollection.size(); i++){
    
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
			  std::string nameHisto){

  TH2D* totalHist = (TH2D*)(histTemp -> Clone ( ( std::string(histTemp -> GetName()) + "_" + std::string(nameHisto) ).c_str() ) );

  totalHist -> Reset();
  
  for(const std::string& sample : *samples){
    
    for(const std::string& campaign : *campaigns){

      if(sample == "ttH" && campaign == "merged_mc16e") continue;

      std::cout << "Opening file " << inputDir + "/" + campaign + "/outVLQAnalysis_" + sample + "_nominal_HIST.root" << std::endl;

      TFile* f = TFile::Open( (inputDir + "/" + campaign + "/outVLQAnalysis_" + sample + "_nominal_HIST.root").c_str(), "READ" );

      TH2D* h_i = ((TH2D*)(f -> Get( ( std::string(region) + "_" + (kinematics -> at(0)) + "_vs_" + (kinematics -> at(1)) ).c_str() ) ) );

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
                          std::string nameHisto){

  TH1D* totalHist = (TH1D*)(histTemp -> Clone ( ( std::string(histTemp -> GetName()) + "_" + std::string(nameHisto) ).c_str() ) );

  totalHist -> Reset();

  for(const std::string& sample : *samples){

    for(const std::string& campaign : *campaigns){

      if(sample == "ttH" && campaign == "merged_mc16e") continue;

      TFile* f = TFile::Open( (inputDir + "/" + campaign + "/outVLQAnalysis_" + sample + "_nominal_HIST.root").c_str(), "READ" );

      TH1D* h_i = ((TH1D*)(f -> Get( ( std::string(region) + "_" + kinematics ).c_str() ) ) );

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

  }
  
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

  bool DEBUG = true;

  TFile* f_out = TFile::Open( outName.c_str(), "UPDATE" );


  // Store all the samples to be reweighted
  std::vector<std::string> list_signal = splitString(signalSamples, ",");
  
  // Store all the background samples
  std::vector<std::string> list_background = splitString(backgroundSamples, ",");
  
  std::vector<std::string> list_campaign = {"merged_mc16a","merged_mc16d","merged_mc16e"};
  
  // Vector containing the kinematics to be reweighted
  std::vector<std::string> list_kin = {"jets_n","meff"};
  
  
  std::vector<std::string> list_reg{};


  // If we are reweighting any ttbar sample or singletopWt use the following regions
  if(std::find(list_signal.begin(), list_signal.end(), "ttbarbb") != list_signal.end() ||
     std::find(list_signal.begin(), list_signal.end(), "ttbarbbaMCPy") != list_signal.end() ||
     std::find(list_signal.begin(), list_signal.end(), "ttbarbbPowHer") != list_signal.end() ||
     std::find(list_signal.begin(), list_signal.end(), "ttbarbbAFII") != list_signal.end()){

    list_reg.push_back("c1lep3jin2bex");

  }

  // IF we are reweighting Zjets use the following regions
  if(std::find(list_signal.begin(), list_signal.end(), "Zjets") != list_signal.end()){
    
    list_reg.push_back("c2lep3jin0bexZwinMLL_sf");
    //list_reg.push_back("c2lep3jin0bexZwinMLL_ee_or_mumu");
  }

  /*double binedges_meff[29] = {0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000,1200,1400,1600,2000,2500,3000,3500,5000};
  double* rebin_meff = &binedges_meff[0];
  int nbins_meff = 28;*/

  // Define rebinning for meff
  double binedges_meff[19] = {0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,5000};
  double* rebin_meff = &binedges_meff[0];
  int nbins_meff = 18;

  // Define rebining for jets_n. Merging bins starting from jets_n = 8 to decrease run time
  double binedges_jets_n[10] = {-0.5, 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 15.5};
  double* rebin_jets_n = &binedges_jets_n[0];
  int nbins_jets_n = 9;


  for(const std::string& region : list_reg){// Start loop through regions

    // Open file containing data
    TFile* f_data = TFile::Open( (inDir + "/mergedData/outVLQAnalysis_Data_nominal_HIST.root").c_str(), "READ" );

    // Retrieve jets_n and meff 2D histogram 
    TH2D* data_hist = (TH2D*)(f_data -> Get( ( (region + "_" + list_kin[0] + "_vs_" + list_kin[1]).c_str() ) ) );

    data_hist -> SetDirectory(0);

    f_data -> Close();

    // Obtain 2D histogram for samples to be reweighted
    TH2D* signal_hist = getTotalKinHisto2D(&list_signal, inDir, region, &list_campaign, &list_kin, data_hist, "signal");

    // OBtain 2D histogram for background samples
    TH2D* background_hist = getTotalKinHisto2D(&list_background, inDir, region, &list_campaign, &list_kin, data_hist, "background");

    TH2D* numeratorHist = (TH2D*)(data_hist -> Clone ( "numerator" ) );

    numeratorHist -> Reset();

    // Define reweighting numerator as data histogram subtracted by background samples
    numeratorHist -> Add(data_hist, background_hist, 1.0, -1.0);
    
    // Define histogram containing the Y-projection (jets_n) for the numerator
    TH1D* numerator_jets_n = numeratorHist -> ProjectionY();

    // Define histogram containing the Y-projection (jets_n) of the samples to be reweighted 
    TH1D* denominator_jets_n = signal_hist -> ProjectionY();
    
    // Apply jets_n rebinning
    numerator_jets_n = (TH1D*) numerator_jets_n->Rebin(nbins_jets_n, numerator_jets_n->GetName(), rebin_jets_n);
    denominator_jets_n = (TH1D*) denominator_jets_n->Rebin(nbins_jets_n, denominator_jets_n->GetName(), rebin_jets_n);

    // Calculate the jets_n reweighting
    numerator_jets_n -> Divide( denominator_jets_n );

    f_out->cd();

    numerator_jets_n -> SetName( (region + "_jets_n").c_str() );
    
    numerator_jets_n -> Write();

    // Define histogram that will contain the meff reweighting after applying the jets_n reweighting
    TH2D* meff_rw = new TH2D( (region + "_meff").c_str(), "", nbins_meff, binedges_meff, nbins_jets_n, binedges_jets_n);
    
    for(int ybin =4; ybin <= meff_rw -> GetNbinsY(); ybin++){ // Start loop through jets_n bins

      // Define 1D histograms that will contain the meff values for data, samples to be reweighted, and
      // background samples
      
      TH1D* data_hist_meff = ((TH2D*)(data_hist -> Clone(data_hist->GetName() ) ) ) -> ProjectionX();
      data_hist_meff -> Reset();

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
	  signal_hist_meff -> SetBinContent(xbin, signal_hist -> GetBinContent(xbin,ybin) * numerator_jets_n -> GetBinContent( ybin ));
	  
	  background_hist_meff -> SetBinContent(xbin, background_hist -> GetBinContent(xbin,ybin));

	} // End loop through meff bns

      } // End if ybin is lower than jets_n = 8
      else{ // Start if ybin is greater than jets_n = 8

	for(int ybin_tail = ybin; ybin_tail <= data_hist -> GetNbinsY(); ybin_tail++){ // Start loop through jets_n bins greater than jets_n = 8 bin
	  
	  // Define temporary histograms that will contain the meff values for jets_n bins greater than jets_n = 8
	  // This values will be added to data_hist_meff, signal_hist_meff and background_hist_meff so that they
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
	    hs -> SetBinContent(xbin, signal_hist -> GetBinContent(xbin,ybin_tail) * numerator_jets_n -> GetBinContent(ybin) );

	    hb -> SetBinContent(xbin, background_hist -> GetBinContent(xbin,ybin_tail) );

	  } // End loop through meff bins

	  // Add the meff histograms for the current jets_n bin

	  data_hist_meff -> Add( hd );

	  signal_hist_meff -> Add( hs );

	  background_hist_meff -> Add( hb );

	} // End loop through jets_n bins greater than jets_n = 8 bin


      } // End if ybin is greater than jets_n = 8
      

      // Apply meff rebinning
      
      data_hist_meff = (TH1D*) data_hist_meff -> Rebin( nbins_meff, data_hist_meff -> GetName(), rebin_meff);

      signal_hist_meff = (TH1D*) signal_hist_meff -> Rebin( nbins_meff, signal_hist_meff -> GetName(), rebin_meff);

      background_hist_meff = (TH1D*) background_hist_meff -> Rebin( nbins_meff, background_hist_meff -> GetName(), rebin_meff);

      // Define histogram containing numerator for meff reweighting
      data_hist_meff -> Add(background_hist_meff, -1.0);

      // Calculate meff reweighting by dividing numerator by samples to be reweighted
      data_hist_meff -> Divide(signal_hist_meff);

      // Store meff reweighting values for the current jets_n bin
      for(int xbin = 1; xbin <= meff_rw -> GetNbinsX(); xbin++){ // Start loop through meff bins

	meff_rw -> SetBinContent(xbin, ybin, data_hist_meff -> GetBinContent(xbin));

	// std::cout << data_hist_meff -> GetBinContent(xbin) << std::endl;
	
      } // End loop through meff bins

    } // End loop through jets_n bins

    //meff_rw -> Draw("COLZ");
    
    f_out -> cd();
    
    meff_rw -> SetName( (region + "_meff").c_str() );

    meff_rw -> Write();


    // Closure Test

    if(DEBUG){
      
      for(int ybin = 4; ybin <= meff_rw -> GetNbinsY(); ybin++){

	std::string test_region = "";
	
	if(signalSamples == "Zjets,"){
	  
	  test_region = Form("c2lep%ijex0bexZwinMLL_sf", ybin-1);
	  if(ybin == meff_rw -> GetNbinsY()) test_region = Form("c2lep%ijin0bexZwinMLL_sf", ybin-1); 

	}
	else{
	  test_region = Form("c1lep%ijex2bex", ybin-1);
	  if(ybin == meff_rw -> GetNbinsY()) test_region = Form("c1lep%ijin2bex", ybin-1);
	}

	//std::string test_region = Form("c1lep%ijex2bex", ybin-1);
	//std::string test_region = Form("c2lep%ijex0bexZwinMLL_sf", ybin-1);
	//std::string test_region = Form("c2lep%ijex0bexZwinMLL_ee_or_mumu", ybin-1);

	//if(ybin == meff_rw -> GetNbinsY()) test_region = Form("c1lep%ijin2bex", ybin-1);
	//if(ybin == meff_rw -> GetNbinsY()) test_region = Form("c2lep%ijin0bexZwinMLL_sf", ybin-1);
	//if(ybin == meff_rw -> GetNbinsY()) test_region = Form("c2lep%ijin0bexZwinMLL_ee_or_mumu", ybin-1);

	TFile* f_data_test = TFile::Open( (inDir + "/mergedData/outVLQAnalysis_Data_nominal_HIST.root").c_str(), "READ" );
	TH1D* data_hist_test = (TH1D*)(f_data_test -> Get( ( (test_region + "_meff").c_str() ) ) );
	data_hist_test -> SetDirectory(0);
	f_data_test -> Close();
	TH1D* signal_hist_test = getTotalKinHisto1D(&list_signal, inDir, test_region, &list_campaign, "meff", data_hist_test, "signal");
	TH1D* background_hist_test = getTotalKinHisto1D(&list_background, inDir, test_region, &list_campaign, "meff", data_hist_test, "background");
	
	data_hist_test = (TH1D*) data_hist_test -> Rebin( nbins_meff, data_hist_test -> GetName(), rebin_meff);
	signal_hist_test = (TH1D*) signal_hist_test -> Rebin( nbins_meff, signal_hist_test -> GetName(), rebin_meff);
	background_hist_test = (TH1D*) background_hist_test -> Rebin( nbins_meff, background_hist_test -> GetName(), rebin_meff);

	for(int xbin =1; xbin < meff_rw -> GetNbinsX(); xbin++){
	    
	  double weight = meff_rw -> GetBinContent(xbin, ybin)*numerator_jets_n -> GetBinContent(ybin);
	  
	  signal_hist_test -> SetBinContent(xbin, signal_hist_test -> GetBinContent(xbin)*weight);
	  
	}

	data_hist_test -> Add(background_hist_test,-1);
	data_hist_test -> Divide(signal_hist_test);
	
	std::cout << "For jets_n = " << ybin -1 << std::endl;

	for(int xbin =1; xbin < meff_rw -> GetNbinsX(); xbin++){
	
	  std::cout << "Closure meff bin " << xbin << " value: " << data_hist_test -> GetBinContent(xbin) << std::endl;
          
	}
	
      }
      
    }


  } // End loop through regions


  f_out->Close();

  return 0;

}

