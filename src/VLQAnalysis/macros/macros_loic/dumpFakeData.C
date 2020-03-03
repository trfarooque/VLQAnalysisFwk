
int produceNewRootFile( const TString &name );

int dumpFakeData(){
  produceNewRootFile("observed_expected");
  produceNewRootFile("observed_RealDT_1J3BHM");
  produceNewRootFile("observed_RealDT_1J4BHM");
  produceNewRootFile("observed_RealDT_2J3B");
  produceNewRootFile("observed_RealDT_2J4B");

  produceNewRootFile("observed_RealDT_1J3BHM_1J4BHM");
  produceNewRootFile("observed_RealDT_2J3B_2J4B");
  produceNewRootFile("observed_RealDT_1J3BHM_1J4BHM_2J3B_2J4B");
  return 1;
}

int produceNewRootFile( const TString &name ){

  std::cout << "==> Processing fake data file for: " << name << std::endl;

  //definition of the different regions to consider
  std::map < std::string, bool > regions;
  regions.insert( std::pair < std::string, bool >( "c1l0RCTTMass6j3b_meff", false) );
  regions.insert( std::pair < std::string, bool >( "c1l0RCTTMass6j4b_meff", false) );
  regions.insert( std::pair < std::string, bool >( "c1l1RCTTMass6j3bLowMbb_meff", false) );
  regions.insert( std::pair < std::string, bool >( "c1l1RCTTMass6j4bLowMbb_meff", false) );
  regions.insert( std::pair < std::string, bool >( "c1l1RCTTMass6j3bHighMbb_meff", false) );
  regions.insert( std::pair < std::string, bool >( "c1l1RCTTMass6j4bHighMbb_meff", false) );
  regions.insert( std::pair < std::string, bool >( "c1l2RCTTMass6j3b_meff", false) );
  regions.insert( std::pair < std::string, bool >( "c1l2RCTTMass6j4b_meff", false) );
  if( name == "observed_expected" ){
    //do nothing here (we use postFit backgorund everywhere)
  }
  if( name.Contains("1J3BHM") ){
    regions["c1l1RCTTMass6j3bHighMbb_meff"] = true;
  }
  if( name.Contains("1J4BHM") ){
    regions["c1l1RCTTMass6j4bHighMbb_meff"] = true;
  }
  if( name.Contains("2J3B") ){
    regions["c1l2RCTTMass6j3b_meff"] = true;
  }
  if( name.Contains("2J4B") ){
    regions["c1l2RCTTMass6j4b_meff"] = true;
  }

  //Data rootfile
  TFile *f = new TFile( "Data.root", "read");
  if(!f){
    std::cerr << "<!> ERROR: the file cannot be opened. Please check." << std::endl;
    return -1;
  }

  //Output file !
  TFile *out = new TFile( name + ".root", "recreate");

  //Now looping
  for ( auto region : regions ){
    const bool isData = region.second;
    TH1F* histo = 0;
    if(isData){
      histo = (TH1F*)f->Get((TString)region.first)->Clone();
      histo -> SetDirectory(0);
    } else {
      TString fileName = "postFit/";
      fileName += region.first;
      fileName += "_postFit.root";
      TFile *postFitFile = new TFile(fileName,"read");
      histo = (TH1F*)postFitFile->Get("h_tot_postFit")->Clone();
      histo -> SetDirectory(0);
      postFitFile->Close();
    }

    histo -> SetName((TString)region.first);
    out -> cd();
    histo -> Write();
  }
  out -> Close();
  return 1;
}
