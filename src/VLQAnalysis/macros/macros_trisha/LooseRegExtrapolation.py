from ROOT import TFile,TH1D,TDirectory
import numpy as np
import os
import sys
import importlib

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )

# default params
indir  = '/nfs/at3/scratch2/tfarooque/VLQAnalysisRun2/FitInputs_RWPARAM/FilesTRexF/JMSRSYST/JMSRECALC/'
var = 'meff'
regDict = "regions_dictionary_sVLQ"
outdir = 'Extrapolated/'

sVLQ_channels = ['WTHt', 'WTZt', 'ZTHt', 'ZTZt']
sVLQ_masses = ['1000', '1200', '1400', '1600', '1800', '2000']
sVLQ_couplings = ['K50']

samples = [ ]
for channel in sVLQ_channels:
    for mass in sVLQ_masses:
        for coupling in sVLQ_couplings:
            samples += [channel+mass+coupling]

#samples += ['ttbarlight', 'ttbarcc', 'ttbarbb','Singletop', 'Wjets', 'Zjets', 'topEW', 'Dibosons', 'ttH', 'tZ', 'VH', 'SM4tops']

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


# make backup of old files in separate folder
if outdir:
    outdir = indir+'/'+outdir 
    os.system('mkdir -p %s'%(outdir))

shapeRegs = []
normRegs = []

# retrieve fit regions
regModule = importlib.import_module(regDict)

fitRegions = []

regMap = {}
for fr in regModule.tight_regions_1l:
    frname = fr['name'].replace('HTX_','')
    fitRegions.append(frname)

    shapename=frname
    shapename=shapename.replace('1bex','1bin')
    shapename=shapename.replace('2bex','1bin')
    shapename=shapename.replace('3bex','1bin')
    shapename=shapename.replace('3bin','1bin')
    shapename=shapename.replace('4bin','1bin')

    shapename=shapename.replace('0fjex','')
    shapename=shapename.replace('1fjin','')

    shapename=shapename.replace('3_5jwin1bin0Tex0Lex','3_5jwin1bin0LTex')

    normnum=""
    normden=""

    jpart=""
    bpart=""
    fjpart=""

    if '3_5jwin' in frname:
        jpart='3_5jwin'
        normden="c1lep3_5jwin1bin"
    elif '6jin' in frname:
        jpart='6jin'
        normden="c1lep6jin1bin"

    if '0fjex' in frname:
        fjpart='0fjex'
    elif '1fjin' in frname:
        fjpart='1fjin'

    if '1bex' in frname:
        bpart='1bex'
    if '2bex' in frname:
        bpart='2bex'
    if '3bex' in frname:
        bpart='3bex'
    if '3bin' in frname:
        bpart='3bin'
    if '4bin' in frname:
        bpart='4bin'

    normnum="c1lep"+jpart+bpart+fjpart

    regMap[frname] = { 
        'shape' : shapename,
        'norm'  : normnum,
        'den'   : normden}
 
def LoadHist(f,hname):
    h = f.Get(hname)
    #h.SetDirectory(0)
    return h

# loop through files
for campaign in ['.mc16a','.mc16d','.mc16e']:

    for sample in samples:

        fname_short = sample+campaign+'.root'
        fname = indir+fname_short
        fout = outdir+fname_short

        # check if file exists
        if not os.path.isfile(fname):
            print "This file doesn't exist! Skipping... : "+fname
            continue

        # loop through histograms and replace
        inf = TFile(fname, 'read')
        outf = TFile(fout, 'recreate')

        for fr in fitRegions:

            inf.cd()

            # doing pmg weight histogram replacing
            # we are replacing h_sys_SR by h_nom_SR * h_sys_loose / h_nom_loose

            # set uncertainties to 0 if histogram has no events
            sr_name=fr+'_'+var
            shape_name=regMap[fr]['shape']+'_'+var
            norm_name=regMap[fr]['norm']+'_'+var
            den_name=regMap[fr]['den']+'_'+var

            print 'SR : ', sr_name
            print 'Shape : ', shape_name
            print 'Norm : ', norm_name
            print 'Denominator : ', den_name

            #try:
            #    h_nom_SR = LoadHist(inf,sr_name,fr)
            #except ReferenceError:
            #    print "Could not find histogram %s for %s. Skipping..."%(sr_name,fr)
            #    continue

            try:
                h_shape = LoadHist(inf,shape_name)
            except ReferenceError:
                print "Could not find histogram %s for %s. Skipping..."%(shape_name,fr)
                continue

            try:
                h_norm = LoadHist(inf,norm_name)
            except ReferenceError:
                print "Could not find histogram %s for %s. Skipping..."%(norm_name,fr)
                continue

            try:
                h_den = LoadHist(inf,den_name)
            except ReferenceError:
                print "Could not find histogram %s for %s. Skipping..."%(den_name,fr)
                continue

            #num_intgl = h_norm.Integral()
            den_intgl = h_den.Integral()

            #h_nom_SR.Reset()
            norm_factor = h_norm.Integral()/h_den.Integral()
            h_nom_SR = h_shape.Clone(sr_name)
            #h_nom_SR.SetName(sr_name)
            h_nom_SR.Scale(norm_factor)
            print '\n',sample, 'mc16'+campaign, fr
            print 'h_new:\t', h_nom_SR.Integral()
            outf.cd()
            h_nom_SR.Write()
                
        outf.Close()
        inf.Close()
