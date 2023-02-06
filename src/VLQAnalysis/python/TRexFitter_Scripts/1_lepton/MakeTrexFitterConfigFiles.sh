#!/bin/bash

MACRODIR=/data/at3/scratch/cbuxovaz/VLQAnalysis_Work_Area/VLQAnalysis_br_pair_prod_ana_Chinmay_2/VLQAnalysisFramework/src/VLQAnalysis/macros/macros_stats
FITINPUTDIR=/data/at3/scratch3/cbuxovaz/PairProd_VLQ_Analysis_Studies/1_lepton_TRexFitter_studies/syst_new_mva_0p27_0p70_November_21_2022/
TEMPLATEDIR=${MACRODIR}/templates/pVLQ_templates/1_lepton_templates/
REGIONSDICT=regions_dictionary_pVLQ_newAna_boosted_object_cut_regions

python ${MACRODIR}/PrepareConfigFilesFromTemplate.py \
inputTemplate=${TEMPLATEDIR}/stat_only/TEMPLATE_configFile_pVLQ__SIGNAL_.txt \
outputFolder=${FITINPUTDIR}/ASIMOV_BONLY_STATONLY_BOC_LIMITFITPREPLOTSPOSTPLOTS \
systConfig=${TEMPLATEDIR}/stat_only/TEMPLATE_Systematics_DUMMY.txt \
signal=ONE signalType=PAIR doOneLep=True doZeroLep=False doSR=True doVR=False doPresel=False useData=False \
regions=${REGIONSDICT} \
inputDir=${FITINPUTDIR}/merged \
signalScaling="DEFAULT" \
useBlindingCuts=False \
statOnly=FALSE \
fitType="BONLY"

#systConfig=${TEMPLATEDIR}/pVLQ_templates/0_lepton_templates/TEMPLATE_Systematics_DUMMY.txt \

#systConfig=${TEMPLATEDIR}/pVLQ_templates/0_lepton_templates/TEMPLATE_Systematics_Weights_Exp.txt,${TEMPLATEDIR}/pVLQ_templates/0_lepton_templates/TEMPLATE_Systematics_Objects.txt,${TEMPLATEDIR}/pVLQ_templates/0_lepton_templates/TEMPLATE_Systematics_BackNorm.txt \

#inputTemplate=${TEMPLATEDIR}/pVLQ_templates/0_lepton_templates/TEMPLATE_configFile_pVLQ__SIGNAL_.txt \
