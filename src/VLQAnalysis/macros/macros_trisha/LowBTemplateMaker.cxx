#include <TH1D.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <vector>
#include <string>
#include <iostream>


TFile* m_infile;
TFile* m_outfile;

bool m_useOnlyMuons;
double m_sc_etomu = 1.;
std::vector<std::string> m_variables{"meff", "hthad", "met_zoom", "mtw_zoom", "ptw_zoom", 
    "RCTTMass_jets_n", "jets_n", "jets_pt", "jets_eta", "lep0_pt_zoom", "lep0_eta", 
    "mtbmin_lowb_3b_zoom", "mtbmin_lowb_4b_zoom"};

std::vector<std::string> m_jets{"", "5j", "6j"};
std::vector<std::string> m_toptags{"2RCTTMass","1RCTTMass","0RCTTMass"};
std::vector<std::string> m_target_btags{"1b"};//, "2b", "3b", "4b"};
std::vector<std::string> m_target_btags_6j_noPresel{};//"3bHighMbb", "3bLowMbb", "4bHighMbb", "4bLowMbb"};

std::vector<std::string> m_source_btags{"0b", "1b", "2b", "3b", "4b"};

std::vector<std::string> m_source_btags_3bHighMbb{};//"0bHighMbb_3b","1bHighMbb_3b","2bHighMbb_3b", "3bHighMbb"};
std::vector<std::string> m_source_btags_4bHighMbb{};//"0bHighMbb_4b","1bHighMbb_4b","2bHighMbb_4b", "4bHighMbb"};
std::vector<std::string> m_source_btags_3bLowMbb{};//"0bLowMbb_3b","1bLowMbb_3b","2bLowMbb_3b", "3bLowMbb"};
std::vector<std::string> m_source_btags_4bLowMbb{};//"0bLowMbb_4b","1bLowMbb_4b","2bLowMbb_4b", "4bLowMbb"};
std::vector<std::string> m_lepchannels{"", "_e", "_mu"};

TH1D* MakeLowBSourceTemplate(const std::string& variable, const std::string& ch_jet, const std::string& ch_lep,
			     const std::string& ch_topjet="", const std::string& ch_mbb="" );
void MakeLowBTemplate(const std::string& variable, TH1D* temp_lowb, const std::string& ch_jet, const std::string& ch_bjet, const std::string& ch_lep,
		      const std::string& ch_topjet="" );

 
int main(int argc, char** argv){

  if(argc<2){ std::cout<<"Provide name of file from which to read inputs; name of file to write to"<<std::endl; }

  m_infile = TFile::Open(argv[1], "READ");
  m_outfile = TFile::Open(argv[2], "RECREATE");

  m_useOnlyMuons = atoi(argv[3]) > 0;

  //Find scaling factor;
  m_sc_etomu = 1.;
  double e_sc = 1.; double mu_sc = 1.;

  TH1D* presel_el = ((TH1D*)(m_infile->Get("c1l2b_e_meff")));
  if(presel_el){ e_sc = presel_el->Integral(); }
  else{ std::cerr << " Cannot find preselection electron histogram " << std::endl; exit(1); }
  presel_el->SetDirectory(0);

  TH1D* presel_mu = ((TH1D*)(m_infile->Get("c1l2b_mu_meff")));
  if(presel_mu){ mu_sc = presel_mu->Integral(); }
  else{ std::cerr << " Cannot find preselection muon histogram " << std::endl; exit(1); }
  presel_mu->SetDirectory(0);

  m_sc_etomu = e_sc/mu_sc;

  delete presel_el; delete presel_mu;

  //------------------ 0b -----------------
  //Find scaling factor;
  double m_sc_etomu_0b = 1.;
  double e_sc_0b = 1.; double mu_sc_0b = 1.;

  TH1D* presel_0b_el = ((TH1D*)(m_infile->Get("c1l0b_e_meff")));
  if(presel_0b_el){ e_sc_0b = presel_0b_el->Integral(); }
  else{ std::cerr << " Cannot find preselection electron histogram " << std::endl; exit(1); }
  presel_0b_el->SetDirectory(0);

  TH1D* presel_0b_mu = ((TH1D*)(m_infile->Get("c1l0b_mu_meff")));
  if(presel_0b_mu){ mu_sc_0b = presel_0b_mu->Integral(); }
  else{ std::cerr << " Cannot find preselection muon histogram " << std::endl; exit(1); }
  presel_0b_mu->SetDirectory(0);

  m_sc_etomu_0b = e_sc_0b/mu_sc_0b;


  //------------------- 1b  ---------------

  //Find scaling factor;
  double m_sc_etomu_1b = 1.;
  double e_sc_1b = 1.; double mu_sc_1b = 1.;

  TH1D* presel_1b_el = ((TH1D*)(m_infile->Get("c1l1b_e_meff")));
  if(presel_1b_el){ e_sc_1b = presel_1b_el->Integral(); }
  else{ std::cerr << " Cannot find preselection electron histogram " << std::endl; exit(1); }
  presel_1b_el->SetDirectory(0);

  TH1D* presel_1b_mu = ((TH1D*)(m_infile->Get("c1l1b_mu_meff")));
  if(presel_1b_mu){ mu_sc_1b = presel_1b_mu->Integral(); }
  else{ std::cerr << " Cannot find preselection muon histogram " << std::endl; exit(1); }
  presel_1b_mu->SetDirectory(0);

  m_sc_etomu_1b = e_sc_1b/mu_sc_1b;

  std::cout<< "m_sc_etomu = "<<m_sc_etomu<<std::endl;
  std::cout<< "m_sc_etomu_0b = "<<m_sc_etomu_0b<<std::endl;
  std::cout<< "m_sc_etomu_1b = "<<m_sc_etomu_1b<<std::endl;
  m_sc_etomu = m_sc_etomu_1b; //ALERTRISHA

  std::string chjet_ie = ""; std::string chbjet_ie="";

  for(const std::string var : m_variables){

    //----------------------------------------

    for( const std::string chjet : m_jets ){
      if(chjet.empty()){ chjet_ie = ""; }
      else{ chjet_ie = (chjet == "5j") ? chjet+"e" : chjet+"i"; }

      for(const std::string chlep : m_lepchannels){

	//========= Presel ==========================
	TH1D* temp_lowb = NULL;
	temp_lowb = MakeLowBSourceTemplate(var, chjet_ie, chlep);

	for(const std::string chbjet : m_target_btags){
	  if(chjet.empty() ){ 
	    if( (chbjet == "3b") || (chbjet == "4b") ){ continue; } 
	    chbjet_ie = chbjet;
	  }
	  else{ chbjet_ie = (chbjet == "4b") ? chbjet+"i" : chbjet+"e"; }

	  MakeLowBTemplate(var, temp_lowb, chjet_ie, chbjet_ie, chlep);
	  chbjet_ie.clear();
	}
	m_outfile->cd();
	temp_lowb->Write(); 
	delete temp_lowb;

	if(chjet.empty() ){ continue; } //Only preselection if no jet multiplicity specified

	std::cout<<"  ============= Preselection Done for chjet = "<<chjet<<" chlep = "<<chlep<<std::endl;
	//============ Analysis regions ================================
	for(const std::string chtopjet : m_toptags){

	  //No Mbb splitting first
	  temp_lowb = MakeLowBSourceTemplate(var, chjet, chlep, chtopjet);
	  for(const std::string chbjet : m_target_btags){
	    MakeLowBTemplate(var, temp_lowb, chjet, chbjet, chlep, chtopjet);
	  }
	  m_outfile->cd();
	  temp_lowb->Write(); 
	  delete temp_lowb;

	  if(chjet == "6j"){
	    //Mbb splitting
	    for(const std::string chmbb : m_target_btags_6j_noPresel){
	      temp_lowb = MakeLowBSourceTemplate(var, chjet, chlep, chtopjet, chmbb); 
	      MakeLowBTemplate(var, temp_lowb, chjet, chmbb, chlep, chtopjet);
	      m_outfile->cd();
	      temp_lowb->Write(); 
	      delete temp_lowb;
	    }//Mbb channels
	  }//6 jet channels

	}//top-tag channels

      }//lepton channels

      chjet_ie.clear();
    }//jet channels
  }//variable loop
  m_outfile->Close();
  m_infile->Close();
  return 0; 
}

//Return the summed low-b source template
TH1D* MakeLowBSourceTemplate(const std::string& variable, const std::string& ch_jet, const std::string& ch_lep,
			     const std::string& ch_topjet, const std::string& ch_mbb){


  //std::cout<<"Inside MakeLowBSourceTemplate ::  ch_jet = "<<ch_jet<<" ch_lep = "<<ch_lep<<" ch_topjet = "<<ch_topjet<<" ch_mbb = "<<ch_mbb<<" variable = "<<variable<<std::endl;   
  //----------------------------------------------------------------------------------------------
  TH1D* temp_lowb=NULL;

  std::vector<std::string>* source_btags = &m_source_btags;

  
  if     (ch_mbb == "3bHighMbb"){ source_btags = &m_source_btags_3bHighMbb; }
  else if(ch_mbb == "4bHighMbb"){ source_btags = &m_source_btags_4bHighMbb; }
  else if(ch_mbb == "3bLowMbb") { source_btags = &m_source_btags_3bLowMbb; }
  else if(ch_mbb == "4bLowMbb") { source_btags = &m_source_btags_4bLowMbb; }

  //Make a "source" low-b template by looping over the source b-tag regions and adding them together
  std::string chbjet_ie = "";
  for(const std::string chb_lowb : *source_btags){

    if(ch_jet.empty()){ 
      if( (chb_lowb == "3b") || (chb_lowb == "4b") ){ continue; }
      chbjet_ie = chb_lowb;
    }
    else{
      if(ch_topjet == ""){
	chbjet_ie = (chb_lowb == "4b") ? chb_lowb +"i" : chb_lowb+"e";
      }
      else{ chbjet_ie = chb_lowb; }
    }
    std::string var_lowb = (m_useOnlyMuons) ? "c1l"+ch_topjet+ch_jet+chbjet_ie+"_mu"+"_"+variable : "c1l"+ch_topjet+ch_jet+chbjet_ie+ch_lep+"_"+variable;
    if(temp_lowb==NULL){
      temp_lowb = (TH1D*)(m_infile->Get(var_lowb.c_str()));
      temp_lowb->SetDirectory(0);
    }
    else{ temp_lowb -> Add ( (TH1D*)(m_infile->Get(var_lowb.c_str())) ); }
    var_lowb.clear();
  }
  std::string var_lowb_NEW = "source_c1l"+ch_topjet+ch_jet+ch_mbb+ch_lep+"_"+variable;

  temp_lowb->SetName(var_lowb_NEW.c_str());
  var_lowb_NEW.clear();
  //--------------------------------------------------------------------------------------------
  source_btags = NULL;
  return temp_lowb;

}

void MakeLowBTemplate(const std::string& variable, TH1D* temp_lowb, const std::string& ch_jet, const std::string& ch_bjet, const std::string& ch_lep,
		      const std::string& ch_topjet){

  std::string target_temp_name = "c1l"+ch_topjet+ch_jet+ch_bjet+ch_lep+"_"+variable;
  std::string target_dist = (m_useOnlyMuons) ? "c1l"+ch_topjet+ch_jet+ch_bjet+"_mu_"+variable : target_temp_name;
  std::string target_dist_NEW = "orig_c1l"+ch_topjet+ch_jet+ch_bjet+ch_lep+"_"+variable;

  TH1D* targethist = (TH1D*)(m_infile->Get(target_dist.c_str()));
  targethist->SetDirectory(0); //because this also needs to be written to the new file
  targethist->SetName(target_dist_NEW.c_str());
  double target_intgl = targethist->Integral();

  TH1D* target_temp = (TH1D*)(temp_lowb->Clone(target_dist.c_str()));
  target_temp->SetDirectory(0);
  double source_intgl = temp_lowb->Integral();
 
  if( source_intgl > 0){
    target_temp->Scale(target_intgl/source_intgl);
  }

  //If using muon channel only, do appropriate scaling
  if(m_useOnlyMuons){
    if(ch_lep=="_e"){ target_temp->Scale(m_sc_etomu); }
    if(ch_lep=="")  { target_temp->Scale(1+m_sc_etomu); }
  }

  target_temp->SetName(target_temp_name.c_str());

  m_outfile->cd();
  targethist->Write();
  target_temp->Write();

  return;

}

  
  
