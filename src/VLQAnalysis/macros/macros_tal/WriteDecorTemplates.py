###
# Decorrelated among ALL regions
###
regions = ['HTX_c1lep3_5jwin1bex1fjin0LTex0Hex1Vin',
           'HTX_c1lep3_5jwin1bex1fjin0Tex1Lin0Hex1Vin',
           'HTX_c1lep3_5jwin2bex1fjin0LTex0Hex1Vin',
           'HTX_c1lep3_5jwin2bex1fjin0Tex1Lin0Hex1Vin',
           'HTX_c1lep3_5jwin3bex1fjin0LTex1Hin0Vex',
           'HTX_c1lep3_5jwin3bex1fjin0Tex1Lin1Hin0Vex',
           'HTX_c1lep3_5jwin3bex1fjin1Tin0Lex1Hin0Vex',
           'HTX_c1lep3_5jwin4bin0fjex1Lin0VTex0Hex',
           'HTX_c1lep3_5jwin4bin1fjin0LTex1Hin0Vex',
           'HTX_c1lep3_5jwin4bin1fjin0Tex1Lin1Hin0Vex',
           'HTX_c1lep3_5jwin4bin1fjin1Tin0Lex1Hin0Vex',
           'HTX_c1lep6jin1bex1fjin0Lex1Tex0Hex1Vin',
           'HTX_c1lep6jin1bex1fjin1Lex0Tex0Hex1Vin',
           'HTX_c1lep6jin1bex1fjin2LTin0Hex1Vin',
           'HTX_c1lep6jin2bex1fjin0Lex1Tex0Hex1Vin',
           'HTX_c1lep6jin2bex1fjin1Lex0Tex0Hex1Vin',
           'HTX_c1lep6jin2bex1fjin2LTin0Hex1Vin',
           'HTX_c1lep6jin3bex1fjin0Lex1VTex1Hin',
           'HTX_c1lep6jin3bex1fjin1Lex0VTex1Hin',
           'HTX_c1lep6jin3bex1fjin2VLTin1Hin',
           'HTX_c1lep6jin4bin0fjex1Lin0VTex0Hex',
           'HTX_c1lep6jin4bin1fjin0Lex1VTex1Hin',
           'HTX_c1lep6jin4bin1fjin1Lex0VTex1Hin',
           'HTX_c1lep6jin4bin1fjin2VLTin1Hin']

regions_blind = ['HTX_c1lep3_5jwin1bex1fjin0LTex0Hex1Vin',
                 'HTX_c1lep3_5jwin1bex1fjin0Tex1Lin0Hex1Vin',
                 'HTX_c1lep3_5jwin2bex1fjin0LTex0Hex1Vin',
                 'HTX_c1lep3_5jwin2bex1fjin0Tex1Lin0Hex1Vin',
                 'HTX_c1lep3_5jwin3bex1fjin0LTex1Hin0Vex',
                 'HTX_c1lep3_5jwin3bex1fjin0Tex1Lin1Hin0Vex',
                 'HTX_c1lep3_5jwin3bex1fjin1Tin0Lex1Hin0Vex',
                 'HTX_c1lep3_5jwin4bin0fjex1Lin0VTex0Hex',
                 'HTX_c1lep6jin1bex1fjin0Lex1Tex0Hex1Vin',
                 'HTX_c1lep6jin1bex1fjin1Lex0Tex0Hex1Vin',
                 'HTX_c1lep6jin2bex1fjin0Lex1Tex0Hex1Vin',
                 'HTX_c1lep6jin2bex1fjin1Lex0Tex0Hex1Vin',
                 'HTX_c1lep6jin2bex1fjin2LTin0Hex1Vin',
                 'HTX_c1lep6jin3bex1fjin1Lex0VTex1Hin',
                 'HTX_c1lep6jin4bin0fjex1Lin0VTex0Hex']

###
# Decorrelated among low/high Njets and number of tagged boosted objects
###
regions_dict = {'LJ-1boost':['HTX_c1lep3_5jwin1bex1fjin0LTex0Hex1Vin',
                            'HTX_c1lep3_5jwin2bex1fjin0LTex0Hex1Vin',
                            'HTX_c1lep3_5jwin3bex1fjin0LTex1Hin0Vex',
                            'HTX_c1lep3_5jwin4bin0fjex1Lin0VTex0Hex',
                            'HTX_c1lep3_5jwin4bin1fjin0LTex1Hin0Vex'],
                'LJ-2boost':['HTX_c1lep3_5jwin1bex1fjin0Tex1Lin0Hex1Vin',
                            'HTX_c1lep3_5jwin2bex1fjin0Tex1Lin0Hex1Vin',
                            'HTX_c1lep3_5jwin3bex1fjin0Tex1Lin1Hin0Vex',
                            'HTX_c1lep3_5jwin3bex1fjin1Tin0Lex1Hin0Vex',
                            'HTX_c1lep3_5jwin4bin1fjin0Tex1Lin1Hin0Vex',
                            'HTX_c1lep3_5jwin4bin1fjin1Tin0Lex1Hin0Vex'],
                'HJ-1boost':['HTX_c1lep6jin4bin0fjex1Lin0VTex0Hex'],
                'HJ-g2boost':['HTX_c1lep6jin1bex1fjin0Lex1Tex0Hex1Vin',
                            'HTX_c1lep6jin1bex1fjin1Lex0Tex0Hex1Vin',
                            'HTX_c1lep6jin2bex1fjin0Lex1Tex0Hex1Vin',
                            'HTX_c1lep6jin2bex1fjin1Lex0Tex0Hex1Vin',
                            'HTX_c1lep6jin3bex1fjin0Lex1VTex1Hin',
                            'HTX_c1lep6jin3bex1fjin1Lex0VTex1Hin',
                            'HTX_c1lep6jin4bin1fjin0Lex1VTex1Hin',
                            'HTX_c1lep6jin4bin1fjin1Lex0VTex1Hin',
                            'HTX_c1lep6jin1bex1fjin2LTin0Hex1Vin',#'HJ-3boost'
                            'HTX_c1lep6jin2bex1fjin2LTin0Hex1Vin',
                            'HTX_c1lep6jin3bex1fjin2VLTin1Hin',
                            'HTX_c1lep6jin4bin1fjin2VLTin1Hin']
                }

regions_dict_blind = {'LJ-1boost':['HTX_c1lep3_5jwin1bex1fjin0LTex0Hex1Vin',
                                  'HTX_c1lep3_5jwin2bex1fjin0LTex0Hex1Vin',
                                  'HTX_c1lep3_5jwin3bex1fjin0LTex1Hin0Vex',
                                  'HTX_c1lep3_5jwin4bin0fjex1Lin0VTex0Hex'],
                      'LJ-2boost':['HTX_c1lep3_5jwin1bex1fjin0Tex1Lin0Hex1Vin',
                                  'HTX_c1lep3_5jwin2bex1fjin0Tex1Lin0Hex1Vin',
                                  'HTX_c1lep3_5jwin3bex1fjin0Tex1Lin1Hin0Vex',
                                  'HTX_c1lep3_5jwin3bex1fjin1Tin0Lex1Hin0Vex'],
                      'HJ-1boost':['HTX_c1lep6jin4bin0fjex1Lin0VTex0Hex'],
                      'HJ-g2boost':['HTX_c1lep6jin1bex1fjin0Lex1Tex0Hex1Vin',
                                  'HTX_c1lep6jin1bex1fjin1Lex0Tex0Hex1Vin',
                                  'HTX_c1lep6jin2bex1fjin0Lex1Tex0Hex1Vin',
                                  'HTX_c1lep6jin2bex1fjin1Lex0Tex0Hex1Vin',
                                  'HTX_c1lep6jin3bex1fjin1Lex0VTex1Hin',
                                  'HTX_c1lep6jin2bex1fjin2LTin0Hex1Vin'] #'HJ-3boost'
                }



##_______________________________________________________________
##
def getRegionLabel(reg):
    """
    Returns very short label, e.g LJ1b1fj1t_{l}0t_{h}0h1V
    """
    label = ""
    region = reg.replace("HTX_c1lep","")
  
    if '3_5jwin' in region:
        label+='LJ'
    if '6jin' in region:
        label+='HJ'
    if 'bwin' in region:
        label+=region[region.index('bwin')-3]+'-'+region[region.index('bwin')-1]+'b'
    elif 'b' in region:
        label+=region[region.index('b')-1]+'b'
    if 'fj' in region:
        label+=region[region.index('fj')-1]+'fj'
    if 'TH' in region and region[region.index('TH')-1].isdigit():
        label+=region[region.index('TH')-1]+'h+t_{h}'
    elif 'T' in region and region[region.index('T')-1].isdigit():
        label+=region[region.index('T')-1]+'t_{h}'
    if 'LT' in region and region[region.index('LT')-1].isdigit():
        label+=region[region.index('LT')-1]+'(t_{h}+t_{l})'
    elif 'L' in region and region[region.index('L')-1].isdigit():
        label+=region[region.index('L')-1]+'t_{l}'
    if 'H' in region and region[region.index('H')-1].isdigit():
        label+=region[region.index('H')-1]+'h'
    if 'VLTH' in region:
        label+=region[region.index('VLTH')-1]+'(v+h+t_{l}+t_{h})'
    elif 'VLT' in region:
        label+=region[region.index('VLT')-1]+'(v+t_{l}+t_{h})'
    if 'VT' in region:
        label+=region[region.index('VT')-1]+'(v+t_{h})'
    if 'V' in region and region[region.index('V')-1].isdigit() and not 'v' in label:
        label+=region[region.index('V')-1]+'V'
    return label

##_______________________________________________________________
##
## TTBAR
##_______________________________________________________________
##

def writeGen_ttbar(f,regions):
    flavs = [['light','light'],['cc','#geq1c'],['bb','#geq1b']]

    for flav in flavs:
        f.write('Systematic: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"HTX_BKGNP_TTBAR%s_GEN_%s";'%(flav[0].upper(),regtype))
                else:
                    f.write('"HTX_BKGNP_TTBAR%s_GEN_%s"\n'%(flav[0].upper(),regtype))

            f.write('Title: ')
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"t#bar{t}+%s gen. %s";'%(flav[1],regtype))
                else:
                    f.write('"t#bar{t}+%s gen. %s"\n'%(flav[1],regtype))

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"HTX_BKGNP_TTBAR%s_GEN_%s";'%(flav[0].upper(),reg))
                else:
                    f.write('"HTX_BKGNP_TTBAR%s_GEN_%s"\n'%(flav[0].upper(),reg))

            f.write('Title: ')
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"t#bar{t}+%s gen. %s";'%(flav[1],getRegionLabel(reg)))
                else:
                    f.write('"t#bar{t}+%s gen. %s"\n'%(flav[1],getRegionLabel(reg)))

        f.write('Type: HISTO\n')
        f.write('Category: "t#bar{t} generator uncertainties"\n')
        f.write('Symmetrisation: ONESIDED\n')
        f.write('Smoothing: 40\n')
        f.write('Samples: ttbar%s\n'%flav[0])
        f.write('HistoFilesUp: TtStAlt/ttbar%saMCPy.mc16a,TtStAlt/ttbar%saMCPy.mc16d,TtStAlt/ttbar%saMCPy.mc16e\n'%(flav[0],flav[0],flav[0]))
        f.write('Regions: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                for reg in regions[regtype]:
                    if not reg == regions[regtype][-1]:
                        f.write(reg+',')
                    else:
                        f.write(reg)
                if not regtype == regions.keys()[-1]:
                    f.write(';')
                else:
                    f.write('\n')

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write(reg+';')
                else:
                    f.write(reg+'\n')
        f.write('ReferenceSample: ttbar%s_AFII\n'%flav[0])
        f.write('\n')

##_______________________________________________________________
##
def writePS_ttbar(f,regions):
    flavs = [['light','light'],['cc','#geq1c'],['bb','#geq1b']]

    for flav in flavs:
        f.write('Systematic: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"HTX_BKGNP_TTBAR%s_PS_%s";'%(flav[0].upper(),regtype))
                else:
                    f.write('"HTX_BKGNP_TTBAR%s_PS_%s"\n'%(flav[0].upper(),regtype))

            f.write('Title: ')
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"t#bar{t}+%s ps. %s";'%(flav[1],regtype))
                else:
                    f.write('"t#bar{t}+%s ps. %s"\n'%(flav[1],regtype))

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"HTX_BKGNP_TTBAR%s_PS_%s";'%(flav[0].upper(),reg))
                else:
                    f.write('"HTX_BKGNP_TTBAR%s_PS_%s"\n'%(flav[0].upper(),reg))

            f.write('Title: ')
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"t#bar{t}+%s ps. %s";'%(flav[1],getRegionLabel(reg)))
                else:
                    f.write('"t#bar{t}+%s ps. %s"\n'%(flav[1],getRegionLabel(reg)))

        f.write('Type: HISTO\n')
        f.write('Category: "t#bar{t} parton shower uncertainties"\n')
        f.write('Symmetrisation: ONESIDED\n')
        f.write('Smoothing: 40\n')
        f.write('Samples: ttbar%s\n'%flav[0])
        f.write('HistoFilesUp: TtStAlt/ttbar%sPowHer.mc16a,TtStAlt/ttbar%sPowHer.mc16d,TtStAlt/ttbar%sPowHer.mc16e\n'%(flav[0],flav[0],flav[0]))
        f.write('Regions: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                for reg in regions[regtype]:
                    if not reg == regions[regtype][-1]:
                        f.write(reg+',')
                    else:
                        f.write(reg)
                if not regtype == regions.keys()[-1]:
                    f.write(';')
                else:
                    f.write('\n')

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write(reg+';')
                else:
                    f.write(reg+'\n')
        f.write('ReferenceSample: ttbar%s_AFII\n'%flav[0])
        f.write('\n')

##_______________________________________________________________
##
def writeVar3c_ttbar(f,regions):
    flavs = [['light','light'],['cc','#geq1c'],['bb','#geq1b']]

    for flav in flavs:
        f.write('Systematic: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"HTX_BKGNP_TTBAR%s_ISR_VAR3C_%s";'%(flav[0].upper(),regtype))
                else:
                    f.write('"HTX_BKGNP_TTBAR%s_ISR_VAR3C_%s"\n'%(flav[0].upper(),regtype))

            f.write('Title: ')
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"t#bar{t}+%s isr Var3c %s";'%(flav[1],regtype))
                else:
                    f.write('"t#bar{t}+%s isr Var3c %s"\n'%(flav[1],regtype))

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"HTX_BKGNP_TTBAR%s_ISR_VAR3C_%s";'%(flav[0].upper(),reg))
                else:
                    f.write('"HTX_BKGNP_TTBAR%s_ISR_VAR3C_%s"\n'%(flav[0].upper(),reg))

            f.write('Title: ')
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"t#bar{t}+%s isr Var3c %s";'%(flav[1],getRegionLabel(reg)))
                else:
                    f.write('"t#bar{t}+%s isr Var3c %s"\n'%(flav[1],getRegionLabel(reg)))

        f.write('Type: HISTO\n')
        f.write('Category: "t#bar{t} isr Var3c uncertainties"\n')
        f.write('HistoNameSufUp: _weight_pmg_Var3cUp\n')
        f.write('HistoNameSufDown: _weight_pmg_Var3cDown\n')
        #f.write('HistoPathUp: "__HISTOPATH__/TheorySys/"\n')
        #f.write('HistoPathDown: "__HISTOPATH__/TheorySys/"\n')
        f.write('Symmetrisation: TwoSided\n')
        f.write('Samples: ttbar%s\n'%flav[0])
        f.write('Regions: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                for reg in regions[regtype]:
                    if not reg == regions[regtype][-1]:
                        f.write(reg+',')
                    else:
                        f.write(reg)
                if not regtype == regions.keys()[-1]:
                    f.write(';')
                else:
                    f.write('\n')

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write(reg+';')
                else:
                    f.write(reg+'\n')
        f.write('\n')

##_______________________________________________________________
##
def writeISR_MUR_ttbar(f,regions):
    flavs = [['light','light'],['cc','#geq1c'],['bb','#geq1b']]

    for flav in flavs:
        f.write('Systematic: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"HTX_BKGNP_TTBAR%s_ISR_MUR_%s";'%(flav[0].upper(),regtype))
                else:
                    f.write('"HTX_BKGNP_TTBAR%s_ISR_MUR_%s"\n'%(flav[0].upper(),regtype))

            f.write('Title: ')
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"t#bar{t}+%s isr #mu_{R} %s";'%(flav[1],regtype))
                else:
                    f.write('"t#bar{t}+%s isr #mu_{R} %s"\n'%(flav[1],regtype))

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"HTX_BKGNP_TTBAR%s_ISR_MUR_%s";'%(flav[0].upper(),reg))
                else:
                    f.write('"HTX_BKGNP_TTBAR%s_ISR_MUR_%s"\n'%(flav[0].upper(),reg))

            f.write('Title: ')
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"t#bar{t}+%s isr #mu_{R} %s";'%(flav[1],getRegionLabel(reg)))
                else:
                    f.write('"t#bar{t}+%s isr #mu_{R} %s"\n'%(flav[1],getRegionLabel(reg)))

        f.write('Type: HISTO\n')
        f.write('Category: "t#bar{t} isr #mu_{R} uncertainties"\n')
        f.write('HistoNameSufUp: _weight_pmg_muR20__muF10\n')
        f.write('HistoNameSufDown: _weight_pmg_muR05__muF10\n')
        #f.write('HistoPathUp: "__HISTOPATH__/TheorySys/"\n')
        #f.write('HistoPathDown: "__HISTOPATH__/TheorySys/"\n')
        f.write('Symmetrisation: TwoSided\n')
        f.write('Samples: ttbar%s\n'%flav[0])
        f.write('Regions: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                for reg in regions[regtype]:
                    if not reg == regions[regtype][-1]:
                        f.write(reg+',')
                    else:
                        f.write(reg)
                if not regtype == regions.keys()[-1]:
                    f.write(';')
                else:
                    f.write('\n')

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write(reg+';')
                else:
                    f.write(reg+'\n')
        f.write('\n')

##_______________________________________________________________
##
def writeISR_MUF_ttbar(f,regions):
    flavs = [['light','light'],['cc','#geq1c'],['bb','#geq1b']]

    for flav in flavs:
        f.write('Systematic: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"HTX_BKGNP_TTBAR%s_ISR_MUF_%s";'%(flav[0].upper(),regtype))
                else:
                    f.write('"HTX_BKGNP_TTBAR%s_ISR_MUF_%s"\n'%(flav[0].upper(),regtype))

            f.write('Title: ')
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"t#bar{t}+%s isr #mu_{F} %s";'%(flav[1],regtype))
                else:
                    f.write('"t#bar{t}+%s isr #mu_{F} %s"\n'%(flav[1],regtype))

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"HTX_BKGNP_TTBAR%s_ISR_MUF_%s";'%(flav[0].upper(),reg))
                else:
                    f.write('"HTX_BKGNP_TTBAR%s_ISR_MUF_%s"\n'%(flav[0].upper(),reg))

            f.write('Title: ')
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"t#bar{t}+%s isr #mu_{F} %s";'%(flav[1],getRegionLabel(reg)))
                else:
                    f.write('"t#bar{t}+%s isr #mu_{F} %s"\n'%(flav[1],getRegionLabel(reg)))
                    
        f.write('Type: HISTO\n')
        f.write('Category: "t#bar{t} isr #mu_{F} uncertainties"\n')
        f.write('HistoNameSufUp: _weight_pmg_muR10__muF20\n')
        f.write('HistoNameSufDown: _weight_pmg_muR10__muF05\n')
        #f.write('HistoPathUp: "__HISTOPATH__/TheorySys/"\n')
        #f.write('HistoPathDown: "__HISTOPATH__/TheorySys/"\n')
        f.write('Symmetrisation: TwoSided\n')
        f.write('Samples: ttbar%s\n'%flav[0])
        f.write('Regions: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                for reg in regions[regtype]:
                    if not reg == regions[regtype][-1]:
                        f.write(reg+',')
                    else:
                        f.write(reg)
                if not regtype == regions.keys()[-1]:
                    f.write(';')
                else:
                    f.write('\n')

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write(reg+';')
                else:
                    f.write(reg+'\n')
        f.write('\n')

##_______________________________________________________________
##
def writeFSR_MURF_ttbar(f,regions):
    flavs = [['light','light'],['cc','#geq1c'],['bb','#geq1b']]

    for flav in flavs:
        f.write('Systematic: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"HTX_BKGNP_TTBAR%s_FSR_MURF_%s";'%(flav[0].upper(),regtype))
                else:
                    f.write('"HTX_BKGNP_TTBAR%s_FSR_MURF_%s"\n'%(flav[0].upper(),regtype))

            f.write('Title: ')
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"t#bar{t}+%s fsr #mu_{R,F} %s";'%(flav[1],regtype))
                else:
                    f.write('"t#bar{t}+%s fsr #mu_{R,F} %s"\n'%(flav[1],regtype))

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"HTX_BKGNP_TTBAR%s_FSR_MURF_%s";'%(flav[0].upper(),reg))
                else:
                    f.write('"HTX_BKGNP_TTBAR%s_FSR_MURF_%s"\n'%(flav[0].upper(),reg))

            f.write('Title: ')
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"t#bar{t}+%s fsr #mu_{R,F} %s";'%(flav[1],getRegionLabel(reg)))
                else:
                    f.write('"t#bar{t}+%s fsr #mu_{R,F} %s"\n'%(flav[1],getRegionLabel(reg)))

        f.write('Type: HISTO\n')
        f.write('Category: "t#bar{t} fsr #mu_{R,F} uncertainties"\n')
        f.write('HistoNameSufUp: _weight_pmg_isr_muRfac10__fsr_muRfac20\n')
        f.write('HistoNameSufDown: _weight_pmg_isr_muRfac10__fsr_muRfac05\n')
        #f.write('HistoPathUp: "__HISTOPATH__/TheorySys/"\n')
        #f.write('HistoPathDown: "__HISTOPATH__/TheorySys/"\n')
        f.write('Symmetrisation: TwoSided\n')
        f.write('Samples: ttbar%s\n'%flav[0])
        f.write('Regions: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                for reg in regions[regtype]:
                    if not reg == regions[regtype][-1]:
                        f.write(reg+',')
                    else:
                        f.write(reg)
                if not regtype == regions.keys()[-1]:
                    f.write(';')
                else:
                    f.write('\n')

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write(reg+';')
                else:
                    f.write(reg+'\n')
        f.write('\n')

##_______________________________________________________________
##
## SINGLETOP
##_______________________________________________________________
##

def writeGen_singletop(f,regions):
    f.write('Systematic: ')

    # received region dict
    if type(regions)==dict:
        for regtype in regions.keys():
            if not regtype == regions.keys()[-1]:
                f.write('"HTX_BKGNP_SINGLETOP_GEN_%s";'%(regtype))
            else:
                f.write('"HTX_BKGNP_SINGLETOP_GEN_%s"\n'%(regtype))

        f.write('Title: ')
        for regtype in regions.keys():
            if not regtype == regions.keys()[-1]:
                f.write('"Single-top gen. %s";'%(regtype))
            else:
                f.write('"Single-top gen. %s"\n'%(regtype))

    # received region list
    else:
        for reg in regions:
            if not reg == regions[-1]:
                f.write('"HTX_BKGNP_SINGLETOP_GEN_%s";'%(reg))
            else:
                f.write('"HTX_BKGNP_SINGLETOP_GEN_%s"\n'%(reg))

        f.write('Title: ')
        for reg in regions:
            if not reg == regions[-1]:
                f.write('"Single-top gen. %s";'%(getRegionLabel(reg)))
            else:
                f.write('"Single-top gen. %s"\n'%(getRegionLabel(reg)))

    f.write('Type: HISTO\n')
    f.write('Category: "Single-top generator uncertainties"\n')
    f.write('Symmetrisation: ONESIDED\n')
    f.write('Smoothing: 40\n')
    f.write('Samples: Singletop\n')
    f.write('HistoFilesUp: TtStAlt/SingletopaMCPy.mc16a,TtStAlt/SingletopaMCPy.mc16d,TtStAlt/SingletopaMCPy.mc16e\n')
    f.write('Regions: ')

    # received region dict
    if type(regions)==dict:
        for regtype in regions.keys():
            for reg in regions[regtype]:
                if not reg == regions[regtype][-1]:
                    f.write(reg+',')
                else:
                    f.write(reg)
            if not regtype == regions.keys()[-1]:
                f.write(';')
            else:
                f.write('\n')

    # received region list
    else:
        for reg in regions:
            if not reg == regions[-1]:
                f.write(reg+';')
            else:
                f.write(reg+'\n')
    f.write('ReferenceSample: Singletop_AFII\n')
    f.write('\n')

##_______________________________________________________________
##
def writePS_singletop(f,regions):

    f.write('Systematic: ')

    # received region dict
    if type(regions)==dict:
        for regtype in regions.keys():
            if not regtype == regions.keys()[-1]:
                f.write('"HTX_BKGNP_SINGLETOP_PS_%s";'%(regtype))
            else:
                f.write('"HTX_BKGNP_SINGLETOP_PS_%s"\n'%(regtype))

        f.write('Title: ')
        for regtype in regions.keys():
            if not regtype == regions.keys()[-1]:
                f.write('"Single-top ps. %s";'%(regtype))
            else:
                f.write('"Single-top ps. %s"\n'%(regtype))

    # received region list
    else:
        for reg in regions:
            if not reg == regions[-1]:
                f.write('"HTX_BKGNP_SINGLETOP_PS_%s";'%(reg))
            else:
                f.write('"HTX_BKGNP_SINGLETOP_PS_%s"\n'%(reg))

        f.write('Title: ')
        for reg in regions:
            if not reg == regions[-1]:
                f.write('"Single-top ps. %s";'%(getRegionLabel(reg)))
            else:
                f.write('"Single-top ps. %s"\n'%(getRegionLabel(reg)))

    f.write('Type: HISTO\n')
    f.write('Category: "Single-top parton shower uncertainties"\n')
    f.write('Symmetrisation: ONESIDED\n')
    f.write('Smoothing: 40\n')
    f.write('Samples: Singletop\n')
    f.write('HistoFilesUp: TtStAlt/SingletopPowHer.mc16a,TtStAlt/SingletopPowHer.mc16d,TtStAlt/SingletopPowHer.mc16e\n')
    f.write('Regions: ')

    # received region dict
    if type(regions)==dict:
        for regtype in regions.keys():
            for reg in regions[regtype]:
                if not reg == regions[regtype][-1]:
                    f.write(reg+',')
                else:
                    f.write(reg)
            if not regtype == regions.keys()[-1]:
                f.write(';')
            else:
                f.write('\n')

    # received region list
    else:
        for reg in regions:
            if not reg == regions[-1]:
                f.write(reg+';')
            else:
                f.write(reg+'\n')
    f.write('ReferenceSample: Singletop_AFII\n')
    f.write('\n')

##_______________________________________________________________
##
def writeDRDS(f,regions):
    flavs = [['ttbarlight','t#bar{t}+light'],['ttbarcc','t#bar{t}+#geq1c'],['ttbarbb','t#bar{t}+#geq1b'],['Singletop','Single-top']]

    for flav in flavs:
        f.write('Systematic: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"HTX_BKGNP_%s_DRDS_%s";'%(flav[0].upper(),regtype))
                else:
                    f.write('"HTX_BKGNP_%s_DRDS_%s"\n'%(flav[0].upper(),regtype))

            f.write('NuisanceParameter: ')
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"HTX_BKGNP_DRDS_%s";'%(regtype))
                else:
                    f.write('"HTX_BKGNP_DRDS_%s"\n'%(regtype))
            f.write('Title: ')
            for regtype in regions.keys():
                if not regtype == regions.keys()[-1]:
                    f.write('"%s DRDS %s";'%(flav[1],regtype))
                else:
                    f.write('"%s DRDS %s"\n'%(flav[1],regtype))

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"HTX_BKGNP_%s_DRDS_%s";'%(flav[0].upper(),reg))
                else:
                    f.write('"HTX_BKGNP_%s_DRDS_%s"\n'%(flav[0].upper(),reg))

            f.write('NuisanceParameter: ')
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"HTX_BKGNP_DRDS_%s";'%(reg))
                else:
                    f.write('"HTX_BKGNP_DRDS_%s"\n'%(reg))
            f.write('Title: ')
            for reg in regions:
                if not reg == regions[-1]:
                    f.write('"%s DR/DS %s";'%(flav[1],getRegionLabel(reg)))
                else:
                    f.write('"%s DR/DS %s"\n'%(flav[1],getRegionLabel(reg)))

        f.write('Type: HISTO\n')
        f.write('Category: "Single-top DR-DS uncertainties"\n')
        f.write('Symmetrisation: ONESIDED\n')
        f.write('Smoothing: 40\n')
        f.write('Samples: %s\n'%flav[0])
        if(flav[0]=="Singletop"):
            f.write('HistoFilesUp: RWDS/%sDiagSub.mc16a,RWDS/%sDiagSub.mc16d,RWDS/%sDiagSub.mc16e\n'%(flav[0],flav[0],flav[0]))
        else:
            f.write('HistoFilesUp: RWDS/%s.mc16a,RWDS/%s.mc16d,RWDS/%s.mc16e\n'%(flav[0],flav[0],flav[0]))
        f.write('Regions: ')

        # received region dict
        if type(regions)==dict:
            for regtype in regions.keys():
                for reg in regions[regtype]:
                    if not reg == regions[regtype][-1]:
                        f.write(reg+',')
                    else:
                        f.write(reg)
                if not regtype == regions.keys()[-1]:
                    f.write(';')
                else:
                    f.write('\n')

        # received region list
        else:
            for reg in regions:
                if not reg == regions[-1]:
                    f.write(reg+';')
                else:
                    f.write(reg+'\n')
        f.write('\n')

#def writeDRDS_singletop(f,regions):
#
#    f.write('Systematic: ')
#
#    # received region dict
#    if type(regions)==dict:
#        for regtype in regions.keys():
#            if not regtype == regions.keys()[-1]:
#                f.write('"HTX_BKGNP_SINGLETOP_DRDS_%s";'%(regtype))
#            else:
#                f.write('"HTX_BKGNP_SINGLETOP_DRDS_%s"\n'%(regtype))
#
#        f.write('Title: ')
#        for regtype in regions.keys():
#            if not regtype == regions.keys()[-1]:
#                f.write('"Single-top DR/DS %s";'%(regtype))
#            else:
#                f.write('"Single-top DR/DS %s"\n'%(regtype))
#
#    # received region list
#    else:
#        for reg in regions:
#            if not reg == regions[-1]:
#                f.write('"HTX_BKGNP_SINGLETOP_DRDS_%s";'%(reg))
#            else:
#                f.write('"HTX_BKGNP_SINGLETOP_DRDS_%s"\n'%(reg))
#
#        f.write('Title: ')
#        for reg in regions:
#            if not reg == regions[-1]:
#                f.write('"Single-top DR/DS %s";'%(getRegionLabel(reg)))
#            else:
#                f.write('"Single-top DR/DS %s"\n'%(getRegionLabel(reg)))
#
#    f.write('Type: HISTO\n')
#    f.write('Category: "Single-top DR-DS uncertainties"\n')
#    f.write('Symmetrisation: ONESIDED\n')
#    f.write('Smoothing: 40\n')
#    f.write('Samples: Singletop\n')
#    f.write('HistoFilesUp: TtStAlt/SingletopDiagSub.mc16a,TtStAlt/SingletopDiagSub.mc16d,TtStAlt/SingletopDiagSub.mc16e\n')
#    f.write('Regions: ')
#
#    # received region dict
#    if type(regions)==dict:
#        for regtype in regions.keys():
#            for reg in regions[regtype]:
#                if not reg == regions[regtype][-1]:
#                    f.write(reg+',')
#                else:
#                    f.write(reg)
#            if not regtype == regions.keys()[-1]:
#                f.write(';')
#            else:
#                f.write('\n')
#
#    # received region list
#    else:
#        for reg in regions:
#            if not reg == regions[-1]:
#                f.write(reg+';')
#            else:
#                f.write(reg+'\n')
#    f.write('\n')
#
##_______________________________________________________________
##
def writeVar3c_singletop(f,regions):

    f.write('Systematic: ')

    # received region dict
    if type(regions)==dict:
        for regtype in regions.keys():
            if not regtype == regions.keys()[-1]:
                f.write('"HTX_BKGNP_SINGLETOP_ISR_VAR3C_%s";'%(regtype))
            else:
                f.write('"HTX_BKGNP_SINGLETOP_ISR_VAR3C_%s"\n'%(regtype))

        f.write('Title: ')
        for regtype in regions.keys():
            if not regtype == regions.keys()[-1]:
                f.write('"Single-top isr Var3c %s";'%(regtype))
            else:
                f.write('"Single-top isr Var3c %s"\n'%(regtype))

    # received region list
    else:
        for reg in regions:
            if not reg == regions[-1]:
                f.write('"HTX_BKGNP_SINGLETOP_ISR_VAR3C_%s";'%(reg))
            else:
                f.write('"HTX_BKGNP_SINGLETOP_ISR_VAR3C_%s"\n'%(reg))

        f.write('Title: ')
        for reg in regions:
            if not reg == regions[-1]:
                f.write('"Single-top isr Var3c %s";'%(getRegionLabel(reg)))
            else:
                f.write('"Single-top isr Var3c %s"\n'%(getRegionLabel(reg)))

    f.write('Type: HISTO\n')
    f.write('Category: "Single-top isr Var3c uncertainties"\n')
    f.write('HistoNameSufUp: _weight_pmg_Var3cUp\n')
    f.write('HistoNameSufDown: _weight_pmg_Var3cDown\n')
    f.write('HistoPathUp: "__HISTOPATH__/TheorySys/"\n')
    f.write('HistoPathDown: "__HISTOPATH__/TheorySys/"\n')
    f.write('ReferenceSample: Singletop_Ref\n')
    f.write('Symmetrisation: TwoSided\n')
    f.write('Smoothing: 40\n')
    f.write('Samples: Singletop\n')
    f.write('Regions: ')

    # received region dict
    if type(regions)==dict:
        for regtype in regions.keys():
            for reg in regions[regtype]:
                if not reg == regions[regtype][-1]:
                    f.write(reg+',')
                else:
                    f.write(reg)
            if not regtype == regions.keys()[-1]:
                f.write(';')
            else:
                f.write('\n')

    # received region list
    else:
        for reg in regions:
            if not reg == regions[-1]:
                f.write(reg+';')
            else:
                f.write(reg+'\n')
    f.write('\n')

##_______________________________________________________________
##
def writeISR_MUR_singletop(f,regions):

    f.write('Systematic: ')

    # received region dict
    if type(regions)==dict:
        for regtype in regions.keys():
            if not regtype == regions.keys()[-1]:
                f.write('"HTX_BKGNP_SINGLETOP_ISR_MUR_%s";'%(regtype))
            else:
                f.write('"HTX_BKGNP_SINGLETOP_ISR_MUR_%s"\n'%(regtype))

        f.write('Title: ')
        for regtype in regions.keys():
            if not regtype == regions.keys()[-1]:
                f.write('"Single-top isr #mu_{R} %s";'%(regtype))
            else:
                f.write('"Single-top isr #mu_{R} %s"\n'%(regtype))

    # received region list
    else:
        for reg in regions:
            if not reg == regions[-1]:
                f.write('"HTX_BKGNP_SINGLETOP_ISR_MUR_%s";'%(reg))
            else:
                f.write('"HTX_BKGNP_SINGLETOP_ISR_MUR_%s"\n'%(reg))

        f.write('Title: ')
        for reg in regions:
            if not reg == regions[-1]:
                f.write('"Single-top isr #mu_{R} %s";'%(getRegionLabel(reg)))
            else:
                f.write('"Single-top isr #mu_{R} %s"\n'%(getRegionLabel(reg)))

    f.write('Type: HISTO\n')
    f.write('Category: "Single-top isr #mu_{R} uncertainties"\n')
    f.write('HistoNameSufUp: _weight_pmg_muR20__muF10\n')
    f.write('HistoNameSufDown: _weight_pmg_muR05__muF10\n')
    f.write('HistoPathUp: "__HISTOPATH__/TheorySys/"\n')
    f.write('HistoPathDown: "__HISTOPATH__/TheorySys/"\n')
    f.write('ReferenceSample: Singletop_Ref\n')
    f.write('Symmetrisation: TwoSided\n')
    f.write('Smoothing: 40\n')
    f.write('Samples: Singletop\n')
    f.write('Regions: ')

    # received region dict
    if type(regions)==dict:
        for regtype in regions.keys():
            for reg in regions[regtype]:
                if not reg == regions[regtype][-1]:
                    f.write(reg+',')
                else:
                    f.write(reg)
            if not regtype == regions.keys()[-1]:
                f.write(';')
            else:
                f.write('\n')

    # received region list
    else:
        for reg in regions:
            if not reg == regions[-1]:
                f.write(reg+';')
            else:
                f.write(reg+'\n')
    f.write('\n')

##_______________________________________________________________
##
def writeISR_MUF_singletop(f,regions):

    f.write('Systematic: ')

    # received region dict
    if type(regions)==dict:
        for regtype in regions.keys():
            if not regtype == regions.keys()[-1]:
                f.write('"HTX_BKGNP_SINGLETOP_ISR_MUF_%s";'%(regtype))
            else:
                f.write('"HTX_BKGNP_SINGLETOP_ISR_MUF_%s"\n'%(regtype))

        f.write('Title: ')
        for regtype in regions.keys():
            if not regtype == regions.keys()[-1]:
                f.write('"Single-top isr #mu_{F} %s";'%(regtype))
            else:
                f.write('"Single-top isr #mu_{F} %s"\n'%(regtype))

    # received region list
    else:
        for reg in regions:
            if not reg == regions[-1]:
                f.write('"HTX_BKGNP_SINGLETOP_ISR_MUF_%s";'%(reg))
            else:
                f.write('"HTX_BKGNP_SINGLETOP_ISR_MUF_%s"\n'%(reg))

        f.write('Title: ')
        for reg in regions:
            if not reg == regions[-1]:
                f.write('"Single-top isr #mu_{F} %s";'%(getRegionLabel(reg)))
            else:
                f.write('"Single-top isr #mu_{F} %s"\n'%(getRegionLabel(reg)))
                
    f.write('Type: HISTO\n')
    f.write('Category: "Single-top isr #mu_{F} uncertainties"\n')
    f.write('HistoNameSufUp: _weight_pmg_muR10__muF20\n')
    f.write('HistoNameSufDown: _weight_pmg_muR10__muF05\n')
    f.write('HistoPathUp: "__HISTOPATH__/TheorySys/"\n')
    f.write('HistoPathDown: "__HISTOPATH__/TheorySys/"\n')
    f.write('ReferenceSample: Singletop_Ref\n')
    f.write('Symmetrisation: TwoSided\n')
    f.write('Smoothing: 40\n')
    f.write('Samples: Singletop\n')
    f.write('Regions: ')

    # received region dict
    if type(regions)==dict:
        for regtype in regions.keys():
            for reg in regions[regtype]:
                if not reg == regions[regtype][-1]:
                    f.write(reg+',')
                else:
                    f.write(reg)
            if not regtype == regions.keys()[-1]:
                f.write(';')
            else:
                f.write('\n')

    # received region list
    else:
        for reg in regions:
            if not reg == regions[-1]:
                f.write(reg+';')
            else:
                f.write(reg+'\n')
    f.write('\n')

##_______________________________________________________________
##
def writeFSR_MURF_singletop(f,regions):

    f.write('Systematic: ')

    # received region dict
    if type(regions)==dict:
        for regtype in regions.keys():
            if not regtype == regions.keys()[-1]:
                f.write('"HTX_BKGNP_SINGLETOP_FSR_MURF_%s";'%(regtype))
            else:
                f.write('"HTX_BKGNP_SINGLETOP_FSR_MURF_%s"\n'%(regtype))

        f.write('Title: ')
        for regtype in regions.keys():
            if not regtype == regions.keys()[-1]:
                f.write('"Single-top fsr #mu_{R,F} %s";'%(regtype))
            else:
                f.write('"Single-top fsr #mu_{R,F} %s"\n'%(regtype))

    # received region list
    else:
        for reg in regions:
            if not reg == regions[-1]:
                f.write('"HTX_BKGNP_SINGLETOP_FSR_MURF_%s";'%(reg))
            else:
                f.write('"HTX_BKGNP_SINGLETOP_FSR_MURF_%s"\n'%(reg))

        f.write('Title: ')
        for reg in regions:
            if not reg == regions[-1]:
                f.write('"Single-top fsr #mu_{R,F} %s";'%(getRegionLabel(reg)))
            else:
                f.write('"Single-top fsr #mu_{R,F} %s"\n'%(getRegionLabel(reg)))

    f.write('Type: HISTO\n')
    f.write('Category: "Single-top fsr #mu_{R-F} uncertainties"\n')
    f.write('HistoNameSufUp: _weight_pmg_isr_muRfac10__fsr_muRfac20\n')
    f.write('HistoNameSufDown: _weight_pmg_isr_muRfac10__fsr_muRfac05\n')
    f.write('HistoPathUp: "__HISTOPATH__/TheorySys/"\n')
    f.write('HistoPathDown: "__HISTOPATH__/TheorySys/"\n')
    f.write('ReferenceSample: Singletop_Ref\n')
    f.write('Symmetrisation: TwoSided\n')
    f.write('Smoothing: 40\n')
    f.write('Samples: Singletop\n')
    f.write('Regions: ')

    # received region dict
    if type(regions)==dict:
        for regtype in regions.keys():
            for reg in regions[regtype]:
                if not reg == regions[regtype][-1]:
                    f.write(reg+',')
                else:
                    f.write(reg)
            if not regtype == regions.keys()[-1]:
                f.write(';')
            else:
                f.write('\n')

    # received region list
    else:
        for reg in regions:
            if not reg == regions[-1]:
                f.write(reg+';')
            else:
                f.write(reg+'\n')
    f.write('\n')

##_______________________________________________________________
##_______________________________________________________________
##

def writeGENPS(f,regions):
    f.write('% ------------------------------------- %\n')
    f.write('% ---  TTBAR MODELLING SYSTEMATICS  --- %\n')
    f.write('% ------------------------------------- %\n')
    f.write('\n')

    writeGen_ttbar(f,regions)
    writePS_ttbar(f,regions)

    f.write('% ------------------------------------------ %\n')
    f.write('% ---  SINGLE-TOP MODELLING SYSTEMATICS  --- %\n')
    f.write('% ------------------------------------------ %\n')
    f.write('\n')

    writeGen_singletop(f,regions)
    writePS_singletop(f,regions)
    writeDRDS(f,regions)

##_______________________________________________________________
##

def writeGENPS_ttbarOnly(f,regions):
    f.write('% ------------------------------------- %\n')
    f.write('% ---  TTBAR MODELLING SYSTEMATICS  --- %\n')
    f.write('% ------------------------------------- %\n')
    f.write('\n')

    writeGen_ttbar(f,regions)
    writePS_ttbar(f,regions)

##_______________________________________________________________
##

def writeGENPS_singletopOnly(f,regions):
    f.write('% ------------------------------------------ %\n')
    f.write('% ---  SINGLE-TOP MODELLING SYSTEMATICS  --- %\n')
    f.write('% ------------------------------------------ %\n')
    f.write('\n')

    writeGen_singletop(f,regions)
    writePS_singletop(f,regions)
    writeDRDS(f,regions)

##_______________________________________________________________
##
def writePMG_ttbar(f,regions):
    f.write('% -------------------------------------------------- %\n')
    f.write('% --------   TTBAR PMG WEIGHTS SYSTEMATICS  -------- %\n')
    f.write('% -------------------------------------------------- %\n')
    f.write('\n')

    writeVar3c_ttbar(f,regions)
    writeISR_MUR_ttbar(f,regions)
    writeISR_MUF_ttbar(f,regions)
    writeFSR_MURF_ttbar(f,regions)

##_______________________________________________________________
##
def writePMG_singletop(f,regions):
    f.write('% ------------------------------------------------------ %\n')
    f.write('% --------   SINGLETOP PMG WEIGHTS SYSTEMATICS  -------- %\n')
    f.write('% ------------------------------------------------------ %\n')
    f.write('\n')

    writeVar3c_singletop(f,regions)
    writeISR_MUR_singletop(f,regions)
    writeISR_MUF_singletop(f,regions)
    writeFSR_MURF_singletop(f,regions)

##_______________________________________________________________
# Decorrelate among ALL REGIONS
##

decorAllRegions = True
if decorAllRegions:
    with open('TEMPLATE_Systematics_ttstMod_decor_ALLREGIONS.txt','w') as f:
        writeGENPS(f,regions)
    with open('TEMPLATE_Systematics_ttstMod_decor_ALLREGIONS_BLIND.txt','w') as f:
        writeGENPS(f,regions_blind)
    
    with open('TEMPLATE_Systematics_Weights_pmg_ttbar_decor_ALLREGIONS.txt','w') as f:
        writePMG_ttbar(f,regions)
    with open('TEMPLATE_Systematics_Weights_pmg_ttbar_decor_ALLREGIONS_BLIND.txt','w') as f:
        writePMG_ttbar(f,regions_blind)

    with open('TEMPLATE_Systematics_Weights_pmg_singletop_decor_ALLREGIONS.txt','w') as f:
        writePMG_singletop(f,regions)
    with open('TEMPLATE_Systematics_Weights_pmg_singletop_decor_ALLREGIONS_BLIND.txt','w') as f:
        writePMG_singletop(f,regions_blind)


##_______________________________________________________________
# Decorrelate among NJETS and NBOOSTED
##

decorNjetsNboosted = True
if decorNjetsNboosted:
    with open('TEMPLATE_Systematics_ttstMod_decor_NJETSBOOSTED.txt','w') as f:
        writeGENPS(f,regions_dict)
    with open('TEMPLATE_Systematics_ttstMod_decor_NJETSBOOSTED_BLIND.txt','w') as f:
        writeGENPS(f,regions_dict_blind)

    with open('TEMPLATE_Systematics_ttMod_decor_NJETSBOOSTED.txt','w') as f:
        writeGENPS_ttbarOnly(f,regions_dict)
    with open('TEMPLATE_Systematics_ttMod_decor_NJETSBOOSTED_BLIND.txt','w') as f:
        writeGENPS_ttbarOnly(f,regions_dict_blind)

    with open('TEMPLATE_Systematics_stMod_decor_NJETSBOOSTED.txt','w') as f:
        writeGENPS_singletopOnly(f,regions_dict)
    with open('TEMPLATE_Systematics_stMod_decor_NJETSBOOSTED_BLIND.txt','w') as f:
        writeGENPS_singletopOnly(f,regions_dict_blind)
    
    with open('TEMPLATE_Systematics_Weights_pmg_ttbar_decor_NJETSBOOSTED.txt','w') as f:
        writePMG_ttbar(f,regions_dict)
    with open('TEMPLATE_Systematics_Weights_pmg_ttbar_decor_NJETSBOOSTED_BLIND.txt','w') as f:
        writePMG_ttbar(f,regions_dict_blind)

    with open('TEMPLATE_Systematics_Weights_pmg_singletop_decor_NJETSBOOSTED.txt','w') as f:
        writePMG_singletop(f,regions_dict)
    with open('TEMPLATE_Systematics_Weights_pmg_singletop_decor_NJETSBOOSTED_BLIND.txt','w') as f:
        writePMG_singletop(f,regions_dict_blind)
