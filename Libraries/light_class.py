# Metainformation about each channel
# [0:X-position, 1:Y-position, 2:Z-position, 3:#TPC, 4:#SN, 5:#SN_ADC, 6:lightModule_type, 7:channel_type, 8:relevant_sumChannel, 9:gain, 10:notes]
viable_sn = [1,2]
viable_ch = [2,3,4,5,6,7,9,10,11,12,13,14,18,19,20,21,22,23,25,26,27,28,29,30,34,35,36,37,38,39,41,42,43,44,45,46,50,51,52,53,54,55,57,58,59,60,61,62]

channelInfo = {
    # SN number 1 (175780172)
    1 :
    {
        # TPC1, ACL Bottom left
        2 : [-308,-600.7,-308,1,1,175780172,'ACL','Channel',8,86.69,None],
        3 : [-308,-553.7,-308,1,1,175780172,'ACL','Channel',8,85.63,None],
        4 : [-308,-488.7,-308,1,1,175780172,'ACL','Channel',8,86.94,None],
        5 : [-308,-441.7,-308,1,1,175780172,'ACL','Channel',8,88.26,None],
        6 : [-308,-376.7,-308,1,1,175780172,'ACL','Channel',8,86.90,None],
        7 : [-308,-329.7,-308,1,1,175780172,'ACL','Channel',8,0,None],
        8 : [-308,-465.2,-308,1,1,175780172,'ACL','Sum',None,-1,None],
        # TPC1, LCM Center-bottom left
        9 : [-308,-290.7,-308,1,1,175780172,'LCM','Channel',15,100.10,None],
        10 : [-308,-243.7,-308,1,1,175780172,'LCM','Channel',15,95.42,None],
        11 : [-308,-178.7,-308,1,1,175780172,'LCM','Channel',15,90.68,None],
        12 : [-308,-131.7,-308,1,1,175780172,'LCM','Channel',15,93.56,None],
        13 : [-308,-66.7,-308,1,1,175780172,'LCM','Channel',15,0,'?'],
        14 : [-308,-19.7,-308,1,1,175780172,'LCM','Channel',15,96.72,None],
        15 : [-308,-155.2,-308,1,1,175780172,'LCM','Sum',None,-1,None],
        # TPC1, ACL Center-top left
        18 : [-308,19.7,-308,1,1,175780172,'ACL','Channel',24,70.28,None],
        19 : [-308,66.7,-308,1,1,175780172,'ACL','Channel',24,65.83,None],
        20 : [-308,131.7,-308,1,1,175780172,'ACL','Channel',24,77.34,None],
        21 : [-308,178.7,-308,1,1,175780172,'ACL','Channel',24,80.13,None],
        22 : [-308,243.7,-308,1,1,175780172,'ACL','Channel',24,65.12,None],
        23 : [-308,290.7,-308,1,1,175780172,'ACL','Channel',24,67.16,None],
        24 : [-308,155.2,-308,1,1,175780172,'ACL','Sum',None,-1,None],
        # TPC1, LCM Top left
        25 : [-308,329.7,-308,1,1,175780172,'LCM','Channel',31,99.31,None],
        26 : [-308,376.7,-308,1,1,175780172,'LCM','Channel',31,97.36,None],
        27 : [-308,441.7,-308,1,1,175780172,'LCM','Channel',31,97.09,None],
        28 : [-308,488.7,-308,1,1,175780172,'LCM','Channel',31,94.76,None],
        29 : [-308,553.7,-308,1,1,175780172,'LCM','Channel',31,95.02,None],
        30 : [-308,600.7,-308,1,1,175780172,'LCM','Channel',31,97.22,None],
        31 : [-308,465.2,-308,1,1,175780172,'LCM','Sum',None,-1,None],
        # TPC2, ACL Bottom right
        34 : [308,-600.7,308,2,1,175780172,'ACL','Channel',40,-1,'Broken'],
        35 : [308,-553.7,308,2,1,175780172,'ACL','Channel',40,-1,'Broken'],
        36 : [308,-488.7,308,2,1,175780172,'ACL','Channel',40,-1,'Broken'],
        37 : [308,-441.7,308,2,1,175780172,'ACL','Channel',40,-1,'Broken'],
        38 : [308,-376.7,308,2,1,175780172,'ACL','Channel',40,-1,'Broken'],
        39 : [308,-329.7,308,2,1,175780172,'ACL','Channel',40,-1,'Broken'],
        40 : [308,-465.2,308,2,1,175780172,'ACL','Sum',None,-1,'Broken'],
        # TPC2, LCM Center-bottom right
        41 : [308,-290.7,308,2,1,175780172,'LCM','Channel',47,96.95,None],
        42 : [308,-243.7,308,2,1,175780172,'LCM','Channel',47,95.75,None],
        43 : [308,-178.7,308,2,1,175780172,'LCM','Channel',47,95.71,None],
        44 : [308,-131.7,308,2,1,175780172,'LCM','Channel',47,93.87,None],
        45 : [308,-66.7,308,2,1,175780172,'LCM','Channel',47,99.96,None],
        46 : [308,-19.7,308,2,1,175780172,'LCM','Channel',47,93.96,None],
        47 : [308,-155.2,308,2,1,175780172,'LCM','Sum',None,-1,None],
        # TPC2, ACL Center-top right
        50 : [308,19.7,308,2,1,175780172,'ACL','Channel',56,73.15,None],
        51 : [308,66.7,308,2,1,175780172,'ACL','Channel',56,75.78,None],
        52 : [308,131.7,308,2,1,175780172,'ACL','Channel',56,80.61,None],
        53 : [308,178.7,308,2,1,175780172,'ACL','Channel',56,77.10,None],
        54 : [308,243.7,308,2,1,175780172,'ACL','Channel',56,78.37,None],
        55 : [308,290.7,308,2,1,175780172,'ACL','Channel',56,80.76,None],
        56 : [308,155.2,308,2,1,175780172,'ACL','Sum',None,-1,None],
        # TPC2, LCM Top right
        57 : [308,329.7,308,2,1,175780172,'LCM','Channel',63,78.54,None],
        58 : [308,376.7,308,2,1,175780172,'LCM','Channel',63,77.09,None],
        59 : [308,441.7,308,2,1,175780172,'LCM','Channel',63,92.80,None],
        60 : [308,488.7,308,2,1,175780172,'LCM','Channel',63,97.58,None],
        61 : [308,553.7,308,2,1,175780172,'LCM','Channel',63,95.91,None],
        62 : [308,600.7,308,2,1,175780172,'LCM','Channel',63,80.35,None],
        63 : [308,465.2,308,2,1,175780172,'LCM','Sum',None,-1,None],
    },
    # SN number 2 (175854781)
    2: {
        # TPC2, ACL Bottom left
        2 : [-308,-600.7,308,2,2,175854781,'ACL','Channel',8,90.89,None],
        3 : [-308,-553.7,308,2,2,175854781,'ACL','Channel',8,88.69,None],
        4 : [-308,-488.7,308,2,2,175854781,'ACL','Channel',8,85.29,None],
        5 : [-308,-441.7,308,2,2,175854781,'ACL','Channel',8,84.73,None],
        6 : [-308,-376.7,308,2,2,175854781,'ACL','Channel',8,85.91,None],
        7 : [-308,-329.7,308,2,2,175854781,'ACL','Channel',8,90.44,None],
        8 : [-308,-465.2,308,2,2,175854781,'ACL','Sum',None,-1,None],
        # TPC2, LCM Center-bottom left
        9 : [-308,-290.7,308,2,2,175854781,'LCM','Channel',15,84.81,None],
        10 : [-308,-243.7,308,2,2,175854781,'LCM','Channel',15,82.3,None],
        11 : [-308,-178.7,308,2,2,175854781,'LCM','Channel',15,84.83,None],
        12 : [-308,-131.7,308,2,2,175854781,'LCM','Channel',15,85.17,None],
        13 : [-308,-66.7,308,2,2,175854781,'LCM','Channel',15,81.63,'?'],
        14 : [-308,-19.7,308,2,2,175854781,'LCM','Channel',15,87,None],
        15 : [-308,-155.2,308,2,2,175854781,'LCM','Sum',None,-1,None],
        # TPC2, ACL Center-top left
        18 : [-308,19.7,308,2,2,175854781,'ACL','Channel',24,89.48,None],
        19 : [-308,66.7,308,2,2,175854781,'ACL','Channel',24,86.17,None],
        20 : [-308,131.7,308,2,2,175854781,'ACL','Channel',24,93.47,None],
        21 : [-308,178.7,308,2,2,175854781,'ACL','Channel',24,91.87,None],
        22 : [-308,243.7,308,2,2,175854781,'ACL','Channel',24,90.85,None],
        23 : [-308,290.7,308,2,2,175854781,'ACL','Channel',24,91.46,None],
        24 : [-308,155.2,308,2,2,175854781,'ACL','Sum',None,-1,None],
        # TPC2, LCM Top left
        25 : [-308,329.7,308,2,2,175854781,'LCM','Channel',31,88.86,None],
        26 : [-308,376.7,308,2,2,175854781,'LCM','Channel',31,88.83,None],
        27 : [-308,441.7,308,2,2,175854781,'LCM','Channel',31,79.66,None],
        28 : [-308,488.7,308,2,2,175854781,'LCM','Channel',31,80.65,None],
        29 : [-308,553.7,308,2,2,175854781,'LCM','Channel',31,81.18,None],
        30 : [-308,600.7,308,2,2,175854781,'LCM','Channel',31,82.17,None],
        31 : [-308,465.2,308,2,2,175854781,'LCM','Sum',None,-1,None],
        # TPC1, ACL Bottom right
        34 : [308,-600.7,-308,1,2,175854781,'ACL','Channel',40,0,'?'],
        35 : [308,-553.7,-308,1,2,175854781,'ACL','Channel',40,93.64,None],
        36 : [308,-488.7,-308,1,2,175854781,'ACL','Channel',40,83.63,None],
        37 : [308,-441.7,-308,1,2,175854781,'ACL','Channel',40,87.79,None],
        38 : [308,-376.7,-308,1,2,175854781,'ACL','Channel',40,89.38,None],
        39 : [308,-329.7,-308,1,2,175854781,'ACL','Channel',40,87.43,None],
        40 : [308,-465.2,-308,1,2,175854781,'ACL','Sum',None,-1,None],
        # TPC1, LCM Center-bottom right
        41 : [308,-290.7,-308,1,2,175854781,'LCM','Channel',47,105.87,None],
        42 : [308,-243.7,-308,1,2,175854781,'LCM','Channel',47,106.14,None],
        43 : [308,-178.7,-308,1,2,175854781,'LCM','Channel',47,108.32,None],
        44 : [308,-131.7,-308,1,2,175854781,'LCM','Channel',47,105.77,None],
        45 : [308,-66.7,-308,1,2,175854781,'LCM','Channel',47,106.70,None],
        46 : [308,-19.7,-308,1,2,175854781,'LCM','Channel',47,105.97,None],
        47 : [308,-155.2,-308,1,2,175854781,'LCM','Sum',None,-1,None],
        # TPC1, ACL Center-top right
        50 : [308,19.7,-308,1,2,175854781,'ACL','Channel',56,94.95,None],
        51 : [308,66.7,-308,1,2,175854781,'ACL','Channel',56,95.59,None],
        52 : [308,131.7,-308,1,2,175854781,'ACL','Channel',56,94.48,None],
        53 : [308,178.7,-308,1,2,175854781,'ACL','Channel',56,98.86,None],
        54 : [308,243.7,-308,1,2,175854781,'ACL','Channel',56,96.53,None],
        55 : [308,290.7,-308,1,2,175854781,'ACL','Channel',56,93.64,None],
        56 : [308,155.2,-308,1,2,175854781,'ACL','Sum',None,-1,None],
        # TPC1, LCM Top right
        57 : [308,329.7,-308,1,2,175854781,'LCM','Channel',63,94.46,None],
        58 : [308,376.7,-308,1,2,175854781,'LCM','Channel',63,93.88,None],
        59 : [308,441.7,-308,1,2,175854781,'LCM','Channel',63,93.84,None],
        60 : [308,488.7,-308,1,2,175854781,'LCM','Channel',63,92.80,None],
        61 : [308,553.7,-308,1,2,175854781,'LCM','Channel',63,92.23,None],
        62 : [308,600.7,-308,1,2,175854781,'LCM','Channel',63,91.71,None],
        63 : [308,465.2,-308,1,2,175854781,'LCM','Sum',None,-1,None],
    }
}



class EventMeta:
    # SUM_CHANNELS = [8, 15, 24, 31, 40, 47, 56, 63]
    SUM_CHANNELS = [0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,18,19,20,21,22,23,24,25,26,27,28,29,30,31,34,35,36,37,38,39,40,41,42,43,44,45,46,47,50,51,52,53,54,55,56,57,58,59,60,61,62,63]
    SN_ADC = {
        1: 175780172,
        2: 175854781
    }
    
    def __init__(self, evid):
        self.evid = evid # Event number
        self.ntp_dt = None # Reference ntp time in datetime
        self.ntp_us = None # Reference ntp time in us (epoch)
        self.dec_offset_us = None # Declared offset with respect to last PPS
        self.jitter_ns = { # Jitter around declared offset
            1: None,
            2: None
        } 
        self.offset_us = { # Actual offset found after window search
            1: None,
            2: None
        } 
        self.successfullyMerged = None # Whether the event is fully merged (0: Error, 1: Ok, 2: Ok, but either multiple entries or other issues)
        self.triggered_ADCs = [] # Which ADCs have a busy signal
        self.nTriggered_ADCs = None # How many ADCs actually produced data
        self.nChannels_ADC = None # Number of channels with a signal found per each ADC
        self.busyFront_ns = { # ns location of busy front
            1: None,
            2: None
        }

        self.entryPerChannel = { # Each channel's waveform is located in TTree['th1s_ptr'][entryNumber]
            1: {},
            2: {}
        }
        for s in self.SUM_CHANNELS:
            self.entryPerChannel[1][s] = None
            self.entryPerChannel[2][s] = None

        self.hist_b = { # Histogram bins
            1: {},
            2: {}
        }
        for s in self.SUM_CHANNELS:
            self.hist_b[1][s] = None
            self.hist_b[2][s] = None

        self.hist_h = { # Histogram contents
            1: {},
            2: {}
        }
        for s in self.SUM_CHANNELS:
            self.hist_h[1][s] = None
            self.hist_h[2][s] = None


        self.t_rel = { # Time relative to busy
            1: {},
            2: {}
        }
        for s in self.SUM_CHANNELS:
            self.t_rel[1][s] = None
            self.t_rel[2][s] = None
        