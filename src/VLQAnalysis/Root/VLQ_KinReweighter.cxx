//#include "VLQAnalysis/VLQ_NtupleData.h"
#include "VLQAnalysis/VLQ_OutputData.h"
#include "VLQAnalysis/VLQ_Options.h"
//#include "VLQAnalysis/VLQ_Selector.h"
#include "VLQAnalysis/VLQ_Enums.h"

//#include "IFAETopFramework/Selection.h"
#include "IFAETopFramework/AnalysisUtils.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TKey.h"

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
m_histograms(0),
m_histograms_2D(0),
m_smoothFunction(0)
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
  m_histograms_2D = q.m_histograms_2D;
  m_smoothFunction = q.m_smoothFunction;
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

  for( std::pair < std::string, TH2F* > pair : *m_histograms_2D){
    delete pair.second;
  }
  delete m_histograms_2D;

  for( std::pair < std::string, TF1* > pair : *m_smoothFunction){
    delete pair.second;
  }
  delete m_smoothFunction;

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
  m_histograms_2D = new std::map < std::string, TH2F* >();
  m_smoothFunction = new std::map < std::string, TF1* >();

  //
  // Reweightings
  //
  m_reweightings = new std::map<std::string, int>();

  //Split by comma
  std::string RWlist = m_opt -> KinRWList();
  std::string RWel = "";
  std::string::size_type pos = 0;

  do{
    pos = AnalysisUtils::ParseString(RWlist, RWel, ",");
    AnalysisUtils::TrimString(RWel);
    if(RWel == "MEFF"){
      m_reweightings->insert(std::pair<std::string,int>(RWel, MEFF));
    }
    else if(RWel == "JETSN"){
      m_reweightings->insert(std::pair<std::string,int>(RWel, JETSN));
    }
    else if(RWel == "MEFFRED"){
      m_reweightings->insert(std::pair<std::string,int>(RWel, MEFFRED));
    }
    else{
      std::cerr << " Unknown reweighting : " << RWel;

    }

  }while(pos != std::string::npos);


  //
  // Regions
  //
  std::vector<std::string> vec_regions{};
  if(m_opt->DoOneLeptonAna() || m_opt->DoTwoLeptonAna() ){


    if((m_outData -> o_is_ttbar) || (m_opt -> SampleName() == SampleName::SINGLETOP)){

      vec_regions.push_back("c1lep3jin2bex");

    }
    if((m_opt -> SampleName() == SampleName::WJETS) || (m_opt -> SampleName() == SampleName::ZJETS)){

      vec_regions.push_back("c2lep3jin1bexZwinMLL_sf");

    }

  }

  //
  // Filling the map with needed ratio plots
  //
  std::cout << "VLQ_KinReweighter::Init(): Reading reweighting histogram from " << fileName << std::endl;
  TFile *f = TFile::Open(fileName.c_str());

  for(const std::string region : vec_regions){

    for(std::pair<std::string, int> kinpair : *m_reweightings){

      std::string histName = region;

      if(kinpair.second == MEFF) histName += "_meff";
      else if(kinpair.second == MEFFRED){

	histName += "_meffred";

      }
      else if(kinpair.second == JETSN) histName += "_jets_n";

      TIter next(f->GetListOfKeys());

      TKey *key;

      while((key = (TKey*)next())){

	std::string tmpHistName = key->GetName();
	std::string tmpHistClass = key->GetClassName();

	if(tmpHistName.find(histName) != std::string::npos){

	  if(tmpHistClass == "TH1D"){

	    TH1F* temp_hist = (TH1F*) f -> Get(tmpHistName.c_str());

	    std::cout << "Adding histogram " << tmpHistName << " of type " << tmpHistClass << std::endl;

	    if(!temp_hist){
	      std::cout << "<!> Error in VLQ_KinReweighter::Init(): Looks like the histo " << tmpHistName;
	      std::cout << " is not in file " << fileName << ". Please check." << std::endl;
	      continue;
	    }

	    temp_hist -> SetDirectory(0);

	    m_histograms -> insert( std::pair < std::string, TH1F* >(tmpHistName,temp_hist) );


	  }
	  else if(tmpHistClass == "TH2D"){

	    TH2F* temp_hist = (TH2F*) f -> Get(tmpHistName.c_str());

	    std::cout << "Adding histogram " <<tmpHistName << " of type " << tmpHistClass << std::endl;

	    if(!temp_hist){
	      std::cout << "<!> Error in VLQ_KinReweighter::Init(): Looks like the histo " << tmpHistName;
	      std::cout << " is not in file " << fileName << ". Please check." << std::endl;
	      continue;
	    }

	    temp_hist -> SetDirectory(0);

	    m_histograms_2D -> insert( std::pair < std::string, TH2F* >(tmpHistName,temp_hist) );

	  }
	  else if(tmpHistClass == "TF1"){

	    TF1* temp_func = (TF1*) f -> Get(tmpHistName.c_str());

	    std::cout << "Adding histogram fit " << tmpHistName << " of type " << tmpHistClass << std::endl;

	    if(!temp_func){
	      std::cout << "<!> Error in VLQ_KinReweighter::Init(): Looks like the histo fit " << tmpHistName;
	      std::cout << " is not in file " << fileName << ". Please check." << std::endl;
              continue;
	    }

	    m_smoothFunction -> insert( std::pair < std::string, TF1* >(tmpHistName, temp_func) );

	  }

	}

      }

    }

  }

  f -> Close();

  delete f;

  return true;

}

//______________________________________________________________________________
//
double VLQ_KinReweighter::GetKinReweight( const int kinematic, const std::string systematic) const {

  std::string source_reg = "";

  //std::cout << "channel type: " << m_outData->o_channel_type << std::endl;

  if( m_outData -> o_channel_type == VLQ_Enums::MUON || m_outData -> o_channel_type == VLQ_Enums::ELECTRON ||
      m_outData -> o_channel_type == VLQ_Enums::MUMU || m_outData -> o_channel_type == VLQ_Enums::ELEL){

    if((m_opt -> SampleName() == SampleName::ZJETS) || (m_opt -> SampleName() == SampleName::WJETS)){

      source_reg = "c2lep3jin1bexZwinMLL_sf";

    }
    else if((m_outData -> o_is_ttbar) || (m_opt -> SampleName() == SampleName::SINGLETOP)){

      source_reg = "c1lep3jin2bex";

    }

  }
  else {

    return 1.;

  }

  //
  // Reweighting
  //
  std::string kin = "";
  double param = 0.;
  int param2 = 0;

  if( kinematic == MEFF ){
    kin = "meff";
    param = m_outData-> o_meff;
    param2 = m_outData-> o_jets_n;
  }
  else if( kinematic == MEFFRED ){
    kin = "meffred";
    param = m_outData-> o_meffred;
    param2 = m_outData-> o_jets_n;
  }
  else if( kinematic == JETSN ){
    kin = "jets_n";
    param = m_outData->o_jets_n;
  }

  //
  // Now, actually grabs the histogram
  //
  //std::string histName = AnalysisUtils::ReplaceString( region_name, "-", "");
  std::string histName = source_reg + "_" + kin + systematic; // + "_" + sample; // + "_" + kin;

  if(kinematic == MEFFRED){

    if(source_reg == "c2lep3jin1bexZwinMLL_sf"){

      if(m_opt->DoKinRwSmoothing() && systematic == ""){

	std::string histName = source_reg + "_" + kin + "_fit" + systematic;

	std::map < std::string, TF1* >::iterator it = m_smoothFunction -> find(histName);

	if( it == m_smoothFunction -> end() ){
	  return 1;
	}

	/*std::cout << "histName : " << histName << std::endl;
	  std::cout<<" kin : " << kin << " param : " << param << " RW : "  << it -> second -> Eval(param)
	  << " source_reg : " << source_reg << " p0 = " << it -> second -> GetParameter(0)
	  << " p1 = " << it -> second -> GetParameter(1)
	  << " p2 = " << it -> second -> GetParameter(2) <<std::endl;*/

	return it -> second -> Eval(param);

      }
      else{

	std::map< std::string, TH1F* >::iterator it = m_histograms -> find(histName);

	if( it == m_histograms -> end() ){
	  //the region is not found ... returning 1 => no systematic
	  return 1;
	}

	int bin = it -> second -> FindBin( param );

	const int max_bin = it -> second -> GetNbinsX();

	if( bin > max_bin ) bin = max_bin;

	/*std::cout << "histName : " << histName << std::endl;
	  std::cout<<" kin : " << kin << " param : " << param << " RW : "  << it -> second -> GetBinContent(bin)
	  << " source_reg : " << source_reg << " bin = " << bin << " max_bin = " << max_bin << std::endl;*/

	return it -> second -> GetBinContent(bin);

      }

    }
    else if(source_reg == "c1lep3jin2bex"){

      if(m_opt->DoKinRwSmoothing() && systematic == ""){

	std::string jet_prefix = (param2 < 7) ? std::to_string(param2) : "7";

	std::string histName = source_reg + "_" + kin + "_jet_" + jet_prefix + "_fit" +  systematic;

	//std::cout << histName << std::endl;

	std::map < std::string, TF1* >::iterator it = m_smoothFunction -> find(histName);

        if( it == m_smoothFunction -> end() ){
          return 1;
        }

        /*std::cout << "histName : " << histName << std::endl;
          std::cout<<" kin : " << kin << " param : " << param << " RW : "  << it -> second -> Eval(param)
          << " source_reg : " << source_reg << " p0 = " << it -> second -> GetParameter(0)
          << " p1 = " << it -> second -> GetParameter(1)
          << " p2 = " << it -> second -> GetParameter(2) <<std::endl;*/

        return it -> second -> Eval(param);

      }
      else{

	std::map < std::string, TH2F* >::iterator it = m_histograms_2D -> find(histName);

	if( it == m_histograms_2D -> end() ){
	  //the region is not found ... returning 1 => no systematic
	  return 1;
	}

	int binx = 0;
	int biny = 0;
	int binz = 0;

	int globalbin = it -> second -> FindBin(param, param2);

	it -> second -> GetBinXYZ(globalbin, binx, biny, binz);

	const int max_xbin = it -> second -> GetNbinsX();

	const int max_ybin = it -> second -> GetNbinsY();

	if( binx > max_xbin) binx = max_xbin;

	if( biny > max_ybin) biny = max_ybin;

	/*std::cout << "histName : " << histName << std::endl;
	  std::cout <<" kin : " << kin << " param1 : " << param << " param2 : " << param2 << " RW : "  << it -> second -> GetBinContent(binx, biny)
  	  << " source_reg : " << source_reg << " binx = " << binx << " max_xbin = " << max_xbin
	  << " biny = " << biny <<" max_ybin = " << max_ybin <<std::endl;*/

	return it -> second -> GetBinContent(binx,biny);

      }

    }

  }
  else if(kinematic == MEFF){

    std::map < std::string, TH2F* >::iterator it = m_histograms_2D -> find(histName);

    if( it == m_histograms_2D -> end() ){
      //the region is not found ... returning 1 => no systematic
      return 1;
    }

    int binx = 0;
    int biny = 0;
    int binz = 0;

    int globalbin = it -> second -> FindBin(param, param2);

    it -> second -> GetBinXYZ(globalbin, binx, biny, binz);

    const int max_xbin = it -> second -> GetNbinsX();

    const int max_ybin = it -> second -> GetNbinsY();

    if( binx > max_xbin) binx = max_xbin;

    if( biny > max_ybin) biny = max_ybin;

    /*std::cout << "histName : " << histName << std::endl;
    std::cout<<" kin : " << kin << " param1 : " << param << " param2 : " << param2 << " RW : "  << it -> second -> GetBinContent(binx, biny)
             << " source_reg : " << source_reg << " binx = " << binx << " max_xbin = " << max_xbin
    	     << " biny = " << biny <<" max_ybin = " << max_ybin <<std::endl;*/

    return it -> second -> GetBinContent(binx,biny);

  }
  else if(kinematic == JETSN){
    std::map< std::string, TH1F* >::iterator it = m_histograms -> find(histName);

    if( it == m_histograms -> end() ){
      //the region is not found ... returning 1 => no systematic
      return 1;
    }

    int bin = it -> second -> FindBin( param );

    const int max_bin = it -> second -> GetNbinsX();

    if( bin > max_bin ) bin = max_bin;

    /*std::cout << "histName : " << histName << std::endl;
    std::cout <<" kin : " << kin << " param : " << param << " RW : "  << it -> second -> GetBinContent(bin)
    << " source_reg : " << source_reg << " bin = " << bin << " max_bin = " << max_bin << std::endl;*/

    return it -> second -> GetBinContent(bin);
  }


  return 1;


}

//______________________________________________________________________________ 
//
double VLQ_KinReweighter::GetKinRwSyst(std::string systematic) const{

  std::string source_reg = "";

  if( m_outData -> o_channel_type == VLQ_Enums::MUON || m_outData -> o_channel_type == VLQ_Enums::ELECTRON ||
      m_outData -> o_channel_type == VLQ_Enums::MUMU || m_outData -> o_channel_type == VLQ_Enums::ELEL){

    if((m_opt -> SampleName() == SampleName::ZJETS) || (m_opt -> SampleName() == SampleName::WJETS)){

      source_reg = "c2lep3jin1bexZwinMLL_sf";

    }
    else if((m_outData -> o_is_ttbar) || (m_opt -> SampleName() == SampleName::SINGLETOP)){

      source_reg = "c1lep3jin2bex";

    }

  }
  else {

    return 1.;

  }

  std::string kin = "meffred";
  
  double param = m_outData-> o_meffred;
  
  int param2 = m_outData-> o_jets_n;

  std::string histName = source_reg + "_" + kin + "_fit" + systematic;

  if(source_reg == "c2lep3jin1bexZwinMLL_sf"){

    std::map < std::string, TF1* >::iterator it = m_smoothFunction -> find(histName);
    
    if( it == m_smoothFunction -> end() ){
      return 1;
    }
    
    /*std::cout << "histName : " << histName << std::endl;                                                                                                                                                                                                                
      std::cout<<" kin : " << kin << " param : " << param << " RW : "  << it -> second -> Eval(param)                                                                                                                                                                     
      << " source_reg : " << source_reg << " p0 = " << it -> second -> GetParameter(0)                                                                                                                                                                                    
      << " p1 = " << it -> second -> GetParameter(1)                                                                                                                                                                                                                      
      << " p2 = " << it -> second -> GetParameter(2) <<std::endl;*/
    
    return it -> second -> Eval(param);

  }
  else if(source_reg == "c1lep3jin2bex"){

    std::string jet_prefix = (param2 < 7) ? std::to_string(param2) : "7";
    
    std::string histName = source_reg + "_" + kin + "_jet_" + jet_prefix + "_fit" +  systematic;

    std::map < std::string, TF1* >::iterator it = m_smoothFunction -> find(histName);

    if( it == m_smoothFunction -> end() ){
      return 1;
    }

    /*std::cout << "histName : " << histName << std::endl;                                                                                                                                                                                                                
      std::cout<<" kin : " << kin << " param : " << param << " RW : "  << it -> second -> Eval(param)                                                                                                                                                                     
      << " source_reg : " << source_reg << " p0 = " << it -> second -> GetParameter(0)                                                                                                                                                                                    
      << " p1 = " << it -> second -> GetParameter(1)                                                                                                                                                                                                                      
      << " p2 = " << it -> second -> GetParameter(2) <<std::endl;*/
    
    return it -> second -> Eval(param);
   
  }

  return 1;

}
