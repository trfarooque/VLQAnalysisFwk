#include "VLQAnalysis/VLQ_Options.h"

#include "IFAETopFramework/AnalysisUtils.h"
#include "IFAETopFramework/OptionsBase.h"

#include <iostream>
#include <string>
#include <stdexcept> // invalid_argument

using std::string;
using std::logic_error;

//______________________________________________________________________________
//
VLQ_Options::VLQ_Options():
OptionsBase(),
m_useLeptonsSF(true),
m_useBtagSF(true),
m_recomputeBtagSF(false),
m_usePileUpWeight(true),
m_dumpHistos(true),
m_dumpTree(false),
m_dumpOverlapTree(false),
m_splitVLQDecays(false),
m_doTruthAnalysis(false),
m_doTRF(false),
m_recomputeTRF(false),
m_applyMetMtwCuts(true),
m_invertMetMtwCuts(false),
m_applyDeltaPhiCut(true),
m_invertDeltaPhiCut(false),
m_applyMetSigObjCut(false),
m_applyMetSignificanceCut(false),
m_applyTtbbCorrection(false),
m_multipleVariablesWithUncertainties(false),
m_verboseOutput(false),
m_RecTtBbRw(false),
m_RwTtFractions(false),
m_useMETTrigger(false),
m_useMETTriggerOneLep(false),
m_useLeptonTrigger(false),
m_useLargeRJets(false),
m_doCutFlow(true),
m_doOneLeptonAna(false),
m_doTwoLeptonAna(false),
m_doZeroLeptonAna(false),
m_isAFII(false),
m_isDiagSub(false),
m_doPreselection(true),
m_doExclusiveJetRegions(false),
m_doExtendedPreselection(false),
m_doSingleVLQRegions(true),
m_doPairVLQRegions(false),
m_doOldPairProdRegions(false),
m_doValidnRegions(false),
m_doFitRegions(true),
m_doLooseSystRegions(true),
m_doLowBRegions(false),
m_doLowJRegions(false),
m_doSplitEMu(false),
m_doOldBoost(false),
m_doSplitMbb(false),
m_doSumRegions(false),
m_scaleTtbarHtSlices(false),
m_applyTtbarNNLOCorrection(false),
m_recomputeTtbarNNLOCorrection(false),
m_applyVjetsSherpa22RW(true),
m_computeTtccNLO(false),
m_deriveReweighting(false),
m_reweightKinematics(false),
m_doKinRwSmoothing(false),
m_doKinRwSyst(false),
m_doFJvtSFWeights(false),
m_makeQCD0LSystematics(false),
m_doPreselSys(false),
m_doExpSys(true),
m_doTheorySys(true),
m_doPDFSys(true),
m_doJMRSys(false),
m_doJMSSys(0),
m_maxb(4),
m_RCNsubjetsCut(2),
m_RCJetsPtCut(300.),
m_jetsPtCut(25.),
m_jetsEtaCut(2.5),
m_fwdJetsPtCut(40.),
m_fwdJetsEtaCut(4.5),
m_trkJetsPtCut(10.),
m_lepPtCut(28.),
m_mtbminCut(160.),
m_minDeltaPhiCut(0.4),
m_maxDeltaPhiCut(-1.),
m_minMeffCut(0.),
m_maxMeffCut(-1.),
m_minMetCutOneLep(0.),
m_maxMetCutOneLep(-1.),
m_minMetCutZeroLep(200.),
m_maxMetCutZeroLep(-1.),
m_maxMetCutTwoLep(-1.),
m_maxLeptopDR(100.),
m_doRecoVLQ("pair"),
// m_btagOP("77"),
m_btagOP(""), // temp
m_btagAlg("MV2c10"),
m_TRFCDIPath("xAODBTaggingEfficiency/13TeV/2016-20_7-13TeV-MC15-CDI-2017-01-31_v1.root"),
m_sampleDat("samples_info_MBJ-2.4.24-1-0.dat"),
m_lepWOpt("VANILLA"),
m_leptopOpt("VETO_RCMATCH"),
m_RCCollection("VR_rho550"),
m_filterType(NOFILTER),
m_btagCollection(CALOPFLOW)
{}

//_____________________________________________________________
//
VLQ_Options::VLQ_Options( const VLQ_Options &q ):
OptionsBase(q)
{
    m_useLeptonsSF       = q.m_useLeptonsSF;
    m_useBtagSF          = q.m_useBtagSF;
    m_recomputeBtagSF    = q.m_recomputeBtagSF;
    m_usePileUpWeight    = q.m_usePileUpWeight;
    m_dumpHistos         = q.m_dumpHistos;
    m_dumpTree           = q.m_dumpTree;
    m_dumpOverlapTree    = q.m_dumpOverlapTree;
    m_splitVLQDecays     = q.m_splitVLQDecays;
    m_doTruthAnalysis    = q.m_doTruthAnalysis;
    m_doTRF              = q.m_doTRF;
    m_recomputeTRF       = q.m_recomputeTRF;
    m_applyMetMtwCuts    = q.m_applyMetMtwCuts;
    m_invertMetMtwCuts   = q.m_invertMetMtwCuts;
    m_applyDeltaPhiCut   = q.m_applyDeltaPhiCut;
    m_invertDeltaPhiCut  = q.m_invertDeltaPhiCut;
    m_applyMetSigObjCut  = q.m_applyMetSigObjCut;
    m_applyMetSignificanceCut            = q.m_applyMetSignificanceCut;
    m_applyTtbbCorrection                = q.m_applyTtbbCorrection;
    m_multipleVariablesWithUncertainties = q.m_multipleVariablesWithUncertainties;
    m_verboseOutput       = q.m_verboseOutput;
    m_useMETTrigger       = q.m_useMETTrigger;
    m_useMETTriggerOneLep = q.m_useMETTriggerOneLep;
    m_useLeptonTrigger    = q.m_useLeptonTrigger;
    m_useLargeRJets       = q.m_useLargeRJets;
    m_doCutFlow           = q.m_doCutFlow;
    m_doOneLeptonAna      = q.m_doOneLeptonAna;
    m_doTwoLeptonAna      = q.m_doTwoLeptonAna;
    m_doZeroLeptonAna     = q.m_doZeroLeptonAna;
    m_doPreselection      = q.m_doPreselection;
    m_doExclusiveJetRegions    = q.m_doExclusiveJetRegions;
    m_isAFII              = q.m_isAFII;
    m_isDiagSub           = q.m_isDiagSub;
    m_doExtendedPreselection   = q.m_doExtendedPreselection;
    m_doSingleVLQRegions  = q.m_doSingleVLQRegions;
    m_doPairVLQRegions    = q.m_doPairVLQRegions;
    m_doOldPairProdRegions = q.m_doOldPairProdRegions;
    m_doValidnRegions     = q.m_doValidnRegions;
    m_doFitRegions        = q.m_doFitRegions;
    m_doLooseSystRegions  = q.m_doLooseSystRegions;
    m_doLowBRegions       = q.m_doLowBRegions;
    m_doLowJRegions       = q.m_doLowJRegions;
    m_doSplitEMu          = q.m_doSplitEMu;
    m_doOldBoost          = q.m_doOldBoost;
    m_doSplitMtb          = q.m_doSplitMtb;
    m_doSplitMbb          = q.m_doSplitMbb;
    m_doSumRegions        = q.m_doSumRegions;
    m_scaleTtbarHtSlices  = q.m_scaleTtbarHtSlices;
    m_applyTtbarNNLOCorrection  = q.m_applyTtbarNNLOCorrection;
    m_recomputeTtbarNNLOCorrection  = q.m_recomputeTtbarNNLOCorrection;
    m_applyVjetsSherpa22RW = q.m_applyVjetsSherpa22RW;
    m_computeTtccNLO     = q.m_computeTtccNLO;
    m_deriveReweighting  = q.m_deriveReweighting;
    m_reweightKinematics = q.m_reweightKinematics;
    m_doKinRwSmoothing = q.m_doKinRwSmoothing;
    m_doKinRwSyst       = q.m_doKinRwSyst;
    m_doFJvtSFWeights   = q.m_doFJvtSFWeights;
    m_makeQCD0LSystematics = q.m_makeQCD0LSystematics;
    m_doPreselSys       = q.m_doPreselSys;
    m_doExpSys          = q.m_doExpSys;
    m_doTheorySys       = q.m_doTheorySys;
    m_doPDFSys          = q.m_doPDFSys;
    m_doJMRSys          = q.m_doJMRSys;
    m_doJMSSys          = q.m_doJMSSys;
    m_RecTtBbRw         = q.m_RecTtBbRw;
    m_RwTtFractions     = q.m_RwTtFractions;
    m_RCNsubjetsCut     = q.m_RCNsubjetsCut;
    m_RCJetsPtCut       = q.m_RCJetsPtCut;
    m_jetsPtCut         = q.m_jetsPtCut;
    m_jetsEtaCut        = q.m_jetsEtaCut;
    m_fwdJetsPtCut      = q.m_fwdJetsPtCut;
    m_fwdJetsEtaCut     = q.m_fwdJetsEtaCut;
    m_trkJetsPtCut      = q.m_trkJetsPtCut;
    m_lepPtCut          = q.m_lepPtCut;
    m_mtbminCut         = q.m_mtbminCut;
    m_minDeltaPhiCut    = q.m_minDeltaPhiCut;
    m_maxDeltaPhiCut    = q.m_maxDeltaPhiCut;
    m_minMeffCut        = q.m_minMeffCut;
    m_maxMeffCut        = q.m_maxMeffCut;
    m_minMetCutOneLep   = q.m_minMetCutOneLep;
    m_maxMetCutOneLep   = q.m_maxMetCutOneLep;
    m_minMetCutZeroLep  = q.m_minMetCutZeroLep;
    m_maxMetCutZeroLep  = q.m_maxMetCutZeroLep;
    m_maxMetCutTwoLep   = q.m_maxMetCutTwoLep;
    m_maxLeptopDR        = q.m_maxLeptopDR;
    m_doRecoVLQ         = q.m_doRecoVLQ;
    m_btagOP            = q.m_btagOP;
    m_btagAlg           = q.m_btagAlg;
    m_TRFCDIPath        = q.m_TRFCDIPath;
    m_maxb              = q.m_maxb;
    m_sampleDat         = q.m_sampleDat;
    m_lepWOpt           = q.m_lepWOpt;
    m_leptopOpt         = q.m_leptopOpt;
    m_RCCollection      = q.m_RCCollection;
    m_kinRWList         = q.m_kinRWList;
    m_filterType        = q.m_filterType;
    m_btagCollection    = q.m_btagCollection;
}

//_____________________________________________________________
//
VLQ_Options::~VLQ_Options()
{}

//_____________________________________________________________
//
bool VLQ_Options::IdentifyOption ( const std::string &argument, const std::string &value ) {

    std::string temp_arg = argument;
    std::string temp_val = value;

    if(!OptionsBase::IdentifyOption(argument, value)){

        //
        // Boolean arguments
        //
        if( temp_arg.find("--USELEPTONSSF") != std::string::npos ){
            m_useLeptonsSF = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--USEBTAGGINGSF") != std::string::npos ){
	         m_useBtagSF = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--RECOMPUTEBTAGSF") != std::string::npos ){
            m_recomputeBtagSF = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--USEPUWEIGHT") != std::string::npos ){
            m_usePileUpWeight = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DUMPHISTOS") != std::string::npos ){
            m_dumpHistos = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DUMPTREE") != std::string::npos ){
            m_dumpTree = AnalysisUtils::BoolValue(temp_val, temp_arg);
	} else if( temp_arg.find("--DUMPOVERLAPTREE") != std::string::npos ){
            m_dumpOverlapTree = AnalysisUtils::BoolValue(temp_val, temp_arg);
        }  else if( temp_arg.find("--SPLITVLQDECAYS") != std::string::npos ){
	    m_splitVLQDecays = AnalysisUtils::BoolValue(temp_val, temp_arg);
        }  else if( temp_arg.find("--DOTRUTHANALYSIS") != std::string::npos ){
            m_doTruthAnalysis = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOTRF") != std::string::npos ){
            m_doTRF = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--RECOMPUTETRF") != std::string::npos ){
            m_recomputeTRF = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--APPLYMETMTWCUTS") != std::string::npos ){
            m_applyMetMtwCuts = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--INVERTMETMTWCUTS") != std::string::npos ){
            m_invertMetMtwCuts = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--APPLYDELTAPHICUT") != std::string::npos ){
            m_applyDeltaPhiCut = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--INVERTDELTAPHICUT") != std::string::npos ){
            m_invertDeltaPhiCut = AnalysisUtils::BoolValue(temp_val, temp_arg);
	} else if( temp_arg.find("--APPLYMETSIGOBJCUT") != std::string::npos ){
	    m_applyMetSigObjCut = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--APPLYMETSIGCUT") != std::string::npos ){
            m_applyMetSignificanceCut = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--APPLYTTBBCORRECTION") != std::string::npos ){
            m_applyTtbbCorrection = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--RECOMPUTETTBBRW") != std::string::npos ){
            m_RecTtBbRw = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--RWTTFRACTIONS") != std::string::npos ){
            m_RwTtFractions = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--OTHERVARIABLES") != std::string::npos ){
            m_multipleVariablesWithUncertainties = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--VERBOSEOUTPUT") != std::string::npos ){
            m_verboseOutput = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--USEMETTRIGGERONELEP") != std::string::npos ){
            m_useMETTriggerOneLep = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--USEMETTRIGGER") != std::string::npos ){
            m_useMETTrigger = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--USELEPTONTRIGGER") != std::string::npos ){
            m_useLeptonTrigger = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--USELARGERJETS") != std::string::npos ){
            m_useLargeRJets = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOCUTFLOW") != std::string::npos ){
            m_doCutFlow     = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOONELEPTONANA") != std::string::npos ){
            m_doOneLeptonAna  = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOTWOLEPTONANA") != std::string::npos ){
            m_doTwoLeptonAna  = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOZEROLEPTONANA") != std::string::npos ){
            m_doZeroLeptonAna = AnalysisUtils::BoolValue(temp_val, temp_arg);
         } else if( temp_arg.find("--DOPRESELECTION") != std::string::npos ){
             m_doPreselection = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOEXCLUSIVEJETREGIONS") != std::string::npos ){
            m_doExclusiveJetRegions = AnalysisUtils::BoolValue(temp_val, temp_arg);
	} else if( temp_arg.find("--ISAFII") != std::string::npos ){
	    m_isAFII = AnalysisUtils::BoolValue(temp_val, temp_arg);
	} else if( temp_arg.find("--ISDIAGSUB") != std::string::npos ){
	    m_isDiagSub = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOEXTENDEDPRESELECTION") != std::string::npos ){
            m_doExtendedPreselection = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOSINGLEVLQREGIONS") != std::string::npos ){
            m_doSingleVLQRegions = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOPAIRVLQREGIONS") != std::string::npos ){
            m_doPairVLQRegions = AnalysisUtils::BoolValue(temp_val, temp_arg);
	} else if( temp_arg.find("--DOOLDPAIRPRODREGIONS") != std::string::npos ){
	    m_doOldPairProdRegions = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOVALIDNREGIONS") != std::string::npos ){
            m_doValidnRegions = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOFITREGIONS") != std::string::npos ){
            m_doFitRegions = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOLOOSESYSTREGIONS") != std::string::npos ){
            m_doLooseSystRegions = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOLOWBREGIONS") != std::string::npos ){
            m_doLowBRegions = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOLOWJREGIONS") != std::string::npos ){
            m_doLowJRegions = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOSPLITEMU") != std::string::npos ){
            m_doSplitEMu = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOOLDBOOST") != std::string::npos ){
            m_doOldBoost = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOSPLITMTB") != std::string::npos ){
            m_doSplitMtb = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOSPLITMBB") != std::string::npos ){
            m_doSplitMbb = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOSUMREGIONS") != std::string::npos ){
            m_doSumRegions = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--SCALETTBARHTSLICES") != std::string::npos ){
            m_scaleTtbarHtSlices = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--APPLYTTBARNNLOCORRECTION") != std::string::npos ){
            m_applyTtbarNNLOCorrection = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--RECOMPUTETTBARNNLOCORRECTION") != std::string::npos ){
            m_recomputeTtbarNNLOCorrection = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--APPLYVJETSSHERPA22RW") != std::string::npos ){
            m_applyVjetsSherpa22RW = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--COMPUTETTCCNLO") != std::string::npos ){
            m_computeTtccNLO = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DERIVEREWEIGHTING") != std::string::npos ){
	    m_deriveReweighting = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--REWEIGHTKINEMATICS") != std::string::npos ){
	    m_reweightKinematics = AnalysisUtils::BoolValue(temp_val, temp_arg);
	} else if( temp_arg.find("--DOKINRWSMOOTHING") != std::string::npos ){
	    m_doKinRwSmoothing = AnalysisUtils::BoolValue(temp_val, temp_arg);
	} else if ( temp_arg.find("--DOKINRWSYST") != std::string::npos ){
	    m_doKinRwSyst = AnalysisUtils::BoolValue(temp_val, temp_arg);
	} else if ( temp_arg.find("--DOFJVTSFWEIGHTS") != std::string::npos ){
	    m_doFJvtSFWeights = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--MAKEQCD0LSYSTEMATICS") != std::string::npos ){
            m_makeQCD0LSystematics = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOPRESELSYS") != std::string::npos ){
            m_doPreselSys = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOEXPSYS") != std::string::npos ){
            m_doExpSys = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOTHEORYSYS") != std::string::npos ){
            m_doTheorySys = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOPDFSYS") != std::string::npos ){
            m_doPDFSys = AnalysisUtils::BoolValue(temp_val, temp_arg);
        } else if( temp_arg.find("--DOJMRSYS") != std::string::npos ){
            m_doJMRSys = AnalysisUtils::BoolValue(temp_val, temp_arg);
        }

        //
        // Float arguments
        //
        else if( temp_arg.find("--RCJETPTCUT") != std::string::npos ){
            m_RCJetsPtCut = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--JETPTCUT") != std::string::npos ){
            m_jetsPtCut = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--JETETACUT") != std::string::npos ){
            m_jetsEtaCut = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--FWDJETPTCUT") != std::string::npos ){
            m_fwdJetsPtCut = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--FWDJETETACUT") != std::string::npos ){
            m_fwdJetsEtaCut = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--TRKJETPTCUT") != std::string::npos ){
            m_trkJetsPtCut = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--LEPPTCUT") != std::string::npos ){
            m_lepPtCut = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--MTBMINCUT") != std::string::npos ){
            m_mtbminCut = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--MINDELTAPHICUT") != std::string::npos ){
	  m_minDeltaPhiCut = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--MAXDELTAPHICUT") != std::string::npos ){
	  m_maxDeltaPhiCut = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--MINMEFFCUT") != std::string::npos ){
	  m_minMeffCut = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--MAXMEFFCUT") != std::string::npos ){
	  m_maxMeffCut = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--MINMETCUTONELEP") != std::string::npos ){
	  m_minMetCutOneLep = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--MAXMETCUTONELEP") != std::string::npos ){
	  m_maxMetCutOneLep = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--MINMETCUTZEROLEP") != std::string::npos ){
	  m_minMetCutZeroLep = atof(temp_val.c_str());
        }
        else if( temp_arg.find("--MAXMETCUTZEROLEP") != std::string::npos ){
	  m_maxMetCutZeroLep = atof(temp_val.c_str());
        }
	else if( temp_arg.find("--MAXMETCUTTWOLEP") != std::string::npos ){
	  m_maxMetCutTwoLep = atof(temp_val.c_str());
	}
	else if( temp_arg.find("--MAXLEPTOPDR") != std::string::npos ){
	  m_maxLeptopDR = atof(temp_val.c_str());
        }
        //
        // String arguments
        //

        else if( temp_arg.find("--DORECOVLQ") != std::string::npos ){
            m_doRecoVLQ = temp_val;
        } else if( temp_arg.find("--BTAGOP") != std::string::npos ){
            m_btagOP = temp_val;
        } else if( temp_arg.find("--BTAGALG") != std::string::npos ){
            m_btagAlg = temp_val;
        } else if( temp_arg.find("--TRFCDI") != std::string::npos ){
            m_TRFCDIPath = temp_val;
        } else if( temp_arg.find("--SAMPLEDAT") != std::string::npos ){
            m_sampleDat = temp_val;
        } else if( temp_arg.find("--LEPWOPT") != std::string::npos ){
            m_lepWOpt = temp_val;
        } else if( temp_arg.find("--LEPTOPOPT") != std::string::npos ){
            m_leptopOpt = temp_val;
        } else if( temp_arg.find("--RCCOLLECTION") != std::string::npos ){
            m_RCCollection = temp_val;
        } else if( temp_arg.find("--KINRWLIST") != std::string::npos ){
	    m_kinRWList = temp_val;
        }
        //
        // Int arguments
        //
        else if( temp_arg.find("--DOJMSSYS") != std::string::npos ){
          m_doJMSSys = atoi(temp_val.c_str());
        }
        else if( temp_arg.find("--MAXTRFB") != std::string::npos ){
            m_maxb = atoi(temp_val.c_str());
        }
        else if( temp_arg.find("--RCNSUBJETSCUT") != std::string::npos ){
            m_RCNsubjetsCut = atoi(temp_val.c_str());
        }
        //
        // Enum arguments
        //
        else if( temp_arg.find("--FILTERTYPE") != std::string::npos ){
            std::transform(temp_val.begin(), temp_val.end(), temp_val.begin(), toupper);
            if ( temp_val.find("NOFILTER") != std::string::npos) 	m_filterType = NOFILTER;
            else if ( temp_val.find("APPLYFILTER") != std::string::npos) 	m_filterType = APPLYFILTER;
            else {
                std::cout<<"Unknown FILTERTYPE option : " << temp_val << std::endl;
            }
        }
        else if( temp_arg.find("--BTAGCOLLECTION") != std::string::npos ){
            std::transform(temp_val.begin(), temp_val.end(), temp_val.begin(), toupper);
            if ( temp_val.find("CALOPFLOW") != std::string::npos) 	m_btagCollection = CALOPFLOW;
            else if ( temp_val.find("CALOTOPO") != std::string::npos) 	m_btagCollection = CALOTOPO;
            else if ( temp_val.find("TRACK") != std::string::npos) 	m_btagCollection = TRACK;
            else {
                std::cout<<"Unknown BTAGCOLLECTION option : " << temp_val << std::endl;
            }
        } else {
            return false;
        }
    }
    return true;
}

//_____________________________________________________________
//
void VLQ_Options::PrintOptions(){
    OptionsBase::PrintOptions();
    std::cout << "============== VLQ_Options ================="        << std::endl;
    std::cout << " m_jetsPtCut               = " << m_jetsPtCut         << std::endl;
    std::cout << " m_jetsEtaCut              = " << m_jetsEtaCut        << std::endl;
    std::cout << " m_fwdJetsPtCut            = " << m_fwdJetsPtCut      << std::endl;
    std::cout << " m_fwdJetsEtaCut           = " << m_fwdJetsEtaCut     << std::endl;
    std::cout << " m_trkJetsPtCut            = " << m_trkJetsPtCut      << std::endl;
    std::cout << " m_lepPtCut                = " << m_lepPtCut          << std::endl;
    std::cout << " m_mtbminCut               = " << m_mtbminCut         << std::endl;
    std::cout << " m_minDeltaPhiCut          = " << m_minDeltaPhiCut    << std::endl;
    std::cout << " m_maxDeltaPhiCut          = " << m_maxDeltaPhiCut    << std::endl;
    std::cout << " m_minMeffCut              = " << m_minMeffCut        << std::endl;
    std::cout << " m_maxMeffCut              = " << m_maxMeffCut        << std::endl;
    std::cout << " m_minMetCutOneLep         = " << m_minMetCutOneLep   << std::endl;
    std::cout << " m_maxMetCutOneLep         = " << m_maxMetCutOneLep   << std::endl;
    std::cout << " m_minMetCutZeroLep        = " << m_minMetCutZeroLep  << std::endl;
    std::cout << " m_maxMetCutZeroLep        = " << m_maxMetCutZeroLep  << std::endl;
    std::cout << " m_maxMetCutTwoLep         = " << m_maxMetCutTwoLep   << std::endl;
    std::cout << " m_maxLeptopDR             = " << m_maxLeptopDR       << std::endl;
    std::cout << " m_doRecoVLQ               = " << m_doRecoVLQ         << std::endl;
    std::cout << " m_btagOP                  = " << m_btagOP            << std::endl;
    std::cout << " m_btagAlg                 = " << m_btagAlg           << std::endl;
    std::cout << " m_btagCollection          = " << m_btagCollection    << std::endl;
    std::cout << " m_useLeptonsSF            = " << m_useLeptonsSF      << std::endl;
    std::cout << " m_useBtagSF               = " << m_useBtagSF         << std::endl;
    std::cout << " m_recomputeBtagSF         = " << m_recomputeBtagSF   << std::endl;
    std::cout << " m_usePileUpWeight         = " << m_usePileUpWeight   << std::endl;
    std::cout << " m_splitVLQDecays          = " << m_splitVLQDecays    << std::endl;
    std::cout << " m_doTruthAnalysis         = " << m_doTruthAnalysis   << std::endl;
    std::cout << " m_doTRF                   = " << m_doTRF             << std::endl;
    std::cout << " m_recomputeTRF            = " << m_recomputeTRF      << std::endl;
    std::cout << " m_TRFCDIPath              = " << m_TRFCDIPath        << std::endl;
    std::cout << " m_maxb                    = " << m_maxb              << std::endl;
    std::cout << " m_RCNsubjetsCut           = " << m_RCNsubjetsCut     << std::endl;
    std::cout << " m_RCJetsPtCut             = " << m_RCJetsPtCut       << std::endl;
    std::cout << " m_applyMetMtwCuts         = " << m_applyMetMtwCuts   << std::endl;
    std::cout << " m_invertMetMtwCuts        = " << m_invertMetMtwCuts   << std::endl;
    std::cout << " m_applyDeltaPhiCut        = " << m_applyDeltaPhiCut  << std::endl;
    std::cout << " m_invertDeltaPhiCut       = " << m_invertDeltaPhiCut  << std::endl;
    std::cout << " m_applyMetSigObjCut       = " << m_applyMetSigObjCut << std::endl;
    std::cout << " m_applyMetSignificanceCut = " << m_applyMetSignificanceCut  << std::endl;
    std::cout << " m_dumpHistos              = " << m_dumpHistos        << std::endl;
    std::cout << " m_dumpTree                = " << m_dumpTree          << std::endl;
    std::cout << " m_dumpOverlapTree         = " << m_dumpOverlapTree   << std::endl;
    std::cout << " m_applyTtbbCorrection     = " << m_applyTtbbCorrection << std::endl;
    std::cout << " m_multipleVariablesWithUncertainties     = " << m_multipleVariablesWithUncertainties << std::endl;
    std::cout << " m_verboseOutout           = " << m_verboseOutput << std::endl;
    std::cout << " m_useLeptonTrigger        = " << m_useLeptonTrigger  << std::endl;
    std::cout << " m_useLargeRJets           = " << m_useLargeRJets     << std::endl;
    std::cout << " m_useMETTrigger           = " << m_useMETTrigger     << std::endl;
    std::cout << " m_useMETTriggerOneLep     = " << m_useMETTriggerOneLep<< std::endl;
    std::cout << " m_doCutFlow               = " << m_doCutFlow         << std::endl;
    std::cout << " m_doOneLeptonAna          = " << m_doOneLeptonAna    << std::endl;
    std::cout << " m_doTwoLeptonAna          = " << m_doTwoLeptonAna    << std::endl;
    std::cout << " m_doZeroLeptonAna         = " << m_doZeroLeptonAna   << std::endl;
    std::cout << " m_doPreselection          = " << m_doPreselection    << std::endl;
    std::cout << " m_doExclusiveJetRegions   = " << m_doExclusiveJetRegions << std::endl;
    std::cout << " m_isAFII                  = " << m_isAFII << std::endl;
    std::cout << " m_isDiagSub               = " << m_isDiagSub << std::endl;
    std::cout << " m_doExtendedPreselection  = " << m_doExtendedPreselection << std::endl;
    std::cout << " m_doSingleVLQRegions      = " << m_doSingleVLQRegions<< std::endl;
    std::cout << " m_doPairVLQRegions        = " << m_doPairVLQRegions  << std::endl;
    std::cout << " m_doOldPairProdRegions    = " << m_doOldPairProdRegions << std::endl;
    std::cout << " m_doValidnRegions         = " << m_doValidnRegions   << std::endl;
    std::cout << " m_doFitRegions            = " << m_doFitRegions      << std::endl;
    std::cout << " m_doLooseSystRegions      = " << m_doLooseSystRegions<< std::endl;
    std::cout << " m_doLowBRegions           = " << m_doLowBRegions     << std::endl;
    std::cout << " m_doLowJRegions           = " << m_doLowJRegions     << std::endl;
    std::cout << " m_doSplitEMu              = " << m_doSplitEMu        << std::endl;
    std::cout << " m_doOldBoost              = " << m_doOldBoost        << std::endl;
    std::cout << " m_doSplitMtb              = " << m_doSplitMtb        << std::endl;
    std::cout << " m_doSplitMbb              = " << m_doSplitMbb        << std::endl;
    std::cout << " m_doSumRegions            = " << m_doSumRegions      << std::endl;
    std::cout << " m_scaleTtbarHtSlices      = " << m_scaleTtbarHtSlices << std::endl;
    std::cout << " m_applyTtbarNNLOCorrection= " << m_applyTtbarNNLOCorrection   << std::endl;
    std::cout << " m_recomputeTtbarNNLOCorrection= " << m_recomputeTtbarNNLOCorrection   << std::endl;
    std::cout << " m_deriveReweighting       = " << m_deriveReweighting << std::endl;
    std::cout << " m_reweightKinematics      = " << m_reweightKinematics << std::endl;
    std::cout << " m_doKinRwSmoothing        = " << m_doKinRwSmoothing  << std::endl;
    std::cout << " m_doKinRwSyst             = " << m_doKinRwSyst       << std::endl;
    std::cout << " m_doFJvtSFWeights         = " << m_doFJvtSFWeights   << std::endl;

    std::cout << " m_makeQCD0LSystematics    = " << m_makeQCD0LSystematics << std::endl;
    std::cout << " m_doPreselSys             = " << m_doPreselSys       << std::endl;
    std::cout << " m_doExpSys                = " << m_doExpSys          << std::endl;
    std::cout << " m_doTheorySys             = " << m_doTheorySys       << std::endl;
    std::cout << " m_doPDFSys                = " << m_doPDFSys          << std::endl;
    std::cout << " m_doJMRSys                = " << m_doJMRSys          << std::endl;
    std::cout << " m_doJMSSys                = " << m_doJMSSys          << std::endl;
    std::cout << " m_applyVjetsSherpa22RW    = " << m_applyVjetsSherpa22RW   << std::endl;
    std::cout << " m_filterType              = " << m_filterType        << std::endl;
    std::cout << " m_RecTtBbRw               = " << m_RecTtBbRw         << std::endl;
    std::cout << " m_RwTtFractions           = " << m_RwTtFractions     << std::endl;
    std::cout << " m_sampleDat               = " << m_sampleDat         << std::endl;
    std::cout << " m_lepWOpt                 = " << m_lepWOpt           << std::endl;
    std::cout << " m_leptopOpt               = " << m_leptopOpt         << std::endl;
    std::cout << " m_RCCollection            = " << m_RCCollection      << std::endl;
    std::cout << " m_kinRWList               = " << m_kinRWList         << std::endl;
    std::cout << "============================================="        << std::endl;
    std::cout << "" << std::endl;
}

//_____________________________________________________________
//
string const bool2string(bool b)
{
    return b ? "true" : "false";
}

//_____________________________________________________________
//
void VLQ_Options::checkConcistency() const
{
 if(!m_useMETTrigger && !m_useLeptonTrigger && !m_useMETTriggerOneLep)
    throw logic_error(string(__FILE__)+"\n"
                      +" choose either one of the two triggers:"
                      +" useMETTrigger "+bool2string(m_useMETTrigger)+","
                      +" useLeptonTrigger "+bool2string(m_useLeptonTrigger)+","
                      +" useMETTriggerOneLep "+bool2string(m_useMETTriggerOneLep));
  if(m_doOneLeptonAna==false and m_doZeroLeptonAna==false and m_doTwoLeptonAna==false)
    throw logic_error(string(__FILE__)+"\n"
                      +" choose at least one of the three analyses:"
                      +" doOneLeptonAna "+bool2string(m_doOneLeptonAna)+","
                      +" doZeroLeptonAna "+bool2string(m_doZeroLeptonAna)
                      +" doTwoLeptonAna "+bool2string(m_doTwoLeptonAna));
  /*if( (m_doOneLeptonAna==true or m_doZeroLeptonAna==true) and m_doTwoLeptonAna==true)
    throw logic_error(string(__FILE__)+"\n"
                      +" Cannot run dilepton channel together with 0-lep or 1-lep channels:"
                      +" doOneLeptonAna "+bool2string(m_doOneLeptonAna)+","
                      +" doZeroLeptonAna "+bool2string(m_doZeroLeptonAna)
                      +" doTwoLeptonAna "+bool2string(m_doTwoLeptonAna));*/
  // TODO implement other checks
}
