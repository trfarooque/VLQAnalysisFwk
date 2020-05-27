#!/bin/python
import os
import sys
import importlib
import json
import numpy as np
from ROOT import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )
from VLQ_Samples_mc import *

regDict = "regions_dictionary_sVLQ"
regDictDir = '.'
doOneLep = True

# Running options
writeToNewDictionary = True
overWriteDictionary = True
correctEmptyBins = True
doSR = True
doVR = False
doPresel = False
debugPrint = False

if overWriteDictionary:
    regDict_blinded = regDict
else:
    regDict_blinded = regDict+"_blinded"

# Blinding options
SBcut = 0.1
doMinMeffCut = True

# Setting up regions
Regions = []
regModule = importlib.import_module(regDict)
if doOneLep:
    if doSR:
        Regions += regModule.fit_regions_1l
    if doVR:
        Regions += regModule.validation_regions_1l
    if doPresel:
        Regions += regModule.preselection_regions_1l

# Setting up input files
# inputDir = '/nfs/at3/scratch2/tvdaalen/SingleVLQWorkArea/br_R21_master_Mar2020/VLQAnalysisFramework/fitinputs_blinding/'
inputDir = '/nfs/at3/scratch2/tvdaalen/SingleVLQWorkArea/RootFiles/RootFiles_VR_mergeStudies/'
f_bkg = {'a':TFile(inputDir+'bkg.mc16a.root','read'),'d':TFile(inputDir+'bkg.mc16a.root','read'),'e':TFile(inputDir+'bkg.mc16a.root','read')}

signals = ['sVLQ_WTHt16K05','sVLQ_WTHt20K05','sVLQ_WTZt16K05','sVLQ_WTZt20K05','sVLQ_ZTHt11K05','sVLQ_ZTHt16K05','sVLQ_ZTHt20K05','sVLQ_ZTZt11K05','sVLQ_ZTZt16K05','sVLQ_WTHt11K03','sVLQ_WTHt16K03','sVLQ_WTZt11K03','sVLQ_ZTHt16K03']
# signals = ['sVLQ_WTHt16K05','sVLQ_WTHt20K05','sVLQ_WTZt16K05','sVLQ_WTZt20K05','sVLQ_ZTHt11K05','sVLQ_ZTHt16K05','sVLQ_ZTHt20K05','sVLQ_ZTZt11K05','sVLQ_WTHt11K03','sVLQ_WTHt16K03','sVLQ_WTZt11K03','sVLQ_ZTHt16K03']

# clean signals for 2 TeV samples
signals  = [s for s in signals if not "20" in s]

# setting up scaling
scalings = {}
scalings['sVLQ_WTHt11K03'] = 503094.198965
scalings['sVLQ_WTHt16K03'] = 2119853.91888
scalings['sVLQ_WTHt16K05'] = 787693.467052
scalings['sVLQ_WTHt20K05'] = 2081511.94825
scalings['sVLQ_WTZt11K03'] = 653197.481558
scalings['sVLQ_WTZt16K05'] = 1031918.20071
scalings['sVLQ_WTZt20K05'] = 3128544.87353
scalings['sVLQ_ZTHt11K05'] = 2321255.96928
scalings['sVLQ_ZTHt16K03'] = 24497351.4952
scalings['sVLQ_ZTHt16K05'] = 9138238.34878
scalings['sVLQ_ZTHt20K05'] = 23634821.4893
scalings['sVLQ_ZTZt11K05'] = 2559420.07106
scalings['sVLQ_ZTZt16K05'] = 10340673.9066

scalings['bkg.mc16a'] = 36207.66
scalings['bkg.mc16d'] = 44307.4
scalings['bkg.mc16e'] = 58450.1

sigyields = {}

# Write to new regions dictionary
if writeToNewDictionary:
    os.system("cp %s/%s.py %s/%s.py"%(regDictDir,regDict,regDictDir,regDict_blinded))

for reg in Regions:
    # Get binning from dictionary
    hname = reg['name'][4:]+'_meff'
    newbins_list = [int(b) for b in reg['binning'].split(',')]
    newbins = np.array(newbins_list,'double')

    # Set up flags
    goodbins = []
    maxMeffReached = False
    totalbkg = 0.0
    maxtotalsignal = 0.0
    correctThisRegion = False
    correctedbins = [b for b in newbins_list]

    # Set up signal yield to calculate total S/B
    for s in signals:
        sigyields[s] = 0.0

    for ibin in range(len(newbins_list[:-1])):
        goodbin = True
        emptybin = False
        bkgYield = 0.0
        maxsignalyield = 0.0
        SB = 0.0

        for campaign in ['a','d','e']:

            # print hname

            this_h = f_bkg[campaign].Get(hname).Rebin(len(newbins_list)-1,hname+"_rebin",newbins).Clone()
            this_h.SetDirectory(0)
            binyield = scalings['bkg.mc16%s'%campaign] * this_h.GetBinContent(ibin+1)
            bkgYield += binyield
            totalbkg += binyield

        # Loop through signals
        for s in signals:
            f_sig = TFile(inputDir+s+'.root','read')
            s_h = f_sig.Get(hname).Rebin(len(newbins_list)-1,hname+"_rebin",newbins).Clone()

            sigYield = scalings[s] * s_h.GetBinContent(ibin+1)
            sigyields[s] += sigYield

            f_sig.Close()

            # Store max S/B
            try:
                if sigYield / bkgYield > SB:
                    SB = sigYield / bkgYield

            # Check if empty bin and correct
            except ZeroDivisionError:
                goodbin = False
                correctThisRegion = True
                if newbins_list[ibin] in correctedbins:
                    correctedbins.remove(newbins_list[ibin])

        if debugPrint:
            print newbins_list[ibin:ibin+2]
            print SB, bkgYield

        if SB >= SBcut:
            goodbin = False
            if doMinMeffCut:
                maxMeffReached = True

        if not maxMeffReached:
            if goodbin and ibin<len(newbins_list)-1:
                if not newbins_list[ibin] in goodbins:
                    goodbins.append(newbins_list[ibin])
                goodbins.append(newbins_list[ibin+1])

    maxsignal = max(sigyields, key=sigyields.get)

    print "\n"+reg['name']
    if goodbins==[]:
        print "Has to be fully blinded!"
    else:
        print "Normal bins:  ",newbins_list
        print "Blinded bins: ",goodbins
    if correctThisRegion:
        print "Normal bins corrected to: ",correctedbins
    print "Max overall S/B: %.4f"%(sigyields[maxsignal]/totalbkg), "("+maxsignal+")"

    if writeToNewDictionary:

        if len(goodbins) < 2:
            goodbins = ""

        # new line to write containing blinded bins
        newline = "\t'binning_blind':"+'"'+str(goodbins).replace('[','').replace(']','').replace(' ','')+'",\n'

        # find line number where to insert new line
        with open('%s/%s.py'%(regDictDir,regDict_blinded),'r') as f:
            contents = f.readlines()
            index = [x for x in range(len(contents)) if reg['name'] in contents[x]][0]

        # insert new line into dictionary
        contents.insert(index+3,newline)
        contents = "".join(contents)

        with open('%s/%s.py'%(regDictDir,regDict_blinded),'w') as f:
            f.write(contents)

        if correctEmptyBins and correctThisRegion:

            newline = "\t'binning':"+'"'+str(correctedbins).replace('[','').replace(']','')+'",\n'

            with open('%s/%s.py'%(regDictDir,regDict_blinded),'r') as f:
                contents = f.readlines()
                index = [x for x in range(len(contents)) if reg['name'] in contents[x]][0]
    
            contents[index+2] = newline
            contents = "".join(contents)
    
            with open('%s/%s.py'%(regDictDir,regDict_blinded),'w') as f:
                f.write(contents)
