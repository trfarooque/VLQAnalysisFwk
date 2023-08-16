//#include "VLQAnalysis/VLQ_NtupleData.h"
#include "VLQAnalysis/VLQ_OutputData.h"
#include "VLQAnalysis/VLQ_Options.h"
//#include "VLQAnalysis/VLQ_Selector.h"
#include "VLQAnalysis/VLQ_Enums.h"

//#include "IFAETopFramework/Selection.h"
#include "IFAETopFramework/AnalysisUtils.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TKey.h"

#include <vector>
#include <map>

#include "VLQAnalysis/VLQ_KinReweighter.h"

//______________________________________________________________________________
//
VLQ_KinReweighter::VLQ_KinReweighter( VLQ_Options *opt, VLQ_OutputData *outData ):
m_outData(outData),
m_opt(opt),
m_reweightings(0),
m_histograms_1D(0),
m_histograms_2D(0),
m_histograms_fits(0)
{
}

//______________________________________________________________________________
//
VLQ_KinReweighter::VLQ_KinReweighter( const VLQ_KinReweighter &q ){
  m_outData         = q.m_outData;
  m_opt             = q.m_opt;
  m_reweightings    = q.m_reweightings;
  m_histograms_1D   = q.m_histograms_1D;
  m_histograms_2D   = q.m_histograms_2D;
  m_histograms_fits = q.m_histograms_fits;
}

//______________________________________________________________________________
//
VLQ_KinReweighter::~VLQ_KinReweighter()
{

  delete m_reweightings;

  for( std::pair < std::string, TH1D* > pair : *m_histograms_1D ){
    delete pair.second;
  }
  delete m_histograms_1D;

  for( std::pair < std::string, TH2D* > pair : *m_histograms_2D){
    delete pair.second;
  }
  delete m_histograms_2D;

  for( std::pair < std::string, TF1* > pair : *m_histograms_fits){
    delete pair.second;
  }
  delete m_histograms_fits;

}

//______________________________________________________________________________
//
bool VLQ_KinReweighter::Init( const std::string &fileName )
{

  m_histograms_1D   = new std::map < std::string, TH1D* >();
  m_histograms_2D   = new std::map < std::string, TH2D* >();
  m_histograms_fits = new std::map < std::string, TF1*  >();

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
      std::cerr << " Unknown reweighting : " << RWel << std::endl;
    }
  }while(pos != std::string::npos);

  std::cout << "VLQ_KinReweighter::Init() : Reading kinematic reweightings from " << fileName << std::endl;

  TFile* f = TFile::Open(fileName.c_str(), "READ");

  TIter next(f->GetListOfKeys());
  TKey* key;

  while( (key = (TKey*)next()) ){
    std::string tmp_rw_name = key->GetName();
    std::string tmp_rw_type = key->GetClassName();

    std::cout << "Reading reweighting " << tmp_rw_name <<  " of type " << tmp_rw_type << std::endl;

    if(tmp_rw_type == "TH1D"){
      TH1D* tmp_rw = (TH1D*)(f->Get(tmp_rw_name.c_str()));
      
      tmp_rw->SetDirectory(0);

      m_histograms_1D->insert( std::pair < std::string, TH1D* >(tmp_rw_name,tmp_rw) );
    }
    else if(tmp_rw_type == "TH2D"){
      TH2D* tmp_rw = (TH2D*)(f->Get(tmp_rw_name.c_str()));

      tmp_rw->SetDirectory(0);

      m_histograms_2D->insert( std::pair < std::string, TH2D* >(tmp_rw_name,tmp_rw) );
    }
    else if(tmp_rw_type == "TF1"){
      TF1* tmp_rw = (TF1*)(f->Get(tmp_rw_name.c_str()));
      
      m_histograms_fits->insert( std::pair < std::string, TF1* >(tmp_rw_name,tmp_rw) );
      
    }
    else{
      std::cout << "< WARNING > : Reweightings stored as " << tmp_rw_type << " are not supported. Skipping." << std::endl;
    }
    
  }

  f -> Close();

  delete f;

  return true;

}

//______________________________________________________________________________
//
double VLQ_KinReweighter::GetKinReweight(const int kinematic, const std::string systematic) const {

  double reweighting = 1.0;

  if(m_outData -> o_channel_type == VLQ_Enums::UNDEFINED){
    return reweighting;
  }
  /*
  if(m_outData -> o_channel_type == VLQ_Enums::UNDEFINED || m_outData -> o_channel_type == VLQ_Enums::ELMU || m_outData -> o_channel_type == VLQ_Enums::FULLHAD){
    return reweighting;
  }
  */
  std::string source_reg = "";

  if((m_opt->SampleName() == SampleName::ZJETS) || (m_opt->SampleName() == SampleName::WJETS)) source_reg = "c2lep3jin1bexZwinMLL_sf";
  else if((m_outData->o_is_ttbar) || (m_opt->SampleName() == SampleName::SINGLETOP)) source_reg = "c1lep3jin2bex";

  if(kinematic == JETSN){    
    std::string rw_name = source_reg + "_jets_n" + systematic;
    
    std::map< std::string, TH1D* >::iterator it = m_histograms_1D->find(rw_name);

    if( it == m_histograms_1D->end() ) return 1.;
  
    int bin = it->second->FindBin( m_outData->o_jets_n );

    if( bin > it->second->GetNbinsX() ) bin = it->second->GetNbinsX();

    if(m_opt -> MsgLevel() == Debug::DEBUG){
      std::cout << "rw_name : " << rw_name << ", jets_n : " << m_outData->o_jets_n << ", bin : " << bin << ", RW : " << it->second->GetBinContent(bin) << std::endl;
    }

    reweighting = it->second->GetBinContent(bin);
  }
  else if(kinematic == MEFF){
    
    std::string rw_name = "";

    if((m_opt->SampleName() == SampleName::ZJETS) || (m_opt->SampleName() == SampleName::WJETS)) rw_name = "c2lep3jex1bexZwinMLL_sf_meff";
    else if((m_outData->o_is_ttbar) || (m_opt->SampleName() == SampleName::SINGLETOP)) rw_name = "c1lep3jex2bex_meff";

    std::map< std::string, TH1D* >::iterator it = m_histograms_1D->find(rw_name);
    
    if( it == m_histograms_1D->end() ) return 1.;
    
    int bin = it->second->FindBin( m_outData->o_meff );
    
    if( bin > it->second->GetNbinsX() ) bin = it->second->GetNbinsX();
    
    if(m_opt -> MsgLevel() == Debug::DEBUG){
      std::cout << "rw_name : " << rw_name << ", meff : " << m_outData->o_meff << ", bin : " << bin << ", RW : " << it->second->GetBinContent(bin) << std::endl;
    }
    
    reweighting = it->second->GetBinContent(bin);
  }
  else if(kinematic == MEFFRED){

    if((m_opt->SampleName() == SampleName::ZJETS) || (m_opt->SampleName() == SampleName::WJETS)){
      std::string rw_name = source_reg+"_meffred";

      if(m_opt->DoKinRwSmoothing() && systematic == ""){
	rw_name += "_fit";
	
	std::map< std::string, TF1* >::iterator it = m_histograms_fits->find(rw_name);

	if( it == m_histograms_fits->end() ) return 1.;

	if(m_opt -> MsgLevel() == Debug::DEBUG){
	  std::cout << "rw_name : " << rw_name << ", meffred : " << m_outData->o_meffred << ", RW : " << it->second->Eval(m_outData->o_meffred) << std::endl;
	}

	reweighting = it->second->Eval(m_outData->o_meffred);
      }
      else{
	rw_name += systematic;

	std::map< std::string, TH1D* >::iterator it = m_histograms_1D->find(rw_name);

	if( it == m_histograms_1D->end() ) return 1.;

	int bin = it->second->FindBin( m_outData->o_meffred );

	if( bin > it->second->GetNbinsX() ) bin = it->second->GetNbinsX();

	if(m_opt -> MsgLevel() == Debug::DEBUG){
	  std::cout << "rw_name : " << rw_name << ", meffred : " << m_outData->o_meffred << ", bin : " << bin << ", RW : " << it->second->GetBinContent(bin) << std::endl;
	}

	reweighting = it->second->GetBinContent(bin);
      }

    }
    else if((m_outData->o_is_ttbar) || (m_opt->SampleName() == SampleName::SINGLETOP)){
      
      if(m_opt->DoKinRwSmoothing() && systematic == ""){

	// this will need to take into account if sVLQ ( < 7 ) or pVLQ ( < 8 )
	int last_njet_bin = (m_opt->UseSVLQConfig()) ? 7 : 8;

	std::string jet_prefix = (m_outData->o_jets_n < last_njet_bin) ? std::to_string(m_outData->o_jets_n) : std::to_string(last_njet_bin);

	std::string rw_name = source_reg + "_meffred_jet_" + jet_prefix + "_fit";

	std::map < std::string, TF1* >::iterator it = m_histograms_fits->find(rw_name);

        if( it == m_histograms_fits->end() ) return 1;

	if(m_opt -> MsgLevel() == Debug::DEBUG){
	  std::cout << "rw_name : " << rw_name << ", meffred : " << m_outData->o_meffred << ", jets_n : " << m_outData->o_jets_n 
		    << ", RW : " << it->second->Eval(m_outData->o_meffred) << std::endl;
	}
	  
	reweighting = it->second->Eval(m_outData->o_meffred);
      }
      else{
	std::string rw_name = source_reg + "_meffred" + systematic;
	
	std::map < std::string, TH2D* >::iterator it = m_histograms_2D->find(rw_name);

        if( it == m_histograms_2D->end() ) return 1;
        
        int binx = 0;
        int biny = 0;
        int binz = 0;

        int global_bin = it->second->FindBin(m_outData->o_meffred, m_outData->o_jets_n);

        it->second->GetBinXYZ(global_bin, binx, biny, binz);

	if(binx > it->second->GetNbinsX()) binx = it->second->GetNbinsX();

	if(biny > it->second->GetNbinsY()) biny = it->second->GetNbinsY();

	if(m_opt -> MsgLevel() == Debug::DEBUG){
	  std::cout << "rw_name : " << rw_name << ", meffred : " << m_outData->o_meffred << ", jets_n : " << m_outData->o_jets_n 
		    << ", binx : " << binx << ", biny : " << biny
		    << ", RW : " << it->second->GetBinContent(binx,biny) << std::endl;
	}	  

	reweighting = it->second->GetBinContent(binx,biny);
      }
      
    }

  }

  return reweighting;

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

    std::map < std::string, TF1* >::iterator it = m_histograms_fits -> find(histName);
    
    if( it == m_histograms_fits -> end() ){
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

    int last_njet_bin = (m_opt->UseSVLQConfig()) ? 7 : 8;

    std::string jet_prefix = (param2 < last_njet_bin) ? std::to_string(param2) : std::to_string(last_njet_bin);
    
    std::string histName = source_reg + "_" + kin + "_jet_" + jet_prefix + "_fit" +  systematic;

    std::map < std::string, TF1* >::iterator it = m_histograms_fits -> find(histName);

    if( it == m_histograms_fits -> end() ){
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

//______________________________________________________________________________ 
//
double VLQ_KinReweighter::GetNJetsKinRwSyst(std::string systematic) const{

  if((m_opt -> SampleName() != SampleName::ZJETS) && (m_opt -> SampleName() != SampleName::WJETS)) return 1;

  std::string rw_name =  "c2lep3jin1bexZwinMLL_sf_jets_n"; 
  
  int param = m_outData->o_jets_n;
  
  std::map< std::string, TH1D* >::iterator it = m_histograms_1D->find(rw_name);

  if( it == m_histograms_1D->end() ) return 1.;

  int bin = it->second->FindBin(param);

  if( bin > it->second->GetNbinsX() ) bin = it->second->GetNbinsX();

  if( systematic == "UP" ) return (it->second->GetBinContent(bin) + it->second->GetBinError(bin));
  else if( systematic == "DOWN" ) return (it->second->GetBinContent(bin) - it->second->GetBinError(bin));
  else return 1;

}
