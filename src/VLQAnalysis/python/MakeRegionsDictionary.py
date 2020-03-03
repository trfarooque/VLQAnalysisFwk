import re
from datetime import datetime

outfile = 'regions_dictionary_sVLQ.py'

doSplitLine = False

with open(outfile,'write') as f:
  f.write("# Created with MakeRegionsDictionary at %s\n\n"%datetime.now())

Presel=['c1lep3_5jwin1bin',
'c1lep3_5jwin2bin',
'c1lep3_5jwin3bin',
'c1lep3jin1bin',
'c1lep3jin2bin',
'c1lep3jin3bin',
'c1lep5jin1bin',
'c1lep5jin2bin',
'c1lep5jin3bin',
'c1lep6jin1bin',
'c1lep6jin2bin',
'c1lep6jin3bin',
'call']

SR=['c1lep3_5jwin1bex1fjin0LTex0Hex1Vin',
'c1lep3_5jwin1bex1fjin0Tex1Lin0Hex1Vin',
'c1lep3_5jwin2bex1fjin0LTex0Hex1Vin',
'c1lep3_5jwin2bex1fjin0Tex1Lin0Hex1Vin',
'c1lep3_5jwin3bex1fjin0LTex1Hin0Vex',
'c1lep3_5jwin3bex1fjin0Tex1Lin1Hin0Vex',
'c1lep3_5jwin3bex1fjin1Tin0Lex1Hin0Vex',
'c1lep3_5jwin4bin1fjin0LTex1Hin0Vex',
'c1lep3_5jwin4bin1fjin0Tex1Lin1Hin0Vex',
'c1lep3_5jwin4bin1fjin1Tin0Lex1Hin0Vex',
'c1lep6jin1bex1fjin1Lex0Tex0Hex1Vin',
'c1lep6jin1bex1fjin0Lex1Tex0Hex1Vin',
'c1lep6jin1bex1fjin2LTin0Hex1Vin',
'c1lep6jin2bex1fjin1Lex0Tex0Hex1Vin',
'c1lep6jin2bex1fjin0Lex1Tex0Hex1Vin',
'c1lep6jin2bex1fjin2LTin0Hex1Vin',
'c1lep6jin3bex1fjin1Lex0VTex1Hin',
'c1lep6jin3bex1fjin0Lex1VTex1Hin',
'c1lep6jin3bex1fjin2VLTin1Hin',
'c1lep6jin4bin1fjin1Lex0VTex1Hin',
'c1lep6jin4bin1fjin0Lex1VTex1Hin',
'c1lep6jin4bin1fjin2VLTin1Hin']

VR=['c1lep3_5jwin1bex0fjex0Tex0Lex0Hex1Vin',
'c1lep3_5jwin1bex0fjex0Tex1Lin0Hex1Vin',
'c1lep3_5jwin1bex1fjin1LTin0Hex0Vex',
'c1lep3_5jwin1bex1fjin1Tin0Lex0Hex1Vin',
'c1lep3_5jwin2bex0fjex0Tex0Lex0Hex1Vin',
'c1lep3_5jwin2bex0fjex0Tex1Lin0Hex1Vin',
'c1lep3_5jwin2bex1fjin1LTin0Hex0Vex',
'c1lep3_5jwin2bex1fjin1Tin0Lex0Hex1Vin',
'c1lep3_5jwin3bex0fjex0Tex0Lex1Hin0Vex',
'c1lep3_5jwin3bex0fjex0Tex1Lin1Hin0Vex',
'c1lep3_5jwin3bex1fjin1VLTin0Hex',
'c1lep3_5jwin4bin0fjex0Tex0Lex1Hin0Vex',
'c1lep3_5jwin4bin0fjex0Tex1Lin1Hin0Vex',
'c1lep3_5jwin4bin1fjin1VLTin0Hex',
'c1lep6jin1bex0fjex1LTex0Hex1Vin',
'c1lep6jin1bex0fjex2LTin0Hex1Vin',
'c1lep6jin1bex1fjin0Tex0Lex1Hin1Vin',
'c1lep6jin1bex1fjin2LTin1Hin0Vex',
'c1lep6jin2bex0fjex1LTex0Hex1Vin',
'c1lep6jin2bex0fjex2LTin0Hex1Vin',
'c1lep6jin2bex1fjin0Tex0Lex1Hin1Vin',
'c1lep6jin2bex1fjin2LTin1Hin0Vex',
'c1lep6jin3bex0fjex1VLTex1Hin',
'c1lep6jin3bex0fjex2VLTin1Hin',
'c1lep6jin3bex1fjin1VLTex0Hex',
'c1lep6jin3bex1fjin2VLTin0Hex',
'c1lep6jin4bin0fjex1VLTex1Hin',
'c1lep6jin4bin1fjin1VLTex0Hex',
'c1lep6jin4bin1fjin2VLTin0Hex']

binningdict = {}

# SR
binningdict['c1lep3_5jwin1bex1fjin0LTex0Hex1Vin']     = '600,700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin1bex1fjin0Tex1Lin0Hex1Vin']  = '600,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin2bex1fjin0LTex0Hex1Vin']     = '600,700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin2bex1fjin0Tex1Lin0Hex1Vin']  = '600,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin3bex1fjin0LTex1Hin0Vex']     = '600,800,900,1100,1300,1500,1900,3000'
binningdict['c1lep3_5jwin3bex1fjin0Tex1Lin1Hin0Vex']  = '600,800,900,1100,1300,1500,1900,3000'
binningdict['c1lep3_5jwin3bex1fjin1Tin0Lex1Hin0Vex']  = '900,1100,1300,1500,2000,3000'
binningdict['c1lep3_5jwin4bin1fjin0LTex1Hin0Vex']     = '600,900,1100,1400,1800,3000'
binningdict['c1lep3_5jwin4bin1fjin0Tex1Lin1Hin0Vex']  = '600,900,1100,1400,1800,3000'
binningdict['c1lep3_5jwin4bin1fjin1Tin0Lex1Hin0Vex']  = '1000,1300,1500,2000,3000'

binningdict['c1lep6jin1bex1fjin1Lex0Tex0Hex1Vin']     = '800,950,1100,1300,1800,3000'
binningdict['c1lep6jin1bex1fjin0Lex1Tex0Hex1Vin']     = '900,1100,1300,1500,2000,3000'
binningdict['c1lep6jin1bex1fjin2LTin0Hex1Vin']        = '1300,1500,1700,1900,2200,2700,3500'
binningdict['c1lep6jin2bex1fjin1Lex0Tex0Hex1Vin']     = '800,1000,1200,1500,1800,3000'
binningdict['c1lep6jin2bex1fjin0Lex1Tex0Hex1Vin']     = '900,1100,1300,1600,1900,2200,3200'
binningdict['c1lep6jin2bex1fjin2LTin0Hex1Vin']        = '1200,1400,1600,1800,2100,2500,3500'
binningdict['c1lep6jin3bex1fjin1Lex0VTex1Hin']        = '900,1100,1300,1600,2000,3000'
binningdict['c1lep6jin3bex1fjin0Lex1VTex1Hin']        = '1000,1200,1400,1700,2000,3000'
binningdict['c1lep6jin3bex1fjin2VLTin1Hin']           = '1400,1600,1900,2300,3500'
binningdict['c1lep6jin4bin1fjin1Lex0VTex1Hin']        = '900,1100,1300,1600,2000,3000'
binningdict['c1lep6jin4bin1fjin0Lex1VTex1Hin']        = '1000,1200,1400,1650,2000,3000'
binningdict['c1lep6jin4bin1fjin2VLTin1Hin']           = '1500,1900,3500'

# VR
binningdict['c1lep3_5jwin1bex0fjex0Tex0Lex0Hex1Vin']  = '600,700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin1bex0fjex0Tex1Lin0Hex1Vin']  = '600,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin1bex1fjin1LTin0Hex0Vex']     = '600,700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin1bex1fjin1Tin0Lex0Hex1Vin']  = '600,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin2bex0fjex0Tex0Lex0Hex1Vin']  = '600,700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin2bex0fjex0Tex1Lin0Hex1Vin']  = '600,700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin2bex1fjin1LTin0Hex0Vex']     = '600,700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin2bex1fjin1Tin0Lex0Hex1Vin']  = '900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin3bex0fjex0Tex0Lex1Hin0Vex']  = '600,700,800,900,1100,1300,1500,2000,3000'
binningdict['c1lep3_5jwin3bex0fjex0Tex1Lin1Hin0Vex']  = '600,800,900,1100,1300,1500,2000,3000'
binningdict['c1lep3_5jwin3bex1fjin1VLTin0Hex']        = '600,900,1100,1400,1800,3000'
binningdict['c1lep3_5jwin4bin0fjex0Tex0Lex1Hin0Vex']  = '600,900,1100,1400,1800,3000'
binningdict['c1lep3_5jwin4bin0fjex0Tex1Lin1Hin0Vex']  = '900,1100,1400,1800,3000'
binningdict['c1lep3_5jwin4bin1fjin1VLTin0Hex']        = '600,800,1000,1300,1800,3000'

binningdict['c1lep6jin1bex0fjex1LTex0Hex1Vin']        = '800,1000,1100,1300,1800,3000'
binningdict['c1lep6jin1bex0fjex2LTin0Hex1Vin']        = '1200,1400,1600,1800,2100,2500,3500'
binningdict['c1lep6jin1bex1fjin0Tex0Lex1Hin1Vin']     = '900,1100,1300,1800,3000'
binningdict['c1lep6jin1bex1fjin2LTin1Hin0Vex']        = '1200,1400,1600,1800,2100,2500,3500'
binningdict['c1lep6jin2bex0fjex1LTex0Hex1Vin']        = '800,1000,1200,1500,1800,3000'
binningdict['c1lep6jin2bex0fjex2LTin0Hex1Vin']        = '1200,1400,1600,1800,2100,2500,3500'
binningdict['c1lep6jin2bex1fjin0Tex0Lex1Hin1Vin']     = '800,1000,1200,1500,1800,3000'
binningdict['c1lep6jin2bex1fjin2LTin1Hin0Vex']        = '1200,1400,1600,1800,2100,2500,3500'
binningdict['c1lep6jin3bex0fjex1VLTex1Hin']           = '1000,1200,1400,1600,2000,3000'
binningdict['c1lep6jin3bex0fjex2VLTin1Hin']           = '1400,1700,1900,2300,3500'
binningdict['c1lep6jin3bex1fjin1VLTex0Hex']           = '900,1100,1300,1600,2000,3000'
binningdict['c1lep6jin3bex1fjin2VLTin0Hex']           = '1000,1200,1400,1600,2000,3000'
binningdict['c1lep6jin4bin0fjex1VLTex1Hin']           = '900,1100,1300,1600,2000,3000'
binningdict['c1lep6jin4bin1fjin1VLTex0Hex']           = '900,1100,1300,1600,2000,3000'
binningdict['c1lep6jin4bin1fjin2VLTin0Hex']           = '1200,1600,2000,3000'

def printregions(l,typ):
  for region in l:
    legend = ""
    items = 0
    if region == "call":
      legend = "No selection"
  
    if '3_5jwin' in region:
      legend+='LJ'
      items+=1
    if '6jin' in region:
      legend+='HJ'
      items+=1

    if 'b' in region:
      legend+=', '+region[region.index('b')-1]+'b'
      items+=1
    if 'fj' in region:
      legend+=', '+region[region.index('fj')-1]+'fj'
      items+=1

    if 'TH' in region:
      legend+=', '+region[region.index('TH')-1]+'h+t_{h}'
      items+=1
    elif 'T' in region and region[region.index('T')-1].isdigit():
      legend+=', '+region[region.index('T')-1]+'t_{h}'
      items+=1

    if 'LT' in region and region[region.index('LT')-1].isdigit():
      legend+=', '+region[region.index('LT')-1]+'(t_{h}+t_{l})'
      items+=2
    elif 'L' in region and region[region.index('L')-1].isdigit():
      legend+=', '+region[region.index('L')-1]+'t_{l}'
      items+=1  

    if 'H' in region:
      legend+=', '+region[region.index('H')-1]+'h'
      items+=1
  
    if 'VLT' in region:
      legend+=', '+region[region.index('VLT')-1]+'(v+t_{l}+t_{h})'
      items+=2
    if 'VT' in region:
      legend+=', '+region[region.index('VT')-1]+'(v+t_{h})'
      items+=2

    if 'V' in region and region[region.index('V')-1].isdigit() and not 'v' in legend:
      legend+=', '+region[region.index('V')-1]+'V'
      items+=1 
    
    if items >= 4 and doSplitLine:
      splitindex = [i.start() for i in re.finditer(', ',legend)][int(items/2)]+1
      legend = '#splitline{%s}{%s}'%(legend[:splitindex],legend[splitindex+1:])

    with open(outfile,'a') as f:
      f.write("reg_"+region[1:]+" = {\n")
      f.write("\t'name':"+'"HTX_'+region+'",\n')
      f.write("\t'legend': "+'"#scale[0.75]{'+legend+'}",\n')
      if region in binningdict.keys():
        f.write("\t'binning':"+'"%s",\n'%binningdict[region])
      else:
        f.write("\t'binning':"+'"600,800,1000,1400,1600,1800,3000",\n')
      f.write("\t'binning_recoVLQ0_m':"+'"600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,2200,3000",\n')
      f.write("\t'type':"+'"%s"\n'%typ)
      f.write('}\n')

with open(outfile,'a') as f:
  f.write("#\n")
  f.write("#\n")
  f.write("# Fit regions\n")
  f.write("#\n")
  f.write("#\n")
printregions(SR,"SIGNAL")

with open(outfile,'a') as f:
  f.write("#\n")
  f.write("#\n")
  f.write("# Validation regions\n")
  f.write("#\n")
  f.write("#\n")
printregions(VR,"VALIDATION")

with open(outfile,'a') as f:
  f.write("#\n")
  f.write("#\n")
  f.write("# Preselection regions\n")
  f.write("#\n")
  f.write("#\n")
printregions(Presel,"VALIDATION")

with open(outfile,'a') as f:
  f.write("fit_regions_1l = [\n")
  for region in SR:
    if region==SR[-1]:
      f.write("reg_"+region[1:]+"\n")
    else:
      f.write("reg_"+region[1:]+",\n")
  f.write("]\n\n")

with open(outfile,'a') as f:
  f.write("validation_regions_1l = [\n")
  for region in VR:
    if region==VR[-1]:
      f.write("reg_"+region[1:]+"\n")
    else:
      f.write("reg_"+region[1:]+",\n")
  f.write("]\n\n")

with open(outfile,'a') as f:
  f.write("preselection_regions_1l = [\n")
  for region in Presel:
    if region==Presel[-1]:
      f.write("reg_"+region[1:]+"\n")
    else:
      f.write("reg_"+region[1:]+",\n")
  f.write("]\n\n")
  f.write("all_regions_1l =  []\n")
  f.write("all_regions_1l += fit_regions_1l\n")
  f.write("all_regions_1l += validation_regions_1l\n")
  f.write("all_regions_1l += preselection_regions_1l\n")