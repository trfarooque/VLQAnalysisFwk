import ROOT as root
import time
import sys
from array import *
import math
import os
import glob


from optparse import OptionParser

##____________________________________________________
## Options
parser = OptionParser()
parser.add_option("-i","--inputDir",dest="inputDir",help="repository for the TRex files are located",action="store",default="")
parser.add_option("-o","--outputDir",dest="outputDir",help="repository where to put the modified files",action="store",default="SlimmedInputs/")

(options, args) = parser.parse_args()

outputDir=options.outputDir
inputDir=options.inputDir


root.gROOT.SetBatch(1)
root.gStyle.SetOptTitle(0)
root.gStyle.SetPalette(1)
root.TH1.SetDefaultSumw2(1)
root.gStyle.SetOptStat(0)


varlist=[ 
    "meff",
    ]

do1Lep=True
do0Lep=True

Regions = []
if do1Lep:
    Regions += ["c1l2b"]
    Regions += ["c1l1RCTTMass6j2b"]
    Regions += ["c1l1RCTTMass6j3bLowMbb"]
    Regions += ["c1l1RCTTMass6j3bHighMbb"]
    Regions += ["c1l1RCTTMass6j4bLowMbb"]
    Regions += ["c1l1RCTTMass6j4bHighMbb"]
    Regions += ["c1l2RCTTMass6j2b"]
    Regions += ["c1l2RCTTMass6j3b"]
    Regions += ["c1l2RCTTMass6j4b"]
    Regions += ["c1l0RCTTMass6j2b"]
    Regions += ["c1l0RCTTMass6j3b"]
    Regions += ["c1l0RCTTMass6j4b"]
    
    Regions += ["c1l1RCTTMass5j2b"]
    Regions += ["c1l1RCTTMass5j3b"]
    Regions += ["c1l1RCTTMass5j4b"]
    Regions += ["c1l2RCTTMass5j2b"]
    Regions += ["c1l2RCTTMass5j3b"]
    Regions += ["c1l2RCTTMass5j4b"]
    Regions += ["c1l0RCTTMass5j2b"]
    Regions += ["c1l0RCTTMass5j3b"]
    Regions += ["c1l0RCTTMass5j4b"]

if do0Lep:
#0lep
    Regions += ["c0l2b"]
    Regions += ["c0l1RCTTMass7j2b"]
    Regions += ["c0l1RCTTMass7j3b"]
    Regions += ["c0l1RCTTMass7j4b"]
    Regions += ["c0l1RCTTMass7j3bLowMtbmin"]
    Regions += ["c0l1RCTTMass7j3bHighMtbmin"]
    Regions += ["c0l1RCTTMass7j4bLowMtbmin"]
    Regions += ["c0l1RCTTMass7j4bHighMtbmin"]
    Regions += ["c0l2RCTTMass7j2b"]
    Regions += ["c0l2RCTTMass7j3b"]
    Regions += ["c0l2RCTTMass7j3bLowMtbmin"]
    Regions += ["c0l2RCTTMass7j3bHighMtbmin"]
    Regions += ["c0l2RCTTMass7j4b"]
    Regions += ["c0l0RCTTMass7j2b"]
    Regions += ["c0l0RCTTMass7j3b"]
    Regions += ["c0l0RCTTMass7j4b"]
    
    Regions += ["c0l1RCTTMass6j2b"]
    Regions += ["c0l1RCTTMass6j3b"]
    Regions += ["c0l1RCTTMass6j4b"]
    Regions += ["c0l2RCTTMass6j2b"]
    Regions += ["c0l2RCTTMass6j3b"]
    Regions += ["c0l2RCTTMass6j4b"]
    Regions += ["c0l0RCTTMass6j2b"]
    Regions += ["c0l0RCTTMass6j3b"]
    Regions += ["c0l0RCTTMass6j4b"]




start =time.time()
print "START"
if(os.path.isdir(outputDir)):
    print "Directory already exists"
else:
    os.system("mkdir -p "+outputDir)

listfiles = glob.glob(inputDir+"/*.root")
proclistfiles = glob.glob(outputDir+"/*.root")
for s,sample in enumerate(listfiles):
    if any(sample[len(inputDir):] in proc for proc in proclistfiles):
        continue
    print sample
    if s % 200 == 0:
        print "Files processed = ",s
    oldfile=root.TFile.Open(sample,"READ")
    #print sample[len(inputDir):]
    newfile=root.TFile.Open(outputDir+"/"+sample[len(inputDir):],"RECREATE")
    
    #print sample
    oldfile.cd()
    listhisto=oldfile.GetListOfKeys()
    #print listhisto
    for histogram in listhisto:
        
        if any(r in histogram.GetName() for r in Regions):
            if any(v in histogram.GetName() for v in varlist):
                histo=oldfile.Get(histogram.GetName())
                histo.SetDirectory(0)
                #print histogram.GetName()
        
                newfile.cd()
                histo.Write()        
        
    oldfile.Close()
    newfile.Close()
        
elapsed=time.time()-start
elapsed=elapsed/60
print elapsed, " minuti trascorsi"
print "END"
