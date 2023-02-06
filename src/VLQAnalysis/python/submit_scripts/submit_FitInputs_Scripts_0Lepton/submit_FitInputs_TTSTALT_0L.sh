#!/bin/bash

for campaign in "mc16a" "mc16d" "mc16e"
do
    python Submit_VLQAnalysis_new.py dryrun=false campaign=${campaign} --sleep=2 \
	producetarball=true \
        --inputDir=/data/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_ntuples/21.1.121-htztx-3-syst_0_lepton_links/ttst_alt/ \
        --sampleDat=samples_info/samples_info_ttst_alt.tag-21.2.213-htztx-0L.${campaign}.dat \
	--outputDirSuffix=FitInputs_PAIRVLQ_TTSTALT_0L_${campaign}_NOW \
	--reweightKinematics=false --kinRWList=JETSN,MEFFRED --doKinRWSmoothing=false --DOKINRWSYST=false --isAFII=true \
	--queue=at3 --NFILESPLIT=200 --NMERGE=1 --removeNull=TRUE \
	--runData=false --runQCD=false --runSignals=false --runTtbar=true --runOtherBkgd=true \
	--RUNDIJET=false \
	--runSingleTop=true --runWjets=false --runZjets=false --runTopEW=false --runDibosons=false \
	--runTtSyst=true --runStSyst=true --splitSTChannels=true \
	--dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=false --splitVLQDecays=false \
	--otherVariables=false --doBlind=false \
	--useObjectSyst=false --useWeightSyst=false --onlyDumpSystHistograms=true \
	--doExpSys=false --doTheorySys=false --DOPDFSYS=false \
	--doOneLeptonAna=false --doTwoLeptonAna=false --doZeroLeptonAna=true \
	--doExclusiveJetRegions=false --doLowBRegions=false --doLowJRegions=false --doSplitEMu=false --doSplitMtb=false \
	--doFitRegions=true --doValidnRegions=false --doPreselection=false --doPreselSys=true --doExtendedPreselection=false --doLooseSystRegions=false \
	--APPLYMETREGIONSCUT=true --METREGIONSCUT=400 --MTBMINCUT=250 \
	--doSingleVLQRegions=false --doPairVLQRegions=true --DoOldPairProdRegions=true \
	--doOldBoost=false \
	--useLeptonsSF=false --useLeptonTrigger=false --useMETTriggerOneLep=false --useMETTrigger=true \
	--applyMetMtwCuts=true --invertMetMtwCuts=false \
	--applydeltaphicut=true --minDeltaPhiCut=0.4 --maxDeltaPhiCut=-1. --INVERTDELTAPHICUT=false \
	--doTRF=false --recomputeTRF=false --recomputeBTagSF=false --btagOP=FixedCutBEff_77 \
	--TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
	--btagCollection=TRACK --trkJetPtCut=20. \
	--RWTTFRACTIONS=true \
	--applyTtbarNNLOCorrection=false --applyVJetsSherpa22RW=false --applyTtbbCorrection=false \
	--filterType=APPLYFILTER --useSlices=true --scaleTtbarHtSlices=true \
	--jetPtCut=25 --fwdJetPtCut=20 --RCJetPtCut=200 --RCNsubjetsCut=0 \
	--leptopOpt=VETO_RCMATCH --maxLeptopDR=1.0 --minMeffCut=1000 --RCCollection=VR_rho550 \
	--APPLYMVA=true --MVAWEIGHTFILE=TMVA/TMVAClassification_MLP.weights_0lep_meffin.xml
    
done
