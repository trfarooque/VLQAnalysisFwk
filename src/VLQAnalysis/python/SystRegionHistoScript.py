from ROOT import TFile,TH1D,TDirectory
import os
import sys
import importlib

# default params
indir  = '/nfs/pic.es/user/t/tvdaalen/scratch2/SingleVLQWorkArea/RootFiles/RootFiles_Singletop_UncTest_FitInputs/'
indir_alt = indir+'SingletopAlt/'
var = 'meff'
regDict = "regions_dictionary_sVLQ"

backupOldFiles = True
startFromBackup = True

doPMGweights = False
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
        elif(argument=="REGIONS"):
            regDict = value
        elif(argument=="DOBACKUP"):
            backupOldFiles = (value.upper()=="TRUE")

# make backup of old files in separate folder
if backupOldFiles:
    backupDir = indir+'Backups'
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
    '_weight_pmg_muR050__muF100',
    '_weight_pmg_muR100__muF050',
    '_weight_pmg_muR100__muF200',
    '_weight_pmg_muR200__muF100',
    '_weight_pmg_isr_muRfac10__fsr_muRfac20',
    '_weight_pmg_isr_muRfac10__fsr_muRfac05']

# modelling systematics
modSysts = ['DiagSub','PowHer','aMCPy']

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

            outf = TFile(fname, 'update')
            outf.cd()

            # loop through histograms and replace
            for fr in fitRegions:
                # doing pmg weight histogram replacing
                # we are replacing h_sys_SR by h_nom_SR * h_sys_loose / h_nom_loose

                # set uncertainties to 0 if histogram has no events
                emptyhist = False
                try:
                    h_nom_SR = outf.Get(fr+'_'+var)
                    if h_nom_SR.Integral() == 0.:
                        print "\nRegion %s is empty in sample %s. Setting uncertainties to 0.0!"%(fr,fname_short)
                        emptyhist = True

                except ReferenceError:
                    print "Could not find histogram(s) for %s. Skipping..."%(fr)
                    continue
                                
                for suf in histoSuffixes:
                    try:
                        h_nom_SR = outf.Get(fr+'_'+var)

                        if emptyhist:
                            h_nom_SR.Write(fr+'_'+var+suf)
                            continue

                        else:
                            h_nom_loose = outf.Get(regMap[fr]+'_'+var)
                            h_sys_loose = outf.Get(regMap[fr]+'_'+var+suf)
                            h_sys_loose.Divide(h_nom_loose)
                            h_nom_SR.Multiply(h_sys_loose)
                            h_nom_SR.Write(fr+'_'+var+suf)

                    except ReferenceError:
                        print "Could not find histogram(s) for %s. Skipping..."%(fr)
                        continue

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

                fname_alt_short = sample+'_'+mod+'.mc16'+campaign+'.root'
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
                        h_nom_SR = outf.Get(nameSR)
                        if h_nom_SR.Integral() == 0.:
                            print "\nRegion %s is empty in sample %s. Setting uncertainties to 0.0!"%(fr,fname_short_AFII)
                            emptyhist = True

                    except ReferenceError:
                        print "Could not find histogram(s) for %s. Skipping..."%(fr)
                        continue
                                    
                    try:
                        outf.cd()

                        h_nom_SR = outf.Get(nameSR).Clone()
                        h_nom_SR_OriginalIntegral = h_nom_SR.Integral()
                        h_nom_loose = outf.Get(nameLoose)

                        outf_alt = TFile(fname_alt, 'update')
                        outf_alt.cd()

                        if emptyhist:
                            h_nom_SR.Write(nameSR)
                            continue

                        h_alt_loose = outf_alt.Get(nameLoose)
                        h_alt_SR = outf_alt.Get(nameSR)

                        h_alt_loose.Divide(h_nom_loose)
                        h_nom_SR.Multiply(h_alt_loose)

                        if h_nom_SR.Integral() == 0. or h_nom_SR.Integral() < 0.:
                            print '\nNew histogram integral <= 0 - Using nominal histogram as alt. instead...'
                            outf.cd()
                            h_nom_SR = outf.Get(nameSR).Clone()
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

