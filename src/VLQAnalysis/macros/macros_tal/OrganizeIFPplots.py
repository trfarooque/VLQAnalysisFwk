import sys
import re
import os
import glob

# input argument is folder to be organized
inputDir = sys.argv[-1]

allplots = glob.glob(inputDir+"/canv_*")

sVLQrecoSeparate = False
truthSeparate = False
D2Separate = False
splitEmu = False
moveToSubFolder = False

regions = []
for plot in allplots:
    ind = [u.start() for u in re.finditer('_', plot)]
    startind = plot.index("canv_")+5
    if splitEmu:
        endind = [i for i in ind if i > startind and not plot[i-1].isdigit() and not plot[i+1].isdigit()][0]
        if plot[endind:].startswith('_el_') or plot[endind:].startswith('_mu_'):
            endind += 3
    else:
        endind = [i for i in ind if i > startind and not plot[i-1].isdigit() and not plot[i+1].isdigit()][0]
    region = plot[startind:endind]
    if region not in regions:
        regions.append(region)

for region in sorted(regions, reverse=True):
    print "Doing region",region
    os.system("mkdir "+inputDir+'/'+region)
    # print "mkdir -p "+inputDir+region

    os.system("mv "+inputDir+"/*"+region+"_* "+inputDir+'/'+region+'/')
    # print "mv "+inputDir+"/*"+region+"_* "+inputDir+region
    
    if D2Separate:
        print "Doing 2D variables separately for region ",region
        os.system("mkdir -p "+inputDir+"/D2_variables")
        os.system("mkdir -p "+inputDir+"/D2_variables/"+region)
        os.system("mv "+inputDir+region+"/*_vs_* "+inputDir+"D2_variables/"+region+'/')
        # os.system("mv "+inputDir+"/*"+region+"_* "+inputDir+regio+'/'n)
    if sVLQrecoSeparate:
        print "Doing sVLQ variables separately for region ",region
        os.system("mkdir -p "+inputDir+"/recoVLQ_variables")
        os.system("mkdir -p "+inputDir+"/recoVLQ_variables/"+region)
        os.system("mv "+inputDir+region+"/*recoVLQ* "+inputDir+"recoVLQ_variables/"+region+'/')
        # os.system("mv "+inputDir+"/*"+region+"*sVLQ* "+inputDir+"sVLQ_reco_variables/"+region+'/')
        # os.system("mv "+inputDir+"/*"+region+"_* "+inputDir+region+'/')
    if truthSeparate:
        print "Doing truth variables separately for region ",region
        os.system("mkdir -p "+inputDir+"/truth_variables")
        os.system("mkdir -p "+inputDir+"/truth_variables/"+region)
        os.system("mv "+inputDir+region+"/*truth* "+inputDir+"truth_variables/"+region+'/')
        # os.system("mv "+inputDir+"/*"+region+"_* "+inputDir+region+'/')

