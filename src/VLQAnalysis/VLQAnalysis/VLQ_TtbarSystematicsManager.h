#ifndef VLQ_TTBARSYSTEMATICSMANAGER_H
#define VLQ_TTBARSYSTEMATICSMANAGER_H

#include <string>
#include <map>

class VLQ_Options;
class VLQ_OutputData;
class VLQ_NtupleData;
class TH1F;
class Selection;

class VLQ_TtbarSystematicsManager {

public:

    //
    // Enums for the systematics
    //
    enum Systs {
        PS = 0,
        GEN = 1,
        GENPS = 2,
        RADHI = 3,
        RADLOW = 4
    };

    //
    // Standard C++ functions
    //
    VLQ_TtbarSystematicsManager( VLQ_Options *opt, VLQ_OutputData *outData, const VLQ_NtupleData *ntupData );
    VLQ_TtbarSystematicsManager( const VLQ_TtbarSystematicsManager & );
    ~VLQ_TtbarSystematicsManager();

    //
    // Init
    //
    bool Init( std::map < int, Selection* >* selection_tree, const std::string &fileName );

    //
    // Real function
    //
    double GetTtbarSystWeight( const std::string &region_name, const int systematic ) const;

private:
    const VLQ_NtupleData *m_ntupData;
    VLQ_OutputData *m_outData;
    VLQ_Options *m_opt;
    std::map < std::string, TH1F* > *m_histograms;
};

#endif //VLQ_TTBARSYSTEMATICSMANAGER_H
