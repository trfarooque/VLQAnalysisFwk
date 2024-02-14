# Extracts data for plotting later. Should be run in the normal analysis environemnt.

import ROOT
import argparse
import numpy as np
import scipy.optimize as spo
import scipy.integrate as spi
import re
import fit_qcd_tools as fqt


df = fqt.getData("/nfs/atlas/tprocter/VLQs/OutputnodPhiFineBinned_dataMC_21.2.213_2023_11_22_1755_combined")
df.to_csv("dataset.csv")


