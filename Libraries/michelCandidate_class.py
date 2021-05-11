class MichelCandidate:
    def __init__(self, evId, trackId, trackId_inEv):
        # Associate to the Michel candidate the original event id it is located in
        # And the original track id that forms the backbone of it (usually entering one)
        self.id_eventId = evId
        self.id_trackId = trackId
        self.id_trackId_inEv = trackId_inEv
        
        # Other attributes that will be assigned on the fly as the candidate is built
        # Containment
        self.cont_originallyFlipped = None
        self.cont_entered = None
        self.cont_exited = None
        self.cont_xEntered = None
        self.cont_yEntered = None
        self.cont_topEntered = None
        self.cont_botEntered = None
        self.cont_xExited = None
        self.cont_yExited = None
        self.cont_topExited = None
        self.cont_botExited = None
        # Start/end
        self.pos_start = None
        self.pos_end = None
        self.pos_length = None
        # Orientation
        self.orient_direction = None
        self.orient_theta = None
        self.orient_phi = None
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