#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <map>
#include "TFile.h"
#include "TMath.h"
#include "TH1.h"
#include "TH2D.h"

std::string sampleList;
std::string variableList;
std::string systematicList;

// Utility function for splitting input string with a given delimiter. Each substring
// is stored in an output vector 
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

    std::transform(argument.begin(), argument.end(), argument.begin(), toupper);

    if(argument == "--SAMPLELIST") sampleList = value;
    else if(argument == "--VARIABLELIST") variableList = value;
    else if(argument == "--SYSTEMATICLIST") systematicList = value;

  }

}

// Utility function that reads configurations from a text file and stores each entry in a 
// map of strings. All entries are then stored in a vector of these maps
//______________________________________________________________________________
//
std::vector< std::map< std::string, std::string > > readFromTextFile(std::string fileName, char delim){

  std::cout << "Opening file " << fileName << std::endl;

  std::ifstream file(fileName);

  std::vector< std::map< std::string, std::string> > vectorMap = {};

  if(!file.is_open()){

    std::cout << "< ERROR > : The file  " << fileName << " does not exist! Exiting from program." << std::endl;

    exit(EXIT_FAILURE);

  }

  std::string line;

  while(getline(file,line)){

    if(line == "NEW"){

      std::cout << "#####################################" << std::endl;
      std::cout << "#     Found reweighting option      #" << std::endl;
      std::cout << "#####################################" << std::endl;

      std::map< std::string, std::string > optMap;

      while(getline(file,line) && line != "END"){

	std::string key = line.substr(0, line.find(delim));

	std::string opt = line.substr(line.find(delim)+1);

	std::cout << key << " : " << opt <<std::endl;

        optMap[key] = opt;

      }

      vectorMap.push_back(optMap);


    }

  }

  std::cout << "#####################################" << std::endl;
  std::cout << "Closing file "  << fileName << std::endl;

  return vectorMap;

}

// Function that books the histogram templates that are to be used for kinematic reweighting
// Inputs:
// kinematics: a vector of maps that contains the information of the the kinematic
// variables to be reweighted
// samples: a vector of maps that contains the information of all the samples needed
// for reweighting
// histo1DMap: a 3D map of TH1D with keys [kinematic name][sample type][systematic] used to
// store the TH1D histogram templates for reweighting
// histo2DMap: a 3D map of TH2D with keys [kinematic name][sample type][systematic] used to
// store the TH2D histogram templates for reweighting
//______________________________________________________________________________
//
void setupReweightingHistograms(std::vector< std::map < std::string, std::string > > kinematics,
				std::vector< std::map < std::string, std::string > > samples,
				std::map< std::string, std::map< std::string, std::map< std::string, TH1D* > > >& histo1DMap,
				std::map< std::string, std::map< std::string, std::map< std::string, TH2D* > > >& histo2DMap){

  std::map< std::string, std::vector< std::string > >::iterator kinIt;

  std::map< std::string, std::vector< std::string > >::iterator samIt;

  for(std::map< std::string, std::string > kinMap : kinematics){ // start loop through kinematic maps
    
    for(std::map < std::string, std::string> sampleMap : samples){ // start loop through sample maps

      std::vector< std::string > systematics = {};
      
      std::stringstream ssSyst(sampleMap["SYSTEMATICS"]);
      
      std::string systName;
      
      // read the systematics available for the current sample and stored them in a vector
      while(getline(ssSyst, systName, ',')) systematics.push_back(systName);
      
      for(std::string systematic : systematics){ // start loop through systematics
	
	if(kinMap["FORMAT"] == "TH1D"){ // start if kinematic to reweight is stored in a TH1D
	  
	  if(histo1DMap[kinMap["NAME"]][sampleMap["SAMPLE"]][systematic] == NULL){
	  
	    TFile* f = TFile::Open((sampleMap["NAME"]).c_str(), "READ");

	    f->cd();
  
	    // histogram to store template of kinematic to reweight
	    TH1D* temp = (TH1D*) f->Get((kinMap["NAME"]).c_str());

	    temp->SetDirectory(0);

	    f->Close();

	    std::cout << "Booking histogram " << kinMap["NAME"]+"_"+sampleMap["SAMPLE"]+"_"+systematic << std::endl;
	      
	    // store the template for the current systematic, sample type, and systematic
	    histo1DMap[kinMap["NAME"]][sampleMap["SAMPLE"]][systematic] = (TH1D*) temp->Clone((kinMap["NAME"]+"_"+sampleMap["SAMPLE"]+"_"+systematic).c_str());
	    
	    histo1DMap[kinMap["NAME"]][sampleMap["SAMPLE"]][systematic]->Reset();

	    delete f;

	    delete temp;
	    
	  }
	  
	} // end if kinematic ro reweight is stored in a TH1D
	
	else if(kinMap["FORMAT"] == "TH2D"){ // start if kinematic to reweight is stored in a TH2D
	  	  
	  if(histo2DMap[kinMap["NAME"]][sampleMap["SAMPLE"]][systematic] == NULL){
	    
	    TFile* f = TFile::Open((sampleMap["NAME"]).c_str(), "READ");

	    f->cd();
	    
	    // histogram to store template of kinematic to reweight
            TH2D* temp = (TH2D*) f->Get((kinMap["NAME"]).c_str());

	    temp->SetDirectory(0);

            f->Close();

	    std::cout << "Booking histogram " << kinMap["NAME"]+"_"+sampleMap["SAMPLE"]+"_"+systematic << std::endl;
	    
	    // store the template for the current systematic, sample type, and systematic
	    histo2DMap[kinMap["NAME"]][sampleMap["SAMPLE"]][systematic] = (TH2D*) temp->Clone((kinMap["NAME"]+"_"+sampleMap["SAMPLE"]+"_"+systematic).c_str());

            histo2DMap[kinMap["NAME"]][sampleMap["SAMPLE"]][systematic]->Reset();

            delete f;

            delete temp;

	  }
	  
	} // end if kinematic to reweight is stored in a TH2d
	
      } // end loop through systematics
      
    } // end loop through sample maps
    
  } // end loop through kinematic maps

}

// Function that fills the histogram templates for reweighting with the required histograms
// Inputs:                                                                                                                                                                                             
// kinematics: a vector of maps that contains the information of the the kinematic                                                                                                                    
// variables to be reweighted                                                                                                                                                                         
// samples: a vector of maps that contains the information of all the samples needed                                                                                                                  
// for reweighting                                                                                                                                                                                    
// histo1DMap: a 3D map of TH1D with keys [kinematic name][sample type][systematic] that contains            
// the histogram templates required for reweighting a kinematic stored in a TH1D                                                                                                     
// histo2DMap: a 3D map of TH2D with keys [kinematic name][sample type][systematic] that contains
// the histogram templates required for reweighting a kinematic stored in a TH2D
//______________________________________________________________________________
//
void fillReweightingHistograms(std::map < std::string, std::map< std::string, std::map< std::string, TH1D* > > >& histo1DMap,
			       std::map < std::string, std::map< std::string, std::map< std::string, TH2D* > > >& histo2DMap,
			       std::vector< std::map < std::string, std::string > > samples,
			       std::vector< std::map < std::string, std::string > > kinematics){

  for(std::map< std::string, std::string > kinMap : kinematics){ // start loop through kinematic maps

    for(std::map < std::string, std::string > sampleMap : samples){ // start loop through sample maps

      std::vector< std::string > systematics = {};

      std::stringstream ssSyst(sampleMap["SYSTEMATICS"]);

      std::string systName;

      // read the systematics available for the current sample and store them in a vector
      while(getline(ssSyst, systName, ',')) systematics.push_back(systName);

      for(std::string systematic : systematics){ // start loop through systematics

	std::string kinSuffix = "";

	// if the current systematic is stored as a weight in the ROOT file, append
	// the name of the systematic to the name of the kinematic
        if(sampleMap["SYSTTYPE"] == "weight") kinSuffix += ("_"+systematic);

        if(kinMap["FORMAT"] == "TH1D"){ // start if kinematic to reweight is stored in a TH1D

	  //std::cout << "Opening file " << sampleMap["NAME"] << std::endl;

	  // open file for the corresponding sample
          TFile* f = TFile::Open((sampleMap["NAME"]).c_str(),"READ");

	  // retrieve the kinematic histogram for the given sample and systematic
          TH1D* histo = (TH1D*) f->Get((kinMap["NAME"] + kinSuffix).c_str());

          if(!histo){

	    std::cout << "< ERROR > : The histogram " << kinMap["NAME"] + kinSuffix << " does not exist! Exiting from program." << std::endl;

            exit(EXIT_FAILURE);

          }
	  
	  // scale the histogram with the corresponding MC campaign luminosity
	  histo->Scale(std::stod(sampleMap["SCALE"]));

	  // add current histogram to corresponding template
	  histo1DMap[kinMap["NAME"]][sampleMap["SAMPLE"]][systematic]->Add(histo);

          f->Close();

        } // end if kinematic to reweight is stored in a TH1D
	else if(kinMap["FORMAT"] == "TH2D"){ // start if kinematic to reweight is stored in a TH2D

	  //std::cout << "Opening file " << sampleMap["NAME"] << std::endl;

	  // open file for the corresponding sample
          TFile* f = TFile::Open((sampleMap["NAME"]).c_str(),"READ");

	  // retrieve the kinematic histogram for the given sample and systematic
          TH2D* histo = (TH2D*) f->Get((kinMap["NAME"] + kinSuffix).c_str());

          if(!histo){

	    std::cout << "< ERROR > : The histogram " << kinMap["NAME"] + kinSuffix << " does not exist! Exiting from program." << std::endl;

            exit(EXIT_FAILURE);

          }

	  // scale the histogram with the corresponding MC campaign luminosity
	  histo->Scale(std::stod(sampleMap["SCALE"]));

	  // add current histogram to corresponding templat
          histo2DMap[kinMap["NAME"]][sampleMap["SAMPLE"]][systematic]->Add(histo);

          f->Close();

        } // end if kinematic to reweight is stored in a TH2D

      } // end loop through systematics

    } // end loop through sample maps

  } // end loop through kinematic maps

}

// Function that reweights a kinematic stored in a TH1D for a given systematic
// Inputs:
// histoMap: map of histogram templates required to derive a kinematic reweighting
// kinematic: map containing the options of a kinematic variable to be reweighted
// systematic: name of the systematic variation to be reweighted
// doMCRatio: option to derive a MC to MC ratio between the nominal kinematic variable
// and the systematic variation of the kinematic variable instead of deriving a 
// new reweighting for the systematic variation
// Returns:
// TH1D pointer of the reweighting histogram or MC to MC ratio for the given kinematic
// variable and systematic variation
//______________________________________________________________________________ 
//
TH1D* reweightKinTH1D(std::map < std::string, std::map< std::string, std::map< std::string, TH1D* > > >& histoMap,
		     std::map < std::string, std::string > kinematic, std::string systematic, bool doMCRatio = false){
  
  std::string systSuffix = (systematic == "nominal") ? "" : "_"+systematic;

  std::vector<double> XbinEdges = {};
  
  // retrieve the desired binning for the reweighting histogram
  std::stringstream XbinSS(kinematic["XBIN"]);

  std::string binEdge;

  // store the binning in a vector
  while(getline(XbinSS, binEdge, ',')) XbinEdges.push_back(std::stod(binEdge));

  // define the name of the reweighting histogram
  std::string kinRwName = (kinematic["RENAME"] != "") ? kinematic["RENAME"]+systSuffix : kinematic["NAME"] + systSuffix;

  // define the reweighting histogram
  TH1D* kinRw = new TH1D((kinRwName).c_str(), "", XbinEdges.size()-1, &XbinEdges[0]);

  if(systematic != "nominal" && doMCRatio){ // start if systematic variation is not nominal and user wants to calculate a MC to MC ratio

    // define numerator histogram for the MC to MC ratio
    TH1D* numerator = new TH1D();

    // define denominator histogram for the MC to MC ratio
    TH1D* denominator =new TH1D();

    // store the nominal variation of the histogram in the numerator 
    *numerator = *(histoMap[kinematic["NAME"]]["signal"]["nominal"]);

    // store the systematic variation of the histogram in the denominator
    *denominator = *(histoMap[kinematic["NAME"]]["signal"][systematic]);

    /*for(int xbins = 1; xbins <= denominator->GetNbinsX(); xbins++){
      
      std::cout << "nominal value at xbin=" << xbins << ": " << numerator->GetBinContent(xbins) << std::endl;
      
      std::cout << "systematic value at xbin=" << xbins << ": " << denominator->GetBinContent(xbins) << std::endl;
      
      }*/
    
    // rebin numerator histogram to the same binning as the reweighting histogram
    numerator = (TH1D*) numerator->Rebin(XbinEdges.size()-1, numerator->GetName(), &XbinEdges[0]);

    // rebin denominator histogram to the same binning as the reweighting histogram
    denominator = (TH1D*) denominator->Rebin(XbinEdges.size()-1, denominator->GetName(), &XbinEdges[0]);
    
    // calculate the MC to MC ratio by dividing the denominator histogram to the numerator histogram
    kinRw->Divide(numerator, denominator);

  } // end if systematic variation is not nominal and user wants to calculate a MC to MC ratio
  else{ // start if systematic variation is nominal or user does not want to calculate a MC to MC ratio

    // define numerator histogram for reweighting
    TH1D* numerator = new TH1D();

    // define denominator histogram for reweighting
    TH1D* denominator =new TH1D();

    // store the data histogram in the numerator
    *numerator = *(histoMap[kinematic["NAME"]]["data"]["nominal"]);

    // store the background to be reweighted histogram in the denominator
    *denominator = *(histoMap[kinematic["NAME"]]["signal"][systematic]);

    // subtract backgrounds to not be reweighted to the data histogram
    numerator->Add(histoMap[kinematic["NAME"]]["background"]["nominal"], -1);

    // rebin numerator histogram to the same binning as the reweighting histogram
    numerator =(TH1D*)numerator->Rebin(XbinEdges.size()-1, numerator->GetName(), &XbinEdges[0]);

    // rebin denominator histogram to the same binning as the reweighting histogram
    denominator = (TH1D*) denominator->Rebin(XbinEdges.size()-1, denominator->GetName(), &XbinEdges[0]);

    // calculate the kinematic reweighting by dividing the denominator histogram to the numerator histogram
    kinRw->Divide(numerator, denominator);

  } // end  if systematic variation is nominal or user does not want to calculate a MC to MC ratio

  return kinRw;

}

// Function that reweights a kinematic stored in a TH2D for a given systematic                                  
// Inputs:                                                                                                                                                                                           
// histoMap: map of histogram templates required to derive a kinematic reweighting                                                                                                                      
// kinematic: map containing the options of a kinematic variable to be reweighted                                                                                                                      
// systematic: name of the systematic variation to be reweighted                                                                                                                                       
// doMCRatio: option to derive a MC to MC ratio between the nominal kinematic variable                                                                                                                 
// and the systematic variation of the kinematic variable instead of deriving a                                                                                                                        
// new reweighting for the systematic variation                                                                                                                                                        
// rW_dep: pointer to a TH1D containing a kinematic reweighting that needs to be
// applied before deriving the kinematic reweighting for the current variable
// Returns:                                                                                                                                                                                            
// TH2D pointer of the reweighting histogram or MC to MC ratio for the given kinematic                                                                                                               
// variable and systematic variation 
//______________________________________________________________________________
//
TH2D* reweightKinTH2D(std::map < std::string,std::map< std::string, std::map< std::string, TH2D* > > >& histoMap,
		      std::map < std::string, std::string > kinematic, std::string systematic, bool doMCRatio = false, TH1D* rW_dep = NULL){

  std::string systSuffix = (systematic == "nominal") ? "" : "_"+systematic;

  std::vector<double> XbinEdges = {};

  std::vector<double> YbinEdges = {};

  // retrieve the desired x-axis binning for the reweighting histogram
  std::stringstream XbinSS(kinematic["XBIN"]);

  // retrieve the desired y-axis binning for the reweighting histogram
  std::stringstream YbinSS(kinematic["YBIN"]);

  std::string binEdge;

  // store the x-axis binning in a vector
  while(getline(XbinSS, binEdge, ',')) XbinEdges.push_back(std::stod(binEdge));

  // store the y-axis binning in a vector
  while(getline(YbinSS, binEdge, ',')) YbinEdges.push_back(std::stod(binEdge));

  // define the name of the reweighting histogram
  std::string kinRwName = (kinematic["RENAME"] != "") ? kinematic["RENAME"]+systSuffix : kinematic["NAME"] + systSuffix;

  // define the reweighting histogram
  TH2D* kinRw = new TH2D( kinRwName.c_str(), "", XbinEdges.size()-1, &XbinEdges[0], YbinEdges.size()-1, &YbinEdges[0]);

  if(systematic != "nominal" && doMCRatio){ // start if systematic variation is not nominal and user wants to calculate a MC to MC ratio

    // define numerator histogram for the MC to MC ratio
    TH2D* numerator = new TH2D();

    // define denominator histogram for the MC to MC ratio
    TH2D* denominator = new TH2D();

    // store the nominal variation of the kinematic histogram in the numerator
    *numerator = *(histoMap[kinematic["NAME"]]["signal"]["nominal"]);

    // store the systematic variation of the kinematic histogram in the denominator
    *denominator = *(histoMap[kinematic["NAME"]]["signal"][systematic]);
    
    if(rW_dep != NULL){ // start if there is a kinematic reweighting to be applied before deriving the MC to MC ratio

      //std::cout << rW_dep->GetName() << std::endl;

      for(int ybins = 1; ybins <= denominator->GetNbinsY(); ybins++){ // start loop through denominator/numerator y bins

	/*int test_bin = (ybins <= rW_dep->GetNbinsX()) ? ybins : rW_dep->GetNbinsX();

	std::cout << "At Njets bin " << ybins << ": [" << denominator->GetYaxis()->GetBinLowEdge(ybins) << "," << denominator->GetYaxis()->GetBinUpEdge(ybins) << "]" << std::endl;

	std::cout << "At Njets bin " << test_bin << ": [" << rW_dep->GetXaxis()->GetBinLowEdge(test_bin) << "," << rW_dep->GetXaxis()->GetBinUpEdge(test_bin) << "]" <<std::endl;*/

	// obtain the kinematic reweighting to be applied prior to calculating the MC to MC ratio
	double weight = (ybins <= rW_dep->GetNbinsX()) ? rW_dep->GetBinContent(ybins) : rW_dep->GetBinContent(rW_dep->GetNbinsX());

        for(int xbins = 1; xbins <= denominator->GetNbinsX(); xbins++){ // start loop through denominator/numerator x bins

	  // apply kinematic reweighting to the denominator histogram
          denominator->SetBinContent(xbins, ybins, denominator->GetBinContent(xbins,ybins) * weight);

	  // scale denominator histogram errors by the kinematic reweighting
	  denominator->SetBinError(xbins,ybins, denominator->GetBinError(xbins,ybins) * weight);

        } // end loop through denominator/numerator x bins

      } // end loop through denominator/numerator y bins

    } // end if there is a kinematic reweighting to be applied before deriving the MC to MC ratio

    for(int ybins = 1; ybins <= kinRw->GetNbinsY(); ybins++){ // start loop through kinRw y bins

      // define the last y-axis bin to project down the numerator and denominator histograms
      int lastybin = (ybins < kinRw->GetNbinsY()) ? ybins : numerator->GetNbinsY();

      /*std::cout << "projecting y axis bins " << ybins << " to " << lastybin << std::endl;

	std::cout << "[" << denominator->GetYaxis()->GetBinLowEdge(ybins) << "," << denominator->GetYaxis()->GetBinUpEdge(lastybin) << "]" << std::endl;*/

      // project down the numerator histogram along the x-axis for the y bin range [ybins,lastybin]
      TH1D* numerator_projx = (TH1D*)(numerator->ProjectionX((std::string(numerator->GetName())+"_px").c_str(), ybins, lastybin, "e"));

      // project down the denominator histogram along the x-axis for the y bin range [ybins,lastybin] 
      TH1D* denominator_projx =(TH1D*)(denominator->ProjectionX((std::string(denominator->GetName())+"_px").c_str(), ybins, lastybin, "e"));

      // rebin the x-axis of the numerator projection to have the same x-axis binning as the reweighting histogram
      numerator_projx = (TH1D*) numerator_projx->Rebin(XbinEdges.size()-1, numerator_projx->GetName(), &XbinEdges[0]);

      // rebin the x-axis of the denominator projection to have the same x-axis binning as the reweighting histogram
      denominator_projx = (TH1D*) denominator_projx->Rebin(XbinEdges.size()-1, denominator_projx->GetName(), &XbinEdges[0]);

      //numerator_projx->Divide(denominator_projx);
      
      TH1D* ratio_projx = (TH1D*) numerator_projx->Clone("ratio_projx");

      ratio_projx->Reset();

      // calculate the ratio of the numerator histogram projection to the denominator histogram projection
      ratio_projx->Divide(numerator_projx, denominator_projx);

      for(int xbins = 1; xbins <= kinRw->GetNbinsX(); xbins++){ // start loop through kinRw x bins

	// set the bin content of kinRw for the bins [xbins,ybins] equal to the bin content of the ratio at the bins xbins
        kinRw->SetBinContent(xbins, ybins, ratio_projx->GetBinContent(xbins));

	kinRw->SetBinError(xbins, ybins, ratio_projx->GetBinError(xbins));

      } // end loop through kinRw x bins

    } // end loop through kinRw y bins

  } // end if systematic variation is not nominal and user wants to calculate a MC to MC ratio
  else{ // start if systeamtic variation is nominal or user does not want to calculate a MC to MC ratio

    TH2D* numerator = new TH2D();

    TH2D* denominator =new TH2D();

    *numerator = *(histoMap[kinematic["NAME"]]["data"]["nominal"]);

    *denominator = *(histoMap[kinematic["NAME"]]["signal"][systematic]);

    numerator->Add(histoMap[kinematic["NAME"]]["background"]["nominal"], -1);

    if(rW_dep != NULL){

      for(int ybins = 1; ybins <= denominator->GetNbinsY(); ybins++){

	int test_bin = (ybins <= rW_dep->GetNbinsX()) ? ybins : rW_dep->GetNbinsX();

	//std::cout << "At Njets bin " << ybins << ": [" << denominator->GetYaxis()->GetBinLowEdge(ybins) << "," << denominator->GetYaxis()->GetBinUpEdge(ybins) << "]" << std::endl;

	//std::cout << "At Njets bin " << test_bin << ": [" << rW_dep->GetXaxis()->GetBinLowEdge(test_bin) << "," << rW_dep->GetXaxis()->GetBinUpEdge(test_bin) << "]" <<std::endl;

	double weight = (ybins <= rW_dep->GetNbinsX()) ? rW_dep->GetBinContent(ybins) : rW_dep->GetBinContent(rW_dep->GetNbinsX());
	
	for(int xbins = 1; xbins <= denominator->GetNbinsX(); xbins++){
	  
	  //std::cout << denominator->GetBinContent(xbins,ybins) << " " << weight << std::endl;

	  denominator->SetBinContent(xbins, ybins, denominator->GetBinContent(xbins,ybins) * weight);
	  
	  denominator->SetBinError(xbins,ybins, denominator->GetBinError(xbins,ybins) * weight);

	}
	
      }

    }

    for(int ybins = 1; ybins <= kinRw->GetNbinsY(); ybins++){

      int lastybin = (ybins < kinRw->GetNbinsY()) ? ybins : numerator->GetNbinsY();

      //std::cout << "projecting y axis bins " << ybins << " to " << lastybin << std::endl;
      
      //std::cout << "[" << denominator->GetYaxis()->GetBinLowEdge(ybins) << "," << denominator->GetYaxis()->GetBinUpEdge(lastybin) << "]" << std::endl; 

      TH1D* numerator_projx = (TH1D*)(numerator->ProjectionX((std::string(numerator->GetName())+"_px").c_str(), ybins, lastybin, "e"));

      TH1D* denominator_projx =(TH1D*)(denominator->ProjectionX((std::string(denominator->GetName())+"_px").c_str(), ybins, lastybin, "e"));

      numerator_projx = (TH1D*) numerator_projx->Rebin(XbinEdges.size()-1, numerator_projx->GetName(), &XbinEdges[0]);

      denominator_projx = (TH1D*) denominator_projx->Rebin(XbinEdges.size()-1, denominator_projx->GetName(), &XbinEdges[0]);

      TH1D* ratio_projx = (TH1D*) numerator_projx->Clone("ratio_projx");

      ratio_projx->Reset();

      ratio_projx->Divide(numerator_projx, denominator_projx);

      //numerator_projx->Divide(denominator_projx);

      for(int xbins = 1; xbins <= kinRw->GetNbinsX(); xbins++){

	//kinRw->SetBinContent(xbins, ybins, numerator_projx->GetBinContent(xbins));

	kinRw->SetBinContent(xbins, ybins, ratio_projx->GetBinContent(xbins));

	//std::cout << ratio_projx->GetBinError(xbins) << std::endl;

	kinRw->SetBinError(xbins, ybins, ratio_projx->GetBinError(xbins));

      }
      
    }

  }

  return kinRw;
  
}

//______________________________________________________________________________
//
int main(int argc, char** argv){

  parseUserOptions(argc, argv);

  std::cout << "=======================================================================" << std::endl;
  std::cout << "=================       Reading sample options        =================" << std::endl;
  std::cout << "=======================================================================" << std::endl;
  std::vector< std::map < std::string, std::string > > sampleAttributes = readFromTextFile(sampleList, ':');
  std::cout << "=======================================================================" << std::endl << std::endl;

  std::cout << "=======================================================================" << std::endl;
  std::cout << "=================       Reading variable options       ================" << std::endl;
  std::cout << "=======================================================================" << std::endl;
  std::vector< std::map < std::string, std::string > > variableAttributes = readFromTextFile(variableList, ':');
  std::cout << "=======================================================================" << std::endl << std::endl;

  std::map < std::string, std::map< std::string, std::map< std::string, TH1D* > > > histo1DMap;

  std::map < std::string, std::map< std::string, std::map< std::string, TH2D* > > > histo2DMap;
  
  std::cout << "=======================================================================" << std::endl;
  std::cout << "==============  Booking reweighting histogram templates  ==============" << std::endl;
  std::cout << "=======================================================================" << std::endl;
  setupReweightingHistograms(variableAttributes, sampleAttributes, histo1DMap, histo2DMap);
  std::cout << "=======================================================================" << std::endl << std::endl;

  std::cout << "=======================================================================" << std::endl;
  std::cout << "==============  Filling reweighting histogram templates  ==============" << std::endl;
  std::cout << "=======================================================================" << std::endl;
  fillReweightingHistograms(histo1DMap, histo2DMap, sampleAttributes, variableAttributes);
  std::cout << "=======================================================================" << std::endl << std::endl;

  // vector to store systematic variations
  std::vector< std::string > systematics = {};

  for(std::map < std::string, std::string > sampleMap : sampleAttributes){

    std::stringstream ssSyst(sampleMap["SYSTEMATICS"]);

    std::string systName;

    while(getline(ssSyst, systName, ',')){
      
      if(std::find(systematics.begin(), systematics.end(), systName) == systematics.end()) systematics.push_back(systName);

    }

  }

  std::map< std::string, TH1D* > kinRw1D;

  std::map< std::string, TH2D* > kinRw2D;

  std::cout << "=======================================================================" << std::endl;
  std::cout << "==============         Entering reweighting loop         ==============" << std::endl;
  std::cout << "=======================================================================" << std::endl;
  for(std::map < std::string, std::string > kinMap : variableAttributes){ // begin loop through kinematic maps

    for(std::string syst : systematics){ // begin loop through systematics

      std::cout << "Reweighting kinematic " << kinMap["NAME"] << " for systematic variation " << syst << std::endl;

      if(kinMap["FORMAT"] == "TH1D"){ // start if kinematic to reweight is stored in a TH1D

	kinRw1D[kinMap["NAME"]+"_"+syst] = reweightKinTH1D(histo1DMap, kinMap, syst, true);
	
      } // end if kinematic to reweight is stored in a TH1D
      else if(kinMap["FORMAT"] == "TH2D"){ // start if kinematic to reweight is stored in a TH2D

	TH1D* kinRw_dep = (kinMap["DEPENDS"] != "") ? kinRw1D[kinMap["DEPENDS"]+"_"+syst] : NULL; 

	TH2D* kinRw = reweightKinTH2D(histo2DMap, kinMap, syst, true, kinRw_dep);

	if(kinMap["SAVEPROJX"] == "true"){ // start if kinematic reweighting should be stored as a TH1D projection on the x-axis

	  std::string nametemp = kinRw->GetName();

	  kinRw->SetName((nametemp+"_2D").c_str());

	  kinRw1D[kinMap["NAME"]+"_"+syst] = (TH1D*) kinRw->ProjectionX(nametemp.c_str(), std::stoi(kinMap["PROJXLOWBINY"]), std::stoi(kinMap["PROJXUPBINY"]) ,"e");

	} 
	else{

	  kinRw2D[kinMap["NAME"]+"_"+syst] = kinRw;

	}

	//kinRw2D[kinMap["NAME"]+"_"+*sysIt] = reweightKinTH2D(histo2DMap, kinMap, *sysIt, true, kinRw_dep);
	
      }
      
      std::cout << "Finished reweighting kinematic " << kinMap["NAME"] << " for systematic variation " << syst << std::endl << std::endl;

    }

  }
  std::cout << "=======================================================================" << std::endl;
  std::cout << "==============         Exiting reweighting loop          ==============" << std::endl;
  std::cout << "=======================================================================" << std::endl;

  TFile* out = new TFile("test.root","RECREATE");

  std::map< std::string, TH1D* >::iterator kinRw1D_it;

  std::map< std::string, TH2D* >::iterator kinRw2D_it;

  for(kinRw1D_it = kinRw1D.begin(); kinRw1D_it != kinRw1D.end(); kinRw1D_it++){
    
    out->cd();

    (kinRw1D_it->second)->Write();

  }

  for(kinRw2D_it = kinRw2D.begin(); kinRw2D_it != kinRw2D.end(); kinRw2D_it++){

    out->cd();

    (kinRw2D_it->second)->Write();

  }

  out->Close();

  return 0;

}
