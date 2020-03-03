#ifndef VLQ_TRUTHMANAGER_H
#define VLQ_TRUTHMANAGER_H

#include <vector>

class AnalysisObject;
class VLQ_Options;
class VLQ_NtupleData;
class VLQ_OutputData;
class VLQ_VariableComputer;

class VLQ_TruthManager {

public:

    //
    // Enum
    //
    enum DecayTypes {
        Ht = 0x1<<25 | 0x1<<6,
        Wb = 0x1<<24 | 0x1<<5,
        Zt = 0x1<<23 | 0x1<<6
    };

    //
    // Standard C++ function
    //
    VLQ_TruthManager( VLQ_Options* opt, const VLQ_NtupleData *ntup , VLQ_OutputData * outData, VLQ_VariableComputer* varComputer);
    VLQ_TruthManager( const VLQ_TruthManager & );
    ~VLQ_TruthManager();

    //
    // Specific functions
    //
    bool Initialize ();
    int GetVLQDecayType();
    int PrintTruthContent() const;
    int FollowDecayChain( const int index, std::string &space ) const;
    double GetTopPt() const;
    double GetHiggsPt() const;

    int FillParticlesPartonsVectors();
    int MatchJetsToBQuarks(std::vector< AnalysisObject* > &jetcol, const double DRMATCH=0.4);

    int MatchPartonToFatJets(std::vector< AnalysisObject* > &fatjets, AnalysisObject* parton, const double DRMATCH=0.75, 
			     const bool rcCollection=true,
			     const bool containment=true, const double DRCONTAINED=1.0,
			     const bool checkPt=false, const double minPtFrac=0.75, const double maxPtFrac=-1);

    int PrepareBoostedTopStudies();
    int MatchTopPartonsToFatJets(std::vector< AnalysisObject* > &Fatjets) const;
    std::vector < double > GetBTagHiggsMass();
    std::vector < double > GetFatJetsMatchedToHiggsMass(std::vector< AnalysisObject* > &Fatjets) const;
    std::vector < double > GetFatJetsMatchedToTopMass(std::vector < AnalysisObject* > &Fatjets) const;
    std::vector < int > GetTruthTops();
    int MatchParticlesPartonsToFatJets(std::vector< AnalysisObject* > &Fatjets) const;

    int PrepareHiggsStudies() ;
    int MatchHiggsPartonsToFatJets(std::vector< AnalysisObject* > &Fatjets) const; 

    int CalculateTruthVariables();


private:
    VLQ_Options *m_opt;
    const VLQ_NtupleData *m_ntupData;
    VLQ_OutputData *m_outData;
    VLQ_VariableComputer *m_varComputer;
 
    bool m_doVLQ;
    bool m_dottbar;
    AnalysisObject* RetrieveChild(const AnalysisObject* parent, int index);


};

#endif // VLQ_TRUTHMANAGER_H
