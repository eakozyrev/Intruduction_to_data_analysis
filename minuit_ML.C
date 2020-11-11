//
//   Example of a program to fit non-equidistant data points
//   =======================================================
//
//   The fitting function fcn is a simple chisquare function
//   The data consists of 5 data points (arrays x,y,z) + the errors in errorsz

#include "TVirtualFitter.h"
#include "TGraphErrors.h"
#include "TMinuit.h"

double z[5],x[5],errorz[5];

//______________________________________________________________________________
double funct(double *x, double *par)
{
  double value=par[0]*(par[2]*par[2]/4.0)/ ((x[0]-par[1])*(x[0]-par[1])+par[2]*par[2]/4.0) + par[3];
  return value;
}

//______________________________________________________________________________
void fcn(Int_t &npar, double *gin, double &f, double *par, Int_t iflag)
{
  int nbins = 5;
  int i;  
  //calculate chisquare
  double chisq = 0;
  double delta;
  for (i=0;i<nbins; i++) {
    double val = x[i];
    delta  = TMath::Poisson(z[i],funct(&val,par));
    chisq += -2.*log(delta);
  }
  
  f = chisq;
}

double fcn1(double *par){

  int nbins = 5;
  int i;
  
  //calculate chisquare
  double chisq = 0;
  double delta;
  for (i=0;i<nbins; i++) {
    double val = x[i];
    delta  = (z[i]-funct(&val,par))/errorz[i];
    chisq += delta*delta;
    //cout << delta << "  " << z[i] << " " << funct(&val,par) << " " <<errorz[i] << " "   << chisq << endl;
  }
      // cout << chisq << " " << par[0] << " " << par[1] << " " << par[2] << " " << par[3] << endl;
  return chisq;
  
}

//______________________________________________________________________________
void minuit()
{
  // The z values
  z[0]=15.0;
  z[1]=30.0;
  z[2]=90.0;
  z[3]=35.0;
  z[4]=18.0;
  // The errors on z values
  double error = 5.0;
  errorz[0]=error;
  errorz[1]=error;
  errorz[2]=error;
  errorz[3]=error;
  errorz[4]=error;
  // the x values
  x[0]=100.0;
  x[1]=110.0;
  x[2]=120.0;
  x[3]=130.0;
  x[4]=140.0;
  
  TVirtualFitter *Minuit = TVirtualFitter::Fitter(0,4);  //initialize TMinuit with a maximum of 4 params
  Minuit->SetFCN(fcn);
  
  // Set starting values and step sizes for parameters
   double vstart[4] = {80.0, 120.0 , 10.0 , 5.0};
   double step[4] = {1.0 , 1.0 , 1.0 , 1.0};
  double ierflg = 0.0001;
  Minuit->SetParameter(0, "a1", vstart[0], step[0], 0,0);
  Minuit->SetParameter(1, "a2", vstart[1], step[1], 0,0);
  Minuit->SetParameter(2, "a3", vstart[2], step[2], 0,0);
  Minuit->SetParameter(3, "a4", vstart[3], step[3], 0,0);
  
  // Now ready for minimization step
  double arglist[10];
  arglist[0] = 0;
  arglist[1] = 0.1;
  Minuit->ExecuteCommand("MINOS",arglist,0);
  Minuit->ExecuteCommand("HESSE",arglist,0);

  // Print results
  
  TGraphErrors *gr = new TGraphErrors(5,x,z,0,errorz);
  gr->Draw("AP");
  
  TF1 *fres = new TF1("fres",funct,90,150,5);
  double par[] = {1,1,1,1};
  for(int i = 0; i < 4; i++){cout << Minuit->GetParameter(i) << " " << Minuit->GetParError(i) << endl;
    fres->SetParameter(i,Minuit->GetParameter(i));
    par[i] = Minuit->GetParameter(i);
  }

  fres->Draw("same");
  
   TCanvas *c2 = new TCanvas("c2","contours",10,10,600,800);
   //get first contour for parameter 1 versus parameter 2
   TGraph *gr12 = (TGraph*)gMinuit->Contour(40,0,3);

   gMinuit->SetErrorDef(2.25);
   
   TGraph *gr2 = (TGraph*)gMinuit->Contour(40,0,3);
   gr2->SetFillColor(42);
   gr2->Draw("Alf");
   gr12->Draw("C");
  
}

