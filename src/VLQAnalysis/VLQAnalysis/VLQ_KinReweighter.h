#ifndef VLQ_KINREWEIGHTER_H
#define VLQ_KINREWEIGHTER_H

#include <string>
#include <map>

class VLQ_Options;
class VLQ_OutputData;
//class VLQ_NtupleData;
class TH1D;
class TH2D;
class TF1;
//class Selection;

class VLQ_KinReweighter {

public:

  //
  // Enums for the reweightings
  //
  enum Kins{
    MET = 0,
    JETPT = 1,
    LEPPT = 2,
    MEFF = 3,
    JETSN = 4,
    HTALL = 5,
    MEFFRED = 6
  };
    //
    // Standard C++ functions
    //
    VLQ_KinReweighter( VLQ_Options *opt, VLQ_OutputData *outData);
    VLQ_KinReweighter( const VLQ_KinReweighter & );
    ~VLQ_KinReweighter();

    //
    // Getter
    //
    inline const std::map<std::string, int>* GetReweightingList() const { return m_reweightings; }

    //
    // Init
    //
    bool Init( const std::string &fileName );

    //
    // Real function
    //
    double GetKinReweight( const int kinematic, const std::string systematic ="") const;
    double GetKinRwSyst(std::string systematic) const;
    double GetNJetsKinRwSyst(std::string systematic) const;

private:
    VLQ_OutputData *m_outData;
    VLQ_Options *m_opt;
    std::map<std::string, int> *m_reweightings; 
    std::map < std::string, TH1D* > *m_histograms_1D;
    std::map < std::string, TH2D* > *m_histograms_2D;
    std::map < std::string, TF1* >  *m_histograms_fits;
};

#endif //VLQ_TTBARSYSTEMATICSMANAGER_H
