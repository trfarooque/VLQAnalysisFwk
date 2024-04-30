# Created with MakeRegionsDictionary at 2021-04-11 19:46:47.845114

#
#
# Preselection regions 1-lepton
#
#
reg_1lep5jin2bin = {
    'name':"HTX_c1lep5jin2bin",
    'legend': "#scale[0.75]{#geq5j, #geq2b}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,5000,7000",
    'binning_blind':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000",
    'type':"SIGNAL"
}

reg_1lep5jin3bin = {
    'name':"HTX_c1lep5jin3bin",
    'legend': "#scale[0.75]{#geq5j, #geq3b}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,5000,7000",
    'binning_blind':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000",
    'type':"SIGNAL"
}

reg_1lep5jin3bin3Jin2Min = {
    'name':"HTX_c1lep5jin3bin3Jin2Min",
    'legend': "#scale[0.75]{#geq5j, #geq2b}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,5000,7000",
    'binning_blind':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000",
    'type':"SIGNAL"
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
    'binning_blind':"1000,1300,1700,2300,2850,7000", #1600 singlet 
    'type':"SIGNAL"
}
reg_1lep5jin3bex0Hex1VTex0_1Lwin = {
    'name':"HTX_c1lep5jin3bex0Hex1VTex0_1Lwin",
    'legend': "#scale[0.75]{#geq5j, 3b, 0H, 1(V+t_{h}), 0-1t_{l}}",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'binning':"1000,1150,1350,1650,2100,2550,7000", #1600 HtHt
    'binning_blind':"1000,1150,1350,1650,2100,2550,7000", #1600 HtHt
    'type':"SIGNAL"
}
reg_1lep5jin4bin0Hex1VTex0_1Lwin = {
    'name':"HTX_c1lep5jin4bin0Hex1VTex0_1Lwin",
    'legend': "#scale[0.75]{#geq5j, #geq4b, 0H, 1(V+t_{h}), 0-1t_{l}}",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'binning':"1000,1350,1650,2100,2600,7000", #1600 Doublet
    'binning_blind':"1000,1350,1650,2100,2600,7000", #1600 Doublet
    'type':"SIGNAL"
}
reg_1lep5jin3bex2Min3Jin0HexHighMVAScore = {
    'name':"HTX_c1lep5jin3bex2Min3Jin0HexHighMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, 0H, HMVA}",
    'binning':"1000,2450,3000,7000",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin3bex2Min3Jin0HexMidMVAScore = {
    'name':"HTX_c1lep5jin3bex2Min3Jin0HexMidMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, 0H, MMVA}",
    'binning':"1000,2100,2350,2650,3050,7000", #1600 Singlet
    'binning_blind':"1000,2100,2350", #1600 Singlet
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin3bex2Min3Jin0HexLowMVAScore = {
    'name':"HTX_c1lep5jin3bex2Min3Jin0HexLowMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, 0H, LMVA}",
    'binning':"1000,1400,1650,2000,2300,7000",
    'binning_blind':"1000,1400,1650,2000,2300,7000",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin4bin2Min3Jin0HexHighMVAScore = {
    'name':"HTX_c1lep5jin4bin2Min3Jin0HexHighMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, 0H, HMVA}",
    'binning':"1000,2550,7000",
    #'binning':"\"AutoBin\",\"TransfoD\",2,2",
    'type':"SIGNAL"
}
reg_1lep5jin4bin2Min3Jin0HexMidMVAScore = {
    'name':"HTX_c1lep5jin4bin2Min3Jin0HexMidMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, 0H, MMVA}",
    'binning':"1000,1800,2000,2250,2500,7000",
    'binning_blind':"1000,1800",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin4bin2Min3Jin0HexLowMVAScore = {
    'name':"HTX_c1lep5jin4bin2Min3Jin0HexLowMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, 0H, LMVA}",
    'binning':"1000,1400,1600,1800,2000,2500,7000",
    'binning_blind':"1000,1400,1600,1800,2000,2500",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin3bex2Min3Jin1HinHighMVAScore = {
    'name':"HTX_c1lep5jin3bex2Min3Jin1HinHighMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, #geq1H, HMVA}",
    'binning':"1000,2000,2400,3000,7000",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin3bex2Min3Jin1HinMidMVAScore = {
    'name':"HTX_c1lep5jin3bex2Min3Jin1HinMidMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, #geq1H, MMVA}",
    'binning':"1000,1850,2050,2300,2650,7000",
    'binning_blind':"1000,1850,2050",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin3bex2Min3Jin1HinLowMVAScore = {
    'name':"HTX_c1lep5jin3bex2Min3Jin1HinLowMVAScore",
    'legend':"#scale[0.75]{#geq5j, 3b, #geq2M, #geq3J, #geq1H, LMVA}",
    'binning':"1000,1450,1700,2050,2400,7000",
    'binning_blind':"1000,1450,1700,2050,2400,7000",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin4bin2Min3Jin1HinHighMVAScore = {
    'name':"HTX_c1lep5jin4bin2Min3Jin1HinHighMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, #geq1H, HMVA}",
    'binning':"1000,2400,7000",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin4bin2Min3Jin1HinMidMVAScore = {
    'name':"HTX_c1lep5jin4bin2Min3Jin1HinMidMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, #geq1H, MMVA}",
    'binning':"1000,2050,7000",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_1lep5jin4bin2Min3Jin1HinLowMVAScore = {
    'name':"HTX_c1lep5jin4bin2Min3Jin1HinLowMVAScore",
    'legend':"#scale[0.75]{#geq5j, #geq4b, #geq2M, #geq3J, #geq1H, LMVA}",
    'binning':"1000,1500,1750,7000", #1600 HtHt
    'binning_blind':"1000,1500,1750,7000", #1600 HtHt
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}


#
#                                                                                                                              
# Preselection regions 0-lepton
#
#  
reg_0lep6jin2bin = {
    'name':"HTX_c0lep6jin2bin",
    'legend': "#scale[0.75]{#geq6j, #geq2b}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,5000,7000",
    'binning_blind':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000",
    'type':"SIGNAL"
}

reg_0lep6jin2bex = {
    'name':"HTX_c0lep6jin2bex",
    'legend': "#scale[0.75]{#geq6j, 2b}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,5000,7000",
    'binning_blind':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000",
    'type':"SIGNAL"
}

reg_0lep6jin3bin = {
    'name':"HTX_c0lep6jin3bin",
    'legend': "#scale[0.75]{#geq6j, #geq3b}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,5000,7000",
    'binning_blind':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000",
    'type':"SIGNAL"
}

reg_0lep6jin4bin = {
    'name':"HTX_c0lep6jin4bin",
    'legend': "#scale[0.75]{#geq6j, #geq4b}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,5000,7000",
    'binning_blind':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000",
    'type':"SIGNAL"
}

reg_0lep6jin2bin2Min = {
    'name':"HTX_c0lep6jin2bin2Min",
    'legend': "#scale[0.75]{#geq6j, #geq2b, #geq2M}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,5000,7000",
    'binning_blind':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000",
    'type':"SIGNAL"
}

#
#                                                                                                                              
# Fit regions 0-lepton                                                                                       
#
#  

reg_0lep6jin2bex2MinHighMetCutLowMVAScore = {
    'name':"HTX_c0lep6jin2bex2MinHighMetCutLowMVAScore",
    'legend': "#scale[0.75]{#geq6j, 2b, #geq2M, LMVA, HMET}",
    #'binning': "0,1000,1400,1600,2000,2500,3000,3500,7000",
    'binning': "1000,1500,1700,1900,2200,2500,7000",
    'binning_blind':"1000,1500,1700,1900,2200,2500,7000",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_0lep6jin3bex2MinLowMVAScore = {
    'name':"HTX_c0lep6jin3bex2MinLowMVAScore",
    'legend': "#scale[0.75]{#geq6j, 3b, #geq2M, LMVA}",
    #'binning': "0,1000,1400,1800,2400,3200,7000",
    #'binning': "1000,1300,1600,1800,2200,2500,7000",
    'binning': "1000,1300,1600,1800,2200,7000",
    'binning_blind': "1000,1300,1600,1800,2200,7000",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_0lep6jin4bin2MinLowMVAScore = {
    'name':"HTX_c0lep6jin4bin2MinLowMVAScore",
    'legend': "#scale[0.75]{#geq6j, #geq4b, #geq2M, LMVA}",
    #'binning': "0,1000,1600,2000,3500,7000",
    'binning': "1000,1500,2000,7000",
    'binning_blind': "1000,1500,2000,7000",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_0lep6jin2bex2MinHighMetCutMidMVAScore = {
    'name':"HTX_c0lep6jin2bex2MinHighMetCutMidMVAScore",
    'legend': "#scale[0.75]{#geq6j, 2b, #geq2M, HMET, MMVA}",
    #'binning': "0,1000,1950,2200,2600,3000,7000",
    'binning': "1000,1950,2200,2600,3000,7000",
    'binning_blind': "1000,1950,2200,2600",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_0lep6jin3bin2MinMidMVAScore = {
    'name':"HTX_c0lep6jin3bin2MinMidMVAScore",
    'legend': "#scale[0.75]{#geq6j, #geq3b, #geq2M, MMVA }",
    #'binning': "0,1000,1950,2100,2500,2800,7000",
    'binning': "1000,1600,1900,2100,2500,2800,7000",
    'binning_blind': "1000,1600,1900,2100,2600",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_0lep6jin2bex2MinHighMetCutHighMVAScore = {
    'name':"HTX_c0lep6jin2bex2MinHighMetCutHighMVAScore",
    'legend': "#scale[0.75]{#geq6j, 2b, #geq2M, HMET, HMVA}",
    #'binning': "0,1000,2000,2500,3000,3500,7000",
    #'binning': "1000,2000,2500,3000,3500,7000",
    'binning': "1000,2000,2400,2800,3200,7000",
    'binning_blind': "",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_0lep6jin3bex2MinLowMetCutHighMVAScore = {
    'name':"HTX_c0lep6jin3bex2MinLowMetCutHighMVAScore",
    'legend': "#scale[0.75]{#geq6j, 3b, #geq2M, LMET, HMVA}",
    #'binning': "0,1000,2500,3000,7000",
    'binning': "1000,2000,2800,7000",
    'binning_blind': "",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_0lep6jin3bex2MinHighMetCutHighMVAScore = {
    'name':"HTX_c0lep6jin3bex2MinHighMetCutHighMVAScore",
    'legend': "#scale[0.75]{#geq6j, 3b, #geq2M, HMET, HMVA}",
    #'binning': "0,1000,2200,3000,7000",
    #'binning': "1000,2000,2800,7000",
    'binning': "1000,2000,2500,7000",
    'binning_blind': "",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_0lep6jin4bin2Min0HexHighMVAScore = {
    'name':"HTX_c0lep6jin4bin2Min0HexHighMVAScore",
    'legend': "#scale[0.75]{#geq6j, #geq4b, #geq2M, 0H, HMVA}",
    #'binning': "0,1000,2500,3000,7000",
    'binning': "1000,2000,2800,7000",
    'binning_blind': "",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
    'type':"SIGNAL"
}
reg_0lep6jin4bin2Min1HinHighMVAScore = {
    'name':"HTX_c0lep6jin4bin2Min1HinHighMVAScore",
    'legend': "#scale[0.75]{#geq6j, #geq4b, #geq2M, #geq1H, HMVA}",
    #'binning': "0,1000,2000,2500,3500,7000",
    'binning': "1000,7000",
    'binning_blind': "",
    #'binning':"\"AutoBin\",\"TransfoD\",3,3",
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


preselection_regions_1l = [
    reg_1lep5jin2bin,
    reg_1lep5jin3bin,
    reg_1lep5jin3bin3Jin2Min
]

all_regions_1l =  []
all_regions_1l += fit_regions_1l
all_regions_1l += preselection_regions_1l

fit_regions_0l = [
#reg_0lep6jin2bex2MinLowMetCutLowMVAScore - discarded b/c high-QCD
reg_0lep6jin2bex2MinHighMetCutLowMVAScore,
reg_0lep6jin3bex2MinLowMVAScore,
reg_0lep6jin4bin2MinLowMVAScore,
reg_0lep6jin2bex2MinHighMetCutMidMVAScore,
reg_0lep6jin3bin2MinMidMVAScore,
reg_0lep6jin2bex2MinHighMetCutHighMVAScore,
reg_0lep6jin3bex2MinLowMetCutHighMVAScore,
reg_0lep6jin3bex2MinHighMetCutHighMVAScore,
reg_0lep6jin4bin2Min0HexHighMVAScore,
reg_0lep6jin4bin2Min1HinHighMVAScore
]

preselection_regions_0l = [
reg_0lep6jin2bin,
reg_0lep6jin2bex,
reg_0lep6jin3bin,
reg_0lep6jin4bin,
reg_0lep6jin2bin2Min
]

all_regions_0l = []
all_regions_0l += fit_regions_0l
all_regions_0l += preselection_regions_0l
