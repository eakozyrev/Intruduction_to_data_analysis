#include "TVirtualFitter.h"
#include "TGraphErrors.h"
#include "TMinuit.h"

{

  gSystem->Load("libMinuit");
  TRandom3 rndm;
  TF1 *func = new TF1("funcname","[0]+[1]*x",0,10);
  TCanvas *s = new TCanvas();
  s->Divide(2,2);
  s->cd(1);
  TH1D *hist = new TH1D("hist","fit by [0]+[1]*x",100,0,10);

  for(int i = 0; i < 1000; i++){

    hist->Fill(rndm.Rndm()*10);

  }
   TVirtualFitter::SetMaxIterations(10000);
   func->SetLineColor(4);
   func->SetLineWidth(10);
   TFitResultPtr r = hist->Fit(func,"S","E1");
   func->SetLineColor(2);
   func->SetLineWidth(1);
   func->SetLineStyle(2);
   r->Print();
   s->cd(2);
   TGraph *gr = new TGraph();TGraph *gr1 = new TGraph();
   int i = 0;
   for(double x = -1.3; x < 1.3; x+=0.04){
     func->FixParameter(1,x);
     r = hist->Fit(func,"SQ+");;
     gr->SetPoint(i,x,r->MinFcnValue());
     gr1->SetPoint(i,x,r->Parameter(0));
     i++;
   }
   gr->Draw("ap*");
   s->cd(3);
   gr1->Draw("ap*");

   s->cd(4);
   func->ReleaseParameter(1);
   hist->Fit(func,"Q+");
   TGraph *gr12 = (TGraph*)gMinuit->Contour(100,0,1);
   gr12->Draw("alp");			
   
}
