#!/bin/python
import os
import sys

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/IFAETopFramework/" )
from BatchTools import *
from Samples import *

##______________________________________________________________________
##
## Object systematics
##
## Nominal
CommonObjectSystematics =  []
CommonObjectSystematics += [getSystematics(name="nominal",nameUp="nominal",oneSided=True)] # the nominal is considered as a systematic variation
## Electron
CommonObjectSystematics += [getSystematics(name="EG_RESOLUTION_ALL",nameUp="EG_RESOLUTION_ALL__1up",nameDown="EG_RESOLUTION_ALL__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="EG_SCALE_ALL",nameUp="EG_SCALE_ALL__1up",nameDown="EG_SCALE_ALL__1down",oneSided=False)]
## Jets
CommonObjectSystematics += [getSystematics(name="JET_BJES_Response",nameUp="JET_BJES_Response__1up",nameDown="JET_BJES_Response__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Detector1",nameUp="JET_EffectiveNP_Detector1__1up",nameDown="JET_EffectiveNP_Detector1__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Detector2",nameUp="JET_EffectiveNP_Detector2__1up",nameDown="JET_EffectiveNP_Detector2__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Mixed1",nameUp="JET_EffectiveNP_Mixed1__1up",nameDown="JET_EffectiveNP_Mixed1__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Mixed2",nameUp="JET_EffectiveNP_Mixed2__1up",nameDown="JET_EffectiveNP_Mixed2__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Mixed3",nameUp="JET_EffectiveNP_Mixed3__1up",nameDown="JET_EffectiveNP_Mixed3__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Modelling1",nameUp="JET_EffectiveNP_Modelling1__1up",nameDown="JET_EffectiveNP_Modelling1__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Modelling2",nameUp="JET_EffectiveNP_Modelling2__1up",nameDown="JET_EffectiveNP_Modelling2__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Modelling3",nameUp="JET_EffectiveNP_Modelling3__1up",nameDown="JET_EffectiveNP_Modelling3__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Modelling4",nameUp="JET_EffectiveNP_Modelling4__1up",nameDown="JET_EffectiveNP_Modelling4__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Statistical1",nameUp="JET_EffectiveNP_Statistical1__1up",nameDown="JET_EffectiveNP_Statistical1__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Statistical2",nameUp="JET_EffectiveNP_Statistical2__1up",nameDown="JET_EffectiveNP_Statistical2__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Statistical3",nameUp="JET_EffectiveNP_Statistical3__1up",nameDown="JET_EffectiveNP_Statistical3__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Statistical4",nameUp="JET_EffectiveNP_Statistical4__1up",nameDown="JET_EffectiveNP_Statistical4__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Statistical5",nameUp="JET_EffectiveNP_Statistical5__1up",nameDown="JET_EffectiveNP_Statistical5__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EffectiveNP_Statistical6",nameUp="JET_EffectiveNP_Statistical6__1up",nameDown="JET_EffectiveNP_Statistical6__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EtaIntercalibration_Modelling",nameUp="JET_EtaIntercalibration_Modelling__1up",nameDown="JET_EtaIntercalibration_Modelling__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EtaIntercalibration_NonClosure_2018data",nameUp="JET_EtaIntercalibration_NonClosure_2018data__1up",
                                           nameDown="JET_EtaIntercalibration_NonClosure_2018data__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EtaIntercalibration_NonClosure_highE",nameUp="JET_EtaIntercalibration_NonClosure_highE__1up",
                                           nameDown="JET_EtaIntercalibration_NonClosure_highE__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EtaIntercalibration_NonClosure_negEta",nameUp="JET_EtaIntercalibration_NonClosure_negEta__1up",
                                           nameDown="JET_EtaIntercalibration_NonClosure_negEta__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EtaIntercalibration_NonClosure_posEta",nameUp="JET_EtaIntercalibration_NonClosure_posEta__1up",
                                           nameDown="JET_EtaIntercalibration_NonClosure_posEta__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_EtaIntercalibration_TotalStat",nameUp="JET_EtaIntercalibration_TotalStat__1up",nameDown="JET_EtaIntercalibration_TotalStat__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_Flavor_Composition",nameUp="JET_Flavor_Composition__1up",nameDown="JET_Flavor_Composition__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_Flavor_Response",nameUp="JET_Flavor_Response__1up",nameDown="JET_Flavor_Response__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_Pileup_OffsetMu",nameUp="JET_Pileup_OffsetMu__1up",nameDown="JET_Pileup_OffsetMu__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_Pileup_OffsetNPV",nameUp="JET_Pileup_OffsetNPV__1up",nameDown="JET_Pileup_OffsetNPV__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_Pileup_PtTerm",nameUp="JET_Pileup_PtTerm__1up",nameDown="JET_Pileup_PtTerm__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_Pileup_RhoTopology",nameUp="JET_Pileup_RhoTopology__1up",nameDown="JET_Pileup_RhoTopology__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_PunchThrough_MC16",nameUp="JET_PunchThrough_MC16__1up",nameDown="JET_PunchThrough_MC16__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_SingleParticle_HighPt",nameUp="JET_SingleParticle_HighPt__1up",nameDown="JET_SingleParticle_HighPt__1down",oneSided=False)]
##########
#CommonObjectSystematics += [getSystematics(name="JET_Rrk_Tracking_mass",nameUp="JET_Rrk_Tracking_mass__1up",nameDown="JET_Rrk_Tracking_mass__1down",oneSided=False)]
#CommonObjectSystematics += [getSystematics(name="JET_Rtrk_Baseline_mass",nameUp="JET_Rtrk_Baseline_mass__1up",nameDown="JET_Rtrk_Baseline_mass__1down",oneSided=False)]
#CommonObjectSystematics += [getSystematics(name="JET_Rtrk_Modelling_mass",nameUp="JET_Rtrk_Modelling_mass__1up",nameDown="JET_Rtrk_Modelling_mass__1down",oneSided=False)]
#CommonObjectSystematics += [getSystematics(name="JET_Rtrk_TotalStat_mass_mass",nameUp="JET_Rtrk_TotalStat_mass__1up",nameDown="JET_Rtrk_TotalStat_mass__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_Rtrk_Baseline_scaled_mass",nameUp="JET_Rtrk_Baseline_scaled_mass__1up",nameDown="JET_Rtrk_Baseline_scaled_mass__1down",oneSided=False)] 
CommonObjectSystematics += [getSystematics(name="JET_Rtrk_ExtraComp_Baseline_scaled_mass",nameUp="JET_Rtrk_ExtraComp_Baseline_scaled_mass__1up",nameDown="JET_Rtrk_ExtraComp_Baseline_scaled_mass__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_Rtrk_ExtraComp_Modelling_scaled_mass",nameUp="JET_Rtrk_ExtraComp_Modelling_scaled_mass__1up",nameDown="JET_Rtrk_ExtraComp_Modelling_scaled_mass__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_Rtrk_Modelling_scaled_mass",nameUp="JET_Rtrk_Modelling_scaled_mass__1up",nameDown="JET_Rtrk_Modelling_scaled_mass__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_Rtrk_TotalStat_scaled_mass",nameUp="JET_Rtrk_TotalStat_scaled_mass__1up",nameDown="JET_Rtrk_TotalStat_scaled_mass__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_Rtrk_Tracking1_scaled_mass",nameUp="JET_Rtrk_Tracking1_scaled_mass__1up",nameDown="JET_Rtrk_Tracking1_scaled_mass__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_Rtrk_Tracking2_scaled_mass",nameUp="JET_Rtrk_Tracking2_scaled_mass__1up",nameDown="JET_Rtrk_Tracking2_scaled_mass__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_Rtrk_Tracking3_scaled_mass",nameUp="JET_Rtrk_Tracking3_scaled_mass__1up",nameDown="JET_Rtrk_Tracking3_scaled_mass__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_1__1up",nameUp="JET_JER_EffectiveNP_1__1up",nameDown="JET_JER_EffectiveNP_1__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_2__1up",nameUp="JET_JER_EffectiveNP_2__1up",nameDown="JET_JER_EffectiveNP_2__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_3__1up",nameUp="JET_JER_EffectiveNP_3__1up",nameDown="JET_JER_EffectiveNP_3__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_4__1up",nameUp="JET_JER_EffectiveNP_4__1up",nameDown="JET_JER_EffectiveNP_4__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_5__1up",nameUp="JET_JER_EffectiveNP_5__1up",nameDown="JET_JER_EffectiveNP_5__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_6__1up",nameUp="JET_JER_EffectiveNP_6__1up",nameDown="JET_JER_EffectiveNP_6__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_7restTerm__1up",nameUp="JET_JER_EffectiveNP_7restTerm__1up",nameDown="JET_JER_EffectiveNP_7restTerm__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="JET_JER_DataVsMC_MC16",nameUp="JET_JER_DataVsMC_MC16__1up",nameDown="JET_JER_DataVsMC_MC16__1down",oneSided=False)]
#CommonObjectSystematics += [getSystematics(name="JET_JMR__1up",nameUp="JET_JMR__1up",nameDown="",oneSided=True)]
#MET
CommonObjectSystematics += [getSystematics(name="MET_SoftTrk_ResoPara",nameUp="MET_SoftTrk_ResoPara",nameDown="",oneSided=True)]
CommonObjectSystematics += [getSystematics(name="MET_SoftTrk_ResoPerp",nameUp="MET_SoftTrk_ResoPerp",nameDown="",oneSided=True)]
CommonObjectSystematics += [getSystematics(name="MET_SoftTrk_Scale",nameUp="MET_SoftTrk_Scale__1up",nameDown="MET_SoftTrk_Scale__1down",oneSided=False)]
#Muon
CommonObjectSystematics += [getSystematics(name="MUON_ID",nameUp="MUON_ID__1up",nameDown="MUON_ID__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="MUON_MS",nameUp="MUON_MS__1up",nameDown="MUON_MS__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="MUON_SCALE",nameUp="MUON_SCALE__1up",nameDown="MUON_SCALE__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="MUON_SAGITTA_RESBIAS",nameUp="MUON_SAGITTA_RESBIAS__1up",nameDown="MUON_SAGITTA_RESBIAS__1down",oneSided=False)]
#CommonObjectSystematics += [getSystematics(name="MUON_SAGITTA_RHO",nameUp="MUON_SAGITTA_RHO__1up",nameDown="MUON_SAGITTA_RHO__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="MUON_SAGITTA_DATASTAT",nameUp="MUON_SAGITTA_DATASTAT__1up",nameDown="MUON_SAGITTA_DATASTAT__1down",oneSided=False)]

##______________________________________________________________________
##
def GetTtbarSamples( useObjectSyst=False, hfSplitted=True, ttbarSystSamples=False, useHTSlices = True, campaign = ""):
    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples = []
    ttbarTypes = ["ttbarlight","ttbarbb","ttbarcc"]
    if not hfSplitted:
        ttbarTypes = ["ttbar",]

    for ttbarType in ttbarTypes:
        Samples     += [getSampleUncertainties(ttbarType,    "410470."+campaign, ObjectSystematics)]
        if useHTSlices: 
            Samples     += [getSampleUncertainties(ttbarType,    "407342."+campaign, ObjectSystematics)]
            Samples     += [getSampleUncertainties(ttbarType,    "407343."+campaign, ObjectSystematics)]
            Samples     += [getSampleUncertainties(ttbarType,    "407344."+campaign, ObjectSystematics)]

        if ttbarSystSamples:
            # Powheg+Herwig7
            Samples     += [getSampleUncertainties(ttbarType+"PowHer",    "411233."+campaign, 
                                                   ObjectSystematics)]
            Samples     += [getSampleUncertainties(ttbarType+"PowHer",    "411234."+campaign, 
                                                   ObjectSystematics)]

            # aMC@NLO+Pythia8
            Samples     += [getSampleUncertainties(ttbarType+"aMCPy",    "410464."+campaign, 
                                                   ObjectSystematics)]
            Samples     += [getSampleUncertainties(ttbarType+"aMCPy",    "410465."+campaign, 
                                                   ObjectSystematics)]

            if useHTSlices:
                Samples     += [getSampleUncertainties(ttbarType+"PowHer",    "411335."+campaign, 
                                                       ObjectSystematics)]
                Samples     += [getSampleUncertainties(ttbarType+"PowHer",    "411336."+campaign, 
                                                       ObjectSystematics)]
                Samples     += [getSampleUncertainties(ttbarType+"PowHer",    "411337."+campaign, 
                                                       ObjectSystematics)]

                Samples     += [getSampleUncertainties(ttbarType+"aMCPy",    "407348."+campaign, 
                                                       ObjectSystematics)]
                Samples     += [getSampleUncertainties(ttbarType+"aMCPy",    "407349."+campaign, 
                                                       ObjectSystematics)]
                Samples     += [getSampleUncertainties(ttbarType+"aMCPy",    "407350."+campaign, 
                                                       ObjectSystematics)]

    return Samples

###_____________________________________________________________________
##
def GetWSamplesSherpa2211( useObjectSyst=False, campaign="", name = "Wjets"):
    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    # Wenu
    Samples     += [getSampleUncertainties(name,"700338."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700339."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700340."+campaign, ObjectSystematics )]
    # Wmunu
    Samples     += [getSampleUncertainties(name,"700341."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700342."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700343."+campaign, ObjectSystematics )]
    # Wtaunu lep
    Samples     += [getSampleUncertainties(name,"700344."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700345."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700346."+campaign, ObjectSystematics )]
    # Wtaunu had
    Samples     += [getSampleUncertainties(name,"700347."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700348."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700349."+campaign, ObjectSystematics )]

    return Samples

##_____________________________________________________________________
##
def GetZSamplesSherpa2211( useObjectSyst=False, campaign="", name = "Zjets"):
    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    # Zee
    Samples     += [getSampleUncertainties(name,"700320."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700321."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700322."+campaign, ObjectSystematics )]
    # Zmumu
    Samples     += [getSampleUncertainties(name,"700323."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700324."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700325."+campaign, ObjectSystematics )]
    # Ztautau lep lep
    Samples     += [getSampleUncertainties(name,"700326."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700327."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700328."+campaign, ObjectSystematics )]
    # Ztautau lep had
    Samples     += [getSampleUncertainties(name,"700329."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700330."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700331."+campaign, ObjectSystematics )]
    # Ztautau had had
    Samples     += [getSampleUncertainties(name,"700332."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700333."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700334."+campaign, ObjectSystematics )]
    # Znunu
    Samples     += [getSampleUncertainties(name,"700335."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700336."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"700337."+campaign, ObjectSystematics )]

    return Samples

##_____________________________________________________________________
##
def GetWSamplesSherpa221 ( useObjectSyst=False, campaign="", name = "Wjets", removeNull=False):

    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    if not(removeNull):
        Samples     += [getSampleUncertainties(name,"364170."+campaign, ObjectSystematics )]
    if not(removeNull and campaign=="mc16d"):
        Samples     += [getSampleUncertainties(name,"364171."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364172."+campaign, ObjectSystematics )]
    if not(removeNull and campaign=="mc16a"):
        Samples     += [getSampleUncertainties(name,"364173."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364174."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364175."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364176."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364177."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364178."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364179."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364180."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364181."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364182."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364183."+campaign, ObjectSystematics )]
    if not(removeNull and (campaign=="mc16a" or campaign=="mc16d")):
        Samples     += [getSampleUncertainties(name,"364156."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364157."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364158."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364159."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364160."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364161."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364162."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364163."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364164."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364165."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364166."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364167."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364168."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364169."+campaign, ObjectSystematics )]
    if not(removeNull):
        Samples     += [getSampleUncertainties(name,"364184."+campaign, ObjectSystematics )]
        Samples     += [getSampleUncertainties(name,"364185."+campaign, ObjectSystematics )]

    if not(removeNull and (campaign=="mc16a" or campaign=="mc16e")):
        Samples     += [getSampleUncertainties(name,"364186."+campaign, ObjectSystematics )]
    if not(removeNull):
        Samples     += [getSampleUncertainties(name,"364187."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364188."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364189."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364190."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364191."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364192."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364193."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364194."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364195."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364196."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364197."+campaign, ObjectSystematics )]
    return Samples

##_____________________________________________________________________
##
def GetZSamplesSherpa221( useObjectSyst=False, campaign="", name = "Zjets", removeNull=False):

    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    ######## Z+jets ########### 
    #ee
    if not(removeNull and campaign=="mc16a"):
        Samples     += [getSampleUncertainties(name,"364114."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364115."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364116."+campaign, ObjectSystematics )]
    if not(removeNull and campaign=="mc16a"):
        Samples     += [getSampleUncertainties(name,"364117."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364118."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364119."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364120."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364121."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364122."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364123."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364124."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364125."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364126."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364127."+campaign, ObjectSystematics )]
    #mumu
    if not(removeNull and (campaign=="mc16a" or campaign=="mc16d")):
        Samples     += [getSampleUncertainties(name,"364100."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364101."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364102."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364103."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364104."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364105."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364106."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364107."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364108."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364109."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364110."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364111."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364112."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364113."+campaign, ObjectSystematics )]
    #tautau
    if not(removeNull):
        Samples     += [getSampleUncertainties(name,"364128."+campaign, ObjectSystematics )]
    if not(removeNull and (campaign=="mc16a" or campaign=="mc16e")):
        Samples     += [getSampleUncertainties(name,"364129."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364130."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364131."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364132."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364133."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364134."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364135."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364136."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364137."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364138."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364139."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364140."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364141."+campaign, ObjectSystematics )]
    #0l nunu
    if(campaign=="mc16a" or campaign=="mc16d"):
        Samples += [getSampleUncertainties(name, "364142."+campaign, ObjectSystematics)]
        Samples += [getSampleUncertainties(name, "364143."+campaign, ObjectSystematics)]
        Samples += [getSampleUncertainties(name, "364144."+campaign, ObjectSystematics)]
        Samples += [getSampleUncertainties(name, "364145."+campaign, ObjectSystematics)]
        Samples += [getSampleUncertainties(name, "364146."+campaign, ObjectSystematics)]
        Samples += [getSampleUncertainties(name, "364147."+campaign, ObjectSystematics)]
        Samples += [getSampleUncertainties(name, "364148."+campaign, ObjectSystematics)]
        Samples += [getSampleUncertainties(name, "364149."+campaign, ObjectSystematics)]
        Samples += [getSampleUncertainties(name, "364150."+campaign, ObjectSystematics)]
        Samples += [getSampleUncertainties(name, "364151."+campaign, ObjectSystematics)]
        Samples += [getSampleUncertainties(name, "364152."+campaign, ObjectSystematics)]
        Samples += [getSampleUncertainties(name, "364153."+campaign, ObjectSystematics)]
        Samples += [getSampleUncertainties(name, "364154."+campaign, ObjectSystematics)]
        Samples += [getSampleUncertainties(name, "364155."+campaign, ObjectSystematics)]

    return Samples


##______________________________________________________________________
##
def GetSingleTopSamples( useObjectSyst=False, campaign="", name = "Singletop", splitChannel=False, 
                         runTchan=True, runWtprod=True, runSchan=True,
                         runSingletopSystSamples=False):

    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []

    nameTChan=name
    nameSChan=name
    nameWtprod=name

    if splitChannel:
        nameTChan=name+"tchan"
        nameSChan=name+"schan"
        nameWtprod=name+"Wtprod"
        
    if runTchan:
        Samples     += [getSampleUncertainties(nameTChan, "410658."+campaign, ObjectSystematics )]
        Samples     += [getSampleUncertainties(nameTChan, "410659."+campaign, ObjectSystematics )]
    if runWtprod:
        Samples     += [getSampleUncertainties(nameWtprod,"410646."+campaign, ObjectSystematics )]
        Samples     += [getSampleUncertainties(nameWtprod,"410647."+campaign, ObjectSystematics )]
    if runSchan:
        Samples     += [getSampleUncertainties(nameSChan, "410644."+campaign, ObjectSystematics )]
        Samples     += [getSampleUncertainties(nameSChan, "410645."+campaign, ObjectSystematics )]

    if runSingletopSystSamples:
        if runWtprod:
            Samples     += [getSampleUncertainties(nameWtprod+"PowHer","411036."+campaign, 
                                                   ObjectSystematics )]
            Samples     += [getSampleUncertainties(nameWtprod+"PowHer","411037."+campaign, 
                                                   ObjectSystematics )]
            Samples     += [getSampleUncertainties(nameWtprod+"aMCPy","412002."+campaign, 
                                                   ObjectSystematics )]
            # DiagSub only applicable to Wt channel
            Samples     += [getSampleUncertainties(nameWtprod+"DiagSub","410654."+campaign, 
                                                   ObjectSystematics )]
            Samples     += [getSampleUncertainties(nameWtprod+"DiagSub","410655."+campaign, 
                                                   ObjectSystematics )]
        if runTchan:
            Samples     += [getSampleUncertainties(nameTChan+"PowHer","411032."+campaign, 
                                                   ObjectSystematics )]
            Samples     += [getSampleUncertainties(nameTChan+"PowHer","411033."+campaign, 
                                                   ObjectSystematics )]
            Samples     += [getSampleUncertainties(nameTChan+"aMCPy","412004."+campaign, 
                                                   ObjectSystematics )]
        if runSchan:
            Samples     += [getSampleUncertainties(nameSChan+"PowHer","411034."+campaign, 
                                                   ObjectSystematics )]
            Samples     += [getSampleUncertainties(nameSChan+"PowHer","411035."+campaign, 
                                                   ObjectSystematics )]
            Samples     += [getSampleUncertainties(nameSChan+"aMCPy","412005."+campaign, 
                                                   ObjectSystematics )]

    return Samples

##______________________________________________________________________
##
def GetDibosonSamplesSherpa221( useObjectSyst=False, campaign="", name = "Dibosons"):

    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    #Sherpa 2.2.1
    Samples     += [getSampleUncertainties(name,"363355."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"363356."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"363357."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"363358."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"363359."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"363360."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"363489."+campaign, ObjectSystematics )]

    return Samples

##______________________________________________________________________
##
def GetDibosonSamplesSherpa2211( useObjectSyst=False, campaign="", name = "Dibosons"):
    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    Samples     += [getSampleUncertainties(name,"700488."+campaign, ObjectSystematics )] #WlvWqq
    Samples     += [getSampleUncertainties(name,"700489."+campaign, ObjectSystematics )] #WlvZqq
    Samples     += [getSampleUncertainties(name,"700490."+campaign, ObjectSystematics )] #WlvZbb
    Samples     += [getSampleUncertainties(name,"700491."+campaign, ObjectSystematics )] #WqqZvv
    Samples     += [getSampleUncertainties(name,"700492."+campaign, ObjectSystematics )] #WqqZll
    Samples     += [getSampleUncertainties(name,"700493."+campaign, ObjectSystematics )] #ZqqZll
    Samples     += [getSampleUncertainties(name,"700494."+campaign, ObjectSystematics )] #ZbbZll
    Samples     += [getSampleUncertainties(name,"700495."+campaign, ObjectSystematics )] #ZqqZvv
    Samples     += [getSampleUncertainties(name,"700496."+campaign, ObjectSystematics )] #ZbbZvv

    return Samples

##______________________________________________________________________
##
def GetTopEWSamples( useObjectSyst=False, campaign="", name = "topEW", sVLQAna = False):

    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    Samples     += [getSampleUncertainties(name,"410155."+campaign, ObjectSystematics )] #ttW
    Samples     += [getSampleUncertainties(name,"410156."+campaign, ObjectSystematics )] #ttZnunu
    Samples     += [getSampleUncertainties(name,"410157."+campaign, ObjectSystematics )] #ttZqq
    Samples     += [getSampleUncertainties(name,"410218."+campaign, ObjectSystematics )] #ttee
    Samples     += [getSampleUncertainties(name,"410219."+campaign, ObjectSystematics )] #ttmumu
    Samples     += [getSampleUncertainties(name,"410220."+campaign, ObjectSystematics )] #tttautau
    #if sVLQAna:
    #    Samples     += [getSampleUncertainties(name,"410276."+campaign, ObjectSystematics )] #ttee_mll_1_5
    #    Samples     += [getSampleUncertainties(name,"410277."+campaign, ObjectSystematics )] #ttmumu_mll_1_5
    #    Samples     += [getSampleUncertainties(name,"410278."+campaign, ObjectSystematics )] #tttautau_mll_1_5
    #    Samples     += [getSampleUncertainties(name,"410560."+campaign, ObjectSystematics )] #tZ

    return Samples

##______________________________________________________________________
##
def GetHiggsSamples( useObjectSyst=False, campaign="", sVLQAna=False):

    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    # Samples     += [getSampleUncertainties(name,"345874."+campaign, ObjectSystematics )] #ttH125_hdamp352p5_semilep
    # Samples     += [getSampleUncertainties(name,"345875."+campaign, ObjectSystematics )] #ttH125_hdamp352p5_dilep
    Samples     += [getSampleUncertainties("ttH","346344."+campaign, ObjectSystematics )] #ttH125_semilep
    Samples     += [getSampleUncertainties("ttH","346345."+campaign, ObjectSystematics )] #ttH125_dilep
    #if sVLQAna:
    #    Samples     += [getSampleUncertainties("VH","342284."+campaign, ObjectSystematics )] #WH
    #    Samples     += [getSampleUncertainties("VH","342285."+campaign, ObjectSystematics )] #ZH
    return Samples

##______________________________________________________________________
##
def Get4TopsSamples(  useObjectSyst=False, campaign="", name = "SM4tops"):

    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    Samples     += [getSampleUncertainties(name,"410080."+campaign, ObjectSystematics )]
    return Samples

##______________________________________________________________________
##
def GetDijetSamples( useObjectSyst=False, campaign="", name = "Dijet"):

    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    #Samples     += [getSampleUncertainties(name,"364700."+campaign, ObjectSystematics )]
    #Samples     += [getSampleUncertainties(name,"364701."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364702."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364703."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364704."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364705."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364706."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364707."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties(name,"364708."+campaign, ObjectSystematics )]
    #Samples     += [getSampleUncertainties(name,"364709."+campaign, ObjectSystematics )]
    #Samples     += [getSampleUncertainties(name,"364710."+campaign, ObjectSystematics )]
    #Samples     += [getSampleUncertainties(name,"364711."+campaign, ObjectSystematics )]
    #Samples     += [getSampleUncertainties(name,"364712."+campaign, ObjectSystematics )]
    return Samples

##______________________________________________________________________
##
def GetBenchmarkVLQSamples(  useObjectSyst=False, campaign=""):

    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    Samples     += [getSampleUncertainties("VLQ_TT_1400","302482."+campaign, ObjectSystematics )]#TT 1400
    Samples     += [getSampleUncertainties("ttbar","307018."+campaign, ObjectSystematics )]
    Samples     += [getSampleUncertainties("ttbar","307061."+campaign, ObjectSystematics )]
    #Samples     += [getSampleUncertainties("WTht2000","307018."+campaign, ObjectSystematics )]
    #Samples     += [getSampleUncertainties("ZTht2000","307061."+campaign, ObjectSystematics )]
    return Samples

##______________________________________________________________________
##
def GetVLQTSamples( useObjectSyst=False, campaign="", postRW=False, allBR=False):

    ObjectSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    ##------------------------- Post BR-reweighted samples for file merging --------------------------
    if postRW:
        VLQ_masses = ["600","800","1000","1100","1200","1300","1400","1500","1600","1700","1800","2000"]
        step_width = 0.05
        steps_n = int((1 + step_width)/step_width)
 
        for mass in VLQ_masses:

            if allBR:
                for iHt in range(0,steps_n):
                    for iWb in range(0,steps_n):
                        for iZt in range(0,steps_n):
                            coupling_Ht = iHt * step_width
                            coupling_Zt = iZt * step_width
                            coupling_Wb = iWb * step_width
                            if abs(coupling_Ht+coupling_Wb+coupling_Zt-1.)<0.001:
                                Samples += [getSampleUncertainties("VLQ_TT_" + mass +
                                                        "_BR_%.2f_%.2f_%.2f"%(coupling_Wb,coupling_Zt,coupling_Ht)
                                                        ,"", ObjectSystematics , [])]
            else:
                Samples += [getSampleUncertainties( "VLQ_TT_" + mass + "_TSinglet"+campaign, "", ObjectSystematics , [])]
                Samples += [getSampleUncertainties( "VLQ_TT_" + mass + "_TDoublet"+campaign, "", ObjectSystematics , [])]
                Samples += [getSampleUncertainties( "VLQ_TT_" + mass + "_HtHt"+campaign,     "", ObjectSystematics , [])]
                Samples += [getSampleUncertainties( "VLQ_TT_" + mass + "_ZtZt"+campaign,     "", ObjectSystematics , [])]

    ##------------------------- Actual DSIDs pre-RW  --------------------------
    else:
        Samples     += [getSampleUncertainties("VLQ_TT_600","302469."+campaign,  ObjectSystematics )]#TT 600
        Samples     += [getSampleUncertainties("VLQ_TT_800","302472."+campaign,  ObjectSystematics )]#TT 800
        Samples     += [getSampleUncertainties("VLQ_TT_1000","302476."+campaign, ObjectSystematics )]#TT 1000
        Samples     += [getSampleUncertainties("VLQ_TT_1100","302478."+campaign, ObjectSystematics )]#TT 1100
        Samples     += [getSampleUncertainties("VLQ_TT_1200","302480."+campaign, ObjectSystematics )]#TT 1200
        Samples     += [getSampleUncertainties("VLQ_TT_1300","302481."+campaign, ObjectSystematics )]#TT 1300
        Samples     += [getSampleUncertainties("VLQ_TT_1400","302482."+campaign, ObjectSystematics )]#TT 1400
        Samples     += [getSampleUncertainties("VLQ_TT_1500","308294."+campaign, ObjectSystematics )]#TT 1500
        Samples     += [getSampleUncertainties("VLQ_TT_1600","308295."+campaign, ObjectSystematics )]#TT 1600
        Samples     += [getSampleUncertainties("VLQ_TT_1700","308296."+campaign, ObjectSystematics )]#TT 1700
        Samples     += [getSampleUncertainties("VLQ_TT_1800","308297."+campaign, ObjectSystematics )]#TT 1800
        Samples     += [getSampleUncertainties("VLQ_TT_2000","308299."+campaign, ObjectSystematics )]#TT 2000  
        
        # Not available
        #Samples     += [getSampleUncertainties("VLQ_TT_700","302470."+campaign,  ObjectSystematics )]#TT 700    
        #Samples     += [getSampleUncertainties("VLQ_TT_750","302471."+campaign,  ObjectSystematics )]#TT 750
        #Samples     += [getSampleUncertainties("VLQ_TT_850","302473."+campaign,  ObjectSystematics )]#TT 850
        #Samples     += [getSampleUncertainties("VLQ_TT_900","302474."+campaign,  ObjectSystematics )]#TT 900
        #Samples     += [getSampleUncertainties("VLQ_TT_950","302475."+campaign,  ObjectSystematics )]#TT 950
        #Samples     += [getSampleUncertainties("VLQ_TT_1050","302477."+campaign, ObjectSystematics )]#TT 1050
        #Samples     += [getSampleUncertainties("VLQ_TT_1150","302479."+campaign, ObjectSystematics )]#TT 1150 

    return Samples

##______________________________________________________________________
##
def GetVLQTDoubletSamples(  useObjectSyst=False, campaign=""):

    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
   # Samples     += [getSampleUncertainties("VLQ_TTD_700","302483."+campaign,  ObjectSystematics )]#TT 700
    Samples     += [getSampleUncertainties("VLQ_TT_1200","302485."+campaign,  ObjectSystematics )]#TT 1200 

    return Samples

##______________________________________________________________________
##
def GetSingleVLQSamples( useObjectSyst=False, campaign="", RWName="nom_mass_K100"):

    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    couplingName = RWName.replace("nom_mass_","")
    couplingName = RWName.replace("low_mass_","")

    DSIDmap = []
    DSIDmap += [{ 'dsid': "312349.", 'mode': "WTZt", 'mass' : 1100}]
    DSIDmap += [{ 'dsid': "312350.", 'mode': "WTZt", 'mass' : 1300}]
    DSIDmap += [{ 'dsid': "312351.", 'mode': "WTZt", 'mass' : 1500}]
    DSIDmap += [{ 'dsid': "312352.", 'mode': "WTZt", 'mass' : 1700}]
    DSIDmap += [{ 'dsid': "312353.", 'mode': "WTZt", 'mass' : 1900}]
    DSIDmap += [{ 'dsid': "312354.", 'mode': "WTZt", 'mass' : 2100}]

    DSIDmap += [{ 'dsid': "313330.", 'mode': "WTHt", 'mass' : 1100}]
    DSIDmap += [{ 'dsid': "313331.", 'mode': "WTHt", 'mass' : 1300}]
    DSIDmap += [{ 'dsid': "313332.", 'mode': "WTHt", 'mass' : 1500}]
    DSIDmap += [{ 'dsid': "313333.", 'mode': "WTHt", 'mass' : 1700}]
    DSIDmap += [{ 'dsid': "313334.", 'mode': "WTHt", 'mass' : 1900}]
    DSIDmap += [{ 'dsid': "313335.", 'mode': "WTHt", 'mass' : 2100}]
    DSIDmap += [{ 'dsid': "313336.", 'mode': "WTHt", 'mass' : 2300}]

    DSIDmap += [{ 'dsid': "313374.", 'mode': "ZTHt", 'mass' : 1100}]
    DSIDmap += [{ 'dsid': "313381.", 'mode': "ZTHt", 'mass' : 1100}]
    DSIDmap += [{ 'dsid': "313375.", 'mode': "ZTHt", 'mass' : 1300}]
    DSIDmap += [{ 'dsid': "313382.", 'mode': "ZTHt", 'mass' : 1300}]
    DSIDmap += [{ 'dsid': "313376.", 'mode': "ZTHt", 'mass' : 1500}]
    DSIDmap += [{ 'dsid': "313383.", 'mode': "ZTHt", 'mass' : 1500}]
    DSIDmap += [{ 'dsid': "313377.", 'mode': "ZTHt", 'mass' : 1700}]
    DSIDmap += [{ 'dsid': "313384.", 'mode': "ZTHt", 'mass' : 1700}]
    DSIDmap += [{ 'dsid': "313378.", 'mode': "ZTHt", 'mass' : 1900}]
    DSIDmap += [{ 'dsid': "313385.", 'mode': "ZTHt", 'mass' : 1900}]
    DSIDmap += [{ 'dsid': "313379.", 'mode': "ZTHt", 'mass' : 2100}]
    DSIDmap += [{ 'dsid': "313386.", 'mode': "ZTHt", 'mass' : 2100}]
    DSIDmap += [{ 'dsid': "313380.", 'mode': "ZTHt", 'mass' : 2300}]
    DSIDmap += [{ 'dsid': "313387.", 'mode': "ZTHt", 'mass' : 2300}]

    DSIDmap += [{ 'dsid': "313316.", 'mode': "ZTZt", 'mass' : 1100}]
    DSIDmap += [{ 'dsid': "313323.", 'mode': "ZTZt", 'mass' : 1100}]
    DSIDmap += [{ 'dsid': "313317.", 'mode': "ZTZt", 'mass' : 1300}]
    DSIDmap += [{ 'dsid': "313324.", 'mode': "ZTZt", 'mass' : 1300}]
    DSIDmap += [{ 'dsid': "313318.", 'mode': "ZTZt", 'mass' : 1500}]
    DSIDmap += [{ 'dsid': "313325.", 'mode': "ZTZt", 'mass' : 1500}]
    DSIDmap += [{ 'dsid': "313319.", 'mode': "ZTZt", 'mass' : 1700}]
    DSIDmap += [{ 'dsid': "313326.", 'mode': "ZTZt", 'mass' : 1700}]
    DSIDmap += [{ 'dsid': "313320.", 'mode': "ZTZt", 'mass' : 1900}]
    DSIDmap += [{ 'dsid': "313327.", 'mode': "ZTZt", 'mass' : 1900}]
    DSIDmap += [{ 'dsid': "313321.", 'mode': "ZTZt", 'mass' : 2100}]
    DSIDmap += [{ 'dsid': "313328.", 'mode': "ZTZt", 'mass' : 2100}]
    DSIDmap += [{ 'dsid': "313322.", 'mode': "ZTZt", 'mass' : 2300}]
    DSIDmap += [{ 'dsid': "313329.", 'mode': "ZTZt", 'mass' : 2300}]


    Samples     =  []

    for dsidSet in DSIDmap:
        mass = dsidSet['mass']
        if "low_mass_" in RWName:
            mass -= 100
        Samples     += [getSampleUncertainties(dsidSet['mode']+str(mass)+couplingName, dsidSet['dsid']+campaign
                                               , ObjectSystematics )]

    return Samples

def GetVLQBSamples( useObjectSys=False, campaign=""):

    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    Samples     += [getSampleUncertainties("VLQ_BB_600","302487.",  ObjectSystematics )]#BB 600
    Samples     += [getSampleUncertainties("VLQ_BB_700","302488.",  ObjectSystematics )]#BB 700
    Samples     += [getSampleUncertainties("VLQ_BB_750","302489.",  ObjectSystematics )]#BB 750
    Samples     += [getSampleUncertainties("VLQ_BB_800","302490.",  ObjectSystematics )]#BB 800
    Samples     += [getSampleUncertainties("VLQ_BB_850","302491.",  ObjectSystematics )]#BB 850
    Samples     += [getSampleUncertainties("VLQ_BB_900","302492.",  ObjectSystematics )]#BB 900
    Samples     += [getSampleUncertainties("VLQ_BB_950","302493.",  ObjectSystematics )]#BB 950
    Samples     += [getSampleUncertainties("VLQ_BB_1000","302494."+campaign, ObjectSystematics )]#BB 1000
    Samples     += [getSampleUncertainties("VLQ_BB_1050","302495."+campaign, ObjectSystematics )]#BB 1050
    Samples     += [getSampleUncertainties("VLQ_BB_1100","302496."+campaign, ObjectSystematics )]#BB 1100
    Samples     += [getSampleUncertainties("VLQ_BB_1150","302497."+campaign, ObjectSystematics )]#BB 1150
    Samples     += [getSampleUncertainties("VLQ_BB_1200","302498."+campaign, ObjectSystematics )]#BB 1200
    Samples     += [getSampleUncertainties("VLQ_BB_1300","302499."+campaign, ObjectSystematics )]#BB 1300
    Samples     += [getSampleUncertainties("VLQ_BB_1400","302500."+campaign, ObjectSystematics )]#BB 1400
    Samples     += [getSampleUncertainties("VLQ_BB_1500","308303."+campaign, ObjectSystematics )]#BB 1500
    Samples     += [getSampleUncertainties("VLQ_BB_1600","308304."+campaign, ObjectSystematics )]#BB 1600

##______________________________________________________________________
##
def GetUEDRPPSamples( useObjectSyst=False, campaign="" ):

    ObjectSystematics = []
    #WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    Samples     += [getSampleUncertainties("UEDRPP_1000","302055."+campaign, ObjectSystematics )]#UED RPP 1 TeV
    Samples     += [getSampleUncertainties("UEDRPP_1200","302056."+campaign, ObjectSystematics )]#UED RPP 1.2 TeV
    Samples     += [getSampleUncertainties("UEDRPP_1400","302057."+campaign, ObjectSystematics )]#UED RPP 1.4 TeV
    Samples     += [getSampleUncertainties("UEDRPP_1600","302058."+campaign, ObjectSystematics )]#UED RPP 1.6 TeV
    Samples     += [getSampleUncertainties("UEDRPP_1800","302059."+campaign, ObjectSystematics )]#UED RPP 1.8 TeV
    return Samples


##______________________________________________________________________
##
def Get4topsCISamples( useObjectSyst=False, campaign="" ):
    ObjectSystematics = []
    #WeightSystematics = []

    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    Samples     += [getSampleUncertainties("CI4tops","302777."+campaign, ObjectSystematics )]#Contact interaction
    return Samples

##______________________________________________________________________
##
def GetDataSamples( sampleName = "Data", data_type = "TOPQ4", sVLQAna=False):
    Samples     =  []

    Samples     += [getSampleUncertainties(sampleName+".grp15", "AllYear.data" + (".DAOD_"+data_type+".grp15" if not sVLQAna else "_2015.DAOD_"+data_type),
                                           [getSystematics(name="nominal",nameUp="",oneSided=True)], [])]
    Samples     += [getSampleUncertainties(sampleName+".grp16", "AllYear.data" + (".DAOD_"+data_type+".grp16" if not sVLQAna else "_2016.DAOD_"+data_type),
                                           [getSystematics(name="nominal",nameUp="",oneSided=True)], [])]
    Samples     += [getSampleUncertainties(sampleName+".grp17", "AllYear.data" + (".DAOD_"+data_type+".grp17" if not sVLQAna else "_2017.DAOD_"+data_type),
                                           [getSystematics(name="nominal",nameUp="",oneSided=True)], [])]
    Samples     += [getSampleUncertainties(sampleName+".grp18", "AllYear.data" + (".DAOD_"+data_type+".grp18" if not sVLQAna else "_2018.DAOD_"+data_type),
                                           [getSystematics(name="nominal",nameUp="",oneSided=True)], [])]

    return Samples;

##______________________________________________________________________
##
def GetQCDSamples( data_type = "TOPQ1" ):
    Samples     =  []
    Samples     += [getSampleUncertainties("QCDE","QCD.DAOD_"+data_type+".",[getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
    Samples     += [getSampleUncertainties("QCDMU","QCD.DAOD_"+data_type+".",[getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
    return Samples;


##______________________________________________________________________
## Retrieve sample by name
##----------------------------------------------------------------------
def retrieveSamples(sampleName, mcCampaign, dataChannel='', useSyst=False, 
                    useTtbarHtSlices=True, hfSplittedTtbar=True, 
                    splitSTChannel=True, 
                    postRWPVLQ=False, allBRPVLQ=False,
                    RWNameSVLQ="nom_mass_K100"):

    Samples=[]

    ##### Data #######
    if sampleName == 'data' :
        Samples=GetDataSamples( data_type='TOPQ4' if dataChannel=='0lep' else 'TOPQ1')

    ##### Nominal backgrounds #######
    if sampleName == 'ttbar' :
        Samples=GetTtbarSamples(useObjectSyst=useSyst, campaign=mcCampaign,
                                  hfSplitted=hfSplittedTtbar, ttbarSystSamples=False, 
                                  useHTSlices=useTtbarHtSlices)
    if sampleName == 'singletop' :
        Samples=GetSingleTopSamples( useObjectSyst=useSyst, campaign=mcCampaign,
                                       splitChannel=splitSTChannel, runSingletopSystSamples=False)
    if sampleName == 'Wjets' :
        Samples= GetWSamplesSherpa2211(useObjectSyst=useSyst, campaign=mcCampaign)
    if sampleName == 'Zjets' :
        Samples= GetZSamplesSherpa2211(useObjectSyst=useSyst, campaign=mcCampaign)
    if sampleName == 'diboson' :
        Samples=GetDibosonSamplesSherpa2211(useObjectSyst=useSyst, campaign=mcCampaign)
    if sampleName == 'topEW' : ## This names needs to be changed, since 4tops is not EW
        Samples=GetTopEWSamples(useObjectSyst=useSyst, campaign=mcCampaign)
        Samples+=GetHiggsSamples(useObjectSyst=useSyst, campaign=mcCampaign) #ttH
        Samples+=Get4TopsSamples(useObjectSyst=useSyst, campaign=mcCampaign)
    if sampleName == 'dijet' :
        Samples=GetDijetSamples(useObjectSyst=False, campaign=mcCampaign)

    ##### Alternative backgrounds #######
    if sampleName == 'ttbar_alt' :
        Samples=GetTtbarSamples(useObjectSyst=False, campaign=mcCampaign,
                                  hfSplitted=hfSplittedTtbar, ttbarSystSamples=True, 
                                  useHTSlices=useTtbarHtSlices)
    if sampleName == 'singletop_alt' :
        Samples=GetSingleTopSamples( useObjectSyst=False, campaign=mcCampaign,
                                       splitChannel=splitSTChannel, runSingletopSystSamples=True)

    ##### Signal #######
    if sampleName == 'pvlq' :
        Samples=GetVLQTSamples(useObjectSyst=useSyst, campaign=mcCampaign, postRW=postRWPVLQ, allBR=allBRPVLQ)
    if sampleName == 'svlq' :
        Samples=GetSingleVLQSamples( useObjectSyst=False, campaign=mcCampaign, RWName=RWNameSVLQ)

    return Samples
    
