'''
propagate_syst.py
==================

Two files are needed for this:

-> systFile: the file containing the relative systematic uncertainty to be
propagated (the name is built on the region+var names)
-> inputFile: the file containing the nominal background distribution (this
is typically the fit inputs for TREXFitter).

Looping over systFile, the code tries to find a histogram with the same name
in the inputFile, or histograms following a given pattern.

Example command:

python propagate_syst.py inputFile=Singletop.root systFile=single_top_syst.root SYSTSUFFIX=SingleTopDR

'''

from ROOT import *
import os
import sys

##------------------------------------------------------------------------------
## Small utility for histograms list
##------------------------------------------------------------------------------
def GetKeyNames( iF, dir = "" ):
    iF.cd(dir)
    return [key.GetName() for key in gDirectory.GetListOfKeys()]
##..............................................................................

##------------------------------------------------------------------------------
## Small utility for regions and variables
##------------------------------------------------------------------------------
def GetRegVarSyst( iF, dir = "" ):
    h = GetKeyNames(iF)
    regions = []
    systematics = []
    variables = []
    for temp_h in h:
        if temp_h.find("_meff")==-1: continue
        if temp_h.find("_vs_")>-1: continue
        if temp_h.find("_meff_")>-1:
            splitted = temp_h.split("_meff_")
            reg = splitted[0]
            syst = splitted[1]
            if not syst in systematics:
                systematics += [syst]
            if not reg in regions:
                regions += [reg]
        else:
            reg = temp_h.replace("_meff","")
            if not reg in regions:
                regions += [reg]
    for temp_h in h:
        original_h = temp_h
        if temp_h.find("_meff")==-1: continue
        if temp_h.find("_vs_")>-1: continue
        variable = ""
        if temp_h.find("_meff_")>-1:
            length = len(original_h)
            region_removed = original_h
            for region in regions:
                temp = original_h.replace(region+"_","")
                if(len(temp)<length):
                    region_removed = temp
                    length = len(temp)
            syst_removed = region_removed
            for systematic in systematics:
                temp = region_removed.replace("_"+systematic,"")
                if(len(temp)<length):
                    syst_removed = temp
                    length = len(temp)
            variable = syst_removed
        else:
            length = len(original_h)
            region_removed = original_h
            for region in regions:
                temp = original_h.replace(region+"_","")
                if(len(temp)<length):
                    region_removed = temp
                    length = len(temp)
            variable = region_removed
        if not variable in variables:
            variables += [variable]
    return [regions,variables,systematics]
##..............................................................................

##------------------------------------------------------------------------------
## Selects the arguments
##------------------------------------------------------------------------------
inputFile = ""
systFile = ""
systSuffix = "Syst"
for iArg in range(1,len(sys.argv)):
    splitted=sys.argv[iArg].split("=")
    argument = splitted[0].upper()
    if(argument=="INPUTFILE"): inputFile = splitted[1]
    elif(argument=="SYSTFILE"): systFile = splitted[1]
    elif(argument=="SYSTSUFFIX"): systSuffix = splitted[1]
    else:
        printWarning("/!\ Unrecognized argument ("+splitted[0]+") ! Please check !")
##..............................................................................

##------------------------------------------------------------------------------
## Now, doing the thing
##------------------------------------------------------------------------------
f_inputFile = TFile(inputFile,"UPDATE")
rvs_inputFile = GetRegVarSyst(f_inputFile)
r_inputFile = rvs_inputFile[0]
v_inputFile = rvs_inputFile[1]
s_inputFile = rvs_inputFile[2]
#
f_systFile = TFile(systFile,"READ")
rvs_systFile = GetRegVarSyst(f_systFile)
r_systFile = rvs_systFile[0]
v_systFile = rvs_systFile[1]
s_systFile = rvs_systFile[2]
#
for reg_input in r_inputFile:

    ##
    ##
    ## Chooses the region to use for the systematics histograms
    ##
    ##
    filtered_regions = []
    if(reg_input in r_systFile):
        '''
        the region actially exists in the file - wooo-hooo
        '''
        filtered_regions = [reg_input]
    else:
        '''
        the region doesn't exist ... probably because the systematics
        are derived at the preselection level. Let's try to guess which
        histogram to use.
        '''
        for reg_syst in r_systFile:
            #Mtbmin classification
            if reg_input.find("HighMtbmin")>-1 and reg_syst.find("HighMtbmin")==-1:
                continue
            if reg_input.find("LowMtbmin")>-1 and reg_syst.find("LowMtbmin")==-1:
                continue
            if reg_input.find("Mtbmin")==-1 and reg_syst.find("Mtbmin")>-1:
                continue
            #lepton classification
            if reg_input.find("1lep")>-1 and reg_syst.find("1lep")==-1:
                continue
            if reg_input.find("0lep")>-1 and reg_syst.find("0lep")==-1:
                continue
            filtered_regions += [reg_syst]
        if len(filtered_regions)>1:
            for reg in filtered_regions:
                if(reg_input.find("2b")>-1):
                    if(reg.find("2b")==-1):
                        filtered_regions.remove(reg)
                if(reg_input.find("3b")>-1 or reg_input.find("4b")>-1):
                    if(reg.find("3b")==-1):
                        filtered_regions.remove(reg)
    if not(len(filtered_regions)==1):
        print("<!> Error with region \"" + reg_input + "\": cannot find the corresponding region in systFile")
        continue

    ##
    ##
    ## Looking for the variable to use
    ##
    ##
    print reg_input + " will be scaled by " + filtered_regions[0]

    for var_input in v_inputFile:
        if not var_input in v_systFile:
            continue
        histName_ForNominal = reg_input + "_" + var_input
        histName_ForSyst = filtered_regions[0]+"_"+var_input
        hist_ForNominal = f_inputFile.Get(histName_ForNominal).Clone()
        hist_ForNominal.SetName( hist_ForNominal.GetName()+"_"+systSuffix)
        hist_ForSyst = f_systFile.Get(histName_ForSyst).Clone()
        for ibin in range(1,hist_ForNominal.GetNbinsX()+1):
            center = hist_ForNominal.GetBinCenter(ibin)
            content = hist_ForNominal.GetBinContent(ibin)
            content_sys = hist_ForSyst.GetBinContent(hist_ForSyst.FindBin(center))
            new_content = content * content_sys
            hist_ForNominal.SetBinContent(ibin,new_content)
        f_inputFile.cd()
        hist_ForNominal.Write()
f_inputFile.Close()
f_systFile.Close()
##..............................................................................
