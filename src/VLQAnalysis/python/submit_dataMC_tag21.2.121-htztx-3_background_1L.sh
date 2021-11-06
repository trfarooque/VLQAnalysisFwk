#!/bin/bash

for campaign in "mc16a" "mc16d" "mc16e"
do
    python Submit_VLQAnalysis_new.py dryrun=false campaign=${campaign} --sleep=2 \
	producetarball=true \
	--inputDir=/data/at3/scratch2/farooque/MBJOutputs/tag-21.2.87-htztx-3-syst/nominal/ \
        --sampleDat=samples_info.tag-21.2.87-htztx-3-syst.${campaign}.dat \
	--outputDirSuffix=dataMC_TRACK_DL1_FixedCutBEff_77_background_${campaign}_NOW \
	--reweightKinematics=true --kinRWList=JETSN,MEFFRED --doKinRWSmoothing=TRUE --DOKINRWSYST=FALSE \
	--queue=at3 --NFILESPLIT=200 --NMERGE=1 --removeNull=TRUE \
	--runData=true --runQCD=false --runSignals=false --runTtbar=true --runOtherBkgd=true \
	--RUNDIJET=true \
	--runSingleTop=true --runWjets=true --runZjets=true --runTopEW=true --runDibosons=true \
	--runTtSyst=false --runStSyst=false --splitSTChannels=true \
	--dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=false --verboseOutput=true --splitVLQDecays=false \
	--otherVariables=false --doBlind=false \
	--useObjectSyst=false --useWeightSyst=false --onlyDumpSystHistograms=true \
	--doExpSys=false --doTheorySys=false --DOPDFSYS=false \
	--doOneLeptonAna=true --doTwoLeptonAna=true --doZeroLeptonAna=false \
	--doExclusiveJetRegions=false --doLowBRegions=true --doLowJRegions=false --doSplitEMu=false --doSplitMtb=false \
	--doFitRegions=true --doValidnRegions=false --doPreselection=true --doPreselSys=true --doExtendedPreselection=false --doLooseSystRegions=false \
	--doSingleVLQRegions=false --doPairVLQRegions=true --doRecoVLQ=pair --DoOldPairProdRegions=true \
	--doOldBoost=false \
	--useLeptonTrigger=true --useMETTriggerOneLep=true --useMETTrigger=true \
	--applyMetMtwCuts=true --invertMetMtwCuts=false \
	--applydeltaphicut=true --minDeltaPhiCut=0.4 --maxDeltaPhiCut=-1. \
	--doTRF=false --recomputeTRF=false --recomputeBTagSF=false --btagOP=FixedCutBEff_77 \
	--TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
	--btagCollection=TRACK --trkJetPtCut=20. \
	--applyTtbarNNLOCorrection=false --applyVJetsSherpa22RW=false --applyTtbbCorrection=false \
	--filterType=APPLYFILTER --useSlices=true --scaleTtbarHtSlices=true \
	--jetPtCut=25 --fwdJetPtCut=20 --RCJetPtCut=200 --RCNsubjetsCut=0 \
	--leptopOpt=VETO_RCMATCH --maxLeptopDR=1.0 --minMeffCut=1000 --RCCollection=VR_rho550 \
	--APPLYMVA=true --MVAWEIGHTFILE=TMVAClassification_MLP_3Mass.weights.xml
    
done

    #--leptopOpt=VETO_RCMATCH --maxLeptopDR=1.0
