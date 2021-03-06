#include <iostream>
#include <iomanip>
#include <string.h>
#include <math.h>
#include <istream>
#include <TLatex.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2F.h>
#include <TArrow.h>
#include "string.h"
#include <sstream>
#include "NCutil.h"
#include <fstream> 

using namespace std;

// TTree *tree_;

/*
NDelete: to delete object
factorial: it returns the factorial number, used by prob
prob: it returns the poissonian probability at LumInt of having nevt events
xi: returns the minimum xi
EHF: returns the minimum sumEHF
NCanvas: returns a canvas 
NLine: draws a line
NArrows: draws an arrow
NText: writes a text
NH1clone: it clones a THF1
NH2clone: it clones a THF2
NHtoV: put the Histo content into a Vector 
IsItaZ : returns the fraction of events of a Z line in a 2 GeV bins
HFCor:  Returns the correction function for HF or Gap =0 
NStatOn(TH1 *histo)
NStatOff(TH1 *histo)
NHSetMarker(TH1 *histo, Color,Style,Size)

 */

/* void NDelete(string name){
   gDirectory->Delete(name);
   return;
   }
 */

// ***********************

int factorial(int n){
  if (n<=1)
    return(1);
  else
    n=n*factorial(n-1); // notice we just keep calling this over until we get to 1!
  return(n);
}



//==*******************==//

Float_t meanEvt( Float_t LumInt){

  Float_t sigma_NSD =  0.070*pow(10,-24);
  Float_t Lum_bunch = (LumInt*pow(10,30)/11346)*sigma_NSD ;

  cout << "Average events = " << Lum_bunch << endl;

  return(Lum_bunch);

}

//==*******************==//

Float_t prob( Float_t LumInt, int nevt){

  Float_t sigma_NSD =  0.060*pow(10,-24);
  //  Float_t sigma_NSD =  0.035*pow(10,-24);
  Float_t Lum_bunch = (LumInt*pow(10,30)/11346)*sigma_NSD ;
  Float_t pois_prob = pow(Lum_bunch,nevt) * exp(-Lum_bunch)/factorial(nevt) ;

  // cout << "Average events = " << Lum_bunch << endl;

  return(pois_prob);

}


//==*******************==//

Float_t prob_ghost( Float_t LumInt, int nevt){

  Float_t sigma_ghost =  0.035*pow(10,-24);
  Float_t Lum_bunch = (LumInt*pow(10,30)/11346)*sigma_ghost ;
  Float_t pois_prob = pow(Lum_bunch,nevt) * exp(-Lum_bunch)/factorial(nevt) ;

  cout << "Average ghost events = " << Lum_bunch << endl;

  return(pois_prob);

}

//==============================

Float_t PtWeight( Float_t Pt){

  Float_t w =2.432245-7.869480*Pt +13.94306*pow(Pt,2)
    -10.01573*pow(Pt,3)+ 2.569346*pow(Pt,4);
  if (Pt>1.5) w = 1.;
  return(w);
}


//==****************************** // 
// selects the minimum xi
// tree_->Draw("xi(xi_PF_plus,xi_PF_minus)")

Float_t xi(Float_t xi_PF_plus, Float_t xi_PF_minus){
  if (xi_PF_plus < xi_PF_minus) {
    return(xi_PF_plus);
  }			  
  return(xi_PF_minus);
} 

//==****************************** // 
// selects the minimum HF
// tree_->Draw("EHF(sumEHF_plus,sumEHF_minus)")
Float_t EHF(Float_t sumEHF_plus, Float_t sumEHF_minus){
  if (sumEHF_plus < sumEHF_minus) {
    return(sumEHF_plus);
  }			  
  return(sumEHF_minus);
}  

///==******************==/

TCanvas * NCanvas( int arg1, int arg2, string text="Test") 
{

  //  TCanvas *MyC = new TCanvas("MyC","Test canvas",1);
  TCanvas *MyC = new TCanvas(text.c_str(),text.c_str(),1);
  MyC->Divide(arg1,arg2,0.002,0.002);
  return(MyC);
}

///==******************==/

void NLine(Float_t x1, Float_t y1, Float_t x2, Float_t y2,Int_t Style=1 ) {
  TLine *NLine1 = new TLine(x1,y1,x2,y2);
  NLine1->SetLineStyle(Style);
  NLine1->Draw();
  return;
}

///==******************==/

void NArrow(Float_t x1, Float_t y1, Float_t x2, Float_t y2, Float_t size, Option_t *option ) {
  TArrow *NArrow = new TArrow(x1,y1,x2,y2,size,option);
  NArrow->Draw();
  return;
}

// ********************==/

void NText(Float_t x1, Float_t y1, string text, Int_t col=1, Float_t Tsize=0.04){
  TLatex l;
  l.SetTextSize(Tsize);
  l.SetTextColor(col);
  l.DrawLatex(x1,y1,text.c_str())   ;
  return;
}

// *********************
/*void NMarker(Int_t style, Int_t size, Int_t color){
  tree_->SetMarkerStyle(style);
  tree_->SetMarkerSize(size);
  tree_->SetMarkerColor(color);
  return;
  }
 */
// ********************************



// Set the x range using the axis coordinates //
void NSetXRange(TH1 *histo, Float_t xLow, Float_t xHigh)
{
  Int_t low =  histo->GetXaxis()->FindBin(xLow);
  Int_t high =  histo->GetXaxis()->FindBin(xHigh);
  histo->GetXaxis()->SetRange(low,high);

}

void NSetTitleSize(TH1 *histo,Float_t Xoffset = 0.7,Float_t Yoffset = 1. ,Float_t Size = 0.08)
{
  histo->GetXaxis()->SetTitleSize(Size);
  histo->GetXaxis()->SetTitleOffset(Xoffset);
  histo->GetYaxis()->SetTitleSize(Size);
  histo->GetYaxis()->SetTitleOffset(Yoffset);
}



// ********************************

void NStatOn(TH1 *histo){
  histo->SetStats(kTRUE);
  gStyle->SetOptStat(1111111);
}


// ********************************
void NStatOff(TH1 *histo){
  histo->SetStats(0);
}


// ********************************

void NHSetMarker(TH1 *histo, Color_t Color=2, Style_t Style=2, Size_t Size = 0.2){
  histo->SetMarkerColor(Color);
  histo->SetMarkerStyle(Style);
  histo->SetLineColor(Color);
  histo->SetMarkerSize(Size);
  return;
}

void NLogAxis(Int_t x, Int_t y){
  gPad->SetLogx(x);
  gPad->SetLogy(y);
  return;
}


//==******************************==/
// NH1clone("toy", "k", EEP_all) 
// The pointer is called "name_new")
// Usage:
// TH1 *pluto;
// pluto = NH1clone("name","title",histo_old);

//TH1* NH1clone (TH1* histo_new, TString name_new, TString title_new, TH1* histo_old ){
TH1* NH1clone (TString name_new, TString title_new, TH1* histo_old ){

  //  gDirectory->Delete(name_new);
  TH1* histo_new =  (TH1*) histo_old->Clone(); 
  histo_new->SetTitle(title_new); 
  histo_new->SetName(name_new); 
  histo_new->Reset();
  return histo_new;
}

TH1* NH1Dclone (TString name_new, TString title_new, TH1* histo_old )
{

  //  gDirectory->Delete(name_new);
  TH1* histo_new =  (TH1*) histo_old->Clone(); 
  histo_new->SetTitle(title_new); 
  histo_new->SetName(name_new); 
  histo_new->Reset();
  return histo_new;
}


TFile *NOpen(TString name_new)
{

  TFile *abc = TFile::Open(name_new);
  return abc;
}


//==***************==/

// Hystogram to Vector
// Float_t *Pcpythia = new Float_t [nbin];
// Float_t *Pepythia = new Float_t [nbin];
// NHtoV(Hpythia, Pcpythia, Pepythia);

void  NHtoV(TH1 * file_in, Float_t* content, Float_t* econtent)

{

  Int_t nbin = file_in->GetNbinsX();
  for (int i=0;i< nbin;i++){
    *(content+i) = file_in->GetBinContent(i);
    *(econtent+i) = file_in->GetBinError(i);
    //  cout << content[i] << " " << i << endl;
    // cout << econtent[i] << " " << i << endl;
  };
  return;
}


// This function returns the fraction of Z->mm events in that 2 GeV bin

//==*********************************==/

Float_t IsItaZ(Float_t mass){

  Float_t mass_up,mass_low;
  Int_t i = 0.;

  Float_t zmmprob[30] = {0,0.00151172,0.00377929,0.000755858,0.00529101,0.00529101,0.00604686,0.00604686,0.0120937,0.00831444,0.0120937,0.0226757,0.0279667,0.0468632,0.0733182,0.179138,0.266062,0.187453,0.0702948,0.0279667,0.010582,0.00755858,0.00151172,0.00377929,0.00453515,0.00151172,0.000755858,0.00226757,0.000755858,0.00151172};

  cout << zmmprob[3] << endl;
  for (i=0; i<30; i++)
  {
    mass_low = 60+i*2;
    mass_up = 62+i*2;    
    if (mass>=mass_low && mass<mass_up){
      cout << "The number  of Z->mm events in the interval "<< mass_low << " << Mx << " << mass_up << " is = " << 100*zmmprob[i]<< " %" <<endl;
      return(zmmprob[i]);
    }
  }		      
  cout << "No fraction found for your mass value" << endl;
  return 0.;
}

///==***************************** // 
// Returns the correction function for HF at any energy


Float_t HFCor(Float_t istlumiPerBX, Float_t EHF)
{

  Float_t x = istlumiPerBX*6.37;  
  Float_t xl; 
  Float_t xu;
  const int xbin = 15;
  Float_t xlow[xbin] = {0., 4.,10.,20.,40.,60.,90.,120.,
    150.,250.,400.,600.,800.,1000.,15000. };
  Float_t slope[xbin-1] = {3.08909,1.48380, 1.08810, 0.751990,
    0.587490,0.456329,0.318799,0.208317,
    0.0850781,-0.113282,-0.349362,-0.729234,
    -1.53780,-2.74242};


  if(x > 0.05) {
    for (int i=0; i<15; i++) {

      xl = xlow[i];
      xu = xlow[i+1];
      if ( EHF >=xl && EHF < xu) return(pow(2.71, slope[i]*x));
    }
  }

  return(1.);

}

///==***************************==// 
// Returns the correction function for Castor+ HF= 0 


Float_t CastorCor(Float_t istlumiPerBX){

  Float_t par[3]={0.001922, -0.01115, 0.01678};
  Float_t x = istlumiPerBX*6.37;
  if(x > 0.05) return(1.*par[0]/(1.*par[0]+1.*par[1]*x+1.*par[2]*x*x));
  return(0.);
} 



// scale file2 so that it has the same max of file1
void N2HSameMax(TH1 *file1, TH1 *file2){

  Int_t Max1 = file1->GetMaximumBin();
  Float_t Max1Cont = file1->GetBinContent(Max1); 
  Int_t Max2 = file2->GetMaximumBin();
  Float_t Max2Cont = file2->GetBinContent(Max2); 

  Float_t scale = Max1Cont/Max2Cont;
  file2->Scale(scale);
  return;
}

// scale file2 so that it has the same area of file1
void N2H1DSameArea(TH1 *file1, TH1 *file2){

  Float_t Max1 = file1->Integral(); 
  Float_t Max2 = file2->Integral(); 

  cout << "file 1 entries " << Max1 << " file 2 entries " << Max2 << endl;
  Float_t scale = Max1/Max2;
  file2->Scale(scale);
  return;
}

//Scale Entries
void NEntriesNorma(TH1 *h_1, TH1 *h_2, TH1 *h_3){

  cout << "Normalization by Entries" << endl;
  double weight1, weight2, weight3;
  double ratio1, ratio2;
  
  weight1 = 1./h_1->GetEntries();
  weight2 = 1./h_2->GetEntries();
  weight3 = 1./h_3->GetEntries();

  ratio1 = weight2/weight1;
  ratio2 = weight3/weight1;

  h_2->Scale(ratio1);
  h_3->Scale(ratio2);
  return;
  
}

//Scale Entries
void NEntriesNorma2D(TH1 *h_1, TH1 *h_2){

  cout << "Normalization by Entries" << endl;
  double weight1, weight2;
  double ratio1;

  weight1 = 1./h_1->GetEntries();
  weight2 = 1./h_2->GetEntries();

  ratio1 = weight2/weight1;

  h_2->Scale(ratio1);
  return;

}

// scale file1 to have Max=1
void NH1FMax1(TH1 *file1){

  Int_t Max1 = file1->GetMaximumBin();
  Float_t Max1Cont = file1->GetBinContent(Max1); 
  if (Max1Cont == 0) 
  {
    cout << "NH1FMax1: Empty Histo!!" << endl;
    return;
  }
  Float_t scale = 1./Max1Cont;
  file1->Scale(scale);
  return;
}

void NH1DMax1(TH1 *file1){

  Int_t Max1 = file1->GetMaximumBin();
  Float_t Max1Cont = file1->GetBinContent(Max1); 
  Float_t scale = 1./Max1Cont;
  file1->Scale(scale);
  cout << "MAX1: " << Max1 << endl;
  cout << "Scale, MAX1: " << scale << endl;
  return;
}

void NH1DArea1(TH1 *file1){

  Int_t Max1 = file1->Integral();
  file1->Scale(1./Max1);
  return;
}

void NH2FArea1(TH2F *file1){

  Int_t Max1 = file1->Integral();
  if (Max1 == 0) 
  {
    cout << "NH2FArea1: Empty Histo!!" << endl;
    return;
  }

  file1->Scale(1./Max1);
  return;
}

void NH1FArea1(TH1 *file1){

  Int_t Max1 = file1->Integral();
  if(Max1>0.)  file1->Scale(1./Max1);
  return;
}


Float_t deltaPhi(double phi1, double phi2) { 
  Float_t result = phi1 - phi2;
  while (result > M_PI) result -= 2*M_PI;
  while (result <= -M_PI) result += 2*M_PI;
  return result;
}

void StyleNice(){

  //gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  gStyle->SetPadLeftMargin(0.2);
  gStyle->SetPadBottomMargin(0.2);
  gStyle->SetLegendBorderSize(0);

  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.);

  gStyle->SetTitleSize(0.0845,"X");
  gStyle->SetTitleSize(0.0845,"Y");

  gStyle->SetLabelSize(0.06,"X");
  gStyle->SetLabelSize(0.06,"Y");

  gStyle->SetLabelOffset(0.01,"Y");
  gStyle->SetLabelOffset(0.01,"X");

  gStyle->SetTitleOffset(0.8,"Y");
  gStyle->SetTitleOffset(1.06,"X");

  gStyle->SetLineWidth(3);
  gStyle->SetHistLineWidth(2);

  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetTickLength(0.01,"Y");
  gStyle->SetTickLength(0.02,"X");

  gROOT->ForceStyle();

}

