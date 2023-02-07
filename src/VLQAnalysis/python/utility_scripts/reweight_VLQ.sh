#!/bin/bash

VLQDIR="/data/at3/scratch2/cbuxovaz/VLQAnalysisRun2/PairProd_DataMC_21.2.121-htztx-3_1lepton_November_5_2021/unmerged/VLQAnalysisOutputs_dataMC_TRACK_DL1_FixedCutBEff_77_signal_mc16a_2021_11_05_1837/"

python LaunchAllCouplingReweightings.py \
    --inputDir=${VLQDIR} \
    --outputDir=${VLQDIR}/ReweightedFiles/ \
    --mcCampaign="mc16a" \
    --queue=at3 --doLepton #--doZeroLepton --tthfitter #--doAllBR #--debug --statOnly --doAllBR 


VLQDIR="/data/at3/scratch2/cbuxovaz/VLQAnalysisRun2/PairProd_DataMC_21.2.121-htztx-3_1lepton_November_5_2021/unmerged/VLQAnalysisOutputs_dataMC_TRACK_DL1_FixedCutBEff_77_signal_mc16d_2021_11_05_1839/"

python LaunchAllCouplingReweightings.py \
    --inputDir=${VLQDIR} \
    --outputDir=${VLQDIR}/ReweightedFiles/ \
    --mcCampaign="mc16d" \
    --queue=at3 --doLepton #--doZeroLepton --tthfitter #--doAllBR #--debug --statOnly --doAllBR 

VLQDIR="/data/at3/scratch2/cbuxovaz/VLQAnalysisRun2/PairProd_DataMC_21.2.121-htztx-3_1lepton_November_5_2021/unmerged/VLQAnalysisOutputs_dataMC_TRACK_DL1_FixedCutBEff_77_signal_mc16e_2021_11_05_1840/"

python LaunchAllCouplingReweightings.py \
    --inputDir=${VLQDIR} \
    --outputDir=${VLQDIR}/ReweightedFiles/ \
    --mcCampaign="mc16e" \
    --queue=at3 --doLepton #--doZeroLepton --tthfitter #--doAllBR #--debug --statOnly --doAllBR
    
#done

#--allRegions  
