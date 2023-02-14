#ifndef VLQ_WEIGHTSETTER_H
#define VLQ_WEIGHTSETTER_H

//in IFAETopFramework
#include "IFAETopFramework/WeightManager.h"

#include <map>

class SampleInfo;

//in VLQAnalysis
class VLQ_Options;
class VLQ_OutputData;
class VLQ_NtupleData;
class VLQ_VariableComputer;
class VLQ_TtbarSystematicsManager;
class VLQ_KinReweighter;
class VLQ_FJVTCalibTool;

//in IFAEReweightingTools
class ttbbNLO_syst;
class TtbarFractionReweighter;
class ttccNLO_syst;

//in IFAETopFramework
class Selection;
class OutputHistManager;
  
//in NNLOReweighter
//class NNLOReweighter;

//in FakeLeptonEstimation
class FakeLeptonEstimation;

class VLQ_WeightManager : public WeightManager {

public:
  
  enum FlagTypes{REWEIGHT=3};

    //
    // Standard C++ functions
    //
  VLQ_WeightManager( VLQ_Options *opt, const VLQ_NtupleData* ntupleData, VLQ_OutputData* outputData,  OutputHistManager* outHistMngr=NULL);
    VLQ_WeightManager( const VLQ_WeightManager & );
    virtual ~VLQ_WeightManager();

    //
    // Class specific functions
    //
    void Init( std::map < int, Selection* >* selection_tree );
    bool AddVLQNominalWeights();
    bool AddVLQSystematicWeights( const bool dump_config = false );
    bool AddKinReweightings();
    bool AddKinRwSyst();
    bool AddFJvtSFWeights();
    bool AddLargeRJetTaggerSFs();

    bool SetLeptonSFWeights( const bool apply_trigger_weights );
    bool SetCrossSectionWeight();
    bool SetQCDWeight();
    bool SetFJvtSFWeights();
    bool SetPMGSystNorm();
    bool SetTtccWeights();
    bool SetTtbarGeneratorSystematics( const std::string &region );
    bool SetNNLOSystematic();
    bool SetTtbarHtSliceScale();
    bool SetTtbarFractionWeight();
    bool SetNNLOWeight( const double topPt );
    bool SetLargeRJetTaggerSFs();
    bool SetKinReweightings();
    bool SetKinRwSyst();
    bool UpdateSysReweighting();

private:
    VLQ_Options *m_vlq_opt;
    const VLQ_NtupleData *m_vlq_ntupData;
    VLQ_OutputData *m_vlq_outData;
    OutputHistManager *m_outHistMngr;

    SampleInfo *m_sampleInfo;
    FakeLeptonEstimation* m_qcdWeight;
    VLQ_VariableComputer *m_varComputer;
    ttbbNLO_syst *m_tool_HFsyst;
    TtbarFractionReweighter *m_tool_ttFractionRw;
    VLQ_TtbarSystematicsManager *m_ttbar_syst_weight;
    //NNLOReweighter* m_nnlo_rw;
    VLQ_KinReweighter* m_kinRw;
    VLQ_FJVTCalibTool* m_fJvt_calibTool;

    std::vector<std::string>* m_syst_regions;

};

#endif //VLQ_WEIGHTSETTER_H
