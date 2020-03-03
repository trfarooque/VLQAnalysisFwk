#!/bin/python
import os
import time
import socket
import sys
import datetime
from ROOT import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )
from VLQ_Samples import *

def getLimit( path ):
    if not os.path.exists(path):
        return -1
    f = TFile.Open(path,"read")
    print path
    h = f.Get("limit").Clone()
    h.SetDirectory(0)
    limit = -1
    limit = h.GetBinContent(2)
    f.Close()
    return limit


h2 = TH2F("frame_ul", ";m_{#tilde{g}};m_{#tilde{#chi}}", 1601, 899.5, 2500.5, 2101, -0.5, 2100.5 );

for sample in GetSUSYSamples():
    SType = sample['sampleType']
    SName = sample['name']
    splitted = SType.split("_")
    neutralino = int(splitted[2])
    gluino = int(splitted[1])
    limit = getLimit( "/nfs/pic.es/user/l/lvalery/scratch2/VLQFitOutputs/2017_04_18_VLQAnalysis-00-03-09-01_WithNNLO/Results_Asimov_0LComb_2bHighMtbmin_SUSY/BONLY__"+SName+"___/Limits/BONLY__"+SName+"___.root" )
    if limit==-1:
        continue
    h2.SetBinContent( h2.FindBin( gluino, neutralino), limit)

out = TFile("SUSY.root","recreate")
c= TCanvas()
gStyle.SetOptStat(0)
gStyle.SetPaintTextFormat(".2f");
h2.Draw("text")
out.cd()
h2.Write()
out.Close()
c.Print("SUSY.eps")
