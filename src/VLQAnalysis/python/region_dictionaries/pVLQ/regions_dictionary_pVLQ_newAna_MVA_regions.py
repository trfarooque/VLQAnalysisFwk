# Created with MakeRegionsDictionary at 2021-04-11 19:46:47.845114

#
#
# Preselection regions 1-lepton
#
#
reg_1lep6jin3bin2Min3Jin = {
    'name':"HTX_c1lep6jin3bin2Min3Jin"
}
reg_1lep5jin3bin2Min3Jin = {
    'name':"HTX_c1lep5jin3bin2Min3Jin"
}

#
#
# Fit regions 1-lepton
#
#
reg_1lep5jin2bex0Hex1VTex1Lex = {
    'name':"HTX_c1lep5jin2bex0Hex1VTex1Lex",
    'legend': "#scale[0.75]{#geq5j, 2b, 0H, 1(V+t_{h}), 1t_{l}}",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'binning':"1000,1300,1700,2300,2850,7000", #1600 singlet 
    'type':"SIGNAL"
}
reg_1lep5jin3bex0Hex1VTex0_1Lwin = {
    'name':"HTX_c1lep5jin3bex0Hex1VTex0_1Lwin",
    'legend': "#scale[0.75]{#geq5j, 3b, 0H, 1(V+t_{h}), 0-1t_{l}}",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'binning':"1000,1150,1350,1650,2100,2550,7000", #1600 HtHt
    'type':"SIGNAL"
}
reg_1lep5jin4bin0Hex1VTex0_1Lwin = {
    'name':"HTX_c1lep5jin4bin0Hex1VTex0_1Lwin",
    'legend': "#scale[0.75]{#geq5j, #geq4b, 0H, 1(V+t_{h}), 0-1t_{l}}",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'binning':"1000,1350,1650,2100,2600,7000", #1600 Doublet
    'type':"SIGNAL"
}
reg_1lep5jin3bex2Min3Jin0HexHighMVAScore = {
    'name':"HTX_c1lep5jin3bex2Min3Jin0HexHighMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, 0H, HMVA}",
    #"binning":"1000,2100,2450,2700,3000,3400,7000",
    'binning':"1000,2100,2450,2700,3000,3400,7000", #1600 Doublet
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin3bex2Min3Jin0HexMidMVAScore = {
    'name':"HTX_c1lep5jin3bex2Min3Jin0HexMidMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, 0H, MMVA}",
    #'binning':"1000,2100,2350,2650,3050,7000",
    'binning':"1000,2100,2350,2650,3050,7000", #1600 Singlet
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin3bex2Min3Jin0HexLowMVAScore = {
    'name':"HTX_c1lep5jin3bex2Min3Jin0HexLowMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, 0H, LMVA}",
    #'binning':"1000,1400,1600,1800,2000,2300,7000",
    'binning':"1000,1400,1650,2000,2300,2800,7000", #1600 HtHt
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin4bin2Min3Jin0HexHighMVAScore = {
    'name':"HTX_c1lep5jin4bin2Min3Jin0HexHighMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, 0H, HMVA}",
    #'binning':"1000,2150,2450,2700,3000,3400,7000",
    'binning':"1000,2150,2450,2700,3000,3400,7000", #1600 Singlet
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin4bin2Min3Jin0HexMidMVAScore = {
    'name':"HTX_c1lep5jin4bin2Min3Jin0HexMidMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, 0H, MMVA}",
    #'binning':"1000,1800,2000,2250,2950,7000",
    'binning':"1000,1800,2000,2250,2500,2900,7000", # 1600 HtHt
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin4bin2Min3Jin0HexLowMVAScore = {
    'name':"HTX_c1lep5jin4bin2Min3Jin0HexLowMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, 0H, LMVA}",
    #'binning':"1000,1400,1600,1800,2050,2400,7000",
    'binning':"1000,1400,1600,1850,2250,2700,7000", # 1600 HtHt
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin3bex2Min3Jin1HinHighMVAScore = {
    'name':"HTX_c1lep5jin3bex2Min3Jin1HinHighMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, #geq1H, HMVA}",
    #'binning':"1000,2200,2400,2650,2900,3300,7000",
    'binning':"1000,2200,2500,2800,3450,7000", #1600 Doublet
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin3bex2Min3Jin1HinMidMVAScore = {
    'name':"HTX_c1lep5jin3bex2Min3Jin1HinMidMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, #geq1H, MMVA}",
    #'binning':"1000,1850,2050,2250,2500,2800,7000",
    'binning':"1000,1850,2050,2300,2650,3050,7000", #1600 Singlet
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin3bex2Min3Jin1HinLowMVAScore = {
    'name':"HTX_c1lep5jin3bex2Min3Jin1HinLowMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, #geq1H, LMVA}",
    #'binning':"1000,1450,1650,1850,2100,2400,7000",
    'binning':"1000,1450,1700,2050,2400,2850,7000", #1600 HtHt
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin4bin2Min3Jin1HinHighMVAScore = {
    'name':"HTX_c1lep5jin4bin2Min3Jin1HinHighMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, #geq1H, HMVA}",
    #'binning':"1000,2150,2450,2750,3050,3450,7000",
    'binning':"1000,2100,2400,2700,3000,3400,7000", #1600 HtHt
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin4bin2Min3Jin1HinMidMVAScore = {
    'name':"HTX_c1lep5jin4bin2Min3Jin1HinMidMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, #geq1H, MMVA}",
    #'binning':"1000,1850,2050,2350,2700,3100,7000",
    'binning':"1000,1850,2050,2350,2700,3100,7000", #1600 Doublet
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin4bin2Min3Jin1HinLowMVAScore = {
    'name':"HTX_c1lep5jin4bin2Min3Jin1HinLowMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, #geq1H, LMVA}",
    #'binning':"1000,1450,1650,1900,2150,2450,7000",
    'binning':"1000,1500,1750,2100,2450,2900,7000", #1600 HtHt
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
#
#
# Fit regions MVA 1-lepton decorrelation
#
#
reg_1lep5jex2bex0Hex1VTex1Lex = {
    'name':"HTX_c1lep5jex2bex0Hex1VTex1Lex",
    'legend':"#scale[0.75]{5j, 2b, 0H, 1(V+t_{h}), 1t_{l}}",
    'binning':"1000,1300,1700,2300,2850,7000",
    'type':"SIGNAL"
}
reg_1lep6jin2bex0Hex1VTex1Lex = {
    'name':"HTX_c1lep6jin2bex0Hex1VTex1Lex",
    'legend':"#scale[0.75]{#geq6j, 2b, 0H, 1(V+t_{h}), 1t_{l}}",
    'binning':"1000,1300,1700,2300,2850,7000",
    'type':"SIGNAL"
}
reg_1lep5jex3bex0Hex1VTex0_1Lwin = {
    'name':"HTX_c1lep5jex3bex0Hex1VTex0_1Lwin",
    'legend':"#scale[0.75]{5j, 3b, 0H, 1(V+t_{h}), 0-1t_{l}}",
    'binning':"1000,1150,1350,1650,2100,2550,7000",
    'type':"SIGNAL"
}
reg_1lep6jin3bex0Hex1VTex0_1Lwin = {
    'name':"HTX_c1lep6jin3bex0Hex1VTex0_1Lwin",
    'legend':"#scale[0.75]{#geq6j, 3b, 0H, 1(V+t_{h}), 0-1t_{l}}",
    'binning':"1000,1150,1350,1650,2100,2550,7000",
    'type':"SIGNAL"
}

reg_1lep5jex4bin0Hex1VTex0_1Lwin = {
    'name':"HTX_c1lep5jex4bin0Hex1VTex0_1Lwin",
    'legend':"#scale[0.75]{5j, #geq4b, 0H, 1(V+t_{h}), 0-1t_{l}}",
    'binning':"1000,1350,1650,2100,2600,7000",
    'type':"SIGNAL"
}
reg_1lep6jin4bin0Hex1VTex0_1Lwin = {
    'name':"HTX_c1lep6jin4bin0Hex1VTex0_1Lwin",
    'legend':"#scale[0.75]{#geq6j, #geq4b, 0H, 1(V+t_{h}), 0-1t_{l}}",
    'binning':"1000,1350,1650,2100,2600,7000",
    'type':"SIGNAL"
}
#
#
# Validation regions MVA 1-lepton
#
#
reg_1lep5jin3bex2Min3Jin0HexHighMVAScore_VR = {
    'name':"HTX_c1lep5jin3bex2Min3Jin0HexHighMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, 0H, HMVA}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"VALIDATION"
}
reg_1lep5jin3bex2Min3Jin0HexMidMVAScore_VR = {
    'name':"HTX_c1lep5jin3bex2Min3Jin0HexMidMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, 0H, MMVA}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"VALIDATION"
}
reg_1lep5jin3bex2Min3Jin0HexLowMVAScore_VR = {
    'name':"HTX_c1lep5jin3bex2Min3Jin0HexLowMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, 0H, LMVA}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"VALIDATION"
}
reg_1lep5jin4bin2Min3Jin0HexHighMVAScore_VR = {
    'name':"HTX_c1lep5jin4bin2Min3Jin0HexHighMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, 0H, HMVA}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"VALIDATION"
}
reg_1lep5jin4bin2Min3Jin0HexMidMVAScore_VR = {
    'name':"HTX_c1lep5jin4bin2Min3Jin0HexMidMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, 0H, MMVA}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"VALIDATION"
}
reg_1lep5jin4bin2Min3Jin0HexLowMVAScore_VR = {
    'name':"HTX_c1lep5jin4bin2Min3Jin0HexLowMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, 0H, LMVA}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"VALIDATION"
}
reg_1lep5jin3bex2Min3Jin1HinHighMVAScore_VR = {
    'name':"HTX_c1lep5jin3bex2Min3Jin1HinHighMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, #geq1H, HMVA}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"VALIDATION"
}
reg_1lep5jin3bex2Min3Jin1HinMidMVAScore_VR = {
    'name':"HTX_c1lep5jin3bex2Min3Jin1HinMidMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, #geq1H, MMVA}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"VALIDATION"
}
reg_1lep5jin3bex2Min3Jin1HinLowMVAScore_VR = {
    'name':"HTX_c1lep5jin3bex2Min3Jin1HinLowMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, #geq1H, LMVA}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"VALIDATION"
}
reg_1lep5jin4bin2Min3Jin1HinHighMVAScore_VR = {
    'name':"HTX_c1lep5jin4bin2Min3Jin1HinHighMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, #geq1H, HMVA}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"VALIDATION"
}
reg_1lep5jin4bin2Min3Jin1HinMidMVAScore_VR = {
    'name':"HTX_c1lep5jin4bin2Min3Jin1HinMidMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, #geq1H, MMVA}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"VALIDATION"
}
reg_1lep5jin4bin2Min3Jin1HinLowMVAScore_VR = {
    'name':"HTX_c1lep5jin4bin2Min3Jin1HinLowMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, #geq1H, LMVA}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"VALIDATION"
}
#
#                                                                                                                              
# Fit regions 0-lepton                                                                                                         
#
#  
reg_0lep7jin2bex0Hex1VTexHighMtbmin = {
    'name':"HTX_c0lep7jin2bex0Hex1VTexHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, 0H, 1(V+t_{h}), HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin2bex0Hex1Vex1TinHighMtbmin = {
    'name':"HTX_c0lep7jin2bex0Hex1Vex1TinHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, 0H, 1V, #geq1t_{h}, HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin2bex0Hex0Vex2TinHighMtbmin = {
    'name':"HTX_c0lep7jin2bex0Hex0Vex2TinHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, 0H, 0V, #geq2t_{h}, HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin2bex0Hex2Vin0TinHighMtbmin = {
    'name':"HTX_c0lep7jin2bex0Hex2Vin0TinHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, 0H, #geq2V, #geq0t_{h}, HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bin0Hex1VTex = {
    'name':"HTX_c0lep7jin3bin0Hex1VTex",
    'legend':"#scale[0.75]{#geq7j, #geq3b, 0H, 1(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bin0Hex1Vex1Tin = {
    'name':"HTX_c0lep7jin3bin0Hex1Vex1Tin",
    'legend':"#scale[0.75]{#geq7j, #geq3b, 0H, 1V, #geq1t_{h}}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bin0Hex0Vex2Tin = {
    'name':"HTX_c0lep7jin3bin0Hex0Vex2Tin",
    'legend':"#scale[0.75]{#geq7j, #geq3b, 0H, 0V, #geq2t_{h}}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bin0Hex2Vin0Tin = {
    'name':"HTX_c0lep7jin3bin0Hex2Vin0Tin",
    'legend':"#scale[0.75]{#geq7j, #geq3b, 0H, #geq2V, #geq0t_{h}}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin2bex1Hin0VTexHighMtbmin = {
    'name':"HTX_c0lep7jin2bex1Hin0VTexHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, #geq1H, 0(V+t_{h}), HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin2bex1Hin1VTexHighMtbmin = {
    'name':"HTX_c0lep7jin2bex1Hin1VTexHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, #geq1H, 1(V+t_{h}), HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin2bex1Hin2VTinHighMtbmin = {
    'name':"HTX_c0lep7jin2bex1Hin2VTinHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, #geq1H, #geq2(V+t_{h}), HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bex1Hex0VTex = {
    'name':"HTX_c0lep7jin3bex1Hex0VTex",
    'legend':"#scale[0.75]{#geq7j, 2b, 1H, 0(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bex1Hex1VTex = {
    'name':"HTX_c0lep7jin3bex1Hex1VTex",
    'legend':"#scale[0.75]{#geq7j, 2b, 1H, 1(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bex1Hex2VTin = {
    'name':"HTX_c0lep7jin3bex1Hex2VTin",
    'legend':"#scale[0.75]{#geq7j, 2b, 1H, #geq2(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin4bin1Hex0VTex = {
    'name':"HTX_c0lep7jin4bin1Hex0VTex",
    'legend':"#scale[0.75]{#geq7j, #geq4b, 1H, 0(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin4bin1Hex1VTex = {
    'name':"HTX_c0lep7jin4bin1Hex1VTex",
    'legend':"#scale[0.75]{#geq7j, #geq4b, 1H, 1(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin4bin1Hex2VTin = {
    'name':"HTX_c0lep7jin4bin1Hex2VTin",
    'legend':"#scale[0.75]{#geq7j, #geq4b, 1H, #geq2(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bin2Hin0Vin0Tin = {
    'name':"HTX_c0lep7jin3bin2Hin0Vin0Tin",
    'legend':"#scale[0.75]{#geq7j, #geq3b, #geq2H, #geq0V, #geq0t_{h}}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}

#
#                                                                                                                              
# Fit regions 0-lepton                                                                                                         
#
#  
reg_0lep7jin2bex0Hex1VTexHighMtbmin = {
    'name':"HTX_c0lep7jin2bex0Hex1VTexHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, 0H, 1(V+t_{h}), HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin2bex0Hex1Vex1TinHighMtbmin = {
    'name':"HTX_c0lep7jin2bex0Hex1Vex1TinHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, 0H, 1V, #geq1t_{h}, HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin2bex0Hex0Vex2TinHighMtbmin = {
    'name':"HTX_c0lep7jin2bex0Hex0Vex2TinHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, 0H, 0V, #geq2t_{h}, HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin2bex0Hex2Vin0TinHighMtbmin = {
    'name':"HTX_c0lep7jin2bex0Hex2Vin0TinHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, 0H, #geq2V, #geq0t_{h}, HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bin0Hex1VTex = {
    'name':"HTX_c0lep7jin3bin0Hex1VTex",
    'legend':"#scale[0.75]{#geq7j, #geq3b, 0H, 1(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bin0Hex1Vex1Tin = {
    'name':"HTX_c0lep7jin3bin0Hex1Vex1Tin",
    'legend':"#scale[0.75]{#geq7j, #geq3b, 0H, 1V, #geq1t_{h}}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bin0Hex0Vex2Tin = {
    'name':"HTX_c0lep7jin3bin0Hex0Vex2Tin",
    'legend':"#scale[0.75]{#geq7j, #geq3b, 0H, 0V, #geq2t_{h}}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bin0Hex2Vin0Tin = {
    'name':"HTX_c0lep7jin3bin0Hex2Vin0Tin",
    'legend':"#scale[0.75]{#geq7j, #geq3b, 0H, #geq2V, #geq0t_{h}}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin2bex1Hin0VTexHighMtbmin = {
    'name':"HTX_c0lep7jin2bex1Hin0VTexHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, #geq1H, 0(V+t_{h}), HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin2bex1Hin1VTexHighMtbmin = {
    'name':"HTX_c0lep7jin2bex1Hin1VTexHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, #geq1H, 1(V+t_{h}), HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin2bex1Hin2VTinHighMtbmin = {
    'name':"HTX_c0lep7jin2bex1Hin2VTinHighMtbmin",
    'legend':"#scale[0.75]{#geq7j, 2b, #geq1H, #geq2(V+t_{h}), HM}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bex1Hex0VTex = {
    'name':"HTX_c0lep7jin3bex1Hex0VTex",
    'legend':"#scale[0.75]{#geq7j, 2b, 1H, 0(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bex1Hex1VTex = {
    'name':"HTX_c0lep7jin3bex1Hex1VTex",
    'legend':"#scale[0.75]{#geq7j, 2b, 1H, 1(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bex1Hex2VTin = {
    'name':"HTX_c0lep7jin3bex1Hex2VTin",
    'legend':"#scale[0.75]{#geq7j, 2b, 1H, #geq2(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin4bin1Hex0VTex = {
    'name':"HTX_c0lep7jin4bin1Hex0VTex",
    'legend':"#scale[0.75]{#geq7j, #geq4b, 1H, 0(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin4bin1Hex1VTex = {
    'name':"HTX_c0lep7jin4bin1Hex1VTex",
    'legend':"#scale[0.75]{#geq7j, #geq4b, 1H, 1(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin4bin1Hex2VTin = {
    'name':"HTX_c0lep7jin4bin1Hex2VTin",
    'legend':"#scale[0.75]{#geq7j, #geq4b, 1H, #geq2(V+t_{h})}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}
reg_0lep7jin3bin2Hin0Vin0Tin = {
    'name':"HTX_c0lep7jin3bin2Hin0Vin0Tin",
    'legend':"#scale[0.75]{#geq7j, #geq3b, #geq2H, #geq0V, #geq0t_{h}}",
    'binning':"0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.05",
    'type':"SIGNAL"
}

fit_regions_1l = [
reg_1lep5jin2bex0Hex1VTex1Lex,
reg_1lep5jin3bex0Hex1VTex0_1Lwin,
reg_1lep5jin4bin0Hex1VTex0_1Lwin,
reg_1lep5jin3bex2Min3Jin0HexHighMVAScore,
reg_1lep5jin3bex2Min3Jin0HexMidMVAScore,
reg_1lep5jin3bex2Min3Jin0HexLowMVAScore,
reg_1lep5jin4bin2Min3Jin0HexHighMVAScore,
reg_1lep5jin4bin2Min3Jin0HexMidMVAScore,
reg_1lep5jin4bin2Min3Jin0HexLowMVAScore,
reg_1lep5jin3bex2Min3Jin1HinHighMVAScore,
reg_1lep5jin3bex2Min3Jin1HinMidMVAScore,
reg_1lep5jin3bex2Min3Jin1HinLowMVAScore,
reg_1lep5jin4bin2Min3Jin1HinHighMVAScore,
reg_1lep5jin4bin2Min3Jin1HinMidMVAScore,
reg_1lep5jin4bin2Min3Jin1HinLowMVAScore
]

validation_regions_1l = [
reg_1lep5jin3bex2Min3Jin0HexHighMVAScore_VR,
reg_1lep5jin3bex2Min3Jin0HexMidMVAScore_VR,
reg_1lep5jin3bex2Min3Jin0HexLowMVAScore_VR,
reg_1lep5jin4bin2Min3Jin0HexHighMVAScore_VR,
reg_1lep5jin4bin2Min3Jin0HexMidMVAScore_VR,
reg_1lep5jin4bin2Min3Jin0HexLowMVAScore_VR,
reg_1lep5jin3bex2Min3Jin1HinHighMVAScore_VR,
reg_1lep5jin3bex2Min3Jin1HinMidMVAScore_VR,
reg_1lep5jin3bex2Min3Jin1HinLowMVAScore_VR,
reg_1lep5jin4bin2Min3Jin1HinHighMVAScore_VR,
reg_1lep5jin4bin2Min3Jin1HinMidMVAScore_VR,
reg_1lep5jin4bin2Min3Jin1HinLowMVAScore_VR
]

preselection_regions_1l = [
    reg_1lep6jin3bin2Min3Jin,
    reg_1lep5jin3bin2Min3Jin
]

all_regions_1l =  []
all_regions_1l += fit_regions_1l
all_regions_1l += validation_regions_1l
all_regions_1l += preselection_regions_1l

fit_regions_0l = []

validation_regions_0l = []

preselection_regions_0l = []

all_regions_0l = []
all_regions_0l += fit_regions_0l
all_regions_0l += validation_regions_0l
all_regions_0l += preselection_regions_0l
