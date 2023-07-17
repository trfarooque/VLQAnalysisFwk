#!/bin/bash

# TODO:
# 1) Set option if user wants to do data/MC or produce fit inputs
#  1.1) Run systematics
#  1.2) Determine which regions to run
# 2) Set lepton channel specific options accordingly
#  2.1) Preselection cuts
#  2.2) MVA file path and cuts
#  2.3) Input directory path
# 3) Set background RW options to true for the corresponding processes

function in_list(){
    LIST=$1
    DELIMITER=$2
    VALUE=$3
    LIST_WHITESPACES=`echo $LIST | tr "$DELIMITER" " "`
    for x in $LIST_WHITESPACES; do
	if [ "$x" = "$VALUE" ]; then
	    return 0
	fi
    done
    return 1
}

processes=$1
channel=$2
campaigns=$3

echo "processes: " ${processes}
echo "channel:   " ${channel}
echo "campaigns: " ${campaigns}

if [ -z ${campaigns} ]; then
    campaigns="mc16a,mc16d,mc16e"
fi

campaigns=`echo $campaigns | tr "," " "`

valid_processes="data pvlq ttbar singletop Wjets Zjets diboson topEW dijet ttbar_alt singletop_alt"

# Lepton channel specific options
doOneLeptonAna="false" 
doTwoLeptonAna="false"
doZeroLeptonAna="false"

if [ ${channel} == "0lep" ]; then
    doZeroLeptonAna="true"
elif [ ${channel} == "1lep" ]; then
    doOneLeptonAna="true"
    doTwoLeptonAna="true"
else
    doTwoLeptonAna="true"
fi

if [ ${processes} == "bkg" ]; then
    processes="singletop Wjets Zjets diboson topEW dijet ttbar"
fi

process_list=`echo $processes | tr "," " "`

for process_name in ${process_list}; do
    # Process specific options
    runTtbar="false"
    runTtSyst="false"
    scaleTtbarHtSlices="false"
    useSlices="false"
    runData="false"
    runTOPQ1Data="false"
    runTOPQ4Data="false"
    runSignals="false"
    runOtherBkgd="false"
    runDijet="false"
    runSingletop="false"
    runStSyst="false"
    runWjets="false"
    runZjets="false"
    runTopEW="false"
    runDibosons="false"
    splitSTChannels="false"
    splitVLQDecays="false"
    kinRWList="JETSN,MEFFRED"

    sample_dat_pattern="samples_info.tag-21.2.213-htztx-syst-1L"
    
    if in_list "$valid_processes" " " ${process_name}; then
	if [ ${process_name} == "ttbar" ]; then
	    runTtbar="true"
	    scaleTtbarHtSlices="true"
	    useSlices="true"
	    kinRWList="JETSN,MEFFRED"
	elif [ ${process_name} == "ttbar_alt" ]; then
            runTtbar="true"
	    useSlices="true"
	    kinRWList="JETSN,MEFFRED"
            runTtSyst="true"
            sample_dat_pattern="samples_info.tag-21.2.213-htztx-syst-TTBARALT-1L"
	elif [ ${process_name} == "data" ]; then
	    runData="true"
	    if [ ${channel} == "1lep" ]; then
		runTOPQ1Data="true"
	    else
		runTOPQ4Data="true"
	    fi
	elif [ ${process_name} == "pvlq" ]; then
	    runSignals="true"
	    splitVLQDecays="true"
	else
	    runOtherBkgd="true"
	    if [ ${process_name} == "singletop" ]; then
		runSingletop="true"
		splitSTChannels="true"
		kinRWList="JETSN,MEFFRED"
	    fi
	    if [ ${process_name} == "singletop_alt" ]; then
		runSingletop="true"
		splitSTChannels="true"
		kinRWList="JETSN,MEFFRED"
		runStSyst="true"
		sample_dat_pattern="samples_info.tag-21.2.213-htztx-syst-STOPALT-1L" 
	    fi
	    if [ ${process_name} == "Wjets" ]; then
		runWjets="true"
		kinRWList="JETSN"
	    fi
	    if [ ${process_name} == "Zjets" ]; then
		runZjets="true"
		kinRWList="JETSN"
	    fi
	    if [ ${process_name} == "diboson" ]; then
		runDibosons="true"
	    fi
	    if [ ${process_name} == "topEW" ]; then
		runTopEW="true"
	    fi
	    if [ ${process_name} == "dijet" ]; then
		runDijet="true"
	    fi
	fi
    else
	echo "Unknown process : " ${process_name} 
	echo "Please choose a process from this list : " ${valid_processes}
	continue
    fi

    for campaign in ${campaigns}
    do
	python ../Submit_VLQAnalysis_new.py dryrun=false campaign=${campaign} --sleep=2 --producetarball=true \
	    --inputDir=/data/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2-21.2.213-htztx-sys_0/${process_name}/ \
	    --sampleDat=samples_info/${sample_dat_pattern}.${campaign}.dat \
	    --outputDirSuffix=dataMC_TRACK_DL1r_FixedCutBEff_77_${process_name}_${channel}_${campaign}_NOW \
	    --queue=at3 --NFILESPLIT=250 --NMERGE=1 --removeNull=TRUE \
	    --reweightKinematics=true --kinRWList=${kinRWList} --doKinRWSmoothing=true --DOKINRWSYST=false --deriveReweighting=true \
	    --runData=${runData} --runTOPQ1Data=${runTOPQ1Data} --runTOPQ4Data=${runTOPQ4Data} --runOtherBkgd=${runOtherBkgd} \
	    --runTtbar=${runTtbar} --useSlices=${useSlices} --scaleTtbarHtSlices=${scaleTtbarHtSlices} --filterType=APPLYFILTER \
	    --runSingleTop=${runSingletop} --splitSTChannels=${splitSTChannels} --runStSyst=${runStSyst} --runTtSyst=${runTtSyst} \
	    --runWjets=${runWjets} --runZjets=${runZjets} --runTopEW=${runTopEW} --runDibosons=${runDibosons} \
	    --runDijet=${runDijet} --runQCD=false \
	    --runSignals=${runSignals} --splitVLQDecays=${splitVLQDecays} --RUNPAIRVLQ=true --RUNSINGLEVLQ=false \
	    --doOneLeptonAna=${doOneLeptonAna} --doTwoLeptonAna=false --doZeroLeptonAna=${doZeroLeptonAna} \
	    --dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=false --verboseOutput=false --makeMVAInputTree=false --makeMVAInputHists=false \
	    --otherVariables=true --doBlind=false \
	    --useObjectSyst=false --useWeightSyst=false --onlyDumpSystHistograms=true \
	    --useLargeRJets=false --doLargeRJetsBOT=false \
	    --doExpSys=false --doTheorySys=false --DOPDFSYS=false \
	    --doExclusiveJetRegions=false --doLowBRegions=false --doLowJRegions=false --doSplitEMu=false --doSplitMtb=false \
	    --doFitRegions=false --doValidnRegions=false --doPreselection=true --doPreselSys=true --doExtendedPreselection=false --doLooseSystRegions=false \
	    --doSingleVLQRegions=false --doPairVLQRegions=true --doRecoVLQ=pair --DoOldPairProdRegions=false \
	    --doOldBoost=false \
	    --useLeptonsSF=true --useLeptonTrigger=true --useMETTriggerOneLep=true --useMETTrigger=true \
	    --applyMetMtwCuts=true --invertMetMtwCuts=false \
            --applydeltaphicut=true --minDeltaPhiCut=0.4 --maxDeltaPhiCut=-1. --maxMetCutTwoLep=100. \
	    --doTRF=false --recomputeTRF=false --recomputeBTagSF=false \
	    --TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
	    --btagCollection=TRACK --trkJetPtCut=20. \
	    --applyTtbarNNLOCorrection=false --applyVJetsSherpa22RW=false --applyTtbbCorrection=false \
	    --jetPtCut=25 --fwdJetPtCut=20 --RCJetPtCut=200 --RCNsubjetsCut=0 \
	    --leptopOpt=VETO_RCMATCH --maxLeptopDR=1.0 --minMeffCut=600 --RCCollection=VR_rho550 \
	    --APPLYMVA=true --MVAWEIGHTFILE=TMVA/weightsCV_1L_30vars_allbkgd/TMVAClassificationCV_MLP.weights.xml \
	    --lowMVACutOneLep=0.17 --highMVACutOneLep=0.59
    done

done

#--maxMetCutTwoLep=100 --onlyDumpSystHistograms=true --dumpHistos=true --minMeffCut=600
#--verboseOutput=true --otherVariables=true