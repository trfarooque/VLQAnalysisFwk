#!/bin/bash

MACRODIR=/data/at3/scratch/cbuxovaz/VLQAnalysis_Work_Area/VLQAnalysis_br_pair_prod_ana_Chinmay_2/VLQAnalysisFramework/src/VLQAnalysis/macros/macros_stats
FITINPUTDIR=/data/at3/scratch3/cbuxovaz/PairProd_VLQ_Analysis_Studies/1_lepton_TRexFitter_studies/syst_new_mva_0p27_0p70_November_21_2022/
TEMPLATEDIR=${MACRODIR}/templates/pVLQ_templates/1_lepton_templates/

REGIONSDICT=regions_dictionary_pVLQ_newAna_MVA_regions
#REGIONSDICT=regions_dictionary_pVLQ_newAna_boosted_object_cut_regions
#REGIONSDICT=regions_dictionary_pVLQ

python ${MACRODIR}/PrepareConfigFilesFromTemplate.py \
inputTemplate=${TEMPLATEDIR}/TEMPLATE_configFile_pVLQ__SIGNAL_.txt \
outputFolder=${FITINPUTDIR}/ASIMOV_BONLY_MVA_REGIONS_TTBAR_SYST_MVA_DECORRELATION_LIMITFITPREPLOTSPOSTPLOTS \
regions=${REGIONSDICT} \
systConfig=${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_Exp.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Objects.txt,${TEMPLATEDIR}/MVA_regions/MVA_decorrelated/TEMPLATE_Systematics_ttMod.txt,${TEMPLATEDIR}/MVA_regions/MVA_decorrelated/TEMPLATE_Systematics_Weights_pmg_ttbar.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_singletopMod.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_singletop.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_Vjets.txt,${TEMPLATEDIR}/MVA_regions/TEMPLATE_Systematics_BackNorm.txt,${TEMPLATEDIR}/MVA_regions/TEMPLATE_Systematics_Weights_RWSYST.txt \
signal=ONE signalType=PAIR doOneLep=True doZeroLep=False doSR=True doVR=False doPresel=False useData=False \
inputDir=${FITINPUTDIR}/merged \
signalScaling="DEFAULT" \
useBlindingCuts=False \
statOnly=FALSE \
fitType="BONLY"
