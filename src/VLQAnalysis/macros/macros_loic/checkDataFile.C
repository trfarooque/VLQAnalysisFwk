
int produceNewRootFile( const TString &name );

int checkDataFile(){
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

  //Output file !
  TFile *out = new TFile( name + ".root", "read");

  //Now looping
  for ( auto region : regions ){
    TH1F* hist = (TH1F*)out -> Get((TString)region.first);
    std::cout << "    -> Region: " << region.first << "   -> " << hist -> Integral() << std::endl;
  }
  out -> Close();
  return 1;
}
