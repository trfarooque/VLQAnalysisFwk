#!/bin/bash

MACRODIR=/nfs/at3/scratch/cbuxovaz/VLQAnalysisFramework_br_pair_prod_ana/VLQAnalysisFramework/src/VLQAnalysis/macros/macros_stats

dirBase=/nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2/PairProd_tag-21.2.121-htztx-3/ASIMOV_BONLY
tarpath=/nfs/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz
fitactions=limitfitpreplotspostplots
fitinputDir="${dirBase}/configFile_"
fitoutputDir="${dirBase}/Results_Fit_Limit/"

python ${MACRODIR}/LaunchTRExFitterOnBatch_newTRexF.py \
tarball=${tarpath} \
action=${fitactions} \
inputDir=${fitinputDir} outputDir=${fitoutputDir}/ \
