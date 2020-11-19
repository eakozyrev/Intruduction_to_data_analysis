#include "TH1D.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include "TF1.h"

int fit(){

  TFile *file = new TFile("file.root","recreate");
  
  TCanvas *s0 = new TCanvas("s0","fit with gausn by Likelihood",700,600);
  TH1D *h = new TH1D("h","Simple fit",100,-5,5);
  h->FillRandom("gaus",440);
  
  TFitResultPtr r0 = h->Fit("gaus","L S M","E1");
  cout<<"Mean= "<<r0->Parameter(1)<<" +- "<<r0->ParError(1)<<endl;
  cout<<"Mean= "<<r0->Parameter(1)<<" + "<<r0->UpperError(1)<<" - "<<r0->LowerError(1)<<endl;
  cout << "r0->Chi2() = " << r0->Chi2() <<  endl;
  r0->Print();
  r0->PrintCovMatrix(cout);


  cout << "Comparison chi2 and maximum Likelihood fits" << endl;
  TCanvas *s = new TCanvas("s","Comparison chi2 and maximum Likelihood fits",700,600);
  TF1 *f_fit = new TF1("f_fit","gausn",-5,5);
  h->Draw();
  r0 = h->Fit(f_fit,"S M0+","e");
  f_fit->Draw("same");				  
  f_fit->SetLineColor(4);				  
  cout << "r0->Chi2() = " << r0->Chi2() <<  endl;

  //Результаты аппроксимации доступны через функцию f_fit либо через объект класса TFitResultPtr
  cout << "Первый параметр = " <<  r0->Parameter(1) << " или  " << f_fit->GetParameter(1) << endl;
  
  TPaveText *f = new TPaveText(1,15,4,20);
  f->AddText("Likelihood");
  ((TText*)f->GetListOfLines()->Last())->SetTextColor(kRed);
  f->AddText("#chi^{2}");
  ((TText*)f->GetListOfLines()->Last())->SetTextColor(kBlue);
  f->Draw();

							       
  
  TH1D hh("hh","Simple fit",100,-5,5);
  TH1D *h_chi2 = new TH1D("h_chi2","#chi^{2}",100,0,200);
  TH1D *h_p0 = new TH1D("h_p0","Constant/ generated number",100000,0.85,1.15);
  TH1D *h_p1 = new TH1D("h_p1","Mean",100,-0.1,0.1);
  TH1D *h_p2 = new TH1D("h_p2","Sigma",100,0.9,1.1);
  
  TCanvas *c0 = new TCanvas("c0","The fits of TOY spectra",700,600);
  c0->Divide(2,2);
  c0->cd(1);
  int number_events = 10000;
  for(int i=0; i<1000; i++){
    hh.Reset();
    hh.FillRandom("gaus",number_events);
    TFitResultPtr r = hh.Fit("gausn","LSQ"); // P L
    h_chi2->Fill( r->Chi2() );
    h_p0->Fill( 10. * r->Parameter(0)/number_events);
    h_p1->Fill( r->Parameter(1) );
    h_p2->Fill( r->Parameter(2) );
  }
  c0->cd(1);
  h_chi2->Draw();  
  c0->cd(2);
  h_p0->Draw();
  TLine *line = new TLine(1,0,1,120);
  line->Draw();
  c0->cd(3);
  h_p1->Draw();
  c0->cd(4);
  h_p2->Draw();
  c0->Draw();
  file->Save();
  return 1;
}
