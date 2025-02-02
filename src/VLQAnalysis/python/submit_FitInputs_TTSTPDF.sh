#!/bin/bash

for campaign in "mc16a" "mc16d" "mc16e"
do
    python Submit_VLQAnalysis_new.py dryrun=false campaign=${campaign} --sleep=2 \
	producetarball=true \
        --inputDir=/nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.121-htztx-3-syst-TTPDF/ \
        --sampleDat=samples_info.tag-21.2.121-htztx-3-syst-TTPDF.${campaign}.dat \
	--outputDirSuffix=FitInputs_RWPARAM_TTSTPDF_${campaign}_NOW \
	--reweightKinematics=true --kinRWList=JETSN,MEFFRED --doKinRWSmoothing=TRUE \
	--queue=at3 --NFILESPLIT=200 --NMERGE=1 --removeNull=TRUE \
	--runData=false --runQCD=false --runSignals=false --runTtbar=true --runOtherBkgd=true \
	--runSingleTop=true --runWjets=false --runZjets=false --runTopEW=false --runDibosons=false \
	--runTtSyst=false --runStSyst=false --splitSTChannels=true \
	--dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=false --splitVLQDecays=false \
	--otherVariables=false --doBlind=false \
	--useObjectSyst=false --useWeightSyst=true --onlyDumpSystHistograms=true \
	--doExpSys=false --doTheorySys=false --doPDFSys=true \
	--doOneLeptonAna=true --doTwoLeptonAna=false --doZeroLeptonAna=false \
	--doExclusiveJetRegions=false --doLowBRegions=false --doLowJRegions=false --doSplitEMu=false --doSplitMtb=false \
	--doFitRegions=true --doValidnRegions=true --doPreselection=true --doPreselSys=true \
	--doExtendedPreselection=false --doLooseSystRegions=true --deriveReweighting=false \
	--doSingleVLQRegions=true --doPairVLQRegions=false \
	--doOldBoost=false \
	--useLeptonTrigger=true --useMETTriggerOneLep=true --useMETTrigger=true \
	--applyMetMtwCuts=true --invertMetMtwCuts=false \
	--applydeltaphicut=true --minDeltaPhiCut=0.4 --maxDeltaPhiCut=-1. \
	--doTRF=false --recomputeTRF=false --recomputeBTagSF=false --btagOP=FixedCutBEff_77 \
	--TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
	--btagCollection=TRACK --trkJetPtCut=20. \
	--RWTTFRACTIONS=true \
	--applyTtbarNNLOCorrection=false --applyVJetsSherpa22RW=false --applyTtbbCorrection=false \
	--filterType=APPLYFILTER --useSlices=true --scaleTtbarHtSlices=true \
	--jetPtCut=25 --fwdJetPtCut=20 --RCJetPtCut=200 --RCNsubjetsCut=0 \
	--leptopOpt=VETO_RCMATCH --maxLeptopDR=1.0 --minMeffCut=600 --RCCollection=VR_rho550

    
done
