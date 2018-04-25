//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Apr 24 11:57:35 2018 by ROOT version 6.08/06
// from TTree run_properties/run_properties
// found on file: ../run_0139.root
//////////////////////////////////////////////////////////

#ifndef MMRunProperties_h
#define MMRunProperties_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class MMRunProperties {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           runNumber;
   Int_t           gain;
   Int_t           tacSlope;
   Int_t           peakTime;
   Int_t           dacCounts;
   Int_t           pulserCounts;
   Int_t           tpSkew;
   Int_t           ckbc;
   Int_t           angle;
   Bool_t          calibrationRun;

   // List of branches
   TBranch        *b_runNumber;   //!
   TBranch        *b_gain;   //!
   TBranch        *b_tacSlope;   //!
   TBranch        *b_peakTime;   //!
   TBranch        *b_dacCounts;   //!
   TBranch        *b_pulserCounts;   //!
   TBranch        *b_tpSkew;   //!
   TBranch        *b_ckbc;   //!
   TBranch        *b_angle;   //!
   TBranch        *b_calibrationRun;   //!

   MMRunProperties(TTree *tree=0);
   virtual ~MMRunProperties();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

inline MMRunProperties::MMRunProperties(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../run_0139.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../run_0139.root");
      }
      f->GetObject("run_properties",tree);

   }
   Init(tree);
}

inline MMRunProperties::~MMRunProperties()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MMRunProperties::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
inline Long64_t MMRunProperties::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

inline void MMRunProperties::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).


   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);


   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("gain", &gain, &b_gain);
   fChain->SetBranchAddress("tacSlope", &tacSlope, &b_tacSlope);
   fChain->SetBranchAddress("peakTime", &peakTime, &b_peakTime);
   fChain->SetBranchAddress("dacCounts", &dacCounts, &b_dacCounts);
   fChain->SetBranchAddress("pulserCounts", &pulserCounts, &b_pulserCounts);
   fChain->SetBranchAddress("tpSkew", &tpSkew, &b_tpSkew);
   fChain->SetBranchAddress("ckbc", &ckbc, &b_ckbc);
   fChain->SetBranchAddress("angle", &angle, &b_angle);
   fChain->SetBranchAddress("calibrationRun", &calibrationRun, &b_calibrationRun);
   Notify();
}

inline Bool_t MMRunProperties::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

inline void MMRunProperties::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
inline Int_t MMRunProperties::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
