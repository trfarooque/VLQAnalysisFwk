#!/bin/python

f = open("/nfs/at3/scratch2/farooque/VLQFITINPUTS/2017_01_09_0LBaselineTest/ConfigFiles_Validation/configFile_0L_NEWBOOST_FIRSTTEST_VLQ_TT_1000_TDoublet_.txt")

region_name = ""
region_legend = ""
region_binning = ""
region_type = ""
region_binning_blind = ""
region_histo = ""
validations = []
control = []
signal = []
for line in f:
    if line.find("Region:")>-1:
        region_name = line.replace("\n","").split(":")[1].replace(" ","")
        region_name = region_name.replace("7j","7jin").replace("6j","6jex").replace("4b","4bin").replace("3b","3bex")
        region_name = region_name.replace("c0l","c0lep").replace("2b","2bex").replace("0lep2Hin","0lep0Tin2Hin")
        region_name = region_name.replace("0_1Hin","0_1Hwin")
        region_name = region_name.replace("_meff","")
        region_name = region_name.replace("\"","")
        region_name = "reg_" + region_name
        print region_name + " = {"
        print "'name':\"" + region_name.replace("reg_","") + "\","
    if region_name == "":
        continue
    if line.find("Type:")>-1:
        region_type = line.replace("\n","").split(":")[1].replace(" ","")
        print "'type':\"" + region_type + "\","
        if region_type.find("VALIDATION")>-1:
            validations += [region_name]
        else:
            control += [region_name]

    if line.find("ShortLabel:")>-1:
        region_legend = line.replace("\n","").split(":")[1].replace(" ","")
        region_legend = region_legend.replace("0l,","0l, ")
        region_legend = region_legend.replace("2Hin,","#geq2H, ")
        region_legend = region_legend.replace("6j,","6j, ")
        region_legend = region_legend.replace("7j,","#geq7j, ")
        region_legend = region_legend.replace("1Tex","1T, ")
        region_legend = region_legend.replace("1Hex,","1H, ")
        region_legend = region_legend.replace("2Tin,","#geq2T, ")
        region_legend = region_legend.replace("2Tin","#geq2T, ")
        region_legend = region_legend.replace("0_1Hin,","0-1H, ")
        region_legend = region_legend.replace("0Hex,","0H, ")
        region_legend = region_legend.replace("0Tex","0T, ")
        region_legend = region_legend.replace("2THin,","#geq2TH, ")
        region_legend = region_legend.replace("4b","#geq4b")
        region_legend = region_legend.replace(",LM",", LM")
        region_legend = region_legend.replace(",HM",", HM")
        print "'legend':"+region_legend+""
        region_name = ""
        print "}"
    if line.find("Binning:")>-1:
        print "'binning':\"" + line.replace("\n","").split(":")[1].replace(" ","") + "\","
        print "'binning_blind':\"\","

print "fit_regions_0l = ["
for reg in control:
    print reg + ","
print "]"

print "validation_regions_0l = ["
for reg in validations:
    print reg + ","
print "]"
