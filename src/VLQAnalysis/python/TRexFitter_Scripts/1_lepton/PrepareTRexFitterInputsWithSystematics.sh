#!/bin/bash

MACRODIR=/data/at3/scratch/cbuxovaz/VLQAnalysis_Work_Area/VLQAnalysis_br_pair_prod_ana_Chinmay_2/VLQAnalysisFramework/src/VLQAnalysis/macros/macros_stats
FITINPUTDIR=/data/at3/scratch3/cbuxovaz/PairProd_VLQ_Analysis_Studies/1_lepton_TRexFitter_studies/syst_new_mva_0p27_0p70_decorrelation_test_November_27_2022/

# Nominal samples
python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/ outputDir=${FITINPUTDIR}/merged/ \
useData=false useBkgd=true signal=pair \
singletopSyst=false ttbarSyst=false useSystematics=true \
splitCampaigns=true splitSingletop=true mergeSingletop=true \

# Background Reweighting Systematics 
python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/RWSYST/ outputDir=${FITINPUTDIR}/merged/RWSYST/ \
useData=false useBkgd=true signal=NONE \
singletopSyst=false ttbarSyst=false useSystematics=false \
splitCampaigns=true splitSingletop=true mergeSingletop=true \

# PMG weight samples
python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/TheorySys/ outputDir=${FITINPUTDIR}/merged/TheorySys/ \
useData=false useBkgd=true signal=NONE \
singletopSyst=false ttbarSyst=false useSystematics=false \
splitCampaigns=true splitSingletop=true mergeSingletop=true \

# Alternative samples
python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/TTSTALT/ outputDir=${FITINPUTDIR}/merged/TtStAlt/ \
useData=false useBkgd=true signal=NONE \
singletopSyst=true ttbarSyst=true useSystematics=false \
splitCampaigns=true splitSingletop=true mergeSingletop=true \

# DRDS TtSt samples
python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/RWDS/ outputDir=${FITINPUTDIR}/merged/RWDS/ \
useData=false useBkgd=true signal=NONE \
singletopSyst=true ttbarSyst=true useSystematics=false \
splitCampaigns=true splitSingletop=true mergeSingletop=true \
singletopNominalDir=${FITINPUTDIR}/merged/ \

#singletopNominalDir=${FITINPUTDIR}/merged/TheorySys/ \

# JMR samples
python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/JMSRSYST/JMR/ outputDir=${FITINPUTDIR}/merged/JMSRSYST/JMR/ \
useData=false useBkgd=true signal=pair \
singletopSyst=false ttbarSyst=false useSystematics=false \
splitCampaigns=true splitSingletop=true mergeSingletop=true \

# JMSUP
python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/JMSRSYST/JMSUP/ outputDir=${FITINPUTDIR}/merged/JMSRSYST/JMSUP/ \
useData=false useBkgd=true signal=pair \
singletopSyst=false ttbarSyst=false useSystematics=false \
splitCampaigns=true splitSingletop=true mergeSingletop=true \

# JMSDOWN
python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/JMSRSYST/JMSDOWN/ outputDir=${FITINPUTDIR}/merged/JMSRSYST/JMSDOWN/ \
useData=false useBkgd=true signal=pair \
singletopSyst=false ttbarSyst=false useSystematics=false \
splitCampaigns=true splitSingletop=true mergeSingletop=true \

# JMSRECALC
python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/JMSRSYST/JMSRECALC/ outputDir=${FITINPUTDIR}/merged/JMSRSYST/JMSRECALC/ \
useData=false useBkgd=true signal=pair \
singletopSyst=false ttbarSyst=false useSystematics=false \
splitCampaigns=true splitSingletop=true mergeSingletop=true \
