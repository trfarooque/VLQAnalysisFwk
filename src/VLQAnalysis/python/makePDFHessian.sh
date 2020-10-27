#!/bin/bash

baseDir="$VLQSCRATCH/FitInputs_RWPARAM/FilesTRexF/PDFSYSTS/"

for sample in "ttbarbb" "ttbarcc" "ttbarlight" "Singletop"
do
    for campaign in "mc16a" "mc16d" "mc16e"
    do
        python ../macros/macros_trisha/MakePDFSyst.py --inputDir=${baseDir} --outputDir=${baseDir}/NewFiles/ \
            --sample=${sample}.${campaign} --doSR=1 --doVR=1 --doPresel=1
    done
done
