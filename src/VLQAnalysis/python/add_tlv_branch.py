#!/bin/env python

"""Example showing how to add a branch to an existing tree.

The branch contains some values computed with a TLorentzVector.

davide.gerbaudo@gmail.com
Jan 2017
"""

import array
import os
import sys
import ROOT as R
R.gROOT.SetBatch(1)

def main():
    if len(sys.argv)!=3:
        print "Usage: %s input_file.root output_file.root"
        print "example input file:"
        print ("/afs/cern.ch/work/o/orlando/public/ForDavide/"
               "user.mcasolin.344081.MadGraphPythia8EvtGen.DAOD_TOPQ1."
               "e5052_a766_a821_r7676_p2669.HtX4Tops_00-00-12_out_TREE.root_TREE.root")
        return
    input_filename = sys.argv[1]
    output_filename = sys.argv[2]
    tree_name = 'tree'
    max_entries_to_process = 10 # -1 : process all

    input_file = R.TFile.Open(input_filename)
    input_tree = input_file.Get(tree_name)



    output_file = R.TFile.Open(output_filename, 'recreate')
    output_tree = input_tree.CloneTree(max_entries_to_process)
    output_tree.SetDirectory(output_file)
    # one alternative is to clone a subset of the branches...let me know if that's a use case
    m_jj = array.array('d', [0.0])
    branch_m_jj = output_tree.Branch('m_jj', m_jj, 'm_jj[1]/D')

    # speedup (not necessary): get the branch addresses before the event loop
    input_tree.GetEntry(0)
    jets_pt  = input_tree.jets_pt
    jets_eta = input_tree.jets_eta
    jets_phi = input_tree.jets_phi
    jets_m   = input_tree.jets_m
    jets_n   = input_tree.jets_n
    # speedup (not necessary): cache Tlv
    Tlv = R.TLorentzVector

    for iEntry in xrange(input_tree.GetEntries()):
        input_tree.GetEntry(iEntry)
        jets_p4 = [Tlv(0.0, 0.0, 0.0, 0.0) for i in xrange(len(jets_pt))]
        [j.SetPtEtaPhiM(pt, eta, phi, m)
         for j, pt, eta, phi, m in zip(jets_p4, jets_pt, jets_eta, jets_phi, jets_m)]

        if len(jets_p4)<2:
            m_jj[0] = 0.0
        else:
            jets_p4 = sorted(jets_p4, key=lambda j: j.Pt(), reverse=True) # make sure they're sorted in pt
            j1 = jets_p4[0]
            j2 = jets_p4[1]
            m_jj[0] = (j1+j2).M()
        branch_m_jj.Fill()
        if max_entries_to_process!=-1 and iEntry>max_entries_to_process:
            break
    print "processed %d entries" % max_entries_to_process
    output_file.Write()
    output_file.Close()
    input_file.Close()

    print "\nNow try:"
    print "    root -l %s" % output_filename
    print "    %s->Scan(\"jets_n:jets_pt[0]:jets_pt[1]:m_jj[0]\")" % tree_name

#___________________________________________________________

if __name__=='__main__':
    main()
