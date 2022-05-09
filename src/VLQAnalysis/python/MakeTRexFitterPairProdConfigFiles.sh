#!/bin/bash

MACRODIR=/data/at3/scratch/cbuxovaz/VLQAnalysisFramework_br_pair_prod_ana_Git_update/VLQAnalysisFramework/src/VLQAnalysis/macros/macros_stats

TEMPLATEDIR=${MACRODIR}/templates/
FITINPUTDIR=/data/at3/scratch2/cbuxovaz/PairVLQ_1L_MVA_REGIONS_March_22_2022/

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

# Prepare config files
python ${MACRODIR}/PrepareConfigFilesFromTemplate.py \
inputTemplate=${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_configFile_pVLQ__SIGNAL_.txt \
outputFolder=$FITINPUTDIR/ASIMOV_BONLY_139IFB_MVA_5jex_6jin_LIMITFITPREPLOTSPOSTPLOTS \
systConfig=${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_Weights_Exp.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_Objects.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_ttMod.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_Weights_pmg_ttbar.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_singletopMod.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_Weights_pmg_singletop.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_Weights_pmg_Vjets.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_BackNorm.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_Weights_RWSYST.txt \
signal=ONE signalType=PAIR doOneLep=True doZeroLep=False doSR=True doVR=False doPresel=False useData=False \
regions=regions_dictionary_pVLQ_newAna_MVA_regions \
inputDir=${FITINPUTDIR}/merged \
signalScaling="DEFAULT" \
useBlindingCuts=False \
fitType="BONLY" \


#fitType="SPLUSB" NORMFACTOR="1,-100,100" fitPOIAsimov=1 \
#fitType="BONLY" \
#discriminant_VR=MVAScore discriminant_VR_title="MVA Score" \

#systConfig=${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_Weights_Exp.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_Objects.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_ttMod.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_Weights_pmg_ttbar.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_singletopMod.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_Weights_pmg_singletop.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_Weights_pmg_Vjets.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_BackNorm.txt,${TEMPLATEDIR}/pVLQ_templates/TEMPLATE_Systematics_Weights_RWSYST.txt \

#lumiValue="36207.66" \
