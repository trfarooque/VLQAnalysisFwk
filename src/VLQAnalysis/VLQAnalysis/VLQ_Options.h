#ifndef VLQ_OPTIONS_H
#define VLQ_OPTIONS_H

#include <string>
#include "IFAETopFramework/OptionsBase.h"

class VLQ_Options : public OptionsBase {

public:

  enum FilteringType {
    NOFILTER = 0,
    APPLYFILTER = 1
  };

  enum BtagColl{
    CALO = 0,
    TRACK = 1
  };
  //
  // Standard C++ functions
  //
  VLQ_Options();
  VLQ_Options( const VLQ_Options & );
  ~VLQ_Options();

  //
  // Specific functions
  //
  virtual void PrintOptions();

  //
  // Getters
  //
  // bools
  inline bool UseLeptonsSF() const { return m_useLeptonsSF; }
  inline bool UseBtagSF() const { return m_useBtagSF; }
  inline bool RecomputeBtagSF() const { return m_recomputeBtagSF; }
  inline bool UsePileUpWeight() const { return m_usePileUpWeight; }
  inline bool DumpHistos() const { return m_dumpHistos; }
  inline bool DumpTree() const { return m_dumpTree; }
  inline bool DumpOverlapTree() const { return m_dumpOverlapTree; }
  inline bool SplitVLQDecays() const { return m_splitVLQDecays; }
  inline bool DoTruthAnalysis() const { return m_doTruthAnalysis; }
  inline bool DoTRF() const { return m_doTRF; }
  inline bool RecomputeTRF() const { return m_recomputeTRF; }
  inline bool ApplyMetMtwCuts() const { return m_applyMetMtwCuts; }
  inline bool InvertMetMtwCuts() const { return m_invertMetMtwCuts; }
  inline bool ApplyDeltaPhiCut() const { return m_applyDeltaPhiCut; }
  inline bool InvertDeltaPhiCut() const { return m_invertDeltaPhiCut; }
  inline bool ApplyMetSignificanceCut() const { return m_applyMetSignificanceCut; }
  inline bool ApplyTtbbCorrection() const { return m_applyTtbbCorrection; }
  inline bool RecomputeTtBbRw() const { return m_RecTtBbRw; }
  inline bool ReweightTtbarFractions() const { return m_RwTtFractions; }
  inline bool MultipleVariables() const { return m_multipleVariablesWithUncertainties; }
  inline bool VerboseOutput() const { return m_verboseOutput; }
  inline bool UseMETTrigger() const { return m_useMETTrigger; }
  inline bool UseMETTriggerOneLep() const { return m_useMETTriggerOneLep; }
  inline bool UseLeptonTrigger() const { return m_useLeptonTrigger; }
  inline bool UseLargeRJets() const { return m_useLargeRJets; }
  inline bool DoCutFlow() const { return m_doCutFlow; }
  inline bool DoOneLeptonAna() const { return m_doOneLeptonAna; }
  inline bool DoTwoLeptonAna() const { return m_doTwoLeptonAna; }
  inline bool DoZeroLeptonAna() const { return m_doZeroLeptonAna; }
  inline bool DoPreselection() const { return m_doPreselection; }
  inline bool DoExclusiveJetRegions() const { return m_doExclusiveJetRegions; }
  inline bool DoExtendedPreselection() const { return m_doExtendedPreselection; }
  inline bool DoSingleVLQRegions() const { return m_doSingleVLQRegions; }
  inline bool DoPairVLQRegions() const { return m_doPairVLQRegions; }
  inline bool DoValidnRegions() const { return m_doValidnRegions; }
  inline bool DoFitRegions() const { return m_doFitRegions; }
  inline bool DoLooseSystRegions() const { return m_doLooseSystRegions; }
  inline bool DoLowBRegions() const { return m_doLowBRegions; }
  inline bool DoLowJRegions() const { return m_doLowJRegions; }
  inline bool DoSplitEMu() const { return m_doSplitEMu; }
  inline bool DoOldBoost() const { return m_doOldBoost; }
  inline bool DoSplitMtb() const { return m_doSplitMtb; }
  inline bool DoSplitMbb() const { return m_doSplitMbb; }
  inline bool DoSumRegions() const { return m_doSumRegions; }
  inline bool ScaleTtbarHtSlices() const { return m_scaleTtbarHtSlices; }
  inline bool ApplyTtbarNNLOCorrection() const { return m_applyTtbarNNLOCorrection; }
  inline bool RecomputeTtbarNNLOCorrection() const { return m_recomputeTtbarNNLOCorrection; }
  inline bool ApplyVjetsSherpa22RW() const { return m_applyVjetsSherpa22RW; }
  inline bool ComputeTtccNLO() const {return m_computeTtccNLO;}
  inline bool ReweightKinematics() const { return m_reweightKinematics; }
  inline bool OnlyReweightTtbarKin() const { return m_onlyReweightTtbarKin; }
  inline bool MakeQCD0LSystematics() const { return m_makeQCD0LSystematics; }
  inline bool DoPreselSys() const { return m_doPreselSys; }
  inline bool DoExpSys() const { return m_doExpSys; }
  inline bool DoTheorySys() const { return m_doTheorySys; }

  // strings
  inline std::string DoRecoVLQ() const { return m_doRecoVLQ; }
  inline std::string BtagOP() const { return m_btagOP; }
  inline std::string BtagAlg() const { return m_btagAlg; }
  inline std::string TRFCDIPath() const { return m_TRFCDIPath; }
  inline std::string SampleDat() const { return m_sampleDat; }
  inline std::string LepWOpt() const { return m_lepWOpt; }
  inline std::string LeptopOpt() const { return m_leptopOpt; }
  inline std::string RCCollection() const { return m_RCCollection; }
  // doubles
  inline double RCJetsPtCut() const { return m_RCJetsPtCut; }
  inline double JetsPtCut() const { return m_jetsPtCut; }
  inline double JetsEtaCut() const { return m_jetsEtaCut; }
  inline double FwdJetsPtCut() const { return m_fwdJetsPtCut; }
  inline double FwdJetsEtaCut() const { return m_fwdJetsEtaCut; }
  inline double TrkJetsPtCut() const { return m_trkJetsPtCut; }
  inline double LepPtCut() const { return m_lepPtCut; }
  inline double MtbminCut() const { return m_mtbminCut; }
  inline double MinDeltaPhiCut() const { return m_minDeltaPhiCut; }
  inline double MaxDeltaPhiCut() const { return m_maxDeltaPhiCut; }
  inline double MinMeffCut() const { return m_minMeffCut; }
  inline double MaxMeffCut() const { return m_maxMeffCut; }
  inline double MinMetCutOneLep() const { return m_minMetCutOneLep; }
  inline double MaxMetCutOneLep() const { return m_maxMetCutOneLep; }
  inline double MinMetCutZeroLep() const { return m_minMetCutZeroLep; }
  inline double MaxMetCutZeroLep() const { return m_maxMetCutZeroLep; }
  inline double MaxLeptopDR() const { return m_maxLeptopDR; }
  // ints
  inline int MaxTRFB() const { return m_maxb; }
  inline double RCNsubjetsCut() const { return m_RCNsubjetsCut; }

  // enums
  inline FilteringType FilterType() const { return m_filterType; }
  inline BtagColl BtagCollection() const { return m_btagCollection; }

  /// check whether the options are consistent
  /**
  Raise std::logic_error if the options are not a vaild choice.
  */
  void checkConcistency() const;
protected:
  virtual bool IdentifyOption( const std::string &, const std::string & );

private:
  bool m_useLeptonsSF, m_useBtagSF, m_recomputeBtagSF, m_usePileUpWeight;
  bool m_dumpHistos, m_dumpTree, m_dumpOverlapTree;
  bool m_splitVLQDecays;
  bool m_doTruthAnalysis;
  bool m_doTRF, m_recomputeTRF;
  bool m_applyMetMtwCuts;
  bool m_invertMetMtwCuts;
  bool m_applyDeltaPhiCut;
  bool m_invertDeltaPhiCut;
  bool m_applyMetSignificanceCut;
  bool m_applyTtbbCorrection;
  bool m_multipleVariablesWithUncertainties;
  bool m_verboseOutput;
  bool m_RecTtBbRw;
  bool m_RwTtFractions;
  bool m_useMETTrigger;
  bool m_useMETTriggerOneLep;
  bool m_useLeptonTrigger;
  bool m_useLargeRJets;
  bool m_doCutFlow;
  bool m_doOneLeptonAna;
  bool m_doTwoLeptonAna;
  bool m_doZeroLeptonAna;

  bool m_doPreselection, m_doExclusiveJetRegions, m_doExtendedPreselection, m_doSingleVLQRegions, m_doPairVLQRegions,
    m_doValidnRegions, m_doFitRegions, m_doLooseSystRegions, m_doLowBRegions, m_doLowJRegions,
    m_doSplitEMu, m_doOldBoost, m_doSplitMtb, m_doSplitMbb;
  bool m_doSumRegions;

  bool m_scaleTtbarHtSlices;
  bool m_applyTtbarNNLOCorrection;
  bool m_recomputeTtbarNNLOCorrection;
  bool m_applyVjetsSherpa22RW;
  bool m_computeTtccNLO;

  bool m_reweightKinematics;
  bool m_onlyReweightTtbarKin;

  bool m_makeQCD0LSystematics;
  bool m_doPreselSys;
  bool m_doExpSys;
  bool m_doTheorySys;

  int m_maxb;
  int m_RCNsubjetsCut;
  double m_RCJetsPtCut;
  double m_jetsPtCut;
  double m_jetsEtaCut;
  double m_fwdJetsPtCut;
  double m_fwdJetsEtaCut;
  double m_trkJetsPtCut;
  double m_lepPtCut;
  double m_mtbminCut; 
  double m_minDeltaPhiCut;
  double m_maxDeltaPhiCut;
  double m_minMeffCut;
  double m_maxMeffCut;
  double m_minMetCutOneLep;
  double m_maxMetCutOneLep;
  double m_minMetCutZeroLep;
  double m_maxMetCutZeroLep;
  double m_maxLeptopDR;

  std::string m_doRecoVLQ;
  std::string m_btagOP;
  std::string m_btagAlg;
  std::string m_TRFCDIPath;
  std::string m_sampleDat;
  std::string m_lepWOpt;
  std::string m_leptopOpt;
  std::string m_RCCollection;

  FilteringType m_filterType;
  BtagColl m_btagCollection;
};

#endif //VLQ_OPTIONS_H
