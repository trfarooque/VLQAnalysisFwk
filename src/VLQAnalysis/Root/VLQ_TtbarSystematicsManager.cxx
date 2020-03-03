#include "VLQAnalysis/VLQ_NtupleData.h"
#include "VLQAnalysis/VLQ_OutputData.h"
#include "VLQAnalysis/VLQ_Options.h"
#include "VLQAnalysis/VLQ_Selector.h"
#include "VLQAnalysis/VLQ_Enums.h"

#include "IFAETopFramework/Selection.h"
#include "IFAETopFramework/AnalysisUtils.h"

#include "TFile.h"
#include "TH1F.h"

#include <vector>
#include <map>

#include "VLQAnalysis/VLQ_TtbarSystematicsManager.h"

//______________________________________________________________________________
//
VLQ_TtbarSystematicsManager::VLQ_TtbarSystematicsManager( VLQ_Options *opt, VLQ_OutputData *outData, const VLQ_NtupleData *ntupData ):
m_ntupData(ntupData),
m_outData(outData),
m_opt(opt),
m_histograms(0)
{
  m_histograms = new std::map < std::string, TH1F* >;
}

//______________________________________________________________________________
//
VLQ_TtbarSystematicsManager::VLQ_TtbarSystematicsManager( const VLQ_TtbarSystematicsManager &q ){
  m_ntupData      = q.m_ntupData;
  m_outData       = q.m_outData;
  m_opt           = q.m_opt;
  m_histograms    = q.m_histograms;
}

//______________________________________________________________________________
//
VLQ_TtbarSystematicsManager::~VLQ_TtbarSystematicsManager()
{
  for( std::pair < std::string, TH1F* > pair : *m_histograms ){
    delete pair.second;
  }
  delete m_histograms;
}

//______________________________________________________________________________
//
bool VLQ_TtbarSystematicsManager::Init( std::map < int, Selection* >* selection_tree, const std::string &fileName )
{
  //
  // Loading all Regions for which systematics required
  //
  std::vector < std::string > vec_regions;
  for( const std::pair<int, Selection*> &sel : *(selection_tree) ){
    if( (sel.second)->PassFlagAtBit(VLQ_Selector::DOSYST) ){
      vec_regions.push_back(AnalysisUtils::ReplaceString((sel.second)->Name(), "-", ""));
    }
  }

  //
  // Systematics
  //
  std::vector < std::string > vec_systs;
  vec_systs.push_back("GEN");
  vec_systs.push_back("PS");
  vec_systs.push_back("radLow");
  vec_systs.push_back("radHi");
  vec_systs.push_back("GENPS");

  //
  // Samples
  //
  std::vector < std::string > vec_samples;
  if(m_opt -> SampleName() == SampleName::TTBAR || m_opt -> SampleName() == SampleName::TTBARLIGHT) vec_samples.push_back("ttbarlight");
  if(m_opt -> SampleName() == SampleName::TTBAR || m_opt -> SampleName() == SampleName::TTBARBB) vec_samples.push_back("ttbarbb");
  if(m_opt -> SampleName() == SampleName::TTBAR || m_opt -> SampleName() == SampleName::TTBARCC) vec_samples.push_back("ttbarcc");

  //
  // Filling the map with needed ratio plots
  //
  TFile *f = TFile::Open(fileName.c_str());
  for ( const std::string region : vec_regions ){
    for ( const std::string sample : vec_samples ){
      for ( const std::string syst : vec_systs ){
        std::string histName = region + "_meff_" + sample + "_" + syst;
        TH1F* temp_hist = (TH1F*) f -> Get(histName.c_str());
        if(!temp_hist){
          std::cout << "<!> Error in VLQ_TtbarSystematicsManager::Init(): Looks like the histo " << histName;
          std::cout << " is not in file " << fileName << ". Please check." << std::endl;
          continue;
        }
        temp_hist -> SetDirectory(0);
        m_histograms -> insert( std::pair < std::string, TH1F* >(histName,temp_hist) );
      }
    }
  }
  f -> Close();
  delete f;
  return true;
}

//______________________________________________________________________________
//
double VLQ_TtbarSystematicsManager::GetTtbarSystWeight( const std::string &region_name, const int systematic ) const {

  if(m_opt -> SampleName() != SampleName::TTBAR && m_opt -> SampleName() != SampleName::TTBARLIGHT &&
  m_opt -> SampleName() != SampleName::TTBARBB && m_opt -> SampleName() != SampleName::TTBARCC ){
    return 1.;
  }

  if( m_outData -> o_channel_type == VLQ_Enums::MUON || m_outData -> o_channel_type == VLQ_Enums::ELECTRON ){
    if( m_outData -> o_jets_n <= 4 ){
      //the systematic uncertainties for <= 4 jets have not been derived yet
      return 1.;
    }
  } else if( m_outData -> o_channel_type == VLQ_Enums::FULLHAD ){
    if( m_outData -> o_jets_n <= 5 ){
      //systematic uncertainties for <= 5 jets have not been derived yet for the 0-lepton channel
      return 1.;
    }
  } else {
    return 1.;
  }

  //
  // Ttbar sample
  //
  std::string sample = "";
  if( m_ntupData -> d_HF_SimpleClassification > 0 ) sample = "ttbarbb";
  else if( m_ntupData -> d_HF_SimpleClassification < 0 ) sample = "ttbarcc";
  else if( m_ntupData -> d_HF_SimpleClassification == 0 ) sample = "ttbarlight";

  //
  // Systematics
  //
  std::string syst = "";
  if( systematic == PS ) syst = "PS";
  else if( systematic == GEN ) syst = "GEN";
  else if( systematic == GENPS ) syst = "GENPS";
  else if( systematic == RADLOW ) syst = "radLow";
  else if( systematic == RADHI ) syst = "radHi";

  //
  // Now, actually grabs the histogram
  //
  std::string histName = AnalysisUtils::ReplaceString( region_name, "-", "");
  histName += "_meff_" + sample + "_" + syst;
  std::map< std::string, TH1F* >::iterator it = m_histograms -> find(histName);
  if( it == m_histograms -> end() ){
    //the region is not found ... returning 1 => no systematic
    return 1;
  }
  int bin = it -> second -> FindBin( m_outData -> o_meff );
  const int max_bin = it -> second -> GetNbinsX();
  if( bin > max_bin ) bin = max_bin;

  return it -> second -> GetBinContent(bin);
}
