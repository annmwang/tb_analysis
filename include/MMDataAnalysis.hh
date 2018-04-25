///
///  \file   MMDataAnalysis.hh
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 Sept
///


#ifndef MMDataAnalysis_HH
#define MMDataAnalysis_HH

#include "include/MMDataBaseTestBeam.hh"
//#include "include/MMRunProperties.hh"
#include "include/MMEventHits.hh"

class MMDataAnalysis : public MMDataBaseTestBeam {

public:
  MMDataAnalysis(TTree *tree=0, int runnum=-1);
  ~MMDataAnalysis();

  virtual Int_t GetNEntries();
  virtual Int_t GetEntry(Long64_t entry);
  virtual Int_t GetTP();
  virtual void  SetTP(Int_t doTP);
  //  virtual void  LoadRunProperties(TTree *rtree=0);

  MMEventHits mm_EventHits;
  //  MMRunProperties mm_RunProperties;

private:
  int m_Nentry;
  int m_doTP;
  int m_RunNum = -1;
};

#endif

inline MMDataAnalysis::MMDataAnalysis(TTree *tree, int runnum)
  : MMDataBaseTestBeam(tree)
{
  if(tree)
    m_Nentry = tree->GetEntries();
  else
    m_Nentry = 0;
  m_doTP = 1;
  m_RunNum = runnum;
}
  
inline MMDataAnalysis::~MMDataAnalysis() {}

inline Int_t MMDataAnalysis::GetNEntries(){
  return m_Nentry;
}

inline Int_t MMDataAnalysis::GetTP(){
  return m_doTP;
}

inline void MMDataAnalysis::SetTP(int doTP){
  m_doTP = doTP;
}

inline Int_t MMDataAnalysis::GetEntry(Long64_t entry){

  if(entry < 0 || entry >= m_Nentry)
    return false;
  

  int ret = MMDataBaseTestBeam::GetEntry(entry);

  // clear previous event micromega hits;
  mm_EventHits = MMEventHits();
  mm_EventHits.SetTime(-1.,-1.);
  //mm_EventHits.SetTime(mm_Time_sec,mm_Time_nsec);
  mm_EventHits.SetEventNum(*triggerCounter);
  // fill event hits
  for(int i = 0; i < chip->size(); i++){
    for(int j = 0; j < channel->at(i).size(); j++){
      MMHit hit(boardId->at(i),
                chip->at(i),
                channel->at(i).at(j),
                m_RunNum);
      hit.SetPDO(pdo->at(i).at(j));
      hit.SetTDO(tdo->at(i).at(j));
      hit.SetBCID(bcid->at(i).at(j));

      mm_EventHits += hit;
    }
  }

  return ret;
}

// inline void MMDataAnalysis::LoadRunProperties(TTree *rtree){
//   mm_RunProperties = MMRunProperties(rtree);
//   std::cout << "try to get first entry" << std::endl;
//   std::cout << rtree->GetEntries() << std::endl;
//   mm_RunProperties.GetEntry(0);
//   std::cout << "success" << std::endl;
//   m_RunNum = mm_RunProperties.runNumber;
// }
