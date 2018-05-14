///
///  \file   RunTBAnalysis.C
///
///  \author A. Wang, based on C. Rogan + A. Tuna
///
///  \date   25 April 2018
///

#include "TH1D.h"
#include "TH2D.h"
#include <iostream>

#include "include/PDOToCharge.hh"
#include "include/TDOToTime.hh"
#include "include/MMDataAnalysis.hh"
#include "include/MMRunProperties.hh"
#include "include/MMPacmanAlgo.hh"
//#include "include/GeoOctuplet.hh"
//#include "include/SimpleTrackFitter.hh"

using namespace std;

int main(int argc, char* argv[]){

  bool skip_transition = true;


  char inputFileName[400];
  char outputFileName[400];
  char PDOFileName[400];
  char TDOFileName[400];
  
  if ( argc < 5 ){
    cout << "Error at Input: please specify input/output .root files ";
    cout << " and (optional) PDO/TDO calibration files" << endl;
    cout << "Example:   ./RunMMAnalysisTemplate.x -i input.root -o output.root" << endl;
    cout << "Example:   ./RunMMAnalysisTemplate.x -i input.root -o output.root";
    cout << " -p PDOcalib.root -t TDOcalib.root" << endl;
    return 0;
  }

  bool b_input = false;
  bool b_out   = false;
  bool b_pdo   = false;
  bool b_tdo   = false;
  for (int i=1;i<argc-1;i++){
    if (strncmp(argv[i],"-i",2)==0){
      sscanf(argv[i+1],"%s", inputFileName);
      b_input = true;
    }
    if (strncmp(argv[i],"-o",2)==0){
      sscanf(argv[i+1],"%s", outputFileName);
      b_out = true;
    }
    if (strncmp(argv[i],"-p",2)==0){
      sscanf(argv[i+1],"%s", PDOFileName);
      b_pdo = true;
    }
    if (strncmp(argv[i],"-t",2)==0){
      sscanf(argv[i+1],"%s", TDOFileName);
      b_tdo = true;
    }
  }

  if(!b_input){
    cout << "Error at Input: please specify input file (-i flag)" << endl;
    return 0;
  }

  if(!b_out){
    cout << "Error at Input: please specify output file (-o flag)" << endl;
    return 0;
  }


  // PDO calibration object
  PDOToCharge* PDOCalibrator;
  if(b_pdo)
    PDOCalibrator = new PDOToCharge(PDOFileName);
  else
    PDOCalibrator = new PDOToCharge();

  // TDO calibration object
  TDOToTime* TDOCalibrator;
  if(b_tdo)
    TDOCalibrator = new TDOToTime(TDOFileName);
  else
    TDOCalibrator = new TDOToTime();

  // clustering algorithm object
  //MMPacmanAlgo* PACMAN = new MMPacmanAlgo(2,2.,0.5);
  MMPacmanAlgo* PACMAN = new MMPacmanAlgo(5,5.,2.);

  // data object
  MMDataAnalysis* DATA;
  TFile* f = new TFile(inputFileName, "READ");
  if(!f){
    cout << "Error: unable to open input file " << inputFileName << endl;
    return false;
  }
  TTree* T = (TTree*) f->Get("vmm");
  TTree* R = (TTree*) f->Get("run_properties");
  if(!T){
    cout << "Error: cannot find tree vmm in " << inputFileName << endl;
    return false;
  }

  if(!R){
    cout << "Error: cannot find tree run_properties in " << inputFileName << endl;
    return false;
  }

  MMRunProperties mm_RunProperties = MMRunProperties(R);                                     
  mm_RunProperties.GetEntry(0);                                                                                                    
  int m_RunNum = mm_RunProperties.runNumber;  
  DATA = (MMDataAnalysis*) new MMDataAnalysis(T, m_RunNum);

  int ibo = 0;
  int nboards = 2;
  std::map< string, TH1D* > h1;
  std::map< string, TH2D* > h2;
  
  h2["strip_position_vs_board"] = new TH2D("strip_position_vs_board", ";strip number;MMFE number;charge [fC]", 64, 0.5, 64.5, 8, -0.5, 7.5);

  for (ibo = 0; ibo < nboards; ibo++){
    h2[Form("strip_q_vs_ch_%i",    ibo)] = new TH2D(Form("strip_q_vs_ch_%i",    ibo), ";strip number;Charge [fC];strip",    64, -0.5, 63.5, 512,   0,  128);
    h2[Form("strip_pdo_vs_ch_%i",  ibo)] = new TH2D(Form("strip_pdo_vs_ch_%i",  ibo), ";strip number;PDO [counts];strip",   64, -0.5, 63.5, 512,   0, 2048);
    h2[Form("strip_tdo_vs_ch_%i",  ibo)] = new TH2D(Form("strip_tdo_vs_ch_%i",  ibo), ";strip number;TDO [counts];strip",   64, -0.5, 63.5, 256,   0,  256);
    h2[Form("strip_tdoc_vs_ch_%i", ibo)] = new TH2D(Form("strip_tdoc_vs_ch_%i", ibo), ";strip number;TDO corr. [ns];strip", 64, -0.5, 63.5, 200, -5,  60);
    h2[Form("strip_time_vs_ch_%i", ibo)] = new TH2D(Form("strip_time_vs_ch_%i", ibo), ";strip number;Time [ns];strip",      64, -0.5, 63.5, 400, -200., 600);
    h2[Form("strip_zpos_vs_ch_%i", ibo)] = new TH2D(Form("strip_zpos_vs_ch_%i", ibo), ";strip number;z_{drift} [mm];strip", 64, -0.5, 63.5, 150, -10,   20);
    h2[Form("strip_bcid_vs_ch_%i", ibo)] = new TH2D(Form("strip_bcid_vs_ch_%i", ibo), ";strip number;BCID [mm];strip",      64, -0.5, 63.5, 4096, -0.5,   4095);
    h2[Form("strip_dbc_vs_ch_%i", ibo)]  = new TH2D(Form("strip_dbc_vs_ch_%i", ibo), ";strip number;#Delta BCID [mm];strip",      64, -0.5, 63.5, 8191, -4095.5,   4095.5);
    h1[Form("x_bary_%i", ibo)]           = new TH1D(Form("x_bary_%i", ibo),          ";x_{bary}; Events", 800, -0.5, 40);
  }
  h1["tdo_gain"] = new TH1D("tdo_gain", "tdo_gain", 100,   0, 3);
  h1["tdo_ped"]  = new TH1D("tdo_ped",  "tdo_ped",  100, -10, 50);
  h1["pdo_gain"] = new TH1D("pdo_gain", "pdo_gain", 100,   0, 30);
  h1["pdo_ped"]  = new TH1D("pdo_ped",  "pdo_ped",  100, -100, 300);

  h2["dtrigBCID_vs_evt"] = new TH2D("dtrigBCID_vs_evt", "dtrigBCID_vs_evt", 10000,-0.5, 9999.5, 8191,-4095.5,4095.5); 
  h2["dtrigBCIDrel_vs_evt"] = new TH2D("dtrigBCIDrel_vs_evt", "dtrigBCIDrel_vs_evt", 10000,-0.5, 9999.5, 8191,-4095.5,4095.5); 

  h1["track_diff01_bary"] = new TH1D("track_diff01_bary", ";x_{bary,0} - x_{bary,1}; Tracks", 800, -20, 20);
  h2["clus_vs_board"]          = new TH2D("clus_vs_board",          ";MMFE number;clusters;Events",            2, -0.5, 1.5, 32, -0.5, 31.5);
  h2["hits_vs_board"]          = new TH2D("hits_vs_board",          ";MMFE number;strips;Events",              2, -0.5, 1.5, 32, -0.5, 31.5);
  h2["trighits_vs_board"]      = new TH2D("trighits_vs_board",          ";MMFE number;hits;Events",              2, -0.5, 1.5, 32, -0.5, 31.5);
  h2["hits_per_clus_vs_board"] = new TH2D("hits_per_clus_vs_board", ";MMFE number;hits in a cluster;Clusters", 2, -0.5, 1.5, 25, -0.5, 24.5);
  h2["hits_per_clus_vs_board_fid"] = new TH2D("hits_per_clus_vs_board_fid", ";MMFE number;hits in a cluster;Clusters", 2, -0.5, 1.5, 25, -0.5, 24.5);

  // collecting clusters and the nominal fit                                                                                                                                         
  std::vector<MMClusterList> clusters_perboard;
  MMClusterList clusters_all;
  MMClusterList clusters_road;
  MMClusterList clusters_x;

  double vdrift = 1.0 / 20; // mm per ns

  int Nevent = DATA->GetNEntries();

  // open output file
  TFile* fout = new TFile(outputFileName, "RECREATE");
  // set style for plotting

  int last_diff = -1;

  for(int evt = 0; evt < Nevent; evt++){
    DATA->GetEntry(evt);
    if(evt%10000 == 0)
      cout << "Processing event # " << evt << " | " << Nevent << endl;

//     if (evt > 10000)
//        break;
    for (auto clus_list: clusters_perboard)
      clus_list.Reset();
    clusters_perboard.clear();

    // DATA->mm_EventHits (MMEventHits class) is the collection
    // of MM hits (MMHit class) for the event
    
    // Calibrate PDO -> Charge
    PDOCalibrator->Calibrate(DATA->mm_EventHits);
    // Calibrate TDO -> Time
    TDOCalibrator->Calibrate(DATA->mm_EventHits);
  
    // initialize PACMAN info for this event
    PACMAN->SetEventTrigBCID(-1);

    // how many duplicate hits in the event
    // (number of hits with at least 1 dup)
    int Ndup_evt = DATA->mm_EventHits.GetNDuplicates();
    
    int dBCID = DATA->mm_EventHits.TrigTimeL0BCID(2,0)- DATA->mm_EventHits.TrigTimeL0BCID(3,0);
    int dBCIDrel = DATA->mm_EventHits.TrigTimeBCID(2,0)- DATA->mm_EventHits.TrigTimeBCID(3,0);
    //std::cout << "bcid1: " << DATA->mm_EventHits.TrigTimeBCID(2,0) << ", bcid2: " << DATA->mm_EventHits.TrigTimeBCID(3,0) << std::endl;
    if (last_diff != -1 && dBCIDrel != last_diff && skip_transition){
      last_diff = dBCIDrel;
      continue;
    }
    last_diff = dBCIDrel;
    h2["dtrigBCID_vs_evt"]->Fill(evt,dBCID);
    h2["dtrigBCIDrel_vs_evt"]->Fill(evt,dBCIDrel);

    // run pacman
    int nboardshit = DATA->mm_EventHits.GetNBoards();
    for(int i = 0; i < nboardshit; i++){
      if(DATA->mm_EventHits[i].GetNHits() == 0)
        continue;
      MMClusterList board_clusters = PACMAN->Cluster(DATA->mm_EventHits[i]);
      if (board_clusters.GetNCluster() > 0)
        clusters_perboard.push_back(board_clusters);
      //      else{
      //std::cout << "eeeeek: " << evt << std::endl;
      //      }
      // strips: PDO vs channel
      // its slow. only run if hella desired.
      for(int ich = 0; ich < DATA->mm_EventHits[i].GetNHits(); ich++){
        auto hit = DATA->mm_EventHits[i][ich];
        ibo = hit.MMFE8Index();                                                                                                                                          

        if (hit.Channel() == 63)
          h2["trighits_vs_board"]->Fill(ibo,hit.GetNHits());

        if( !PACMAN->IsGoodHit(hit) )
          continue;

        h1["tdo_gain"]->Fill(hit.TDOGain());
        h1["tdo_ped"] ->Fill(hit.TDOPed());
        h1["pdo_gain"]->Fill(hit.PDOGain());
        h1["pdo_ped"] ->Fill(hit.PDOPed());

        h2[Form("strip_tdoc_vs_ch_%i", ibo)]->Fill(hit.Channel(), hit.Time()+20);
        h2[Form("strip_time_vs_ch_%i", ibo)]->Fill(hit.Channel(), hit.DriftTime(30., 0));
        h2[Form("strip_zpos_vs_ch_%i", ibo)]->Fill(hit.Channel(), hit.DriftTime(30., 0) * vdrift);

        h2[Form("strip_q_vs_ch_%i",    ibo)]->Fill(hit.Channel(), hit.Charge());
        h2[Form("strip_pdo_vs_ch_%i",  ibo)]->Fill(hit.Channel(), hit.PDO());
        h2[Form("strip_tdo_vs_ch_%i",  ibo)]->Fill(hit.Channel(), hit.TDO());
        h2[Form("strip_bcid_vs_ch_%i", ibo)]->Fill(hit.Channel(), hit.BCID());
        h2[Form("strip_dbc_vs_ch_%i",  ibo)]->Fill(hit.Channel(), hit.BCID() - DATA->mm_EventHits.TrigTimeBCID(hit.MMFE8(),0));
      }
    }

    int test;
    if (clusters_perboard.size() < 1) {
      for (int ipl = 0; ipl < nboards; ipl++){
        h2["clus_vs_board"]->Fill(ipl, 0);
        h2["hits_vs_board"]->Fill(ipl, 0);
      }
      continue;
    }
    // hits, duplicates, clusters per board
    // ------------------------------------
    for (int ipl = 0; ipl < nboards; ipl++){
      test = -1;
      for (int i = 0; i < nboardshit; i++){
        // hits on this board!
        ibo = DATA->mm_EventHits[i].MMFE8Index();
        if (ipl == ibo){
          test = i;
          //std::cout << "foundhits!, cluster size" << clusters_perboard[i].size()<< std::endl;
          h2["clus_vs_board"]->Fill(ipl, clusters_perboard[i].size());
          h2["hits_vs_board"]->Fill(ipl, DATA->mm_EventHits[i].GetNHits());
          //std::cout << "filledhits!" << ipl<< std::endl;
        }
      }
      // no hits on this board!
      if (test == -1){
        //std::cout << "no hits!" << std::endl;
        h2["clus_vs_board"]->Fill(ipl, 0);
        h2["hits_vs_board"]->Fill(ipl, 0);
        //h2["dups_vs_board"]->Fill(ibo, 0);
      }
    }
    
    //continue;

    // barycenter board_i vs board_j
    int hit_0 = 0, hit_1 = 0, hit_6 = 0, hit_7 = 0;
    double x_i = 0;
    double x_j = 0;
    double dx = 0;

    for (int i = 0; i < clusters_perboard.size(); i++){
      for (int j = 0; j < clusters_perboard[i].size(); j++){
        const MMCluster& clus = clusters_perboard[i][j];
        if (clus.MMFE8() == 2){
          ibo = 0;
        }
        else {
          ibo = 1;
        }
        h1[Form("x_bary_%i", ibo)]->Fill(clus.Channel()*0.4);
//         if (clus.GetNHits() < 2)
//           continue;
        h2["hits_per_clus_vs_board"]->Fill(ibo, clus.GetNHits());
        if (clusters_perboard[i].size() > 1)
          continue;
        if (ibo == 0) {
          hit_0 = 1;
          x_i = clus.Channel()*0.4;
        }
        else if (ibo == 1) {
          hit_1 = 1;
          x_j = clus.Channel()*0.4;
        }
        if ( ( hit_0 && (x_i < 0.8) ) || 
               ( hit_0 && (x_i > 23.6) ) ||
             (hit_1 && (x_j < 0.8 )) || 
              (hit_1 && (x_j > 23.6) ) ) {
          continue;
        }
        h2["hits_per_clus_vs_board_fid"]->Fill(ibo, clus.GetNHits());
      }
    }

    // fiducial cut!
    
    if ( (x_i < 0.8 || x_i > 23.6) || (x_j < 0.8 || x_j > 23.6) ) 
      continue;
    
    if (hit_1 && hit_0) {
      dx = x_i - x_j - 1.2 ;
      h1["track_diff01_bary"]->Fill(dx);
    }
  }
  fout->cd();
  fout->mkdir("histograms");
  fout->cd("histograms");

  for (auto kv: h1)
    kv.second->Write();
  for (auto kv: h2)
    kv.second->Write();
  fout->Close();
}
