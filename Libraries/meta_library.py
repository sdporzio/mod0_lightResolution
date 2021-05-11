import os, uproot
import pandas as pd
import numpy as np
from pathlib import Path
# Locate the file archive and load it
pathFromHere = Path(__file__).parent.absolute()
fileArchivePath = str(pathFromHere)+'/ChargeLightFilepaths/fileArchive.csv'
fArc = pd.read_csv(fileArchivePath)

def FindPartnerLightFile(cfile,ldirectory):
    # Extract the name substring from the charge file path
    split = np.array(cfile.split('/')[-1].split('_'))
    cfileCore = '_'.join(split[ np.where(split=='2021')[0][0] : np.where(split=='CEST')[0][0] ])
    # Query the database for the corresponding light file
    lfileCore = fArc[fArc['chargeFile'].str.contains(cfileCore)]['lightFile'].values
    # Check if we at least one match
    if len(lfileCore)==0:
        raise Exception(f'ERROR. No matching light file for {cfileCore}. Exiting.')
    # If not let's notify that's the case
    print(f'Charge file {cfileCore} has been matched to light file {lfileCore}.')
    # Form the light file name
    lfile = 'rwf_'+lfileCore[0]+'.root'
    lpath = ldirectory+'/'+lfile
    # Now make sure the file actually exists
    if not os.path.isfile(lpath):
        raise Exception(f'ERROR. Matching file {lpath} does not exist. Exiting.')
    # Load the necessary files
    df = (uproot.open(lpath)['rwf']).arrays(['event','sn','ch','utime_ms','tai_ns'],library='pd')
    return df, lpath