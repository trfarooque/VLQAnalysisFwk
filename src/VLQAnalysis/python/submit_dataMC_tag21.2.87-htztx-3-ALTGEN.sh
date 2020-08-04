#!/bin/bash

#declare -a btagOPList=( "FixedCutBEff_77" "FixedCutBEff_77" "FixedCutBEff_85" "FixedCutBEff_85")

for campaign in "mc16a" "mc16d" "mc16e"
do
    for btagAlg in "DL1" #"DL1"
    do
	for btagColl in "TRACK" #"CALO"
	do
	    for btagOP in "FixedCutBEff_77" #"FixedCutBEff_60" "FixedCutBEff_70" "FixedCutBEff_85"
	    do

		python Submit_VLQAnalysis_new.py dryrun=false producetarball=true campaign=${campaign} --sleep=2 \
		    --inputDir=/nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.87-htztx-3-syst/alt_samples_v2/ \
		    --sampleDat=samples_info_ttst_alt.tag-21.2.87-htztx-3-syst.${campaign}.dat \
		    --outputDirSuffix=dataMC_${btagColl}_${btagAlg}_${btagOP}_ttbar_${campaign}_NOW \
		    --queue=at3 --NFILESPLIT=200 --NMERGE=1 \
		    --dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=false --splitVLQDecays=false \
		    --otherVariables=false --doBlind=false \
		    --useObjectSyst=false --useWeightSyst=false --onlyDumpSystHistograms=true \
		    --doOneLeptonAna=true --doZeroLeptonAna=false --doTwoLeptonAna=true --doSplitEMu=false \
		    --doLowBRegions=false --doLowJRegions=false \
		    --doFitRegions=false --doValidnRegions=false --DOSINGLEVLQREGIONS=true --doExtendedPreselection=false \
		    --doExclusiveJetRegions=true --DOPRESELSYS=true --DOPRESELECTION=true \
		    --computeWeightSys=false --DOEXPSYS=false --DOTHEORYSYS=false \
		    --ISAFII=true --ISDIAGSUB=false \
		    --MAXMETCUTTWOLEP=100. \
		    --RWTTFRACTIONS=true --RUNTTSYST=true --RUNSTSYST=true \
		    --runSignals=false --runOtherBkgd=true --runTtbar=true --runQCD=false --runData=false \
		    --runTopEW=false --runDibosons=false --runSingletop=true --runZjets=false --runWjets=false \
		    --SPLITSTCHANNELS=true --runTchan=false --runSchan=false --runWtProd=true \
		    --runTOPQ1Data=true --runTOPQ4Data=true \
		    --useLeptonsSF=true --useLeptonTrigger=true --useMETTrigger=true --USEMETTRIGGERONELEP=true \
		    --applyMetMtwCuts=true --invertMetMtwCuts=false --APPLYMETSIGOBJCUT=false \
		    --applydeltaphicut=true --minDeltaPhiCut=0.4 --maxDeltaPhiCut=-1. \
		    --doTRF=false --recomputeTRF=false --recomputeBTagSF=false --btagOP=${btagOP} \
		    --TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
		    --btagCollection=${btagColl} --trkJetPtCut=20. \
		    --applyTtbarNNLOCorrection=false --applyVJetsSherpa22RW=false --applyTtbbCorrection=false \
		    --filterType=APPLYFILTER --useSlices=true --scaleTtbarHtSlices=true \
		    --jetPtCut=25 --fwdJetPtCut=20 --RCJetPtCut=200 --RCNsubjetsCut=0 --RCCollection=VR_rho550 \
		    --minMeffCut=600 \
		    --REWEIGHTKINEMATICS=true --KINRWLIST=JETSN,MEFFRED --DOKINRWSMOOTHING=true
		
	    done
	done
    done
    
done
