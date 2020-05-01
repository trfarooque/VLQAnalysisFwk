#ifndef VLQ_ANALYSIS_DATA2015_H
#define VLQ_ANALYSIS_DATA2015_H

#include <map>
#include <string>
#include <vector>

//IFAEFramework classes
class OutputHistManager;
class OutputTreeManager;
//class WeightManager;

//VLQ specific classes
class VLQ_Options;
class VLQ_OutputData;
class VLQ_AnalysisTools;
class VLQ_VariableComputer;
class VLQ_TruthManager;
class VLQ_TRFManager;
class VLQ_WeightManager;
class VLQ_NtupleData;
class VLQ_NtupleReader;
class VLQ_Selector;

class VLQ_Analysis_Data2015 {

public:
    VLQ_Analysis_Data2015(VLQ_Options *opt);
    ~VLQ_Analysis_Data2015();

    bool Begin();
    bool Loop();
    bool Process(Long64_t entry);
    bool Terminate();

private:
    /// a text summary of the yields from the main histograms
    std::string summaryYields();
    bool SumAnalysisRegions(const bool newFile);

private:
    VLQ_Options *m_opt;
    const VLQ_NtupleData *m_ntupData;
    VLQ_NtupleReader *m_reader;
    VLQ_OutputData* m_outData;
    OutputHistManager *m_outMngrHist;
    OutputTreeManager *m_outMngrTree;
    OutputTreeManager *m_outMngrOvlerlapTree;
    VLQ_AnalysisTools *m_anaTools;
    VLQ_VariableComputer *m_varComputer;
    VLQ_TruthManager *m_truthMngr;
    VLQ_TRFManager *m_TRFMngr;
    VLQ_WeightManager *m_weightMngr;
    VLQ_Selector *m_selector;
    std::map < std::string, std::vector < std::string > > m_channels;
    std::vector < std::string > m_topTaggers;
    std::vector < std::string > m_truthRCTypes;
    std::map < int, int > m_map_region_enum_index;

};//end of class VLQ_Analysis_Data2015

#endif //VLQ_ANALYSIS_DATA2015_H
