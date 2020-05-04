#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include <vector>

#include "TFile.h"
#include "TH1.h"
#include "TKey.h"


bool m_doOneLep;
bool m_doZeroLep;
bool m_doSplitEMu;
bool m_doFitReg;
bool m_doValidReg;
bool m_doWeightSys;
bool m_debug;
 
TFile* m_inFile;
TFile* m_outFile;
std::string m_ref_reg;
std::string m_ref_var;
std::vector<std::string> m_regList;
std::map<std::string, std::vector<std::string> > m_targetPrintList;
std::vector<std::string> m_wgtList;
std::vector<std::string> m_varList;  


bool MakeRegionList();
bool MakeWeightList( );
bool SumRegions();
void AddHistogramToTarget(TH1D* hist, TH1D** target, const std::string& name, double scale = 1.);

int main(int argc, char** argv){

  m_doOneLep    = true;
  m_doZeroLep   = true;
  m_doSplitEMu  = false;
  m_doFitReg    = true;
  m_doValidReg  = true;
  m_doWeightSys = true;
  m_debug       = false;

  m_inFile = NULL;
  m_outFile = NULL; 
  m_ref_reg = "";
  m_ref_var = "meff";

  m_regList = {};
  m_targetPrintList = {};
  m_wgtList = {""};

  m_varList = {"meff"}; //, "jets_n", "bjets_n", "RCMTop_jets_n", "RCMHiggs_jets_n"};

  std::string inputFileName = "";
  std::string outputFileName = "";

  int optind = 1;
  std::stringstream ss;
  std::string _switch = "";

  while ((optind < argc) && (argv[optind][0]=='-')) {
    _switch = std::string(argv[optind]);
    optind++;  ss << argv[optind];

    if       (_switch == "--doOneLep")             { ss >> m_doOneLep; }       //Do 1-lepton regions
    else if  (_switch == "--doZeroLep")            { ss >> m_doZeroLep; }      //Do 0-lepton regions
    else if  (_switch == "--doSplitEMu")           { ss >> m_doSplitEMu; }     //Do elec and muon regions
    else if  (_switch == "--doFitRegions")         { ss >> m_doFitReg; }       //Contains fit regions
    else if  (_switch == "--doValidationRegions")  { ss >> m_doValidReg; }     //Contains fit regions
    else if  (_switch == "--doWeightSys")          { ss >> m_doWeightSys; }    //Do weight systematics
    else if  (_switch == "--debug")                { ss >> m_debug; }          //Print debug messages

    else if  (_switch == "--inputFile")            { ss >> inputFileName; }    //Name of input file
    else if  (_switch == "--outputFile")           { ss >> outputFileName; }   //Name of output file

    else if  (_switch == "--ref_region")           { ss >> m_ref_reg; }        //Reference region used to get list of weight systematics
    else if  (_switch == "--ref_variable")         { ss >> m_ref_var; }        //Reference variable used to get list of weight systematics

    else std::cout<<"Unknown switch "<<_switch<<std::endl;

    optind++; ss.clear(); _switch.clear();
  }

  std::cout << " m_inputFileName   = " << inputFileName << std::endl;
  std::cout << " m_outputFileName  = " << outputFileName << std::endl;
  std::cout << " m_doOneLep        = " << m_doOneLep << std::endl;
  std::cout << " m_doZeroLep       = " << m_doZeroLep << std::endl;
  std::cout << " m_doSplitEMu      = " << m_doSplitEMu << std::endl;
  std::cout << " m_doFitReg        = " << m_doFitReg << std::endl;
  std::cout << " m_doValidReg      = " << m_doValidReg << std::endl;
  std::cout << " m_doWeightSys     = " << m_doWeightSys << std::endl;

  std::cout << " m_ref_reg         = " << m_ref_reg << std::endl;
  std::cout << " m_ref_var         = " << m_ref_var << std::endl;

  bool fail = false;
  if(inputFileName.empty()){ std::cerr << "Input file name must be provided (--inputFile)" << std::endl; fail=true; }
  if(outputFileName.empty()){ std::cerr << "Output file name must be provided (--outputFile)" << std::endl; fail=true; }

  if(fail) { return -1; }

  m_inFile = TFile::Open( inputFileName.c_str(), "READ" );
  m_outFile = TFile::Open( outputFileName.c_str(), "RECREATE" );

  //Make list of regions
  MakeRegionList();

  //Make list of weight systematics
  MakeWeightList( );

  //Sum the region
  SumRegions();

  return 0;

}


bool MakeRegionList(){

  if( m_doOneLep ){

    if( m_doFitReg ){
      std::vector<std::string> m_regList_1L_fit = {
	"c1lep0Tex0Hex6jin2bex",
	"c1lep0Tex0Hex6jin3bex",
	"c1lep0Tex0Hex6jin4bin",
	"c1lep0Tex1Hex6jin2bex",
	"c1lep0Tex1Hex6jin3bex",
	"c1lep0Tex1Hex6jin4bin",
	"c1lep1Tex0Hex6jin2bex",
	"c1lep1Tex0Hex6jin3bex",
	"c1lep1Tex0Hex6jin4bin",
	"c1lep1Tex1Hex6jin2bex",
	"c1lep1Tex1Hex6jin3bex",
	"c1lep1Tex1Hex6jin4bin",
	"c1lep2Tin0_1Hwin6jin2bex",
	"c1lep2Tin0_1Hwin6jin3bex",
	"c1lep2Tin0_1Hwin6jin4bin",
	"c1lep0Tin2Hin6jin2bex",
	"c1lep0Tin2Hin6jin3bex",
	"c1lep0Tin2Hin6jin4bin"
      };
      m_regList.insert(m_regList.end(), m_regList_1L_fit.begin(), m_regList_1L_fit.end());
    }//fit
    
    if( m_doValidReg ){
      std::vector<std::string> m_regList_1L_valid = {
	"c1lep0Tex0Hex5jex2bex",
	"c1lep0Tex0Hex5jex3bex",
	"c1lep0Tex0Hex5jex4bin",
	"c1lep0Tex1Hex5jex2bex",
	"c1lep0Tex1Hex5jex3bex",
	"c1lep0Tex1Hex5jex4bin",
	"c1lep1Tex0Hex5jex2bex",
	"c1lep1Tex0Hex5jex3bex",
	"c1lep1Tex0Hex5jex4bin",
	"c1lep1Tex1Hex5jex2bex",
	"c1lep1Tex1Hex5jex3bex",
	"c1lep2Tin0_1Hwin5jex2bex",
	"c1lep2Tin0_1Hwin5jex3bex",
	"c1lep0Tin2Hin5jex2bex",
	"c1lep0Tin2Hin5jex3bex",
	"c1lep2THin5jex4bin"
      };
      m_regList.insert(m_regList.end(), m_regList_1L_valid.begin(), m_regList_1L_valid.end());
    }//valid
    

    std::map<std::string, std::vector<std::string> > m_targetPrintList_1L = {
      {"sum1lep5jin2bin", {}}, 
      {"sum1lep5jin3bin", {}}, 
      {"sum1lep6jin2bin", {}},
      {"sum1lep6jin3bin", {}},
      {"sum1lep1THin6jin3bin", {}}
    };
    
    m_targetPrintList.insert(m_targetPrintList_1L.begin(), m_targetPrintList_1L.end());
    
    
  }//1-lepton

  if( m_doZeroLep ){

    if( m_doFitReg ){
      std::vector<std::string> m_regList_0L_fit = {
	"c0lep0Tex0Hex7jin2bexLowMtbmin",
	"c0lep0Tex0Hex7jin2bexHighMtbmin",
	"c0lep0Tex1Hex7jin2bexLowMtbmin",
	"c0lep0Tex1Hex7jin2bexHighMtbmin",
	"c0lep1Tex0Hex7jin2bexLowMtbmin",
	"c0lep1Tex0Hex7jin2bexHighMtbmin",
	"c0lep2THin7jin2bexLowMtbmin",
	"c0lep2THin7jin2bexHighMtbmin",
	"c0lep0Tex0Hex7jin3bexLowMtbmin",
	"c0lep0Tex0Hex7jin3bexHighMtbmin",
	"c0lep0Tex1Hex7jin3bexLowMtbmin",
	"c0lep0Tex1Hex7jin3bexHighMtbmin",
	"c0lep1Tex0Hex7jin3bexLowMtbmin",
	"c0lep1Tex0Hex7jin3bexHighMtbmin",
	"c0lep1Tex1Hex7jin3bexLowMtbmin",
	"c0lep1Tex1Hex7jin3bexHighMtbmin",
	"c0lep2Tin0_1Hwin7jin3bexLowMtbmin",
	"c0lep2Tin0_1Hwin7jin3bexHighMtbmin",
	"c0lep0Tin2Hin7jin3bex",
	"c0lep0Tex0Hex7jin4binLowMtbmin",
	"c0lep0Tex0Hex7jin4binHighMtbmin",
	"c0lep0Tex1Hex7jin4binLowMtbmin",
	"c0lep0Tex1Hex7jin4binHighMtbmin",
	"c0lep1Tex0Hex7jin4binLowMtbmin",
	"c0lep1Tex0Hex7jin4binHighMtbmin",
	"c0lep2THin7jin4bin"
      };
      m_regList.insert(m_regList.end(), m_regList_0L_fit.begin(), m_regList_0L_fit.end());
    }//fit

    if( m_doValidReg ){
      std::vector<std::string> m_regList_0L_valid = {
	"c0lep0Tex0Hex6jex2bexLowMtbmin",
	"c0lep0Tex0Hex6jex2bexHighMtbmin",
	"c0lep0Tex1Hex6jex2bexLowMtbmin",
	"c0lep0Tex1Hex6jex2bexHighMtbmin",
	"c0lep1Tex0Hex6jex2bexLowMtbmin",
	"c0lep1Tex0Hex6jex2bexHighMtbmin",
	"c0lep2THin6jex2bexLowMtbmin",
	"c0lep2THin6jex2bexHighMtbmin",
	"c0lep0Tex0Hex6jex3bexLowMtbmin",
	"c0lep0Tex0Hex6jex3bexHighMtbmin",
	"c0lep0Tex1Hex6jex3bexLowMtbmin",
	"c0lep0Tex1Hex6jex3bexHighMtbmin",
	"c0lep1Tex0Hex6jex3bexLowMtbmin",
	"c0lep1Tex0Hex6jex3bexHighMtbmin",
	"c0lep2THin6jex3bex",
	"c0lep0Tex0Hex6jex4binLowMtbmin",
	"c0lep0Tex0Hex6jex4binHighMtbmin",
	"c0lep0Tex1Hex6jex4bin",
	"c0lep1Tex0Hex6jex4bin",
	"c0lep2THin6jex4bin"
      };
      m_regList.insert(m_regList.end(), m_regList_0L_valid.begin(), m_regList_0L_valid.end());
    }//valid

    std::map<std::string, std::vector<std::string> > m_targetPrintList_0L = {
      {"sum0lep6jin2bin", {}},	
      {"sum0lep7jin2bin", {}},	
      {"sum0lep6jin3bin", {}},	
      {"sum0lep7jin3bin", {}},	
      
      {"sum0lep1THin7jin2bin", {}}
    };
    
    m_targetPrintList.insert(m_targetPrintList_0L.begin(), m_targetPrintList_0L.end());

  }//0-lep
  
  for(const std::string& region : m_regList){

    std::string boostcat = "";
    if(region.find("0Tex0Hex") != std::string::npos){ boostcat = "0Tex0Hex"; }
    else if( (region.find("1Tex0Hex") != std::string::npos) || (region.find("0Tex1Hex") != std::string::npos) ){ boostcat = "1THex"; }
    else if( (region.find("2THin") != std::string::npos) || (region.find("2Tin0_1Hwin") != std::string::npos) 
    	     || (region.find("1Tex1Hex") != std::string::npos) || (region.find("0Tin2Hin") != std::string::npos) ){ boostcat = "2THin"; }

    std::string jetcat = "";
    if(region.find("5jex") != std::string::npos){ jetcat = "5jex"; }
    else if(region.find("6jex") != std::string::npos){ jetcat = "6jex"; }
    else if(region.find("5jin") != std::string::npos){ jetcat = "5jin"; }
    else if(region.find("6jin") != std::string::npos){ jetcat = "6jin"; }
    else if(region.find("7jin") != std::string::npos){ jetcat = "7jin"; }
    
    std::string bcat = "";
    if(region.find("2bex") != std::string::npos){ bcat = "2bex"; }
    else if(region.find("2bin") != std::string::npos){ bcat = "2bin"; }
    else if(region.find("3bex") != std::string::npos){ bcat = "3bex"; }
    else if(region.find("3bin") != std::string::npos){ bcat = "3bin"; }
    else if(region.find("4bin") != std::string::npos){ bcat = "4bin"; }

    if(region.find("0lep") != std::string::npos){
      //=========================== Preselections ==============================================
      m_targetPrintList.at("sum0lep6jin2bin").push_back(region);
      if(region.find("7jin") != std::string::npos){
	m_targetPrintList.at("sum0lep7jin2bin").push_back(region);
      }

      if( (region.find("3bex") != std::string::npos) || (region.find("4bin") != std::string::npos) ){
	m_targetPrintList.at("sum0lep6jin3bin").push_back(region);

	if(region.find("7jin") != std::string::npos){
	  m_targetPrintList.at("sum0lep7jin3bin").push_back(region);
	}//7jin3bin

      }//3bin

      //m_targetPrintList.at("sum0lep"+boostcat+"6jin2bin").push_back(region);
      if(region.find("7jin") != std::string::npos){
	if( !boostcat.empty() && (boostcat.find("0Tex0Hex") == std::string::npos) ){
	  m_targetPrintList.at("sum0lep1THin7jin2bin").push_back(region);
	}
      }
      //m_targetPrintList.at("sum0lep"+boostcat+jetcat+bcat).push_back(region);

    }//0L regions

    //===================== 1L REGIONS ========================================
    else if(region.find("1lep") != std::string::npos){
      m_targetPrintList.at("sum1lep5jin2bin").push_back(region);
      if(region.find("6jin") != std::string::npos){
	m_targetPrintList.at("sum1lep6jin2bin").push_back(region);
      }

      if( (region.find("3bex") != std::string::npos) || (region.find("4bin") != std::string::npos) ){
	m_targetPrintList.at("sum1lep5jin3bin").push_back(region);

	if(region.find("6jin") != std::string::npos){
	  m_targetPrintList.at("sum1lep6jin3bin").push_back(region);
	  if( !boostcat.empty() && (boostcat.find("0Tex0Hex") == std::string::npos) ){
	    m_targetPrintList.at("sum1lep1THin6jin3bin").push_back(region);
	  }
	  //m_targetPrintList.at("sum1lep"+boostcat+"6jin3bin").push_back(region);
	}//6jin3bin

	//m_targetPrintList.at("sum1lep"+boostcat+jetcat+bcat).push_back(region);

      }//3bin
	 
    }//1L regions

  }

  if( m_debug ){
    //============== PRINT regions lists ===================
    for(std::pair<std::string, std::vector<std::string> > targetPair : m_targetPrintList){
      std::cout << " TARGET REGION : " <<  targetPair.first << " built from :: " <<std::endl;
      for(std::string source_reg : targetPair.second){
	std::cout<<source_reg<<std::endl;
      }
      std::cout<<std::endl<<std::endl;
    }
  }

  return true;
}

bool MakeWeightList(){

  TIter next(m_inFile->GetListOfKeys());
  TKey* key = NULL;
  
  std::string className = "";
  std::string histName = "";

  std::string weightName = "";

  while( (key = (TKey*)next()) ){

    className = (std::string)(key->GetClassName());
    if(className.find("TH1D") == std::string::npos){ className.clear(); continue; }

    histName = (std::string)(key->GetName());
    std::string pref = m_ref_reg + "_" + m_ref_var + "_";
    if(histName.find(pref) != 0){continue;}
    weightName = histName.substr(pref.size());
    m_wgtList.push_back("_"+weightName);

    className.clear();
    histName.clear();
    weightName.clear();

  }//Loop over keys

  return 0;

}

void AddHistogramToTarget(TH1D* hist, TH1D** target, const std::string& name, double scale){

  if(*target != NULL){ (*target)->Add(hist, scale); }
  else{ 
    *target = (TH1D*)(hist->Clone(name.c_str()));
    (*target)->SetDirectory(0);
    (*target)->Scale(scale);
  } 
      
  return;

}


bool SumRegions(){
  

  std::vector<std::string> lepchannels{""}; 
  if(m_doSplitEMu){
    lepchannels.push_back("_el"); 
    lepchannels.push_back("_mu");
  }
  
  std::map<std::string, std::string> boostcat_systs = { 
    {"0T0H","0Tex0Hex"},{"1T0H","1Tex0Hex"},{"0T1H","0Tex1Hex"},
    {"1T1H","1Tex1Hex"},{"2T0_1H","2Tin0_1Hwin"},{"0T2H","0Tin2Hin"},{"2TH","2THin"}
  };
			
  
  for(  const std::string& variable : m_varList ){

    for(std::pair<std::string, std::vector<std::string> > targetPair : m_targetPrintList){

      for(const std::string& channel : lepchannels){
	
	if(!channel.empty() && targetPair.first.find("1lep") == std::string::npos){ continue; }	  
	
	//Regular weight systematics
	for(std::string sys : m_wgtList){

	  //std::cout<<"============================ TARGET : "<<targetPair.first<<" ========================="<<std::endl;	  
	  TH1D* targethist = NULL;
	  double sumint = 0.;
	  for(std::string source_reg : targetPair.second){
	    TH1D* sourcehist = (TH1D*)( m_inFile->Get( (source_reg + channel + "_" + variable + sys).c_str() ) );
	    //std::cout<<" reg : "<<source_reg<<" entries = "<<sourcehist->GetEntries()<<std::endl;
	    AddHistogramToTarget(sourcehist, &targethist, targetPair.first + channel + "_" + variable + sys);
	    sumint += sourcehist->GetEntries();
	  }//source regions
	  //std::cout<< " sumint = "<<sumint<< " targetint = "<<targethist->GetEntries()<<std::endl;
	  m_outFile->cd();
	  if(targethist){
	    targethist->Write();
	    delete targethist;
	  }
	  //std::cout<<"======================================================================================"<<std::endl;
	}//systematics + nominal

	//========

	//Region dependent weight systematics, including proxies for normalisation uncertainties 
	for(std::pair<std::string, std::string> boostcat : boostcat_systs){

	  //weight uncertainty
	  TH1D* targethist_QCD_el_up = NULL;
	  TH1D* targethist_QCD_mu_up = NULL;

	  for(std::string source_reg : targetPair.second){

	    if(source_reg.find(boostcat.second) != std::string::npos){
	      //If source region is in the  boost category of the systematic, add the systematically varied histograms
	      TH1D* sourcehist_el_up = (TH1D*)(m_inFile->Get( (source_reg + channel + "_" + variable + "_weight_qcd_el_up").c_str() ) );
	      TH1D* sourcehist_mu_up = (TH1D*)(m_inFile->Get( (source_reg + channel + "_" + variable + "_weight_qcd_mu_up").c_str() ) );

	      AddHistogramToTarget(sourcehist_el_up, &targethist_QCD_el_up, targetPair.first + channel + "_" + variable + "_weight_qcd_el_"+boostcat.first+"_up");
	      AddHistogramToTarget(sourcehist_mu_up, &targethist_QCD_mu_up, targetPair.first + channel + "_" + variable + "_weight_qcd_mu_"+boostcat.first+"_up");

	    }
	    else{
	      TH1D* sourcehist = (TH1D*)(m_inFile->Get( (source_reg + channel + "_" + variable).c_str() ) );
	      AddHistogramToTarget(sourcehist, &targethist_QCD_el_up, targetPair.first + channel + "_" + variable + "_weight_qcd_el_"+boostcat.first+"_up");
	      AddHistogramToTarget(sourcehist, &targethist_QCD_mu_up, targetPair.first + channel + "_" + variable + "_weight_qcd_mu_"+boostcat.first+"_up");

	    }
	  }//source regions

	  m_outFile->cd();
	  if(targethist_QCD_el_up){
	    targethist_QCD_el_up->Write();
	    delete targethist_QCD_el_up;
	  }
	  if(targethist_QCD_mu_up){
	    targethist_QCD_mu_up->Write();
	    delete targethist_QCD_mu_up;
	  }
	  
	}//boosted categories

      }//lepchannels

    }//target regions

  }//histogram list
  
  m_outFile->Close();

  return true;

}