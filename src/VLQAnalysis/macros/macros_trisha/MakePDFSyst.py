import ROOT as root
#from ROOT import *
import time
import sys
from array import *
import math
import os
import numpy
import lhapdf

from optparse import OptionParser

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )
from regions_dictionary_sVLQ import *

pset = lhapdf.getPDFSet("PDF4LHC15_nlo_30")

##____________________________________________________
## Options
parser = OptionParser()
parser.add_option("--inputDir",dest="inputDir",help="repository for the TRex files are located",action="store",default="")
parser.add_option("--outputDir",dest="outputDir",help="repository where to put the modified files",action="store",default="NewPDFSystFile/")
parser.add_option("--doLepton",dest="doLepton",help="consider 1L regions",action="store_true",default=True)
parser.add_option("--doZeroLepton",dest="doZeroLepton",help="consider 0L regions",action="store_true",default=False)
parser.add_option("--allRegions",dest="allRegions",help="Use all regions",action="store_true",default=False)
parser.add_option("--doSR",dest="doSR",help="Use signal regions",type=int,action="store",default=1)
parser.add_option("--doVR",dest="doVR",help="Use validation regions",type=int,action="store",default=1)
parser.add_option("--doPresel",dest="doPresel",help="Use preselection regions",type=int,action="store",default=0)
parser.add_option("--otherVariables",dest="otherVariables",help="Do variables other than meff",action="store_true",default=False)
parser.add_option("--sample",dest="sample",help="Name of sample to process",action="store",default="ttbarlight")

(options, args) = parser.parse_args()

outputDir=options.outputDir
inputDir=options.inputDir
doLepton=options.doLepton
doZeroLepton=options.doZeroLepton
allRegions=options.allRegions
doSR=options.doSR
doVR=options.doVR
doPresel=options.doPresel
otherVariables=options.otherVariables
sample=options.sample


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
      "RCMV_jets_n",
      "RCMHiggs_jets_n",
      "RCMTop_jets_n",
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
       if( doSR ):
          Regions += fit_regions_1l
       if( doVR ):
          Regions += validation_regions_1l
       if( doPresel ):
          Regions += preselection_regions_1l
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
start =time.time()
print "START"
if(os.path.isdir(outputDir)):
    print "Directory already exists"
else:
    os.system("mkdir -p "+outputDir)


##________________________________________________________
## List of weight variations

weight_list=["weight_pmg_PDFset90901",
             "weight_pmg_PDFset90902",
             "weight_pmg_PDFset90903",
             "weight_pmg_PDFset90904",
             "weight_pmg_PDFset90905",
             "weight_pmg_PDFset90906",
             "weight_pmg_PDFset90907",
             "weight_pmg_PDFset90908",
             "weight_pmg_PDFset90909",
             "weight_pmg_PDFset90910",
             "weight_pmg_PDFset90911",
             "weight_pmg_PDFset90912",
             "weight_pmg_PDFset90913",
             "weight_pmg_PDFset90914",
             "weight_pmg_PDFset90915",
             "weight_pmg_PDFset90916",
             "weight_pmg_PDFset90917",
             "weight_pmg_PDFset90918",
             "weight_pmg_PDFset90919",
             "weight_pmg_PDFset90920",
             "weight_pmg_PDFset90921",
             "weight_pmg_PDFset90922",
             "weight_pmg_PDFset90923",
             "weight_pmg_PDFset90923",
             "weight_pmg_PDFset90924",
             "weight_pmg_PDFset90925",
             "weight_pmg_PDFset90926",
             "weight_pmg_PDFset90927",
             "weight_pmg_PDFset90928",
             "weight_pmg_PDFset90929",
             "weight_pmg_PDFset90930"]

os.system("cp "+inputDir+"/"+sample+".root "+outputDir+"/"+sample+".root ")
sampleFile=root.TFile.Open(outputDir+"/"+sample+".root", "UPDATE")

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

        nominalhisto=sampleFile.Get(reg+"_"+var)
        nominalhisto.SetDirectory(0)
        binning=region['binning']
        xbins = numpy.array(binning.split(','),float)
        xbins=numpy.insert(xbins,0,0.)
        nominalhisto=nominalhisto.Rebin(len(xbins)-1,"",xbins)
                   
        uphisto=nominalhisto.Clone()
        uphisto.SetDirectory(0)
        uphisto.SetName(reg+"_"+var+"_weight_PDF_up")

        downhisto=nominalhisto.Clone()
        downhisto.SetDirectory(0)
        downhisto.SetName(reg+"_"+var+"_weight_PDF_down")

        list_pdf = []
        for weight_var in weight_list:
           weightvarhisto=sampleFile.Get(reg+"_"+var+"_"+weight_var)
           weightvarhisto.SetDirectory(0)
           weightvarhisto=weightvarhisto.Rebin(len(xbins)-1,"",xbins)
           sampleFile.cd()
           weightvarhisto.Write()
           list_pdf.append(weightvarhisto)
           
        for iBin in range(1,nominalhisto.GetNbinsX()+1):
           #PDF manipulation part
           value = nominalhisto.GetBinContent(iBin)
           pdfvars = numpy.array([h_pdf.GetBinContent(iBin) for h_pdf in list_pdf])
           pdfsym = pset.uncertainty(pdfvars).errsymm
           uphisto.SetBinContent(iBin,value+pdfsym)
           downhisto.SetBinContent(iBin,value-pdfsym)
           

#              bc_w = weightvarhisto.GetBinContent(i)
#              if( bc_w < downhisto.GetBinContent(i) ):
#                 downhisto.SetBinContent(i,bc_w)
#              if( bc_w > uphisto.GetBinContent(i) ):
#                 uphisto.SetBinContent(i,bc_w)

        sampleFile.cd()
        uphisto.Write()
        downhisto.Write()
        nominalhisto.Write()

sampleFile.Close()
        
elapsed=time.time()-start
elapsed=elapsed/60
print elapsed, " minuti trascorsi"
print "END"
