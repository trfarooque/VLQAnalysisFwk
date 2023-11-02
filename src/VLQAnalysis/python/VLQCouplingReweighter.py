##
## VLQCoupling reweighter is designed to run over each mass point and each coupling
## possibility.
## It can be launched interactively or using the script: LaunchAllCouplingReweighting.py
##

#!/bin/python
import os
import string
import random
import re
import time, getpass
import socket
import sys
import datetime
from glob import glob
from ROOT import *
from optparse import OptionParser
from VLQ_BR import *
from VLQ_Samples_mc import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "python/IFAETopFramework/" )
from BatchTools import *
from Samples import *

##____________________________________________________
## Options
parser = OptionParser()
parser.add_option("-i","--inputDir",dest="inputDir",help="repository for the splitted files are located",action="store",default="./")
parser.add_option("-o","--outputDir",dest="outputDir",help="repository where to put the hadded files",action="store",default="./test/")
parser.add_option("-s","--statOnly",dest="statOnly",help="Flag to consider systematics",action="store",default="False")
parser.add_option("-m","--mass",dest="vlqMass",help="value of the VLQ mass",action="store",default="800")
parser.add_option("-c","--vlqCoupling",dest="vlqCoupling",help="value of the coupling (Wb,Zt,Ht)",action="store",default="HtHt,0.,0.,1.")
parser.add_option("-r","--regions",dest="regions",help="Regions to consider (coma separated list), can use \"all\" to use all regions",action="store",default="")
parser.add_option("-f","--outputFile",dest="outputFile",help="Dummy option", action="store",default="test.root")
parser.add_option("-v","--variables",dest="variables",help="Variables to consider (coma separated list)",action="store",default="meff")
parser.add_option("-p","--mcCampaign",dest="mcCampaign",help="MC campaign",action="store",default="mc16a")
parser.add_option("--fileSuffix",dest="fileSuffix",help="Any suffix to add to name of input and output files",action="store",default="")
parser.add_option("--postMerging",dest="postMerging",help="Uses post-merging naming convention for input files when this set",action="store",type=int,default=0)

(options, args) = parser.parse_args()

outputDir=options.outputDir
inputDir=options.inputDir
if(options.statOnly.upper() == "TRUE"):
    statOnly = True
else:
    statOnly = False
#statOnly=options.statOnly
vlqMass=options.vlqMass
vlqCoupling=options.vlqCoupling
regions=options.regions
variables=options.variables
outputFile=options.outputFile
mcCampaign=options.mcCampaign
fileSuffix=options.fileSuffix
postMerging=options.postMerging

os.system("mkdir -p " + outputDir)
##........................................................

##________________________________________________________
## Getting all signal samples and their associated weight/object systematics
Samples = []
Samples     += [getSampleUncertainties("VLQ","VLQ_TT_"+vlqMass,CommonObjectSystematics,isSignal=True)]
printGoodNews("--> All VLQ samples recovered")
##........................................................

##________________________________________________________
## Defining the list of variables to look at
Variables = []
for var in variables.split(","):
    Variables += [var]
##........................................................

##________________________________________________________
## Defining the list of regions to look at
Regions = []
for reg in regions.split(","):
    Regions += [reg]
##........................................................

##________________________________________________________
## Naming convention
#template_fileName = "outVLQAna_SAMPLE_*_OBJSYSTEMATIC__*.root"
#VLQ_TT_1600_ZtZt.mc16e.root
#ttbarlight.mc16e_JET_BJES_Response__1down.root
#outVLQAna_ttbarlight_410470.mc16e_JET_BJES_Response__1down_6.root
if postMerging:
    template_fileName = "SAMPLE.*_OBJSYSTEMATIC_*"+fileSuffix+".root"
else:
    template_fileName = "outVLQAna_SAMPLE_*_OBJSYSTEMATIC__*"+fileSuffix+".root"

template_histName = "REGION_VLQTYPE_VARIABLE_WGTSYSTEMATIC"
##........................................................

##________________________________________________________
## BR definitions
BRs = []
couplings = vlqCoupling.split(",")
if len(couplings)==4:
    BRs += [{'name':couplings[0],'values':{'all':(float(couplings[1]),float(couplings[2]),float(couplings[3]))}}]
elif len(couplings)==1:
    if couplings[0].upper().find("TSINGLET")>-1:
        BRs += [{'name':couplings[0],'values':VLT_Singlet}]
    elif couplings[0].upper().find("TDOUBLET")>-1:
        BRs += [{'name':"TDoublet",'values':VLT_Doublet}]
    else:
        printError("<!> Cannot recognize the coupling configuration :-( Please check !")
        print "    -> ", couplings[0]
else:
    printError("<!> Cannot recognize the coupling configuration :-( Please check !")
    print "    -> ", vlqCoupling
##........................................................

##________________________________________________________
## Values of reweighting values
def getRWValues(values):
    rw = []
    # Structure have:
    #   0: WbWb, 1: WbZt, 2:ZtZt, 3: WbHt, 4: ZtHt, 5:HtHt
    rw += [values[2]*values[2]/(1./9)] #5
    rw += [2*values[0]*values[2]/(2./9)] #3
    rw += [2*values[1]*values[2]/(2./9)] #4
    rw += [values[0]*values[0]/(1./9)] #0
    rw += [2.*values[0]*values[1]/(2./9)] #1
    rw += [values[1]*values[1]/(1./9)] #2

    return rw
##........................................................

##________________________________________________________
## Locate a chain of characters in a string
def find_between( s, first, last ):
    try:
        start = s.index( first ) + len( first )
        end = s.index( last, start )
        return s[start:end]
    except ValueError:
        return ""
##........................................................

##________________________________________________________
## Locate a chain of characters in a string
def find_between_r( s, first, last ):
    try:
        start = s.rindex( first ) + len( first )
        end = s.rindex( last, start )
        return s[start:end]
    except ValueError:
        return ""
##........................................................

##________________________________________________________
## Small utility for histograms list
def GetKeyNames( iF, dir = "" ):
    iF.cd(dir)
    return [key.GetName() for key in gDirectory.GetListOfKeys()]
##........................................................

##________________________________________________________
## Creates the reweighted histograms
def CreateAllHistograms(inputFileName, outputFileName, br):

    inF = TFile(inputFileName,"read")
    outF = TFile(outputFileName,"recreate")

    rw = getRWValues(br)
    if(len(rw)==0):
        return

    #Getting the list of histograms
    keyList = GetKeyNames(inF)

    #checks if the user wants to run over all the variables
    global Regions
    if len(Regions)==1 and Regions[0].upper()=="ALL":
        Regions = []
        for key in keyList:
            if key.find("_vlq")>-1: continue
            if key.find("meff")==-1: continue
            if key.find("meff_")>-1: continue
            temp_region = key.replace("_meff","")
            Regions += [temp_region]

    for region in Regions:

        for var in Variables:
            WeightSystematics = []

            #get the list of systematics automatically
            wgtSyst = []
            if statOnly:
                wgtSyst = [""]
            else:
                for key in keyList:
                    if region+"_"+var in key:
                        wgtSyst += [key.replace(region+"_"+var,"")]

            for wgt in wgtSyst:
                histoName = template_histName.replace("REGION",region).replace("VARIABLE",var).replace("_WGTSYSTEMATIC",wgt)
                outputHistoName = histoName.replace("_VLQTYPE","")

                histos = []
                for iVLQ in range(1,7):
                    iVLQ_histoName = histoName.replace("VLQTYPE","vlq"+`iVLQ`)
                    #print(iVLQ_histoName)
                    h = inF.Get(iVLQ_histoName).Clone()
                    histos += [h]

                if(len(histos)!=len(rw)):
                    printError("<!> Different numbers of reweighting factors and histograms to reweight ... Please check !")
                    continue
                elif(len(histos)==0):
                    printError("<!> No histograms to reweight ... Please check !")
                    continue

                finalHisto = histos[0].Clone()
                #print "Integral before scaling = " + str(finalHisto.Integral())
                finalHisto.Scale(rw[0])
                #print "Integral after scaling = " + str(finalHisto.Integral())
                #print "Scaling histogram by " + str(rw[0])
                for iRw in range(1,len(rw)):
                    #print "Integral before scaling = " + str(histos[iRw].Integral())
                    #histos[iRw].Scale(rw[iRw])
                    #print "Integral after scaling = " + str(histos[iRw].Integral())
                    #finalHisto.Add(histos[iRw])
                    finalHisto.Add(histos[iRw],rw[iRw])
                    #print "Scaling histogram by " + str(rw[iRw])
                    
                outF.cd()
                finalHisto.SetName(outputHistoName)
                finalHisto.SetTitle(outputHistoName)
                finalHisto.Write(outputHistoName)
    outF.Close()
    inF.Close()
##........................................................

##________________________________________________________
## Loop over samples and files
printGoodNews("--> Starting !")
for BR in BRs:
    for iSample in range(len(Samples)):
        printGoodNews("--> Sample : "+ Samples[iSample]['name'])

        SName = Samples[iSample]['name'] # sample name

        key = ''
        if(len(BR['values'])==1):
            key = 'all'
        else:
            if(SName.find("_350")>-1): key = '350'
            elif(SName.find("_400")>-1): key = '400'
            elif(SName.find("_450")>-1): key = '450'
            elif(SName.find("_500")>-1): key = '500'
            elif(SName.find("_550")>-1): key = '550'
            elif(SName.find("_600")>-1): key = '600'
            elif(SName.find("_650")>-1): key = '650'
            elif(SName.find("_700")>-1): key = '700'
            elif(SName.find("_750")>-1): key = '750'
            elif(SName.find("_800")>-1): key = '800'
            elif(SName.find("_850")>-1): key = '850'
            elif(SName.find("_900")>-1): key = '900'
            elif(SName.find("_950")>-1): key = '950'
            elif(SName.find("_1000")>-1): key = '1000'
            elif(SName.find("_1050")>-1): key = '1050'
            elif(SName.find("_1100")>-1): key = '1100'
            elif(SName.find("_1150")>-1): key = '1150'
            elif(SName.find("_1200")>-1): key = '1200'
            elif(SName.find("_1300")>-1): key = '1300'
            elif(SName.find("_1400")>-1): key = '1400'
            elif(SName.find("_1500")>-1): key = '1500'
            elif(SName.find("_1600")>-1): key = '1600'
            elif(SName.find("_1700")>-1): key = '1700'
            elif(SName.find("_1800")>-1): key = '1800'
            elif(SName.find("_1900")>-1): key = '1900'
            elif(SName.find("_2000")>-1): key = '2000'

        myBR = BR['values'][key]
        Systs = []
        if(statOnly):
            Systs += [""]
        else:
            for syst in Samples[iSample]['objSyst']:
                if(syst['oneSided']):
                    Systs += [syst['nameUp']]
                else:
                    Systs += [syst['nameUp']]
                    Systs += [syst['nameDown']]

        # Loops over the object systematics
        for syst in Systs:
            print ".syst : " + syst
            space = "_"
            if(statOnly):
                space = "_nominal"

            #Creating the input files list
            if ((syst=="") or (syst == "nominal")) and postMerging:
                inputFileName = inputDir + "/" + template_fileName.replace("SAMPLE",SName).replace("_OBJSYSTEMATIC_","")
            else:
                inputFileName = inputDir + "/" + template_fileName.replace("SAMPLE",SName).replace("_OBJSYSTEMATIC_",space+syst)

            print("inputFile : " + inputFileName)
            filelist=glob.glob(inputFileName)

            for iFile in filelist:
                #Computing the name of the output file
                outputFileName = outputDir + "/"
                print(template_fileName)
                print(iFile)

                if postMerging:
                    if syst=="nominal" or syst=="":
                        outputFileName += template_fileName\
                            .replace("SAMPLE.",SName+"_"+BR['name']+"."+mcCampaign)\
                            .replace("_OBJSYSTEMATIC_","")\
                            .replace("*"+fileSuffix+".root",find_between_r( iFile, space+syst, ".root" ))\
                            .replace("*","")+".root"
                    else:
                        outputFileName += template_fileName\
                            .replace("SAMPLE.",SName+"_"+BR['name']+"."+mcCampaign)\
                            .replace("_OBJSYSTEMATIC_",space+syst)\
                            .replace("*"+fileSuffix+".root",find_between_r( iFile, space+syst, ".root" ))\
                            .replace("*","")+".root"
                else:
                    outputFileName += template_fileName.replace("SAMPLE",SName+"_"+BR['name']+"."+mcCampaign)\
                    .replace("_OBJSYSTEMATIC",space+syst)\
                    .replace("*"+fileSuffix+".root",find_between_r( iFile, space+syst, ".root" ))\
                    .replace("*","")+".root"

                print("outputFile : " + outputFileName)
                if(os.path.exists(outputFileName)):
                    print (outputFileName+' exists. Skipping...')
                    continue
                CreateAllHistograms(filelist[0],outputFileName,myBR)
##........................................................
