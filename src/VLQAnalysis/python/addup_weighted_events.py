#!/bin/env python

# collect the number of weighted events used for normalization

# The normalization of each simulated sample is computed based on the
# info in
#    VLQAnalysis/data/samples_info.dat
# which is
#    <dsid>. <number of weighted events> <cross section in pb>
# This script provides you with the 2nd parameter, extracted by
# looping on the input files.
#
# davide.gerbaudo@gmail.com
# Mar 2016

import optparse
import os
import ROOT as R
R.gROOT.SetBatch(1)
R.PyConfig.IgnoreCommandLineOptions = True # don't let root steal your cmd-line options

usage="""
    %prog input1 [input2 ...] [options]
    where
    input1, input2, ... are root files or dirs or filelist.txt
    Example:
    %prog \
    input/sample1/*/*root \
    -v \
    """

default_tree_name = 'sumWeights'
default_leaf_name = 'totalEventsWeighted'

def main():
    parser = optparse.OptionParser(usage=usage)
    parser.add_option('-t', '--tree-name', default=default_tree_name)
    parser.add_option('-l', '--leaf-name', default=default_leaf_name)
    parser.add_option('-v', '--verbose', action='store_true', default=False)
    (opts, args) = parser.parse_args()
    verbose = opts.verbose

    input_files = [i for ia in args for i in parse_input(ia, opts)]
    all_options = [x.dest for x in parser._get_all_options()[1:]]
    if opts.verbose :
        print '\nUsing the following opts:\n'
        print '\n'.join("%s : %s"%(o, str(getattr(opts, o))) for o in all_options)

    tree_name = opts.tree_name
    leaf_name = opts.leaf_name
    counter = 0.0
    chain = R.TChain(tree_name)
    for input_file_name in input_files:
        chain.Add(input_file_name)
    for event in chain:
        counter += getattr(event, leaf_name)
    print(counter)

def args_are_valid(args):
    return (len(args)>=1 and
            all(os.path.exists(a) for a in args))

def parse_input(name, opts):
    verbose = opts.verbose
    result = []
    if os.path.isfile(name) and name.endswith('.txt'):
        result = [l.strip() for l in open(name).readlines() if '.root' in l]
    elif os.path.isfile(name) and '.root' in name:
        result = [name]
    elif os.path.isdir(name):
        result = glob.glob(os.path.join(name, '*.root*'))
    if verbose : print "parsed {0} files from input {1}".format(len(result), name)
    return result

if __name__=='__main__':
    main()
