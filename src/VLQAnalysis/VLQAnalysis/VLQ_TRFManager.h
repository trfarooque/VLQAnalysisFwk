#ifndef VLQ_TRFMANAGER_H
#define VLQ_TRFMANAGER_H

#include <vector>
#include <string>

//ITF
class AnalysisObject;
class WeightManager;
#include "IFAETopFramework/TRFManager.h"

//VA
class VLQ_OutputData;
class VLQ_NtupleData;
class VLQ_Options;
class VLQ_NtupleData;

//TRF
class TRFinterface;

class VLQ_TRFManager: public TRFManager {

public:

    //
    // Standard C++ functions
    //
    VLQ_TRFManager( VLQ_Options* opt, WeightManager* weightMngr, const VLQ_NtupleData* ntupData, VLQ_OutputData* outData );
    VLQ_TRFManager( const VLQ_TRFManager & );
    ~VLQ_TRFManager();

    //
    // Specific functions
    //
    void Init();
    using TRFManager::ComputeTRFWeights;
    bool ComputeTRFWeights();
    bool UpdateBTagging(const bool isIncl, const int req_nbjets);

private:
    VLQ_Options *m_opt;
    WeightManager* m_weightMngr;
    const VLQ_NtupleData* m_ntupData;
    VLQ_OutputData* m_outData;
    TRFinterface *m_trfint;
    std::string m_btag_calib_scheme;
    std::string m_btag_name;
};

#endif //VLQ_TRFMANAGER_H
