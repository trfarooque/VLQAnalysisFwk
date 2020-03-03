#ifndef VLQ_RESONANCEMAKER_H
#define VLQ_RESONANCEMAKER_H

#include <string>
#include <map>

class VLQ_Options;
class VLQ_OutputData;

class VLQ_ResonanceMaker {

public:

  //
  // Options for LepW reconstruction
  //
  static const int MW_CONSTRAINT;
  static const int MTOP_CONSTRAINT;
  static const int COLLINEAR;
  static const int VANILLA;

  //
  // Options for LepTop reconstruction
  //
  static const int USE_LIGHT;
  static const int PREF_BTAG;
  static const int VETO_RCMATCH;


  //
  // VLQ decay type
  //
  enum VLQ_decay{
    Ht=1, Wb, Zt
  };

    //
    // Standard C++ functions
    //
    VLQ_ResonanceMaker( VLQ_Options *opt, VLQ_OutputData *outData  );
    VLQ_ResonanceMaker( const VLQ_ResonanceMaker & );
    ~VLQ_ResonanceMaker();

    //
    // Getter
    //


    //
    // Init
    //
    bool Init(  );

    //
    // Real function
    //
    int MakeLepW( const int mode);
    int MakeLeptop( const int mode, const double drmax=1.0);
    int MakeSemiBoostedHadtop( const double mmin = 140., const double mmax = 210., const double drmax = 1.0);
    int MakeSingleVLQ( const std::string&  decay ); 
    int MakePairVLQ( const std::string&  decay, const double drmax=3.0 ); 

    //
    // Mass constants
    //
    static const double MW;

private:

    VLQ_Options *m_opt;
    VLQ_OutputData *m_outData;

};

#endif //VLQ_TTBARSYSTEMATICSMANAGER_H
