#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TMVA/Tools.h>
#include "IFAETopFramework/AnalysisUtils.h"

struct variable{
  std::string name;
  int ind;
  double separation;
  bool retained;
  std::string betterPartner;
  std::string retainedPartner;
  variable(std::string _name, int _index=-1, double _separation=0.) : 
    name(_name),
    ind(_index),
    separation(_separation),
    retained(true),
    betterPartner(""),
    retainedPartner(""){}
  variable() { variable(""); }
  void print_variable(){
    std::cout << "============================"<<std::endl;
    std::cout << " ind : " << ind << std::endl;
    std::cout << " name : " << name << std::endl;
    std::cout << " separation : " << separation << std::endl;
    std::cout << " retained : " << retained << std::endl;
    std::cout << " betterPartner : " << betterPartner << std::endl;
    std::cout << " retainedPartner : " << retainedPartner << std::endl;
    std::cout << "============================"<<std::endl;
  }

};

typedef std::map<std::string, variable> variableMap;

std::string m_inFileName;
std::string m_outFileName;
double m_corlnThresh;
double m_corlnThreshDiff;
double m_separnThresh;
int m_maxN;
bool m_doPlots;

variableMap m_inputVariables;
variableMap m_prunedVariables;
variableMap m_rankedVariables;
TH1F* m_inputCorrelationB = NULL;
TH1F* m_inputCorrelationS = NULL;
TH1D* m_h_separation = NULL;

int ReadTMVAFile(const std::string& fName);
int PruneVariableCorrelations();
int RankSeparations();
int PlotCharacteristics();

std::string FindRetainedPartner(const std::string& varName, const variableMap& varMap);
void printUsage();


int main(int argc, char** argv){
  
  if(argc < 2){
    std::cerr << "ERROR: Insufficient number of arguments." << std::endl;
    printUsage();
    abort();
  }

  m_inFileName = "";
  m_outFileName = "";
  m_corlnThresh = 75.;
  m_corlnThreshDiff = 5.;
  m_separnThresh = 0.1;
  m_maxN = 20;
  m_doPlots = true;

  for(int i = 1; i<argc; i++){

    std::string opt(argv[i]);
    std::string argument,value;
    size_t pos = opt.find("=");
    if(pos == std::string::npos){//the sign = is not found, skip the argument with a warning message                          
      std::cout << "<!> Argument has no '=' sign, skipping : " << opt << std::endl;
      continue;
    }
    argument = opt.substr(0, pos);
    std::transform(argument.begin(), argument.end(), argument.begin(), toupper);//convert to upper case

    value=opt.erase(0, pos + 1);

    if(argument=="--INFILE"){ m_inFileName = value; }
    else if(argument=="--OUTFILE"){ m_outFileName = value; }
    else if(argument=="--CORLNTHRESH"){ m_corlnThresh = std::atof(value.c_str()); }
    else if(argument=="--CORLNTHRESHDIFF"){ m_corlnThreshDiff = std::atof(value.c_str()); }
    else if(argument=="--SEPARNTHRESH"){ m_separnThresh = std::atof(value.c_str()); }
    else if(argument=="--MAXN"){ m_maxN = std::atoi(value.c_str()); }
    else if(argument=="--DOPLOTS"){ 
      m_doPlots = AnalysisUtils::BoolValue(value);
    }
    else{ std::cerr << "Unknown argument : " << argument << "... Skipping." << std::endl; }
  }

  int status = 0;  
  status += ReadTMVAFile(m_inFileName);
  status += PruneVariableCorrelations();
  status += RankSeparations();
  if(m_doPlots){
    status += PlotCharacteristics();
  }

  return status;

}

int ReadTMVAFile(const std::string& fName){

  m_inputVariables = {}; //initial set of variables to start with 
  
  TMVA::Tools& tmva_tool = TMVA::Tools::Instance();

  //Open the file
  TFile* infile = TFile::Open(fName.c_str(), "READ");
  if(!infile || infile->IsZombie())
    {
      std::cout << "Failed to load : " << fName << std::endl;
      return -1;
    }

  //===================== Fill training variable information ==============================

  //Directory containing input variable list:
  TDirectory* IDvar_dir = (TDirectory*)(infile->Get("binclass/InputVariables_Id"));

  //read histograms from the directory
  TObject *obj_bkgd = NULL;
  TObject *obj_sig = NULL;

  TKey *key;
  TIter next( IDvar_dir->GetListOfKeys());
  std::string key_name = "";
  while ((key = (TKey *) next())) {

    obj_bkgd = IDvar_dir->Get(key->GetName());
    if(!(obj_bkgd->InheritsFrom("TH1"))) continue;

    key_name = key->GetName();      

    //Find the background histogram first
    if(key_name.find("__Background_Id") == std::string::npos) continue;

    std::string varname = key_name.substr(0, key_name.find("__Background_Id"));
    std::cout << "Adding variable with name " << varname << std::endl;
    m_inputVariables[varname] = variable(varname);

    //Find the corresponding signal histogram
    std::string signame = varname+"__Signal_Id";
    obj_sig = IDvar_dir->Get(signame.c_str());

    //Then, fill in the signal separation
    double _separation = tmva_tool.GetSeparation((TH1*)obj_sig, (TH1*)obj_bkgd);
    m_inputVariables.at(varname).separation = _separation;

  }
  std::cout << "Extracted " << m_inputVariables.size() << " variables from ID directory. " << std::endl;
  //====================================================================

  //Get the correlation matrices

  m_inputCorrelationB = (TH1F*)(infile->Get("binclass/CorrelationMatrixB"));
  m_inputCorrelationS = (TH1F*)(infile->Get("binclass/CorrelationMatrixS"));
 
  std::cout << " Number of bins in the correlation matrix : " << m_inputCorrelationB->GetXaxis()->GetNbins() 
	    << " x " << m_inputCorrelationB->GetYaxis()->GetNbins() << std::endl;

  //Now, fill in the index of the variables from the CorrelationMatrixB
  for( int i = 1; i <= m_inputCorrelationB->GetXaxis()->GetNbins(); i++){
    std::string varname = m_inputCorrelationB->GetXaxis()->GetBinLabel(i);
    std::cout << " index : " << i << " varname : " << varname << std::endl;
    m_inputVariables.at(varname).ind = i;
  }

  std::cout << " training variable information complete " << std::endl;
  
  //===================== training variable information complete ==============================

  return 0;

}

int PruneVariableCorrelations(){

  //===================== Fill correlation information ==============================

  std::map< int, std::vector<double> > map_abs_correlationB = {}; //abs values of correlation, for sorting

  std::map< int, std::vector<double> > map_correlationB = {};
  std::map< int, std::vector<double> > map_correlationS = {};


  for( int i = 1; i <= m_inputCorrelationB->GetXaxis()->GetNbins(); i++){

    std::string name_i = m_inputCorrelationB->GetXaxis()->GetBinLabel(i);

    if( !(m_inputVariables.at(name_i).retained) ) continue;
    double separn_i = m_inputVariables.at(name_i).separation;

    map_correlationB[i] = {};
    map_correlationS[i] = {};
    int _k=0;
    for( int j = i+1; j <= m_inputCorrelationB->GetYaxis()->GetNbins(); j++){

      map_abs_correlationB[i].push_back(fabs(m_inputCorrelationB->GetBinContent(i,j)));

      map_correlationB[i].push_back(m_inputCorrelationB->GetBinContent(i,j));
      map_correlationS[i].push_back(m_inputCorrelationS->GetBinContent(i,j));

      _k++;
    }

    //======================= Now start ranking the correlations ================================
    std::vector<size_t> sorted_corrB_index = AnalysisUtils::SortVectorIndices(map_abs_correlationB[i]);

    //look at the m_inputVariables that are highest correlated with variable i. For each of them, check if the 
    //signal separation for any of those other m_inputVariables is higher than variable i. If so, discard i in 
    //favour of the one with highest separation

    for(size_t k : sorted_corrB_index){

      double corrB = map_correlationB[i].at(k);
      double corrS = map_correlationS[i].at(k);

      std::string name_k = m_inputCorrelationB->GetYaxis()->GetBinLabel(k+i+1); //bin numbers shifted by i+1 w.r.t. vector index

      if( fabs(corrB) > m_corlnThresh ) {
	if( fabs(corrS) > m_corlnThresh-m_corlnThreshDiff ){

	  if( !(m_inputVariables.at(name_k).retained) ) continue;
	  double separn_k = m_inputVariables.at(name_k).separation;
	  if(separn_k > separn_i){
	    m_inputVariables.at(name_i).retained = false;
	    m_inputVariables.at(name_i).betterPartner = name_k;
	  }
	  else{
	    m_inputVariables.at(name_k).retained = false;
	    m_inputVariables.at(name_k).betterPartner = name_i;
	  }


	}//check signal correlation
      }//check bkgd correlation
      else{
	break; //no need to examine lower correlation m_inputVariables
      }

    }//inner 
    

  }//outer

  for ( std::pair<std::string, variable> var : m_inputVariables){
    m_inputVariables.at(var.first).retainedPartner = FindRetainedPartner(var.second.name, m_inputVariables);
    //m_inputVariables.at(var.first).print_variable();
    if(var.second.retained){
      m_prunedVariables[var.first] = m_inputVariables.at(var.first);
    }

  }

  std::cout << "Retained " << m_prunedVariables.size() << " variables after correlation pruning. " << std::endl;

  return 0;
}

int RankSeparations(){

  std::vector<double> var_separn;
  std::vector<std::string> var_name;

  if(m_doPlots){
    m_h_separation = new TH1D("separations", "", 100, 0., 1.);
  }
  for ( std::pair<std::string, variable> tvar : m_prunedVariables){
    if(m_doPlots){
      m_h_separation -> Fill(tvar.second.separation);
    }
    var_separn.push_back(tvar.second.separation);
    var_name.push_back(tvar.second.name);

  }

  std::vector<size_t> sorted_separn_index = AnalysisUtils::SortVectorIndices(var_separn);

  //Retain only variables that pass separation threshold and only m_maxN variables
  int nvar = 0;
  double separn_0 = -1.;

  std::ofstream outFStr; 
  outFStr.open(m_outFileName.c_str()); 

  outFStr << "Variable : Separation" << std::endl;
  //for ( std::pair<std::string, variable> var : m_rankedVariables){
    //}

  for(int kk : sorted_separn_index){

    if(nvar == 0) separn_0 = var_separn.at(kk);
    else if( var_separn.at(kk) < m_separnThresh * separn_0 ) break;

    if(nvar >= m_maxN) break;

    //std::cout << "Var : " << var_name.at(kk) << " : separation : " << var_separn.at(kk) << std::endl;
    outFStr << var_name.at(kk) << " : " << var_separn.at(kk) << std::endl;
    m_rankedVariables[var_name.at(kk)] = m_prunedVariables.at(var_name.at(kk));
    nvar++;
  }
  outFStr.close();

  return 0;

}

int PlotCharacteristics(){

  int ntrain =  m_rankedVariables.size();

  //====================== Plotting====================== 

  TH2D* h_new_corlnB = new TH2D("final_correlationB", "CorrelationB", ntrain, 0, ntrain, ntrain, 0, ntrain);
  TH2D* h_new_corlnS = new TH2D("final_correlationS", "CorrelationS", ntrain, 0, ntrain, ntrain, 0, ntrain);

  int i_kk = 1; //x-index of new corln matrix
  for( const std::pair<std::string, variable>& varpair_kk : m_rankedVariables ){

    //for(int r = 0; r < nvar; r++){

    //kk = sorted_separn_index.at(r);

    int index_kk = varpair_kk.second.ind; //x-index of old corln matrix
    h_new_corlnB->GetXaxis()->SetBinLabel(i_kk,varpair_kk.second.name.c_str());
    h_new_corlnS->GetXaxis()->SetBinLabel(i_kk,varpair_kk.second.name.c_str());


    //int index_kk = _trainingVars.at(var_name.at(kk)).ind; //x-index of old corln matrix
    int i_mm = 1;  //y-index of new corln matrix

    //h_new_corlnB->GetXaxis()->SetBinLabel(i_kk,var_name.at(kk).c_str());
    //h_new_corlnS->GetXaxis()->SetBinLabel(i_kk,var_name.at(kk).c_str());
    
    //for(int mm : sorted_separn_index){
    for( const std::pair<std::string, variable>& varpair_mm : m_rankedVariables ){


      int index_mm = varpair_mm.second.ind; //x-index of old corln matrix
      h_new_corlnB->GetXaxis()->SetBinLabel(i_mm,varpair_mm.second.name.c_str());
      h_new_corlnS->GetXaxis()->SetBinLabel(i_mm,varpair_mm.second.name.c_str());

      //h_new_corlnB->GetYaxis()->SetBinLabel(i_mm,var_name.at(mm).c_str());
      //h_new_corlnS->GetYaxis()->SetBinLabel(i_mm,var_name.at(mm).c_str());

      //int index_mm = _trainingVars.at(var_name.at(mm)).ind; //y-index of new corln matrix
      h_new_corlnB->SetBinContent(i_kk,i_mm, m_inputCorrelationB->GetBinContent(index_kk,index_mm));
      h_new_corlnS->SetBinContent(i_kk,i_mm, m_inputCorrelationS->GetBinContent(index_kk,index_mm));

      //std::cout << "index_kk : " << index_kk << " (" << var_name.at(kk) << ")"
      //<< "index_mm : " << index_mm << " (" << var_name.at(mm) << ")"
      //<< " corB = " << correlationB->GetBinContent(index_kk,index_mm)
      //<< " corS = " << correlationS->GetBinContent(index_kk,index_mm)
      //<< std::endl;
      i_mm++;
    }

    i_kk++;
  }

  TCanvas* csep = new TCanvas("csep", "", 1000, 1000); csep->cd();
  m_h_separation->Draw();
  TCanvas* cB = new TCanvas("cB", "", 1000, 1000); cB->cd();
  h_new_corlnB->Draw("colztext");
  TCanvas* cS = new TCanvas("cS", "", 1000, 1000); cS->cd();
  h_new_corlnS->Draw("colztext");

  return 0;

}

std::string FindRetainedPartner(const std::string& varName, const variableMap& varMap){

  variable _var = varMap.at(varName);
  if(_var.retained || (_var.betterPartner=="")) return "";

  bool found_retained = false;
  std::string retained_partner = "";

  _var.print_variable();
  while(!found_retained){
    //std::cout << "Better partner for variable "<<_var.name << " is " << _var.betterPartner << " retained : " 
    //	      << varMap.at(_var.betterPartner).retained << std::endl;
    _var = varMap.at(_var.betterPartner);
    found_retained = _var.retained;
    if(found_retained) retained_partner = _var.name;
    else continue;
  }

  std::cout << " Retained partner for variable " << varName << " found to be " << retained_partner << std::endl;

  return retained_partner;

}

void printUsage(){

  std::cout << "Usage: SortMVAInputVariables <options>" << std::endl;
  std::cout << "Arguments : " << std::endl;
  std::cout << "--INFILE <name of input TMVA Root file>" << std::endl;
  std::cout << "--OUTFILE <name of output text file containing optimised variable list>" << std::endl;
  std::cout << "Optional arguments : " << std::endl;
  std::cout << "--CORLNTHRESH <background correlation threshold above which to prune>" << std::endl;
  std::cout << "--CORLNTHRESHDIFF <tolerance threshold of difference between signal and background correlation>" << std::endl;
  std::cout << "--SEPARNTHRESH <separation threshold of last ranked variable as fraction of top separation >" << std::endl;
  std::cout << "--MAXN <maximum number of variables to retain>" << std::endl;
  std::cout << "--DOPLOTS <make correlation and separation plots>" << std::endl;

  return;
}
