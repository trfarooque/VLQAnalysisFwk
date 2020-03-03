#!/bin/bash

ONELEP=1
ZEROLEP=1
CURDIR=$PWD 

while [[ $# -gt 1 ]]
do
    key="$1"
    
    case $key in
	-i|--inputDir)
	    INDIR="$2"
	    shift # past argument
	    ;;
	-z|--doZeroLep)
	    ZEROLEP="$2"
	    shift # past argument
	    ;;
	-l|--doOneLep)
	    ONELEP="$2"
	    shift # past argument
	    ;;
	*)
            # unknown option
	    ;;
    esac
    shift # past argument or value
done

echo "inDir = " ${INDIR}
echo "doZeroLep = " ${ZEROLEP}
echo "doOneLep = " ${ONELEP}

cd ${INDIR}
mkdir HistFiles
mv outVLQAnalysis*.root HistFiles/.
mkdir MergedFiles

if [[ ${ONELEP} -eq 1 ]];
then
    hadd MergedFiles/outVLQAnalysis_QCD_nominal_HIST.root HistFiles/outVLQAnalysis_QCD*.root
    hadd MergedFiles/outVLQAnalysis_Data_TOPQ1_nominal_HIST.root HistFiles/outVLQAnalysis_Data*TOPQ1*.root
fi

if [[ ${ZEROLEP} -eq 1 ]];
then
    hadd MergedFiles/outVLQAnalysis_Data_TOPQ4_nominal_HIST.root HistFiles/outVLQAnalysis_Data*TOPQ4*.root
fi

hadd MergedFiles/outVLQAnalysis_Singletop_nominal_HIST.root HistFiles/outVLQAnalysis_Singletop_*.root
hadd MergedFiles/outVLQAnalysis_topEW_nominal_HIST.root HistFiles/outVLQAnalysis_topEW_*.root HistFiles/outVLQAnalysis_ttH_*.root 

hadd MergedFiles/outVLQAnalysis_Wjets_nominal_HIST.root HistFiles/outVLQAnalysis_W+jets_*.root
hadd MergedFiles/outVLQAnalysis_Zjets_nominal_HIST.root HistFiles/outVLQAnalysis_Z+jets_*.root

hadd MergedFiles/outVLQAnalysis_others_nominal_HIST.root HistFiles/outVLQAnalysis_Dibosons_*.root HistFiles/outVLQAnalysis_SM4tops_*.root

hadd MergedFiles/outVLQAnalysis_ttbarbb_nominal_HIST.root HistFiles/outVLQAnalysis_ttbarbb_*.root
hadd MergedFiles/outVLQAnalysis_ttbarcc_nominal_HIST.root HistFiles/outVLQAnalysis_ttbarcc_*.root
hadd MergedFiles/outVLQAnalysis_ttbarlight_nominal_HIST.root HistFiles/outVLQAnalysis_ttbarlight_*.root

cd ${CURDIR}
