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
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_1__1up",nameUp="JET_JER_EffectiveNP_1__1up",nameDown="",oneSided=True)]
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_2__1up",nameUp="JET_JER_EffectiveNP_2__1up",nameDown="",oneSided=True)]
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_3__1up",nameUp="JET_JER_EffectiveNP_3__1up",nameDown="",oneSided=True)]
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_4__1up",nameUp="JET_JER_EffectiveNP_4__1up",nameDown="",oneSided=True)]
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_5__1up",nameUp="JET_JER_EffectiveNP_5__1up",nameDown="",oneSided=True)]
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_6__1up",nameUp="JET_JER_EffectiveNP_6__1up",nameDown="",oneSided=True)]
CommonObjectSystematics += [getSystematics(name="JET_JER_EffectiveNP_7restTerm__1up",nameUp="JET_JER_EffectiveNP_7restTerm__1up",nameDown="",oneSided=True)]
CommonObjectSystematics += [getSystematics(name="JET_JER_DataVsMC_MC16",nameUp="JET_JER_DataVsMC_MC16__1up",nameDown="JET_JER_DataVsMC_MC16__1down",oneSided=False)]
#CommonObjectSystematics += [getSystematics(name="JET_JMR__1up",nameUp="JET_JMR__1up",nameDown="",oneSided=True)]
#MET
CommonObjectSystematics += [getSystematics(name="MET_SoftTrk_ResoPara",nameUp="MET_SoftTrk_ResoPara",nameDown="",oneSided=True)]
CommonObjectSystematics += [getSystematics(name="MET_SoftTrk_ResoPerp",nameUp="MET_SoftTrk_ResoPerp",nameDown="",oneSided=True)]
CommonObjectSystematics += [getSystematics(name="MET_SoftTrk_Scale",nameUp="MET_SoftTrk_ScaleUp",nameDown="MET_SoftTrk_ScaleDown",oneSided=False)]
#Muon
CommonObjectSystematics += [getSystematics(name="MUON_ID",nameUp="MUON_ID__1up",nameDown="MUON_ID__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="MUON_MS",nameUp="MUON_MS__1up",nameDown="MUON_MS__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="MUON_SCALE",nameUp="MUON_SCALE__1up",nameDown="MUON_SCALE__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="MUON_SAGITTA_RESBIAS",nameUp="MUON_SAGITTA_RESBIAS__1up",nameDown="MUON_SAGITTA_RESBIAS__1down",oneSided=False)]
CommonObjectSystematics += [getSystematics(name="MUON_SAGITTA_RHO",nameUp="MUON_SAGITTA_RHO__1up",nameDown="MUON_SAGITTA_RHO__1down",oneSided=False)]

##______________________________________________________________________
##
def GetTtbarSamples( useWeightSyst=False, useObjectSyst=False, hfSplitted=True, ttbarSystSamples=False, useHTSlices = True, campaign = "" ):
    ObjectSystematics = []
    WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples = []
    ttbarTypes = ["ttbarlight","ttbarbb","ttbarcc"]
    if not hfSplitted:
        ttbarTypes = ["ttbar",]

    for ttbarType in ttbarTypes:
        Samples     += [getSampleUncertainties(ttbarType,    "410470."+campaign, ObjectSystematics, WeightSystematics)]
        if useHTSlices: 
            Samples     += [getSampleUncertainties(ttbarType,    "407342."+campaign, ObjectSystematics, WeightSystematics)]
            Samples     += [getSampleUncertainties(ttbarType,    "407343."+campaign, ObjectSystematics, WeightSystematics)]
            Samples     += [getSampleUncertainties(ttbarType,    "407344."+campaign, ObjectSystematics, WeightSystematics)]

        if ttbarSystSamples:
            # Powheg+Herwig7
            Samples     += [getSampleUncertainties(ttbarType+"PowHer",    "410557."+campaign, ObjectSystematics, WeightSystematics)]
            Samples     += [getSampleUncertainties(ttbarType+"PowHer",    "410558."+campaign, ObjectSystematics, WeightSystematics)]

            # aMC@NLO+Pythia8
            Samples     += [getSampleUncertainties(ttbarType+"aMCPy",    "410464."+campaign, ObjectSystematics, WeightSystematics)]
            Samples     += [getSampleUncertainties(ttbarType+"aMCPy",    "410465."+campaign, ObjectSystematics, WeightSystematics)]

            if useHTSlices:
                Samples     += [getSampleUncertainties(ttbarType+"PowHer",    "407354."+campaign, ObjectSystematics, WeightSystematics)]
                Samples     += [getSampleUncertainties(ttbarType+"PowHer",    "407355."+campaign, ObjectSystematics, WeightSystematics)]
                Samples     += [getSampleUncertainties(ttbarType+"PowHer",    "407356."+campaign, ObjectSystematics, WeightSystematics)]

                Samples     += [getSampleUncertainties(ttbarType+"aMCPy",    "407348."+campaign, ObjectSystematics, WeightSystematics)]
                Samples     += [getSampleUncertainties(ttbarType+"aMCPy",    "407349."+campaign, ObjectSystematics, WeightSystematics)]
                Samples     += [getSampleUncertainties(ttbarType+"aMCPy",    "407350."+campaign, ObjectSystematics, WeightSystematics)]

#            Samples     += [getSampleUncertainties(ttbarType+"radHi",      "410001."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#            Samples     += [getSampleUncertainties(ttbarType+"radLow",     "410002."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#            Samples     += [getSampleUncertainties(ttbarType+"aMCHer",     "410003."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#            Samples     += [getSampleUncertainties(ttbarType+"PowHer",     "410004."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#            if useHTSlices:
#                Samples     += [getSampleUncertainties(ttbarType+"radHi",      "407029."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#                Samples     += [getSampleUncertainties(ttbarType+"radHi",      "407030."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#                Samples     += [getSampleUncertainties(ttbarType+"radHi",      "407031."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#                Samples     += [getSampleUncertainties(ttbarType+"radHi",      "407032."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#                Samples     += [getSampleUncertainties(ttbarType+"radLow",     "407033."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#                Samples     += [getSampleUncertainties(ttbarType+"radLow",     "407034."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#                Samples     += [getSampleUncertainties(ttbarType+"radLow",     "407035."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#                Samples     += [getSampleUncertainties(ttbarType+"radLow",     "407036."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#                Samples     += [getSampleUncertainties(ttbarType+"PowHer",     "407037."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#                Samples     += [getSampleUncertainties(ttbarType+"PowHer",     "407038."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#                Samples     += [getSampleUncertainties(ttbarType+"PowHer",     "407039."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
#                Samples     += [getSampleUncertainties(ttbarType+"PowHer",     "407040."+campaign, [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
    return Samples

##______________________________________________________________________
##
def GetOtherBackgroundSamples ( useWeightSyst=False, useObjectSyst=False, campaign = ""
                                , includeSingleTop=True 
                                , includeWjets=True, includeZjets=True
                                , includeTopEW=True, includeDibosons=True, includeDijet=False
                                , includeSingletopSystSamples=False 
                                , splitSTChannels=False
                                , includeTchan=True, includeWtprod=True, includeSchan=True
                                , removeNull=False):
    Samples =  []
    if includeWjets:
        Samples += GetWSamplesSherpa221( useWeightSyst, useObjectSyst, campaign, "Wjets", removeNull )
    if includeZjets:
        Samples += GetZSamplesSherpa221( useWeightSyst, useObjectSyst, campaign, "Zjets", removeNull )
    if includeSingleTop:
        Samples += GetSingleTopSamples( useWeightSyst, useObjectSyst, campaign, 
                                        splitChannel=splitSTChannels,
                                        runTchan=includeTchan, runWtprod=includeWtprod, runSchan=includeSchan,
                                        runSingletopSystSamples=includeSingletopSystSamples)
    if includeTopEW:
        Samples += GetTopEWSamples( useWeightSyst, useObjectSyst, campaign )
        Samples += GetTtHSamples( useWeightSyst, useObjectSyst, campaign )
    if includeDibosons:
        Samples += GetDibosonSamples( useWeightSyst, useObjectSyst, campaign )
    if includeDijet:
        Samples += GetDijetSamples( useWeightSyst, useObjectSyst, campaign )
    Samples += Get4TopsSamples( useWeightSyst, useObjectSyst, campaign )

    return Samples

##_____________________________________________________________________
##

def GetSignalSamples(useWeightSyst=False, useObjectSyst=False, campaign=""
                                 , includeSingleVLQ=True, includePairVLQ=False, include4tops=False):

    Samples =  []
    if include4tops:
        Samples += Get4topsCISamples( useWeightSyst, useObjectSyst )
        Samples += GetUEDRPPSamples( useWeightSyst, useObjectSyst )
    if includeSingleVLQ:
        Samples += GetSingleVLQSamples( useWeightSyst, useObjectSyst, campaign )
    if includePairVLQ:
        Samples += GetVLQTSamples( useWeightSyst, useObjectSyst )

    return Samples



##_____________________________________________________________________
##
def GetWSamplesSherpa221 ( useWeightSyst=False, useObjectSyst=False, campaign="", name = "Wjets", removeNull=False):

    ObjectSystematics = []
    WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    if not(removeNull):
        Samples     += [getSampleUncertainties(name,"364170."+campaign, ObjectSystematics , WeightSystematics)]
    if not(removeNull and campaign=="mc16d"):
        Samples     += [getSampleUncertainties(name,"364171."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364172."+campaign, ObjectSystematics , WeightSystematics)]
    if not(removeNull and campaign=="mc16a"):
        Samples     += [getSampleUncertainties(name,"364173."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364174."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364175."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364176."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364177."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364178."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364179."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364180."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364181."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364182."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364183."+campaign, ObjectSystematics , WeightSystematics)]
    if not(removeNull and (campaign=="mc16a" or campaign=="mc16d")):
        Samples     += [getSampleUncertainties(name,"364156."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364157."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364158."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364159."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364160."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364161."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364162."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364163."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364164."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364165."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364166."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364167."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364168."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364169."+campaign, ObjectSystematics , WeightSystematics)]
    if not(removeNull):
        Samples     += [getSampleUncertainties(name,"364184."+campaign, ObjectSystematics , WeightSystematics)]
        Samples     += [getSampleUncertainties(name,"364185."+campaign, ObjectSystematics , WeightSystematics)]

    if not(removeNull and (campaign=="mc16a" or campaign=="mc16e")):
        Samples     += [getSampleUncertainties(name,"364186."+campaign, ObjectSystematics , WeightSystematics)]
    if not(removeNull):
        Samples     += [getSampleUncertainties(name,"364187."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364188."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364189."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364190."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364191."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364192."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364193."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364194."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364195."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364196."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364197."+campaign, ObjectSystematics , WeightSystematics)]
    return Samples

##_____________________________________________________________________
##
def GetZSamplesSherpa221( useWeightSyst=False, useObjectSyst=False, campaign="", name = "Zjets", removeNull=False):

    ObjectSystematics = []
    WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    ######## Z+jets ########### 
    #ee
    if not(removeNull and campaign=="mc16a"):
        Samples     += [getSampleUncertainties(name,"364114."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364115."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364116."+campaign, ObjectSystematics , WeightSystematics)]
    if not(removeNull and campaign=="mc16a"):
        Samples     += [getSampleUncertainties(name,"364117."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364118."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364119."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364120."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364121."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364122."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364123."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364124."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364125."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364126."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364127."+campaign, ObjectSystematics , WeightSystematics)]
    #mumu
    if not(removeNull and (campaign=="mc16a" or campaign=="mc16d")):
        Samples     += [getSampleUncertainties(name,"364100."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364101."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364102."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364103."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364104."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364105."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364106."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364107."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364108."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364109."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364110."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364111."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364112."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364113."+campaign, ObjectSystematics , WeightSystematics)]
    #tautau
    if not(removeNull):
        Samples     += [getSampleUncertainties(name,"364128."+campaign, ObjectSystematics , WeightSystematics)]
    if not(removeNull and (campaign=="mc16a" or campaign=="mc16e")):
        Samples     += [getSampleUncertainties(name,"364129."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364130."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364131."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364132."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364133."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364134."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364135."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364136."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364137."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364138."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364139."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364140."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364141."+campaign, ObjectSystematics , WeightSystematics)]

    return Samples


##______________________________________________________________________
##
def GetSingleTopSamples( useWeightSyst=False, useObjectSyst=False, campaign="", name = "Singletop", splitChannel=False, 
                         runTchan=True, runWtprod=True, runSchan=True,
                         runSingletopSystSamples=False):

    ObjectSystematics = []
    WeightSystematics = []
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
        Samples     += [getSampleUncertainties(nameTChan, "410658."+campaign, ObjectSystematics , WeightSystematics)]
        Samples     += [getSampleUncertainties(nameTChan, "410659."+campaign, ObjectSystematics , WeightSystematics)]
    if runWtprod:
        Samples     += [getSampleUncertainties(nameWtprod,"410646."+campaign, ObjectSystematics , WeightSystematics)]
        Samples     += [getSampleUncertainties(nameWtprod,"410647."+campaign, ObjectSystematics , WeightSystematics)]
    if runSchan:
        Samples     += [getSampleUncertainties(nameSChan, "410644."+campaign, ObjectSystematics , WeightSystematics)]
        Samples     += [getSampleUncertainties(nameSChan, "410645."+campaign, ObjectSystematics , WeightSystematics)]

    if runSingletopSystSamples:
        if runWtprod:
            Samples     += [getSampleUncertainties(nameWtprod+"PowHer","411036."+campaign, ObjectSystematics , WeightSystematics)]
            Samples     += [getSampleUncertainties(nameWtprod+"PowHer","411037."+campaign, ObjectSystematics , WeightSystematics)]
            Samples     += [getSampleUncertainties(nameWtprod+"aMCPy","412002."+campaign, ObjectSystematics , WeightSystematics)]
            # DiagSub only applicable to Wt channel
            Samples     += [getSampleUncertainties(nameWtprod+"DiagSub","410654."+campaign, ObjectSystematics , WeightSystematics)]
            Samples     += [getSampleUncertainties(nameWtprod+"DiagSub","410655."+campaign, ObjectSystematics , WeightSystematics)]
        if runTchan:
            Samples     += [getSampleUncertainties(nameTChan+"PowHer","411032."+campaign, ObjectSystematics , WeightSystematics)]
            Samples     += [getSampleUncertainties(nameTChan+"PowHer","411033."+campaign, ObjectSystematics , WeightSystematics)]
            Samples     += [getSampleUncertainties(nameTChan+"aMCPy","412004."+campaign, ObjectSystematics , WeightSystematics)]
        if runSchan:
            Samples     += [getSampleUncertainties(nameSChan+"PowHer","411034."+campaign, ObjectSystematics , WeightSystematics)]
            Samples     += [getSampleUncertainties(nameSChan+"PowHer","411035."+campaign, ObjectSystematics , WeightSystematics)]
            Samples     += [getSampleUncertainties(nameSChan+"aMCPy","412005."+campaign, ObjectSystematics , WeightSystematics)]

    return Samples

##______________________________________________________________________
##
def GetDibosonSamples( useWeightSyst=False, useObjectSyst=False, campaign="", name = "Dibosons"):

    ObjectSystematics = []
    WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    #Sherpa 2.2.1
    Samples     += [getSampleUncertainties(name,"363355."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"363356."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"363357."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"363358."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"363359."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"363360."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"363489."+campaign, ObjectSystematics , WeightSystematics)]

    return Samples

##______________________________________________________________________
##
def GetTopEWSamples( useWeightSyst=False, useObjectSyst=False, campaign="", name = "topEW"):

    ObjectSystematics = []
    WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    # Samples     += [getSampleUncertainties(name,"410155."+campaign, ObjectSystematics , WeightSystematics)] #ttW
    # Samples     += [getSampleUncertainties(name,"410156."+campaign, ObjectSystematics , WeightSystematics)] #ttZnunu
    # Samples     += [getSampleUncertainties(name,"410157."+campaign, ObjectSystematics , WeightSystematics)] #ttZqq
    # Samples     += [getSampleUncertainties(name,"410218."+campaign, ObjectSystematics , WeightSystematics)] #ttee
    # Samples     += [getSampleUncertainties(name,"410219."+campaign, ObjectSystematics , WeightSystematics)] #ttmumu
    # Samples     += [getSampleUncertainties(name,"410220."+campaign, ObjectSystematics , WeightSystematics)] #tttautau
    # Samples     += [getSampleUncertainties(name,"410276."+campaign, ObjectSystematics , WeightSystematics)] #ttee_mll_1_5
    # Samples     += [getSampleUncertainties(name,"410277."+campaign, ObjectSystematics , WeightSystematics)] #ttmumu_mll_1_5
    # Samples     += [getSampleUncertainties(name,"410278."+campaign, ObjectSystematics , WeightSystematics)] #tttautau_mll_1_5
    Samples     += [getSampleUncertainties(name,"410560."+campaign, ObjectSystematics , WeightSystematics)] #tZ
    return Samples

##______________________________________________________________________
##
def GetTtHSamples( useWeightSyst=False, useObjectSyst=False, campaign="", name = "ttH"):

    ObjectSystematics = []
    WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    # Samples     += [getSampleUncertainties(name,"345874."+campaign, ObjectSystematics , WeightSystematics)] #ttH125_hdamp352p5_semilep
    # Samples     += [getSampleUncertainties(name,"345875."+campaign, ObjectSystematics , WeightSystematics)] #ttH125_hdamp352p5_dilep
    Samples     += [getSampleUncertainties(name,"3463445."+campaign, ObjectSystematics , WeightSystematics)] #ttH125_semilep
    return Samples

##______________________________________________________________________
##
def Get4TopsSamples( useWeightSyst=False, useObjectSyst=False, campaign="", name = "SM4tops"):

    ObjectSystematics = []
    WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    Samples     += [getSampleUncertainties(name,"410080."+campaign, ObjectSystematics , WeightSystematics)]
    return Samples

##______________________________________________________________________
##
def GetDijetSamples( useWeightSyst=False, useObjectSyst=False, campaign="", name = "Dijet"):

    ObjectSystematics = []
    WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    #Samples     += [getSampleUncertainties(name,"364700."+campaign, ObjectSystematics , WeightSystematics)]
    #Samples     += [getSampleUncertainties(name,"364701."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364702."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364703."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364704."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364705."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364706."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364707."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties(name,"364708."+campaign, ObjectSystematics , WeightSystematics)]
    #Samples     += [getSampleUncertainties(name,"364709."+campaign, ObjectSystematics , WeightSystematics)]
    #Samples     += [getSampleUncertainties(name,"364710."+campaign, ObjectSystematics , WeightSystematics)]
    #Samples     += [getSampleUncertainties(name,"364711."+campaign, ObjectSystematics , WeightSystematics)]
    #Samples     += [getSampleUncertainties(name,"364712."+campaign, ObjectSystematics , WeightSystematics)]
    return Samples

##______________________________________________________________________
##
def GetVLQSamples( useWeightSyst=False, useObjectSyst=False, campaign=""):
    Samples =  []
    Samples += GetVLQTSamples( useWeightSyst, useObjectSyst )
    return Samples

##______________________________________________________________________
##
def GetBenchmarkVLQSamples( useWeightSyst=False, useObjectSyst=False, campaign=""):

    ObjectSystematics = []
    WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    Samples     += [getSampleUncertainties("VLQ_TT_1400","302482."+campaign, ObjectSystematics , WeightSystematics)]#TT 1400
    Samples     += [getSampleUncertainties("ttbar","307018."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ttbar","307061."+campaign, ObjectSystematics , WeightSystematics)]
    #Samples     += [getSampleUncertainties("WTht2000","307018."+campaign, ObjectSystematics , WeightSystematics)]
    #Samples     += [getSampleUncertainties("ZTht2000","307061."+campaign, ObjectSystematics , WeightSystematics)]
    return Samples

##______________________________________________________________________
##
def GetVLQTSamples( useWeightSyst=False, useObjectSyst=False, campaign=""):

    ObjectSystematics = []
    WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    Samples     += [getSampleUncertainties("VLQ_TT_600","302469.",  ObjectSystematics , WeightSystematics)]#TT 600
    Samples     += [getSampleUncertainties("VLQ_TT_700","302470.",  ObjectSystematics , WeightSystematics)]#TT 700
    Samples     += [getSampleUncertainties("VLQ_TT_750","302471.",  ObjectSystematics , WeightSystematics)]#TT 750
    Samples     += [getSampleUncertainties("VLQ_TT_800","302472.",  ObjectSystematics , WeightSystematics)]#TT 800
    Samples     += [getSampleUncertainties("VLQ_TT_850","302473.",  ObjectSystematics , WeightSystematics)]#TT 850
    Samples     += [getSampleUncertainties("VLQ_TT_900","302474.",  ObjectSystematics , WeightSystematics)]#TT 900
    Samples     += [getSampleUncertainties("VLQ_TT_950","302475.",  ObjectSystematics , WeightSystematics)]#TT 950
    Samples     += [getSampleUncertainties("VLQ_TT_1000","302476."+campaign, ObjectSystematics , WeightSystematics)]#TT 1000
    Samples     += [getSampleUncertainties("VLQ_TT_1050","302477."+campaign, ObjectSystematics , WeightSystematics)]#TT 1050
    Samples     += [getSampleUncertainties("VLQ_TT_1100","302478."+campaign, ObjectSystematics , WeightSystematics)]#TT 1100
    Samples     += [getSampleUncertainties("VLQ_TT_1150","302479."+campaign, ObjectSystematics , WeightSystematics)]#TT 1150
    Samples     += [getSampleUncertainties("VLQ_TT_1200","302480."+campaign, ObjectSystematics , WeightSystematics)]#TT 1200
    Samples     += [getSampleUncertainties("VLQ_TT_1300","302481."+campaign, ObjectSystematics , WeightSystematics)]#TT 1300
    Samples     += [getSampleUncertainties("VLQ_TT_1400","302482."+campaign, ObjectSystematics , WeightSystematics)]#TT 1400
    Samples     += [getSampleUncertainties("VLQ_TT_1500","308294."+campaign, ObjectSystematics , WeightSystematics)]#TT 1500                                                                  
    Samples     += [getSampleUncertainties("VLQ_TT_1600","308295."+campaign, ObjectSystematics , WeightSystematics)]#TT 1600 

    return Samples

##______________________________________________________________________
##
def GetSingleVLQSamples( useWeightSyst=False, useObjectSyst=False, campaign=""):

    ObjectSystematics = []
    WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]
        
    Samples     =  []

    # Samples     += [getSampleUncertainties("sVLQ_WTHt11K03","310778."+campaign, ObjectSystematics , WeightSystematics)]
    # Samples     += [getSampleUncertainties("sVLQ_WTHt16K03","311376."+campaign, ObjectSystematics , WeightSystematics)]
    # Samples     += [getSampleUncertainties("sVLQ_WTHt16K05","311377."+campaign, ObjectSystematics , WeightSystematics)]
    # Samples     += [getSampleUncertainties("sVLQ_WTHt20K05","310777."+campaign, ObjectSystematics , WeightSystematics)]
    # #Samples     += [getSampleUncertainties("sVLQ_WTHt20K05R20","307018."+campaign, ObjectSystematics , WeightSystematics)]

    # Samples     += [getSampleUncertainties("sVLQ_WTZt11K03","310776."+campaign, ObjectSystematics , WeightSystematics)]
    # Samples     += [getSampleUncertainties("sVLQ_WTZt16K05","306997."+campaign, ObjectSystematics , WeightSystematics)]
    # Samples     += [getSampleUncertainties("sVLQ_WTZt20K05","310779."+campaign, ObjectSystematics , WeightSystematics)]

    # Samples     += [getSampleUncertainties("sVLQ_ZTHt11K05","307054."+campaign, ObjectSystematics , WeightSystematics)]
    # Samples     += [getSampleUncertainties("sVLQ_ZTHt16K03","311365."+campaign, ObjectSystematics , WeightSystematics)]
    # Samples     += [getSampleUncertainties("sVLQ_ZTHt16K05","307059."+campaign, ObjectSystematics , WeightSystematics)]
    # Samples     += [getSampleUncertainties("sVLQ_ZTHt20K05","307061."+campaign, ObjectSystematics , WeightSystematics)]

    # Samples     += [getSampleUncertainties("sVLQ_ZTZt11K05","307042."+campaign, ObjectSystematics , WeightSystematics)]
    # Samples     += [getSampleUncertainties("sVLQ_ZTZt16K05","307047."+campaign, ObjectSystematics , WeightSystematics)]

    Samples     += [getSampleUncertainties("WTZt1100","312349."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("WTZt1300","312350."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("WTZt1500","312351."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("WTZt1700","312352."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("WTZt1900","312353."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("WTZt2100","312354."+campaign, ObjectSystematics , WeightSystematics)]

    Samples     += [getSampleUncertainties("WTHt1100","313330."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("WTHt1300","313331."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("WTHt1500","313332."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("WTHt1700","313333."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("WTHt1900","313334."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("WTHt2100","313335."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("WTHt2300","313336."+campaign, ObjectSystematics , WeightSystematics)]

    Samples     += [getSampleUncertainties("ZTHt1100","313374."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTHt1300","313375."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTHt1500","313376."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTHt1700","313377."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTHt1900","313378."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTHt2100","313379."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTHt2300","313380."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTHt1100","313381."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTHt1300","313382."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTHt1500","313383."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTHt1700","313384."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTHt1900","313385."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTHt2100","313386."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTHt2300","313387."+campaign, ObjectSystematics , WeightSystematics)]

    Samples     += [getSampleUncertainties("ZTZt1100","313316."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTZt1300","313317."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTZt1500","313318."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTZt1700","313319."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTZt1900","313320."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTZt2100","313321."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTZt2300","313322."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTZt1100","313323."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTZt1300","313324."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTZt1500","313325."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTZt1700","313326."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTZt1900","313327."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTZt2100","313328."+campaign, ObjectSystematics , WeightSystematics)]
    Samples     += [getSampleUncertainties("ZTZt2300","313329."+campaign, ObjectSystematics , WeightSystematics)]
    return Samples

def GetVLQBSamples( useWeight=False, useObjectSys=False, campaign=""):

    ObjectSystematics = []
    WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    Samples     += [getSampleUncertainties("VLQ_BB_600","302487.",  ObjectSystematics , WeightSystematics)]#BB 600
    Samples     += [getSampleUncertainties("VLQ_BB_700","302488.",  ObjectSystematics , WeightSystematics)]#BB 700
    Samples     += [getSampleUncertainties("VLQ_BB_750","302489.",  ObjectSystematics , WeightSystematics)]#BB 750
    Samples     += [getSampleUncertainties("VLQ_BB_800","302490.",  ObjectSystematics , WeightSystematics)]#BB 800
    Samples     += [getSampleUncertainties("VLQ_BB_850","302491.",  ObjectSystematics , WeightSystematics)]#BB 850
    Samples     += [getSampleUncertainties("VLQ_BB_900","302492.",  ObjectSystematics , WeightSystematics)]#BB 900
    Samples     += [getSampleUncertainties("VLQ_BB_950","302493.",  ObjectSystematics , WeightSystematics)]#BB 950
    Samples     += [getSampleUncertainties("VLQ_BB_1000","302494."+campaign, ObjectSystematics , WeightSystematics)]#BB 1000
    Samples     += [getSampleUncertainties("VLQ_BB_1050","302495."+campaign, ObjectSystematics , WeightSystematics)]#BB 1050
    Samples     += [getSampleUncertainties("VLQ_BB_1100","302496."+campaign, ObjectSystematics , WeightSystematics)]#BB 1100
    Samples     += [getSampleUncertainties("VLQ_BB_1150","302497."+campaign, ObjectSystematics , WeightSystematics)]#BB 1150
    Samples     += [getSampleUncertainties("VLQ_BB_1200","302498."+campaign, ObjectSystematics , WeightSystematics)]#BB 1200
    Samples     += [getSampleUncertainties("VLQ_BB_1300","302499."+campaign, ObjectSystematics , WeightSystematics)]#BB 1300
    Samples     += [getSampleUncertainties("VLQ_BB_1400","302500."+campaign, ObjectSystematics , WeightSystematics)]#BB 1400
    Samples     += [getSampleUncertainties("VLQ_BB_1500","308303."+campaign, ObjectSystematics , WeightSystematics)]#BB 1500
    Samples     += [getSampleUncertainties("VLQ_BB_1600","308304."+campaign, ObjectSystematics , WeightSystematics)]#BB 1600

##______________________________________________________________________
##
def GetUEDRPPSamples( useWeightSyst=False, useObjectSyst=False, campaign="" ):

    ObjectSystematics = []
    WeightSystematics = []
    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    Samples     += [getSampleUncertainties("UEDRPP_1000","302055."+campaign, ObjectSystematics , WeightSystematics)]#UED RPP 1 TeV
    Samples     += [getSampleUncertainties("UEDRPP_1200","302056."+campaign, ObjectSystematics , WeightSystematics)]#UED RPP 1.2 TeV
    Samples     += [getSampleUncertainties("UEDRPP_1400","302057."+campaign, ObjectSystematics , WeightSystematics)]#UED RPP 1.4 TeV
    Samples     += [getSampleUncertainties("UEDRPP_1600","302058."+campaign, ObjectSystematics , WeightSystematics)]#UED RPP 1.6 TeV
    Samples     += [getSampleUncertainties("UEDRPP_1800","302059."+campaign, ObjectSystematics , WeightSystematics)]#UED RPP 1.8 TeV
    return Samples


##______________________________________________________________________
##
def Get4topsCISamples( useWeightSyst=False, useObjectSyst=False, campaign="" ):
    ObjectSystematics = []
    WeightSystematics = []

    if useObjectSyst:
        ObjectSystematics += CommonObjectSystematics
    else:
        ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

    Samples     =  []
    Samples     += [getSampleUncertainties("CI4tops","302777."+campaign, ObjectSystematics , WeightSystematics)]#Contact interaction
    return Samples

##______________________________________________________________________
##
def GetDataSamples( sampleName = "Data", data_type = "TOPQ4" ):
    Samples     =  []
    Samples     += [getSampleUncertainties(sampleName+".grp15","AllYear.data_2015"+".DAOD_"+data_type,
                                           [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
    Samples     += [getSampleUncertainties(sampleName+".grp16","AllYear.data_2016"+".DAOD_"+data_type,
                                           [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
    Samples     += [getSampleUncertainties(sampleName+".grp17","AllYear.data_2017"+".DAOD_"+data_type,
                                           [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
    Samples     += [getSampleUncertainties(sampleName+".grp18","AllYear.data_2018"+".DAOD_"+data_type,
                                           [getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
    return Samples;

##______________________________________________________________________
##
def GetQCDSamples( data_type = "TOPQ1" ):
    Samples     =  []
    Samples     += [getSampleUncertainties("QCDE","QCD.DAOD_"+data_type+".",[getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
    Samples     += [getSampleUncertainties("QCDMU","QCD.DAOD_"+data_type+".",[getSystematics(name="nominal",nameUp="",oneSided=True)] ,[])]
    return Samples;

