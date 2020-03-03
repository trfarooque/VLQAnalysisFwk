import ROOT as root
#from ROOT import *
import time
import sys
from array import *
import math
import os

from optparse import OptionParser

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )
from regions_dictionary import *

##____________________________________________________
## Options
parser = OptionParser()
parser.add_option("-i","--inputDir",dest="inputDir",help="repository for the TRex files are located",action="store",default="")
parser.add_option("-o","--outputDir",dest="outputDir",help="repository where to put the modified files",action="store",default="NewTtbbFile/")
parser.add_option("-l","--doLepton",dest="doLepton",help="consider 1L regions",action="store_true",default=False)
parser.add_option("-z","--doZeroLepton",dest="doZeroLepton",help="consider 0L regions",action="store_true",default=False)
parser.add_option("-r","--allRegions",dest="allRegions",help="Use all regions",action="store_true",default=False)
parser.add_option("-v","--otherVariables",dest="otherVariables",help="Do variables other than meff",action="store_true",default=False)

(options, args) = parser.parse_args()

outputDir=options.outputDir
inputDir=options.inputDir
doLepton=options.doLepton
doZeroLepton=options.doZeroLepton
allRegions=options.allRegions
otherVariables=options.otherVariables

root.gROOT.SetBatch(1)
root.gStyle.SetOptTitle(0)
root.gStyle.SetPalette(1)
root.TH1.SetDefaultSumw2(1)
root.gStyle.SetOptStat(0)

varlist=[ 
    "meff",
]

if otherVariables:

   varlist +=[
      "jets_n",
      "bjets_n",
      #    "met",
      #    "met_zoom",
      #    "jet0_pt",
      "RCMHiggs_jets_n",
      "RCMTop_jets_n",
      "mtbmin_zoom",
      #    "RCjet0_m",
      #    "RCjet0_pt",
      #    "RCjets_m",
      #    "RCjets_pt",
      #    "lep0_pt_zoom",
      #    "lep0_eta",
      #    "mtw_zoom"
    ]
   
##________________________________________________________
## Defining the list of regions to look at
Regions = []
if allRegions:
   Regions = [{'name':"all"}]
else:
    if( doLepton ):
        #
        # Control/signal regions
        #
        Regions += all_regions_1l
    if( doZeroLepton ):
        #
        # Validation regions
        #
        Regions += all_regions_0l


##________________________________________________________
## Small utility for histograms list
def GetKeyNames( iF, dir = "" ):
    iF.cd(dir)
    return [key.GetName() for key in root.gDirectory.GetListOfKeys()]
##........................................................


##........................................................
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

os.system("cp "+inputDir+"/ttbarbb.root "+outputDir+"/ttbarbb.root ")
ttbbfile=root.TFile.Open(outputDir+"/"+"ttbarbb.root", "UPDATE")

if len(Regions)==1 and Regions[0]['name'].upper()=="ALL":
    keyList = GetKeyNames(ttbbfile)
    Regions = []
    for key in keyList:
        if key.find("meff")==-1: continue
        if key.find("meff_")>-1: continue
        temp_region = key.replace("_meff","")
        Regions += [{'name':temp_region}]

for v,var in enumerate(varlist):

    for r,region in enumerate(Regions): 

        reg = region['name'].replace("HTX_","")

        print "Get histogram ",reg+"_"+var
        nominalhisto=ttbbfile.Get(reg+"_"+var)
        nominalhisto.SetDirectory(0)
        
        aMcMgHpphisto=ttbbfile.Get(reg+"_"+var+"_weight_ttbb_aMcAtNloHpp")
        aMcMgHpphisto.SetDirectory(0)
        
        aMcMgPy8histo=ttbbfile.Get(reg+"_"+var+"_weight_ttbb_aMcAtNloPy8")
        aMcMgPy8histo.SetDirectory(0)
        
        denhisto=aMcMgPy8histo.Clone(aMcMgPy8histo.GetName()+"clone")
        numhisto=aMcMgHpphisto.Clone(reg+"_"+var+"_weight_ttbb_aMcAtNloPS") 
        numhisto.Divide(denhisto)
        numhisto.Multiply(nominalhisto)        

        ttbbfile.cd()
        numhisto.Write()
        
ttbbfile.Close()
        
elapsed=time.time()-start
elapsed=elapsed/60
print elapsed, " minuti trascorsi"
print "END"
