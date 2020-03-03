enum PROC{TTH,BBH,TBH};

#include "AtlasUtils.C"
#include <vector>
#include <string>

void PlotLimits(PROC proc=PROC::TTH)
{
  /*
loglim_batch_Moriond_5j0J_13_12_2016_v0_Hplus.log
loglim_batch_Moriond_5j0J_13_12_2016_v0_ttbb.log   
loglim_batch_Moriond_5j0J_13_12_2016_v0_tttt.log   
loglim_batch_Moriond_5j_13_12_2016_v0_Hplus.log    
loglim_batch_Moriond_5j_13_12_2016_v0_ttbb.log     
loglim_batch_Moriond_5j_13_12_2016_v0_tttt.log     
loglim_batch_Moriond_6j_13_12_2016_v0_Hplus.log    
loglim_batch_Moriond_6j_13_12_2016_v0_ttbb.log     
loglim_batch_Moriond_6j_13_12_2016_v0_tttt.log     
loglim_batch_Top_13_12_2016_v0_Hplus.log           
loglim_batch_Top_13_12_2016_v0_ttbb.log            
loglim_batch_Top_13_12_2016_v0_tttt.log  
  */

  gROOT  ->ProcessLine(".x AtlasStyle.C");

  std::vector<std::string> FitVariants;
  FitVariants.push_back("Top");
  FitVariants.push_back("Moriond_5j0J");
  FitVariants.push_back("Moriond_5j");
  FitVariants.push_back("Moriond_6j");
  //FitVariants.push_back("Moriond_5jplusHighNj");

  std::vector<std::string> leg_names;
  leg_names.push_back("Top setup");
  leg_names.push_back("5j plus 2b-1/2J, no m_{bb} splitting");
  leg_names.push_back("5j, no m_{bb} splitting");
  leg_names.push_back("6j, no m_{bb} splitting");
  //leg_names.push_back("5j, high N_{j}, no m_{bb} splitting");
  
  std::vector<int> col_list;
  col_list.push_back(kRed);
  col_list.push_back(kBlue);
  col_list.push_back(kOrange);
  col_list.push_back(kMagenta+2);
  col_list.push_back(kSpring+4);
  
  std::string sproc;
  if(proc==PROC::TTH)     sproc="tttt";
  else if(proc==PROC::BBH)sproc="ttbb";
  else if(proc==PROC::TBH)sproc="Hplus";
  else {std::cout<<"proc option not found, exit"<<std::endl; exit(0);}
  
  std::string sytitle;
  if(proc==PROC::TTH)     sytitle="Excluded #sigma(ttH) X BR(H #rightarrow tt)";
  else if(proc==PROC::BBH)sytitle="Excluded #sigma(bbH) X BR(H #rightarrow tt)";
  else if(proc==PROC::TBH)sytitle="Excluded #sigma(tbH^{+}) X BR(H #rightarrow tb)";
  else {std::cout<<"proc option not found, exit"<<std::endl; exit(0);} 

  std::vector < TGraph* > graph_list=GetGraph(FitVariants,sproc);

  TCanvas *mycnv = new TCanvas();
  mycnv ->SetTopMargin(0.17);
  mycnv ->SetGridy();
  
  for(int i=0; i<graph_list.size(); i++)
    {
      graph_list[i]->SetLineColor(col_list[i]);
      graph_list[i]->SetMarkerColor(col_list[i]);
      graph_list[i]->SetMarkerStyle(20);
      if(i==0){
	graph_list[i]->GetYaxis()->SetTitle(sytitle.c_str());
	graph_list[i]->GetXaxis()->SetTitle("Higgs boson mass [GeV]");
	graph_list[i]->Draw("ape");
      }
      else graph_list[i]->Draw("samep");      
    }

  //TLegend * leg = new TLegend(xmin,ymin,xmax,ymax);
  TLegend * leg = new TLegend(0.05,0.83,0.98,0.95);
  leg->SetNColumns(2);
  leg->SetTextSize(0.038);
  for(int i=0; i<leg_names.size(); i++) leg->AddEntry(graph_list[i],leg_names[i].c_str(),"p");
  leg->SetShadowColor(1);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetFillColor(kWhite);
  leg->Draw();

  char title[1000]; sprintf(title,"plot_%s.pdf",sproc.c_str());
  mycnv->SaveAs(title);

  std::vector< TGraph* > graph_list_ratio=GraphRatio(graph_list);

  TCanvas *rmycnv = new TCanvas();
  rmycnv ->SetTopMargin(0.17);
  rmycnv ->SetGridy();
  
  for(int i=1; i<graph_list_ratio.size(); i++)
    {
      graph_list_ratio[i]->SetLineColor(col_list[i]);
      graph_list_ratio[i]->SetMarkerColor(col_list[i]);
      graph_list_ratio[i]->SetMarkerStyle(20);
      if(i==1){
	graph_list_ratio[i]->GetYaxis()->SetTitle(sytitle.c_str());
	graph_list_ratio[i]->GetYaxis()->SetRangeUser(0.2,1.4);
	graph_list_ratio[i]->GetXaxis()->SetTitle("Higgs boson mass [GeV]");
	graph_list_ratio[i]->Draw("ape");
      }
      else graph_list_ratio[i]->Draw("samep");      
    }

  //TLegend * leg = new TLegend(xmin,ymin,xmax,ymax);
  TLegend * rleg = new TLegend(0.05,0.85,0.95,0.94);
  rleg->SetNColumns(2);
  rleg->SetTextSize(0.038);
  for(int i=1; i<leg_names.size(); i++) rleg->AddEntry(graph_list_ratio[i],leg_names[i].c_str(),"p");
  rleg->SetShadowColor(1);
  rleg->SetBorderSize(0);
  rleg->SetFillStyle(0);
  rleg->SetFillColor(kWhite);
  rleg->Draw();

  TLegend * leg_1 = new TLegend(0.1,0.95,0.90,0.99);
  leg_1->SetTextSize(0.04);
  char leg_text[1000]; sprintf(leg_text,"Limits normalised to Top-conf.");
  leg_1->AddEntry( (TObject*)0,leg_text,"");
  leg_1->SetShadowColor(1);
  leg_1->SetBorderSize(0);
  leg_1->SetFillStyle(0);
  leg_1->SetFillColor(kWhite);
  leg_1->Draw();


  char rtitle[1000]; sprintf(rtitle,"plot_ratio_%s.pdf",sproc.c_str());
  rmycnv->SaveAs(rtitle);

 }


std::vector< TGraph* > GetGraph(std::vector<std::string> FitVariants, std::string sproc)
{
  std::vector< TGraph* > graphs; 

  for(int i=0; i<FitVariants.size(); i++)
    {
      //loglim_batch_Top_13_12_2016_v0
      char fit_res[1000]; sprintf(fit_res,"loglim_batch_%s_13_12_2016_v0_%s.log",FitVariants[i].c_str(),sproc.c_str());

      std::cout<<"Opening "<<fit_res<<std::endl;
      TGraph *myg = new TGraph(fit_res);
      graphs.push_back(myg);
    }

  return graphs;
}


std::vector< TGraph* > GraphRatio(std::vector< TGraph* > graphs)
{
  std::vector< TGraph* > graphs_ratio; 

  Int_t n0     = graphs[0]->GetN();
  const Double_t *x0 = graphs[0]->GetX();
  const Double_t *y0 = graphs[0]->GetY();

  
  for(int i=0; i<graphs.size(); i++)
    {
      Int_t n           = graphs[i]->GetN();
      const Double_t *x = graphs[i]->GetX();
      const Double_t *y = graphs[i]->GetY();


      if(i!=0)
	for(int bin=0; bin<n; bin++)
	  {
	    y[bin]=y[bin]/y0[bin];
	  }
      
      TGraph *myg = new TGraph(n,x,y);
      graphs_ratio.push_back(myg);
    }

  return graphs_ratio;
}



  
