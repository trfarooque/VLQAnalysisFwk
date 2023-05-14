
MVA_30vars_allbkgd = {
    'path':"/data/at3/scratch3/cbuxovaz/PairProd_VLQ_Analysis_Studies/MVA_evaluation_studies/1lep/ROC_Curves_Inputs/30vars_allbkgd/",
    'label':"30 vars., all bkgd.",
    'signal':"TT_MASS_DECAY.root",
    'background':"all_background.root",
    'color':4,
    'ratioopt':"REF",
    'signal_scales':{"mc16a":36207.66, "mc16d":44037.4, "mc16e":58450.1},
    'background_scales':{"mc16a":36207.66, "mc16d":44037.4, "mc16e":58450.1}
}
MVA_30vars_ttbar = {
    'path':"/data/at3/scratch3/cbuxovaz/PairProd_VLQ_Analysis_Studies/MVA_evaluation_studies/1lep/ROC_Curves_Inputs/30vars_ttbar/",
    'label':"30 vars., ttbar only",
    'signal':"TT_MASS_DECAY.root",
    'background':"all_background.root",
    'color':2,
    'ratioopt':"INC",
    'signal_scales':{"mc16a":36207.66, "mc16d":44037.4, "mc16e":58450.1},
    'background_scales':{"mc16a":36207.66, "mc16d":44037.4, "mc16e":58450.1}
}
MVA_20vars_allbkgd = {
    'path':"/data/at3/scratch3/cbuxovaz/PairProd_VLQ_Analysis_Studies/MVA_evaluation_studies/1lep/ROC_Curves_Inputs/20vars_allbkgd/",
    'label':"20 vars., all bkgd.",
    'signal':"TT_MASS_DECAY.root",
    'background':"all_background.root",
    'color':3,
    'ratioopt':"INC",
    'signal_scales':{"mc16a":36207.66, "mc16d":44037.4, "mc16e":58450.1},
    'background_scales':{"mc16a":36207.66, "mc16d":44037.4, "mc16e":58450.1}
}

dictionary_list=[
    MVA_30vars_allbkgd,
    MVA_30vars_ttbar,
    MVA_20vars_allbkgd
]
