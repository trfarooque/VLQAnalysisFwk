#!/bin/python
import os
import time
import socket
import sys
import datetime

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )
from VLQ_Samples import *
from regions_dictionary import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/IFAETopFramework/" )
from BatchTools import *
from Samples import *

##________________________________________________________
## OPTIONS
channels = ["./.",]
dataLumi = (3212.96 + 32861.6) #in pb-1
isMCMC = False
dumpFiles = True
lumiLabel = "%.1f fb^{-1}"%(dataLumi/1000.)
##........................................................

##________________________________________________________
## Defines some useful variables
now = "rootfile_newcalib/"
here = os.getcwd()
##........................................................

##________________________________________________________
## Defining the paths and the tarball
inputDir="/nfs/pic.es/user/l/lvalery/scratch2/VLQAnalysisRun2/VLQAnalysisOutputs_DataMC_NewCCalib_NoTRF_2017_04_25_2105/"
outputDir = inputDir
folderRootFiles=here+"/Rootfiles_Plots_" + now
##........................................................

##________________________________________________________
## Creating usefull repositories
os.system("mkdir -p " + folderRootFiles)
##........................................................

##________________________________________________________
## Getting all samples and their associated weight/object systematics
Samples = []
Samples += [{'type':"data_topq1",'pattern':["Data_data.DAOD_TOPQ1.",]}]
Samples += [{'type':"data_topq4",'pattern':["Data_data.DAOD_TOPQ4.",]}]
Samples += [{'type':"ttbarlight",'pattern':["ttbarlight",]}]
Samples += [{'type':"ttbarcc",'pattern':["ttbarcc",]}]
Samples += [{'type':"ttbarbb",'pattern':["ttbarbb"]}]
#Samples += [{'type':"Others",'pattern':["W+jets22","Z+jets22","Singletop","Dibosons","topEW","ttH"]}]
Samples += [{'type':"wjets",'pattern':["W+jets22light","W+jets22beauty","W+jets22charm"]}]
Samples += [{'type':"zjets",'pattern':["Z+jets22light","Z+jets22beauty","Z+jets22charm"]}]
Samples += [{'type':"singletop",'pattern':["Singletop",]}]
Samples += [{'type':"dibosons",'pattern':["Dibosons",]}]
Samples += [{'type':"ttX",'pattern':["topEW","ttH"]}]
Samples += [{'type':"qcd",'pattern':["QCDE","QCDMU"]}]
if isMCMC:
    Samples += [{'type':"vlq_1000_doublet",'pattern':["VLQ_TT_1000_TDoublet",]}]
    Samples += [{'type':"vlq_1200_doublet",'pattern':["VLQ_TT_1200_TDoublet",]}]
printGoodNews("--> All background samples recovered")
##........................................................

##_________________
## Style definition
Styles = []
normStyle = "NORM"
if isMCMC:
    normStyle = "NORM"
Styles += [{'type':"ttbarlight",'legend':"t#bar{t}+ light-jets",'drawopt':"hist",'legopt':"f",'drawstack':"TRUE",'resopt':"SKIP",'drawscale':"NORM",'linecolor':"kBlack",'fillcolor':"kWhite",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':"%.1f"}]
Styles += [{'type':"ttbarbb",'legend':"t#bar{t}+#geq1b",'drawopt':"hist",'legopt':"f",'drawstack':"TRUE",'resopt':"SKIP",'drawscale':"NORM",'linecolor':"kBlack",'fillcolor':"kBlue-6",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':"%.1f"}]
Styles += [{'type':"ttbarcc",'legend':"t#bar{t}+#geq1c",'drawopt':"hist",'legopt':"f",'drawstack':"TRUE",'resopt':"SKIP",'drawscale':"NORM",'linecolor':"kBlack",'fillcolor':"kBlue-10",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':"%.1f"}]
Styles += [{'type':"wjets",'legend':"W+jets",'drawopt':"hist",'legopt':"f",'drawstack':"TRUE",'resopt':"SKIP",'drawscale':"NORM",'linecolor':"kBlack",'fillcolor':"kGreen-1",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':"%.1f"}]
Styles += [{'type':"zjets",'legend':"Z+jets",'drawopt':"hist",'legopt':"f",'drawstack':"TRUE",'resopt':"SKIP",'drawscale':"NORM",'linecolor':"kBlack",'fillcolor':"kGreen-5",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':"%.1f"}]
Styles += [{'type':"singletop",'legend':"Single-top",'drawopt':"hist",'legopt':"f",'drawstack':"TRUE",'resopt':"SKIP",'drawscale':"NORM",'linecolor':"kBlack",'fillcolor':"kCyan+1",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':"%.1f"}]
Styles += [{'type':"ttX",'legend':"t#bar{t}+X",'drawopt':"hist",'legopt':"f",'drawstack':"TRUE",'resopt':"SKIP",'drawscale':"NORM",'linecolor':"kBlack",'fillcolor':"kCyan+2",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':"%.1f"}]
Styles += [{'type':"others",'legend':"Others",'drawopt':"hist",'legopt':"f",'drawstack':"TRUE",'resopt':"SKIP",'drawscale':"NORM",'linecolor':"kBlack",'fillcolor':"kOrange-3",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':"%.1f"}]
Styles += [{'type':"qcd",'legend':"QCD",'drawopt':"hist",'legopt':"f",'drawstack':"TRUE",'resopt':"SKIP",'drawscale':"NORM",'linecolor':"kBlack",'fillcolor':"kViolet-1",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"0",'markercolor':"kBlack",'markerstyle':"0",'yieldformat':"%.1f"}]
Styles += [{'type':"data",'legend':"Data",'drawopt':"e0",'legopt':"lep",'drawstack':"FALSE",'resopt':"INC",'drawscale':"NONE",'linecolor':"kBlack",'fillcolor':"0",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"1",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':"%.0f"}]
Styles += [{'type':"data_topq4",'legend':"Data",'drawopt':"e0",'legopt':"lep",'drawstack':"FALSE",'resopt':"INC",'drawscale':"NONE",'linecolor':"kBlack",'fillcolor':"0",'fillstyle':"1001",'linestyle':"1",'linewidth':"2",'markersize':"1",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':"%.0f"}]
Styles += [{'type':"vlq_1000_doublet",'legend':"TTD (1 TeV)",'drawopt':"hist",'legopt':"l",'drawstack':"FALSE",'resopt':"INC",'drawscale':normStyle,'linecolor':"kRed",'fillcolor':"0",'fillstyle':"0",'linestyle':"2",'linewidth':"2",'markersize':"1",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':"%.0f"}]
Styles += [{'type':"vlq_1200_doublet",'legend':"TTD (1.2 TeV)",'drawopt':"hist",'legopt':"l",'drawstack':"FALSE",'resopt':"SKIP",'drawscale':normStyle,'linecolor':"kRed",'fillcolor':"0",'fillstyle':"0",'linestyle':"3",'linewidth':"2",'markersize':"1",'markercolor':"kBlack",'markerstyle':"20",'yieldformat':"%.0f"}]

##________________________________________________________
## Defining the list of variables to look at

Variables = []
if isMCMC: #just some variables that aare reweighted
    Variables += [{'name':"jets_n",'XMIN':"3.5"}]
    Variables += [{'name':"bjets_n"}]
    Variables += [{'name':"meff"}]
    Variables += [{'name':"meff_zoom"}]
    Variables += [{'name':"met"}]
    Variables += [{'name':"met_zoom"}]
    Variables += [{'name':"hthad"}]
    Variables += [{'name':"mtw"}]
    Variables += [{'name':"mtw_zoom"}]
    Variables += [{'name':"mtbmin"}]
    Variables += [{'name':"mtbmin_zoom"}]
else:
    Variables += [{'name':"jets_n",'XMIN':"3.5"}]
    Variables += [{'name':"bjets_n"}]
    Variables += [{'name':"RCjets_pt"}]
    Variables += [{'name':"RCjets_eta"}]
    Variables += [{'name':"RCjets_m"}]

    # Kinematic variables
    Variables += [{'name':"meff"}]
    Variables += [{'name':"meff_zoom"}]
    Variables += [{'name':"met_zoom"}]
    Variables += [{'name':"met"}]
    Variables += [{'name':"hthad"}]
    Variables += [{'name':"mtw_zoom"}]
    Variables += [{'name':"mtw"}]

    # Jet variables
    Variables += [{'name':"jets_pt"}]
    Variables += [{'name':"jets_eta"}]
    Variables += [{'name':"jets_phi"}]
    Variables += [{'name':"jets_btagw"}]
    Variables += [{'name':"jets_jvt"}]
    for counter in range(0,5):
        Variables += [{'name':"jet"+`counter`+"_pt"}]
        Variables += [{'name':"jet"+`counter`+"_eta"}]
        Variables += [{'name':"jet"+`counter`+"_phi"}]
    Variables += [{'name':"bjets_eta"}]
    Variables += [{'name':"bjets_pt"}]
    Variables += [{'name':"bjets_btagw"}]
    Variables += [{'name':"bjets_jvt"}]
    for counter in range(0,4):
        Variables += [{'name':"bjet"+`counter`+"_pt"}]
        Variables += [{'name':"bjet"+`counter`+"_eta"}]

    # Lepton variables
    Variables += [{'name':"mus_pt"}]
    Variables += [{'name':"mus_pt_zoom"}]
    Variables += [{'name':"mus_eta"}]
    Variables += [{'name':"mus_d0sig"}]
    Variables += [{'name':"mus_z0"}]
    #Variables += [{'name':"mus_ptvarcone30"}]
    #Variables += [{'name':"mus_topoetcone20"}]
    #Variables += [{'name':"mus_charge"}]
    Variables += [{'name':"els_pt"}]
    Variables += [{'name':"els_pt_zoom"}]
    Variables += [{'name':"els_eta"}]
    Variables += [{'name':"els_d0sig"}]
    Variables += [{'name':"els_z0"}]
    #Variables += [{'name':"els_ptvarcone20"}]
    #Variables += [{'name':"els_topoetcone20"}]
    #Variables += [{'name':"els_charge"}]

    # Top-tagged jets
    for type in ["RCTTMass","RCMHiggs","RCMTop"]:
        Variables += [{'name':type+"_jets_n"}]
        Variables += [{'name':type+"_jets_pt"}]
        Variables += [{'name':type+"_jets_eta"}]
        Variables += [{'name':type+"_jets_m"}]
        #Variables += [{'name':type+"_jets_consts_n"}]

    # dR objects
    Variables += [{'name':"dR_ejet",'XMAX':"4.5"}]
    Variables += [{'name':"dR_mujet",'XMAX':"4.5"}]
    Variables += [{'name':"dR_jetjet",'XMAX':"4.5"}]
    Variables += [{'name':"dPhi_jetmet"}]
    Variables += [{'name':"mbb_mindR"}]

    # Variables += [{'name':"lep0_pt"}]
    # Variables += [{'name':"lep0_pt_zoom"}]
    # Variables += [{'name':"lep0_eta"}]
    #Variables += [{'name':"lep0_charge"}]

    Variables += [{'name':"mtbmin"}]
    Variables += [{'name':"mtbmin_zoom"}]
##........................................................

##________________________________________________________
## Defining the list of regions to look at
Regions = []
# Regions += [{'name':"c1l2b",'legend':"1l preselection "}]
# Regions += [{'name':"c1l2b_e",'legend':"1l preselection, e"}]
# Regions += [{'name':"c1l2b_mu",'legend':"1l preselection, #mu"}]
# Regions += [{'name':"c0l2b",'legend':"0l preselection"}]
# for nTopTags in ["0","1","2"]:
#    for nJets in ["5","6"]:
#        for nBjets in ["2","3","4"]:
#            channels = ["","_e","_mu"]
#            if isMCMC:
#                channels = ["",]
#            for lepChannel in channels:
#                legend = ""
#                if lepChannel!="":
#                    legend += "#splitline{"
#                if nTopTags=="2":
#                    legend += "#geq"
#                legend += nTopTags + " HOT, "
#                if nJets=="6":
#                    legend += "#geq"
#                legend += nJets + "j, "
#                if nBjets=="4":
#                    legend += "#geq"
#                legend += nBjets + "b"
#                if lepChannel!="":
#                    legend += "}{"
#                    if lepChannel=="_e":
#                        legend += "e channel}"
#                    elif lepChannel=="_mu":
#                        legend += "#mu channel}"
#                Regions += [{'name':"c1l"+nTopTags+"RCTTMass"+nJets+"j"+nBjets+"b"+lepChannel,'legend':legend}]
# for channel in ["","_el","_mu"]:
#     channel_label = ""
#     if channel == "_el":
#         channel_label = ", (e channel)"
#     elif channel == "_mu":
#         channel_label = ", (#mu channel)"
#     for jetmult in ["5jin",]:
#         jet_mult_legend = ""
#         if jetmult.find("in")>-1:
#             jet_mult_legend = "#geq"
#         jet_mult_legend += jetmult.replace("jin","").replace("jex","")
#         jet_mult_legend += "j"
#         Regions += [{'name':"c1lep"+jetmult+"2bin"+channel,'legend':"1l, "+jet_mult_legend+", #geq2b"+channel_label}]
#         #Regions += [{'name':"c1lep"+jetmult+"1bex"+channel,'legend':"1l, "+jet_mult_legend+", 1b"+channel_label}]
#         #Regions += [{'name':"c1lep"+jetmult+"0bex"+channel,'legend':"1l, "+jet_mult_legend+", 0b"+channel_label}]
# Regions += [{'name':"c0lep6jin2bin",'legend':"0l, #geq6j, #geq2b"}]

Regions = []
Regions += [{'name':"c1lep4jin2bin",'legend':"1l, #geq 4j, #geq 2b"}]
Regions += [{'name':"c1lep5jin2bin",'legend':"1l, #geq 4j, #geq 2b"}]
Regions += [{'name':"c0lep6jin2bin",'legend':"0l, #geq 6j, #geq 2b"}]
# Regions = []
# for reg in fit_regions_1l:
#     Regions += [{'name':reg['name'],'legend':reg['legend']}]
FullNameVariables = []
for var in Variables:
    for reg in Regions:
        FullNameVariables += [{'name':reg['name'] + "_" + var['name'],'regionlegend':reg['legend'],'var':var}]

##_____________________________________________________________________________
## Config files for variables
f_vars_list = open("vars_list.list","w")
f_vars_list.write("BEGIN\n")
for var in FullNameVariables:
    f_vars_list.write("NEW\n")
    f_vars_list.write("NAME : "+var['name']+"\n")
    f_vars_list.write("YLABEL : Events\n")
    if isMCMC:
        f_vars_list.write("RESLABEL : TTD (1TeV) / Back.\n")
    else:
        f_vars_list.write("RESLABEL : Data/MC\n")
    f_vars_list.write("DRAWSTACK : TRUE\n")
    f_vars_list.write("DRAWRES : RATIO\n")
    if var['name'].find("cone")>-1:
        f_vars_list.write("ISLOGY : TRUE\n")
    else:
        f_vars_list.write("ISLOGY : FALSE\n")
    f_vars_list.write("REBIN : 0\n")
    if not isMCMC:
        f_vars_list.write("DOSCALE : NORM\n")
    else:
        f_vars_list.write("DOSCALE : "+normStyle+"\n")
    f_vars_list.write("DOWIDTH : FALSE\n")
    f_vars_list.write("DRAWRESERR : REFBAND\n")
    f_vars_list.write("EXTRALABEL : #splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#splitline{#sqrt{s} = 13 TeV, " + lumiLabel + "}{"+var['regionlegend']+"}}\n")
    if 'XMIN' in var['var']:
        f_vars_list.write("XMIN : " + var['var']['XMIN'] + "\n")
    if 'XMAX' in var['var']:
        f_vars_list.write("XMAX : " + var['var']['XMAX'] + "\n")
    f_vars_list.write("\n")
f_vars_list.write("END\n")
f_vars_list.close()

##_____________________________________________________________________________
## Config files for file list (keep the old structure !)
f_files_list = open("files_list.list","w")
f_files_list.write("BEGIN\n")
f_files_list.write("KEY : FILE : SCALE\n")

##_____________________________________________________________________________
## Config files for sample
f_sample_list = open("sample_list.list","w")
f_sample_list.write("BEGIN\n")

##_____________________________________________________________________________
## Config files for style
f_style_list = open("style_list.list","w")
f_style_list.write("BEGIN\n")

##------------------------------------------------------
## Loop over the samples and systematics
##------------------------------------------------------
Commands = []
listfiles = glob.glob(inputDir+"/outVLQAnalysis_*.root")

for sample in Samples:
    SType = sample['type']
    cleaned_sampleType = sample['type']
    name_temp_rootfile = folderRootFiles + "/" + cleaned_sampleType + ".root"
    print "--> Sample: " + SType
    scale = 1
    if(SType.find("qcd")==-1 and SType.find("data")==-1):
        scale = dataLumi

    com = "hadd -n 40 " + name_temp_rootfile
    already_used_files = []
    for subsample in sample['pattern']:
        for f in listfiles:
            if f.find("_"+subsample+"_")>-1:
                com += " " + f
                print "      "+f
                already_used_files += [f]
        for f in already_used_files:
            #print f
            listfiles.remove(f)
        already_used_files = []
    Commands += [com]

    index = ""
    if(len(Commands)<10):
        index += "0"
    index += `len(Commands)`
    for sampleDef in Styles:
        if( sampleDef['type'] == sample['type'] ):
            f_sample_list.write("NEW\n")
            f_sample_list.write("NAME : " + index + "_" + SType + "\n")
            f_sample_list.write("LEGLABEL : " + sampleDef['legend'] + "\n")
            f_sample_list.write("STYLEKEY : " + `len(Commands)` + "\n")
            f_sample_list.write("DRAWOPT : " + sampleDef['drawopt'] + "\n")
            f_sample_list.write("DRAWSTACK : " + sampleDef['drawstack'] + "\n")
            f_sample_list.write("RESOPT : " + sampleDef['resopt'] + "\n")
            f_sample_list.write("DRAWSCALE : " + sampleDef['drawscale'] + "\n")
            f_sample_list.write("LEGOPT : " + sampleDef['legopt'] + "\n")
            f_sample_list.write("YIELDFORMAT : " + sampleDef['yieldformat'] + "\n")
            #f_sample_list.write("WRITE : TRUE\n")
            #f_sample_list.write("OUTFILENAME : " + SType.replace("#","_").replace("{","_").replace("}","_").replace(" ","_") + ".root\n")
            f_sample_list.write("\n")

            f_style_list.write("NEW\n")
            f_style_list.write("KEY : " + `len(Commands)` + "\n")
            f_style_list.write("LINECOLOUR : " + sampleDef['linecolor'] + "\n")
            f_style_list.write("FILLCOLOUR : " + sampleDef['fillcolor'] + "\n")
            f_style_list.write("MARKERCOLOUR : " + sampleDef['markercolor'] + "\n")
            f_style_list.write("LINEWIDTH : " + sampleDef['linewidth'] + "\n")
            f_style_list.write("MARKERSIZE : " + sampleDef['markersize'] + "\n")
            f_style_list.write("LINESTYLE : " + sampleDef['linestyle'] + "\n")
            f_style_list.write("FILLSTYLE : " + sampleDef['fillstyle'] + "\n")
            f_style_list.write("MARKERSTYLE : " + sampleDef['markerstyle'] + "\n")
            f_style_list.write("\n")

            f_files_list.write(index+"_"+SType + " : ")
            f_files_list.write( name_temp_rootfile + " : ")
            f_files_list.write(`scale`+" \n")

#------------------------------------------------------
# Actually adding the rootfiles
#------------------------------------------------------
for Comm in Commands:
    command = Comm
    if not dumpFiles:
        continue
    splittedCommand = Comm.split(" ")
    if len(splittedCommand)==3:
        com = "cp "+splittedCommand[2]+" "+splittedCommand[1]
        os.system(com)
    elif len(splittedCommand)>3:
        os.system(Comm)
    else:
        printError( "Invalid command line: " + Comm )

#------------------------------------------------------
# Defining the sum sample
#------------------------------------------------------
f_sample_list.write("NEW\n")
f_sample_list.write("NAME : SUM\n")
f_sample_list.write("LEGLABEL : Total background\n")
f_sample_list.write("STYLEKEY : 100\n")
f_sample_list.write("DRAWOPT : e2\n")
f_sample_list.write("DRAWSTACK : FALSE\n")
if not isMCMC:
    f_sample_list.write("RESOPT : REF\n")
else:
    f_sample_list.write("RESOPT : REF\n")
f_sample_list.write("DRAWSCALE : " + normStyle + "\n")
f_sample_list.write("LEGOPT : f\n")
f_sample_list.write("YIELDFORMAT : %.2f\n")
f_sample_list.write("\n")

f_style_list.write("NEW\n")
f_style_list.write("KEY : 100\n")
f_style_list.write("LINECOLOUR : kBlack\n")
f_style_list.write("FILLCOLOUR : kBlack\n")
f_style_list.write("MARKERCOLOUR : kBlack\n")
f_style_list.write("LINEWIDTH : 2\n")
f_style_list.write("MARKERSIZE : 0\n")
f_style_list.write("LINESTYLE : 1\n")
f_style_list.write("FILLSTYLE : 3005\n")
f_style_list.write("MARKERSTYLE : 0\n")
f_style_list.write("\n")

f_files_list.write("END\n")
f_sample_list.write("END\n")
f_style_list.write("END\n")
f_files_list.close()
