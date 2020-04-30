#!/bin/bash

# ------------ General options -------------
campaigns=( " mc16a mc16d mc16e " ) 
sampledat=samples_info.tag-21.2.87-htztx-3-syst._CAMPAIGN_.dat # "_CAMPAIGN_" will be replaced in loop

inputdir=/nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.87-htztx-3-syst/
outputdirsuffix=ExampleSuffix__CAMPAIGN_                       # "_CAMPAIGN_" will be replaced in loop
                                                               # Add "NOW" to include timestamp

# ------------- Output options -------------
otherVariables=false
onlyDumpSystHistograms=true
preselSys=true
splitEMu=false
doTruth=false

oneLepton=true
twoLepton=false
zeroLepton=false

# ------------- Region options -------------
presel=true
extPresel=true
exclJetRegions=false

singleVLQRegions=true
pairVLQRegions=false

fitRegions=true
validnRegions=true
looseSystRegions=true

# ------------- Sample options -------------
runData=false
runQCD=false
runSignals=true
runTtbar=true

runOtherBkgd=true # Needs to be true if you want to run any of below backgrounds
runSingletop=true
runWjets=true
runZjets=true
runTopEW=true
runDibosons=true

splitSingletopChannels=false

# ----------- Systematics options ----------
ttbarModSyst=false
singletopModSyst=false

weightSyst=false
expSyst=false
objSyst=false
theorySyst=false

# ------- Ttbar HT slicing options ---------
# Should be: true/APPLYFILTER or false/NOFILTER
doTtbarHTSlicing=true
filterType=APPLYFILTER

# ------------------------------------------------ #
# Other options typically don't need to be changed #
# ------------------------------------------------ #

for campaign in ${campaigns[@]}
do
    python Submit_VLQAnalysis_new.py dryrun=false campaign=${campaign} --sleep=1 \
    producetarball=true \
    --inputDir=${inputdir} \
    --sampleDat=${sampledat/_CAMPAIGN_/${campaign}} \
    --outputDirSuffix=${outputdirsuffix/_CAMPAIGN_/${campaign}} \
    --queue=at3 --NFILESPLIT=10 --NMERGE=5 \
    --doOneLeptonAna=${oneLepton} --doTwoLeptonAna=${twoLepton} --doZeroLeptonAna=${zeroLepton} \
    --dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=${doTruth} --splitVLQDecays=false \
    --runData=${runData} --runQCD=${runQCD} --runSignals=${runSignals} --doBlind=false \
    --runTtbar=${runTtbar} --runOtherBkgd=${runOtherBkgd} \
    --runSingletop=${runSingletop} --runWjets=${runWjets} \
    --runZjets=${runZjets} --runTopEW=${runTopEW} --runDibosons=${runDibosons} \
    --splitSTChannels=${splitSingletopChannels} \
    --onlyDumpSystHistograms=${onlyDumpSystHistograms} --otherVariables=${otherVariables} \
    --runTtSyst=${ttbarModSyst} --runStSyst=${singletopModSyst}  \
    --useObjectSyst=${objSyst} --useWeightSyst=${weightSyst} --doExpSys=${expSyst} --doTheorySys=${theorySyst} \
    --doExclusiveJetRegions=${exclJetRegions} --doLowBRegions=false --doLowJRegions=false --doSplitEMu=${splitEMu} --doSplitMtb=false \
    --doFitRegions=${fitRegions} --doValidnRegions=${validnRegions} --doPreselection=${presel} --doExtendedPreselection=${extPresel} \
    --doSingleVLQRegions=${singleVLQRegions} --doPairVLQRegions=${pairVLQRegions} --doLooseSystRegions=${looseSystRegions} \
    --doOldBoost=false --doPreselSys=${preselSys} \
    --useLeptonTrigger=true --useMETTriggerOneLep=true --useMETTrigger=true \
    --applyMetMtwCuts=true --invertMetMtwCuts=false \
    --applydeltaphicut=true --minDeltaPhiCut=0.4 --maxDeltaPhiCut=-1. \
    --doTRF=false --recomputeTRF=false --recomputeBTagSF=false --btagOP=FixedCutBEff_77 \
    --TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
    --btagCollection=TRACK --trkJetPtCut=20. \
    --applyTtbarNNLOCorrection=false --applyVJetsSherpa22RW=false --applyTtbbCorrection=false \
    --filterType=${filterType} --useSlices=${doTtbarHTSlicing} \
    --scaleTtbarHtSlices=true --RwTtFractions=true \
    --jetPtCut=25 --fwdJetPtCut=20 --RCJetPtCut=200 --RCNsubjetsCut=0 \
    --leptopOpt=VETO_RCMATCH --maxLeptopDR=1.0 --minMeffCut=600 --RCCollection=VR_rho550

done
