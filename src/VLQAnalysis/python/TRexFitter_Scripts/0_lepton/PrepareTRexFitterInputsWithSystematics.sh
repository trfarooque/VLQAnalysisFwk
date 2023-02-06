#!/bin/bash

MACRODIR=/data/at3/scratch/cbuxovaz/VLQAnalysis_Work_Area/VLQAnalysis_br_pair_prod_ana_Chinmay_2/VLQAnalysisFramework/src/VLQAnalysis/macros/macros_stats
FITINPUTDIR=/data/at3/scratch3/cbuxovaz/PairProd_VLQ_Analysis_Studies/0_lepton_TRexFitter_studies/syst_November_9_2022/

# Nominal samples
#python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
#inputDir=${FITINPUTDIR}/FitInputs/ outputDir=${FITINPUTDIR}/merged/ \
#useData=false useBkgd=true signal=pair \
#singletopSyst=false ttbarSyst=false useSystematics=true \
#splitCampaigns=true splitSingletop=true mergeSingletop=true \

# PMG weight samples
#python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
#inputDir=${FITINPUTDIR}/FitInputs/TheorySys/ outputDir=${FITINPUTDIR}/merged/TheorySys/ \
#useData=false useBkgd=true signal=NONE \
#singletopSyst=false ttbarSyst=false useSystematics=false \
#splitCampaigns=true splitSingletop=true mergeSingletop=true \

python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/TTSTALT/ outputDir=${FITINPUTDIR}/merged/TtStAlt/ \
useData=false useBkgd=true signal=NONE \
singletopSyst=true ttbarSyst=true useSystematics=false \
splitCampaigns=true splitSingletop=true mergeSingletop=true \
singletopNominalDir=${FITINPUTDIR}/merged/ \
