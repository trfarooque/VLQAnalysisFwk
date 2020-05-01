from ROOT import TFile,TH1D,TDirectory
import numpy as np
import os
import sys
import importlib

# default params
indir  = '/nfs/pic.es/user/t/tvdaalen/scratch2/SingleVLQWorkArea/RootFiles/RootFiles_Singletop_UncTest_FitInputs/EXTRAPOLATED_2_STonly/'
indir_alt = indir+'SingletopAlt/'

var = 'meff'
regDict = "regions_dictionary_sVLQ"

backupOldFiles = True
startFromBackup = False

doPMGweights = True
doModSysts = True

samples = ['Singletop'] #SingletopWtprod','Singletopschan','Singletoptchan']

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
        elif(argument=="INPUTDIRALT"):
            indir_alt = value
        elif(argument=="REGIONS"):
            regDict = value
        elif(argument=="DOBACKUP"):
            backupOldFiles = (value.upper()=="TRUE")


# make backup of old files in separate folder
if backupOldFiles:
    backupDir = indir+'Backups'
    os.system('mkdir -p %s'%(backupDir))

looseRegs = ['c1lep3_5jwin1_2bwin1VLTHin','c1lep3_5jwin3bin1VLTHin','c1lep6jin1_2bwin1VLTHin','c1lep6jin3bin1VLTHin']

# retrieve fit regions
regModule = importlib.import_module(regDict)
fitRegions = []
binningDict = {}
for fr in regModule.fit_regions_1l:
    frname = fr['name'].replace('HTX_','')
    if frname in looseRegs:
        continue
    fitRegions.append(frname)
    binningDict[frname] = np.array([int(b) for b in fr['binning'].split(',')],'double')

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
    '_weight_pmg_muR050__muF100',
    '_weight_pmg_muR100__muF050',
    '_weight_pmg_muR100__muF200',
    '_weight_pmg_muR200__muF100',
    '_weight_pmg_isr_muRfac10__fsr_muRfac20',
    '_weight_pmg_isr_muRfac10__fsr_muRfac05']

# modelling systematics
modSysts = ['DiagSub','PowHer','aMCPy']

def LoadBinnedHist(f,hname,region):
    newbins = binningDict[region]
    return f.Get(hname).Rebin(newbins.size-1,hname,newbins).Clone()

# loop through files
for campaign in ['a','d','e']:

    for sample in samples:

        fname_short = sample+'.mc16'+campaign+'.root'
        fname = indir+fname_short

        # check if file exists
        if not os.path.isfile(fname):
            print "This file doesn't exist! Skipping... : "+fname
            continue

        # start from backup
        if startFromBackup:
            if os.path.isfile('%s/%s'%(backupDir,fname_short)):
                os.system('cp %s/%s %s'%(backupDir,fname_short,fname))

        # make backup
        if backupOldFiles:
            if not os.path.isfile('%s/%s'%(backupDir,fname_short)):
                os.system('cp %s %s/%s'%(fname,backupDir,fname_short))

        if doPMGweights:

            # loop through histograms and replace
            for fr in fitRegions:

                outf = TFile(fname, 'update')
                outf.cd()

                # doing pmg weight histogram replacing
                # we are replacing h_sys_SR by h_nom_SR * h_sys_loose / h_nom_loose

                # set uncertainties to 0 if histogram has no events
                emptyhist = False
                try:
                    h_nom_SR = LoadBinnedHist(outf,fr+'_'+var,fr)
                    if h_nom_SR.Integral() == 0.:
                        print "\nRegion %s is empty in sample %s. Setting uncertainties to 0.0!"%(fr,fname_short)
                        emptyhist = True

                except ReferenceError:
                    print "Could not find histogram(s) for %s. Skipping..."%(fr)
                    continue
                                
                for suf in histoSuffixes:
                    try:
                        h_nom_SR = LoadBinnedHist(outf,fr+'_'+var,fr)

                        # write the nominal histogram instead if it's empty
                        if emptyhist:
                            h_nom_SR.Write(fr+'_'+var+suf)
                            continue

                        else:
                            # load loose hist with same binning as region to extrapolate to
                            print fname
                            print fr
                            print regMap[fr]+'_'+var
                            print regMap[fr]+'_'+var+suf
                            h_nom_loose = LoadBinnedHist(outf,regMap[fr]+'_'+var,fr)
                            h_sys_loose = LoadBinnedHist(outf,regMap[fr]+'_'+var+suf,fr)

                            h_sys_loose.Divide(h_nom_loose)
                            h_nom_SR.Multiply(h_sys_loose)

                            print '\n',sample, 'mc16'+campaign, suf, fr
                            print 'h_pmg_new:\t', h_nom_SR.Integral()
                            h_nom_SR.Write(fr+'_'+var+suf)

                    except ReferenceError:
                        print "Could not find histogram(s) for %s. Skipping..."%(fr)
                        continue

                outf.Close()

        if doModSysts:

            fname_short_AFII = sample+campaign+'.root'
            fname_AFII = indir_alt+fname_short

            # doing modelling systematic histogram replacing
            # we are replacing h_alt_SR by h_nom_SR * h_alt_loose / h_nom_loose
            for mod in modSysts:

                # only do DiagSub for Wt channel
                if mod == 'DiagSub':
                    # if 'Wt' not in sample:
                    #     continue
                    # else:
                    # use fullsim sample as nominal for DS syst
                    fname_short_AFII = sample+'.mc16'+campaign+'.root'
                    fname_AFII = indir+fname_short_AFII

                fname_alt_short = sample+mod+'.mc16'+campaign+'.root'
                fname_alt = indir_alt+fname_alt_short

                # check if file exists
                if not os.path.isfile(fname_alt):
                    print "This file doesn't exist! Skipping... : "+fname_alt
                    continue

                # start from backup
                if startFromBackup:
                    if os.path.isfile('%s/%s'%(backupDir,fname_alt_short)):
                        os.system('cp %s/%s %s'%(backupDir,fname_alt_short,fname_alt))

                # make backup
                if backupOldFiles:
                    if not os.path.isfile('%s/%s'%(backupDir,fname_alt_short)):
                        os.system('cp %s %s/%s'%(fname_alt,backupDir,fname_alt_short))

                # loop through histograms and replace
                for fr in fitRegions:

                    nameSR = fr+'_'+var
                    nameLoose = regMap[fr]+'_'+var

                    outf = TFile(fname_AFII, 'read')

                    # set uncertainties to 0 if histogram has no events
                    emptyhist = False
                    try:
                        h_nom_SR = LoadBinnedHist(outf,nameSR,fr)
                        if h_nom_SR.Integral() == 0.:
                            print "\nRegion %s is empty in sample %s. Setting uncertainties to 0.0!"%(fr,fname_short_AFII)
                            emptyhist = True

                    except ReferenceError:
                        print "Could not find histogram(s) for %s. Skipping..."%(fr)
                        continue
                                    
                    try:
                        outf.cd()

                        h_nom_SR = LoadBinnedHist(outf,nameSR,fr)
                        h_nom_SR_OriginalIntegral = h_nom_SR.Integral()
                        h_nom_loose = LoadBinnedHist(outf,nameLoose,fr)

                        outf_alt = TFile(fname_alt, 'update')
                        outf_alt.cd()

                        if emptyhist:
                            h_nom_SR.Write(nameSR)
                            continue

                        h_alt_loose = LoadBinnedHist(outf_alt,nameLoose,fr)
                        h_alt_SR = LoadBinnedHist(outf_alt,nameSR,fr)

                        h_alt_loose.Divide(h_nom_loose)
                        h_nom_SR.Multiply(h_alt_loose)

                        if h_nom_SR.Integral() == 0. or h_nom_SR.Integral() < 0.:
                            print '\nNew histogram integral <= 0 - Using nominal histogram as alt. instead...'
                            outf.cd()
                            h_nom_SR = LoadBinnedHist(outf,nameSR,fr)

                            outf_alt.cd()

                        print '\n',sample, 'mc16'+campaign, mod, fr
                        print 'h_alt_SR:\t', h_alt_SR.Integral()
                        print 'h_alt_new:\t', h_nom_SR.Integral()
                        print 'h_nom_SR:\t', h_nom_SR_OriginalIntegral

                        h_nom_SR.Write(nameSR)

                        outf.Close()
                        outf_alt.Close()

                    except ReferenceError:
                        print "Could not find histogram(s) for %s. Skipping..."%(fr)
                        continue

                outf_alt.Close()
