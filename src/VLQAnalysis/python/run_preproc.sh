#!/bin/bash

baseDir='/data/at3/scratch2/farooque/CarlosFitInputsTest/MVAHISTS/'
#baseDir='/data/at3/scratch2/tfarooque/VLQAnalysisRun2/VLQAnalysisOutputs_0L_fitRegions_statOnly_v3/'

python src/VLQAnalysis/python/FitInputsPreProcessor.py --inputDir=${baseDir} --outputDir=${baseDir} \
--channel='1lep' --useSyst=0 --otherVariables=0 --inputSuffix=MVAHISTS \
--processes pvlq ttbar singletop topEW Wjets Zjets diboson dijet \
--checkOutputs=0 --relaunchFailedJobs=0 --linkFiles=0 --mergeFiles=0  \
--reweightVLQ=1 --extrapolateSingletopSyst=0 --makeEnvelopeVjetsSyst=0 #--dryRun=1

# Wjets Zjets
#singletop_alt Wjets Zjets
