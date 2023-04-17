// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
///
/// \brief This task is an empty skeleton that fills a simple eta histogram.
///        it is meant to be a blank page for further developments.
/// \author
/// \since

#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"
#include "Common/DataModel/TrackSelectionTables.h"

using namespace o2;
using namespace o2::framework;

//STEP 0
//This is an empty analysis skeleton: the starting point! 
struct taskskeleton {

  Configurable<int> selectedTracks{"select", 1, "Choice of track selection. 0 = no selection, 1 = globalTracks, 2 = globalTracksSDD"};
  Configurable<int> noOfPtBins{"noOfPtBins", 100, "No of pT bins"};
  
  HistogramRegistry registry{
    "registry",
    {
      {"eta", "#eta", {HistType::kTH1F, {{102, -2.01, 2.01}}}},     //
      {"phi", "#varphi", {HistType::kTH1F, {{100, 0., 2. * M_PI}}}}, //
      {"pT", "pT", {HistType::kTH1F, {{noOfPtBins, 0., 5.}}}}, //
      {"trackDCA", "trackDCA", {HistType::kTH1F, {{50, 0., 5.}}}}, //
      {"zvtx", "zvtx", {HistType::kTH1F, {{50, -10, 10.}}}} //
    }                                                               //
  };
  
  void process(aod::Collision const& collision, soa::Join<aod::TracksIU, aod::TracksExtra, aod::TracksDCA> const& tracks)
  {
    registry.get<TH1>(HIST("zvtx"))->Fill(collision.posZ());
    
    for (auto& track : tracks) {
      
      // select tracks according to configurable selectedTracks
      // if (selectedTracks == 1 && !track.isGlobalTrack()) {
      //   continue;
      // } else if (selectedTracks == 2 && !track.isGlobalTrackSDD()) {
      //   continue;
      // }
      //track selections
      if(track.tpcNClsCrossedRows() < 70) continue;
      if(fabs(track.dcaXY()) > .2 ) continue;
      //filling monitor histograms
      registry.get<TH1>(HIST("eta"))->Fill(track.eta());
      registry.get<TH1>(HIST("pT"))->Fill(track.pt());
      registry.get<TH1>(HIST("phi"))->Fill(track.phi());
      registry.get<TH1>(HIST("trackDCA"))->Fill(track.dcaXY());
    }
  }
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    adaptAnalysisTask<taskskeleton>(cfgc)
  };
}
