namespace VLQ_Enums {

    enum RejectCode {
        TRIGGER_REJECTED = 1,
        BADJET_REJECTED = 2,
        LEPTON_REJECTED = 3,
        TRIGGERMATCH_REJECTED = 4,
        JETS_REJECTED = 5,
        BJETS_REJECTED = 6,
        METMTW_REJECTED = 7,
        DPHI_REJECTED = 8,
        MEFF_REJECTED = 9,
        METONELEP_REJECTED = 10,
        METZEROLEP_REJECTED = 11,
        METSIGOBJ_REJECTED = 12
    };

    enum VLQType {
        HtHt = 1,
        HtWb = 2,
        HtZt = 3,
        WbWb = 4,
        WbZt = 5,
        ZtZt = 6
    };

    enum ChannelType {
        UNDEFINED = 0,
        ELECTRON = 1,
        MUON = 2,
	ELEL = 3,
	MUMU = 4,
	ELMU = 5,
        FULLHAD = 6
    };

    enum TriggerType {
      TRIGELEC = 1,
      TRIGMUON = 2,
      TRIGMET = 3
    };
    enum DataPeriod { //Used as bit shifts
      DATA2015 = 0x1<<0,
      DATA2016 = 0x1<<1,
      DATA2017 = 0x1<<2,
      DATA2018 = 0x1<<3
    };

}
