#!/bin/bash

for campaign in "mc16a" "mc16d" "mc16e"
do
    python ../../Submit_VLQAnalysis_new.py dryrun=false campaign=${campaign} --sleep=2 \
	producetarball=true \
	--inputDir=/data/at3/scratch2/farooque/MBJOutputs/tag-21.2.87-htztx-3-syst/nominal/ \
        --sampleDat=samples_info/samples_info.tag-21.2.87-htztx-3-syst.${campaign}.dat \
	--outputDirSuffix=new_mva/dataMC_TRACK_DL1_FixedCutBEff_77_background_1L_${campaign}_NOW \
	--reweightKinematics=false --kinRWList=JETSN,MEFFRED --doKinRWSmoothing=false --DOKINRWSYST=FALSE --DERIVEREWEIGHTING=true \
	--queue=at3 --NFILESPLIT=20 --NMERGE=1 --removeNull=TRUE \
	--runData=true --runQCD=false --runSignals=false --runTtbar=true --runOtherBkgd=true \
	--RUNDIJET=false \
	--runSingleTop=true --runWjets=true --runZjets=true --runTopEW=true --runDibosons=true \
	--runTtSyst=false --runStSyst=false --splitSTChannels=true \
	--dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=false --verboseOutput=false --splitVLQDecays=false \
	--otherVariables=false --doBlind=false \
	--useObjectSyst=false --useWeightSyst=false --onlyDumpSystHistograms=true \
	--doExpSys=false --doTheorySys=false --DOPDFSYS=false \
	--doOneLeptonAna=true --doTwoLeptonAna=false --doZeroLeptonAna=false \
	--doExclusiveJetRegions=false --doLowBRegions=true --doLowJRegions=false --doSplitEMu=true --doSplitMtb=false \
	--doFitRegions=false --doValidnRegions=false --doPreselection=true --doPreselSys=true --doExtendedPreselection=false --doLooseSystRegions=false \
	--doSingleVLQRegions=true --doPairVLQRegions=false --doRecoVLQ=pair --DoOldPairProdRegions=false \
	--doOldBoost=false \
	--useLeptonTrigger=true --useMETTriggerOneLep=true --useMETTrigger=true --doLeptonISO=false \
	--applyMetMtwCuts=true --invertMetMtwCuts=false \
	--applydeltaphicut=true --minDeltaPhiCut=0.4 --maxDeltaPhiCut=-1. --maxMetCutTwoLep=100. \
	--doTRF=false --recomputeTRF=false --recomputeBTagSF=false --btagOP=FixedCutBEff_77 \
	--TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
	--btagCollection=TRACK --trkJetPtCut=20. \
	--applyTtbarNNLOCorrection=false --applyVJetsSherpa22RW=false --applyTtbbCorrection=false \
	--filterType=APPLYFILTER --useSlices=true --scaleTtbarHtSlices=true \
	--jetPtCut=25 --fwdJetPtCut=20 --RCJetPtCut=200 --RCNsubjetsCut=0 \
	--leptopOpt=VETO_RCMATCH --maxLeptopDR=1.0 --minMeffCut=600 --RCCollection=VR_rho550 \
	--APPLYMVA=false --MVAWEIGHTFILE=TMVA/weightsCV_1L/TMVAClassificationCV_MLP.weights.xml --lowMVACutOneLep=0.27 --highMVACutOneLep=0.7
    
done
#--lowMVACutOneLep=0.27 --highMVACutOneLep=0.7
#--leptopOpt=VETO_RCMATCH --maxLeptopDR=1.0

#--otherVariables=false
#--doTwoLeptonAna=false
#--doSplitEMu=false --doLowBRegions=false
#--doSingleVLQRegions=false --doPairVLQRegions=true
#--reweightKinematics=true --kinRWList=JETSN,MEFFRED --doKinRWSmoothing=true --DOKINRWSYST=FALSE
#--doMVAVars=true
#--minMeffCut=1000
#--APPLYMVA=true
