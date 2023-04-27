/// Based on TMVAClassification by Andreas Hoecker
  
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
 
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

#include "VLQAnalysis/VLQ_MVAManager.h"

//INPUT ARGUMENTS
std::string m_prunedVarFileName; 
std::string m_inputTreeDir;
std::string m_outFileName; 
std::vector<std::string> m_signal_masses; 
std::vector<std::string> m_backgrounds; 
bool m_doCV;
bool m_doAllVars;
bool m_readPrunedVars;
int m_nfolds;


int ParseArguments(int argc, char* argv**){

//file containing list of pruned variables
std::string m_prunedVarFileName; 
//Directory containing input trees
std::string m_inputTreeDir;
//name of output file
std::string m_outFileName; 
//comma-separated list of signal masses to be used in training
std::vector<std::string> m_signal_masses; 
//comma-separated list of background processes to be used in training
std::vector<std::string> m_backgrounds; 
//do cross-validation/cross-evaluation 
bool m_doCV;
//do all variables
bool m_doAllVars;
//read input variables from list of pruned variables
bool m_readPrunedVars;
//number of folds used for cross validation
int m_nfolds;

}

int ParseArguments(int argc, char* argv**);
int ReadInputVariables(); // Either from MVAManager, or from factory
int SetupTrainingTrees();
int InitialiseFactory();

int main( int argc, char** argv )
{




  bool lepton = false;
  // Select methods (don't look at this code - not of interest)
  TString methodList;
  for (int i=1; i<argc; i++) {
    TString regMethod(argv[i]);
    if( regMethod == "--lepton" ) lepton = true;
    else{
      if(regMethod=="-b" || regMethod=="--batch") continue;
      if (!methodList.IsNull()) methodList += TString(",");
      methodList += regMethod;
    }
  }
  return TMVAClassification(methodList,lepton);
}





int TMVAClassification( TString myMethodList = "", bool leptonic )
{
  // The explicit loading of the shared libTMVA is done in TMVAlogon.C, defined in .rootrc
  // if you use your private .rootrc, or run from a different directory, please copy the
  // corresponding lines from .rootrc
 
  // Methods to be processed can be given as an argument; use format:
  //
  //     mylinux~> root -l TMVAClassification.CUndefined control sequence \"
 
  //---------------------------------------------------------------
  // This loads the library
  TMVA::Tools::Instance();
 
  // Default MVA methods to be trained + tested
  std::map<std::string,int> Use;
 
  //
  // Neural Networks (all are feed-forward Multilayer Perceptrons)
  Use["MLP"]             = 1; // Recommended ANN he cambiado aqui el 1, no se que hace realmente 
  Use["DNN_CPU"]         = 0; // Multi-core accelerated DNN.
  //
  // ---------------------------------------------------------------
 
  std::cout << std::endl;
  std::cout << "==> Start TMVAClassification" << std::endl;
 
  // Select methods (don't look at this code - not of interest)
  if (myMethodList != "") {
    for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;
 
    std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
    for (UInt_t i=0; i<mlist.size(); i++) {
      std::string regMethod(mlist[i]);
 
      if (Use.find(regMethod) == Use.end()) {
	std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
	for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
	std::cout << std::endl;
	return 1;
      }
      Use[regMethod] = 1;
    }
  }
 
  // --------------------------------------------------------------------------------------------------
 
  // Here the preparation phase begins
 
  // Read training and test data

  const std::string& inputDir = "/data/at3/scratch2/tfarooque/VLQAnalysisRun2/" ; //make this an input argument
    //: "/data/at3/scratch2/cseth/VLQAnalysisRun2/";

  std::vector<std::string> sigMasses = {"1200","1400","1600","1800"};
  std::vector<std::string> bkgdProcs = {"ttbarlight", "ttbarcc", "ttbarbb", "singletop", "Wjets", "Zjets"};

  TChain *sigTree = new TChain("mvaTree");
  for(const std::string& mass : sigMasses){
    sigTree->Add((inputDir+"/"+campaign+"/outVLQAna_VLQ_TT_"+mass+"*TREE.root").c_str());
  }

  std::map<std::string, TChain*> bkgdTrees = {};
  for(const std::string& proc : bkgdProcs){
    bkgdTrees[proc] = new TChain("mvaTree");
    (bkgdTrees.at(proc))->Add((inputDir+"/"+campaign+"/outVLQAna_"+proc+"*TREE.root").c_str());
  }
 
  // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
  //TString outfileName( "Training3Mass_ReducedList700Cycles_L30_10_10_5_fold5.root" );
  const std::string* outFileName = "Training3Mass_ReducedList700Cycles_L30_10_10_5_fold5.root";
  TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
 
  // Create the factory object. Later you can choose the methods
  // whose performance you'd like to investigate. The factory is
  // the only TMVA object you have to interact with
  //
  // The first argument is the base of the name of all the
  // weightfiles in the directory weight/
  //
  // The second argument is the output file for the training results
  // All TMVA output can be suppressed by removing the "!" (not) in
  // front of the "Silent" argument in the option string

  TString opt_fct = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification"; 


  TString splitExpr="int([event_number])\%int([numFolds])";

  TString opt_cv = TString::Format("!V:!Silent:!Correlations:AnalysisType=Classification:NumFolds=5:SplitType=Deterministic:SplitExpr=\%s",splitExpr.Data());


  //TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile, opt_fct);


  TMVA::DataLoader *dataloader=new TMVA::DataLoader("binclass");
  TMVA::CrossValidation *cross_validation = new TMVA::CrossValidation("TMVAClassificationCV", dataloader, outputFile, opt_cv);

  //When a training tree is given, you can either add *all* of its variables, or read them from a json or text file produced by 
  //



  // If you wish to modify default settings
  // (please check "src/Config.h" to see all available global options)
  //
  //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
  //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";
  // Define the input variables that shall be used for the MVA training
  // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
  // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]

  //**************************************************//
  //********** SET 1 *********************************//

  //Metadata variables are always added as spectators
  dataloader->AddSpectator( "event_number","","", 'l' ); //TRAIN
  dataloader->AddSpectator( "VLQtype", "","", 'I' );
  dataloader->AddSpectator( "sampleID", "","", 'I' );
  dataloader->AddSpectator( "sampleName", "","", 'I' );

   

  dataloader->AddVariable( "meff","","GeV", 'D' ); //TRAIN
  //RC angular variables
  dataloader->AddVariable ( "leadingdEta_RC :=leadingdEta_RCjets", "", "", 'D' ); //TRAIN
  dataloader->AddSpectator( "leadingdEta_RCTTM := leadingdEta_RCTTMassRCTTMass", "", "", 'D' );
  dataloader->AddSpectator( "leadingdPhi_RC := leadingdPhi_RCjets", "", "", 'D' );
  dataloader->AddSpectator( "leadingdPhi_RCTTM := leadingdPhi_RCTTMassRCTTMass", "", "", 'D' );
  dataloader->AddVariable ( "leadingdR_RC := leadingdR_RCjets", "", "", 'D' ); //TRAIN
  dataloader->AddVariable ( "leadingdR_RCTTM := leadingdR_RCTTMassRCTTMass", "", "", 'D' ); //TRAIN
  dataloader->AddSpectator( "dEtamin_RC := dEtamin_RCjets", "", "", 'F' );
  dataloader->AddVariable ( "dEtamin_RCTTM := dEtamin_RCTTMassRCTTMass", "", "", 'F' ); //TRAIN
  dataloader->AddSpectator( "dPhimin_RC := dPhimin_RCjets", "", "", 'F' );
  dataloader->AddSpectator( "dPhimin_RCTTM := dPhimin_RCTTMassRCTTMass", "", "", 'F' );
  dataloader->AddSpectator( "dRmin_RC := dRmin_RCjets", "", "", 'F' );
  dataloader->AddVariable ( "dRmin_RCTTM := dRmin_RCTTMassRCTTMass", "", "", 'F' ); //TRAIN
  dataloader->AddSpectator( "dEtaaverage_RC := dEtaaverage_RCjets", "", "", 'D' );
  dataloader->AddSpectator( "dEtaaverage_RCTTM := dEtaaverage_RCTTMassRCTTMass", "", "", 'D' );
  dataloader->AddVariable ( "dPhiaverage_RC := dPhiaverage_RCjets", "", "", 'D' ); //TRAIN
  dataloader->AddSpectator( "dPhiaverage_RCTTM := dPhiaverage_RCTTMassRCTTMass", "", "", 'D' );
  dataloader->AddVariable ( "dRaverage_RC := dRaverage_RCjets", "", "", 'D' ); //TRAIN
  dataloader->AddSpectator( "dRaverage_RCTTM := dRaverage_RCTTMassRCTTMass", "", "", 'D' );

  //RC jet properties
  dataloader->AddSpectator( "RCjet0_pt := Alt$( RCjets_pt[0],0)", "", "", 'D' );
  dataloader->AddVariable ( "RCjet1_pt := Alt$( RCjets_pt[1],0)", "", "", 'D' ); //TRAIN
  dataloader->AddVariable ( "RCjet2_pt := Alt$( RCjets_pt[2],0)", "", "", 'D' ); //TRAIN

  dataloader->AddVariable ( "RCMTop_jet0_pt := Alt$( RCMTop_jets_pt[0],0)", "", "", 'D' ); //TRAIN
  dataloader->AddSpectator( "RCMTop_jet1_pt := Alt$( RCMTop_jets_pt[1],0)", "", "", 'D' );
  dataloader->AddSpectator( "RCMTop_jet2_pt := Alt$( RCMTop_jets_pt[2],0)", "", "", 'D' );

  dataloader->AddVariable ( "RCMHiggs_jet0_pt := Alt$( RCMHiggs_jets_pt[0],0)", "", "", 'D' ); //TRAIN
  dataloader->AddVariable ( "RCMHiggs_jet1_pt := Alt$( RCMHiggs_jets_pt[1],0)", "", "", 'D' ); //TRAIN
  dataloader->AddSpectator( "RCMHiggs_jet2_pt := Alt$( RCMHiggs_jets_pt[2],0)", "", "", 'D' );

  dataloader->AddSpectator( "RCMV_jet0_pt := Alt$( RCMV_jets_pt[0],0)", "", "", 'D' );
  dataloader->AddSpectator( "RCMV_jet1_pt := Alt$( RCMV_jets_pt[1],0)", "", "", 'D' );
  dataloader->AddSpectator( "RCMV_jet2_pt := Alt$( RCMV_jets_pt[2],0)", "", "", 'D' );

  dataloader->AddSpectator( "RCMTop_jet0_eta := Alt$( RCMTop_jets_eta[0],-10)", "", "", 'D' );
  dataloader->AddSpectator( "RCMTop_jet1_eta := Alt$( RCMTop_jets_eta[1],-10)", "", "", 'D' );
  dataloader->AddSpectator( "RCMTop_jet2_eta := Alt$( RCMTop_jets_eta[2],-10)", "", "", 'D' );

  dataloader->AddSpectator( "RCMHiggs_jet0_eta := Alt$( RCMHiggs_jets_eta[0],-10)", "", "", 'D' );
  dataloader->AddSpectator( "RCMHiggs_jet1_eta := Alt$( RCMHiggs_jets_eta[1],-10)", "", "", 'D' );
  dataloader->AddSpectator( "RCMHiggs_jet2_eta := Alt$( RCMHiggs_jets_eta[2],-10)", "", "", 'D' );

  dataloader->AddVariable ( "RCMV_jet0_eta := Alt$( RCMV_jets_eta[0],-10)", "", "", 'D' ); //TRAIN
  dataloader->AddSpectator( "RCMV_jet1_eta := Alt$( RCMV_jets_eta[1],-10)", "", "", 'D' );
  dataloader->AddSpectator( "RCMV_jet2_eta := Alt$( RCMV_jets_eta[2],-10)", "", "", 'D' );

  dataloader->AddSpectator( "RCMTop_jet0_nconsts := Alt$( RCMTop_jets_consts_n[0],0)", "", "", 'I' );
  dataloader->AddSpectator( "RCMTop_jet1_nconsts := Alt$( RCMTop_jets_consts_n[1],0)", "", "", 'I' );
  dataloader->AddSpectator( "RCMTop_jet2_nconsts := Alt$( RCMTop_jets_consts_n[2],0)", "", "", 'I' );

  dataloader->AddVariable ( "RCMHiggs_jet0_nconsts := Alt$( RCMHiggs_jets_consts_n[0],0)", "", "", 'I' ); //TRAIN
  dataloader->AddSpectator( "RCMHiggs_jet1_nconsts := Alt$( RCMHiggs_jets_consts_n[1],0)", "", "", 'I' );
  dataloader->AddSpectator( "RCMHiggs_jet2_nconsts := Alt$( RCMHiggs_jets_consts_n[2],0)", "", "", 'I' );

  dataloader->AddSpectator( "RCMV_jet0_nconsts := Alt$( RCMV_jets_consts_n[0],0)", "", "", 'I' );
  dataloader->AddSpectator( "RCMV_jet1_nconsts := Alt$( RCMV_jets_consts_n[1],0)", "", "", 'I' );
  dataloader->AddSpectator( "RCMV_jet2_nconsts := Alt$( RCMV_jets_consts_n[2],0)", "", "", 'I' );

  dataloader->AddSpectator( "RCMTop_jet0_nbconsts := Alt$( RCMTop_jets_bconsts_n[0],0)", "", "", 'I' );
  dataloader->AddSpectator( "RCMTop_jet1_nbconsts := Alt$( RCMTop_jets_bconsts_n[1],0)", "", "", 'I' );
  dataloader->AddSpectator( "RCMTop_jet2_nbconsts := Alt$( RCMTop_jets_bconsts_n[2],0)", "", "", 'I' );

  dataloader->AddVariable ( "RCMHiggs_jet0_nbconsts := Alt$( RCMHiggs_jets_bconsts_n[0],0)", "", "", 'I' ); //TRAIN
  dataloader->AddSpectator( "RCMHiggs_jet1_nbconsts := Alt$( RCMHiggs_jets_bconsts_n[1],0)", "", "", 'I' );
  dataloader->AddSpectator( "RCMHiggs_jet2_nbconsts := Alt$( RCMHiggs_jets_bconsts_n[2],0)", "", "", 'I' );

  dataloader->AddVariable ( "RCMV_jet0_nbconsts := Alt$( RCMV_jets_bconsts_n[0],0)", "", "", 'I' ); //TRAIN
  dataloader->AddSpectator( "RCMV_jet1_nbconsts := Alt$( RCMV_jets_bconsts_n[1],0)", "", "", 'I' );
  dataloader->AddSpectator( "RCMV_jet2_nbconsts := Alt$( RCMV_jets_bconsts_n[2],0)", "", "", 'I' );

  //lepton, met leptop properties
  dataloader->AddSpectator( "lep_pt := el1_pt+mu1_pt", "", "", 'D' );
  dataloader->AddVariable ( "leptop_pt", "", "", 'D' ); //TRAIN
  dataloader->AddSpectator( "ptw", "", "", 'D' );
  dataloader->AddVariable ( "mtw", "", "", 'D' ); //TRAIN
  dataloader->AddVariable ( "residualMET_Pt", "", "", 'D' ); //TRAIN
  dataloader->AddVariable ( "met", "", "", 'D' ); //TRAIN
  dataloader->AddVariable ( "mT_bmin", "", "", 'D' ); //TRAIN

  //multiplicities 
  dataloader->AddSpectator( "jets_n", "", "", 'I' );
  dataloader->AddVariable ( "trkbjets_n", "", "", 'I' ); //TRAIN
  dataloader->AddSpectator( "RCTTMass_jets_n", "", "", 'I' );
  dataloader->AddSpectator( "RCMTop_jets_n", "", "", 'I' );
  dataloader->AddSpectator( "RCMHiggs_jets_n", "", "", 'I' );
  dataloader->AddSpectator( "RCMV_jets_n", "", "", 'I' );


  //reco vlq
  dataloader->AddVariable ( "mvlq_rcttm1 := Alt$(m_vlq_rcttmass_drmax[0],0)", "", "", 'D' );  //TRAIN
  dataloader->AddVariable ( "mvlq_rcttm2 := Alt$(m_vlq_rcttmass_drmax[1],0)", "", "", 'D'); //TRAIN 
  dataloader->AddSpectator( "mvlq_rc1 := Alt$(m_vlq_rcjets_drmax[0],0)", "", "", 'D' );
  dataloader->AddVariable ( "mvlq_rc2 := Alt$(m_vlq_rcjets_drmax[1],0)", "", "", 'D' ); //TRAIN 



  //**************************************************//


  // You can add so-called "Spectator variables", which are not used in the MVA training,
  // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
  // input variables, the response values of all trained MVAs, and the spectator variables
 
  //dataloader->AddSpectator( "spec_pt := o ",  "Spectator Pt", "GeV", 'D' );
  //dataloader->AddSpectator( "spec2 := var1*3",  "Spectator 2", "units", 'F' );


  //Add the  Decay Mode and Mass point
 
  //global event weights per tree (see below for setting event-wise weights)
  //Double_t signalWeight     = 1.0  ;
  //Double_t backgroundWeight = 1.0;

   dataloader->SetWeightExpression ("(nomWeight_weight_elec*nomWeight_weight_elec_trigger*nomWeight_weight_jvt*nomWeight_weight_muon*nomWeight_weight_muon_trigger*nomWeight_weight_norm*nomWeight_weight_pu*nomWeight_weight_trkbtag*nomWeight_weight_mc)/nomWeight_weight_xsec");
   //dataloader->SetBackgroundWeightExpression( "(nomWeight_weight_elec*nomWeight_weight_elec_trigger*nomWeight_weight_jvt*nomWeight_weight_muon*nomWeight_weight_muon_trigger*nomWeight_weight_norm*nomWeight_weight_pu*nomWeight_weight_trkbtag*nomWeight_weight_mc)/nomWeight_weight_xsec" );
  
 
  // Apply additional cuts on the signal and background samples (can be different)
  //TCut mycuts = "  meff > 1000 && RCjets_n >=2 && ( leptop_n + RCjets_n) >=4 && jets_n >=6 && trkbjets_n >=3 && RCTTMass_jets_n >=2 ";

  //std::cout << mycuts << std::endl;
  
  //std::string gcuts = " ( VLQtype==1 || VLQtype==2 ||  VLQtype==3) ";


  /*
  auto *signalTreeSPTest = sigTree->CopyTree(mycuts && gcuts.c_str()&& "((event_number%2) == 1)" );
  auto *signalTreeGTr = sigTree->CopyTree(mycuts && gcuts.c_str() && "((event_number%2) == 0)");
  std::cout << "Copied signal test and train trees" << std::endl;


  std::cout << "ttbarlight entries : " << ttbarlightTree->GetEntries() << std::endl;
  auto *ttbarlightTest = ttbarlightTree->CopyTree(mycuts && "((event_number%3) == 0) && ((event_number%2) == 1)");
  auto *ttbarlightTrain = ttbarlightTree->CopyTree(mycuts && "((event_number%3) == 0) &&  ((event_number%2) == 0)");
  std::cout << "ttbarlight test entries : " << ttbarlightTest->GetEntries() << std::endl;
  std::cout << "ttbarlight train entries : " << ttbarlightTrain->GetEntries() << std::endl;
  
  std::cout << "ttbarcc entries : " << ttbarccTree->GetEntries() << std::endl;
  auto *ttbarccTest = ttbarccTree->CopyTree(mycuts && "((event_number%3) == 0) && ((event_number%2) == 1)");
  auto *ttbarccTrain = ttbarccTree->CopyTree(mycuts && "((event_number%3) == 0) &&  ((event_number%2) == 0)");
  std::cout << "ttbarcc test entries : " << ttbarccTest->GetEntries() << std::endl;
  std::cout << "ttbarcc train entries : " << ttbarccTrain->GetEntries() << std::endl;
  
  std::cout << "ttbarbb entries : " << ttbarbbTree->GetEntries() << std::endl;
  auto *ttbarbbTest = ttbarbbTree->CopyTree(mycuts && "((event_number%3) == 0) && ((event_number%2) == 1)");
  auto *ttbarbbTrain = ttbarbbTree->CopyTree(mycuts && "((event_number%3) == 0) &&  ((event_number%2) == 0)");
  std::cout << "ttbarbb test entries : " << ttbarbbTest->GetEntries() << std::endl;
  std::cout << "ttbarbb train entries : " << ttbarbbTrain->GetEntries() << std::endl;
  
  std::cout << "Copied background test and train trees" << std::endl;
  */
  //dataloader->AddSignalTree( signalTreeGTst ,1 ,"Test");   // signalWeight );
  /*
  dataloader->AddSignalTree( signalTreeGTr, 1,"Train");
  dataloader->AddSignalTree( signalTreeSPTest, 1,"Test");

  dataloader->AddBackgroundTree( ttbarlightTrain, 1,"Train");  //, backgroundWeight );
  dataloader->AddBackgroundTree( ttbarccTrain, 1,"Train");  //, backgroundWeight );
  dataloader->AddBackgroundTree( ttbarbbTrain, 1,"Train");  //, backgroundWeight );
  
  dataloader->AddBackgroundTree( ttbarlightTest, 1,"Test");  //, backgroundWeight );
  dataloader->AddBackgroundTree( ttbarccTest, 1,"Test");  //, backgroundWeight );
  dataloader->AddBackgroundTree( ttbarbbTest, 1,"Test");  //, backgroundWeight );
  */
  dataloader->AddSignalTree( sigTree ); 
  dataloader->AddBackgroundTree( ttbarlightTree ); 
  dataloader->AddBackgroundTree( ttbarccTree ); 
  dataloader->AddBackgroundTree( ttbarbbTree ); 


  TCut mycuts = " (meff > 1000 && RCjets_n >=2 && ( leptop_n + RCjets_n) >=4 && jets_n >=6 && trkbjets_n >=3 && RCTTMass_jets_n >=2) && ( VLQtype==1 || VLQtype==2 ||  VLQtype==3) ";

  TCut mycutb = " (meff > 1000 && RCjets_n >=2 && ( leptop_n + RCjets_n) >=4 && jets_n >=6 && trkbjets_n >=3 && RCTTMass_jets_n >=2) ";


  //std::cout << "General test : " + gcutstest << std::endl;
  
  // Tell the dataloader how to use the training and testing events
  //
  // If no numbers of events are given, half of the events in the tree are used
  // for training, and the other half for testing:
  //
  //    dataloader->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
  //
  // To also specify the number of testing events, use:
  //
  //    dataloader->PrepareTrainingAndTestTree( mycut,
  //         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );
  dataloader->PrepareTrainingAndTestTree( mycuts, mycutb,
  					    "nTest_Signal=0:nTest_Background=0");
  //dataloader->PrepareTrainingAndTestTree( mycuts, mycutb,
  //					    "SplitMode=Random:NormMode=NumEvents:!V:nTest_Background=30000:nTrain_Background=30000");
  
  // What does NornmMode = NumEvents do ? 

    // ### Book MVA methods
    //
    // Please lookup the various method configuration options in the corresponding cxx files, eg:
    // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
    // it is possible to preset ranges in the option string in which the cut optimisation should be done:
    // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable
 
 
    // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
    if (Use["MLP"]){
      TString mlp_opt = "!V:NeuronType=sigmoid:VarTransform=N:NCycles=700:EpochMonitoring=false:HiddenLayers=30,10,10,5:LearningRate=0.01:DecayRate=0.0:TestRate=5";
      //std::string mlp_opt = "!V:NeuronType=sigmoid:VarTransform=N:NCycles=800:EpochMonitoring=false:HiddenLayers=22,22,22,22,22,22:TestRate=5:ConvergenceTests=100:ConvergenceImprove=1e-30:!UseRegulator";
      //factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", mlp_opt.c_str() );
      cross_validation->BookMethod( TMVA::Types::kMLP, "MLP", mlp_opt );

      std ::cout << mlp_opt << std::endl;
    } 
    
    /*    std::cout << NeuronType << std::endl;
    std::cout << VarTransform << std::endl;
    std::cout << NCycles << std::endl;    
    std::cout << EpochMonitoring << std::endl;
    std::cout << HiddenLayers << std::endl;
    std::cout << TestRate << std::endl;
What does VarTransform do ? options ? G,D,G,A  .....................And UseRegulator = false ??? 
*/
 
    
    //if (Use["MLP"])
    //factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh   ReLU, sigmoid,ELUs (not present in root aparently) , :VarTransform=N:NCycles=750:HiddenLayers=N+5:TestRate=5:!UseRegulator" );
    // N is the number of input variables, try reducing the number of hidden layers. Testt rate : test of overtraining in epoch monitoring, maybe should be at 20; 
    // BPMode=sequential (default) or batch (back propagating learning mode); EpochMonitoring=true monitors epochs 
    // Multi-architecture DNN implementation.
    if (Use["DNN_CPU"]) {
      // General layout.
      TString layoutString ("Layout=TANH|128,TANH|128,TANH|128,LINEAR");
 
      // Training strategies.
      TString training0("LearningRate=1e-1,Momentum=0.9,Repetitions=1,"
                         "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                         "WeightDecay=1e-4,Regularization=L2,"
			"DropConfig=0.0+0.5+0.5+0.5, Multithreading=True");
      TString training1("LearningRate=1e-2,Momentum=0.9,Repetitions=1,"
                         "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                         "WeightDecay=1e-4,Regularization=L2,"
			"DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
      TString training2("LearningRate=1e-3,Momentum=0.0,Repetitions=1,"
                         "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                         "WeightDecay=1e-4,Regularization=L2,"
			"DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
      TString trainingStrategyString ("TrainingStrategy=");
      trainingStrategyString += training0 + "|" + training1 + "|" + training2;
 
      // General Options.
      TString dnnOptions ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
			  "WeightInitialization=XAVIERUNIFORM");
      dnnOptions.Append (":"); dnnOptions.Append (layoutString);
      dnnOptions.Append (":"); dnnOptions.Append (trainingStrategyString);
 
      // Cuda implementation.
      if (Use["DNN_GPU"]) {
	TString gpuOptions = dnnOptions + ":Architecture=GPU";
	//factory->BookMethod(dataloader, TMVA::Types::kDNN, "DNN_GPU", gpuOptions);
	cross_validation->BookMethod(TMVA::Types::kDNN, "DNN_GPU", gpuOptions);
      }
      // Multi-core CPU implementation.
      if (Use["DNN_CPU"]) {
	TString cpuOptions = dnnOptions + ":Architecture=CPU";
	//factory->BookMethod(dataloader, TMVA::Types::kDNN, "DNN_CPU", cpuOptions);
	cross_validation->BookMethod(TMVA::Types::kDNN, "DNN_CPU", cpuOptions);
      }
    }
 
    // For an example of the category classifier usage, see: TMVAClassificationCategory
    //
    // --------------------------------------------------------------------------------------------------
    //  Now you can optimize the setting (configuration) of the MVAs using the set of training events
    // STILL EXPERIMENTAL and only implemented for BDT's !
    //
    //     factory->OptimizeAllMethods("SigEffAt001","Scan");
    //     factory->OptimizeAllMethods("ROCIntegral","FitGA");
    //
    // --------------------------------------------------------------------------------------------------
 
    // Now you can tell the factory to train, test, and evaluate the MVAs
    //
    // Train MVAs using the set of training events
    //factory->TrainAllMethods();
 
    // Evaluate all MVAs using the set of test events
    //factory->TestAllMethods();
 
    // Evaluate and compare performance of all configured MVAs
    //factory->EvaluateAllMethods();
 
    cross_validation->Evaluate();
    // --------------------------------------------------------------
 
    // Save the output
    outputFile->Close();
 
    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done! Good Job Fernando!" << std::endl;
 
    std::cout << "Configuration applied: " << std::endl;
 
    //delete factory;
    delete cross_validation;
    delete dataloader;
    // Launch the GUI for the root macros
    if (!gROOT->IsBatch()){

      // Draw cv-specific graphs
      //cross_validation->GetResults()[0].DrawAvgROCCurve(kTRUE, "Avg ROC for MLP");
 
      TMVA::TMVAGui( outfileName );
    }
    return 0;
}
 
