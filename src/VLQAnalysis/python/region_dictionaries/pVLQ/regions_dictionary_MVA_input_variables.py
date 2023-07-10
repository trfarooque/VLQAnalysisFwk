# Created with MakeRegionsDictionary at 2021-04-11 19:46:47.845114

#
#
# Search regions 1-lepton
#
#
reg_1lep6jin3bin2Min3Jin = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"1000,1200,1400,1600,2000,2500",
    'type':"SIGNAL"
}

#
#
# Validation regions MVA 1-lepton
#
#
reg_1lep6jin3bin2Min3Jin_RCMHiggs0_pt = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,50,100,150,200,250,300,350,400,450,500,550",
    'type':"VALIDATION",
    'discriminant':"RCMHiggs0_pt",
    'xtitle':"p_{T}(RCMHiggs0) [GeV]"
}
reg_1lep6jin3bin2Min3Jin_RCMTop0_eta = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"-2.2,-2.0,-1.8,-1.6,-1.4,-1.2,-1.0,-0.8,-0.6,-0.4,-0.2,0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2",
    'type':"VALIDATION",
    'discriminant':"RCMTop0_eta",
    'xtitle':'"#eta(RCMTop0)"'
}
reg_1lep6jin3bin2Min3Jin_RCMTop0_nbconsts = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"-0.5,0.5,1.5,2.5,3.5",
    'type':"VALIDATION",
    'discriminant':"RCMTop0_nbconsts",
    'xtitle':"N_{bconsts}(RCMTop0)"
}
reg_1lep6jin3bin2Min3Jin_RCMTop0_nconsts = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"1.5,2.5,3.5,4.5,5.5",
    'type':"VALIDATION",
    'discriminant':"RCMTop0_nconsts",
    'xtitle':"N_{consts}(RCMTop0)"
}
reg_1lep6jin3bin2Min3Jin_RCMTop0_pt = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750",
    'type':"VALIDATION",
    'discriminant':"RCMTop0_pt",
    'xtitle':"p_{T}(RCMTop0) [GeV]"
}
reg_1lep6jin3bin2Min3Jin_RCMTop1_pt = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,50,100,150,200,250,300,350,400,450,500,550,600",
    'type':"VALIDATION",
    'discriminant':"RCMTop1_pt",
    'xtitle':"p_{T}(RCMTop1) [GeV]"
}
reg_1lep6jin3bin2Min3Jin_RCMTop_jets_n = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"-0.5,0.5,1.5,2.5,3.5",
    'type':"VALIDATION",
    'discriminant':"RCMTop_jets_n",
    'xtitle':"N_{RCMTop}"
}
reg_1lep6jin3bin2Min3Jin_RCMV0_nbconsts = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"-0.5,0.5,1.5",
    'type':"VALIDATION",
    'discriminant':"RCMV0_nbconsts",
    'xtitle':"N_{bconsts}(RCMV0)"
}
reg_1lep6jin3bin2Min3Jin_RCMV0_pt = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,50,100,150,200,250,300,350,400,450,500,550,600",
    'type':"VALIDATION",
    'discriminant':"RCMV0_pt",
    'xtitle':"p_{T}(RCMV0) [GeV]"
}
reg_1lep6jin3bin2Min3Jin_RCMV_jets_n = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"-0.5,0.5,1.5",
    'type':"VALIDATION",
    'discriminant':"RCMV_jets_n",
    'xtitle':"N_{RCMV}"
}
reg_1lep6jin3bin2Min3Jin_RCjet0_pt = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750",
    'type':"VALIDATION",
    'discriminant':"RCjet0_pt",
    'xtitle':"p_{T}(RCjet0) [GeV]"
}
reg_1lep6jin3bin2Min3Jin_RCjet1_pt = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,50,100,150,200,250,300,350,400,450,500,550,600",
    'type':"VALIDATION",
    'discriminant':"RCjet1_pt",
    'xtitle':"p_{T}(RCjet1) [GeV]"
}
reg_1lep6jin3bin2Min3Jin_RCjet2_pt = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,50,100,150,200,250,300,350,400,450",
    'type':"VALIDATION",
    'discriminant':"RCjet2_pt",
    'xtitle':"p_{T}(RCjet2) [GeV]"
}
reg_1lep6jin3bin2Min3Jin_bjets_n = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"-0.5,0.5,1.5,2.5,3.5,4.5",
    'type':"VALIDATION",
    'discriminant':"bjets_n",
    'xtitle':"N_{bjets}^{trk}"
}
reg_1lep6jin3bin2Min3Jin_dEtaavg_RCMTT = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,4.0",
    'type':"VALIDATION",
    'discriminant':"dEtaavg_RCMTT",
    'xtitle':'"#Delta#eta_{avg}(RCMTT,RCMTT)"'
}
reg_1lep6jin3bin2Min3Jin_dEtaavg_RCjets = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6",
    'type':"VALIDATION",
    'discriminant':"dEtaavg_RCjets",
    'xtitle':'"#Delta#eta_{avg}(RC,RC)"'
}
reg_1lep6jin3bin2Min3Jin_dEtamin_RCMTT = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8",
    'type':"VALIDATION",
    'discriminant':"dEtamin_RCMTT",
    'xtitle':'"#Delta#eta_{min}(RCMTT,RCMTT)"'
}
reg_1lep6jin3bin2Min3Jin_dEtamin_RCjets = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0",
    'type':"VALIDATION",
    'discriminant':"dEtamin_RCjets",
    'xtitle':'"#Delta#eta_{min}(RC,RC)"'
}
reg_1lep6jin3bin2Min3Jin_dPhiavg_RCMET = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2",
    'type':"VALIDATION",
    'discriminant':"dPhiavg_RCMET",
    'xtitle':'"#Delta#phi_{avg}(RC,MET)"'
}
reg_1lep6jin3bin2Min3Jin_dPhiavg_RCMTTMET = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2",
    'type':"VALIDATION",
    'discriminant':"dPhiavg_RCMTTMET",
    'xtitle':'"#Delta#phi_{avg}(RCMTT,MET)"'
}
reg_1lep6jin3bin2Min3Jin_dPhimin_RCMET = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6",
    'type':"VALIDATION",
    'discriminant':"dPhimin_RCMET",
    'xtitle':'"#Delta#phi_{min}(RC,MET)"'
}
reg_1lep6jin3bin2Min3Jin_dPhimin_RCMTT = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3",
    'type':"VALIDATION",
    'discriminant':"dPhimin_RCMTT",
    'xtitle':'"#Delta#phi_{min}(RCMTT,RCMTT)"'
}
reg_1lep6jin3bin2Min3Jin_dPhimin_RCMTTMET = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8",
    'type':"VALIDATION",
    'discriminant':"dPhimin_RCMTTMET",
    'xtitle':'"#Delta#phi_{min}(RCMTT,MET)"'
}
reg_1lep6jin3bin2Min3Jin_dPhimin_RCjets = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2",
    'type':"VALIDATION",
    'discriminant':"dPhimin_RCjets",
    'xtitle':'"#Delta#phi_{min}(RC,RC)"'
}
reg_1lep6jin3bin2Min3Jin_dRavg_RCjets = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7",
    'type':"VALIDATION",
    'discriminant':"dRavg_RCjets",
    'xtitle':'"#DeltaR_{avg}(RC,RC)"'
}
reg_1lep6jin3bin2Min3Jin_dRmin_RCMTT = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,4.0,4.1,4.2,4.3,4.4,4.5",
    'type':"VALIDATION",
    'discriminant':"dRmin_RCMTT",
    'xtitle':'"#DeltaR_{min}(RCMTT,RCMTT)"'
}
reg_1lep6jin3bin2Min3Jin_dRmin_RCjets = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,3.5,3.6",
    'type':"VALIDATION",
    'discriminant':"dRmin_RCjets",
    'xtitle':'"#DeltaR_{min}(RC,RC)"'
}
reg_1lep6jin3bin2Min3Jin_hthad = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"600,700,800,900,1000,1200,1400,1600,2000",
    'type':"VALIDATION",
    'discriminant':"hthad",
    'xtitle':"H_{T}^{had} [GeV]"
}
reg_1lep6jin3bin2Min3Jin_jets_n = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"4.5,5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5,15.5",
    'type':"VALIDATION",
    'discriminant':"jets_n",
    'xtitle':"N_{jets}"
}
reg_1lep6jin3bin2Min3Jin_leadingdEta_RCMTT = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,4.0",
    'type':"VALIDATION",
    'discriminant':"leadingdEta_RCMTT",
    'xtitle':'"Leading #Delta#eta(RCMTT,RCMTT)"'
}
reg_1lep6jin3bin2Min3Jin_leadingdEta_RCjets = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,4.0",
    'type':"VALIDATION",
    'discriminant':"leadingdEta_RCjets",
    'xtitle':'"Leading #Delta#eta(RC,RC)"'
}
reg_1lep6jin3bin2Min3Jin_leadingdPhi_RCMET = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6",
    'type':"VALIDATION",
    'discriminant':"leadingdPhi_RCMET",
    'xtitle':'"Leading #Delta#phi(RC,MET)"'
}
reg_1lep6jin3bin2Min3Jin_leadingdPhi_RCMTT = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2",
    'type':"VALIDATION",
    'discriminant':"leadingdPhi_RCMTT",
    'xtitle':'"Leading #Delta#phi(RCMTT,RCMTT)"'
}
reg_1lep6jin3bin2Min3Jin_leadingdPhi_RCMTTMET = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2",
    'type':"VALIDATION",
    'discriminant':"leadingdPhi_RCMTTMET",
    'xtitle':'"Leading #Delta#phi(RCMTT,MET)"'
}
reg_1lep6jin3bin2Min3Jin_leadingdPhi_RCjets = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2",
    'type':"VALIDATION",
    'discriminant':"leadingdPhi_RCjets",
    'xtitle':'"Leading #Delta#phi(RC,RC)"'
}
reg_1lep6jin3bin2Min3Jin_leadingdR_RCMTT = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,4.0,4.1,4.2,4.3,4.4,4.5",
    'type':"VALIDATION",
    'discriminant':"leadingdR_RCMTT",
    'xtitle':'"Leading #DeltaR(RCMTT,RCMTT)"'
}
reg_1lep6jin3bin2Min3Jin_leadingdR_RCjets = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,4.0,4.1,4.2,4.3,4.4,4.5",
    'type':"VALIDATION",
    'discriminant':"leadingdR_RCjets",
    'xtitle':'"Leading #DeltaR(RC,RC)"'
}
reg_1lep6jin3bin2Min3Jin_leptop_n = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"-0.5,0.5,1.5",
    'type':"VALIDATION",
    'discriminant':"leptop_n",
    'xtitle':"N_{leptonic top}"
}
reg_1lep6jin3bin2Min3Jin_leptop_pt = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,50,100,150,200,250,300,350,400,450,500,550,600,650,700",
    'type':"VALIDATION",
    'discriminant':"leptop_pt",
    'xtitle':"p_{T}(leptonic top) [GeV]"
}
reg_1lep6jin3bin2Min3Jin_met = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,100,200,300",
    'type':"VALIDATION",
    'discriminant':"met",
    'xtitle':"E_{T}^{miss} [GeV]"
}
reg_1lep6jin3bin2Min3Jin_mtbmin = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,20,40,60,80,100,120",
    'type':"VALIDATION",
    'discriminant':"mtbmin",
    'xtitle':"m_{T,b}^{min} [GeV]"
}
reg_1lep6jin3bin2Min3Jin_mtw = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,20,40,60,80,100,120,140,160,180,200,220,240",
    'type':"VALIDATION",
    'discriminant':"mtw",
    'xtitle':"m_{T}^{W} [GeV]"
}
reg_1lep6jin3bin2Min3Jin_ptw = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420",
    'type':"VALIDATION",
    'discriminant':"ptw",
    'xtitle':"p_{T}^{W} [GeV]"
}
reg_1lep6jin3bin2Min3Jin_residualMET = {
    'name':"HTX_c1lep6jin3bin2Min3Jin",
    'legend':"#scale[0.75]{#geq6j, #geq3b, #geq2M, #geq3J}",
    'binning':"0,20,40,60,80,100,120",
    'type':"VALIDATION",
    'discriminant':"residualMET",
    'xtitle':"Residual MET [GeV]"
}

fit_regions_1l = [
    reg_1lep6jin3bin2Min3Jin
]

validation_regions_1l = [
    reg_1lep6jin3bin2Min3Jin_RCMHiggs0_pt,
    reg_1lep6jin3bin2Min3Jin_RCMTop0_eta,
    reg_1lep6jin3bin2Min3Jin_RCMTop0_nbconsts,
    reg_1lep6jin3bin2Min3Jin_RCMTop0_nconsts,
    reg_1lep6jin3bin2Min3Jin_RCMTop0_pt,
    reg_1lep6jin3bin2Min3Jin_RCMTop1_pt,
    reg_1lep6jin3bin2Min3Jin_RCMTop_jets_n,
    reg_1lep6jin3bin2Min3Jin_RCMV0_nbconsts,
    reg_1lep6jin3bin2Min3Jin_RCMV0_pt,
    reg_1lep6jin3bin2Min3Jin_RCMV_jets_n,
    reg_1lep6jin3bin2Min3Jin_RCjet0_pt,
    reg_1lep6jin3bin2Min3Jin_RCjet1_pt,
    reg_1lep6jin3bin2Min3Jin_RCjet2_pt,
    reg_1lep6jin3bin2Min3Jin_bjets_n,
    reg_1lep6jin3bin2Min3Jin_dEtaavg_RCMTT,
    reg_1lep6jin3bin2Min3Jin_dEtaavg_RCjets,
    reg_1lep6jin3bin2Min3Jin_dEtamin_RCMTT,
    reg_1lep6jin3bin2Min3Jin_dEtamin_RCjets,
    reg_1lep6jin3bin2Min3Jin_dPhiavg_RCMET,
    reg_1lep6jin3bin2Min3Jin_dPhiavg_RCMTTMET,
    reg_1lep6jin3bin2Min3Jin_dPhimin_RCMET,
    reg_1lep6jin3bin2Min3Jin_dPhimin_RCMTT,
    reg_1lep6jin3bin2Min3Jin_dPhimin_RCMTTMET,
    reg_1lep6jin3bin2Min3Jin_dPhimin_RCjets,
    reg_1lep6jin3bin2Min3Jin_dRavg_RCjets,
    reg_1lep6jin3bin2Min3Jin_dRmin_RCMTT,
    reg_1lep6jin3bin2Min3Jin_dRmin_RCjets,
    reg_1lep6jin3bin2Min3Jin_hthad,
    reg_1lep6jin3bin2Min3Jin_jets_n,
    reg_1lep6jin3bin2Min3Jin_leadingdEta_RCMTT,
    reg_1lep6jin3bin2Min3Jin_leadingdEta_RCjets,
    reg_1lep6jin3bin2Min3Jin_leadingdPhi_RCMET,
    reg_1lep6jin3bin2Min3Jin_leadingdPhi_RCMTT,
    reg_1lep6jin3bin2Min3Jin_leadingdPhi_RCMTTMET,
    reg_1lep6jin3bin2Min3Jin_leadingdPhi_RCjets,
    reg_1lep6jin3bin2Min3Jin_leadingdR_RCMTT,
    reg_1lep6jin3bin2Min3Jin_leadingdR_RCjets,
    reg_1lep6jin3bin2Min3Jin_leptop_n,
    reg_1lep6jin3bin2Min3Jin_met,
    reg_1lep6jin3bin2Min3Jin_mtbmin,
    reg_1lep6jin3bin2Min3Jin_mtw,
    reg_1lep6jin3bin2Min3Jin_ptw,
    reg_1lep6jin3bin2Min3Jin_residualMET
]

preselection_regions_1l = []

all_regions_1l =  []
all_regions_1l += fit_regions_1l
all_regions_1l += validation_regions_1l
all_regions_1l += preselection_regions_1l

fit_regions_0l = []

validation_regions_0l = []

preselection_regions_0l = []

all_regions_0l = []
all_regions_0l += fit_regions_0l
all_regions_0l += validation_regions_0l
all_regions_0l += preselection_regions_0l
