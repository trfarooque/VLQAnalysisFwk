#!/bin/env python

# Generic utility functions for SusyTest0
#
# davide.gerbaudo@gmail.com
# 2013-07-25

import collections
import difflib
from functools import wraps
import glob
import json
import os
import re
import sys
import subprocess
import unittest

def get_command_output(command, with_current_environment=False):
    "lifted from supy (https://github.com/elaird/supy/blob/master/utils/io.py)"
    env = None if not with_current_environment else os.environ.copy()
    p = subprocess.Popen(command, shell = True, stdout = subprocess.PIPE, stderr = subprocess.PIPE, env=env)
    stdout,stderr = p.communicate()
    return {"stdout":stdout, "stderr":stderr, "returncode":p.returncode}

def filter_with_regexp(string_list, regexp, func=lambda x : x) :
    return [d for d in string_list if re.search(regexp, func(d))]

def exclude_with_regexp(string_list, regexp, func=lambda x : x) :
    return [d for d in string_list if not re.search(regexp, func(d))]

def find_latest_one_or_two_root_files(dir) :
    files = filter(os.path.isfile, glob.glob(dir + "*.root"))
    files.sort(key=lambda x: os.path.getmtime(x))
    return files[-2:] if len(files)>=2 else files

def find_last_root_file(dir) : return find_latest_one_or_two_root_files(dir)[-1]

def common_prefix(list) : return os.path.commonprefix(list)

def common_suffix(list) : return os.path.commonprefix([l[::-1] for l in list])[::-1]

def longest_common_substring(s1, s2) :
    m = difflib.SequenceMatcher(None, s1, s2).find_longest_match(0, len(s1), 0, len(s2))
    return s1[m.a : m.a+m.size]

class Memoize :
    """A class to cache cpu-intensive functions.
    Arguments must be hashable.
    See for example
    http://stackoverflow.com/questions/1988804/what-is-memoization-and-how-can-i-use-it-in-python
    """
    def __init__(self, f) :
        self.f = f
        self.memo = {}
    def __call__(self, *args) :
        if not args in self.memo : self.memo[args] = self.f(*args)
        return self.memo[args]

def dict_keys_sorted_by_value(a_dict={}) :
    "Given a dict, return its keys sorted by their values"
    return [x[0] for x in sorted(a_dict.iteritems(), key=operator.itemgetter(1))]

def dict_sum(d0, d1) :
    "see http://stackoverflow.com/questions/6005066/adding-dictionaries-together-python"
    return dict(d0, **d1)

def first(list_or_dict) :
    lod = list_or_dict
    return lod.itervalues().next() if type(lod) is dict else lod[0] if lod else None

def json_write(obj, fname) :
    with open(fname, 'w') as out :
        json.dump(obj, out)

def json_read(fname) :
    with open(fname) as inp :
        return json.load(inp)

def rm_if_exists(filename) :
    if os.path.exists(filename) : os.remove(filename)

def mkdir_if_needed(dirname, verbose=False) :
    dest_dir = None
    if os.path.exists(dirname) and os.path.isdir(dirname) :
        dest_dir = dirname
    elif not os.path.exists(dirname) :
        os.makedirs(dirname)
        if verbose: print "created %s" % dirname
        dest_dir = dirname
    return dest_dir

def vertical_slice(list2d) :
    "http://stackoverflow.com/questions/6253586/python-vertical-array-slicing"
    return zip(*list2d)

def transpose_dict(d) :
    "given a dict[key1][key2] return dict[key2][key1]"
    possible_k2s = [sorted(row.keys()) for row in d.values()]
    assert len(frozenset(possible_k2s[0]))==len(possible_k2s[0]),"ambigous keys, cannot transpose %s"%str(possible_k2s[0])
    assert len(frozenset([frozenset(ks) for ks in possible_k2s])),"rows with different keys, cannot transpose %s"%str(possible_k2s)
    k2s = first(possible_k2s)
    return dict([(k2, dict([(k1, d[k1][k2]) for k1 in d.keys()])) for k2 in k2s])

def rename_dict_key(d, old, new) :
    d[new] = d.pop(old)
    return d

def sorted_as(d={}, sorted_keys=[]) :
    "take a dictionary and access its item with a specified order; unspecified keys go at the end"
    all_keys = d.keys()
    keys = [k for k in sorted_keys if k in all_keys] + [k for k in all_keys if k not in sorted_keys]
    #return collections.OrderedDict([(k, d[k]) for k in keys]) # OrderedDict not available in 2.6.5 ??
    return [(k, d[k]) for k in keys]

def remove_duplicates(seq=[]) :
    "see http://stackoverflow.com/questions/480214/how-do-you-remove-duplicates-from-a-list-in-python-whilst-preserving-order"
    seen = set()
    seen_add = seen.add
    return [ x for x in seq if x not in seen and not seen_add(x)]

def print_running_conditions(parser, opts):
    print "working from {0}".format(os.getcwd())
    print "being called as : {0}".format(' '.join(os.sys.argv))

def print_parsed_options(parser, opts):
    all_options = [x.dest for x in parser._get_all_options()[1:]]
    print "options parsed:\n"+'\n'.join("%s : %s"%(o, str(getattr(opts, o))) for o in all_options)

def guess_id_from_name(samplename='', verbose=False):
    "extract numeric dsid or run number from sample name; raise AttributeError if not a parseable name"
    match = re.search('\.(\d+)\.', samplename)
    num_id = None
    try:
        num_id = match.group(1)
    except AttributeError:
        raise ValueError("'%s' does not match \.(\d+)\. : cannot extract dsid" % samplename)
    return num_id

def filename_without_extension(filename):
    if not os.path.isfile(filename):
        raise IOError("'%s' is not a file" % filename)
    return os.path.splitext(os.path.basename(filename))[0]

def drop_continuation_lines(split_lines=[]):
    """remove continuation characters '\' from split lines

    Adapted from
    http://stackoverflow.com/questions/16480495/read-a-file-with-line-continuation-characters-in-python
    """
    out_lines = []
    current_line = ''
    for line in split_lines:
        line = line.rstrip('\n')
        if line.endswith('\\'):
            current_line += line[:-1]
        else:
            out_lines.append(current_line)
            current_line = ''
    return out_lines
#
# testing
#
class test_guess_md_tag(unittest.TestCase) :
    def test_known_values(self) :
        known_values = [('out/foo/ttbar_Aug_23.root',     'Aug_23'),
                       ('foo/baz_Aug/ttbar_Aug_23.root', 'Aug_23'),
                       ('out/foo/ttbar_Aug_2.root',      'Aug_2'),
                       ('out/foo/ttbar_August_2.root',    None),
                       ]
        for s, tag in  known_values :
            g_tag = guess_month_day_tag(s)
            self.assertEqual(tag, g_tag)

if __name__ == "__main__":
    unittest.main()
