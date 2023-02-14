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
reg_1lep3_5jwin1_2bwin1fjin0Tex0Hex1Vin = {
    'name':"HTX_c1lep3_5jwin1_2bwin1fjin0Tex0Hex1Vin",
    'legend': "#splitline{1l, 3-5j, 1-2b, #geq1fj}{0h, #geq1v, 0t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"RESERVE"
}
reg_1lep3_5jwin1_2bwin1fjin0Tex1LTin0Hex1Vin = {
    'name':"HTX_c1lep3_5jwin1_2bwin1fjin0Tex1LTin0Hex1Vin",
    'legend': "#splitline{1l, 3-5j, 1-2b, #geq1fj}{0h, #geq1v, 0t, #geq1lt}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"SIGNAL"
}
reg_1lep3_5jwin3bin1fjin0Tex1LTin1Hin0Vex = {
    'name':"HTX_c1lep3_5jwin3bin1fjin0Tex1LTin1Hin0Vex",
    'legend': "#splitline{1l, 3-5j, #geq3b, #geq1fj}{#geq1h, 0v, 0t, #geq1lt}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"SIGNAL"
}
reg_1lep6jin2bex1fjin2VTin0Hex = {
    'name':"HTX_c1lep6jin2bex1fjin2VTin0Hex",
    'legend': "#splitline{1l, #geq6j, 2b, #geq1fj}{0h, #geq2v-t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"RESERVE"
}
reg_1lep6jin2bex1fjin1LTTin0Hex1Vin = {
    'name':"HTX_c1lep6jin2bex1fjin1LTTin0Hex1Vin",
    'legend': "#splitline{1l, #geq6j, 2b, #geq1fj}{0h, #geq1v, #geq1lt-t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"SIGNAL"
}
reg_1lep6jin3bin1fjin1VTin1Hin = {
    'name':"HTX_c1lep6jin3bin1fjin1VTin1Hin",
    'legend': "#splitline{1l, #geq6j, #geq3b, #geq1fj}{#geq1h, #geq1v-t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"SIGNAL"
}
reg_1lep6jin3bin1fjin0Tex1LTin1Hin0Vex = {
    'name':"HTX_c1lep6jin3bin1fjin0Tex1LTin1Hin0Vex",
    'legend': "#splitline{1l, #geq6j, #geq3b, #geq1fj}{#geq1h, 0v, 0t, #geq1lt}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"RESERVE"
}
reg_1lep6jin3bin1fjin1VLTTin1Hin = {
    'name':"HTX_c1lep6jin3bin1fjin1VLTTin1Hin",
    'legend': "#splitline{1l, #geq6j, #geq3b, #geq1fj}{#geq1h, #geqVlt-t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"RESERVE"
}
reg_1lep3_5jwin1_2bwin0fjex0Tex0Hex1Vin = {
    'name':"HTX_c1lep3_5jwin1_2bwin0fjex0Tex0Hex1Vin",
    'legend': "#splitline{1l, 3-5j, 1-2b, 0fj}{0h, #geq1v, 0t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep3_5jwin1_2bwin1fjin1Tin0Hex0Vex = {
    'name':"HTX_c1lep3_5jwin1_2bwin1fjin1Tin0Hex0Vex",
    'legend': "#splitline{1l, 3-5j, 1-2b, #geq1fj}{0h, 0v, #geq1t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep3_5jwin1_2bwin0fjex0Tex1LTin0Hex1Vin = {
    'name':"HTX_c1lep3_5jwin1_2bwin0fjex0Tex1LTin0Hex1Vin",
    'legend': "#splitline{1l, 3-5j, 1-2b, 0fj}{0h, #geq1v, 0t, #geq1lt}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep3_5jwin1_2bwin1fjin1Tin0LTex0Hex1Vin = {
    'name':"HTX_c1lep3_5jwin1_2bwin1fjin1Tin0LTex0Hex1Vin",
    'legend': "#splitline{1l, 3-5j, 1-2b, #geq1fj}{0h, #geq1v, #geq1t, #geq0lt}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep3_5jwin3bin0fjex0Tex1LTin1Hin0Vex = {
    'name':"HTX_c1lep3_5jwin3bin0fjex0Tex1LTin1Hin0Vex",
    'legend': "#splitline{1l, 3-5j, #geq3b, 0fj}{#geq1h, 0v, 0t, #geq1lt}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep3_5jwin3bin1fjin1Tin0LTex0Hex0Vex = {
    'name':"HTX_c1lep3_5jwin3bin1fjin1Tin0LTex0Hex0Vex",
    'legend': "#splitline{1l, 3-5j, #geq3b, #geq1fj}{0h, 0v, #geq1t, #geq0lt}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep6jin2bex0fjex2VTin0Hex = {
    'name':"HTX_c1lep6jin2bex0fjex2VTin0Hex",
    'legend': "#splitline{1l, #geq6j, 2b, 0fj}{0h, #geq2v-t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep6jin2bex1fjin1Tex0Hex0Vex = {
    'name':"HTX_c1lep6jin2bex1fjin1Tex0Hex0Vex",
    'legend': "#splitline{1l, #geq6j, 2b, #geq1fj}{0h, 0v, 1t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep6jin2bex0fjex1LTTin0Hex1Vin = {
    'name':"HTX_c1lep6jin2bex0fjex1LTTin0Hex1Vin",
    'legend': "#splitline{1l, #geq6j, 2b, 0fj}{0h, #geq1v, #geq1lt-t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep6jin2bex1fjin1LTTin0Hex0Vex = {
    'name':"HTX_c1lep6jin2bex1fjin1LTTin0Hex0Vex",
    'legend': "#splitline{1l, #geq6j, 2b, #geq1fj}{0h, 0v, #geq1lt-t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep6jin3bin0fjex1VTin1Hin = {
    'name':"HTX_c1lep6jin3bin0fjex1VTin1Hin",
    'legend': "#splitline{1l, #geq6j, #geq3b, 0fj}{#geq1h, #geq1v-t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep6jin3bin1fjin1VTin0Hex = {
    'name':"HTX_c1lep6jin3bin1fjin1VTin0Hex",
    'legend': "#splitline{1l, #geq6j, #geq3b, #geq1fj}{0h, #geq1v-t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep6jin3bin0fjex0Tex1LTin1Hin0Vex = {
    'name':"HTX_c1lep6jin3bin0fjex0Tex1LTin1Hin0Vex",
    'legend': "#splitline{1l, #geq6j, #geq3b, 0fj}{#geq1h, 0v, 0t, #geq1lt}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep6jin3bin1fjin0Tex1LTin0Hex0Vex = {
    'name':"HTX_c1lep6jin3bin1fjin0Tex1LTin0Hex0Vex",
    'legend': "#splitline{1l, #geq6j, #geq3b, #geq1fj}{0h, 0v, 0t, #geq1lt}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep6jin3bin0fjex1VLTTin1Hin = {
    'name':"HTX_c1lep6jin3bin0fjex1VLTTin1Hin",
    'legend': "#splitline{1l, #geq6j, #geq3b, 0fj}{#geq1h, #geqVlt-t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
reg_1lep6jin3bin1fjin1VLTTin0Hex = {
    'name':"HTX_c1lep6jin3bin1fjin1VLTTin0Hex",
    'legend': "#splitline{1l, #geq6j, #geq3b, #geq1fj}{#geqVlt-t}",
    'binning':"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,4500,5000",
    'type':"VALIDATION"
}
fit_regions_1l = [
# reg_1lep3_5jwin1_2bwin1fjin0Tex0Hex1Vin,
reg_1lep3_5jwin1_2bwin1fjin0Tex1LTin0Hex1Vin,
reg_1lep3_5jwin3bin1fjin0Tex1LTin1Hin0Vex,
# reg_1lep6jin2bex1fjin2VTin0Hex,
reg_1lep6jin2bex1fjin1LTTin0Hex1Vin,
reg_1lep6jin3bin1fjin1VTin1Hin
# reg_1lep6jin3bin1fjin0Tex1LTin1Hin0Vex
# reg_1lep6jin3bin1fjin1VLTTin1Hin
]

validation_regions_1l = [
# reg_1lep3_5jwin1_2bwin0fjex0Tex0Hex1Vin,
# reg_1lep3_5jwin1_2bwin1fjin1Tin0Hex0Vex,
reg_1lep3_5jwin1_2bwin0fjex0Tex1LTin0Hex1Vin,
reg_1lep3_5jwin1_2bwin1fjin1Tin0LTex0Hex1Vin,
reg_1lep3_5jwin3bin0fjex0Tex1LTin1Hin0Vex,
reg_1lep3_5jwin3bin1fjin1Tin0LTex0Hex0Vex,
# reg_1lep6jin2bex0fjex2VTin0Hex,
# reg_1lep6jin2bex1fjin1Tex0Hex0Vex,
reg_1lep6jin2bex0fjex1LTTin0Hex1Vin,
reg_1lep6jin2bex1fjin1LTTin0Hex0Vex,
reg_1lep6jin3bin0fjex1VTin1Hin,
reg_1lep6jin3bin1fjin1VTin0Hex,
# reg_1lep6jin3bin0fjex0Tex1LTin1Hin0Vex,
# reg_1lep6jin3bin1fjin0Tex1LTin0Hex0Vex
# reg_1lep6jin3bin0fjex1VLTTin1Hin,
# reg_1lep6jin3bin1fjin1VLTTin0Hex
]

all_regions_1l =  []
all_regions_1l += fit_regions_1l
all_regions_1l += validation_regions_1l
