// For Fitting
#include "Fit/Fitter.h"
#include "Fit/BinData.h"
#include "Fit/UnBinData.h"
#include "Fit/Chi2FCN.h"
#include "Fit/FitResult.h"
#include "Fit/DataOptions.h"
#include "Fit/FitConfig.h"


// For defining the functions
#include "TList.h"
#include "Math/WrappedMultiTF1.h"
#include "HFitInterface.h"

// For plotting
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"


#include <iostream>	// std::cout
#include <vector>


  Double_t background(Double_t *x, Double_t *par) {
      return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
   }

   // Lorentzian Peak function
   Double_t lorentzianPeak(Double_t *x, Double_t *par) {
      return (0.5*par[0]*par[1]/TMath::Pi()) / TMath::Max(1.e-10,(x[0]-par[2])*(x[0]-par[2])+ .25*par[1]*par[1]);
   }

   // Sum of background and peak function
   Double_t fitFunction(Double_t *x, Double_t *par) {
     double par1[] = {par[3],par[4],par[5]};
     return background(x,par) + lorentzianPeak(x,par1);
   }


void unbinnedTest()
{
  gROOT->Reset();
  ROOT::Math::IntegratorOneDimOptions::SetDefaultIntegrator("GAUSS");
  TF1 *fitFcn = new TF1("fitFcn",fitFunction,0,3,6);
  fitFcn->SetParameters(2.5,42,-12.,13.,0.16,1);
  TH1F *histo = new TH1F("example_9_1","Lorentzian Peak on Quadratic Background",60,0,3);
  
  ROOT::Fit::DataOptions opt;
  
  ROOT::Fit::DataRange range(0,3); 
  
  ROOT::Fit::UnBinData data(opt, range,500);
  
  
  for(int i = 0; i < 500; i++){
    double ebl = fitFcn->GetRandom();
    histo->Fill(ebl);
    data.Add(ebl);
  }
  
  fitFcn->SetNormalized(true);
  
  ROOT::Math::WrappedMultiTF1 fitFunction( *fitFcn, fitFcn->GetNdim() );
  ROOT::Fit::Fitter fitter;

  fitter.SetFunction( fitFunction, false);
  double initialParams[] = {0,1,1,1,1,1};
  fitter.Config().SetParamsSettings(6,initialParams);
  fitter.Config().SetUpdateAfterFit();


  fitter.LikelihoodFit(data);

  ROOT::Fit::FitResult r=fitter.Result();
  r.Print(std::cout);

  fitFcn->SetNormalized(false);
  fitFcn->SetParameters(0,1,1,1,1,1);
  cout << "Likelihood FIT " << endl;
  histo->Fit(fitFcn,"LME+");
  cout << "Chi2 FIT " << endl;
  histo->Fit(fitFcn,"ME+");  
  fitFcn->SetParameter(3,0);
  fitFcn->SetLineColor(4);
  fitFcn->Draw("same");
  return;
}
