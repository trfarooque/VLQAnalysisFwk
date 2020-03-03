#include "TFile.h"
#include "TH1D.h"
#include "TH1F.h"

#include "IFAETopFramework/OutputHistManager.h"
#include "IFAETopFramework/AnalysisUtils.h"
#include "IFAETopFramework/Selection.h"
#include "VLQAnalysis/VLQ_Options.h"
#include "VLQAnalysis/VLQ_Selector.h"
#include "VLQAnalysis/VLQ_PropagateSingleTopSystematics.h"

VLQ_PropagateSingleTopSystematics::VLQ_PropagateSingleTopSystematics ( const VLQ_Options* opt, OutputHistManager* outMngrHist) : 
  m_opt(opt),
  m_outMngrHist(outMngrHist)
{
  
  m_varList = {};
  m_regList = {};
  m_sourceHists = {};
  
}

VLQ_PropagateSingleTopSystematics::~VLQ_PropagateSingleTopSystematics( ){
  
  m_varList.clear();
  m_regList.clear();
  for(std::pair<std::string, TH1F*> histPair : m_sourceHists){
    delete histPair.second;
  }
  m_sourceHists.clear();
  
} 

bool VLQ_PropagateSingleTopSystematics::Init( const std::map<int, Selection*>* selection_tree, const std::string& fileName ){
  
  //Set up variable list
  for( std::pair< std::string, OutputHistManager::h1Def* > hpair : *(m_outMngrHist->StdTH1Def()) ){
    
    if( !hpair.second->hasSyst ){ continue; }
    m_varList.push_back(hpair.first);
    
  }
  
  //Set up region list
  for( const std::pair<int, Selection*> &sel : *(selection_tree) ){
    if( (sel.second)->PassFlagAtBit(VLQ_Selector::DOSYST) ){
      m_regList.push_back(AnalysisUtils::ReplaceString((sel.second)->Name(), "-", ""));
    }
  }

  std::vector<std::string> sourceReg{};
  if( m_opt -> DoOneLeptonAna() ){
    sourceReg.push_back("c1lep5jin2bin");
    sourceReg.push_back("c1lep5jin3bin");
  }
  if( m_opt -> DoZeroLeptonAna() ){
    sourceReg.push_back("c0lep6jin2bin");
    sourceReg.push_back("c0lep6jin2binLowMtbmin");
    sourceReg.push_back("c0lep6jin2binHighMtbmin");
    sourceReg.push_back("c0lep6jin3bin");
    sourceReg.push_back("c0lep6jin3binLowMtbmin");
    sourceReg.push_back("c0lep6jin3binHighMtbmin");
  }


  //Open input file and read all required histograms
  TFile* sourceFile = TFile::Open(fileName.c_str(), "READ");
  for(const std::string& variable : m_varList){

    for(const std::string& region : sourceReg){
      std::string histName = region + "_" + variable;
      TH1F* temp_hist = (TH1F*) sourceFile -> Get(histName.c_str());
      if(!temp_hist){
	std::cout << "<!> Error in VLQ_PropagateSingleTopSystematics::Init(): Looks like the histo " << histName;
	std::cout << " is not in file " << fileName << ". Please check." << std::endl;
	continue;
      }
      temp_hist -> SetDirectory(0);
      m_sourceHists.insert( std::pair < std::string, TH1F* >(histName,temp_hist) );
    }

  }

  sourceFile->Close();
  delete sourceFile;

  return true;



}

bool VLQ_PropagateSingleTopSystematics::WriteAllSystHistograms(const bool writeToFile, const bool newFile){

  TFile* outfile = NULL;
  if(writeToFile){
    std::string fileMode = newFile ? "RECREATE" : "UPDATE";
    outfile = TFile::Open( m_opt->OutputFile().c_str(), fileMode.c_str());
  }
  bool stat = true;
  for(const std::string& variable : m_varList){
    for(const std::string& region : m_regList){
      stat = MakeSystHistogram( region, variable, outfile) && stat;
    }
  }
  if(outfile){ outfile -> Close(); }

  return stat;
}

bool VLQ_PropagateSingleTopSystematics::MakeSystHistogram(const std::string& region, const std::string& variable, TFile* outFile){

  std::string lep_pref = "";
  std::string b_pref = "";
  std::string mtb_pref = "";

  if(region.find("c1lep") != std::string::npos){ lep_pref = "c1lep5jin"; }
  else if(region.find("c0lep") != std::string::npos){ lep_pref = "c0lep6jin"; }

  if(region.find("2b") != std::string::npos){b_pref = "2bin"; }
  else if( (region.find("3b") != std::string::npos) || (region.find("4b") != std::string::npos) ){b_pref = "3bin"; }

  if(region.find("LowMtbmin") != std::string::npos){ mtb_pref = "LowMtbmin"; }
  else if(region.find("HighMtbmin") != std::string::npos){ mtb_pref = "HighMtbmin"; }

  std::string sourceName =  lep_pref+b_pref+mtb_pref + "_" + variable;
  std::string targetName = region + "_" + variable + "_SingletopDRDS";

  std::map<std::string, TH1F*>::iterator sourceFinder = m_sourceHists.find(sourceName);
  if(sourceFinder == m_sourceHists.end()){
    std::cerr << "VLQ_PropagateSingleTopSystematics::MakeSystHistogram --> ERROR : Cannot find histogram "<<sourceName<<" for DRDS systematic "<<std::endl;
    return false;
  }
  TH1F* sourceHist = sourceFinder->second;
  TH1D* targetHist = m_outMngrHist->HistMngr()->CloneTH1D( targetName, region + "_" + variable);

  m_outMngrHist->HistMngr()->FinaliseTH1Bins( targetName );

  for(int ibin = 1; ibin <= targetHist->GetNbinsX(); ibin++){
    double bx = targetHist -> GetBinCenter(ibin);
    double bc = targetHist -> GetBinContent(ibin);
    //double be = targetHist -> GetBinError(ibin);

    double sysFactor = sourceHist -> GetBinContent( sourceHist -> FindBin(bx) );
    targetHist -> SetBinContent ( ibin, bc * sysFactor );
  
  }

  if(outFile){
    outFile -> cd();
    targetHist->Write();
    m_outMngrHist->HistMngr()->ClearTH1(targetName);
  }
  sourceName.clear();
  targetName.clear();

  return true;

}

