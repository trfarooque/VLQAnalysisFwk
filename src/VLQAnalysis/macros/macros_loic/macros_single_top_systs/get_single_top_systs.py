#!/bin/python
import os
import time
import socket
import sys
from array import array
import datetime
from ROOT import *
import math

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )
from VLQ_Samples import *
from regions_dictionary import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/IFAETopFramework/" )
from BatchTools import *
from Samples import *


parser.add_option("-o","--otherVariables",dest="otherVariables",help="Variables other than meff",action="store_true",default=False)
parser.add_option("-p","--preselRegions",dest="preselRegions",help="Use preselection regions",action="store_true",default=False)
parser.add_option("-d","--dumpPlots",dest="dumpPlots",help="Dump comparison plots",action="store_true",default=False)

(options, args) = parser.parse_args()

otherVariables=options.otherVariables
preselRegions=options.preselRegions
dumpPlots=options.dumpPlots

##______________________________________________________________________________
##
def dump_comparison_plot( region, variable, h_nom, h_rel_variation, syst ):
    lumi = 36100
    c = TCanvas()

    pad1 = TPad("pad1","pad1", 0, 0.3, 1, 1.0)
    pad1.SetBottomMargin(0)
    pad1.Draw()
    pad1.cd()
    gStyle.SetOptStat(0)
    h_nom.SetLineColor(kBlack)
    h_nom.Scale(lumi)
    h_nom.Draw("hist")
    pad1.SetTickx()
    pad1.SetTicky()
    h_var = h_rel_variation.Clone()
    h_var.SetDirectory(0)
    h_var.Multiply(h_nom)
    h_var.SetLineColor(kRed)
    h_var.SetLineStyle(2)
    h_var.SetLineColor(kRed)
    h_var.Draw("e0same")
    h_nom.SetMaximum(max(h_nom.GetMaximum(),h_var.GetMaximum())*1.2)
    SetOwnership(c, False)
    SetOwnership(pad1, False)

    #ATLAS logo
    atlas = TLatex(0.4,0.8,"#font[72]{ATLAS} #font[42]{Simulation Internal}")
    atlas.SetNDC()
    atlas.Draw("same")

    #Lumi and CME
    lcme = TLatex(0.4,0.74,"#font[42]{#scale[0.9]{#sqrt{s} = 13 TeV, 36.1 fb^{-1}}}")
    lcme.SetNDC()
    lcme.Draw("same")

    #Region legend
    reg = TLatex(0.4,0.68,"#font[42]{#scale[0.8]{"+region['legend']+"}}")
    reg.SetNDC()
    reg.Draw("same")

    #Legend
    leg = TLegend(0.4,0.61,0.8,0.66)
    leg.AddEntry(h_nom,"Nominal", "l")
    leg.AddEntry(h_var,syst['legend'], "l")
    leg.SetTextFont(42)
    leg.SetLineColor(0)
    leg.SetNColumns(2)
    leg.SetFillStyle(0)
    leg.Draw()

    c.cd()
    pad2 = TPad("pad2", "pad2", 0, 0.05, 1, 0.3)
    pad2.SetTopMargin(0)
    pad2.SetBottomMargin(0.2)
    pad2.Draw()
    pad2.cd()
    SetOwnership(pad2, False)

    h_rel_variation.Draw("e0")
    h_rel_variation.SetMinimum(0)
    h_rel_variation.SetMaximum(2)
    h_rel_variation.GetYaxis().SetTitle("Relative uncertainty")
    h_rel_variation.GetYaxis().SetNdivisions(4)
    pad2.SetGridy()
    h_rel_variation.GetYaxis().SetTitleSize(h_rel_variation.GetYaxis().GetTitleSize()*3)
    h_rel_variation.GetYaxis().SetTitleOffset(h_rel_variation.GetYaxis().GetTitleOffset()/3)
    h_rel_variation.GetYaxis().SetLabelSize(h_rel_variation.GetYaxis().GetLabelSize()*3)
    h_rel_variation.GetXaxis().SetLabelSize(h_rel_variation.GetXaxis().GetLabelSize()*4)
    h_rel_variation.GetXaxis().SetTitleSize(h_rel_variation.GetXaxis().GetTitleSize()*4)
    pad2.SetTickx()
    pad2.SetTicky()

    outFolder = "Plots/"
    outFolder += region['name'] + "/"
    outFolder += variable['name'] + "/"
    os.system("mkdir -p " + outFolder)
    c.Print(outFolder + syst['name'] + ".png")
    pad1.SetLogy()
    h_nom.SetMaximum(h_nom.GetMaximum()*20)
    c.Print(outFolder + syst['name'] + "_log.png")
##..............................................................................
##

##______________________________________________________________________________
##
def get_histograms( region, variable, syst ):
    histo_name = region['name'].replace("HTX_","")+"_"+variable['name']
    f_nom = TFile(syst['nom'],"read")
    h_nom = f_nom.Get(histo_name).Clone()
    h_nom.SetDirectory(0)
    f_var = TFile(syst['sys'],"read")
    h_var = f_var.Get(histo_name).Clone()
    h_var.SetDirectory(0)
    if variable['name']=="meff":
        bins_string = region['binning'].split(",")
        bins_list = []
        for bin in bins_string:
            bins_list += [int(bin)]
        bins = array('d',bins_list)
        h_nom = h_nom.Rebin(len(bins_list)-1,h_nom.GetName(),bins)
        h_var = h_var.Rebin(len(bins_list)-1,h_var.GetName(),bins)
    h_nom.SetDirectory(0)
    h_var.SetDirectory(0)

    f_ref = TFile()
    if 'ref' in syst.keys():
        f_ref = TFile(syst['ref'],"read")
        h_ref = f_ref.Get(histo_name).Clone()
        h_ref.SetDirectory(0)
        if variable['name']=="meff":
            h_ref = h_ref.Rebin(len(bins_list)-1,h_ref.GetName(),bins)
        h_var.Divide(h_ref)
    else:
        h_var.Divide(h_nom)
    return h_nom,h_var
##..............................................................................
##

##______________________________________________________________________________
##
def get_all_plots( o_file, region, variable, syst ):
    hists = get_histograms( region, variable, syst )
    o_file.cd()
    hists[1].Write()
    if dumpPlots:
        dump_comparison_plot( region, variable, hists[0], hists[1], syst)
##..............................................................................
##

##______________________________________________________________________________
##
def make_plots( systematics, regions, variables ):
    o_file = TFile("single_top_syst.root","RECREATE")
    for syst in systematics:
        for reg in regions:
            for var in variables:
                get_all_plots( o_file, reg, var,syst )
    o_file.Close()
##..............................................................................
##

##______________________________________________________________________________
##
def get_uncertainty_table( systematics, regions, variable, correlation = False ):
    '''
    Getting the uncertainty for each of the provided regions for single top.
    Uncertainties are provided with a splitting of the different sources, and
    the total uncertainty assuming different correlation schemes.
    correlation = False assumes that the different uncertainty sources are
    uncorrelated across the different single top channels.
    '''
    os.system("mkdir -p Tables/")
    tex_name = "Summary"
    if not correlation:
        tex_name += "_uncorr"
    else:
        tex_name += "_corr"
    table_tex = open("Tables/" + tex_name + ".tex", 'w')
    table_tex.write("\\begin{landscape}\n")
    table_tex.write("\\begin{table}\n")
    table_tex.write("    \\small")
    preamble_table = "   \\begin{tabular}{ l | "
    title_line     = "       \\hline \n"
    title_line     += "       "
    for sys in systematics:
        preamble_table += "   c "
        title_line += " & " + sys['name']
    preamble_table += " | c }"
    title_line += " & Total \\\\\n        \\hline \n"
    table_tex.write(preamble_table + "\n")
    table_tex.write(title_line + "\n")
    for reg in regions:
        uncertainties = []
        if not "cut_variable" in reg.keys():
            line = "        " + reg['legend'].replace("#geq","$\geq$")
        else:
            line = "        " + reg['legend'].replace("#geq","$\geq$") + "(" + reg['cut_variable']
            if reg['cut_type']=="lower":
                line += "$>$"
            else:
                line += "$<$"
            line += reg['cut_value'] + ")"
        line += " & "

        #Computing the different uncertainty sources
        for sys in systematics:
            integral_nom = 0
            integral_var = 0
            if not "cut_variable" in reg.keys():
                hists = get_histograms( reg, variable, sys)
                hists[1].Multiply(hists[0])
                integral_nom = hists[0].Integral()
                integral_var = hists[1].Integral()
            else:
                hists = get_histograms( reg, {'name':reg['cut_variable']}, sys)
                hists[1].Multiply(hists[0])
                bin_high = 1
                bin_low = 1
                bin_cut = hists[0].FindBin(int(reg['cut_value']))
                if reg['cut_type']=="upper":
                    bin_high = bin_cut-1
                    bin_low = 1
                elif reg['cut_type']=="lower":
                    bin_low = bin_cut
                    bin_high = hists[0].GetNbinsX()
                integral_nom = hists[0].Integral(bin_low,bin_high)
                integral_var = hists[1].Integral(bin_low,bin_high)
            uncertainty = ( integral_var - integral_nom ) / integral_nom
            uncertainty *= 100
            uncertainties += [{'name':sys['name'],'uncertainty': uncertainty}]
            line += " %.2f" % (uncertainty)
            line += " & "
        #Now computing the total uncertainties
        radHi = 0
        radLo = 0
        total_unc = 0
        for sys in uncertainties:
            if(sys['name'].find("radHi")>-1):
                if correlation:
                    radHi += sys['uncertainty']
                else:
                    radHi += math.pow(sys['uncertainty'],2)
            elif(sys['name'].find("radLo")>-1):
                if correlation:
                    radLo += sys['uncertainty']
                else:
                    radLo += math.pow(sys['uncertainty'],2)
            else:
                total_unc += sys['uncertainty']*sys['uncertainty']
        if not correlation:
            radHi = math.sqrt(radHi)
            radLo = math.sqrt(radLo)
        total_unc += math.pow((radHi+radLo)/2,2)
        total_unc = math.sqrt(total_unc)
        line += " %.2f" % (total_unc)
        line += " \\\\ \n"
        table_tex.write(line)
    table_tex.write("   \n        \\hline \n   \\end{tabular} \n")
    table_tex.write("   \\caption{Various component of the systematic uncertainties on the single-top prediction expressed in \\%, in each of the considered regions.} \n")

    table_tex.write("\\end{table} \n")
    table_tex.write("\\end{landscape}\n")
    table_tex.close()
##..............................................................................
##



#
# List of systematics
#
systematics =  []
#Wt uncertainties
# systematics += [{'name':"WtchanradHi",'legend':"radHi - Wt",'nom':"Nominal_AF2.root",'sys':"WtchanradHi_AF2.root"}]
# systematics += [{'name':"WtchanradLo",'legend':"radLo - Wt",'nom':"Nominal_AF2.root",'sys':"WtchanradLo_AF2.root"}]
# systematics += [{'name':"WtchanPS",'legend':"Powheg+Hpp - Wt",'nom':"Nominal_AF2.root",'sys':"WtchanPS_AF2.root"}]
systematics += [{'name':"WtchanDS",'legend':"Diag. sub. - Wt",'nom':"Nominal_FullSim.root",'sys':"WtchanDS_FullSim.root"}]
# #t uncertainties
# systematics += [{'name':"tchanradHi",'legend':"radHi - t",'nom':"Nominal_AF2.root",'sys':"tchanradHi_AF2.root"}]
# systematics += [{'name':"tchanradLo",'legend':"radLo - t",'nom':"Nominal_AF2.root",'sys':"tchanradLo_AF2.root"}]
# systematics += [{'name':"tchanPS",'legend':"Powheg+Hpp - t",'nom':"Nominal_AF2.root",'sys':"tchanPS_AF2.root"}]
# systematics += [{'name':"tchanGen",'legend':"aMC@NLO vs Powheg - t",'nom':"Nominal_AF2.root",'sys':"tchanGen_AF2.root",'ref':"tchanPS_AF2.root"}]
# #s uncertainties
# systematics += [{'name':"schanradHi",'legend':"radHi - s",'nom':"Nominal_AF2.root",'sys':"schanradHi_AF2.root"}]
# systematics += [{'name':"schanradLo",'legend':"radLo - s",'nom':"Nominal_AF2.root",'sys':"schanradLo_AF2.root"}]

#
# Regions
#


regions =  []
if preselRegions:
    regions += [{'name':"c0lep6jin2bin",'legend':"0l, #geq 6j, #geq 2b",'binning':"0,500,700,800,900,1000,1100,1200,1300,1400,1500,1600,1900,2500,3500"}]
    regions += [{'name':"c0lep6jin3bin",'legend':"0l, #geq 6j, #geq 3b",'binning':"0,500,700,800,900,1000,1100,1200,1300,1400,1500,1600,1900,2500,3500"}]
    regions += [{'name':"c0lep6jin2binLowMtbmin",'legend':"0l, #geq 6j, #geq 2b, LM",'binning':"0,600,800,1000,1200,1400,1600,2000,2500,3500"}]
    regions += [{'name':"c0lep6jin2binHighMtbmin",'legend':"0l, #geq 6j, #geq 2b, HM",'binning':"0,600,800,1000,1200,1400,1600,2000,2500,3500"}]
    regions += [{'name':"c0lep6jin3binLowMtbmin",'legend':"0l, #geq 6j, #geq 3b, LM",'binning':"0,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1900,2500,3500"}]
    regions += [{'name':"c0lep6jin3binHighMtbmin",'legend':"0l, #geq 6j, #geq 3b, HM",'binning':"0,600,900,1200,1500,2000,2500,3500"}]
    regions += [{'name':"c1lep5jin3bin",'legend':"1l, #geq 5j, #geq 3b",'binning':"0,500,700,800,900,1000,1100,1200,1300,1400,1500,1600,1900,2500,3500"}]
    regions += [{'name':"c1lep5jin2bin",'legend':"1l, #geq 5j, #geq 2b",'binning':"0,500,700,800,900,1000,1100,1200,1300,1400,1500,1600,1900,2500,3500"}]
else:    
    regions += fit_regions_0l+fit_regions_1l


##regions += [{'name':"c1lep5jin2bin",'legend':"1l, #geq 5j, #geq 2b",'binning':"1000,1100,1200,1300,1400,1500,1600,1900,2500,3500"}]

# regions += [{'name':"c1lep5jin3bin",'legend':"1l, #geq 5j, #geq 3b",'binning':"1000,1100,1200,1300,1400,1500,1600,1900,2500,3500"}]
# #regions += [{'name':"c0lep6jin2bin",'legend':"0l, #geq 6j, #geq 2b",'binning':"1000,1100,1200,1300,1400,1500,1600,1900,2500,3500"}]
# regions += [{'name':"c0lep6jin3bin",'legend':"0l, #geq 6j, #geq 3b",'binning':"1000,1100,1200,1300,1400,1500,1600,1900,2500,3500"}]
# #regions += [{'name':"c0lep6jin2binLowMtbmin",'legend':"0l, #geq 6j, #geq 2b, LM",'binning':"1000,1100,1200,1300,1400,1500,1600,1900,2500,3500"}]
# regions += [{'name':"c0lep6jin2binHighMtbmin",'legend':"0l, #geq 6j, #geq 2b, HM",'binning':"1000,1100,1200,1300,1400,1500,1600,1900,2500,3500"}]
# regions += [{'name':"c0lep6jin3binLowMtbmin",'legend':"0l, #geq 6j, #geq 3b, LM",'binning':"1000,1100,1200,1300,1400,1500,1600,1900,2500,3500"}]
# regions += [{'name':"c0lep6jin3binHighMtbmin",'legend':"0l, #geq 6j, #geq 3b, HM",'binning':"1000,1100,1200,1300,1400,1500,1600,1900,2500,3500"}]


#
# Variables
#
variables = []
#variables += [{'name':"meff"}]
#variables += [{'name':"mtbmin"}]
#variables += [{'name':"jets_n"}]
#variables += [{'name':"bjets_n"}]

variables += [{'name':"meff"}]
if otherVariables:
    variables += [{'name':"RCjets_m"}]
    variables += [{'name':"RCjets_pt"}]
    variables += [{'name':"RCjet0_m"}]
    variables += [{'name':"RCjet0_pt"}]
    variables += [{'name':"jets_n"}]
    variables += [{'name':"bjets_n"}]
    variables += [{'name':"met"}]
    variables += [{'name':"jet0_pt"}]
    variables += [{'name':"RCMHiggs_jets_n"}]
    variables += [{'name':"RCMTop_jets_n"}]
    variables += [{'name':"mtbmin_zoom"}]
    variables += [{'name':"RCjets_m"}]
    variables += [{'name':"RCjets_pt"}]
    variables += [{'name':"lep0_pt_zoom"}]
    variables += [{'name':"lep0_eta"}]
    variables += [{'name':"mtw_zoom"}]


make_plots( systematics, regions, variables )

#get_uncertainty_table( systematics, regions, variable={'name':"meff"}, correlation = True )
#get_uncertainty_table( systematics, regions, variable={'name':"meff"}, correlation = False )
