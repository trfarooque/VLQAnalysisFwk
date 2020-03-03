#!/bin/bash

INDIR=$1
CURDIR=$PWD 

cd ${INDIR}
mkdir MergedTreeFiles

hadd MergedTreeFiles/outVLQAnalysis_Data_nominalTREE.root TreeFiles/outVLQAnalysis_Data_*TREE.root
hadd MergedTreeFiles/outVLQAnalysis_ttbarlight_nominalTREE.root TreeFiles/outVLQAnalysis_ttbarlight_41*TREE.root TreeFiles/outVLQAnalysis_ttbarlight_40*TREE.root 
hadd MergedTreeFiles/outVLQAnalysis_ttbarcc_nominalTREE.root TreeFiles/outVLQAnalysis_ttbarcc_41*TREE.root TreeFiles/outVLQAnalysis_ttbarcc_40*TREE.root
hadd MergedTreeFiles/outVLQAnalysis_ttbarbb_nominalTREE.root TreeFiles/outVLQAnalysis_ttbarbb_41*TREE.root TreeFiles/outVLQAnalysis_ttbarbb_40*TREE.root
hadd MergedTreeFiles/outVLQAnalysis_singletop_nominalTREE.root TreeFiles/outVLQAnalysis_Singletop_*TREE.root
hadd MergedTreeFiles/outVLQAnalysis_Wjets22_nominalTREE.root TreeFiles/outVLQAnalysis_W+jets22*_*TREE.root
hadd MergedTreeFiles/outVLQAnalysis_Zjets22_nominalTREE.root TreeFiles/outVLQAnalysis_Z+jets22*_*TREE.root
hadd MergedTreeFiles/outVLQAnalysis_others_nominal_TREE.root TreeFiles/outVLQAnalysis_topEW_*TREE.root TreeFiles/outVLQAnalysis_ttH_*TREE.root TreeFiles/outVLQAnalysis_Dibosons_*TREE.root TreeFiles/outVLQAnalysis_4tops_*TREE.root


cd ${CURDIR}
