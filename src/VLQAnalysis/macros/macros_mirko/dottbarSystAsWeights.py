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
    "_PowHer",         
    "_radHi",  
    "_radLow",      
    "_aMCHer",       

    ] 

systtypes=[

    "_PS",       
    "_radHi",  
    "_radLow",       
    "_GENPS",       
    
]

MEfiles=[
    "ttlight_PowHer", 
    "ttbb_PowHer", 
    "ttcc_PowHer", 
     ]




Regions = []
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

    "ttlight",
    "ttbb",
    "ttcc",

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

newfile=root.TFile.Open(outputDir+"/"+"TtbarSystematicsWeights.root","RECREATE")
for v,var in enumerate(varlist):
    for s,sample in enumerate(samples):
        pp6file=root.TFile.Open(inputDir+"/ForSystematics_TRF/"+sample+".root")
        pp6filedt=root.TFile.Open(inputDir+"/ForSystematics_DirectTagging/"+sample+".root")
        pphfile=root.TFile.Open(inputDir+"/ForSystematics_TRF/"+sample+"_PowHer.root")
        pphfiledt=root.TFile.Open(inputDir+"/ForSystematics_DirectTagging/"+sample+"_PowHer.root")
        for i,VarFiles in enumerate(files):
            varfile=root.TFile.Open(inputDir+"/ForSystematics_TRF/"+sample+VarFiles+".root")
            varfiledt=root.TFile.Open(inputDir+"/ForSystematics_DirectTagging/"+sample+VarFiles+".root")
                
            for r,reg in enumerate(Regions): 
                pp6file.cd()
                pp6histo=pp6file.Get(reg+"_"+var)
                pp6histo.SetDirectory(0)
                

                pp6filedt.cd()
                pp6histodt=pp6filedt.Get(reg+"_"+var)
                pp6histodt.SetDirectory(0)
            
                pp6errtrf=root.Double()
                pp6errdt=root.Double()
                intpp6trf=pp6histo.IntegralAndError(0,-1,pp6errtrf)
                intpp6dt=pp6histodt.IntegralAndError(0,-1,pp6errdt)
                pp6R=intpp6dt/intpp6trf
                pp6eps=math.sqrt(((pp6errdt*pp6errdt)/(intpp6trf*intpp6trf))+(((intpp6dt*intpp6dt)/(intpp6trf*intpp6trf*intpp6trf*intpp6trf))*(pp6errtrf*pp6errtrf)))

                if (pp6histodt.GetEntries()>(0.1*pp6histo.GetEntries()) and pp6histodt.Integral()!=0):
                    if pp6R > (1 +2*pp6eps):
                        pp6histo.Scale(pp6R)
                    if pp6R < (1-2*pp6eps):
                        pp6histo.Scale(pp6R)
    
                varfile.cd()
                varhisto=varfile.Get(reg+"_"+var)
                varhisto.SetDirectory(0)
                
                varfiledt.cd()
                varhistodt=varfiledt.Get(reg+"_"+var)
                varhistodt.SetDirectory(0)


                varerrtrf=root.Double()
                varerrdt=root.Double()
                intvartrf=varhisto.IntegralAndError(0,-1,varerrtrf)
                intvardt=varhistodt.IntegralAndError(0,-1,varerrdt)
                varR=intvardt/intvartrf
                vareps=math.sqrt(((varerrdt*varerrdt)/(intvartrf*intvartrf))+(((intvardt*intvardt)/(intvartrf*intvartrf*intvartrf*intvartrf))*(varerrtrf*varerrtrf)))

                if (varhistodt.GetEntries()>(0.1*varhisto.GetEntries()) and varhistodt.Integral()!=0):
                    if varR > (1 +2*vareps):
                        varhisto.Scale(varR)
                    if varR < (1-2*vareps):
                        varhisto.Scale(varR)


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
                numhisto2=numhisto.Clone()

                numhisto.Divide(denhisto)

                
                newfile.cd()
                numhisto.Write(reg+"_"+var+"_"+sample+systtypes[i])
                
                if "aMCHer" in VarFiles:
                    pphfile.cd()
                    pphhisto=pphfile.Get(reg+"_"+var)
                    pphhisto.SetDirectory(0)
                    
                    pphfiledt.cd()
                    pphhistodt=pphfiledt.Get(reg+"_"+var)
                    pphhistodt.SetDirectory(0)


                    ppherrtrf=root.Double()
                    ppherrdt=root.Double()
                    intpphtrf=pphhisto.IntegralAndError(0,-1,ppherrtrf)
                    intpphdt=pphhistodt.IntegralAndError(0,-1,ppherrdt)
                    pphR=intpphdt/intpphtrf
                    ppheps=math.sqrt(((ppherrdt*ppherrdt)/(intpphtrf*intpphtrf))+(((intpphdt*intpphdt)/(intpphtrf*intpphtrf*intpphtrf*intpphtrf))*(ppherrtrf*ppherrtrf)))

                    if (pphhistodt.GetEntries()>(0.1*pphhisto.GetEntries())):
                        if pphR > (1 +2*ppheps):
                            pphhisto.Scale(pphR)
                        if pphR < (1-2*ppheps):
                            pphhisto.Scale(pphR)


                    pphhisto2=pphhisto.Rebin(len(newbins)-1,"pphhisto2",newbins)
                
                    numhisto2.Divide(pphhisto2)
                    newfile.cd()
                    numhisto2.Write(reg+"_"+var+"_"+sample+"_GEN")
        
            varfile.Close()
            varfiledt.Close()

        pp6file.Close()
        pp6filedt.Close()
        pphfile.Close()
        pphfiledt.Close()

newfile.Close()
        
elapsed=time.time()-start
elapsed=elapsed/60
print elapsed, " minuti trascorsi"
print "END"
