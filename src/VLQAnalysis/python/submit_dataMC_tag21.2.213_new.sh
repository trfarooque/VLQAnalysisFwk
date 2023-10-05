#!/bin/bash


channel='0lep'
inDir=""
if [ ${channel} == "0lep" ]; then
    inDir="/data/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2-21.2.213-htztx-sys_1_0lep"
else
    inDir="/data/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2-21.2.213-htztx-sys_0"
fi

python Submit_VLQAnalysis_new.py --dryRun 0 --campaign 'mc16e' --sleep 2 --rewriteTarball 1 \
    --inputDir ${inDir} --channel ${channel} \
    --useProcSubdirs 1 --useSyst 0 \
    --outputDirSuffix='RWtest' --processes 'ttbar' --campaigns 'mc16a' --nMerge 10 #\
#    --vlqOptString='--reweightKinematics=TRUE'

#--mode 'DATAMC' 
#dataMC_21.2.213_NOW
