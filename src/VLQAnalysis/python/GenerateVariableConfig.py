from ROOT import TFile
import importlib

#f = TFile("/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NEWPROD_TRACKJET/2lep3jin0bexZwinMLL_SF_Zjets_NJets_Meff_2lep_1lep_extendted_bjets_reweighted_regions/merged_mc16a/outVLQAnalysis_Zjets_nominal_HIST.root")

#f = TFile("/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_v2/original/merged_mc16a/outVLQAnalysis_ttbarlight_nominal_HIST.root")

doBlind = True
doSR = False
doVR = True
doPresel = False

f = TFile("/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_tag-21.2.87-htztx-3-syst_v2/original/merged_mc16a/outVLQAnalysis_Zjets_nominal_HIST.root")
#f = TFile("/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_tag-21.2.87-htztx-3-syst_v2/original/sVLQ_signal/outVLQAna_sVLQ_outVLQAna_sVLQ_WTHt20K05_310777.mc16a_nominal_0.root")

#f = TFile("/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/VLQAnalysisFramework_meffred/original/merged_mc16a/outVLQAnalysis_Zjets_nominal_HIST.root")

#f = TFile("/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020/original/merged_mc16a/outVLQAnalysis_Zjets_nominal_HIST.root") 

varList = {"RCMHiggs_jets_eta", "RCMHiggs_jets_m", "RCMHiggs_jets_n", "RCMHiggs_jets_pt", "RCMTop_jets_eta", "RCMTop_jets_m", "RCMTop_jets_n", "RCMTop_jets_pt", "RCMV_jets_m", "RCMV_jets_n", 
           "RCMV_jets_pt", "RCjet0_eta", "RCjet0_m", "RCjet0_nconsts", "RCjet0_pt", "RCjets_eta", "RCjets_m", "RCjets_n", "RCjets_nconsts", "RCjets_pt", "trkbjet0_btagw", "trkbjet0_eta", "trkbjet0_pt", "trkbjets_btagw", 
           "trkbjets_eta", "trkbjets_n", "trkbjets_pt", "fwdjet0_eta", "fwdjet0_m", "fwdjet0_pt", "fwdjets_eta", "fwdjets_m", "fwdjets_n", "fwdjets_pt", "htall", "hthad", "htred", 
           "jet0_btagw", "jet0_eta", "jet0_m", "jet0_phi", "jet0_pt", "jets_btagw", "jets_eta", "jets_m", "jets_n", "jets_phi", "jets_pt", "lep0_pt", "meff", "meffred", "met", "met_phi", "mtbmin",
           "mtw", "mll","mu_n", "el_n","metsig_obj"}

varList = {"meff", "meffred"}

regList = {#"c2lep3jin1binZwinMLL_sf","c2lep3jex1binZwinMLL_sf","c2lep4jex1binZwinMLL_sf","c2lep5jex1binZwinMLL_sf","c2lep6jex1binZwinMLL_sf",
           #"c2lep7jex1binZwinMLL_sf","c2lep8jin1binZwinMLL_sf",
           #"c2lep3jin0bexZwinMLL_sf","c2lep3jex0bexZwinMLL_sf","c2lep4jex0bexZwinMLL_sf","c2lep5jex0bexZwinMLL_sf","c2lep6jex0bexZwinMLL_sf",
           #"c2lep7jex0bexZwinMLL_sf",
           #"c2lep8jin0bexZwinMLL_sf",
           #"c2lep3jin1bexZwinMLL_sf","c2lep3jex1bexZwinMLL_sf","c2lep4jex1bexZwinMLL_sf","c2lep5jex1bexZwinMLL_sf","c2lep6jex1bexZwinMLL_sf",
           #"c2lep7jex1bexZwinMLL_sf",
           #"c2lep8jin1bexZwinMLL_sf",
           #"c2lep3jin2binZwinMLL_sf","c2lep3jex2binZwinMLL_sf","c2lep4jex2binZwinMLL_sf","c2lep5jex2binZwinMLL_sf","c2lep6jex2binZwinMLL_sf",
           #"c2lep7jin2binZwinMLL_sf",
           #"c2lep3jin1bexZwinMLL_ee","c2lep3jex1bexZwinMLL_ee","c2lep4jex1bexZwinMLL_ee","c2lep5jex1bexZwinMLL_ee","c2lep6jex1bexZwinMLL_ee",
           #"c2lep7jex1bexZwinMLL_ee","c2lep8jin1bexZwinMLL_ee",
           #"c2lep3jin1bexZwinMLL_mumu","c2lep3jex1bexZwinMLL_mumu","c2lep4jex1bexZwinMLL_mumu","c2lep5jex1bexZwinMLL_mumu","c2lep6jex1bexZwinMLL_mumu",
           #"c2lep7jex1bexZwinMLL_mumu","c2lep8jin1bexZwinMLL_mumu",





           #"c1lep3jin0bex","c1lep3jex0bex", "c1lep4jex0bex", "c1lep5jex0bex", "c1lep6jex0bex", "c1lep7jex0bex", #"c1lep8jin0bex",
           #"c1lep3jin1bex","c1lep3jex1bex", "c1lep4jex1bex", "c1lep5jex1bex", "c1lep6jex1bex", "c1lep7jex1bex", "c1lep7jin1bex",
           #"c1lep3jin2bex","c1lep3jex2bex", "c1lep4jex2bex", "c1lep5jex2bex", "c1lep6jex2bex", "c1lep7jex2bex", "c1lep8jin2bex",
           #"c1lep3jin1bex0fjex","c1lep3jex1bex0fjex", "c1lep4jex1bex0fjex", "c1lep5jex1bex0fjex", "c1lep6jex1bex0fjex", "c1lep7jex1bex0fjex", "c1lep8jin1bex0fjex",
           #"c1lep7jin2bex","c1lep8jex2bex","c1lep9jex2bex", "c1lep10jex2bex", "c1lep11jex2bex",
           #"c1lep3jin1bin","c1lep3jin2bin",
           #"c1lep3_5jwin1bin", "c1lep6jin2bin",

           "c1lep3_5jwin1bex1fjin0LTex0Hex1Vin", "c1lep3_5jwin1bex1fjin0Tex1Lin0Hex1Vin", "c1lep3_5jwin2bex1fjin0LTex0Hex1Vin", "c1lep3_5jwin2bex1fjin0Tex1Lin0Hex1Vin",
           "c1lep3_5jwin3bex1fjin0LTex1Hin0Vex", "c1lep3_5jwin3bex1fjin0Tex1Lin1Hin0Vex", "c1lep3_5jwin4bin1fjin0LTex1Hin0Vex", "c1lep3_5jwin4bin1fjin0Tex1Lin1Hin0Vex",
           "c1lep6jin1bex1fjin1Lex0Tex0Hex1Vin", "c1lep6jin1bex1fjin0Lex1Tex0Hex1Vin", "c1lep6jin1bex1fjin2LTin0Hex1Vin", "c1lep6jin2bex1fjin1Lex0Tex0Hex1Vin",
           "c1lep6jin2bex1fjin0Lex1Tex0Hex1Vin", "c1lep6jin2bex1fjin2LTin0Hex1Vin", "c1lep6jin3bex1fjin1Lex0VTex1Hin", "c1lep6jin3bex1fjin0Lex1VTex1Hin",
           "c1lep6jin3bex1fjin2VLTin1Hin", "c1lep6jin4bin1fjin1Lex0VTex1Hin", "c1lep6jin4bin1fjin0Lex1VTex1Hin", "c1lep6jin4bin1fjin2VLTin1Hin",



           "c1lep3_5jwin1bex0fjex0Tex0Lex0Hex1Vin", "c1lep3_5jwin1bex0fjex0Tex1Lin0Hex1Vin", "c1lep3_5jwin1bex1fjin1LTin0Hex0Vex", "c1lep3_5jwin1bex1fjin1Tin0Lex0Hex1Vin", 
           "c1lep3_5jwin2bex0fjex0Tex0Lex0Hex1Vin", "c1lep3_5jwin2bex0fjex0Tex1Lin0Hex1Vin", "c1lep3_5jwin2bex1fjin1LTin0Hex0Vex", "c1lep3_5jwin2bex1fjin1Tin0Lex0Hex1Vin",
           "c1lep3_5jwin3bex0fjex0Tex0Lex1Hin0Vex", "c1lep3_5jwin3bex0fjex0Tex1Lin1Hin0Vex", "c1lep3_5jwin3bex1fjin1Tin0Lex1Hin0Vex","c1lep3_5jwin3bex1fjin1VLTin0Hex",
           "c1lep3_5jwin4bin0fjex0Tex0Lex1Hin0Vex", "c1lep3_5jwin4bin0fjex0Tex1Lin1Hin0Vex", "c1lep3_5jwin4bin1fjin1Tin0Lex1Hin0Vex","c1lep3_5jwin4bin1fjin1VLTin0Hex",
           "c1lep6jin1bex0fjex1LTex0Hex1Vin", "c1lep6jin1bex0fjex2LTin0Hex1Vin", "c1lep6jin1bex1fjin0Tex0Lex1Hin1Vin", "c1lep6jin1bex1fjin2LTin1Hin0Vex",
           "c1lep6jin2bex0fjex1LTex0Hex1Vin", "c1lep6jin2bex0fjex2LTin0Hex1Vin", "c1lep6jin2bex1fjin0Tex0Lex1Hin1Vin", "c1lep6jin2bex1fjin2LTin1Hin0Vex",
           "c1lep6jin3bex0fjex1VLTex1Hin", "c1lep6jin3bex0fjex2VLTin1Hin", "c1lep6jin3bex1fjin1VLTex0Hex", "c1lep6jin3bex1fjin2VLTin0Hex", 
           "c1lep6jin4bin0fjex1VLTex1Hin", "c1lep6jin4bin0fjex2VLTin1Hin","c1lep6jin4bin1fjin1VLTex0Hex", "c1lep6jin4bin1fjin2VLTin0Hex"}



regList = {"c1lep3jin2bex"}

#regList = {"c1lep3jex2bex1fjin", "c1lep4jex2bex1fjin", "c1lep5jex2bex1fjin", "c1lep6jex2bex1fjin", "c1lep7jex2bex1fjin", "c1lep8jex2bex1fjin", "c1lep9jex2bex1fjin","c1lep10jex2bex1fjin",
#           "c1lep3jex2bex0fjex", "c1lep4jex2bex0fjex", "c1lep5jex2bex0fjex", "c1lep6jex2bex0fjex", "c1lep7jex2bex0fjex", "c1lep8jex2bex0fjex", "c1lep9jex2bex0fjex","c1lep10jex2bex0fjex"}

# Get regions from regions_dictionary_sVLQ
regDictModule = "regions_dictionary_sVLQ_blinded"
regModule = importlib.import_module(regDictModule)
regList = []
rebinDict = {}
regDict = []

# add proper regions to regDict
if doSR:
    regDict += regModule.fit_regions_1l
if doVR:
    regDict += regModule.validation_regions_1l
if doPresel:
    regDict += regModule.preselection_regions_1l

# loop through regDict and add regions to regList and binning to rebinDict
for fr in regDict:

    # retrieve region name from regions dictionary
    regname = fr['name'].replace('HTX_','')
    
    if doBlind:

        # no 'binning_blind' available for all regions, so use 'binning' in that case
        try:
          binning = fr['binning_blind']
        except KeyError:
          binning = fr['binning']

        # don't use region if fully blinded
        if binning == '':
          continue

        # otherwise store binning in rebinDict
        rebinDict[regname] = binning
    else:
        rebinDict[regname] = fr['binning']

    # append region to regList (only happens if not fully blinded)
    regList.append(regname)

varConfig = open("variableConfig.txt", "w+")

varConfig.write("BEGIN\n")

for var in varList:

    for reg in regList:
        
        fullName = reg+"_"+var

        if(f.GetListOfKeys().Contains(fullName)):
            varConfig.write("NEW\n")
            varConfig.write("NAME : "+fullName+"\n")
            varConfig.write("DRAWSTACK : TRUE\n")
            varConfig.write("DRAWRES : RATIO\n")
            varConfig.write("DRAWRESSTACK : TRUE\n")
            varConfig.write("DOSCALE : NORM\n")
            varConfig.write("RESMIN : 0.\n")
            varConfig.write("RESMAX : 1.6\n")
            varConfig.write("YMIN   : 0.\n")
            varConfig.write("DOWIDTH : TRUE\n")
            if('meff' in fullName):
                varConfig.write("REBINVAR : "+rebinDict[reg]+"\n")

            if(reg == "c1lep3jex2bex1fjin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=3j, =2b, #geq1fj, 1l}\n")
            elif(reg == "c1lep4jex2bex1fjin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=4j, =2b, #geq1fj, 1l}\n")
            elif(reg == "c1lep5jex2bex1fjin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=5j, =2b, #geq1fj, 1l}\n")
            elif(reg == "c1lep6jex2bex1fjin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=6j, =2b, #geq1fj, 1l}\n")
            elif(reg == "c1lep7jex2bex1fjin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=7j, =2b, #geq1fj, 1l}\n")
            elif(reg == "c1lep8jex2bex1fjin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=8j, =2b, #geq1fj, 1l}\n")
            elif(reg == "c1lep9jex2bex1fjin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=9j, =2b, #geq1fj, 1l}\n")
            elif(reg == "c1lep10jex2bex1fjin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=10j, =2b, #geq1fj, 1l}\n")
            elif(reg == "c1lep3jex2bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=3j, =2b, 0fj, 1l}\n")
            elif(reg == "c1lep4jex2bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=4j, =2b, 0fj, 1l}\n")
            elif(reg == "c1lep5jex2bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=5j, =2b, 0fj, 1l}\n")
            elif(reg == "c1lep6jex2bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=6j, =2b, 0fj, 1l}\n")
            elif(reg == "c1lep7jex2bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=7j, =2b, 0fj, 1l}\n")
            elif(reg == "c1lep8jex2bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=8j, =2b, 0fj, 1l}\n")
            elif(reg == "c1lep9jex2bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=9j, =2b, 0fj, 1l}\n")
            elif(reg == "c1lep10jex2bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=10j, =2b, =0fj, 1l}\n")
    
            elif(reg == "c1lep3jin0bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq3j, =0b, 1l}\n")
            elif(reg == "c1lep3jex0bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=3j, =0b, 1l}\n")
            elif(reg == "c1lep4jex0bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=4j, =0b, 1l}\n")
            elif(reg == "c1lep5jex0bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=5j, =0b, 1l}\n")
            elif(reg == "c1lep6jex0bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=6j, =0b, 1l}\n")
            elif(reg == "c1lep7jex0bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=7j, =0b, 1l}\n")
            elif(reg == "c1lep8jin0bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq8j, =0b, 1l}\n")


            elif(reg == "c1lep3jin1bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq3j, =1b, 1l}\n")
            elif(reg == "c1lep3jex1bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=3j, =1b, 1l}\n")
            elif(reg == "c1lep4jex1bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=4j, =1b, 1l}\n")
            elif(reg == "c1lep5jex1bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=5j, =1b, 1l}\n")
            elif(reg == "c1lep6jex1bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=6j, =1b, 1l}\n")
            elif(reg == "c1lep7jex1bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=7j, =1b, 1l}\n")
            elif(reg == "c1lep8jin1bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq8j, =1b, 1l}\n")
            elif(reg == "c1lep3jin1bexHighMetSig"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq3j, =1b, 1l, met sig > 5}\n")



            elif(reg == "c1lep3jin1bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq3j, =1b, =0fj, 1l}\n")
            elif(reg == "c1lep3jex1bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=3j, =1b, =0fj, 1l}\n")
            elif(reg == "c1lep4jex1bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=4j, =1b, =0fj, 1l}\n")
            elif(reg == "c1lep5jex1bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=5j, =1b, =0fj, 1l}\n")
            elif(reg == "c1lep6jex1bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=6j, =1b, =0fj, 1l}\n")
            elif(reg == "c1lep7jex1bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=7j, =1b, =0fj, 1l}\n")
            elif(reg == "c1lep8jin1bex0fjex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq8j, =1b, =0fj, 1l}\n")



            elif(reg == "c1lep3jin2bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq3j, =2b, 1l}\n")
            elif(reg == "c1lep3jex2bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=3j, =2b, 1l}\n")
            elif(reg == "c1lep4jex2bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=4j, =2b, 1l}\n")
            elif(reg == "c1lep5jex2bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=5j, =2b, 1l}\n")
            elif(reg == "c1lep6jex2bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=6j, =2b, 1l}\n")
            elif(reg == "c1lep7jex2bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=7j, =2b, 1l}\n")
            elif(reg == "c1lep8jex2bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=8j, =2b, 1l}\n")
            elif(reg == "c1lep9jex2bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=9j, =2b, 1l}\n")
            elif(reg == "c1lep10jex2bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=10j, =2b, 1l}\n")
            elif(reg == "c1lep11jex2bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=11j, =2b, 1l}\n")
            elif(reg == "c1lep7jin2bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq7j, =2b, 1l}\n")
            elif(reg == "c1lep8jin2bex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq8j, =2b, 1l}\n")
                
            elif(reg == "c1lep3_5jwin1bin"):
                varConfig.write("EXTRALABEL :  #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, #geq1b, 1l}\n")

            elif(reg =="c1lep6jin2bin"):
                varConfig.write("EXTRALABEL :  #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{6j, #geq2b, 1l}\n")

            elif(reg == "c1lep3jin1bin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq3j, #geq1b, 1l}\n")

            elif(reg == "c1lep3jin2bin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq3j, #geq2b, 1l}\n")

            ## SIGNAL REGIONS ##

            elif(reg == "c1lep3_5jwin1bex1fjin0LTex0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 1b, #geq1fj, 0LT, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep3_5jwin1bex1fjin0Tex1Lin0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 1b, #geq1fj, 0T, #geq1L, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep3_5jwin2bex1fjin0LTex0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 2b, #geq1fj, 0T, 0LT, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep3_5jwin2bex1fjin0Tex1Lin0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 2b, #geq1fj, 0T, #geq1L, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep3_5jwin3bex1fjin0LTex1Hin0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 3b, #geq1fj, 0LT, #geq1H, 0V, 1l}\n")
            elif(reg == "c1lep3_5jwin3bex1fjin0Tex1Lin1Hin0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 3b, #geq1fj, 0T, #geq1L, #geq1H, 0V, 1l}\n")
            elif(reg == "c1lep3_5jwin4bin1fjin0LTex1Hin0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, #geq4b, #geq1fj, 0T, 0LT, #geq1H, 0V, 1l}\n")
            elif(reg == "c1lep3_5jwin4bin1fjin0Tex1Lin1Hin0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, #geq4b, #geq1fj, 0T, #geq1L, #geq1H, 0V, 1l}\n")

            elif(reg == "c1lep6jin1bex1fjin1Lex0Tex0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 1b, #geq1fj, 0T, 1L, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin1bex1fjin0Lex1Tex0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 1b, #geq1fj, 1T, 0L, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin1bex1fjin2LTin0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 1b, #geq1fj, #geq2LT, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin2bex1fjin1Lex0Tex0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 2b, #geq1fj, 1L, 0T, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin2bex1fjin0Lex1Tex0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 2b, #geq1fj, 0L, 1T, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin2bex1fjin2LTin0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 2b, #geq1fj, #geq2LT, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin3bex1fjin1Lex0VTex1Hin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 3b, #geq1fj, 1L, #geq1H, 0VT, 1l}\n")
            elif(reg == "c1lep6jin3bex1fjin0Lex1VTex1Hin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 3b, #geq1fj, 0L, #geq1H, 1VT, 1l}\n")
            elif(reg == "c1lep6jin3bex1fjin2VLTin1Hin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 3b, #geq1fj, #geq1H, #geq2VLT, 1l}\n")
            elif(reg == "c1lep6jin4bin1fjin1Lex0VTex1Hin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, #geq4b, #geq1fj, 1L, #geq1H, 0VT, 1l}\n")
            elif(reg == "c1lep6jin4bin1fjin0Lex1VTex1Hin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, #geq4b, #geq1fj, 0L, #geq1H, 1VT, 1l}\n")
            elif(reg == "c1lep6jin4bin1fjin2VLTin1Hin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, #geq4b, #geq1fj, #geq1H, #geq2VLT, 1l}\n")


            ## VALIDATION REGIONS ##

            elif(reg == "c1lep3_5jwin1bex0fjex0Tex0Lex0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 1b, 0fj, 0T, 0L, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep3_5jwin1bex0fjex0Tex1Lin0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 1b, 0fj, 0T, #geq1L, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep3_5jwin1bex1fjin1LTin0Hex0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 1b, #geq1fj, 0T, #geq1LT, 0H, 0V, 1l}\n")
            elif(reg == "c1lep3_5jwin1bex1fjin1Tin0Lex0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 1b, #geq1fj, #geq1T, 0L, 0H, #geq1V, 1l}\n")

            elif(reg == "c1lep3_5jwin2bex0fjex0Tex0Lex0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 2b, 0fj, 0T, 0L, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep3_5jwin2bex0fjex0Tex1Lin0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 2b, 0fj, 0T, #geq1L, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep3_5jwin2bex1fjin1LTin0Hex0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 2b, #geq1fj, 0T, #geq1LT, 0H, 0V, 1l}\n")
            elif(reg == "c1lep3_5jwin2bex1fjin1Tin0Lex0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 2b, #geq1fj, #geq1T, 0L, 0H, #geq1V, 1l}\n")

                
            elif(reg == "c1lep3_5jwin3bex0fjex0Tex0Lex1Hin0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 3b, 0fj, 0T, 0L, #geq1H, 0V, 1l}\n")
            elif(reg == "c1lep3_5jwin3bex0fjex0Tex1Lin1Hin0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 3b, 0fj, 0T, #geq1L, #geq1H, 0V, 1l}\n")
            elif(reg == "c1lep3_5jwin3bex1fjin1Tin0Lex1Hin0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 3b, #geq1fj, #geq1T, 0L, #geq1H, 0V, 1l}\n")
            elif(reg == "c1lep3_5jwin3bex1fjin1VLTin0Hex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, 3b, #geq1fj, #geq1VLT, 0H, 1l}\n")

                
            elif(reg == "c1lep3_5jwin4bin0fjex0Tex0Lex1Hin0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, #geq4b, 0fj, 0T, 0L, #geq1H, 0V, 1l}\n")
            elif(reg == "c1lep3_5jwin4bin0fjex0Tex1Lin1Hin0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, #geq4b, 0fj, 0T, #geq1L, #geq1H, 0V, 1l}\n")
            elif(reg == "c1lep3_5jwin4bin1fjin1Tin0Lex1Hin0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, #geq4b, #geq1fj, #geq1T, 0L, #geq1H, 0V, 1l}\n")
            elif(reg == "c1lep3_5jwin4bin1fjin1VLTin0Hex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{5#geq3j, #geq4b, #geq1fj, #geq1VLT, 0H, 1l}\n")
                
                
            elif(reg == "c1lep6jin1bex0fjex1LTex0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 1b, 0fj, 1LT, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin1bex0fjex2LTin0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 1b, 0fj, #geq2LT, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin1bex1fjin0Tex0Lex1Hin1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 1b, #geq1fj, 0T, 0L, #geq1H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin1bex1fjin2LTin1Hin0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 1b, #geq1fj, 2LT, #geq1H, 0V, 1l}\n")

            
            elif(reg == "c1lep6jin2bex0fjex1LTex0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 2b, 0fj, 1LT, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin2bex0fjex2LTin0Hex1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 2b, 0fj, #geq2LT, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin2bex1fjin0Tex0Lex1Hin1Vin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 2b, #geq1fj, 0T, 0L, #geq1H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin2bex1fjin2LTin1Hin0Vex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 2b, #geq1fj, 2LT, #geq1H, 0V, 1l}\n")

                
            elif(reg == "c1lep6jin3bex0fjex1VLTex1Hin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 3b, 0fj, 1LT, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin3bex0fjex2VLTin1Hin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 3b, 0fj, #geq2VLT, #geq1H, 1l}\n")
            elif(reg == "c1lep6jin3bex1fjin1VLTex0Hex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 3b, #geq1fj, 1VLT, 0H, 1l}\n")
            elif(reg == "c1lep6jin3bex1fjin2VLTin0Hex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, 3b, #geq1fj, #geq2VLT, 0H, 1l}\n")

            elif(reg == "c1lep6jin4bin0fjex1VLTex1Hin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, #geq4b, 0fj, 1LT, 0H, #geq1V, 1l}\n")
            elif(reg == "c1lep6jin4bin1fjin1VLTex0Hex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, #geq4b, #geq1fj, 1VLT, 0H, 1l}\n")
            elif(reg == "c1lep6jin4bin0fjex2VLTin1Hin"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, #geq4b, 0fj, #geq2VLT, #geq1H, 1l}\n")
            elif(reg == "c1lep6jin4bin1fjin2VLTin0Hex"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq6j, #geq4b, #geq1fj, #geq2VLT, 0H, 1l}\n")


            elif(reg == "c2lep3jin0bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq 3j, =0b, SF, ZwinMLL}\n")
            elif(reg == "c2lep3jex0bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=3j, =0b, SF, ZwinMLL}\n")
            elif(reg == "c2lep4jex0bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=4j, =0b, SF, ZwinMLL}\n")
            elif(reg == "c2lep5jex0bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=5j, =0b, SF, ZwinMLL}\n")
            elif(reg == "c2lep6jex0bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=6j, =0b, SF, ZwinMLL}\n")
            elif(reg == "c2lep7jex0bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=7j, =0b, SF, ZwinMLL}\n")
            elif(reg == "c2lep8jin0bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq 8j, =0b, SF, ZwinMLL}\n")
            elif(reg == "c2lep3jin1binZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq3j, #geq1b, SF, ZwinMLL}\n")
            elif(reg == "c2lep3jex1binZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=3j, #geq1b, SF, ZwinMLL}\n")
            elif(reg == "c2lep4jex1binZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=4j, #geq1b, SF, ZwinMLL}\n")
            elif(reg == "c2lep5jex1binZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=5j, #geq1b, SF, ZwinMLL}\n")
            elif(reg == "c2lep6jex1binZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=6j, #geq1b, SF, ZwinMLL}\n")
            elif(reg == "c2lep7jex1binZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=7j, #geq1b, SF, ZwinMLL}\n")
            elif(reg == "c2lep8jin1binZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq8j, #geq1b, SF, ZwinMLL}\n")


            elif(reg == "c2lep3jin1bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq 3j, =1b, SF, ZwinMLL}\n")
            elif(reg == "c2lep3jex1bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=3j, =1b, SF, ZwinMLL}\n")
            elif(reg == "c2lep4jex1bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=4j, =1b, SF, ZwinMLL}\n")
            elif(reg == "c2lep5jex1bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=5j, =1b, SF, ZwinMLL}\n")
            elif(reg == "c2lep6jex1bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=6j, =1b, SF, ZwinMLL}\n")
            elif(reg == "c2lep7jex1bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=7j, =1b, SF, ZwinMLL}\n")
            elif(reg == "c2lep8jin1bexZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq 8j, =1b, SF, ZwinMLL}\n")




            elif(reg == "c2lep3jin2binZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq 3j, >2b, SF, ZwinMLL}\n")
            elif(reg == "c2lep3jex2binZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=3j, >2b, SF, ZwinMLL}\n")
            elif(reg == "c2lep4jex2binZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=4j, >2b, SF, ZwinMLL}\n")
            elif(reg == "c2lep5jex2binZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=5j, >2b, SF, ZwinMLL}\n")
            elif(reg == "c2lep6jex2binZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=6j, >2b, SF, ZwinMLL}\n")
            elif(reg == "c2lep7jin2binZwinMLL_sf"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=7j, >2b, SF, ZwinMLL}\n")

            elif(reg == "c2lep3jin1bexZwinMLL_ee"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq 3j, =1b, ee, ZwinMLL}\n")
            elif(reg == "c2lep3jex1bexZwinMLL_ee"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=3j, =1b, ee, ZwinMLL}\n")
            elif(reg == "c2lep4jex1bexZwinMLL_ee"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=4j, =1b, ee, ZwinMLL}\n")
            elif(reg == "c2lep5jex1bexZwinMLL_ee"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=5j, =1b, ee, ZwinMLL}\n")
            elif(reg == "c2lep6jex1bexZwinMLL_ee"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=6j, =1b, ee, ZwinMLL}\n")
            elif(reg == "c2lep7jex1bexZwinMLL_ee"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=7j, =1b, ee, ZwinMLL}\n")
            elif(reg == "c2lep8jin1bexZwinMLL_ee"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq 8j, =1b, ee, ZwinMLL}\n")

            elif(reg == "c2lep3jin1bexZwinMLL_mumu"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq 3j, =1b, #mu#mu, ZwinMLL}\n")
            elif(reg == "c2lep3jex1bexZwinMLL_mumu"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=3j, =1b, #mu#mu, ZwinMLL}\n")
            elif(reg == "c2lep4jex1bexZwinMLL_mumu"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=4j, =1b, #mu#mu, ZwinMLL}\n")
            elif(reg == "c2lep5jex1bexZwinMLL_mumu"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=5j, =1b, #mu#mu, ZwinMLL}\n")
            elif(reg == "c2lep6jex1bexZwinMLL_mumu"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=6j, =1b, #mu#mu, ZwinMLL}\n")
            elif(reg == "c2lep7jex1bexZwinMLL_mumu"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{=7j, =1b, #mu#mu, ZwinMLL}\n")
            elif(reg == "c2lep8jin1bexZwinMLL_mumu"):
                varConfig.write("EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}{#geq 8j, =1b, #mu#mu, ZwinMLL}\n")

            else:
                print("WARNING! Region not found, EXTRALABEL parameter will not be set!")

            varConfig.write("\n")

        else:
            print("WARNING! Variable "+fullName+" not found in input file!")

f.Close()
varConfig.write("END")
varConfig.close()
