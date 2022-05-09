#!/bin/bash

MACRODIR=/data/at3/scratch/cbuxovaz/VLQAnalysisFramework_br_pair_prod_ana_Git_update/VLQAnalysisFramework/src/VLQAnalysis/macros/macros_stats
dirBase=/data/at3/scratch2/cbuxovaz/PairVLQ_1L_MVA_REGIONS_March_22_2022/ASIMOV_SPLUSB_MU1_139IFB_MVA_NJETS_DECORRELATION_RANKING/
tarpath=/data/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz
fitactions=limitfitpreplotspostplots
fitinputDir="${dirBase}/configFile_"
fitoutputDir="${dirBase}/Results_Fit_Limit/"

python ${MACRODIR}/LaunchTRExFitterOnBatch_newTRexF.py \
tarball=${tarpath} \
action=${fitactions} \
inputDir=${fitinputDir} outputDir=${fitoutputDir} \
