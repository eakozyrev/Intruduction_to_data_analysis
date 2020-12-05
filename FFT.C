#include "TH1D.h"
#include "TVirtualFFT.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TMath.h"

void FFT()
{

   //prepare the canvas for drawing
   TCanvas *myc = new TCanvas("myc", "Fast Fourier Transform", 800, 600);
   myc->Divide(2,2);
   myc->cd(1);
   //A function to sample
   TF1 *fsin = new TF1("fsin", "sin(x)+sin(7*x)+sin(3*x)+1+TMath::Gaus(x,20,4)", 0, 40);
   //fsin->Draw();
   
   Int_t n=400;
   TH1D *hsin = new TH1D("hsin", "hsin", n+1, 0, 40);
   Double_t x;
   
   //Fill the histogram with function values
   for (Int_t i=0; i<=n; i++){
      x = (Double_t(i)/n)*(40);
      hsin->SetBinContent(i+1, fsin->Eval(x));
   }
   hsin->Draw();
   fsin->GetXaxis()->SetLabelSize(0.05);
   fsin->GetYaxis()->SetLabelSize(0.05);
   
   myc->cd(2);
   //Compute the transform and look at the magnitude of the output
   TH1 *hm =0;

   hm = hsin->FFT(hm, "MAG");
   hm->SetTitle("Magnitude of the transform");
   hm->GetXaxis()->SetLimits(0,hm->GetXaxis()->GetXmax()/2./TMath::Pi());
   hm->Draw();
   
   hm->SetStats(kFALSE);
   hm->GetXaxis()->SetLabelSize(0.05);
   hm->GetYaxis()->SetLabelSize(0.05);
   myc->cd(3);
   //Look at the phase of the output   
   TH1 *hp = 0;
   hp = hsin->FFT(hp, "PH");
   hp->SetTitle("Phase of the 1st transform");
   hp->Draw();
   hp->SetStats(kFALSE);
   hp->GetXaxis()->SetLabelSize(0.05);
   hp->GetYaxis()->SetLabelSize(0.05);
   
   TVirtualFFT *fft = TVirtualFFT::GetCurrentTransform();
   myc->cd(4);
   //Use the following method to get the full output:
   Double_t *re_full = new Double_t[n];
   Double_t *im_full = new Double_t[n];
   fft->GetPointsComplex(re_full,im_full);
  
   //Now let's make a backward transform:
   TVirtualFFT *fft_back = TVirtualFFT::FFT(1, &n, "C2R");
   fft_back->SetPointsComplex(re_full,im_full);
   fft_back->Transform();
   TH1 *hb = 0;
   //Let's look at the output
   hb = TH1::TransformHisto(fft_back,hb,"Re");
   hb->SetTitle("The backward transform result");
   hb->Draw();

   hb->SetStats(kFALSE);
   hb->GetXaxis()->SetLabelSize(0.05);
   hb->GetYaxis()->SetLabelSize(0.05);

}
