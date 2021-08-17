class ProtonCandidate:
    def __init__(self, evId, trackId):
        # Associate to the Michel candidate the original event id it is located in
        # And the original track id that forms the backbone of it (usually entering one)
        self.id_eventId = evId # The event ID where in which we found the Michel candidate
        self.id_trackId = trackId # The track ID at the h5 level of the ENTERING MUON used for Michel candidate
        
        # General
        self.ev_ts = None # The timestamp of the event (PPS)
        self.ev_dtts = None # The timestamp of the event (PPS) in string(datetime format)
        self.ev_tref = None # The trigger time of the event (w.r.t. PPS)

        # Coordinates
        self.pos_xStart = None
        self.pos_yStart = None
        self.pos_zStart = None
        self.pos_xEnd = None
        self.pos_yEnd = None
        self.pos_zEnd = None

        # Orientation
        self.dir_flipped = False

        # dEdx
        self.dedx_distances = None
        self.dedx_charges = None
        

