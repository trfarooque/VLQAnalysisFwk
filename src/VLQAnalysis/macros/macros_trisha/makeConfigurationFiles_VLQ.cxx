#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <TFile.h>
#include <TH1.h>
#include <TChain.h>

std::string m_hist_file_prefix = "/afs/cern.ch/work/f/farooque/VLQ2016/PreselectionHISTS/outVLQConf";
std::string m_hist_file_suffix = "nominal.root";
bool m_make_file_list = false;
bool m_make_sample_config = false;
bool m_make_variable_config = false;

std::vector<std::string> m_sample_list{ "Data", "ttbarlight", "ttbarcc", "ttbarbb", "singletop", "Wjets", "Zjets", "topEW", "diboson", "fourtops", "SUM" };

//jet and lepton multiplicities for log plots
//std::vector<std::string> m_variable_list{ "jets_n", "bjets_n", "rcjets_n", "RCTTMass_jets_n", "el_n", "mu_n", "lep_n", "baseline_lep_n", "baseline_el_n", "baseline_mu_n" };


//std::vector<std::string> m_variable_list{ "rcjets_n", "rcjets_pt", "rcjets_eta", "rcjets_m", "rcjets_nconsts", 
//    "fjets_n", "fjets_pt", "fjets_eta", "fjets_m"};//, "baseline_lep_n", "baseline_el_n", "baseline_mu_n" };


std::vector<std::string> m_variable_list{ "meff", //"meff_4j", //"mJsum", 
    "met", "mtw", //"hthad", 
    //"mtbmin", 
    "dPhi_jetmet", "dPhi_lepmet", 
    //"dR_ejet", "dR_mujet", "dR_jetjet", 
    "jets_n", "bjets_n", "jets_pt", "jets_eta", "jets_m", "jets_btagw", "bjets_pt", "bjets_eta", "bjets_btagw", 
    "RCjets_n", "RCjets_pt", "RCjets_eta", "RCjets_m", "RCjets_nconsts", 
    "FatJets_n", "FatJets_pt", "FatJets_eta", "FatJets_m", 
    //"RCTTMass_jets_n", "RCTTMass_jets_pt", "RCTTMass_jets_eta", "RCTTMass_jets_m", "RCTTMass_jets_nconsts",
    //"el_n", "el0_pt", "el0_eta", "el0_phi", "mu_n", "mu0_pt", "mu0_eta", "mu0_phi",
    "el_n", "mu_n", "lep_n"};//, "baseline_lep_n", "baseline_el_n", "baseline_mu_n" };

//selection 55 : 1lep_W
//selection 56 : 1lep_NOMET

//MET trigger
//std::vector<std::string> m_prefix_list{ "Presel_0L", "Presel_1L" };
//Lepton trigger
std::vector<std::string> m_prefix_list{ "c1l2b" };
//"presel1L"//, //"presel1L_ljOR", "presel1L_ljNOOR"
    //   , "presel1L_EL"//, //"presel1L_EL_ljOR", "presel1L_EL_ljNOOR"
    // , "presel1L_MU"//, "presel1L_MU_ljOR", "presel1L_MU_ljNOOR"
    // , "presel0L"};//, "presel0L_dphirev", "presel0L_NOdphi"
//, "presel0BASEL_dphicut", "presel0BASEL_dphirev", "presel0BASEL_NOdphi" };


//std::vector<std::string> m_prefix_list{ "c0l", "Gtt0L_SR1", "Gtt0L_SR2", "Gtt0L_SR3", "Gtt0L_SR4", "Gtt0L_CR1", "Gtt0L_CR2", "Gtt0L_CR3", "Gtt0L_CR4"
//    , "Gtt0L_VR1_0L", "Gtt0L_VR2_0L","Gtt0L_VR3_0L","Gtt0L_VR4_0L", "Gtt0L_VR1_1L", "Gtt0L_VR2_1L","Gtt0L_VR3_1L","Gtt0L_VR4_1L" };

void makeSampleConfigurationFile();
void makeVariableConfigurationFile();

int main(int argc, char** argv){

  int i_sample = 0;
  int i_variable = 0;
  int i_flist = 0;
  std::stringstream ss; std::string sw = "";
  int optind = 1;
  while ((optind < argc) && (argv[optind][0]=='-')) {
    ss << argv[optind]; sw = ss.str();
    //std::cout<<"Parsing '"<<argv[optind]<<" : "<<sw<<std::endl;
    if(sw == "--makeFileLists"){optind++;  ss.str(""); ss << argv[optind]; ss >> i_flist;}
    else if(sw == "--makeSampleConfig"){optind++;  ss.str(""); ss << argv[optind]; ss >> i_sample;}
    else if(sw == "--makeVariableConfig"){optind++;  ss.str(""); ss << argv[optind]; ss >> i_variable;}

    else if     (sw == "--hist_file_prefix"){optind++; ss.str(""); ss << argv[optind]; ss >> m_hist_file_prefix;}
    else if     (sw == "--hist_file_suffix"){optind++; ss.str(""); ss << argv[optind]; ss >> m_hist_file_suffix;}
    else std::cout<<"Unknown switch "<<argv[optind]<<" sw = "<<sw<<std::endl;
    
    optind++; sw.clear();  ss.str(""); ss.clear();
  }

  m_make_file_list = (i_flist>0);
  m_make_sample_config = (i_sample>0);
  m_make_variable_config = (i_variable>0);

  std::cout<<" m_make_file_list = "<<m_make_file_list<<" m_make_sample_config = "<<m_make_sample_config<<" m_make_variable_config = "<<m_make_variable_config<<std::endl;

  if(m_make_sample_config){ makeSampleConfigurationFile();}
  if(m_make_variable_config){ makeVariableConfigurationFile(); }
  
  return 0;
}


void makeSampleConfigurationFile(){

  std::string samp_name = ""; //will change
  std::string samp_key = ""; //will change
  std::string samp_fkey = ""; //will change
  std::string samp_leglabel = ""; //will change
  std::string samp_stylekey = ""; //will change
  std::string samp_drawopt = "e0hist"; //will change for data
  std::string samp_legopt = "lpf"; //will change for data
  std::string samp_drawscale = "NORM"; //default - will change for data
  std::string samp_drawstack = "TRUE"; //will change to default for data
  std::string samp_resopt = "INC"; //will change for data
  std::string samp_blinding = "NONE"; //will DEFINITELY change for data

  std::string samp_filename = "";
  //std::vector<std::string> m_sample_list{ "ttbar", "singletop", "Wjets", "Zjets", "diboson", "topEW" };

  std::map<std::string, std::string> samp_fkey_map; samp_fkey_map.clear();
  samp_fkey_map["Data"] = "data";
  samp_fkey_map["ttbar"] = "ttbar";
  samp_fkey_map["ttbarlight"] = "ttbarlight";
  samp_fkey_map["ttbarcc"] = "ttbarcc";
  samp_fkey_map["ttbarbb"] = "ttbarbb";
  samp_fkey_map["singletop"] = "singletop";
  samp_fkey_map["Wjets"] = "Wjets";
  samp_fkey_map["Zjets"] = "Zjets";
  samp_fkey_map["diboson"] = "diboson";
  samp_fkey_map["topEW"] = "topEW";
  samp_fkey_map["fourtops"] = "4tops";


  std::map<std::string, std::string> samp_key_map; samp_key_map.clear();
  samp_key_map["Data"] = "0_Data";
  samp_key_map["ttbarlight"] = "13_ttbarlight";
  samp_key_map["ttbarcc"] = "12_ttbarcc";
  samp_key_map["ttbarbb"] = "11_ttbarbb";
  samp_key_map["singletop"] = "2_singletop";
  samp_key_map["Wjets"] = "3_Wjets";
  samp_key_map["Zjets"] = "4_Zjets";
  samp_key_map["diboson"] = "5_dibosons";
  samp_key_map["topEW"] = "6_topEW";
  samp_key_map["fourtops"] = "7_fourtops";
  samp_key_map["SUM"] = "SUM";

  std::map<std::string, std::string> samp_label_map; samp_label_map.clear();
  samp_label_map["Data"] = "Data";
  samp_label_map["ttbar"] = "t#bar{t}";
  samp_label_map["ttbarlight"] = "t#bar{t}+light";
  samp_label_map["ttbarcc"] = "t#bar{t}+c#bar{c}";
  samp_label_map["ttbarbb"] = "t#bar{t}+b#bar{b}";
  samp_label_map["singletop"] = "single top";
  samp_label_map["Wjets"] = "W+jets";
  samp_label_map["Zjets"] = "Z+jets";
  samp_label_map["diboson"] = "Dibosons";
  samp_label_map["topEW"] = "TopEW";
  samp_label_map["fourtops"] = "t#bar{t}t#bar{t}";
  samp_label_map["SUM"] = "SM Total";

  std::ofstream sample_config; sample_config.open("VLQ_sample_config.txt");
  sample_config << std::endl << "BEGIN" <<std::endl; 

  std::ofstream file_list; 
  if(m_make_file_list){ 
    file_list.open("VLQ_file_list.txt"); 
    file_list << std::endl << "BEGIN" << std::endl;
    file_list << "KEY : FILE : SCALE" << std::endl;
  }
  for(unsigned int s = 0; s < m_sample_list.size(); s++){
    samp_name = m_sample_list.at(s);
    samp_key = samp_key_map[samp_name];
    samp_fkey = samp_fkey_map[samp_name];
    samp_leglabel = samp_label_map[samp_name];
    samp_stylekey = samp_key.substr(0,1);
    if(samp_name == "Data"){ 
      samp_drawopt = "e0"; 
      samp_legopt = "lpe";
      samp_drawscale = "NONE"; 
      samp_drawstack = "FALSE"; 
      samp_resopt = "INC";
    }
    else if(samp_name == "SUM"){ 
      samp_drawopt = "e2"; 
      samp_legopt = "lef";
      samp_drawscale = "NONE"; 
      samp_drawstack = "FALSE"; 
      samp_resopt = "REF";
    }
    else{ 
      samp_drawopt = "e0hist"; 
      samp_legopt = "f"; 
      samp_drawscale = "NORM"; 
      samp_drawstack = "TRUE"; 
      samp_resopt = "INC";
    }

    sample_config << "NEW" <<std::endl;
    sample_config << "NAME : "<<samp_key<<std::endl;
    sample_config << "LEGLABEL : "<<samp_leglabel<<std::endl;
    sample_config << "STYLEKEY : "<<samp_stylekey<<std::endl;
    sample_config << "DRAWOPT : "<<samp_drawopt<<std::endl;
    sample_config << "LEGOPT : "<<samp_legopt<<std::endl;
    sample_config << "DRAWSCALE : "<<samp_drawscale<<std::endl;
    sample_config << "DRAWSTACK : "<<samp_drawstack<<std::endl;
    sample_config << "RESOPT : "<<samp_resopt<<std::endl;
    //sample_config << "BLINDING : "<<samp_blinding<<std::endl;
    sample_config << std::endl;

    if(m_make_file_list){ 
      samp_filename = m_hist_file_prefix + "_" + samp_fkey + "_" + m_hist_file_suffix;
      file_list << samp_key << " : " << samp_filename << " : " << "1." <<std::endl; 
    }

  }

  sample_config << std::endl << "END" << std::endl;
  sample_config.close();

  if(m_make_file_list){ 
  file_list << std::endl << "END" << std::endl;
  file_list.close();

  }

  return;
}

void makeVariableConfigurationFile(){
  /*
  std::string var_name = ""; //will change
  std::string var_drawstack = "TRUE"; //won't change 
  std::string var_drawres = "RATIO"; //won't change
  std::string var_doscale = "NORM"; //won't change
  std::string var_blinding = "BIN"; //may change
  */
  std::string var_name = ""; //will change
  std::string var_drawstack = "TRUE"; //won't change 
  std::string var_drawres = "RATIO"; //won't change
  std::string var_doscale = "NORM"; //won't change
  std::string var_label = ""; //will change
  std::string var_title = ""; //will change for each region
  std::string var_rebin = ""; //will change

  std::ofstream var_config; var_config.open("VLQ_variable_config.txt");
  var_config << std::endl << "BEGIN" <<std::endl; 

  for(unsigned int r = 0; r < m_prefix_list.size(); r++){
    const std::string& region = m_prefix_list.at(r);
    std::cout << " region = " << region << std::endl;
    var_title = "#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{" + region + "}"; 

    for(unsigned int v = 0; v < m_variable_list.size(); v++){

      const std::string& variable = m_variable_list.at(v);
      std::cout<<" variable = " << variable << std::endl;

      var_name = region + "_" + variable;
      std::cout<< " var_name = "<<var_name<<std::endl;
      var_config << "NEW" <<std::endl;
      var_config << "NAME : "<<var_name<<std::endl;
      var_config << "DRAWSTACK : "<<var_drawstack<<std::endl;
      var_config << "DRAWRES : "<<var_drawres<<std::endl;
      var_config << "DOSCALE : "<<var_doscale<<std::endl;
      var_config << "EXTRALABEL : "<<var_title<<std::endl; 
      var_config << "OUTPUTFOLDER : "<<region<<std::endl;

      if(variable=="bjets_n"){ var_config << "LABEL : Number of b-jets (77% WP)" << std::endl;}
      if( (variable=="jets_n") || (variable=="bjets_n") || (variable=="el_n") || (variable=="mu_n")  
	  || (variable=="baseline_el_n") || (variable=="baseline_mu_n") 
	  || (variable=="lep_n") || (variable=="baseline_lep_n") 
	  || (variable=="rcjets_n") || (variable=="RCTTMass_jets_n") ){
	var_config << "ISCOUNT : TRUE" << std::endl; 
	//var_config << "ISLOGY : TRUE" << std::endl;
      }
      if( (variable=="mJsum") || (variable=="mJsum_4j") ){ var_config << "XMAX : 800." << std::endl; }
      if(variable=="met"){ var_config << "XMAX : 700." << std::endl; }
      if( (variable=="mtbmin") || (variable=="mtw") ){ var_config << "XMAX : 500." << std::endl; }
      if( (variable=="meff") || (variable=="jets_pt") || (variable=="bjets_pt") 
	  || (variable=="rcjets_pt") || (variable=="RCTTMass_jets_pt") 
	  || (variable=="rcjets_m") || (variable=="RCTTMass_jets_m") ){ var_config << "REBIN : 2" << std::endl; }
      var_config << std::endl;
    } //var loop

  } // reg loop

  var_config << std::endl << "END" << std::endl;
  var_config.close();
  return; 
}

