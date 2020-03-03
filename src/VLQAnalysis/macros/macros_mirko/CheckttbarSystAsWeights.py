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
parser.add_option("-s","--sourceFile",dest="sourceFile",help="path to source file",action="store",default="")
parser.add_option("-c","--checkFile",dest="checkFile",help="path file to be checked",action="store",default="")
parser.add_option("-t","--Threshold",dest="Threshold",help="relative error threshold to rebin",action="store",default="10")
(options, args) = parser.parse_args()

sourceFilepath=options.sourceFile
checkFilepath=options.checkFile
threshold=float(options.Threshold)/100.

print "SETUP DIFFERENCE BIN THRESHOLD = ", threshold

root.gROOT.SetBatch(1)
root.gStyle.SetOptTitle(0)
root.gStyle.SetPalette(1)
root.TH1.SetDefaultSumw2(1)
root.gStyle.SetOptStat(0)


varlist=[ 
    "meff",
    ]




systtypes=[

    "_PS",       
    "_radHi",  
    "_radLow",       
    "_GENPS",       
    
]
samples=[

    "ttbarlight",
    "ttbarbb",
    "ttbarcc",

]



Regions = []
Regions += ["c1l1TTRCLooser6j2b"]
Regions += ["c1l1TTRCLooser6j3bLowMbb"]
Regions += ["c1l1TTRCLooser6j3bHighMbb"]
Regions += ["c1l1TTRCLooser6j4bLowMbb"]
Regions += ["c1l1TTRCLooser6j4bHighMbb"]
Regions += ["c1l2TTRCLooser6j2b"]
Regions += ["c1l2TTRCLooser6j3b"]
Regions += ["c1l2TTRCLooser6j4b"]
Regions += ["c1l0TTRCLooser6j2b"]
Regions += ["c1l0TTRCLooser6j3b"]
Regions += ["c1l0TTRCLooser6j4b"]
Regions += ["c1l1TTRCLooser5j2b"]
Regions += ["c1l1TTRCLooser5j3b"]
Regions += ["c1l1TTRCLooser5j4b"]
Regions += ["c1l2TTRCLooser5j2b"]
Regions += ["c1l2TTRCLooser5j3b"]
Regions += ["c1l2TTRCLooser5j4b"]
Regions += ["c1l0TTRCLooser5j2b"]
Regions += ["c1l0TTRCLooser5j3b"]
Regions += ["c1l0TTRCLooser5j4b"]




c1 = root.TCanvas("c1","c1",600,600)
leg = root.TLegend(0.56,0.70,0.77,0.91)
leg.SetFillColor(0)
leg.SetLineColor(0)
start =time.time()
print "START"

sourcefile=root.TFile.Open(sourceFilepath)
checkfile=root.TFile.Open(checkFilepath)
print sourceFilepath,checkFilepath
for v,var in enumerate(varlist):
    for s,sample in enumerate(samples):
        for t,syst in enumerate(systtypes):
            for r,reg in enumerate(Regions): 
            
                sourcefile.cd()
                #print reg+"_"+var+"_"+sample+"_"+syst
                shisto=sourcefile.Get(reg+"_"+var+"_"+sample+syst)
                shisto.SetDirectory(0)
                

                checkfile.cd()
                chisto=checkfile.Get(reg+"_"+var+"_"+sample+syst)
                chisto.SetDirectory(0)
            
                if(shisto.GetNbinsX()!=chisto.GetNbinsX()):
                    print "DIFFERENT NUMBER OF BINS ===> "
                    print "REGION = ",reg
                    print "SAMPLE = ",sample," SYST = ",syst
                    print "NBIN 1 = ",shisto.GetNbinsX(),"  NBIN 2 = ",chisto.GetNbinsX()
                
                else:    
                    for bin in range(1,shisto.GetNbinsX()+1):
                        
                        binc=shisto.GetBinContent(bin)
                        binc2=chisto.GetBinContent(bin)
                        if binc2!=0 and binc!=0:
                            if(abs(1-(binc/binc2))>threshold):
                                print "SCALING DOING SOMETHING WEIRD CHACK ===> "
                                print "REGION = ",reg
                                print "SAMPLE = ",sample," SYST = ",syst
                                print "BIN NUMBER = ",bin
                                print "CONTENT 1 = ",binc, "CONTENT 2 = ",binc2
                        else:
                            if(binc!=binc2):
                                print "SOMETHING SUPERWEIRD IS HAPPENING CHACK ===> "
                                print "REGION = ",reg
                                print "SAMPLE = ",sample," SYST = ",syst
                                print "BIN NUMBER = ",bin
                                print "CONTENT 1 = ",binc, "  CONTENT 2 = ",binc2
        

sourcefile.Close()
checkfile.Close()
  
elapsed=time.time()-start
elapsed=elapsed/60
print elapsed, " minuti trascorsi"
print "END"
