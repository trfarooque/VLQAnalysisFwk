#!/bin/bash

#parser.add_option("-i","--inputDir",dest="inputDir",help="repository for the splitted files are located",action="store",default="")
#parser.add_option("-o","--outputDir",dest="outputDir",help="repository where to put the hadded files",action="store",default="")
#parser.add_option("-s","--statOnly",dest="statOnly",help="repository where to put the hadded files",action="store_true",default=False)
#parser.add_option("-B","--doBatch",dest="doBatch",help="launch this job on batch",action="store_true",default=False)
#parser.add_option("-l","--doLepton",dest="doLepton",help="consider 1L regions",action="store_true",default=False)
#parser.add_option("-z","--doZeroLepton",dest="doZeroLepton",help="consider 0L regions",action="store_true",default=False)
#parser.add_option("-a","--doAllBR",dest="doAllBR",help="do all the BR by 0.1 steps",action="store_true",default=False)
#parser.add_option("-q","--queue",dest="queue",help="batch queue",action="store",default="at3_short")
#parser.add_option("-t","--tthfitter",dest="tthfitter",help="for TRexFitter",action="store_true",default=False)
#parser.add_option("-n","--nMerge",type=int,dest="nMerge",help="Merging the operations of the script",default=1)
#parser.add_option("-d","--debug",dest="debug",help="Debug mode (no job submission)",action="store_true",default=False)
#parser.add_option("-r","--allRegions",dest="allRegions",help="Use all regions",action="store_true",default=False)

defaultDir="${VLQSCRATCH}/VLQAnalysisOutputs_meffGT1TeV_DefaultTopHiggs_FitInputs_2018_07_03_1750"
hardDir="${VLQSCRATCH}/VLQAnalysisOutputs_meffGT1TeV_HardTopHiggs_FitInputs_2018_07_03_1825"
hardMergedDir="${VLQSCRATCH}/VLQAnalysisOutputs_meffGT1TeV_HardMergedTopHiggs_FitInputs_2018_07_03_1913"
hardMergedMwinDir="${VLQSCRATCH}/VLQAnalysisOutputs_meffGT1TeV_HardMergedMwinTopHiggs_FitInputs_2018_07_03_2036"

#/nfs/at3/scratch2/farooque/VLQMaster/run/outVLQAnalysis_VLQ_TT_1400_302482._nominal_0.root
#for VLQDIR in ${defaultDir} ${hardDir} ${hardMergedDir} ${hardMergedMwinDir}
#do 
VLQDIR="/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/VLQAnalysisOutputs_FitInputs_PAIRVLQ_SIGNAL_mc16a_2020_11_09_1620"
python LaunchAllCouplingReweightings.py \
    --inputDir=${VLQDIR} \
    --outputDir=${VLQDIR}/ReweightedFiles/ \
    --queue=at3 --doLepton --tthfitter --doAllBR #--debug --statOnly --doAllBR
    
#done

#--allRegions  
