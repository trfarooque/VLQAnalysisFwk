#!/bin/bash

dirBase=/nfs/at3/scratch2/tvdaalen/TRExFitter_WorkArea/Note/FullFit/ST_tt+Wt_Tests/ #unsplitST_PMGcor_untreated_fullSyst_fits/
tarpath=/nfs/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz
fitactions=prefit #fitpreplotspostplotslimits #fitpreplotspostplotslimits

# inDirs=(" splitST_RW_untreated_prefit_2Jul ") #unsplitST_untreated_templates_2Jul 
# inDirs=(" TREATED_prefit_STonly UNTREATED_prefit_STonly ")
# inDirs=(" Asimov_BONLY_Jul2 Asimov_BONLY_Blind_Jul2 BlindedDataFit_Jul2 ")
inDirs=(" unsplitST_PMGcor_untreated_templates_SMOOTHING40_2Jul ") #unsplitST_PMGcor_untreated_templates_2Jul ")

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