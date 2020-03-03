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
parser.add_option("-t","--Threshold",dest="Threshold",help="relative error threshold to rebin",action="store",default="10")
(options, args) = parser.parse_args()

outputDir=options.outputDir
inputDir=options.inputDir
threshold=float(options.Threshold)/100.

print "SETUP REBIN THRESHOLD = ", threshold

root.gROOT.SetBatch(1)
root.gStyle.SetOptTitle(0)
root.gStyle.SetPalette(1)
root.TH1.SetDefaultSumw2(1)
root.gStyle.SetOptStat(0)


varlist=[ 
    "meff",
    ]


files=[
    "_radHi",  
    "_radLo",
    "_PowHer",         
    "_DiagSub",       

    ] 

systtypes=[
    "_radHi",  
    "_radLow",
    "_PS",       
    "_DS",       
    
]


Regions = []
Regions += ["c1l2b"]
Regions += ["c0l2b"]
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


#0lep

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




Binning=[

    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2200,2400,2600,3000]),
    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2500,3000]),
    array("d",[0,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2500,3000]),
    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1800,2000,2500,3000]),
    array("d",[0,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2200,2400,2600,3000]),
    array("d",[0,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2200,2400,2600,3000]),
    array("d",[0,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2200,2400,2600,3000]),

    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2200,2400,2600,3000]),
    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2500,3000]),
    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2200,2400,2600,3000]),
    array("d",[0,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2200,2400,2600,3000]),
    array("d",[0,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2200,2400,2600,3000]),

    #0lep

    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2200,2400,2600,3000]),
    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2500,3000]),
    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2500,3000]),
    array("d",[0,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2500,3000]),
    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1800,2000,2500,3000]),
    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1800,2000,2500,3000]),
    array("d",[0,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2200,2400,2600,3000]),
    array("d",[0,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2200,2400,2600,3000]),
    array("d",[0,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2200,2400,2600,3000]),

    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2200,2400,2600,3000]),
    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2500,3000]),
    array("d",[0,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,900,1000,1100,1200,1300,1400,1500,1600,1800,2000,2500,3000]),
    array("d",[0,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2200,2400,2600,3000]),
    array("d",[0,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2200,2400,2600,3000]),
    array("d",[0,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2200,2400,2600,3000]),


]

samples=[

    "Singletop_schan",
    "Singletop_tchan",
    "Singletop_Wtprod",

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

newfile=root.TFile.Open(outputDir+"/"+"SingletopSystematicsWeights.root","RECREATE")
for v,var in enumerate(varlist):
    for s,sample in enumerate(samples):
        pp6file=root.TFile.Open(inputDir+"/ForSystematics_TRF/"+sample+".root")
        
        for i,VarFiles in enumerate(files):
            if s==0 and i>1:
                continue
            elif s==1 and i>2:
                continue
            varfile=root.TFile.Open(inputDir+"/ForSystematics_TRF/"+sample+VarFiles+".root")
                        
            for r,reg in enumerate(Regions): 
                pp6file.cd()
                pp6histo=pp6file.Get(reg+"_"+var)
                pp6histo.SetDirectory(0)
                                  
    
                varfile.cd()
                varhisto=varfile.Get(reg+"_"+var)
                varhisto.SetDirectory(0)
                
               
                rebinning=True
                newbins=array('d')
                while(rebinning):
                    del newbins[:]
                    rebinning=False
                    #print "COntrollo = ",varhisto.GetNbinsX()+1
                    for bin in range(1,varhisto.GetNbinsX()+1):
                        if bin==1:
                            newbins.append(varhisto.GetBinLowEdge(bin))
                            continue
                        binc=varhisto.GetBinContent(bin)
                        bine=varhisto.GetBinError(bin)

                        
                        if binc==0:
                            rebinning=True
                        elif (abs(bine/binc) > threshold) :
                            rebinning=True
                        else:
                            if bin==2:
                                bin1ccheck=varhisto.GetBinContent(1)
                                bin1echeck=varhisto.GetBinError(1)
                                if bin1ccheck==0:
                                    rebinning=True
                                elif (abs(bin1echeck/bin1ccheck) > threshold):
                                    rebinning=True
                                else:
                                    newbins.append(varhisto.GetBinLowEdge(bin))
                            else:
                                newbins.append(varhisto.GetBinLowEdge(bin))

                        if bin == varhisto.GetNbinsX() and varhisto.GetBinLowEdge(bin+1) not in newbins:
                            newbins.append(varhisto.GetBinLowEdge(bin+1))

                    if len(newbins) < 3:
                        rebinning=False
                        #print "Regione = ", reg, " Sample = ",sample, " VarFiles = ",VarFiles
                        #print "controllo 2 = ", newbins, "  lunghezza = ",len(newbins)
                    varhisto=varhisto.Rebin(len(newbins)-1,"",newbins)

                denhisto=pp6histo.Rebin(len(newbins)-1,"denhisto",newbins)

                numhisto=varhisto.Clone()
                if reg=="c1l2b" or reg=="c0l2b":
                    print "reg ",reg,"  syst  ",systtypes[i]
                    print "Interesting ratio = ",numhisto.Integral()/denhisto.Integral()
                    err1=root.Double()
                    err2=root.Double()
                    print "Integrali = ",numhisto.IntegralAndError(0,-1,err1),"  ",denhisto.IntegralAndError(0,-1,err2)
                    print "Err = ",err1,"   ",err2
                numhisto.Divide(denhisto)

                
                newfile.cd()
                numhisto.Write(reg+"_"+var+"_"+sample+systtypes[i])
                
            varfile.Close()
            

        pp6file.Close()

newfile.Close()
        
elapsed=time.time()-start
elapsed=elapsed/60
print elapsed, " minuti trascorsi"
print "END"
