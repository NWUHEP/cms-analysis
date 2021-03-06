from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'BPrime2Xb_X2ll_muonChannel_13TeV_genSim'
config.General.workArea = 'crab_dimuon_13TeV'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'PYTHIA8_heavyB2Xb_mumu_13TeV_cff_py_GEN_SIM.py'

config.Data.outputPrimaryDataset = 'BPrime2Xb_X2ll_muonChannel_13TeV'
config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 400
NJOBS = 250  # This is not a configuration parameter, but an auxiliary variable that we use in the next line.
config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = True
config.Data.outputDatasetTag = 'BPrime2Xb_X2ll_muonChannel_13TeV'

config.Site.storageSite = 'T3_US_FNALLPC'

