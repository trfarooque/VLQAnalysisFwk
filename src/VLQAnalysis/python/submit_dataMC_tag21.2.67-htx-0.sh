#!/bin/bash

#declare -a btagOPList=( "FixedCutBEff_77" "FixedCutBEff_77" "FixedCutBEff_85" "FixedCutBEff_85")

for campaign in "mc16e" #"mc16d" "mc16e"
do
    for btagAlg in "MV2" #"DL1"
    do
	for btagColl in "TRACK" #"CALO"
	do
	    for btagOP in "FixedCutBEff_77" #"FixedCutBEff_60" "FixedCutBEff_70" "FixedCutBEff_85"
	    do

		python Submit_VLQAnalysis_new.py dryrun=false producetarball=true campaign=${campaign} --sleep=2 \
		    --inputDir=/nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.67-0-fJvt/${btagAlg}/ \
		    --sampleDat=samples_info_tag-21.2.67-0-fJvt-${btagAlg}.${campaign}.dat \
		    --outputDirSuffix=dataMC_${btagColl}_${btagAlg}_${btagOP}_ttbar_${campaign}_NOW \
		    --queue=at3 --NFILESPLIT=2 --NMERGE=1 \
		    --dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=false --splitVLQDecays=false \
		    --otherVariables=false --doBlind=false \
		    --useObjectSyst=false --useWeightSyst=false --onlyDumpSystHistograms=true \
		    --doOneLeptonAna=true --doZeroLeptonAna=false --doTwoLeptonAna=true --doSplitEMu=false --DOSPLITFWDJET=false \
		    --doLowBRegions=true --doLowJRegions=false \
		    --doFitRegions=true --doValidnRegions=true --DOSINGLEVLQREGIONS=true --doSingleVLQPreselection=true --doExtendedPreselection=false \
		    --doExclusiveJetRegions=true \
		    --doNewMergeBoost=true --doNewSuperMergeBoost=false \
		    --runSignals=false --runOtherBkgd=true --runTtbar=true --runQCD=false --runData=false \
		    --runTopEW=false --runDibosons=false --runSingletop=true --runZjets=false --runWjets=false \
		    --simpleSingleTop=false --runTchan=false --runSchan=false --runWtProd=true \
		    --runTOPQ1Data=true --runTOPQ4Data=true \
		    --useLeptonsSF=true --useLeptonTrigger=true --useMETTrigger=true \
		    --applyMetMtwCuts=true --invertMetMtwCuts=false --APPLYMETSIGOBJCUT=false \
		    --applydeltaphicut=true --minDeltaPhiCut=0.4 --maxDeltaPhiCut=-1. \
		    --doTRF=false --recomputeTRF=false --recomputeBTagSF=true --btagOP=${btagOP} \
		    --TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
		    --btagCollection=${btagColl} --trkJetPtCut=20. \
		    --applyTtbarNNLOCorrection=false --applyVJetsSherpa22RW=false --applyTtbbCorrection=false \
		    --filterType=APPLYFILTER --useSlices=true --scaleTtbarHtSlices=false \
		    --jetPtCut=25 --fwdJetPtCut=20 --RCJetPtCut=200 --RCNsubjetsCut=0 --RCCollection=VR_rho550 \
		    --REWEIGHTKINEMATICS=true --KINRWLIST=JETSN,MEFF
		
	    done
	done
    done
    
done
