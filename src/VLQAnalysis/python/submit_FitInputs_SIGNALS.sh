#!/bin/bash


for mass in "low_mass" "nom_mass"
do
    for coupling in "K20" "K25" "K30" "K40" "K50" "K60" "K70" "K80" "K90" "K100" "K110" "K120" "K130" "K140" "K150" "K160"
    do
	RWName="${mass}_${coupling}"
	for campaign in "mc16e" "mc16d" "mc16a"
	do
	    python Submit_VLQAnalysis_new.py dryrun=false campaign=${campaign} --sleep=1 \
		producetarball=true \
		--inputDir=/nfs/at3/scratch2/tvdaalen/VLQ_FullRun2/Production_tag-21.2.121-htztx-3-syst_Signal_FullGrid/ \
		--sampleDat=samples_info.tag-21.2.121-htztx-3-syst_Signal.${campaign}.dat \
		--outputDirSuffix=FitInputs_FULLGRID_SIGNAL_${mass}_${coupling}_${campaign} \
		--reweightKinematics=false \
		--vlqRWbranch=${RWName} \
		--queue=at3 --NFILESPLIT=200 --NMERGE=1 --removeNull=TRUE \
		--runData=false --runQCD=false --runSignals=true --runTtbar=false --runOtherBkgd=false \
		--runSingleTop=false --runWjets=false --runZjets=false --runTopEW=false --runDibosons=false \
		--runTtSyst=false --runStSyst=false --splitSTChannels=true \
		--dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=false --splitVLQDecays=false \
		--otherVariables=false --doBlind=false \
		--useObjectSyst=true --useWeightSyst=true --onlyDumpSystHistograms=true \
		--doExpSys=true --doTheorySys=false \
		--doOneLeptonAna=true --doTwoLeptonAna=false --doZeroLeptonAna=false \
		--doExclusiveJetRegions=false --doLowBRegions=false --doLowJRegions=false --doSplitEMu=false --doSplitMtb=false \
		--doFitRegions=true --doValidnRegions=true --doPreselection=true --doPreselSys=true \
		--doExtendedPreselection=false --doLooseSystRegions=true \
		--doSingleVLQRegions=true --doPairVLQRegions=false \
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
		--leptopOpt=VETO_RCMATCH --maxLeptopDR=1.0 --minMeffCut=600 --RCCollection=VR_rho550
	    sleep 10

	    done
	done
done

    #
