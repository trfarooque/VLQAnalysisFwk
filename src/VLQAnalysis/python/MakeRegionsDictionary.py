import re
from datetime import datetime

doSplitLine = False
consistentLastBin = True
lastbinedge = 3500

doSVLQ = False
doPVLQ = True

doPresel = False
doSR = True
doVR = True
do1LR = True
do0LR = True
doTtbarCR = False
doLooseSyst = False

if doSVLQ:
  outfile = 'regions_dictionary_sVLQ.py'
elif doPVLQ:
  outfile = 'regions_dictionary_pVLQ.py'

with open(outfile,'write') as f:
  f.write("# Created with MakeRegionsDictionary at %s\n\n"%datetime.now())

if doSVLQ:

  Presel1L=['c1lep3_5jwin1bin',
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
  
  SR1L=['c1lep3_5jwin1bex1fjin0LTex0Hex1Vin',
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
  if doLooseSyst:
    SR1L.extend(['c1lep3_5jwin1_2bwin1VLTHin',
               'c1lep3_5jwin3bin1VLTHin',
               'c1lep6jin1_2bwin1VLTHin',
               'c1lep6jin3bin1VLTHin'])

  # ttbar+HF CRs
  if doTtbarCR:
    SR1L.extend(['c1lep3_5jwin4bin0fjex1Lin0VTex0Hex',
               'c1lep6jin4bin0fjex1Lin0VTex0Hex'])

  VR1L=['c1lep3_5jwin1bex0fjex0Tex0Lex0Hex1Vin',
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


if doPVLQ:

  if do1LR:

    SR1L=['c1lep0Tex0Hex6jin3bex',
          'c1lep1Tex0Hex6jin3bex',
          'c1lep0Tex1Hex6jin3bex',
          'c1lep1Tex1Hex6jin3bex',
          'c1lep2Tin0_1Hwin6jin3bex',
          'c1lep0Tin2Hin6jin3bex',
          'c1lep0Tex0Hex6jin4bin',
          'c1lep1Tex0Hex6jin4bin',
          'c1lep0Tex1Hex6jin4bin',
          'c1lep1Tex1Hex6jin4bin',
          'c1lep2Tin0_1Hwin6jin4bin',
          'c1lep0Tin2Hin6jin4bin']
    
    VR1L=['c1lep0Tex0Hex5jex3bex',
          'c1lep1Tex0Hex5jex3bex',
          'c1lep0Tex1Hex5jex3bex',
          'c1lep1Tex1Hex5jex3bex',
          'c1lep2Tin0_1Hwin5jex3bex',
          'c1lep0Tin2Hin5jex3bex',
          'c1lep0Tex0Hex5jex4bin',
          'c1lep1Tex0Hex5jex4bin',
          'c1lep0Tex1Hex5jex4bin',
          'c1lep2THin5jex4bin']

  if do0LR:
    
     SR0L=['c0lep0Tex0Hex7jin2bexHighMtbmin',
          'c0lep1Tex0Hex7jin2bexHighMtbmin',
          'c0lep0Tex1Hex7jin2bexHighMtbmin',
          'c0lep2THin7jin2bexHighMtbmin',
          'c0lep0Tex0Hex7jin3bexLowMtbmin',
          'c0lep1Tex0Hex7jin3bexLowMtbmin',
          'c0lep0Tex1Hex7jin3bexLowMtbmin',
          'c0lep1Tex1Hex7jin3bexLowMtbmin',
          'c0lep2Tin0_1Hwin7jin3bexLowMtbmin',
          'c0lep0Tex0Hex7jin3bexHighMtbmin',
          'c0lep1Tex0Hex7jin3bexHighMtbmin',
          'c0lep0Tex1Hex7jin3bexHighMtbmin',
          'c0lep1Tex1Hex7jin3bexHighMtbmin',
          'c0lep2Tin0_1Hwin7jin3bexHighMtbmin',
          'c0lep0Tin2Hin7jin3bex',
          'c0lep0Tex0Hex7jin4binLowMtbmin',
          'c0lep1Tex0Hex7jin4binLowMtbmin',
          'c0lep0Tex1Hex7jin4binLowMtbmin',
          'c0lep0Tex0Hex7jin4binHighMtbmin',
          'c0lep1Tex0Hex7jin4binHighMtbmin',
          'c0lep0Tex1Hex7jin4binHighMtbmin',
          'c0lep2THin7jin4bin']

     VR0L=['c0lep0Tex0Hex6jex2bexHighMtbmin',
          'c0lep1Tex0Hex6jex2bexHighMtbmin',
          'c0lep0Tex1Hex6jex2bexHighMtbmin',
          'c0lep2THin6jex2bexHighMtbmin',
          'c0lep0Tex0Hex6jex3bexLowMtbmin',
          'c0lep1Tex0Hex6jex3bexLowMtbmin',
          'c0lep0Tex1Hex6jex3bexLowMtbmin',
          'c0lep0Tex0Hex6jex3bexHighMtbmin',
          'c0lep1Tex0Hex6jex3bexHighMtbmin',
          'c0lep0Tex1Hex6jex3bexHighMtbmin',
          'c0lep2THin6jex3bex',
          'c0lep0Tex0Hex6jex4binLowMtbmin',
          'c0lep0Tex0Hex6jex4binHighMtbmin',
          'c0lep1Tex0Hex6jex4bin',
          'c0lep0Tex1Hex6jex4bin',
          'c0lep2THin6jex4bin']


binningdict = {}

if doSVLQ:

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

if doPVLQ:

  # SR                                                                                                                                                                                              
  binningdict['c1lep0Tex0Hex6jin3bex'] =                      '1000,1200,1400,1600,1900,2500,3500'
  binningdict['c1lep1Tex0Hex6jin3bex'] =                      '1000,1200,1400,1600,1900,2500,3500'
  binningdict['c1lep0Tex1Hex6jin3bex'] =                      '1000,1100,1300,1700,2100,2500,3500'
  binningdict['c1lep1Tex1Hex6jin3bex'] =                      '0,1000,1300,2000,2500,3500'
  binningdict['c1lep2Tin0_1Hwin6jin3bex'] =                   '0,1000,1300,2000,2500,3000,3500'
  binningdict['c1lep0Tin2Hin6jin3bex'] =                      '0,1300,2000,3500'
  binningdict['c1lep0Tex0Hex6jin4bin'] =                      '1000,1100,1300,1600,2000,3500'
  binningdict['c1lep1Tex0Hex6jin4bin'] =                      '1000,1100,1300,1700,2100,2500,3500'
  binningdict['c1lep0Tex1Hex6jin4bin'] =                      '1000,1300,1700,2100,3500'
  binningdict['c1lep1Tex1Hex6jin4bin'] =                      '0,1300,2000,3500'
  binningdict['c1lep2Tin0_1Hwin6jin4bin'] =                   '0,1300,2000,2500,3500'
  binningdict['c1lep0Tin2Hin6jin4bin'] =                      '0,1300,2000,3500'
  binningdict['c0lep0Tex0Hex7jin2bexHighMtbmin'] =            '1000,1200,1400,1600,3500'
  binningdict['c0lep1Tex0Hex7jin2bexHighMtbmin'] =            '1000,1200,1400,1600,2000,3500'
  binningdict['c0lep0Tex1Hex7jin2bexHighMtbmin'] =            '1000,1200,1400,1600,2000,3500'
  binningdict['c0lep2THin7jin2bexHighMtbmin'] =               '1000,1400,1600,2000,3500'
  binningdict['c0lep0Tex0Hex7jin3bexLowMtbmin'] =             '1000,1200,1400,1600,2000,3500'
  binningdict['c0lep1Tex0Hex7jin3bexLowMtbmin'] =             '1000,1200,1400,1600,2000,3500'
  binningdict['c0lep0Tex1Hex7jin3bexLowMtbmin'] =             '1000,1200,1400,1600,3500'
  binningdict['c0lep1Tex1Hex7jin3bexLowMtbmin'] =             '1000,1200,1400,1600,2000,3500'
  binningdict['c0lep2Tin0_1Hwin7jin3bexLowMtbmin'] =          '1000,1400,1600,2000,3500'
  binningdict['c0lep0Tex0Hex7jin3bexHighMtbmin'] =            '1000,1200,1400,1600,3500'
  binningdict['c0lep1Tex0Hex7jin3bexHighMtbmin'] =            '1000,1200,1400,1600,2000,3500'
  binningdict['c0lep0Tex1Hex7jin3bexHighMtbmin'] =            '1000,1200,1400,3500'
  binningdict['c0lep1Tex1Hex7jin3bexHighMtbmin'] =            '1000,1600,3500'
  binningdict['c0lep2Tin0_1Hwin7jin3bexHighMtbmin'] =         '1000,2000,3500'
  binningdict['c0lep0Tin2Hin7jin3bex'] =                      '1000,1400,3500'
  binningdict['c0lep0Tex0Hex7jin4binLowMtbmin'] =             '1000,1200,1400,1600,3500'
  binningdict['c0lep1Tex0Hex7jin4binLowMtbmin'] =             '1000,1200,1400,1600,2000,3500'
  binningdict['c0lep0Tex1Hex7jin4binLowMtbmin'] =             '1000,1200,1400,1600,3500'
  binningdict['c0lep0Tex0Hex7jin4binHighMtbmin'] =            '1000,1200,1400,3500'
  binningdict['c0lep1Tex0Hex7jin4binHighMtbmin'] =            '1000,1200,1600,3500'
  binningdict['c0lep0Tex1Hex7jin4binHighMtbmin'] =            '1000,3500'
  binningdict['c0lep2THin7jin4bin'] =                         '1000,1600,2000,3500'
  
  # VR
  binningdict['c1lep0Tex0Hex5jex3bex'] =                      '1000,1200,1300,1600,2000,2500,3500'
  binningdict['c1lep1Tex0Hex5jex3bex'] =                      '1000,1200,1500,1900,2500,3500'
  binningdict['c1lep0Tex1Hex5jex3bex'] =                      '1000,1100,1300,1700,2100,2500,3500'
  binningdict['c1lep1Tex1Hex5jex3bex'] =                      '0,1300,2000,3500'
  binningdict['c1lep2Tin0_1Hwin5jex3bex'] =                   '0,1300,2000,3500'
  binningdict['c1lep0Tin2Hin5jex3bex'] =                      '0,1300,2000,3500'
  binningdict['c1lep0Tex0Hex5jex4bin'] =                      '1000,1300,1600,2000,2500,3500'
  binningdict['c1lep1Tex0Hex5jex4bin'] =                      '1000,1300,2000,3500'
  binningdict['c1lep0Tex1Hex5jex4bin'] =                      '1000,1300,1700,2100,2500,3500'
  binningdict['c1lep2THin5jex4bin'] =                         '0,1300,2000,3500'
  binningdict['c0lep0Tex0Hex6jex2bexHighMtbmin'] =            '1000,1200,1400,1600,3500'
  binningdict['c0lep1Tex0Hex6jex2bexHighMtbmin'] =            '1000,1200,1400,1600,2000,3500'
  binningdict['c0lep0Tex1Hex6jex2bexHighMtbmin'] =            '1000,1200,1400,1600,1800,3500'
  binningdict['c0lep2THin6jex2bexHighMtbmin'] =               '1000,1400,3500'
  binningdict['c0lep0Tex0Hex6jex3bexLowMtbmin'] =             '1000,1200,1400,1600,2000,3500'
  binningdict['c0lep1Tex0Hex6jex3bexLowMtbmin'] =             '1000,1200,1400,1600,2000,3500'
  binningdict['c0lep0Tex1Hex6jex3bexLowMtbmin'] =             '1000,1200,1400,1600,3500'
  binningdict['c0lep0Tex0Hex6jex3bexHighMtbmin'] =            '1000,1200,1400,1600,3500'
  binningdict['c0lep1Tex0Hex6jex3bexHighMtbmin'] =            '1000,1200,1400,1600,2000,3500'
  binningdict['c0lep0Tex1Hex6jex3bexHighMtbmin'] =            '1000,1400,3500'
  binningdict['c0lep2THin6jex3bex'] =                         '1000,3500'
  binningdict['c0lep0Tex0Hex6jex4binLowMtbmin'] =             '1000,3500'
  binningdict['c0lep0Tex0Hex6jex4binHighMtbmin'] =            '1000,3500'
  binningdict['c0lep1Tex0Hex6jex4bin'] =                      '1000,1200,3500'
  binningdict['c0lep0Tex1Hex6jex4bin'] =                      '1000,3500'
  binningdict['c0lep2THin6jex4bin'] =                         '1000,3500'


if doSVLQ:
  if consistentLastBin:
    for reg in binningdict.keys():
      binningdict[reg] = binningdict[reg][:binningdict[reg].rfind(',')+1]+str(lastbinedge)

def printregions(l,typ):
  for region in l:
    legend = ""
    items = 0
    if region == "call":
      legend = "No selection"

    if doSVLQ:
  
      if '3_5jwin' in region:
        legend+='LJ'
        items+=1
      if '6jin' in region:
        legend+='HJ'
        items+=1
    
    elif doPVLQ:
    
      if '6jin' in region:
        legend+='#geq6j'
        items+=1
      if '7jin' in region:
        legend+='#geq7j'
        items+=1
      if '5jex' in region:
        legend+='5j'
        items+=1
      if '6jex' in region:
        legend+='6j'
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

    if 'Hwin' in region:
      legend+=', '+region[region.index('Hwin')-3]+'-'+region[region.index('Hwin')-1]+'H'
      items+=1
    elif 'H' in region and region[region.index('H')-1].isdigit():
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
    
    if 'LowMtbmin' in region:
      legend+=', LM'
      items+=1
    elif 'HighMtbmin' in region:
      legend+=', HM'
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

# Write region definitions
if doSR:
  with open(outfile,'a') as f:
    f.write("#\n")
    f.write("#\n")
    f.write("# Fit regions\n")
    f.write("#\n")
    f.write("#\n")
  if do1LR:
    printregions(SR1L,"SIGNAL")
  if do0LR:
    printregions(SR0L,"SIGNAL")

if doVR:
  with open(outfile,'a') as f:
    f.write("#\n")
    f.write("#\n")
    f.write("# Validation regions\n")
    f.write("#\n")
    f.write("#\n")
  if do1LR:
    printregions(VR1L,"VALIDATION")
  if do0LR:
    printregions(VR0L,"VALIDATION")

if doPresel:
  with open(outfile,'a') as f:
    f.write("#\n")
    f.write("#\n")
    f.write("# Preselection regions\n")
    f.write("#\n")
    f.write("#\n")
  if do1LR:
    printregions(Presel1L,"VALIDATION")
  if do0LR:
    printregions(Presel0L,"VALIDATION")


# Add regions to dictionary to be read later
if doSR:
  with open(outfile,'a') as f:
    
    if do1LR:
      f.write("\nfit_regions_1l = [\n")
      for region in SR1L:
        if region==SR1L[-1]:
          f.write("reg_"+region[1:]+"\n")
        else:
          f.write("reg_"+region[1:]+",\n")
      f.write("]\n")
    
    if do0LR:
      f.write("\nfit_regions_0l = [\n")
      for region in SR0L:
        if region==SR0L[-1]:
          f.write("reg_"+region[1:]+"\n")
        else:
          f.write("reg_"+region[1:]+",\n")
      f.write("]\n")
      

if doVR:
  with open(outfile,'a') as f:
    
    if do1LR:
      f.write("\nvalidation_regions_1l = [\n")
      for region in VR1L:
        if region==VR1L[-1]:
          f.write("reg_"+region[1:]+"\n")
        else:
          f.write("reg_"+region[1:]+",\n")
      f.write("]\n")

    if do0LR:
      f.write("\nvalidation_regions_0l = [\n")
      for region in VR0L:
        if region==VR0L[-1]:
          f.write("reg_"+region[1:]+"\n")
        else:
          f.write("reg_"+region[1:]+",\n")
      f.write("]\n")

if doPresel:
  with open(outfile,'a') as f:

    if do1LR:
      f.write("\npreselection_regions_1l = [\n")
      for region in Presel1L:
        if region==Presel1L[-1]:
          f.write("reg_"+region[1:]+"\n")
        else:
          f.write("reg_"+region[1:]+",\n")
      f.write("]\n")

    if do0LR:
      f.write("\npreselection_regions_0l = [\n")
      for region in Presel0L:
        if region==Presel0L[-1]:
          f.write("reg_"+region[1:]+"\n")
        else:
          f.write("reg_"+region[1:]+",\n")
      f.write("]\n")

# Final tally
with open(outfile,'a') as f:
  
  if do1LR:
    f.write("all_regions_1l =  []\n")
    if doSR:
      f.write("all_regions_1l += fit_regions_1l\n")
    if doVR:
      f.write("all_regions_1l += validation_regions_1l\n")
    if doPresel:
      f.write("all_regions_1l += preselection_regions_1l\n")

  if do0LR:
    f.write("all_regions_0l =  []\n")
    if doSR:
      f.write("all_regions_0l += fit_regions_0l\n")
    if doVR:
      f.write("all_regions_0l += validation_regions_0l\n")
    if doPresel:
      f.write("all_regions_0l += preselection_regions_0l\n")
