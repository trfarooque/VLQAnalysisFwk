from ROOT import TFile,TH1D,TDirectory
import numpy as np
import os
import sys
import importlib

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )

# default params
indir      = '/nfs/at3/scratch2/tfarooque/VLQAnalysisRun2/JMSRSYSTFiles/FilesTRexF/JMR/Extrapolation/'
indir_ref  = '/nfs/at3/scratch2/tfarooque/VLQAnalysisRun2/JMSRSYSTFiles/FilesTRexF/JMSRECALC/Extrapolation/'
indir_nom  = '/nfs/at3/scratch2/tfarooque/VLQAnalysisRun2/FitInputs_RWPARAM/FilesTRexF/'
var = 'meff'
regDict = "regions_dictionary_sVLQ"
outdir = '/nfs/at3/scratch2/tfarooque/VLQAnalysisRun2/FitInputs_RWPARAM/FilesTRexF/JMSRSYST/JMR/Extrapolated/'

samples = ['sVLQ_WTHt16K05', 'sVLQ_WTZt16K05', 'sVLQ_ZTHt16K05', 'sVLQ_ZTZt16K05']
#, 'ttbarlight', 'ttbarcc', 'ttbarbb',  'Singletop', 'Wjets', 'Zjets', 'topEW', 'Dibosons', 'ttH', 'tZ', 'VH', 'SM4tops']

# user input
if(len(sys.argv))>1:
    for x in sys.argv[1:]:
        splitted=x.split("=")
        if(len(splitted)!=2):
            print "The argument \"" + x + "\" has no equal signs ... Please check"
            sys.exit(-1)
        argument = splitted[0].upper().replace("--","")
        value = splitted[1]
        if(argument=="VAR"):
            var = value
        elif(argument=="INPUTDIR"):
            indir = value
        elif(argument=="REGIONS"):
            regDict = value
        elif(argument=="OUTPUTDIR"):
            outdir = value


if outdir:
    os.system('mkdir -p %s'%(outdir))

# retrieve fit regions
regModule = importlib.import_module(regDict)

fitRegions = []

regMap = {}
for fr in regModule.all_regions_1l:
    frname = fr['name'].replace('HTX_','')
    fitRegions.append(frname)
 
def LoadHist(f,hname):
    h = f.Get(hname)
    #h.SetDirectory(0)
    return h

# loop through files
for campaign in ['']:
#for campaign in ['.mc16a','.mc16d','.mc16e']:

    for sample in samples:

        fname_short = sample+campaign+'.root'
        fname = indir+fname_short
        fname_ref = indir_ref+fname_short
        fname_nom = indir_nom+fname_short
        fout = outdir+fname_short

        # check if file exists
        if not os.path.isfile(fname):
            print "This file doesn't exist! Skipping... : "+fname
            continue

        # check if file exists
        if not os.path.isfile(fname_ref):
            print "This file doesn't exist! Skipping... : "+fname_ref
            continue

        # check if file exists
        if not os.path.isfile(fname_nom):
            print "This file doesn't exist! Skipping... : "+fname_nom
            continue

        # loop through histograms and replace
        inf = TFile(fname, 'read')
        inf_ref = TFile(fname_ref, 'read')
        inf_nom = TFile(fname_nom, 'read')

        outf = TFile(fout, 'recreate')

        for fr in fitRegions:

            #inf.cd()

            # doing systematics histogram replacing
            # we are replacing h_sys by h_sys * h_nom / h_ref

            # set uncertainties to 0 if histogram has no events
            sr_name=fr+'_'+var

            try:
                print 'Loading sys '+sr_name
                h_sys = LoadHist(inf,sr_name)
                print 'Loaded sys '+sr_name

            except ReferenceError:
                print "Could not find histogram %s for %s. Skipping..."%(sr_name,fr)
                continue

            try:
                print 'Loading ref '+sr_name
                h_ref = LoadHist(inf_ref,sr_name)
                print 'Loaded ref '+sr_name
            except ReferenceError:
                print "Could not find histogram %s for %s. Skipping..."%(sr_name,fr)
                continue

            try:
                print 'Loading nom '+sr_name
                h_nom = LoadHist(inf_nom,sr_name)
                print 'Loaded nom '+sr_name
            except ReferenceError:
                print "Could not find histogram %s for %s. Skipping..."%(sr_name,fr)
                continue

            h_sys.Divide(h_ref)
            h_sys.Multiply(h_nom)

            print '\n',sample, 'mc16'+campaign, fr

            outf.cd()
            h_sys.Write()
                
        outf.Close()
        inf.Close()
        inf_ref.Close()
        inf_nom.Close()
