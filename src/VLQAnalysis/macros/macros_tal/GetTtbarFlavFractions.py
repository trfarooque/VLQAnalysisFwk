from ROOT import *

# pathbase = '/nfs/atlas-data07/tvdaalen/SingleVLQWorkArea/RootFiles/RootFiles_Ttbar+Singletop_Syst/'
# pathbase = '/nfs/atlas-data07/tvdaalen/SingleVLQWorkArea/RootFiles/RootFiles_Ttbar_HF_reweighting/HFunreweighted/'
# pathbase = '/nfs/atlas-data07/tvdaalen/SingleVLQWorkArea/RootFiles/RootFiles_Ttbar_HF_reweighting/HFreweighted/'
# pathbase = '/nfs/atlas-data07/tvdaalen/VLQAnalysisRun2/FitInputs_RW_Jun2020/FilesTRexF/TtStAlt/'
pathbase = '/nfs/atlas-data07/tvdaalen/VLQAnalysisRun2/FitInputs_RW_HFcor_Debug_Jun2020/FilesTRexF/TheorySys/'
campaigns = ['a','d','e']
flavs = ['ttbarlight','ttbarcc','ttbarbb']
gens = ['AFII','PowHer','aMCPy'] #['Nominal_AFII','PowhegHerwig7','aMcAtNloPythia8']

pmgs = ['','weight_pmg_muR10__muF20','weight_pmg_muR10__muF05','weight_pmg_muR20__muF10','weight_pmg_muR05__muF10','weight_pmg_Var3cUp','weight_pmg_Var3cDown','weight_pmg_isr_muRfac10__fsr_muRfac20','weight_pmg_isr_muRfac10__fsr_muRfac05']

nominal_gen = 'AFII'

evnts = {}

"""
# check alt samples:
for gen in gens:
    evnts[gen] = {}
    evnts[gen]['total'] = 0.
    for flav in flavs:
        evnts[gen][flav] = 0.
        for campaign in campaigns:

            if gen == nominal_gen:
                fn = pathbase+flav+'.mc16'+campaign+'.root'
            else:
                fn = pathbase+flav+gen+'.mc16'+campaign+'.root'

            f = TFile(fn,'read')
            h = f.Get("c1lep3jin1bin_meff")
            evnts[gen][flav] += h.Integral()
            evnts[gen]['total'] += h.Integral()

for gen in gens:
    print gen
    for flav in flavs:
        print flav
        # print 'ratio to nominal:',evnts[gen][flav]/evnts['Nominal_AFII'][flav],'\tfraction of total:',evnts[gen][flav]/evnts[gen]['total'],'\tratio to nominal of fraction:',(evnts[gen][flav]/evnts[gen]['total'])/(evnts['Nominal_AFII'][flav]/evnts['Nominal_AFII']['total'])
        print 'ratio to nominal:',evnts[gen][flav]/evnts[nominal_gen][flav],'\tfraction of total:',evnts[gen][flav]/evnts[gen]['total'],'\tratio to nominal of fraction:',(evnts[nominal_gen][flav]/evnts[nominal_gen]['total'])/(evnts[gen][flav]/evnts[gen]['total'])
    print ""
"""

# check pmg distributions:
for pmg in pmgs:
    evnts[pmg] = {}
    evnts[pmg]['total'] = 0.
    for flav in flavs:
        evnts[pmg][flav] = 0.
        for campaign in campaigns:

            fn = pathbase+flav+'.mc16'+campaign+'.root'

            f = TFile(fn,'read')

            if pmg == '':
                h = f.Get("c1lep3jin1bin_meff")
            else:
                h = f.Get("c1lep3jin1bin_meff_"+pmg)
            evnts[pmg][flav] += h.Integral()
            evnts[pmg]['total'] += h.Integral()

for pmg in pmgs:
    print pmg
    for flav in flavs:
        print flav
        print 'ratio to nominal:',evnts[pmg][flav]/evnts[''][flav],'\tfraction of total:',evnts[pmg][flav]/evnts[pmg]['total'],'\tratio to nominal of fraction:',(evnts[''][flav]/evnts['']['total'])/(evnts[pmg][flav]/evnts[pmg]['total'])
    print ""