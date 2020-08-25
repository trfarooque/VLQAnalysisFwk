#!/bin/bash

baseDir="$VLQSCRATCH/FitInputs_RWPARAM/FilesTRexF/TheorySys/"

for sample in "Wjets" "Zjets"
do
    for campaign in "mc16a" "mc16d" "mc16e"
    do
        python ../macros/macros_trisha/MakeVjetsSyst.py --inputDir=${baseDir} --outputDir=${baseDir}/NewVJets/ \
            --sample=${sample}.${campaign} --doSR=1 --doVR=1 --doPresel=1
    done
done
