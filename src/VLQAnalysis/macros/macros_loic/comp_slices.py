import ROOT as root
import time
import sys
from array import *
import math
import os
from ROOT import *

from optparse import OptionParser

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )
from regions_dictionary import *

##______________________________________________________________________________
## ROOT options
root.gROOT.SetBatch(1)
root.gStyle.SetOptTitle(0)
root.gStyle.SetPalette(1)
root.TH1.SetDefaultSumw2(1)
root.gStyle.SetOptStat(0)
##..............................................................................

##______________________________________________________________________________
## Options
parser = OptionParser()
parser.add_option("-i","--inputDir",dest="inputDir",help="repository where to find the outputs of the jobs",action="store",default="")
parser.add_option("-o","--outputDir",dest="outputDir",help="repository where to put the output files",action="store",default="SlicesReweighting/")
(options, args) = parser.parse_args()
outputDir=options.outputDir
inputDir=options.inputDir
##..............................................................................

Regions =  []
Regions += ["c1lep"]
Regions += ["c0lep"]

ttbarType =  []
#ttbarType += ["ttbarlight"]
ttbarType += ["ttbarbb"]
ttbarType += ["ttbarcc"]

samples =  []
#samples += [{'name':"nominal",'incl':"410000",'slice':"407012"}]
#samples += [{'name':"radHi",'incl':"410001",'slice':"407032"}]
#samples += [{'name':"radLo",'incl':"410002",'slice':"407036"}]
samples += [{'name':"PowHpp",'incl':"410004",'slice':"407040"}]

os.system("mkdir -p " + outputDir)

if not os.path.exists(outputDir + "/out_reweight.root"):
    f_out = TFile(outputDir + "/out_reweight.root","recreate")
    for reg in Regions:
        for ttbar in ttbarType:
            h_out = TH1F(reg+"_"+ttbar,"",len(samples),0,len(samples))
            h_out.SetDirectory(0)
            for i_sample,sample in enumerate(samples):
                #Produces the merged file
                com = "hadd " + outputDir + "/incl_" + ttbar + "_" + sample['name'] + ".root " + inputDir + "/out*" + ttbar + "*" + sample['incl'] + "*.root"
                os.system(com)
                com = "hadd " + outputDir + "/met_" + ttbar + "_" + sample['name'] + ".root " + inputDir + "/out*" + ttbar + "*" + sample['slice'] + "*.root"
                os.system(com)

                #Computes the correcting factor
                f_incl = TFile(outputDir + "/incl_" + ttbar + "_" + sample['name'] + ".root")
                f_met = TFile(outputDir + "/met_" + ttbar + "_" + sample['name'] + ".root")
                histo = ""
                if reg=="c1lep":
                    histo = "c1lep0Tex0Hex6jin4bin"
                elif reg=="c0lep":
                    histo = "c0lep0Tex0Hex7jin4bin"
                histo += "_truth_met"
                h_incl = f_incl.Get(histo).Clone()
                h_incl.SetDirectory(0)
                h_met = f_met.Get(histo).Clone()
                h_met.SetDirectory(0)
                int_incl = h_incl.Integral(h_incl.FindBin(200),h_incl.GetNbinsX())
                int_met = h_met.Integral(h_met.FindBin(200),h_met.GetNbinsX())
                RW = int_incl/int_met
                h_out.SetBinContent(i_sample+1,RW)
                h_out.GetXaxis().SetBinLabel(i_sample+1,sample['slice'])
            f_out.cd()
            h_out.Write()
    f_out.Close()
else:
    reg_checks = fit_regions_1l+validation_regions_1l+fit_regions_0l+validation_regions_0l
    for ttbar in ttbarType:
        for i_sample,sample in enumerate(samples):
            f_incl = TFile(outputDir + "/incl_" + ttbar + "_" + sample['name'] + ".root")
            f_met = TFile(outputDir + "/met_" + ttbar + "_" + sample['name'] + ".root")
            for reg in reg_checks:
                for var in ["_meff","_truth_met"]:
                    h_incl = f_incl.Get(reg['name']+var).Clone()
                    h_incl.SetDirectory(0)
                    h_met = f_met.Get(reg['name']+var).Clone()
                    h_met.SetDirectory(0)

                    #Get the correcting factor
                    f_correction = TFile(outputDir + "/out_reweight.root")
                    h_correction = f_correction.Get("c1lep"+"_"+ttbar)
                    if(reg['name'].find("c0lep")>-1):
                        h_correction = f_correction.Get("c0lep"+"_"+ttbar)
                    count_bin = 0
                    for bin in range(1,h_correction.GetNbinsX()+1):
                        if sample['slice']==h_correction.GetXaxis().GetBinLabel(bin):
                            count_bin = bin
                            break
                    h_met.Scale(h_correction.GetBinContent(count_bin))
                    c = TCanvas()
                    c.Divide(0,2)
                    c.cd(1)
                    h_met.SetLineColor(kBlack)
                    h_incl.SetLineColor(kRed)
                    h_met.Draw("hist")
                    h_incl.Draw("e0same")
                    c.SetLogy()
                    h_ratio = h_met.Clone()
                    for ibin in range(1,h_ratio.GetNbinsX()+1):
                        if(h_incl.GetBinContent(ibin)):
                            h_ratio.SetBinContent(ibin,h_met.GetBinContent(ibin)/h_incl.GetBinContent(ibin))
                            h_ratio.SetBinError(ibin,h_met.GetBinError(ibin)/h_incl.GetBinContent(ibin))
                    h_ratio.SetMaximum(1.5)
                    h_ratio.SetMinimum(0.5)
                    c.cd(2)
                    h_ratio.Draw()
                    c.Print(reg['name']+"_" + ttbar + "_" + sample['name'] + ".png")


#
# Regions = []
# for reg in fit_regions_1l+validation_regions_1l+fit_regions_0l+validation_regions_0l:
#     Regions += [reg['name']]
#
#
# f_slice = TFile("../macros_ttbar_systs/Filters/ttbarbbradHi.root")
# f_noslice = TFile("../macros_ttbar_systs/NoFilter/ttbarbbradHi.root")
# for reg in Regions:
#     h_slice = f_slice.Get(reg+"_meff")
#     h_slice.GetXaxis().SetRangeUser(0,3000)
#     h_no_slice = f_noslice.Get(reg+"_meff")
#     h_no_slice.GetXaxis().SetRangeUser(0,3000)
#     err_slice = Double()
#     err_noslice = Double()
#     integral_slice = h_slice.IntegralAndError(1,h_slice.GetNbinsX(),err_slice)
#     integral_noslice = h_no_slice.IntegralAndError(1,h_no_slice.GetNbinsX(),err_noslice)
#     c = TCanvas()
#     c.Divide(0,2)
#     c.cd(1)
#     h_slice.SetLineColor(kBlack)
#     h_no_slice.SetLineColor(kRed)
#     h_slice.Draw("hist")
#     h_no_slice.Draw("e0same")
#     #c.Print(reg+".png")
#     c.SetLogy()
#     h_ratio = h_slice.Clone()
#     for ibin in range(1,h_ratio.GetNbinsX()+1):
#         if(h_no_slice.GetBinContent(ibin)):
#             h_ratio.SetBinContent(ibin,h_slice.GetBinContent(ibin)/h_no_slice.GetBinContent(ibin))
#             h_ratio.SetBinError(ibin,h_slice.GetBinError(ibin)/h_no_slice.GetBinContent(ibin))
#     h_ratio.SetMaximum(1.5)
#     h_ratio.SetMinimum(0.5)
#     c.cd(2)
#     h_ratio.Draw()
#     c.Print(reg+"_log.png")
#
#     #if abs((integral_slice-integral_noslice)/err_noslice)>1:
#     #    print "Region: " + reg + "    " + `(integral_slice-integral_noslice)/err_noslice` + "    Slice: " + `integral_slice` + "+- "+`err_slice`+"    No-slice: " + `integral_noslice`+ "+- "+`err_noslice`
