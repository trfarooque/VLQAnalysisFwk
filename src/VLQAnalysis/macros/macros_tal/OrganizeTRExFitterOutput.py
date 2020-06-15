import sys
import re
import os
import glob

# input argument is TRExFitter output folder (containing config files and "Results" folder)
inputDir = sys.argv[-1]

# use names in config files to collect all output dirs in Results
outputNames = [n.replace(n[:n.index("/configFile_sVLQ_")+17],"").replace(".txt","") for n in glob.glob(inputDir+"/configFile_sVLQ_*")]
outputDirs = [d for d in glob.glob(inputDir+"/Results/*") if not "scripts" in d and any(n in d for n in outputNames)]

# loop through output dirs
for d in outputDirs:

    # organize Plots
    plotDir = d+'/Plots/'
    if os.path.isdir(plotDir) and os.listdir(plotDir):

        if any('VR' in p for p in os.listdir(plotDir)):
            print "Organizing %s Plots in %s..."%(len(os.listdir(plotDir)),d)
        else:
            continue

        os.system('mkdir -p '+plotDir+'/SR/')
        os.system('mkdir -p '+plotDir+'/VR/')

        os.system('mv '+plotDir+'/HTX_*_VR.png '+plotDir+'/VR/')
        os.system('mv '+plotDir+'/HTX_*.png '+plotDir+'/SR/')

    # organize systematic template control plots
    templateDir = d+'/Systematics/'
    if os.path.isdir(templateDir) and os.listdir(templateDir):
        print "Organizing %s Systematics folders in %s..."%(len(os.listdir(templateDir)),d)

        systs = os.listdir(templateDir)

        # loop through systematics
        for syst in systs:
            systDir = templateDir+syst

            # check to see if we want to divide between SR and VR
            hasVR = False
            if any('VR' in p for p in os.listdir(systDir)):
                hasVR = True

            # find samples having this systematic applied
            regionsandsamples = [n[:n.find(syst)-1].replace("png","") for n in os.listdir(systDir)]
            samples = []
            for r in regionsandsamples:
                if any(n in r for n in outputNames):
                    samples.extend([s for s in outputNames if s in r])
                else:
                    samples.append(r[r.rfind('_')+1:])
            samples = list(set(samples))

            for sample in samples:
                os.system('mkdir -p '+systDir+'/'+sample)
                if hasVR:
                    os.system('mkdir -p '+systDir+'/'+sample+'/SR')
                    os.system('mkdir -p '+systDir+'/'+sample+'/VR')

                    os.system('mv '+systDir+'/*_VR_'+sample+'*'+syst+'*.png '+systDir+'/'+sample+'/VR')
                    os.system('mv '+systDir+'/*'+sample+'*'+syst+'*.png '+systDir+'/'+sample+'/SR')
                else:
                    os.system('mv '+systDir+'/*'+sample+'*'+syst+'*.png '+systDir+'/'+sample)

