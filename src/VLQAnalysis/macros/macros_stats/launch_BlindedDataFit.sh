#!/bin/bash

dirBase=/nfs/at3/scratch2/tvdaalen/TRExFitter_WorkArea/Note/FullFit/BlindedDataFit_allSyst/
tarpath=/nfs/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz
fitactions=fitpreplots
fitinputDir="${dirBase}/configFile_"
fitoutputDir="${dirBase}/Results/"

python LaunchTRExFitterOnBatch_newTRexF.py \
    tarball=${tarpath} \
    action=${fitactions} \
    inputDir=${fitinputDir} outputDir=${fitoutputDir}/ \
    queue=at3_short
