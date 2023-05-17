from ROOT import *

##_______________________________________________________________________________________
##
def ExtraLabel(xmin,ymin,xmax,ymax,extra_text="1l, #geq6j, #geq3b, #geq2M, #geq3J"):
    ttlbox = TPaveText(xmin,ymin,xmax,ymax, "NBNDC");
    ttlbox.SetTextAlign(12)
    ttlbox.SetFillColor(0)
    ttlbox.SetFillStyle(0)
    ttlbox.SetLineColor(0)
    ttlbox.SetLineStyle(0)
    ttlbox.SetLineWidth(0)
    ttlbox.SetBorderSize(0)
    ttlbox.SetTextSize(0.035)
    ttlbox.SetTextFont(42)
    ttlbox.SetShadowColor(0)
    extra_label = "#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}"
    if extra_text:
        extra_label = "#splitline{"+extra_label+"}{"+extra_text+"}"
    ttlbox.AddText(extra_label);
    return ttlbox

##_______________________________________________________________________________________
##
def SetupLegend(xmin,ymin,xmax,ymax):
    legend = TLegend(xmin,ymin,xmax,ymax)
    legend.SetFillColor(0)
    legend.SetFillStyle(0)
    legend.SetTextSize(0.035)
    legend.SetLineColor(0)
    return legend

##_______________________________________________________________________________________
##
def SetupCanvas(name="canvas", do_ratio = False):
    canvas = TCanvas(name,"",1000,800)
    canvas.SetBottomMargin(0.15)
    canvas.SetLeftMargin(0.15)
    canvas.SetRightMargin(0.05)
    canvas.SetTopMargin(0.05)

    if not do_ratio:
        canvas.SetTicks(1,1)
    else:
        pad_top = TPad(name+"_pad_top", "", 0, 0.4, 1, 1)
        pad_top.SetTopMargin(0.05)
        pad_top.SetBottomMargin(0)
        pad_top.SetLeftMargin(0.15)
        pad_top.SetRightMargin(0.03)
        pad_top.SetTicks(1,1)
        
        pad_bottom = TPad(name+"_pad_bottom", "", 0, 0, 1, 0.4)
        pad_bottom.SetTopMargin(0)
        pad_bottom.SetBottomMargin(0.25)
        pad_bottom.SetLeftMargin(0.15)
        pad_bottom.SetRightMargin(0.03)
        pad_bottom.SetTicks(0,1)

    return [canvas] if not do_ratio else [canvas, pad_top, pad_bottom]

##_______________________________________________________________________________________
##
def SetupTGraph(n_points, marker_style, marker_size, marker_color):
    g = TGraph(n_points)
    g.SetMarkerStyle(marker_style)
    g.SetMarkerSize(marker_size)
    g.SetMarkerColor(marker_color)
    return g

##_______________________________________________________________________________________
##
def SetHistogramConfiguration(histogram, options):
    if("color" in options):
        histogram.SetLineColor(options["color"])
    if("line_style" in options):
        histogram.SetLineStyle(options["line_style"])
    if("line_width" in options):
        histogram.SetLineWidth(options["line_width"])
    if("fill_style" in options):
        histogram.SetFillStyle(options["fill_style"])
    if("title" in options):
        histogram.SetTitle(options["title"])
    if("stats" in options):
        histogram.SetStats(options["stats"])
    if("x_label_size" in options):
        histogram.GetXaxis().SetLabelSize(options["x_label_size"])
    if("x_title_size" in options):
        histogram.GetXaxis().SetTitleSize(options["x_title_size"])
    if("x_title" in options):
        histogram.GetXaxis().SetTitle(options["x_title"])
    if("x_title_offset" in options):
        histogram.GetXaxis().SetTitleOffset(options["x_title_offset"])
    if("x_range" in options):
        histogram.GetXaxis().SetRangeUser(options["x_range"][0], options["x_range"][1])
    if("y_label_size" in options):
        histogram.GetYaxis().SetLabelSize(options["y_label_size"])
    if("y_title_size" in options):
        histogram.GetYaxis().SetTitleSize(options["y_title_size"])
    if("y_title" in options):
        histogram.GetYaxis().SetTitle(options["y_title"])
    if("y_title_offset" in options):
        histogram.GetYaxis().SetTitleOffset(options["y_title_offset"])
    if("y_range" in options):
        histogram.GetYaxis().SetRangeUser(options["y_range"][0], options["y_range"][1])
