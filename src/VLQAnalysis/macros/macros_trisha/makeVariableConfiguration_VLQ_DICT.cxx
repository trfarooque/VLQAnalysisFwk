#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "IFAETopFramework/AnalysisUtils.h"

std::ofstream m_var_config; 
std::ifstream m_onevar_bins_dict;
std::map<std::string, std::vector<std::string> > m_regions; 

std::vector<std::string> m_variable_list;
std::vector<std::string> m_variable_list_RC;


int main(int argc, char** argv){

  //***********0**************//
  std::stringstream ss; std::string sw = "";

  std::string config_name          = "";
  std::string opt_reg              = "";
  std::string opt_onevar           = "";
  std::string opt_onevar_rebin     = "";
  std::string opt_onevar_bins_dict = "";
  std::string opt_folder           = "";
  std::string var_doscale          = "";
  std::string var_drawstack        = "";
  std::string var_drawres          = "";
  std::string var_resmin           = "";
  std::string var_resmax           = "";
  std::string var_islogy           = "";
  std::string var_resrefline       = "";

  int optind = 1;

  std::string _switch = ""; 
  std::cout<<" argc = "<<argc<<std::endl;
  while ((optind < argc) && (argv[optind][0]=='-')) {
    _switch = std::string(argv[optind]);
    //ss << argv[optind];
    //_switch = ss.str();  
    optind++;  ss << argv[optind];
    std::cout<<" _switch = "<<_switch<<" ss = "<<ss.str()<<std::endl;
    if       (_switch == "--config_name")   { ss >> config_name; } //Name of output config file
    else if  (_switch == "--opt_regions")   { ss >> opt_reg; } //Regions to include: 0L_ana, 1L_ana, 0L_presel or 1L_presel
    else if  (_switch == "--opt_onevar")    { ss >> opt_onevar; } //Single variable name to include
    else if  (_switch == "--opt_onevar_rebin")    { ss >> opt_onevar_rebin; } //Rebin single bin if TRUE
    else if  (_switch == "--opt_onevar_bins_dict")    { ss >> opt_onevar_bins_dict; } //Dictionary from which to read binning for single variable
    else if  (_switch == "--opt_folder")    { ss >> opt_folder; } //Separate folder for each region created if opt_folder==REGION
    else if  (_switch == "--doscale")       { ss >> var_doscale; } //NORM or SHAPE
    else if  (_switch == "--drawstack")     { ss >> var_drawstack; } //TRUE or FALSE
    else if  (_switch == "--drawres")       { ss >> var_drawres; } //RATIO, RESIDUAL, NONE, etc
    else if  (_switch == "--resmin")        { ss >> var_resmin; } //y-axis minimum in bottom panel
    else if  (_switch == "--resmax")        { ss >> var_resmax; } //y-axis maximum in bottom panel
    else if  (_switch == "--islogy")        { ss >> var_islogy; } //TRUE or FALSE
    else if  (_switch == "--resrefline")    { ss >> var_resrefline; } //y-coordinate of reference line to be drawn in bottom panel

    else std::cout<<"Unknown switch "<<_switch<<std::endl;

    optind++; ss.clear(); _switch.clear();
  }

  //***********0**************//
  if( config_name.empty()   ) { config_name = "VLQ_variable_config.txt"; }
  if( opt_folder.empty()    ) { opt_folder = "REGION"; }
  if( var_doscale.empty()   ) { var_doscale = "NORM";  }
  if( var_drawstack.empty() ) { var_drawstack = "TRUE"; } 
  if( var_drawres.empty()   ) { var_drawres = "RATIO"; } 
  if( var_resmin.empty()    ) { var_resmin = "0.2"; } 
  if( var_resmax.empty()    ) { var_resmax = "1.8"; } 
  if( opt_onevar_rebin=="TRUE" && opt_onevar_bins_dict.empty() ) { 
    opt_onevar_bins_dict = std::getenv("VLQAnalysisFramework_DIR") + std::string("/python/VLQAnalysis/regions_dictionary.py");
  }

  std::cout<<"config_name       =  " << config_name      << std::endl;
  std::cout<<"opt_regions       =  " << opt_reg          << std::endl;
  std::cout<<"opt_onevar        =  " << opt_onevar       << std::endl;
  std::cout<<"opt_onevar_rebin  =  " << opt_onevar_rebin << std::endl;
  std::cout<<"opt_onevar_bins_dict =  " << opt_onevar_bins_dict << std::endl;
  std::cout<<"opt_folder        =  " << opt_folder       << std::endl;
  std::cout<<"doscale           =  " << var_doscale      << std::endl;
  std::cout<<"drawstack         =  " << var_drawstack    << std::endl;
  std::cout<<"drawres           =  " << var_drawres      << std::endl;
  std::cout<<"resmin            =  " << var_resmin       << std::endl;
  std::cout<<"resmax            =  " << var_resmax       << std::endl;
  std::cout<<"islogy            =  " << var_islogy       << std::endl;
  std::cout<<"resrefline        =  " << var_resrefline   << std::endl;

  m_onevar_bins_dict.open(opt_onevar_bins_dict.c_str());
  m_var_config.open(config_name.c_str());
  m_var_config << std::endl << "BEGIN" <<std::endl; 

  std::string var_name = ""; //will change
  std::string var_label = ""; //will change
  std::string var_title = ""; //will change for each region
  //std::string var_rebin = "0.,700.,800.,900.,1000.,1200.,1400.,1600.,2000.,2500.,3000."; //will change

  std::vector<std::string> m_prefix; m_prefix.clear();
  if(opt_reg == "0L_presel"){ m_prefix = {"c0lep6jin2bin"}; }
  else if(opt_reg == "1L_presel"){ m_prefix = {"c1lep5jin2bin"}; }
  else if(opt_reg == "presel"){ m_prefix = {"c0lep6jin2bin", "c1lep5jin2bin"}; }

  else if(opt_reg.find("ana") != std::string::npos){

    std::vector<std::string> boost_common = {"0Tex0Hex","0Tex1Hex", "1Tex0Hex" };

    std::vector<std::string> boost_highb;
    boost_highb.insert(boost_highb.end(), boost_common.begin(), boost_common.end());
    boost_highb.push_back("2THin");

    std::vector<std::string> boost_lowb;
    boost_lowb.insert(boost_lowb.end(), boost_common.begin(), boost_common.end());
    boost_lowb.push_back("0Tin2Hin");
    boost_lowb.push_back("1Tex1Hex");
    boost_lowb.push_back("2Tin0_1Hwin");

    std::string lowj = "";
    std::string highj = "";
    std::string channel = "";

    if(opt_reg == "0L_ana"){ lowj = "6jex"; highj = "7jin"; channel = "c0lep"; }
    else if(opt_reg == "1L_ana"){ lowj = "5jex"; highj = "6jin"; channel = "c1lep"; }
    else{ std::cerr<<"Error: Unknown region option "<<opt_reg<<std::endl; return -1; }

    for(const std::string& jets : {lowj, highj}){ 

      for( const std::string& bjets : {"2bex", "3bex", "4bin"} ){

	std::vector<std::string>& boostlist = 
	  ( (jets==lowj) && (bjets=="2bex" || bjets=="3bex" || bjets=="4bin")) || ( (jets==highj) && (bjets=="2bex" || bjets=="4bin")) ? boost_highb : boost_lowb; 
	//( (jets==lowj) && (bjets=="3bex" || bjets=="4bin")) || ( (jets==highj) && (bjets=="4bin")) ? boost_highb : boost_lowb; 

	for(const std::string& boost : boostlist){

	  if(opt_reg == "0L_ana"){
	    if( ( (bjets != "2bex") && ( (boost == "0Tin2Hin") || (boost == "2THin") ) ) || ( (jets=="6jex") && (bjets=="4bin") && (boost!="0Tex0Hex") ) ){
	      m_prefix.push_back(channel+boost+jets+bjets);
	    }
	    else{
	      m_prefix.push_back(channel+boost+jets+bjets+"HighMtbmin");
	      if(bjets != "2bex"){
		m_prefix.push_back(channel+boost+jets+bjets+"LowMtbmin");
	      }
	    }
	  }

	}//boost

      }//bjet

    }//jets

  }//if analysis regions
  std::cout<< " number of regions = "<<m_prefix.size()<<std::endl;

  if(opt_onevar.empty()){
    m_variable_list = { "meff", "hthad", "mtbmin", "met", "met_phi", "met_sig",
			"jets_n", "bjets_n", "RCMTop_jets_n", "RCMHiggs_jets_n", "RCjets_n", 
			"jets_pt", "jets_eta", "jets_m", "jets_phi", "jets_btagw", 
			"jet0_pt", "jet0_eta", "jet0_m", "jet0_phi", "jet0_btagw",
			"bjets_pt", "bjets_eta", "bjets_btagw", 
			"bjet0_pt", "bjet0_eta", "bjet0_btagw",
			"RCjets_pt", "RCjets_eta", "RCjets_m", "RCjets_nconsts", 
			"RCjet0_pt", "RCjet0_eta", "RCjet0_m", "RCjet0_nconsts", 
			"dR_jetjet", "dR_bjetbjet", "dPhi_jetmet" 
    };
    if(opt_reg.find("1L") != std::string::npos){
    
      std::vector<std::string> m_variable_list_1L{"ptw", "mtw", "dPhi_lepmet", "lep0_pt_zoom", "lep0_eta", "lep0_phi", "dR_ejet", "dR_ebjet", "dR_mujet", "dR_mubjet" };
      m_variable_list.insert(m_variable_list.end(), m_variable_list_1L.begin(), m_variable_list_1L.end());
    }

    m_variable_list_RC = {
      //"RCTTMass_jets_pt", "RCTTMass_jets_eta", "RCTTMass_jets_m", "RCTTMass_jets_consts_n",
      "RCMTop_jets_pt", "RCMTop_jets_eta", "RCMTop_jets_m", 
      "RCMHiggs_jets_pt", "RCMHiggs_jets_eta", "RCMHiggs_jets_m", 
    };

  }
  std::cout<<" number of variables globally = "<<m_variable_list.size()<<std::endl;

  for(unsigned int r = 0; r < m_prefix.size(); r++){
    const std::string& region = m_prefix.at(r);
    std::cout << " region = " << region << std::endl;
    std::string reg_label = "";
    if     (region.find("0Tex0Hex") != std::string::npos){ reg_label += "0T, 0H, "; }
    else if(region.find("0Tex1Hex") != std::string::npos){ reg_label += "0T, 1H, "; }
    else if(region.find("1Tex0Hex") != std::string::npos){ reg_label += "1T, 0H, "; }
    else if(region.find("1Tex1Hex") != std::string::npos){ reg_label += "1T, 1H, "; }
    else if(region.find("0Tin2Hin") != std::string::npos){ reg_label += "#geq2H, "; }
    else if(region.find("2Tin0_1Hwin") != std::string::npos){ reg_label += "#geq2T, 0-1H, "; }
    else if(region.find("2THin") != std::string::npos){ reg_label += "#geq2TH"; }

    if     (region.find("5jex") != std::string::npos){ reg_label += "6j, "; }
    else if(region.find("6jex") != std::string::npos){ reg_label += "6j, "; }
    else if(region.find("6jin") != std::string::npos){ reg_label += "#geq6j, "; }
    else if(region.find("7jin") != std::string::npos){ reg_label += "#geq7j, "; }

    if     (region.find("2bex") != std::string::npos) { reg_label += "2b, "; }
    else if(region.find("3bex") != std::string::npos) { reg_label += "3b, "; }
    else if(region.find("4bin") != std::string::npos) { reg_label += "#geq4b, "; }

    if     (region.find("c1lep") != std::string::npos){ reg_label += " 1l"; }
    else if(region.find("c0lep") != std::string::npos){ reg_label += " 0l"; }

    if     (region.find("HighMtbmin") != std::string::npos){ reg_label += ", HM"; }
    else if(region.find("LowMtbmin") != std::string::npos){ reg_label += ", LM"; }


    var_title = "#splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 36.1 fb^{-1}}}{" + reg_label + "}"; 

    std::string reg_binning = "";
    int reg_nbins = 0;

    std::vector<std::string> v_variable;
    if(opt_onevar.empty()){
      v_variable = m_variable_list;
      if( (region.find("0Mex") == std::string::npos) && (region.find("0Tex0Hex") == std::string::npos) ){
	v_variable.insert(v_variable.end(), m_variable_list_RC.begin(), m_variable_list_RC.end());
      }
    }
    else{
      v_variable = {opt_onevar};
      std::string line = "";
      std::string delim = ":\"";
      bool found_reg = false;
      if(!opt_onevar_rebin.empty()){
	m_onevar_bins_dict.clear();
	m_onevar_bins_dict.seekg(0, std::ios::beg);

	while(std::getline(m_onevar_bins_dict, line)){
	  AnalysisUtils::TrimString(line); //remove all whitespaces
	  if(line.find("#") != std::string::npos){ line = line.substr(0,line.find("#")); } //remove comments

	  if(found_reg){
	    if(line.find("\'binning\':") != std::string::npos){
	      line=line.substr(line.find("\"")+1);
	      line=AnalysisUtils::ReplaceString(line,"\",",""); //remove the comma and quotation mark at the end of the line
	      reg_binning = line;
	      reg_nbins = AnalysisUtils::CountSubstring(reg_binning, ",");
	      break;
	    }
	    else{ continue; }
	  }
	  else{
	    if( line.find("name") != std::string::npos ){
	      std::string regcur = line.substr( line.find(delim) + delim.size() );
	      regcur = AnalysisUtils::ReplaceString(regcur,"\",","");  //remove the comma and quotation mark at the end of the line
	      if(regcur == "HTX_"+region){ found_reg = true; }
	    }
	    else{ continue; }
	  }
	  
	}//loop over lines in regions_dictionary
	
	
      }//if the binning dictionary is provided
      
    }//One variable
    for(unsigned int v = 0; v < v_variable.size(); v++){

      const std::string& variable = v_variable.at(v);
      std::cout<<" variable = " << variable << std::endl;

      var_name = region + "_" + variable;
      std::cout<< " var_name = "<<var_name<<std::endl;

      m_var_config << "NEW"               <<std::endl;
      m_var_config << "NAME : "        << var_name        <<std::endl;
      m_var_config << "DRAWSTACK : "   << var_drawstack   <<std::endl;
      m_var_config << "DRAWRES : "     << var_drawres     <<std::endl;
      m_var_config << "DOSCALE : "     << var_doscale     <<std::endl;
      m_var_config << "RESMIN : "      << var_resmin      <<std::endl;
      m_var_config << "RESMAX : "      << var_resmax      <<std::endl;
      if(var_islogy == "TRUE"){
	m_var_config << "ISLOGY : TRUE"                   <<std::endl;
	m_var_config << "YMIN   : 1.e-5"                  <<std::endl;
      }
      if(!var_resrefline.empty()){
	m_var_config << "RESREFLINE : " << var_resrefline <<std::endl;
      }
      m_var_config << "EXTRALABEL : "  << var_title      <<std::endl; 
      if(opt_folder == "REGION"){ m_var_config << "OUTPUTFOLDER : "<<region<<std::endl; }
      if(variable=="bjets_n"){ m_var_config << "LABEL : Number of b-jets (77% WP)" << std::endl;}


      if( (variable=="mJsum") || (variable=="mJsum_4j") ){ m_var_config << "XMAX : 800." << std::endl; }
      if(variable=="met"){ m_var_config << "XMAX : 700." << std::endl; }
      if( (variable=="mtbmin") || (variable=="mtw") ){ m_var_config << "XMAX : 500." << std::endl; }

      std::cout<<" reg_nbins = "<<reg_nbins<<" reg_binning = "<<reg_binning<<std::endl;
      if(!(opt_onevar.empty() || opt_onevar_bins_dict.empty()) && (reg_nbins>0)){
	m_var_config << "REBIN : " << reg_nbins << std::endl;
	m_var_config << "REBINVAR : " << reg_binning << std::endl;
      }
      m_var_config << std::endl;
    } //var loop
    
    v_variable.clear();
    
  } // reg loop

  
  m_prefix.clear();
  
  return 0; 
}
      //m_var_config << "REBIN : 2"<<std::endl; //ALERTRISHA
      //m_var_config << "REBINVAR : "<<var_rebin<<std::endl; //ALERTRISHA
      /*
      if( (variable=="jets_n") || (variable=="bjets_n") || (variable=="el_n") || (variable=="mu_n")  
	  || (variable=="baseline_el_n") || (variable=="baseline_mu_n") 
	  || (variable=="lep_n") || (variable=="baseline_lep_n") 
	  || (variable=="rcjets_n") || (variable=="RCTTMass_jets_n") ){
	m_var_config << "ISCOUNT : TRUE" << std::endl; 
	//m_var_config << "ISLOGY : TRUE" << std::endl;
      }
      */
      /*
      if( (variable=="meff") || (variable=="jets_pt") || (variable=="bjets_pt") 
	  || (variable=="rcjets_pt") || (variable=="RCTTMass_jets_pt") 
	  || (variable=="rcjets_m") || (variable=="RCTTMass_jets_m") ){ m_var_config << "REBIN : 2" << std::endl; }
      */
