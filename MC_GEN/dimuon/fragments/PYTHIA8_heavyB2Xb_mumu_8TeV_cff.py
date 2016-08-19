import FWCore.ParameterSet.Config as cms

from Configuration.Generator.Pythia8CommonSettings_cfi import *   
from Configuration.Generator.Pythia8CUEP8M1Settings_cfi import *  

generator = cms.EDFilter("Pythia8GeneratorFilter", 
    maxEventsToPrint = cms.untracked.int32(1),
    #pythiaPylistVerbosity = cms.untracked.int32(1),
    filterEfficiency = cms.untracked.double(1.0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(8000), 
    PythiaParameters = cms.PSet(
      processParameters = cms.vstring(
	'FourthBottom:qq2bPrimeq(t:W) = on', 			# t-channel production of B'(pdgid=7) via W exchange
	'7:m0 = 173.3',						# B' mass
	'7:mWidth = 1.35',					# B' mass width
	'7:onMode = off',					# Switch off B' decay
	'7:addChannel = 1 1.0 100 5 4900023',			# Add B'->X+b
	'7:onIfAny = 4900023',					# Switch on B'->X+b
	'4900023:mMin = 10',					# Set the minimum mass of X
	'4900023:m0 = 28.7',					# Set the mass of X
	'4900023:mWidth = 1.9',				# Set the mass width of X
	'4900023:onMode = off',				# Switch off X decay modes
	'4900023:onIfMatch = 13 -13'	  			# Force to X decay into muons
        ),
      parameterSets = cms.vstring('processParameters')
      )
    )

