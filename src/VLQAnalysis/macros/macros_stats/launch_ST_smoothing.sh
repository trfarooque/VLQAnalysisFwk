#!/bin/bash

dirBase=/nfs/at3/scratch2/tvdaalen/TRExFitter_WorkArea/Note/FullFit/ST_tt+Wt_Tests/NORW_untreated_SmoothingTests/
tarpath=/nfs/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz
fitactions=prefit 

inDirs=(" Smoothing_COMMONTOOLSMOOTHMONOTONIC ") #Smoothing_COMMONTOOLSMOOTHPARABOLIC Smoothing_KERNELDELTAGAUSS Smoothing_KERNELRATIOGAUSS Smoothing_KERNELRATIOUNIFORM Smoothing_MAXVARIATION Smoothing_TTBARRESONANCE ") 

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