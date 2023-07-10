import os
import string
import random
import itertools
import re
import time, getpass
import socket
import sys
import datetime
import numpy as np
from array import array
from optparse import OptionParser
from ROOT import *

from Plot_Utils import *
import samples_dictionary_template

gROOT.SetBatch(1)

##_______________________________________________________________________________________
##
def AddHistograms(path_to_files, file_name, campaign_to_scales_dict, histogram_name, histogram_name_pattern="", global_scale=1.0):
    histogram = nullptr

    histogram_name_new = histogram_name.replace("*", histogram_name_pattern)

    for campaign in campaign_to_scales_dict:
        campaign_file = TFile(path_to_files+"/"+campaign+"/"+file_name, "READ")

        if histogram == nullptr:
            histogram = campaign_file.Get(histogram_name_new)
            histogram.SetDirectory(nullptr)
            histogram.Scale(global_scale*campaign_to_scales_dict[campaign])
        else:
            temp_histogram = campaign_file.Get(histogram_name_new)
            temp_histogram.Scale(global_scale*campaign_to_scales_dict[campaign])
            histogram.Add(temp_histogram)

        campaign_file.Close()

    return histogram

##_______________________________________________________________________________________
##
def CalculateEfficiencies(sample_dictionary_list, histogram_name, mass, decay, global_signal_scale = 1.0, signal_pattern = "", do_normed = True, bins = [], 
                          background_rejection_quantiles=[0.76, 0.89]):
    for sample_dictionary in sample_dictionary_list:
        signal_file_name = sample_dictionary['signal'].replace("MASS",mass).replace("DECAY",decay)

        sample_dictionary['efficiency_sgnl'] = []
        sample_dictionary['efficiency_bkgd'] = []

        histogram_sgnl = AddHistograms(sample_dictionary['path'], signal_file_name, sample_dictionary['signal_scales'], 
                                       histogram_name, signal_pattern, global_signal_scale)

        histogram_bkgd = AddHistograms(sample_dictionary['path'], sample_dictionary['background'], sample_dictionary['background_scales'],
                                       histogram_name, "", 1.0)

        first_bin = histogram_sgnl.FindBin(0)
        last_bin  = histogram_sgnl.FindBin(1)

        if len(bins):
            histogram_sgnl = histogram_sgnl.Rebin(len(bins)-1, histogram_sgnl.GetName(), bins)
            histogram_bkgd = histogram_bkgd.Rebin(len(bins)-1, histogram_bkgd.GetName(), bins)
            first_bin = 1
            last_bin = histogram_sgnl.GetNbinsX()

        n_events_sgnl = histogram_sgnl.Integral(first_bin, last_bin) if do_normed else 1.0
        n_events_bkgd = histogram_bkgd.Integral(first_bin, last_bin) if do_normed else 1.0

        background_rejection_quantiles = array('d', background_rejection_quantiles)
        sample_dictionary['MVA_cuts'] = array('d',[0,0])
        histogram_bkgd.GetQuantiles(2, sample_dictionary['MVA_cuts'], background_rejection_quantiles)

        sample_dictionary['rejection_bkgd'] = array('d',[histogram_bkgd.Integral(1, histogram_bkgd.FindBin(sample_dictionary['MVA_cuts'][0]))/n_events_bkgd,
                                                         histogram_bkgd.Integral(1, histogram_bkgd.FindBin(sample_dictionary['MVA_cuts'][1]))/n_events_bkgd])

        sample_dictionary['acceptance_sgnl'] = array('d',[histogram_sgnl.Integral(histogram_sgnl.FindBin(sample_dictionary['MVA_cuts'][0]), last_bin)/n_events_sgnl,
                                                          histogram_sgnl.Integral(histogram_sgnl.FindBin(sample_dictionary['MVA_cuts'][1]), last_bin)/n_events_sgnl])

        for nbin in range(first_bin, last_bin+1):
            sample_dictionary['efficiency_sgnl'].insert(0, histogram_sgnl.Integral(nbin, last_bin)/n_events_sgnl)
            sample_dictionary['efficiency_bkgd'].insert(0, histogram_bkgd.Integral(nbin, last_bin)/n_events_bkgd)

##_______________________________________________________________________________________
##
def ROCCurvePlotsHist(sample_dictionary_list, histogram_name, mass, decay, global_signal_scale, signal_pattern, doLogY, outDir, extraLabel, mode="1MinusEffB"):

    temp_name = ("ROC_"+mode+"_"+mass+"_"+decay).replace(" ","_")
    x_label = "#epsilon_{signal}"
    y_label = "1-#epsilon_{background}" if mode == "1MinusEffB" else "1/#epsilon_{background}"
    
    Canvas_Objects = SetupCanvas("canv_"+temp_name, True)
    canvas, pad1, pad2 = [Canvas_Objects[i] for i in (0,1,2)]

    extra_text = "#splitline{"+extraLabel+"}"+"{"+decay+" "+mass+" GeV}"
    ttlbox = ExtraLabel(0.18, 0.76, 0.38, 0.87, text_size = 0.045, extra_text=extra_text)
    if mode == "1MinusEffB":
        legend = SetupLegend(0.18,0.20,0.7,0.70, text_size = 0.045)
    else:
        legend = SetupLegend(0.43,0.4,0.9,0.9, text_size = 0.035)

    ROC_histograms = {}
    ROC_histograms_ratio = {}

    ROC_WP_multigraph = {}

    interpolation_points = np.arange(0.0125, 1, 0.025)
    bins = np.arange(0, 1.05, 0.025)
    #interpolation_points = np.arange(0.025, 1, 0.05)
    #bins = np.arange(0, 1.05, 0.05)
    nbins = len(bins)-1

    CalculateEfficiencies(sample_dictionary_list, histogram_name, mass, decay, global_signal_scale, signal_pattern, do_normed = True)

    max_val = 0

    for sample_dictionary in sample_dictionary_list:

        key = sample_dictionary["label"]

        ROC_WP_multigraph[key] = TMultiGraph()
        LMVA_WP = SetupTGraph(1, 23, 2, sample_dictionary['color'])
        HMVA_WP = SetupTGraph(1, 22, 2, sample_dictionary['color'])

        LMVA_cut = sample_dictionary['MVA_cuts'][0]
        LMVA_eff = sample_dictionary['acceptance_sgnl'][0] 
        LMVA_rej = sample_dictionary['rejection_bkgd'][0] if mode == "1MinusEffB" else 1.0/(1.0-sample_dictionary['rejection_bkgd'][0])

        HMVA_cut = sample_dictionary['MVA_cuts'][1]
        HMVA_eff = sample_dictionary['acceptance_sgnl'][1]
        HMVA_rej = sample_dictionary['rejection_bkgd'][1] if mode == "1MinusEffB" else 1.0/(1.0-sample_dictionary['rejection_bkgd'][1])

        LMVA_WP.SetPoint(0, LMVA_eff, LMVA_rej)
        HMVA_WP.SetPoint(0, HMVA_eff, HMVA_rej)
        
        LMVA_legend_text = ("(MVA Score = {0:.2f}, XLABEL =  {1:.2f}, YLABEL = {2:.2f})").format(LMVA_cut, LMVA_eff, LMVA_rej)
        LMVA_legend_text = LMVA_legend_text.replace("XLABEL", x_label).replace("YLABEL", y_label)

        HMVA_legend_text = ("(MVA Score = {0:.2f}, XLABEL =  {1:.2f}, YLABEL = {2:.2f})").format(HMVA_cut, HMVA_eff, HMVA_rej)
        HMVA_legend_text = HMVA_legend_text.replace("XLABEL", x_label).replace("YLABEL", y_label)

        legend.AddEntry(LMVA_WP, LMVA_legend_text)
        legend.AddEntry(HMVA_WP, HMVA_legend_text)

        ROC_WP_multigraph[key].Add(LMVA_WP)
        ROC_WP_multigraph[key].Add(HMVA_WP)

        histogram_ROC_name = ("h_"+temp_name+"_"+sample_dictionary["label"]).replace(".","").replace(",","").replace(" ","")

        ROC_histograms[key] = TH1D(histogram_ROC_name, ";#epsilon_{signal};"+y_label, nbins, bins)  

        bkgd_rejection_tmp  = []

        # linear interpolation of signal efficiency (x) and background rejection (y)
        # y_new = y_a + (y_b-y_a)*(x_new-x_a)/(x_b-x_a) where b > a
        for interpolation_index, x_new in enumerate(interpolation_points):
            b = next(i for i,v in enumerate(sample_dictionary['efficiency_sgnl']) if v > x_new)
            
            x_b = sample_dictionary['efficiency_sgnl'][b]
            y_b = 1.0 - sample_dictionary['efficiency_bkgd'][b] if mode == "1MinusEffB" else 1.0/(sample_dictionary['efficiency_bkgd'][b]+1.0e-3)

            x_a = sample_dictionary['efficiency_sgnl'][b-1]
            y_a = 1.0 - sample_dictionary['efficiency_bkgd'][b-1] if mode == "1MinusEffB" else 1.0/(sample_dictionary['efficiency_bkgd'][b-1]+1.0e-3)
            
            if interpolation_index:
                x_a = max(x_a, interpolation_points[interpolation_index-1])
                
                if interpolation_points[interpolation_index-1] >= x_a:
                    y_a = bkgd_rejection_tmp[interpolation_index-1]

            y_new = y_a + (y_b-y_a)*((x_new-x_a)/(x_b-x_a))
            bkgd_rejection_tmp.append(y_new)
        
        for ROC_i, (eff_sgnl,rej_bkgd) in enumerate(zip(interpolation_points, bkgd_rejection_tmp)):
            #ROC_histograms[key].SetBinContent(ROC_i, rej_bkgd)
            ROC_histograms[key].Fill(eff_sgnl, rej_bkgd)

        max_val = ROC_histograms[key].GetMaximum() if ROC_histograms[key].GetMaximum() > max_val else max_val

        histogram_options = {"color":sample_dictionary['color'],
                             "stats":False,
                             "line_style":5,
                             "line_width":3,
                             "fill_style":0,
                             "x_label_size":0,
                             "x_title_size":0,
                             "y_title_size":0.05}
        SetHistogramConfiguration(ROC_histograms[key], histogram_options)

        legend.AddEntry(ROC_histograms[key], sample_dictionary['label'])

        ROC_histograms_ratio[key] = ROC_histograms[key].Clone(ROC_histograms[key].GetName()+"_ratio")

        histogram_options["x_label_size"] = 0.07
        histogram_options["x_title_size"] = 0.07
        histogram_options["y_label_size"] = 0.07
        histogram_options["y_title_size"] = 0.07
        histogram_options["y_title"] = "Ratio"
        histogram_options["y_title_offset"] = 0.5
        histogram_options["y_range"] = [0.7,1.15]

        SetHistogramConfiguration(ROC_histograms_ratio[key], histogram_options)

        if(sample_dictionary['ratioopt'] == "REF"):
            MVA_model_ratio_reference = key

    max_val *= 1.05


    canvas.cd()
    canvas.Draw()
    pad1.Draw()
    pad2.Draw()
    pad1.cd()
    for nsample, hist_key in enumerate(ROC_histograms):
        drawopt = "hist" if not nsample else "histsame"
        ROC_histograms_ratio[hist_key].Divide(ROC_histograms[MVA_model_ratio_reference])
        ROC_histograms[hist_key].SetMaximum(max_val)
        ROC_histograms[hist_key].Draw(drawopt)
        ROC_WP_multigraph[hist_key].Draw("P")


    ttlbox.Draw("same")
    legend.Draw("same")

    pad2.cd()
    for nsample, hist_key in enumerate(ROC_histograms_ratio):
        drawopt = "hist" if not nsample else "histsame"
        ROC_histograms_ratio[hist_key].Draw(drawopt)

    outName = outDir+canvas.GetName()
    canvas.Draw()
    canvas.SaveAs(outName+".png")
    canvas.SaveAs(outName+".pdf")
 
##_______________________________________________________________________________________
##
def ROCCurvePlots(sample_dictionary_list, histogram_name, mass, decay, global_signal_scale, signal_pattern, doLogY, outDir, extraLabel, mode="1MinusEffB"):
    canvas = SetupCanvas(("canv_ROC_curve_"+mass+"_"+decay+"_"+mode).replace(" ","_"))[0]
    canvas.cd()
    extra_text = "#splitline{"+extraLabel+"}"+"{"+decay+" "+mass+" GeV}"
    ttlbox = ExtraLabel(0.18, 0.76, 0.38, 0.87, extra_text=extra_text)    
    legend = SetupLegend(0.54,0.7,0.9,0.9)

    multigraph = TMultiGraph()

    CalculateEfficiencies(sample_dictionary_list, histogram_name, mass, decay, global_signal_scale, signal_pattern)

    for sample_dictionary in sample_dictionary_list:
        g = SetupTGraph(len(sample_dictionary['efficiency_sgnl']), 8, 1, sample_dictionary['color'])
        for i in range(len(sample_dictionary['efficiency_sgnl'])):
            if doLogY and not sample_dictionary['efficiency_bkgd'][i]:
                y_value = 1
            else:
                y_value = 1.0-sample_dictionary['efficiency_bkgd'][i] if mode == "1MinusEffB" else 1.0/sample_dictionary['efficiency_bkgd'][i]
            g.SetPoint(i, sample_dictionary['efficiency_sgnl'][i], y_value) 
        legend.AddEntry(g,sample_dictionary["label"])
        multigraph.Add(g)

    multigraph.GetXaxis().SetTitle("#epsilon_{signal}")
    y_axis_title = "1-#epsilon_{background}" if mode == "1MinusEffB" else "1/#epsilon_{background}"
    multigraph.GetYaxis().SetTitle(y_axis_title)

    multigraph.Draw("AP")
    legend.Draw("same")
    ttlbox.Draw("same")
    canvas.Draw()

    outputName = outDir+"/canv_"+histogram_name+mass+"_"+decay+"_ROC_"+mode
    outputName = outputName.replace("MVAScore","").replace("*","")

    canvas.SaveAs(outputName+".png")
    canvas.SaveAs(outputName+".pdf")

    if doLogY:
        multigraph.GetYaxis().SetLimits(0.0001, 100)
        multigraph.Draw("AP")
        legend.Draw("same")
        ttlbox.Draw("same")
        canvas.SetLogy()
        canvas.Draw()
        canvas.SaveAs(outputName+"_logy.png")
        canvas.SaveAs(outputName+"_logy.pdf")

##_______________________________________________________________________________________ 
##
def SoverBPlots(sample_dictionary_list, histogram_name, mass, decay, global_signal_scale, signal_pattern, doLogY, doRatio, outDir, extraLabel, mode="Bin By Bin"):

    Canvas_SoverB_Objects     = SetupCanvas(("SoverB_"+mass+"_"+decay+"_"+mode).replace(" ","_"), doRatio)
    Canvas_SoverSqrtB_Objects = SetupCanvas(("SoverSqrtB_"+mass+"_"+decay+"_"+mode).replace(" ","_"), doRatio)
    
    canvas_SoverB     = Canvas_SoverB_Objects[0]
    canvas_SoverSqrtB = Canvas_SoverSqrtB_Objects[0]
    
    MVA_SoverB     = {}
    MVA_SoverSqrtB = {}

    if doRatio:
        pad1_SoverB     = Canvas_SoverB_Objects[1]
        pad2_SoverB     = Canvas_SoverB_Objects[2]
        pad1_SoverSqrtB = Canvas_SoverSqrtB_Objects[1]
        pad2_SoverSqrtB = Canvas_SoverSqrtB_Objects[2]
        MVA_SoverB_ratio     = {}
        MVA_SoverSqrtB_ratio = {}

    extra_text = "#splitline{"+extraLabel+"}"+"{"+decay+" "+mass+" GeV}"
    ttlbox = ExtraLabel(0.18, 0.76, 0.38, 0.87, extra_text = extra_text, text_size=0.045)
    legend = SetupLegend(0.45,0.7,0.83,0.9,0.045)

    MVA_rebin = np.arange(-0.1, 1.1, 0.050)

    for index, sample_dictionary in enumerate(sample_dictionary_list):

        signal_file_name = sample_dictionary['signal'].replace("MASS",mass).replace("DECAY",decay)

        MVA_scores_sgnl = AddHistograms(sample_dictionary['path'], signal_file_name, sample_dictionary['signal_scales'],
                                                                    histogram_name, signal_pattern, global_signal_scale)

        MVA_scores_bkgd = AddHistograms(sample_dictionary['path'], sample_dictionary['background'], sample_dictionary['background_scales'],
                                                                    histogram_name, "", 1.0)
        
        MVA_scores_sgnl = MVA_scores_sgnl.Rebin(len(MVA_rebin)-1, MVA_scores_sgnl.GetName(), MVA_rebin)
        MVA_scores_bkgd = MVA_scores_bkgd.Rebin(len(MVA_rebin)-1, MVA_scores_bkgd.GetName(), MVA_rebin)

        MVA_model = sample_dictionary['label'].replace(".","").replace(",","").replace(" ","_")

        MVA_SoverB[MVA_model]     = MVA_scores_sgnl.Clone((MVA_model+"_"+mass+"_"+decay+"_"+mode+"_SoverB").replace(" ","_"))
        MVA_SoverSqrtB[MVA_model] = MVA_scores_sgnl.Clone((MVA_model+"_"+mass+"_"+decay+"_"+mode+"_SoverSqrtB").replace(" ","_"))

        first_bin = MVA_SoverB[MVA_model].FindBin(0)
        last_bin  = MVA_SoverB[MVA_model].FindBin(1)
       
        for nbin in range(first_bin,last_bin+1):
            if mode == "Bin By Bin":
                y_Title_SoverB = "S/B"
                y_Title_SoverSqrtB = "S/#sqrt{B}"
                SoverB_val     = 0 if MVA_scores_bkgd.GetBinContent(nbin) == 0 else MVA_scores_sgnl.GetBinContent(nbin)/MVA_scores_bkgd.GetBinContent(nbin)
                SoverSqrtB_val = 0 if MVA_scores_bkgd.GetBinContent(nbin) == 0 else MVA_scores_sgnl.GetBinContent(nbin)/np.sqrt(MVA_scores_bkgd.GetBinContent(nbin))
            elif mode == "Cumulative Inverted":
                y_Title_SoverB = "#int^{1}_{MVA} S#/#int^{1}_{MVA} B"
                y_Title_SoverSqrtB = "#int^{1}_{MVA} S#/#surd#int^{1}_{MVA} B"
                SoverB_val     = 0 if MVA_scores_bkgd.Integral(nbin, last_bin) == 0 else MVA_scores_sgnl.Integral(nbin, last_bin)/MVA_scores_bkgd.Integral(nbin, last_bin)
                SoverSqrtB_val = 0 if MVA_scores_bkgd.Integral(nbin, last_bin) == 0 else MVA_scores_sgnl.Integral(nbin, last_bin)/np.sqrt(MVA_scores_bkgd.Integral(nbin, last_bin))
            elif mode == "Cumulative":
                y_Title_SoverB = "#int^{MVA}_{0} S#/#int^{MVA}_{0} B"
                y_Title_SoverSqrtB = "#int^{MVA}_{0} S#/#surd#int^{MVA}_{0} B"
                SoverB_val     = MVA_scores_sgnl.Integral(first_bin, nbin)/MVA_scores_bkgd.Integral(first_bin, nbin)
                SoverSqrtB_val = MVA_scores_sgnl.Integral(first_bin, nbin)/np.sqrt(MVA_scores_bkgd.Integral(first_bin, nbin))

            MVA_SoverB[MVA_model].SetBinContent(nbin, SoverB_val)
            MVA_SoverSqrtB[MVA_model].SetBinContent(nbin, SoverSqrtB_val)
    
        max_SoverB = MVA_SoverB[MVA_model].GetMaximum() if not index else max(max_SoverB, MVA_SoverB[MVA_model].GetMaximum())
        max_SoverSqrtB = MVA_SoverSqrtB[MVA_model].GetMaximum() if not index else max(max_SoverSqrtB, MVA_SoverSqrtB[MVA_model].GetMaximum())
        
        histogram_options = {"color":sample_dictionary['color'],
                             "line_style":5,
                             "line_width":3,
                             "fill_style":0,
                             "title":";MVA score;"+y_Title_SoverB,
                             "stats":False}

        if doRatio:
            histogram_options["x_label_size"] = 0
            histogram_options["x_title_size"] = 0
            histogram_options["y_title_size"] = 0.05 

        SetHistogramConfiguration(MVA_SoverB[MVA_model], histogram_options)
        legend.AddEntry(MVA_SoverB[MVA_model], sample_dictionary['label'])

        histogram_options["title"] = ";MVA score;"+y_Title_SoverSqrtB
        SetHistogramConfiguration(MVA_SoverSqrtB[MVA_model], histogram_options)

        if doRatio:
            if(sample_dictionary['ratioopt'] == "REF"):
                MVA_model_ratio_reference = MVA_model
            MVA_SoverB_ratio[MVA_model] = MVA_SoverB[MVA_model].Clone((MVA_model+"_"+mass+"_"+decay+"_"+mode+"_SoverB_ratio").replace(" ","_"))
            MVA_SoverSqrtB_ratio[MVA_model] = MVA_SoverSqrtB[MVA_model].Clone((MVA_model+"_"+mass+"_"+decay+"_"+mode+"_SoverSqrtB_ratio").replace(" ","_"))

            histogram_options["x_label_size"] = 0.07
            histogram_options["x_title_size"] = 0.07
            histogram_options["y_label_size"] = 0.07
            histogram_options["y_title_size"] = 0.07
            histogram_options["y_title"] = "Ratio"
            histogram_options["y_title_offset"] = 0.5
            histogram_options["y_range"] = [0,1.6]

            SetHistogramConfiguration(MVA_SoverB_ratio[MVA_model], histogram_options)
            SetHistogramConfiguration(MVA_SoverSqrtB_ratio[MVA_model], histogram_options)

    max_SoverB *= 1.05
    max_SoverSqrtB *= 1.05

    outputName = outDir+"/canv_"+histogram_name+mass+"_"+decay+"_"+mode
    outputName = outputName.replace("MVAScore","").replace("*","").replace(" ","_")

    canvas_SoverB.cd()
    canvas_SoverB.Draw()
    
    if doRatio:
        pad1_SoverB.Draw()
        pad2_SoverB.Draw()
        pad1_SoverB.cd()
        for model in MVA_SoverB_ratio:
            MVA_SoverB_ratio[model].Divide(MVA_SoverB[MVA_model_ratio_reference])

    for index, model in enumerate(MVA_SoverB):
        drawopt = "hist" if not index else "histsame"
        MVA_SoverB[model].SetMaximum(max_SoverB)
        MVA_SoverB[model].Draw(drawopt)
        if doRatio:
            pad2_SoverB.cd()
            MVA_SoverB_ratio[model].Draw(drawopt)
            pad1_SoverB.cd()


    ttlbox.Draw("same")
    legend.Draw("same")

    canvas_SoverB.Draw()
    canvas_SoverB.SaveAs(outputName+"_SoverB.png")
    canvas_SoverB.SaveAs(outputName+"_SoverB.pdf")

    if doLogY:
        if doRatio:
            pad1_SoverB.cd()
            pad1_SoverB.SetLogy()
            pad1_SoverB.Draw()
        else:
            canvas_SoverB.SetLogy()
        canvas_SoverB.Draw()
        canvas_SoverB.SaveAs(outputName+"_SoverB_LogY.png")
        canvas_SoverB.SaveAs(outputName+"_SoverB_LogY.pdf")

    canvas_SoverSqrtB.cd()
    canvas_SoverSqrtB.Draw()

    if doRatio:
        pad1_SoverSqrtB.Draw()
        pad2_SoverSqrtB.Draw()
        pad1_SoverSqrtB.cd()
        for model in MVA_SoverSqrtB_ratio:
            MVA_SoverSqrtB_ratio[model].Divide(MVA_SoverSqrtB[MVA_model_ratio_reference])

    for index, model in enumerate(MVA_SoverSqrtB):
        drawopt = "hist" if not index else "histsame"
        MVA_SoverSqrtB[model].SetMaximum(max_SoverSqrtB)
        MVA_SoverSqrtB[model].Draw(drawopt)
        if doRatio:
            pad2_SoverSqrtB.cd()
            MVA_SoverSqrtB_ratio[model].Draw(drawopt)
            pad1_SoverSqrtB.cd()
    
    ttlbox.Draw("same")
    legend.Draw("same")
    canvas_SoverSqrtB.Draw()
    canvas_SoverSqrtB.SaveAs(outputName+"_SoverSqrtB.png")
    canvas_SoverSqrtB.SaveAs(outputName+"_SoverSqrtB.pdf")
    if doLogY:
        if doRatio:
            pad1_SoverSqrtB.cd()
            pad1_SoverSqrtB.SetLogy()
            pad1_SoverSqrtB.Draw()
        else:
            canvas_SoverSqrtB.SetLogy()
        canvas_SoverSqrtB.Draw()
        canvas_SoverSqrtB.SaveAs(outputName+"_SoverSqrtB_LogY.png")
        canvas_SoverSqrtB.SaveAs(outputName+"_SoverSqrtB_LogY.pdf")

##_______________________________________________________________________________________
##
def AcceptanceRejectionPlots(sample_dictionary_list, histogram_name, mass, decay, global_signal_scale, signal_pattern, outDir, extraLabel, do_normed = False):
    
    temp_sgnl_name = "signal_efficiency" if do_normed else "signal_acceptance"
    temp_bkgd_name = "background_inv_efficiency" if do_normed else "background_rejection"

    ylabel_sgnl = "#int^{1}_{MVA}S" if not do_normed else "#frac{1}{S}#int^{1}_{MVA}S"
    ylabel_bkgd = "#int^{MVA}_{0}B" if not do_normed else "#frac{1}{B}#int^{MVA}_{0}B"

    temp_sgnl_name += ("_"+mass+"_"+decay).replace(" ","_")
    temp_bkgd_name += ("_"+mass+"_"+decay).replace(" ","_")

    Canvas_Sgnl_Objects = SetupCanvas("canv_"+temp_sgnl_name, True)
    Canvas_Bkgd_Objects = SetupCanvas("canv_"+temp_bkgd_name, True)

    canvas_sgnl, pad1_sgnl, pad2_sgnl = [Canvas_Sgnl_Objects[i] for i in (0,1,2)]
    canvas_bkgd, pad1_bkgd, pad2_bkgd = [Canvas_Bkgd_Objects[i] for i in (0,1,2)]

    extra_text = "#splitline{"+extraLabel+"}"+"{"+decay+" "+mass+" GeV}"
    ttlbox_sgnl = ExtraLabel(0.18, 0.76, 0.38, 0.87, text_size = 0.045, extra_text = extra_text)
    ttlbox_bkgd = ExtraLabel(0.18, 0.76, 0.38, 0.87, text_size = 0.045, extra_text = extraLabel)

    legend_sgnl = SetupLegend(0.7,0.6,0.95,0.9, 0.045)
    legend_bkgd = SetupLegend(0.7,0.6,0.95,0.9, 0.045)

    bins = np.arange(-0.1, 1.1, 0.050)
    nbins = len(bins) - 1

    histogram_sgnl = {}
    histogram_bkgd = {}

    histogram_sgnl_ref = {}
    histogram_bkgd_ref = {}

    multigraph_sgnl = {}
    multigraph_bkgd = {}

    CalculateEfficiencies(sample_dictionary_list, histogram_name, mass, decay, global_signal_scale, signal_pattern, do_normed, bins)

    sgnl_max = 0
    bkgd_max = 0

    for sample_dictionary in sample_dictionary_list:
        
        key = sample_dictionary["label"]
        
        histogram_sgnl_name = ("h_"+temp_sgnl_name+"_"+sample_dictionary["label"]).replace(".","").replace(",","").replace(" ","")
        histogram_bkgd_name = ("h_"+temp_bkgd_name+"_"+sample_dictionary["label"]).replace(".","").replace(",","").replace(" ","")

        histogram_sgnl[key] = TH1D(histogram_sgnl_name, ";MVA Score;"+ylabel_sgnl, nbins, bins)
        histogram_bkgd[key] = TH1D(histogram_bkgd_name, ";MVA Score;"+ylabel_bkgd, nbins, bins)
        
        multigraph_sgnl[key] = TMultiGraph()
        multigraph_bkgd[key] = TMultiGraph()

        graph_lo_sgnl = SetupTGraph(1, 23, 2, sample_dictionary['color'])
        graph_hi_sgnl = SetupTGraph(1, 22, 2, sample_dictionary['color'])
        graph_lo_sgnl.SetPoint(0, sample_dictionary['MVA_cuts'][0], sample_dictionary['acceptance_sgnl'][0])
        graph_hi_sgnl.SetPoint(0, sample_dictionary['MVA_cuts'][1], sample_dictionary['acceptance_sgnl'][1])
        legend_sgnl.AddEntry(graph_lo_sgnl, "("+str("%.2f"%sample_dictionary['MVA_cuts'][0])+", "+str("%.2f"%sample_dictionary['acceptance_sgnl'][0])+")")
        legend_sgnl.AddEntry(graph_hi_sgnl, "("+str("%.2f"%sample_dictionary['MVA_cuts'][1])+", "+str("%.2f"%sample_dictionary['acceptance_sgnl'][1])+")")

        graph_lo_bkgd = SetupTGraph(1, 23, 2, sample_dictionary['color'])
        graph_hi_bkgd = SetupTGraph(1, 22, 2, sample_dictionary['color'])
        graph_lo_bkgd.SetPoint(0, sample_dictionary['MVA_cuts'][0], sample_dictionary['rejection_bkgd'][0])
        graph_hi_bkgd.SetPoint(0, sample_dictionary['MVA_cuts'][1], sample_dictionary['rejection_bkgd'][1])
        legend_bkgd.AddEntry(graph_lo_bkgd, "("+str("%.2f"%sample_dictionary['MVA_cuts'][0])+", "+str("%.2f"%sample_dictionary['rejection_bkgd'][0])+")")
        legend_bkgd.AddEntry(graph_hi_bkgd, "("+str("%.2f"%sample_dictionary['MVA_cuts'][1])+", "+str("%.2f"%sample_dictionary['rejection_bkgd'][1])+")")

        multigraph_sgnl[key].Add(graph_lo_sgnl)
        multigraph_sgnl[key].Add(graph_hi_sgnl)

        multigraph_bkgd[key].Add(graph_lo_bkgd)
        multigraph_bkgd[key].Add(graph_hi_bkgd)

        histogram_options = {"color":sample_dictionary['color'],
                             "stats":False,
                             "line_style":5,
                             "line_width":3,
                             "fill_style":0,
                             "x_label_size":0,
                             "x_title_size":0,
                             "y_title_size":0.05}

        SetHistogramConfiguration(histogram_sgnl[key], histogram_options)
        SetHistogramConfiguration(histogram_bkgd[key], histogram_options)

        for nbin, (eff_sgnl, eff_bkgd) in enumerate( zip(reversed(sample_dictionary["efficiency_sgnl"]), reversed(sample_dictionary["efficiency_bkgd"])) ):

            bkgd_value = 1 - eff_bkgd if do_normed else sample_dictionary["efficiency_bkgd"][-1] - eff_bkgd

            histogram_sgnl[key].SetBinContent(nbin+1, eff_sgnl)
            histogram_bkgd[key].SetBinContent(nbin+1, bkgd_value)

        sgnl_max = max(histogram_sgnl[key].GetMaximum(), sgnl_max)
        bkgd_max = max(histogram_bkgd[key].GetMaximum(), bkgd_max)    

        if(sample_dictionary['ratioopt'] == "REF"):
            MVA_model_ratio_reference = key
        histogram_sgnl_ref[key] = histogram_sgnl[key].Clone(histogram_sgnl[key].GetName()+"_ratio")
        histogram_bkgd_ref[key] = histogram_bkgd[key].Clone(histogram_bkgd[key].GetName()+"_ratio")

        histogram_options["x_label_size"] = 0.07
        histogram_options["x_title_size"] = 0.07
        histogram_options["y_label_size"] = 0.07
        histogram_options["y_title_size"] = 0.07
        histogram_options["y_title"] = "Ratio"
        histogram_options["y_title_offset"] = 0.5
        histogram_options["y_range"] = [0.9,1.1]
        
        SetHistogramConfiguration(histogram_sgnl_ref[key], histogram_options)
        histogram_options["y_range"] = [0.85,1.15]
        SetHistogramConfiguration(histogram_bkgd_ref[key], histogram_options)


    sgnl_max *= 1.6
    bkgd_max *= 1.6

    canvas_sgnl.cd()
    canvas_sgnl.Draw()
    pad1_sgnl.Draw() 
    pad2_sgnl.Draw()
    pad1_sgnl.cd()
    for nsample, hist_key in enumerate(histogram_sgnl):
        drawopt = "hist" if not nsample else "histsame"
        legend_sgnl.AddEntry(histogram_sgnl[hist_key], hist_key)
        histogram_sgnl_ref[hist_key].Divide(histogram_sgnl[MVA_model_ratio_reference])
        histogram_sgnl[hist_key].SetMaximum(sgnl_max)
        histogram_sgnl[hist_key].Draw(drawopt)
        multigraph_sgnl[hist_key].Draw("P")

    ttlbox_sgnl.Draw("same")
    legend_sgnl.Draw("same")

    pad2_sgnl.cd()
    for nsample, hist_key in enumerate(histogram_sgnl_ref):
        drawopt = "hist" if not nsample else "histsame"
        histogram_sgnl_ref[hist_key].Draw(drawopt)

    outNameSgnl = outDir+temp_sgnl_name
    canvas_sgnl.Draw()
    canvas_sgnl.SaveAs(outNameSgnl+".png")
    canvas_sgnl.SaveAs(outNameSgnl+".pdf")

    canvas_bkgd.cd()
    pad1_bkgd.Draw()
    pad2_bkgd.Draw()
    pad1_bkgd.cd()
    for nsample, hist_key in enumerate(histogram_bkgd):
        drawopt = "hist" if not nsample else "histsame"
        legend_bkgd.AddEntry(histogram_bkgd[hist_key], hist_key)
        histogram_bkgd_ref[hist_key].Divide(histogram_bkgd[MVA_model_ratio_reference])
        histogram_bkgd[hist_key].SetMaximum(bkgd_max)
        histogram_bkgd[hist_key].Draw(drawopt)
        multigraph_bkgd[hist_key].Draw("P")

    ttlbox_bkgd.Draw("same")
    legend_bkgd.Draw("same")

    pad2_bkgd.cd()
    for nsample, hist_key in enumerate(histogram_bkgd_ref):
        drawopt = "hist" if not nsample else "histsame"
        histogram_bkgd_ref[hist_key].Draw(drawopt)

    outNameBkgd = outDir+temp_bkgd_name
    canvas_bkgd.Draw()
    canvas_bkgd.SaveAs(outNameBkgd+".png")
    canvas_bkgd.SaveAs(outNameBkgd+".pdf")


##_______________________________________________________________________________________
##
parser = OptionParser()
parser.add_option("-O", "--outDir",help="Output directory.",dest="outDir",default="./test/")
parser.add_option("-H", "--histo",help="Name of input histogram.", dest="histo", default="")
parser.add_option("-L", "--doLogy",help="Include additional plots with y-axis log scale.",dest="doLogy",action="store_true",default=False)
parser.add_option("-M", "--mass",help="Signal mass point in GeV.", dest="mass", default="1400")
parser.add_option("-D", "--decay",help="Signal decay channel.", dest="decay", default="HtHt")
parser.add_option("-P", "--sgnlPattern",help="Signal pattern to replace '*' character in histogram name.", dest="sgnlPattern", default = "")
parser.add_option("-S", "--sgnlScale", help="Global scaling factor for signal samples. This is applied to all MC campaigns.", dest="sgnlScale", default="1.0")
parser.add_option("-r", "--doROCPlots", help="Do ROC curve plots.", dest="doROCPlots", action="store_true", default=False)
parser.add_option("-s", "--doSoverBPlots", help="Do yield (S/B) and significance (S/sqrt(B)) plots.", dest="doSoverBPlots", action="store_true", default=False)
parser.add_option("-e", "--doEfficiencyPlots", help="Do signal efficiency and background rejection plots.", dest="doEfficiencyPlots", action="store_true" , default=False)

(options, args) = parser.parse_args()
outDir = options.outDir
histo = options.histo
doLogY = options.doLogy
mass = options.mass
decay = options.decay
sgnlPattern = options.sgnlPattern
sgnlScale = float(options.sgnlScale)
doROCPlots = options.doROCPlots
doSoverBPlots = options.doSoverBPlots
doAcceptanceRejectionPlots = options.doEfficiencyPlots

sample_dictionary_list = samples_dictionary_template.dictionary_list

os.system("mkdir -p "+outDir)

extraLabel = "1l, #geq6j, #geq3b, #geq2M, #geq3J"

if doROCPlots:
    os.system("mkdir -p "+outDir+"/ROC")
    ROCCurvePlotsHist(sample_dictionary_list, histo, mass, decay, sgnlScale, sgnlPattern, doLogY, outDir+"/ROC/", extraLabel, mode="1MinusEffB")
    ROCCurvePlotsHist(sample_dictionary_list, histo, mass, decay, sgnlScale, sgnlPattern, doLogY, outDir+"/ROC/", extraLabel, mode="1OverEffB")

if doSoverBPlots:
    os.system("mkdir -p "+outDir+"/SoverB")
    for mode in ["Bin By Bin", "Cumulative", "Cumulative Inverted"]:
        SoverBPlots(sample_dictionary_list, histo, mass, decay, sgnlScale, sgnlPattern, True, True, outDir+"/SoverB/", extraLabel, mode=mode)

if doAcceptanceRejectionPlots:
    os.system("mkdir -p "+outDir+"/AcceptanceRejection")
    AcceptanceRejectionPlots(sample_dictionary_list, histo, mass, decay, sgnlScale, sgnlPattern, outDir+"/AcceptanceRejection/", extraLabel, do_normed = False)
    AcceptanceRejectionPlots(sample_dictionary_list, histo, mass, decay, sgnlScale, sgnlPattern, outDir+"/AcceptanceRejection/", extraLabel, do_normed = True)
