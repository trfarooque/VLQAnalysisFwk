#ifndef VLQ_KINREWEIGHTER_H
#define VLQ_KINREWEIGHTER_H

#include <string>
#include <map>

class VLQ_Options;
class VLQ_OutputData;
//class VLQ_NtupleData;
class TH1F;
//class Selection;

class VLQ_KinReweighter {

public:

  //
  // Enums for the reweightings
  //
  enum Kins{
    MET = 0,
    JETPT = 1,
    LEPPT = 2
  };
    //
    // Standard C++ functions
    //
    VLQ_KinReweighter( VLQ_Options *opt, VLQ_OutputData *outData /*, const VLQ_NtupleData *ntupData*/ );
    VLQ_KinReweighter( const VLQ_KinReweighter & );
    ~VLQ_KinReweighter();

    //
    // Getter
    //
    inline const std::map<std::string, int>* GetReweightingList() const { return m_reweightings; }

    //
    // Init
    //
    bool Init( /*std::map < int, Selection* >* selection_tree,*/ const std::string &fileName );

    //
    // Real function
    //
    double GetKinReweight( /*const std::string &region_name,*/ const int kinematic ) const;

private:
    //const VLQ_NtupleData *m_ntupData;
    VLQ_OutputData *m_outData;
    VLQ_Options *m_opt;
    std::map<std::string, int> *m_reweightings; 
    std::map < std::string, TH1F* > *m_histograms;
};

#endif //VLQ_TTBARSYSTEMATICSMANAGER_H
