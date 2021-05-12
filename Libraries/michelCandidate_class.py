class MichelCandidate:
    def __init__(self, evId, trackId, trackId_inEv):
        # Associate to the Michel candidate the original event id it is located in
        # And the original track id that forms the backbone of it (usually entering one)
        self.id_eventId = evId # The event ID where in which we found the Michel candidate
        self.id_trackId = trackId # The track ID at the h5 level of the ENTERING MUON used for Michel candidate
        self.id_trackId_inEv = trackId_inEv # The track ID of the ENTERING MUON in the event (e.g. track 3 of 5) used for Michel candidate
        
        # Other attributes that will be assigned on the fly as the candidate is built
        # Event information
        self.ev_nTracks = None # The number of tracks in the event
        self.ev_nTracksCandidate = None # The number of tracks used to build our candidate (mu + stitches + e)
        self.ev_nTracksNotCandidate = None # The number of tracks NOT used to build our candidate
        self.ev_nHits = None # The number of hits in the event
        self.ev_nHitsCandidate = None # The number of hits used to build our candidate (mu + stitches + e)
        self.ev_nHitsNotCandidate = None # The number of hits NOT used to build our candidate
        # Containment
        self.cont_originallyFlipped = None # Whether the entering muon was originally flipped
        self.cont_entered = None # Whether the entering muon entered (should always be true!)
        self.cont_exited = None # Whether the entering muon also exited, after stitching
        self.cont_xEntered = None # Whether the entering muon entered from the light detector side
        self.cont_zEntered = None # Whether the entering muon entered from the anode side
        self.cont_yEntered = None # Whether the entering muon entered from either top or bottom
        self.cont_topEntered = None # Whether the entering muon entered from the top
        self.cont_botEntered = None # Whether the entering muon entered from the bottom
        self.cont_xExited = None # Whether the entering muon exited from the light detector side
        self.cont_zExited = None # Whether the entering muon exited from the anode side
        self.cont_yExited = None # Whether the entering muon exited from either top or bottom
        self.cont_topExited = None # Whether the entering muon exited from the top
        self.cont_botExited = None # Whether the entering muon exited from the bottom
        # Start/end
        self.mu_pos_start = None # Start position of entering muon
        self.mu_pos_end = None # End position of muon candidate, after stitching
        self.mu_pos_length = None # Total length of muon candidate, after stitching
        # Orientation
        self.mu_orient_direction = None
        self.mu_orient_theta = None
        self.mu_orient_phi = None
        # Stitching
        self.stitch_stitched = None
        self.stitch_tooFar = None
        self.stitch_nStitches = None
        self.stitch_iStitches = []
        self.stitch_iStitches_inEv = []
        self.stitch_locStitch = []
        self.stitch_locAlmostStitch= []
        # Electron
        self.e_nCandidates = None
        self.e_id_trackId = []
        self.e_id_trackId_inEv = []
        self.e_pos_start = None
        self.e_pos_end = None
        self.e_pos_length = None
        self.e_orient_direction = None
        self.e_orient_theta = None
        self.e_orient_phi = None