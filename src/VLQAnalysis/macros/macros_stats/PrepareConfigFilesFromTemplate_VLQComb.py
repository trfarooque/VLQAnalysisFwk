#!/bin/python
import os
import sys

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )
from VLQ_Samples import *
from regions_dictionary import *

##------------------------------------------------------
## Checking the arguments
##------------------------------------------------------
if(len(sys.argv)<5):
    printWarning("Output checker ==> Wrong input arguments")
    print ""
    print "    python "+sys.argv[0]+" [arg]"
    print ""
    print "Arguments"
    print "========="
    print "    inputTemplate=<path to the template config file>"
    print "    outputFolder=<place where to store the config files>"
    print "    inputDir=<absolute path to rootfiles for TRexFitter>"
    print "    systConfig=<coma separated list of absolute paths to the systematics template config files>"
    print "    addition=<additional information to be stored in the names of the jobs, scripts, folders, ...>"
    print "    signal=<ALL/ONE> ALL: all signals are put in the config file (default), ONE: only one signal in each config file (default is ALL)"
    print "    normfactor=<Init,Low,High> settings for the signal normalisation factor (default is: 1,0,100)"
    print "    doZeroLep=TRUE/FALSE use the zero lepton regions"
    print "    doOneLep=TRUE/FALSE use the one lepton regions"
    print "    doOneLepEMu=TRUE/FALSE use the separate e/mu regions in one lepton"
    print "    doVR=TRUE/FALSE use the validation regions"
    print "    useBlindingCuts=TRUE/FALSE use the meff blinding cuts"
    print ""
    print "Other options can also be provided. Possible options are in the following list:"
    print "  -> lumiValue, addition, histoPath, statOnly, fitType, fitRegion, fitPOIAsimov, limitBlind, limitPOIAsimov"
    print ""
    sys.exit()

##------------------------------------------------------
## Defines some useful variables
##------------------------------------------------------
here = os.getcwd()

##------------------------------------------------------
## Options
##------------------------------------------------------
_ARGUMENTS_ = []
#Job arguments
_ARGUMENTS_ += [{'arg':"__LUMIVALUE__",'value':"36097.6"}]
_ARGUMENTS_ += [{'arg':"__ADDITION__",'value':""}]
_ARGUMENTS_ += [{'arg':"__STATONLY__",'value':"FALSE"}]
#Fit
_ARGUMENTS_ += [{'arg':"__FITTYPE__",'value':"BONLY"}]#BONLY, SPLUSB
_ARGUMENTS_ += [{'arg':"__FITREGION__",'value':"CRSR"}]#CRSR, CRONLY
_ARGUMENTS_ += [{'arg':"__FITPOIASIMOV__",'value':"0"}]
_ARGUMENTS_ += [{'arg':"__FITBLIND__",'value':"TRUE"}]
#Limit
_ARGUMENTS_ += [{'arg':"__LIMITBLIND__",'value':"TRUE"}]
_ARGUMENTS_ += [{'arg':"__LIMITPOIASIMOV__",'value':"0"}]

##------------------------------------------------------
## Selects the arguments
##------------------------------------------------------
inputTemplateConfigFile = ""
outputFolder = ""
inputDir = ""
addition = ""
signalHandling = "ALL"
SignalNormFactor = "1,0,100"
systConfig = []
for syst_file in ["BackNorm", "Objects", "Weights"]:
    systConfig += [os.getenv("VLQAnalysisFramework_DIR")+"/../VLQAnalysis/macros/macros_stats/templates/TEMPLATE_Systematics_"+syst_file+".txt"]
doAllBR = False
useFourTopsAsBack = True
doZeroLep = False
doOneLep = False
doOneLepEMu = False
doVR = False
useBlindingCuts = False
useData = False
for iArg in range(1,len(sys.argv)):
    splitted=sys.argv[iArg].split("=")
    argument = splitted[0].upper()
    if(argument=="INPUTTEMPLATE"):
        inputTemplateConfigFile = splitted[1]
    elif(argument=="OUTPUTFOLDER"):
        outputFolder = splitted[1]
    elif(argument=="INPUTDIR"):
        inputDir = splitted[1]
    elif(argument=="SIGNAL"):
        signalHandling = splitted[1].upper()
    elif(argument=="NORMFACTOR"):
        SignalNormFactor = splitted[1]
    elif(argument=="SYSTCONFIG"):
        systConfig = splitted[1].split(",")
    elif(argument=="USE4TOPS"):
        if splitted[1].upper()=="TRUE": useFourTopsAsBack = True
        elif  splitted[1].upper()=="FALSE": useFourTopsAsBack = False
    elif(argument=="DOALLBR"):
        if splitted[1].upper()=="TRUE": doAllBR = True
        elif splitted[1].upper()=="FALSE": doAllBR = False
        else: printError("Error for DOALLBR")
    elif(argument=="DOZEROLEP"):
        if splitted[1].upper()=="TRUE": doZeroLep = True
        elif splitted[1].upper()=="FALSE": doZeroLep = False
        else: printError("Error for DOZEROLEP")
    elif(argument=="DOONELEP"):
        if splitted[1].upper()=="TRUE": doOneLep = True
        elif splitted[1].upper()=="FALSE": doOneLep = False
        else: printError("Error for DOONELEP")
    elif(argument=="DOONELEPEMU"):
        if splitted[1].upper()=="TRUE": doOneLepEMu = True
        elif splitted[1].upper()=="FALSE": doOneLepEMu = False
        else: printError("Error for DOONELEPEMU")
    elif(argument=="DOVR"):
        if splitted[1].upper()=="TRUE": doVR = True
        elif splitted[1].upper()=="FALSE": doVR = False
        else: printError("Error for DOVR")
    elif(argument=="USEBLINDINGCUTS"):
        if splitted[1].upper()=="TRUE": useBlindingCuts = True
        elif splitted[1].upper()=="FALSE": useBlindingCuts = False
        else: printError("Error for USEBLINDINGCUTS")
    elif(argument=="USEDATA"):
        if splitted[1].upper()=="TRUE": useData = True
        elif splitted[1].upper()=="FALSE": useData = False
        else: printError("Error for USEDATA")
    else:
        for arg in _ARGUMENTS_:
            if arg['arg'] != "__"+argument+"__": continue
            if arg['arg'] == "__ADDITION__":
                arg['value'] += "_" + splitted[1]
                addition += "_" + splitted[1]
            else:
                arg['value'] = splitted[1]

if(inputTemplateConfigFile==""):
    printError("<!> Please provide an input config file to use !")
    sys.exit()

##------------------------------------------------------
## Options (that can only be filled aftewards ...)
##------------------------------------------------------
_ARGUMENTS_ += [{'arg':"__HISTOPATH__",'value':inputDir}]

##------------------------------------------------------
## Creating the output folder
##------------------------------------------------------
os.system( "mkdir -p " + outputFolder )
##------------------------------------------------------
## Getting all signals
##------------------------------------------------------
Signals = []
VLQ_masses = ["600","700","750","800","850","900","950","1000","1050","1100","1150","1200","1300","1400","1500"]
VLQB_masses = ["600","700","750","800","850","900","950","1000","1050","1100","1150","1200","1300","1400","1500","1600"]
if doAllBR:
    VLQ_masses = ["600","700","750","800","850","900","950","1000","1050","1100","1150","1200","1300","1400","1500"]

for mass in VLQ_masses:
    if doAllBR:
        step_width = 0.1
        steps_n = int((1 + step_width)/step_width)
        for iHt in range(0,steps_n):
            for iWb in range(0,steps_n):
                for iZt in range(0,steps_n):
                    coupling_Ht = iHt * step_width
                    coupling_Zt = iZt * step_width
                    coupling_Wb = iWb * step_width
                    if abs(coupling_Ht+coupling_Wb+coupling_Zt-1.)<0.001:
                        Signals += [getSampleUncertainties("VLQ_TT_" + mass + "_BR_%.2f_%.2f_%.2f" %(coupling_Wb,coupling_Zt,coupling_Ht),"VLQ_TT_" + mass + "_BR_%.2f_%.2f_%.2f" %(coupling_Wb,coupling_Zt,coupling_Ht), CommonObjectSystematics , [])]
    else:
        #Signals += [getSampleUncertainties("VLQ_TT_" + mass + "_TDoublet","VLQ_TT_" + mass + "_Doublet", CommonObjectSystematics , [])]
        #Signals += [getSampleUncertainties("VLQ_TT_" + mass + "_TSinglet","VLQ_TT_" + mass + "_Singlet", CommonObjectSystematics , [])]
        #Signals += [getSampleUncertainties("VLQ_TT_" + mass + "_HtHt","VLQ_TT_" + mass + "_BR_0.00_0.00_1.00", CommonObjectSystematics , [])]
        #Signals += [getSampleUncertainties("VLQ_TT_" + mass + "_ZtZt","VLQ_TT_" + mass + "_BR_0.00_1.00_0.00", CommonObjectSystematics , [])]
        Signals += [getSampleUncertainties("VLQ_TT_" + mass + "_BR_1.00_0.00_0.00","VLQ_TT_" + mass + "_BR_1.00_0.00_0.00", CommonObjectSystematics , [])]

for mass in VLQB_masses:
    #Signals += [getSampleUncertainties("VLQ_BB_" + mass + "_BDoublet","VLQ_BB_" + mass + "_Doublet", CommonObjectSystematics , [])]
    #Signals += [getSampleUncertainties("VLQ_BB_" + mass + "_BSinglet","VLQ_BB_" + mass + "_Singlet", CommonObjectSystematics , [])]
    #Signals += [getSampleUncertainties("VLQ_BB_" + mass + "_HbHb","VLQ_BB_" + mass + "_BR_0.00_0.00_1.00", CommonObjectSystematics , [])]
    #Signals += [getSampleUncertainties("VLQ_BB_" + mass + "_ZbZb","VLQ_BB_" + mass + "_BR_0.00_1.00_0.00", CommonObjectSystematics , [])]
    Signals += [getSampleUncertainties("VLQ_BB_" + mass + "_BR_1.00_0.00_0.00","VLQ_BB_" + mass + "_BR_1.00_0.00_0.00", CommonObjectSystematics , [])]
    
#for mass in ["1000","1200","1400","1600","1800"]:
#    Signals += [getSampleUncertainties("UEDRPP_" + mass + ")","UEDRPP_" + mass, CommonObjectSystematics , [])]
#Signals += [getSampleUncertainties("CI4tops","CI4tops", CommonObjectSystematics , [])]
#Signals += [getSampleUncertainties("SM4tops","SM4tops", CommonObjectSystematics , [])]

##------------------------------------------------------
## Getting all regions
##------------------------------------------------------
Regions = []
if doOneLep:
    Regions += fit_regions_1l
    if doVR:
        Regions += validation_regions_1l#+validation_regions_1l_2b
if doZeroLep:
    Regions += fit_regions_0l
    if doVR:
        Regions += validation_regions_0l


##------------------------------------------------------
## Creating all the config files
##------------------------------------------------------
for counter,sample in enumerate(Signals):

    #Sample characteristics
    SName = sample['name']
    SType = sample['sampleType']
    TypeTemp = SType

    printGoodNews("-> Generating config file for " + SName + " , " + SType)

    cleaned_sampleType = SType.replace("#","").replace(" ","").replace("{","").replace("}","").replace("+","").replace("(","").replace(")","")

    #Open the template file
    f_template = open(inputTemplateConfigFile,'r')

    #Open the new file adapted to the signal
    template_file_path_split = inputTemplateConfigFile.split("/")
    template_file_name = template_file_path_split[len(template_file_path_split)-1]
    f_adapted = 0
    if signalHandling=="ONE":
        f_adapted = open(outputFolder + "/" + template_file_name.replace("_SIGNAL_",sample['name']).replace("TEMPLATE_","").replace(".txt",addition+".txt"),'w')
    else:
        mode = 'w'
        if counter>0:
            mode='a'
        f_adapted = open(outputFolder + "/" + template_file_name.replace("_SIGNAL_","AllSignals").replace("TEMPLATE_","").replace(".txt",addition+".txt"),mode)

    #Updating the config file with the user's configuration
    if (signalHandling=="ONE" or counter==0):
        for inputLine in f_template:
            corrected_line = inputLine
            if inputLine.find("_REGIONLIST_")>-1:
                    corrected_line = "\n"
                    for reg in Regions:
                        LepChannels = [""]
                        if (reg['name'].find("c1lep")>-1) and doOneLepEMu:
                            LepChannels += ["_el","_mu"]
                        binning_key = "binning"
                        if useBlindingCuts:
                            binning_key = "binning_blind"
                        #if(reg['name'].find("c1lep")>-1 and reg['name'].find("6jin")>-1):
                        #    binning_key = "binning_low"

                        if reg[binning_key]=="":
                            print "=> The binning seems to be undefined for this region ("+reg['name']+"). Skipping the region."
                            continue
                        for channel in LepChannels:    
                            corrected_line += "\n"
                            corrected_line += "Region: " + reg['name'] + channel + "\n"
                            corrected_line += "Type: " + reg['type'] + "\n"
                            corrected_line += "HistoName: " + reg['name'].replace("HTX_","") + channel + "_meff\n"
                            corrected_line += "VariableTitle: m_{eff} [GeV]\n"
                            corrected_line += "Binning: " + reg[binning_key] + "\n"
                            corrected_line += "Label: " + reg['legend'] + "\n"
                            corrected_line += "ShortLabel: " + reg['legend'] + "\n"
                            if(reg['name'].find("c1lep")>-1 and inputDir.find("/1lep")==-1):
                                corrected_line += "HistoPathSuff: 1lep/ \n"
                            elif(reg['name'].find("c0lep")>-1 and inputDir.find("/0lep")==-1):
                                corrected_line += "HistoPathSuff: 0lep/ \n"
                            corrected_line += "\n"
            if signalHandling=="ONE":
                corrected_line = corrected_line.replace("_SIGNAL_",SName)
            for arg in _ARGUMENTS_:
                corrected_line = corrected_line.replace(arg['arg'],arg['value'])
            f_adapted.write(corrected_line)

        #Adding the data sample if needed
        if(useData):
            f_adapted.write("\n")
            f_adapted.write("Sample: \"Data\"\n")
            f_adapted.write("Title: \"Data\"\n")
            f_adapted.write("Type: \"data\"\n")
            f_adapted.write("HistoFile: \"Data\"\n")

        #Adding the SM 4tops as a background in case this is not the investigated signal ... Check if the option is set before
        if(useFourTopsAsBack and SName.find("SM4tops")==-1):
            f_adapted.write("\n")
            f_adapted.write("Sample: \"OTHERS\"\n")
            f_adapted.write("Title: \"SM 4tops\"\n")
            f_adapted.write("Type: \"background\"\n")
            f_adapted.write("FillColor: 410\n")
            f_adapted.write("LineColor: 1\n")
            f_adapted.write("HistoFile: \"SM4tops\"\n")
            f_adapted.write("Group: \"Non-t#bar{t}\"\n\n")

    #Copying the content of the template systematics config file
    f_adapted.write("\n")
    for systfile in systConfig:
        f_templateSysts = open(systfile,'r')
        for systLine in f_templateSysts:
            f_adapted.write(systLine)
        f_adapted.write("\n")

    #Adding the signal at the very end of the config file
    if counter==0 or signalHandling=="ONE":
        f_adapted.write(" \n")
        f_adapted.write("% --------------------------- %\n")
        f_adapted.write("% --------- SIGNALS --------- %\n")
        f_adapted.write("% --------------------------- %\n")
        f_adapted.write(" \n")

    #f_adapted.write("Sample: \""+SName+"\"\n")
    f_adapted.write("Sample: SIGNAL\n")
    f_adapted.write("  Title: \""+TypeTemp+"\"\n")
    f_adapted.write("  Type: \"signal\"\n")
    f_adapted.write("  NormFactor: \"mu_signal\","+SignalNormFactor+"\n")
    f_adapted.write("  FillColor: 2\n")
    f_adapted.write("  LineColor: 2\n")
    f_adapted.write("  HistoFile: "+cleaned_sampleType+"\n")

    f_adapted.write(" \n")

    f_adapted.close()
    f_template.close()
