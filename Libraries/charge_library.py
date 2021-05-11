import numpy as np
import yaml

# Locate current file location
from pathlib import Path
pathFromHere = Path(__file__).parent.absolute()



def GetGeometryHelper(info,geometryFile='multi_tile_layout-2.1.16.yaml'):
    '''
    Get a geometry helper to figure out detector
    '''
    # Locate the geometry file
    geometry_file = str(pathFromHere)+'/Geometries/'+geometryFile
    # Open geometry file
    with open(geometry_file, 'r') as gf:
        tile_layout = yaml.load(gf, Loader=yaml.FullLoader)

    # Get pixel_pitch in cm
    mm2cm = 0.1
    pixel_pitch = tile_layout['pixel_pitch'] * mm2cm
    # Map channel to position
    chip_channel_to_position = tile_layout['chip_channel_to_position']
    tile_chip_to_io = tile_layout['tile_chip_to_io']
    io_group_io_channel_to_tile = {}
    for tile in tile_chip_to_io:
        for chip in tile_chip_to_io[tile]:
            io_group_io_channel = tile_chip_to_io[tile][chip]
            io_group = io_group_io_channel//1000
            io_channel = io_group_io_channel%1000
            io_group_io_channel_to_tile[(io_group,io_channel)]=tile

    # Get positions and orientation of the tiles
    tile_positions = np.array(list(tile_layout['tile_positions'].values()))
    tile_orientations = np.array(list(tile_layout['tile_orientations'].values()))

    cm2mm = 10
    xs = np.array(list(chip_channel_to_position.values()))[:,0] * pixel_pitch * cm2mm   
    ys = np.array(list(chip_channel_to_position.values()))[:,1] * pixel_pitch * cm2mm
    tile_borders = np.zeros((2,2))
    tpc_borders = np.zeros((0, 3, 2))
    tpc_centers = np.array(list(tile_layout['tpc_centers'].values()))
    tile_borders[0] = [-(max(xs)+pixel_pitch)/2, (max(xs)+pixel_pitch)/2]
    tile_borders[1] = [-(max(ys)+pixel_pitch)/2, (max(ys)+pixel_pitch)/2]
    tpcs = np.unique(tile_positions[:,0])
    tpc_borders = np.zeros((len(tpcs), 3, 2))
    drift_length = abs(tile_positions[0][0])
    drift_time = drift_length/info['vdrift']/info['clock_period']

    for itpc,tpc_id in enumerate(tpcs):
            this_tpc_tile = tile_positions[tile_positions[:,0] == tpc_id]
            this_orientation = tile_orientations[tile_positions[:,0] == tpc_id]

            x_border = min(this_tpc_tile[:,2])+tile_borders[0][0]+tpc_centers[itpc][0], \
                       max(this_tpc_tile[:,2])+tile_borders[0][1]+tpc_centers[itpc][0]
            y_border = min(this_tpc_tile[:,1])+tile_borders[1][0]+tpc_centers[itpc][1], \
                       max(this_tpc_tile[:,1])+tile_borders[1][1]+tpc_centers[itpc][1]
            z_border = min(this_tpc_tile[:,0])+tpc_centers[itpc][2], \
                       max(this_tpc_tile[:,0])+drift_length*this_orientation[:,0][0]+tpc_centers[itpc][2]

            tpc_borders[itpc] = (x_border, y_border, z_border)


    # Build geometry helper with what we got so far
    geometryHelper = {
        'io_group_io_channel_to_tile': io_group_io_channel_to_tile,
        'tile_positions': tile_positions,
        'tile_orientations': tile_orientations,
        'vdrift': info['vdrift'],
        'clock_period': info['clock_period'],
        'tpc_borders': tpc_borders,
    }

    return geometryHelper
    


def GetEventStartTime(evid,h5):
    '''
    Make an estimate of the event start time
    '''
    thisEvent = h5['events'][evid]
    # 1st simple solution, if event already has an external light
    # system trigger use just that (the earliest of course).
    if thisEvent['n_ext_trigs']!=0:
        return h5['ext_trigs'][thisEvent['ext_trig_ref']]['ts'][0]

    # 2nd alternative solution, look at charge and find the earliest
    # "bump" in charge. This works only if track (or other event hits)
    # crossed the anode.
    ticks_per_qsum = 10 # Clock ticks per time bin
    t0_charge_threshold = 200.0 # Rough qsum threshold
    hitRef = thisEvent['hit_ref'] # The hits ref. from this event
    thisHits = h5['hits'][hitRef] # The hits from this event
    # Determine the left and right edges of the histogram
    min_ts = np.amin(thisHits['ts'])
    max_ts = np.amax(thisHits['ts'])
    # This is going to work only if our integrating window is big enough
    if (max_ts - min_ts) > ticks_per_qsum:
        time_bins = np.arange(min_ts-ticks_per_qsum,
                                max_ts+ticks_per_qsum)
        # integrate q in sliding window to produce qsum profile
        #  histogram raw charge
        q_vs_t = np.histogram(thisHits['ts'],
                                bins=time_bins,
                                weights=thisHits['q'])[0]
        #  calculate rolling qsum
        qsum_vs_t = np.convolve(q_vs_t,
                                np.ones(ticks_per_qsum,dtype=int),
                                'valid')
        t0_bin_index = np.argmax(qsum_vs_t>t0_charge_threshold)
        t0_bin_index += ticks_per_qsum
        start_time = time_bins[t0_bin_index]
        # Check if qsum exceed threshold
        if start_time < max_ts:
            return start_time

    # 3rd solution. We give up, we are just going to use
    # the first hit in event
    return thisEvent['ts_start']



def ConvertTimeToZ(geometryHelper, io_group, io_channel, time, tref):
    '''
    Convert hit time to z coordinate
    '''
    # Get information regarding the tile associated with the hit
    # Based on its io_group and io_channel
    try:
        tile_id = geometryHelper['io_group_io_channel_to_tile'][io_group,io_channel]
    except:
        tile_id = 0
    z_anode = geometryHelper['tile_positions'][tile_id-1][0]
    drift_direction = geometryHelper['tile_orientations'][tile_id-1][0]
    # Other information from info
    vdrift = geometryHelper['vdrift']
    clock_period = geometryHelper['clock_period']
    # Derive z coordinate
    z_coordinate =  z_anode + (time-tref)*vdrift*clock_period*drift_direction
    return z_coordinate