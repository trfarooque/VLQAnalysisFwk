#!/bin/python
import os
import sys
import importlib
import json

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )
from VLQ_Samples_mc import *
#from regions_dictionary import *

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
    print "    signalType=<PAIR/SINGLE> (default is PAIR)"
    print "    signalScaling=<DEFAULT/BENCHMARK> (default is DEFAULT)"
    print "    signalBenchmark=Benchmark cross section for signal normalisation"
    print "    doZeroLep=TRUE/FALSE use the zero lepton regions"
    print "    doOneLep=TRUE/FALSE use the one lepton regions"
    print "    doOneLepEMu=TRUE/FALSE use the separate e/mu regions in one lepton"
    print "    doSR=TRUE/FALSE use the search regions"
    print "    doVR=TRUE/FALSE use the validation regions"
    print "    doPresel=TRUE/FALSE use the preselection regions"
    print "    useBlindingCuts=TRUE/FALSE use the meff blinding cuts"
    print "    discriminant=<name of variable to be used as discriminant> (default is meff)"
    print "    discriminant_title=<title of variable to be used as discriminant> (default is m_{eff} [GeV])"
    print "    discriminant_VR=<name of variable to be used as discriminant in validation regions> (default is the same as discriminant)"
    print "    discriminant_VR_title=<title of variable to be used as discriminant in validation regions> (default is the same as discriminant_title)"
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
_ARGUMENTS_ += [{'arg':"__LUMIVALUE__",'value':"139020.02"}]
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
signalType = "PAIR"
signalScaling = "DEFAULT"
signalBenchmark = 0.1
SignalNormFactor = "1,0,100"
systConfig = []
#for syst_file in ["BackNorm", "Objects", "Weights"]:
for syst_file in ["Objects", "Weights_Exp"]:
    systConfig += [os.getenv("VLQAnalysisFramework_DIR")+"/../src/VLQAnalysis/macros/macros_stats/templates/TEMPLATE_Systematics_"+syst_file+".txt"]
doAllBR = False
useFourTopsAsBack = True
doZeroLep = False
doOneLep = False
doOneLepEMu = False
doSR = True
doVR = False
doPresel = False
useBlindingCuts = False
useData = False
discriminant = "meff"
discriminant_title = "m_{eff} [GeV]"
discriminant_VR = discriminant
discriminant_VR_title = discriminant_title
#regDict = "regions_dictionary_sVLQ_ORIG"
regDict = "regions_dictionary"

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
    elif(argument=="DOSR"):
        if splitted[1].upper()=="TRUE": doSR = True
        elif splitted[1].upper()=="FALSE": doSR = False
        else: printError("Error for DOSR")
    elif(argument=="DOVR"):
        if splitted[1].upper()=="TRUE": doVR = True
        elif splitted[1].upper()=="FALSE": doVR = False
        else: printError("Error for DOVR")
    elif(argument=="DOPRESEL"):
        if splitted[1].upper()=="TRUE": doPresel = True
        elif splitted[1].upper()=="FALSE": doPresel = False
        else: printError("Error for DOPRESEL")
    elif(argument=="USEBLINDINGCUTS"):
        if splitted[1].upper()=="TRUE": useBlindingCuts = True
        elif splitted[1].upper()=="FALSE": useBlindingCuts = False
        else: printError("Error for USEBLINDINGCUTS")
    elif(argument=="USEDATA"):
        if splitted[1].upper()=="TRUE": useData = True
        elif splitted[1].upper()=="FALSE": useData = False
        else: printError("Error for USEDATA")
    elif(argument=="DISCRIMINANT"):
        discriminant = splitted[1]
    elif(argument=="DISCRIMINANT_TITLE"):
        discriminant_title = splitted[1]
    elif(argument=="DISCRIMINANT_VR"):
        discriminant_VR = splitted[1]
    elif(argument=="DISCRIMINANT_VR_TITLE"):
        discriminant_VR_title = splitted[1]
    elif(argument=="REGIONS"):
        regDict = splitted[1]
    elif(argument=="SIGNALTYPE"):
        signalType = splitted[1].upper()
    elif(argument=="SIGNALSCALING"):
        signalScaling = splitted[1].upper()
    elif(argument=="SIGNALBENCHMARK"):
      signalBenchmark = float(splitted[1])
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

if(signalType=="PAIR"):
    VLQ_masses = ["600","800","1000","1100","1200","1300","1400","1500","1600","1700","1800","2000"]
    #VLQ_masses = ["1300","1400","1500","1600","1700"]
    VLQB_masses = ["600","700","750","800","850","900","950","1000","1050","1100","1150","1200","1300","1400","1500","1600"]
    
    #Signals += GetVLQTSamples( )
    

    if doAllBR:
        VLQ_masses = ["600","800","1000","1100","1200","1300","1400","1500","1600","1700","1800","2000"]
        #VLQ_masses = ["700","750","800","850","900","950","1000"]

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
                            Signals += [getSampleUncertainties("VLQ_TT_" + mass + "_BR_%.2f_%.2f_%.2f" %(coupling_Wb,coupling_Zt,coupling_Ht)
                                                            ,"VLQ_TT_" + mass + "_BR_%.2f_%.2f_%.2f" %(coupling_Wb,coupling_Zt,coupling_Ht)
                                                               , CommonObjectSystematics , [])]
        else:
            Signals += [getSampleUncertainties("VLQ_TT_" + mass + "_TDoublet","VLQ_TT_" + mass + "_TDoublet", CommonObjectSystematics , [])]
            Signals += [getSampleUncertainties("VLQ_TT_" + mass + "_TSinglet","VLQ_TT_" + mass + "_TSinglet", CommonObjectSystematics , [])]
            Signals += [getSampleUncertainties("VLQ_TT_" + mass + "_HtHt","VLQ_TT_" + mass + "_HtHt", CommonObjectSystematics , [])]
            Signals += [getSampleUncertainties("VLQ_TT_" + mass + "_ZtZt","VLQ_TT_" + mass + "_ZtZt", CommonObjectSystematics , [])]


            
    '''for mass in VLQB_masses:
        Signals += [getSampleUncertainties("VLQ_BB_" + mass + "_BDoublet","VLQ_BB_" + mass + "_BDoublet", CommonObjectSystematics , [])]
        Signals += [getSampleUncertainties("VLQ_BB_" + mass + "_BSinglet","VLQ_BB_" + mass + "_BSinglet", CommonObjectSystematics , [])]
        Signals += [getSampleUncertainties("VLQ_BB_" + mass + "_HbHb","VLQ_BB_" + mass + "_HbHb", CommonObjectSystematics , [])]
        Signals += [getSampleUncertainties("VLQ_BB_" + mass + "_ZbZb","VLQ_BB_" + mass + "_ZbZb", CommonObjectSystematics , [])]'''

elif(signalType=="SINGLE"):

    VLQ_masses = ["low_mass"] #, "nom_mass"]

    VLQ_couplings =["K50"]
    for vlq_mass in VLQ_masses:
        for vlq_coupling in VLQ_couplings:
            Signals +=  GetSingleVLQSamples( RWName=vlq_mass+"_"+vlq_coupling )
            
    #Signals += GetOldSingleVLQSamples( )

#for mass in ["1000","1200","1400","1600","1800"]:
#    Signals += [getSampleUncertainties("UEDRPP_" + mass + ")","UEDRPP_" + mass, CommonObjectSystematics , [])]
#Signals += [getSampleUncertainties("CI4tops","CI4tops", CommonObjectSystematics , [])]
#Signals += [getSampleUncertainties("SM4tops","SM4tops", CommonObjectSystematics , [])]

##------------------------------------------------------
## Getting all regions
##------------------------------------------------------
Regions = []

regModule = importlib.import_module(regDict)


if doOneLep:
    if doSR:
        Regions += regModule.fit_regions_1l
    if doVR:
        Regions += regModule.validation_regions_1l#+validation_regions_1l_2b
    if doPresel:
        Regions += regModule.preselection_regions_1l
if doZeroLep:
    if doSR:
        Regions += regModule.fit_regions_0l
    if doVR:
        Regions += regModule.validation_regions_0l
    if doPresel:
        Regions += regModule.preselection_regions_0l

##------------------------------------------------------
## Creating all the config files
##------------------------------------------------------

#open json xsec database if needed
##________________________________________________________                                                                                                                                                
## Looking into the XSe DB file                                                                                                                                                                           
with open(os.getenv("VLQAnalysisFramework_DIR") + '/data/VLQAnalysis/samples_info/xsec_files/xsec_list.json','r') as f:
    xsecdata = json.load(f)

for counter,sample in enumerate(Signals):

    #Sample characteristics
    SName = sample['name']
    SType = sample['sampleType']
    TypeTemp = SType

    printGoodNews("-> Generating config file for " + SType)

    cleaned_sampleType = SType.replace("#","").replace(" ","").replace("{","").replace("}","").replace("+","").replace("(","").replace(")","")

    #Open the template file
    f_template = open(inputTemplateConfigFile,'r')

    #Open the new file adapted to the signal
    template_file_path_split = inputTemplateConfigFile.split("/")
    template_file_name = template_file_path_split[len(template_file_path_split)-1]
    f_adapted = 0
    if signalHandling=="ONE":
        f_adapted = open(outputFolder + "/" + template_file_name.replace("_SIGNAL_",SType).replace("TEMPLATE_","").replace(".txt",addition+".txt"),'w')
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
                            if reg['type']=="VALIDATION":
                                region_name_discriminant=""
                                if(reg['discriminant']):
                                    region_name_discriminant = "_"+reg['discriminant']
                                corrected_line += "Region: " + reg['name'] + region_name_discriminant + "_VR" + channel + "\n"
                            else:
                                corrected_line += "Region: " + reg['name'] + channel + "\n"
                            corrected_line += "Type: " + reg['type'] + "\n"

                            reg_discriminant = discriminant
                            if(reg['type']=="VALIDATION" and reg['discriminant']):
                                    reg_discriminant = reg['discriminant']

                            if (discriminant=="recoVLQ0_m"):
                                if('0Hex' in reg['name']):
                                    reg_discriminant = "Zt_" + discriminant
                                else:
                                    reg_discriminant = "Ht_" + discriminant

                            corrected_line += "HistoName: " + reg['name'].replace("HTX_","") + channel + "_" + reg_discriminant + "\n"
                            if(reg['type']=="VALIDATION" and reg['xtitle']):
                                corrected_line += "VariableTitle: " + reg['xtitle'] +"\n"
                            else: 
                                corrected_line += "VariableTitle: " + discriminant_title +"\n"
                            corrected_line += "Binning: " + reg[binning_key] + "\n"
                            #corrected_line += "Binning: \"AutoBin\",\"TransfoD\",3,3\n"
                            corrected_line += "Label: " + "\"" + reg['legend'] + "\"\n"
                            corrected_line += "ShortLabel: " + "\"" + reg['legend'] + "\"\n"
                            #if(reg['name'].find("c1lep")>-1 and inputDir.find("/1lep")==-1):
                             #   corrected_line += "HistoPathSuff: 1lep/ \n"
                            #elif(reg['name'].find("c0lep")>-1 and inputDir.find("/0lep")==-1):
                            #    corrected_line += "HistoPathSuff: 0lep/ \n"
                            corrected_line += "\n"
            if signalHandling=="ONE":
                corrected_line = corrected_line.replace("_SIGNAL_",SType)
            for arg in _ARGUMENTS_:
                corrected_line = corrected_line.replace(arg['arg'],arg['value'])
            f_adapted.write(corrected_line)

        #Adding the data sample if needed
        if(useData):
            f_adapted.write("\n")
            f_adapted.write("Sample: \"Data\"\n")
            f_adapted.write("Title: \"Data\"\n")
            f_adapted.write("Type: data \n")
            f_adapted.write("HistoFiles: Data.grp15,Data.grp16,Data.grp17,Data.grp18 \n")

        #Adding the SM 4tops as a background in case this is not the investigated signal ... Check if the option is set before
        #if(useFourTopsAsBack and SName.find("SM4tops")==-1):
        #    f_adapted.write("\n")
        #    f_adapted.write("Sample: \"OTHERS_BackSM4tops\"\n")
        #    f_adapted.write("Title: \"SM 4tops\"\n")
        #    f_adapted.write("Type: \"background\"\n")
        #    f_adapted.write("FillColor: 410\n")
        #    f_adapted.write("LineColor: 1\n")
        #    f_adapted.write("HistoFile: \"SM4tops\"\n")
        #    f_adapted.write("Group: \"Non-t#bar{t}\"\n\n")

    #Copying the content of the template systematics config file
    f_adapted.write("\n")
    #Adding the signal at the very end of the config file
    if counter==0 or signalHandling=="ONE":

        for systfile in systConfig:
            f_templateSysts = open(systfile,'r')
            for systLine in f_templateSysts:
                corrected_systLine = systLine
                for arg in _ARGUMENTS_:
                    corrected_systLine = corrected_systLine.replace(arg['arg'],arg['value'])
                f_adapted.write(corrected_systLine)
            f_adapted.write("\n")

        f_adapted.write(" \n")
        f_adapted.write("% --------------------------- %\n")
        f_adapted.write("% --------- SIGNALS --------- %\n")
        f_adapted.write("% --------------------------- %\n")
        f_adapted.write(" \n")


    lumiscale = 1.
    for arg in _ARGUMENTS_:
        if arg['arg'] == "__LUMIVALUE__":
            lumiscale = float(arg['value'])
            break
    #####
    #Cross section scaling
    if signalScaling=="BENCHMARK":
        searchname = SName.replace(".","")
        d_SampleInfo = xsecdata.get(searchname)
        if not d_SampleInfo:
            printError("<!> Sample not found in json file for benchmark xsec scaling")
            sys.exit()


        crossSection = d_SampleInfo.get('xsec')
        kFactor = d_SampleInfo.get('kFactor')
        filterEff = d_SampleInfo.get('genFiltEff')
        crossSection = float(crossSection*filterEff*kFactor)

        lumiscale = (signalBenchmark / crossSection)*lumiscale;
        
    f_adapted.write("Sample: \""+SType+"\"\n")
    f_adapted.write("  Title: \""+TypeTemp+"\"\n")
    f_adapted.write("  Type: signal\n")
    f_adapted.write("  NormFactor: \"mu_signal\","+SignalNormFactor+"\n")
    f_adapted.write("  SeparateGammas: TRUE\n")
    f_adapted.write("  FillColor: 2\n")
    f_adapted.write("  LineColor: 2\n")
    #f_adapted.write("  HistoFile: "+cleaned_sampleType+"\n")
    #f_adapted.write("  LumiScale: "+str(lumiscale)+"\n")
    f_adapted.write("  HistoFiles: nominal/"+cleaned_sampleType+".mc16a,nominal/"+cleaned_sampleType+".mc16d,nominal/"+cleaned_sampleType+".mc16e\n")
    f_adapted.write("  LumiScales: 36207.66,44307.4,58450.1\n")
    f_adapted.write(" \n")

    f_adapted.close()
    f_template.close()
