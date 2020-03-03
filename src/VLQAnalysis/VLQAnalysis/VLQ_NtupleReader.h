#ifndef VLQ_NTUPLEREADER
#define VLQ_NTUPLEREADER

#include "IFAETopFramework/NtupleReader.h"

#include <string>

class VLQ_Options;
class VLQ_NtupleData;
class TriggerInfo;

class VLQ_NtupleReader : public NtupleReader {
    
public:
    //
    // Standard C++ functions
    //
    VLQ_NtupleReader(VLQ_Options* opt);
    ~VLQ_NtupleReader();
    
    //
    // Branch setters
    //
    int SetAllBranchAddresses();
    int SetEventBranchAddresses();
    int SetJetBranchAddresses(const std::string &sj = "");
    int SetLeptonBranchAddresses();
    int SetWeightBranchAddresses();
    int SetMETBranchAddresses();
    int SetTruthParticleBranchAddresses();
    int SetTRFBranchAddresses();
    
    //Set the list of triggers
    inline void SetTriggerList(std::vector<TriggerInfo*>& trigger_list){ m_trigger_list = &trigger_list; }

    //
    // Get the ntuple data
    //
    const inline VLQ_NtupleData* Data() const{ return m_VLQ_ntupData; }
    
private:
    //
    // Here we declare the local VLQ_NtupleData object to store the
    // analysis specific variables
    //
    VLQ_NtupleData* m_VLQ_ntupData;
    VLQ_Options* m_opt;
    const std::vector<TriggerInfo*>* m_trigger_list;
    
};

#endif //VLQ_NTUPLEREADER_H
