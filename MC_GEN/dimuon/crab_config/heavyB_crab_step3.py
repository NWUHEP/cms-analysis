from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'BPrime2Xb_X2ll_muonChannel_13TeV_miniaod'
config.General.workArea = 'crab_dimuon_13TeV'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'heavyB_step3_PAT.py'


#config.Data.inputDataset = '/heavyB2Xb_mumu_13TeV/jbueghly-heavyB2Xb_mumu_13TeV_step2-fe4811dc26de49439a731cd9b050a083/USER'
#config.Data.inputDataset = '/heavyB2Xb_mumu_13TeV/jbueghly-heavyB2Xb_mumu_13TeV_step2-fe4811dc26de49439a731cd9b050a083/USER'
config.Data.inputDataset = '/BPrime2Xb_X2ll_muonChannel_13TeV/jbueghly-BPrime2Xb_X2ll_muonChannel_13TeV_step2-fe4811dc26de49439a731cd9b050a083/USER'
config.Data.inputDBS = 'phys03'
#config.Data.outputPrimaryDataset = 'axgamma_step2'
config.Data.ignoreLocality = True
config.Site.whitelist = ["T2_US*"]
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 18
#NJOBS = 10  # This is not a configuration parameter, but an auxiliary variable that we use in the next line.
#config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.totalUnits = 229
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = True
config.Data.outputDatasetTag = 'BPrime2Xb_X2ll_muonChannel_13TeV_miniaod'

config.Site.storageSite = 'T3_US_FNALLPC'

