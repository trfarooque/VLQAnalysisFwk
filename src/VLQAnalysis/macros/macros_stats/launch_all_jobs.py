#!/bin/python
import optparse
import os

parser = optparse.OptionParser()
parser.add_option('-i', '--input_files_folder', default="")
parser.add_option('-I', '--input_config_file', default="/sps/atlas/l/lvalery/VLQ/AnalysisCode/VLQAnalysis/macros/macros_stats/templates/TEMPLATE_configFile__SIGNAL_.txt")
parser.add_option('-o', '--output_folder', default="")
parser.add_option('-f', '--fits', default="BONLY_Asimov1lep,BONLY_Asimov0lep,BONLY_Asimov0lep1lep,BONLY_BlindData1lep,BONLY_BlindData0lep,BONLY_BlindData0lep1lep")
parser.add_option('-l', '--limits', default="BONLY_Asimov1lep,BONLY_Asimov0lep,BONLY_Asimov0lep1lep")
parser.add_option('-t', '--tarball', default="/sps/atlas/l/lvalery/VLQ/TtHFitter.tgz")
parser.add_option('-a', '--allBR', default=False, action="store_true")
(opts, args) = parser.parse_args()
input_files_folder = opts.input_files_folder
input_CF = opts.input_config_file
output_folder = opts.output_folder
fits = opts.fits.split(",")
limits = opts.limits.split(",")
tarball = opts.tarball
allBR = opts.allBR

for fit in fits:
    print fit
    com = "python PrepareConfigFilesFromTemplate.py inputTemplate=" + input_CF
    com += " outputFolder=" + output_folder + "/ConfigFiles_" + fit + "/"
    com += " inputDir=" + input_files_folder
    com += " addition=\"" + fit + "\""

    com += " FITBLIND="
    if(fit.upper().find("ASIMOV")>-1):
        com += "TRUE"
    else:
        com += "FALSE"

    com += " LIMITBLIND="
    if(fit.upper().find("ASIMOV")>-1):
        com += "TRUE"
    else:
        com += "FALSE"

    com += " signal=ONE"

    com += " FITTYPE="
    if(fit.upper().find("BONLY")>-1):
        com += "BONLY"
    else:
        com += "SPLUSB"

    com += " NORMFACTOR="
    if(fit.upper().find("RANKING")>-1):
        com += "1,-20,20"
    else:
        com += "1,0,100"

    com += " USEDATA="
    if(fit.upper().find("DATA")>-1):
        com += "TRUE"
    else:
        com += "FALSE"

    com += " DOONELEP="
    if(fit.upper().find("1LEP")>-1):
        com += "TRUE"
    else:
        com += "FALSE"

    com += " DOZEROLEP="
    if(fit.upper().find("0LEP")>-1):
        com += "TRUE"
    else:
        com += "FALSE"

    com += " DOVR="
    if(fit.upper().find("DATA")>-1 and not fit in limits):
        com += "TRUE"
    else:
        com += "FALSE"

    com += " useBlindingCuts="
    if(fit.upper().find("BLIND")>-1):
        com += "TRUE"
    else:
        com += "FALSE"
    com += " USE4TOPS=TRUE LUMIVALUE=36074.6"

    if allBR:
        com += " DOALLBR=TRUE"
    
    os.system(com)

    com = "python LaunchTRExFitterOnBatch.py tarball=" + tarball
    com += " outputDir=" + output_folder + "/Results_" + fit + "/"
    if(fit.upper().find("RANKING")>-1):
        com += " action=ranking"
    else:
        com += " action="
        if fit.upper().find("PLOTS")>-1:
            com += " fitpreplotspostplots"
        if( fit in limits ):
            com += "limits"

    if(fit.upper().find("BLIND")>-1 and fit.upper().find("DATA")>-1):
        com += " queue=at3_xxl"

    com += " inputDir=" + output_folder + "/ConfigFiles_" + fit + "/configFile_"
    ref_com = com
    final_coms = []
    if(fit.upper().find("RANKING")==-1):
        temp_com = ref_com
        if(fit.upper().find("0LEP")>-1):
            temp_com += "VLQ_TT"
        if(fit.upper().find("BLIND")>-1 and fit.upper().find("DATA")>-1):
            temp_com += "*1000_TDoublet*"
        final_coms += [temp_com]
    else:
        for sig in ["VLQ_TT_1000_TDoublet*","UEDRPP_1400*"]:
            temp_com = ref_com
            temp_com += sig
            final_coms += [temp_com]

    for command in final_coms:
        os.system(command)
        os.system("sleep 2")

