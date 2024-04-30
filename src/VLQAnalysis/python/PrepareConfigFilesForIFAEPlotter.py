import os
import time
import socket
import sys
import datetime
import csv
import re
import subprocess

# from VLQ_Samples import *
from VLQ_Samples_mc import *

sys.path.append("../../IFAETopFramework/python/")

from BatchTools import *
from Samples import *

##________________________________________________________
## Defines some useful variables
now = "SingleVLQ"
now += datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
here = os.getcwd()
##........................................................

##________________________________________________________
## OPTIONS
channels = ["./.",]
isMCMC = True
do0lep = False
do1lep = True
dumpFiles = True
mergeBackgrounds = True
makePlots = False
plotDir = "/Plots_" + now
reweightSignals = True
oneVar = False
signalXsec = 0.1 #pb
listKeyword = ""
onlySignalsWithKeys = False
noRegionsWithKeys = False
onlyRegionsWithKeys = False
noVariablesWithKeys = False
onlyVariablesWithKeys = False
Regions = False
Variables = False
readFromFile = False
correctLabels = False
pruneVariables = False
splitPairVLQ = False
acceptUnknownSamples = False
ttbarHfSplitted = True
yLabel="Events"
doPairVLQDecays = ['HtHt']
inputDir="/nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.67-htztx-0-MV2"
# inputDir="/nfs/at3/scratch2/tvdaalen/VLQ_FullRun2/Production_tag_Feb2019_correctedsamples/"

## Config file specific options
dataLumi = 139020.
normStyle = "NORM" # "SHAPE"
showYields = False
drawRes = "NONE" # "NONE" / RATIO" / "INVRATIO" / "DIFF" / "RESIDUAL"
signalresopt = "INC" # "SKIP" / "REF"
doSumSample = False
doScaledSignalDisplay = False
reslabel = "Data/MC" # "Sig/Bkg"

## TO IMPLEMENT: print statement to organize plot output

##........................................................

userParams = []
if(len(sys.argv))>1:
    for x in sys.argv[1:]:
        splitted=x.split("=")
        if(len(splitted)!=2):
            printError("<!> The argument \"" + x + "\" has no equal signs ... Please check")
            sys.exit(-1)
        argument = splitted[0].upper().replace("--","")
        value = splitted[1]
        if argument=="CHANNELS":
            channels = ["./.",]
        elif argument=="DATALUMI":
            dataLumi = float(value)
        elif argument=="ISMCMC":
            isMCMC = (value.upper()=="TRUE")
        elif argument=="DO0LEP":
            do0lep = (value.upper()=="TRUE")
        elif argument=="DO1LEP":
            do1lep = (value.upper()=="TRUE")
        elif argument=="DUMPFILES":
            dumpFiles = (value.upper()=="TRUE")
        elif argument=="MERGEBACKGROUNDS":
            mergeBackgrounds = (value.upper()=="TRUE")
        elif argument=="MAKEPLOTS":
            makePlots = (value.upper()=="TRUE")
        elif argument=="PLOTDIR":
            plotDir = str(value)
        elif argument=="REWEIGHTSIGNALS":
            reweightSignals = (value.upper()=="TRUE")
        elif argument=="ONEVAR":
            oneVar = str(value)
        elif argument=="SIGNALXSEC":
            signalXsec = float(value)
        elif argument=="NORMSTYLE":
            normStyle = str(value)
        elif argument=="DOSUMSAMPLE":
            doSumSample = (value.upper()=="TRUE")
        elif argument=="LISTKEYWORD":
            listKeyword = str(value)
        elif argument=="ONLYSIGNALSWITHKEYS":
            onlySignalsWithKeys = False if (value.upper()=="FALSE") else map(str, value.strip('[]').split(','))
        elif argument=="NOREGIONSWITHKEYS":
            noRegionsWithKeys = False if (value.upper()=="FALSE") else map(str, value.strip('[]').split(','))
        elif argument=="ONLYREGIONSWITHKEYS":
            onlyRegionsWithKeys = False if (value.upper()=="FALSE") else map(str, value.strip('[]').split(','))
        elif argument=="NOVARIABLESWITHKEYS":
            noVariablesWithKeys = False if (value.upper()=="FALSE") else map(str, value.strip('[]').split(','))
        elif argument=="ONLYVARIABLESWITHKEYS":
            onlyVariablesWithKeys = False if (value.upper()=="FALSE") else map(str, value.strip('[]').split(','))
        elif argument=="REGIONS":
            Regions = False if (value.upper()=="FALSE") else map(str, value.strip('[]').split(','))
        elif argument=="VARIABLES":
            Variables = False if (value.upper()=="FALSE") else map(str, value.strip('[]').split(','))
        elif argument=="SHOWYIELDS":
            showYields = (value.upper()=="TRUE")
        elif argument=="DRAWRES":
            drawRes = str(value)
        elif argument=="READFROMFILE":
            readFromFile = (value.upper()=="TRUE")
        elif argument=="CORRECTLABELS":
            correctLabels = (value.upper()=="TRUE")
        elif argument=="INPUTDIR":
            inputDir = str(value)
        elif argument=="PRUNEVARIABLES":
            pruneVariables = (value.upper()=="TRUE")
        elif argument=="SPLITPAIRVLQ":
            splitPairVLQ = (value.upper()=="TRUE")
        elif argument=="ACCEPTUNKNOWNSAMPLES":
            acceptUnknownSamples = (value.upper()=="TRUE")
        elif argument=="DOPAIRVLQDECAYS":
            doPairVLQDecays = False if (value.upper()=="FALSE") else map(str, value.strip('[]').split(',')) if '[' in value else (value.upper()=="TRUE")
        elif argument=="TTBARHFSPLITTED":
            ttbarHfSplitted = (value.upper()=="TRUE")
        elif argument=="DOSCALEDSIGNALDISPLAY":
            doScaledSignalDisplay = (value.upper()=="TRUE")
        elif argument=="RESLABEL":
            reslabel = str(value)
        elif argument=="YLABEL":
            yLabel = str(value)
        else:
            userParams += [{'arg':argument,'value':value}]
else:
    printError("<!> No arguments seen ... Aborting !")
    sys.exit(-1)

##........................................................
## Some useful functions

def NormaliseSample(DSID,int_lumi,xsec):
    samples_info_path = '../data/samples_info.dat'
    # samples_info_path = '../data/samples_info_AT_xsec.dat'
    with open(samples_info_path) as f:
        reader=csv.reader(f, delimiter=' ')
        DSID_info,sumweight_info,xsecxBR_info = zip(*reader)
    for d in DSID_info:
        if str(int(float(d))) == str(int(float(DSID))):
            samplexsecxBR = xsecxBR_info[DSID_info.index(d)]
            samplessumweight = sumweight_info[DSID_info.index(d)]
            break
    return ( xsec*int_lumi )/( float(samplexsecxBR) )#*float(samplessumweight) )

def merge_two_dicts(x, y):
    z = x.copy()
    z.update(y)
    return z

def GetRegionsAndVariables(file,prunevariables=False,Regions=False,Variables=False,noRegionsWithKeys=False,onlyRegionsWithKeys=False,noVariablesWithKeys=False,onlyVariablesWithKeys=False):
    lsrootpath = "/lsroot-00-01-03/bin/lsRoot"
    if prunevariables:
        proc = subprocess.Popen([".%s /dir %s"%(lsrootpath,file)], stdout=subprocess.PIPE, shell=True)
    else:
        proc = subprocess.Popen([".%s %s"%(lsrootpath,file)], stdout=subprocess.PIPE, shell=True)
    (out, err) = proc.communicate()
    contents = out.split()
    regions,variables = [],[]
    for content in contents:
        if prunevariables and file in content:
            variables.append([])
        if (len(content) < 9) or (not content.startswith('c')): continue
        if (not content.startswith('call') and not content[1].isdigit()): continue
        ind = [u.start() for u in re.finditer('_', content)]
        # NOT CURRENTLY WORKING (I THINK)
        if '_el_' in content or '_mu_' in content: # split el or mu regions
            # splitind = [i for i in ind if not content[i-1].isdigit() and not content[i+1].isdigit() and not content[i:i+4]=='_el_' and not content[i:i+4]=='_mu_'][0]
            continue
        else:
            splitind = [i for i in ind if not content[i-1].isdigit() and not content[i+1].isdigit()][0]
        region = content[:splitind]
        variable = content[splitind+1:]
        if not region in regions:
            regions.append(region)
        if prunevariables:
            if not variable in variables[-1]:
                variables[-1].append(variable)
        elif not variable in variables:
            variables.append(variable)
    if prunevariables:
        variables = set.intersection(*map(set,variables))

    if Variables:
        variables = Variables
    else:
        if noVariablesWithKeys:
            variables = [variable for variable in variables if not any(key in variable for key in noVariablesWithKeys)]
        if onlyVariablesWithKeys:
            variables = [variable for variable in variables if all(key in variable for key in onlyVariablesWithKeys)]
    if Regions:
        regions = [region for region in regions if region in Regions]
    elif noRegionsWithKeys:
        regions = [region for region in regions if not any(key in region for key in noRegionsWithKeys)]
    elif onlyRegionsWithKeys:
        regions = [region for region in regions if all(key in region for key in onlyRegionsWithKeys)]

    print( "Doing regions:")
    for reg in regions:
        print( reg)
    print( "\nDoing %s variables"%len(variables))
    for var in sorted(variables):
        print( var)
    print( "")
    return regions,variables

##________________________________________________________
## Defining the paths and the tarball
outputDir = inputDir
if listKeyword:
    listFolder=here+"/ConfigFiles_" + listKeyword + "/"
    folderRootFiles=here+"/Rootfiles_Plots_" + listKeyword + "/"

##........................................................

##________________________________________________________
## Creating usefull repositories
if listFolder:
    os.system("mkdir -p " + listFolder) #list files folder
if dumpFiles:
    os.system("mkdir -p " + folderRootFiles) #root files folder
##........................................................

##________________________________________________________
## Getting all samples and their associated weight/object systematics
Samples = []
if not isMCMC:
    Samples += GetDataSamples( data_type = "TOPQ1" )
    
Samples += GetTtbarSamples(hfSplitted=True,ttbarSystSamples=True,useHTSlices=True)
# Samples += GetSingleTopSamples( useWeightSyst = False, useObjectSyst = False, SingletopSystSamples=False)
# Samples += GetTopEWSamples( useWeightSyst = False, useObjectSyst = False)
# Samples += GetDibosonSamples( useWeightSyst=False, useObjectSyst=False)

# Samples += GetBenchmarkVLQSamples( useWeightSyst = False, useObjectSyst = False )

Samples += GetSingleVLQSamples( useWeightSyst = False, useObjectSyst = False )

# Samples += GetZSamplesSherpa221( useWeightSyst = False, useObjectSyst = False)

# Samples += GetWSamplesSherpa221( useWeightSyst = False, useObjectSyst = False)

# Samples += GetTtbarSamples ( useWeightSyst = False, useObjectSyst = False, ttbarSystSamples = False, hfSplitted = False, useHTSlices = True, campaign = "" )

Samples += GetOtherSamples ( useWeightSyst = False, useObjectSyst = False,
                            includeSignals = False, includeVLQ = False,
                            include4tops = False, campaign="")

printGoodNews("--> All background samples recovered")

##........................................................

##_________________
## Style definition
Styles = []

drawBkgStack = "TRUE"
if 'yield' in listKeyword:
    drawBkgStack = "FALSE"

if showYields:
    yieldformat = "%.2f"
else:
    yieldformat = "NONE"

if doScaledSignalDisplay:
    signaldrawopt = "DUMMY"
else:
    signaldrawopt = "hist"

if not yLabel:
    if normStyle=="SHAPE": 
        yLabel = "Arbitrary Units"
    else:
        yLabel = "Events"


# Generic sample to fill any unknown samples
Styles += [{'sampleKey':'Unknown','legend':None,'drawopt':"hist",'legopt':"f",'drawstack':drawBkgStack,'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"kWhite",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':yieldformat}]

# Background samples
Styles += [{'sampleKey':'AllBkg','legend':"Total bkg",'drawopt':"hist",'legopt':"f",'drawstack':drawBkgStack,'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"kGray+3",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':yieldformat}]
if not ttbarHfSplitted:
    Styles += [{'sampleKey':'ttbar','legend':"t#bar{t}",'drawopt':"hist",'legopt':"f",'drawstack':drawBkgStack,'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"kBlue-6",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':yieldformat}]
else:
    Styles += [{'sampleKey':'ttbarlight','legend':"t#bar{t}+light-jets",'drawopt':"hist",'legopt':"f",'drawstack':drawBkgStack,'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"kWhite",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':yieldformat}]
    Styles += [{'sampleKey':'ttbarcc','legend':"t#bar{t}+#geq1c",'drawopt':"hist",'legopt':"f",'drawstack':drawBkgStack,'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"kBlue-10",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':yieldformat}]
    Styles += [{'sampleKey':'ttbarbb','legend':"t#bar{t}+#geq1b",'drawopt':"hist",'legopt':"f",'drawstack':drawBkgStack,'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"kBlue-6",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':yieldformat}]
Styles += [{'sampleKey':'Singletop','legend':"Single top",'drawopt':"hist",'legopt':"f",'drawstack':drawBkgStack,'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"kPink-4",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':yieldformat}]
Styles += [{'sampleKey':'Wjets','legend':"W+jets",'drawopt':"hist",'legopt':"f",'drawstack':drawBkgStack,'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"kSpring+4",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':yieldformat}]
Styles += [{'sampleKey':'Z+jets','legend':"Z+jets",'drawopt':"hist",'legopt':"f",'drawstack':drawBkgStack,'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"kGreen+3",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':yieldformat}]
Styles += [{'sampleKey':'topEW','legend':"Top EW",'drawopt':"hist",'legopt':"f",'drawstack':drawBkgStack,'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"kRed-6",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':yieldformat}]
Styles += [{'sampleKey':'Dibosons','legend':"Others",'drawopt':"hist",'legopt':"f",'drawstack':drawBkgStack,'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"kOrange+6",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':yieldformat}]
# Styles += [{'sampleKey':'Others','legend':"Others",'drawopt':"hist",'legopt':"f",'drawstack':drawBkgStack,'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"kYellow",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':yieldformat}]
# Styles += [{'legend':"QCD",'drawopt':"hist",'legopt':"f",'drawstack':drawBkgStack,'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"kViolet",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':yieldformat}]

# Data sample
if not isMCMC:
    Styles += [{'sampleKey':'Data','legend':"Data",'drawopt':"e0",'legopt':"lep",'drawstack':"FALSE",'resopt':"INC",'drawscale':"NONE",'linecolor':"kBlack",'fillcolor':"0",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"1",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]

# Signal samples
else:
    ### sVLQ only:
    if not doPairVLQDecays:




        # OLD VLQ NAME OPTIONS
        """
        Styles += [{'sampleKey':'WTHt1100','legend':"T(#rightarrowHt)qb (1.1 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kGreen+2",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'WTHt1600','legend':"T(#rightarrowHt)qb (1.6 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kGreen+2",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'WTht2000','legend':"T(#rightarrowHt)qb (2.0 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kGreen+2",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]

        Styles += [{'sampleKey':'ZTht1100','legend':"T(#rightarrowHt)qt (1.1 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kOrange+7",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'ZTht1600','legend':"T(#rightarrowHt)qt (1.6 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kOrange+7",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'ZTht2000','legend':"T(#rightarrowHt)qt (2.0 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kOrange+7",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]

        Styles += [{'sampleKey':'WTZt1100','legend':"T(#rightarrowZt)qb (1.1 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kBlue+1",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'WTZt1600','legend':"T(#rightarrowZt)qb (1.6 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kBlue+1",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'WTZt2000','legend':"T(#rightarrowZt)qb (2.0 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kBlue+1",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]


        Styles += [{'sampleKey':'ZTZt1100','legend':"T(#rightarrowZt)qt (1.1 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kPink-3",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'ZTZt1600','legend':"T(#rightarrowZt)qt (1.6 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kPink-3",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'ZTZt2000','legend':"T(#rightarrowZt)qt (2.0 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kPink-3",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        """


        Styles += [{'sampleKey':'sVLQ_WTHt11K03','legend':"T(#rightarrowHt)qb (1.1 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kGreen+2",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'sVLQ_WTHt16K05','legend':"T(#rightarrowHt)qb (1.6 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kGreen+2",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'sVLQ_WTHt20K05','legend':"T(#rightarrowHt)qb (2.0 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kGreen+2",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]

        Styles += [{'sampleKey':'sVLQ_ZTHt11K05','legend':"T(#rightarrowHt)qt (1.1 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kOrange+7",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'sVLQ_ZTHt16K05','legend':"T(#rightarrowHt)qt (1.6 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kOrange+7",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'sVLQ_ZTHt20K05','legend':"T(#rightarrowHt)qt (2.0 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kOrange+7",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]

        Styles += [{'sampleKey':'sVLQ_WTZt11K03','legend':"T(#rightarrowZt)qb (1.1 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kBlue+1",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'sVLQ_WTZt16K05','legend':"T(#rightarrowZt)qb (1.6 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kBlue+1",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'sVLQ_WTZt20K05','legend':"T(#rightarrowZt)qb (2.0 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kBlue+1",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]


        Styles += [{'sampleKey':'sVLQ_ZTZt11K05','legend':"T(#rightarrowZt)qt (1.1 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kPink-3",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'sVLQ_ZTZt16K05','legend':"T(#rightarrowZt)qt (1.6 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kPink-3",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'sVLQ_ZTZt20K05','legend':"T(#rightarrowZt)qt (2.0 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kPink-3",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]

    ### sVLQ and pair production together
    else:
        Styles += [{'sampleKey':'WTht1100','legend':"T(#rightarrowHt)qb (1.1 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kCyan+2",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'WTZt1100','legend':"T(#rightarrowZt)qb (1.1 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kOrange+4",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'ZTht1100','legend':"T(#rightarrowHt)qt (1.1 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kOrange-3",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'ZTZt1100','legend':"T(#rightarrowZt)qt (1.1 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kPink-3",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]

        Styles += [{'sampleKey':'WTHt1600','legend':"T(#rightarrowHt)qb (1.6 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kCyan+2",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'WTZt1600','legend':"T(#rightarrowZt)qb (1.6 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kOrange+4",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'ZTht1600','legend':"T(#rightarrowHt)qt (1.6 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kOrange-3",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'ZTZt1600','legend':"T(#rightarrowZt)qt (1.6 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kPink-3",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]

        Styles += [{'sampleKey':'WTht2000','legend':"T(#rightarrowHt)qb (2.0 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kCyan+2",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'WTZt2000','legend':"T(#rightarrowZt)qb (2.0 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kOrange+4",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'ZTht2000','legend':"T(#rightarrowHt)qt (2.0 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kOrange-3",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'ZTZt2000','legend':"T(#rightarrowZt)qt (2.0 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kPink-3",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]

    # 1: WbWb, 2: WbZt, 3:ZtZt, 4: WbHt, 5: ZtHt, 6:HtHt
    if splitPairVLQ:
        # Styles += [{'sampleKey':'VLQ_TT_1200','decay':'WbWb','legend':"T#bar{T} #rightarrow WbW#bar{b} (1.2 TeV)",'inpattern':"vlq_1",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kRed+3",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        # Styles += [{'sampleKey':'VLQ_TT_1200','decay':'WbZt','legend':"T#bar{T} #rightarrow WbZ#bar{t} (1.2 TeV)",'inpattern':"vlq_2",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        # Styles += [{'sampleKey':'VLQ_TT_1200','decay':'ZtZt','legend':"T#bar{T} #rightarrow ZtZ#bar{t} (1.2 TeV)",'inpattern':"vlq_3",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kViolet-5",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        # Styles += [{'sampleKey':'VLQ_TT_1200','decay':'WbHt','legend':"T#bar{T} #rightarrow HtW#bar{b} (1.2 TeV)",'inpattern':"vlq_4",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kGreen+2",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        # Styles += [{'sampleKey':'VLQ_TT_1200','decay':'ZtHt','legend':"T#bar{T} #rightarrow HtZ#bar{t} (1.2 TeV)",'inpattern':"vlq_5",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kBlue+1",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        # Styles += [{'sampleKey':'VLQ_TT_1200','decay':'HtHt','legend':"T#bar{T} #rightarrow HtH#bar{t} (1.2 TeV)",'inpattern':"vlq_6",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kRed-4",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]

        Styles += [{'sampleKey':'VLQ_TT_1400','decay':'WbWb','legend':"T#bar{T} #rightarrow WbW#bar{b} (1.4 TeV)",'inpattern':"vlq_1",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kPink+7",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'VLQ_TT_1400','decay':'WbZt','legend':"T#bar{T} #rightarrow WbZ#bar{t} (1.4 TeV)",'inpattern':"vlq_2",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kBlack",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'VLQ_TT_1400','decay':'ZtZt','legend':"T#bar{T} #rightarrow ZtZ#bar{t} (1.4 TeV)",'inpattern':"vlq_3",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kViolet-5",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'VLQ_TT_1400','decay':'WbHt','legend':"T#bar{T} #rightarrow HtW#bar{b} (1.4 TeV)",'inpattern':"vlq_4",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kGreen+2",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'VLQ_TT_1400','decay':'ZtHt','legend':"T#bar{T} #rightarrow HtZ#bar{t} (1.4 TeV)",'inpattern':"vlq_5",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kBlue+1",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'VLQ_TT_1400','decay':'HtHt','legend':"T#bar{T} #rightarrow HtH#bar{t} (1.4 TeV)",'inpattern':"vlq_6",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kRed-4",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]

    else:
        Styles += [{'sampleKey':'pVLQ_TT1200','legend':"T#bar{T} (1.2 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kOrange+4",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        # Styles += [{'sampleKey':'pVLQ_TT1400','legend':"T#bar{T} (1.4 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kViolet+2",'fillcolor':"0",'fillstyle':"0",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
        Styles += [{'sampleKey':'TT_1400','legend':"T#bar{T} (1.4 TeV)",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':normStyle,'linecolor':"kPink+7",'fillcolor':"0",'fillstyle':"0",'linestyle':"5",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]


#     Styles += [{'legend':"VLQ_TT_1000_TSingletExWbWb",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':"SHAPE",'linecolor':"kOrange-3",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
#     Styles += [{'legend':"VLQ_TT_1000_TSingletExWbZt",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':"SHAPE",'linecolor':"kRed",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
#     Styles += [{'legend':"VLQ_TT_1000_TSingletExWbHt",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':"SHAPE",'linecolor':"kBlue+1",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
#     Styles += [{'legend':"VLQ_TT_1000_TSingletExZtZt",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':"SHAPE",'linecolor':"kAzure+7",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
#     Styles += [{'legend':"VLQ_TT_1000_TSingletExZtHt",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':"SHAPE",'linecolor':"kGreen",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]
#     Styles += [{'legend':"VLQ_TT_1000_TSingletExHtHt",'drawopt':signaldrawopt,'legopt':"l",'drawstack':"FALSE",'resopt':signalresopt,'drawscale':"SHAPE",'linecolor':"kGreen+3",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':yieldformat}]

##_________________________________________________________
## Making the sample objects
Samples_Styles = []
for sample in Samples:
    skipSample = False
    sampleKey = sample['sampleType']

    if mergeBackgrounds:
        for bgkey in ['ttbar','jets','top','ttH','boson','Others']:
            if bgkey in sample['sampleType']:
                sampleKey = 'AllBkg'
    # else:
    #     for bgkey in ['top','ttH']:
    #         if bgkey in sample['sampleType']:
    #             sampleKey = 'Others'

    if onlySignalsWithKeys:
        if not any(bgkey in sampleKey for bgkey in ['AllBkg','ttbar','jets','top','ttH','Others']):
            if 'OR' in onlySignalsWithKeys:
                skipSample = True
                for signalkey in onlySignalsWithKeys:
                    if signalkey in sampleKey:
                        skipSample = False
            else:
                for signalkey in onlySignalsWithKeys:
                    if not signalkey in sampleKey:
                        skipSample = True

    sampleAdded = False
    for style in Styles:
        if style['sampleKey'] in sampleKey:
            sample = merge_two_dicts(sample,style)
            if not skipSample:
                Samples_Styles.append(sample)
                sampleadded = True

    if acceptUnknownSamples and not sampleAdded and not skipSample:
        sample = merge_two_dicts(sample,Styles[0]) # get the style for generic unknown sample
        sample['sampleKey'] = sampleKey
        sample['legend'] = sampleKey
        Samples_Styles.append(sample)

##________________________________________________________
## Reading regions and variables to plot from file
if readFromFile:
    if pruneVariables:
        prunedir = inputDir
        if len(os.listdir(folderRootFiles)) > 1:
            prunedir = folderRootFiles
        regionsFromFile,variablesFromFile = GetRegionsAndVariables(prunedir,prunevariables=True,Regions=Regions,Variables=Variables,noVariablesWithKeys=noVariablesWithKeys,onlyVariablesWithKeys=onlyVariablesWithKeys,noRegionsWithKeys=noRegionsWithKeys,onlyRegionsWithKeys=onlyRegionsWithKeys)
    else:
        inputRootFile = glob.glob(inputDir+"/outVLQAnalysis_*.root")[0]
        # inputRootFile = glob.glob(folderRootFiles+"*.root")[0]
        regionsFromFile,variablesFromFile = GetRegionsAndVariables(inputRootFile,Regions=Regions,Variables=Variables,noVariablesWithKeys=noVariablesWithKeys,onlyVariablesWithKeys=onlyVariablesWithKeys,noRegionsWithKeys=noRegionsWithKeys,onlyRegionsWithKeys=onlyRegionsWithKeys)

##________________________________________________________
## Defining the list of variables to look at

Variables_1L = []
Variables_0L = []

if do1lep:
    if readFromFile and not oneVar:
        for varFromFile in variablesFromFile:
            if splitPairVLQ:
                varFromFile = "*"+varFromFile
            Variables_1L += [{'name':varFromFile}]
    elif oneVar:
        if splitPairVLQ:
            oneVar = "*"+oneVar
        Variables_1L += [{'name':oneVar}]
    else:

        Variables_1L += [{'name':"meff"}]
        Variables_1L += [{'name':"hthad"}]
        Variables_1L += [{'name':"met"}]
        Variables_1L += [{'name':"mtbmin"}]
        Variables_1L += [{'name':"mtw"}]
        Variables_1L += [{'name':"ptw"}]
        Variables_1L += [{'name':"jets_n",'XMIN':"1.5"}]
        Variables_1L += [{'name':"jets_pt"}]
        Variables_1L += [{'name':"jets_eta"}]
        Variables_1L += [{'name':"bjets_n"}]
        Variables_1L += [{'name':"bjets_pt"}]
        Variables_1L += [{'name':"lep_n"}]
        Variables_1L += [{'name':"leps_pt"}]
        Variables_1L += [{'name':"leps_eta"}]

if do0lep:
    if readFromFile and not oneVar:
        for varFromFile in variablesFromFile:
            if splitPairVLQ:
                varFromFile = "*"+varFromFile
            Variables_0L += [{'name':varFromFile}]
    if oneVar:
        if splitPairVLQ:
            oneVar = "*"+oneVar
        Variables_0L += [{'name':oneVar}]
    else:
        Variables_0L += [{'name':"jets_n",'XMIN':"1.5"}]
        Variables_0L += [{'name':"bjets_n"}]
        Variables_0L += [{'name':"meff"}]
        Variables_0L += [{'name':"met"}]
        Variables_0L += [{'name':"hthad"}]
        Variables_0L += [{'name':"mtw"}]

##........................................................

##________________________________________________________
## Defining the list of regions to look at

Regions_0L = []
Regions_1L = []
FullNameVariables = []

# 1 lepton
if do1lep == True:
    if readFromFile:
        regions = list(filter(lambda reg: reg.startswith("c1l") or reg.startswith("call"), regionsFromFile))

        # regions.append("call")

    else:

        regions = sorted(["c1lep2jin0bin","c1lep2jin1bin","c1lep2jin2bin","c1lep3jin1bin","c1lep3jin1bin_el","c1lep3jin1bin_mu","c1lep3jin2bin","c1lep3jin2bin_el","c1lep3jin2bin_mu","c1lep4jin2bin","c1lep4jin2bin_el","c1lep4jin2bin_mu","c1lep5jin2bin","c1lep5jin2bin_el","c1lep5jin2bin_mu","call"])

    # take out unwanted regions
    if noRegionsWithKeys:
        filteredregions = []
        for region in regions:
            for regionkey in noRegionsWithKeys:
                if not regionkey in region:
                    filteredregions.append(region)
        regions = filteredregions

    for region in regions:
        legend = ""
        if region == "call":
            legend = "No selection"
        if '1lep' in region:
            legend+='1l, '
        elif '0lep' in region:
            legend+='0l, '
        if 'jex' in region:
            if not region[region.index('jex')-1]=='f':
                legend+=region[region.index('jex')-1]+'j'
        if 'jin' in region:
            if not region[region.index('jin')-1]=='f':
                legend+='#geq'+region[region.index('jin')-1]+'j'
        if 'jwin' in region:
            legend+=region[region.index('jwin')-3]+'-'+region[region.index('jwin')-1]+'j'
        if 'bex' in region:
            legend+=', '+region[region.index('bex')-1]+'b'
        elif 'bin' in region:
            legend+=', #geq'+region[region.index('bin')-1]+'b'
        elif 'bwin' in region:
            legend+=', '+region[region.index('bwin')-3]+'-'+region[region.index('bwin')-1]+'b'
        if 'fjex' in region:
            legend+=', '+region[region.index('fjex')-1]+'fj'
        elif 'fjin' in region:
            legend+=', #geq'+region[region.index('fjin')-1]+'fj'
        if 'THin' in region:
            legend+=', #geq'+region[region.index('THin')-1]+'h+t_{h}'
        elif 'Hin' in region:
            legend+=', #geq'+region[region.index('Hin')-1]+'h'
        elif 'Hex' in region:
            legend+=', '+region[region.index('Hex')-1]+'h'
        if 'Vex' in region:
            legend+=', '+region[region.index('Vex')-1]+'v'
        elif 'Vin' in region:
            legend+=', #geq'+region[region.index('Vin')-1]+'v'
        if 'VLTin' in region:
            legend+=', #geq'+region[region.index('VLTin')-1]+'v+t_{l}+t_{h}'
        if 'LTin' in region and region[region.index('LTin')-1].isdigit():
            legend+=', #geq'+region[region.index('LTin')-1]+'t_{h}+t_{l}'
        if 'Tin' in region and region[region.index('Tin')-1].isdigit():
            legend+=', #geq'+region[region.index('Tin')-1]+'t_{h}'
        if 'VLTex' in region:
            legend+=', '+region[region.index('VLTex')-1]+'v+t_{l}+t_{h}'
        if 'LTex' in region and region[region.index('LTex')-1].isdigit():
            legend+=', '+region[region.index('LTex')-1]+'t_{l}+t_{h}'
        if 'Tex' in region and region[region.index('Tex')-1].isdigit():
            legend+=', '+region[region.index('Tex')-1]+'t_{h}'
        if 'VTin' in region:
            legend+=', #geq'+region[region.index('VTin')-1]+'v+t_{h}'
        if 'VTex' in region:
            legend+=', '+region[region.index('VTex')-1]+'v+t_{h}'
        if 'Lin' in region:
            legend+=', #geq'+region[region.index('Lin')-1]+'t_{l}'
        if 'Lex' in region:
            legend+=', '+region[region.index('Lex')-1]+'t_{l}'


        Regions_1L += [{'name':region,'legend':legend}]

    # making FullNameVariables
    for reg in Regions_1L:
        for var in Variables_1L:
            FullNameVariables += [{'name':reg['name'] + "_" + var['name'],'regionlegend':reg['legend'],'var':var}]
# 0 lepton
if do0lep == True:
    if readFromFile:
        regions = list(filter(lambda reg: reg.startswith("c0l") or reg.startswith("call"), regionsFromFile))

    else:
        regions = sorted(['c0lep6jin2bex','c0lep6jin2bin','c0lep6jin3bin','c0lep7jin2bex','c0lep7jin2bin','c0lep7jin3bin'])

    for region in regions:
        legend = ""
        if region == "call":
            legend = "No selection"
        if '0lep' in region:
            legend+='0l, '
        if 'jex' in region:
            if not region[region.index('jex')-1]=='f':
                legend+=region[region.index('jex')-1]+'j'
        if 'jin' in region:
            if not region[region.index('jin')-1]=='f':
                legend+='#geq'+region[region.index('jin')-1]+'j'
        if 'jwin' in region:
            legend+=region[region.index('jwin')-3]+'-'+region[region.index('jwin')-1]+'j'
        if 'bex' in region:
            legend+=', '+region[region.index('bex')-1]+'b'
        elif 'bin' in region:
            legend+=', #geq'+region[region.index('bin')-1]+'b'
        elif 'bwin' in region:
            legend+=', '+region[region.index('bwin')-3]+'-'+region[region.index('bwin')-1]+'b'
        if 'fjex' in region:
            legend+=', '+region[region.index('fjex')-1]+'fj'
        elif 'fjin' in region:
            legend+=', #geq'+region[region.index('fjin')-1]+'fj'
        if 'THin' in region:
            legend+=', #geq'+region[region.index('THin')-1]+'h-t'
        elif 'Hin' in region:
            legend+=', #geq'+region[region.index('Hin')-1]+'h'
        elif 'Hex' in region:
            legend+=', '+region[region.index('Hex')-1]+'h'
        if 'Vex' in region:
            legend+=', '+region[region.index('Vex')-1]+'v'
        elif 'Vin' in region:
            legend+=', #geq'+region[region.index('Vin')-1]+'v'
        if 'Tin' in region and region[region.index('Tin')-1].isdigit():
            legend+=', #geq'+region[region.index('Tin')-1]+'t'
        elif 'Tex' in region and region[region.index('Tex')-1].isdigit():
            legend+=', '+region[region.index('Tex')-1]+'t'
        if 'VTin' in region:
            legend+=', #geq'+region[region.index('VTin')-1]+'v-t'
        if 'LTex' in region:
            legend+=', #geq'+region[region.index('LTex')-1]+'lt'
        elif 'LTin' in region:
            legend+=', #geq'+region[region.index('LTin')-1]+'lt'
        if 'LTTin' in region:
            legend+=', #geq'+region[region.index('LTTin')-1]+'lt-t'
        Regions_0L += [{'name':region,'legend':legend}]

    # making FullNameVariables
    for reg in Regions_0L:
        for var in Variables_0L:
            FullNameVariables += [{'name':reg['name'] + "_" + var['name'],'regionlegend':reg['legend'],'var':var}]

##_____________________________________________________________________________
## Rebinning certain variables

rebinVarDict = {'meff':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500',
    'hthad':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500',
    'hthadRC':'100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500',
    'hthadRCM':'100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500',
    'met_sig':'0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,17,20,25,30,50',
    'jets_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'jet0_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'jet1_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'jet2_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'jet3_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'fwdjets_n':'-0.5,0.5,1.5,2.5,3.5,4.5,5.5',
    'fwdjets_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'fwdjet0_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'fwdjet1_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'fwdjet2_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'fwdjet3_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'bjets_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'bjet0_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'bjet1_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'bjet2_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'bjet3_pt':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'RCjets_m':'0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,180,190,200,240,280,320,400',
    'RCjet0_m':'0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,180,190,200,240,280,320,400',
    'ptw':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'met':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000',
    'leptop_pt':'0,50,100,150,200,250,300,400,500,600,800,1000',
    'leptop_m':'0,20,40,60,80,100,120,160,200,240,280,320,360,400,440,500',
    'Ht_recoVLQ0_m':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Ht_recoVLQ1_m':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Ht_recoVLQs_m':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Zt_recoVLQ0_m':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Zt_recoVLQ1_m':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Zt_recoVLQs_m':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Wb_recoVLQ0_m':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Wb_recoVLQ1_m':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Wb_recoVLQs_m':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Ht_recoVLQ0_redm':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Ht_recoVLQ1_redm':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Ht_recoVLQs_redm':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Zt_recoVLQ0_redm':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Zt_recoVLQ1_redm':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Zt_recoVLQs_redm':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Wb_recoVLQ0_redm':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Wb_recoVLQ1_redm':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Wb_recoVLQs_redm':'0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Ht_recoVLQ0_pt':'0,50,100,150,200,250,300,350,400,500,600,700,800,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Ht_recoVLQ1_pt':'0,50,100,150,200,250,300,350,400,500,600,700,800,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Ht_recoVLQs_pt':'0,50,100,150,200,250,300,350,400,500,600,700,800,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Zt_recoVLQ0_pt':'0,50,100,150,200,250,300,350,400,500,600,700,800,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Zt_recoVLQ1_pt':'0,50,100,150,200,250,300,350,400,500,600,700,800,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Zt_recoVLQs_pt':'0,50,100,150,200,250,300,350,400,500,600,700,800,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Wb_recoVLQ0_pt':'0,50,100,150,200,250,300,350,400,500,600,700,800,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Wb_recoVLQ1_pt':'0,50,100,150,200,250,300,350,400,500,600,700,800,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'Wb_recoVLQs_pt':'0,50,100,150,200,250,300,350,400,500,600,700,800,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000',
    'truth_hadtop0_m_reco':'0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500',
    'truth_hadZ0_m_reco':'0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500',
    'truth_hadW0_m_reco':'0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500',
    'truth_Hbb0_m_reco':'0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500',
    'truth_hadtop_W0_m_reco':'0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500',
    'truth_hadtop0_dR_reco':'.0,.1,.2,.3,.4,.5,.6,.7,.8,.9,1.0',
    'truth_hadZ0_dR_reco':'.0,.1,.2,.3,.4,.5,.6,.7,.8,.9,1.0',
    'truth_hadW0_dR_reco':'.0,.1,.2,.3,.4,.5,.6,.7,.8,.9,1.0',
    'truth_Hbb0_dR_reco':'.0,.1,.2,.3,.4,.5,.6,.7,.8,.9,1.0',
    'truth_hadtop_W0_dR_reco':'.0,.1,.2,.3,.4,.5,.6,.7,.8,.9,1.0'}

##_____________________________________________________________________________
## Config files for variables
f_vars_list = open(listFolder+"vars_list.list","w")
f_vars_list.write("BEGIN\n")
for var in FullNameVariables:

    ind = [u.start() for u in re.finditer('_', var['name'])]    
    startind = [i for i in ind if not var['name'][i-1].isdigit() and not var['name'][i+1].isdigit()][0]
    varname = var['name'][startind+1:]
    if varname == 'jets_n':
        var['XMIN'] = 2.5
        var['XMAX'] = 9.5

    f_vars_list.write("NEW\n")
    f_vars_list.write("NAME : "+var['name']+"\n")

    if correctLabels: # Manually add correct labels if VLQAnalysis did not add them
        ind = [u.start() for u in re.finditer('_', var['name'])]    
        startind = [i for i in ind if not var['name'][i-1].isdigit() and not var['name'][i+1].isdigit()][0]
        varname = var['name'][startind+1:]
        if varname == "lep0_pt":
            f_vars_list.write("LABEL : Leading lepton p_{T} [GeV]\n")
        elif varname=="lep0_eta":
            f_vars_list.write("LABEL : Leading lepton #eta\n")
        elif varname=="jets_m":
            f_vars_list.write("LABEL : Jets mass [GeV]\n")
        elif varname=="jets_pt":
            f_vars_list.write("LABEL : Jets p_{T} [GeV]\n")
        elif varname=="jets_eta":
            f_vars_list.write("LABEL : Jets #eta\n")
        elif varname=="jets_y":
            f_vars_list.write("LABEL : Jets y\n")
        elif varname=="jets_n":
            f_vars_list.write("LABEL : Number of jets\n")
        elif varname=="bjets_m":
            f_vars_list.write("LABEL : B-tagged jets mass [GeV]\n")
        elif varname=="bjets_pt":
            f_vars_list.write("LABEL : B-tagged jets p_{T} [GeV]\n")
        elif varname=="bjets_eta":
            f_vars_list.write("LABEL : B-tagged jets #eta\n")
        elif varname=="bjets_n":
            f_vars_list.write("LABEL : Number of b-tagged jets\n")
        elif varname=="RCjets_m":
            f_vars_list.write("LABEL : Reclustered jet mass [GeV]\n")
        elif varname=="RCjets_pt":
            f_vars_list.write("LABEL : Reclustered jet p_{T} [GeV]\n")
        elif varname=="RCjets_eta":
            f_vars_list.write("LABEL : Reclustered jet #eta\n")
        elif varname=="RCjets_y":
            f_vars_list.write("LABEL : Reclustered jet y\n")
        elif varname=="RCjets_n":
            f_vars_list.write("LABEL : Number of reclustered jets\n")
        elif varname=="RCjets_nconsts":
            f_vars_list.write("LABEL : Number of RC jet constituents\n")
        elif varname=="meff":
            f_vars_list.write("LABEL : m_{eff} [GeV]\n")
        elif varname=="mtbmin":
            f_vars_list.write("LABEL : m_{T}^{min}(b,MET) [GeV]\n")
        elif varname=="hthad":
            f_vars_list.write("LABEL : H_{T} [GeV]\n")
        elif varname=="dPhi_lepmet":
            f_vars_list.write("LABEL : #Delta#phi(lep,E_{T}^{miss})\n")
        elif varname=="met":
            f_vars_list.write("LABEL : E_{T}^{miss} [GeV]\n")
        elif varname=="mtw":
            f_vars_list.write("LABEL : m_{T}(W) [GeV]\n")
        elif varname=="ptw":
            f_vars_list.write("LABEL : p_{T}(W) [GeV]\n")
        elif varname=="dPhi_lepjet":
            f_vars_list.write("LABEL : #Delta#phi^{min}(lep,jet)\n")
        else:
            for (nametype,label) in [['fwd-','fwd'],['top-tagged ','RCMTop'],['Higgs-tagged ','RCMHiggs'],['V-tagged ','RCMV']]: # include jets without any of these labels for the regular jet label filling
                if "sVLQ" in varname: continue
                if label in varname:
                    if varname==label+"_jets_n":
                        f_vars_list.write("LABEL : Number of %sjets\n"%nametype)
                    else:
                        for n in ['s','0','1','2','3']:
                            if n in varname:
                                if varname==label+"_jets_m":
                                    f_vars_list.write("LABEL : %sjet%s mass [GeV]\n"%(nametype.capitalize(),n))
                                if varname==label+"_jets_pt":
                                    f_vars_list.write("LABEL : %sjet%s p_{T} [GeV]\n"%(nametype.capitalize(),n))
                                if varname==label+"_jets_eta":
                                    f_vars_list.write("LABEL : %sjet%s #eta\n"%(nametype.capitalize(),n))
                                if varname==label+"_jets_y":
                                    f_vars_list.write("LABEL : %sjet%s y\n"%(nametype.capitalize(),n))
                                if varname==label+"_jets_phi":
                                    f_vars_list.write("LABEL : %sjet%s #phi\n"%(nametype.capitalize(),n))
            if "sVLQ" in varname:
                for vlqtype in ['Htop','Hb','Vtop','Vb','max']:
                    if vlqtype in varname:
                        if varname==vlqtype+"_sVLQs_n":
                            f_vars_list.write("LABEL : Number of VLQ_{single}^{reco}(%s) candidates\n"%vlqtype)
                        elif varname=="dPhi_lep_sVLQ_"+vlqtype:
                            f_vars_list.write("LABEL : #Delta#phi(lep, VLQ_{single}^{reco}(%s))\n"%vlqtype)
                        elif varname=="dR_lep_sVLQ_"+vlqtype:
                            f_vars_list.write("LABEL : #DeltaR(lep, VLQ_{single}^{reco}(%s))\n"%vlqtype)
                        elif varname=="dPhi_met_sVLQ_"+vlqtype:
                            f_vars_list.write("LABEL : #Delta#phi(MET, VLQ_{single}^{reco}(%s))\n"%vlqtype)
                        else:
                            for n in ['s','0','1','2','3']:
                                if n in varname:
                                    if varname==vlqtype+"_sVLQ"+n+"_m":
                                        f_vars_list.write("LABEL : VLQ_{single}^{reco}(%s)%s mass [GeV]\n"%(vlqtype,n))
                                    if varname==vlqtype+"_sVLQ"+n+"_pt":
                                        f_vars_list.write("LABEL : VLQ_{single}^{reco}(%s)%s p_{T} [GeV]\n"%(vlqtype,n))
                                    if varname==vlqtype+"_sVLQ"+n+"_eta":
                                        f_vars_list.write("LABEL : VLQ_{single}^{reco}(%s)%s #eta\n"%(vlqtype,n))
                                    if varname==vlqtype+"_sVLQ"+n+"_phi":
                                        f_vars_list.write("LABEL : VLQ_{single}^{reco}(%s)%s #phi\n"%(vlqtype,n))

    f_vars_list.write("YLABEL : %s\n"%yLabel)
    f_vars_list.write("RESLABEL : %s\n"%reslabel)
    f_vars_list.write("DRAWSTACK : TRUE\n")
    f_vars_list.write("DRAWRESSTACK : TRUE\n")
    f_vars_list.write("DRAWRES : %s\n"%drawRes)
    if var['name'].find("cone")>-1:
        f_vars_list.write("ISLOGY : TRUE\n")
    else:
        f_vars_list.write("ISLOGY : FALSE\n")
    f_vars_list.write("#REBIN : 0\n")
    f_vars_list.write("DOSCALE : %s\n"%normStyle)
    f_vars_list.write("DRAWRESERR : REFBAND\n")
    f_vars_list.write("EXTRALABEL : #splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#splitline{#sqrt{s} = 13 TeV, L = "+"%.0f"%(dataLumi/1000.)+" fb^{-1}}{"+var['regionlegend']+"}}\n")
    f_vars_list.write("YMIN : 0.\n")
    if 'XMIN' in var.keys():
        f_vars_list.write("XMIN : " + str(var['XMIN']) + "\n")
    if 'XMAX' in var.keys():
        f_vars_list.write("XMAX : " + str(var['XMAX']) + "\n")
    if varname in rebinVarDict.keys():
        f_vars_list.write("DOWIDTH : TRUE\n")
        f_vars_list.write("REBINVAR : %s\n"%rebinVarDict[varname])
    else:
        f_vars_list.write("DOWIDTH : FALSE\n")
    f_vars_list.write("\n")
f_vars_list.write("END\n")
f_vars_list.close()

##_____________________________________________________________________________
## Config files for file list (keep the old structure !)
f_files_list = open(listFolder+"files_list.list","w")
f_files_list.write("BEGIN\n")
f_files_list.write("KEY : FILE : SCALE\n")

##_____________________________________________________________________________
## Config files for sample
f_sample_list = open(listFolder+"sample_list.list","w")
f_sample_list.write("BEGIN\n")

##_____________________________________________________________________________
## Config files for style
f_style_list = open(listFolder+"style_list.list","w")
f_style_list.write("BEGIN\n")

##------------------------------------------------------
## Loop over the samples and systematics
##------------------------------------------------------

CommDict = {}
allSampleKeys = []
listfiles = glob.glob(inputDir+"/outVLQAnalysis_*.root")

# Writing the configuration lists
for sampleDef in Samples_Styles:

    SDSID = sampleDef['name']
    SType = sampleDef['sampleType']
    SKey = sampleDef['sampleKey']

    cleaned_sampleType = SKey.replace("#","").replace("","").replace("{","").replace("}","").replace("+","").replace("(","").replace(")","")
    name_temp_rootfile = folderRootFiles + "/" + cleaned_sampleType + ".root"

    if splitPairVLQ and sampleDef.get('decay') in doPairVLQDecays:
        SKey += "_" + sampleDef.get('decay')
    # if not sampleDef.get('decay') in doPairVLQDecays:
    #     continue

    # Setting the normalisation
    scale = 1
    if(SType.upper().find("DATA")==-1 and SType.upper().find("QCD")==-1 ):
        scale = dataLumi
    if reweightSignals:
        if(SType.upper().find("ZT")!=-1) or (SType.upper().find("HT")!=-1):
            scale = NormaliseSample(SDSID,dataLumi,signalXsec)

    if SKey not in allSampleKeys or ( splitPairVLQ and sampleDef.get('decay') in doPairVLQDecays ):
        allSampleKeys.append(SKey)
        # CommDict[SKey] = "hadd "+name_temp_rootfile
        CommDict[SKey] = "hadd -f "+name_temp_rootfile
        SNumber = str(len(allSampleKeys))

        if len(SNumber) == 1:
            SNumber = "0"+SNumber

        f_sample_list.write("NEW\n")
        f_sample_list.write("NAME : " + SNumber + "_" + SKey + "\n")
        f_sample_list.write("LEGLABEL : " + sampleDef['legend'] + "\n")
        if splitPairVLQ:
            if 'inpattern' in sampleDef.keys():
                f_sample_list.write("INPATTERN : " + sampleDef['inpattern'] + "\n")
        f_sample_list.write("STYLEKEY : " + SNumber + "\n")
        f_sample_list.write("DRAWOPT : " + sampleDef['drawopt'] + "\n")
        f_sample_list.write("DRAWSTACK : " + sampleDef['drawstack'] + "\n")
        f_sample_list.write("RESOPT : " + sampleDef['resopt'] + "\n")
        f_sample_list.write("DRAWSCALE : " + sampleDef['drawscale'] + "\n")
        f_sample_list.write("LEGOPT : " + sampleDef['legopt'] + "\n")
        f_sample_list.write("YIELDFORMAT : " + sampleDef['yieldformat'] + "\n")
        f_sample_list.write("WRITE : TRUE\n")
        f_sample_list.write("OUTFILENAME : " + SKey.replace("#","_").replace("{","_").replace("}","_").replace(" ","_") + ".root\n")
        f_sample_list.write("\n")

        # Adding the sample copy to be scaled to the SUM
        if doScaledSignalDisplay and sampleDef['drawopt']=="DUMMY":
            f_sample_list.write("NEW\n")
            f_sample_list.write("NAME : " + SNumber + "_" + SKey + "_display\n")
            if splitPairVLQ:
                if 'inpattern' in sampleDef.keys():
                    f_sample_list.write("INPATTERN : " + sampleDef['inpattern'] + "\n")
            f_sample_list.write("STYLEKEY : " + SNumber + "\n")
            f_sample_list.write("DRAWOPT : hist\n")
            f_sample_list.write("DRAWSTACK : " + sampleDef['drawstack'] + "\n")
            f_sample_list.write("RESOPT : SKIP\n")
            f_sample_list.write("DRAWSCALE : NORM\n")
            f_sample_list.write("WRITE : TRUE\n")
            f_sample_list.write("OUTFILENAME : " + SKey.replace("#","_").replace("{","_").replace("}","_").replace(" ","_") + ".root\n")
            f_sample_list.write("SCALETOREF : SUM\n")
            f_sample_list.write("\n")

        f_style_list.write("NEW\n")
        f_style_list.write("KEY : " + SNumber + "\n")
        f_style_list.write("LINECOLOUR : " + sampleDef['linecolor'] + "\n")
        f_style_list.write("FILLCOLOUR : " + sampleDef['fillcolor'] + "\n")
        f_style_list.write("MARKERCOLOUR : " + sampleDef['markercolor'] + "\n")
        f_style_list.write("LINEWIDTH : " + sampleDef['linewidth'] + "\n")
        f_style_list.write("MARKERSIZE : " + sampleDef['markersize'] + "\n")
        f_style_list.write("LINESTYLE : " + sampleDef['linestyle'] + "\n")
        f_style_list.write("FILLSTYLE : " + sampleDef['fillstyle'] + "\n")
        f_style_list.write("MARKERSTYLE : " + sampleDef['markerstyle'] + "\n")
        f_style_list.write("\n")

        if doScaledSignalDisplay and sampleDef['drawopt']=="DUMMY":
            f_files_list.write(SNumber+"_"+SKey+","+SNumber+"_"+SKey+"_display" + " : ")
        else:
            f_files_list.write(SNumber+"_"+SKey + " : ")
        f_files_list.write(name_temp_rootfile + " : ")
        f_files_list.write(str(scale)+" \n")

    listfiles = list(set(listfiles))

    for f in listfiles:
        if f.find("_"+SType)>-1 and f.find("_"+SDSID)>-1:
            CommDict[SKey] += " " + f

if doSumSample:
    #DEFINING THE SUM SAMPLE
    f_sample_list.write("NEW\n")
    f_sample_list.write("NAME : SUM\n")
    f_sample_list.write("LEGLABEL : SM Total\n")
    f_sample_list.write("STYLEKEY : 100\n")
    f_sample_list.write("DRAWOPT : e2\n")
    f_sample_list.write("DRAWSTACK : FALSE\n")
    if not isMCMC:
        f_sample_list.write("RESOPT : REF\n")
    else:
        f_sample_list.write("RESOPT : REF\n")
    if not isMCMC:
        f_sample_list.write("DRAWSCALE : NONE\n")
    else:
        f_sample_list.write("DRAWSCALE : NORM\n")
    f_sample_list.write("LEGOPT : f\n")
    f_sample_list.write("YIELDFORMAT : %.2f\n")
    f_sample_list.write("\n")
    
    f_style_list.write("NEW\n")
    f_style_list.write("KEY : 100\n")
    f_style_list.write("LINECOLOUR : kBlack\n")
    f_style_list.write("FILLCOLOUR : kBlack\n")
    f_style_list.write("MARKERCOLOUR : kBlack\n")
    f_style_list.write("LINEWIDTH : 2\n")
    f_style_list.write("MARKERSIZE : 0\n")
    f_style_list.write("LINESTYLE : 1\n")
    f_style_list.write("FILLSTYLE : 3005\n")
    f_style_list.write("MARKERSTYLE : 0\n")
    f_style_list.write("\n")
    
f_files_list.write("END\n")
f_sample_list.write("END\n")
f_style_list.write("END\n")
f_files_list.close()

if dumpFiles:
    alldonecommands = []
    for key in allSampleKeys:
        # filter commands for potential doubles
        if CommDict[key] in alldonecommands:
            continue
        print( "\n",CommDict[key],"\n")
        os.system(CommDict[key])
        alldonecommands.append(CommDict[key])

if makePlots:
    if not listKeyword:
        printError("Please provide a listKeyWord when choosing to plot")
    else:
        os.system('cd %s'%os.getcwd())
        print( '\nplot --SAMPLES="ConfigFiles_%s/sample_list.list" --VARIABLES="ConfigFiles_%s/vars_list.list" --STYLELIB="ConfigFiles_%s/style_list.list" --FILELIST="ConfigFiles_%s/files_list.list" --OUTFORMAT="PNG,PDF" --TITLEXMIN=0.17 --TITLEYMIN=0.765 --TITLEXMAX=0.42 --TITLEYMAX=0.915 --NEWCONFIG=TRUE --NEWFILELIST=FALSE --LEGENDTEXTSIZE=0.036 --LEGENDXMAX=0.95 --LEGENDYMAX=0.9 --YTITLEOFFSET=0.85 --RESDRAWOPT=%s --MAKEMOMENTSTABLE=FALSE --OUTPUTFOLDER="Plots_%s" --SHOWYIELDS=%s --PRINTVALUE=FALSE --RESMIN="0.2" --RESMAX="1.8"\n'%(listKeyword,listKeyword,listKeyword,listKeyword,drawRes,listKeyword,showYields))
        with open("tempplot.sh",'w') as f:
            f.write('#!/bin/bash\n')
            f.write('plot --SAMPLES="ConfigFiles_%s/sample_list.list" --VARIABLES="ConfigFiles_%s/vars_list.list" --STYLELIB="ConfigFiles_%s/style_list.list" --FILELIST="ConfigFiles_%s/files_list.list" --OUTFORMAT="PNG,PDF" --TITLEXMIN=0.17 --TITLEYMIN=0.765 --TITLEXMAX=0.42 --TITLEYMAX=0.915 --NEWCONFIG=TRUE --NEWFILELIST=FALSE --LEGENDTEXTSIZE=0.036 --LEGENDXMAX=0.95 --LEGENDYMAX=0.9 --YTITLEOFFSET=0.85 --RESDRAWOPT=%s --MAKEMOMENTSTABLE=FALSE --OUTPUTFOLDER="Plots_%s" --SHOWYIELDS=%s --PRINTVALUE=FALSE --RESMIN="0.2" --RESMAX="1.8"\n'%(listKeyword,listKeyword,listKeyword,listKeyword,drawRes,listKeyword,showYields))
        f.close()
        os.system('chmod -x tempplot.sh')
        # os.system('source tempplot.sh')
        # os.system('rm tempplot.sh')

        # os.system('plot --SAMPLES="ConfigFiles_%s/sample_list.list" --VARIABLES="ConfigFiles_%s/vars_list.list" --STYLELIB="ConfigFiles_%s/style_list.list" --FILELIST="ConfigFiles_%s/files_list.list" --OUTFORMAT="PNG,PDF" --TITLEXMIN=0.17 --TITLEYMIN=0.765 --TITLEXMAX=0.42 --TITLEYMAX=0.915 --NEWCONFIG=TRUE --NEWFILELIST=FALSE --LEGENDTEXTSIZE=0.033 --LEGENDXMAX=0.85 --LEGENDYMAX=0.92 --YTITLEOFFSET=1.5 --RESDRAWOPT="FALSE" --MAKEMOMENTSTABLE=FALSE --OUTPUTFOLDER="Plots_%s" --SHOWYIELDS=FALSE --PRINTVALUE=FALSE'%(listKeyword,listKeyword,listKeyword,listKeyword,listKeyword))

    print( "\n")
