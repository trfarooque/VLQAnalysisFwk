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

# Loose Syst regions
SR.extend(['c1lep3_5jwin1_2bwin1VLTHin',
           'c1lep3_5jwin3bin1VLTHin',
           'c1lep6jin1_2bwin1VLTHin',
           'c1lep6jin3bin1VLTHin'])

# ttbar+HF CRs
SR.extend(['c1lep3_5jwin4bin0fjex1Lin0VTex0Hex',
          'c1lep6jin4bin0fjex1Lin0VTex0Hex'])

VR=['c1lep3_5jwin1bex0fjex0Tex0Lex0Hex1Vin',
    'c1lep3_5jwin1bex0fjex0Tex1Lin0Hex1Vin',
    'c1lep3_5jwin1bex1fjin1LTin0Hex0Vex',
    'c1lep3_5jwin1bex1fjin1Tin0Lex0Hex1Vin',
    'c1lep3_5jwin2bex0fjex0Tex0Lex0Hex1Vin',
    'c1lep3_5jwin2bex0fjex0Tex1Lin0Hex1Vin',
    'c1lep3_5jwin2bex1fjin1LTin0Hex0Vex',
    'c1lep3_5jwin2bex1fjin1Tin0Lex0Hex1Vin',
    'c1lep3_5jwin3bin0fjex0Tex1Hin0Vex',
    'c1lep3_5jwin3bin1fjin1VLTin0Hex',
    'c1lep6jin1bex0fjex1LTex0Hex1Vin',
    'c1lep6jin1bex0fjex2LTin0Hex1Vin',
    'c1lep6jin1bex1fjin0Tex0Lex1Hin1Vin',
    'c1lep6jin1bex1fjin2LTin1Hin0Vex',
    'c1lep6jin2bex0fjex1LTex0Hex1Vin',
    'c1lep6jin2bex0fjex2LTin0Hex1Vin',
    'c1lep6jin2bex1fjin0Tex0Lex1Hin1Vin',
    'c1lep6jin2bex1fjin2LTin1Hin0Vex',
    'c1lep6jin3bin1fjin1VLTin0Hex',
    'c1lep6jin3bin0fjex1VLTin1Hin']


binningdict = {}

# SR
binningdict['c1lep3_5jwin1bex1fjin0LTex0Hex1Vin'] =         '700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin1bex1fjin0Tex1Lin0Hex1Vin'] =      '700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin2bex1fjin0LTex0Hex1Vin'] =         '700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin2bex1fjin0Tex1Lin0Hex1Vin'] =      '700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin3bex1fjin0LTex1Hin0Vex'] =         '700,800,900,1100,1300,1500,1900,3000'
binningdict['c1lep3_5jwin3bex1fjin0Tex1Lin1Hin0Vex'] =      '700,800,900,1100,1300,1500,1900,3000'
binningdict['c1lep3_5jwin3bex1fjin1Tin0Lex1Hin0Vex'] =      '900,1100,1300,1650,2100,3000'
binningdict['c1lep3_5jwin4bin1fjin0LTex1Hin0Vex'] =         '700,900,1100,1350,1700,3000'
binningdict['c1lep3_5jwin4bin1fjin0Tex1Lin1Hin0Vex'] =      '900,1150,1400,1700,3000'
binningdict['c1lep3_5jwin4bin1fjin1Tin0Lex1Hin0Vex'] =      '1000,1300,1600,1950,3000'

binningdict['c1lep6jin1bex1fjin1Lex0Tex0Hex1Vin'] =         '800,950,1100,1300,1600,2000,3000'
binningdict['c1lep6jin1bex1fjin0Lex1Tex0Hex1Vin'] =         '900,1100,1300,1600,2000,3000'
binningdict['c1lep6jin1bex1fjin2LTin0Hex1Vin'] =            '1300,1500,1700,1950,2300,2700,3500'
binningdict['c1lep6jin2bex1fjin1Lex0Tex0Hex1Vin'] =         '800,1000,1200,1500,1800,3000'
binningdict['c1lep6jin2bex1fjin0Lex1Tex0Hex1Vin'] =         '900,1100,1300,1600,1900,2200,3200'
binningdict['c1lep6jin2bex1fjin2LTin0Hex1Vin'] =            '1200,1400,1600,1800,2100,2500,3500'
binningdict['c1lep6jin3bex1fjin1Lex0VTex1Hin'] =            '900,1100,1300,1600,2000,3000'
binningdict['c1lep6jin3bex1fjin0Lex1VTex1Hin'] =            '1000,1200,1400,1700,2000,3000'
binningdict['c1lep6jin3bex1fjin2VLTin1Hin'] =               '1400,1600,1900,2350,3500'
binningdict['c1lep6jin4bin1fjin1Lex0VTex1Hin'] =            '900,1100,1300,1600,2000,3000'
binningdict['c1lep6jin4bin1fjin0Lex1VTex1Hin'] =            '1000,1200,1400,1650,2000,3000'
binningdict['c1lep6jin4bin1fjin2VLTin1Hin'] =               '1500,1950,3500'

# VR
binningdict['c1lep3_5jwin1bex0fjex0Tex0Lex0Hex1Vin'] =      '700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin1bex0fjex0Tex1Lin0Hex1Vin'] =      '700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin1bex1fjin1LTin0Hex0Vex'] =         '700,800,950,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin1bex1fjin1Tin0Lex0Hex1Vin'] =      '800,1100,1300,1600,2000,3000'
binningdict['c1lep3_5jwin2bex0fjex0Tex0Lex0Hex1Vin'] =      '700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin2bex0fjex0Tex1Lin0Hex1Vin'] =      '700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin2bex1fjin1LTin0Hex0Vex'] =         '700,800,950,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin2bex1fjin1Tin0Lex0Hex1Vin'] =      '900,1100,1300,1600,2000,3000'
binningdict['c1lep3_5jwin3bin0fjex0Tex1Hin0Vex'] =          '700,800,900,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin3bin1fjin1VLTin0Hex'] =            '700,800,950,1100,1300,1500,1800,3000'

binningdict['c1lep6jin1bex0fjex1LTex0Hex1Vin'] =            '800,1000,1100,1300,1800,3000'
binningdict['c1lep6jin1bex0fjex2LTin0Hex1Vin'] =            '1200,1400,1600,1800,2100,2500,3500'
binningdict['c1lep6jin1bex1fjin0Tex0Lex1Hin1Vin'] =         '900,1100,1300,1800,3000'
binningdict['c1lep6jin1bex1fjin2LTin1Hin0Vex'] =            '1200,1400,1600,1800,2100,2500,3500'
binningdict['c1lep6jin2bex0fjex1LTex0Hex1Vin'] =            '800,1000,1200,1500,1900,3000'
binningdict['c1lep6jin2bex0fjex2LTin0Hex1Vin'] =            '1200,1400,1600,1800,2100,2500,3500'
binningdict['c1lep6jin2bex1fjin0Tex0Lex1Hin1Vin'] =         '900,1200,1500,1900,3000'
binningdict['c1lep6jin2bex1fjin2LTin1Hin0Vex'] =            '1200,1400,1600,1800,2100,2500,3500'
binningdict['c1lep6jin3bin1fjin1VLTin0Hex'] =               '900,1100,1300,1600,2000,3000'
binningdict['c1lep6jin3bin0fjex1VLTin1Hin'] =               '900,1100,1300,1600,2000,3000'

# ttbar+HF Control Regions
binningdict['c1lep3_5jwin4bin0fjex1Lin0VTex0Hex'] =         '700,900,1100,1400,1800,3000'
binningdict['c1lep6jin4bin0fjex1Lin0VTex0Hex'] =            '900,1100,1300,1600,2000,3000'

# Loose Syst Regions
binningdict['c1lep3_5jwin1_2bwin1VLTHin'] =                 '700,800,950,1100,1300,1500,1800,3000'
binningdict['c1lep3_5jwin3bin1VLTHin'] =                    '700,800,950,1100,1300,1650,3000'
binningdict['c1lep6jin1_2bwin1VLTHin'] =                    '700,850,950,1100,1300,1650,3000'
binningdict['c1lep6jin3bin1VLTHin'] =                       '700,900,1100,1300,1600,2000,3000'

# Preselection
binningdict['c1lep3_5jwin1bin'] =                           '700,800,1000,1400,1600,1800,3000'
binningdict['c1lep3_5jwin2bin'] =                           '700,800,1000,1400,1600,1800,3000'
binningdict['c1lep3_5jwin3bin'] =                           '700,800,1000,1400,1600,1800,3000'
binningdict['c1lep3jin1bin'] =                              '700,800,1000,1400,1600,1800,3000'
binningdict['c1lep3jin2bin'] =                              '700,800,1000,1400,1600,1800,3000'
binningdict['c1lep3jin3bin'] =                              '700,800,1000,1400,1600,1800,3000'
binningdict['c1lep5jin1bin'] =                              '700,800,1000,1400,1600,1800,3000'
binningdict['c1lep5jin2bin'] =                              '700,800,1000,1400,1600,1800,3000'
binningdict['c1lep5jin3bin'] =                              '700,800,1000,1400,1600,1800,3000'
binningdict['c1lep6jin1bin'] =                              '700,800,1000,1400,1600,1800,3000'
binningdict['c1lep6jin2bin'] =                              '700,800,1000,1400,1600,1800,3000'
binningdict['c1lep6jin3bin'] =                              '700,800,1000,1400,1600,1800,3000'
binningdict['call'] =                                       '700,800,1000,1400,1600,1800,3000'

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

    if 'bwin' in region:
      legend+=', '+region[region.index('bwin')-3]+'-'+region[region.index('bwin')-1]+'b'

    elif 'b' in region:
      legend+=', '+region[region.index('b')-1]+'b'
      items+=1
    if 'fj' in region:
      legend+=', '+region[region.index('fj')-1]+'fj'
      items+=1

    if 'TH' in region and region[region.index('TH')-1].isdigit():
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

    if 'H' in region and region[region.index('H')-1].isdigit():
      legend+=', '+region[region.index('H')-1]+'h'
      items+=1
  
    if 'VLTH' in region:
      legend+=', '+region[region.index('VLTH')-1]+'(v+h+t_{l}+t_{h})'
      items+=2
    elif 'VLT' in region:
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

      # if typ=="VALIDATION":
      #   f.write("\t'legend': "+'"#scale[0.75]{'+legend+' VR}",\n')
      # elif typ=="SIGNAL":
      #   f.write("\t'legend': "+'"#scale[0.75]{'+legend+' SR}",\n')
      # else:
      f.write("\t'legend': "+'"#scale[0.75]{'+legend+'}",\n')
      
      if region in binningdict.keys():
        f.write("\t'binning':"+'"%s",\n'%binningdict[region])
      else:
        f.write("\t'binning':"+'"700,800,1000,1400,1600,1800,3000",\n')
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