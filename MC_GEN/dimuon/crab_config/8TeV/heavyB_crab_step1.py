from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'BPrime2Xb_X2ll_muonChannel_8TeV_step1'
config.General.workArea = 'crab_dimuon_8TeV'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'heavyB_REDIGI_DIGI_L1_DIGI2RAW_HLT_PU.py'


#config.Data.inputDataset = '/heavyB2Xb_mumu_13TeV/jbueghly-heavyB2Xb_mumu_13TeV_genSim-d00224c36ab09867d241c6a02aba266a/USER'
#config.Data.inputDataset = '/heavyB2Xb_mumu_13TeV/jbueghly-heavyB2Xb_mumu_13TeV_genSim-10237840607af40cdec3776b728d9bd5/USER'
#config.Data.inputDataset = '/BPrime2Xb_X2ll_muonChannel_13TeV/jbueghly-BPrime2Xb_X2ll_muonChannel_13TeV-10237840607af40cdec3776b728d9bd5/USER'
config.Data.inputDataset = '/BPrime2Xb_X2ll_muonChannel_8TeV/jbueghly-BPrime2Xb_X2ll_muonChannel_8TeV-b91297fd1839346bb961714df2c1f415/USER'
config.Data.inputDBS = 'phys03'
#config.Data.outputPrimaryDataset = 'axgamma_step1'
config.Data.ignoreLocality = True
config.Site.whitelist = ["T2_US*"]

config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
NJOBS = 479  # This is not a configuration parameter, but an auxiliary variable that we use in the next line.
config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = True
config.Data.outputDatasetTag = 'BPrime2Xb_X2ll_muonChannel_8TeV_step1'

config.Site.storageSite = 'T3_US_FNALLPC'

