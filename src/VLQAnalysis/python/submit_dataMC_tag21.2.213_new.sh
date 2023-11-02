#!/bin/bash


channel='0lep'
inDir=""
if [ ${channel} == "0lep" ]; then
    inDir="/data/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2-21.2.213-htztx-sys_1_0lep"
else
    inDir="/data/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2-21.2.213-htztx-sys_0"
fi

python Submit_VLQAnalysis_new.py --dryRun 0 --sleep 2 --rewriteTarball 1 \
    --inputDir ${inDir} --channel ${channel} \
    --useProcSubdirs 1 --useSyst 0 --mode 'DATAMC' \
    --outputDirSuffix='0L_fitRegions_statOnly_v3' --processes bkg pvlq --nMerge 10 \
    --vlqOptString='--reweightKinematics=TRUE --lowMVACutZeroLep=0.34 --highMVACutZeroLep=0.90 --doFitRegions=TRUE'

#--mode 'DATAMC' 
#dataMC_21.2.213_NOW
