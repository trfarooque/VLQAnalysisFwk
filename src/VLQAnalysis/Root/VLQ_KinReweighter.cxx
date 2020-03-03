//#include "VLQAnalysis/VLQ_NtupleData.h"
#include "VLQAnalysis/VLQ_OutputData.h"
#include "VLQAnalysis/VLQ_Options.h"
//#include "VLQAnalysis/VLQ_Selector.h"
#include "VLQAnalysis/VLQ_Enums.h"

//#include "IFAETopFramework/Selection.h"
#include "IFAETopFramework/AnalysisUtils.h"

#include "TFile.h"
#include "TH1F.h"

#include <vector>
#include <map>

#include "VLQAnalysis/VLQ_KinReweighter.h"

//______________________________________________________________________________
//
VLQ_KinReweighter::VLQ_KinReweighter( VLQ_Options *opt, VLQ_OutputData *outData /*, const VLQ_NtupleData *ntupData*/ ):
  //m_ntupData(ntupData),
m_outData(outData),
m_opt(opt),
m_reweightings(0),
m_histograms(0)
{
}

//______________________________________________________________________________
//
VLQ_KinReweighter::VLQ_KinReweighter( const VLQ_KinReweighter &q ){
  //m_ntupData      = q.m_ntupData;
  m_outData       = q.m_outData;
  m_opt           = q.m_opt;
  m_reweightings  = q.m_reweightings;
  m_histograms    = q.m_histograms;
}

//______________________________________________________________________________
//
VLQ_KinReweighter::~VLQ_KinReweighter()
{

  delete m_reweightings;

  for( std::pair < std::string, TH1F* > pair : *m_histograms ){
    delete pair.second;
  }
  delete m_histograms;
}

//______________________________________________________________________________
//
bool VLQ_KinReweighter::Init( /*std::map < int, Selection* >* selection_tree,*/ const std::string &fileName )
{
  /*
  //
  // Loading all Regions for which systematics required
  //
  std::vector < std::string > vec_regions;
  for( const std::pair<int, Selection*> &sel : *(selection_tree) ){
    if( (sel.second)->PassFlagAtBit(VLQ_Selector::DOSYST) ){
    vec_regions.push_back(AnalysisUtils::ReplaceString((sel.second)->Name(), "-", ""));
    }
    }
  */

  m_histograms = new std::map < std::string, TH1F* >();

  //
  // Reweightings
  //
  m_reweightings = new std::map<std::string, int>{
    {"MET", MET},
    {"JETPT", JETPT},
    {"LEPPT", LEPPT}
  };
  
  //
  // Regions 
  //
  std::vector<std::string> vec_regions{}; 
  if(m_opt->DoOneLeptonAna()){
    vec_regions.push_back("c1lep5jin2bin");
    if(m_opt->DoSplitEMu()){
      vec_regions.push_back("c1lep5jin2bin_el");
      vec_regions.push_back("c1lep5jin2bin_mu");
    }
  }
  if(m_opt->DoZeroLeptonAna()){
    vec_regions.push_back("c0lep6jin2bin");
  }
  
  
  //
  // Filling the map with needed ratio plots
  //
  TFile *f = TFile::Open(fileName.c_str());

  for ( std::pair<std::string, int> kinpair : *m_reweightings ){
    std::string suffix = "";
    if(kinpair.second == MET) suffix = "met";
    else if(kinpair.second == JETPT) suffix = "jet0_pt";
    else if(kinpair.second == LEPPT) suffix = "lep0_pt";


    for ( const std::string region : vec_regions ){
    
      std::string histName = region + "_" + suffix; 
      TH1F* temp_hist = (TH1F*) f -> Get(histName.c_str());
      if(!temp_hist){
	std::cout << "<!> Error in VLQ_KinReweighter::Init(): Looks like the histo " << histName;
	std::cout << " is not in file " << fileName << ". Please check." << std::endl;
	continue;
      }
      temp_hist -> SetDirectory(0);
      m_histograms -> insert( std::pair < std::string, TH1F* >(histName,temp_hist) );
    }
    suffix.clear();
  }
  f -> Close();
  delete f;
  return true;
}

//______________________________________________________________________________
//
double VLQ_KinReweighter::GetKinReweight( const int kinematic ) const {
  /*
  std::cout<<" In GetKinReweight kinematic : " << kinematic 
	   << " jets_n : "<<m_outData -> o_jets_n 
	   << " el_n : "<<m_outData -> o_el_n 
	   << " mu_n : "<<m_outData -> o_mu_n 
	   << " met : "<<m_outData -> o_met
	   << " dPhi_jetmet : "<<m_outData -> o_dPhi_jetmet 
 
	   << " ISMUON : " << (m_outData -> o_channel_type == VLQ_Enums::MUON) 
	   << " ISELECTRON : " << (m_outData -> o_channel_type == VLQ_Enums::ELECTRON)
	   << " FULLHAD : " << (m_outData -> o_channel_type == VLQ_Enums::FULLHAD)
	   << std::endl;  
  */
  std::string source_reg = "";
  
  if( m_outData -> o_channel_type == VLQ_Enums::MUON || m_outData -> o_channel_type == VLQ_Enums::ELECTRON ){
    if( m_outData -> o_jets_n <= 4 ){
      //the systematic uncertainties for <= 4 jets have not been derived yet
      return 1.;
    }
    if( m_opt->DoSplitEMu() ){
      if( m_outData -> o_channel_type == VLQ_Enums::ELECTRON ){ source_reg = "c1lep5jin2bin_el"; }
      else if( m_outData -> o_channel_type == VLQ_Enums::MUON ){ source_reg = "c1lep5jin2bin_mu"; }
    }
    else{
      source_reg = "c1lep5jin2bin";
    }
    
  }
  else if( m_outData -> o_channel_type == VLQ_Enums::FULLHAD ){
    if( m_outData -> o_jets_n <= 5 ){
      //systematic uncertainties for <= 5 jets have not been derived yet for the 0-lepton channel
      return 1.;
    }
    if( kinematic == LEPPT ){
      //Cannot reweight to lepton pT if there is no lepton in the event
      return 1.;
    }
    source_reg = "c0lep6jin2bin";
  } 
  else {
    return 1.;
  }
  
  //
  // Reweighting
  //
  std::string kin = "";
  double param = 0.;
  
  if( kinematic == MET ){
    kin = "met";
    param = m_outData -> o_met;
  }
  else if( kinematic == JETPT ){
    kin = "jet0_pt";
    param = m_outData -> o_jets->at(0)->Pt();
  }
  else if( kinematic == LEPPT ){
    kin = "lep0_pt";
    param = m_outData -> o_lep->at(0)->Pt(); 
  }

  //
  // Now, actually grabs the histogram
  //
  //std::string histName = AnalysisUtils::ReplaceString( region_name, "-", "");
  std::string histName = source_reg + "_" + kin; // + "_" + sample; // + "_" + kin;
  std::map< std::string, TH1F* >::iterator it = m_histograms -> find(histName);
  if( it == m_histograms -> end() ){
    //the region is not found ... returning 1 => no systematic
    return 1;
  }
  int bin = it -> second -> FindBin( param );
  const int max_bin = it -> second -> GetNbinsX();
  if( bin > max_bin ) bin = max_bin;

  //std::cout<<" kin : " << kin << " param : " << param << " RW : "  << it -> second -> GetBinContent(bin) 
  //	   << " source_reg : " << source_reg << " bin = " << bin << "max_bin = " << max_bin << std::endl;

  return it -> second -> GetBinContent(bin);

}
