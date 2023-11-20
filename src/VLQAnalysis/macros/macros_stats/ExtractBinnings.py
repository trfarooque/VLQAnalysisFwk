#!/bin/python3

import argparse
import sys,os
import importlib

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )

parser = argparse.ArgumentParser()

parser.add_argument('--baseDir', 
                    help='Directory containing TREXFitter fit results for all signals', 
                    required=True)
parser.add_argument('--outFileName',
                    help='Name of output file',
                    default='BinningList.txt')
parser.add_argument('--signals',
                    help='comma-separated list of signals',
                    required=True)
parser.add_argument('--regDict', 
                    help='Name of region dictionary module', 
                    default='regions_dictionary_pVLQ_newAna_MVA_regions')
parser.add_argument('--doOneLep',
                    help='Use 1lep channel regions',
                    type=int, required=True)
parser.add_argument('--doZeroLep',
                    help='Use 0lep channel regions',
                    type=int, required=True)

args = parser.parse_args()

baseDir=args.baseDir
outFileName=args.outFileName
regDict=args.regDict
doOneLep=args.doOneLep
doZeroLep=args.doZeroLep

signalList = (args.signals).split(',')

##------------------------------------------------------
## Getting all regions
##------------------------------------------------------
Regions = []
regModule = importlib.import_module(regDict)

if doOneLep:
    Regions += regModule.fit_regions_1l
if doZeroLep:
    Regions += regModule.fit_regions_0l

##------------------------------------------------------
## Extract and print binnings
##------------------------------------------------------

outputFile = open(outFileName, 'w')
for reg in Regions:
    outputFile.write(reg['name']+':\n')
    for signal in signalList:
        regFile = open(baseDir+'BONLY__'+signal+'__/Plots/'+reg['name']+'_prefit.yaml','r')
        for line in regFile:
            if 'BinEdges: ' in line:
                outputFile.write(signal + ' : ' + line.split(': ')[1])
        regFile.close()
    outputFile.write('=============\n')
outputFile.close()

