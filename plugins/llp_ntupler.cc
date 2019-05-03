// -*- C++ -*-
// Class:      llp_ntupler
/*
  Description: Base class for miniAOD analysis with CRAB
*/
//         Author:  Cristián Peña and Si Xie.
//         Created:  Thu, 26 March 2019 15:00:06 GMT

#include "llp_ntupler.h"
//------ Constructors and destructor ------//
llp_ntupler::llp_ntupler(const edm::ParameterSet& iConfig):
  //get inputs from config file
  isData_(iConfig.getParameter<bool> ("isData")),
  //isFourJet_(iConfig.getParameter<bool> ("isFourJet")),
  useGen_(iConfig.getParameter<bool> ("useGen")),
  isFastsim_(iConfig.getParameter<bool> ("isFastsim")),
  //isQCD_(iConfig.getParameter<bool> ("isQCD")),
  enableTriggerInfo_(iConfig.getParameter<bool> ("enableTriggerInfo")),
  //enableRecHitInfo_(iConfig.getParameter<bool> ("enableRecHitInfo")),
  readGenVertexTime_(iConfig.getParameter<bool> ("readGenVertexTime")),
  triggerPathNamesFile_(iConfig.getParameter<string> ("triggerPathNamesFile")),
  eleHLTFilterNamesFile_(iConfig.getParameter<string> ("eleHLTFilterNamesFile")),
  muonHLTFilterNamesFile_(iConfig.getParameter<string> ("muonHLTFilterNamesFile")),
  photonHLTFilterNamesFile_(iConfig.getParameter<string> ("photonHLTFilterNamesFile")),
  verticesToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
  tracksTag_(consumes<edm::View<reco::Track> >(iConfig.getParameter<edm::InputTag>("tracks"))),
  cscSegmentInputToken_(consumes<CSCSegmentCollection>(edm::InputTag("cscSegments"))),
  dtSegmentInputToken_(consumes<DTRecSegment4DCollection>(edm::InputTag("dt4DCosmicSegments"))),
  rpcRecHitInputToken_(consumes<RPCRecHitCollection>(edm::InputTag("rpcRecHits"))),
  muonsToken_(consumes<reco::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
  electronsToken_(consumes<reco::GsfElectronCollection>(iConfig.getParameter<edm::InputTag>("electrons"))),
  tausToken_(consumes<reco::PFTauCollection>(iConfig.getParameter<edm::InputTag>("taus"))),
  photonsToken_(consumes<reco::PhotonCollection>(iConfig.getParameter<edm::InputTag>("photons"))),
  jetsToken_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("jets"))),
  jetsPuppiToken_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("jetsPuppi"))),
  jetsAK8Token_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("jetsAK8"))),
  PFCandsToken_(consumes<reco::PFCandidateCollection>(iConfig.getParameter<edm::InputTag>("pfCands"))),
  PFClustersToken_(consumes<reco::PFClusterCollection>(iConfig.getParameter<edm::InputTag>("pfClusters"))),
  //genParticlesToken_(consumes<edm::View<reco::GenParticle> >(iConfig.getParameter<edm::InputTag>("genParticles"))),
  //genParticlesToken_(consumes<edm::View<pat::PackedGenParticle> >(iConfig.getParameter<edm::InputTag>("genParticles"))),
  genParticlesToken_(consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticles"))),
  genJetsToken_(consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("genJets"))),
  triggerBitsToken_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerBits"))),
  hepMCToken_(consumes<edm::HepMCProduct>(iConfig.getParameter<edm::InputTag>("hepMC"))),
  //triggerObjectsToken_(consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("triggerObjects"))),
  //triggerPrescalesToken_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("triggerPrescales"))),
  genMetCaloToken_(consumes<reco::GenMETCollection>(iConfig.getParameter<edm::InputTag>("genMetsCalo"))),
  genMetTrueToken_(consumes<reco::GenMETCollection>(iConfig.getParameter<edm::InputTag>("genMetsTrue"))),
  metToken_(consumes<reco::PFMETCollection>(iConfig.getParameter<edm::InputTag>("mets"))),
//  metNoHFToken_(consumes<reco::PFMETCollection>(iConfig.getParameter<edm::InputTag>("metsNoHF"))),
  metPuppiToken_(consumes<reco::PFMETCollection>(iConfig.getParameter<edm::InputTag>("metsPuppi"))),
  metFilterBitsToken_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("metFilterBits"))),
  //hbheNoiseFilterToken_(consumes<bool>(iConfig.getParameter<edm::InputTag>("hbheNoiseFilter"))),
  //hbheTightNoiseFilterToken_(consumes<bool>(iConfig.getParameter<edm::InputTag>("hbheTightNoiseFilter"))),
  //hbheIsoNoiseFilterToken_(consumes<bool>(iConfig.getParameter<edm::InputTag>("hbheIsoNoiseFilter"))),
  //badChargedCandidateFilterToken_(consumes<bool>(iConfig.getParameter<edm::InputTag>("BadChargedCandidateFilter"))),
  //badMuonFilterToken_(consumes<bool>(iConfig.getParameter<edm::InputTag>("BadMuonFilter"))),
//  lheRunInfoTag_(iConfig.getParameter<edm::InputTag>("lheInfo")),
//  lheRunInfoToken_(consumes<LHERunInfoProduct,edm::InRun>(lheRunInfoTag_)),
//  lheInfoToken_(consumes<LHEEventProduct>(iConfig.getParameter<edm::InputTag>("lheInfo"))),
  genInfoToken_(consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("genInfo"))),
  genLumiHeaderToken_(consumes<GenLumiInfoHeader,edm::InLumi>(edm::InputTag("generator",""))),
  puInfoToken_(consumes<std::vector<PileupSummaryInfo> >(iConfig.getParameter<edm::InputTag>("puInfo"))),
  //hcalNoiseInfoToken_(consumes<HcalNoiseSummary>(iConfig.getParameter<edm::InputTag>("hcalNoiseInfo"))),
  secondaryVerticesToken_(consumes<vector<reco::VertexCompositePtrCandidate> >(iConfig.getParameter<edm::InputTag>("secondaryVertices"))),
  rhoAllToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoAll"))),
  rhoFastjetAllToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoFastjetAll"))),
  rhoFastjetAllCaloToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoFastjetAllCalo"))),
  rhoFastjetCentralCaloToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoFastjetCentralCalo"))),
  rhoFastjetCentralChargedPileUpToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoFastjetCentralChargedPileUp"))),
  rhoFastjetCentralNeutralToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoFastjetCentralNeutral"))),
  beamSpotToken_(consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("beamSpot"))),
  ebRecHitsToken_(consumes<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > >(iConfig.getParameter<edm::InputTag>("ebRecHits"))),
  eeRecHitsToken_(consumes<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > >(iConfig.getParameter<edm::InputTag>("eeRecHits"))),
  esRecHitsToken_(consumes<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > >(iConfig.getParameter<edm::InputTag>("esRecHits"))),
  ebeeClustersToken_(consumes<vector<reco::CaloCluster> >(iConfig.getParameter<edm::InputTag>("ebeeClusters"))),
  esClustersToken_(consumes<vector<reco::CaloCluster> >(iConfig.getParameter<edm::InputTag>("esClusters"))),
  conversionsToken_(consumes<vector<reco::Conversion> >(iConfig.getParameter<edm::InputTag>("conversions"))),
  singleLegConversionsToken_(consumes<vector<reco::Conversion> >(iConfig.getParameter<edm::InputTag>("singleLegConversions"))),
  gedGsfElectronCoresToken_(consumes<vector<reco::GsfElectronCore> >(iConfig.getParameter<edm::InputTag>("gedGsfElectronCores"))),
  gedPhotonCoresToken_(consumes<vector<reco::PhotonCore> >(iConfig.getParameter<edm::InputTag>("gedPhotonCores")))
  //superClustersToken_(consumes<vector<reco::SuperCluster> >(iConfig.getParameter<edm::InputTag>("superClusters"))),
  //  lostTracksToken_(consumes<vector<reco::PFCandidate> >(iConfig.getParameter<edm::InputTag>("lostTracks")))
{
  //declare the TFileService for output
  edm::Service<TFileService> fs;

  //set up output tree
  llpTree = fs->make<TTree>("llp", "selected AOD information for llp analyses");
  //llpTree = new TTree("Jets", "selected AOD information");
  NEvents = fs->make<TH1F>("NEvents",";;NEvents;",1,-0.5,0.5);

  /*
  //set up electron MVA ID
  std::vector<std::string> myTrigWeights;
  myTrigWeights.push_back(edm::FileInPath("cms_lpc_llp/llp_ntupler/data/TrigIDMVA_25ns_EB_BDT.weights.xml").fullPath().c_str());
  myTrigWeights.push_back(edm::FileInPath("cms_lpc_llp/llp_ntupler/data/TrigIDMVA_25ns_EE_BDT.weights.xml").fullPath().c_str());

  myMVATrig = new EGammaMvaEleEstimatorCSA14();
  myMVATrig->initialize("BDT",
  EGammaMvaEleEstimatorCSA14::kTrig,
  true,
  myTrigWeights);

  std::vector<std::string> myNonTrigWeights;
  myNonTrigWeights.push_back(edm::FileInPath("cms_lpc_llp/llp_ntupler/data/EIDmva_EB1_5_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml").fullPath().c_str());
  myNonTrigWeights.push_back(edm::FileInPath("cms_lpc_llp/llp_ntupler/data/EIDmva_EB2_5_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml").fullPath().c_str());
  myNonTrigWeights.push_back(edm::FileInPath("cms_lpc_llp/llp_ntupler/data/EIDmva_EE_5_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml").fullPath().c_str());
  myNonTrigWeights.push_back(edm::FileInPath("cms_lpc_llp/llp_ntupler/data/EIDmva_EB1_10_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml").fullPath().c_str());
  myNonTrigWeights.push_back(edm::FileInPath("cms_lpc_llp/llp_ntupler/data/EIDmva_EB2_10_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml").fullPath().c_str());
  myNonTrigWeights.push_back(edm::FileInPath("cms_lpc_llp/llp_ntupler/data/EIDmva_EE_10_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml").fullPath().c_str());

  myMVANonTrig = new ElectronMVAEstimatorRun2NonTrig();
  myMVANonTrig->initialize("BDTG method",
  ElectronMVAEstimatorRun2NonTrig::kPHYS14,
  true,
  myNonTrigWeights);

  //set up photon MVA ID
  std::vector<std::string> myPhotonMVAWeights;
  myPhotonMVAWeights.push_back(edm::FileInPath("cms_lpc_llp/llp_ntupler/data/PhotonIDMVA_Spring15_50ns_v0_EB.weights.xml").fullPath().c_str());
  myPhotonMVAWeights.push_back(edm::FileInPath("cms_lpc_llp/llp_ntupler/data/PhotonIDMVA_Spring15_50ns_v0_EE.weights.xml").fullPath().c_str());
  std::vector<std::string> myPhotonMVAMethodNames;
  myPhotonMVAMethodNames.push_back("BDTG photons barrel");
  myPhotonMVAMethodNames.push_back("BDTG photons endcap");

  myPhotonMVA = new EGammaMvaPhotonEstimator();
  myPhotonMVA->initialize(myPhotonMVAMethodNames,myPhotonMVAWeights,
    EGammaMvaPhotonEstimator::kPhotonMVATypeDefault);

*/
  //------------------------------------------------------------------
  //Read in HLT Trigger Path List from config file
  //------------------------------------------------------------------
  for (int i = 0; i<NTriggersMAX; ++i) triggerPathNames[i] = "";
  ifstream myfile (edm::FileInPath(triggerPathNamesFile_.c_str()).fullPath().c_str()) ;
  if (myfile.is_open())
  {
    std::string line;
    int index;
    std::string hltpathname;

    while(myfile>>index>>hltpathname)
    {
      if (index < NTriggersMAX)
      {
        triggerPathNames[index] = hltpathname;
      }
    }
    myfile.close();
  }
  else
  {
    std::cout << "ERROR!!! Could not open trigger path name file : " << edm::FileInPath(triggerPathNamesFile_.c_str()).fullPath().c_str() << "\n";
  }

  if(enableTriggerInfo_)
  {
    std::cout << "\n";
    std::cout << "****************** Trigger Paths Defined For Razor Ntuple ******************\n";
    for (int i = 0; i<NTriggersMAX; ++i)
    {
      if (triggerPathNames[i] != "") std::cout << "Trigger " << i << " " << triggerPathNames[i] << "\n";
    }
    std::cout << "****************************************************************************\n";
    std::cout << "\n";
  }
  if(readGenVertexTime_) genParticles_t0_Token_ = consumes<float>(iConfig.getParameter<edm::InputTag>("genParticles_t0"));
  /*
  fJetPhotonRecHitEta = new std::vector<float>; fJetPhotonRecHitEta->clear();
  fJetPhotonRecHitPhi = new std::vector<float>; fJetPhotonRecHitPhi->clear();
  fJetPhotonRecHitE = new std::vector<float>; fJetPhotonRecHitE->clear();
  fJetPhotonRecHitTime = new std::vector<float>; fJetPhotonRecHitTime->clear();
*/
}

llp_ntupler::~llp_ntupler()
{
};

//***********************************************
//Enable output ntuple branches
//***********************************************

void llp_ntupler::setBranches()
{
  enableEventInfoBranches();
  enablePVAllBranches();
  enablePVTracksBranches();
  enablePileUpBranches();
  enableMuonBranches();
  enableElectronBranches();
  enableTauBranches();
  enableIsoPFCandidateBranches();
  enablePhotonBranches();
  enableMuonSystemBranches();
  enableEcalRechitBranches();
  enableJetBranches();
  enableJetAK8Branches();
  enableMetBranches();
  enableTriggerBranches();
  enableMCBranches();
  enableGenParticleBranches();
};

void llp_ntupler::enableEventInfoBranches()
{
  llpTree->Branch("isData", &isData, "isData/O");
  llpTree->Branch("nPV", &nPV, "nPV/I");
  llpTree->Branch("runNum", &runNum, "runNum/i");
  llpTree->Branch("nSlimmedSecondV", &nSlimmedSecondV, "nSlimmedSecondV/i");
  llpTree->Branch("lumiNum", &lumiNum, "lumiNum/i");
  llpTree->Branch("eventNum", &eventNum, "eventNum/i");
  llpTree->Branch("eventTime", &eventTime, "eventTime/i");
  llpTree->Branch("pvX", &pvX, "pvX/F");
  llpTree->Branch("pvY", &pvY, "pvY/F");
  llpTree->Branch("pvZ", &pvZ, "pvZ/F");
  llpTree->Branch("fixedGridRhoAll", &fixedGridRhoAll, "fixedGridRhoAll/F");
  llpTree->Branch("fixedGridRhoFastjetAll", &fixedGridRhoFastjetAll, "fixedGridRhoFastjetAll/F");
  llpTree->Branch("fixedGridRhoFastjetAllCalo", &fixedGridRhoFastjetAllCalo, "fixedGridRhoFastjetAllCalo/F");
  llpTree->Branch("fixedGridRhoFastjetCentralCalo", &fixedGridRhoFastjetCentralCalo, "fixedGridRhoFastjetCentralCalo/F");
  llpTree->Branch("fixedGridRhoFastjetCentralChargedPileUp", &fixedGridRhoFastjetCentralChargedPileUp, "fixedGridRhoFastjetCentralChargedPileUp/F");
  llpTree->Branch("fixedGridRhoFastjetCentralNeutral", &fixedGridRhoFastjetCentralNeutral, "fixedGridRhoFastjetCentralNeutral/F");
}

void llp_ntupler::enablePVAllBranches()
{
  llpTree->Branch("nPVAll", &nPVAll,"nPVAll/I");
  llpTree->Branch("pvAllX", pvAllX,"pvAllX[nPVAll]/F");
  llpTree->Branch("pvAllY", pvAllY,"pvAllY[nPVAll]/F");
  llpTree->Branch("pvAllZ", pvAllZ,"pvAllZ[nPVAll]/F");
  llpTree->Branch("pvAllLogSumPtSq", pvAllLogSumPtSq,"pvAllLogSumPtSq[nPVAll]/F");
  llpTree->Branch("pvAllSumPx", pvAllSumPx,"pvAllSumPx[nPVAll]/F");
  llpTree->Branch("pvAllSumPy", pvAllSumPy,"pvAllSumPy[nPVAll]/F");
};

void llp_ntupler::enablePVTracksBranches()
{
  llpTree->Branch("nPVTracks", &nPVTracks,"nPVTracks/I");
  llpTree->Branch("pvTrackPt", pvTrackPt,"pvTrackPt[nPVTracks]/F");
  llpTree->Branch("pvTrackEta", pvTrackEta,"pvTrackEta[nPVTracks]/F");
  llpTree->Branch("pvTrackPhi", pvTrackPhi,"pvTrackPhi[nPVTracks]/F");
};

void llp_ntupler::enablePileUpBranches()
{
  llpTree->Branch("nBunchXing", &nBunchXing, "nBunchXing/I");
  llpTree->Branch("BunchXing", BunchXing, "BunchXing[nBunchXing]/I");
  llpTree->Branch("nPU", nPU, "nPU[nBunchXing]/I");
  llpTree->Branch("nPUmean", nPUmean, "nPUmean[nBunchXing]/F");
};

void llp_ntupler::enableMuonBranches()
{
  llpTree->Branch("nMuons", &nMuons,"nMuons/I");
  llpTree->Branch("muonE", muonE,"muonE[nMuons]/F");
  llpTree->Branch("muonPt", muonPt,"muonPt[nMuons]/F");
  llpTree->Branch("muonEta", muonEta,"muonEta[nMuons]/F");
  llpTree->Branch("muonPhi", muonPhi,"muonPhi[nMuons]/F");
  llpTree->Branch("muonCharge", muonCharge, "muonCharge[nMuons]/I");
  llpTree->Branch("muonIsLoose", muonIsLoose,"muonIsLoose[nMuons]/O");
  llpTree->Branch("muonIsMedium", muonIsMedium,"muonIsMedium[nMuons]/O");
  llpTree->Branch("muonIsTight", muonIsTight,"muonIsTight[nMuons]/O");
  llpTree->Branch("muon_d0", muon_d0, "muon_d0[nMuons]/F");
  llpTree->Branch("muon_dZ", muon_dZ, "muon_dZ[nMuons]/F");
  llpTree->Branch("muon_ip3d", muon_ip3d, "muon_ip3d[nMuons]/F");
  llpTree->Branch("muon_ip3dSignificance", muon_ip3dSignificance, "muon_ip3dSignificance[nMuons]/F");
  llpTree->Branch("muonType", muonType, "muonType[nMuons]/i");
  llpTree->Branch("muonQuality", muonQuality, "muonQuality[nMuons]/i");
  llpTree->Branch("muon_pileupIso", muon_pileupIso, "muon_pileupIso[nMuons]/F");
  llpTree->Branch("muon_chargedIso", muon_chargedIso, "muon_chargedIso[nMuons]/F");
  llpTree->Branch("muon_photonIso", muon_photonIso, "muon_photonIso[nMuons]/F");
  llpTree->Branch("muon_neutralHadIso", muon_neutralHadIso, "muon_neutralHadIso[nMuons]/F");
  llpTree->Branch("muon_ptrel", muon_ptrel, "muon_ptrel[nMuons]/F");
  llpTree->Branch("muon_chargedMiniIso", muon_chargedMiniIso, "muon_chargedMiniIso[nMuons]/F");
  llpTree->Branch("muon_photonAndNeutralHadronMiniIso", muon_photonAndNeutralHadronMiniIso, "muon_photonAndNeutralHadronMiniIso[nMuons]/F");
  llpTree->Branch("muon_chargedPileupMiniIso", muon_chargedPileupMiniIso, "muon_chargedPileupMiniIso[nMuons]/F");
  llpTree->Branch("muon_activityMiniIsoAnnulus", muon_activityMiniIsoAnnulus, "muon_activityMiniIsoAnnulus[nMuons]/F");
  llpTree->Branch("muon_passSingleMuTagFilter", muon_passSingleMuTagFilter, "muon_passSingleMuTagFilter[nMuons]/O");
  llpTree->Branch("muon_passHLTFilter", &muon_passHLTFilter, Form("muon_passHLTFilter[nMuons][%d]/O",MAX_MuonHLTFilters));
  llpTree->Branch("muon_validFractionTrackerHits", muon_validFractionTrackerHits, "muon_validFractionTrackerHits[nMuons]/F");
  llpTree->Branch("muon_isGlobal", muon_isGlobal,"muon_isGlobal[nMuons]/O");
  llpTree->Branch("muon_normChi2", muon_normChi2,"muon_normChi2[nMuons]/F");
  llpTree->Branch("muon_chi2LocalPosition", muon_chi2LocalPosition,"muon_chi2LocalPosition[nMuons]/F");
  llpTree->Branch("muon_kinkFinder", muon_kinkFinder,"muon_kinkFinder[nMuons]/F");
  llpTree->Branch("muon_segmentCompatability", muon_segmentCompatability,"muon_segmentCompatability[nMuons]/F");
  llpTree->Branch("muonIsICHEPMedium", muonIsICHEPMedium,"muonIsICHEPMedium[nMuons]/O");
};

void llp_ntupler::enableElectronBranches()
{
  llpTree->Branch("nElectrons", &nElectrons,"nElectrons/I");
  llpTree->Branch("eleE", eleE,"eleE[nElectrons]/F");
  llpTree->Branch("elePt", elePt,"elePt[nElectrons]/F");
  llpTree->Branch("eleEta", eleEta,"eleEta[nElectrons]/F");
  llpTree->Branch("elePhi", elePhi,"elePhi[nElectrons]/F");
  llpTree->Branch("eleCharge", eleCharge, "eleCharge[nElectrons]/F");
  //llpTree->Branch("EleE_SC", eleE_SC,"eleE_SC[nElectrons]/F");
  llpTree->Branch("eleEta_SC", eleEta_SC,"eleEta_SC[nElectrons]/F");
  //llpTree->Branch("elePhi_SC", elePhi_SC,"elePhi_SC[nElectrons]/F");
  llpTree->Branch("eleSigmaIetaIeta", eleSigmaIetaIeta, "eleSigmaIetaIeta[nElectrons]/F");
  llpTree->Branch("eleFull5x5SigmaIetaIeta", eleFull5x5SigmaIetaIeta, "eleFull5x5SigmaIetaIeta[nElectrons]/F");
  llpTree->Branch("eleR9", eleR9, "eleR9[nElectrons]/F");
  llpTree->Branch("ele_dEta", ele_dEta, "ele_dEta[nElectrons]/F");
  llpTree->Branch("ele_dPhi", ele_dPhi, "ele_dPhi[nElectrons]/F");
  llpTree->Branch("ele_HoverE", ele_HoverE, "ele_HoverE[nElectrons]/F");
  llpTree->Branch("ele_d0", ele_d0, "ele_d0[nElectrons]/F");
  llpTree->Branch("ele_dZ", ele_dZ, "ele_dZ[nElectrons]/F");
  llpTree->Branch("ele_ip3d", ele_ip3d, "ele_ip3d[nElectrons]/F");
  llpTree->Branch("ele_ip3dSignificance", ele_ip3dSignificance, "ele_ip3dSignificance[nElectrons]/F");
  llpTree->Branch("ele_pileupIso", ele_pileupIso, "ele_pileupIso[nElectrons]/F");
  llpTree->Branch("ele_chargedIso", ele_chargedIso, "ele_chargedIso[nElectrons]/F");
  llpTree->Branch("ele_photonIso", ele_photonIso, "ele_photonIso[nElectrons]/F");
  llpTree->Branch("ele_neutralHadIso", ele_neutralHadIso, "ele_neutralHadIso[nElectrons]/F");
  llpTree->Branch("ele_MissHits", ele_MissHits, "ele_MissHits[nElectrons]/I");
  llpTree->Branch("ele_PassConvVeto", ele_PassConvVeto, "ele_PassConvVeto[nElectrons]/O");
  llpTree->Branch("ele_OneOverEminusOneOverP", ele_OneOverEminusOneOverP, "ele_OneOverEminusOneOverP[nElectrons]/F");
  llpTree->Branch("ele_IDMVAGeneralPurpose", ele_IDMVAGeneralPurpose, "ele_IDMVAGeneralPurpose[nElectrons]/F");
  llpTree->Branch("ele_IDMVACategoryGeneralPurpose", ele_IDMVACategoryGeneralPurpose, "ele_IDMVACategoryGeneralPurpose[nElectrons]/I");
  llpTree->Branch("ele_IDMVAHZZ", ele_IDMVAHZZ, "ele_IDMVAHZZ[nElectrons]/F");
  llpTree->Branch("ele_IDMVACategoryHZZ", ele_IDMVACategoryHZZ, "ele_IDMVACategoryHZZ[nElectrons]/I");
  llpTree->Branch("ele_RegressionE", ele_RegressionE, "ele_RegressionE[nElectrons]/F");
  llpTree->Branch("ele_CombineP4", ele_CombineP4, "ele_CombineP4[nElectrons]/F");
  llpTree->Branch("ele_ptrel", ele_ptrel, "ele_ptrel[nElectrons]/F");
  llpTree->Branch("ele_chargedMiniIso", ele_chargedMiniIso, "ele_chargedMiniIso[nElectrons]/F");
  llpTree->Branch("ele_photonAndNeutralHadronMiniIso", ele_photonAndNeutralHadronMiniIso, "ele_photonAndNeutralHadronMiniIso[nElectrons]/F");
  llpTree->Branch("ele_chargedPileupMiniIso", ele_chargedPileupMiniIso, "ele_chargedPileupMiniIso[nElectrons]/F");
  llpTree->Branch("ele_activityMiniIsoAnnulus", ele_activityMiniIsoAnnulus, "ele_activityMiniIsoAnnulus[nElectrons]/F");
  llpTree->Branch("ele_passSingleEleTagFilter", ele_passSingleEleTagFilter, "ele_passSingleEleTagFilter[nElectrons]/O");
  llpTree->Branch("ele_passTPOneTagFilter", ele_passTPOneTagFilter, "ele_passTPOneTagFilter[nElectrons]/O");
  llpTree->Branch("ele_passTPTwoTagFilter", ele_passTPTwoTagFilter, "ele_passTPTwoTagFilter[nElectrons]/O");
  llpTree->Branch("ele_passTPOneProbeFilter", ele_passTPOneProbeFilter, "ele_passTPOneProbeFilter[nElectrons]/O");
  llpTree->Branch("ele_passTPTwoProbeFilter", ele_passTPTwoProbeFilter, "ele_passTPTwoProbeFilter[nElectrons]/O");
  llpTree->Branch("ele_passHLTFilter", &ele_passHLTFilter, Form("ele_passHLTFilter[nElectrons][%d]/O",MAX_ElectronHLTFilters));
  if (enableEcalRechits_) {
    ele_EcalRechitIndex = new std::vector<std::vector<uint> >; ele_EcalRechitIndex->clear();
    llpTree->Branch("ele_EcalRechitIndex", "std::vector<std::vector<uint> >",&ele_EcalRechitIndex);
    ele_SeedRechitIndex = new std::vector<uint>; ele_SeedRechitIndex->clear();
    llpTree->Branch("ele_SeedRechitIndex", "std::vector<uint>",&ele_SeedRechitIndex);
  }
};

void llp_ntupler::enableTauBranches()
{
  llpTree->Branch("nTaus", &nTaus,"nTaus/I");
  llpTree->Branch("tauE", tauE,"tauE[nTaus]/F");
  llpTree->Branch("tauPt", tauPt,"tauPt[nTaus]/F");
  llpTree->Branch("tauEta", tauEta,"tauEta[nTaus]/F");
  llpTree->Branch("tauPhi", tauPhi,"tauPhi[nTaus]/F");
  llpTree->Branch("tau_IsLoose", tau_IsLoose, "tau_IsLoose[nTaus]/O");
  llpTree->Branch("tau_IsMedium", tau_IsMedium, "tau_IsMedium[nTaus]/O");
  llpTree->Branch("tau_IsTight", tau_IsTight, "tau_IsTight[nTaus]/O");
  llpTree->Branch("tau_passEleVetoLoose", tau_passEleVetoLoose, "tau_passEleVetoLoose[nTaus]/O");
  llpTree->Branch("tau_passEleVetoMedium", tau_passEleVetoMedium, "tau_passEleVetoMedium[nTaus]/O");
  llpTree->Branch("tau_passEleVetoTight", tau_passEleVetoTight, "tau_passEleVetoTight[nTaus]/O");
  llpTree->Branch("tau_passMuVetoLoose", tau_passMuVetoLoose, "tau_passMuVetoLoose[nTaus]/O");
  llpTree->Branch("tau_passMuVetoMedium", tau_passMuVetoMedium, "tau_passMuVetoMedium[nTaus]/O");
  llpTree->Branch("tau_passMuVetoTight", tau_passMuVetoTight, "tau_passMuVetoTight[nTaus]/O");
  llpTree->Branch("tau_ID", tau_ID, "tau_ID[nTaus]/i");
  llpTree->Branch("tau_combinedIsoDeltaBetaCorr3Hits", tau_combinedIsoDeltaBetaCorr3Hits, "tau_combinedIsoDeltaBetaCorr3Hits[nTaus]/F");
  llpTree->Branch("tau_chargedIsoPtSum", tau_chargedIsoPtSum, "tau_chargedIsoPtSum[nTaus]/F");
  llpTree->Branch("tau_neutralIsoPtSum", tau_neutralIsoPtSum, "tau_neutralIsoPtSum[nTaus]/F");
  llpTree->Branch("tau_puCorrPtSum", tau_puCorrPtSum, "tau_puCorrPtSum[nTaus]/F");
  llpTree->Branch("tau_eleVetoMVA", tau_eleVetoMVA, "tau_eleVetoMVA[nTaus]/F");
  llpTree->Branch("tau_eleVetoCategory", tau_eleVetoCategory, "tau_eleVetoCategory[nTaus]/I");
  llpTree->Branch("tau_muonVetoMVA", tau_muonVetoMVA, "tau_muonVetoMVA[nTaus]/F");
  llpTree->Branch("tau_isoMVAnewDMwLT", tau_isoMVAnewDMwLT, "tau_isoMVAnewDMwLT[nTaus]/F");
  llpTree->Branch("tau_isoMVAnewDMwoLT", tau_isoMVAnewDMwoLT, "tau_isoMVAnewDMwoLT[nTaus]/F");
  llpTree->Branch("tau_leadCandPt", tau_leadCandPt, "tau_leadCandPt[nTaus]/F");
  llpTree->Branch("tau_leadCandID", tau_leadCandID, "tau_leadCandID[nTaus]/I");
  llpTree->Branch("tau_leadChargedHadrCandPt", tau_leadChargedHadrCandPt, "tau_leadChargedHadrCandPt[nTaus]/F");
  llpTree->Branch("tau_leadChargedHadrCandID", tau_leadChargedHadrCandID, "tau_leadChargedHadrCandID[nTaus]/I");
};

void llp_ntupler::enableIsoPFCandidateBranches()
{
  llpTree->Branch("nIsoPFCandidates", &nIsoPFCandidates, "nIsoPFCandidates/i");
  llpTree->Branch("isoPFCandidatePt", isoPFCandidatePt, "isoPFCandidatePt[nIsoPFCandidates]/F");
  llpTree->Branch("isoPFCandidateEta", isoPFCandidateEta, "isoPFCandidateEta[nIsoPFCandidates]/F");
  llpTree->Branch("isoPFCandidatePhi", isoPFCandidatePhi, "isoPFCandidatePhi[nIsoPFCandidates]/F");
  llpTree->Branch("isoPFCandidateIso04", isoPFCandidateIso04, "isoPFCandidateIso04[nIsoPFCandidates]/F");
  llpTree->Branch("isoPFCandidateD0", isoPFCandidateD0, "isoPFCandidateD0[nIsoPFCandidates]/F");
  llpTree->Branch("isoPFCandidatePdgId", isoPFCandidatePdgId, "isoPFCandidatePdgId[nIsoPFCandidates]/I");
};

void llp_ntupler::enablePhotonBranches()
{
  llpTree->Branch("nPhotons", &nPhotons,"nPhotons/I");
  llpTree->Branch("nPhotons_overlap", &nPhotons_overlap,"nPhotons_overlap/I");
  llpTree->Branch("phoE", phoE,"phoE[nPhotons]/F");
  llpTree->Branch("phoPt", phoPt,"phoPt[nPhotons]/F");
  llpTree->Branch("phoEta", phoEta,"phoEta[nPhotons]/F");
  llpTree->Branch("phoPhi", phoPhi,"phoPhi[nPhotons]/F");
  llpTree->Branch("phoSigmaIetaIeta", phoSigmaIetaIeta, "phoSigmaIetaIeta[nPhotons]/F");
  llpTree->Branch("phoFull5x5SigmaIetaIeta", phoFull5x5SigmaIetaIeta, "phoFull5x5SigmaIetaIeta[nPhotons]/F");
  llpTree->Branch("phoR9", phoR9, "phoR9[nPhotons]/F");
  llpTree->Branch("pho_sminor", pho_sminor, "pho_sminor[nPhotons]/F");
  llpTree->Branch("pho_smajor", pho_smajor, "pho_smajor[nPhotons]/F");
  llpTree->Branch("pho_HoverE", pho_HoverE, "pho_HoverE[nPhotons]/F");
  llpTree->Branch("pho_sumChargedHadronPtAllVertices", &pho_sumChargedHadronPtAllVertices,Form("pho_sumChargedHadronPtAllVertices[nPhotons][%d]/F",MAX_NPV));
  llpTree->Branch("pho_sumChargedHadronPt", &pho_sumChargedHadronPt, "pho_sumChargedHadronPt[nPhotons]/F");
  llpTree->Branch("pho_sumNeutralHadronEt", pho_sumNeutralHadronEt, "pho_sumNeutralHadronEt[nPhotons]/F");
  llpTree->Branch("pho_sumPhotonEt", pho_sumPhotonEt, "pho_sumPhotonEt[nPhotons]/F");
  llpTree->Branch("pho_ecalPFClusterIso", pho_ecalPFClusterIso, "pho_ecalPFClusterIso[nPhotons]/F");
  llpTree->Branch("pho_hcalPFClusterIso", pho_hcalPFClusterIso, "pho_hcalPFClusterIso[nPhotons]/F");
  llpTree->Branch("pho_trkSumPtHollowConeDR03", pho_trkSumPtHollowConeDR03, "pho_trkSumPtHollowConeDR03[nPhotons]/F");
  llpTree->Branch("pho_sumWorstVertexChargedHadronPt", pho_sumWorstVertexChargedHadronPt, "pho_sumWorstVertexChargedHadronPt[nPhotons]/F");
  llpTree->Branch("pho_pfIsoChargedHadronIso", pho_pfIsoChargedHadronIso, "pho_pfIsoChargedHadronIso[nPhotons]/F");
  llpTree->Branch("pho_pfIsoChargedHadronIsoWrongVtx", pho_pfIsoChargedHadronIsoWrongVtx, "pho_pfIsoChargedHadronIsoWrongVtx[nPhotons]/F");
  llpTree->Branch("pho_pfIsoNeutralHadronIso", pho_pfIsoNeutralHadronIso, "pho_pfIsoNeutralHadronIso[nPhotons]/F");
  llpTree->Branch("pho_pfIsoPhotonIso", pho_pfIsoPhotonIso, "pho_pfIsoPhotonIso[nPhotons]/F");
  llpTree->Branch("pho_pfIsoModFrixione", pho_pfIsoModFrixione, "pho_pfIsoModFrixione[nPhotons]/F");
  llpTree->Branch("pho_pfIsoSumPUPt", pho_pfIsoSumPUPt, "pho_pfIsoSumPUPt[nPhotons]/F");
  llpTree->Branch("pho_isConversion", pho_isConversion, "pho_isConversion[nPhotons]/O");
  llpTree->Branch("pho_passEleVeto", pho_passEleVeto, "pho_passEleVeto[nPhotons]/O");
  llpTree->Branch("pho_RegressionE", pho_RegressionE, "pho_RegressionE[nPhotons]/F");
  llpTree->Branch("pho_RegressionEUncertainty", pho_RegressionEUncertainty, "pho_RegressionEUncertainty[nPhotons]/F");
  llpTree->Branch("pho_IDMVA", pho_IDMVA, "pho_IDMVA[nPhotons]/F");
  llpTree->Branch("pho_superClusterEnergy", pho_superClusterEnergy, "pho_superClusterEnergy[nPhotons]/F");
  llpTree->Branch("pho_superClusterRawEnergy", pho_superClusterRawEnergy, "pho_superClusterRawEnergy[nPhotons]/F");
  llpTree->Branch("pho_superClusterEta", pho_superClusterEta, "pho_superClusterEta[nPhotons]/F");
  llpTree->Branch("pho_superClusterPhi", pho_superClusterPhi, "pho_superClusterPhi[nPhotons]/F");
  llpTree->Branch("pho_superClusterX", pho_superClusterX, "pho_superClusterX[nPhotons]/F");
  llpTree->Branch("pho_superClusterY", pho_superClusterY, "pho_superClusterY[nPhotons]/F");
  llpTree->Branch("pho_superClusterZ", pho_superClusterZ, "pho_superClusterZ[nPhotons]/F");
  llpTree->Branch("pho_hasPixelSeed", pho_hasPixelSeed, "pho_hasPixelSeed[nPhotons]/O");
  llpTree->Branch("pho_passHLTFilter", &pho_passHLTFilter, Form("pho_passHLTFilter[nPhotons][%d]/O",MAX_PhotonHLTFilters));
  llpTree->Branch("pho_convType", pho_convType, "pho_convType[nPhotons]/I");
  llpTree->Branch("pho_convTrkZ", pho_convTrkZ, "pho_convTrkZ[nPhotons]/F");
  llpTree->Branch("pho_convTrkClusZ", pho_convTrkClusZ, "pho_convTrkClusZ[nPhotons]/F");
  llpTree->Branch("pho_vtxSumPx", &pho_vtxSumPx,Form("pho_vtxSumPx[nPhotons][%d]/F",MAX_NPV));
  llpTree->Branch("pho_vtxSumPy", &pho_vtxSumPy,Form("pho_vtxSumPy[nPhotons][%d]/F",MAX_NPV));
  llpTree->Branch("pho_isStandardPhoton", pho_isStandardPhoton, "pho_isStandardPhoton[nPhotons]/O");
  llpTree->Branch("pho_seedRecHitSwitchToGain6", pho_seedRecHitSwitchToGain6, "pho_seedRecHitSwitchToGain6[nPhotons]/F");
  llpTree->Branch("pho_seedRecHitSwitchToGain1", pho_seedRecHitSwitchToGain1, "pho_seedRecHitSwitchToGain1[nPhotons]/F");
  llpTree->Branch("pho_anyRecHitSwitchToGain6", pho_anyRecHitSwitchToGain6, "pho_anyRecHitSwitchToGain6[nPhotons]/F");
  llpTree->Branch("pho_anyRecHitSwitchToGain1", pho_anyRecHitSwitchToGain1, "pho_anyRecHitSwitchToGain1[nPhotons]/F");
  if (enableEcalRechits_) {
    pho_EcalRechitIndex = new std::vector<std::vector<uint> >; pho_EcalRechitIndex->clear();
    llpTree->Branch("pho_EcalRechitIndex", "std::vector<std::vector<uint> >",&pho_EcalRechitIndex);
    pho_SeedRechitIndex = new std::vector<uint>; pho_SeedRechitIndex->clear();
    llpTree->Branch("pho_SeedRechitIndex", "std::vector<uint>",&pho_SeedRechitIndex);
  }

};

void llp_ntupler::enableMuonSystemBranches()
{

    // csc_Phi = new std::vector<float>; 
    // csc_Eta = new std::vector<float>;
    // csc_X = new std::vector<float>;
    // csc_Y = new std::vector<float>;
    // csc_Z = new std::vector<float>;
    // csc_NRecHits = new std::vector<float>;
    // csc_T = new std::vector<float>;
    // csc_Chi2 = new std::vector<float>;

    llpTree->Branch("nCsc",&nCsc,"nCsc/I"); 
    llpTree->Branch("cscPhi",cscPhi,"cscPhi[nCsc]");
    llpTree->Branch("cscEta",cscEta,"cscEta[nCsc]"); 
    llpTree->Branch("cscX",cscX,"cscX[nCsc]"); 
    llpTree->Branch("cscY",cscY,"cscY[nCsc]"); 
    llpTree->Branch("cscZ",cscZ,"cscZ[nCsc]"); 
    llpTree->Branch("cscNRecHits",cscNRecHits,"cscNRecHits[nCsc]");
    llpTree->Branch("cscT",cscT,"cscT[nCsc]"); 
    llpTree->Branch("cscChi2",cscChi2,"cscChi2[nCsc]");

    llpTree->Branch("nRpc",&nRpc,"nRpc/I"); 
    llpTree->Branch("rpcPhi",rpcPhi,"rpcPhi[nRpc]");
    llpTree->Branch("rpcEta",rpcEta,"rpcEta[nRpc]"); 
    llpTree->Branch("rpcX",rpcX,"rpcX[nRpc]"); 
    llpTree->Branch("rpcY",rpcY,"rpcY[nRpc]"); 
    llpTree->Branch("rpcZ",rpcZ,"rpcZ[nRpc]"); 
    llpTree->Branch("rpcT",rpcT,"rpcT[nRpc]"); 
    llpTree->Branch("rpcTError",rpcTError,"rpcTError[nRpc]");

    llpTree->Branch("nDt",&nDt,"nDt/I"); 
    llpTree->Branch("dtPhi",dtPhi,"dtPhi[nDt]");
    llpTree->Branch("dtEta",dtEta,"dtEta[nDt]"); 
    llpTree->Branch("dtX",dtX,"dtX[nDt]"); 
    llpTree->Branch("dtY",dtY,"dtY[nDt]"); 
    llpTree->Branch("dtZ",dtZ,"dtZ[nDt]"); 
    llpTree->Branch("dtDirX",dtDirX,"dtDirX[nDt]"); 
    llpTree->Branch("dtDirY",dtDirY,"dtDirY[nDt]"); 
    llpTree->Branch("dtDirZ",dtDirZ,"dtDirZ[nDt]"); 
    llpTree->Branch("dtT",dtT,"dtT[nDt]"); 
    llpTree->Branch("dtTError",dtTError,"dtTError[nDt]");
};

void llp_ntupler::enableEcalRechitBranches()
{
  ecalRechit_Eta = new std::vector<float>; ecalRechit_Eta->clear();
  ecalRechit_Phi = new std::vector<float>; ecalRechit_Phi->clear();
  ecalRechit_X = new std::vector<float>; ecalRechit_X->clear();
  ecalRechit_Y = new std::vector<float>; ecalRechit_Y->clear();
  ecalRechit_Z = new std::vector<float>; ecalRechit_Z->clear();
  ecalRechit_E = new std::vector<float>; ecalRechit_E->clear();
  ecalRechit_T = new std::vector<float>; ecalRechit_T->clear();
  ecalRechit_ID = new std::vector<uint>; ecalRechit_ID->clear();
  ecalRechit_FlagOOT = new std::vector<bool>; ecalRechit_FlagOOT->clear();
  ecalRechit_GainSwitch1 = new std::vector<bool>; ecalRechit_GainSwitch1->clear();
  ecalRechit_GainSwitch6 = new std::vector<bool>; ecalRechit_GainSwitch6->clear();
  ecalRechit_transpCorr = new std::vector<float>; ecalRechit_transpCorr->clear();

  llpTree->Branch("ecalRechit_Eta", "std::vector<float>",&ecalRechit_Eta);
  llpTree->Branch("ecalRechit_Phi", "std::vector<float>",&ecalRechit_Phi);
  llpTree->Branch("ecalRechit_X", "std::vector<float>",&ecalRechit_X);
  llpTree->Branch("ecalRechit_Y", "std::vector<float>",&ecalRechit_Y);
  llpTree->Branch("ecalRechit_Z", "std::vector<float>",&ecalRechit_Z);
  llpTree->Branch("ecalRechit_E", "std::vector<float>",&ecalRechit_E);
  llpTree->Branch("ecalRechit_T", "std::vector<float>",&ecalRechit_T);
  llpTree->Branch("ecalRechit_ID", "std::vector<uint>",&ecalRechit_ID);
  llpTree->Branch("ecalRechit_FlagOOT", "std::vector<bool>",&ecalRechit_FlagOOT);
  llpTree->Branch("ecalRechit_GainSwitch1", "std::vector<bool>",&ecalRechit_GainSwitch1);
  llpTree->Branch("ecalRechit_GainSwitch6", "std::vector<bool>",&ecalRechit_GainSwitch6);
  llpTree->Branch("ecalRechit_transpCorr", "std::vector<float>",&ecalRechit_transpCorr);

};

void llp_ntupler::enableJetBranches()
{
  llpTree->Branch("nJets", &nJets,"nJets/I");
  llpTree->Branch("jetE", jetE,"jetE[nJets]/F");
  llpTree->Branch("jetPt", jetPt,"jetPt[nJets]/F");
  llpTree->Branch("jetEta", jetEta,"jetEta[nJets]/F");
  llpTree->Branch("jetPhi", jetPhi,"jetPhi[nJets]/F");
  llpTree->Branch("jetCSV", jetCSV,"jetCSV[nJets]/F");
  llpTree->Branch("jetCISV", jetCISV,"jetCISV[nJets]/F");
  llpTree->Branch("jetProbb", jetProbb,"jetProbb[nJets]/F");
  llpTree->Branch("jetProbc", jetProbc,"jetProbc[nJets]/F");
  llpTree->Branch("jetProbudsg", jetProbudsg,"jetProbudsg[nJets]/F");
  llpTree->Branch("jetProbbb", jetProbbb,"jetProbbb[nJets]/F");
  llpTree->Branch("jetMass", jetMass, "jetMass[nJets]/F");
  llpTree->Branch("jetJetArea", jetJetArea, "jetJetArea[nJets]/F");
  llpTree->Branch("jetPileupE", jetPileupE, "jetPileupE[nJets]/F");
  llpTree->Branch("jetPileupId", jetPileupId, "jetPileupId[nJets]/F");
  llpTree->Branch("jetPileupIdFlag", jetPileupIdFlag, "jetPileupIdFlag[nJets]/I");
  llpTree->Branch("jetPassIDLoose", jetPassIDLoose, "jetPassIDLoose[nJets]/O");
  llpTree->Branch("jetPassIDTight", jetPassIDTight, "jetPassIDTight[nJets]/O");
  llpTree->Branch("jetPassMuFrac", jetPassMuFrac, "jetPassMuFrac[nJets]/O");
  llpTree->Branch("jetPassEleFrac", jetPassEleFrac, "jetPassEleFrac[nJets]/O");
  llpTree->Branch("jetPartonFlavor", jetPartonFlavor, "jetPartonFlavor[nJets]/I");
  llpTree->Branch("jetHadronFlavor", jetHadronFlavor, "jetHadronFlavor[nJets]/I");
  llpTree->Branch("jetChargedEMEnergyFraction", jetChargedEMEnergyFraction, "jetChargedEMEnergyFraction[nJets]/F");
  llpTree->Branch("jetNeutralEMEnergyFraction", jetNeutralEMEnergyFraction, "jetNeutralEMEnergyFraction[nJets]/F");
  llpTree->Branch("jetChargedHadronEnergyFraction", jetChargedHadronEnergyFraction, "jetChargedHadronEnergyFraction[nJets]/F");
  llpTree->Branch("jetNeutralHadronEnergyFraction", jetNeutralHadronEnergyFraction, "jetNeutralHadronEnergyFraction[nJets]/F");
  llpTree->Branch("jetMuonEnergyFraction", jetMuonEnergyFraction, "jetMuonEnergyFraction[nJets]/F");
  llpTree->Branch("jetHOEnergyFraction", jetHOEnergyFraction, "jetHOEnergyFraction[nJets]/F");
  llpTree->Branch("jetHFHadronEnergyFraction", jetHFHadronEnergyFraction, "jetHFHadronEnergyFraction[nJets]/F");
  llpTree->Branch("jetHFEMEnergyFraction",jetHFEMEnergyFraction, "jetHFEMEnergyFraction[nJets]/F");
  llpTree->Branch("jetAllMuonPt", jetAllMuonPt,"jetAllMuonPt[nJets]/F");
  llpTree->Branch("jetAllMuonEta", jetAllMuonEta,"jetAllMuonEta[nJets]/F");
  llpTree->Branch("jetAllMuonPhi", jetAllMuonPhi,"jetAllMuonPhi[nJets]/F");
  llpTree->Branch("jetAllMuonM", jetAllMuonM,"jetAllMuonM[nJets]/F");
  llpTree->Branch("jetPtWeightedDZ", jetPtWeightedDZ,"jetPtWeightedDZ[nJets]/F");
  llpTree->Branch("jetNRechits", jetNRechits,"jetNRechits[nJets]/I");
  llpTree->Branch("jetRechitE", jetRechitE,"jetRechitE[nJets]/F");
  llpTree->Branch("jetRechitT", jetRechitT,"jetRechitT[nJets]/F");

};

void llp_ntupler::enableJetAK8Branches()
{
  llpTree->Branch("nFatJets", &nFatJets,"nFatJets/i");
  llpTree->Branch("fatJetE", fatJetE,"fatJetE[nFatJets]/F");
  llpTree->Branch("fatJetPt", fatJetPt,"fatJetPt[nFatJets]/F");
  llpTree->Branch("fatJetEta", fatJetEta,"fatJetEta[nFatJets]/F");
  llpTree->Branch("fatJetPhi", fatJetPhi,"fatJetPhi[nFatJets]/F");
  llpTree->Branch("fatJetCorrectedPt", fatJetCorrectedPt,"fatJetCorrectedPt[nFatJets]/F");
  // llpTree->Branch("fatJetCorrectedEta", fatJetCorrectedEta,"fatJetCorrectedEta[nFatJets]/F");
  // llpTree->Branch("fatJetCorrectedPhi", fatJetCorrectedPhi,"fatJetCorrectedPhi[nFatJets]/F");
  llpTree->Branch("fatJetPrunedM", fatJetPrunedM,"fatJetPrunedM[nFatJets]/F");
  llpTree->Branch("fatJetTrimmedM", fatJetTrimmedM,"fatJetTrimmedM[nFatJets]/F");
  llpTree->Branch("fatJetFilteredM", fatJetFilteredM,"fatJetFilteredM[nFatJets]/F");
  llpTree->Branch("fatJetSoftDropM", fatJetSoftDropM,"fatJetSoftDropM[nFatJets]/F");
  llpTree->Branch("fatJetCorrectedSoftDropM", fatJetCorrectedSoftDropM,"fatJetCorrectedSoftDropM[nFatJets]/F");
  llpTree->Branch("fatJetUncorrectedSoftDropM", fatJetUncorrectedSoftDropM,"fatJetUncorrectedSoftDropM[nFatJets]/F");
  llpTree->Branch("fatJetTau1", fatJetTau1,"fatJetTau1[nFatJets]/F");
  llpTree->Branch("fatJetTau2", fatJetTau2,"fatJetTau2[nFatJets]/F");
  llpTree->Branch("fatJetTau3", fatJetTau3,"fatJetTau3[nFatJets]/F");
  llpTree->Branch("fatJetMaxSubjetCSV", fatJetMaxSubjetCSV, "fatJetMaxSubjetCSV[nFatJets]/F");
  llpTree->Branch("fatJetPassIDLoose", fatJetPassIDLoose,"fatJetPassIDLoose[nFatJets]/O");
  llpTree->Branch("fatJetPassIDTight", fatJetPassIDTight,"fatJetPassIDTight[nFatJets]/O");
};

void llp_ntupler::enableMetBranches()
{
  llpTree->Branch("metPt", &metPt, "metPt/F");
  llpTree->Branch("metPhi", &metPhi, "metPhi/F");
  llpTree->Branch("sumMET", &sumMET, "sumMET/F");
  llpTree->Branch("metType0Pt", &metType0Pt, "metType0Pt/F");
  llpTree->Branch("metType0Phi", &metType0Phi, "metType0Phi/F");
  llpTree->Branch("metType1Pt_raw", &metType1Pt_raw, "metType1Pt_raw/F");
  llpTree->Branch("metType1Pt", &metType1Pt, "metType1Pt/F");
  llpTree->Branch("metType1Px", &metType1Px, "metType1Px/F");
  llpTree->Branch("metType1Py", &metType1Py, "metType1Py/F");
  llpTree->Branch("metType1Eta", &metType1Eta, "metType1Eta/F");
  llpTree->Branch("metType1Phi", &metType1Phi, "metType1Phi/F");
  llpTree->Branch("metType1Phi_raw", &metType1Phi_raw, "metType1Phi_raw/F");
  llpTree->Branch("metType0Plus1Pt", &metType0Plus1Pt, "metType0Plus1Pt/F");
  llpTree->Branch("metType0Plus1Phi", &metType0Plus1Phi, "metType0Plus1Phi/F");
  llpTree->Branch("metNoHFPt", &metNoHFPt, "metNoHFPt/F");
  llpTree->Branch("metNoHFPhi", &metNoHFPhi, "metNoHFPhi/F");
  llpTree->Branch("metPuppiPt", &metPuppiPt, "metPuppiPt/F");
  llpTree->Branch("metPuppiPhi", &metPuppiPhi, "metPuppiPhi/F");
  llpTree->Branch("metCaloPt", &metCaloPt, "metCaloPt/F");
  llpTree->Branch("metCaloPhi", &metCaloPhi, "metCaloPhi/F");

  llpTree->Branch("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter, "Flag_HBHENoiseFilter/O");
  llpTree->Branch("Flag_HBHETightNoiseFilter", &Flag_HBHETightNoiseFilter, "Flag_HBHETightNoiseFilter/O");
  llpTree->Branch("Flag_HBHEIsoNoiseFilter", &Flag_HBHEIsoNoiseFilter, "Flag_HBHEIsoNoiseFilter/O");
  llpTree->Branch("Flag_badChargedCandidateFilter", &Flag_badChargedCandidateFilter, "Flag_badChargedCandidateFilter/O");
  llpTree->Branch("Flag_badMuonFilter", &Flag_badMuonFilter, "Flag_badMuonFilter/O");
  llpTree->Branch("Flag_badGlobalMuonFilter", &Flag_badGlobalMuonFilter, "Flag_badGlobalMuonFilter/O");
  llpTree->Branch("Flag_duplicateMuonFilter", &Flag_duplicateMuonFilter, "Flag_duplicateMuonFilter/O");
  llpTree->Branch("Flag_CSCTightHaloFilter", &Flag_CSCTightHaloFilter, "Flag_CSCTightHaloFilter/O");
  llpTree->Branch("Flag_hcalLaserEventFilter", &Flag_hcalLaserEventFilter, "Flag_hcalLaserEventFilter/O");
  llpTree->Branch("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter, "Flag_EcalDeadCellTriggerPrimitiveFilter/O");
  llpTree->Branch("Flag_EcalDeadCellBoundaryEnergyFilter", &Flag_EcalDeadCellBoundaryEnergyFilter, "Flag_EcalDeadCellBoundaryEnergyFilter/O");
  llpTree->Branch("Flag_goodVertices", &Flag_goodVertices, "Flag_goodVertices/O");
  llpTree->Branch("Flag_trackingFailureFilter", &Flag_trackingFailureFilter, "Flag_trackingFailureFilter/O");
  llpTree->Branch("Flag_eeBadScFilter", &Flag_eeBadScFilter, "Flag_eeBadScFilter/O");
  llpTree->Branch("Flag_ecalLaserCorrFilter", &Flag_ecalLaserCorrFilter, "Flag_ecalLaserCorrFilter/O");
  llpTree->Branch("Flag_trkPOGFilters", &Flag_trkPOGFilters, "Flag_trkPOGFilters/O");
  llpTree->Branch("Flag_trkPOG_manystripclus53X", &Flag_trkPOG_manystripclus53X, "Flag_trkPOG_manystripclus53X/O");
  llpTree->Branch("Flag_trkPOG_toomanystripclus53X", &Flag_trkPOG_toomanystripclus53X, "Flag_trkPOG_toomanystripclus53X/O");
  llpTree->Branch("Flag_trkPOG_logErrorTooManyClusters", &Flag_trkPOG_logErrorTooManyClusters, "Flag_trkPOG_logErrorTooManyClusters/O");
  llpTree->Branch("Flag_BadPFMuonFilter", &Flag_BadPFMuonFilter, "Flag_BadPFMuonFilter/O");
  llpTree->Branch("Flag_BadChargedCandidateFilter", &Flag_BadChargedCandidateFilter, "Flag_BadChargedCandidateFilter/O");
  llpTree->Branch("Flag_ecalBadCalibFilter", &Flag_ecalBadCalibFilter, "Flag_ecalBadCalibFilter/O");
  llpTree->Branch("Flag_METFilters", &Flag_METFilters, "Flag_METFilters/O");
};

void llp_ntupler::enableTriggerBranches()
{
  nameHLT = new std::vector<std::string>; nameHLT->clear();
  llpTree->Branch("HLTDecision", &triggerDecision, ("HLTDecision[" + std::to_string(NTriggersMAX) +  "]/O").c_str());
  llpTree->Branch("HLTPrescale", &triggerHLTPrescale, ("HLTPrescale[" + std::to_string(NTriggersMAX) +  "]/I").c_str());
  //llpTree->Branch("HLTMR", &HLTMR, "HLTMR/F");
  //llpTree->Branch("HLTRSQ", &HLTRSQ, "HLTRSQ/F");
};

void llp_ntupler::enableMCBranches()
{
  llpTree->Branch("nGenJets", &nGenJets, "nGenJets/I");
  llpTree->Branch("genJetE", genJetE, "genJetE[nGenJets]/F");
  llpTree->Branch("genJetPt", genJetPt, "genJetPt[nGenJets]/F");
  llpTree->Branch("genJetEta", genJetEta, "genJetEta[nGenJets]/F");
  llpTree->Branch("genJetPhi", genJetPhi, "genJetPhi[nGenJets]/F");
  llpTree->Branch("genMetPtCalo", &genMetPtCalo, "genMetPtCalo/F");
  llpTree->Branch("genMetPhiCalo", &genMetPhiCalo, "genMetPhiCalo/F");
  llpTree->Branch("genMetPtTrue", &genMetPtTrue, "genMetPtTrue/F");
  llpTree->Branch("genMetPhiTrue", &genMetPhiTrue, "genMetPhiTrue/F");
  llpTree->Branch("genVertexX", &genVertexX, "genVertexX/F");
  llpTree->Branch("genVertexY", &genVertexY, "genVertexY/F");
  llpTree->Branch("genVertexZ", &genVertexZ, "genVertexZ/F");
  llpTree->Branch("genVertexT", &genVertexT, "genVertexT/F");
  llpTree->Branch("genWeight", &genWeight, "genWeight/F");
  llpTree->Branch("genSignalProcessID", &genSignalProcessID, "genSignalProcessID/i");
  llpTree->Branch("genQScale", &genQScale, "genQScale/F");
  llpTree->Branch("genAlphaQCD", &genAlphaQCD, "genAlphaQCD/F");
  llpTree->Branch("genAlphaQED", &genAlphaQED, "genAlphaQED/F");
  scaleWeights = new std::vector<float>; scaleWeights->clear();
  pdfWeights = new std::vector<float>; pdfWeights->clear();
  alphasWeights = new std::vector<float>; alphasWeights->clear();
  if (isFastsim_) {
    llpTree->Branch("lheComments", "std::string",&lheComments);
  }
  llpTree->Branch("scaleWeights", "std::vector<float>",&scaleWeights);
  llpTree->Branch("pdfWeights", "std::vector<float>",&pdfWeights);
  llpTree->Branch("alphasWeights", "std::vector<float>",&alphasWeights);
};

void llp_ntupler::enableGenParticleBranches()
{
  llpTree->Branch("nGenParticle", &nGenParticle, "nGenParticle/I");
  llpTree->Branch("gParticleMotherId", gParticleMotherId, "gParticleMotherId[nGenParticle]/I");
  llpTree->Branch("gParticleMotherIndex", gParticleMotherIndex, "gParticleMotherIndex[nGenParticle]/I");
  llpTree->Branch("gParticleId", gParticleId, "gParticleId[nGenParticle]/I");
  llpTree->Branch("gParticleStatus", gParticleStatus, "gParticleStatus[nGenParticle]/I");
  llpTree->Branch("gParticleE", gParticleE, "gParticleE[nGenParticle]/F");
  llpTree->Branch("gParticlePt", gParticlePt, "gParticlePt[nGenParticle]/F");
  llpTree->Branch("gParticlePx", gParticlePx, "gParticlePx[nGenParticle]/F");
  llpTree->Branch("gParticlePy", gParticlePy, "gParticlePy[nGenParticle]/F");
  llpTree->Branch("gParticlePz", gParticlePz, "gParticlePz[nGenParticle]/F");
  llpTree->Branch("gParticleEta", gParticleEta, "gParticleEta[nGenParticle]/F");
  llpTree->Branch("gParticlePhi", gParticlePhi, "gParticlePhi[nGenParticle]/F");
  llpTree->Branch("gParticleProdVertexX", gParticleProdVertexX, "gParticleProdVertexX[nGenParticle]/F");
  llpTree->Branch("gParticleProdVertexY", gParticleProdVertexY, "gParticleProdVertexY[nGenParticle]/F");
  llpTree->Branch("gParticleProdVertexZ", gParticleProdVertexZ, "gParticleProdVertexZ[nGenParticle]/F");
};

//------ Load the miniAOD objects and reset tree variables for each event ------//
void llp_ntupler::loadEvent(const edm::Event& iEvent)//load all miniAOD objects for the current event
{
  iEvent.getByToken(triggerBitsToken_, triggerBits);
  iEvent.getByToken(hepMCToken_, hepMC);
  iEvent.getByToken(triggerBitsToken_, triggerBits);
  iEvent.getByToken(metFilterBitsToken_, metFilterBits);
  iEvent.getByToken(verticesToken_, vertices);
  iEvent.getByToken(cscSegmentInputToken_,cscSegments);
  iEvent.getByToken(dtSegmentInputToken_,dtSegments);
  iEvent.getByToken(rpcRecHitInputToken_,rpcRecHits);
  iEvent.getByToken(tracksTag_,tracks);
  iEvent.getByToken(PFCandsToken_, pfCands);
  iEvent.getByToken(PFClustersToken_, pfClusters);
  iEvent.getByToken(muonsToken_, muons);
  iEvent.getByToken(electronsToken_, electrons);
  iEvent.getByToken(photonsToken_, photons);
  iEvent.getByToken(tausToken_, taus);
  iEvent.getByToken(jetsToken_, jets);
  iEvent.getByToken(jetsPuppiToken_, jetsPuppi);
  iEvent.getByToken(jetsAK8Token_, jetsAK8);
  iEvent.getByToken(genMetCaloToken_, genMetsCalo);
  iEvent.getByToken(genMetTrueToken_, genMetsTrue);
  iEvent.getByToken(metToken_, mets);
  //iEvent.getByToken(metNoHFToken_, metsNoHF);
  iEvent.getByToken(metPuppiToken_, metsPuppi);
//  iEvent.getByToken(hcalNoiseInfoToken_,hcalNoiseInfo);
  iEvent.getByToken(secondaryVerticesToken_,secondaryVertices);
  iEvent.getByToken(rhoAllToken_,rhoAll);
  iEvent.getByToken(rhoFastjetAllToken_,rhoFastjetAll);
  iEvent.getByToken(rhoFastjetAllCaloToken_,rhoFastjetAllCalo);
  iEvent.getByToken(rhoFastjetCentralCaloToken_,rhoFastjetCentralCalo);
  iEvent.getByToken(rhoFastjetCentralChargedPileUpToken_,rhoFastjetCentralChargedPileUp);
  iEvent.getByToken(rhoFastjetCentralNeutralToken_,rhoFastjetCentralNeutral);
  iEvent.getByToken(beamSpotToken_,beamSpot);
  iEvent.getByToken(ebRecHitsToken_,ebRecHits);
  iEvent.getByToken(eeRecHitsToken_,eeRecHits);
  iEvent.getByToken(esRecHitsToken_,esRecHits);
  iEvent.getByToken(ebeeClustersToken_,ebeeClusters);
  iEvent.getByToken(esClustersToken_,esClusters);
  iEvent.getByToken(conversionsToken_,conversions);
  iEvent.getByToken(singleLegConversionsToken_,singleLegConversions);
  iEvent.getByToken(gedGsfElectronCoresToken_,gedGsfElectronCores);
  iEvent.getByToken(gedPhotonCoresToken_, gedPhotonCores);
//  iEvent.getByToken(superClustersToken_,superClusters);
//  iEvent.getByToken(lostTracksToken_,lostTracks);
//  iEvent.getByToken(hbheNoiseFilterToken_, hbheNoiseFilter);
//  iEvent.getByToken(hbheTightNoiseFilterToken_, hbheTightNoiseFilter);
//  iEvent.getByToken(hbheIsoNoiseFilterToken_, hbheIsoNoiseFilter);
  //iEvent.getByToken(badChargedCandidateFilterToken_, badChargedCandidateFilter);
  //iEvent.getByToken(badMuonFilterToken_, badMuonFilter);
  if(readGenVertexTime_) iEvent.getByToken(genParticles_t0_Token_,genParticles_t0);
  if (useGen_) {
//    iEvent.getByToken(genParticlesToken_,genParticles);
    iEvent.getByToken(genParticlesToken_,genParticles);
    iEvent.getByToken(genJetsToken_,genJets);

    //for Spring16 fastsim, this has been changed and removed
//    if (!isFastsim_) iEvent.getByToken(lheInfoToken_, lheInfo);

    iEvent.getByToken(genInfoToken_,genInfo);
    iEvent.getByToken(puInfoToken_,puInfo);
  }


}

//called by the loadEvent() method
void llp_ntupler::resetBranches()
{
    //reset tree variables
    resetEventInfoBranches();
    resetPVAllBranches();
    resetPVTracksBranches();
    resetPileUpBranches();
    resetMuonBranches();
    resetElectronBranches();
    resetTauBranches();
    resetPhotonBranches();
    resetJetBranches();
    resetMuonSystemBranches();
    resetMetBranches();
    resetGenParticleBranches();
    resetMCBranches();
    resetTriggerBranches();
};

void llp_ntupler::resetEventInfoBranches()
{
  //Event
  nPV = -1;
  eventNum = 0;
  eventTime = 0;
  lumiNum = 0;
  runNum = 0;
  nSlimmedSecondV = 0;
  pvX = -999.0;
  pvY = -999.0;
  pvZ = -999.0;
  fixedGridRhoAll = -999.0;
  fixedGridRhoFastjetAll = -999.0;
  fixedGridRhoFastjetAllCalo = -999.0;
  fixedGridRhoFastjetCentralCalo = -999.0;
  fixedGridRhoFastjetCentralChargedPileUp = -999.0;
  fixedGridRhoFastjetCentralNeutral = -999.0;
  return;
};

void llp_ntupler::resetPVAllBranches()
{
  nPVAll = 0;
  for(int i = 0; i < MAX_NPV; i++)
  {
    pvAllX[i] = -999.;
    pvAllY[i] = -999.;
    pvAllZ[i] = -999.;
    pvAllLogSumPtSq[i] = -999.;
    pvAllSumPx[i] = -999.;
    pvAllSumPy[i] = -999.;
  }
};

void llp_ntupler::resetPVTracksBranches()
{
  nPVTracks = 0;
  for(int i = 0; i < OBJECTARRAYSIZE; i++)
  {
    pvTrackPt[i]  = -999.;
    pvTrackEta[i] = -999.;
    pvTrackPhi[i] = -999.;
  }
};

void llp_ntupler::resetPileUpBranches()
{
  nBunchXing = 0;
  for(int i = 0; i < MAX_NBX; i++)
  {
    BunchXing[i] = -999;
    nPU[i]       = -999;
    nPUmean[i]   = -999.;
  }
};

void llp_ntupler::resetMuonBranches()
{
  nMuons = 0;
  for(int i = 0; i < OBJECTARRAYSIZE; i++)
  {
    muonE[i] = 0.0;
    muonPt[i] = 0.0;
    muonEta[i] = 0.0;
    muonPhi[i] = 0.0;
    muonCharge[i] = -99;
    muonIsLoose[i] = false;
    muonIsMedium[i] = false;
    muonIsTight[i] = false;
    muon_d0[i] = -99.0;
    muon_dZ[i] = -99.0;
    muon_ip3d[i] = -99.0;
    muon_ip3dSignificance[i] = -99.0;
    muonType[i] = 0;
    muonQuality[i] = 0;
    muon_pileupIso[i] = -99.0;
    muon_chargedIso[i] = -99.0;
    muon_photonIso[i] = -99.0;
    muon_neutralHadIso[i] = -99.0;
    muon_ptrel[i] = -99.0;
    muon_chargedMiniIso[i] = -99.0;
    muon_photonAndNeutralHadronMiniIso[i] = -99.0;
    muon_chargedPileupMiniIso[i] = -99.0;
    muon_activityMiniIsoAnnulus[i] = -99.0;
    muon_passSingleMuTagFilter[i] = false;
    for (int q=0;q<MAX_MuonHLTFilters;q++) muon_passHLTFilter[i][q] = false;
    muon_validFractionTrackerHits[i] = -99.0;
    muon_isGlobal[i] = false;
    muon_normChi2[i] = -99.0;
    muon_chi2LocalPosition[i] = -99.0;
    muon_kinkFinder[i] = -99.0;
    muon_segmentCompatability[i] = -99.0;
    muonIsICHEPMedium[i] = false;
  }
};

void llp_ntupler::resetElectronBranches()
{
  nElectrons = 0;
  for(int i = 0; i < OBJECTARRAYSIZE; i++)
  {
    eleE[i] = 0.0;
    elePt[i] = 0.0;
    eleEta[i] = 0.0;
    elePhi[i] = 0.0;
    eleE_SC[i] = -99.0;
    eleEta_SC[i] = -99.0;
    elePhi_SC[i] = -99.0;
    eleSigmaIetaIeta[i] = -99.0;
    eleFull5x5SigmaIetaIeta[i] = -99.0;
    eleR9[i] = -99;
    ele_dEta[i] = -99;
    ele_dPhi[i] = -99;
    ele_HoverE[i] = -99;
    ele_d0[i] = -99;
    ele_dZ[i] = -99;
    ele_ip3d[i] = -99;
    ele_ip3dSignificance[i] = -99;
    ele_pileupIso[i] = -99.0;
    ele_chargedIso[i] = -99.0;
    ele_photonIso[i] = -99.0;
    ele_neutralHadIso[i] = -99.0;
    ele_MissHits[i] = -99;
    ele_PassConvVeto[i] = false;
    ele_OneOverEminusOneOverP[i] = -99.0;
    ele_IDMVAGeneralPurpose[i] = -99.0;
    ele_IDMVACategoryGeneralPurpose[i] = -1;
    ele_IDMVAHZZ[i] = -99.0;
    ele_IDMVACategoryHZZ[i] = -1;
    ele_RegressionE[i] = -99.0;
    ele_CombineP4[i] = -99.0;
    ele_ptrel[i] = -99.0;
    ele_chargedMiniIso[i] = -99.0;
    ele_photonAndNeutralHadronMiniIso[i] = -99.0;
    ele_chargedPileupMiniIso[i] = -99.0;
    ele_activityMiniIsoAnnulus[i] = -99.0;
    ele_passSingleEleTagFilter[i] = false;
    ele_passTPOneTagFilter[i] = false;
    ele_passTPTwoTagFilter[i] = false;
    ele_passTPOneProbeFilter[i] = false;
    ele_passTPTwoProbeFilter[i] = false;
    for (int q=0;q<MAX_ElectronHLTFilters;q++) ele_passHLTFilter[i][q] = false;
  }

};

void llp_ntupler::resetTauBranches()
{
  nTaus = 0;
  for(int i = 0; i < OBJECTARRAYSIZE; i++)
  {
    tauE[i] = -999.;
    tauPt[i] = -999.;
    tauEta[i] = -999.;
    tauPhi[i] = -999.;
    tau_IsLoose[i] = false;
    tau_IsMedium[i] = false;
    tau_IsTight[i] = false;
    tau_passEleVetoLoose[i] = false;
    tau_passEleVetoMedium[i] = false;
    tau_passEleVetoTight[i] = false;
    tau_passMuVetoLoose[i] = false;
    tau_passMuVetoMedium[i] = false;
    tau_passMuVetoTight[i] = false;
    tau_ID[i] = 0;//tauID Bits
    tau_combinedIsoDeltaBetaCorr3Hits[i] = -999.;;
    tau_chargedIsoPtSum[i] = -999.;;
    tau_neutralIsoPtSum[i] = -999.;;
    tau_puCorrPtSum[i] = -999.;;
    tau_eleVetoMVA[i] = -999.;;
    tau_eleVetoCategory[i] = -999.;;
    tau_muonVetoMVA[i] = -999.;;
    tau_isoMVAnewDMwLT[i] = -999.;;
    tau_isoMVAnewDMwoLT[i] = -999.;;
    tau_leadCandPt[i] = -999.;;
    tau_leadCandID[i] = -999.;;
    tau_leadChargedHadrCandPt[i] = -999.;;
    tau_leadChargedHadrCandID[i] = -999.;;
  }
};

void llp_ntupler::resetPhotonBranches()
{
  nPhotons = 0;
  nPhotons_overlap = 0;
  //Photon
  for(int i = 0; i < OBJECTARRAYSIZE; i++)
  {
    phoE[i] = 0.0;
    phoPt[i] = 0.0;
    phoEta[i] = 0.0;
    phoPhi[i] = 0.0;
    phoSigmaIetaIeta[i] = -99.0;
    phoFull5x5SigmaIetaIeta[i] = -99.0;
    phoR9[i] = -99.0;
    pho_sminor[i] = -99.0;
    pho_smajor[i] = -99.0;
    pho_HoverE[i] = -99.0;
    pho_sumChargedHadronPt[i] = -99.0;
    pho_sumNeutralHadronEt[i] = -99.0;
    pho_sumPhotonEt[i] = -99.0;
    pho_ecalPFClusterIso[i] = -99.0;
    pho_hcalPFClusterIso[i] = -99.0;
    pho_trkSumPtHollowConeDR03[i] = -99.0;
    pho_sumWorstVertexChargedHadronPt[i] = -99.0;
    pho_pfIsoChargedHadronIso[i] = -99.0;
    pho_pfIsoChargedHadronIsoWrongVtx[i] = -99.0;
    pho_pfIsoNeutralHadronIso[i] = -99.0;
    pho_pfIsoPhotonIso[i] = -99.0;
    pho_pfIsoModFrixione[i] = -99.0;
    pho_pfIsoSumPUPt[i] = -99.0;
    pho_isConversion[i] = false;
    pho_passEleVeto[i] = false;
    pho_RegressionE[i] = -99.0;
    pho_RegressionEUncertainty[i] = -99.0;
    pho_IDMVA[i] = -99.0;
    pho_superClusterEnergy[i] = -99.0;
    pho_superClusterRawEnergy[i] = -99.0;
    pho_superClusterEta[i]    = -99.0;
    pho_superClusterPhi[i]    = -99.0;
    pho_superClusterX[i]      = -99.0;
    pho_superClusterY[i]      = -99.0;
    pho_superClusterZ[i]      = -99.0;
    pho_hasPixelSeed[i] = false;
    for (int q=0;q<MAX_PhotonHLTFilters;q++) pho_passHLTFilter[i][q] = false;
    pho_convType[i] = -99;
    pho_convTrkZ[i] = -99.;
    pho_convTrkClusZ[i] = -99.;

    for (int ipv=0; ipv < MAX_NPV; ++ipv) {
      pho_sumChargedHadronPtAllVertices[i][ipv] = -99.0;
      pho_vtxSumPx[i][ipv] = 0.;
      pho_vtxSumPy[i][ipv] = 0.;
    }
    pho_isStandardPhoton[i] = true;
    pho_seedRecHitSwitchToGain6[i] = false;
    pho_seedRecHitSwitchToGain1[i] = false;
    pho_anyRecHitSwitchToGain6[i] = false;
    pho_anyRecHitSwitchToGain1[i] = false;
  }
  return;
};

void llp_ntupler::resetMuonSystemBranches()
{
    nCsc = 0;
    for ( int i = 0; i < OBJECTARRAYSIZE; i++)
    {
      cscPhi[i] = 0.0;
      cscEta[i] = 0.0;
      cscX[i] = 0.0;
      cscY[i] = 0.0;
      cscZ[i] = 0.0;
      cscNRecHits[i] = 0.0;
      cscT[i] = 0.0;
      cscChi2[i] = 0.0;
    }
    nRpc = 0;
    for ( int i = 0; i < OBJECTARRAYSIZE; i++)
    {
      rpcPhi[i] = 0.0;
      rpcEta[i] = 0.0;
      rpcX[i] = 0.0;
      rpcY[i] = 0.0;
      rpcZ[i] = 0.0;
      rpcT[i] = 0.0;
      rpcTError[i] = 0.0;
    }
    nDt = 0;
    for ( int i = 0; i < OBJECTARRAYSIZE; i++)
    {
      dtPhi[i] = 0.0;
      dtEta[i] = 0.0;
      dtX[i] = 0.0;
      dtY[i] = 0.0;
      dtZ[i] = 0.0;
      dtDirX[i] = 0.0;
      dtDirY[i] = 0.0;
      dtDirZ[i] = 0.0;
      dtT[i] = 0.0;
      dtTError[i] = 0.0;
    }
    return;
};
void llp_ntupler::resetJetBranches()
{
  nJets = 0;
  for ( int i = 0; i < OBJECTARRAYSIZE; i++)
  {
    jetE[i] = 0.0;
    jetPt[i] = 0.0;
    jetEta[i] = 0.0;
    jetPhi[i] = 0.0;
    jetCSV[i] = 0.0;
    jetCISV[i] = 0.0;
    jetMass[i] =  -99.0;
    jetJetArea[i] = -99.0;
    jetPileupE[i] = -99.0;
    jetPileupId[i] = -99.0;
    jetPileupIdFlag[i] = -1;
    jetPassIDLoose[i] = false;
    jetPassIDTight[i] = false;
    jetPassMuFrac[i] = false;
    jetPassEleFrac[i] = false;
    jetPartonFlavor[i] = 0;
    jetHadronFlavor[i] = 0;
    jetChargedEMEnergyFraction[i] = -99.0;
    jetNeutralEMEnergyFraction[i] = -99.0;
    jetChargedHadronEnergyFraction[i] = -99.0;
    jetNeutralHadronEnergyFraction[i] = -99.0;
    jetMuonEnergyFraction[i] = -99.0;
    jetHOEnergyFraction[i] = -99.0;
    jetHFHadronEnergyFraction[i] = -99.0;
    jetHFEMEnergyFraction[i] = -99.0;
    jetAllMuonPt[i] = 0.0;
    jetAllMuonEta[i] = 0.0;
    jetAllMuonPhi[i] = 0.0;
    jetAllMuonM[i] = 0.0;
    jetPtWeightedDZ[i] = 0.0;
    jetNRechits[i] = 0;
    jetRechitE[i] = 0.0;
    jetRechitT[i] = 0.0;
  }
  return;
};

void llp_ntupler::resetMetBranches()
{
  metPt = -999;
  metPhi = -999;
  sumMET = -99.0;
  UncMETdpx = -99.0;
  UncMETdpy = -99.0;
  UncMETdSumEt = -99.0;
  metType0Pt = -99.0;
  metType0Phi = -99.0;
  metType1Pt_raw = -99.0;
  metType1Pt = -99.0;
  metType1Px = -99.0;
  metType1Py = -99.0;
  metType1Eta = -99.0;
  metType1Phi = -99.0;
  metType1Phi_raw = -99.0;
  metType0Plus1Pt = -99.0;
  metType0Plus1Phi = -99.0;
  metPtRecomputed = -99.0;
  metPhiRecomputed = -99.0;
  metNoHFPt = -99.0;
  metNoHFPhi = -99.0;
  metPuppiPt = -99.0;
  metPuppiPhi = -99.0;
  metCaloPt = -999;
  metCaloPhi = -999;
  Flag_HBHENoiseFilter = false;
  Flag_HBHETightNoiseFilter = false;
  Flag_HBHEIsoNoiseFilter = false;
  Flag_badChargedCandidateFilter = false;
  Flag_badMuonFilter = false;
  Flag_badGlobalMuonFilter = false;
  Flag_duplicateMuonFilter = false;
  Flag_CSCTightHaloFilter = false;
  Flag_hcalLaserEventFilter = false;
  Flag_EcalDeadCellTriggerPrimitiveFilter = false;
  Flag_EcalDeadCellBoundaryEnergyFilter = false;
  Flag_goodVertices = false;
  Flag_trackingFailureFilter = false;
  Flag_eeBadScFilter = false;
  Flag_ecalLaserCorrFilter = false;
  Flag_trkPOGFilters = false;
  Flag_trkPOG_manystripclus53X = false;
  Flag_trkPOG_toomanystripclus53X = false;
  Flag_trkPOG_logErrorTooManyClusters = false;
  Flag_BadPFMuonFilter = false;
  Flag_BadChargedCandidateFilter = false;
  Flag_ecalBadCalibFilter = false;
  Flag_METFilters = false;

  metType1PtJetResUp=-999.;
  metType1PtJetResDown=-999.;
  metType1PtJetEnUp=-999.;
  metType1PtJetEnDown=-999.;
  metType1PtMuonEnUp=-999.;
  metType1PtMuonEnDown=-999.;
  metType1PtElectronEnUp=-999.;
  metType1PtElectronEnDown=-999.;
  metType1PtTauEnUp=-999.;
  metType1PtTauEnDown=-999.;
  metType1PtUnclusteredEnUp=-999.;
  metType1PtUnclusteredEnDown=-999.;
  metType1PtPhotonEnUp=-999.;
  metType1PtPhotonEnDown=-999.;
  metType1PtMETUncertaintySize=-999.;
  metType1PtJetResUpSmear=-999.;
  metType1PtJetResDownSmear=-999.;
  metType1PtMETFullUncertaintySize=-999.;

  metType1PhiJetResUp=-999.;
  metType1PhiJetResDown=-999.;
  metType1PhiJetEnUp=-999.;
  metType1PhiJetEnDown=-999.;
  metType1PhiMuonEnUp=-999.;
  metType1PhiMuonEnDown=-999.;
  metType1PhiElectronEnUp=-999.;
  metType1PhiElectronEnDown=-999.;
  metType1PhiTauEnUp=-999.;
  metType1PhiTauEnDown=-999.;
  metType1PhiUnclusteredEnUp=-999.;
  metType1PhiUnclusteredEnDown=-999.;
  metType1PhiPhotonEnUp=-999.;
  metType1PhiPhotonEnDown=-999.;
  metType1PhiMETUncertaintySize=-999.;
  metType1PhiJetResUpSmear=-999.;
  metType1PhiJetResDownSmear=-999.;
  metType1PhiMETFullUncertaintySize=-999.;
};
void llp_ntupler::resetGenParticleBranches()
{
  for(int i = 0; i < GENPARTICLEARRAYSIZE; i++)
  {
    gParticleMotherId[i] = -99999;
    gParticleMotherIndex[i] = -99999;
    gParticleId[i] = -99999;
    gParticleStatus[i] = -99999;
    gParticleE[i] = -99999.0;
    gParticlePt[i] = -99999.0;
    gParticlePx[i] = -99999.0;
    gParticlePy[i] = -99999.0;
    gParticlePz[i] = -99999.0;
    gParticleEta[i] = -99999.0;
    gParticlePhi[i] = -99999.0;

    gParticleProdVertexX[i] = -99999.0;
    gParticleProdVertexY[i] = -99999.0;
    gParticleProdVertexZ[i] = -99999.0;

  }
  return;
};

void llp_ntupler::resetMCBranches()
{
  nGenJets = 0;
  for ( int i = 0; i < OBJECTARRAYSIZE; i++ )
  {
    genJetE[i] = -999.;
    genJetPt[i] = -999.;
    genJetEta[i] = -999.;
    genJetPhi[i] = -999.;
    genJetMET[i] = -999.;
    //genJet_match_jet_index[i] = 666;
    //genJet_min_delta_r_match_jet[i] = -666.;
  }

  genMetPtCalo  = -999.;
  genMetPhiCalo = -999.;
  genMetPtTrue  = -999.;
  genMetPhiTrue = -999.;

  genVertexX = -999.;
  genVertexY = -999.;
  genVertexZ = -999.;
  genVertexT = -999.;

  genWeight = -999.;
  genSignalProcessID = 0;
  genQScale = -999.;
  genAlphaQCD = -999.;
  genAlphaQED = -999.;

  return;
};

void llp_ntupler::resetTriggerBranches()
{
  for( int i = 0; i < NTriggersMAX; i++ )
  {
    triggerDecision[i] = false;
    triggerHLTPrescale[i] = 0;
  }
};
//------ Method called for each run ------//

void llp_ntupler::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {


}


//------ Method called for each lumi block ------//
void llp_ntupler::beginLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const&) {

}


//------ Method called for each event ------//

void llp_ntupler::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  using namespace edm;

  //initialize
  loadEvent(iEvent); //loads objects and resets tree branches
  NEvents->Fill(0); //increment event count

  //resetting output tree branches
  resetBranches();
  cout << "here1\n";
  fillEventInfo(iEvent);
  cout << "here2\n";
  fillPVAll();
  fillPVTracks();
  cout << "here3\n";
  fillMuons(iEvent);
    //cout << "here4\n";
    fillMuonSystem(iEvent, iSetup);
    //cout << "here5\n";
  fillElectrons(iEvent);
  cout << "here6\n";
  fillPhotons(iEvent, iSetup);
  cout << "here7\n";
  fillTaus();
  cout << "here8\n";
  fillJets(iSetup);
  cout << "here9\n";
  fillMet(iEvent);
  cout << "here10\n";
  if ( enableTriggerInfo_ ) fillTrigger( iEvent );
  cout << "here10\n";
  if (!isData) {
    fillPileUp();
    fillMC();
    fillGenParticles();
  }

  cout << "here13\n";
  llpTree->Fill();
};

//------ Method called once each job just before starting event loop ------//
void llp_ntupler::beginJob()
{
  setBranches();
}

//------ Method called once each job just after ending the event loop ------//
void llp_ntupler::endJob(){};



//------ Methods to fill tree variables ------//
bool llp_ntupler::fillEventInfo(const edm::Event& iEvent)
{
  isData = isData_;
  runNum = iEvent.id().run();
  lumiNum = iEvent.luminosityBlock();
  eventNum = iEvent.id().event();
  eventTime = iEvent.eventAuxiliary().time().unixTime();

  //number of slimmedSecondaryVertices
  nSlimmedSecondV = secondaryVertices->size();

  //select the primary vertex, if any
  nPV = 0;
  myPV = &(vertices->front());
  bool foundPV = false;
  for(unsigned int i = 0; i < vertices->size(); i++)
  {
    if(vertices->at(i).isValid() && !vertices->at(i).isFake())
    {
      if (!foundPV)
      {
        myPV = &(vertices->at(i));
        foundPV = true;
      }
      nPV++;
    }
  }

  pvX = myPV->x();
  pvY = myPV->y();
  pvZ = myPV->z();

  //get rho
  fixedGridRhoAll = *rhoAll;
  fixedGridRhoFastjetAll = *rhoFastjetAll;
  fixedGridRhoFastjetAllCalo = *rhoFastjetAllCalo;
  fixedGridRhoFastjetCentralCalo = *rhoFastjetCentralCalo;
  fixedGridRhoFastjetCentralChargedPileUp = *rhoFastjetCentralChargedPileUp;
  fixedGridRhoFastjetCentralNeutral = *rhoFastjetCentralNeutral;

  return true;
};

bool llp_ntupler::fillPVAll()
{
  nPVAll = std::min(int(vertices->size()),int(MAX_NPV));
  for (int ipv = 0; ipv < nPVAll; ++ipv)
  {
    const reco::Vertex &vtx = vertices->at(ipv);
    pvAllX[ipv] = vtx.x();
    pvAllY[ipv] = vtx.y();
    pvAllZ[ipv] = vtx.z();
  }

  double pvAllSumPtSqD[MAX_NPV];
  double pvAllSumPxD[MAX_NPV];
  double pvAllSumPyD[MAX_NPV];

  for (int ipv=0; ipv<nPVAll; ++ipv)
  {
    pvAllSumPtSqD[ipv] = 0.;
    pvAllSumPxD[ipv]   = 0.;
    pvAllSumPyD[ipv]   = 0.;
  }

/*
  int counter = 0;
  for (const pat::PackedCandidate &pfcand : *packedPFCands)
  {
    counter++;
    if (pfcand.charge()==0) continue;
    double mindz = std::numeric_limits<double>::max();
    int ipvmin = -1;
    for (int ipv = 0; ipv < nPVAll; ++ipv) {
      const reco::Vertex &vtx = vertices->at(ipv);
      double dz = std::abs(pfcand.dz(vtx.position()));
      if (dz<mindz) {
        mindz = dz;
        ipvmin = ipv;
      }
    }

    if (mindz<0.2 && ipvmin>=0 && ipvmin<MAX_NPV) {
      pvAllSumPtSqD[ipvmin] += pfcand.pt()*pfcand.pt();
      pvAllSumPxD[ipvmin] += pfcand.px();
      pvAllSumPyD[ipvmin] += pfcand.py();
    }
  }
  */
  for (int ipv=0; ipv<nPVAll; ++ipv) {
    pvAllLogSumPtSq[ipv] = log(pvAllSumPtSqD[ipv]);
    pvAllSumPx[ipv] = pvAllSumPxD[ipv];
    pvAllSumPy[ipv] = pvAllSumPyD[ipv];
  }

  return true;
};

bool llp_ntupler::fillPVTracks()
{
  //select the primary vertex, if any
  //myPV = &(vertices->front());
  //bool foundPV = false;
  for(unsigned int i = 0; i < vertices->size(); i++)
  {
    if(vertices->at(i).isValid() && !vertices->at(i).isFake())
    {
      myPV = &(vertices->at(i));
      for(auto pvTrack = myPV->tracks_begin(); pvTrack != myPV->tracks_end(); pvTrack++)
      {
        if( (*pvTrack)->pt() > 1.0 )
        {
          pvTrackPt[nPVTracks]  = (*pvTrack)->pt();
          pvTrackEta[nPVTracks] = (*pvTrack)->eta();
          pvTrackPhi[nPVTracks] = (*pvTrack)->phi();
          nPVTracks++;
        }
      }
    }
  }

  return true;
};

bool llp_ntupler::fillPileUp()
{
  for(const PileupSummaryInfo &pu : *puInfo)
  {
    BunchXing[nBunchXing] = pu.getBunchCrossing();
    nPU[nBunchXing] = pu.getPU_NumInteractions();
    nPUmean[nBunchXing] = pu.getTrueNumInteractions();
    nBunchXing++;
  }
  return true;
};

bool llp_ntupler::fillMuonSystem(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    edm::ESHandle<CSCGeometry> cscG;
    edm::ESHandle<DTGeometry> dtG;
    edm::ESHandle<RPCGeometry> rpcG;

    iSetup.get<MuonGeometryRecord>().get(cscG); 
    iSetup.get<MuonGeometryRecord>().get(dtG); 
    iSetup.get<MuonGeometryRecord>().get(rpcG); 

    for (const CSCSegment cscSegment : *cscSegments) {
	float globPhi   = 0.;
	float globX = 0.;
	float globY = 0.;
	float globZ = 0.;
	float globEta = 0.;
	CSCDetId id  = (CSCDetId)(cscSegment).cscDetId();
	LocalPoint segPos = (cscSegment).localPosition();
	const CSCChamber* cscchamber = cscG->chamber(id);
	if (cscchamber) {
	    GlobalPoint globalPosition = cscchamber->toGlobal(segPos);
	    globPhi   = globalPosition.phi();
	    globEta   = globalPosition.eta();
	    globX = globalPosition.x();
	    globY = globalPosition.y();
	    globZ = globalPosition.z();
	    // globR = pow(globX*globX+globY*globY,0.5);
	    cscNRecHits[nCsc] = cscSegment.nRecHits();
	    cscX[nCsc] = globX;
	    cscY[nCsc] = globY;
	    cscZ[nCsc] = globZ;
	    cscPhi[nCsc] = globPhi;
	    cscEta[nCsc] = globEta;
	    cscT[nCsc] = cscSegment.time();
	    cscChi2[nCsc] = cscSegment.chi2();
	    nCsc++;
	}
    }
    for (const RPCRecHit rpcRecHit : *rpcRecHits){
	LocalPoint  rpcRecHitLocalPosition       = rpcRecHit.localPosition();
	// LocalError  segmentLocalDirectionError = iDT->localDirectionError();
	DetId geoid = rpcRecHit.geographicalId();
	RPCDetId rpcdetid = RPCDetId(geoid);
	const RPCChamber * rpcchamber = rpcG->chamber(rpcdetid);
	if (rpcchamber) {
	    GlobalPoint globalPosition = rpcchamber->toGlobal(rpcRecHitLocalPosition);
	    rpcX[nRpc] = globalPosition.x();
	    rpcY[nRpc] = globalPosition.y();
	    rpcZ[nRpc] = globalPosition.z();
	    rpcPhi[nRpc] = globalPosition.phi();
	    rpcEta[nRpc] = globalPosition.eta();
	    rpcT[nRpc] = rpcRecHit.time();
	    rpcTError[nRpc] = rpcRecHit.timeError();
	    nRpc++;
	}
    }      
    for(DTRecSegment4D dtSegment : *dtSegments){
	LocalPoint  segmentLocalPosition       = dtSegment.localPosition();
	LocalVector segmentLocalDirection      = dtSegment.localDirection();
	// LocalError  segmentLocalPositionError  = iDT->localPositionError();
	// LocalError  segmentLocalDirectionError = iDT->localDirectionError();
	DetId geoid = dtSegment.geographicalId();
	DTChamberId dtdetid = DTChamberId(geoid);
	const DTChamber * dtchamber = dtG->chamber(dtdetid);
	if (dtchamber) {
	    GlobalPoint globalPosition = dtchamber->toGlobal(segmentLocalPosition);
	    GlobalVector globalDirection = dtchamber->toGlobal(segmentLocalDirection);

	    dtPhi[nDt] = globalPosition.phi();
	    dtEta[nDt] = globalPosition.eta();
	    dtX[nDt] = globalPosition.x();
	    dtY[nDt] = globalPosition.y();
	    dtZ[nDt] = globalPosition.z();
	    dtDirX[nDt] = globalDirection.x();
	    dtDirY[nDt] = globalDirection.y();
	    dtDirZ[nDt] = globalDirection.z();
	    dtT[nDt] = 0;//dtSegment.time();
	    dtTError[nDt] = -1;//dtSegment.timeError();
	    nDt++;
	}

    }


    return true;
}

bool llp_ntupler::fillMuons(const edm::Event& iEvent)
{
  for(const pat::Muon &mu : *muons)
  {
    if(mu.pt() < 5) continue;
    muonE[nMuons] = mu.energy();
    muonPt[nMuons] = mu.pt();
    muonEta[nMuons] = mu.eta();
    muonPhi[nMuons] = mu.phi();
    muonCharge[nMuons] = mu.charge();
    muonIsLoose[nMuons] = mu.isLooseMuon();
    muonIsMedium[nMuons] = mu.isMediumMuon();
    muonIsTight[nMuons] = mu.isTightMuon(*myPV);
    muon_d0[nMuons] = -mu.muonBestTrack()->dxy(myPV->position());
    muon_dZ[nMuons] = mu.muonBestTrack()->dz(myPV->position());
    muon_ip3d[nMuons] = mu.dB(pat::Muon::PV3D);
    muon_ip3dSignificance[nMuons] = mu.dB(pat::Muon::PV3D)/mu.edB(pat::Muon::PV3D);
    muonType[nMuons] = mu.isMuon() + mu.isGlobalMuon() + mu.isTrackerMuon() + mu.isStandAloneMuon()
      + mu.isCaloMuon() + mu.isPFMuon() + mu.isRPCMuon();
    muonQuality[nMuons] =
      muon::isGoodMuon(mu,muon::All)
    + muon::isGoodMuon(mu,muon::AllGlobalMuons)
    + muon::isGoodMuon(mu,muon::AllStandAloneMuons)
    + muon::isGoodMuon(mu,muon::AllTrackerMuons)
    + muon::isGoodMuon(mu,muon::TrackerMuonArbitrated)
    + muon::isGoodMuon(mu,muon::AllArbitrated)
    + muon::isGoodMuon(mu,muon::GlobalMuonPromptTight)
    + muon::isGoodMuon(mu,muon::TMLastStationLoose)
    + muon::isGoodMuon(mu,muon::TMLastStationTight)
    + muon::isGoodMuon(mu,muon::TM2DCompatibilityLoose)
    + muon::isGoodMuon(mu,muon::TM2DCompatibilityTight)
    + muon::isGoodMuon(mu,muon::TMOneStationLoose)
    + muon::isGoodMuon(mu,muon::TMOneStationTight)
    + muon::isGoodMuon(mu,muon::TMLastStationOptimizedLowPtLoose)
    + muon::isGoodMuon(mu,muon::TMLastStationOptimizedLowPtTight)
    + muon::isGoodMuon(mu,muon::GMTkChiCompatibility)
    + muon::isGoodMuon(mu,muon::GMStaChiCompatibility)
    + muon::isGoodMuon(mu,muon::GMTkKinkTight)
    + muon::isGoodMuon(mu,muon::TMLastStationAngLoose)
    + muon::isGoodMuon(mu,muon::TMLastStationAngTight)
    + muon::isGoodMuon(mu,muon::TMOneStationAngLoose)
    + muon::isGoodMuon(mu,muon::TMOneStationAngTight)
    + muon::isGoodMuon(mu,muon::TMLastStationOptimizedBarrelLowPtLoose)
    + muon::isGoodMuon(mu,muon::TMLastStationOptimizedBarrelLowPtTight)
    + muon::isGoodMuon(mu,muon::RPCMuLoose);
    muon_pileupIso[nMuons] = mu.pfIsolationR04().sumPUPt;
    muon_chargedIso[nMuons] = mu.pfIsolationR04().sumChargedHadronPt;
    muon_photonIso[nMuons] = mu.pfIsolationR04().sumPhotonEt;
    muon_neutralHadIso[nMuons] = mu.pfIsolationR04().sumNeutralHadronEt;
    //muon_ptrel[nMuons] = getLeptonPtRel( jets, &mu );
    //tuple<double,double,double> PFMiniIso = getPFMiniIsolation(packedPFCands, dynamic_cast<const reco::Candidate *>(&mu), 0.05, 0.2, 10., false, false);
    //muon_chargedMiniIso[nMuons] = std::get<0>(PFMiniIso);
    //muon_photonAndNeutralHadronMiniIso[nMuons] = std::get<1>(PFMiniIso);
    //muon_chargedPileupMiniIso[nMuons] = std::get<2>(PFMiniIso);
    //muon_activityMiniIsoAnnulus[nMuons] = ActivityPFMiniIsolationAnnulus( packedPFCands, dynamic_cast<const reco::Candidate *>(&mu), 0.4, 0.05, 0.2, 10.);
    muon_validFractionTrackerHits[nMuons] = (mu.innerTrack().isNonnull() ? mu.track()->validFraction() : -99.0);
    muon_isGlobal[nMuons] = muon::isGoodMuon(mu,muon::AllGlobalMuons);
    muon_normChi2[nMuons] = ( muon::isGoodMuon(mu,muon::AllGlobalMuons) ? mu.globalTrack()->normalizedChi2() : -99.0);
    muon_chi2LocalPosition[nMuons] = mu.combinedQuality().chi2LocalPosition;
    muon_kinkFinder[nMuons] = mu.combinedQuality().trkKink;
    muon_segmentCompatability[nMuons] = muon::segmentCompatibility(mu);
    bool isGoodGlobal = mu.isGlobalMuon() && mu.globalTrack()->normalizedChi2() < 3 && mu.combinedQuality().chi2LocalPosition < 12 && mu.combinedQuality().trkKink < 20;
    muonIsICHEPMedium[nMuons] = muon::isLooseMuon(mu) && muon_validFractionTrackerHits[nMuons] > 0.49 && muon::segmentCompatibility(mu) > (isGoodGlobal ? 0.303 : 0.451);
    //-----------------------
    //Trigger Object Matching
    //-----------------------
    bool passTagMuonFilter = false;
    /*for (pat::TriggerObjectStandAlone trigObject : *triggerObjects) {
      std::cout << "muon debug 18 " << nMuons << std::endl;
      if (deltaR(trigObject.eta(), trigObject.phi(),mu.eta(),mu.phi()) > 0.3) continue;
      trigObject.unpackFilterLabels(iEvent, *triggerBits);
      std::cout << "muon debug 19 " << nMuons << std::endl;
      //check single muon filters
      if ( trigObject.hasFilterLabel("hltL3fL1sMu25L1f0Tkf27QL3trkIsoFiltered0p09") ||
    	   trigObject.hasFilterLabel("hltL3fL1sMu20Eta2p1L1f0Tkf24QL3trkIsoFiltered0p09") ||
    	   trigObject.hasFilterLabel("hltL3fL1sMu16Eta2p1L1f0Tkf20QL3trkIsoFiltered0p09") ||
    	   trigObject.hasFilterLabel("hltL3fL1sMu16L1f0Tkf20QL3trkIsoFiltered0p09") ||
    	   trigObject.hasFilterLabel("hltL3crIsoL1sMu25L1f0L2f10QL3f27QL3trkIsoFiltered0p09") ||
    	   trigObject.hasFilterLabel("hltL3crIsoL1sMu20Eta2p1L1f0L2f10QL3f24QL3trkIsoFiltered0p09") ||
    	   trigObject.hasFilterLabel("hltL3crIsoL1sMu16Eta2p1L1f0L2f10QL3f20QL3trkIsoFiltered0p09") ||
    	   trigObject.hasFilterLabel("hltL3crIsoL1sMu16L1f0L2f10QL3f20QL3trkIsoFiltered0p09")
    	   ) passTagMuonFilter = true;
         std::cout << "muon debug 20 " << nMuons << std::endl;
      //check all filters
      for ( int q=0; q<MAX_MuonHLTFilters;q++) {
    	if (trigObject.hasFilterLabel(muonHLTFilterNames[q].c_str())) muon_passHLTFilter[nMuons][q] = true;
      std::cout << "muon debug 21 " << nMuons << std::endl;
      }

    }*/

    muon_passSingleMuTagFilter[nMuons] = passTagMuonFilter;
    nMuons++;
  }

  return true;
};


bool llp_ntupler::fillElectrons(const edm::Event& iEvent)
{

  // Get MVA values and categories (optional)
  edm::Handle<edm::ValueMap<float> > mvaGeneralPurposeValues;
  edm::Handle<edm::ValueMap<int> > mvaGeneralPurposeCategories;
  edm::Handle<edm::ValueMap<float> > mvaHZZValues;
  edm::Handle<edm::ValueMap<int> > mvaHZZCategories;
  //iEvent.getByToken(mvaGeneralPurposeValuesMapToken_,mvaGeneralPurposeValues);
  //iEvent.getByToken(mvaGeneralPurposeCategoriesMapToken_,mvaGeneralPurposeCategories);
  //iEvent.getByToken(mvaHZZValuesMapToken_,mvaHZZValues);
  //iEvent.getByToken(mvaHZZCategoriesMapToken_,mvaHZZCategories);

  for(const pat::Electron &ele : *electrons){
  //for (size_t i = 0; i < electrons->size(); ++i){
    //const auto ele = electrons->ptrAt(i);
    if(ele.pt() < 5) continue;
    eleE[nElectrons] = ele.energy();
    elePt[nElectrons] = ele.pt();
    eleEta[nElectrons] = ele.eta();
    elePhi[nElectrons] = ele.phi();
    eleCharge[nElectrons] = ele.charge();

    eleE_SC[nElectrons] = ele.superCluster()->energy();
    eleEta_SC[nElectrons] = ele.superCluster()->eta();
    elePhi_SC[nElectrons] = ele.superCluster()->phi();

    eleSigmaIetaIeta[nElectrons] = ele.sigmaIetaIeta();
    eleFull5x5SigmaIetaIeta[nElectrons] = ele.full5x5_sigmaIetaIeta();
    eleR9[nElectrons] = ele.r9();
    ele_dEta[nElectrons] = ele.deltaEtaSuperClusterTrackAtVtx() - ele.superCluster()->eta() + ele.superCluster()->seed()->eta();

    ele_dPhi[nElectrons] = ele.deltaPhiSuperClusterTrackAtVtx();
    ele_HoverE[nElectrons] = ele.hcalOverEcal();
    ele_d0[nElectrons] = -ele.gsfTrack().get()->dxy(myPV->position());
    ele_dZ[nElectrons] = ele.gsfTrack().get()->dz(myPV->position());

    //ele_ip3d[nElectrons] = ((edm::Ptr<pat::Electron>)(ele))->dB(pat::Electron::PV3D);
    //ele_ip3dSignificance[nElectrons] = ((edm::Ptr<pat::Electron>)(ele))->dB(pat::Electron::PV3D)/((edm::Ptr<pat::Electron>)(ele))->edB(pat::Electron::PV3D);
    ele_pileupIso[nElectrons] = ele.pfIsolationVariables().sumPUPt;
    ele_chargedIso[nElectrons] = ele.pfIsolationVariables().sumChargedHadronPt;
    ele_photonIso[nElectrons] = ele.pfIsolationVariables().sumPhotonEt;
    ele_neutralHadIso[nElectrons] = ele.pfIsolationVariables().sumNeutralHadronEt;
    ele_MissHits[nElectrons] = ele.gsfTrack()->hitPattern().numberOfAllHits(reco::HitPattern::MISSING_INNER_HITS);

    //---------------
    //Conversion Veto
    //---------------
    ele_PassConvVeto[nElectrons] = false;
    if( beamSpot.isValid() && conversions.isValid() )
    {
      //ele_PassConvVeto[nElectrons] = !ConversionTools::hasMatchedConversion(*ele,conversions,beamSpot->position());
    } else {
      cout << "\n\nERROR!!! conversions not found!!!\n";
    }
    //std::cout << "debug ele 5 " << nElectrons << std::endl;
    // 1/E - 1/P
    if( ele.ecalEnergy() == 0 ){
      ele_OneOverEminusOneOverP[nElectrons] = 1e30;
    } else if( !std::isfinite(ele.ecalEnergy())){
      ele_OneOverEminusOneOverP[nElectrons] = 1e30;
    } else {
    ele_OneOverEminusOneOverP[nElectrons] = 1./ele.ecalEnergy()  -  ele.eSuperClusterOverP()/ele.ecalEnergy();
    }
    //----------------------
    //ID MVA
    //----------------------
    //ele_IDMVAGeneralPurpose[nElectrons] = (*mvaGeneralPurposeValues)[ele];
    //ele_IDMVACategoryGeneralPurpose[nElectrons] = (*mvaGeneralPurposeCategories)[ele];
    //ele_IDMVAHZZ[nElectrons] = (*mvaHZZValues)[ele];
    //ele_IDMVACategoryHZZ[nElectrons] = (*mvaHZZCategories)[ele];


    //ele_RegressionE[nElectrons] = ((edm::Ptr<pat::Electron>)(ele))->ecalRegressionEnergy();
    //ele_CombineP4[nElectrons]   = ((edm::Ptr<pat::Electron>)(ele))->ecalTrackRegressionEnergy();

    //ele_ptrel[nElectrons]   = getLeptonPtRel( jets, &(*ele) );
    //tuple<double,double,double> PFMiniIso = getPFMiniIsolation(packedPFCands, dynamic_cast<const reco::Candidate *>(&(*ele)), 0.05, 0.2, 10., false, false);
    //ele_chargedMiniIso[nElectrons] = std::get<0>(PFMiniIso);
    //ele_photonAndNeutralHadronMiniIso[nElectrons] = std::get<1>(PFMiniIso);
    //ele_chargedPileupMiniIso[nElectrons] = std::get<2>(PFMiniIso);
    //ele_activityMiniIsoAnnulus[nElectrons] = ActivityPFMiniIsolationAnnulus( packedPFCands, dynamic_cast<const reco::Candidate *>(&(*ele)), 0.4, 0.05, 0.2, 10.);

    //-----------------------
    //Trigger Object Matching
    //-----------------------
    bool passSingleEleTagFilter = false;
    bool passTPOneTagFilter= false;
    bool passTPTwoTagFilter= false;
    bool passTPOneProbeFilter= false;
    bool passTPTwoProbeFilter= false;
    /*
    for (pat::TriggerObjectStandAlone trigObject : *triggerObjects)
    {
      if (deltaR(trigObject.eta(), trigObject.phi(),ele.eta(),ele.phi()) > 0.3) continue;
      trigObject.unpackFilterLabels(iEvent, *triggerBits);

      //check Single ele filters
      if (trigObject.hasFilterLabel("hltEle23WPLooseGsfTrackIsoFilter")  ||
    	  trigObject.hasFilterLabel("hltEle27WPLooseGsfTrackIsoFilter")  ||
    	  trigObject.hasFilterLabel("hltEle27WPTightGsfTrackIsoFilter")  ||
    	  trigObject.hasFilterLabel("hltEle32WPLooseGsfTrackIsoFilter")  ||
    	  trigObject.hasFilterLabel("hltEle32WPTightGsfTrackIsoFilter")
    	  )
        {
          passSingleEleTagFilter = true;
        }
      std::cout << "debug ele 7 " << nElectrons << std::endl;
      //check Tag and Probe Filters
      if (trigObject.hasFilterLabel("hltEle25WP60Ele8TrackIsoFilter")) passTPOneTagFilter = true;
      if (trigObject.hasFilterLabel("hltEle25WP60SC4TrackIsoFilter")) passTPTwoTagFilter = true;
      if (trigObject.hasFilterLabel("hltEle25WP60Ele8Mass55Filter")) passTPOneProbeFilter = true;
      if (trigObject.hasFilterLabel("hltEle25WP60SC4Mass55Filter")) passTPTwoProbeFilter = true;
      std::cout << "debug ele 8 " << nElectrons << std::endl;
      //check all filters
      for ( int q=0; q<MAX_ElectronHLTFilters;q++)
      {
        if (trigObject.hasFilterLabel(eleHLTFilterNames[q].c_str())) ele_passHLTFilter[nElectrons][q] = true;
      }
    }
    */
    //std::cout << "debug ele 9 " << nElectrons << std::endl;
    ele_passSingleEleTagFilter[nElectrons] = passSingleEleTagFilter;
    ele_passTPOneTagFilter[nElectrons] = passTPOneTagFilter;
    ele_passTPTwoTagFilter[nElectrons] = passTPTwoTagFilter;
    ele_passTPOneProbeFilter[nElectrons] = passTPOneProbeFilter;
    ele_passTPTwoProbeFilter[nElectrons] = passTPTwoProbeFilter;

    if (enableEcalRechits_)
    {
      ele_SeedRechitID.push_back(ele.superCluster()->seed()->seed().rawId());
      //---------------------
      //Find relevant rechits
      //---------------------
      std::vector<uint> rechits; rechits.clear();
      const std::vector< std::pair<DetId, float>>& v_id =ele.superCluster()->seed()->hitsAndFractions();
      for ( size_t i = 0; i < v_id.size(); ++i )
      {
        ecalRechitID_ToBeSaved.push_back(v_id[i].first);
        rechits.push_back(v_id[i].first.rawId());
      }
      ecalRechitEtaPhi_ToBeSaved.push_back( pair<double,double>( ele.superCluster()->eta(), ele.superCluster()->phi() ));
      ele_EcalRechitID.push_back(rechits);
    }
    //std::cout << "debug ele 10 " << nElectrons << std::endl;
    nElectrons++;
  }

  return true;
};

bool llp_ntupler::fillTaus(){
  for (const reco::PFTau &tau : *taus) {
    if (tau.pt() < 20) continue;
    tauE[nTaus] = tau.energy();
    tauPt[nTaus] = tau.pt();
    tauEta[nTaus] = tau.eta();
    tauPhi[nTaus] = tau.phi();

    //comment here
    /*
    tau_IsLoose[nTaus] = bool(tau.tauID("byLooseCombinedIsolationDeltaBetaCorr3Hits"));
    tau_IsMedium[nTaus] = bool(tau.tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits"));
    tau_IsTight[nTaus] = bool(tau.tauID("byTightCombinedIsolationDeltaBetaCorr3Hits"));
    tau_passEleVetoLoose[nTaus] = bool(tau.tauID("againstElectronLooseMVA6"));
    tau_passEleVetoMedium[nTaus] = bool(tau.tauID("againstElectronMediumMVA6"));
    tau_passEleVetoTight[nTaus] = bool(tau.tauID("againstElectronTightMVA6"));
    tau_passMuVetoLoose[nTaus] = bool(tau.tauID("againstMuonLoose3"));
    //tau_passMuVetoMedium[nTaus] = bool(tau.tauID("")); //doesn't exist anymore in miniAOD 2015 v2
    tau_passMuVetoTight[nTaus] = bool(tau.tauID("againstMuonTight3") );
    tau_combinedIsoDeltaBetaCorr3Hits[nTaus] = tau.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    tau_chargedIsoPtSum[nTaus] = tau.tauID("chargedIsoPtSum");
    tau_neutralIsoPtSum[nTaus] = tau.tauID("neutralIsoPtSum");
    tau_puCorrPtSum[nTaus] = tau.tauID("puCorrPtSum");
    tau_eleVetoMVA[nTaus] = tau.tauID("againstElectronMVA6Raw") ;
    tau_eleVetoCategory[nTaus] = tau.tauID("againstElectronMVA6category");
    //tau_muonVetoMVA[nTaus] = tau.tauID("againstMuonMVAraw"); //doesn't exist anymore in miniAOD 2015 v2
    tau_isoMVAnewDMwLT[nTaus] = tau.tauID("byIsolationMVArun2v1DBnewDMwLTraw");
    //tau_isoMVAnewDMwoLT[nTaus] = tau.tauID("byIsolationMVA3newDMwoLTraw") ; //doesn't exist anymore in miniAOD 2015 v2
    tau_ID[nTaus] =
      bool(tau.tauID("decayModeFinding")) +
      bool(tau.tauID("decayModeFindingNewDMs")) +
      bool(tau.tauID("byLooseCombinedIsolationDeltaBetaCorr3Hits")) +
      bool(tau.tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits")) +
      bool(tau.tauID("byTightCombinedIsolationDeltaBetaCorr3Hits")) +
      bool(tau.tauID("againstElectronVLooseMVA6")) +
      bool(tau.tauID("againstElectronLooseMVA6")) +
      bool(tau.tauID("againstElectronMediumMVA6")) +
      bool(tau.tauID("againstElectronTightMVA6")) +
      bool(tau.tauID("againstElectronVTightMVA6")) +
      bool(tau.tauID("againstMuonLoose3")) +
      bool(tau.tauID("againstMuonTight3")) +
      bool(tau.tauID("byVLooseIsolationMVArun2v1DBnewDMwLT")) +
      bool(tau.tauID("byLooseIsolationMVArun2v1DBnewDMwLT")) +
      bool(tau.tauID("byMediumIsolationMVArun2v1DBnewDMwLT")) +
      bool(tau.tauID("byTightIsolationMVArun2v1DBnewDMwLT")) +
      bool(tau.tauID("byVTightIsolationMVArun2v1DBnewDMwLT")) +
      bool(tau.tauID("byVVTightIsolationMVArun2v1DBnewDMwLT"));
      */
//tohere
    tau_leadCandPt[nTaus] = 0;
    tau_leadCandID[nTaus] = 0;
    tau_leadChargedHadrCandPt[nTaus] = 0;
    tau_leadChargedHadrCandID[nTaus] = 0;

  if (tau.leadPFCand().isNonnull()) {
      tau_leadCandPt[nTaus] = tau.leadPFCand()->pt();
      tau_leadCandID[nTaus] = tau.leadPFCand()->pdgId();
    }


    if (tau.leadPFChargedHadrCand().isNonnull()) {
      tau_leadChargedHadrCandPt[nTaus] = tau.leadPFChargedHadrCand()->pt();
      tau_leadChargedHadrCandID[nTaus] = tau.leadPFChargedHadrCand()->pdgId();
    }


    nTaus++;
  }

  return true;
};


bool llp_ntupler::fillPhotons(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  noZS::EcalClusterLazyTools *lazyToolnoZS = new noZS::EcalClusterLazyTools(iEvent, iSetup, ebRecHitsToken_, eeRecHitsToken_);
  for (const reco::Photon &pho : *photons) {
    //if (pho.pt() < 20) continue;
    std::vector<float> vCov = lazyToolnoZS->localCovariances( *(pho.superCluster()->seed()) );
    //-------------------------------------------------
    //default photon 4-mometum already vertex corrected
    //-------------------------------------------------
    //phoE[nPhotons] = pho.getCorrectedEnergy(reco::Photon::P4type::ecal_standard);
    phoE[nPhotons]   = pho.energy();
    phoPt[nPhotons]  = pho.pt();
    phoEta[nPhotons] = pho.eta(); //correct this for the vertex
    phoPhi[nPhotons] = pho.phi(); //correct this for the vertex
    /*std::cout << "phoE: " << pho.energy() << " phoCorr En:" << pho.getCorrectedEnergy(reco::Photon::P4type::regression2) << " un: "
    << pho.getCorrectedEnergyError(reco::Photon::P4type::regression2) << " "
    << pho.getCorrectedEnergyError( pho.getCandidateP4type() ) << std::endl;*/

    phoSigmaIetaIeta[nPhotons] = pho.sigmaIetaIeta();
    phoFull5x5SigmaIetaIeta[nPhotons] = pho.full5x5_sigmaIetaIeta();

    //phoR9[nPhotons] = pho.r9();
    //Use the noZS version of this according to Emanuele
    phoR9[nPhotons] = pho.full5x5_r9();
    pho_HoverE[nPhotons] = pho.hadTowOverEm();
    pho_isConversion[nPhotons] = pho.hasConversionTracks();
    //pho_passEleVeto[nPhotons] = !hasMatchedPromptElectron(pho.superCluster(),electrons,conversions, beamSpot->position());

    //------------------------------------------
    // Fill default miniAOD isolation quantities
    //------------------------------------------
    pho_pfIsoChargedHadronIso[nPhotons] = pho.chargedHadronIso();
    pho_pfIsoChargedHadronIsoWrongVtx[nPhotons] = pho.chargedHadronIsoWrongVtx();
    pho_pfIsoNeutralHadronIso[nPhotons] = pho.neutralHadronIso();
    pho_pfIsoPhotonIso[nPhotons] = pho.photonIso();
    pho_pfIsoModFrixione[nPhotons] = pho.getPflowIsolationVariables().modFrixione;
    pho_pfIsoSumPUPt[nPhotons] = pho.sumPUPt();
    //-----------------------
    // super cluster position and time
    //-----------------------
    pho_superClusterEnergy[nPhotons] = pho.superCluster()->energy();
    pho_superClusterRawEnergy[nPhotons] = pho.superCluster()->rawEnergy();
    pho_superClusterEta[nPhotons]    = pho.superCluster()->eta();
    pho_superClusterPhi[nPhotons]    = pho.superCluster()->phi();
    pho_superClusterX[nPhotons]      = pho.superCluster()->x();
    pho_superClusterY[nPhotons]      = pho.superCluster()->y();
    pho_superClusterZ[nPhotons]      = pho.superCluster()->z();
    pho_hasPixelSeed[nPhotons]       = pho.hasPixelSeed();

    pho_superClusterSeedX[nPhotons]      = pho.superCluster()->seed()->x();
    pho_superClusterSeedY[nPhotons]      = pho.superCluster()->seed()->y();
    pho_superClusterSeedZ[nPhotons]      = pho.superCluster()->seed()->z();

    pho_superClusterSeedE[nPhotons]      = pho.superCluster()->seed()->energy();

    /*for (const reco::PFCluster &pfcluster : *pfClusters)
    {
      if(pfcluster.seed() == pho.superCluster()->seed()->seed())
      {
        pho_superClusterSeedT[nPhotons] = pfcluster.time();
        pho_pfClusterSeedE[nPhotons]      = pfcluster.energy();
        //std::cout<<"find seed cluster for photon #"<<nPhotons<<std::endl;
      }
    }*/
    //std::cout<<"finished searching for seed cluster for photon #"<<nPhotons<<std::endl;


    //------------------------------------------------------
    //Compute PF isolation
    //absolute uncorrected isolations with footprint removal
    //------------------------------------------------------
    const float coneSizeDR = 0.3;
    const float dxyMax = 0.1;
    const float dzMax = 0.2;
    float chargedIsoSumAllVertices[MAX_NPV];
    for (int q=0;q<MAX_NPV;++q) chargedIsoSumAllVertices[q] = 0.0;
    float chargedIsoSum = 0;
    float chargedIsoSum_NewPV_NoTiming = 0;
    float chargedIsoSum_NewPV_Timing50_TrkVtx = 0;
    float chargedIsoSum_NewPV_Timing80_TrkVtx = 0;
    float chargedIsoSum_NewPV_Timing100_TrkVtx = 0;
    float chargedIsoSum_NewPV_Timing120_TrkVtx = 0;
    float chargedIsoSum_NewPV_Timing50_TrkPho = 0;
    float chargedIsoSum_NewPV_Timing80_TrkPho = 0;
    float chargedIsoSum_NewPV_Timing100_TrkPho = 0;
    float chargedIsoSum_NewPV_Timing120_TrkPho = 0;

    float neutralHadronIsoSum = 0;
    float photonIsoSum = 0;
    // First, find photon direction with respect to the good PV
    math::XYZVector photon_directionWrtVtx(pho.superCluster()->x() - myPV->x(),pho.superCluster()->y() - myPV->y(),pho.superCluster()->z() - myPV->z());
    //math::XYZVector photon_directionWrtVtx_GenMatch(pho.superCluster()->x() - myPV_GenMatch->x(),pho.superCluster()->y() - myPV_GenMatch->y(),pho.superCluster()->z() - myPV_GenMatch->z());

    // old PV, Loop over all PF candidates
    for (const reco::PFCandidate &candidate : *pfCands)
    {
      // Check if this candidate is within the isolation cone
      float dR=deltaR(photon_directionWrtVtx.Eta(),photon_directionWrtVtx.Phi(),
      candidate.eta(), candidate.phi());
      if( dR > coneSizeDR ) continue;

      // Check if this candidate is not in the footprint

      //bool inFootprint = false;
      //for (auto itr : pho.associatedPackedPFCandidates()) {
      //if ( &(*itr) == &candidate) {
      //inFootprint = true;
      //  }
      //}
      //if( inFootprint ) continue;

      // Find candidate type
      reco::PFCandidate::ParticleType thisCandidateType = reco::PFCandidate::X;

      // the neutral hadrons and charged hadrons can be of pdgId types
      // only 130 (K0L) and +-211 (pi+-) in packed candidates
      const int pdgId = candidate.pdgId();
      if( pdgId == 22 )
      thisCandidateType = reco::PFCandidate::gamma;
      else if( abs(pdgId) == 130) // PDG ID for K0L
      thisCandidateType = reco::PFCandidate::h0;
      else if( abs(pdgId) == 211) // PDG ID for pi+-
      thisCandidateType = reco::PFCandidate::h;


      // Increment the appropriate isolation sum
      if( thisCandidateType == reco::PFCandidate::h ){
        // for charged hadrons, additionally check consistency
        // with the PV
        float dxy = -999, dz = -999;

        //For the primary vertex
        dz = candidate.trackRef()->dz(myPV->position());
        dxy =candidate.trackRef()->dxy(myPV->position());
        if (fabs(dz) <= dzMax && fabs(dxy) <= dxyMax) {
          chargedIsoSum += candidate.pt();
        }

        //loop over all vertices
        for(int q = 0; q < nPVAll; q++){
          if(!(vertices->at(q).isValid() && !vertices->at(q).isFake())) continue;

          dz = candidate.trackRef()->dz(vertices->at(q).position());
          dxy =candidate.trackRef()->dxy(vertices->at(q).position());
          if (fabs(dz) > dzMax) continue;
          if(fabs(dxy) > dxyMax) continue;
          // The candidate is eligible, increment the isolation
          chargedIsoSumAllVertices[q] += candidate.pt();
        }
      }
      if( thisCandidateType == reco::PFCandidate::h0 )
      neutralHadronIsoSum += candidate.pt();
      if( thisCandidateType == reco::PFCandidate::gamma )
      photonIsoSum += candidate.pt();
    }

    /*
    //PROBLEM WITH myPV_GenMatch->position()
    // new PV, Loop over all PF candidates
    for (const reco::PFCandidate &candidate : *pfCands)
    {
      // Check if this candidate is within the isolation cone
      //float dR=deltaR(photon_directionWrtVtx_GenMatch.Eta(),photon_directionWrtVtx_GenMatch.Phi(),
      //candidate.eta(), candidate.phi());
      //if( dR > coneSizeDR ) continue;

      // Check if this candidate is not in the footprint

      //bool inFootprint = false;
      //for (auto itr : pho.associatedPackedPFCandidates()) {
      //if ( &(*itr) == &candidate) {
      //inFootprint = true;
      //  }
      //}
      //if( inFootprint ) continue;

      // Find candidate type
      reco::PFCandidate::ParticleType thisCandidateType = reco::PFCandidate::X;
      std::cout << "debug photons 6.1 " <<  std::endl;
      // the neutral hadrons and charged hadrons can be of pdgId types
      // only 130 (K0L) and +-211 (pi+-) in packed candidates
      const int pdgId = candidate.pdgId();
      if( pdgId == 22 )
      thisCandidateType = reco::PFCandidate::gamma;
      else if( abs(pdgId) == 130) // PDG ID for K0L
      thisCandidateType = reco::PFCandidate::h0;
      else if( abs(pdgId) == 211) // PDG ID for pi+-
      thisCandidateType = reco::PFCandidate::h;

      std::cout << "debug photons 6.2 " <<  std::endl;
      // Increment the appropriate isolation sum
      if( thisCandidateType == reco::PFCandidate::h )
      {
        //charged hadrons, additionally check consistency with PV
        float dxy = -999, dz = -999;
        //For the primary vertex
        dz = candidate.trackRef()->dz(myPV_GenMatch->position());
        dxy =candidate.trackRef()->dxy(myPV_GenMatch->position());
        float dt_TrkVtx = (*times)[candidate.trackRef()] - myPV_GenMatch->t();
        std::cout << "debug photons 6.3 " <<  std::endl;
        //photon time is the time extrapolated to (0,0,0)
        float CmToNs = 0.1/2.99792458;
        float globalOffset = 0.0111;//global offset of 0.0111 ns
        float pho_000_mag = sqrt(pow(pho_superClusterSeedX[nPhotons],2.0)+pow(pho_superClusterSeedY[nPhotons],2.0)+pow(pho_superClusterSeedZ[nPhotons],2.0));
        float photrk_mag = sqrt(pow(pho_superClusterSeedX[nPhotons] - candidate.trackRef()->vx(),2.0) + pow(pho_superClusterSeedY[nPhotons] - candidate.trackRef()->vy(),2.0) + pow(pho_superClusterSeedZ[nPhotons] - candidate.trackRef()->vz(),2.0) );
        TRandom3 randomPhotonTime(1111);
        float phoTime_m = randomPhotonTime.Gaus(pho_superClusterSeedT[nPhotons] + CmToNs*pho_000_mag - globalOffset, 0.03);
        float phoTime_track = (*times)[candidate.trackRef()] + CmToNs*photrk_mag;
        float dt_TrkPho = phoTime_track - phoTime_m;
        std::cout << "debug photons 6.4 " <<  std::endl;
        if (fabs(dz) <= dzMax && fabs(dxy) <= dxyMax)
        {
          chargedIsoSum_NewPV_NoTiming += candidate.pt();
        }
        if (fabs(dz) <= dzMax && fabs(dxy) <= dxyMax && fabs(dt_TrkVtx)<0.05 )
        {
          chargedIsoSum_NewPV_Timing50_TrkVtx += candidate.pt();
        }
        if (fabs(dz) <= dzMax && fabs(dxy) <= dxyMax && fabs(dt_TrkVtx)<0.08 )
        {
          chargedIsoSum_NewPV_Timing80_TrkVtx += candidate.pt();
        }

        if (fabs(dz) <= dzMax && fabs(dxy) <= dxyMax && fabs(dt_TrkVtx)<0.10 ) {
          chargedIsoSum_NewPV_Timing100_TrkVtx += candidate.pt();
        }

        if (fabs(dz) <= dzMax && fabs(dxy) <= dxyMax && fabs(dt_TrkVtx)<0.12 ) {
          chargedIsoSum_NewPV_Timing120_TrkVtx += candidate.pt();
        }

        std::cout << "debug photons 6.5 " <<  std::endl;
        if (fabs(dz) <= dzMax && fabs(dxy) <= dxyMax && fabs(dt_TrkPho)<0.05 ) {
          chargedIsoSum_NewPV_Timing50_TrkPho += candidate.pt();
        }

        if (fabs(dz) <= dzMax && fabs(dxy) <= dxyMax && fabs(dt_TrkPho)<0.08 ) {
          chargedIsoSum_NewPV_Timing80_TrkPho += candidate.pt();
        }

        if (fabs(dz) <= dzMax && fabs(dxy) <= dxyMax && fabs(dt_TrkPho)<0.10 ) {
          chargedIsoSum_NewPV_Timing100_TrkPho += candidate.pt();
        }

        if (fabs(dz) <= dzMax && fabs(dxy) <= dxyMax && fabs(dt_TrkPho)<0.12 ) {
          chargedIsoSum_NewPV_Timing120_TrkPho += candidate.pt();
        }

      }
    }
*/
    //fill the proper variables
    for(int q = 0; q < nPVAll; q++) {
      pho_sumChargedHadronPtAllVertices[nPhotons][q] = chargedIsoSumAllVertices[q];
    }
    pho_sumChargedHadronPt[nPhotons] = chargedIsoSum;

    pho_sumChargedHadronPt_NewPV_NoTiming[nPhotons] = chargedIsoSum_NewPV_NoTiming;

    pho_sumChargedHadronPt_NewPV_Timing50_TrkVtx[nPhotons] = chargedIsoSum_NewPV_Timing50_TrkVtx;
    pho_sumChargedHadronPt_NewPV_Timing80_TrkVtx[nPhotons] = chargedIsoSum_NewPV_Timing80_TrkVtx;
    pho_sumChargedHadronPt_NewPV_Timing100_TrkVtx[nPhotons] = chargedIsoSum_NewPV_Timing100_TrkVtx;
    pho_sumChargedHadronPt_NewPV_Timing120_TrkVtx[nPhotons] = chargedIsoSum_NewPV_Timing120_TrkVtx;

    pho_sumChargedHadronPt_NewPV_Timing50_TrkPho[nPhotons] = chargedIsoSum_NewPV_Timing50_TrkPho;
    pho_sumChargedHadronPt_NewPV_Timing80_TrkPho[nPhotons] = chargedIsoSum_NewPV_Timing80_TrkPho;
    pho_sumChargedHadronPt_NewPV_Timing100_TrkPho[nPhotons] = chargedIsoSum_NewPV_Timing100_TrkPho;
    pho_sumChargedHadronPt_NewPV_Timing120_TrkPho[nPhotons] = chargedIsoSum_NewPV_Timing120_TrkPho;

    pho_sumNeutralHadronEt[nPhotons] = neutralHadronIsoSum;
    pho_sumPhotonEt[nPhotons] = photonIsoSum;


    //-------------------------------------------------
    //Compute Worst Isolation Looping over all vertices
    //-------------------------------------------------
    const double ptMin = 0.0;
    const float dRvetoBarrel = 0.0;
    const float dRvetoEndcap = 0.0;
    float dRveto = 0;
    if (pho.isEB()) dRveto = dRvetoBarrel;
    else dRveto = dRvetoEndcap;

    float worstIsolation = 999;
    std::vector<float> allIsolations;
    for(unsigned int ivtx=0; ivtx<vertices->size(); ++ivtx) {

      // Shift the photon according to the vertex
      reco::VertexRef vtx(vertices, ivtx);
      math::XYZVector photon_directionWrtVtx(pho.superCluster()->x() - vtx->x(),
      pho.superCluster()->y() - vtx->y(),
      pho.superCluster()->z() - vtx->z());

      float sum = 0;
      // Loop over all PF candidates
      for (const reco::PFCandidate &candidate : *pfCands) {

        //require that PFCandidate is a charged hadron
        const int pdgId = candidate.pdgId();
        if( abs(pdgId) != 211) continue;

        if (candidate.pt() < ptMin)
        continue;

        float dxy = -999, dz = -999;
        dz = candidate.trackRef()->dz(myPV->position());
        dxy =candidate.trackRef()->dxy(myPV->position());
        if( fabs(dxy) > dxyMax) continue;
        if ( fabs(dz) > dzMax) continue;

        float dR = deltaR(photon_directionWrtVtx.Eta(), photon_directionWrtVtx.Phi(),
        candidate.eta(),      candidate.phi());
        if(dR > coneSizeDR || dR < dRveto) continue;

        sum += candidate.pt();
      }

      allIsolations.push_back(sum);
    }

    if( allIsolations.size()>0 )
    worstIsolation = * std::max_element( allIsolations.begin(), allIsolations.end() );

    pho_sumWorstVertexChargedHadronPt[nPhotons] = worstIsolation;

    //-----------------------
    //Photon ID MVA variable
    //-----------------------
    //pho_IDMVA[nPhotons] = myPhotonMVA->mvaValue( pho,  *rhoAll, photonIsoSum, chargedIsoSum, worstIsolation,lazyToolnoZS, false);

    //pho_RegressionE[nPhotons] = pho.getCorrectedEnergy(reco::Photon::P4type::regression1);
    //pho_RegressionEUncertainty[nPhotons] = pho.getCorrectedEnergyError(reco::Photon::P4type::regression1);

    //---------------------
    //Use Latest Regression
    //---------------------
    pho_RegressionE[nPhotons]            = pho.getCorrectedEnergy( pho.getCandidateP4type() );
    pho_RegressionEUncertainty[nPhotons] = pho.getCorrectedEnergyError( pho.getCandidateP4type() );


    //conversion matching for beamspot pointing
    const reco::Conversion *convmatch = 0;
    double drmin = std::numeric_limits<double>::max();
    //double leg conversions
    for (const reco::Conversion &conv : *conversions) {
      if (conv.refittedPairMomentum().rho()<10.) continue;
      if (!conv.conversionVertex().isValid()) continue;
      if (TMath::Prob(conv.conversionVertex().chi2(),  conv.conversionVertex().ndof())<1e-6) continue;

      math::XYZVector mom(conv.refittedPairMomentum());
      math::XYZPoint scpos(pho.superCluster()->position());
      math::XYZPoint cvtx(conv.conversionVertex().position());
      math::XYZVector cscvector = scpos - cvtx;

      double dr = reco::deltaR(mom,cscvector);

      if (dr<drmin && dr<0.1) {
        drmin = dr;
        convmatch = &conv;
      }
    }
    if (!convmatch) {
      drmin = std::numeric_limits<double>::max();
      //single leg conversions
      for (const reco::Conversion &conv : *singleLegConversions) {
        math::XYZVector mom(conv.tracksPin()[0]);
        math::XYZPoint scpos(pho.superCluster()->position());
        math::XYZPoint cvtx(conv.conversionVertex().position());
        math::XYZVector cscvector = scpos - cvtx;

        double dr = reco::deltaR(mom,cscvector);

        if (dr<drmin && dr<0.1) {
          drmin = dr;
          convmatch = &conv;
        }
      }
    }

    //matched conversion, compute conversion type
    //and extrapolation to beamline
    //FIXME Both of these additional two requirements are inconsistent and make the conversion
    //selection depend on poorly defined criteria, but we keep them for sync purposes
    //if (convmatch && pho.hasConversionTracks() && conversions->size()>0) {
    if (convmatch){// && pho.hasConversionTracks() && conversions->size()>0) {
      int ntracks = convmatch->nTracks();

      math::XYZVector mom(ntracks==2 ? convmatch->refittedPairMomentum() : convmatch->tracksPin()[0]);
      math::XYZPoint scpos(pho.superCluster()->position());
      math::XYZPoint cvtx(convmatch->conversionVertex().position());
      math::XYZVector cscvector = scpos - cvtx;

      double z = cvtx.z();
      double rho = cvtx.rho();

      int legtype = ntracks==2 ? 0 : 1;
      int dettype = pho.isEB() ? 0 : 1;
      int postype =0;

      if (pho.isEB()) {
        if (rho<15.) {
          postype = 0;
        }
        else if (rho>=15. && rho<60.) {
          postype = 1;
        }
        else {
          postype = 2;
        }
      }
      else {
        if (std::abs(z) < 50.) {
          postype = 0;
        }
        else if (std::abs(z) >= 50. && std::abs(z) < 100.) {
          postype = 1;
        }
        else {
          postype = 2;
        }
      }

      pho_convType[nPhotons] = legtype + 2*dettype + 4*postype;
      pho_convTrkZ[nPhotons] = cvtx.z() - ((cvtx.x()-beamSpot->x0())*mom.x()+(cvtx.y()-beamSpot->y0())*mom.y())/mom.rho() * mom.z()/mom.rho();
      pho_convTrkClusZ[nPhotons] = cvtx.z() - ((cvtx.x()-beamSpot->x0())*cscvector.x()+(cvtx.y()-beamSpot->y0())*cscvector.y())/cscvector.rho() * cscvector.z()/cscvector.rho();
    }

    nPhotons++;
  }

  /*
  //CRASHING MEMORY, I THINK
  std::cout << "debug photons 9 " <<  std::endl;
  double pho_vtxSumPxD[OBJECTARRAYSIZE][MAX_NPV];
  double pho_vtxSumPyD[OBJECTARRAYSIZE][MAX_NPV];

  std::cout << "debug photons 10 " <<  std::endl;
  for (int ipho = 0; ipho<nPhotons; ++ipho) {
    for (int ipv = 0; ipv<MAX_NPV; ++ipv) {
      pho_vtxSumPxD[ipho][ipv] = 0.;
      pho_vtxSumPyD[ipho][ipv] = 0.;

    }
  }

  std::cout << "debug photons 11 " <<  std::endl;
  //fill information on tracks to exclude around photons for vertex selection purposes
  for (const reco::PFCandidate &pfcand : *pfCands) {
    if (pfcand.charge()==0) continue;
    double mindz = std::numeric_limits<double>::max();
    int ipvmin = -1;
    for (int ipv = 0; ipv < nPVAll; ++ipv) {
      const reco::Vertex &vtx = vertices->at(ipv);
      //double dz = std::abs(pfcand.dz(vtx.position()));
      double dz = std::abs(pfcand.vz()-vtx.z());
      if (dz<mindz) {
        mindz = dz;
        ipvmin = ipv;
      }
    }

    std::cout << "debug photons 12 " <<  std::endl;
    if (mindz<0.2 && ipvmin>=0 && ipvmin<MAX_NPV) {
      const reco::Vertex &vtx = vertices->at(ipvmin);
      for (int ipho = 0; ipho < nPhotons; ++ipho) {
        const reco::Photon &pho = photons->at(ipho);
        math::XYZVector phodir(pho.superCluster()->x()-vtx.x(),pho.superCluster()->y()-vtx.y(),pho.superCluster()->z()-vtx.z());
        double dr = reco::deltaR(phodir, pfcand);
        if (dr<0.05) {
          pho_vtxSumPxD[ipho][ipvmin] += pfcand.px();
          pho_vtxSumPyD[ipho][ipvmin] += pfcand.py();
        }
        //add addition dt cut here:
        //
      }
    }
  }
  std::cout << "debug photons 13 " <<  std::endl;
  for (int ipho = 0; ipho<nPhotons; ++ipho) {
    for (int ipv = 0; ipv<nPVAll; ++ipv) {
      pho_vtxSumPx[ipho][ipv] = pho_vtxSumPxD[ipho][ipv];
      pho_vtxSumPy[ipho][ipv] = pho_vtxSumPyD[ipho][ipv];

    }
  }
  */
  delete lazyToolnoZS;
  return true;

};


bool llp_ntupler::fillJets(const edm::EventSetup& iSetup)
{
  for (const reco::PFJet &j : *jets)
  {
    if (j.pt() < 20) continue;
    if (fabs(j.eta()) > 2.4) continue;
    //-------------------
    //Fill Jet-Level Info
    //-------------------
    jetE[nJets] = j.energy();
    jetPt[nJets] = j.pt();
    jetEta[nJets] = j.eta();
    jetPhi[nJets] = j.phi();
    jetMass[nJets] = j.mass();

    TLorentzVector thisJet;
    thisJet.SetPtEtaPhiE(jetPt[nJets], jetEta[nJets], jetPhi[nJets], jetE[nJets]);
    //jetCISV = j.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");

    jetJetArea[nJets] = j.jetArea();
    jetPileupE[nJets] = j.pileup();

    jetPileupIdFlag[nJets] = 0;
    jetPassIDLoose[nJets] = passJetID(&j, 0);
    jetPassIDTight[nJets] = passJetID(&j, 1);
    jetPassMuFrac[nJets]  = ( j.muonEnergyFraction() < 0.80 );
    jetPassEleFrac[nJets]  = ( j.electronEnergyFraction() < 0.90 );


    // if (useGen_) {
    //   jetPartonFlavor = j.partonFlavour();
    //   jetHadronFlavor = j.hadronFlavour();
    // }

    jetChargedEMEnergyFraction[nJets] = j.chargedEmEnergyFraction();
    jetNeutralEMEnergyFraction[nJets] = j.neutralEmEnergyFraction();
    jetChargedHadronEnergyFraction[nJets] = j.chargedHadronEnergyFraction();
    jetNeutralHadronEnergyFraction[nJets] = j.neutralHadronEnergyFraction();
    //jet_charged_hadron_multiplicity[nJets] = j.chargedHadronMultiplicity();
    //jet_neutral_hadron_multiplicity[nJets] = j.neutralHadronMultiplicity();
    //jet_photon_multiplicity[nJets] = j.photonMultiplicity();
    //jet_electron_multiplicity[nJets] = j.electronMultiplicity();
    //jet_muon_multiplicity[nJets] = j.muonMultiplicity();
    //jet_HF_hadron_multiplicity[nJets] = j.HFHadronMultiplicity();
    //jet_HF_em_multiplicity[nJets] = j.HFEMMultiplicity();
    //jet_charged_multiplicity[nJets] = j.chargedMultiplicity();
    //jet_neutral_multiplicity[nJets] = j.neutralMultiplicity();


    //---------------------------
    //find photons inside the jet
    //---------------------------
    /*
    for (const reco::Photon &pho : *photons) {
      //cout << "Nphoton: " << fJetNPhotons << "\n";

      if (!(deltaR(pho.eta(), pho.phi() , j.eta(), j.phi()) < 0.5)) continue;


      fJetPhotonPt[fJetNPhotons]  = pho.pt();
      fJetPhotonEta[fJetNPhotons] = pho.eta(); //correct this for the vertex
      fJetPhotonPhi[fJetNPhotons] = pho.phi(); //correct this for the vertex

      fJetPhotonSeedRecHitE[fJetNPhotons]      = pho.superCluster()->seed()->x();
      fJetPhotonSeedRecHitEta[fJetNPhotons]      = pho.superCluster()->seed()->y();
      fJetPhotonSeedRecHitPhi[fJetNPhotons]      = pho.superCluster()->seed()->z();
      fJetPhotonSeedRecHitTime[fJetNPhotons]      = pho.superCluster()->seed()->energy();

      // //get time coordinate for the seed
      // for (const reco::PFCluster &pfcluster : *pfClusters) {
      // 	if(pfcluster.seed() == pho.superCluster()->seed()->seed())
      // 	  {
      // 	    pho_superClusterSeedT[fJetNPhotons] = pfcluster.time();
      // 	    pho_pfClusterSeedE[fJetNPhotons]      = pfcluster.energy();
      // 	  }
      // }

      //-------------------------------
      //fill all rechits inside photons
      //-------------------------------

      fJetNPhotons++;

    }
    */

    //---------------------------
    //Find RecHits Inside the Jet
    //---------------------------
    // geometry (from ECAL ELF)

    edm::ESHandle<CaloGeometry> geoHandle;
    iSetup.get<CaloGeometryRecord>().get(geoHandle);
    const CaloSubdetectorGeometry *barrelGeometry = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
    //const CaloSubdetectorGeometry *endcapGeometry = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
    //double ecal_radius = 129.0;
    int n_matched_rechits = 0;
    for (EcalRecHitCollection::const_iterator recHit = ebRecHits->begin(); recHit != ebRecHits->end(); ++recHit)
    {
      if ( recHit->checkFlag(0) )
      {
        const DetId recHitId = recHit->detid();
        const auto recHitPos = barrelGeometry->getGeometry(recHitId)->getPosition();
        if ( deltaR(jetEta[nJets], jetPhi[nJets], recHitPos.eta(), recHitPos.phi())  < 0.4)
        {
          //double rechit_x = ecal_radius * cos(recHitPos.phi());
          //double rechit_y = ecal_radius * sin(recHitPos.phi());
          //double rechit_z = ecal_radius * sinh(recHitPos.eta());
          //double photon_pv_travel_time = (1./30) * sqrt(pow(pvX-rechit_x,2)+pow(pvY-rechit_y,2)+pow(pvZ-rechit_z,2));

          if (recHit->energy() > 1.0)
          {
            jetRechitE[nJets] += recHit->energy();
            jetRechitT[nJets] += recHit->time()*recHit->energy();
          }
          n_matched_rechits++;
        }
      }
    }
    //cout << "Last Nphoton: " << fJetNPhotons << "\n";
    //std::cout << "n: " << n_matched_rechits << std::endl;
    jetNRechits[nJets] = n_matched_rechits;
    jetRechitT[nJets] = jetRechitT[nJets]/jetRechitE[nJets];
    nJets++;
  } //loop over jets

  return true;
};

bool llp_ntupler::fillMet(const edm::Event& iEvent)
{
  const reco::PFMET &Met = mets->front();

  //metPt = Met.uncorPt();
  //metPhi = Met.uncorPhi();
  sumMET = Met.sumEt();
  metType0Pt = 0;
  metType0Phi = 0;
  metType1Pt_raw = Met.pt();
  metType1Pt = Met.pt();
  metType1Px = Met.px();
  metType1Py = Met.py();
  metType1Eta = Met.eta();
  metType1Phi_raw = Met.phi();
  metType1Phi = Met.phi();
  metType0Plus1Pt = 0;
  metType0Plus1Phi = 0;
  //metCaloPt = Met.caloMETPt();
  //metCaloPhi = Met.caloMETPhi();

/*
  if(!isData_)
    {
      metType1PtJetResUp           = Met.shiftedPt(reco::PFMET::METUncertainty::JetResUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PtJetResDown         = Met.shiftedPt(reco::PFMET::METUncertainty::JetResDown, reco::PFMET::METCorrectionLevel::Type1);
      metType1PtJetEnUp            = Met.shiftedPt(reco::PFMET::METUncertainty::JetEnUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PtJetEnDown          = Met.shiftedPt(reco::PFMET::METUncertainty::JetEnDown, reco::PFMET::METCorrectionLevel::Type1);
      metType1PtMuonEnUp           = Met.shiftedPt(reco::PFMET::METUncertainty::MuonEnUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PtMuonEnDown         = Met.shiftedPt(reco::PFMET::METUncertainty::MuonEnDown, reco::PFMET::METCorrectionLevel::Type1);
      metType1PtElectronEnUp       = Met.shiftedPt(reco::PFMET::METUncertainty::ElectronEnUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PtElectronEnDown     = Met.shiftedPt(reco::PFMET::METUncertainty::ElectronEnDown, reco::PFMET::METCorrectionLevel::Type1);
      metType1PtTauEnUp	           = Met.shiftedPt(reco::PFMET::METUncertainty::TauEnUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PtTauEnDown          = Met.shiftedPt(reco::PFMET::METUncertainty::TauEnDown, reco::PFMET::METCorrectionLevel::Type1);
      metType1PtUnclusteredEnUp    = Met.shiftedPt(reco::PFMET::METUncertainty::UnclusteredEnUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PtUnclusteredEnDown  = Met.shiftedPt(reco::PFMET::METUncertainty::UnclusteredEnDown, reco::PFMET::METCorrectionLevel::Type1);
      metType1PtPhotonEnUp         = Met.shiftedPt(reco::PFMET::METUncertainty::PhotonEnUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PtPhotonEnDown       = Met.shiftedPt(reco::PFMET::METUncertainty::PhotonEnDown, reco::PFMET::METCorrectionLevel::Type1);
      // metType1PtMETUncertaintySize = Met.shiftedPt(reco::PFMET::METUncertainty::METUncertaintySize, reco::PFMET::METCorrectionLevel::Type1);
      // metType1PtJetResUpSmear     = Met.shiftedPt(reco::PFMET::METUncertainty::JetResUpSmear, reco::PFMET::METCorrectionLevel::Type1);
      // metType1PtJetResDownSmear   = Met.shiftedPt(reco::PFMET::METUncertainty::JetResDownSmear, reco::PFMET::METCorrectionLevel::Type1);
      // metType1PtMETFullUncertaintySize = Met.shiftedPt(reco::PFMET::METUncertainty::METFullUncertaintySize, reco::PFMET::METCorrectionLevel::Type1);

      metType1PhiJetResUp          = Met.shiftedPhi(reco::PFMET::METUncertainty::JetResUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PhiJetResDown        = Met.shiftedPhi(reco::PFMET::METUncertainty::JetResDown, reco::PFMET::METCorrectionLevel::Type1);
      metType1PhiJetEnUp           = Met.shiftedPhi(reco::PFMET::METUncertainty::JetEnUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PhiJetEnDown         = Met.shiftedPhi(reco::PFMET::METUncertainty::JetEnDown, reco::PFMET::METCorrectionLevel::Type1);
      metType1PhiMuonEnUp          = Met.shiftedPhi(reco::PFMET::METUncertainty::MuonEnUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PhiMuonEnDown        = Met.shiftedPhi(reco::PFMET::METUncertainty::MuonEnDown, reco::PFMET::METCorrectionLevel::Type1);
      metType1PhiElectronEnUp      = Met.shiftedPhi(reco::PFMET::METUncertainty::ElectronEnUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PhiElectronEnDown    = Met.shiftedPhi(reco::PFMET::METUncertainty::ElectronEnDown, reco::PFMET::METCorrectionLevel::Type1);
      metType1PhiTauEnUp           = Met.shiftedPhi(reco::PFMET::METUncertainty::TauEnUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PhiTauEnDown         = Met.shiftedPhi(reco::PFMET::METUncertainty::TauEnDown, reco::PFMET::METCorrectionLevel::Type1);
      metType1PhiUnclusteredEnUp   = Met.shiftedPhi(reco::PFMET::METUncertainty::UnclusteredEnUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PhiUnclusteredEnDown = Met.shiftedPhi(reco::PFMET::METUncertainty::UnclusteredEnDown, reco::PFMET::METCorrectionLevel::Type1);
      metType1PhiPhotonEnUp        = Met.shiftedPhi(reco::PFMET::METUncertainty::PhotonEnUp, reco::PFMET::METCorrectionLevel::Type1);
      metType1PhiPhotonEnDown      = Met.shiftedPhi(reco::PFMET::METUncertainty::PhotonEnDown, reco::PFMET::METCorrectionLevel::Type1);
      // metType1PhiMETUncertaintySize = Met.shiftedPhi(reco::PFMET::METUncertainty::METUncertaintySize, reco::PFMET::METCorrectionLevel::Type1);
      // metType1PhiJetResUpSmear     = Met.shiftedPhi(reco::PFMET::METUncertainty::JetResUpSmear, reco::PFMET::METCorrectionLevel::Type1);
      // metType1PhiJetResDownSmear   = Met.shiftedPhi(reco::PFMET::METUncertainty::JetResDownSmear, reco::PFMET::METCorrectionLevel::Type1);
      // metType1PhiMETFullUncertaintySize = Met.shiftedPhi(reco::PFMET::METUncertainty::METFullUncertaintySize, reco::PFMET::METCorrectionLevel::Type1);
    }
 */

  const reco::PFMET &MetPuppi = metsPuppi->front();
  //const reco::PFMET &MetNoHF = metsNoHF->front();
  metPuppiPt = MetPuppi.pt();
  metPuppiPhi = MetPuppi.phi();
  //metNoHFPt = MetNoHF.pt();
  //metNoHFPhi = MetNoHF.phi();

  //MET filters
  if (!isFastsim_) {
    const edm::TriggerNames &metNames = iEvent.triggerNames(*metFilterBits);

    //----------------------------
    //For Debug printout
    //----------------------------
    // for (unsigned int i = 0, n = metFilterBits->size(); i < n; ++i) {
    // 	std::cout << "MET Filter " << metNames.triggerName(i).c_str() << "\n";
    // }

    for(unsigned int i = 0, n = metFilterBits->size(); i < n; ++i)
    {
      if(strcmp(metNames.triggerName(i).c_str(), "Flag_trackingFailureFilter") == 0)
      Flag_trackingFailureFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_goodVertices") == 0)
      Flag_goodVertices = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_globalTightHalo2016Filter") == 0)
      Flag_CSCTightHaloFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_trkPOGFilters") == 0)
      Flag_trkPOGFilters = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_trkPOG_logErrorTooManyClusters") == 0)
      Flag_trkPOG_logErrorTooManyClusters = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_EcalDeadCellTriggerPrimitiveFilter") == 0)
      Flag_EcalDeadCellTriggerPrimitiveFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_EcalDeadCellBoundaryEnergyFilter") == 0)
      Flag_EcalDeadCellBoundaryEnergyFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_ecalLaserCorrFilter") == 0)
      Flag_ecalLaserCorrFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_trkPOG_manystripclus53X") == 0)
      Flag_trkPOG_manystripclus53X = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_eeBadScFilter") == 0)
      Flag_eeBadScFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_METFilters") == 0)
      Flag_METFilters = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_HBHENoiseFilter") == 0)
      Flag_HBHENoiseFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_HBHENoiseIsoFilter") == 0)
      Flag_HBHEIsoNoiseFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_trkPOG_toomanystripclus53X") == 0)
      Flag_trkPOG_toomanystripclus53X = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_hcalLaserEventFilter") == 0)
      Flag_hcalLaserEventFilter = metFilterBits->accept(i);
    } //loop over met filters

    //use custom hbhefilter, because miniAOD filters are problematic.
    //Flag_HBHENoiseFilter = *hbheNoiseFilter;
    //Flag_HBHETightNoiseFilter = *hbheTightNoiseFilter;
    //Flag_HBHEIsoNoiseFilter = *hbheIsoNoiseFilter;
    //Flag_badChargedCandidateFilter = *badChargedCandidateFilter;
    //Flag_badMuonFilter = *badMuonFilter;
  }

  return true;
};

bool llp_ntupler::passJetID( const reco::PFJet *jet, int cutLevel) {
  bool result = false;

  double NHF = jet->neutralHadronEnergyFraction();
  double NEMF = jet->neutralEmEnergyFraction();
  int NumConst = jet->chargedMultiplicity() + jet->neutralMultiplicity() ;
  double CHF = jet->chargedHadronEnergyFraction();
  double MUF = jet->muonEnergyFraction();
  double CEMF = jet->chargedEmEnergyFraction();
  int NumNeutralParticles =jet->neutralMultiplicity();
  int CHM = jet->chargedMultiplicity();

  //Loose
  if (cutLevel == 0) {
    if ( fabs(jet->eta()) <= 2.4) {
      if ( NHF  < 0.99 && NEMF < 0.99 && NumConst > 1
	   && CHF > 0 && CHM > 0 && CEMF < 0.99 ) result = true;
    } else if( fabs(jet->eta()) <= 3.0)  {
      if ( NHF  < 0.99 && NEMF < 0.99 && NumConst > 1 ) result = true;
    } else {
      if ( NEMF < 0.90 && NumNeutralParticles > 10 ) result = true;
    }
  }

  //Tight
  else if (cutLevel == 1) {
    if ( fabs(jet->eta()) <= 2.4) {
      if ( NHF  < 0.90 && NEMF < 0.90 && NumConst > 1
	   && CHF > 0 && CHM > 0 && CEMF < 0.99 ) result = true;
    } else if( fabs(jet->eta()) <= 3.0)  {
      if ( NHF  < 0.90 && NEMF < 0.90 && NumConst > 1 ) result = true;
    } else {
      if ( NEMF < 0.90 && NumNeutralParticles > 10 ) result = true;
    }
  }

  //Tight Lep Veto
  else if (cutLevel == 2) {
    if ( fabs(jet->eta()) <= 2.4) {
      if ( NHF  < 0.90 && NEMF < 0.90 && NumConst > 1
	   && CHF > 0 && CHM > 0 && CEMF < 0.99 && MUF < 0.8 ) result = true;
    } else if( fabs(jet->eta()) <= 3.0)  {
      if ( NHF  < 0.90 && NEMF < 0.90 && NumConst > 1 ) result = true;
    } else {
      if ( NEMF < 0.90 && NumNeutralParticles > 10 ) result = true;
    }
  }

  return result;
}

double llp_ntupler::deltaPhi(double phi1, double phi2)
{
  double dphi = phi1-phi2;
  while (dphi > TMath::Pi())
  {
    dphi -= TMath::TwoPi();
  }
  while (dphi <= -TMath::Pi())
  {
    dphi += TMath::TwoPi();
  }
  return dphi;
};

double llp_ntupler::deltaR(double eta1, double phi1, double eta2, double phi2)
{
double dphi = deltaPhi(phi1,phi2);
double deta = eta1 - eta2;
return sqrt( dphi*dphi + deta*deta);
};

bool llp_ntupler::fillMC()
{
  for(const reco::GenJet &j : *genJets)
  {
    //std::cout << nGenJets << std::endl;
    genJetE[nGenJets] = j.energy();
    genJetPt[nGenJets] = j.pt();
    genJetEta[nGenJets] = j.eta();
    genJetPhi[nGenJets] = j.phi();
    genJetMET[nGenJets] = j.invisibleEnergy();
    nGenJets++;
  }

  const reco::GenMET &GenMetCalo = genMetsCalo->front();
  genMetPtCalo  = GenMetCalo.pt();
  genMetPhiCalo = GenMetCalo.phi();

  const reco::GenMET &GenMetTrue = genMetsTrue->front();
  genMetPtTrue  = GenMetTrue.pt();
  genMetPhiTrue = GenMetTrue.phi();

  bool foundGenVertex = false;
  for(size_t i=0; i<genParticles->size();i++)
  {
    if (!foundGenVertex)
    {
      for (unsigned int j=0; j<(*genParticles)[i].numberOfDaughters(); ++j)
      {
        const reco::Candidate *dau = (*genParticles)[i].daughter(j);
        if (dau)
        {
          genVertexX = dau->vx();
          genVertexY = dau->vy();
          genVertexZ = dau->vz();
          if(readGenVertexTime_) genVertexT = *genParticles_t0;
          foundGenVertex = true;
          break;
        }
      }
    }
  }

  genWeight = genInfo->weight();
  genSignalProcessID = genInfo->signalProcessID();
  genQScale = genInfo->qScale();
  genAlphaQCD = genInfo->alphaQCD();
  genAlphaQED = genInfo->alphaQED();
  /*
  for ( int i_genJet = 0; i_genJet < nGenJets; i_genJet++ )
  {

    unsigned int match_jet_index = 666;
    double min_delta_r = 666.;

    for (int i_jet = 0; i_jet < nJets; i_jet++)
    {

      double current_delta_r = deltaR(genJetEta[i_genJet],genJetPhi[i_genJet] , jetEta[i_jet], jetPhi[i_jet]);

      if ( current_delta_r < min_delta_r )
      {
        min_delta_r = current_delta_r;
        match_jet_index = i_jet;
      }
     }//end matching to jets
     if ( min_delta_r < 0.3 )
     {
       genJet_match_jet_index[i_genJet] = match_jet_index;
       genJet_min_delta_r_match_jet[i_genJet] = min_delta_r;
     }
   }


*/


    /*
    if (isFastsim_) {

      //get lhe weights for systematic uncertainties:
      double nomlheweight = genInfo->weights()[0];

      //fill scale variation weights
      if (genInfo->weights().size()>=10) {
	for (unsigned int iwgt=1; iwgt<10; ++iwgt) {
	  //normalize to
	  double wgtval = genInfo->weights()[iwgt]*genWeight/genInfo->weights()[1];
	  scaleWeights->push_back(wgtval);
	}
      }

      //fill pdf variation weights
      if (firstPdfWeight>=0 && lastPdfWeight>=0 && lastPdfWeight<int(genInfo->weights().size()) && (lastPdfWeight-firstPdfWeight+1)==100) {

	//fill pdf variation weights after converting with mc2hessian transformation
	std::array<double, 100> inpdfweights;
	for (int iwgt=firstPdfWeight, ipdf=0; iwgt<=lastPdfWeight; ++iwgt, ++ipdf) {
	  inpdfweights[ipdf] = genInfo->weights()[iwgt]/genInfo->weights()[firstPdfWeight-1];
	}

	std::array<double, 60> outpdfweights;
	pdfweightshelper.DoMC2Hessian(inpdfweights.data(),outpdfweights.data());

	for (unsigned int iwgt=0; iwgt<60; ++iwgt) {
	  double wgtval = outpdfweights[iwgt]*genWeight;
	  pdfWeights->push_back(wgtval);
	}

	//fill alpha_s variation weights
	if (firstAlphasWeight>=0 && lastAlphasWeight>=0 && lastAlphasWeight<int(genInfo->weights().size())) {
	  for (int iwgt = firstAlphasWeight; iwgt<=lastAlphasWeight; ++iwgt) {
	    double wgtval = genInfo->weights()[iwgt]*genWeight/nomlheweight;
	    alphasWeights->push_back(wgtval);
	  }
	}

      }
    } else {

      if (lheInfo.isValid() && lheInfo->weights().size()>0) {

	double nomlheweight = lheInfo->weights()[0].wgt;

	//fill scale variation weights
	if (lheInfo->weights().size()>=9) {
	  for (unsigned int iwgt=0; iwgt<9; ++iwgt) {
	    double wgtval = lheInfo->weights()[iwgt].wgt*genWeight/nomlheweight;
	    scaleWeights->push_back(wgtval);
	  }
	}

	//fill pdf variation weights
	if (firstPdfWeight>=0 && lastPdfWeight>=0 && lastPdfWeight<int(lheInfo->weights().size()) && (lastPdfWeight-firstPdfWeight+1)==100) {

	  //fill pdf variation weights after converting with mc2hessian transformation
	  std::array<double, 100> inpdfweights;
	  for (int iwgt=firstPdfWeight, ipdf=0; iwgt<=lastPdfWeight; ++iwgt, ++ipdf) {
	    inpdfweights[ipdf] = lheInfo->weights()[iwgt].wgt/nomlheweight;
	  }

	  std::array<double, 60> outpdfweights;
	  pdfweightshelper.DoMC2Hessian(inpdfweights.data(),outpdfweights.data());

	  for (unsigned int iwgt=0; iwgt<60; ++iwgt) {
	    double wgtval = outpdfweights[iwgt]*genWeight;
	    pdfWeights->push_back(wgtval);
	  }

	  //fill alpha_s variation weights
	  if (firstAlphasWeight>=0 && lastAlphasWeight>=0 && lastAlphasWeight<int(lheInfo->weights().size())) {
	    for (int iwgt = firstAlphasWeight; iwgt<=lastAlphasWeight; ++iwgt) {
	      double wgtval = lheInfo->weights()[iwgt].wgt*genWeight/nomlheweight;
	      alphasWeights->push_back(wgtval);
	    }
	  }
	}
      }
    }

    //fill sum of weights histograms
    sumWeights->Fill(0.,genWeight);

    for (unsigned int iwgt=0; iwgt<scaleWeights->size(); ++iwgt) {
      sumScaleWeights->Fill(double(iwgt),(*scaleWeights)[iwgt]);
    }
    for (unsigned int iwgt=0; iwgt<pdfWeights->size(); ++iwgt) {
      sumPdfWeights->Fill(double(iwgt),(*pdfWeights)[iwgt]);
    }
    for (unsigned int iwgt=0; iwgt<alphasWeights->size(); ++iwgt) {
      sumAlphasWeights->Fill(double(iwgt),(*alphasWeights)[iwgt]);
    }
*/
    return true;
};


bool llp_ntupler::fillGenParticles(){
  std::vector<const reco::Candidate*> prunedV;//Allows easier comparison for mother finding
  //Fills selected gen particles
  //double pt_cut = isFourJet ? 20.:20.;//this needs to be done downstream
  const double pt_cut = 0.0;
  //int llp_id = isFourJet ? 35:9000006;

  for(size_t i=0; i<genParticles->size();i++)
  {
    if( (abs((*genParticles)[i].pdgId()) >= 1 && abs((*genParticles)[i].pdgId()) <= 6 && ( (*genParticles)[i].status() < 30 ))
       || (abs((*genParticles)[i].pdgId()) >= 11 && abs((*genParticles)[i].pdgId()) <= 16)
       || (abs((*genParticles)[i].pdgId()) == 21 && (*genParticles)[i].status() < 30)
       || (abs((*genParticles)[i].pdgId()) >= 22 && abs((*genParticles)[i].pdgId()) <= 25 && ( (*genParticles)[i].status() < 30))
       || (abs((*genParticles)[i].pdgId()) >= 32 && abs((*genParticles)[i].pdgId()) <= 42)
       || (abs((*genParticles)[i].pdgId()) >= 1000001 && abs((*genParticles)[i].pdgId()) <= 1000039)
       || (abs((*genParticles)[i].pdgId()) == 9000006 || abs((*genParticles)[i].pdgId()) == 9000007))
       {
         if ((*genParticles)[i].pt()>pt_cut){
           prunedV.push_back(&(*genParticles)[i]);
         }
       }

  }

  //Total number of gen particles
  nGenParticle = prunedV.size();

  //Look for mother particle and Fill gen variables
  for(unsigned int i = 0; i < prunedV.size(); i++)
  {
    gParticleId[i] = prunedV[i]->pdgId();
    gParticleStatus[i] = prunedV[i]->status();
    gParticleE[i] = prunedV[i]->energy();
    gParticlePt[i] = prunedV[i]->pt();
    gParticlePx[i] = prunedV[i]->px();
    gParticlePy[i] = prunedV[i]->py();
    gParticlePz[i] = prunedV[i]->pz();
    gParticleEta[i] = prunedV[i]->eta();
    gParticlePhi[i] = prunedV[i]->phi();
    gParticleProdVertexX[i] = prunedV[i]->vx();
    gParticleProdVertexY[i] = prunedV[i]->vy();
    gParticleProdVertexZ[i] = prunedV[i]->vz();
    gParticleMotherId[i] = 0;
    gParticleMotherIndex[i] = -1;

    /*
    //For Neutralinos we try to find the decay vertex locaton.
    //Algorithm: Find the first daughter particle that is not a neutralino,
    //and call that the daughter. get the creation vertex of that daughter.
    if ( (gParticleId[i] == 1000022 && gParticleStatus[i] == 22) )
    {
      const reco::Candidate *dau = 0;
      bool foundDaughter = false;
      bool noDaughter = false;
      const reco::Candidate *tmpParticle = prunedV[i];

      while (!foundDaughter && !noDaughter)
      {
        if (tmpParticle->numberOfDaughters() > 0)
        {
          dau = tmpParticle->daughter(0);
          if (dau && dau->pdgId() != 1000022){
            foundDaughter = true;
          }
          else{
            tmpParticle = dau;
          }
        }
        else
        {
          noDaughter = true;
        }
      }

      if (foundDaughter)
      {
        gParticleDecayVertexX[i] = dau->vx();
        gParticleDecayVertexY[i] = dau->vy();
        gParticleDecayVertexZ[i] = dau->vz();
      }
    }
*/

    if(prunedV[i]->numberOfMothers() > 0)
    {
      //find the ID of the first mother that has a different ID than the particle itself
      const reco::Candidate* firstMotherWithDifferentID = findFirstMotherWithDifferentID(prunedV[i]);
      if (firstMotherWithDifferentID)
      {
        gParticleMotherId[i] = firstMotherWithDifferentID->pdgId();
        //gParticleDecayVertexX[i] = firstMotherWithDifferentID->vx();
        //gParticleDecayVertexY[i] = firstMotherWithDifferentID->vy();
        //gParticleDecayVertexZ[i] = firstMotherWithDifferentID->vz();
      }

      //find the mother and keep going up the mother chain if the ID's are the same
      const reco::Candidate* originalMotherWithSameID = findOriginalMotherWithSameID(prunedV[i]);
      for(unsigned int j = 0; j < prunedV.size(); j++)
      {
        if(prunedV[j] == originalMotherWithSameID)
        {
          gParticleMotherIndex[i] = j;
          break;
        }
      }
    }
    else
    {
      gParticleMotherIndex[i] = -1;
    }
/*
    //---------------------------------------
    //Find LLPs production and decay vertices
    //---------------------------------------
    if ( (gParticleId[i] == llp_id || gParticleId[i] == llp_id+1) && gParticleStatus[i] == 22 )
    {
      if (gParticleId[i] == llp_id)
      {
        gLLP_prod_vertex_x[0] = prunedV[i]->vx();
        gLLP_prod_vertex_y[0] = prunedV[i]->vy();
        gLLP_prod_vertex_z[0] = prunedV[i]->vz();
      }
      else if (gParticleId[i] == llp_id+1)
      {
        gLLP_prod_vertex_x[1] = prunedV[i]->vx();
        gLLP_prod_vertex_y[1] = prunedV[i]->vy();
        gLLP_prod_vertex_z[1] = prunedV[i]->vz();
      }

      const reco::Candidate *dau = 0;
      bool foundDaughter = false;
      bool noDaughter = false;
      const reco::Candidate *tmpParticle = prunedV[i];

      while (!foundDaughter && !noDaughter)
      {
        if (tmpParticle->numberOfDaughters() > 0)
        {
          dau = tmpParticle->daughter(0);
          if (dau && (dau->pdgId() != llp_id && dau->pdgId() != llp_id+1))
          {
            foundDaughter = true;
          } else
          {
            tmpParticle = dau;
          }
        }
        else
        {
          noDaughter = true;
        }
      }

      if (foundDaughter)
      {

        if (gParticleId[i] == llp_id)
        {
          gLLP_decay_vertex_x[0] = dau->vx();
          gLLP_decay_vertex_y[0] = dau->vy();
          gLLP_decay_vertex_z[0] = dau->vz();
          gLLP_beta[0] = sqrt(gParticlePx[i]*gParticlePx[i]+gParticlePy[i]*gParticlePy[i]+gParticlePz[i]*gParticlePz[i])/gParticleE[i];
          gLLP_travel_time[0] = sqrt(pow(gLLP_decay_vertex_x[0]-gLLP_prod_vertex_x[0],2)
                                  +pow(gLLP_decay_vertex_y[0]-gLLP_prod_vertex_y[0],2)
                                  +pow(gLLP_decay_vertex_z[0]-gLLP_prod_vertex_z[0],2))/(30. * gLLP_beta[0]);//1/30 is to convert cm to ns
          double radius = sqrt( pow(gLLP_decay_vertex_x[0],2) + pow(gLLP_decay_vertex_y[0],2) );
          double ecal_radius = 129.0;
          double hcal_radius = 179.0;


          for (unsigned int id = 0; id < tmpParticle->numberOfDaughters(); id++ )
          {
          //std::cout << "====================" << std::endl;
          //std::cout << " -> "<< tmpParticle->daughter(id)->pdgId() << std::endl;
            if( id > 1 ) break;
            TLorentzVector tmp;
            tmp.SetPxPyPzE(tmpParticle->daughter(id)->px(), tmpParticle->daughter(id)->py(), tmpParticle->daughter(id)->pz(), tmpParticle->daughter(id)->energy());
            if(tmp.Pt()<pt_cut) continue;
            gLLP_daughter_pt[id] = tmp.Pt();
            gLLP_daughter_eta[id] = tmp.Eta();
            gLLP_daughter_phi[id] = tmp.Phi();
            gLLP_daughter_e[id]  = tmp.E();

            double gLLP_daughter_travel_time_hcal= (1./30.)*(hcal_radius-radius)/(tmp.Pt()/tmp.E());// - (1./30.) * ecal_radius * cosh(tmp.Eta());//1/30 is to convert cm to ns

            gLLP_daughter_travel_time[id] = (1./30.)*(ecal_radius-radius)/(tmp.Pt()/tmp.E());// - (1./30.) * ecal_radius * cosh(tmp.Eta());//1/30 is to convert cm to ns
          //Calculate dt from generation point to ECAL face
            double x_ecal = gLLP_decay_vertex_x[0] + 30. * (tmp.Px()/tmp.E())*gLLP_daughter_travel_time[id];
            double y_ecal = gLLP_decay_vertex_y[0] + 30. * (tmp.Py()/tmp.E())*gLLP_daughter_travel_time[id];
            double z_ecal = gLLP_decay_vertex_z[0] + 30. * (tmp.Pz()/tmp.E())*gLLP_daughter_travel_time[id];
            double x_hcal = gLLP_decay_vertex_x[0] + 30. * (tmp.Px()/tmp.E())*gLLP_daughter_travel_time_hcal;
            double y_hcal = gLLP_decay_vertex_y[0] + 30. * (tmp.Py()/tmp.E())*gLLP_daughter_travel_time_hcal;
            double z_hcal = gLLP_decay_vertex_z[0] + 30. * (tmp.Pz()/tmp.E())*gLLP_daughter_travel_time_hcal;

      //if( fabs(z_ecal) < 10 && radius <= 1)
            if( fabs(z_ecal) < 271.6561246934 && radius <= ecal_radius)
    	      {
    	        photon_travel_time[id] = (1./30) * sqrt(pow(ecal_radius,2)+pow(z_ecal,2));
              photon_travel_time_pv[id] = (1./30) * sqrt(pow(x_ecal-genVertexX,2) + pow(y_ecal-genVertexY,2) + pow(z_ecal-genVertexZ,2));
              gen_time_pv[id] =  gLLP_travel_time[0] + gLLP_daughter_travel_time[id] - photon_travel_time_pv[id] + genVertexT;
              gen_time[id] = gLLP_travel_time[0] + gLLP_daughter_travel_time[id] - photon_travel_time[id] + genVertexT;

            }
            else
            {
              gLLP_daughter_travel_time[id] = -666;
              gen_time_pv[id] = -666.;
              gen_time[id] = -666.;
              photon_travel_time[id] = -666.;
              photon_travel_time_pv[id] = -666.;
            }
    	      double min_delta_r = 666.;
    	      double min_delta_r_nocorr = 666.;
            double min_delta_r_hcal = 666.;
    	      unsigned int match_jet_index = 666;
            unsigned int match_jet_index_hcal = 666;

    	      double genJet_min_delta_r = 666.;
    	      unsigned int match_genJet_index = 666;

        // Correction of eta and phi based on ecal points
    	      double phi = atan((y_ecal-genVertexY)/(x_ecal-genVertexX));
            if  (x_ecal < 0.0){
              phi = TMath::Pi() + phi;
    	      }
    	      phi = deltaPhi(phi,0.0);
    	      double theta = atan(sqrt(pow(x_ecal-genVertexX,2)+pow(y_ecal-genVertexY,2))/abs(z_ecal-genVertexZ));
            double eta = -1.0*TMath::Sign(1.0, z_ecal-genVertexZ)*log(tan(theta/2));
    	      gLLP_daughter_eta_ecalcorr[id] = eta;
            gLLP_daughter_phi_ecalcorr[id] = phi;

        // Correction of eta and phi based on hcal points
            phi = atan((y_hcal-genVertexY)/(x_hcal-genVertexX));
            if  (x_hcal < 0.0){
              phi = TMath::Pi()  + phi;
            }
            phi = deltaPhi(phi,0.0);
            theta = atan(sqrt(pow(x_hcal-genVertexX,2)+pow(y_hcal-genVertexY,2))/abs(z_hcal-genVertexZ));
            eta = -1.0*TMath::Sign(1.0, z_hcal-genVertexZ)*log(tan(theta/2));
            gLLP_daughter_eta_hcalcorr[id] = eta;
            gLLP_daughter_phi_hcalcorr[id] = phi;


    	      for ( int i_jet = 0; i_jet < nGenJets; i_jet++)
    	      {
    		       double genJet_current_delta_r = deltaR(gLLP_daughter_eta[id], gLLP_daughter_phi[id],  genJetEta[i_jet], genJetPhi[i_jet]);
    	        //std::cout << i_jet << " current dR = " << genJet_current_delta_r << eta<<phi<<theta<<tan(theta/2.0)<<log(tan(theta/2.0))<<std::endl;
            	if ( genJet_current_delta_r < genJet_min_delta_r )
            	{
                genJet_min_delta_r = genJet_current_delta_r;
            		match_genJet_index = i_jet;
            		  //std::cout << i_jet << " min dR = " << genJet_min_delta_r << std::endl;
            	}
    	      }//end matching to genJets
    	      for ( int i_jet = 0; i_jet < nJets; i_jet++ )
    	      {
  		        double current_delta_r = deltaR(gLLP_daughter_eta_ecalcorr[id], gLLP_daughter_phi_ecalcorr[id], jetEta[i_jet], jetPhi[i_jet]);
  	          if ( current_delta_r < min_delta_r )
    	        {
    	  	      min_delta_r_nocorr = deltaR(gLLP_daughter_eta[id], gLLP_daughter_phi[id], jetEta[i_jet], jetPhi[i_jet]);
    		        min_delta_r = current_delta_r;
    		        match_jet_index = i_jet;
    		  //std::cout << i_jet << " min dR = " << min_delta_r << std::endl;
    	        }
    	      }//end matching to jets using ECAL radius
            for ( int i_jet = 0; i_jet < nJets; i_jet++ )
    	      {
  		        double current_delta_r = deltaR(gLLP_daughter_eta_hcalcorr[id], gLLP_daughter_phi_hcalcorr[id], jetEta[i_jet], jetPhi[i_jet]);
  	          if ( current_delta_r < min_delta_r_hcal )
    	        {
    		        min_delta_r_hcal = current_delta_r;
    		        match_jet_index_hcal = i_jet;
    	        }
    	      }//end matching to jets using HCAL radius
    	      if ( min_delta_r < 0.3 )
    	      {
    	        gLLP_daughter_match_jet_index[id] = match_jet_index;
    	        gLLP_min_delta_r_match_jet[id] = min_delta_r;
    	        gLLP_min_delta_r_nocorr_match_jet[id] = min_delta_r_nocorr;
    	      }
            if ( min_delta_r < 0.45 )
            {
              gLLP_daughter_match_jet_index_loose[id] = match_jet_index;
              gLLP_min_delta_r_match_jet_loose[id] = min_delta_r;
            }
            if ( min_delta_r_hcal < 0.3 )
    	      {
    	        gLLP_daughter_match_jet_index_hcal[id] = match_jet_index_hcal;
    	        gLLP_min_delta_r_match_jet_hcal[id] = min_delta_r_hcal;
    	      }
            if ( min_delta_r_hcal < 0.45 )
    	      {
    	        gLLP_daughter_match_jet_index_hcal_loose[id] = match_jet_index_hcal;
    	        gLLP_min_delta_r_match_jet_hcal_loose[id] = min_delta_r_hcal;
    	      }
    	      if ( genJet_min_delta_r < 0.3 )
    	      {
    	        gLLP_daughter_match_genJet_index[id] = match_genJet_index;
    	        gLLP_min_delta_r_match_genJet[id] = genJet_min_delta_r;
    	        //std::cout << "min dR = " << min_delta_r << " matched to jet index " << match_jet_index << std::endl;
    	      }

          }
        }
    	  else if (gParticleId[i] == llp_id+1)
    	  {
    	    gLLP_decay_vertex_x[1] = dau->vx();
    	    gLLP_decay_vertex_y[1] = dau->vy();
    	    gLLP_decay_vertex_z[1] = dau->vz();
    	    gLLP_beta[1] = sqrt(gParticlePx[i]*gParticlePx[i]+gParticlePy[i]*gParticlePy[i]+gParticlePz[i]*gParticlePz[i])/gParticleE[i];
    	    gLLP_travel_time[1] = sqrt(pow(gLLP_decay_vertex_x[1]-gLLP_prod_vertex_x[1],2)
    				      +pow(gLLP_decay_vertex_y[1]-gLLP_prod_vertex_y[1],2)
    				      +pow(gLLP_decay_vertex_z[1]-gLLP_prod_vertex_z[1],2))/(30. * gLLP_beta[1]);//1/30 is to convert cm to ns
    	    double radius = sqrt( pow(gLLP_decay_vertex_x[1],2) + pow(gLLP_decay_vertex_y[1],2) );
    	    double ecal_radius = 129.0;
          double hcal_radius = 179.0;
    	    //--------------------------------------------------
    	    //Second two LLP daughters belong to LLP->pdgID()=36
          //--------------------------------------------------
    	    for (unsigned int id = 0; id < tmpParticle->numberOfDaughters(); id++ )
    	    {
    	      //std::cout << " -> "<< tmpParticle->daughter(id)->pdgId() << std::endl;
    	      if( id > 1 ) break;
    	      TLorentzVector tmp;
    	      tmp.SetPxPyPzE(tmpParticle->daughter(id)->px(), tmpParticle->daughter(id)->py(), tmpParticle->daughter(id)->pz(), tmpParticle->daughter(id)->energy());
            if(tmp.Pt()<pt_cut) continue;
            gLLP_daughter_pt[id+2] = tmp.Pt();
    	      gLLP_daughter_eta[id+2] = tmp.Eta();
    	      gLLP_daughter_phi[id+2] = tmp.Phi();
    	      gLLP_daughter_e[id+2]  = tmp.E();
    	      //gLLP_daughter_travel_time[id+2] = (1./30.)*(ecal_radius-radius)/(tmp.Pt()/tmp.E()) - (1./30.) * ecal_radius * cosh(tmp.Eta());//1/30 is to convert cm to ns
            double gLLP_daughter_travel_time_hcal = (1./30.)*(hcal_radius-radius)/(tmp.Pt()/tmp.E());// - (1./30.) * ecal_radius * cosh(tmp.Eta());//1/30 is to convert cm to ns
            gLLP_daughter_travel_time[id+2] = (1./30.)*(ecal_radius-radius)/(tmp.Pt()/tmp.E());// - (1./30.) * ecal_radius * cosh(tmp.Eta());//1/30 is to convert cm to ns

    	      //Calculate dt from generation point to ECAL face
    	      double x_ecal = gLLP_decay_vertex_x[1] + 30. * (tmp.Px()/tmp.E())*gLLP_daughter_travel_time[id+2];
    	      double y_ecal = gLLP_decay_vertex_y[1] + 30. * (tmp.Py()/tmp.E())*gLLP_daughter_travel_time[id+2];
    	      double z_ecal = gLLP_decay_vertex_z[1] + 30. * (tmp.Pz()/tmp.E())*gLLP_daughter_travel_time[id+2];

            double x_hcal = gLLP_decay_vertex_x[1] + 30. * (tmp.Px()/tmp.E())*gLLP_daughter_travel_time_hcal;
    	      double y_hcal = gLLP_decay_vertex_y[1] + 30. * (tmp.Py()/tmp.E())*gLLP_daughter_travel_time_hcal;
    	      double z_hcal = gLLP_decay_vertex_z[1] + 30. * (tmp.Pz()/tmp.E())*gLLP_daughter_travel_time_hcal;
    	      if( fabs(z_ecal) < 271.6561246934 && radius <= ecal_radius)
    	      // if( fabs(z_ecal) < 10 && radius <= 0.1)
    	      {
              photon_travel_time[id+2] = (1./30) * sqrt(pow(ecal_radius,2)+pow(z_ecal,2));
              photon_travel_time_pv[id+2] = (1./30) * sqrt(pow(x_ecal-genVertexX,2) + pow(y_ecal-genVertexY,2) + pow(z_ecal-genVertexZ,2));
              gen_time_pv[id+2] =  gLLP_travel_time[1] + gLLP_daughter_travel_time[id+2] - photon_travel_time_pv[id+2] + genVertexT;
              gen_time[id+2] = gLLP_travel_time[1] + gLLP_daughter_travel_time[id+2] - photon_travel_time[id+2] + genVertexT;

    	      }
    	      else
    	      {
    	        gLLP_daughter_travel_time[id+2] = -666;
              gen_time_pv[id+2] = -666.;
              gen_time[id+2] = -666.;
              photon_travel_time[id+2] = -666.;
              photon_travel_time_pv[id+2] = -666.;
    	      }
    	      double genJet_min_delta_r = 666.;
            unsigned int match_genJet_index = 666;
    	      double min_delta_r = 666.;
            double min_delta_r_hcal = 666.;
    	      double min_delta_r_nocorr = 666.;
    	      unsigned int match_jet_index = 666;
            unsigned int match_jet_index_hcal = 666;

            //Corrections for angles based on ECAL radius
    	      double phi = atan((y_ecal-genVertexY)/(x_ecal-genVertexX));
            if  (x_ecal < 0.0){
              phi = TMath::Pi() + phi;
            }
    	      phi = deltaPhi(phi,0.0);
    	      double theta = atan(sqrt(pow(x_ecal-genVertexX,2)+pow(y_ecal-genVertexY,2))/abs(z_ecal-genVertexZ));
    	      double eta = -1.0*TMath::Sign(1.0,z_ecal-genVertexZ)*log(tan(theta/2));
    	      gLLP_daughter_eta_ecalcorr[id+2] = eta;
    	      gLLP_daughter_phi_ecalcorr[id+2] = phi;

            //Corrections for angles based on HCAL radius
            phi = atan((y_hcal-genVertexY)/(x_hcal-genVertexX));
            if  (x_hcal < 0.0){
              phi = TMath::Pi() + phi;
            }
            phi = deltaPhi(phi,0.0);
            theta = atan(sqrt(pow(x_hcal-genVertexX,2)+pow(y_hcal-genVertexY,2))/abs(z_hcal-genVertexZ));
            eta = -1.0*TMath::Sign(1.0,z_ecal-genVertexZ)*log(tan(theta/2));
            gLLP_daughter_eta_hcalcorr[id+2] = eta;
            gLLP_daughter_phi_hcalcorr[id+2] = phi;
    	      for ( int i_jet = 0; i_jet < nGenJets; i_jet++ )
    	      {
      		    double genJet_current_delta_r = deltaR(gLLP_daughter_eta[id+2], gLLP_daughter_phi[id+2],genJetEta[i_jet], genJetPhi[i_jet]);
      	      if ( genJet_current_delta_r < genJet_min_delta_r )
      	      {
    	  	      genJet_min_delta_r = genJet_current_delta_r;
    		        match_genJet_index = i_jet;
      		  //std::cout << i_jet << " min dR = " << min_delta_r << std::endl;
      	      }
    	      }//end matching to genJets
    	      for ( int i_jet = 0; i_jet < nJets; i_jet++ )
    	      {
              double current_delta_r = deltaR(gLLP_daughter_eta_ecalcorr[id+2], gLLP_daughter_phi_ecalcorr[id+2] , jetEta[i_jet], jetPhi[i_jet]);
          		if ( current_delta_r < min_delta_r )
          		{
          		  min_delta_r_nocorr = deltaR(gLLP_daughter_eta[id+2], gLLP_daughter_phi[id+2], jetEta[i_jet], jetPhi[i_jet]);
          		  min_delta_r = current_delta_r;
          		  match_jet_index = i_jet;
          		}
    	      }//end matching to jets ecal
            for ( int i_jet = 0; i_jet < nJets; i_jet++ )
    	      {
              double current_delta_r = deltaR(gLLP_daughter_eta_hcalcorr[id+2], gLLP_daughter_phi_hcalcorr[id+2], jetEta[i_jet], jetPhi[i_jet]);
          		if ( current_delta_r < min_delta_r_hcal )
          		{
          		  min_delta_r_hcal = current_delta_r;
          		  match_jet_index_hcal = i_jet;
          		}
    	      }//end matching to jets hcal
            if ( min_delta_r < 0.3 )
    	      {
    	        gLLP_daughter_match_jet_index[id+2] = match_jet_index;
    	        gLLP_min_delta_r_match_jet[id+2] = min_delta_r;
    	        gLLP_min_delta_r_nocorr_match_jet[id] = min_delta_r_nocorr;
    	      }
            if ( min_delta_r < 0.45 )
            {
              gLLP_daughter_match_jet_index_loose[id+2] = match_jet_index;
              gLLP_min_delta_r_match_jet_loose[id+2] = min_delta_r;
            }
            if ( min_delta_r_hcal < 0.3 )
    	      {
    	        gLLP_daughter_match_jet_index_hcal[id+2] = match_jet_index_hcal;
    	        gLLP_min_delta_r_match_jet_hcal[id+2] = min_delta_r_hcal;
    	      }
            if ( min_delta_r_hcal < 0.3 )
    	      {
    	        gLLP_daughter_match_jet_index_hcal_loose[id+2] = match_jet_index_hcal;
    	        gLLP_min_delta_r_match_jet_hcal_loose[id+2] = min_delta_r_hcal;
    	      }
    	      if ( genJet_min_delta_r < 0.3 )
    	      {
    	        gLLP_daughter_match_genJet_index[id+2] = match_genJet_index;
    	        gLLP_min_delta_r_match_genJet[id+2] = genJet_min_delta_r;
    	        //std::cout << "min dR = " << min_delta_r << " matched to jet index " << match_jet_index << std::endl;
    	      }
    	    }//for daughters loop
    	  }//if particle ID = 36
    	}//if found daughters
    }

  */
    /*
    //----------------------
    //QCD Matching
    //----------------------
    if (isQCD_) {
      if (abs(gParticleId[i])  <= 6 || abs(gParticleId[i]) == 21)
      {
	      if (gParticleStatus[i] == 23)
	      {
          const reco::Candidate *tmpParticle = prunedV[i];
          TLorentzVector tmp;
          tmp.SetPxPyPzE(tmpParticle->px(), tmpParticle->py(), tmpParticle->pz(), tmpParticle->energy());
          genQCD_pt[nGenQCDParticles] = tmp.Pt();
          genQCD_eta[nGenQCDParticles] = tmp.Eta();
          genQCD_phi[nGenQCDParticles] = tmp.Phi();
          genQCD_e[nGenQCDParticles]  = tmp.E();

          double min_delta_r = 666.;
          unsigned int match_jet_index = 666;

          for ( int i_jet = 0; i_jet < nJets; i_jet++ )
          {
            double current_delta_r = deltaR(genQCD_eta[nGenQCDParticles], genQCD_phi[nGenQCDParticles], jetEta[i_jet], jetPhi[i_jet]);
            if ( current_delta_r < min_delta_r )
            {
              min_delta_r = current_delta_r;
              match_jet_index = i_jet;
            }
          }//end matching to jets hcal
          if ( min_delta_r < 0.3 )
          {
            genParticleQCD_match_jet_index[nGenQCDParticles] = match_jet_index;
            genParticleQCD_min_delta_r_match_jet[nGenQCDParticles] = min_delta_r;
          }
          nGenQCDParticles ++;
        }
      }
    }
    */
  }// for loop of genParticles
  return true;
};



bool llp_ntupler::fillTrigger(const edm::Event& iEvent)
{

  //fill trigger information
  const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
  // std::cout << "\n === TRIGGER PATHS === " << std::endl;
  //------------------------------------------------------------------
  //Option to save all HLT path names in the ntuple per event
  //Expensive option in terms of ntuple size
  //------------------------------------------------------------------
  nameHLT->clear();
  for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i)
  {
    string hltPathNameReq = "HLT_";
    //if (triggerBits->accept(i))
    if ((names.triggerName(i)).find(hltPathNameReq) != string::npos) nameHLT->push_back(names.triggerName(i));
    /*
    std::cout << "Trigger " << names.triggerName(i) <<
    ", prescale " << triggerPrescales->getPrescaleForIndex(i) <<
    ": " << (triggerBits->accept(i) ? "PASS" : "fail (or not run)")
    << std::endl;
    if ((names.triggerName(i)).find(hltPathNameReq) != string::npos && triggerBits->accept(i)) std::cout << "Trigger " << names.triggerName(i) <<
    ": " << (triggerBits->accept(i) ? "PASS" : "fail (or not run)")
    << std::endl;
    */
  }
  //std::cout << "n triggers: " <<  nameHLT->size() << std::endl;
  //std::cout << "====================" << std::endl;
  //for ( unsigned int i = 0; i < nameHLT->size(); i++ )
  //{
  //  std::cout << i << " -> " << nameHLT->at(i) << std::endl;
  //}
  //------------------------------------------------------------------
  // Save trigger decisions in array of booleans
  //------------------------------------------------------------------
  for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i)
  {
    string hltPathNameReq = "HLT_";
    if ((names.triggerName(i)).find(hltPathNameReq) == string::npos) continue;
    if ((names.triggerName(i)).find_last_of("_") == string::npos) continue;
    int lastUnderscorePos = (names.triggerName(i)).find_last_of("_");
    string hltPathNameWithoutVersionNumber = (names.triggerName(i)).substr(0,lastUnderscorePos);

    for (unsigned int j = 0; j < NTriggersMAX; ++j)
    {
      if (triggerPathNames[j] == "") continue;
      if (hltPathNameWithoutVersionNumber == triggerPathNames[j])
      {
        triggerDecision[j] = triggerBits->accept(i);
        //triggerHLTPrescale[j] = triggerPrescales->getPrescaleForIndex(i);
      }
    }
  }
  //------------------------------------------------------------------
  // Print Trigger Objects
  //------------------------------------------------------------------
/*
  for (pat::TriggerObjectStandAlone trigObject : *triggerObjects)
  {
    //cout << "triggerObj: " << trigObject.pt() << " " << trigObject.eta() << " " << trigObject.phi() << "\n";
    //bool foundRazor = false;
    //Need to unpack the filter labels before checking
    trigObject.unpackFilterLabels(iEvent, *triggerBits);
    for(int j=0; j<int(trigObject.filterLabels().size());j++)
    {
      //if ((trigObject.filterLabels())[j] == "hltRsqMR200Rsq0p0196MR100Calo") foundRazor = true;
      // trigObject.unpackPathNames(names);
      // cout << "filter: " << (trigObject.pathNames())[j] << " " << (trigObject.filterLabels())[j] << "\n";
      //cout << "filter: " << (trigObject.filterLabels())[j] << "\n";
    }
  }
*/
//define this as a plug-in
  return true;
};
DEFINE_FWK_MODULE(llp_ntupler);
