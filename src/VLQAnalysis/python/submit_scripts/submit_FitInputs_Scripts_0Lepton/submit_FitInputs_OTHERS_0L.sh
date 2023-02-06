#!/bin/bash

for campaign in "mc16a" "mc16d" "mc16e"
do
    python Submit_VLQAnalysis_new.py dryrun=false campaign=${campaign} --sleep=2 \
	producetarball=true \
	--inputDir=/data/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_ntuples/21.1.121-htztx-3-syst_0_lepton_links/ \
	--sampleDat=samples_info.tag-21.2.121-htztx-3-syst-0L.${campaign}.dat \
	--outputDirSuffix=FitInputs_PAIRVLQ_OTHERS_0L_${campaign}_NOW \
	--reweightKinematics=false \
	--queue=at3 --NFILESPLIT=200 --NMERGE=1 --removeNull=FALSE \
	--runData=false --runQCD=false --runSignals=false --runTtbar=false --runOtherBkgd=true \
	--RUNDIJET=false \
	--runSingleTop=false --runWjets=false --runZjets=false --runTopEW=true --runDibosons=true \
	--runTtSyst=false --runStSyst=false --splitSTChannels=true \
	--dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=false --splitVLQDecays=false \
	--otherVariables=false --doBlind=false \
	--useObjectSyst=true --useWeightSyst=true --onlyDumpSystHistograms=true \
	--doExpSys=true --doTheorySys=true --DOPDFSYS=false \
	--doOneLeptonAna=false --doTwoLeptonAna=false --doZeroLeptonAna=true \
	--doExclusiveJetRegions=false --doLowBRegions=false --doLowJRegions=false --doSplitEMu=false --doSplitMtb=false \
	--doFitRegions=true --doValidnRegions=false --doPreselection=false --doPreselSys=true --doExtendedPreselection=false --doLooseSystRegions=false \
	--doSingleVLQRegions=false --doPairVLQRegions=true --DoOldPairProdRegions=false \
	--doOldBoost=false \
	--useLeptonsSF=false --useLeptonTrigger=false --useMETTriggerOneLep=false --useMETTrigger=true \
	--APPLYMETREGIONSCUT=true --METREGIONSCUT=400 --MTBMINCUT=250 \
	--applyMetMtwCuts=true --invertMetMtwCuts=false \
	--applydeltaphicut=true --minDeltaPhiCut=0.4 --maxDeltaPhiCut=-1. --INVERTDELTAPHICUT=false \
	--doTRF=false --recomputeTRF=false --recomputeBTagSF=false --btagOP=FixedCutBEff_77 \
	--TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
	--btagCollection=TRACK --trkJetPtCut=20. \
	--applyTtbarNNLOCorrection=false --applyVJetsSherpa22RW=false --applyTtbbCorrection=false \
	--filterType=APPLYFILTER --useSlices=true --scaleTtbarHtSlices=true \
	--jetPtCut=25 --fwdJetPtCut=20 --RCJetPtCut=200 --RCNsubjetsCut=0 \
	--leptopOpt=VETO_RCMATCH --maxLeptopDR=1.0 --minMeffCut=1000 --RCCollection=VR_rho550 \
	--APPLYMVA=true --MVAWEIGHTFILE=TMVAClassification_MLP.weights_0lep_meffin.xml
    
done

    #
