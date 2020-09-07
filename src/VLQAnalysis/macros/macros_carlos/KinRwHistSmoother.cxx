#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <set>
#include <map>
#include <vector>

#include "TFile.h"
#include "TH1D.h"
#include "TF1.h"
#include "TMatrixD.h"
#include "TVector.h"
#include "TMath.h"
#include "TGraphErrors.h"

#include "Fit/BinData.h"
#include "Math/WrappedMultiTF1.h"
#include "HFitInterface.h"
#include "Fit/Fitter.h"

std::string histoList;
std::string inputFile;
std::string method = "";
double CL = 0.954;
int nSteps = 1000;

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

    if(argument == "--HISTOLIST") histoList = value;
    else if(argument == "--INPUTFILE") inputFile = value;
    else if(argument == "--UNCERTAINTYMETHOD") method = value;
    else if(argument == "--CONFIDENCELEVEL") CL = std::stod(value);
    else if(argument == "--NUMBEROFSTEPS") nSteps = std::stoi(value);


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
      std::cout << "#      Found smoothing option       #" << std::endl;
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

// Function that fits a histogram to a given function 
// Inputs:
// histo: histogram to fit
// fit: TF1 object containing the smooth function that will be used to fit the histogram
// rWMap: map containing configuration options of the histogram to be reweighted
// Output:
// ROOT::Fit::FitResult object that contains the summary of the fit
//______________________________________________________________________________
//
ROOT::Fit::FitResult fitHistogram(TH1D* histo, TF1* fit, std::map< std::string, std::string > &rWMap){

  // Setup the fitter

  ROOT::Fit::DataOptions opt;
  
  // Fit the histogram only the range specified by the user
  ROOT::Fit::DataRange range(std::stod(rWMap["XMIN"]),std::stod(rWMap["XMAX"]));

  ROOT::Fit::BinData data(opt,range);

  // Provide the histogram to fit
  ROOT::Fit::FillData(data, histo);

  // Provide the smooth function template
  ROOT::Math::WrappedMultiTF1 fitFunction(*fit, fit->GetNdim() );

  ROOT::Fit::Fitter fitter;

  fitter.SetFunction( fitFunction, false);

  // Read initial parameters of the smooth function

  std::vector< double > fitParams = {};

  std::vector< std::string > optParams = {};

  std::stringstream fitParamSS(rWMap["PARAMETERS"]);

  std::stringstream optParamSS(rWMap["PARAMOPT"]);

  std::string param;

  std::string paramOpt;

  while(getline(fitParamSS,param,',')) fitParams.push_back(std::stod(param));

  while(getline(optParamSS,paramOpt,',')) optParams.push_back(paramOpt);

  fitter.Config().SetParamsSettings(fitParams.size(), &fitParams[0]);

  std::vector< int > freeParamIndex = {};
  
  // Fix parameters specified by user
  for(unsigned int i = 0; i < optParams.size(); i++){

    if(optParams[i] == "FIXED"){

      fitter.Config().ParSettings(i).Fix();

    }
    else{

      freeParamIndex.push_back(i);

    }

  }

  // Set fit minimizer
  fitter.Config().MinimizerOptions().SetStrategy(0);

  fitter.Config().MinimizerOptions().SetPrintLevel(0);

  fitter.Config().SetMinimizer("Minuit2","Migrad");

  fitter.Fit(data);

  // Perform the fit
  ROOT::Fit::FitResult result = fitter.Result();

  if(result.Status() != 0 && result.Status() != 1){ // start if fit didn't converge with strategy 0

    std::cout << "< WARNING >: The fit did not converge with minimizer status " << result.Status() << std::endl;

    result.Print(std::cout);

    result.PrintCovMatrix(std::cout);

    std::cout << "Retrying with strategy 1" << std::endl;

    fitter.Config().MinimizerOptions().SetStrategy(1);

    fitter.Fit(data);

    result = fitter.Result();

  }
  if(result.Status() != 0 && result.Status() != 1){ // start if fit didn't converge with strategy 1

    std::cout << "< WARNING >: The fit did not converge with minimizer status "<< result.Status() << std::endl;

    result.Print(std::cout);

    result.PrintCovMatrix(std::cout);

    std::cout << "Retrying with strategy 2" << std::endl;

    fitter.Config().MinimizerOptions().SetStrategy(2);

    fitter.Fit(data);

    result = fitter.Result();

  }

  std::cout << "Fit converged with status: " << result.Status() << std::endl;

  result.Print(std::cout);

  fit->SetFitResult(result);

  return result;

}

// Experimental method to calculate the fit uncertainty
//______________________________________________________________________________
//
std::vector< TF1* > getFitEigenVariationsMethodA(TF1* fit, TMatrixD* cov, TMatrixD* eVec, std::map< std::string, std::string > rWMap){

  std::vector< TF1* > fitEigenVariations = {};

  std::stringstream optParamSS(rWMap["PARAMOPT"]);

  std::string paramOpt;

  std::vector< int > freeParamIndex = {};

  std::vector< int > fixedParamIndex = {};

  TMatrixD* eVecT = new TMatrixD(cov->GetNrows(),cov->GetNcols());

  eVecT->Transpose(*eVec);

  for(int i = 0; i < fit->GetNpar(); i++){

    getline(optParamSS, paramOpt, ',');

    if(paramOpt == "FREE") freeParamIndex.push_back(i);
    else fixedParamIndex.push_back(i);

  }

  double* param = new double[freeParamIndex.size()];

  for(unsigned int i = 0; i < freeParamIndex.size(); i++) param[i]  = fit->GetParameter(freeParamIndex[i]);
 
  TVectorD* paramVec = new TVectorD(freeParamIndex.size(), param);

  TVectorD* paramVecEigenSpace = new TVectorD((*eVec) * (*paramVec));

  for(int i = 0; i < paramVecEigenSpace->GetNrows(); i++){
    
    double eigenVariation = 0;

    for(int j = 0; j < paramVecEigenSpace->GetNrows(); j++){

      for(int k = 0; k < paramVecEigenSpace->GetNrows(); k++){

	eigenVariation += ((*eVec)[i][j])*((*eVec)[i][k])*((*cov)[j][k]);

      }
      
    }

    eigenVariation = TMath::Sqrt(eigenVariation);

    TVectorD* paramVecEigenSpaceVar = new TVectorD(*paramVecEigenSpace);

    (*paramVecEigenSpaceVar)[i] += eigenVariation;

    TVectorD* paramVecVar = new TVectorD((*eVecT) * (*paramVecEigenSpaceVar));
    
    //std::cout << "################" << std::endl;
    //paramVecVar->Print();
    //std::cout << "################" << std::endl;

    TF1* fitVar = new TF1((rWMap["NAME"] + "_fit_eigenVar"+std::to_string(i)).c_str(), (rWMap["FUNCTION"]).c_str(), std::stod(rWMap["XMIN"]), 5000);

    for(unsigned int nPar = 0; nPar < freeParamIndex.size(); nPar++) fitVar->SetParameter(freeParamIndex[nPar], (*paramVecVar)[nPar]);

    for(unsigned int nPar = 0; nPar < fixedParamIndex.size(); nPar++) fitVar->SetParameter(fixedParamIndex[nPar], fit->GetParameter(fixedParamIndex[nPar]));


    fitEigenVariations.push_back(fitVar);

  }

  delete [] param;
  
  return fitEigenVariations;

}

// Experimental method to calculate the fit uncertainty based on the SM 4-tops anaylsis method
//______________________________________________________________________________
//
std::vector< TF1* > getFitEigenVariationsMethodB(TF1* fit, TVectorD* eVal, TMatrixD* eVec, std::map< std::string, std::string > rWMap){
  
  std::vector< TF1* > fitEigenVariations = {};

  std::stringstream optParamSS(rWMap["PARAMOPT"]);

  std::string paramOpt;

  std::vector< int > freeParamIndex = {};

  std::vector< int > fixedParamIndex = {};

  TMatrixD* eVecT = new TMatrixD(eVec->GetNrows(),eVec->GetNcols());

  eVecT->Transpose(*eVec);

  for(int i = 0; i < fit->GetNpar(); i++){

    getline(optParamSS, paramOpt, ',');

    if(paramOpt == "FREE") freeParamIndex.push_back(i);
   
    else fixedParamIndex.push_back(i);

  }

  TVectorD* eValSqrt = new TVectorD(eVal->Sqrt());

  //for(int i = 0; i < eVecT->GetNo)

  //TVectorD* eValSqrt = (TVectorD*) (eVal->Sqrt());

  TVectorD* eigenVarsUp = new TVectorD((*eVecT) * (*eValSqrt));

  TVectorD* eigenVarsDn = new TVectorD((*eVecT) * (*eValSqrt));

  TF1* fitVarUp = new TF1((rWMap["NAME"] + "_fit_eigenVar_Up").c_str(), (rWMap["FUNCTION"]).c_str(), std::stod(rWMap["XMIN"]), 5000);
  
  TF1* fitVarDn = new TF1((rWMap["NAME"] + "_fit_eigenVar_Dn").c_str(), (rWMap["FUNCTION"]).c_str(), std::stod(rWMap["XMIN"]), 5000);

  for(unsigned int nPar = 0; nPar < freeParamIndex.size(); nPar++){

    (*eigenVarsUp)[nPar] = fit->GetParameter(freeParamIndex[nPar]) + (*eigenVarsUp)[nPar];
    
    (*eigenVarsDn)[nPar] = fit->GetParameter(freeParamIndex[nPar]) - (*eigenVarsDn)[nPar];

  }

  for(unsigned int nPar = 0; nPar < freeParamIndex.size(); nPar++){

    fitVarUp->SetParameter(freeParamIndex[nPar], (*eigenVarsUp)[nPar]);
    
    fitVarDn->SetParameter(freeParamIndex[nPar], (*eigenVarsDn)[nPar]);

  }

  for(unsigned int nPar = 0; nPar < fixedParamIndex.size(); nPar++){

    fitVarUp->SetParameter(fixedParamIndex[nPar], fit->GetParameter(fixedParamIndex[nPar]));

    fitVarDn->SetParameter(fixedParamIndex[nPar], fit->GetParameter(fixedParamIndex[nPar]));

  }

  //std::cout << "#######Up#######" << std::endl;
  //eigenVarsUp->Print();
  //std::cout << "#######Dn#######" << std::endl;
  //eigenVarsDn->Print();
  //std::cout << "################" << std::endl;

  fitEigenVariations.push_back(fitVarUp);

  fitEigenVariations.push_back(fitVarDn);

  return fitEigenVariations;

}

// Function that calculates the confidence interval of a histogram fit
// Inputs:
// fit: TF1 containing the template of the histogram fit
// cov: Covariance matrix of the fit
// rWMap: map containing the options of the histogram to fit
// xStepSize: Number of points to calculate the fit confidence interval along the x-axis
// cl: Confidence level to calculate the confidence interval
// upBand: TGraph object used to store the upper band of the confidence interval
// dnBand: TGraph object used to store the down band of the confidence interval
//______________________________________________________________________________
//
void getFitCI(TF1* fit, TMatrixD* cov, std::map< std::string, std::string > rWMap, int xStepSize, double cl, TGraph* upBand, TGraph* dnBand){

  std::vector< TGraph* > fitCIBands = {};
  
  //TGraphErrors* fitCIGraph = new TGraphErrors(xStepSize);

  // calculate the grid spacing along the x-axis to calculate the CI
  double deltaX = (5000.0 - std::stod(rWMap["XMIN"]))/xStepSize;

  // calculate the correction factor needed for the CI
  double correctionFactor = TMath::StudentQuantile(0.5 + cl/2.0, fit->GetNDF())*TMath::Sqrt(fit->GetChisquare()/fit->GetNDF());

  std::stringstream optParamSS(rWMap["PARAMOPT"]);

  std::string paramOpt;

  std::vector< int > freeParamIndex = {};

  std::vector< int > fixedParamIndex = {};

  for(int i = 0; i < fit->GetNpar(); i++){

    getline(optParamSS, paramOpt, ',');

    if(paramOpt == "FREE") freeParamIndex.push_back(i);
    else fixedParamIndex.push_back(i);

  }

  double xmin = std::stod(rWMap["XMIN"]);

  // vector to hold the values of the derivatives of the fit w.r.t the free parameters
  std::vector< double > grad(freeParamIndex.size());

  for(int ndx = 0; ndx < xStepSize; ndx++){ // start loop through the grid points along the x-axis

    // Calculate the derivative of the fit w.r.t the free parameters numerically

    for(unsigned int ipar = 0; ipar < freeParamIndex.size(); ipar++){ // start loop through free parameters of the fit
      
      // step size for the deriviate w.r.t the free parameter
      double parStepSize = 0;

      //std::cout << fit->GetParError(freeParamIndex[ipar]) << std::endl;

      if(fit->GetParError(freeParamIndex[ipar]) > 0) parStepSize = std::max(fit->GetParError(freeParamIndex[ipar]) * 1.E-5, 1.E-15);
      else parStepSize = std::min(0.0001, std::max(fit->GetParameter(freeParamIndex[ipar]) * 1.E-5, 1.E-15 ) );
      
      TF1* f_par_ph = new TF1(*fit);

      f_par_ph->SetParameter(freeParamIndex[ipar], fit->GetParameter(freeParamIndex[ipar]) + parStepSize);

      TF1* f_par = new TF1(*fit);

      TF1* f_par_mh = new TF1(*fit);

      f_par_mh->SetParameter(freeParamIndex[ipar], fit->GetParameter(freeParamIndex[ipar]) - parStepSize);

      TF1* f_par_m2h = new TF1(*fit);
      
      f_par_m2h->SetParameter(freeParamIndex[ipar], fit->GetParameter(freeParamIndex[ipar]) - 2.0*parStepSize);


      // estimate the first derivative as f'(p) = [2.0*f(p+h) + 3.0*f(p) - 6.0*f(p-h) + f(p-2h)]/(6.0*h)
      grad[ipar] = (2.0*( f_par_ph->Eval( xmin+( ndx*deltaX ) ) ) 
		    + 3.0*( f_par->Eval( xmin+( ndx*deltaX ) ) ) 
		    - 6.0*( f_par_mh->Eval( xmin+( ndx*deltaX ) ) ) 
		    + f_par_m2h->Eval( xmin+( ndx*deltaX ) ) )/(6.0*parStepSize);

    } // end loop through free parameters of the fit

    // variable used to store the standard deviation of the fit
    double r2 = 0;
    
    // Calculate the standard deviation of the fit at the current grid point along the x-axis
    for(unsigned int ipar = 0; ipar < freeParamIndex.size(); ipar++){

      for(unsigned int jpar = 0; jpar < freeParamIndex.size(); jpar++){

	r2 += grad[ipar]*((*cov)[ipar][jpar])*grad[jpar];

      }

    }

    double r = TMath::Sqrt(r2)*correctionFactor;

    //fitCIGraph->SetPoint(ndx, xmin+( ndx*deltaX ), fit->Eval( xmin+( ndx*deltaX ) ) );
    //fitCIGraph->SetPointError(ndx, 0, r);

    // propagate the up variation of the confidence interval
    upBand->SetPoint(ndx, xmin+( ndx*deltaX ), fit->Eval( xmin+( ndx*deltaX ) ) + r);
    
    // propagate the down variation of the confidence inteval
    dnBand->SetPoint(ndx, xmin+( ndx*deltaX ), fit->Eval( xmin+( ndx*deltaX ) ) - r);

  }

  //return fitCIGraph;

}

//______________________________________________________________________________
//
int main(int argc, char** argv){

  parseUserOptions(argc, argv);

  std::cout << "=======================================================================" << std::endl;
  std::cout << "=================      Reading smoothing options      =================" << std::endl;
  std::cout << "=======================================================================" << std::endl;
  std::vector < std::map< std::string, std::string > > rWOpt = readFromTextFile(histoList,':');
  std::cout << "=======================================================================" << std::endl << std::endl;;

  TFile* input = TFile::Open(inputFile.c_str(), "UPDATE");

  std::cout << "=======================================================================" << std::endl;
  std::cout << "=================       Entering smoothing loop       =================" << std::endl;
  std::cout << "=======================================================================" << std::endl;
  for(std::map < std::string, std::string> rWMap : rWOpt){

    // Get histogram to fit
    TH1D* histo = (TH1D*) input->Get(rWMap["NAME"].c_str());

    //Get smooth function template to fit the histogram
    TF1* fit = new TF1((rWMap["RENAME"]).c_str(), (rWMap["FUNCTION"]).c_str(), std::stod(rWMap["XMIN"]), 5000);

    std::cout << "Smoothing histogram " << histo->GetName() << std::endl;

    // Fit the function to the histogram
    ROOT::Fit::FitResult* result = new ROOT::Fit::FitResult(fitHistogram(histo, fit, rWMap));

    // Obtain the covariance matrix of the fit
    TMatrixD* cov = new TMatrixD(result->NTotalParameters(),result->NTotalParameters());

    result->GetCovarianceMatrix(*cov);

    //===============================================================================
    // Resize the covariance matrix to be consistent with the number of free parameters

    std::vector< std::string > optParams = {};

    std::stringstream optParamSS(rWMap["PARAMOPT"]);

    std::string paramOpt;

    while(getline(optParamSS,paramOpt,',')) optParams.push_back(paramOpt);

    std::vector< int > freeParamIndex = {};

    for(unsigned int i = 0; i < optParams.size(); i++){

      if(optParams[i] != "FIXED"){

	freeParamIndex.push_back(i);

      }

    }

    if(result->NFreeParameters() != result->NTotalParameters()){
      
      for(unsigned int i = 0; i < freeParamIndex.size(); i++){

	TMatrixDColumn(*cov, i) = TMatrixDColumn(*cov, freeParamIndex[i]);

	TMatrixDRow(*cov, i) = TMatrixDRow(*cov, freeParamIndex[i]);

      }

      cov->ResizeTo(freeParamIndex.size(),freeParamIndex.size());

    }
    //================================================================================
    
    cov->Print();

    input->cd();

    fit->Write();

    TVectorD* eVal = new TVectorD(cov->GetNcols());
    
    // calculate the eigen values and eigen vectors of the fit covariance matrix
    TMatrixD* eVec = new TMatrixD(cov->EigenVectors(*eVal));

    std::cout << "Eigen vectors:" << std::endl;

    eVec->Print();

    std::cout << "Calculating fit uncertainty for " << histo->GetName() << std::endl;

    if(method == "A"){
      std::vector< TF1* > fitEigenVariationsA = getFitEigenVariationsMethodA(fit, cov, eVec, rWMap);

      for(TF1* variation : fitEigenVariationsA){
	
	input->cd();

	variation->Write();

      } 

    }
    else if(method == "B"){
      std::vector< TF1* > fitEigenVariationsB = getFitEigenVariationsMethodB(fit, eVal, eVec, rWMap);
      
      for(TF1* variation : fitEigenVariationsB){
	
	input->cd();

	variation->Write();

      }
      
    }
    else{

      TGraph* upBand = new TGraph(1000);

      TGraph* dnBand = new TGraph(1000);

      getFitCI(fit, cov, rWMap, nSteps, CL, upBand, dnBand);

      TF1* fitUpBand = new TF1((std::string(fit->GetName()) + "_upBand").c_str(), [&](double* x, double* p){ return p[0]*upBand->Eval(x[0]); }, std::stod(rWMap["XMIN"]), 5000, 1 );

      TF1* fitDnBand = new TF1((std::string(fit->GetName()) + "_dnBand").c_str(), [&](double* x, double* p){ return p[0]*dnBand->Eval(x[0]); }, std::stod(rWMap["XMIN"]), 5000, 1 );

      fitUpBand->SetParameter(0,1.0);
      
      fitDnBand->SetParameter(0,1.0);
      
      input->cd();
      
      fitUpBand->Write();
      
      input->cd();

      fitDnBand->Write();
      
      /*fitCI->SetName((std::string(histo->GetName()) + "_CI2S").c_str());

      input->cd();
      
      fitCI->Write();*/
      
    }

  }
  std::cout << "=======================================================================" << std::endl;
  std::cout << "==============          Exiting smoothing loop           ==============" << std::endl;
  std::cout << "=======================================================================" << std::endl;

  input->Close();

  return 0;

}
