
bool cond(double x1, double y1, double angle){

  double x = x1*cos(angle) + y1*sin(angle);
  double y = -x1*sin(angle) + y1*cos(angle);
  if(sqrt(pow(x+3,2)+pow(y+4,2)) < 3)return true;
  if(sqrt(pow(x-3,2)+pow(y-0,2)) < 0.07)return true;
  if(sqrt(pow(x-7,2)+pow(y-3,2)) < 0.5)return true;
  if(sqrt(pow(x-3,2)+pow(y+3,2)) < 2)return true;
  if(sqrt(pow(x+3,2)+pow(y-5,2)) < 3)return true;
  if(sqrt(pow(x-5,2)+pow(y-6,2)) < 1)return true;
  if(sqrt(pow(x-0,2)+pow(y-0,2)) < 2)return true; 
  
  return false;
}

void fill(TH2D *hist, double angle){

  hist->Reset();
  TRandom3 m3;
  for(int i = 1; i <= hist->GetNbinsX(); i++){
    for(int j = 1; j <= hist->GetNbinsY(); j++){
      double r1 = -10+20*(double)i/hist->GetNbinsX();
      double r2 = -10+20*(double)j/hist->GetNbinsY();
      if(cond(r1,r2,angle))hist->SetBinContent(i,j,1);
      //hist->SetBinContent(i,j,hist->GetBinContent(i,j)+m3.Rndm());

    }
  }

}

void BP(TH2D *hist, vector<TH1D> *vect, vector<double> angles){

  int n = vect->size();
  cout << "n = " << n << endl;
  for(int i = 0; i < 100; i++){
    for(int j = 0; j < 100; j++){
      int k = 0;
      for (TH1D  el : *vect) {
	double angle = angles[k];
	int num = 50. + (i-50)*cos(angle) - (j-50.)*sin(angle);
	hist->SetBinContent(i,j,hist->GetBinContent(i,j) + el.GetBinContent(num));
	k++;
      }
    }
    
  }
}


   
void FBP(TH2D *hist, vector<TH1D> *vect, vector<double> angles){

    int n = vect->size();
    TH1D *hff_re = new TH1D("hff_re","hff_re",100,0,100);
    TH1D *hff_im = new TH1D("hff_im","hff_im",100,0,100);
    vector<TH1D> fvect;
    int kol = 100;
    double* re = new double[kol];
    double* im = new double[kol];
    TH1 *hb = 0;
    for (TH1D  el : *vect) {
      el.FFT(hff_re,"RE");
      el.FFT(hff_im,"IM");
      for(int i = 0; i < 100; i ++){
	re[i] = hff_re->GetArray()[i+1];
	im[i] = hff_im->GetArray()[i+1];
      }
      for(int i = 0; i < 100; i ++){
	if(i <= 49){
	  re[i] = re[i]*i;//(i-i*i/50.);
	  im[i] = im[i]*i;//(i-i*i/50.);
	}
	if(i > 49){
	  re[i] = 0.;
	  im[i] = 0.;
	}
      }

      TVirtualFFT *fft_back = TVirtualFFT::FFT(1, &kol, "C2R");
      fft_back->SetPointsComplex(re,im);
      fft_back->Transform();
      hb = TH1::TransformHisto(fft_back,hb,"Re");
      for(int i = 1; i <= 100; i++)hff_re->SetBinContent(i,hb->GetBinContent(i)); 
      fvect.push_back(*hff_re);

    }


    for(int i = 0; i < 100; i++){
      for(int j = 0; j < 100; j++){
	int k = 0;
	for (TH1D  el : fvect) {
	  double angle = angles[k];
	  int num = 50. + (i-50)*cos(angle) - (j-50.)*sin(angle);
	  hist->SetBinContent(i,j,hist->GetBinContent(i,j) + el.GetBinContent(num));
	  k++;
	}
      }
      
    }
}


int scr_tomo(){
  TH1D *histx;
  TH2D *hist = new TH2D("hist","hist",100,-10,10,100,-10,10);
  TH2D *hist_rec0 = new TH2D("hist_rec0","BP",100,-10,10,100,-10,10);
  TH2D *hist_rec = new TH2D("hist_rec","FBP",100,-10,10,100,-10,10);
  vector<TH1D > vect;
  vector<double> angles;
  for(double angle = 0; angle < 6.28; angle+=6.28/30){
    angles.push_back(angle);
    hist->Reset();
    fill(hist,angle);
    histx = (TH1D *) hist->ProjectionX();
    vect.push_back(*histx);
  }
  hist->Reset();
  fill(hist,0);
  TCanvas *s = new TCanvas();
  s->Divide(3,1);
  s->cd(1);
  hist->Draw("colz");
  s->cd(2);
  BP(hist_rec0,&vect,angles);
  hist_rec0->Draw("colz");
  s->cd(3);
  FBP(hist_rec,&vect,angles);
  hist_rec->Draw("colz");

  return 0;

}

