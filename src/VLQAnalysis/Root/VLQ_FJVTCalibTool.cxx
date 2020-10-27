
#include "IFAETopFramework/AnalysisUtils.h"

#include <TFile.h>
#include <TH2D.h>
#include "VLQAnalysis/VLQ_NtupleData.h"
#include "VLQAnalysis/VLQ_Options.h"


//#include "TH2D.h"
//#include "TF1.h"

#include "VLQAnalysis/VLQ_FJVTCalibTool.h"

//______________________________________________________________________________
//
VLQ_FJVTCalibTool::VLQ_FJVTCalibTool( const VLQ_NtupleData* ntupleData, const VLQ_Options* vlqOpt ):
  m_wp("Loose"),
  m_ntupData(ntupleData),
  m_opt(vlqOpt),
  m_eff_map(0),
  m_SF_map(0)
{
}

VLQ_FJVTCalibTool::VLQ_FJVTCalibTool( const VLQ_FJVTCalibTool &q ){
  m_wp              = q.m_wp;
  m_ntupData        = q.m_ntupData;
  m_opt             = q.m_opt;
  m_eff_map         = q.m_eff_map;
  m_SF_map          = q.m_SF_map;

}

//______________________________________________________________________________
//
VLQ_FJVTCalibTool::~VLQ_FJVTCalibTool()
{

  delete m_eff_map;
  delete m_SF_map;

}

//______________________________________________________________________________
//

bool VLQ_FJVTCalibTool::Init( const std::string &calibFileName ){

  TFile* calibFile = TFile::Open(calibFileName.c_str(), "READ");
  std::cout << "<INFO> VLQ_FJVTCalibTool::Init --> Opening fJvt calibration file "<< calibFileName << std::endl;
  if(!calibFile){
    std::cerr << "<ERROR> VLQ_FJVTCalibTool::Init --> Calibration file not found in path "
	      << calibFileName << std::endl; 
    return false; 
  }

  m_eff_map = (TH2D*)(calibFile->Get(("Eff"+m_wp).c_str()));
  if(!m_eff_map){
    std::cerr << "<ERROR> VLQ_FJVTCalibTool::Init --> Efficiency map "<< "Eff"+m_wp 
	      << " not found in calibration file" << std::endl;
    return false;
  }
  m_eff_map->SetDirectory(0);

  m_SF_map = (TH2D*)(calibFile->Get(("Jvt"+m_wp).c_str()));
  if(!m_SF_map){
    std::cerr << "<ERROR> VLQ_FJVTCalibTool::Init --> Scale factor map "<< "Jvt"+m_wp 
	      << " not found in calibration file" << std::endl;
    return false;
  }
  m_SF_map->SetDirectory(0);

  return true;

}


double VLQ_FJVTCalibTool::GetJetEfficiencyScaleFactor(const double pt, const double mu, const int sys_sign) const{

  double eff_sf = 1.;

  int jetbin = 0;
  jetbin  = m_SF_map -> FindBin( pt, mu );
  eff_sf  = m_SF_map -> GetBinContent( jetbin );
  if(sys_sign !=0) eff_sf += sys_sign * (m_SF_map -> GetBinError( jetbin ));

  return eff_sf;

}

double VLQ_FJVTCalibTool::GetJetInefficiencyScaleFactor(const double pt, const double mu, const int sys_sign) const{

  double ineff_sf = 1.;

  int jetbin = 0;
  jetbin = m_SF_map -> FindBin( pt, mu );

  double eff_sf = m_SF_map -> GetBinContent( jetbin );
  if(sys_sign !=0) eff_sf += sys_sign * (m_SF_map -> GetBinError( jetbin ));

  double eff = m_eff_map -> GetBinContent( jetbin );
  if(sys_sign !=0) eff    += sys_sign * (m_eff_map -> GetBinError( jetbin ));

  ineff_sf = (1. - eff_sf*eff)/(1.- eff);

  return ineff_sf;
}


double VLQ_FJVTCalibTool::GetFJVTEventWeight( const int sys_sign ) const{

  double evt_wgt = 1.;
  double pileup_mu = m_ntupData -> d_mu;

  for ( unsigned int iFwdjet = 0; iFwdjet < m_ntupData -> d_fwdjet_pt -> size(); ++iFwdjet ) {

    if( m_ntupData -> d_fwdjet_truthLabel -> at(iFwdjet) <= 0) continue; 

    //Check acceptance
    double fwdpt = m_ntupData -> d_fwdjet_pt -> at(iFwdjet);
    double fwdpt_MEV = fwdpt * 1000.;
    double fwdeta = m_ntupData -> d_fwdjet_eta -> at(iFwdjet);

    bool inRange =  ( fwdpt >= m_opt->FwdJetsPtCut()) &&
      TMath::Abs( fwdeta ) < m_opt->FwdJetsEtaCut()
      && TMath::Abs( fwdeta ) >= m_opt->JetsEtaCut()
      && (fwdpt < 120.);

    if(!inRange) continue;

    bool isFwdJet = (m_ntupData -> d_fwdjet_fJvt -> at(iFwdjet) < 0.5);

    if( isFwdJet ){  evt_wgt *= GetJetEfficiencyScaleFactor(fwdpt_MEV, pileup_mu, sys_sign);  }
    else{ evt_wgt *= GetJetInefficiencyScaleFactor(fwdpt_MEV, pileup_mu, sys_sign); }

  }//loop over all fwd jets                 

  return evt_wgt;

}
