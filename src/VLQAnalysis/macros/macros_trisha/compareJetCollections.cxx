#include "TChain.h"
#include "TTree.h"
#include "TTreeIndex.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TH2D.h"
#include "TH1D.h"

#include <vector>
#include <map>
#include <string>
#include <iostream>

std::map<std::string, TFile*> infile;
std::map<std::string, TChain*> ch;

//Branches
std::map<std::string,long int> event_number;
std::map<std::string, std::vector<float>* > jets_pt;
std::map<std::string, std::vector<float>* > jets_eta;
std::map<std::string, std::vector<float>* > jets_phi;

std::map<std::string, std::vector<float>* > fwjets_pt;
std::map<std::string, std::vector<float>* > fwjets_eta;
std::map<std::string, std::vector<float>* > fwjets_phi;

std::map<std::string, TTreeIndex*> treeindex;

int SetVariableToChain(const std::string& chname, const std::string& varname, void* variable){

  ch[chname]->SetBranchStatus(varname.c_str(), 1);

  TBranch* branch = 0;
  int stat = ch[chname]->SetBranchAddress(varname.c_str(), variable, &branch);
  return stat;

}

//int main(int argc, char** argv){
int main(){
  //std::cout << "CHECK 0 " << std::endl;
  std::string basedir="/nfs/at3/scratch2/farooque/JetColComp/";
  for(const std::string jetcol : {"EMTopo", "EMPFlow"}){

    //std::cout << "CHECK 1 " << std::endl;
    //user.farooque.410470.mc16e.ttbar_EMPFlow.DAOD_TOPQ1.e6337_s3126_r10724_p3832.21.2.87-htztx-3-PFlow_output_tree.root
    //user.farooque.410470.mc16e.ttbar_EMTopo.DAOD_TOPQ1.e6337_s3126_r10724_p3832.21.2.87-htztx-3-EMTopo_output_tree.root

    //infile[jetcol] = TFile::Open((basedir+"sub_"+jetcol+"/data-output_tree/mc16_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_TOPQ1.e6337_s3126_r9364_p3832.root").c_str(), "READ");
    //ch[jetcol] = (TTree*)(infile[jetcol]->Get("nominal"));

    ch[jetcol] = new TChain("nominal");
    ch[jetcol]->Add((basedir+"sub_"+jetcol+"/data-output_tree/*.root").c_str());


    //std::cout << "CHECK 2 ch[jetcol] : " << ch[jetcol] << std::endl;
    //std::cout << "CHECK 2 ch[jetcol] : " << ch[jetcol] << " chain nentries : " << ch[jetcol]->GetEntries() << std::endl;

    event_number[jetcol] = 0;
    jets_pt[jetcol] = 0;
    jets_eta[jetcol] = 0;
    jets_phi[jetcol] = 0;
    fwjets_pt[jetcol] = 0;
    fwjets_eta[jetcol] = 0;
    fwjets_phi[jetcol] = 0;

    ch[jetcol]->BuildIndex("event_number");
    treeindex[jetcol] = (TTreeIndex*)ch[jetcol]->GetTreeIndex();
    //std::cout << "CHECK 3 jetcol : " << jetcol << " treeindex : " << treeindex[jetcol] << std::endl;

    SetVariableToChain(jetcol, "event_number", &(event_number[jetcol]));
    SetVariableToChain(jetcol, "jets_pt", &(jets_pt[jetcol]));
    SetVariableToChain(jetcol, "jets_eta", &(jets_eta[jetcol]));
    SetVariableToChain(jetcol, "jets_phi", &(jets_phi[jetcol]));
    SetVariableToChain(jetcol, "fwjets_pt", &(fwjets_pt[jetcol]));
    SetVariableToChain(jetcol, "fwjets_eta", &(fwjets_eta[jetcol]));
    SetVariableToChain(jetcol, "fwjets_phi", &(fwjets_phi[jetcol]));


  }

  //std::cout << "CHECK 4 " << std::endl;

  std::string ref_jetcol = "EMTopo";
  std::string probe_jetcol = "EMPFlow";

  TH2D* ptmatch_vs_ptref = new TH2D("ptmatch_vs_ptref", "", 100,0.,1000.,100,0.,1000.); 
  TH2D* matchratio_vs_ptref = new TH2D("matchratio_vs_ptref", "", 100,0.,1000.,200,0.,2.); 

  TH2D* fmatch_vs_ptref = new TH2D("fmatch_vs_ptref", "", 100,0.,1000.,2,0.,1.);
  //std::cout << "CHECK 5 ref_jetcol : " << ref_jetcol << " treeindex : " << treeindex[ref_jetcol] << std::endl;
  std::cout << "CHECK 5 ref_jetcol : " << ref_jetcol << " treeindex : " << treeindex[ref_jetcol] << " N : " << treeindex[ref_jetcol]->GetN() << std::endl;

  for(int iref = 0; iref< treeindex[ref_jetcol]->GetN(); iref++){

    if(iref%500==0) std::cout << "iref :  " << iref << std::endl;
    std::cout << " treeindex[ref_jetcol]->GetIndex()[iref] : " << treeindex[ref_jetcol]->GetIndex()[iref] << std::endl;
    //Long64_t ref_local = ch[ref_jetcol]->LoadTree( treeindex[ref_jetcol]->GetIndex()[iref] );
    //ch[ref_jetcol]->GetEntry(ref_local);
    ch[ref_jetcol]->GetEntry(treeindex[ref_jetcol]->GetIndex()[iref]);
    if(fwjets_pt[ref_jetcol]->size() <= 0) continue;
    
    //std::cout << "CHECK 7 " << std::endl;

    int iprobe_hist = 0;
    for(int iprobe = iprobe_hist; iprobe< treeindex[probe_jetcol]->GetN(); iprobe++){

      //std::cout << "CHECK 7 probe_jetcol : " << probe_jetcol << " treeindex : " << treeindex[probe_jetcol] << std::endl;
      //std::cout << "CHECK 7 probe_jetcol : " << probe_jetcol << " treeindex : " << treeindex[probe_jetcol] << " N : " << treeindex[probe_jetcol]->GetN() << std::endl;
      
      //Long64_t probe_local = ch[probe_jetcol]->LoadTree( treeindex[probe_jetcol]->GetIndex()[iprobe] );
      //ch[probe_jetcol]->GetEntry(probe_local);
      ch[probe_jetcol]->GetEntry(treeindex[probe_jetcol]->GetIndex()[iprobe]);

      if(event_number[probe_jetcol] < event_number[ref_jetcol]) continue;
      if(event_number[probe_jetcol] > event_number[ref_jetcol]) break;
      iprobe_hist++;

      //std::cout << "CHECK 8 " << std::endl;

      //start the jet matching loop
      for(unsigned int jref = 0; jref < fwjets_pt[ref_jetcol]->size(); jref++){
	double ref_jetpt = fwjets_pt[ref_jetcol]->at(jref);
	double ref_jeteta = fwjets_eta[ref_jetcol]->at(jref);
	double ref_jetphi = fwjets_phi[ref_jetcol]->at(jref);

	//std::cout << "CHECK 9 " << std::endl;

	TLorentzVector tlv_ref;
	tlv_ref.SetPtEtaPhiM(ref_jetpt, ref_jeteta, ref_jetphi, 0.);

	double drmin = 0.4; int jmatch = -1; double ptmatch = -999.;
	for(unsigned int jprobe = 0; jprobe < fwjets_pt[probe_jetcol]->size(); jprobe++){
	  //std::cout << "CHECK 10 " << std::endl;

	  double probe_jetpt = fwjets_pt[probe_jetcol]->at(jprobe);
	  double probe_jeteta = fwjets_eta[probe_jetcol]->at(jprobe);
	  double probe_jetphi = fwjets_phi[probe_jetcol]->at(jprobe);
	  //std::cout << "CHECK 11 " << std::endl;

	  TLorentzVector tlv_probe;
	  tlv_probe.SetPtEtaPhiM(probe_jetpt, probe_jeteta, probe_jetphi, 0.);
	  
	  double drpair = tlv_ref.DeltaR(tlv_probe);
	  if(drpair<drmin){
	    drmin=drpair; jmatch=jprobe; ptmatch=probe_jetpt;
	  }
	  //std::cout << "CHECK 12 " << std::endl;

	}//probe jets
	//std::cout << "CHECK 13 " << std::endl;

	//if match found
	int nmatch = 0;
	if(jmatch>=0){
	  nmatch++;
	  ptmatch_vs_ptref->Fill(ref_jetpt, ptmatch);
	  matchratio_vs_ptref->Fill(ref_jetpt, ptmatch/ref_jetpt);
	}
	fmatch_vs_ptref->Fill(ref_jetpt, nmatch);

	//std::cout << "CHECK 14 " << std::endl;

      }//rejets

    }//probe event loop


  }//ref event loop

  TFile* matchfile = TFile::Open("matchfile.root","RECREATE");
  matchfile->cd();
  ptmatch_vs_ptref->Write();
  matchratio_vs_ptref->Write();
  fmatch_vs_ptref->Write();
  matchfile->Close();


  return 0;

}
