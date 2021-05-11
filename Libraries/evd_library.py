import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d.art3d as art3d
import Libraries.charge_library as cl
import Libraries.light_library as ll

def SmallEventDisplay(evid,cpath,cdata,geometryHelper,showHits=True,showTracks=True,xTrackOffset=10,yTrackOffset=0,rot=45):
    ## GENERAL SETTINGS
    ticksize = 11
    labelsize = 12

    ## PREPARE AXES
    fig = plt.figure(constrained_layout=False,figsize=(13.0,6.5),facecolor='white')
    gs_2d = fig.add_gridspec(nrows=1, ncols=2, width_ratios=[1,1],
                                        left=0.12, right=0.5, top=0.86, bottom=0.05,
                                        hspace=0, wspace=0.1)
    gs_3d = fig.add_gridspec(nrows=1, ncols=1, width_ratios=[1],
                                        left=0.35, right=1, top=0.86, bottom=0.05,
                                        hspace=0, wspace=0.03)
    ax_xy = fig.add_subplot(gs_2d[0])
    for tk in ax_xy.get_yticklabels(): tk.set_fontsize(ticksize)
    for tk in ax_xy.get_xticklabels(): tk.set_fontsize(ticksize)

    ax_zy = fig.add_subplot(gs_2d[1], sharey=ax_xy)
    ax_zy.axvline(0,ls='-',lw=1,color='C7',alpha=0.5)
    for tk in ax_zy.get_xticklabels(): tk.set_fontsize(ticksize)

    ## MORE COMPLEX FOR 3D. DRAW ANODE/CATHODE
    anode1 = plt.Rectangle((geometryHelper['tpc_borders'][0][0][0], geometryHelper['tpc_borders'][0][1][0]),
                                geometryHelper['tpc_borders'][0][0][1]-geometryHelper['tpc_borders'][0][0][0],
                                geometryHelper['tpc_borders'][0][1][1]-geometryHelper['tpc_borders'][0][1][0],
                       linewidth=1, fc='none',
                       edgecolor='gray')
    anode2 = plt.Rectangle((geometryHelper['tpc_borders'][0][0][0], geometryHelper['tpc_borders'][0][1][0]),
                                geometryHelper['tpc_borders'][0][0][1]-geometryHelper['tpc_borders'][0][0][0],
                                geometryHelper['tpc_borders'][0][1][1]-geometryHelper['tpc_borders'][0][1][0],
                            linewidth=1, fc='none',
                            edgecolor='gray')
    cathode = plt.Rectangle((geometryHelper['tpc_borders'][0][0][0], geometryHelper['tpc_borders'][0][1][0]),
                            geometryHelper['tpc_borders'][0][0][1]-geometryHelper['tpc_borders'][0][0][0],
                            geometryHelper['tpc_borders'][0][1][1]-geometryHelper['tpc_borders'][0][1][0],
                            linewidth=1, fc='gray', alpha=0.25,
                            edgecolor='gray')

    ax_xyz = fig.add_subplot(gs_3d[0], projection='3d')
    ax_xyz.set_facecolor('none')
    ax_xyz.grid(False)
    ax_xyz.set_box_aspect((2,2,4))
    for tk in ax_zy.get_yticklabels(): tk.set_visible(False)
    for tk in ax_xyz.get_xticklabels(): tk.set_fontsize(ticksize)
    for tk in ax_xyz.get_yticklabels(): tk.set_fontsize(ticksize)
    for tk in ax_xyz.get_zticklabels(): tk.set_fontsize(ticksize)
    ax_xyz.add_patch(anode1)
    art3d.pathpatch_2d_to_3d(anode1, z=geometryHelper['tpc_borders'][0][2][0], zdir="y")
    ax_xyz.add_patch(anode2)
    art3d.pathpatch_2d_to_3d(anode2, z=geometryHelper['tpc_borders'][1][2][0], zdir="y")
    ax_xyz.add_patch(cathode)
    art3d.pathpatch_2d_to_3d(cathode, z=0, zdir="y")

    ## GET EVENT AND METAINFORMATION
    tref = cl.GetEventStartTime(evid,cdata)
    # print(f"- Event: {evid}")
    # print(f"|_Number of hits: {cdata['events'][evid]['nhit']}")
    # print(f"|_Number of tracks: {cdata['events'][evid]['ntracks']}")
    # print("----------------------------------------------")
    # print(f"Time: {cdata['events'][evid]['unix_ts']} s + {tref} us")

    ## PREPARE ALL HITS AND TRACKS AND CONVERT TO CORRECT TIMING
    myHits = cdata['hits'][cdata['events'][evid]['hit_ref']]
    myTracks = cdata['tracks'][cdata['events'][evid]['track_ref']]
    z_hits = [cl.ConvertTimeToZ(geometryHelper, io_group, io_channel, time, tref) for io_group, io_channel, time in zip(myHits['iogroup'], myHits['iochannel'], myHits['ts'])]

    ## PREPARE FOR PLOTS
    # cmap = plt.cm.get_cmap('jet')
    cmap = plt.cm.get_cmap('viridis')
    norm = matplotlib.colors.Normalize(vmin=0,vmax=200, clip=True)

    ## XY PLOT
    if showHits:
        ax_xy.scatter(myHits['px'],myHits['py'],c=cmap(norm(myHits['q'])),s=1.5)
    if showTracks:
        for i,t in enumerate(myTracks):
            ax_xy.plot( [t['start'][0]+xTrackOffset,t['end'][0]+xTrackOffset],
                        [t['start'][1]+yTrackOffset,t['end'][1]+yTrackOffset],
                        c=f'C{i+1}',
                        lw=2
            )
    ax_xy.set_xlim(np.min(geometryHelper['tpc_borders'][:,0,:]), np.max(geometryHelper['tpc_borders'][:,0,:]))
    ax_xy.set_ylim(np.min(geometryHelper['tpc_borders'][:,1,:]), np.max(geometryHelper['tpc_borders'][:,1,:]))
    ax_xy.set_xlabel("x [mm]", fontsize=labelsize)
    ax_xy.set_ylabel("y [mm]", fontsize=labelsize)

    ## ZY PLOT
    if showHits:
        ax_zy.scatter(z_hits,myHits['py'],c=cmap(norm(myHits['q'])),s=1.5)
    if showTracks:
        for i,t in enumerate(myTracks):
            ax_zy.plot( [t['start'][2]+xTrackOffset,t['end'][2]+xTrackOffset],
                        [t['start'][1]+yTrackOffset,t['end'][1]+yTrackOffset],
                        c=f'C{i+1}',
                        lw=2
            )
    ax_zy.set_xlim(np.min(geometryHelper['tpc_borders'][:,2,:]), np.max(geometryHelper['tpc_borders'][:,2,:]))
    ax_zy.set_ylim(np.min(geometryHelper['tpc_borders'][:,1,:]), np.max(geometryHelper['tpc_borders'][:,1,:]))
    ax_zy.set_xlabel("z [mm]", fontsize=labelsize)

    ## 3D PLOT
    if showHits:
        ax_xyz.scatter(myHits['px'],z_hits,myHits['py'],c=cmap(norm(myHits['q'])),s=1.5)
    if showTracks:
        for i,t in enumerate(myTracks):
            ax_xyz.plot([t['start'][0],t['end'][0]],
                        [t['start'][2]+xTrackOffset,t['end'][2]+xTrackOffset],
                        [t['start'][1]+yTrackOffset,t['end'][1]+yTrackOffset],
                        c=f'C{i+1}',
                        lw=2
            )
    ax_xyz.set_xlim(np.min(geometryHelper['tpc_borders'][:,0,:]), np.max(geometryHelper['tpc_borders'][:,0,:]))
    ax_xyz.set_ylim(np.min(geometryHelper['tpc_borders'][:,2,:]), np.max(geometryHelper['tpc_borders'][:,2,:]))
    ax_xyz.set_zlim(np.min(geometryHelper['tpc_borders'][:,1,:]), np.max(geometryHelper['tpc_borders'][:,1,:]))
    ax_xyz.set_xlabel("x [mm]", fontsize=labelsize)
    ax_xyz.set_ylabel("z [mm]", fontsize=labelsize)
    ax_xyz.set_zlabel("y [mm]", fontsize=labelsize)
    ax_xyz.view_init(azim=rot)
    
    fig.suptitle(f"""
        {cpath.split('/')[-1]} | Event: {evid}, Time: {cdata['events'][evid]['unix_ts']} s, PPS Delay: {tref} us
        #Hits: {len(myHits)}, #Tracks: {len(myTracks)}
        """,
        va='top',ha='center',
        x=0.46, y=0.98,
        fontsize=12
        )

    # plt.show()
    return fig, ax_xy, ax_zy, ax_xyz