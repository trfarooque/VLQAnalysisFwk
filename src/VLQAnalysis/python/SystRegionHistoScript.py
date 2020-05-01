from ROOT import TFile
import os
import sys
import importlib

# default params
indir  = '/nfs/pic.es/user/t/tvdaalen/scratch2/SingleVLQWorkArea/RootFiles/RootFiles_Singletop_LooseSystRegs/'
var = 'meff'
regDict = "regions_dictionary_sVLQ"
backupOldFiles = True

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
        elif(argument=="DOBACKUP"):
            backupOldFiles = (value.upper()=="TRUE")

# make backup of old files in separate folder
if backupOldFiles:
    backupDir = indir+'SingletopBackups'
    os.system('mkdir -p %s'%(backupDir))

# retrieve fit regions
regModule = importlib.import_module(regDict)
fitRegions = []
for fr in regModule.fit_regions_1l:
    fitRegions.append(fr['name'].replace('HTX_',''))

# mapping fit regions to their loose syst regions
regMap = {}
for fr in fitRegions:
    if '3_5jwin' in fr:
        if '1bex' in fr or '2bex' in fr:
            regMap[fr] = 'c1lep3_5jwin1_2bwin1VLTHin'
        elif '3bex' in fr or '4bin' in fr:
            regMap[fr] = 'c1lep3_5jwin3bin1VLTHin'
    if '6jin' in fr:
        if '1bex' in fr or '2bex' in fr:
            regMap[fr] = 'c1lep6jin1_2bwin1VLTHin'
        elif '3bex' in fr or '4bin' in fr:
            regMap[fr] = 'c1lep6jin3bin1VLTHin'

# pmg weight names
histoSuffixes = ['_weight_pmg_Var3cUp',
    '_weight_pmg_Var3cDown',
    # '_weight_pmg_muR20__muF10',
    # '_weight_pmg_muR05__muF10',
    # '_weight_pmg_muR10__muF20',
    # '_weight_pmg_muR10__muF05',
    '_weight_pmg_isr_muRfac10__fsr_muRfac20',
    '_weight_pmg_isr_muRfac10__fsr_muRfac05']

# loop through files
for campaign in ['a','d','e']:

    fname_short = 'Singletop.mc16'+campaign+'.root'
    fname = indir+fname_short

    # check if file exists
    if not os.path.isfile(fname):
        print "This file doesn't exist! Skipping... : "+fname
        continue

    # make backup
    if backupOldFiles:
        if not os.path.isfile('%s/%s'%(backupDir,fname_short)):
            os.system('cp %s %s/%s'%(fname,backupDir,fname_short))

    outf = TFile(fname, 'update')
    outf.cd()

    # loop through histograms and replace
    for fr in fitRegions:
        for suf in histoSuffixes:
            try:
                hnew = outf.Get(regMap[fr]+'_'+var+suf)
                hnew.Write(fr+'_'+var+suf)
            except ReferenceError:
                print "Could not find histogram %s Skipping..."%(regMap[fr]+'_'+var+suf)

