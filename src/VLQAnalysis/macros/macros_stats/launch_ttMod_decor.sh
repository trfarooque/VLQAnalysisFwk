#!/bin/bash

dirBase=/nfs/at3/scratch2/tvdaalen/TRExFitter_WorkArea/Note/FullFit/TtbarMod_DecorTests/
tarpath=/nfs/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz
fitactions=fitpreplotspostplots

# inDirs=(" BlindedDataFit_allSyst Asimov_BONLY_Blind_Jun4 Asimov_BONLY_Jun4 ") #BlindedDataFit_allSyst
# inDirs=(" Asimov_BONLY_Template_prefit_Jun4 ") #BlindedDataFit_allSyst

# inDirs=(" TtbarDebug_Asimov_TtStOnly_prefit_HF+PMGcor ") #BlindedDataFit_allSyst
# inDirs=(" Asimov_BONLY_HF+PMGcor_Jun18 Asimov_BONLY_HF+PMGcor_Blind_Jun18 BlindedDataFit_HF+PMGcor_Jun18 ")

# inDirs=(" Decor_AllRegions_Data_Blind_Jun25 Decor_AllRegions_Asimov_Jun25 Decor_AllRegions_Asimov_Blind_Jun25 ")

inDirs=(" Decor_AllRegions_GENPS+PMG/Asimov_Blind_Jun26 ") #" Decor_AllRegions_GENPSonly/Asimov_Jun27 Decor_AllRegions_PMGonly/Asimov_Jun27 ") #Asimov_Blind_Jun26 Asimov_Jun26 Data_Blind_Jun26

for dir in ${inDirs[@]}
do

    fitinputDir="${dirBase}/${dir}/configFile_"
    fitoutputDir="${dirBase}/${dir}/Results/"

    python LaunchTRExFitterOnBatch_newTRexF.py \
        tarball=${tarpath} \
        action=${fitactions} \
        inputDir=${fitinputDir} outputDir=${fitoutputDir}/ \
        queue=at3_short
done