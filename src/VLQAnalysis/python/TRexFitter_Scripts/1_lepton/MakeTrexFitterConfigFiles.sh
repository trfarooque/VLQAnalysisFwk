#!/bin/bash

MACRODIR=/data/at3/scratch/cbuxovaz/VLQAnalysis_Work_Area/VLQAnalysis_br_pair_prod_ana_dev/VLQAnalysisFramework/src/VLQAnalysis/macros/macros_stats
FITINPUTDIR=/data/at3/scratch2/cbuxovaz/VLQAnalysisRun2/VLQAnalysisOutputs_1L_fit_inputs_new_test_file_merge_mc16a/MergedFiles/merged/
OUTPUTDIR=/data/at3/scratch2/cbuxovaz/PairVLQ_1L_Statistical_Analysis_NEW/ASIMOV_SPLUSB_MU0/
TEMPLATEDIR=${MACRODIR}/templates/pVLQ_templates/1_lepton_templates/

REGIONSDICT=regions_dictionary_pVLQ_newAna_MVA_regions

python ${MACRODIR}/PrepareConfigFilesFromTemplate.py \
inputTemplate=${TEMPLATEDIR}/STOP_EXTRAPOLATION_TEST/TEMPLATE_configFile_pVLQ__SIGNAL_.txt \
outputFolder=${OUTPUTDIR} \
regions=${REGIONSDICT} \
systConfig=${TEMPLATEDIR}/TEMPLATE_Systematics_Objects.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_Exp.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_ttMod.txt,${TEMPLATEDIR}/STOP_EXTRAPOLATION_TEST/TEMPLATE_Systematics_singletopMod.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_BackNorm.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_RWSYST.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_Vjets.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_ttbar.txt,${TEMPLATEDIR}/STOP_EXTRAPOLATION_TEST/TEMPLATE_Systematics_Weights_pmg_singletop.txt \
signal=ONE signalType=PAIR doOneLep=True doZeroLep=False doSR=True doVR=False doPresel=False useData=False \
inputDir=${FITINPUTDIR} \
signalScaling="DEFAULT" \
useBlindingCuts=False \
statOnly=FALSE \
NORMFACTOR="1,-100,100" \
fitPOIAsimov=0 \
fitType="BONLY"
#FitBlind="FALSE" LimitBlind="FALSE" useData=True  Use this when running with data
#fitType="SPLUSB" fitPOIAsimov=0 (1) Use this when running S+B fits with mu=0 (1)
