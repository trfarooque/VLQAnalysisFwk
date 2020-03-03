#!/bin/bash

for campaign in "mc16d" #"mc16d" "mc16e"
do
    python Submit_VLQAnalysis_new.py dryrun=false campaign=${campaign} --sleep=2 \
	producetarball=true \
	--inputDir=/nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.87-htztx-2-syst/ \
	--sampleDat=samples_info_tag-21.2.67-0-fJvt-MV2.${campaign}.dat \
	--outputDirSuffix=TestSysts_${campaign}_NOW \
	--queue=at3 --NFILESPLIT=10 --NMERGE=1 \
	--runData=false --runQCD=false --runSignals=false --runTtbar=false --runOtherBkgd=true \
	--runSingleTop=false --runWjets=true --runZjets=false --runTopEW=false --runDibosons=false \
	--runTtSyst=false --runStSyst=false \
	--dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=false --splitVLQDecays=false \
	--otherVariables=false --doBlind=false \
	--useObjectSyst=true --useWeightSyst=true --onlyDumpSystHistograms=true \
	--doOneLeptonAna=true --doTwoLeptonAna=false --doZeroLeptonAna=false \
	--doExclusiveJetRegions=false --doLowBRegions=false --doLowJRegions=false --doSplitEMu=false --doSplitMtb=false \
	--doFitRegions=true --doValidnRegions=true --doPreselection=false --doExtendedPreselection=false \
	--doSingleVLQRegions=true --doPairVLQRegions=false \
	--doOldBoost=false \
	--useLeptonTrigger=true --useMETTriggerOneLep=false --useMETTrigger=true \
	--applyMetMtwCuts=true --invertMetMtwCuts=false \
	--applydeltaphicut=true --minDeltaPhiCut=0.4 --maxDeltaPhiCut=-1. \
	--doTRF=false --recomputeTRF=false --recomputeBTagSF=false --btagOP=FixedCutBEff_77 \
	--TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
	--btagCollection=${btagColl} --trkJetPtCut=20. \
	--applyTtbarNNLOCorrection=false --applyVJetsSherpa22RW=false --applyTtbbCorrection=false \
	--filterType=APPLYFILTER --useSlices=true --scaleTtbarHtSlices=false \
	--jetPtCut=25 --fwdJetPtCut=20 --RCJetPtCut=200 --RCNsubjetsCut=0 \
	--leptopOpt=VETO_RCMATCH --maxLeptopDR=1.0 --minMeffCut=600


    
done

    #--RCCollection=VR_rho300
