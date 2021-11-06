#!/bin/bash

for campaign in "mc16a" "mc16d" "mc16e"
do
    python Submit_VLQAnalysis_new.py dryrun=false campaign=${campaign} --sleep=2 \
	producetarball=true \
	--inputDir=/nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_ntuples/21.1.121-htztx-3-syst_0_lepton/ \
	--sampleDat=samples_info.tag-21.2.121-htztx-3-syst-PAIRVLQ_0L.${campaign}.dat \
	--outputDirSuffix=dataMC_TRACK_DL1_FixedCutBEff_77_signal_${campaign}_NOW \
	--reweightKinematics=false \
	--queue=at3 --NFILESPLIT=200 --NMERGE=1 --removeNull=TRUE \
	--runData=false --runQCD=false --runSignals=true --runTtbar=false --runOtherBkgd=false \
	--RUNDIJET=false \
	--runSingleTop=false --runWjets=false --runZjets=false --runTopEW=false --runDibosons=false \
	--RUNPAIRVLQ=true --RUNSINGLEVLQ=false \
	--runTtSyst=false --runStSyst=false --splitSTChannels=true \
	--dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=false --verboseOutput=false --splitVLQDecays=true \
	--otherVariables=true --doBlind=false \
	--useObjectSyst=false --useWeightSyst=false --onlyDumpSystHistograms=true \
	--doExpSys=false --doTheorySys=false --DOPDFSYS=false \
	--doOneLeptonAna=false --doTwoLeptonAna=false --doZeroLeptonAna=true \
	--doExclusiveJetRegions=false --doLowBRegions=false --doLowJRegions=true --doSplitEMu=false --doSplitMtb=false \
	--doFitRegions=true --doValidnRegions=false --doPreselection=true --doPreselSys=true --doExtendedPreselection=false --doLooseSystRegions=false \
	--doSingleVLQRegions=false --doPairVLQRegions=true --doRecoVLQ=pair --DoOldPairProdRegions=true \
	--doOldBoost=false \
	--useLeptonTrigger=true --useMETTriggerOneLep=true --useMETTrigger=true \
	--applyMetMtwCuts=true --invertMetMtwCuts=false \
	--APPLYMETSIGCUT=true --APPLYMETSIGOBJCUT=false \
	--applydeltaphicut=true --minDeltaPhiCut=0.4 --maxDeltaPhiCut=-1. --INVERTDELTAPHICUT=false \
	--doTRF=false --recomputeTRF=false --recomputeBTagSF=false --btagOP=FixedCutBEff_77 \
	--TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
	--btagCollection=TRACK --trkJetPtCut=20. \
	--applyTtbarNNLOCorrection=false --applyVJetsSherpa22RW=false --applyTtbbCorrection=false \
	--filterType=APPLYFILTER --useSlices=true --scaleTtbarHtSlices=true \
	--jetPtCut=25 --fwdJetPtCut=20 --RCJetPtCut=200 --RCNsubjetsCut=0 \
	--leptopOpt=VETO_RCMATCH --maxLeptopDR=1.0 --minMeffCut=600 --RCCollection=VR_rho550
    
done

    #
