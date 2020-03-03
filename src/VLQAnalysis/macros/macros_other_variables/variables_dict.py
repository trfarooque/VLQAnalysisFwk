#!/bin/python


#oneLep3b = {'name':"c1lep5jin3bin",'legend':"1l, #geq5j, #geq3b"}
#oneLep2b = {'name':"c1lep5jin2bin",'legend':"1l, #geq5j, #geq2b"}
#oneLep2bex = {'name':"c1lep5jin2bex",'legend':"1l, #geq5j, 2b"}
#oneLep6j3b = {'name':"c1lep6jin3bin",'legend':"1l, #geq6j, #geq3b"}
#oneLep6j2b = {'name':"c1lep6jin2bin",'legend':"1l, #geq6j, #geq2b"}
#
#oneLep6j3b_e = {'name':"c1lep6jin3bin_el",'legend':"1e, 0#mu, #geq6j, #geq3b"}
#oneLep6j3b_mu = {'name':"c1lep6jin3bin_mu",'legend':"1#mu, 0e, #geq6j, #geq3b"}
#oneLep5j3b_e = {'name':"c1lep5jin3bin_el",'legend':"1e, 0#mu, #geq5j, #geq3b"}
#oneLep5j3b_mu = {'name':"c1lep5jin3bin_mu",'legend':"1#mu, 0e, #geq5j, #geq3b"}
#
#zeroLep3b = {'name':"c0lep6jin3bin",'legend':"0l, #geq6j, #geq3b"}
#zeroLep2b = {'name':"c0lep6jin2bin",'legend':"0l, #geq6j, #geq2b"}
#zeroLep2bex = {'name':"c0lep6jin2bex",'legend':"0l, #geq6j, 2b"}
#zeroLep3b7j = {'name':"c0lep7jin3bin",'legend':"0l, #geq7j, #geq3b"}
#zeroLep2b7j = {'name':"c0lep7jin2bin",'legend':"0l, #geq7j, #geq2b"}
#
#zeroLep2bHM = {'name':"c0lep6jin2binHighMtbmin",'legend':"0l, #geq6j, #geq2b, HM"}
#zeroLep2bexHM = {'name':"c0lep6jin2bexHighMtbmin",'legend':"0l, #geq6j, 2b, HM"}
#zeroLep2b7jHM = {'name':"c0lep7jin2binHighMtbmin",'legend':"0l, #geq7j, #geq2b, HM"}

#regions = [zeroLep2bHM,zeroLep2b7jHM]

#regions = [oneLep2b, oneLep2bex, zeroLep2b, zeroLep2bex, zeroLep2bHM, zeroLep2bexHM, oneLep3b, zeroLep3b]#, oneLep6j3b, oneLep6j2b, zeroLep3b7j, zeroLep2b7j]
#regions = [oneLep2b, oneLep3b]#, oneLep6j3b, oneLep6j2b]
#regions = [zeroLep2b, zeroLep3b]#, zeroLep3b7j, zeroLep2b7j]

sum_oneLep3b = {'name':"sum1lep5jin3bin",'legend':"1l, #geq5j, #geq3b"}
sum_oneLep2b = {'name':"sum1lep5jin2bin",'legend':"1l, #geq5j, #geq2b"}
sum_oneLep6j3b = {'name':"sum1lep6jin3bin",'legend':"1l, #geq6j, #geq3b"}
sum_oneLep6j3b1TH = {'name':"sum1lep1THin6jin3bin",'legend':"1l, #geq1TH #geq6j, #geq3b"}
#sum_oneLep6j2b = {'name':"sum1lep6jin2bin",'legend':"1l, #geq6j, #geq2b"}

sum_zeroLep3b = {'name':"sum0lep6jin3bin",'legend':"0l, #geq6j, #geq3b"}
sum_zeroLep2b = {'name':"sum0lep6jin2bin",'legend':"0l, #geq6j, #geq2b"}
#sum_zeroLep3b7j = {'name':"sum0lep7jin3bin",'legend':"0l, #geq7j, #geq3b"}
sum_zeroLep2b7j = {'name':"sum0lep7jin2bin",'legend':"0l, #geq7j, #geq2b"}
sum_zeroLep2b7j1TH = {'name':"sum0lep1THin7jin2bin",'legend':"0l, #geq1TH, #geq7j, #geq2b"}

regions = [sum_oneLep2b, sum_oneLep3b, sum_oneLep6j3b, sum_oneLep6j3b1TH, sum_zeroLep2b, sum_zeroLep2b7j, sum_zeroLep2b7j1TH]


#jets_n_1l       = {'name':"jets_n",'legend':"Jet multiplicity",'binning':"4.5,5.5,6.5,7.5,8.5,9.5,10.5"}
jets_n_1l       = {'name':"jets_n",'legend':"Jet multiplicity",'binning':"4.5,5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5,13.5,14.5"}
bjets_n         = {'name':"bjets_n",'legend':"b-jet multiplicity",'binning':"1.5,2.5,3.5,4.5,5.5"}
#meff            = {'name':"meff",'legend':"m_{eff} [GeV]",'binning':"1000,1200,1400,1600,2000,2500,3500",'width':"200"}
meff            = {'name':"meff",'legend':"m_{eff} [GeV]",'binning':"2000,2500,3500",'width':"200"}
met             = {'name':"met_zoom",'legend':"E_{T}^{miss} [GeV]"}
jet0pt          = {'name':"jet0_pt",'legend':"Leading jet p_{T} [GeV]",'binning':"50,100,150,200,250,300,350,400,450,500,550,600,700,800,1000",'width':"50"}
higgs_n         = {'name':"RCMHiggs_jets_n",'legend':"Higgs-tagged jets multiplicity",'binning':"-0.5,0.5,1.5,2.5,3.5"}
top_n           = {'name':"RCMTop_jets_n",'legend':"Top-tagged jets multiplicity",'binning':"-0.5,0.5,1.5,2.5,3.5"}
mtbmin          = {'name':"mtbmin_zoom",'legend':"m_{T,min}^{b} [GeV]"}
RCjet0_m        = {'name':"RCjet0_m",'legend':"Leading RC jet mass [GeV]",'binning':"50,70,90,110,130,150,170,190,210,240,260,300",'width':"20"}
RCjet0_pt       = {'name':"RCjet0_pt",'legend':"Leading RC jet p_{T} [GeV]"}
RCjets_m        = {'name':"RCjets_m",'legend':"RC jet mass [GeV]",'binning':"50,70,90,110,130,150,170,190,210,240,260,300",'width':"20"}
RCjets_pt       = {'name':"RCjets_pt",'legend':"RC jet p_{T} [GeV]"}
lep0_pt         = {'name':"lep0_pt_zoom",'legend':"Lepton p_{T} [GeV]",'reg':"c1lep5jin2bin,c1lep5jin3bin"}
lep0_eta         = {'name':"lep0_eta",'legend':"Lepton #eta",'binning':"-3,-2.5,-2,-1.5,-1,-0.5,0,0.5,1.0,1.5,2.0,2.5,3.0"}#,'reg':"c1lep5jin2bin,c1lep5jin3bin"}
mtw_zoom         = {'name':"mtw_zoom",'legend':"m_{T}(W) [GeV]"}#,'reg':"c1lep5jin2bin,c1lep5jin3bin"}

list_variables = []
#list_variables += [RCjets_m]
#list_variables += [RCjets_pt]
#list_variables += [RCjet0_m]
#list_variables += [RCjet0_pt]
list_variables += [jets_n_1l]
list_variables += [bjets_n]
list_variables += [meff]
#list_variables += [met]
#list_variables += [jet0pt]
list_variables += [higgs_n]
list_variables += [top_n]
list_variables += [mtbmin]
#list_variables += [RCjets_m]
#list_variables += [RCjets_pt]
#list_variables += [lep0_pt]
#list_variables += [lep0_eta]
#list_variables += [mtw_zoom]

# jet0_btagw       = {'name':"jet0_btagw",'legend':"Leading MV2c10 jet MV2c10 score", 'binning':"-1,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8,1.0"}
# jet1_btagw       = {'name':"jet1_btagw",'legend':"2nd MV2c10 jet MV2c10 score", 'binning':"-1,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8,1.0"}
# jet2_btagw       = {'name':"jet2_btagw",'legend':"2nd MV2c10 jet MV2c10 score", 'binning':"-1,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8,1.0"}
# jet3_btagw       = {'name':"jet3_btagw",'legend':"2nd MV2c10 jet MV2c10 score", 'binning':"-1,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8,1.0"}
# list_variables = []
# list_variables += [jet0_btagw]
# list_variables += [jet1_btagw]
# list_variables += [jet2_btagw]
# list_variables += [jet3_btagw]
