#include "TRandom.h"
#include "TH2D.h"
#include "fstream"
#include "iostream"
#include "TCanvas.h"
using namespace std;


int h9_09_20(){

  TCanvas *s = new TCanvas();
  TRandom Rm;
  s->Divide(3,2);
  s->cd(1);
  TH2D *hist1 = new TH2D("histname","histtitle",100,0,1,100,0,1);
  hist1->SetXTitle("X axis, popugai1");
  hist1->SetYTitle("Y axis, popugai2"); 
  for(int i = 0; i < 10000; i++)hist1->Fill(Rm.Gaus(0.5,0.1),Rm.Gaus(0.6,0.04));
  hist1->SetMarkerSize(0.8);
  hist1->Draw(); //"surf1"

  s->cd(2);
  TH1D *hist2 = new TH1D("hist2name","hist2title",100,0,1);
  hist2 = hist1->ProjectionX();
  hist2->SetLineColor(2);
  hist2->SetLineWidth(4);
  hist2->SetMarkerSize(1.9);
  hist2->SetTitle(Form("#splitline{Scale, GetEntries() = %g,}{GetSum() = %g, GetSumOfWeights() = %g}", hist2->GetEntries(), hist2->GetSum(),hist2->GetSumOfWeights()));
  hist2->Draw();

  s->cd(3);
  TH1D *hist3 = (TH1D *)hist2->Clone();
  hist3->Scale(0.8);
  hist3->SetTitle(Form("#splitline{Scale, GetEntries() = %g,}{GetSum() = %g, GetSumOfWeights() = %g}", hist3->GetEntries(), hist3->GetSum(),hist3->GetSumOfWeights()));
  hist3->Draw("e");

  s->cd(4);
  TH1D *hist4 = new TH1D(*hist3);
  hist4->SetBinContent(70,500);
  hist4->SetTitle(Form("#splitline{Scale + SetBinContent, GetEntries() = %g,}{GetSum() = %g, GetSumOfWeights() = %g}", hist4->GetEntries(), hist4->GetSum(),hist4->GetSumOfWeights()));
  hist4->Draw("HIST");
  hist4->SetLineColor(1);

  s->cd(5);
  TH1D *hist5 = new TH1D(*hist4);
  hist5->Rebin(4);
  hist5->Draw("AH");

  s->cd(6);
  TH1D *hist6 = new TH1D("histname6","histtitle6",100,0,1);
  hist6->Add(hist4,2);
  hist6->Add(hist1->ProjectionY(),1.5);
  hist6->SetTitle("2*popugai1 + 1.5*popugai2");
  hist6->SetFillStyle(3002);
  hist6->SetFillColor(3);
  hist6->Draw("HIST");

  s->SaveAs("09.09.20.root");
  return 1;
}
