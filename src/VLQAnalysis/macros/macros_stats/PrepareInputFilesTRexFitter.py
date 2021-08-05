#!/bin/python
import os
import time, getpass
import sys
import datetime
from ROOT import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/")
from VLQ_Samples_mc import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/IFAETopFramework/" )
from BatchTools import *
from Samples import *

##------------------------------------------------------
## Checking the arguments
##------------------------------------------------------
if(len(sys.argv)<3):
    printWarning(sys.argv[0] + " ==> Wrong input arguments")
    print ""
    print "    python "+sys.argv[0]+" [arg]"
    print ""
    print "Arguments"
    print "========="
    print "    inputDir=<path to input files>"
    print "    ttbarSyst=<TRUE/FALSE> files for ttbar systematics *only*"
    print "    singletopSyst=<TRUE/FALSE> files for singletop systematics *only*"
    print "    splitSingletop=<TRUE/FALSE> split singletop channels"
    print "    mergeSingletop=<TRUE/FALSE> merge split singletop channels at the end"
    print "    mergeRareBkg=<TRUE/FALSE> merge rare background samples at the end"
    print "    singletopNominalDir=<path to singletop nominal input files (required for DiagSub merging)>"
    print "    useData=<TRUE/FALSE> use the real data"
    print "    useBkgd=<TRUE/FALSE> use background samples"
    print "    useSystematics=<TRUE/FALSE> run over systematics"
    print "    allBR=<TRUE/FALSE> run all BR configs for 2D plots"
    print "    splitCampaigns=<TRUE/FALSE> merge separately by mc campaign"
    print "    signal=<pair/single/NONE> Use pair or single VLQ samples for signal"
    print "    [outputDir]=<place where to store the output files>"
    print ""
    sys.exit()

##------------------------------------------------------
## Defines some useful variables
##------------------------------------------------------
now = datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
here = os.getcwd()

##------------------------------------------------------
## Selects the arguments
##------------------------------------------------------
inputDir=""
ttbarSyst=False
singletopSyst=False
splitSingletop=False
mergeSingletop=False
mergeRareBkg=False
singletopNominalDir=""
useData = True
useBkgd = True
folderRootFiles = "RootFilesForTtHFitter_"+now
useSystematics = True
doAllBR=False
splitCampaigns=False
signal="NONE"
for iArg in range(1,len(sys.argv)):
    splitted=sys.argv[iArg].split("=")
    argument = splitted[0].upper()
    if(argument=="INPUTDIR"): inputDir = splitted[1]
    elif(argument=="USEDATA"):
        if splitted[1].upper()=="TRUE":
            useData = True
        elif splitted[1].upper()=="FALSE":
            useData = False
    elif(argument=="USEBKGD"):
        if splitted[1].upper()=="TRUE":
            useBkgd = True
        elif splitted[1].upper()=="FALSE":
            useBkgd = False
    elif(argument=="TTBARSYST"):
        if splitted[1].upper()=="TRUE":
            ttbarSyst = True
        elif splitted[1].upper()=="FALSE":
            ttbarSyst = False
        else:
            printWarning("/!\ The argument for ttbarSyst is not recognised ... Please check !")
    elif(argument=="SINGLETOPSYST"):
        if splitted[1].upper()=="TRUE":
            singletopSyst = True
        elif splitted[1].upper()=="FALSE":
            singletopSyst = False
        else:
            printWarning("/!\ The argument for singletopSyst is not recognised ... Please check !")
    elif(argument=="SPLITSINGLETOP"):
        if splitted[1].upper()=="TRUE":
            splitSingletop = True
        elif splitted[1].upper()=="FALSE":
            splitSingletop = False
        else:
            printWarning("/!\ The argument for splitSingletop is not recognised ... Please check !")
    elif(argument=="MERGESINGLETOP"):
        if splitted[1].upper()=="TRUE":
            mergeSingletop = True
        elif splitted[1].upper()=="FALSE":
            mergeSingletop = False
        else:
            printWarning("/!\ The argument for mergeSingletop is not recognised ... Please check !")
    elif(argument=="MERGERAREBKG"):
        if splitted[1].upper()=="TRUE":
            mergeRareBkg = True
        elif splitted[1].upper()=="FALSE":
            mergeRareBkg = False
        else:
            printWarning("/!\ The argument for mergeRareBkg is not recognised ... Please check !")
    elif(argument=="SINGLETOPNOMINALDIR"): 
        singletopNominalDir = splitted[1]
    elif(argument=="OUTPUTDIR"):
        folderRootFiles = splitted[1]
    elif(argument=="USESYSTEMATICS"):
        if splitted[1].upper()=="TRUE":
            useSystematics = True
        elif splitted[1].upper()=="FALSE":
            useSystematics = False
        else:
            printWarning("/!\ The argument for useSystematics is not recognised ... Please check !")
    elif(argument=="SIGNAL"):
        signal=splitted[1].upper()
        if( not(signal == "PAIR" or signal == "SINGLE" or signal == "NONE") ):
            printWarning("/!\ The argument for SIGNAL is not recognised ... Please check !")
    elif(argument=="ALLBR"):
        if splitted[1].upper()=="TRUE":
            doAllBR = True
        elif splitted[1].upper()=="FALSE":
            doAllBR = False
        else:
            printWarning("/!\ The argument for allBR is not recognised ... Please check !")
    elif(argument=="SPLITCAMPAIGNS"):
        if splitted[1].upper()=="TRUE":
            splitCampaigns = True
        elif splitted[1].upper()=="FALSE":
            splitCampaigns = False
        else:
            printWarning("/!\ The argument for splitCampaigns is not recognised ... Please check !")
    else:
        printWarning("/!\ Unrecognized argument ("+splitted[0]+") ! Please check !")
if(inputDir==""):
    printError("<!> Please provide an input config file to use !")
    sys.exit()

##------------------------------------------------------
## Creating the output repository
##------------------------------------------------------
os.system("mkdir -p "+folderRootFiles)

##------------------------------------------------------
## Getting all samples and their associated weight/object systematics
##------------------------------------------------------
Samples = []

campaigns = []
if splitCampaigns:
    #campaigns = ["mc16a"]
    campaigns = ["mc16a", "mc16d", "mc16e"]
else:
    campaigns = [""]

#ttbar+singletop systematics
if(ttbarSyst or singletopSyst):
    if(ttbarSyst):  
        for mc_campaign in campaigns:
            Samples += GetTtbarSamples( useObjectSyst=False, hfSplitted = True, ttbarSystSamples = True, campaign=mc_campaign )
    if(singletopSyst):
        for mc_campaign in campaigns:
            Samples += GetSingleTopSamples( useObjectSyst=False, campaign=mc_campaign, splitChannel=splitSingletop, runSingletopSystSamples=True)

else:
    #-- Data
    if(useData):
        #Samples += GetQCDSamples( ) 
        Samples += GetDataSamples( sampleName = "Data", data_type = "DAOD_TOPQ1" )

        #Samples += GetDataSamples( sampleName = "Data", data_type = "DAOD_TOPQ4" )

        #Samples += GetDataSamples( sampleName = "Data_data.DAOD_TOPQ1.", data_type = "DAOD_TOPQ1" ) 
    #-- MC backgrounds and 4tops signals
    if(useBkgd):
        for mc_campaign in campaigns:
            Samples += GetTtbarSamples( useObjectSyst=useSystematics, hfSplitted = True, ttbarSystSamples = False, campaign=mc_campaign )
            Samples += GetOtherBackgroundSamples( useObjectSyst=useSystematics, campaign=mc_campaign
                                                  , includeSingleTop=True, splitSTChannels=splitSingletop
                                                  , includeWjets=True, includeZjets=True
                                                  , includeTopEW=True, includeDibosons=True, includeDijet=True
                                                  , includeSingletopSystSamples=False )

    ObjectSystematics = []
    if useSystematics:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]



    VLQ_masses = []
    if(signal=="PAIR"):
        # -- VLQ samples
        VLQ_masses = ["600","800","1000","1100","1200","1300","1400","1500","1600","1700","1800","2000"]

        for mass in VLQ_masses:
            for mc_campaign in campaigns:
                if mc_campaign:
                    mc_campaign = "."+mc_campaign
                Samples += [getSampleUncertainties( "VLQ_TT_" + mass + "_TSinglet"+mc_campaign, "", ObjectSystematics , [])]
                Samples += [getSampleUncertainties( "VLQ_TT_" + mass + "_TDoublet"+mc_campaign, "", ObjectSystematics , [])]
                Samples += [getSampleUncertainties( "VLQ_TT_" + mass + "_HtHt"+mc_campaign,     "", ObjectSystematics , [])]
                Samples += [getSampleUncertainties( "VLQ_TT_" + mass + "_ZtZt"+mc_campaign,     "", ObjectSystematics , [])]
                #Samples += [getSampleUncertainties( "VLQ_TT_" + mass + "_HtZt"+mc_campaign,     "", ObjectSystematics , [])]
            if doAllBR:
                step_width = 0.05
                steps_n = int((1 + step_width)/step_width)
                for iHt in range(0,steps_n):
                    for iWb in range(0,steps_n):
                        for iZt in range(0,steps_n):
                            coupling_Ht = iHt * step_width
                            coupling_Zt = iZt * step_width
                            coupling_Wb = iWb * step_width
                            if abs(coupling_Ht+coupling_Wb+coupling_Zt-1.)<0.001:
                                #for mc_campaign in campaigns:
                                    #if mc_campaign:
                                        #mc_campaign = "."+mc_campaign
                                Samples += [getSampleUncertainties("VLQ_TT_" + mass + 
                                                                   "_BR_%.2f_%.2f_%.2f" %(coupling_Wb,coupling_Zt,coupling_Ht)#+campaign
                                                                   ,"", ObjectSystematics , [])]
    elif(signal=="SINGLE"):
        Samples +=  GetOldSingleVLQSamples( useObjectSyst=useSystematics )

printGoodNews("--> All samples recovered")
##........................................................

##------------------------------------------------------
## Loop over the samples and systematics
##------------------------------------------------------
Combination = []
Commands = []
listfiles = glob.glob(inputDir+"/outVLQAna_*.root")
for sample in Samples:

    SName = sample['name'] # sample name
    SType = sample['sampleType']
    MC_campaign=""
    if ".mc16a" in SName:
        MC_campaign=".mc16a"
    elif ".mc16d" in SName:
        MC_campaign=".mc16d"
    elif ".mc16e" in SName:
        MC_campaign=".mc16e"


    print "-> Sample: " + SType + "; " + SName + "; " + MC_campaign

    cleaned_sampleType = SType.replace("#","").replace(" ","").replace("{","").replace("}","").replace("+","").replace("(","").replace(")","")

    #Do the list of systematics
    systList = []
    for Systematic in sample['objSyst']:
        if Systematic['oneSided']:
            systList += [Systematic['name']]
        else:
            systList += [Systematic['nameUp']]
            systList += [Systematic['nameDown']]

    ##------------------------------------------------------
    ## Loop over the systematics
    ##------------------------------------------------------
    for syst in systList:
        print "   -> Syst: " + syst
        name_temp_rootfile = folderRootFiles+"/"+cleaned_sampleType+MC_campaign
        print name_temp_rootfile
        if syst.upper().find("NOMINAL")==-1:
            name_temp_rootfile += "_"+syst
        name_temp_rootfile += ".root"

        if not name_temp_rootfile in Combination:
            Combination += [name_temp_rootfile]
            com = "hadd " + name_temp_rootfile
            usedFiles = []
            SType = sample['sampleType']
            for f in listfiles:
                if f.find("outVLQAna_"+SType+"_")>-1 and f.find(syst)>-1 and f.find(MC_campaign)>-1:
                    com += " " + f
                    usedFiles += [f]
            Commands += [com]
            for usedfile in usedFiles:
                listfiles.remove(usedfile)
            usedFiles = []
        else:
            counter = 0
            for Comb in Combination:
                if(Comb==name_temp_rootfile):
                    usedFiles = []
                    for f in listfiles:
                        if f.find("outVLQAna_"+SType+"_")>-1 and f.find(syst)>-1 and f.find(MC_campaign)>-1:
                            Commands[counter] += " " + f
                            usedFiles += [f]
                    for usedfile in usedFiles:
                        listfiles.remove(usedfile)
                    usedFiles = []
                counter += 1

##------------------------------------------------------
## Actually adding the rootfiles
##------------------------------------------------------
for Comm in Commands:
    command = Comm
    splittedCommand = Comm.split(" ")
    if len(splittedCommand)==3:
        com = "cp "+splittedCommand[2]+" "+splittedCommand[1]
        printGoodNews("-> Copying file for sample: " + splittedCommand[1])
        os.system(com)
    elif len(splittedCommand)>3:
        printGoodNews("-> Hadding files for sample: " + splittedCommand[1])
        os.system(Comm)
    else:
        printError( "Invalid command line: " + Comm )

##------------------------------------------------------
## Merge single top files if needed
##------------------------------------------------------
if mergeSingletop:
    printGoodNews("--> Now merging single-top channels")
    systVar = []
    if singletopSyst:
        systVar = ["","PowHer","aMCPy"]
    else:
        STfiles = glob.glob(folderRootFiles+"/Singletop*mc16*root")
        for f in STfiles:
            systclean = f.replace(folderRootFiles,"").replace("mc16a_","").replace("mc16d_","").replace("mc16e_","").replace(".root","")
            systclean = systclean.replace("mc16a","").replace("mc16d","").replace("mc16e","")
            systclean = systclean.replace("Singletop.","").replace("Singletopschan.","").replace("Singletoptchan.","").replace("SingletopWtprod.","")
            systVar.append(systclean)
        systVar = list(set(systVar))
    for mc_campaign in campaigns:
        if mc_campaign:
            mc_campaign = "."+mc_campaign
        for syst in systVar:
            if singletopSyst or syst=="":
                filesuffix = syst+mc_campaign+".root "
            else:
                filesuffix = mc_campaign+"_"+syst+".root "
            com = "hadd "+folderRootFiles+"/Singletop"+filesuffix
            com += folderRootFiles+"/Singletopschan"+filesuffix
            com += folderRootFiles+"/Singletoptchan"+filesuffix
            com += folderRootFiles+"/SingletopWtprod"+filesuffix

            printGoodNews("  --> "+mc_campaign+" ; "+syst)
            os.system(com)
        if singletopNominalDir:
            printGoodNews("  --> DiagSub "+mc_campaign)
            com = "hadd "+folderRootFiles+"/SingletopDiagSub"+mc_campaign+".root "  
            com += singletopNominalDir+"/Singletopschan"+mc_campaign+".root "
            com += singletopNominalDir+"/Singletoptchan"+mc_campaign+".root "
            com += folderRootFiles+"/SingletopWtprodDiagSub"+mc_campaign+".root "
            os.system(com)

##------------------------------------------------------
## Merge rare backgrounds (tZ, SM4tops, VH)
##------------------------------------------------------
if mergeRareBkg:
    printGoodNews("--> Now merging rare backgrounds")
    systVar = []

    RBfiles = glob.glob(folderRootFiles+"/tZ*mc16*root")
    RBfiles.extend(glob.glob(folderRootFiles+"/SM4tops*mc16*root"))
    RBfiles.extend(glob.glob(folderRootFiles+"/VH*mc16*root"))

    for f in RBfiles:
        systclean = f.replace(folderRootFiles,"").replace("mc16a_","").replace("mc16d_","").replace("mc16e_","").replace(".root","")
        systclean = systclean.replace("mc16a","").replace("mc16d","").replace("mc16e","")
        systclean = systclean.replace("tZ.","").replace("SM4tops.","").replace("VH.","")

        systVar.append(systclean)
    systVar = list(set(systVar))

    for mc_campaign in campaigns:
        if mc_campaign:
            mc_campaign = "."+mc_campaign
        for syst in systVar:
            if syst=="":
                filesuffix = syst+mc_campaign+".root "
            else:
                filesuffix = mc_campaign+"_"+syst+".root "
            printGoodNews("  --> "+mc_campaign+" ; "+syst)
            com = "hadd "+folderRootFiles+"/rareBkg"+filesuffix
            com += folderRootFiles+"/tZ"+filesuffix
            com += folderRootFiles+"/SM4tops"+filesuffix
            com += folderRootFiles+"/VH"+filesuffix

            os.system(com)
