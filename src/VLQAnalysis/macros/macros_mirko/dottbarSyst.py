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
parser.add_option("-o","--outputDir",dest="outputDir",help="repository where to put the modified files",action="store",default="SystematicFilesforTRexFitter")
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


files=[
    "ttbarlight_PowHer",  
    "ttbarbb_PowHer",  
    "ttbarcc_PowHer",       
    ] 

DFfiles=[
    "ttbarlight_aMCHer", 
    "ttbarbb_aMCHer", 
    "ttbarcc_aMCHer", 
     ]

Totfiles=DFfiles

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


PowPytfiles=[

    "tbartlight",
    "tbartbb",
    "tbartcc",

]




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


for v,var in enumerate(varlist):
    ratiolist=[]
    for reg in Regions: 
        regratio=[]
        
        for i,VarFiles in enumerate(files):
            
            varfile=root.TFile.Open(inputDir+"/"+VarFiles+".root")
            varhisto=varfile.Get(reg+"_"+var)
            varhisto.SetDirectory(0)
            varfile.Close()
            denhisto=varhisto.Clone()
            mefile=root.TFile.Open(inputDir+"/"+DFfiles[i]+".root")
            mehisto=mefile.Get(reg+"_"+var)
            mehisto.SetDirectory(0)
            mefile.Close()
            mehisto.Divide(denhisto)
            regratio.append(mehisto)
        ratiolist.append(regratio)

    
    for k,npf in enumerate(PowPytfiles):
        oldfile = root.TFile.Open(inputDir+"/"+npf+".root")
        
        newfile=root.TFile.Open(outputDir+"/"+npf+Totfiles[k][-7:]+".root","RECREATE")
            
        for r,ch in enumerate(Regions):
            variedhisto=oldfile.Get(ch+"_"+var)
            variedhisto.SetDirectory(0)

                
            newfile.cd()
            #print ratiolist[r][k].GetName()
            variedhisto.Multiply(ratiolist[r][k])
            variedhisto.Write()
            
            
        newfile.Close()    


        oldfile.Close()
        

elapsed=time.time()-start
elapsed=elapsed/60
print elapsed, " minuti trascorsi"
print "END"
