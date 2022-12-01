import FWCore.ParameterSet.Config as cms

import EventFilter.CSCRawToDigi.cscUnpacker_cfi
muonCSCDigis = EventFilter.CSCRawToDigi.cscUnpacker_cfi.muonCSCDigis.clone()
muonCSCDigis.InputObjects = 'rawDataRepacker'

import EventFilter.DTRawToDigi.dtunpacker_cfi
muonDTDigis = EventFilter.DTRawToDigi.dtunpacker_cfi.muonDTDigis.clone()
muonDTDigis.inputLabel = 'rawDataRepacker'

from RecoLocalMuon.CSCRecHitD.cscRecHitD_cfi import *
from RecoLocalMuon.DTRecHit.dt1DRecHits_LinearDriftFromDB_cfi import *

muonSystemClusterSelSeq = cms.Sequence(
			   muonCSCDigis * muonDTDigis * dt1DRecHits * csc2DRecHits
                           )

