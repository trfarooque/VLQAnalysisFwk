from ROOT import *
import sys
import subprocess
import re
import os

samples = map(str, sys.argv[-3].strip('[]').split(','))
list_key = sys.argv[-2]

gStyle.SetOptStat(0)
gStyle.SetPalette(kBird)

noRegionsWithKeys = False#['1lep']
onlyVariablesWithKeys = ['_vs_']
noVariablesWithKeys = False
plotTruthVarsTogether = True
plotRCTypesTogether = True
 
plotdir = "IFP_PNG/Plots_"+list_key+"/"

os.system("mkdir -p "+plotdir)

def GetRegionLabel(region):
    legend = ""
    if region == "call":
        legend = "No selection"
    if '1lep' in region:
        legend+='1l, '
    elif '0lep' in region:
        legend+='0l, '
    if 'jex' in region:
        if not region[region.index('jex')-1]=='f':
            legend+=region[region.index('jex')-1]+'j'
    if 'jin' in region:
        if not region[region.index('jin')-1]=='f':
            legend+='#geq'+region[region.index('jin')-1]+'j'
    if 'jwin' in region:
        legend+=region[region.index('jwin')-3]+'-'+region[region.index('jwin')-1]+'j'
    if 'bex' in region:
        legend+=', '+region[region.index('bex')-1]+'b'
    elif 'bin' in region:
        legend+=', #geq'+region[region.index('bin')-1]+'b'
    elif 'bwin' in region:
        legend+=', '+region[region.index('bwin')-3]+'-'+region[region.index('bwin')-1]+'b'
    if 'fjex' in region:
        legend+=', '+region[region.index('fjex')-1]+'fj'
    elif 'fjin' in region:
        legend+=', #geq'+region[region.index('fjin')-1]+'fj'
    if 'THin' in region:
        legend+=', #geq'+region[region.index('THin')-1]+'h-t'
    elif 'Hin' in region:
        legend+=', #geq'+region[region.index('Hin')-1]+'h'
    elif 'Hex' in region:
        legend+=', '+region[region.index('Hex')-1]+'h'
    if 'Vex' in region:
        legend+=', '+region[region.index('Vex')-1]+'v'
    elif 'Vin' in region:
        legend+=', #geq'+region[region.index('Vin')-1]+'v'
    if 'Tin' in region and region[region.index('Tin')-1].isdigit():
        legend+=', #geq'+region[region.index('Tin')-1]+'t'
    elif 'Tex' in region and region[region.index('Tex')-1].isdigit():
        legend+=', '+region[region.index('Tex')-1]+'t'
    if 'VTin' in region:
        legend+=', #geq'+region[region.index('VTin')-1]+'v-t'
    if 'LTex' in region:
        legend+=', #geq'+region[region.index('LTex')-1]+'lt'
    elif 'LTin' in region:
        legend+=', #geq'+region[region.index('LTin')-1]+'lt'
    if 'LTTin' in region:
        legend+=', #geq'+region[region.index('LTTin')-1]+'lt-t'

    if 'fj' in legend:
        legend = "#splitline{%s}{%s}"%(legend[:legend.index("fj, ")+3],legend[legend.index("fj, ")+4:])

    return legend

def GetLegendLabel(truthType):
    if truthType == "truthTop":
        return "Top matched"
    if truthType == "truthW":
        return "W matched"
    if truthType == "truthZ":
        return "Z matched"
    if truthType == "truthHiggs":
        return "Higgs matched"
    if truthType == "truthOther":
        return "Unmatched"

def GetRegionsAndVariables(file,prunevariables=False,noRegionsWithKeys=False,noVariablesWithKeys=False,onlyVariablesWithKeys=False):
    lsrootpath = "/lsroot-00-01-03/bin/lsRoot"
    if prunevariables:
        proc = subprocess.Popen([".%s /dir %s"%(lsrootpath,file)], stdout=subprocess.PIPE, shell=True)
    else:
        proc = subprocess.Popen([".%s %s"%(lsrootpath,file)], stdout=subprocess.PIPE, shell=True)
    (out, err) = proc.communicate()
    contents = out.split()
    regions,variables = [],[]
    for content in contents:
        if prunevariables and inputDir in content:
            variables.append([])
        if (len(content) < 9) or (not content.startswith('c')): continue
        if (not content.startswith('call') and not content[1].isdigit()): continue
        ind = [u.start() for u in re.finditer('_', content)]
        splitind = [i for i in ind if not content[i-1].isdigit() and not content[i+1].isdigit()][0]
        region = content[:splitind]
        variable = content[splitind+1:]
        if not region in regions:
            regions.append(region)
        if prunevariables:
            if not variable in variables[-1]:
                variables[-1].append(variable)
        elif not variable in variables:
            variables.append(variable)
    if prunevariables:
        variables = set.intersection(*map(set,variables))

    if noVariablesWithKeys:
        variables = [variable for variable in variables if not any(key in variable for key in noVariablesWithKeys)]
    if onlyVariablesWithKeys:
        variables = [variable for variable in variables if all(key in variable for key in onlyVariablesWithKeys)]
    if noRegionsWithKeys:
        regions = [region for region in regions if not any(key in region for key in noRegionsWithKeys)]

    print "Doing regions:"
    for reg in regions:
        print reg
    print "\nDoing %s variables"%len(variables)

    # regionsvariables = []
    # for region in regions:
    #     for variable in variables:
    #         regionsvariables.append(region+"_"+variable)
    return regions,variables

"""

for sample in samples:

    filename = "Rootfiles_Plots_"+list_key+"/"+sample+".root"
    print "\nSample    -- %s"%sample
    print "File name -- %s\n"%filename

    f = TFile(filename,'read')

    regions,variables = GetRegionsAndVariables(filename,noRegionsWithKeys=noRegionsWithKeys,noVariablesWithKeys=noVariablesWithKeys,onlyVariablesWithKeys=onlyVariablesWithKeys)

    regionsvariables = []
    for region in regions:
        for variable in variables:
            regionsvariables.append(region+"_"+variable)

    for var in regionsvariables:
        print var
        h = f.Get(var)
        c = TCanvas(h.GetName(),"",0,0,800,600)
        # print c.GetName()
        h.Draw('colz')
        c.Print("IFP_PNG/Plots_"+list_key+"/canv_"+var+"_"+sample+".png")
        c.Print("IFP_PDF/Plots_"+list_key+"/canv_"+var+"_"+sample+".pdf")
        c.Clear()
    
"""

if plotTruthVarsTogether:

    gStyle.SetPadTopMargin(0.05)
    gStyle.SetPadRightMargin(0.21)
    gStyle.SetPadBottomMargin(0.12)
    gStyle.SetPadLeftMargin(0.08)
    gStyle.SetOptStat(0)
    gStyle.SetStatStyle(0)
    gStyle.SetTitleStyle(0)
    gStyle.SetStatStyle(0)
    gStyle.SetTitleStyle(0)
    gStyle.SetCanvasBorderSize(0)
    gStyle.SetFrameBorderSize(0)
    gStyle.SetLegendBorderSize(0)
    gStyle.SetTitleBorderSize(0)

    print "Plotting truth variables together"

    truthTypes = ['truthOther','truthW','truthZ','truthHiggs','truthTop']
    truthcolors = [kOrange-2,kTeal+9,kOrange+7,kAzure+7,kMagenta-3]

    hdict = {}
    for truthType in truthTypes:
        hdict[truthType] = {}

    for sample in samples:

        os.system("mkdir -p "+plotdir+sample)

        filename = "Rootfiles_Plots_"+list_key+"/"+sample+".root"
        print "\nSample    -- %s"%sample
        print "File name -- %s\n"%filename

        f = TFile(filename,'read')

        regions,variables = GetRegionsAndVariables(filename,noRegionsWithKeys=noRegionsWithKeys,noVariablesWithKeys=noVariablesWithKeys,onlyVariablesWithKeys=onlyVariablesWithKeys)

        variables_clean = []

        for region in regions:

            os.system("mkdir -p "+plotdir+sample+"/"+region)

            for truthType in truthTypes:

                if truthType=='truthZ' and not 'Zt' in sample:
                    continue
                if truthType=='truthHiggs' and not 'Ht' in sample:
                    continue

                hdict[truthType][region] = {}
                for variable in variables:
                    if truthType in variable:
                        hdict[truthType][region][variable] = f.Get(region+'_'+variable)
                        variables_clean.append(variable.replace(truthType+'_','TYPE_'))

        variables_clean = list(set(variables_clean))

        for region in regions:
            for variable in variables_clean:
                hlist = []

                c = TCanvas(region+'_'+variable.replace('TYPE_',''),"",0,0,1400,900)
                leg = TLegend(0.805,0.5,1.,0.92)
                leg.SetHeader(GetRegionLabel(region))
                leg.SetTextSize(0.037)
                leg.AddEntry(0,"","")

                for n,truthType in enumerate(truthTypes):

                    if truthType=='truthZ' and not 'Zt' in sample:
                        continue
                    if truthType=='truthHiggs' and not 'Ht' in sample:
                        continue

                    hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].SetLineColor(truthcolors[n])
                    hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].SetLineWidth(3)
                    hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].SetFillColor(0)
                    hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].SetTitle("")

                    scale = hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].Integral()
                    try:
                        hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].Scale(1./scale)
                    except ZeroDivisionError:
                        pass

                    hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].GetXaxis().SetTitleOffset(1.25)

                    xtitle = hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].GetXaxis().GetTitle()
                    ytitle = hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].GetYaxis().GetTitle()

                    xtitle = xtitle.replace("unmatched ","")
                    ytitle = ytitle.replace("unmatched ","")

                    hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].GetXaxis().SetTitle(xtitle)
                    hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].GetYaxis().SetTitle(ytitle)

                    hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].GetXaxis().SetTitleSize(0.04)
                    hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].GetYaxis().SetTitleSize(0.04)
                    hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].GetXaxis().SetLabelSize(0.035)
                    hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].GetYaxis().SetLabelSize(0.035)

                    hdict[truthType][region][variable.replace('TYPE_',truthType+'_')].Draw('box same')
                    leg.AddEntry(hdict[truthType][region][variable.replace('TYPE_',truthType+'_')],GetLegendLabel(truthType),'f')

                leg.Draw()

                printInfo = TLatex()
                printInfo.SetNDC()
                printInfo.SetTextFont(72)
                printInfo.SetTextSize(0.043)
                printInfo.DrawLatex(0.12, 0.88, "#scale[0.9]{ATLAS}")
                printInfo.SetTextFont(42)
                printInfo.DrawLatex(0.21, 0.88, "#scale[0.9]{Internal}")
                printInfo.DrawLatex(0.12, 0.82, "#sqrt{s} = 13 TeV, 139 fb^{-1}")
                printInfo.SetTextFont(41)
                # printInfo.DrawLatex(0.12, 0.77, "%s"%GetRegionLabel(region))

                c.Print("IFP_PNG/Plots_"+list_key+"/"+sample+"/"+region+"/canv_"+region+'_'+variable.replace('TYPE_','')+".png")
                # c.Print("IFP_PDF/Plots_"+list_key+"/canv_"+region+'_'+variable.replace('TYPE_','')+'_'+sample+".pdf")
                c.Clear()





