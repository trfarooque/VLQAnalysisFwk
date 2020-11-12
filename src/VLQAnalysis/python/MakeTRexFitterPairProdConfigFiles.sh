#!/bin/bash

MACRODIR=/nfs/at3/scratch/cbuxovaz/VLQAnalysisFramework_br_pair_prod_ana/VLQAnalysisFramework/src/VLQAnalysis/macros/macros_stats

TEMPLATEDIR=${MACRODIR}/templates/

FITINPUTDIR=/nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2/PairProd_tag-21.2.121-htztx-3/

# Nominal samples
python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/ outputDir=${FITINPUTDIR}/merged/ \
useData=true useBkgd=true signal=pair \
singletopSyst=false ttbarSyst=false useSystematics=true \
splitCampaigns=true splitSingletop=true mergeSingletop=true \

# PMG weight samples
python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/TheorySys outputDir=${FITINPUTDIR}/merged/TheorySys \
useData=false useBkgd=true signal=NONE \
singletopSyst=false ttbarSyst=false useSystematics=false \
splitCampaigns=true splitSingletop=true mergeSingletop=true \

# Alternative samples
python ${MACRODIR}/PrepareInputFilesTRexFitter.py \
inputDir=${FITINPUTDIR}/FitInputs/TtStAlt/ outputDir=${FITINPUTDIR}/merged/TtStAlt/ \
useData=false useBkgd=true signal=NONE \
singletopSyst=true ttbarSyst=true useSystematics=false \
splitCampaigns=true splitSingletop=true mergeSingletop=true \


python ${MACRODIR}/PrepareConfigFilesFromTemplate.py \
inputTemplate=${TEMPLATEDIR}/TEMPLATE_configFile_pVLQ__SIGNAL_.txt \
outputFolder=$FITINPUTDIR/ASIMOV_BONLY/ \
systConfig=${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_Exp.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Objects.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_ttMod.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_ttbar.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_singletopMod.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_singletop.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_Vjets.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_BackNorm.txt \
signal=ONE signalType=PAIR doOneLep=True doZeroLep=False doSR=True doVR=True doPresel=False useData=False \
regions=regions_dictionary_pVLQ \
inputDir=${FITINPUTDIR}/merged \
signalScaling="DEFAULT" \
useBlindingCuts=False
