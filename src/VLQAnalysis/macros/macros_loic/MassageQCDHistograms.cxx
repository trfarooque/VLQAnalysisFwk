//
// Massage the histograms for QCD
//
// Usage
// =========
//      root -l -b -q 'MassageQCDHistograms.cxx+("QCD.root")' | grep DECISION
//
//

#include <iostream>
#include <map>
#include <iomanip>

#include "TFile.h"
#include "TTree.h"
#include "TIterator.h"
#include "TList.h"
#include "TCanvas.h"
#include "TPie.h"
#include "TKey.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TLegend.h"

bool floateq(float a, float b){ return fabs(a-b)<1.e-10; }
bool floateq(double a, double b){ return fabs(a-b)<1.e-10; }

//__________________________________________________________________________
//
void CompareBeforeAfter( TH1F* newHist, TH1F* oldHist ){
  TCanvas c;
  newHist -> SetLineColor(kGreen+2);
  newHist -> SetFillColor(kGreen+2);
  newHist -> SetLineWidth(2);
  newHist -> SetFillStyle(3003);

  oldHist -> SetLineColor(kRed);
  oldHist -> SetLineWidth(2);
  oldHist -> SetFillStyle(0);

  double _max_ = TMath::Max(newHist->GetMaximum(), oldHist->GetMaximum());
  newHist -> Draw("e6");

  TH1F* linenewHist = (TH1F*)newHist -> Clone( (TString)(newHist->GetName()) + "_new" );
  linenewHist -> SetFillStyle(0);
  linenewHist -> Draw("histsame");

  newHist -> SetMaximum( _max_ * 1.3 );
  oldHist -> Draw("same");

  TLegend *leg = new TLegend( 0.6, 0.6, 0.8, 0.8);
  leg -> SetLineColor(0);
  leg -> SetFillColor(0);
  leg -> AddEntry(oldHist, "Before treatment", "l");
  leg -> AddEntry(newHist, "After treatment", "l");
  leg -> Draw();

  gSystem -> mkdir("Plots/");
  c.Print( "Plots/" + (TString)(oldHist->GetName()) + ".png" );
}

//__________________________________________________________________________
//
void FixNullBins( TH1F* newHist ){
  for( unsigned int iBin = 1; iBin <= newHist -> GetNbinsX(); ++iBin ){
    if( newHist -> GetBinContent(iBin) < 1e-06 ){
      newHist -> SetBinContent( iBin, 1e-09 );
      newHist -> SetBinError( iBin, 1e-09 );
    }
  }
}

//__________________________________________________________________________
//
void ResetHistogram( TH1F* newHist ){
  for( unsigned int iBin = 1; iBin <= newHist -> GetNbinsX(); ++iBin ){
    newHist -> SetBinContent( iBin, 1e-09 );
    newHist -> SetBinError( iBin, 1e-09 );
  }
}

//__________________________________________________________________________
//
void CheckNegativeBins( TH1F* newHist ){
  double integral = newHist -> Integral();
  for( unsigned int iBin = 1; iBin <= newHist -> GetNbinsX(); ++iBin ){
    if( newHist -> GetBinContent(iBin) < 0 ){
      newHist -> SetBinContent( iBin, 0. );
      newHist -> SetBinError( iBin, 0. );
    }
  }
  double newIntegral = newHist->Integral();
  if(newIntegral < 1e-05){
    ResetHistogram(newHist);
    return;
  }

  newHist -> Scale( integral / newIntegral );
  FixNullBins( newHist );
}

//__________________________________________________________________________
//
void CheckStatisticalFluctuations( TH1F* newHist ){
  double integral = newHist -> Integral();
  for( unsigned int iBin = 1; iBin <= newHist -> GetNbinsX(); ++iBin ){
    if( fabs(newHist -> GetBinContent(iBin)) - fabs(newHist -> GetBinError(iBin)) <= 1e-05 ){
      newHist -> SetBinContent( iBin, 0. );
      newHist -> SetBinError( iBin, 0. );
    }
  }
  double newIntegral = newHist->Integral();
  if(newIntegral < 1e-05){
    ResetHistogram(newHist);
    return;
  }

  newHist -> Scale( integral / newIntegral );
  FixNullBins( newHist );
  return;
}

//__________________________________________________________________________
//
void RecomputeStatUncertainty( TH1F* newHist, TH1F* oldHist ){
  for( unsigned int iBin = 1; iBin <= newHist -> GetNbinsX(); ++iBin ){
    //
    // Original stat uncertainty for this bin
    //
    double original_content   = oldHist -> GetBinContent(iBin);
    double original_error     = oldHist -> GetBinError(iBin);
    double original_rel_error = original_error ? fabs(original_error/original_content) : 0;
    original_rel_error = original_rel_error>1 ? 1. : original_rel_error;

    //
    // Sets the uncertainty in the new histogram
    //
    newHist -> SetBinError( iBin, newHist -> GetBinContent(iBin) * original_rel_error );
  }
}

//__________________________________________________________________________
//
void SmoothAndCureTheHistogram( TH1F* newHist, TH1F* oldHist, bool inrange=false ){

  std::string opt_smooth = inrange ? "R" : "";

  //
  // Check integral
  //
  double err;
  double integral = newHist -> IntegralAndError( 0, newHist->GetNbinsX(), err );
  if( fabs(integral)-fabs(err) <= 1e-06 || integral <= 1e-06 ){
    ResetHistogram( newHist );
    CompareBeforeAfter(newHist,oldHist);
    return;
  }

  //
  // Apply ROOT's smoothing to have smoother templates
  //
  if( newHist -> GetNbinsX() > 2 ){
    newHist -> Smooth(1, opt_smooth.c_str());
  }

  //
  // Check huge statistical fluctuations
  //
  //CheckStatisticalFluctuations( newHist );

  //
  // Check negative bins while conserving the integral constant:
  //
  CheckNegativeBins( newHist );

  //
  // Apply ROOT's smoothing to have smoother templates
  //
  if( newHist -> GetNbinsX() > 2 ){
    newHist -> Smooth(1, opt_smooth.c_str());
  }

  RecomputeStatUncertainty(newHist,oldHist);
  CompareBeforeAfter(newHist,oldHist);
}

//__________________________________________________________________________
//
// inRange - will only smooth the histogram in the range where the bins are filled
//
int MassageQCDHistograms( const std::string &fileName, bool inRange=false ){

  std::cout << "==> Analysing histograms located in the file: " << fileName << std::endl;

  TFile *f = new TFile( fileName.c_str(), "read");
  if(!f){
    std::cerr << "<!> ERROR: the file cannot be opened. Please check." << std::endl;
    return -1;
  }

  TFile *out = new TFile( "QCD_new.root", "recreate");

  //
  // Loop over the content of the file and studies all trees
  //
  TList *list = f -> GetListOfKeys();
  TIterator *it = list -> MakeIterator();

  std::map < std::string, long > map_sizes;
  while ( TKey *key = (TKey*)it -> Next() ){

    TObject *obj = (TObject*)key -> ReadObj();
    //if(!((TString)obj->GetName()).Contains("_meff")) continue;
    //if(((TString)obj->GetName()).Contains("_zoom")) continue;
    if(((TString)obj->GetName()).Contains("_tageff")) continue;
    if(((TString)obj->GetName()).Contains("_matcheff")) continue;
    //if(((TString)obj->GetName()).Contains("_el_")) continue;
    //if(((TString)obj->GetName()).Contains("_mu_")) continue;
    if(((TString)obj->GetName()).Contains("c0lep")) continue;

    if( obj -> InheritsFrom (TH1::Class()) ){
      TH1F *origin_hist = (TH1F*)obj -> Clone();
      origin_hist -> SetDirectory(0);

      TH1F *new_hist = (TH1F*)origin_hist -> Clone( (TString)(origin_hist->GetName()) + "_new" );
      new_hist -> SetDirectory(0);

      if(inRange){

	int bmin=1;
	int bmax=origin_hist->GetNbinsX(); 
	
	for(int i = 1; i < origin_hist->GetNbinsX(); i++){
	  if( floateq(origin_hist->GetBinContent(i),0.0) ) continue;
	  if(bmin == 1)	bmin = i;
	  bmax = i;
	}
	new_hist->GetXaxis()->SetRange(bmin, bmax);

      }

      SmoothAndCureTheHistogram(new_hist,origin_hist, inRange);
      if(inRange){
	new_hist->GetXaxis()->SetRange(1,origin_hist->GetNbinsX());
      }
      if(new_hist -> Integral() < 1e-05){
        std::cout << "DECISION ----> REMOVED in " << origin_hist -> GetName() << std::endl;
      } else {
        std::cout << "DECISION ----> KEPT in " << origin_hist -> GetName() << std::endl;
      }
      out -> cd();
      new_hist -> Write(origin_hist -> GetName());

    }

  }
  out -> Close();
  return 1;
}
