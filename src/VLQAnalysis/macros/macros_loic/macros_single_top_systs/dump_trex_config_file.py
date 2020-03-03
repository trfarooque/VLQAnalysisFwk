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


config_trexF = open("config_trexF.txt","w")
sys_types = ["1lep","0lepHighMtbmin","0lepLowMtbmin","0lepOther"]
for sys_type in sys_types:
    lines = []
    lines += ["Systematics: "]
    lines += ["Title: "]
    lines += ["Category: \"Background uncertainties\""]
    uncertainty = 0.6
    if sys_type == "0lepHighMtbmin":
        uncertainty = 0.8
    lines += ["OverallUp: " + `uncertainty`]
    lines += ["OverallDown: -" + `uncertainty`]
    lines += ["Samples: \"Singletop\""]
    lines += ["Regions: "]

    boost_regions = ["0Tex0Hex","1Tex0Hex","0Tex1Hex","1Tex1Hex","2Tin0_1Hwin","0Tin2Hin","2THin"]

    for boost in boost_regions:
        count = 0
        regions = []
        regions += fit_regions_0l+fit_regions_1l+validation_regions_0l+validation_regions_1l
        for reg in regions:
            if reg['name'].find(boost)==-1:
                continue
            if sys_type=="1lep" and reg['name'].find("1lep")==-1:
                continue
            if sys_type=="0lep" and reg['name'].find("0lep")==-1:
                continue
            if sys_type.find("0lep")>-1 and not sys_type.replace("0lep","")=="Other" and reg['name'].find(sys_type.replace("0lep",""))==-1:
                continue
            if sys_type.find("Other")>-1:
                if reg['name'].find("Mtbmin")>-1:
                    continue
                if reg['name'].find("1lep")>-1:
                    continue
            lines[6] += reg['name']
            lines[6] += ","
            count += 1
        if count==0:
            continue;
        #NP name
        lines[0] += "HTX_BKGNP_OTHERS_Singletop_XS_" + sys_type + "_" + boost + "; "
        #NP title
        lines[1] += "\"Single-top norm. ("
        if(sys_type.find("1lep")>-1):
            lines[1] += "1l, "
        else:
            lines[1] += "0l, "
        boosted_regime = boost.replace("Tex","T, ").replace("Hex","H").replace("2Tin","#geq2T,").replace("2THin","#geq2TH").replace("0_1Hwin","0-1H").replace("2Hin","#geq2H").replace("0Tin","#geq0T")
        lines[1] += boosted_regime+")"
        lines[1] += "\"; "
        lines[6] += "; "
    config_trexF.write("\n")
    for line in lines:
        line
        final_line = line + "\n"
        final_line = final_line.replace(",;",";")
        final_line = final_line.replace("; \n","\n")
        config_trexF.write(final_line)
    config_trexF.write("\n")
    config_trexF.write("\n")
    lines = []

config_trexF.close()
