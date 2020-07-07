#!/bin/bash

dirBase=/nfs/at3/scratch2/tvdaalen/TRExFitter_WorkArea/Note/FullFit/Ttbar+Singletop_DecorTests/Decor_NjetsNboosted/
tarpath=/nfs/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz
fitactions=fitpreplotspostplotslimits

inDirs1=(" FullFits_st_decor FullFits_ttst_decor ") #FullFits_tt_decor 

inDirs2=( " Asimov_Jul6 Asimov_Blind_Jul6 Data_Blind_Jul6")

for dir1 in ${inDirs1[@]}
do

    for dir2 in ${inDirs2[@]}
    do

        fitinputDir="${dirBase}/${dir1}/${dir2}/configFile_"
        fitoutputDir="${dirBase}/${dir1}/${dir2}/Results/"

        python LaunchTRExFitterOnBatch_newTRexF.py \
            tarball=${tarpath} \
            action=${fitactions} \
            inputDir=${fitinputDir} outputDir=${fitoutputDir}/ \
            queue=at3_short

    done
done