from ROOT import *

gStyle.SetPadTopMargin(0.07)
gStyle.SetPadRightMargin(0.01)
gStyle.SetPadBottomMargin(0.12)
gStyle.SetPadLeftMargin(0.095)
gStyle.SetOptStat(0)
gStyle.SetStatStyle(0)
gStyle.SetTitleStyle(0)
gStyle.SetCanvasBorderSize(0)
gStyle.SetFrameBorderSize(0)
gStyle.SetLegendBorderSize(0)
gStyle.SetTitleBorderSize(0)

drawAllPlots = False
findBestConfigOverall = False
findBestConfigPerSignal = True

# not currently implemented fully 
draw1s = False
draw2s = False

# base path
# currently configured to expect <base path>_<alg>_<coll>_<var>
resultpath = '/nfs/at3/scratch2/tvdaalen/TRExFitter_WorkArea/SingleVLQ_RCStudies/singleVLQ_RC_studies_configFiles_ref2_'

# configs to loop over
algs = ['pT_ref_N_stan','pT_stan_N_stan','P2']
colls = ['RCJ_r10pt05', 'VR_rho450', 'VR_rho500', 'VR_rho550', 'VR_rho600', 'VR_rho650']
vars=['meff','recoVLQ_m','recoVLQ_redm']

# sample names, colors, styles
sampledict = {"T(#rightarrowHt)qb": [['sVLQ_WTHt11K03','sVLQ_WTHt16K05','sVLQ_WTHt20K05'],kGreen+2],
            "T(#rightarrowHt)qt":[['sVLQ_ZTHt11K05','sVLQ_ZTHt16K05','sVLQ_ZTHt20K05'],kOrange+7],
            "T(#rightarrowZt)qt":[['sVLQ_ZTZt11K05','sVLQ_ZTZt16K05'],kPink-3],
            "T(#rightarrowZt)qb":[['sVLQ_WTZt11K03','sVLQ_WTZt16K05','sVLQ_WTZt20K05'],kBlue+1]}
linestyles = [1,2,3,6,9,7,10,5]

#
# FUNCTIONS
#

def getSpecificLimit(fn):
    f = TFile(fn,'read')
    h = f.Get("limit")
    return 0.1*h.GetBinContent(1)

def getLimits(fntemplate=False,samples=False,draw1s=False,draw2s=False):
    xs,ys = [],[]
    for n,sample in enumerate(samples):
        fn = fntemplate.replace('SIGNAL',sample)
        f = TFile(fn,'read')
        h = f.Get("limit")
        limit = 0.1*h.GetBinContent(1)
        limitplus1s = 0.1*h.GetBinContent(4)
        limitmin1s = 0.1*h.GetBinContent(5)
        limitplus2s = 0.1*h.GetBinContent(3)
        limitmin2s = 0.1*h.GetBinContent(6)
        if n==0:
            x = 1100.
        elif n==1:
            x = 1600.
        elif n==2:
            x = 2000.
        else:
            print "wrong"
            return
        xs.append(x)
        ys.append(limit)
        if draw1s:
            yplus1s.append(limitplus1s)
            ymin1s.append(limitmin1s)
        if draw2s:
            yplus2s.append(limitplus2s)
            ymin2s.append(limitmin2s)
    out = [xs,ys]
    if draw1s:
        out.append(yplus1s,ymin1s)
    if draw2s:
        out.append(yplus2s,ymin2s)
    return out

def getLimitTGraph(x,y):
    tlimit=TGraphAsymmErrors(len(x))
    for n in range(len(x)):
        tlimit.SetPoint(n,x[n],y[n])
    return tlimit

def setLimitStyle(TGlimit):
    TGlimit.GetXaxis().SetTitle("Mass [GeV]")
    TGlimit.GetYaxis().SetTitle("#sigma #times BR [pb]")
    TGlimit.GetYaxis().SetTitleOffset(1.05)
    TGlimit.GetXaxis().SetTitleOffset(1.2)
    TGlimit.GetXaxis().SetTitleSize(0.045)
    TGlimit.GetYaxis().SetTitleSize(0.045)
    TGlimit.GetXaxis().SetLabelSize(0.035)
    TGlimit.GetYaxis().SetLabelSize(0.035)

def add1sLimits(TGlimit,tlimitplus1s,tlimitmin1s):
    TGlimit.Add(tlimitplus1s)
    TGlimit.Add(tlimitmin1s)
    return TGlimit

def add2sLimits(TGlimit,tlimitplus2s,tlimitmin2s):
    TGlimit.Add(tlimitplus2s)
    TGlimit.Add(tlimitmin2s)
    return TGlimit

def printLimitCanvas(resultpath=False,sampledict=False,compare='colls',colls=False,vars=False,algs=False,coll='RCJ_r10pt05',alg='P2',var='meff',draw1s=False,draw2s=False):
    linestyles = [1,2,3,6,9,7,10,5]
    for sampletype in sampledict.keys():
        c1 = TCanvas("c1",'',10,10,1000,750)
        c1.SetGrid()
        if sampletype=="T(#rightarrowHt)qb":
            leg = TLegend(0.13,0.65,0.43,0.925)
        else:
            leg = TLegend(0.7,0.65,0.988,0.925)
        leg.SetTextSize(0.045) 
        TGlimit = TMultiGraph()
        if compare=='colls':
            for i,coll in enumerate(colls):
                fntemplate = resultpath+alg+'_'+coll+'_'+var+'/Results/BONLY__SIGNAL__/Limits/BONLY__SIGNAL__.root'
                inlist = getLimits(fntemplate=fntemplate,samples=sampledict[sampletype][0])
                tlimitlist = inlist[0:1]
                if draw1s and draw2s:
                    plusmin1s = inlist[2:3]
                    plusmin2s = inlist[4:5]
                elif draw1s:
                    plusmin1s = inlist[2:3]
                elif draw2s:
                    plusmin2s = inlist[2:3]

                tlimit = getLimitTGraph(*tlimitlist)
                tlimit.SetLineColor(sampledict[sampletype][1])
                tlimit.SetLineStyle(linestyles[i])
                tlimit.SetLineWidth(3)
                TGlimit.Add(tlimit)
                leg.AddEntry(tlimit,coll,"l")
            TGlimit.Draw('al3')
            TGlimit.SetTitle(sampletype+', alg. = '+alg+', discr. = '+var)
            setLimitStyle(TGlimit)
            leg.Draw()
            c1.Print("Plots_TRExLimits/%s_ref2_limits_comp-colls_%s_%s.png"%(sampledict[sampletype][0][0][5:9],alg,var))
            c1.Print("Plots_TRExLimits/%s_ref2_limits_comp-colls_%s_%s.pdf"%(sampledict[sampletype][0][0][5:9],alg,var))
            c1.Clear()
            del c1

        if compare=='algs':
            for i,alg in enumerate(algs):
                fntemplate = resultpath+alg+'_'+coll+'_'+var+'/Results/BONLY__SIGNAL__/Limits/BONLY__SIGNAL__.root'
                tlimitlist = getLimits(fntemplate=fntemplate,samples=sampledict[sampletype][0])
                tlimit = getLimitTGraph(*tlimitlist)
                tlimit.SetLineColor(sampledict[sampletype][1])
                tlimit.SetLineStyle(linestyles[i])
                tlimit.SetLineWidth(3)
                TGlimit.Add(tlimit)
                leg.AddEntry(tlimit,alg,"l")
            TGlimit.Draw('al3')
            TGlimit.SetTitle(sampletype+', coll. = '+coll+', discr. = '+var)
            setLimitStyle(TGlimit)
            leg.Draw()
            c1.Print("Plots_TRExLimits/%s_ref2_limits_comp-algs_%s_%s.png"%(sampledict[sampletype][0][0][5:9],coll,var))
            c1.Print("Plots_TRExLimits/%s_ref2_limits_comp-algs_%s_%s.pdf"%(sampledict[sampletype][0][0][5:9],coll,var))
            c1.Clear()
            del c1

        if compare=='vars':
            for i,var in enumerate(vars):
                fntemplate = resultpath+alg+'_'+coll+'_'+var+'/Results/BONLY__SIGNAL__/Limits/BONLY__SIGNAL__.root'
                tlimitlist = getLimits(fntemplate=fntemplate,samples=sampledict[sampletype][0])
                tlimit = getLimitTGraph(*tlimitlist)
                tlimit.SetLineColor(sampledict[sampletype][1])
                tlimit.SetLineStyle(linestyles[i])
                tlimit.SetLineWidth(3)
                TGlimit.Add(tlimit)
                leg.AddEntry(tlimit,var,"l")
            TGlimit.Draw('al3')
            TGlimit.SetTitle(sampletype+', coll. = '+coll+', alg. = '+alg)
            setLimitStyle(TGlimit)
            leg.Draw()
            c1.Print("Plots_TRExLimits/%s_ref2_limits_comp-vars_%s_%s.png"%(sampledict[sampletype][0][0][5:9],alg,coll))
            c1.Print("Plots_TRExLimits/%s_ref2_limits_comp-vars_%s_%s.pdf"%(sampledict[sampletype][0][0][5:9],alg,coll))
            c1.Clear()
            del c1

#
# RUN
#

#
if drawAllPlots:
    for alg in algs:
        for var in vars:
            printLimitCanvas(resultpath=resultpath,sampledict=sampledict,compare='colls',colls=colls,alg=alg,var=var)
    
    for alg in algs:
        for coll in colls:
            printLimitCanvas(resultpath=resultpath,sampledict=sampledict,compare='vars',vars=vars,coll=coll,alg=alg)
    
    for coll in colls:
        for var in vars:
            printLimitCanvas(resultpath=resultpath,sampledict=sampledict,compare='algs',algs=algs,coll=coll,var=var)

#
if findBestConfigPerSignal:

    print 'Best: \t\talg: \tvar: \t\tcoll:'
    for sampletype in sampledict.keys():
        for signal in sampledict[sampletype][0]:
    
            lim = 100000.
            bestalg,bestvar,bestcoll=0,0,0
    
            for alg in algs:
                for var in vars:
                    for coll in colls:
    
                        fntemplate = resultpath+alg+'_'+coll+'_'+var+'/Results/BONLY__SIGNAL__/Limits/BONLY__SIGNAL__.root'
                        fn = fntemplate.replace('SIGNAL',signal)
                        thislim = getSpecificLimit(fn)
                        if thislim < lim:
                            lim = thislim
                            bestalg=alg
                            bestvar=var
                            bestcoll=coll
    
            print signal,'\t',bestalg,'\t',bestvar,'\t',bestcoll

#
if findBestConfigOverall:

    configdict,configdict_no11 = {},{}
    for alg in algs:
        for var in vars:
            for coll in colls:
    
                configdict[alg+'_'+var+'_'+coll] = 0.
                configdict_no11[alg+'_'+var+'_'+coll] = 0.
    
                for sampletype in sampledict.keys():
                    for signal in sampledict[sampletype][0]:
                        fntemplate = resultpath+alg+'_'+coll+'_'+var+'/Results/BONLY__SIGNAL__/Limits/BONLY__SIGNAL__.root'
                        fn = fntemplate.replace('SIGNAL',signal)
                        thislim = getSpecificLimit(fn)
    
                        configdict[alg+'_'+var+'_'+coll] += thislim
                        if not '11' in signal:
                            configdict_no11[alg+'_'+var+'_'+coll] += thislim

    lim = 10000
    for key in configdict.keys():
        if configdict[key] < lim:
            lim = configdict[key]
            bestconfig = key
    
    lim = 10000
    for key in configdict_no11.keys():
        if configdict_no11[key] < lim:
            lim = configdict_no11[key]
            bestconfig_no11 = key
    
    print "Best config (all masses): ",bestconfig
    print "Best config (no 1.1 TeV): ",bestconfig_no11
    
    
    print "\nSorted configs and total limits:"
    for key in sorted(configdict, key=configdict.get, reverse=False):
        print key, configdict[key]
    
    print "\nSorted configs and total limits (no 1.1 TeV):"
    for key in sorted(configdict_no11, key=configdict_no11.get, reverse=False):
        print key, configdict_no11[key]
