#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <stdio.h>
#include <libgen.h>
#include <time.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TH1.h"
#include "TVirtualFFT.h"
#include "TEntryList.h"
#include "TTreeIndex.h"
#include "TObjString.h"


TTree * Tr_tree=0;
TTree * Ev_tree=0;
TTree * RWF_tree=0;
TTree * comb_tr=0;

   // Declaration of leaf types
   UInt_t          event;
   UInt_t          sn;
   UInt_t          ch;
   ULong64_t       utime_ms;
   ULong64_t       tai_ns;
   TH1S            *th1s_ptr;

   Int_t           trackID;
   Int_t           track_eventID;
   Int_t           track_nhits;
   Float_t         track_start_pos_x;
   Float_t         track_start_pos_y;
   Float_t         track_start_pos_z;
   Float_t         track_start_pos_t;
   Float_t         track_end_pos_x;
   Float_t         track_end_pos_y;
   Float_t         track_end_pos_z;
   Float_t         track_end_pos_t;
   Float_t         track_length;
   Float_t         track_q;
   Float_t         track_q_raw;
   Float_t         track_theta;
   Float_t         track_phi;
   Float_t         track_residual_x;
   Float_t         track_residual_y;
   Float_t         track_residual_z;
   Float_t         track_hits_x[10000];   //[track_nhits]
   Float_t         track_hits_y[10000];   //[track_nhits]
   Float_t         track_hits_z[10000];   //[track_nhits]
   Int_t           track_hits_ts[10000];   //[track_nhits]
   Float_t         track_hits_q[10000];   //[track_nhits]

   Int_t           eventID;
   Int_t           event_start_t;
   Int_t           event_end_t;
   Int_t           event_duration;
   Int_t           event_unix_ts;
   Int_t           event_nhits;
   Float_t         event_q;
   Float_t         event_q_raw;
   Int_t           event_ntracks;
   Int_t           event_n_ext_trigs;
   Float_t         event_hits_x[100000];   //[event_nhits]
   Float_t         event_hits_y[100000];   //[event_nhits]
   Float_t         event_hits_z[100000];   //[event_nhits]
   Int_t           event_hits_ts[100000];   //[event_nhits]
   Float_t         event_hits_q[100000];   //[event_nhits]
   Int_t           event_hits_io_group[100000];   //[event_nhits]
   Int_t           trigID[10];   //[event_n_ext_trigs]
   Int_t           trig_type[10];   //[event_n_ext_trigs]
   Int_t           trig_time[10];   //[event_n_ext_trigs]


TFile *ofile=0;
TDirectory *memdir=0;
TEntryList *tr_elist; //list of associated entries in Tracks tree in CDS file
TEntryList *lds_elist; //list of associated entries (by time) in Hits tree in LDS file
Int_t crossing=-1;
Int_t stopped=-1;
Int_t decayed=-1;
Int_t contained=-1;
Int_t HIP=-1;
Int_t Dt_unix_ms;
Int_t Dt_ns;
#define offset_ns_nmax 128*10
Int_t offset_ns[offset_ns_nmax];
Int_t offset_ns_n=0;

TEntryList *elist00;
TEntryList *elist;
TEntryList *tlist;
//TTreeIndex *fIndex;
  Int_t npassed;
  ULong64_t t0_175854781, t0_175780172;


  Long64_t trel;
  Int_t bins; 
  char st[256];
  char st2[256];
  EColor color=kBlack;

 TH1 *hm =0;
 TH1 *hp =0;
int events_processed=0;


void FilterSinus(TH1S * h);
void combevt(int ie=64, int window_us=100);


void combine2(const char * CDSfname, const char * LDSfname, Int_t window_us=100, Int_t StartEntry=0, Int_t NEntries=1000000)
{
memdir=gDirectory;
 
tr_elist=new TEntryList("tr_elist","Tracks list");
lds_elist=new TEntryList("lds_elist","LDS tree entries list");

  TFile *f=new TFile(CDSfname);
  f->GetObject("events",Ev_tree);
  f->GetObject("tracks",Tr_tree);
  if(Ev_tree==NULL) {printf("Failed to open CDS events tree.\n"); return;}
  if(Tr_tree==NULL) {printf("Failed to open CDS tracks tree.\n"); return;}
  printf("CDS File opened.\n");

  (new TFile(LDSfname))->GetObject("rwf",RWF_tree);
  if(RWF_tree==NULL) {printf("Failed to open LDS tree.\n"); return;}
  printf("LDS File opened.\n");
  RWF_tree->SetBranchAddress("event", &event);
  RWF_tree->SetBranchAddress("sn", &sn);
  RWF_tree->SetBranchAddress("ch", &ch);
  RWF_tree->SetBranchAddress("utime_ms", &utime_ms);
  RWF_tree->SetBranchAddress("tai_ns", &tai_ns);
  RWF_tree->SetBranchAddress("th1s_ptr", &th1s_ptr);
  
   Tr_tree->SetBranchAddress("trackID", &trackID);
   Tr_tree->SetBranchAddress("track_eventID", &track_eventID);
   Tr_tree->SetBranchAddress("track_nhits", &track_nhits);
   Tr_tree->SetBranchAddress("track_start_pos_x", &track_start_pos_x);
   Tr_tree->SetBranchAddress("track_start_pos_y", &track_start_pos_y);
   Tr_tree->SetBranchAddress("track_start_pos_z", &track_start_pos_z);
   Tr_tree->SetBranchAddress("track_start_pos_t", &track_start_pos_t);
   Tr_tree->SetBranchAddress("track_end_pos_x", &track_end_pos_x);
   Tr_tree->SetBranchAddress("track_end_pos_y", &track_end_pos_y);
   Tr_tree->SetBranchAddress("track_end_pos_z", &track_end_pos_z);
   Tr_tree->SetBranchAddress("track_end_pos_t", &track_end_pos_t);
   Tr_tree->SetBranchAddress("track_length", &track_length);
   Tr_tree->SetBranchAddress("track_nhits", &track_nhits);
   Tr_tree->SetBranchAddress("track_q", &track_q);
   Tr_tree->SetBranchAddress("track_q_raw", &track_q_raw);
   Tr_tree->SetBranchAddress("track_theta", &track_theta);
   Tr_tree->SetBranchAddress("track_phi", &track_phi);
   Tr_tree->SetBranchAddress("track_residual_x", &track_residual_x);
   Tr_tree->SetBranchAddress("track_residual_y", &track_residual_y);
   Tr_tree->SetBranchAddress("track_residual_z", &track_residual_z);
   Tr_tree->SetBranchAddress("track_hits_x", track_hits_x);
   Tr_tree->SetBranchAddress("track_hits_y", track_hits_y);
   Tr_tree->SetBranchAddress("track_hits_z", track_hits_z);
   Tr_tree->SetBranchAddress("track_hits_ts", track_hits_ts);
   Tr_tree->SetBranchAddress("track_hits_q", track_hits_q);

   Ev_tree->SetBranchAddress("eventID", &eventID);
   Ev_tree->SetBranchAddress("event_start_t", &event_start_t);
   Ev_tree->SetBranchAddress("event_end_t", &event_end_t);
   Ev_tree->SetBranchAddress("event_duration", &event_duration);
   Ev_tree->SetBranchAddress("event_unix_ts", &event_unix_ts);
   Ev_tree->SetBranchAddress("event_nhits", &event_nhits);
   Ev_tree->SetBranchAddress("event_q", &event_q);
   Ev_tree->SetBranchAddress("event_q_raw", &event_q_raw);
   Ev_tree->SetBranchAddress("event_ntracks", &event_ntracks);
   Ev_tree->SetBranchAddress("event_n_ext_trigs", &event_n_ext_trigs);
   Ev_tree->SetBranchAddress("event_hits_x", event_hits_x);
   Ev_tree->SetBranchAddress("event_hits_y", event_hits_y);
   Ev_tree->SetBranchAddress("event_hits_z", event_hits_z);
   Ev_tree->SetBranchAddress("event_hits_ts", event_hits_ts);
   Ev_tree->SetBranchAddress("event_hits_q", event_hits_q);
   Ev_tree->SetBranchAddress("event_hits_io_group", event_hits_io_group);
   Ev_tree->SetBranchAddress("trigID", trigID);
   Ev_tree->SetBranchAddress("trig_type", trig_type);
   Ev_tree->SetBranchAddress("trig_time", trig_time);

Tr_tree->SetAlias("dx","track_end_pos_x-track_start_pos_x");
Tr_tree->SetAlias("dy","track_end_pos_y-track_start_pos_y");
Tr_tree->SetAlias("dz","track_end_pos_z-track_start_pos_z");
Tr_tree->SetAlias("phi","atan2(dz,dx)");
Tr_tree->SetAlias("theta","atan2(sqrt(dx*dx+dz*dz), abs(dy))");
Tr_tree->SetAlias("good","track_residual_z!=0&&track_residual_x!=0&&track_residual_y!=0");

Ev_tree->SetAlias("good","event_hits_x!=0 && event_hits_y!=0 && event_hits_z!=0 ");



int totevs=Ev_tree->GetEntries();
int EndEntry=StartEntry+NEntries;
if (EndEntry > totevs) EndEntry=totevs;


  sprintf(st,"%s",CDSfname);
  memcpy(st2,CDSfname,strlen(basename(st))-9);
  sprintf(st,"comb%s_%d_%d.root",st2+7,StartEntry,EndEntry-1);
  printf("Creating output file %s\n",st);
  ofile = TFile::Open(st,"RECREATE");
  
  TObjString LDS_fn(LDSfname);
  TObjString CDS_fn(CDSfname);
  ofile->WriteObject(&LDS_fn,"LDS");
  ofile->WriteObject(&CDS_fn,"CDS");

  comb_tr=new TTree();
  comb_tr->Branch("eventID",&eventID,"eventID/I");
  comb_tr->Branch("tracks",&tr_elist);
  comb_tr->Branch("lds",&lds_elist);
  comb_tr->Branch("Dt_unix_ms",&Dt_unix_ms,"Dt_unix_ms/I");
  comb_tr->Branch("Dt_ns",&Dt_ns,"Dt_ns/I");
  comb_tr->Branch("offset_ns_n",&offset_ns_n,"offset_ns_n/I");
  comb_tr->Branch("offset_ns",&offset_ns[0],"offset_ns[offset_ns_n]/I");
  

 // comb_tr->Branch("crossing",&crossing,"crossing/I");
///  comb_tr->Branch("stopped",&stopped,"stopped/I");
//  comb_tr->Branch("decayed",&decayed,"decayed/I");
//  comb_tr->Branch("contained",&contained,"contained/I");
//  comb_tr->Branch("HIP",&HIP,"HIP/I");
  //comb_tr->SetBasketSize("*",100000);

 memdir->cd();



//for(int evv=0; evv<totevs; evv++)
for(int evv=StartEntry; evv<EndEntry; evv++)
{
printf("************************************** Processing event # %d of %d *******************************\n",evv, totevs);
combevt(evv, window_us);
events_processed++;
 ofile->cd();
  comb_tr->Fill();
  comb_tr->FlushBaskets();
  comb_tr->Write("comb_tr",TTree::kOverwrite);
 memdir->cd();
}


//combevt(64, window_us);

 ofile->cd();
 comb_tr->Write("comb_tr",TTree::kOverwrite);
 ofile->Write();
 ofile->Close();

}


  void FilterSinus(TH1S * h)
{
 int n=h->GetXaxis()->GetNbins();
 TVirtualFFT::SetTransform(0);
 hm = h->FFT(hm, "MAG");
 hp = h->FFT(hp, "PH");
 TVirtualFFT *fft = TVirtualFFT::GetCurrentTransform();
 Double_t *re_full = new Double_t[n];
 Double_t *im_full = new Double_t[n];
 fft->GetPointsComplex(re_full,im_full);
int i1,i2;
if(n==2048) { i1=198; i2=212;}
if(n==1024) { i1=99; i2=106;}
if(n==256) { i1=24; i2=27;}

//for(int i=99; i<=106; i++) 
for(int i=i1; i<=i2; i++) 
  { 
    im_full[i]=0;
    im_full[n-i]=0;
    re_full[i]=0;
    re_full[n-i]=0;
  }
if(n==1024) 
    { 
    int i=3;
    im_full[i]=0;
    im_full[n-i]=0;
    re_full[i]=0;
    re_full[n-i]=0;
    }
 
TVirtualFFT *fft_back = TVirtualFFT::FFT(1, &n, "C2R M K");
fft_back->SetPointsComplex(re_full,im_full);
fft_back->Transform();
TH1 *hb = 0;
hb = TH1::TransformHisto(fft_back,hb,"Re");
//hb->Scale(1./n);
//hb->Copy(h);
h->Reset();
h->Add(hb, 1./n);
//delete hp;
delete hb;
//delete fft;
//delete fft_back;

}



void combevt(int ie, int window_us)
{  
   
 
  Ev_tree->GetEntry(ie);

  sprintf(st,"track_eventID==%d",eventID);
  Tr_tree->Draw(">>tlist",st,"entrylist");
  tlist = (TEntryList*)gDirectory->Get("tlist");
  printf("%lld tracks reconstructed in this event\n",tlist->GetN());
  tr_elist->Reset(); tr_elist->Add(tlist);
  delete tlist;   
    Float_t offset_usec = event_start_t/10.;
    Int_t offset_100ns = event_start_t;
  ULong64_t utime_sec=event_unix_ts; //t_of_day;
  

  sprintf(st,"abs(utime_ms-%lld)<1000 && abs(tai_ns/1e2-%d)<%d &&  ch==00",utime_sec*1000, offset_100ns,window_us*10);
  RWF_tree->SetEventList(NULL);
  RWF_tree->Draw(">>elist00",st,"entrylist");
  elist00 = (TEntryList*)gDirectory->Get("elist00");

  printf("Elist00 ready.\n");

 // sprintf(st,"abs(utime_ms-%lld)<1000 && abs(tai_ns/1e2-%d)<%d", utime_sec*1000, offset_100ns,window_us);
  sprintf(st,"abs(utime_ms-%lld)<1000 && abs(tai_ns/1e2-%d)<%d && ch>=0 && ch<=63", utime_sec*1000, offset_100ns,window_us);
 // sprintf(st,"abs(utime_ms-%lld)<1000 && abs(tai_ns/1e2-%d)<%d && (ch==31 || ch==15 || ch==63 || ch==47 \
                      ||ch==8 || ch==40 || ch==24 || ch==56 || ch==00)", utime_sec*1000, offset_100ns,window_us);
  RWF_tree->SetEventList(NULL);
  RWF_tree->Draw(">>elist",st,"entrylist");
  elist = (TEntryList*)gDirectory->Get("elist");
  printf("Elist ready.\n");
  lds_elist->Reset(); lds_elist->Add(elist);
  
  RWF_tree->SetEntryList(elist00);
  printf("Detecting relative t0 for both boards...\n");
  t0_175780172=0;
  t0_175854781=0;
  for(int i=0; i<elist00->GetN(); i++) {  RWF_tree->GetEntry(elist00->GetEntry(i)); if(sn==175780172) {t0_175780172=tai_ns; break;}}
  for(int i=0; i<elist00->GetN(); i++) {  RWF_tree->GetEntry(elist00->GetEntry(i)); if(sn==175854781) {t0_175854781=tai_ns; break;}}

  if(t0_175780172==0) t0_175780172=t0_175854781;
  if(t0_175854781==0) t0_175854781=t0_175780172;
  printf("ADC_175854781 t0=%lld \n",t0_175854781);
  printf("ADC_175780172 t0=%lld \n",t0_175780172);
  printf("\nADC_175780172 t0 mismatch w.r.t. given offset: %lld ns\n",t0_175780172-offset_100ns*100);
  
  //Correction based on busy WF
  //find first waveform on both adcs
  int b;
  int b_175780172;
  int b_175854781;
  

  for(int i=0; i<elist00->GetN(); i++) {  RWF_tree->GetEntry(elist00->GetEntry(i)); if(sn==175780172) break;}
  for(b=50; b < 250; b++) if(th1s_ptr->GetBinContent(b)<-10000) break;
  if(b<250)
  {
  printf("Busy front detected for ADC %d at %d ns\n",sn,b*10);
  b_175780172=b*10;
  }
  else 
  {
  printf("Busy front NOT detected for ADC %d, setting correction to zero\n",sn);
  b_175780172=0;
  }

  for(int i=0; i<elist00->GetN(); i++) {  RWF_tree->GetEntry(elist00->GetEntry(i)); if(sn==175854781) break;}
  for(b=50; b < 250; b++) if(th1s_ptr->GetBinContent(b)<-10000) break;
  if(b<250)
  {
  printf("Busy front detected for ADC %d at %d ns\n",sn,b*10);
  b_175854781=b*10;
  }
  else 
  {
  printf("Busy front NOT detected for ADC %d, setting correction to zero\n",sn);
  b_175854781=0;
  }
  
  RWF_tree->SetEntryList(elist);
  npassed = elist->GetN();  // number of events to pass cuts
  printf("%d entries passed the time cut.\n",npassed);
  Dt_unix_ms = utime_ms - utime_sec*1000;

  // Margins for time line
  Int_t MinT=1e9, MaxT=-1e9;
  Int_t bin0;
  Int_t adc_i=0;


  offset_ns_n=0;
  for(int i=0; i<npassed; i++)
  {
    RWF_tree->GetEntry(elist->Next()); 
    trel=0;
 
  if(sn==175854781) trel=Int_t(tai_ns-t0_175854781); 
  else if(sn==175780172) trel=Int_t(tai_ns-t0_175780172); 
  else printf("Unknown SN=%d in the stream!\n",sn); 

     if(sn==175854781) trel=trel-b_175854781;
      if(sn==175780172) trel=trel-b_175780172;
      trel=trel+325; //Busy delay, must be more or less equal for both adcs
    
    bins=th1s_ptr->GetXaxis()->GetNbins();
    
  //  if(ch==8 || ch==40 || ch==24 || ch==56 ) { FilterSinus(th1s_ptr); th1s_ptr->Smooth(4);}
    
    th1s_ptr->GetXaxis()->Set(bins,trel,trel+bins*10);
    th1s_ptr->GetXaxis()->SetTitle("ns");
  //  printf("%d : SN %d ch %d; tai_ns=%lld Rel.time,ns: %lld;  utime_ms %lld\n",i,sn,ch,tai_ns,trel,utime_ms);
    th1s_ptr->GetYaxis()->SetRangeUser(-32800,5000); 
    if(sn==175780172) adc_i=0; else adc_i=1;
 //   bin0=chh[adc_i][ch]->GetXaxis()->FindBin(trel);
    for(int x=0; x<th1s_ptr->GetXaxis()->GetNbins(); x++) 
      { 
  //    chh[adc_i][ch]->SetBinContent(bin0+x,th1s_ptr->GetBinContent(x));
      }
    if(MinT>trel) MinT=trel;
    if(MaxT<trel+bins*10) MaxT=trel+bins*10;
    if(offset_ns_n<offset_ns_nmax)
    {
    offset_ns[offset_ns_n]=trel;
    offset_ns_n++;
    }
  }
  printf("Time margins: %d ns  to %d ns.\n",MinT, MaxT);
    if(elist->GetN()>2) {Dt_ns=t0_175780172-offset_100ns*100; }
    else { Dt_ns=1000000000; Dt_unix_ms=1000000000;} ;
   printf("\nADC_175780172 t0 mismatch w.r.t. given offset: %d ns, %d Unix ms\n",Dt_ns, Dt_unix_ms);

  

  RWF_tree->SetEntryList(0);
  delete elist;
  delete elist00;

}





