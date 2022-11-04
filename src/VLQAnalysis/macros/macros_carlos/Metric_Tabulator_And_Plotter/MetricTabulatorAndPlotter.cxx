#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <boost/algorithm/string/replace.hpp>
#include <regex>

#include "TFile.h"
#include "TH1D.h"


struct SampleInfo{
  std::string _key;
  std::string _label;
  std::string _f_mc16a;
  std::string _f_mc16d;
  std::string _f_mc16e;
  std::string _suffix;
  double _mc16a_scale;
  double _mc16d_scale;
  double _mc16e_scale;
  
  SampleInfo(const std::string &key, const std::string &label, const std::string &f_mc16a, const std::string &f_mc16d, const std::string &f_mc16e,
	     const std::string &suffix = "", double mc16a_scale = 36207.66, double mc16d_scale = 44037.4, double mc16e_scale = 58450.1){
    _key         = key;
    _label       = label;
    _f_mc16a     = f_mc16a;
    _f_mc16d     = f_mc16d;
    _f_mc16e     = f_mc16e;
    _suffix      = suffix;
    _mc16a_scale = mc16a_scale;
    _mc16d_scale = mc16d_scale;
    _mc16e_scale = mc16e_scale;
  }
};

//__________________________________________________________________________________________________
//
void FillSampleInfoMap(std::map<std::string, SampleInfo> &info_map, const std::string &sample_config_file, char delim = ':'){

  std::cout << "Opening sample configuration file " << sample_config_file << std::endl;

  std::ifstream file(sample_config_file);

  if(!file.is_open()){
    std::cout << "< ERROR! > : The file " << sample_config_file << "does not exist. Exiting from program." << std::endl;
    exit(EXIT_FAILURE);
  }
  
  std::string line;

  while(getline(file, line)){

    std::string key = "";
    std::string label = "";
    std::string f_mc16a= "";
    std::string f_mc16d = "";
    std::string f_mc16e = "";
    std::string suffix = "";
    double mc16a_scale = 36207.66;
    double mc16d_scale = 44037.4;
    double mc16e_scale = 58450.1;

    if(line == "NEW"){

      std::cout << "#####################################" << std::endl;
      std::cout << "#          Found sample info        #" << std::endl;
      std::cout << "#####################################" << std::endl;

      while(getline(file,line) && line != "END"){

	std::string opt = line.substr(0, line.find(delim));

	std::string val = line.substr(line.find(delim)+1);

	std::transform(opt.begin(), opt.end(), opt.begin(), toupper);

	std::cout << opt << " : " << val <<std::endl;

        if(opt == "NAME") key = val;
        else if(opt == "LABEL") label = val;
        else if(opt == "MC16A_FILE") f_mc16a = val;
        else if(opt == "MC16D_FILE") f_mc16d = val;
        else if(opt == "MC16E_FILE") f_mc16e = val;
        else if(opt == "SUFFIX") suffix = val;
        else if(opt == "MC16A_SCALE") mc16a_scale = std::stod(val);
        else if(opt == "MC16D_SCALE") mc16d_scale = std::stod(val);
        else if(opt == "MC16E_SCALE") mc16e_scale = std::stod(val);
      }
      
      info_map.insert(std::pair<std::string, SampleInfo>(key, SampleInfo(key, label, f_mc16a, f_mc16d, f_mc16e, suffix, mc16a_scale, mc16d_scale, mc16e_scale)));
    }
  }

  std::cout << "Closing sample configuration file "  << sample_config_file << std::endl;


  file.close();
  
}

//__________________________________________________________________________________________________
//
void FillSampleRegionInfoMap(SampleInfo &sample_info, std::map<std::string, std::vector<double> > &region_info, const std::string &region_list, 
			     const std::string &mode = "YIELDS", const std::string &var = "meff", double start_value = 1000){

  std::map<std::string, TFile*> f_mc16;

  // At least one file is expected
  f_mc16["mc16a"] = TFile::Open((sample_info._f_mc16a).c_str(), "READ");
  if((sample_info._f_mc16d) != "") f_mc16["mc16d"] = TFile::Open((sample_info._f_mc16d).c_str(), "READ");
  if((sample_info._f_mc16e) != "") f_mc16["mc16e"] = TFile::Open((sample_info._f_mc16e).c_str(), "READ");

  //TFile* f_mc16a = TFile::Open((sample_info._f_mc16a).c_str(), "READ");
  //TFile* f_mc16d = TFile::Open((sample_info._f_mc16d).c_str(), "READ");
  //TFile* f_mc16e = TFile::Open((sample_info._f_mc16e).c_str(), "READ");

  std::ifstream region_file(region_list);

  if(!region_file.is_open()){
    std::cout << "< ERROR! > : The file " << region_list << "does not exist. Exiting from program." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string region;

  std::string histo_name_suffix = (sample_info._suffix == "" ? "_" + var : "_" + sample_info._suffix + "_"+var);

  while(getline(region_file, region)){
    
    std::string histo_name = region + histo_name_suffix;

    //TH1D* h_mc16a = (TH1D*)(f_mc16a->Get(histo_name.c_str()));
    //TH1D* h_mc16d = (TH1D*)(f_mc16d->Get(histo_name.c_str()));
    //TH1D* h_mc16e = (TH1D*)(f_mc16e->Get(histo_name.c_str()));

    //std::cout << "Opening the histogram " << histo_name << std::endl;

    TH1D* h_mc16a = (TH1D*)(f_mc16.at("mc16a")->Get(histo_name.c_str()));
    h_mc16a->Scale(sample_info._mc16a_scale);

    if((sample_info._f_mc16d) != ""){
      TH1D* h_mc16d = (TH1D*)(f_mc16.at("mc16d")->Get(histo_name.c_str()));
      h_mc16d->Scale(sample_info._mc16d_scale);
      h_mc16a->Add(h_mc16d);
      delete h_mc16d;
    }
    if((sample_info._f_mc16e) != ""){
      TH1D* h_mc16e = (TH1D*)(f_mc16.at("mc16e")->Get(histo_name.c_str()));
      h_mc16e->Scale(sample_info._mc16e_scale);
      h_mc16a->Add(h_mc16e);
      delete h_mc16e;
    }
    
    if(mode == "YIELDS"){
      int start_bin = h_mc16a->FindBin(start_value);
      double stat_unc = 0;
      double yield = h_mc16a->IntegralAndError(start_bin, -1, stat_unc); 
      region_info.insert(std::pair<std::string, std::vector<double> >(region, {yield, stat_unc}));
    }
    else if(mode == "MVA"){
      std::vector< double > yield_pass = {};
      int start_bin = 1;
      int end_bin = h_mc16a->GetNbinsX();

      for(int i = start_bin; i <= end_bin; i++) yield_pass.push_back(h_mc16a->Integral(i,end_bin));
      yield_pass.push_back(h_mc16a->Integral()); // Store total number of entries for normalization
      yield_pass.push_back(h_mc16a->GetNbinsX());
      region_info.insert(std::pair<std::string, std::vector<double> >(region, yield_pass));
    }

    delete h_mc16a;

  }

  for(std::map<std::string, TFile*>::iterator f_mc16_it = f_mc16.begin(); f_mc16_it != f_mc16.end(); ++f_mc16_it){
    (f_mc16_it->second)->Close();
    delete (f_mc16_it->second);
  }
  
  region_file.close();
 
}

//__________________________________________________________________________________________________
//
void MakeSignalMVAPlots(TFile* &outfile, const std::string &sig_name, 
			std::map<std::string, std::vector<double> > &sig_region_info, std::map<std::string, std::vector<double> > &bkg_region_info){

  double l_bin = -0.1; //0
  double h_bin = 1.05; //1

  for(std::map<std::string, std::vector<double> >::iterator it = sig_region_info.begin(); it != sig_region_info.end(); ++it){

    int nbins = (it->second)[it->second.size()-1];
    double S_norm = (it->second)[it->second.size()-2];
    double B_norm = (bkg_region_info.at(it->first))[bkg_region_info.at(it->first).size()-2];
    
    TH1D* efficiency   = new TH1D((sig_name+"_efficiency_"+(it->first)).c_str(),    ";MVA Score;Efficiency",    nbins, l_bin, h_bin);
    TH1D* purity       = new TH1D((sig_name+"_purity_"+(it->first)).c_str(),        ";MVA Score;Purity",        nbins, l_bin, h_bin);
    TH1D* significance = new TH1D((sig_name+"_significance_"+(it->first)).c_str(),  ";MVA Score;Significance",  nbins, l_bin, h_bin);

    TH1D* efficiency_norm   = new TH1D((sig_name+"_normed_efficiency_"+(it->first)).c_str(),    ";MVA Score;Efficiency",   nbins, l_bin, h_bin);
    TH1D* purity_norm       = new TH1D((sig_name+"_normed_purity_"+(it->first)).c_str(),        ";MVA Score;Purity",       nbins, l_bin, h_bin);
    TH1D* significance_norm = new TH1D((sig_name+"_normed_significance_"+(it->first)).c_str(),  ";MVA Score;Significance", nbins, l_bin, h_bin);

    
    for(int i = 1; i <= nbins; i++){
      
      double S = (it->second)[i-1];
      double B = (bkg_region_info.at(it->first))[i-1];
      
      efficiency->SetBinContent(i, S);
      purity->SetBinContent(i, (S==0 && B == 0) ? 0 :S/(S+B));      
      significance->SetBinContent(i, (S==0 && B==0) ? 0 : S/std::sqrt(S+B));     
      
      efficiency_norm->SetBinContent(i, S/S_norm);
      purity_norm->SetBinContent(i, (S==0 && B ==0) ? 0 : (S/S_norm)/((S/S_norm)+(B/B_norm)));
      significance_norm->SetBinContent(i, (S==0 && B ==0) ? 0 : (S/S_norm)/std::sqrt((S/S_norm)+(B/B_norm)));
      
    }

    outfile->cd();
    efficiency->Write();
    purity->Write();
    significance->Write();
    efficiency_norm->Write();
    purity_norm->Write();
    significance_norm->Write();
  }

}

//__________________________________________________________________________________________________
//
void MakeBackgroundMVAPlots(TFile* &outfile, std::map<std::string, std::vector<double> > &bkg_region_info){
  
  double l_bin = -0.1; //0
  double h_bin = 1.05; //1 

  for(std::map<std::string, std::vector<double> >::iterator it = bkg_region_info.begin(); it != bkg_region_info.end(); ++it){

    int nbins = (it->second)[it->second.size()-1];
    double B_norm = (it->second)[it->second.size()-2];

    TH1D* rejection      = new TH1D(("background_rejection_"+(it->first)).c_str(),        ";MVA Score;Rejection",   nbins, l_bin, h_bin);
    TH1D* rejection_norm = new TH1D(("background_normed_rejection_"+(it->first)).c_str(), ";MVA Score;Rejection",   nbins, l_bin, h_bin);

    for(int i = 1; i <= nbins; i++){

      double B = (it->second)[i-1];
      
      rejection->SetBinContent(i, B_norm-B);
      rejection_norm->SetBinContent(i, 1-(B/B_norm));

    }
    
    outfile->cd();
    rejection->Write();
    rejection_norm->Write();
  }
    
}

//__________________________________________________________________________________________________
//
std::string GetRegionLabel(const std::string &region){

  std::string tmp_region = region;

  std::vector< std::string > labelComponents;

  std::smatch match;

  std::regex JBOMR("[[:d:]_[:d:]]{1,3}[jfbMJHVLT]+[a-z]{2,3}");

  while(std::regex_search(tmp_region, match, JBOMR)){

    for(std::string comp : match){

      std::string tmp_label = "";

      if(comp.find("win") != std::string::npos){
        tmp_label += std::string(1,comp[0]) + "-" + std::string(1,comp[2]);
        comp = comp.substr(3, comp.find("win")-3);
      }
      else if(comp.find("in") != std::string::npos){
        tmp_label += "#geq" + std::string(1,comp[0]);
        comp = comp.substr(1, comp.find("in")-1);
      }
      else if(comp.find("ex") != std::string::npos){
        tmp_label += comp[0];
        comp = comp.substr(1, comp.find("ex")-1);
      }
      else{
        tmp_label += "#oplus" + std::string(1,comp[0]);
        comp = comp.substr(1, comp.find("or")-1);
      }

      // LT, VT, HT, VLT
      if(comp.size() > 1 && comp != "fj"){

        tmp_label += "(";

	std::string::iterator it = comp.begin();

        while(it != comp.end()){

          tmp_label += (*it != 'L' && *it != 'T') ? std::string(1,*it) : ( (*it == 'L') ? "t_{l}" : "t_{h}" );

          if(next(it) != comp.end()) tmp_label+="+";

          ++it;

        }

        tmp_label += ")";

      }

      // j, b, fj, H, V, T, L
      else{

        tmp_label += (comp != "L" && comp != "T") ? comp : ( (comp == "L") ? "t_{l}" : "t_{h}");

      }

      labelComponents.push_back(tmp_label);

    }

    tmp_region = match.suffix().str();

  }

  if(region.find("HighMtbmin") != std::string::npos) labelComponents.push_back("HMTBMIN");
  if(region.find("LowMtbmin") != std::string::npos) labelComponents.push_back("LMTBIN");
  if(region.find("LowMetCut") != std::string::npos) labelComponents.push_back("LMET");
  if(region.find("HighMetCut") != std::string::npos) labelComponents.push_back("HMET");
  if(region.find("HighMVAScore") != std::string::npos) labelComponents.push_back("HMVA");
  if(region.find("MidMVAScore") != std::string::npos) labelComponents.push_back("MMVA");
  if(region.find("LowMVAScore") != std::string::npos) labelComponents.push_back("LMVA");

  std::string label = "";

  for(unsigned int i = 0; i < labelComponents.size()-1; i++) label += labelComponents[i]+", ";

  label += labelComponents[labelComponents.size()-1];

  return label;

}

//__________________________________________________________________________________________________
//
void MakeLaTeXTable(const std::string &fName, const std::string &sample_label, 
		    std::map<std::string, std::vector<double> > &sig_region_info, std::map<std::string, std::vector<double> > &bkg_region_info,
		    const std::string &attribute_file_name = ""){

  //
  std::map<int, std::vector<std::string> > region_attributes;
  if(attribute_file_name != ""){

    std::ifstream attribute_file(attribute_file_name);
    std::string region_atr;

    while(getline(attribute_file, region_atr)){
      //index:name:color
      
      std::stringstream ss(region_atr);
      std::string item;

      int current_index;
      int attribute_counter = 0;

      while(getline(ss, item, ':')){
	if(attribute_counter == 0){
	  current_index = std::stoi(item);
	  region_attributes[current_index] = {};
	}
	else{
	  region_attributes.at(current_index).push_back(item);
	}
	attribute_counter++;
      }

    }

    attribute_file.close();

  }
  else{
    int region_counter = 0;
    for(std::map<std::string, std::vector< double > >::iterator it = sig_region_info.begin(); it != sig_region_info.end(); ++it){
      region_attributes[region_counter] = {it->first, "NONE"};
      region_counter++;
    }
  }
  //

  std::ofstream file(fName);
  file << "\\documentclass{article}\n";
  file << "\\usepackage{xcolor}\n";
  file << "\\begin{document}\n";
  file << "\\begin{table}[h!]\n";
  file << "\\centering\n";
  file << "\\hspace*{-3cm}\n";
  file << "\\begin{tabular}{| c || c | c | c | c | c | c |}\n";
  file << "\\hline\\hline\n";
  file << "\\hline\n";
  file << "Region & Signal & S/B & S/$\\sqrt{B}$ & S & B & B stat. unc.\\\\\n";
  file << "\\hline\n";

  for(std::map<int, std::vector< std::string > >::iterator it = region_attributes.begin(); it != region_attributes.end(); ++it){
    std::string region_label = boost::replace_all_copy(GetRegionLabel((it->second)[0]), "#geq", "$\\geq$");
    region_label = boost::replace_all_copy(region_label, "#oplus", "$\\oplus$");
    std::string signal_label = boost::replace_all_copy(sample_label, "_", " ");

    if((it->second)[1] != "NONE") region_label = "\\color{"+(it->second)[1]+"}{"+region_label+"}";

    double S = sig_region_info.at((it->second)[0])[0];
    double B = bkg_region_info.at((it->second)[0])[0];
    double B_stat_unc = bkg_region_info.at((it->second)[0])[1];
    
    file << region_label << " & ";
    file << signal_label << " & " ;
    file << std::setprecision(3) << S/B << " & " << S/std::sqrt(B) << " & " << S << " & " << B << " & " << B_stat_unc ;
    file << " \\\\\n";
    file << "\\hline\n";

  }
  
  /*for(std::map<std::string, std::vector< double > >::iterator it = sig_region_info.begin(); it != sig_region_info.end(); ++it){

    std::string region_label = boost::replace_all_copy(GetRegionLabel(it->first), "#geq", "$\\geq$");
    region_label = boost::replace_all_copy(region_label, "#oplus", "$\\oplus$");
    std::string signal_label = boost::replace_all_copy(sample_label, "_", " ");

    double S = (it->second)[0];
    double B = (bkg_region_info.at(it->first))[0];
    double B_stat_unc = (bkg_region_info.at(it->first))[1];

    file << region_label << " & ";
    file << signal_label << " & " ;
    file << std::setprecision(3) << S/B << " & " << S/std::sqrt(B) << " & " << S << " & " << B << " & " << B_stat_unc ;
    file << " \\\\\n";
    file << "\\hline\n";
    
    }*/
  
  file << "\\hline\\hline\n";
  file << "\\end{tabular}\n";
  file << "\\end{table}\n";
  file << "\\end{document}\n";
  file.close();

}

//__________________________________________________________________________________________________
//
int main(int argc, char** argv){

  std::string sample_config_file = "";
  std::string region_list = "";  
  std::string region_attributes = "";
  bool do_latex_tables = false;
  bool do_mva_plots = false;

  for(int i = 1; i < argc; i++){

    std::string opt(argv[i]);
    std::string argument;
    std::string value;
    size_t pos = opt.find("=");
    argument = opt.substr(0, pos);
    value = opt.erase(0, pos + 1);

    std::transform(argument.begin(), argument.end(), argument.begin(), toupper);

    if(argument == "--SAMPLEFILE") sample_config_file = value;
    else if(argument == "--REGIONLIST") region_list = value;
    else if(argument == "--REGIONATRB") region_attributes = value;
    else if(argument == "--DOLATEXTABLE"){
      std::transform(value.begin(), value.end(), value.begin(), toupper);
      do_latex_tables = (value == "TRUE");
    }
    else if(argument == "--DOMVAPLOTS"){
      std::transform(value.begin(), value.end(), value.begin(), toupper);
      do_mva_plots = (value == "TRUE");
    }
    else{ std::cout << "WARNING: Option " << argument << " not recognized." << std::endl; }
  }
  
  TFile* output = TFile::Open("output.root", "RECREATE");

  std::map<std::string, SampleInfo> info_map = {};

  FillSampleInfoMap(info_map, sample_config_file);

  std::map<std::string, std::map<std::string, std::vector<double> > > sample_region_info = {};
  
  // Get the yields from each region
  for(std::map<std::string, SampleInfo>::iterator it = info_map.begin(); it != info_map.end(); ++it){
    
    sample_region_info.insert(std::pair<std::string, std::map<std::string, std::vector<double> > >(it->first, {}));
    
    if(do_latex_tables) FillSampleRegionInfoMap(it->second, sample_region_info.at(it->first), region_list, "YIELDS");
    else if(do_mva_plots) FillSampleRegionInfoMap(it->second, sample_region_info.at(it->first), region_list, "MVA", "MVAScore");
  }
  
  // Generate the latex tables or plots
  for(std::map<std::string, SampleInfo>::iterator it = info_map.begin(); it != info_map.end(); ++it){
    if(it->first == "background"){
      if(do_mva_plots) MakeBackgroundMVAPlots(output, sample_region_info.at("background"));
    }
    else{
      if(do_latex_tables) MakeLaTeXTable(it->first+"_table.tex", (it->second)._label, sample_region_info.at(it->first), sample_region_info.at("background"), 
					 region_attributes);
      else if(do_mva_plots) MakeSignalMVAPlots(output, it->first, sample_region_info.at(it->first), sample_region_info.at("background"));
    }
  }
  
  output->Close();
  delete output;

  return 0;
}
