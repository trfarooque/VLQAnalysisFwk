#include <iostream>


int compareSliceToInclusive(){
    
    TH1::SetDefaultSumw2;
    
    std::vector< std::string > variables;
    variables.push_back("meff");
    variables.push_back("truth_ht");
    variables.push_back("jets_n");
    variables.push_back("TTRCLooser_jets_n");
    
    std::vector< std::string > regions;
    regions.push_back("c1l2b");
    regions.push_back("c1l2TTRCLooser5j2b");
    regions.push_back("c1l2TTRCLooser5j3b");
    regions.push_back("c1l2TTRCLooser5j4b");
    regions.push_back("c1l2TTRCLooser6j2b");
    regions.push_back("c1l2TTRCLooser6j3b");
    regions.push_back("c1l2TTRCLooser6j4b");
    
    regions.push_back("c1l1TTRCLooser5j2b");
    regions.push_back("c1l1TTRCLooser5j3b");
    regions.push_back("c1l1TTRCLooser5j4b");
    regions.push_back("c1l1TTRCLooser6j2b");
    regions.push_back("c1l1TTRCLooser6j3b");
    regions.push_back("c1l1TTRCLooser6j4b");
    
    regions.push_back("c1l0TTRCLooser5j2b");
    regions.push_back("c1l0TTRCLooser5j3b");
    regions.push_back("c1l0TTRCLooser5j4b");
    regions.push_back("c1l0TTRCLooser6j2b");
    regions.push_back("c1l0TTRCLooser6j3b");
    regions.push_back("c1l0TTRCLooser6j4b");
    
    std::vector< std::string > distributions;
    for( const auto reg : regions ){
        for ( const auto var : variables ){
            distributions.push_back(reg + "_" + var);
            
        }
    }
    
    TFile *f_slice0 = new TFile("slice0.root","read");
    TFile *f_slice1 = new TFile("slice1.root","read");
    TFile *f_slice2 = new TFile("slice2.root","read");
    TFile *f_slice3 = new TFile("slice3.root","read");
    TFile *f_incl = new TFile("incl.root","read");
    
    
    for( const auto distrib : distributions ){
        TH1F* h_slice0 = (TH1F*)f_slice0 -> Get(distrib.c_str()); h_slice0 -> SetDirectory(0); h_slice0 -> SetFillColor(kGreen+2); h_slice0 -> SetLineColor(kGreen+2);
        TH1F* h_slice1 = (TH1F*)f_slice1 -> Get(distrib.c_str()); h_slice1 -> SetDirectory(0); h_slice1 -> SetFillColor(kBlue); h_slice1 -> SetLineColor(kBlue);
        TH1F* h_slice2 = (TH1F*)f_slice2 -> Get(distrib.c_str()); h_slice2 -> SetDirectory(0); h_slice2 -> SetFillColor(kRed); h_slice2 -> SetLineColor(kRed);
        TH1F* h_slice3 = (TH1F*)f_slice3 -> Get(distrib.c_str()); h_slice3 -> SetDirectory(0); h_slice3 -> SetFillColor(kOrange); h_slice3 -> SetLineColor(kOrange);
        TH1F* h_incl = (TH1F*)f_incl -> Get(distrib.c_str()); h_incl -> SetDirectory(0); h_incl -> SetFillColor(0); h_incl -> SetLineColor(kBlack); h_incl -> SetFillStyle(0); h_incl -> SetLineWidth(2); h_incl -> SetMarkerStyle(20);
        
        TH1F* h_sum = (TH1F*)h_slice0 -> Clone();
        h_sum -> Add(h_slice1);
        h_sum -> Add(h_slice2);
        h_sum -> Add(h_slice3);
        h_sum -> SetLineColor(kBlack);
        h_sum -> SetLineStyle(1);
        h_sum -> SetFillColor(kBlack);
        h_sum -> SetFillStyle(3005);
        
        THStack *hs = new THStack("hs","");
        hs -> Add(h_slice0);
        hs -> Add(h_slice1);
        hs -> Add(h_slice2);
        hs -> Add(h_slice3);
        
        TCanvas c;
        c.SetFillColor(0);
        hs -> Draw("HIST");
        hs -> GetXaxis() -> SetTitle( h_slice0 -> GetXaxis() -> GetTitle() );
        h_incl -> Draw("samee0");
        h_sum -> Draw("samee2");
        c.Print( (distrib+".png").c_str());
        c.SetLogy();
        c.Print( (distrib+"_log.png").c_str());
        
        delete h_slice0;
        delete h_slice1;
        delete h_slice2;
        delete h_slice3;
        delete h_incl;
        delete hs;
        delete h_sum;
    }
    return 1;
}