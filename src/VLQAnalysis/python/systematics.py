"""
A catalogue of the systematic uncertainties used in the vlq/hbsm analysis

This catalogue is used to submit the utilsVLQAnalysis jobs

TODO weight variations

davide.gerbaudo@gmail.com
Jul 2016
"""

import copy

class Variation(object):
    treename_suffix = '' # this is the suffix that should be removed from the output filenames
    def __init__(self, input_tree):
        self.input_tree = input_tree
        self.is_weight_variation = False
        self.is_object_variation = False
        self._filelist = None
    @property
    def name(self):
        if self.input_tree.endswith(Variation.treename_suffix):
            return self.input_tree[:-len(Variation.treename_suffix)]
        else:
            raise ValueError("Variation: cannot interpred treename '%s' as a variation name" % self.input_tree)
    @property
    def filelist(self):
        if not self._filelist:
            raise IOError("missing input data for '%s'\nPlease call SampleCatalogue.add_filelists" % self.name)
        else:
            return self._filelist
    @filelist.setter
    def filelist(self, value):
        self._filelist = value

class WeightVariation(Variation):
    "A variation that only changes the event weight, but not the event selection"
    def __init__(self, input_tree):
        super(WeightVariation, self).__init__(input_tree)
        self.is_weight_variation = True

class ObjectVariation(Variation):
    "A variation that changes the object, and therefore the event selection"
    def __init__(self, input_tree):
        super(ObjectVariation, self).__init__(input_tree)
        self.is_object_variation = True

class SystematicUncertainty(object):
    "A syst uncertainty holds one or two variations"
    def __init__(self, name, variations=[]):
        self.name = name
        self.variations = variations
        if len(self.variations) not in [1, 2]:
            raise ValueError("SystematicUncertainty '%s' must have either 1 or 2 variations, not %d" % (name, len(variations)))
    @property
    def is_one_sided(self):
        return len(self.variations)==1
    @property
    def is_two_sided(self):
        return len(self.variations)==2
    @property
    def is_nominal(self):
        return self.name=='nominal'

class SystematicCatalogue(object):
    """A catalogue of uncertainties

    Note: when a used asks for the uncertainties (and their
    variations), we always provide a copy. The reason for this is that
    they will be attached to samples, so we want to have different
    copies of the same Variation objects for each sample.

    Note to self: perhaps I should also hide '._' all the remaining
    attributes (such as electron_object_uncertainties etc.).
    """
    def __init__(self):
        self._nominal = SystematicUncertainty(name='nominal', variations=[Variation(input_tree='nominal')])
        self.electron_object_uncertainties = [
            SystematicUncertainty(name='EG_RESOLUTION_ALL',
                                  variations=[ObjectVariation(input_tree='EG_RESOLUTION_ALL__1up'),
                                              ObjectVariation(input_tree='EG_RESOLUTION_ALL__1down')]),
            SystematicUncertainty(name='EG_SCALE_ALL',
                                  variations=[ObjectVariation(input_tree='EG_SCALE_ALL__1up'),
                                              ObjectVariation(input_tree='EG_SCALE_ALL__1down')]),
            ]
        self.jet_object_uncertainties = [
            SystematicUncertainty(name='JET_BJES_Response',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_BJES_Response__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_BJES_Response__1down')]),
            SystematicUncertainty(name='JET_EffectiveNP_1',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_EffectiveNP_1__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_EffectiveNP_1__1down')]),
            SystematicUncertainty(name='JET_EffectiveNP_2',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_EffectiveNP_2__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_EffectiveNP_2__1down')]),
            SystematicUncertainty(name='JET_EffectiveNP_3',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_EffectiveNP_3__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_EffectiveNP_3__1down')]),
            SystematicUncertainty(name='JET_EffectiveNP_4',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_EffectiveNP_4__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_EffectiveNP_4__1down')]),
            SystematicUncertainty(name='JET_EffectiveNP_5',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_EffectiveNP_5__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_EffectiveNP_5__1down')]),
            SystematicUncertainty(name='JET_EffectiveNP_6restTerm',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_EffectiveNP_6restTerm__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_EffectiveNP_6restTerm__1down')]),
            SystematicUncertainty(name='JET_EtaIntercalibration_NonClosure',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_EtaIntercalibration_NonClosure__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_EtaIntercalibration_NonClosure__1down')]),
            SystematicUncertainty(name='JET_EtaIntercalibration_Modelling',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_EtaIntercalibration_Modelling__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_EtaIntercalibration_Modelling__1down')]),
            SystematicUncertainty(name='JET_EtaIntercalibration_TotalStat',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_EtaIntercalibration_TotalStat__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_EtaIntercalibration_TotalStat__1down')]),
            SystematicUncertainty(name='JET_Flavor_Composition',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_Flavor_Composition__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_Flavor_Composition__1down')]),
            SystematicUncertainty(name='JET_Flavor_Response',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_Flavor_Response__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_Flavor_Response__1down')]),
            SystematicUncertainty(name='JET_Pileup_OffsetMu',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_Pileup_OffsetMu__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_Pileup_OffsetMu__1down')]),
            SystematicUncertainty(name='JET_Pileup_OffsetNPV',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_Pileup_OffsetNPV__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_Pileup_OffsetNPV__1down')]),
            SystematicUncertainty(name='JET_Pileup_PtTerm',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_Pileup_PtTerm__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_Pileup_PtTerm__1down')]),
            SystematicUncertainty(name='JET_Pileup_RhoTopology',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_Pileup_RhoTopology__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_Pileup_RhoTopology__1down')]),
            SystematicUncertainty(name='JET_PunchThrough_MC15',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_PunchThrough_MC15__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_PunchThrough_MC15__1down')]),
            SystematicUncertainty(name='JET_SingleParticle_HighPt',
                                  variations=[ObjectVariation(input_tree='JET_19NP_JET_SingleParticle_HighPt__1up'),
                                              ObjectVariation(input_tree='JET_19NP_JET_SingleParticle_HighPt__1down')]),
            SystematicUncertainty(name='JET_JER_SINGLE_NP__1up',
                                  variations=[ObjectVariation(input_tree='JET_JER_SINGLE_NP__1up')])
            ]
        self.met_object_uncertainties = [
            SystematicUncertainty(name='MET_SoftTrk_ResoPara',
                                  variations=[ObjectVariation(input_tree='MET_SoftTrk_ResoPara')]),
            SystematicUncertainty(name='MET_SoftTrk_ResoPerp',
                                  variations=[ObjectVariation(input_tree='MET_SoftTrk_ResoPerp')]),
            SystematicUncertainty(name='MET_SoftTrk_Scale',
                                  variations=[ObjectVariation(input_tree='MET_SoftTrk_ScaleUp'),
                                              ObjectVariation(input_tree='MET_SoftTrk_ScaleDown')]),
            ]
        self.muon_object_uncertainties = [
            SystematicUncertainty(name="MUONS_ID",
                                  variations=[ObjectVariation(input_tree="MUONS_ID__1up"),
                                              ObjectVariation(input_tree="MUONS_ID__1down")]),
            SystematicUncertainty(name="MUONS_MS",
                                  variations=[ObjectVariation(input_tree="MUONS_MS__1up"),
                                              ObjectVariation(input_tree="MUONS_MS__1down")]),
            SystematicUncertainty(name="MUONS_SCALE",
                                  variations=[ObjectVariation(input_tree="MUONS_SCALE__1up"),
                                              ObjectVariation(input_tree="MUONS_SCALE__1down")]),
            ]

    def object_uncertainties(self):
        return copy.deepcopy([self.electron_object_uncertainties +
                              self.jet_object_uncertainties +
                              self.met_object_uncertainties +
                              self.muon_object_uncertainties])

    def nominal(self):
        return copy.deepcopy(self._nominal)

    def weight_uncertainties(self):
        raise NotImplementedError("SystematicCatalogue: weight_uncertainties not there yet")

    def all_uncertainties(self):
        return [self.nominal()] + self.object_uncertainties() # + self.weight_uncertainties()

    def print_all(self):
        print "SystematicCatalogue:\n"+'\n'.join([s.name for s in self.all_uncertainties()])
