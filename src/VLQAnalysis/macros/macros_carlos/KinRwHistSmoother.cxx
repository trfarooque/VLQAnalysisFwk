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

  }

}

//______________________________________________________________________________
//
std::vector< std::map< std::string, std::string > > readFromTextFile(std::string fileName, char delim){
  
  std::cout << "=======================================================================" << std::endl;
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
  
  std::cout << "=======================================================================" << std::endl;
  
  return vectorMap;

}

//______________________________________________________________________________
ROOT::Fit::FitResult fitHistogram(TH1D* histo, TF1* fit, std::map< std::string, std::string > &rWMap){

  ROOT::Fit::DataOptions opt;

  ROOT::Fit::DataRange range(std::stod(rWMap["XMIN"]),std::stod(rWMap["XMAX"]));

  ROOT::Fit::BinData data(opt,range);

  ROOT::Fit::FillData(data, histo);

  ROOT::Math::WrappedMultiTF1 fitFunction(*fit, fit->GetNdim() );

  ROOT::Fit::Fitter fitter;

  fitter.SetFunction( fitFunction, false);

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

  for(int i = 0; i < optParams.size(); i++){

    if(optParams[i] == "FIXED"){

      fitter.Config().ParSettings(i).Fix();

    }
    else{

      freeParamIndex.push_back(i);

    }

  }

  fitter.Config().MinimizerOptions().SetStrategy(0);

  fitter.Config().MinimizerOptions().SetPrintLevel(0);

  fitter.Config().SetMinimizer("Minuit2","Migrad");

  fitter.Fit(data);

  ROOT::Fit::FitResult result = fitter.Result();

  if(result.Status() != 0 && result.Status() != 1){

    std::cout << "< WARNING >: The fit did not converge with minimizer status " << result.Status() << std::endl;

    result.Print(std::cout);

    result.PrintCovMatrix(std::cout);

    std::cout << "Retrying with strategy 1" << std::endl;

    fitter.Config().MinimizerOptions().SetStrategy(1);

    fitter.Fit(data);

    result = fitter.Result();

  }
  if(result.Status() != 0 && result.Status() != 1){

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

  //std::cout << fit->GetChisquare() << " " << fit->GetNDF() << " " << fit->GetParError(0) << " " << fit->GetParError(3) << std::endl;

  //std::cout << result.Chi2() << " " << result.Ndf() << " " << result.ParError(0) << " " << result.ParError(3) << std::endl;

  return result;

}

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

  for(int i = 0; i < freeParamIndex.size(); i++) param[i]  = fit->GetParameter(freeParamIndex[i]);
 
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

    for(int nPar = 0; nPar < freeParamIndex.size(); nPar++) fitVar->SetParameter(freeParamIndex[nPar], (*paramVecVar)[nPar]);

    for(int nPar = 0; nPar < fixedParamIndex.size(); nPar++) fitVar->SetParameter(fixedParamIndex[nPar], fit->GetParameter(fixedParamIndex[nPar]));


    fitEigenVariations.push_back(fitVar);

  }

  delete [] param;
  
  return fitEigenVariations;

}

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

  for(int nPar = 0; nPar < freeParamIndex.size(); nPar++){

    (*eigenVarsUp)[nPar] = fit->GetParameter(freeParamIndex[nPar]) + (*eigenVarsUp)[nPar];
    
    (*eigenVarsDn)[nPar] = fit->GetParameter(freeParamIndex[nPar]) - (*eigenVarsDn)[nPar];

  }

  for(int nPar = 0; nPar < freeParamIndex.size(); nPar++){

    fitVarUp->SetParameter(freeParamIndex[nPar], (*eigenVarsUp)[nPar]);
    
    fitVarDn->SetParameter(freeParamIndex[nPar], (*eigenVarsDn)[nPar]);

  }

  for(int nPar = 0; nPar < fixedParamIndex.size(); nPar++){

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

//______________________________________________________________________________
//
TGraphErrors*  getFitCI(TF1* fit, TMatrixD* cov, std::map< std::string, std::string > rWMap, int xStepSize, double cl, TGraph* upBand, TGraph* dnBand){

  std::vector< TGraph* > fitCIBands = {};
  
  TGraphErrors* fitCIGraph = new TGraphErrors(xStepSize);

  double deltaX = (5000.0 - std::stod(rWMap["XMIN"]))/xStepSize;

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

  std::vector< double > grad(freeParamIndex.size());

  for(int ndx = 0; ndx < xStepSize; ndx++){

    for(int ipar = 0; ipar < freeParamIndex.size(); ipar++){

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

      grad[ipar] = (2.0*( f_par_ph->Eval( xmin+( ndx*deltaX ) ) ) 
		    + 3.0*( f_par->Eval( xmin+( ndx*deltaX ) ) ) 
		    - 6.0*( f_par_mh->Eval( xmin+( ndx*deltaX ) ) ) 
		    + f_par_m2h->Eval( xmin+( ndx*deltaX ) ) )/(6.0*parStepSize);


      //for(int jpar = 0; jpar < fit->GetNpar(); jpar++) std::cout << fit->GetParameter(jpar) << " " << f_par_ph->GetParameter(jpar) << " " << f_par->GetParameter(jpar) << " " << f_par_mh->GetParameter(jpar) << " " << f_par_m2h->GetParameter(jpar) << std::endl;
    
    }

    double r2 = 0;

    for(int ipar = 0; ipar < freeParamIndex.size(); ipar++){

      for(int jpar = 0; jpar < freeParamIndex.size(); jpar++){

	r2 += grad[ipar]*((*cov)[ipar][jpar])*grad[jpar];

      }

    }

    double r = TMath::Sqrt(r2)*correctionFactor;

    fitCIGraph->SetPoint(ndx, xmin+( ndx*deltaX ), fit->Eval( xmin+( ndx*deltaX ) ) );

    fitCIGraph->SetPointError(ndx, 0, r);

    upBand->SetPoint(ndx, xmin+( ndx*deltaX ), fit->Eval( xmin+( ndx*deltaX ) ) + r);
    
    dnBand->SetPoint(ndx, xmin+( ndx*deltaX ), fit->Eval( xmin+( ndx*deltaX ) ) - r);

    //std::cout << r << std::endl;

  }

  return fitCIGraph;

}

//______________________________________________________________________________
//
int main(int argc, char** argv){

  parseUserOptions(argc, argv);

  std::vector < std::map< std::string, std::string > > rWOpt = readFromTextFile(histoList,':');

  TFile* input = TFile::Open(inputFile.c_str(), "UPDATE");

  std::cout << "Starting to fit histograms" << std::endl;

  for(std::map < std::string, std::string> rWMap : rWOpt){
    
    TH1D* histo = (TH1D*) input->Get(rWMap["NAME"].c_str());

    TF1* fit = new TF1((rWMap["RENAME"]).c_str(), (rWMap["FUNCTION"]).c_str(), std::stod(rWMap["XMIN"]), 5000);

    std::cout << "=======================================================================" << std::endl;
    std::cout << "Fitting histogram " << histo->GetName() << std::endl;

    ROOT::Fit::FitResult* result = new ROOT::Fit::FitResult(fitHistogram(histo, fit, rWMap));

    TMatrixD* cov = new TMatrixD(result->NTotalParameters(),result->NTotalParameters());

    result->GetCovarianceMatrix(*cov);

    //===============================================================================

    std::vector< std::string > optParams = {};

    std::stringstream optParamSS(rWMap["PARAMOPT"]);

    std::string paramOpt;

    while(getline(optParamSS,paramOpt,',')) optParams.push_back(paramOpt);

    std::vector< int > freeParamIndex = {};

    for(int i = 0; i < optParams.size(); i++){

      if(optParams[i] != "FIXED"){

	freeParamIndex.push_back(i);

      }

    }

    if(result->NFreeParameters() != result->NTotalParameters()){
      
      for(int i = 0; i < freeParamIndex.size(); i++){

	TMatrixDColumn(*cov, i) = TMatrixDColumn(*cov, freeParamIndex[i]);

	TMatrixDRow(*cov, i) = TMatrixDRow(*cov, freeParamIndex[i]);

      }

      cov->ResizeTo(freeParamIndex.size(),freeParamIndex.size());

    }
    //================================================================================
    
    cov->Print();

    input->cd();

    fit->Write();

    //================================================================================
    
    /*TGraphErrors* confidenceIntGraph = new TGraphErrors();

    std::string ci_name = std::string(histo->GetName()) + "_confidence_interval";

    // This function by decault normalizes the confidence interval by the chi2/NDOF
    // https://root.cern.ch/doc/v608/HFitInterface_8cxx_source.html#l00976
    // The correction factor is given by TMath::StudentQuantile(0.5 + cl/2, fNdf) * std::sqrt(fChi2/fNdf))
    // The method that is used to calculate the CI can be found here
    // https://root.cern.ch/doc/v608/FitResult_8cxx_source.html#l00560
    std::cout << "blah" << std::endl;
    ROOT::Fit::GetConfidenceIntervals(histo, *result, confidenceIntGraph, 0.68);

    confidenceIntGraph->SetName((ci_name).c_str());

    std::cout << "Confidence interval values" << std::endl;

    confidenceIntGraph->Print();

    input->cd();

    confidenceIntGraph->Write();*/
    //================================================================================

    TVectorD* eVal = new TVectorD(cov->GetNcols());

    TMatrixD* eVec = new TMatrixD(cov->EigenVectors(*eVal));

    /*TMatrixD* eMtxCheck = new TMatrixD( (*cov) * (*eVec));

    for(int nCols = 0; nCols < cov->GetNcols(); nCols++){

      TVectorD* eVecCheck = new TVectorD(cov->GetNrows());

      for(int nRows = 0; nRows < cov->GetNrows(); nRows++){

	(*eVecCheck)[nRows] = ((*eMtxCheck)[nRows][nCols])/((*eVal)[nCols]);

      }


      eVecCheck->Print();

      }*/

    std::cout << "Eigen vectors:" << std::endl;

    eVec->Print();

    std::cout << "Calculating fit parameter eigenvariations for " << histo->GetName() << std::endl;

    std::vector< TF1* > fitEigenVariationsA = getFitEigenVariationsMethodA(fit, cov, eVec, rWMap);

    std::vector< TF1* > fitEigenVariationsB = getFitEigenVariationsMethodB(fit, eVal, eVec, rWMap);

    TGraph* upBand1S = new TGraph(1000);

    TGraph* dnBand1S = new TGraph(1000);

    TGraph* upBand2S = new TGraph(1000);

    TGraph* dnBand2S = new TGraph(1000);

    TGraph* upBand3S = new TGraph(1000);

    TGraph* dnBand3S = new TGraph(1000);

    TGraphErrors* fitCI1S = getFitCI(fit, cov, rWMap, 1000, 0.682, upBand1S, dnBand1S); 

    TGraphErrors* fitCI2S = getFitCI(fit, cov, rWMap, 1000, 0.954, upBand2S, dnBand2S);

    TGraphErrors* fitCI3S = getFitCI(fit, cov, rWMap, 1000, 0.996, upBand3S, dnBand3S);

    //std::cout << upBand->Eval(3500) << " " << fit->Eval(3500) << " " << dnBand->Eval(3500) << std::endl;

    TF1* fitUpBand1S = new TF1((std::string(fit->GetName()) + "_upBand1S").c_str(), [&](double* x, double* p){ return p[0]*upBand1S->Eval(x[0]); }, std::stod(rWMap["XMIN"]), 5000, 1 );

    TF1* fitDnBand1S = new TF1((std::string(fit->GetName()) + "_dnBand1S").c_str(), [&](double* x, double* p){ return p[0]*dnBand1S->Eval(x[0]); }, std::stod(rWMap["XMIN"]), 5000, 1 );

    TF1* fitUpBand2S = new TF1((std::string(fit->GetName()) + "_upBand2S").c_str(), [&](double* x, double* p){ return p[0]*upBand2S->Eval(x[0]); }, std::stod(rWMap["XMIN"]), 5000, 1 );

    TF1* fitDnBand2S = new TF1((std::string(fit->GetName()) + "_dnBand2S").c_str(), [&](double* x, double* p){ return p[0]*dnBand2S->Eval(x[0]); }, std::stod(rWMap["XMIN"]), 5000, 1 );

    TF1* fitUpBand3S = new TF1((std::string(fit->GetName()) + "_upBand3S").c_str(), [&](double* x, double* p){ return p[0]*upBand3S->Eval(x[0]); }, std::stod(rWMap["XMIN"]), 5000, 1 );

    TF1* fitDnBand3S = new TF1((std::string(fit->GetName()) + "_dnBand3S").c_str(), [&](double* x, double* p){ return p[0]*dnBand3S->Eval(x[0]); }, std::stod(rWMap["XMIN"]), 5000, 1 );

    fitUpBand1S->SetParameter(0,1.0);

    fitDnBand1S->SetParameter(0,1.0);

    fitUpBand2S->SetParameter(0,1.0);

    fitDnBand2S->SetParameter(0,1.0);

    fitUpBand3S->SetParameter(0,1.0);

    fitDnBand3S->SetParameter(0,1.0);

    /*input->cd();

    fitUpBand1S->Write();

    input->cd();

    fitDnBand1S->Write();*/

    input->cd();

    fitUpBand2S->Write();

    input->cd();

    fitDnBand2S->Write();

    /*input->cd();

    fitUpBand3S->Write();

    input->cd();

    fitDnBand3S->Write();*/

    fitCI1S->SetName((std::string(histo->GetName()) + "_CI1S").c_str());

    fitCI2S->SetName((std::string(histo->GetName()) + "_CI2S").c_str());

    fitCI3S->SetName((std::string(histo->GetName()) + "_CI3S").c_str());

    input->cd();

    fitCI1S->Write();

    input->cd();

    fitCI2S->Write();

    input->cd();

    fitCI3S->Write();

    /*for(TF1* fitVar : fitEigenVariationsA){

      input->cd();
      
      fitVar->Write();

    }

    for(TF1* fitVar : fitEigenVariationsB){

      input->cd();

      fitVar->Write();

    }*/


  }

  input->Close();

  return 0;

}
