/*
 VLQAnalysis --outputFile=outVLQAnalysis_VLQ_TT_800_nominal_0.root --inputFile=/nfs/atlas-data07/lvalery/VLQFiles/MBJ-00-01-13-2015_10//././user.mcasolin.302474.Signal.DAOD_SUSY10.e4112_s2608_r6765_r6282_p2419_VLQ_v13_output_tree.root/user.mcasolin.6693688._000001.output_tree.root --textFileList=false --sampleName=VLQ_TT_800 --useLeptonsSF=true --btagOP=85 --useMiniIsolation=false --usePUWeight=false --splitVLQDecays=false --dumpHistos=true --applyMetMtwCuts=false --doTruthAnalysis=false --sampleID=302474. --inputTree=nominal

 */

 int cutflow_exe( const std::string &file, const bool do0L );
//______________________________________________________
//
int cutflow(){
  cutflow_exe( "~/scratch2/VLQAnalysisRun2/VLQAnalysisOutputs_PURW_METMT_LepSF_QCDModeDEP_btagOP77_DataMCPlots25GeVJets_2016_06_07_2057/outVLQAnalysis_ttbar_410000._nominal_Loose_0.root", false);
  std::cout <<"\n \n"<<std::endl;
  cutflow_exe( "~/scratch2/VLQAnalysisRun2/VLQAnalysisOutputs_PURW_METMT_LepSF_QCDModeDEP_btagOP77_DataMCPlots25GeVJets_2016_06_07_2057/outVLQAnalysis_ttbar_410000._nominal_Loose_0.root", true);
  std::cout <<"\n \n"<<std::endl;
  cutflow_exe( "data.root", false);
  std::cout <<"\n \n"<<std::endl;
  cutflow_exe( "data.root", true);
  return 1;
}

int cutflow_exe( const std::string &file, const bool do0L ){
    TFile *f = new TFile(file.c_str(),"read");
    std::vector < TString > types;
    if(!do0L){
      types.push_back("cutFlow_unWeight");
      types.push_back("cutFlow_weight_2");
    } else {
      types.push_back("cutFlow0L_unWeight");
      types.push_back("cutFlow0L_weight_2");
    }

    std::map < int, TString > cuts;
    if(!do0L){
      cuts[1] = "No cut";
      cuts[2] = "Trigger";
      cuts[3] = "Lepton";
      cuts[4] = "Trigger matching";
      cuts[5] = ">=5jets";
      cuts[6] = ">=2b-jets";
      cuts[7] = "MET > 20 GeV";
      cuts[8] = "MET+MTW > 60 GeV";
      cuts[9] = "meff > 400 GeV";
    } else {
      cuts[1] = "No cut";
      cuts[2] = "Trigger";
      cuts[3] = "Lepton veto";
      cuts[4] = "MET > 200 GeV";
      cuts[5] = "dPhi cut";
      cuts[6] = ">=6jets";
      cuts[7] = ">=2b-jets";
      cuts[8] = "meff > 400 GeV";
    }
    for (  auto cut : cuts ){
        std::cout << "Cut : " << cut.second;
        if(cut.first==6 || cut.first==7 || cut.first==4) std::cout << " \t ";
        else std::cout << " \t \t ";
        for (  auto type : types ){
            TH1F* h = (TH1F*) f -> Get(type);
            h->SetDirectory(0);
            std::cout << std::setprecision(10) << h -> GetBinContent( cut.first ) << " \t ";

            delete h;
        }
        std::cout << std::endl;
    }

    return 1;
}
