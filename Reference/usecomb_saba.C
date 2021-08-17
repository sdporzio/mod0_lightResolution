
#define LDS_PATH "/data/LRS/Converted/"
//#define CRS_PATH "/data/LArPix/SingleModule_March2021/TPC12/dataRuns/evdData/good_anticorrelation/outrootfile/"
//#define CRS_PATH  "/data/Combined/test_data/"
//#define CRS_PATH  "/data/Combined/out_light_charge_tree/"
#define CRS_PATH "/data/LArPix/SingleModule_March2021/TPC12/dataRuns/evdData/good_anticorrelation/outrootfile/with_integrated_light/"

#define offset_ns_nmax 128*10

#include <TSystem.h>

  TFile *comf;
  TFile *cdsf;
  TFile *ldsf;
  TObjString *lds_os;
  TObjString *cds_os;
  TTree *comb_tr;
  TTree *cds_Evtr;
  TTree *cds_Trtr;
  TTree *lds_rwftr;

  //TFile *my_file;
  //TTree *my_tr;
  //TEventList *my_list;

  char cut_string[256];

  TH1S * chh;
 
  
      // Declaration of leaf types
   UInt_t          event =0;
   UInt_t          sn =0;
   UInt_t          ch =0;
   ULong64_t       utime_ms =0;
   ULong64_t       tai_ns =0;
   TH1S            *th1s_ptr;
   Int_t offset_ns[offset_ns_nmax] ={0};
   Int_t offset_ns_n=0;
   Int_t wf =0;

  //Decleration of charge data leaf types
  Int_t track_ID =0, track_nhits=0, event_ID=0, event_ntracks=0, event_nhits=0, event_unix_ts=0, event_start_t=0, 
	track_eventID =0, track_no_in_event=0;
  Float_t theta=0.0, phi=0.0, dl=0.0, track_q=0.0, event_q=0.0, dx=0.0, dy=0.0, dz=0.0;
  //tracks hit properties
  Float_t tr_h_x[100000], tr_h_y[100000],tr_h_z[100000], tr_h_q[100000], tr_h_dr[100000], tr_h_Dt[100000];
  Int_t tr_h_ts[100000];

  //events hit properties
  Float_t ev_h_x[100000] ={0.0}, ev_h_y[100000]={0.0},ev_h_z[100000]={0.0}, ev_h_q[100000]={0.0};
  Int_t ev_h_ts[100000] ={0};

  Int_t dev=0;
  EColor color=kBlack;

TCanvas *c;
TCanvas *ct;
TCanvas *cc;

TEntryList *tr_elist; //list of associated entries in Tracks tree in CDS file
TEventList *rwf_elist; //list of associated entries (by time) in Hits tree in LDS file

Int_t eventID;
Int_t Dt_unix_ms;
Int_t Dt_ns;

//#define offset_ns_nmax 128*10
void new_select(int ev);
void check_match();
void display_event(int ev);
void select_event(int ev);
int GetNTracks();
void select_track(int tr);
int GetNWaveforms();
TH1S* GetWaveform(int wf);
//TH1S* GetNextWaveform();

Float_t gain[2][64] ={{-1, -1, 90.89, 88.69, 85.29, 84.73, 85.91, 90.44, -1, 84.81, 82.3, 84.83, 85.17, 81.63, 87, -1, -1, -1, 89.48, 86.17, 93.47, 91.87, 90.85, 91.46, -1, 88.86, 88.83, 79.66, 80.65, 81.18,
      82.17, -1, -1, -1, -1, 93.64, 83.63, 87.79, 89.38, 87.43, -1, 105.87, 106.14, 108.32, 105.77, 106.7, 105.97, -1, -1, -1, 94.95, 95.59, 94.48, 98.86, 96.53, 93.64, -1, 94.46, 93.88, 93.84, 92.8, 92.23, 91.71, -1},
     {-1, -1, 86.69, 85.63, 86.94, 88.26, 86.9, -1, -1, 100.1, 95.42, 90.68, 93.56, -1, 96.72, -1, -1, -1, 70.28, 65.83, 77.34, 80.13, 65.12, 67.16, -1, 99.31, 97.36, 97.09, 94.76, 95.02, 97.22,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 96.95, 95.75, 95.71, 93.87, 99.96, 93.96, -1, -1, -1, 73.15, 75.78, 80.61, 77.1, 78.37, 80.76, -1, 78.54, 77.09, 92.8, 97.58, 95.91, 80.35, -1}};


  char st[256];
  const char *sptr;


void RestoreBL(TH1S * h)
{
 Double_t bl;
 Int_t nbl=80;
 bl=0;
 for(int i=0;i<nbl;i++) bl=bl+h->GetBinContent(i+1);
 bl=bl/nbl;
 for(int i=0;i<h->GetXaxis()->GetNbins();i++) 
   { 
     if(h->GetBinContent(i+1)-bl<-32767) h->SetBinContent(i+1,-32767);
     else if(h->GetBinContent(i+1)-bl>32767) h->SetBinContent(i+1,32767);
     else h->SetBinContent(i+1, h->GetBinContent(i+1)-bl);
   }   
}

void FilterSinus(TH1S * h)
{
 TH1 *hm =0;
 TH1 *hp =0;
 int n=h->GetXaxis()->GetNbins();
 //cout<<" The n bins in the filter sinus is:" <<n<<endl; // There are 256 bins
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
if(n==512) { i1 =48; i2 =52;}
if(n==256) { i1=22; i2=28;}
for(int i=i1; i<=i2; i++) 
  { 
    im_full[i]=0;
    im_full[n-i]=0;
    re_full[i]=0;
    re_full[n-i]=0;
  }
for(int i=48; i<=52; i++) 
  {
    im_full[i]=0;
    im_full[n-i]=0;
    re_full[i]=0;
    re_full[n-i]=0;
  }
for(int i=74; i<=78; i++) 
  {
    im_full[i]=0;
    im_full[n-i]=0;
    re_full[i]=0;
    re_full[n-i]=0;
  }

for(int i=98; i<=106; i++) 
  {
    im_full[i]=0;
    im_full[n-i]=0;
    re_full[i]=0;
    re_full[n-i]=0;
  }
for(int i=122; i<=125; i++) 
  {
    im_full[i]=0;
    //im_full[n-i]=0;
    re_full[i]=0;
    //re_full[n-i]=0;
  }


//if(n==256)
if(n==1024) 
//for(int i = 1; i<4; i++) 
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

//cc->cd();
//TCanvas * ctest = new TCanvas("ct","ct",500,300);
//hm->Draw("");
//hp->Draw("same");
//hb->Draw("same");
h->Reset();
h->Add(hb, 1./n);
delete hb;
delete hm;
delete hp;
}
 

char fname1[256];
char fname2[256];

void usecomb_saba(const char * fname)
{
  comf= new TFile(fname);
  comf->GetObject("LDS",lds_os);
  comf->GetObject("CDS",cds_os); 

  //my_file = TFile::Open((std::string(LDS_PATH)+ std::string( basename(lds_os->GetName()) ) ).c_str() );
  //my_tr = (TTree*)my_file->Get("rwf");

  printf("File links %s and %s\n",(std::string(LDS_PATH)+ std::string( basename(lds_os->GetString().Data()) )).c_str() , (std::string(CRS_PATH) + std::string(basename(cds_os->GetString().Data()))).c_str() );
  comf->GetObject("comb_tr",comb_tr);
  comb_tr->SetBranchAddress("eventID",&eventID);
  comb_tr->SetBranchAddress("tracks",&tr_elist);
  //comb_tr->SetBranchAddress("lds",&rwf_elist);
  comb_tr->SetBranchAddress("Dt_unix_ms",&Dt_unix_ms);
  comb_tr->SetBranchAddress("Dt_ns",&Dt_ns);
  comb_tr->SetBranchAddress("offset_ns_n",&offset_ns_n);
  comb_tr->SetBranchAddress("offset_ns",&offset_ns[0]);

  //cdsf=new TFile(cds_os->GetString().Data());
  cdsf= new TFile((std::string(CRS_PATH) + std::string(basename(cds_os->GetString().Data()))).c_str() , "UPDATE" );
  cdsf->GetObject("events",cds_Evtr);  //Get event tree
  cdsf->GetObject("tracks",cds_Trtr);

   cds_Trtr->SetBranchAddress("track_nhits",&track_nhits);
   cds_Trtr->SetBranchAddress("trackID", &track_ID);
   cds_Trtr->SetBranchAddress("track_eventID",&track_eventID);
   cds_Trtr->SetBranchAddress("track_no_in_event",&track_no_in_event);

   cds_Trtr->SetBranchAddress("track_theta", &theta);
   cds_Trtr->SetBranchAddress("track_phi", &phi);
   cds_Trtr->SetBranchAddress("track_length", &dl);
   cds_Trtr->SetBranchAddress("track_q", &track_q);
   cds_Trtr->SetBranchAddress("track_dx",&dx);
   cds_Trtr->SetBranchAddress("track_dy",&dy);
   cds_Trtr->SetBranchAddress("track_dz",&dz);

   cds_Trtr->SetBranchAddress("track_hits_x", tr_h_x);
   cds_Trtr->SetBranchAddress("track_hits_y", tr_h_y);
   cds_Trtr->SetBranchAddress("track_hits_z", tr_h_z);
   cds_Trtr->SetBranchAddress("track_hits_Dt", tr_h_Dt);
   cds_Trtr->SetBranchAddress("track_hits_q", tr_h_q);
   cds_Trtr->SetBranchAddress("track_hits_dr", tr_h_dr);
   cds_Trtr->SetBranchAddress("track_hits_ts", tr_h_ts);

   cds_Trtr->BuildIndex("track_eventID","track_no_in_event");

   cds_Evtr->SetBranchAddress("eventID",&event_ID);
   cds_Evtr->SetBranchAddress("event_unix_ts",&event_unix_ts);
   cds_Evtr->SetBranchAddress("event_start_t",&event_start_t);
   cds_Evtr->SetBranchAddress("event_ntracks",&event_ntracks);
   cds_Evtr->SetBranchAddress("event_nhits",&event_nhits);
   cds_Evtr->SetBranchAddress("event_q",&event_q);
   cds_Evtr->SetBranchAddress("event_hits_x", ev_h_x);
   cds_Evtr->SetBranchAddress("event_hits_y", ev_h_y);
   cds_Evtr->SetBranchAddress("event_hits_z", ev_h_z);
   cds_Evtr->SetBranchAddress("event_hits_q", ev_h_q);
   cds_Evtr->SetBranchAddress("event_hits_ts", ev_h_ts);

   //ldsf=new TFile(lds_os->GetString().Data());
   ldsf= new TFile((std::string(LDS_PATH)+ std::string( basename(lds_os->GetString().Data()) )).c_str());
   ldsf->GetObject("rwf",lds_rwftr);  //Get light tree
   //lds_rwftr->Scan();

   lds_rwftr->SetBranchAddress("event", &event);
   lds_rwftr->SetBranchAddress("sn", &sn);
   lds_rwftr->SetBranchAddress("ch", &ch);
   lds_rwftr->SetBranchAddress("utime_ms", &utime_ms);
   lds_rwftr->SetBranchAddress("tai_ns", &tai_ns);
   lds_rwftr->SetBranchAddress("th1s_ptr", &th1s_ptr);

  if(chh==0) 
  {
   chh=new TH1S("chh","chh",31000,-1000,30000);
   chh->GetYaxis()->SetRangeUser(-32800,5000);
   chh->GetXaxis()->SetTitle("ns");
   chh->GetYaxis()->SetTitle("ADC");
   chh->SetStats(0);
  } 

//select_event(0);
//new_select(0);
//select_track(0);
//lds_rwftr->GetEntry(0);
}

Int_t bins;
Int_t trel;

int GetNWaveforms()
{
 return rwf_elist->GetN();
 //return my_list->GetN();
}

TH1S* GetWaveform(int wf)
{
  lds_rwftr->GetEntry(rwf_elist->GetEntry(wf));
  //lds_rwftr->GetEntry(my_list->GetEntry(wf));
  //if(ch==8 || ch==40 || ch==24 || ch==56 ) { FilterSinus(th1s_ptr); th1s_ptr->Smooth(4);} 
  if(ch<9 || (ch>17 && ch<25) || (ch>33 && ch<41) || (ch>49 && ch<57) ) { FilterSinus(th1s_ptr); th1s_ptr->Smooth(4);} 
  RestoreBL(th1s_ptr);
  trel=offset_ns[wf];
  bins=th1s_ptr->GetXaxis()->GetNbins();
  th1s_ptr->SetLineColor(color);
  th1s_ptr->GetXaxis()->Set(bins,trel,trel+bins*10);
  th1s_ptr->SetMinimum( min(-5000.0, th1s_ptr->GetMinimum() ));
  th1s_ptr->SetMaximum( max(1000.0, th1s_ptr->GetMaximum() ));
  //th1s_ptr->Draw("same");
  return th1s_ptr;
}
/*
TH1S* GetNextWaveform()
{
  lds_rwftr->GetEntry(rwf_elist->Next());
  //lds_rwftr->GetEntry(my_list->Next());
  if(ch<9 || (ch>17 && ch<25) || (ch>33 && ch<41) || (ch>49 && ch<57) ) { FilterSinus(th1s_ptr); th1s_ptr->Smooth(4);} 
  //if(ch==8 || ch==40 || ch==24 || ch==56 ) { FilterSinus(th1s_ptr); th1s_ptr->Smooth(4);} 
  RestoreBL(th1s_ptr);
  trel=offset_ns[wf];
  bins=th1s_ptr->GetXaxis()->GetNbins();
  th1s_ptr->SetLineColor(color);
  th1s_ptr->GetXaxis()->Set(bins,trel,trel+bins*10);
  th1s_ptr->SetMinimum( min(-5000.0, th1s_ptr->GetMinimum() ));
  th1s_ptr->SetMaximum( max(1000.0, th1s_ptr->GetMaximum() ));
  //th1s_ptr->Draw();
  return th1s_ptr;
}
*/
int GetNTracks()
{
 return tr_elist->GetN();
}


void select_track(int tr)
{
 cds_Trtr->GetEntry(tr_elist->GetEntry(tr));
}

void select_event(int ev)
{
  comb_tr->GetEntry(ev);
  cds_Evtr->GetEntry(ev);
  cds_Trtr->SetEntryList(tr_elist);
  lds_rwftr->SetEventList(rwf_elist);
}


void new_select(int ev){
  comb_tr->GetEntry(ev);
  cdsf->cd();
  cds_Evtr->GetEntry(ev);
  cds_Trtr->SetEntryList(tr_elist);
 
  // I have to open the file again. otherwise the elist is not filled correctly !! I dont know why. 
   //my_file = TFile::Open((std::string(LDS_PATH)+ std::string( basename(lds_os->GetName()) ) ).c_str() );  
   //my_file->cd();
   //my_tr = (TTree*)my_file->Get("rwf");
   //my_tr->Scan();

   sprintf(cut_string, "abs(utime_ms-%lld)<1000 && abs(tai_ns/1e3-%d)<%d ",(long long)event_unix_ts*1000, event_start_t /10 , 50 );
   ldsf->cd();
   lds_rwftr->SetEventList(0);
   lds_rwftr->Draw(">>elist_light", cut_string,"*");

   //my_tr->Draw(">>elist_new", cut_string,"*");
   //my_list = (TEventList*)gDirectory->Get("elist_new");
   //my_list->Print();
   //my_tr->SetEventList(my_list);

   rwf_elist =(TEventList*)gDirectory->Get("elist_light");
   lds_rwftr->SetEventList(rwf_elist);
}


  
void display_event(int ev)
{

   //select_event(ev);
   new_select(ev);
   int map_ch_to_bin[32] ={-1,-1,0,1,2,3,4,5,-1,6,7,8,9,10,11,-1,-1,-1,12,13,14,15,16,17,-1,18,19,20,21,22,23,-1};

   Float_t gain_1[2][64] ={{-1, -1, 90.89, 88.69, 85.29, 84.73, 85.91, 90.44, -1, 84.81, 82.3, 84.83, 85.17, 81.63, 87, -1, -1, -1, 89.48, 86.17, 93.47, 91.87, 90.85, 91.46, -1, 88.86, 88.83, 79.66, 80.65, 81.18,
      82.17, -1, -1, -1, -1, 93.64, 83.63, 87.79, 89.38, 87.43, -1, 105.87, 106.14, 108.32, 105.77, 106.7, 105.97, -1, -1, -1, 94.95, 95.59, 94.48, 98.86, 96.53, 93.64, -1, 94.46, 93.88, 93.84, 92.8, 92.23, 91.71, -1},
     {-1, -1, 86.69, 85.63, 86.94, 88.26, 86.9, -1, -1, 100.1, 95.42, 90.68, 93.56, -1, 96.72, -1, -1, -1, 70.28, 65.83, 77.34, 80.13, 65.12, 67.16, -1, 99.31, 97.36, 97.09, 94.76, 95.02, 97.22,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 96.95, 95.75, 95.71, 93.87, 99.96, 93.96, -1, -1, -1, 73.15, 75.78, 80.61, 77.1, 78.37, 80.76, -1, 78.54, 77.09, 92.8, 97.58, 95.91, 80.35, -1}};

   int zbin;
   TH2D *xy, *zy;
   TH2D *l_display_1 = new TH2D("light-320","light readout x=-320",2,-320,320,24,-640,640);
   TH2D *l_display_2 = new TH2D("light+320","light readout x=320",2,-320,320,24,-640,640);
   TH1S *temp, *h[10], *g[10];
   
   //c->Clear();
   TCanvas *c = new TCanvas("c","display",1600,800);
   c->Divide(4,1);
   c->cd(2);
   cds_Evtr->Draw("event_hits_y:event_hits_x:event_hits_q>>xy_view(50,0,1000,320,-320,320,640,-640,640)",Form("eventID == %d",ev),"colz");
   xy = (TH2D*)gDirectory->Get("xy_view");
   xy->GetXaxis()->SetTitle("x [mm]");
   xy->GetYaxis()->SetTitle("y [mm]");
   xy->SetTitle(Form("event %d xy view",ev));
   xy->SetStats(0);

   c->cd(1);
   cds_Evtr->Draw("event_hits_y:event_hits_z:event_hits_q>>zy_view(50,0,1000,320,-320,320,640,-640,640)",Form("eventID == %d",ev),"colz");
   zy =(TH2D*)gDirectory->Get("zy_view");
   zy->GetXaxis()->SetTitle("z [mm]");
   zy->GetYaxis()->SetTitle("y [mm]");
   zy->SetTitle(Form("Event %d zy view",ev));
   zy->SetStats(0);

   c->cd(3);
   cout<<" N waveforms: "<< GetNWaveforms()<<endl;
   //if(GetNWaveforms() == 0) continue;

   try{
   	if(GetNWaveforms() ==0) throw 1;
	if(GetNWaveforms() ==58) throw 2;
   	for(int i =0; i < GetNWaveforms(); i++){
	     temp = GetWaveform(i);

	     if(ch<32 && sn ==175780172 && map_ch_to_bin[ch] != -1 && gain[1][ch] !=-1) //sn 0
        	 l_display_1->Fill((double)-150.0 ,(double)-610 + (640+640)/24 *map_ch_to_bin[ch],-1*(temp->Integral(temp->FindBin(0),temp->FindBin(500))/(gain_1[1][ch] * 64.0)));

   	     if(ch<32 && sn == 175854781 && map_ch_to_bin[ch] != -1 && gain[0][ch] !=-1) //sn 1
        	 l_display_1->Fill((double)+150.0,(double)-610 + (640+640)/24 * map_ch_to_bin[ch],-1*(temp->Integral(temp->FindBin(0),temp->FindBin(500))/(64.0 *gain_1[0][ch])));

        	//--------

	      if(ch>=32 && sn == 175854781 && map_ch_to_bin[ch-32] != -1 && gain[0][ch] !=-1) //sn 1
        	 l_display_2->Fill((double)-150.0 ,(double)-610 + (640+640)/24 *map_ch_to_bin[ch-32],-1*(temp->Integral(temp->FindBin(0),temp->FindBin(500))/(64.0 * gain_1[0][ch])));

	      if(ch>=32 && sn ==175780172 && map_ch_to_bin[ch-32] != -1 && gain[1][ch] !=-1) //sn 0
        	 l_display_2->Fill((double)150.0,(double)-610 + (640+640)/24 *map_ch_to_bin[ch-32],-1*(temp->Integral(temp->FindBin(0),temp->FindBin(500))/(64.0 *gain_1[1][ch])));

   	 }


	double max_light = max(l_display_1->GetMaximum() , l_display_2->GetMaximum()); 
	max_light = min(4000.00 , max_light);
   	l_display_1->SetMaximum(max_light);
	l_display_1->SetMinimum(0);
        l_display_1->GetXaxis()->SetTitle("z [mm]");
        l_display_1->GetYaxis()->SetTitle("y [mm]");
	l_display_1->SetStats(0);
   	l_display_1->Draw("colz");

   	c->cd(4);
   	l_display_2->SetMaximum(max_light);
        l_display_2->SetMinimum(0);
        l_display_2->GetXaxis()->SetTitle("z [mm]");
        l_display_2->GetYaxis()->SetTitle("y [mm]");
        l_display_2->SetStats(0);
   	l_display_2->Draw("colz");

   	TCanvas *c2 = new TCanvas("c2","display_waveforms, tpc1_blue, tpc2_red",500,800);

	for(int i =0; i < GetNWaveforms(); i++){
     		temp = GetWaveform(i);
      		if(ch == 8  && sn == 175780172){ h[7] = (TH1S*)temp->Clone();} 
      		if(ch == 15 && sn == 175780172){ h[5] = (TH1S*)temp->Clone();}
      		if(ch == 24 && sn == 175780172){ h[3] = (TH1S*)temp->Clone();}
      		if(ch == 31 && sn == 175780172){ h[1] = (TH1S*)temp->Clone();}
      
      		if(ch == 40 && sn == 175854781){ h[8]= (TH1S*)temp->Clone();}
      		if(ch == 47 && sn == 175854781){ h[6]= (TH1S*)temp->Clone();}
      		if(ch == 56 && sn == 175854781){ h[4]= (TH1S*)temp->Clone();}
      		if(ch == 63 && sn == 175854781){ h[2] =(TH1S*)temp->Clone();} 
      
      		if(ch == 8  && sn == 175854781){ g[7] = (TH1S*)temp->Clone();} 
      		if(ch == 15 && sn == 175854781){ g[5] = (TH1S*)temp->Clone();} 
      		if(ch == 24 && sn == 175854781){ g[3] = (TH1S*)temp->Clone();} 
      		if(ch == 31 && sn == 175854781){ g[1] = (TH1S*)temp->Clone();} 

      		if(ch == 40 && sn == 175780172){ g[8] = (TH1S*)temp->Clone();} 
      		if(ch == 47 && sn == 175780172){ g[6] = (TH1S*)temp->Clone();} 
      		if(ch == 56 && sn == 175780172){ g[4] = (TH1S*)temp->Clone();} 
      		if(ch == 63 && sn == 175780172){ g[2] = (TH1S*)temp->Clone();} 
   	}
   	c2->Clear();
   	c2->Divide(2,4);
   	for(int j = 1; j<9; j++){
      		c2->cd(j);
      		if(h[j] && h[j] != NULL){
	      	h[j]->SetLineColor(kBlue);
      		h[j]->GetYaxis()->SetRangeUser(-30000,5000);
      		h[j]->Draw("histsame");
 		}
		if (g[j] && g[j] != NULL ){
      		g[j]->SetLineColor(kRed);
      		g[j]->GetYaxis()->SetRangeUser(-30000,5000);
      		g[j]->Draw("histsame");
		}
   	}
   } catch (int i ){
       if(i==1) cout<<"There is no waveforms.";
	if(i==2) cout<<"Half of the waveforms are missing.";
   }

}

void check_match(){
	cds_Evtr->GetEntries();

	for(int i=200; i< cds_Evtr->GetEntries(); i++){
		if(i ==196) {cout<<"Test here breaks"<<endl; continue;}
		if(i ==214) {cout<<"Test here breaks"<<endl; continue;}
                if(i ==709) {cout<<"Test here breaks"<<endl; continue;}
                if(i ==718) {cout<<"Test here breaks"<<endl; continue;}


		//select_event(i);
		new_select(i);
		cout<<"ev" <<i << ", N waveforms"<<GetNWaveforms() << endl;
		//if(i==194) continue;
		if(GetNWaveforms() != 116) { cout<<"skip"<<endl; continue; }
		//cout<<"test"<<endl;
		try{ GetWaveform(0); }
		catch(exception){ cout<<"something wrong"<<endl; }
		

		cout<<"event n "<<i <<endl;
//		cout<<"Light ev, utime_ms: "<<utime_ms<<"\t tai_ns: "<<tai_ns<< "\t in us: " << (utime_ms*1000 + tai_ns/1000)<<endl;
//		cout<<"Charge ev, event_unix_ts: "<<event_unix_ts<<"\t event_start_t: "<<event_start_t<< "\t in us: "<<(long long)event_unix_ts *1000000 + event_start_t/10<<endl;

		cout<<"Difference in sec: "<< (long long)(utime_ms) - ((long long)event_unix_ts *1000) <<endl;
		cout<<"Difference: in us "<< (long long)(tai_ns/1000) - ((long long) event_start_t/10) <<endl;
	}
}

void sum_light(){
	//cds_Evtr->GetEntries();
	float_t total_light;
	float_t int_ch_waveform;
	float_t ch_gain;
	int count =0;

 	TH1F *tot_l = new TH1F("total_light","total_light",50,0,50000);
        TH1F *tot_ch = new TH1F("total_charge","total_charge",50,0,50000);

	for(int i=0; i<400; i++){
		cds_Evtr->GetEntry(i);
		if(event_q>50000 || event_q<10000) continue;
		count ++ ;
		//if(count >50) break;
		new_select(i);
		total_light =0;
		cout<<"event "<< i <<", N tracks: "<< event_ntracks <<", N waveforms: "<<GetNWaveforms()<<"\r"<<flush;
		//cout<<"Total charge: "<< event_q <<endl;
		for(int j =0; j<GetNWaveforms();j++){
			auto *s = GetWaveform(j);
			ch_gain = (sn == 175854781) ? gain[0][ch] : gain[1][ch];
			int_ch_waveform = -1.0 * s->Integral(s->FindBin(0),s->FindBin(500))/(64.0 * ch_gain);
			//cout<<"sn: "<<sn<<" ch: "<<ch<<" gain: "<<ch_gain<<" N photons: "<< int_ch_waveform<< endl;
			if(ch_gain !=-1) total_light += int_ch_waveform; 
		}

		//cout<<"total light: "<<total_light<<endl;

		tot_l->Fill(total_light);
		tot_ch->Fill(event_q);

	}// Event loop finished

	TCanvas *c_light = new TCanvas("c_light","c_light",700,500);
	c_light->Divide(2,1);
	c_light->cd(1);
	tot_ch->Draw("hist");
	c_light->cd(2);
	tot_l ->Draw("hist");
}


void plot_waveforms_all(int ev){
	TH1S *h [2][64];
	bool check_fill[2][64] = {{false}};

	TCanvas *c0 = new TCanvas("w_0","waveforms_sn0",1600,900);
	c0->cd();
	c0->Divide(8,8);
        TCanvas *c1 = new TCanvas("w_1","waveforms_sn1",1600,900);
	c1->cd();
	c1->Divide(8,8);

	new_select(ev);
	for(int i=0; i<GetNWaveforms(); i++){
		//cout<<"waveform ch"<<i ;
		auto *s = GetWaveform(i);
		//cout<<" ch "<<ch <<endl;
		if(sn == 175854781 ) {
			h[1][ch] = (TH1S*)s->Clone(); 
			check_fill[1][ch]= true; 
		}else {
			h[0][ch] = (TH1S*)s->Clone();
			check_fill[0][ch] = true;
		}
	}

	c0->Clear();
	c0->Divide(8,8);
	for(int j=0; j<64; j++){
		//cout<<"j"<<j<<endl;
		//cout<<"entries: "<<h[0][j]->GetEntries()<<endl;
		//if(j==1 || j==16 ||j==17 ||j==32 ||j==33 ||j==48 ||j ==49) continue;
		if(check_fill[0][j] == true){
			c0->cd(j+1);
			h[0][j]->SetMaximum(max(7000.0, h[0][j]->GetMaximum() ));
			h[0][j]->SetMinimum(min(-30000.0, h[0][j]->GetMinimum() ));
			h[0][j]->Draw("hist");
		}
	}
	c1->Clear();
	//c1->cd();
	c1->Divide(8,8);
	for(int j=0; j<64; j++){
                if(check_fill[1][j] ==true){
			c1->cd(j+1);
                        h[1][j]->SetMaximum(max(7000.0, h[1][j]->GetMaximum() ));
                        h[1][j]->SetMinimum(min(-30000.0, h[1][j]->GetMinimum() ));
                        h[1][j]->Draw("hist");
		}
	}
}


float Get_dy(int ev){

	float ymax =0.0, ymin=0.0;

	cds_Evtr->GetEntry(ev);
	for(int i=0; i<event_nhits; i++){
		if (i ==0) {ymax = ev_h_y[i]; ymin = ev_h_y[i];}
		else{
			if(ev_h_y[i] > ymax) ymax = ev_h_y[i];
			if(ev_h_y[i] < ymin) ymin = ev_h_y[i];
		}
	}
	//cout<<"dy = "<<(float)(ymax - ymin) <<endl;
	return (ymax - ymin);
}

void doIntegral(){

        Float_t lds_integral[2][64] ={{0.0}};
        Float_t lds_nph[2][64]= {{0.0}};
	Float_t lds_gain[2][64]={{0.0}};
	Int_t lds_ch[2][64] ={{0}};
        TBranch *blds_integral;
        TBranch *blds_nph;
	TBranch *blds_ch;
	TBranch *blds_gain;
        TH1S *h1;
	Float_t gain_0[2][64] ={{-1, -1, 90.89, 88.69, 85.29, 84.73, 85.91, 90.44, -1, 84.81, 82.3, 84.83, 85.17, 81.63, 87, -1, -1, -1, 89.48, 86.17, 93.47, 91.87, 90.85, 91.46, -1, 88.86, 88.83, 79.66, 80.65, 81.18,
      82.17, -1, -1, -1, -1, 93.64, 83.63, 87.79, 89.38, 87.43, -1, 105.87, 106.14, 108.32, 105.77, 106.7, 105.97, -1, -1, -1, 94.95, 95.59, 94.48, 98.86, 96.53, 93.64, -1, 94.46, 93.88, 93.84, 92.8, 92.23, 91.71, -1},
     {-1, -1, 86.69, 85.63, 86.94, 88.26, 86.9, -1, -1, 100.1, 95.42, 90.68, 93.56, -1, 96.72, -1, -1, -1, 70.28, 65.83, 77.34, 80.13, 65.12, 67.16, -1, 99.31, 97.36, 97.09, 94.76, 95.02, 97.22,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 96.95, 95.75, 95.71, 93.87, 99.96, 93.96, -1, -1, -1, 73.15, 75.78, 80.61, 77.1, 78.37, 80.76, -1, 78.54, 77.09, 92.8, 97.58, 95.91, 80.35, -1}};



        cdsf->cd();
        blds_integral = cds_Evtr->Branch("lds_integral",&lds_integral,"lds_integral[2][64]/F");
        blds_nph = cds_Evtr->Branch("lds_nph",&lds_nph,"lds_nph[2][64]/F");
	blds_ch = cds_Evtr->Branch("lds_ch",&lds_ch,"lds_ch[2][64]/I");
	blds_gain = cds_Evtr->Branch("lds_gain",&lds_gain, "lds_gain[2][64]/F");

        int neve=cds_Evtr->GetEntries();
        for(int i=0;i<neve;i++){
		cout<<"ev: "<<i << "\r"<<flush;
                //cout<<"ev: "<<i <<endl;
                memset(lds_integral, 0, sizeof(lds_integral));
                memset(lds_nph, 0, sizeof(lds_nph));
		//cout<<"lds_integral"<<lds_integral[0][43]<<endl;

		//if(i>210){
		cds_Evtr->GetEntry(i);
		//cout<<"event nhits: "<< event_ntracks <<"event q : "<<event_q<<endl;
		if(event_q >10000 && event_ntracks<5 && Get_dy(i)>1000) {

       		        int long_track_index= -1;
 	        	for(int j =0; j<event_ntracks; j++){
                	        cds_Trtr->GetEntryWithIndex(i,j);
                        	if( dy>=1000) long_track_index = j;
                	}
                	if(long_track_index !=-1){

			new_select(i);

	                for(int j=0;j<GetNWaveforms();j++){

	                        if(GetNWaveforms()!=116) break;
        	                h1=GetWaveform(j);
				//if (ch == 44 )cout<<"h1 integral: "<<h1->Integral(h1->FindBin(0),h1->FindBin(500))/(-64.0)<<endl;
                        	if(sn==175854781){ 
					lds_ch[0][ch] = ch;
					lds_gain[0][ch] =gain_0[0][ch];
        	                        lds_integral[0][ch]= -(h1->Integral(h1->FindBin(0),h1->FindBin(500)) )/(float_t)(64.0);
                	                lds_nph[0][ch]= (float_t)lds_integral[0][ch]/(float_t)gain_0[0][ch];
                                }
                        	else if(sn==175780172){
					lds_ch[1][ch] = ch;
                                	lds_gain[1][ch] =gain_0[1][ch];
                               		lds_integral[1][ch]=(float)( -(h1->Integral(h1->FindBin(0),h1->FindBin(500))) )/(float_t)(64.0) ;
                                	lds_nph[1][ch]= (float_t)lds_integral[1][ch]/(float_t)gain_0[1][ch];
                                }
                  	} //end for
			}//end if
		}//end if
                blds_integral->Fill();
                blds_nph->Fill();
		blds_ch->Fill();
		blds_gain->Fill();

		if(i % 100 ==0) cds_Evtr->AutoSave();
        }

        //TFile *new_file = new TFile("test.root","NEW");
        //new_file->cd();
	cdsf->cd();
        cds_Evtr->Write("", TObject::kOverwrite);
        //cds_Trtr->Write();
        //cdsf->Close();
        
}
