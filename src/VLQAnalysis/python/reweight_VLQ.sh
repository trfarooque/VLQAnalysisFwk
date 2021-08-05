#!/bin/bash

VLQDIR="/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/VLQAnalysisOutputs_dataMC_TRACK_DL1_FixedCutBEff_77_signal_mc16e_2021_07_20_1756/"

python LaunchAllCouplingReweightings.py \
    --inputDir=${VLQDIR} \
    --outputDir=${VLQDIR}/ReweightedFiles/ \
    --mcCampaign="mc16e" \
    --queue=at3 --doLepton --statOnly --tthfitter #--doZeroLepton --tthfitter #--doAllBR #--debug --statOnly --doAllBR
    
#done

#--allRegions  
