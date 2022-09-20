#ifndef VLQ_ANALYSIS_TOOLS_H
#define VLQ_ANALYSIS_TOOLS_H

#include <vector>
#include <string>

class AnalysisObject;
class OutputHistManager;

class VLQ_NtupleData;
class TTH_NtupleData;
class VLQ_NtupleData;
class VLQ_OutputData;
class VLQ_Options;
class VLQ_TRFManager;
class VLQ_VariableComputer;
class VLQ_WeightManager;
class VLQ_ResonanceMaker;
class VLQ_SmearingTool;

class VLQ_AnalysisTools {

public:

    //
    // Standard C++ functions
    //
    VLQ_AnalysisTools( VLQ_Options*, OutputHistManager*, const VLQ_NtupleData *, VLQ_OutputData *, VLQ_WeightManager *, VLQ_TRFManager *, VLQ_VariableComputer * );
    VLQ_AnalysisTools( const VLQ_AnalysisTools & );
    ~VLQ_AnalysisTools();

    //
    // Objects selection
    //
    bool GetObjectVectors();

    //
    // Histograms handling
    //
    void BookAllHistograms( const std::string &key/*, const bool split = false*/, const bool useSysts = true );
    void FillAllHistograms( const std::string &key, const int type = -1/*, const bool split = false*/ );
    void BookAllTH1DHistogramsHistManager(/*const bool split,*/ const std::string &name, const std::string &title, double binsize, double xlow, double xup, const std::string &key="", const std::string &xtitle="", const std::string &ytitle="", int lw=2, int lc=1);
    void BookAllTH1DHistogramsHistManager(/*const bool split,*/ const std::string &name, const std::string &title, int nbins, double* xedges, const std::string &key="", const std::string &xtitle="", const std::string &ytitle="", int lw=2, int lc=1);
    void FillAllTH1DHistogramsHistManager( const std::string &key, double val, double wgt, const int type = -1/*, const bool split = false*/ );

    void BookHistosForTopTaggingEfficiency( const std::string &type, const std::string &taggername );
    void FillHistosForTopTaggingEfficiency( const std::string &type, const std::string &taggername, float meff, float fatjetpt, float toppt, const bool isNumerator);
    void FillHistosForMisTagStudy( const std::string &type, const std::string &taggername, int classif);

    //
    // Update region-dependant weights (e.g. ttbar systematics)
    //
    bool UpdateRegionDependentWeight( const std::string &region_name);

    //
    // TRF handling
    //
    bool PassBTagRequirement( const int btag_req, const bool isIncl );

    //
    // Blinding
    //
    //bool PassBlindCut(  );

    //
    // Variable computation
    //
    bool ComputeAllVariables();
    bool ComputeZeroLepMVAVariables();
    bool ComputeOneLepMVAVariables();
    bool ComputeBTagVariables();
    bool UpdateBTagMoments();

protected:
    //
    // Blinding
    //
    //bool PassBlindCut_0L(  );
    //bool PassBlindCut_1L(  );


private:
    VLQ_Options *m_opt;
    OutputHistManager* m_outputMngr;

    const VLQ_NtupleData* m_ntupData;
    VLQ_OutputData* m_outData;
    VLQ_WeightManager* m_weightMngr;
    VLQ_TRFManager* m_trfMngr;
    VLQ_VariableComputer* m_varComputer;
    VLQ_ResonanceMaker* m_resonance_maker;
    VLQ_SmearingTool* m_smearing_tool;

    int m_lepWRecoOpt;
    int m_leptopRecoOpt;
};

#endif //VLQ_ANALYSIS_TOOLS_H
