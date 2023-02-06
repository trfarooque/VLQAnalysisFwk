#!/bin/bash

MACRODIR=/data/at3/scratch/cbuxovaz/VLQAnalysis_Work_Area/VLQAnalysis_br_pair_prod_ana_Chinmay_2/VLQAnalysisFramework/src/VLQAnalysis/macros/macros_stats
FITINPUTDIR=/data/at3/scratch3/cbuxovaz/PairProd_VLQ_Analysis_Studies/1_lepton_TRexFitter_studies/stat_only_new_mva_0p27_0p70_November_11_2022/

# Nominal samples
python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/ outputDir=${FITINPUTDIR}/merged/ \
useData=false useBkgd=true signal=pair \
singletopSyst=false ttbarSyst=false useSystematics=false \
splitCampaigns=true splitSingletop=true mergeSingletop=true
