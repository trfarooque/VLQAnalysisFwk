#!/bin/python
import os
import sys
import importlib
import json

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )
from VLQ_Samples_mc import *
#from regions_dictionary import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/SignalTools/")
from VLQCouplingCalculator import VLQCouplingCalculator as couplingCalculator
from VLQCrossSectionCalculator import *

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
    print "    sampleInfo=<name of sample_info.dat file; only mc16a version is needed, and only if scaling signals to a new cross section>"
    print "    systConfig=<coma separated list of absolute paths to the systematics template config files>"
    print "    addition=<additional information to be stored in the names of the jobs, scripts, folders, ...>"
    print "    signal=<ALL/ONE> ALL: all signals are put in the config file (default), ONE: only one signal in each config file (default is ALL)"
    print "    normfactor=<Init,Low,High> settings for the signal normalisation factor (default is: 1,0,100)"
    print "    signalType=<PAIR/SINGLE> (default is PAIR)"
    print "    signalScaling=<DEFAULT/BENCHMARK/THEORY> (default is DEFAULT. BENCHMARK can be used to scale to a constant benchmark cross section. **ONLY FOR SVLQ***, THEORY will scale signals to cross sections calculated in the theory model)"
    print "    signalBenchmark=Benchmark cross section for signal normalisation"

    print "    sVLQMass=Single VLQ signal mass point (default is 0.=ALL)"
    print "    sVLQMode=Single VLQ signal production/decay mode: ALL (default), WTHT,WTZT,ZTHT,ZTZT,TSINGLET,TDOUBLET"

    print "    doZeroLep=TRUE/FALSE use the zero lepton regions"
    print "    doOneLep=TRUE/FALSE use the one lepton regions"
    print "    doOneLepEMu=TRUE/FALSE use the separate e/mu regions in one lepton"
    print "    doSR=TRUE/FALSE use the search regions"
    print "    doVR=TRUE/FALSE use the validation regions"
    print "    doPresel=TRUE/FALSE use the preselection regions"
    print "    useBlindingCuts=TRUE/FALSE use the meff blinding cuts"
    print "    discriminant=<name of variable to be used as discriminant> (default is meff)"
    print "    discriminant_title=<title of variable to be used as discriminant> (default is m_{eff} [GeV])"
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
sampleInfo = ""
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
    elif(argument=="SAMPLEINFO"):
        sampleInfo = splitted[1]
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

if( (signalScaling == "THEORY") or (signalScaling == "BENCHMARK") ) and (sampleInfo == ""):
    printError("<!> Please provide a sampleInfo file for cross section scaling!")
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
SignalSets = {}

if(signalType=="PAIR"):

    Signals = []
    VLQ_masses = ["600","700","750","800","850","900","950","1000","1050","1100","1150","1200","1300","1400"]
    VLQB_masses = ["600","700","750","800","850","900","950","1000","1050","1100","1150","1200","1300","1400","1500","1600"]
    if doAllBR:
        VLQ_masses = ["700","750","800","850","900","950","1000","1050","1100","1150","1200","1300","1400","1500"]
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
            Signals += [getSampleUncertainties("VLQ_TT_" + mass + "_HtHt","VLQ_TT_" + mass + "_BR_0.00_0.00_1.00", CommonObjectSystematics , [])]
            Signals += [getSampleUncertainties("VLQ_TT_" + mass + "_ZtZt","VLQ_TT_" + mass + "_ZtZt", CommonObjectSystematics , [])]
            
    for mass in VLQB_masses:
        Signals += [getSampleUncertainties("VLQ_BB_" + mass + "_BDoublet","VLQ_BB_" + mass + "_BDoublet", CommonObjectSystematics , [])]
        Signals += [getSampleUncertainties("VLQ_BB_" + mass + "_BSinglet","VLQ_BB_" + mass + "_BSinglet", CommonObjectSystematics , [])]
        Signals += [getSampleUncertainties("VLQ_BB_" + mass + "_HbHb","VLQ_BB_" + mass + "_HbHb", CommonObjectSystematics , [])]
        Signals += [getSampleUncertainties("VLQ_BB_" + mass + "_ZbZb","VLQ_BB_" + mass + "_ZbZb", CommonObjectSystematics , [])]

    if signalHandling=="ONE":
        for sample in sVLQ_samples.items():
            SType = sample['sampleType']
            SignalSets[SType] = [sample]
    else:
        SignalSets["pVLQ"] += Signals
                
    #SignalSets["pVLQ"] = Signals


elif(signalType=="SINGLE"):

    VLQ_masses = [1100, 1300, 1500, 1700, 1900, 2100, 2300]
    VLQ_massRW = ["low_mass"]#, "nom_mass"]
    VLQ_couplings =["K30","K50","K70"]

    if signalHandling=="ALL":
        SignalSets["sVLQ"] = []

    for vlq_coupling in VLQ_couplings:

        for massRW in VLQ_massRW:

            for vlqMass in VLQ_masses:

                sVLQ_samples =  GetSingleVLQSamples( RWName=massRW+"_"+vlq_coupling, mode="", mass=vlqMass )
                if signalHandling=="ONE":
                    for sample in sVLQ_samples:
                        SType = sample['sampleType']
                        if SType in SignalSets:
                            SignalSets[SType] += [sample]
                        else:
                            SignalSets[SType] = [sample]
                else:
                    SignalSets["sVLQ"] += sVLQ_samples


                strParam=vlq_coupling+str(vlqMass)+massRW
                SignalSets["sVLQ_TS"+strParam] = sVLQ_samples
                SignalSets["sVLQ_TD"+strParam] = []
                SignalSets["sVLQ_TD"+strParam] += GetSingleVLQSamples( RWName=massRW+"_"+vlq_coupling, mode="ZTHt", mass=vlqMass )
                SignalSets["sVLQ_TD"+strParam] += GetSingleVLQSamples( RWName=massRW+"_"+vlq_coupling, mode="ZTZt", mass=vlqMass )
                
            
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

##________________________________________________________
#open sampleInfo if needed
with open(os.getenv("VLQAnalysisFramework_DIR") + '/data/VLQAnalysis/'+sampleInfo,'r') as f:
    xsecdata = json.load(f)


def GetSVLQSampleXSec( vlqMultiplet, samples ):

    #VQAqMK
    #Parse the sampleType
    xSec = 0.
    for sampleType in samples.split(','):
        sigType = ""
        if 'WTHt' in sampleType:
            sigType = "WTHt"
        elif 'WTZt' in sampleType:
            sigType = "WTZt"
        elif 'ZTHt' in sampleType:
            sigType = "ZTHt"
        elif 'ZTZt' in sampleType:
            sigType = "ZTZt"
            
        params = sampleType.replace(sigType,'')
        kappaIndex = params.index('K')
        kappa = float( params[kappaIndex+1:] )/100.
        mass = float( params[:kappaIndex] )

        xiW=0
        xiZ=0
        if vlqMultiplet.upper()=="TSINGLET":
            xiW=0.5
            xiZ=0.25
        elif vlqMultiplet.upper()=="TDOUBLET":
            xiW=0.
            xiZ=0.5
        else: #dummy singlet values for now
            xiW=0.5
            xiZ=0.25

        xSec += GetSVLQXSec(sigType, mass, kappa, xiW, xiZ)

    return xSec


def GetSVLQXSec( sigTypes, M, kappa, xiW, xiZ ):

    xSec = 0.;

    for sigtype in sigTypes.split(','):
        vlq = couplingCalculator(M, 'T')
        vlq.setKappaxi(kappa, xiW, xiZ) # kappa, xiW, xiZ. xiH = 1 - xiW - xiZ
        
        cVals = vlq.getcVals()
        BRs = vlq.getBRs()
        Gamma = vlq.getGamma()

        # 0=W; 1=Z; 2=H
        prodIndex = -1
        decayIndex = -1
        
        if(sigtype=="WTHt" or sigtype=="WTZt"):
            prodIndex = 0 #W-mediated production
        elif(sigtype=="ZTHt" or sigtype=="ZTZt"):
            prodIndex = 1 #Z-mediated production

        if(sigtype=="WTZt" or sigtype=="ZTZt"):
            decayIndex = 1 #decay to Z
        elif(sigtype=="WTHt" or sigtype=="ZTHt"):
            decayIndex = 2 #decay to H

        xSec += XS_NWA(M, cVals[prodIndex])*BRs[decayIndex]/PNWA(proc=sigtype, mass=M, GM=Gamma/M)

    return xSec


for setName,Signals in SignalSets.items():

    printGoodNews("-> Generating config file for the SignalSet :: " + setName)
    print "Number of signals in set : ", len(Signals)

    #Get relevant information about the signals
    #e.g. name, title, histoFileNames, theoryXSec, inputXSec

    SignalInfos = {}

    #setInputXSec = 0.;
    setTheoryXSec = 0.;

    for sample in Signals:

        #Sample characteristics
        SName = sample['name']
        SType = sample['sampleType']
        TypeTemp = SType
        cleaned_sampleType = SType.replace("#","").replace(" ","").replace("{","").replace("}","").replace("+","").replace("(","").replace(")","")

        inputXSec = 0.
        theoryXSec = 0.

        ###############################################
        ####   Input cross section
        ###############################################
        searchname = SName.replace(".",".mc16a")
        d_SampleInfo = xsecdata.get(searchname)
        if not d_SampleInfo:
            printError("<!> Sample not found in json file for benchmark xsec scaling")
            sys.exit()
        inputXSec = d_SampleInfo.get('crossSection')


        ###############################################
        ####   Theory cross section
        ###############################################
        multiplet=""
        if 'sVLQ_TS' in setName:
            multiplet="TSINGLET"
        elif 'sVLQ_TD' in setName:
            multiplet="TDOUBLET"
        theoryXSec = GetSVLQSampleXSec(multiplet, SType )
        setTheoryXSec += theoryXSec    


        ###############################################
        ####   Add signal info
        ###############################################
        if SType in SignalInfos:
            SignalInfos[SType]['inputXSec'] += inputXSec
            continue
        else:
            SignalInfos[SType] = { 'name':SType, 'title':TypeTemp, 'fileName':cleaned_sampleType, 'inputXSec':inputXSec, 'theoryXSec': theoryXSec }


################################################################

    #Open the template file
    f_template = open(inputTemplateConfigFile,'r')
    
    #Open the new file adapted to the signal
    template_file_path_split = inputTemplateConfigFile.split("/")
    template_file_name = template_file_path_split[len(template_file_path_split)-1]
    f_adapted = open(outputFolder + "/" + template_file_name.replace("_SIGNAL_",setName).replace("TEMPLATE_","").replace(".txt",addition+".txt"),'w')

    for inputLine in f_template:
        corrected_line = inputLine
        if inputLine.find("_REGIONLIST_")>-1:
                corrected_line += "\n"
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
                            corrected_line += "Region: " + reg['name'] + "_VR" + channel + "\n"
                        else:
                            corrected_line += "Region: " + reg['name'] + channel + "\n"
                        corrected_line += "Type: " + reg['type'] + "\n"

                        reg_discriminant = discriminant
                        if (discriminant=="recoVLQ0_m"):
                            if('0Hex' in reg['name']):
                                reg_discriminant = "Zt_" + discriminant
                            else:
                                reg_discriminant = "Ht_" + discriminant

                        corrected_line += "HistoName: " + reg['name'].replace("HTX_","") + channel + "_" + reg_discriminant + "\n"
                        corrected_line += "VariableTitle: " + discriminant_title +"\n"
                        corrected_line += "Binning: " + reg[binning_key] + "\n"
                        corrected_line += "Label: " + "\"" + reg['legend'] + "\"\n"
                        corrected_line += "ShortLabel: " + "\"" + reg['legend'] + "\"\n"
                        #if(reg['name'].find("c1lep")>-1 and inputDir.find("/1lep")==-1):
                         #   corrected_line += "HistoPathSuff: 1lep/ \n"
                        #elif(reg['name'].find("c0lep")>-1 and inputDir.find("/0lep")==-1):
                        #    corrected_line += "HistoPathSuff: 0lep/ \n"
                        corrected_line += "\n"

        corrected_line = corrected_line.replace("_SIGNAL_",setName)
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

    for systfile in systConfig:
        f_templateSysts = open(systfile,'r')
        for systLine in f_templateSysts:
            corrected_systLine = systLine
            for arg in _ARGUMENTS_:
                corrected_systLine = corrected_systLine.replace(arg['arg'],arg['value'])
            f_adapted.write(corrected_systLine)
        f_adapted.write("\n")


    for name,sample in SignalInfos.items():

        printGoodNews("-> Adding signal " + name)
        f_adapted.write(" \n")
        f_adapted.write("% --------------------------- %\n")
        f_adapted.write("% --------- SIGNALS --------- %\n")
        f_adapted.write("% --------------------------- %\n")
        f_adapted.write(" \n")
    
        lumiscale_mc16a = 36207.66
        lumiscale_mc16d = 44307.4
        lumiscale_mc16e = 58450.1

        ###############################################
        ####   Cross section scaling
        ###############################################

        scaleFactor = 1.
        #if scaling by theory
        if signalScaling=="THEORY":
            scaleFactor = (sample['theoryXSec']/sample['inputXSec'])
            
        if signalScaling=="BENCHMARK":

            scaleFactor = signalBenchmark / sample['inputXSec']

            #if we are scaling a combination of signals, need to preserve the relative theoretical cross section between them
            if ('sVLQ_TS' in setName) or ('sVLQ_TD' in setName):
                scaleFactor *= (sample['theoryXSec']/setTheoryXSec)

        lumiscale_mc16a *= scaleFactor
        lumiscale_mc16d *= scaleFactor
        lumiscale_mc16e *= scaleFactor

#        SignalInfos[SType] = { 'name':SType, 'title':TypeTemp, 'fileName':cleaned_sampleType, 'inputXSec':inputXSec, 'theoryXSec': theory }

        f_adapted.write("Sample: \""+sample['name']+"\"\n")
        f_adapted.write("  Title: \""+sample['title']+"\"\n")
        f_adapted.write("  Type: signal\n")
        f_adapted.write("  NormFactor: \"mu_signal\","+SignalNormFactor+"\n")
        f_adapted.write("  SeparateGammas: TRUE\n")
        f_adapted.write("  FillColor: 2\n")
        f_adapted.write("  LineColor: 2\n")
        #f_adapted.write("  HistoFile: "+cleaned_sampleType+"\n")
        #f_adapted.write("  LumiScale: "+str(lumiscale)+"\n")
        f_adapted.write("  HistoFiles: "+sample['fileName']+".mc16a,"+sample['fileName']+".mc16d,"+sample['fileName']+".mc16e\n")
        f_adapted.write("  LumiScales: %.2f,%.2f,%.2f" % (lumiscale_mc16a, lumiscale_mc16d, lumiscale_mc16e) +"\n")
        f_adapted.write(" \n")
    
    f_adapted.close()
    f_template.close()
    
  
