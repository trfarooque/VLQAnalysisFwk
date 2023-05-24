/// Based on TMVAClassification by Andreas Hoecker
  
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
 
#include "TMVA/Factory.h"
#include "TMVA/CrossValidation.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

#include "IFAETopFramework/AnalysisUtils.h"
#include "IFAETopFramework/VariableDef.h"
#include "VLQAnalysis/VLQ_MVAManager.h"

//INPUT ARGUMENTS
std::string m_inputTreeDir;
std::string m_outFileName; 
std::string m_prunedVarFileName; 
std::string m_treeName;
std::string m_dataLoaderName;

std::vector<std::string> m_signal_masses; 
std::vector<std::string> m_backgrounds; 
std::vector<std::string> m_varList;
std::string m_mlp_nodes;
std::string m_mlp_ncycles;
std::string m_mlp_neuronType;
std::string m_nfolds;
std::string m_channel;
bool m_doCrossEval;
bool m_useAllVars;
bool m_usePrunedVars;
bool m_doKinRW;

//INTERNAL VARIABLES
VLQ_MVAManager* m_mvaManager;
TMVA::DataLoader* m_dataLoader;
TMVA::Factory* m_factory;
TMVA::CrossValidation* m_crossValid;
TChain *m_sigTree;
std::map<std::string, TChain*> m_bkgdTrees;

int ParseArguments(int argc, char** argv); //
int AddDataLoaderVariable(VariableDef* var, const bool isSpectator=false); //

int SetupTrainingTrees(); //
int InitDataLoader(); //
int InitFactoryAndEvaluate();//
//int InspectResults();

int SetupTrainingTrees(){

  m_sigTree = new TChain(m_treeName.c_str());
  for(const std::string& mass : m_signal_masses){
    m_sigTree->Add((m_inputTreeDir+"/"+"/outVLQAna_VLQ_TT_"+mass+"*TREE.root").c_str());
  }
  m_dataLoader->AddSignalTree( m_sigTree ); 

  m_bkgdTrees = {};
  for(const std::string& proc : m_backgrounds){
    m_bkgdTrees[proc] = new TChain(m_treeName.c_str());
    (m_bkgdTrees.at(proc))->Add((m_inputTreeDir+"/"+"/outVLQAna_"+proc+"*TREE.root").c_str());
    m_dataLoader->AddBackgroundTree(m_bkgdTrees.at(proc)); 
  }

  std::string cutstr = "(meff > 1000 && RCjets_n >=2)";
  std::string cutsig = "";
  if(m_channel=="ONELEP"){
    //cutstr += " && (leptop_pt>100. + RCjets_n)>=4 && (jets_n>=5 && trkbjets_n>=3 && RCMTT_jets_n>=2)";
    cutstr += " && ( (RCjets_n>=4) || (leptop_pt>100. && RCjets_n>=3 ) ) && (jets_n>=5 && bjets_n>=3 && RCMTT_jets_n>=2)";
    cutsig = "(VLQtype==1 || VLQtype==2 ||  VLQtype==3)";
  }
  if(m_channel=="ALLHAD"){
    cutstr += " && (jets_n>=6 && bjets_n>=2 && RCMTT_jets_n>=2)";
    cutsig = "(VLQtype==1 || VLQtype==2 ||  VLQtype==3 || VLQtype==5 || VLQtype==6)";
  }

  TCut trainingCut(cutstr.c_str());
  TCut trainingCut_sig((cutstr+" & "+cutsig).c_str());
 
  TString splitstr = (m_doCrossEval) ? "nTest_Signal=0:nTest_Background=0" : 
    "SplitMode=Random:NormMode=EqualNumEvents";

  m_dataLoader->PrepareTrainingAndTestTree( trainingCut_sig, trainingCut, splitstr);


  return 0;

}


int InitFactoryAndEvaluate(){

  TFile* outputFile = TFile::Open( m_outFileName.c_str(), "RECREATE" );

  TString mlp_opt = TString::Format("!V:NeuronType=%s:VarTransform=N:NCycles=%s:EpochMonitoring=false:HiddenLayers=%s:LearningRate=0.01:DecayRate=0.0:TestRate=5",m_mlp_neuronType.c_str(), m_mlp_ncycles.c_str(), m_mlp_nodes.c_str() );

  if(m_doCrossEval){
    TString splitExpr="Long64_t([event_number])\%int([numFolds])";
    TString opt_cv = 
      TString::Format("!V:!Silent:!Correlations:AnalysisType=Classification:NumFolds=%s:SplitType=Deterministic:SplitExpr=%s"
		      ,m_nfolds.c_str(),splitExpr.Data());
    m_crossValid = new TMVA::CrossValidation("TMVAClassificationCV", m_dataLoader, outputFile, opt_cv);
    m_crossValid->BookMethod( TMVA::Types::kMLP, "MLP", mlp_opt );
    m_crossValid->Evaluate();    
  } 
  else{
    TString opt_fct = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification"; 
    m_factory = new TMVA::Factory( "TMVAClassification", outputFile, opt_fct);
    m_factory->BookMethod(m_dataLoader, TMVA::Types::kMLP, "MLP", mlp_opt );

    // Train MVAs using the set of training events
    m_factory->TrainAllMethods();

    // Evaluate all MVAs using the set of test events
    m_factory->TestAllMethods();

    // Evaluate and compare performance of all configured MVAs
    m_factory->EvaluateAllMethods();

  }
  
  // --------------------------------------------------------------
 
  // Save the output
  outputFile->Close();
 
  std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
  std::cout << "==> MVA training complete" << std::endl;
 
  return 0;

}
/*
int InspectResults(){



    // Launch the GUI for the root macros
    if (!gROOT->IsBatch()){

      // Draw cv-specific graphs
      //cross_validation->GetResults()[0].DrawAvgROCCurve(kTRUE, "Avg ROC for MLP");
 
      TMVA::TMVAGui( m_outfileName );
    }
    return 0;



  return 0;

}
*/
int AddDataLoaderVariable(VariableDef* var, const bool isSpectator){

  //std::string dataType = "";
  char dataType = 'D';

  if(var->VarType() == VariableDef::INT){
    dataType = 'I';
  }
  else if(var->VarType() == VariableDef::LONGINT){
    dataType = 'L';
  }
  else if(var->VarType() == VariableDef::UINT){
    dataType = 'i';
  }
  else if(var->VarType() == VariableDef::ULONGINT){
    dataType = 'l';
  }

  if(isSpectator){
    //m_dataLoader->AddSpectator( var->Name().c_str(),var->Title().c_str(), "", dataType);
    m_dataLoader->AddSpectator( var->Name().c_str(),var->Name().c_str(), "", dataType);
  }
  else{
    //m_dataLoader->AddVariable( var->Name().c_str(),var->Title().c_str(), "", dataType);
    m_dataLoader->AddVariable( var->Name().c_str(),var->Name().c_str(), "", dataType);
  }
  
  return 0;
}

int InitDataLoader(){

  m_mvaManager = new VLQ_MVAManager(NULL/*m_opt*/, NULL/*m_outData*/, NULL/*m_weightMngr*/, 
				    NULL/*m_outMngrMVAHist*/, NULL/*m_outMngrMVATree*/);
  m_mvaManager->MakeVariableStore();

  m_dataLoader = new TMVA::DataLoader(m_dataLoaderName.c_str());

  //====================== Add metadata as spectator ========================
  std::string dataType = "";
  for( std::pair<std::string, VariableDef*> specPair : m_mvaManager->GetMetadataStore()){
    std::cout << specPair.first << std::endl;
    AddDataLoaderVariable(specPair.second, true);
  }

  //====================== Add variables ==============================
  if(m_useAllVars){
    for( std::pair<std::string, VariableDef*> varPair : m_mvaManager->GetVariableStore()){
      if(varPair.first == "leptop_n") continue;
      AddDataLoaderVariable(varPair.second, false);
    }
  }
  else{

    const std::map<std::string, VariableDef*> varStore = m_mvaManager->GetVariableStore();

    if(m_usePrunedVars){
      std::ifstream inFStr;
      inFStr.open(m_prunedVarFileName.c_str());

      std::string line;
      std::string varName;
      //Read the list of pruned variables
      while (std::getline(inFStr, line)){

	if(line.find("Variable : Separation") != std::string::npos){ continue; }
	//varName = line;
	AnalysisUtils::ParseString(line,varName, " : ");
	AnalysisUtils::TrimString(varName);

	if(varStore.find(varName) != varStore.end()){
	  AddDataLoaderVariable( varStore.at(varName), false);
	}
	else{
	  std::cout << "<WARNING> MVATrainer::InitDataLoader--> Unknown variable " << varName << std::endl;
	}

      }//iterate over lines in file

      inFStr.close();

    }//pruned variables
    else{

      for(const std::string& varName : m_varList){

	if(varStore.find(varName) != varStore.end()){
	  AddDataLoaderVariable( varStore.at(varName), false);
	}
	else{
	  std::cout << "<WARNING> MVATrainer::InitDataLoader--> Unknown variable " << varName << std::endl;
	}

      }//iterate over input varList


    }//if not pruned variables

  }//not all variables

  std::vector<std::string> weightList = {
    "weight_jvt", "weight_pu", "weight_norm", "weight_mc", "weight_trkbtag"};
  if(m_doKinRW){
    weightList.push_back("weight_RW_JETSN");
    weightList.push_back("weight_RW_MEFFRED");
  }

  if(m_channel == "ONELEP"){
    std::vector<std::string> weightList_1L = {"weight_elec","weight_elec_trigger","weight_muon","weight_muon_trigger" };
    weightList.insert(weightList.end(), weightList_1L.begin(), weightList_1L.end());
    weightList_1L.clear();
  }

  std::string weightExpression = "";
  bool firstWeight=true;
  for( const std::string& weight : weightList ){
    if(firstWeight){firstWeight=false;}
    else{ weightExpression += "*";}
    weightExpression += "nomWeight_"+weight;
  }
  m_dataLoader->SetWeightExpression (weightExpression.c_str());

  return 0;
}


int ParseArguments(int argc, char** argv){

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

    
    if(argument=="--INPUTTREEDIR"){ 
      //Directory containing input trees
      m_inputTreeDir = value; 
    }
    else if(argument=="--OUTFILENAME"){ 
      //name of output file
      m_outFileName = value; 
    }
    else if(argument=="--PRUNEDVARFILENAME"){ 
      //file containing list of pruned variables
      m_prunedVarFileName = value; 
    }
    else if(argument=="--DATALOADERNAME"){ 
      //file containing list of pruned variables
      m_dataLoaderName = value; 
    }
    else if(argument=="--SIGNALMASSES"){ 
      //comma-separated list of signal masses to be used in training
      m_signal_masses = AnalysisUtils::SplitString(value, ","); 
    }
    else if(argument=="--BACKGROUNDS"){ 
      //comma-separated list of background processes to be used in training
      m_backgrounds = AnalysisUtils::SplitString(value, ","); 
    }
    else if(argument=="--VARLIST"){
      m_varList = AnalysisUtils::SplitString(value,",");
    }
    else if(argument=="--CHANNEL"){ 
      //allowed values: ONELEP,ALLHAD
      m_channel = value; 
      if( !( (m_channel == "ALLHAD") || (m_channel == "ONELEP") ) ){ 
	std::cout << "<WARNING> MVATrainer::ParseArguments --> Invalid CHANNEL " << m_channel << std::endl;
	std::cout << "Setting m_channel to ONELEP" << std::endl;
	m_channel = "ONELEP";
      }
    }
    else if(argument=="--MLPNODES"){ 
      //String describing MLP node architecture
      m_mlp_nodes = value; 
    }
    else if(argument=="--MLPNCYCLES"){ 
      //NCycles for MLP training
      m_mlp_ncycles = value; 
    }
    else if(argument=="--MLPNEURONTYPE"){ 
      //Neuron activation function
      m_mlp_neuronType = value; 
    }
    else if(argument=="--NFOLDS"){
      //number of folds used for cross validation
      m_nfolds = value.c_str();
    }
    else if(argument=="--DOCROSSEVAL"){
      //do cross-validation/cross-evaluation 
      m_doCrossEval = AnalysisUtils::BoolValue(value);
    }
    else if(argument=="--USEALLVARS"){
      //do all variables
      m_useAllVars = AnalysisUtils::BoolValue(value);
    }
    else if(argument=="--USEPRUNEDVARS"){
      //do all variables
      m_usePrunedVars = AnalysisUtils::BoolValue(value);
    }
    else if(argument=="--DOKINRW"){
      //do all variables
      m_doKinRW = AnalysisUtils::BoolValue(value);
    }

  }//loop over all command-line arguments 

    return 0;
    
}


int main( int argc, char** argv )
{

  m_inputTreeDir = "";
  m_outFileName = ""; 
  m_prunedVarFileName = ""; 
  m_treeName = "mvaTree";
  m_dataLoaderName = "binclass";
  m_signal_masses = {"1200","1600","1800"}; 
  m_backgrounds = {"ttbarlight","ttbarcc","ttbarbb","singletop","Wjets","Zjets"}; 
  m_varList = {};
  m_mlp_nodes = "";
  m_mlp_ncycles = "800";
  m_mlp_neuronType = "sigmoid";
  m_nfolds = "5";
  m_channel = "ONELEP";
  m_doCrossEval = true;
  m_useAllVars = false;
  m_usePrunedVars = true;
  m_doKinRW = false;

  //INTERNAL VARIABLES
  m_mvaManager = NULL;
  m_dataLoader = NULL;
  m_factory = NULL;
  m_crossValid = NULL;
  m_sigTree = NULL;
  m_bkgdTrees = {};


  int status = 0;

 status += ParseArguments(argc, argv);
 status += InitDataLoader();
 status += SetupTrainingTrees();


 status += InitFactoryAndEvaluate();
 //if(m_inspectResults){
 //status += InspectResults();
 //}

 return status;


}

 
 
