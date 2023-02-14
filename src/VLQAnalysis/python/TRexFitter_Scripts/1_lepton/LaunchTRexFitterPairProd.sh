#!/bin/bash

MACRODIR=/data/at3/scratch/cbuxovaz/VLQAnalysis_Work_Area/VLQAnalysis_br_pair_prod_ana_Chinmay_2/VLQAnalysisFramework/src/VLQAnalysis/macros/macros_stats
dirBase=/data/at3/scratch3/cbuxovaz/PairProd_VLQ_Analysis_Studies/1_lepton_TRexFitter_studies/syst_new_mva_0p27_0p70_November_21_2022/ASIMOV_BONLY_MVA_REGIONS_TTBAR_SYST_MVA_DECORRELATION_LIMITFITPREPLOTSPOSTPLOTS/


tarpath=/data/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz
fitactions=limitfitpreplotspostplots
fitinputDir="${dirBase}/configFile_"
fitoutputDir="${dirBase}/Results_Fit_Limit/"

python ${MACRODIR}/LaunchTRExFitterOnBatch_newTRexF.py \
tarball=${tarpath} \
action=${fitactions} \
inputDir=${fitinputDir} outputDir=${fitoutputDir} \
