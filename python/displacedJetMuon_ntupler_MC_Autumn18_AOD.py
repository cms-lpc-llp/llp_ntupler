import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
#------ Setup ------#

#initialize the process
process = cms.Process("displacedJetMuonNtupler")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.EventContent.EventContent_cff")
process.load("cms_lpc_llp.llp_ntupler.metFilters_cff_2017")


#load input files
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#'file:///storage/user/christiw/login-1/christiw/LLP/CMSSW_9_4_7/src/cms_lpc_llp/llp_ntupler/F2E310F0-1513-A741-B89D-BC588E298466.root',
#'file:///mnt/hadoop/store/mc/RunIIAutumn18DRPremix/WminusH_HToSSTobbbb_WToLNu_MH-125_TuneCP5_13TeV-powheg-pythia8/AODSIM/rp_102X_upgrade2018_realistic_v15-v2/70000/A01CF620-9B12-3044-BF55-C4E34E4D3349.root',
#'file:///storage/user/christiw/login-1/christiw/LLP/CMSSW_10_2_16/src/cms_lpc_llp/llp_ntupler/EGM-RunIIAutumn18DR-00031.root',
#        '/store/mc/RunIIAutumn18DRPremix/ttHJetTobb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8/AODSIM/102X_upgrade2018_realistic_v15-v1/260000/EC536CE3-5405-9F4D-B571-2EF83D5C17D9.root'
        #'/store/group/phys_exotica/privateProduction/DR/step2_RECOSIM/RunIIFall18/ggH_HToSSTobbbb_ms55_pl1000/batch1/v1/ggH_HToSSTobbbb_ms55_pl1000/crab_PrivateProduction_Fall18_DR_step2_ggH_HToSSTobbbb_ms55_pl1000_batch1_v1/191224_123235/0000/RECOSIM_150.root'
        #'/store/mc/RunIIAutumn18DRPremix/ggH_HToSSTobbbb_MH-125_TuneCP5_13TeV-powheg-pythia8/GEN-SIM-RECO/rp_102X_upgrade2018_realistic_v15-v1/280001/A7084B2E-EF2D-9B4C-911C-AD7072A597D7.root'
        '/store/mc/RunIIAutumn18DRPremix/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/AODSIM/102X_upgrade2018_realistic_v15-v1/00000/903F62EA-EB24-CA42-81BE-4ED9F52C8DD5.root'
        )
)

process.options = cms.untracked.PSet(

)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1

#TFileService for output
process.TFileService = cms.Service("TFileService",
	fileName = cms.string('displacedJetMuon_ntupler.root'),
    closeFileFast = cms.untracked.bool(True)
)

#load run conditions
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.load('Configuration.Geometry.GeometryIdeal_cff')
#process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

#------ Declare the correct global tag ------#

process.GlobalTag.globaltag = '102X_upgrade2018_realistic_v19'

#------ If we add any inputs beyond standard event content, import them here ------#
process.load('RecoMET.METFilters.BadPFMuonFilter_cfi')


process.output = cms.OutputModule("PoolOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(4),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    dropMetaData = cms.untracked.string('ALL'),
    eventAutoFlushCompressedSize = cms.untracked.int32(-900),
    fastCloning = cms.untracked.bool(False),
    fileName = cms.untracked.string('miniAOD-prod_PAT.root'),
    outputCommands = cms.untracked.vstring('keep *'),
)



  
#------ Analyzer ------#

# For AOD Track variables
process.load("RecoTracker.TkNavigation.NavigationSchoolESProducer_cfi")
process.MaterialPropagator = cms.ESProducer('PropagatorWithMaterialESProducer',
    ComponentName = cms.string('PropagatorWithMaterial'),
    Mass = cms.double(0.105),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('alongMomentum'),
    SimpleMagneticField = cms.string(''),
    ptMin = cms.double(-1.0),
    useRungeKutta = cms.bool(False)
)

process.TransientTrackBuilderESProducer = cms.ESProducer('TransientTrackBuilderESProducer',
    ComponentName = cms.string('TransientTrackBuilder')
)


#list input collections
process.ntuples = cms.EDAnalyzer('displacedJetMuon_ntupler',
    isData = cms.bool(False),
    useGen = cms.bool(True),
    isRECO = cms.bool(False),                                
    isFastsim = cms.bool(False),
    readMuonDigis = cms.bool(False),
    enableTriggerInfo = cms.bool(True),
    enableEcalRechits = cms.bool(False),
    enableCaloJet = cms.bool(True),
    enableGenLLPInfo = cms.bool(True),
    readGenVertexTime = cms.bool(False),#need to be false for displaced samples
    genParticles_t0 = cms.InputTag("genParticles", "t0", ""),
    triggerPathNamesFile = cms.string("cms_lpc_llp/llp_ntupler/data/trigger_names_llp_v3.dat"),
    eleHLTFilterNamesFile = cms.string("SUSYBSMAnalysis/RazorTuplizer/data/RazorElectronHLTFilterNames.dat"),
    muonHLTFilterNamesFile = cms.string("SUSYBSMAnalysis/RazorTuplizer/data/RazorMuonHLTFilterNames.dat"),
    photonHLTFilterNamesFile = cms.string("SUSYBSMAnalysis/RazorTuplizer/data/RazorPhotonHLTFilterNames.dat"),

    #vertices = cms.InputTag("offlinePrimaryVerticesWithBS"),  # for non-timing case
    vertices = cms.InputTag("offlinePrimaryVertices", "", "RECO"),
    muons = cms.InputTag("muons"),
    electrons = cms.InputTag("gedGsfElectrons"),
    taus = cms.InputTag("selectedPatTaus"),
    photons = cms.InputTag("gedPhotons"),
    jetsCalo = cms.InputTag("ak4CaloJets","","RECO"),
    jetsPF = cms.InputTag("ak4PFJets"),
    #jets = cms.InputTag("ak4PFJetsCHS"),
    jets = cms.InputTag("selectedPatJets"),
    jetsPuppi = cms.InputTag("ak4PFJets"),
    #jetsAK8 = cms.InputTag("ak8PFJetsCHS"),
    jetsAK8 = cms.InputTag("selectedPatJetsAK8PFCHS"),

    mets = cms.InputTag("pfMet"),
    #metsNoHF = cms.InputTag("pfMet30"),
    metsPuppi = cms.InputTag("pfMet"),
    pfCands = cms.InputTag("particleFlow","","RECO"),

    #packedPfCands = cms.InputTag("packedPFCandidates"),

    genParticles = cms.InputTag("genParticles"),
    MuonCSCSimHits = cms.InputTag("g4SimHits", "MuonCSCHits","SIM"),
    MuonCSCComparatorDigi = cms.InputTag("simMuonCSCDigis", "MuonCSCComparatorDigi", "HLT"),
    MuonCSCStripDigi = cms.InputTag("simMuonCSCDigis", "MuonCSCStripDigi", "HLT"),
    MuonCSCWireDigi = cms.InputTag("simMuonCSCDigis", "MuonCSCWireDigi", "HLT"),
    MuonCSCWireDigiSimLinks = cms.InputTag( "simMuonCSCDigis", "MuonCSCWireDigiSimLinks", "HLT"),
    MuonCSCStripDigiSimLinks = cms.InputTag("simMuonCSCDigis","MuonCSCStripDigiSimLinks", "HLT"),

    #packedGenParticles = cms.InputTag("packedGenParticles"),
    #prunedGenParticles = cms.InputTag("prunedGenParticles"),
    genMetsCalo = cms.InputTag("genMetCalo"),
    genMetsTrue = cms.InputTag("genMetTrue"),
    genJets = cms.InputTag("ak4GenJets"),

    triggerBits = cms.InputTag("TriggerResults","","HLT"),
    #triggerBits = cms.InputTag("TriggerResults","","RECO"),
    hepMC = cms.InputTag("generatorSmeared", "", "SIM"),

    #triggerPrescales = cms.InputTag("patTrigger"),
    #triggerObjects = cms.InputTag("selectedPatTrigger"),

    metFilterBits = cms.InputTag("TriggerResults", "", "RECO"),

    #hbheNoiseFilter = cms.InputTag("HBHENoiseFilterResultProducer","HBHENoiseFilterResult"),
    #hbheTightNoiseFilter = cms.InputTag("HBHENoiseFilterResultProducer","HBHENoiseFilterResultRun2Tight"),
    #hbheIsoNoiseFilter = cms.InputTag("HBHENoiseFilterResultProducer","HBHEIsoNoiseFilterResult"),

    #BadChargedCandidateFilter = cms.InputTag("BadChargedCandidateFilter",""),
    #BadMuonFilter = cms.InputTag("BadPFMuonFilter",""),

    #lheInfo = cms.InputTag("externalLHEProducer", "", ""),
    genInfo = cms.InputTag("generator", "", "SIM"),

    tracks = cms.InputTag("generalTracks", "", "RECO"),
    #trackTime = cms.InputTag("trackTimeValueMapProducer","generalTracksConfigurableFlatResolutionModel"),
    #trackTimeReso = cms.InputTag("trackTimeValueMapProducer","generalTracksConfigurableFlatResolutionModelResolution"),

    #puInfo = cms.InputTag("addPileupInfo", "", "HLT"), #uncomment if no pre-mixing
    puInfo = cms.InputTag("mixData", "", "HLT"), #uncomment for samples with pre-mixed pileup
    #hcalNoiseInfo = cms.InputTag("hcalnoise", "", "RECO"),

    #secondaryVertices = cms.InputTag("inclusiveSecondaryVertices", "", "RECO"),
    secondaryVertices = cms.InputTag("inclusiveCandidateSecondaryVertices","", "RECO"),

    rhoAll = cms.InputTag("fixedGridRhoAll", "", "RECO"),

    rhoFastjetAll = cms.InputTag("fixedGridRhoFastjetAll", "", "RECO"),
    rhoFastjetAllCalo = cms.InputTag("fixedGridRhoFastjetAllCalo", "", "RECO"),
    rhoFastjetCentralCalo = cms.InputTag("fixedGridRhoFastjetCentralCalo", "", "RECO"),
    rhoFastjetCentralChargedPileUp = cms.InputTag("fixedGridRhoFastjetCentralChargedPileUp", "", "RECO"),
    rhoFastjetCentralNeutral = cms.InputTag("fixedGridRhoFastjetCentralNeutral", "", "RECO"),

    beamSpot = cms.InputTag("offlineBeamSpot", "", "RECO"),
    pfClusters = cms.InputTag("particleFlowClusterECAL","","RECO"),
    ebRecHits = cms.InputTag("reducedEcalRecHitsEB", "","RECO"),
    #ebRecHits = cms.InputTag("EcalRecHit", "reducedEcalRecHitsEB", "RECO"),
    eeRecHits  = cms.InputTag("reducedEcalRecHitsEE", "","RECO"),
    esRecHits = cms.InputTag("reducedEcalRecHitsES", "","RECO"),
    #ebeeClusters = cms.InputTag("reducedEgamma", "reducedEBEEClusters", "RECO"),
    ebeeClusters = cms.InputTag("particleFlowEGamma", "EBEEClusters", "RECO"),
    esClusters = cms.InputTag("particleFlowEGamma", "ESClusters", "RECO"),
    #conversions = cms.InputTag("reducedEgamma", "reducedConversions", "RECO"),
    conversions = cms.InputTag("allConversions", "", "RECO"),

    #singleLegConversions = cms.InputTag("reducedEgamma", "reducedSingleLegConversions", "RECO"),
    singleLegConversions = cms.InputTag("particleFlowEGamma", "", "RECO"),

    gedGsfElectronCores = cms.InputTag("gedGsfElectronCores", "", "RECO"),
    gedPhotonCores = cms.InputTag("gedPhotonCore", "", "RECO"),
    generalTracks = cms.InputTag("generalTracks", "", "RECO"),
    #superClusters = cms.InputTag("reducedEgamma", "reducedSuperClusters", "RECO"),

    #lostTracks = cms.InputTag("lostTracks", "", "RECO")
)

#Add jettiness for AK8 jets
process.load('RecoJets.JetProducers.nJettinessAdder_cfi')
process.NjettinessAK8CHS = process.Njettiness.clone()

#PAT Stuff
process.load('PhysicsTools.PatAlgos.producersLayer1.tauProducer_cff')
process.load('PhysicsTools.PatAlgos.producersLayer1.jetProducer_cff')

process.patCandidatesTask = cms.Task(
    #makePatElectronsTask,
    #makePatMuonsTask,
    process.makePatTausTask,
    #makePatPhotonsTask,
    #makePatOOTPhotonsTask,
    process.makePatJetsTask,
    #makePatMETsTask
    )
process.patCandidates = cms.Sequence(process.patCandidatesTask)


process.load('PhysicsTools.PatAlgos.selectionLayer1.tauSelector_cfi')
process.load('PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi')
process.selectedPatCandidatesTask = cms.Task(
     #selectedPatElectrons,
     #selectedPatMuons,
    process.selectedPatTaus,
     #selectedPatPhotons,
     #selectedPatOOTPhotons,
    process.selectedPatJets
 )
process.selectedPatCandidates = cms.Sequence(process.selectedPatCandidatesTask)

process.load('PhysicsTools.PatAlgos.slimming.slimmedTaus_cfi')
process.load('PhysicsTools.PatAlgos.slimming.packedPFCandidates_cff')
process.slimmingTask = cms.Task(
    process.packedPFCandidatesTask,
     # lostTracks,
     # isolatedTracks,
     # offlineSlimmedPrimaryVertices,
     # primaryVertexAssociation,
     # genParticlesTask,
     # selectedPatTrigger,
     # slimmedPatTrigger,
     # slimmedCaloJets,
     # slimmedJets,
     # slimmedJetsAK8,
     # slimmedGenJets,
     # slimmedGenJetsAK8,
     # slimmedElectrons,
     # slimmedMuons,
     # slimmedPhotons,
     # slimmedOOTPhotons,
    process.slimmedTaus,
     # slimmedSecondaryVertices,
     # slimmedKshortVertices,
     # slimmedLambdaVertices,
     # slimmedMETs,
     # metFilterPathsTask,
     # reducedEgamma,
     # bunchSpacingProducer,
     # oniaPhotonCandidates
    )
process.patTask = cms.Task(
    process.patCandidatesTask,
    process.selectedPatCandidatesTask,
    #process.slimmingTask,
    #process.bunchSpacingProducer
)

#Define Execution Paths
process.outputPath = cms.EndPath(process.output)
process.p = cms.Path(process.NjettinessAK8CHS * process.metFilters * process.ntuples )
process.schedule = cms.Schedule(process.p)


#Define Jet Tool Box Stuff
listBtagDiscriminatorsAK4 = [ 
                'pfJetProbabilityBJetTags',
                'pfCombinedInclusiveSecondaryVertexV2BJetTags',
                'pfCombinedMVAV2BJetTags',
                'pfCombinedCvsLJetTags',
                'pfCombinedCvsBJetTags',
                ]
from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox
#jetToolbox( process, 'ak8', 'ak8JetSubs', 'jetSequence', PUMethod='CHS', bTagDiscriminators=listBtagDiscriminatorsAK4, addPruning=True, addSoftDrop=True, addTrimming=True, addFiltering=True, addMassDrop=True, addNsub=True, addNsubSubjets=True, addPrunedSubjets=True, addPUJetID=True, addQJets=True, addQGTagger=True, miniAOD=False )   ### For example
jetToolbox( process, 'ak8', 'ak8JetSubs', "out", PUMethod='CHS', bTagDiscriminators=listBtagDiscriminatorsAK4, addSoftDrop=True, addNsub=True, addNsubSubjets=True, miniAOD=False )   ### For example


#Add PAT tasks for jet Toolbox to execution schedule
process.schedule.associate(process.patTask)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)


#miniAOD_customize stuff
process.patTaus.isoDeposits = cms.PSet()
process.selectedPatTaus.cut = cms.string("pt > 18. && tauID('decayModeFindingNewDMs')> 0.5")
process.selectedPatJets.cut = cms.string("pt > 10")

process.patJets.discriminatorSources = cms.VInputTag(
    cms.InputTag("pfJetBProbabilityBJetTags"),
    cms.InputTag("pfJetProbabilityBJetTags"),
    cms.InputTag("pfTrackCountingHighEffBJetTags"),
    cms.InputTag("pfSimpleSecondaryVertexHighEffBJetTags"),
    cms.InputTag("pfSimpleInclusiveSecondaryVertexHighEffBJetTags"),
    cms.InputTag("pfCombinedSecondaryVertexV2BJetTags"),
    cms.InputTag("pfCombinedInclusiveSecondaryVertexV2BJetTags"),
    cms.InputTag("softPFMuonBJetTags"),
    cms.InputTag("softPFElectronBJetTags"),
    cms.InputTag("pfCombinedMVAV2BJetTags"),   
    )