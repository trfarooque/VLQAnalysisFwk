import ROOT as root
import time
import sys
from array import *
import math
import os
from ROOT import *

from optparse import OptionParser

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )
from regions_dictionary import *

##______________________________________________________________________________
## Options
parser = OptionParser()
parser.add_option("-i","--inputDir",dest="inputDir",help="repository for the TRex files are located",action="store",default="")
parser.add_option("-o","--outputDir",dest="outputDir",help="repository where to put the modified files",action="store",default="SystematicFilesforTRexFitter")
parser.add_option("-t","--threshold",dest="threshold",help="relative error threshold to rebin (in %)",action="store",default="10")
parser.add_option("-v","--variables",dest="variables",help="name of the variables to use (default: meff)",action="store",default="meff")
parser.add_option("-a","--allRegions1L",dest="allRegions1L",help="use all regions in the root file for 1-lepton regions, or only the ones in the dictionary",action="store_true",default=False)
parser.add_option("-z","--allRegions0L",dest="allRegions0L",help="use all regions in the root file for 0-lepton regions, or only the ones in the dictionary",action="store_true",default=False)
parser.add_option("-p","--printPlots",dest="printPlots",help="dump png plots to control the shapes",action="store_true",default=False)
parser.add_option("-w","--printWarnings",dest="printWarnings",help="print warnings in case of low stat",action="store_true",default=False)
parser.add_option("-m","--mergeRegions",dest="mergeRegions",help="merge regions",action="store_true",default=False)

(options, args) = parser.parse_args()
outputDir=options.outputDir
inputDir=options.inputDir
threshold=float(options.threshold)/100.
variables=options.variables
allRegions1L=options.allRegions1L
allRegions0L=options.allRegions0L
printPlots=options.printPlots
printWarnings=options.printWarnings
mergeRegions=options.mergeRegions

print ("inputDir = ", inputDir)
##______________________________________________________________________________
## ROOT options
root.gROOT.SetBatch(1)
root.gStyle.SetOptTitle(0)
root.gStyle.SetPalette(1)
root.TH1.SetDefaultSumw2(1)
root.gStyle.SetOptStat(0)
##..............................................................................

##______________________________________________________________________________
## Small utility for histograms list
def GetKeyNames( iF, dir = "" ):
    iF.cd(dir)
    return [key.GetName() for key in gDirectory.GetListOfKeys()]
##..............................................................................

varlist=variables.split(",")

files=[
"PowHer",
"radHi",
"radLow",
"aMCHer"
]

systtypes=[
"_PS",
"_radHi",
"_radLow",
"_GENPS"
]

#By default, loading the list of regions in the
#input nominal rootfile ... Expected to be something
#quite independant on the parameters used to process
#the inputs
Regions = []
if not allRegions0L:
    for reg in all_regions_0l:
        Regions += [reg['name'].replace("HTX_","")]
else:
    input_file = root.TFile.Open(inputDir+"/ttbarbb.root")
    keyList = GetKeyNames(input_file)
    for key in keyList:
        if(key.find("0lep")==-1):
            continue
        splitted_key = key.split("_")
        if key.find("1Tex2Hin")>-1:
            continue
        if key.find("2Tin1Hex")>-1:
           continue
        if key.find("2Tin2Hin")>-1:
            continue
        if key.find("3THin")>-1:
            continue
        if key.find("0Tex2Hin")>-1:
            continue
        if ("meff" in splitted_key) and not ("vs" in splitted_key) and not ("zoom" in splitted_key) :
            temp_region = key.replace("_meff","")
            Regions += [temp_region]


if not allRegions1L:
    for reg in all_regions_1l:
        Regions += [reg['name'].replace("HTX_","")]
else:
    input_file = root.TFile.Open(inputDir+"/ttbarbb.root")
    keyList = GetKeyNames(input_file)
    for key in keyList:
        if(key.find("1lep")==-1):
            continue
        splitted_key = key.split("_")
        if key.find("1Tex2Hin")>-1:
            continue
        if key.find("2Tin1Hex")>-1:
           continue
        if key.find("2Tin2Hin")>-1:
            continue
        if key.find("3THin")>-1:
            continue
        if key.find("0Tex2Hin")>-1:
            continue
        if ("meff" in splitted_key) and not ("vs" in splitted_key) and not ("zoom" in splitted_key) :
            temp_region = key.replace("_meff","")
            Regions += [temp_region]

samples=[
"ttbarlight",
"ttbarbb",
"ttbarcc",
]

#Merged regions
merged_regions = []
##
merged_regions += [{'name':"c0lep1Tex1Hex6jex2bexHighMtbmin",'samples':"ttbarlight",'systs':["aMCHer",],'to_add':["c0lep1Tex1Hex6jex2bexLowMtbmin","c0lep1Tex1Hex6jex2bexHighMtbmin"]}]
merged_regions += [{'name':"c0lep1Tex1Hex6jex2bexHighMtbmin",'samples':"ttbarbb",'systs':["PowHer","radHi","radLow","aMCHer"],'to_add':["c0lep1Tex1Hex6jex2bexLowMtbmin","c0lep1Tex1Hex6jex2bexHighMtbmin","c0lep2THin6jex3bex"]}]
merged_regions += [{'name':"c0lep1Tex1Hex6jex2bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer",],'to_add':["c0lep0Tex1Hex6jex2bexHighMtbmin","c0lep1Tex1Hex6jex2bexLowMtbmin","c0lep1Tex1Hex6jex2bexHighMtbmin","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
##
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bexHighMtbmin",'samples':"ttbarlight",'systs':["aMCHer",],'to_add':["c0lep2Tin0_1Hwin6jex2bexLowMtbmin","c0lep2Tin0_1Hwin6jex2bexHighMtbmin","c0lep2THin6jex3bex"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bexHighMtbmin",'samples':"ttbarbb",'systs':["PowHer","radHi","radLow","aMCHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bexLowMtbmin","c0lep2Tin0_1Hwin6jex2bexHighMtbmin","c0lep0Tin2Hin6jex2bex","c0lep1Tex1Hex6jex2bex","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bexHighMtbmin",'samples':"ttbarcc",'systs':["PowHer","radHi","radLow","aMCHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bexLowMtbmin","c0lep2Tin0_1Hwin6jex2bexHighMtbmin","c0lep0Tin2Hin6jex2bex","c0lep1Tex1Hex6jex2bex","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
##
merged_regions += [{'name':"c0lep2THin6jex3bexHighMtbmin",'samples':"ttbarlight",'systs':["aMCHer",],'to_add':["c0lep2THin6jex3bexLowMtbmin","c0lep2THin6jex3bexHighMtbmin"]}]
merged_regions += [{'name':"c0lep2THin6jex3bexHighMtbmin",'samples':"ttbarbb",'systs':["radHi","radLow","aMCHer"],'to_add':["c0lep2THin6jex3bexLowMtbmin","c0lep2THin6jex3bexHighMtbmin","c0lep2THin6jex4binLowMtbmin","c0lep2THin6jex4binHighMtbmin"]}]
merged_regions += [{'name':"c0lep2THin6jex3bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer",],'to_add':["c0lep2THin6jex3bexLowMtbmin","c0lep2THin6jex3bexHighMtbmin"]}]
##
merged_regions += [{'name':"c0lep0Tin2Hin6jex2bexHighMetSig",'samples':"ttbarlight",'systs':["aMCHer",],'to_add':["c0lep0Tin2Hin6jex2bexHighMetSig","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
merged_regions += [{'name':"c0lep0Tin2Hin6jex2bexHighMetSig",'samples':"ttbarbb",'systs':["PowHer","radHi","radLow","aMCHer"],'to_add':["c0lep0Tin2Hin6jex2bexHighMetSig","c0lep2Tin0_1Hwin6jex2bexHighMetSig","c0lep1Tex1Hex6jex2bexHighMetSig","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
merged_regions += [{'name':"c0lep0Tin2Hin6jex2bexHighMetSig",'samples':"ttbarcc",'systs':["aMCHer","radHi","radLow","PowHer"],'to_add':["c0lep0Tin2Hin6jex2bexHighMetSig","c0lep2Tin0_1Hwin6jex2bexHighMetSig","c0lep1Tex1Hex6jex2bexHighMetSig","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
##
merged_regions += [{'name':"c0lep0Tin2Hin6jex2bex",'samples':"ttbarlight",'systs':["aMCHer",],'to_add':["c0lep0Tin2Hin6jex2bex","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
merged_regions += [{'name':"c0lep0Tin2Hin6jex2bex",'samples':"ttbarbb",'systs':["PowHer","radHi","radLow","aMCHer"],'to_add':["c0lep0Tin2Hin6jex2bex","c0lep2Tin0_1Hwin6jex2bex","c0lep1Tex1Hex6jex2bex","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
merged_regions += [{'name':"c0lep0Tin2Hin6jex2bex",'samples':"ttbarcc",'systs':["aMCHer","radHi","radLow","PowHer"],'to_add':["c0lep0Tin2Hin6jex2bex","c0lep2Tin0_1Hwin6jex2bex","c0lep1Tex1Hex6jex2bex","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]

##
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bexLowMtbminHighMetSig",'samples':"ttbarlight",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bexLowMtbminHighMetSig","c0lep2Tin0_1Hwin6jex2bexHighMtbmin","c0lep2THin6jex3bex"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bexLowMtbminHighMetSig",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bexLowMtbminHighMetSig","c0lep0Tin2Hin6jex2bexLowMtbminHighMetSig","c0lep2THin6jex3bex","c0lep1Tex1Hex6jex2bexLowMtbminHighMetSig"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bexLowMtbminHighMetSig",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bexLowMtbminHighMetSig","c0lep2Tin0_1Hwin6jex2bexHighMtbmin","c0lep2THin6jex3bex"]}]
##
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bexLowMtbmin",'samples':"ttbarlight",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bexLowMtbmin","c0lep2Tin0_1Hwin6jex2bexHighMtbmin","c0lep2THin6jex3bex"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bexLowMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bexLowMtbmin","c0lep0Tin2Hin6jex2bexLowMtbmin","c0lep2THin6jex3bex","c0lep1Tex1Hex6jex2bexLowMtbmin"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bexLowMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bexLowMtbmin","c0lep2Tin0_1Hwin6jex2bexHighMtbmin","c0lep2THin6jex3bex"]}]

##
merged_regions += [{'name':"c0lep0Tex1Hex7jin2bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex7jin2bexHighMtbmin","c0lep0Tex1Hex7jin2bexLowMtbmin"]}]
merged_regions += [{'name':"c0lep0Tex1Hex7jin2bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex7jin2bexHighMtbmin","c0lep0Tex1Hex7jin2bexLowMtbmin"]}]
##
merged_regions += [{'name':"c0lep1Tex1Hex7jin2bex",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex7jin2bex","c0lep1Tex1Hex7jin3bex","c0lep2THin7jin4bin"]}]
##
merged_regions += [{'name':"c0lep1Tex1Hex7jin3bex",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex7jin2bex","c0lep1Tex1Hex7jin3bex","c0lep2THin7jin4bin"]}]
##
merged_regions += [{'name':"c0lep0Tex1Hex7jin4bin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex7jin2bex","c0lep0Tex1Hex7jin3bex","c0lep0Tex1Hex7jin4bin"]}]
##
merged_regions += [{'name':"c0lep1Tex1Hex7jin2bexLowMtbminHighMetSig",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex7jin2bexLowMtbminHighMetSig","c0lep1Tex1Hex7jin2bexHighMtbmin","c0lep1Tex1Hex7jin3bex","c0lep2THin7jin4bin","c0lep2Tin0_1Hwin7jin2bex","c0lep2Tin0_1Hwin7jin3bex"]}]
##
merged_regions += [{'name':"c0lep1Tex1Hex7jin2bexLowMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex7jin2bexLowMtbmin","c0lep1Tex1Hex7jin2bexHighMtbmin","c0lep1Tex1Hex7jin3bex","c0lep2THin7jin4bin","c0lep2Tin0_1Hwin7jin2bex","c0lep2Tin0_1Hwin7jin3bex"]}]
##
merged_regions += [{'name':"c0lep1Tex1Hex7jin2bexHighMtbmin",'samples':"ttbarlight",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex7jin2bexHighMtbmin","c0lep1Tex1Hex7jin2bexLowMtbmin"]}]
merged_regions += [{'name':"c0lep1Tex1Hex7jin2bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex7jin2bexHighMtbmin","c0lep1Tex1Hex7jin2bexLowMtbmin","c0lep1Tex1Hex7jin3bex","c0lep2THin7jin4bin"]}]
merged_regions += [{'name':"c0lep1Tex1Hex7jin2bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex7jin2bexHighMtbmin","c0lep1Tex1Hex7jin2bexLowMtbmin"]}]
##
merged_regions += [{'name':"c0lep2Tin0_1Hwin7jin2bexHighMtbmin",'samples':"ttbarlight",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin7jin2bexLowMtbmin","c0lep2Tin0_1Hwin7jin2bexHighMtbmin"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin7jin2bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin7jin2bexLowMtbmin","c0lep2Tin0_1Hwin7jin2bexHighMtbmin"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin7jin2bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin7jin2bexLowMtbmin","c0lep2Tin0_1Hwin7jin2bexHighMtbmin"]}]
##
merged_regions += [{'name':"c0lep0Tin2Hin7jin2bexHighMetSig",'samples':"ttbarlight",'systs':["aMCHer"],'to_add':["c0lep0Tin2Hin7jin2bexHighMetSig","c0lep2Tin0_1Hwin7jin2bex","c0lep0Tin2Hin7jin3bex","c0lep2THin7jin4bin"]}]
merged_regions += [{'name':"c0lep0Tin2Hin7jin2bexHighMetSig",'samples':"ttbarbb",'systs':["aMCHer","radHi","radLow"],'to_add':["c0lep0Tin2Hin7jin2bexHighMetSig","c0lep2Tin0_1Hwin7jin2bex","c0lep1Tex1Hex7jin2bex","c0lep0Tin2Hin7jin3bex","c0lep2THin7jin4bin"]}]
merged_regions += [{'name':"c0lep0Tin2Hin7jin2bexHighMetSig",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep0Tin2Hin7jin2bexHighMetSig","c0lep0Tin2Hin7jin3bex","c0lep2THin7jin4bin"]}]
##
merged_regions += [{'name':"c0lep0Tin2Hin7jin2bex",'samples':"ttbarlight",'systs':["aMCHer"],'to_add':["c0lep0Tin2Hin7jin2bex","c0lep2Tin0_1Hwin7jin2bex","c0lep0Tin2Hin7jin3bex","c0lep2THin7jin4bin"]}]
merged_regions += [{'name':"c0lep0Tin2Hin7jin2bex",'samples':"ttbarbb",'systs':["aMCHer","radHi","radLow"],'to_add':["c0lep0Tin2Hin7jin2bex","c0lep2Tin0_1Hwin7jin2bex","c0lep1Tex1Hex7jin2bex","c0lep0Tin2Hin7jin3bex","c0lep2THin7jin4bin"]}]
merged_regions += [{'name':"c0lep0Tin2Hin7jin2bex",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep0Tin2Hin7jin2bex","c0lep0Tin2Hin7jin3bex","c0lep2THin7jin4bin"]}]

##
merged_regions += [{'name':"c0lep0Tin2Hin7jin3bex",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep0Tin2Hin7jin2bex","c0lep0Tin2Hin7jin3bex","c0lep2Tin0_1Hwin7jin2bex","c0lep2Tin0_1Hwin7jin3bex","c0lep2THin7jin4bin"]}]
merged_regions += [{'name':"c0lep0Tin2Hin7jin3bex",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep0Tin2Hin7jin2bex","c0lep0Tin2Hin7jin3bex","c0lep2THin7jin4bin"]}]
##
merged_regions += [{'name':"c0lep0Tex1Hex7jin3bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex7jin3bexHighMtbmin","c0lep0Tex1Hex7jin3bexLowMtbmin"]}]
merged_regions += [{'name':"c0lep0Tex1Hex7jin3bexHighMtbmin",'samples':"ttbarlight",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex7jin3bexHighMtbmin","c0lep0Tex1Hex7jin3bexLowMtbmin","c0lep0Tex1Hex7jin2bexHighMtbmin","c0lep0Tex1Hex7jin2bexLowMtbmin"]}]
merged_regions += [{'name':"c0lep0Tex1Hex7jin3bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex7jin3bexHighMtbmin","c0lep0Tex1Hex7jin3bexLowMtbmin"]}]
##
merged_regions += [{'name':"c0lep1Tex1Hex7jin3bexLowMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex7jin3bexLowMtbmin","c0lep1Tex1Hex7jin3bexHighMtbmin","c0lep2THin7jin4binLowMtbmin"]}]
##
merged_regions += [{'name':"c0lep1Tex1Hex7jin3bexHighMtbmin",'samples':"ttbarlight",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex7jin3bexLowMtbmin","c0lep1Tex1Hex7jin3bexHighMtbmin"]}]
merged_regions += [{'name':"c0lep1Tex1Hex7jin3bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex7jin3bexHighMtbmin","c0lep1Tex1Hex7jin3bexHighMtbmin","c0lep2THin7jin4bin"]}]
merged_regions += [{'name':"c0lep1Tex1Hex7jin3bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex7jin3bexLowMtbmin","c0lep1Tex1Hex7jin3bexHighMtbmin"]}]
##
merged_regions += [{'name':"c0lep2Tin0_1Hwin7jin3bexHighMtbmin",'samples':"ttbarlight",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin7jin3bexLowMtbmin","c0lep2Tin0_1Hwin7jin3bexHighMtbmin"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin7jin3bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin7jin3bexLowMtbmin","c0lep2Tin0_1Hwin7jin3bexHighMtbmin"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin7jin3bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin7jin3bexLowMtbmin","c0lep2Tin0_1Hwin7jin3bexHighMtbmin"]}]
##
merged_regions += [{'name':"c0lep0Tex1Hex7jin4binHighMtbmin",'samples':"ttbarlight",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex7jin4binLowMtbmin","c0lep0Tex1Hex7jin4binHighMtbmin"]}]
merged_regions += [{'name':"c0lep0Tex1Hex7jin4binHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex7jin4binLowMtbmin","c0lep0Tex1Hex7jin4binHighMtbmin","c0lep0Tex1Hex7jin3bexLowMtbmin","c0lep0Tex1Hex7jin3bexHighMtbmin"]}]
merged_regions += [{'name':"c0lep0Tex1Hex7jin4binHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex7jin4binLowMtbmin","c0lep0Tex1Hex7jin4binHighMtbmin"]}]
##
merged_regions += [{'name':"c0lep0Tex1Hex6jex2bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex6jex2bexHighMtbmin","c0lep0Tex1Hex6jex2bexLowMtbmin"]}]
merged_regions += [{'name':"c0lep0Tex1Hex6jex2bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex6jex2bexHighMtbmin","c0lep0Tex1Hex6jex2bexLowMtbmin"]}]
##
merged_regions += [{'name':"c0lep1Tex1Hex6jex2bexLowMtbminHighMetSig",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex6jex2bexLowMtbminHighMetSig","c0lep1Tex1Hex6jex2bexHighMtbmin","c0lep2THin6jex3bex"]}]
merged_regions += [{'name':"c0lep1Tex1Hex6jex2bexLowMtbminHighMetSig",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex6jex2bexLowMtbmin","c0lep1Tex1Hex6jex2bexLowMtbminHighMetSig","c0lep1Tex1Hex6jex2bexHighMtbmin","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
##
merged_regions += [{'name':"c0lep0Tex1Hex6jex3bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex6jex3bexLowMtbmin","c0lep0Tex1Hex6jex3bexHighMtbmin"]}]
merged_regions += [{'name':"c0lep0Tex1Hex6jex3bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex6jex3bexLowMtbmin","c0lep0Tex1Hex6jex3bexHighMtbmin"]}]
##
merged_regions += [{'name':"c0lep2THin6jex3bexLowMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep2THin6jex3bexLowMtbmin","c0lep2THin6jex3bexHighMtbmin","c0lep2THin6jex4binLowMtbmin","c0lep2THin6jex4binHighMtbmin"]}]
merged_regions += [{'name':"c0lep2THin6jex3bexLowMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep2THin6jex3bexLowMtbmin","c0lep2THin6jex3bexHighMtbmin","c0lep2THin6jex4binLowMtbmin","c0lep2THin6jex4binHighMtbmin"]}]
##
merged_regions += [{'name':"c0lep2THin6jex4bin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep2THin6jex4bin","c0lep2THin6jex3bex"]}]
merged_regions += [{'name':"c0lep2THin6jex4bin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep2THin6jex4bin","c0lep2THin6jex3bex"]}]
##
merged_regions += [{'name':"c1lep2Tin0_1Hwin5jex3bex",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c1lep2Tin0_1Hwin5jex2bex","c1lep0Tin2Hin5jex3bex","c1lep2THin5jex4bin"]}]
##
merged_regions += [{'name':"c1lep0Tin2Hin5jex3bex",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c1lep0Tin2Hin5jex2bex","c1lep0Tin2Hin5jex3bex","c1lep2THin5jex4bin","c1lep2Tin0_1Hwin5jex2bex","c1lep2Tin0_1Hwin5jex3bex"]}]
#
merged_regions += [{'name':"c1lep2THin5jex4bin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c1lep2Tin0_1Hwin5jex2bex","c1lep0Tin2Hin5jex2bex","c1lep2Tin0_1Hwin5jex3bex","c1lep0Tin2Hin5jex3bex","c1lep2THin5jex4bin"]}]
#
merged_regions += [{'name':"c1lep0Tin2Hin5jex2bex",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c1lep0Tin2Hin5jex2bex","c1lep2Tin0_1Hwin5jex2bex","c1lep0Tin2Hin5jex3bex","c1lep2Tin0_1Hwin5jex3bex","c1lep2THin5jex4bin"]}]
#
merged_regions += [{'name':"c1lep0Tin2Hin6jin4bin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c1lep0Tin2Hin6jin4bin","c1lep0Tin2Hin6jin3bex"]}]
#
merged_regions += [{'name':"c0lep1Tex0Hex7jin2bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex0Hex7jin2bexHighMtbmin","c0lep1Tex0Hex7jin2bexLowMtbmin"]}]
#
merged_regions += [{'name':"c0lep1Tex0Hex7jin3bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex0Hex7jin3bexHighMtbmin","c0lep1Tex0Hex7jin3bexLowMtbmin"]}]
#
merged_regions += [{'name':"c0lep1Tex0Hex6jex4bin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex0Hex6jex4bin","c0lep1Tex0Hex6jex3bex"]}]
#
merged_regions += [{'name':"c0lep1Tex0Hex6jex2bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex0Hex6jex2bexHighMtbmin","c0lep1Tex0Hex6jex2bexLowMtbmin"]}]
merged_regions += [{'name':"c0lep1Tex0Hex6jex2bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep1Tex0Hex6jex2bexHighMtbmin","c0lep1Tex0Hex6jex2bexLowMtbmin"]}]
#
merged_regions += [{'name':"c0lep1Tex0Hex6jex3bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex0Hex6jex3bexLowMtbmin","c0lep1Tex0Hex6jex3bexHighMtbmin"]}]
merged_regions += [{'name':"c0lep1Tex0Hex6jex3bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep1Tex0Hex6jex3bexLowMtbmin","c0lep1Tex0Hex6jex3bexHighMtbmin"]}]
#
merged_regions += [{'name':"c1lep1Tex1Hex5jex3bex",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c1lep1Tex1Hex5jex2bex","c1lep1Tex1Hex5jex3bex","c1lep2THin5jex4bin"]}]
#
merged_regions += [{'name':"c0lep2Tin0_1Hwin7jin2bexLowMtbminHighMetSig",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin7jin2bexLowMtbminHighMetSig","c0lep2Tin0_1Hwin7jin2bexHighMtbmin"]}]
#
merged_regions += [{'name':"c0lep2Tin0_1Hwin7jin2bexLowMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin7jin2bexLowMtbmin","c0lep2Tin0_1Hwin7jin2bexHighMtbmin"]}]
#
merged_regions += [{'name':"c0lep0Tex1Hex7jin4binLowMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep0Tex1Hex7jin4binLowMtbmin","c0lep0Tex1Hex7jin4binHighMtbmin","c0lep0Tex1Hex7jin3bexLowMtbmin","c0lep0Tex1Hex7jin3bexHighMtbmin"]}]
#
merged_regions += [{'name':"c0lep1Tex0Hex6jex2bexLowMtbminHighMetSig",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex0Hex6jex2bexLowMtbminHighMetSig","c0lep1Tex0Hex6jex2bexHighMtbmin"]}]
#
merged_regions += [{'name':"c0lep1Tex0Hex6jex2bexLowMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex0Hex6jex2bexLowMtbmin","c0lep1Tex0Hex6jex2bexHighMtbmin"]}]
#
merged_regions += [{'name':"c0lep0Tex0Hex7jin4binHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep0Tex0Hex7jin4binHighMtbmin","c0lep0Tex0Hex7jin4binLowMtbmin"]}]
#
merged_regions += [{'name':"c0lep1Tex0Hex7jin4binHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex0Hex7jin4binHighMtbmin","c0lep1Tex0Hex7jin4binLowMtbmin"]}]
#
merged_regions += [{'name':"c0lep0Tex0Hex6jex3bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep0Tex0Hex6jex3bexHighMtbmin","c0lep0Tex0Hex6jex3bexLowMtbmin"]}]
#
merged_regions += [{'name':"c0lep2THin6jex3bex",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
#
merged_regions += [{'name':"c0lep1Tex1Hex6jex2bexHighMetSig",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex6jex2bexHighMetSig","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
merged_regions += [{'name':"c0lep1Tex1Hex6jex2bexHighMetSig",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex6jex2bexHighMetSig","c0lep0Tex1Hex6jex2bexHighMetSig","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
#
merged_regions += [{'name':"c0lep1Tex1Hex6jex2bex",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex6jex2bex","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
merged_regions += [{'name':"c0lep1Tex1Hex6jex2bex",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep1Tex1Hex6jex2bex","c0lep0Tex1Hex6jex2bex","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
#
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bexHighMetSig",'samples':"ttbarlight",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bexHighMetSig","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bexHighMetSig",'samples':"ttbarbb",'systs':["aMCHer","PowHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bexHighMetSig","c0lep0Tin2Hin6jex2bexHighMetSig","c0lep1Tex1Hex6jex2bexHighMetSig","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bexHighMetSig",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bexHighMetSig","c0lep0Tin2Hin6jex2bexHighMetSig","c0lep1Tex1Hex6jex2bexHighMetSig","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
#
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bex",'samples':"ttbarlight",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bex","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bex",'samples':"ttbarbb",'systs':["aMCHer","PowHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bex","c0lep0Tin2Hin6jex2bex","c0lep1Tex1Hex6jex2bex","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
merged_regions += [{'name':"c0lep2Tin0_1Hwin6jex2bex",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep2Tin0_1Hwin6jex2bex","c0lep0Tin2Hin6jex2bex","c0lep1Tex1Hex6jex2bex","c0lep2THin6jex3bex","c0lep2THin6jex4bin"]}]
#
merged_regions += [{'name':"c0lep2THin6jex2bexHighMtbmin",'samples':"ttbarlight",'systs':["aMCHer"],'to_add':["c0lep2THin6jex2bexHighMtbmin","c0lep2THin6jex2bexLowMtbmin"]}]
merged_regions += [{'name':"c0lep2THin6jex2bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep2THin6jex2bexHighMtbmin","c0lep2THin6jex2bexLowMtbmin","c0lep2THin6jex3bexHighMtbmin","c0lep2THin6jex3bexLowMtbmin"]}]
merged_regions += [{'name':"c0lep2THin6jex2bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep2THin6jex2bexHighMtbmin","c0lep2THin6jex2bexLowMtbmin","c0lep2THin6jex3bex"]}]
#
merged_regions += [{'name':"c0lep2THin7jin2bexHighMtbmin",'samples':"ttbarbb",'systs':["aMCHer"],'to_add':["c0lep2THin7jin2bexHighMtbmin","c0lep1Tex1Hex7jin3bexHighMtbmin","c0lep2Tin0_1Hwin7jin3bexHighMtbmin","c0lep0Tin2Hin7jin3bexHighMtbmin"]}]
merged_regions += [{'name':"c0lep2THin7jin2bexHighMtbmin",'samples':"ttbarcc",'systs':["aMCHer"],'to_add':["c0lep2THin7jin2bexHighMtbmin","c0lep1Tex1Hex7jin3bexHighMtbmin","c0lep2Tin0_1Hwin7jin3bexHighMtbmin","c0lep0Tin2Hin7jin3bexHighMtbmin"]}]


if not mergeRegions:
    merged_regions = []
elif printWarnings:
    for ttbar in ["ttbarlight","ttbarcc","ttbarbb"]:
        print "========================================================================"
        print ttbar
        print "========================================================================"
        for reg in ["1lep","0lep",]:
            print "......................................................................."
            print reg
            print "......................................................................."
            for syst in files:
                print "--------------------"
                print syst
                print "--------------------"

                for merging in merged_regions:
                    if merging['name'].find(reg)==-1:
                        continue
                    if merging['samples']!=ttbar:
                        continue
                    if not syst in merging['systs']:
                        continue
                    print "* " + merging['name']
                    for add_reg in merging['to_add']:
                        print "    - " + add_reg


start =time.time()
if not(os.path.isdir(outputDir)):
    os.system("mkdir -p "+outputDir)

newfile=root.TFile.Open(outputDir+"/"+"TtbarSystematicsWeights.root","RECREATE")
for v,var in enumerate(varlist):
    for s,sample in enumerate(samples):
        pp6file=root.TFile.Open(inputDir+"/"+sample+".root")
        pphfile=root.TFile.Open(inputDir+"/"+sample+"PowHer.root")
        for i,VarFiles in enumerate(files):
            varfile=root.TFile.Open(inputDir+"/"+sample+VarFiles+".root")
            for r,reg in enumerate(Regions):
                print reg
                pp6file.cd()
                pp6histo=pp6file.Get(reg+"_"+var).Clone()
                pp6histo.SetDirectory(0)

                varfile.cd()
                varhisto=varfile.Get(reg+"_"+var).Clone()
                varhisto.SetDirectory(0)

                for merging in merged_regions:
                    if merging['name'] != reg:
                        continue
                    if merging['samples'] != sample:
                        continue
                    if not VarFiles in merging['systs']:
                        continue
                    for hist_to_add in merging['to_add']:
                        if hist_to_add==merging['name']:
                            continue
                        print "      " + hist_to_add
                        temp_pp6 = pp6file.Get(hist_to_add+"_"+var).Clone()
                        temp_pp6.SetDirectory(0)
                        pp6histo.Add(temp_pp6)

                        temp_var = varfile.Get(hist_to_add+"_"+var).Clone()
                        temp_var.SetDirectory(0)
                        varhisto.Add(temp_var)
                err_nom = Double()
                err_var = Double()
                yield_nom = pp6histo.IntegralAndError(1,pp6histo.GetNbinsX(),err_nom)
                yield_var = varhisto.IntegralAndError(1,varhisto.GetNbinsX(),err_var)
                if( (err_nom/yield_nom >0.35 or err_var/yield_var >0.35) and printWarnings):
                    print "Region: " + pp6histo.GetName()
                    print "   Histo: " + pp6histo.GetName() + "   File: " + inputDir+"/"+sample+".root   \t \t Error: " + `(err_nom/yield_nom*100.)` + "    Yield = " + `yield_nom`
                    print "   Histo: " + varhisto.GetName() + "   File: " + inputDir+"/"+sample+VarFiles+".root   \t \t Error: " + `(err_var/yield_var*100.)` + "    Yield = " + `yield_var`

                rebinning=True
                newbins=array('d')
                while(rebinning):
                    del newbins[:]
                    rebinning=False
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
                    varhisto=varhisto.Rebin(len(newbins)-1,"",newbins)

                denhisto=pp6histo.Rebin(len(newbins)-1,"denhisto",newbins)

                numhisto=varhisto.Clone()
                numhisto2=numhisto.Clone()

                numhisto.Divide(denhisto)

                newfile.cd()
                numhisto.Write(reg+"_"+var+"_"+sample+systtypes[i])

                c = TCanvas()
                numhisto.Draw("e0")
                numhisto.SetLineWidth(2)
                numhisto.SetLineColor(kRed)
                if printPlots:
                    c.Print(reg+"_"+var+"_"+sample+systtypes[i]+".png")

                if "aMCHer" in VarFiles:
                    pphfile.cd()
                    pphhisto=pphfile.Get(reg+"_"+var).Clone()
                    pphhisto.SetDirectory(0)
                    for merging in merged_regions:
                        if merging['name'] != reg:
                            continue
                        if merging['samples'] != sample:
                            continue
                        if not VarFiles in merging['systs']:
                            continue
                        for hist_to_add in merging['to_add']:
                            if hist_to_add==merging['name']:
                                continue
                            temp_var = pphfile.Get(hist_to_add+"_"+var).Clone()
                            temp_var.SetDirectory(0)
                            pphhisto.Add(temp_var)
                    pphhisto2=pphhisto.Rebin(len(newbins)-1,"pphhisto2",newbins)
                    numhisto2.Divide(pphhisto2)
                    newfile.cd()
                    numhisto2.Write(reg+"_"+var+"_"+sample+"_GEN")
                    c.cd()
                    numhisto2.Draw("e0")
                    numhisto2.SetLineWidth(2)
                    numhisto2.SetLineColor(kRed)
                    if printPlots:
                        c.Print(reg+"_"+var+"_"+sample+"_GEN.png")

            varfile.Close()

        pp6file.Close()
        pphfile.Close()

newfile.Close()
