#!/bin/env python

description = """
Given an input file, merge the histograms from several regions.
"""

usage = """
    %prog merge_config.txt in.root out.root

Merged histograms are written to out.root.

Regions to be merged and output histogram filenames are specified in merge_config.txt.

Example:
$ %prog --generate-config > merge_config.txt

# Edit merge_config.txt
# If needed, print all the region names:
$ %prog input.root --print-regions

# then
$ merge_regions.py merge_config.txt input.root output.root

Histograms that are not in the list of the regions to merge, or that
don't have <variable> in their name, are just copied to the ouput.

davide.gerbaudo@gmail.com
Aug 2016
"""

import collections
import optparse
import os
import sys
import ROOT as R
R.gROOT.SetBatch(1)

def main():
    parser = optparse.OptionParser(description=description, usage=usage)
    parser.add_option('-g', '--generate-config', action='store_true', help='print an example merge configuration')
    parser.add_option('-v', '--variable', default='meff', help='variable used to build parse the histogram names')
    parser.add_option('-p', '--print-regions', action='store_true', help='print the selection regions available in the input')
    parser.add_option('-s', '--skip-clones', action='store_true', help='skip the region clones with suffix _e, _mu')
    (opts, args) = parser.parse_args()
    if opts.generate_config:
        print example_merge_config()
        return
    elif opts.print_regions:
        if len(args)<1 : parser.error('provide an input file')
        input_file = args[0]
        variable = opts.variable
        print "List of available regions for variable '%s':" % variable
        print '\n'.join(sorted(histonames_by_region(get_histonames(R.TFile.Open(input_file), variable)[0], variable)))
        return
    if len(args)!=3:
        parser.error('Invalid arguments')

    input_config = args[0]
    input_filename = args[1]
    output_filename = args[2]
    variable = opts.variable
    if not (input_config.endswith('.txt') and os.path.exists(input_config)):
        parser.error("Invalid input configuration %s" % input_config)
    if not (input_filename.endswith('.root') and os.path.exists(input_filename)):
        parser.error("Invalid input root file %s" % input_filename)
    if not (output_filename.endswith('.root') and not os.path.exists(output_filename)):
        parser.error("Invalid output root file %s" % output_filename)

    merge_configurations = [MergeConfiguration(*l) for l in eval(open(input_config).read())]
    if not opts.skip_clones:
        suffixes = ['', '_e', '_mu']
        merge_configurations = [mc.clone_with_suffix(s) for s in suffixes for mc in merge_configurations]
    input_file = R.TFile.Open(input_filename)
    histonames, other_histonames = get_histonames(input_file, variable)
    regions_histonames = histonames_by_region(histonames, variable)
    check_all_regions_have_the_same_number_of_histgrams(regions_histonames)
    regions_histonames_to_merge = {r:hs for r, hs in regions_histonames.iteritems()
                                   if any(mc.histogram_should_be_merged(hn, variable)
                                          for hn in hs
                                          for mc in merge_configurations)}
    regions_histonames_to_copy = {r:hs for r, hs in regions_histonames.iteritems()
                                  if not any(mc.histogram_should_be_merged(hn, variable)
                                             for hn in hs
                                             for mc in merge_configurations)}
    output_file = R.TFile.Open(output_filename, 'recreate')
    output_file.cd()
    counter_regions_read = 0
    counter_regions_write = 0
    for hn in other_histonames:
        h = input_file.Get(hn)
        counter_regions_read += 1
        h.SetDirectory(output_file)
        h.Write()
        counter_regions_write += 1
    for mc in merge_configurations:
        target_region = mc.target_regionname
        input_regions = mc.input_regionnames
        master_region = input_regions[-1]
        other_regions = input_regions[:-1]
        histonames = regions_histonames_to_merge[master_region]
        print "merging %s to %s" % (str(input_regions), target_region)
        for hn in histonames:
            master_histo = input_file.Get(hn)
            other_histos = [input_file.Get(hn.replace(master_region, r)) for r in other_regions]
            master_histo.SetName(master_histo.GetName().replace(master_region, target_region))
            master_histo.SetDirectory(output_file)
            for h in other_histos:
                master_histo.Add(h)
            master_histo.Write()
            counter_regions_read += len(input_regions)
            counter_regions_write += 1
    for region, histonames in regions_histonames_to_copy.iteritems():
        print "copying %s" % str(region)
        histos = [input_file.Get(hn) for hn in histonames]
        for h in histos:
            h.SetDirectory(output_file)
            h.Write()
            counter_regions_read += 1
            counter_regions_write += 1
    output_file.Close()
    print "Read %d histograms from %s" % (counter_regions_read, input_filename)
    print "Wrote %d histograms to %s" % (counter_regions_write, output_filename)

def get_histonames(input_file, variable):
    keys = input_file.GetListOfKeys()
    histonames = list(set([k.GetName() for k in keys if isTH(k.GetClassName())])) # protect against clones
    histonames_with_var = sorted([n for n in histonames if variable in n and ('vs_'+variable) not in n])
    histonames_wout_var = sorted([n for n in histonames if n not in histonames_with_var])
    return histonames_with_var, histonames_with_var

def isTH(classname):
    if not hasattr(isTH, 'th1') : isTH.th1 = R.TH1.Class() # cache function attr
    return R.TClass(classname).InheritsFrom(isTH.th1)

def histonames_by_region(histonames, variable):
    "assumes that all the histograms are named like <region>_<variable>_<other stuff>"
    variable = ('_'+variable) if variable[0]!='_' else variable # prepend _ if necessary
    def extract_region(n, v):
        "strip everything in name after var"
        i = n.index(v) if v in n else 0
        return n[:i]
    names_by_region = collections.defaultdict(list)
    for n in histonames:
        names_by_region[extract_region(n, variable)].append(n)
    return names_by_region

def check_all_regions_have_the_same_number_of_histgrams(regions_histonames):
    one_length = len(list(set(len(hs) for hs in regions_histonames.values())))==1
    if not one_length:
        def most_common(lst):
            return max(set(lst), key=lst.count)
        mc = most_common([len(hs) for hs in regions_histonames.values()])
        outliers = ["'%s':[%d]" % (r, len(hs)) for r, hs in regions_histonames.iteritems() if len(hs)!=mc]
        msg = ("check_all_regions_have_the_same_number_of_histgrams:"
               +" most common: %d\n"%mc
               +" outliers:\n" + '\n'.join(outliers))
        raise RuntimeWarning(msg)

class MergeConfiguration(object):
    """Holder of a configuration to merge two (or more) histograms.
    When the target_regionname is not there, use the last input histogram instead.
    """
    def __init__(self, target_regionname, input_regionnames=[]):
        self._target_regionname = target_regionname
        self.input_regionnames = input_regionnames
        if len(input_regionnames)<2:
            raise NotImplementedError("need at least two regions to merge: %s" % str(input_regionnames))
    @property
    def target_regionname(self):
        return self._target_regionname if self._target_regionname else self.input_regionnames[-1]
    def clone_with_suffix(self, suffix):
        "provide a clone where suffix is appended to all available histonames"
        return MergeConfiguration(target_regionname=(self._target_regionname+suffix) if self._target_regionname else None,
                                  input_regionnames=[hn+suffix for hn in self.input_regionnames])
    def histogram_should_be_merged(self, histoname, variablename):
        return any(histoname.startswith(r+'_'+variablename) for r in self.input_regionnames)
    def __str__(self):
        return "MergeConfiguration: target '%s', inputs '%s'" % (self.target_regionname, str(self.input_regionnames))


def example_merge_config():
    return """
# Each line should be formatted as
#     (<output_histo_name>, (<input_1_histo_name>, <input_2_histo_name>, ...))
# When output_histo_name is not specified (or None),
# then the last input histogram is used as output_histo_name
(
    (None, ('c1l2RCTTMass5j2b', 'c1l1RCTTMass5j2b')),
    (None, ('c1l2RCTTMass5j3b', 'c1l1RCTTMass5j3b')),
    ('aDummyHisto', ('c1l2RCTTMass5j4b', 'c1l1RCTTMass5j4b')),
)
"""
if __name__=='__main__':
    main()
