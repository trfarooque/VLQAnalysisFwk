import ROOT as root
import time
import sys
from array import *
import math
import os


from optparse import OptionParser

##____________________________________________________
## Options
parser = OptionParser()
parser.add_option("-i","--inputDir",dest="inputDir",help="repository for the TRex files are located",action="store",default="")
parser.add_option("-o","--outputDir",dest="outputDir",help="repository where to put the modified files",action="store",default="NewTtbbFile/")

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


Regions = []
'''
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
'''

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



c1 = root.TCanvas("c1","c1",600,600)
leg = root.TLegend(0.56,0.70,0.77,0.91)
leg.SetFillColor(0)
leg.SetLineColor(0)
start =time.time()
print "START"
if(os.path.isdir(outputDir)):
    print "Directory already exists"
else:
    os.system("mkdir -p "+outputDir)

newfile=root.TFile.Open(outputDir+"/"+"ttbarbb_modified.root","RECREATE")
for v,var in enumerate(varlist):
    ttbbfile=root.TFile.Open(inputDir+"ttbarbb.root")

    for r,reg in enumerate(Regions): 
        ttbbfile.cd()
        nominalhisto=ttbbfile.Get(reg+"_"+var)
        nominalhisto.SetDirectory(0)
        
        
        aMcMgHpphisto=ttbbfile.Get(reg+"_"+var+"_weight_ttbb_aMcAtNloHpp")
        aMcMgHpphisto.SetDirectory(0)
        
        
        aMcMgPy8histo=ttbbfile.Get(reg+"_"+var+"_weight_ttbb_aMcAtNloPy8")
        aMcMgPy8histo.SetDirectory(0)
        
        
        denhisto=aMcMgPy8histo.Clone(aMcMgPy8histo.GetName()+"clone")
        numhisto=aMcMgHpphisto.Clone(aMcMgHpphisto.GetName()+"clone")
        
        
        numhisto.Divide(denhisto)
        numhisto.Multiply(nominalhisto)        

        newfile.cd()
        numhisto.Write(reg+"_"+var+"_weight_ttbb_aMcAtNloPS")        
        
    ttbbfile.Close()
        

newfile.Close()
        
elapsed=time.time()-start
elapsed=elapsed/60
print elapsed, " minuti trascorsi"
print "END"
