///
///  \file   MMEventHits.hh
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 Sept
///


#ifndef MMEventHits_HH
#define MMEventHits_HH

#include "include/MMFE8Hits.hh"

class MMEventHits {

public:
  MMEventHits();
  MMEventHits(const MMHit& hit);
  MMEventHits(const MMFE8Hits& hits);
  MMEventHits(const MMEventHits& event_hits);
  
  ~MMEventHits();

  bool AddHit(const MMHit& hit);
  bool AddHits(const MMFE8Hits& hits);
  bool AddLinkedHit(const MMLinkedHit& hit);

  bool operator += (const MMHit& hit);
  bool operator += (const MMFE8Hits& hits);
  
  int GetNBoards() const;
  int MMFE8(int iboard) const;
  MMFE8Hits const& Get(int iboard) const;
  MMFE8Hits const& operator [] (int iboard) const;

  int GetNDuplicates() const;

  MMEventHits GetDuplicates() const;

  void SetTime(int time, int time_ns);    
  double Time();

  void SetEventNum(std::vector<int> evt);    
  int EventNum(int ib, int ivmm);
  
  friend class PDOToCharge;
  friend class TDOToTime;
  
private:
  std::vector<MMFE8Hits*> m_boards;
  double m_time;
  std::vector<int> m_evt;
};

inline MMEventHits::MMEventHits() {}

inline MMEventHits::MMEventHits(const MMHit& hit){
  AddHit(hit);
}
inline MMEventHits::MMEventHits(const MMFE8Hits& hits){
  AddHits(hits);
}

inline MMEventHits::MMEventHits(const MMEventHits& evt_hits){
  int Nboard = evt_hits.GetNBoards();
  for(int i = 0; i < Nboard; i++)
    AddHits(evt_hits[i]);
}
  
inline MMEventHits::~MMEventHits(){
  int N = GetNBoards();
  for(int i = 0; i < N; i++)
    delete m_boards[i];
}

inline bool MMEventHits::AddHit(const MMHit& hit){
  int Nboard = GetNBoards();
  for(int i = 0; i < Nboard; i++)
    if(m_boards[i]->AddHit(hit))
      return true;

  m_boards.push_back(new MMFE8Hits(hit));
  return true;
}

inline bool MMEventHits::AddLinkedHit(const MMLinkedHit& hit){
  int Nboard = GetNBoards();
  for(int i = 0; i < Nboard; i++)
    if(m_boards[i]->AddLinkedHit(hit))
      return true;
  
  m_boards.push_back(new MMFE8Hits(hit));
  return true;
}

inline bool MMEventHits::AddHits(const MMFE8Hits& hits){
  int Nboard = GetNBoards();
  for(int i = 0; i < Nboard; i++)
    if(m_boards[i]->IsSameMMFE8(hits))
      if(m_boards[i]->AddHits(hits))
	return true;
  m_boards.push_back(new MMFE8Hits(hits));
  return true;
}

inline bool MMEventHits::operator += (const MMHit& hit){
  return AddHit(hit);
}
inline bool MMEventHits::operator += (const MMFE8Hits& hits){
  return AddHits(hits);
}
  
inline int MMEventHits::MMFE8(int iboard) const {
  if(iboard < 0 || iboard >= GetNBoards())
    return -1;

  return m_boards[0]->MMFE8();
}

inline int MMEventHits::GetNBoards() const {
  return int(m_boards.size());
}

inline MMFE8Hits const& MMEventHits::Get(int iboard) const {
  return *m_boards[iboard];
}

inline MMFE8Hits const& MMEventHits::operator [] (int iboard) const {
  return Get(iboard);
}

inline int MMEventHits::GetNDuplicates() const {
  int Ndup = 0;
  int N = GetNBoards();
  for(int i = 0; i < N; i++)
    Ndup += m_boards[i]->GetNDuplicates();
  
  return Ndup;
}

inline MMEventHits MMEventHits::GetDuplicates() const {
  MMEventHits dups;
  int N = GetNBoards();
  for(int i = 0; i < N; i++)
    dups += m_boards[i]->GetDuplicates();
  
  return dups;
}

inline void MMEventHits::SetTime(int time, int time_ns){
  m_time = time+time_ns/pow(10,9.);
}

inline double MMEventHits::Time(){
  return m_time;
}

inline void MMEventHits::SetEventNum(std::vector<int> evt){
  m_evt = evt;
}

inline int MMEventHits::EventNum(int ib, int ivmm){
  return m_evt[ib*8+ivmm];
}

inline MMEventHits operator + (const MMEventHits& evt_hits, 
			       const MMFE8Hits& hits){
  MMEventHits ret(evt_hits);
  ret += hits;
  return ret;
}

inline MMEventHits operator + (const MMEventHits& evt_hits, 
			       const MMHit& hit){
  MMEventHits ret(evt_hits);
  ret += hit;
  return ret;
}

inline MMEventHits operator + (const MMFE8Hits& hits, 
			       const MMEventHits& evt_hits){
  MMEventHits ret(evt_hits);
  ret += hits;
  return ret;
}

inline MMEventHits operator + (const MMHit& hit, 
			       const MMEventHits& evt_hits){
  MMEventHits ret(evt_hits);
  ret += hit;
  return ret;
}

#endif
