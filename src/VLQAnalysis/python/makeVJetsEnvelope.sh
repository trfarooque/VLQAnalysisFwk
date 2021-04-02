#!/bin/bash

#baseDir="$VLQSCRATCH/FitInputs_RWPARAM/FilesTRexF/TheorySys/"
baseDir="/nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2/PairProd_tag-21.2.121-htztx-3/merged/TheorySys/"

for sample in "Wjets" "Zjets"
do
    for campaign in "mc16a" "mc16d" "mc16e"
    do
        python ../macros/macros_trisha/MakeVjetsSyst.py --inputDir=${baseDir} --outputDir=${baseDir}/NewVJets/ \
            --sample=${sample}.${campaign} --doSR=1 --doVR=1 --doPresel=0
    done
done
