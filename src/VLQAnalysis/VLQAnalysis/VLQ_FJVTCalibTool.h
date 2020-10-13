#ifndef VLQ_FJVTCALIBTOOL_H
#define VLQ_FJVTCALIBTOOL_H

#include <string>

class TH2D;
class VLQ_NtupleData;
class VLQ_Options;

class VLQ_FJVTCalibTool {

public:

    //
    // Standard C++ functions
    //
  VLQ_FJVTCalibTool( const VLQ_NtupleData* ntupleData, const VLQ_Options* vlqOpt );
  VLQ_FJVTCalibTool( const VLQ_FJVTCalibTool & );
  ~VLQ_FJVTCalibTool();

    //
    // Init
    //
    bool Init( const std::string &calibFileName );

    //
    // Real function
    //
    double GetJetEfficiencyScaleFactor(const double pt, const double mu, const int sys_sign=0) const;
    double GetJetInefficiencyScaleFactor(const double pt, const double mu, const int sys_sign=0) const;
    double GetFJVTEventWeight(const int sys_sign=0) const;

private:

    std::string m_wp;
    const VLQ_NtupleData* m_ntupData;
    const VLQ_Options* m_opt;

    TH2D* m_eff_map;
    TH2D* m_SF_map;

};

#endif //VLQ_FJVTCALIBTOOL_H
