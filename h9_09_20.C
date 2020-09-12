#include "TRandom.h"
#include "TH2D.h"
#include "fstream"
#include "iostream"
#include "TCanvas.h"
using namespace std;


int h9_09_20(){

  TCanvas *s = new TCanvas();
  s->Divide(3,2);
  s->cd(1);
  TH2D *hist1 = new TH2D("histname","histtitle",100,0,1,100,0,1);
  hist1->SetXTitle("X axis, popugai1");
  hist1->SetYTitle("Y axis, popugai2"); 
  for(int i = 0; i < 100; i++)hist1->Fill(0.4,0.5);
  hist1->SetMarkerSize(0.3);
  hist1->Draw();

  s->cd(2);
  TH1D *hist2 = new TH1D("hist2name","hist2title",100,0,1);
  hist2 = hist1->ProjectionX();
  hist2->SetLineColor(2);
  hist2->SetLineWidth(4);
  hist2->SetMarkerSize(1.9);
  hist2->Draw();

  s->cd(3);
  hist2->Draw("e");

  s->cd(4);
  TH1D *hist3 = new TH1D(*hist2);
  hist3->Scale(0.8);
  hist3->SetTitle("Scale");
  hist3->Draw("e");

  s->cd(5);
  TH1D *hist4 = new TH1D(*hist2);
  hist4->SetTitle("SetNormfactor");
  hist4->SetNormFactor(70);
  hist4->Draw("e");  


  s->cd(6);
  TH1D *hist5 = new TH1D("hist5name","hist5title",100,0,1);
  TH1D *hist6 = new TH1D("hist6name","hist6title",100,0,1);
  hist6 = hist1->ProjectionY();
  hist5->Add(hist3,2);
  hist5->Add(hist6,1.5);
  hist5->SetTitle("2*popugai1 + 1.5*popugai2");
  hist5->Draw("e");

  s->SaveAs("09.09.20.root");
  
  return 1;
}
