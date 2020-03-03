#!/bin/python
#
# Dictionnary containing all the information about the various regions to be
# considered in the analysis.
# A few lists are also built in order to avoid the manual intervention of the
# user and the duplication of information
#
#
################################################################################
#
# 1L regions
#
################################################################################
#
#
# Fit regions
#
#
reg_1lep0Tex0Hex6jin2bex = {
'name':"HTX_c1lep0Tex0Hex6jin2bex",
'legend':"0T, 0H, #geq6j, 2b",
'binning':"1000,1100,1200,1300,1400,1500,1600,1900,2500,3500",
'binning_low':"1000,1100,1200,1300,1400,1500,1600,2000",
'binning_blind':"1000,1100,1200,1300,1400,1500,1600,1900",
'binning_blind_low':"500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1900",
'binning_5pc_blind':"1000,1100,1200,1300,1400,1500,1600,1900",
'type':"VALIDATION"
}
reg_1lep0Tex0Hex6jin3bex = {
'name':"HTX_c1lep0Tex0Hex6jin3bex",
'legend':"0T, 0H, #geq6j, 3b",
'binning':"1000,1200,1400,1600,1900,2500,3500",
'binning_low':"1000,1200,1400,1600,2000",
'binning_blind':"1000,1200,1400,1600,1900,2500,3500",
'binning_blind_low':"500,600,700,800,900,1000,1200,1400,1600,1900,2500,3500",
'binning_5pc_blind':"1000,1200,1400,1600,1900",
'type':"CONTROL"
}
reg_1lep0Tex0Hex6jin4bin = {
'name':"HTX_c1lep0Tex0Hex6jin4bin",
'legend':"0T, 0H, #geq6j, #geq4b",
'binning':"1000,1100,1300,1600,2000,3500",
'binning_low':"1000,1100,1300,1600,2000",
'binning_blind':"1000,1100,1300,1600,2000",
'binning_blind_low':"500,600,700,800,900,1000,1100,1300,1600,2000",
'binning_5pc_blind':"1000,1100,1300",
'type':"CONTROL"
}

reg_1lep1THex6jin2bex = {
'name':"HTX_c1lep1THex6jin2bex",
'legend':"1TH, #geq6j, 2b",
'binning':"1000,1100,1200,1300,1400,1700,2100,2500,3500",
'binning_low':"1000,1100,1200,1300,1400,1600,2000",
'binning_blind':"1000,1100,1200,1300,1400,1600,1900",
'binning_blind_low':"500,700,800,900,1000,1100,1200,1300,1400,1600,1900",
'binning_5pc_blind':"1000,1100,1200,1300,1400,1600,1900",
'type':"VALIDATION"
}
reg_1lep1THex6jin3bex = {
'name':"HTX_c1lep1THex6jin3bex",
'legend':"1TH, #geq6j, 3b",
'binning':"1000,1100,1300,1700,2100,2500,3500",
'binning_low':"1000,1100,1300,1700,2000",
'binning_blind':"1000,1100,1300,1700,2100,2500",
'binning_blind_low':"500,700,800,900,1000,1100,1300,1700,2100,2500",
'binning_5pc_blind':"1000,1100,1300",
'type':"CONTROL"
}
reg_1lep1THex6jin4bin = {
'name':"HTX_c1lep1THex6jin4bin",
'legend':"1TH, #geq6j, #geq4b",
'binning':"1000,1300,1700,2100,3500",
'binning_low':"1000,1300,1700,2000",
'binning_blind':"1000,1300,1700",
'binning_blind_low':"500,700,800,900,1000,1300,1700",
'binning_5pc_blind':"1000,1300",
'type':"CONTROL"
}

reg_1lep2THin6jin2bex = {
'name':"HTX_c1lep2THin6jin2bex",
'legend':"#geq2TH, #geq6j, 2b",
'binning':"0,1000,1300,2000,2500,3500",
'binning_low':"0,1000,1300,2000",
'binning_blind':"1000,1300,1900",
'binning_blind_low':"500,700,1000,1300,1900",
'binning_5pc_blind':"1000,1300,1900",
'type':"VALIDATION"
}
reg_1lep2THin6jin3bex = {
'name':"HTX_c1lep2THin6jin3bex",
'legend':"#geq2TH, #geq6j, 3b",
'binning':"0,1000,1300,2000,2500,3500",
'binning_low':"0,1000,1300,2000",
'binning_blind':"0,1000,1300,2000",
'binning_blind_low':"0,1000,1300,2000",
'binning_5pc_blind':"0,1000,1300",
'type':"CONTROL"
}
reg_1lep2THin6jin4bin = {
'name':"HTX_c1lep2THin6jin4bin",
'legend':"#geq2TH, #geq6j, #geq4b",
'binning':"0,1300,2000,3500",
'binning_low':"0,1300,2000",
'binning_blind':"0,1300",
'binning_blind_low':"0,1300",
'binning_5pc_blind':"0,1300",
'type':"SIGNAL"
}

fit_regions_1l_supermerge = [
reg_1lep0Tex0Hex6jin2bex,
reg_1lep0Tex0Hex6jin3bex,
reg_1lep0Tex0Hex6jin4bin,
reg_1lep1THex6jin2bex,
reg_1lep1THex6jin3bex,
reg_1lep1THex6jin4bin,
reg_1lep2THin6jin2bex,
reg_1lep2THin6jin3bex,
reg_1lep2THin6jin4bin,
]

#
#
# Validation regions
#
#
reg_1lep0Tex0Hex5jex2bex = {
'name':"HTX_c1lep0Tex0Hex5jex2bex",
'legend':"0T, 0H, 5j, 2b",
'binning':"1000,1100,1200,1300,1400,1500,1600,1900,2500,3500",
'binning_blind':"1000,1100,1200,1300,1400,1500,1600,1900",
'binning_blind_low':"500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1900",
'binning_5pc_blind':"1000,1100,1200,1300,1400,1500,1600,1900",
'type':"VALIDATION"
}
reg_1lep0Tex0Hex5jex3bex = {
'name':"HTX_c1lep0Tex0Hex5jex3bex",
'legend':"0T, 0H, 5j, 3b",
'binning':"1000,1200,1300,1600,2000,2500,3500",
'binning_blind':"1000,1200,1300,1600,2000,2500,3500",
'binning_blind_low':"500,600,700,800,900,1000,1200,1300,1600,2000,2500,3500",
'binning_5pc_blind':"1000,1200,1300",
'type':"VALIDATION"
}
reg_1lep0Tex0Hex5jex4bin = {
'name':"HTX_c1lep0Tex0Hex5jex4bin",
'legend':"0T, 0H, 5j, #geq4b",
'binning':"1000,1300,1600,2000,2500,3500",
'binning_blind':"1000,1300,1600,2000",
'binning_blind_low':"500,600,700,800,900,1000,1300,1600,2000",
'binning_5pc_blind':"1000,1300",
'type':"VALIDATION"
}

reg_1lep1THex5jex2bex = {
'name':"HTX_c1lep1THex5jex2bex",
'legend':"1TH, 5j, 2b",
'binning':"1000,1100,1200,1300,1400,1600,1900,3500",
'binning_blind':"1000,1100,1200,1300,1400,1600,1900,2500",
'binning_blind_low':"500,700,800,900,1000,1100,1200,1300,1400,1600,1900,2500",
'binning_5pc_blind':"1000,1100,1200,1300,1400,1600,1900",
'type':"VALIDATION"
}
reg_1lep1THex5jex3bex = {
'name':"HTX_c1lep1THex5jex3bex",
'legend':"1TH, 5j, 3b",
'binning':"1000,1100,1300,1700,2100,2500,3500",
'binning_blind':"1000,1100,1300,1700,2100,2500",
'binning_blind_low':"500,700,800,900,1000,1100,1300,1700,2100,2500",
'binning_5pc_blind':"1000,1100,1300",
'type':"VALIDATION"
}
reg_1lep1THex5jex4bin = {
'name':"HTX_c1lep1THex5jex4bin",
'legend':"1TH, 5j, #geq4b",
'binning':"1000,1300,1700,2100,2500,3500",
'binning_blind':"1000,1300,1700",
'binning_blind_low':"500,700,800,900,1000,1300,1700",
'binning_5pc_blind':"1000,1300",
'type':"VALIDATION"
}
reg_1lep2THin5jex2bex = {
'name':"HTX_c1lep2THin5jex2bex",
'legend':"#geq2TH, 5j, 2b",
'binning':"1000,1300,2000,2500,3500",
'binning_blind':"1000,1300,2000",
'binning_blind_low':"500,700,1000,1300,2000",
'binning_5pc_blind':"1000,1300,1900",
'type':"VALIDATION"
}
reg_1lep2THin5jex3bex = {
'name':"HTX_c1lep2THin5jex3bex",
'legend':"#geq2TH, 5j, 3b",
'binning':"0,1300,2000,3500",
'binning_blind':"0,1300,2000",
'binning_blind_low':"0,1300,2000",
'binning_5pc_blind':"0,1300",
'type':"VALIDATION"
}
reg_1lep2THin5jex4bin = {
'name':"HTX_c1lep2THin5jex4bin",
'legend':"#geq2TH, 5j, #geq4b",
'binning':"0,1300,2000,3500",
'binning_blind':"0,1300",
'binning_blind_low':"0,1300",
'binning_5pc_blind':"0,1300",
'type':"VALIDATION"
}
validation_regions_1l_supermerge = [
reg_1lep0Tex0Hex5jex2bex,
reg_1lep0Tex0Hex5jex3bex,
reg_1lep0Tex0Hex5jex4bin,
reg_1lep1THex5jex2bex,
reg_1lep1THex5jex3bex,
reg_1lep1THex5jex4bin,
reg_1lep2THin5jex2bex,
reg_1lep2THin5jex3bex,
reg_1lep2THin5jex4bin,
]
all_regions_1l =  []
all_regions_1l += fit_regions_1l_supermerge
all_regions_1l += validation_regions_1l_supermerge
#
#
#
################################################################################
#
# 0L regions
#
################################################################################
#
#
# Fit regions
#
#
reg_c0lep0Tex0Hex7jin2bexHighMtbmin = {
'name':"HTX_c0lep0Tex0Hex7jin2bexHighMtbmin",
'type':"SIGNAL",
'binning':"1000,1200,1400,1600,3500",
'binning_blind':"1000,1200,1400,1600,3500",
'binning_blind_low':"0,700,800,900,1000,1200,1400,1600,3500",
'binning_5pc_blind':"1000,1200,1400",
'legend':"0T, 0H, #geq7j, 2b, HM"
}
reg_c0lep1THex7jin2bexHighMtbmin = {
'name':"HTX_c0lep1THex7jin2bexHighMtbmin",
'type':"SIGNAL",
'binning':"1000,1200,1400,1600,2000,3500",
'binning_blind':"1000,1200,1400,1600,2000",
'binning_blind_low':"0,700,1000,1200,1400,1600,2000",
'binning_5pc_blind':"1000,1200,1400",
'legend':"1TH, #geq7j, 2b, HM"
}
reg_c0lep2THin7jin2bexHighMtbmin = {
'name':"HTX_c0lep2THin7jin2bexHighMtbmin",
'type':"SIGNAL",
'binning':"1000,1400,1600,2000,3500",
'binning_blind':"1000,1400,1600",
'binning_blind_low':"1000,1400,1600",
'binning_5pc_blind':"1000,1400",
'legend':"#geq2TH, #geq7j, 2b, HM"
}
reg_c0lep0Tex0Hex7jin3bex = {
'name':"HTX_c0lep0Tex0Hex7jin3bex",
'type':"SIGNAL",
'binning':"1000,1200,1400,1600,2000,3500",
'binning_blind':"1000,1200,1400,1600,2000,3500",
'binning_blind_low':"0,700,800,900,1000,1200,1400,1600,2000,3500",
'binning_5pc_blind':"1000,1200,1400,1600,1800",
'legend':"0T, 0H, #geq7j, 3b"
}
reg_c0lep1THex7jin3bex = {
'name':"HTX_c0lep1THex7jin3bex",
'type':"SIGNAL",
'binning':"1000,1200,1400,1600,3500",
'binning_blind':"1000,1200,1400,1600,3500",
'binning_blind_low':"700,800,900,1000,1200,1400,1600,3500",
'binning_5pc_blind':"1000,1200,1400,1600",
'legend':"1TH, #geq7j, 3b"
}
reg_c0lep2THin7jin3bex = {
'name':"HTX_c0lep2THin7jin3bex",
'type':"SIGNAL",
'binning':"1000,1200,1400,1600,2000,3500",
'binning_blind':"1000,1200,1400,1600",
'binning_blind_low':"700,1000,1200,1400,1600",
'binning_5pc_blind':"1000,1200,1400,1600",
'legend':"#geq2TH, #geq7j, 3b"
}
reg_c0lep0Tex0Hex7jin4bin = {
'name':"HTX_c0lep0Tex0Hex7jin4bin",
'type':"SIGNAL",
'binning':"1000,1200,1400,1600,3500",
'binning_blind':"1000,1200,1400,1600,3500",
'binning_blind_low':"0,700,800,900,1000,1200,1400,1600,3500",
'binning_5pc_blind':"1000,1200,1400,1600",
'legend':"0T, 0H, #geq7j, #geq4b"
}
reg_c0lep1THex7jin4bin = {
'name':"HTX_c0lep1THex7jin4bin",
'type':"SIGNAL",
'binning':"1000,1200,1400,1600,3500",
'binning_blind':"1000,1200,1400,1600",
'binning_blind_low':"700,1000,1200,1400,1600",
'binning_5pc_blind':"1000,1200,1400",
'legend':"1TH, #geq7j, #geq4b"
}
reg_c0lep2THin7jin4bin = {
'name':"HTX_c0lep2THin7jin4bin",
'type':"SIGNAL",
'binning':"1000,1600,2000,3500",
'binning_blind':"1000,1600",
'binning_blind_low':"1000,1600",
'binning_5pc_blind':"",#BLIND
'legend':"#geq2TH, #geq7j, #geq4b"
}

fit_regions_0l_supermerge = [
reg_c0lep0Tex0Hex7jin2bexHighMtbmin,
reg_c0lep1THex7jin2bexHighMtbmin,
reg_c0lep2THin7jin2bexHighMtbmin,
reg_c0lep0Tex0Hex7jin3bex,
reg_c0lep1THex7jin3bex,
reg_c0lep2THin7jin3bex,
reg_c0lep0Tex0Hex7jin4bin,
reg_c0lep1THex7jin4bin,
reg_c0lep2THin7jin4bin,
]

#####################
reg_c0lep0Tex0Hex6jex2bexHighMtbmin = {
'name':"HTX_c0lep0Tex0Hex6jex2bexHighMtbmin",
'type':"VALIDATION",
'binning':"1000,1200,1400,1600,3500",
'binning_blind':"1000,1200,1400,1600,3500",
'binning_blind_low':"0,700,800,900,1000,1200,1400,1600,3500",
'binning_5pc_blind':"1000,1200,1400,1600",
'legend':"0T, 0H, 6j, 2b, HM"
}
reg_c0lep1THex6jex2bexHighMtbmin = {
'name':"HTX_c0lep1THex6jex2bexHighMtbmin",
'type':"VALIDATION",
'binning':"1000,1200,1400,1600,3500",
'binning_blind':"1000,1200,1400,1600,1800",
'binning_blind_low':"700,800,900,1000,1200,1400,1600,1800",
'binning_5pc_blind':"1000,1200,1400,1600,1800",
'legend':"1TH, 6j, 2b, HM"
}
reg_c0lep2THin6jex2bexHighMtbmin = {
'name':"HTX_c0lep2THin6jex2bexHighMtbmin",
'type':"VALIDATION",
'binning':"1000,1400,3500",
'binning_blind':"1000,1400",
'binning_blind_low':"900,1000,1400",
'binning_5pc_blind':"1000,1400",
'legend':"#geq2TH, 6j, 2b, HM"
}

##
reg_c0lep0Tex0Hex6jex3bex = {
'name':"HTX_c0lep0Tex0Hex6jex3bex",
'type':"VALIDATION",
'binning':"1000,1200,1400,1600,2000,3500",
'binning_blind':"1000,1200,1400,1600,2000",
'binning_blind_low':"0,700,800,900,1000,1200,1400,1600,2000",
'binning_5pc_blind':"1000,1200,1400,1600",
'legend':"0T, 0H, 6j, 3b"
}
reg_c0lep1THex6jex3bex = {
 'name':"HTX_c0lep1THex6jex3bex",
 'type':"VALIDATION",
 'binning':"1000,1200,1400,1600,3500",
 'binning_blind':"1000,1200,1400,1600",
 'binning_5pc_blind':"1000,1200,1400,1600",
 'legend':"1TH, 6j, 3b"
 }
reg_c0lep2THin6jex3bex= {
'name':"HTX_c0lep2THin6jex3bex",
'type':"VALIDATION",
'binning':"1000,3500",
'binning_blind':"1000,3500",
'binning_blind_low':"900,1000,3500",
'binning_5pc_blind':"",#BLIND
'legend':"#geq2TH, 6j, 3b"
}
##
reg_c0lep0Tex0Hex6jex4bin = {
 'name':"HTX_c0lep0Tex0Hex6jex4bin",
 'type':"VALIDATION",
 'binning':"1000,3500",
 'binning_blind':"1000,3500",
 'binning_5pc_blind':"1000,1400",
 'legend':"0T, 0H, 6j, geq4b"
}
reg_c0lep1THex6jex4bin = {
'name':"HTX_c0lep1THex6jex4bin",
'type':"VALIDATION",
'binning':"1000,1200,3500",
'binning_blind':"1000,1200,3500",
'binning_blind_low':"0,900,1000,1200,3500",
'binning_5pc_blind':"1000,1200",
'legend':"1TH, 6j, #geq4b"
}
reg_c0lep2THin6jex4bin = {
'name':"HTX_c0lep2THin6jex4bin",
'type':"VALIDATION",
'binning':"1000,3500",
'binning_blind':"",#BLIND
'binning_blind_low':"",#BLIND
'binning_5pc_blind':"",#BLIND
'legend':"#geq2TH, 6j, #geq4b"
}

validation_regions_0l_supermerge = [
reg_c0lep0Tex0Hex6jex2bexHighMtbmin,
reg_c0lep1THex6jex2bexHighMtbmin,
reg_c0lep2THin6jex2bexHighMtbmin,
reg_c0lep0Tex0Hex6jex3bex,
reg_c0lep1THex6jex3bex,
reg_c0lep2THin6jex3bex,
reg_c0lep0Tex0Hex6jex4bin,
reg_c0lep1THex6jex4bin,
reg_c0lep2THin6jex4bin,
]

all_regions_0l = []
all_regions_0l += fit_regions_0l_supermerge
all_regions_0l += validation_regions_0l_supermerge
