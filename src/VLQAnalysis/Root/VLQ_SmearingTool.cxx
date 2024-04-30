

#include "IFAETopFramework/AnalysisUtils.h"

#include <TFile.h>
#include <TH1D.h>
//#include <TRandom3.h>

//#include "TH2D.h"
//#include "TF1.h"

#include "VLQAnalysis/VLQ_SmearingTool.h"

//______________________________________________________________________________
//
VLQ_SmearingTool::VLQ_SmearingTool(  ):
  //m_ntupData(ntupData),
m_smear_map(0),
m_res_relative(0.),
m_random(0)
{
}

VLQ_SmearingTool::VLQ_SmearingTool( const VLQ_SmearingTool &q ){
  //m_ntupData      = q.m_ntupData;
  m_smear_map       = q.m_smear_map;
  m_res_relative    = q.m_res_relative;
  m_random          = q.m_random;
}

//______________________________________________________________________________
//
VLQ_SmearingTool::~VLQ_SmearingTool()
{

  delete m_smear_map;
  delete m_random;

}

//______________________________________________________________________________
//
bool VLQ_SmearingTool::Init( const std::string &fileName, const std::string& histName, const double res_relative )
{

  m_res_relative = res_relative;

  //m_random = new TRandom();
  //m_random->SetSeed( 100 );

  TFile *f = TFile::Open(fileName.c_str(), "READ");
  m_smear_map = (TH1D*)(f->Get(histName.c_str()));

  if(!m_smear_map){
    std::cout << "<!> Error in VLQ_SmearingTool::Init(): Looks like the histo " << histName;
    std::cout << " is not in file " << fileName << ". Please check." << std::endl;
    return false;
  }
  m_smear_map->SetDirectory(0);


  return true;

}

double VLQ_SmearingTool::GetSmearFactor1D(const double param) const{

  double smear_factor = 1.;

  double gaus_factor = sqrt(m_res_relative*(2. + m_res_relative));

  int pbin = m_smear_map->FindBin(param);
  double res_norm = m_smear_map->GetBinContent(pbin);

  smear_factor = 1.;//m_random->Gaus(1.,res_norm*gaus_factor);

  return smear_factor; 

}
