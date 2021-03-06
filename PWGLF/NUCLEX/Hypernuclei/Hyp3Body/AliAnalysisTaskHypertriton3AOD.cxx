/**************************************************************************
 * Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/


///////////////////////////////////////////////////////////////////////////
// AliAnalysisTaskHypertriton3AOD class
// analysis task for the study of the production of hypertriton
// which decays in 3 prongs: d+p+pi^-
// This task is optimized for AODs.root
//
// Author: 
// S. Trogolo, trogolo@to.infn.it
///////////////////////////////////////////////////////////////////////////

#include <Riostream.h>
//#include <vector>

#include <TArray.h>
#include <TAxis.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TLorentzRotation.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TParticle.h>
#include <TString.h>
#include <TTree.h>
#include <TVector3.h>

#include "AliAnalysisManager.h"
#include "AliAnalysisTaskHypertriton3AOD.h"
#include "AliAnalysisTaskSE.h"
#include "AliAODEvent.h"
#include "AliAODTrack.h"
#include "AliAODVertex.h"
#include "AliCentrality.h"
#include "AliESD.h"
#include "AliESDInputHandler.h"
#include "AliESDtrack.h"
#include "AliESDtrackCuts.h"
#include "AliESDVertex.h"
#include "AliExternalTrackParam.h"
#include "AliInputEventHandler.h"
#include "AliMCEventHandler.h"
#include "AliMCEvent.h"
#include "AliMultiplicity.h"
#include "AliPID.h"
#include "AliPIDResponse.h"
#include "AliPhysicsSelection.h"
#include "AliStack.h"
#include "AliVertexerTracks.h"
#include "AliVEvent.h"
#include "AliVTrack.h"


ClassImp(AliAnalysisTaskHypertriton3AOD)

using std::vector;

/* $Id$ */
//________________________________________________________________________
AliAnalysisTaskHypertriton3AOD::AliAnalysisTaskHypertriton3AOD(TString taskname):
  AliAnalysisTaskSE(taskname.Data()),
  fAODevent(0),
  fPrimaryVertex(0x0),
  fPIDResponse(0x0),
  fVertexer(0x0),
  fVtx2(0x0),
  fTrkArray(0x0),
  fMC(kFALSE),
  fFillTree(kFALSE),
  fCentrality(0x0),
  fCentralityPercentile(0x0),
  fTriggerConfig(1),
  fSideBand(kFALSE),
  fRequireMinTPCcls(80),
  fRequireMinPionTPCcls(100),
  fRequireMinTPCclsSignal(80),
  fRequireMaxChi2perNDF(5.),
  fRequireEtaMin(-0.9),
  fRequireEtaMax(0.9),
  fRequireTPCrefit(kTRUE),
  fRequireNoKinks(kTRUE),
  fMinPionPt(0.2),
  fMaxPionPt(1.2),
  fDCAPiPVmin(0.1),
  fDCAPPVmin(0.),
  fDCADPVmin(0.),
  fCosPointingAngle(0.998),
  fMaxDecayLength(15.),
  fMinDecayLength(0.),
  fMinNormalizedDecL(0.),
  fMaxLifeTime(9999.),
  fMinLifeTime(0.),
  fRapidity(0.5),
  fMaxPtMother(10.),
  fMinPtMother(2.),
  fDCAPiSVxymax(0.6),
  fDCAPiSVzmax(0.8),
  fDCAProSVmax(0.7),
  fDCADeuSVmax(0.6),
  fDCAdp(0.2),
  fDCApip(0.5),
  fDCAdpi(0.5),
  fAngledp(TMath::Pi()),
  fMaxPMotherCM(999.),
  fLowCentrality(0.),
  fHighCentrality(80.),
  fOutput(0x0),
  fHistCount(0x0),
  fHistCentralityClass(0x0),
  fHistCentralityPercentile(0x0),
  fHistTrigger(0x0),
  fHistMultiplicity(0x0),
  fHistZPrimaryVtx(0x0),
  fHistXPrimaryVtx(0x0),
  fHistYPrimaryVtx(0x0),
  fHistChi2perTPCcluster(0x0),
  fHistTPCpid(0x0),
  fHistTPCdeusignal(0x0),
  fHistTPCprosignal(0x0),
  fHistTPCpionsignal(0x0),
  fHistTPCantideusignal(0x0),
  fHistTPCantiprosignal(0x0),
  fHistTPCpionplussignal(0x0),
  fHistTOFsignal(0x0),
  //fHistTOFdeusignal(0x0),
  //fHistTOFprosignal(0x0),
  //fHistTOFantideusignal(0x0),
  //fHistTOFantiprosignal(0x0),
  fHistTOFdeumass(0x0),
  fHistTOFpromass(0x0),
  fHistpionTPCcls(0x0),
  fHistpTpion(0x0),
  //fHistCorrDCAdprimary(0x0),
  //fHistCorrDCApprimary(0x0),
  //fHistCorrDCApiprimary(0x0),
  fHistDCApiprimary(0x0),
  fHistDCApprimary(0x0),
  fHistDCAdprimary(0x0),
  fHistDCAdeupro(0x0),
  fHistDCApiondeu(0x0),	  
  fHistDCApionpro(0x0),
  fHistDCAdpdpi(0x0),
  fHistDCApdppi(0x0),
  fHistDCApidpip(0x0),
  fHistZDecayVtx(0x0),
  fHistXDecayVtx(0x0),
  fHistYDecayVtx(0x0),
  fHistDCAXYdeuvtx(0x0),
  fHistDCAZdeuvtx(0x0),
  fHistDCAXYprovtx(0x0),
  fHistDCAZprovtx(0x0),
  fHistDCAXYpionvtx(0x0),
  fHistDCAZpionvtx(0x0),
  fHistDecayLengthH3L(0x0),
  fHistNormalizedDecayL(0x0),
  fHistLifetime(0x0),
  fHistAngle_deu_pro(0x0),
  fHistAngle_deu_pion(0x0),
  fHistAngle_pro_pion(0x0),
  fHistAngleCorr_dp_dpi(0x0),
  fHistAngleCorr_dp_ppi(0x0),
  fHistAngleCorr_ppi_dpi(0x0),
  fHistHyperRapidity(0x0),
  fHistCosPointingAngle(0x0),
  fHistDalitz_dp_dpi(0x0),
  fHistDecayMomCM_X(0x0),
  fHistDecayMomCM_Y(0x0),
  fHistDecayMomCM_Z(0x0),
  fHistDecayMomCM_XY(0x0),
  fHistDecayMomCM_XZ(0x0),
  fHistDecayMomCM_YZ(0x0),
  fHistDecayMomCM(0x0),
  fHistMassHypertriton(0x0),
  fHistMassAntiHypertriton(0x0),
  fHistParticle(0x0),
  fHistpionTPCclsMCt(0x0),
  fHistpTpionMCt(0x0),
  fHistpTproMCt(0x0),
  fHistpTdeuMCt(0x0),
  fHistCorrDCAdprimaryMCt(0x0),
  fHistCorrDCApprimaryMCt(0x0),
  fHistCorrDCApiprimaryMCt(0x0),
  fHistDCApiprimaryMCt(0x0),
  fHistDCApprimaryMCt(0x0),
  fHistDCAdprimaryMCt(0x0),
  fHistDCAdeuproMCt(0x0),
  fHistDCApiondeuMCt(0x0),
  fHistDCApionproMCt(0x0),
  fHistZDecayVtxMCt(0x0),
  fHistXDecayVtxMCt(0x0),
  fHistYDecayVtxMCt(0x0),
  fHistDCAXYdeuvtxMCt(0x0),
  fHistDCAZdeuvtxMCt(0x0),
  fHistDCAXYprovtxMCt(0x0),
  fHistDCAZprovtxMCt(0x0),
  fHistDCAXYpionvtxMCt(0x0),
  fHistDCAZpionvtxMCt(0x0),
  fHistMassHypertritonMCt(0x0),
  fTTree(0x0),
  fTchi2NDFdeu(0x0),
  fTPCclsdeu(0x0),
  fTPCclsPIDdeu(0x0),
  fTpTPCdeu(0x0),
  fTpTdeu(0x0),
  fTpdeu(0x0),
  fTTPCnsigmadeu(0x0),
  fTTOFmassdeu(0x0),
  fTDCAXYdeuprvtx(0x0),
  fTDCAZdeuprvtx(0x0),
  fTchi2NDFpro(0x0),
  fTPCclspro(0x0),
  fTPCclsPIDpro(0x0),
  fTpTPCpro(0x0),
  fTpTpro(0x0),
  fTppro(0x0),
  fTTPCnsigmapro(0x0),
  fTTOFmasspro(0x0),
  fTDCAXYproprvtx(0x0),
  fTDCAZproprvtx(0x0),
  fTchi2NDFpion(0x0),
  fTPCclspion(0x0),
  fTPCclsPIDpion(0x0),
  fTpTPCpion(0x0),
  fTpTpion(0x0),
  fTppion(0x0),
  fTTPCnsigmapion(0x0),
  fTDCAXYpioprvtx(0x0),
  fTDCAZpioprvtx(0x0),
  fTDCAdp(0x0),
  fTDCAdpi(0x0),
  fTDCAppi(0x0),
  fTDCAXYdvtx(0x0),
  fTDCAZdvtx(0x0),
  fTDCAXYpvtx(0x0),
  fTDCAZpvtx(0x0),
  fTDCAXYpivtx(0x0),
  fTDCAZpivtx(0x0),
  fTDecayLength(0x0),
  fTCosPA(0x0),
  fTInvariantMass(0x0)
{
  //Constructor

  DefineInput(0, TChain::Class());
  DefineOutput(1, TList::Class());
  DefineOutput(2, TTree::Class());

}


//________________________________________________________________________
AliAnalysisTaskHypertriton3AOD::~AliAnalysisTaskHypertriton3AOD(){
  //Destructor
  if(fOutput){
    delete fOutput;
    fOutput = 0;
  }

  if(fTTree) delete fTTree;

    if(fPIDResponse){
    delete fPIDResponse;
  }

    if(fPrimaryVertex) delete fPrimaryVertex;
    if(fVertexer) delete fVertexer;
    if(fTrkArray) delete fTrkArray;
    if(fVtx2) delete fVtx2;


} // end of Destructor

//________________________________________________________________________
Double_t AliAnalysisTaskHypertriton3AOD::GetDCAcut(Int_t part, Double_t dca)const{
  Double_t cut;
  if(part == 5){ // p-d vs pi-d
    cut = fDCAdpi*(1-(dca/fDCAdp));
    return cut;
  }
  else if(part == 4){ // p-d vs p-pi 
    cut = fDCApip*(1-(dca/fDCAdp));
    return cut;
  }
  return -1;
}

//________________________________________________________________________
void AliAnalysisTaskHypertriton3AOD::SetConvertedAODVertices(AliESDVertex *ESDvtxs)const{

  Double_t pos[3], cov[6], chi2perNDF;
  
  //Conversion of the secondary vertex
  ESDvtxs->GetXYZ(pos);
  ESDvtxs->GetCovMatrix(cov);
  chi2perNDF = ESDvtxs->GetChi2toNDF();

  fVtx2->SetPosition(pos[0], pos[1], pos[2]);
  fVtx2->SetCovMatrix(cov);
  fVtx2->SetChi2perNDF(chi2perNDF);
  fVtx2->SetID(ESDvtxs->GetID());
  fVtx2->SetType(AliAODVertex::kUndef);
  
}

//________________________________________________________________________
void AliAnalysisTaskHypertriton3AOD::UserCreateOutputObjects(){
  // Create a TList with Histograms
  // Called once
  //printf("**************************************************\n");
  //printf("AliAnalysisTaskHypertriton3AOD::CreateOutputObjects()\n");
  //printf("**************************************************\n");

  fVertexer = new AliVertexerTracks();
  fTrkArray = new TObjArray(3);
  fVtx2 = new AliAODVertex();
    
  fOutput = new TList();
  fOutput->SetOwner();
  fOutput->SetName("clistHypertriton");

  fHistCount = new TH1F("fHistCount","Counter histogram",2,-0.5,1.5);
  fHistCount->GetXaxis()->SetBinLabel(1,"Event");
  
  fHistCentralityClass = new TH1F("fHistCentralityClass","Centrality Class; centrality class; entries",11,-0.5,10.5);
  fHistCentralityPercentile = new TH1F("fHistCentralityPercentile","Centrality; centrality percentile; entries",101,-0.5,100.5);
  
  fHistTrigger = new TH1F("fHistTrigger","Trigger statistics",4,-0.5,3.5);
  fHistTrigger->GetXaxis()->SetBinLabel(1,"kMB");
  fHistTrigger->GetXaxis()->SetBinLabel(2,"kCentral");
  fHistTrigger->GetXaxis()->SetBinLabel(3,"kSemiCentral");

  fHistMultiplicity = new TH1F("fHistMultiplicity ", "Multiplicity; multiplicity; entries", 100, 0, 20000);
  
  fHistZPrimaryVtx = new TH1F("fHistZPrimaryVtx","primary vertex - Z coordinate; z_{primary vtx} (cm); entries",40,-10.,10.);

  fHistXPrimaryVtx = new TH1F("fHistXPrimaryVtx","primary vertex - X coordinate; x_{primary vtx} (cm); entries",800,-1.,1.);
  
  fHistYPrimaryVtx = new TH1F("fHistYPrimaryVtx","primary vertex - Y coordinate; y_{primary vtx} (cm); entries",800,-1.,1.);

  fHistChi2perTPCcluster = new TH1F("fHistChi2perTPCcluster","#Chi^{2}/TPC cluster distribution; #Chi^{2}/TPCcls; entries",44,-0.5,10.5);
 
  
  //TPC
  fHistTPCpid = new TH2F("fHistTPCpid", "dE/dx for all tracks; p (GeV/c); TPC signal", 500, 0., 10, 300, 0, 2100);
  fHistTPCpid->SetOption("scat");
  fHistTPCpid->SetMarkerStyle(kFullCircle);

  //Hypertriton prongs
  fHistTPCdeusignal = new TH2F("fHistTPCdeusignal", "dE/dx after d PID; p (GeV/c); TPC signal", 500, 0., 10, 300, 0, 2100);
  fHistTPCdeusignal->SetOption("scat");
  fHistTPCdeusignal->SetMarkerStyle(kFullCircle);

  fHistTPCprosignal = new TH2F("fHistTPCprosignal", "dE/dx after p PID; p (GeV/c); TPC signal", 500, 0., 10, 300, 0, 2100);
  fHistTPCprosignal->SetOption("scat");
  fHistTPCprosignal->SetMarkerStyle(kFullCircle);

  fHistTPCpionsignal= new TH2F("fHistTPCpionsignal", "dE/dx after  #pi^{-} PID; p (GeV/c); TPC signal", 500, 0., 10, 300, 0, 2100);
  fHistTPCpionsignal->SetOption("scat");
  fHistTPCpionsignal->SetMarkerStyle(kFullCircle);
  
  //Anti-hypertriton prongs
  fHistTPCantideusignal = new TH2F("fHistTPCantideusignal", "dE/dx after d PID; p (GeV/c); TPC signal", 500, 0., 10, 300, 0, 2100);
  fHistTPCantideusignal->SetOption("scat");
  fHistTPCantideusignal->SetMarkerStyle(kFullCircle);

  fHistTPCantiprosignal = new TH2F("fHistTPCantiprosignal", "dE/dx after p PID; p (GeV/c); TPC signal", 500, 0., 10, 300, 0, 2100);
  fHistTPCantiprosignal->SetOption("scat");
  fHistTPCantiprosignal->SetMarkerStyle(kFullCircle);

  fHistTPCpionplussignal= new TH2F("fHistTPCpionplussignal", "dE/dx after  #pi^{+} PID; p (GeV/c); TPC signal", 500, 0., 10, 300, 0, 2100);
  fHistTPCpionplussignal->SetOption("scat");
  fHistTPCpionplussignal->SetMarkerStyle(kFullCircle);

  //TOF
  
  fHistTOFsignal = new TH2F("fHistTOFsignal","TOF signal; p_{TPC} (GeV/c); #beta",400,0.,4.,400,0.,1.1);

  //fHistTOFdeusignal = new TH2F("fHistTOFdeusignal","#beta vs TPCmom - deuteron; p_{TPC} (GeV/c); #beta",400,0.,4.,400,0.,1.1);

  //fHistTOFprosignal = new TH2F("fHistTOFprosignal","#beta vs TPCmom - proton; p_{TPC} (GeV/c); #beta",400,0.,4.,400,0.,1.1);

  //fHistTOFantideusignal = new TH2F("fHistTOFantideusignal","#beta vs TPCmom - anti-deuteron; p_{TPC} (GeV/c); #beta",400,0.,4.,400,0.,1.1);

  //fHistTOFantiprosignal = new TH2F("fHistTOFantiprosignal","#beta vs TPCmom - anti-proton; p_{TPC} (GeV/c); #beta",400,0.,4.,400,0.,1.1);
  
  fHistTOFdeumass = new TH1F("fHistTOFdeumass","deuteron mass distribution - TOF; mass (GeV/c^{2}); entries",400,0.8,2.8);
  
  fHistTOFpromass = new TH1F("fHistTOFpromass","proton mass distribution - TOF; mass (GeV/c^{2}); entries",200,0.5,1.5);


  fHistpionTPCcls = new TH1F("fHistpionTPCcls","#pi^{-} TPC clusters; TPC clusters; entries",201,-0.5,200.5);
  fHistpTpion = new TH1F("fHistpTpion","pion p_{T} distribution; p_{T} (GeV/c);entries",800,0.,8.);
  //fHistCorrDCAdprimary = new TH2F("fHistCorrDCAdprimary","DCA_{PV,xy} vs DCA_{PV,z} - deuteron; DCA_{xy} (cm); DCA_{z} (cm)",320,-20.f,20.f,320,-20.f,20.f);
  //fHistCorrDCApprimary = new TH2F("fHistCorrDCApprimary","DCA_{PV,xy} vs DCA_{PV,z} - proton; DCA_{xy} (cm); DCA_{z} (cm)",320,-20.f,20.f,320,-20.f,20.f);
  //fHistCorrDCApiprimary = new TH2F("fHistCorrDCApiprimary","DCA_{PV,xy} vs DCA_{PV,z} - pion; DCA_{xy} (cm); DCA_{z} (cm)",320,-20.f,20.f,320,-20.f,20.f);
  fHistDCApiprimary = new TH1F("fHistDCApiprimary","DCA pion-primary vertex; DCA (cm); entries",1600,0.f,40.f);
  fHistDCApprimary = new TH1F("fHistDCApprimary","DCA proton-primary vertex; DCA (cm); entries",1600,0.f,40.f);
  fHistDCAdprimary = new TH1F("fHistDCAdprimary","DCA deuteron-primary vertex; DCA (cm); entries",1600,0.f,40.f);
  
  //DCA prongs
  fHistDCAdeupro = new TH1F("fHistDCAdeupro","DCA d-p tracks;d-p DCA (cm);entries",550,-0.5,5.0);
  
  fHistDCApiondeu = new TH1F("fHistDCApiondeu","DCA #pi^{-}-d tracks; #pi^{-}-d DCA (cm); entries",550,-0.5,5.0);
  
  fHistDCApionpro = new TH1F("fHistDCApionpro","DCA #pi^{-}-p tracks; #pi^{-}-p DCA (cm); entries",550,-0.5,5.0);

  fHistDCAdpdpi = new TH2F("fHistDCAdpdpi","DCA deu-pro vs DCA deu-pion;DCA_{dp} (cm); DCA_{d#pi} (cm)",100,0.,1.,100,0.,1.);
  fHistDCApdppi = new TH2F("fHistDCApdppi","DCA deu-pro vs DCA pion-pro; DCA_{dp} (cm); DCA_{#pip} (cm)",100,0.,1.,100,0.,1.);
  fHistDCApidpip = new TH2F("fHistDCApidpip","DCA pion-pro vs DCA pion-deu;DCA_{#pip} (cm); DCA_{#pid} (cm)",100,0.,1.,100,0.,1.);
  
  //Decay histo
  fHistZDecayVtx = new TH1F("fHistZDecayVtx","decay vertex - z coordinate; z_{decay vtx} (cm); entries",800,-20.f,20.f);
  fHistXDecayVtx = new TH1F("fHistXDecayVtx","decay vertex - x coordinate; x_{decay vtx} (cm); entries",8000,-20.f,20.f);
  fHistYDecayVtx = new TH1F("fHistYDecayVtx","decay vertex - y coordinate; y_{decay vtx} (cm); entries",8000,-20.f,20.f);
  fHistDCAXYdeuvtx = new TH1F("fHistDCAXYdeuvtx","DCA candidate d-decay vertex - xy coordinate; DCA_{xy} (cm); entries",200,-5.,5.);
  fHistDCAZdeuvtx = new TH1F("fHistDCAZdeuvtx","DCA candidate d-decay vertex - z coordinate; DCA_{z} (cm); entries",200,-10.,10.);
  fHistDCAXYprovtx = new TH1F("fHistDCAXYprovtx","DCA candidate p-decay vertex - xy coordinate; DCA_{xy} (cm); entries",200,-5.,5.);
  fHistDCAZprovtx = new TH1F("fHistDCAZprovtx","DCA candidate p-decay vertex - z coordinate; DCA_{z} (cm); entries",200,-10.,10.);
  fHistDCAXYpionvtx = new TH1F("fHistDCAXYpionvtx","DCA candidate #pi^{-}-decay vertex - xy coordinate; DCA_{xy} (cm); entries",200,-5.,5.);
  fHistDCAZpionvtx = new TH1F("fHistDCAZpionvtx","DCA candidate #pi^{-}-decay vertex - z coordinate; DCA_{z} (cm); entries",200,-10.,10.);
  fHistDecayLengthH3L = new TH1F("fHistDecayLengthH3L","decay length ^{3}H_{#Lambda}; decay length (cm); entries",400,0.,400.);
  fHistNormalizedDecayL = new TH1F("fHistNormalizedDecayL","normalized decay length; decL/#sigma_{dL}; entries",400,0.,100.);
  fHistLifetime = new TH1F("fHistLifetime","ct ^{3}H_{#Lambda}; ct(cm); entries",400,0.,400.);
  
  fHistAngle_deu_pro = new TH1F("fHistAngle_deu_pro","Angle between d and p; #alpha_{d_p} (rad); entries/(0.03 rad)",100,0.,TMath::Pi());
  fHistAngle_deu_pion = new TH1F("fHistAngle_deu_pion","Angle between d and #pi; #beta_{d_#pi} (rad); entries/(0.03 rad)",100,0.,TMath::Pi());
  fHistAngle_pro_pion = new TH1F("fHistAngle_pro_pion","Angle between p and #pi; #gamma_{p_#pi} (rad);entries/(0.03 rad)",100,0.,TMath::Pi());
  fHistAngleCorr_dp_dpi = new TH2F("fHistAngleCorr_dp_dpi","Correlation: #alpha_{d_p} vs #beta_{d_#pi};#alpha_{d_p};#beta_{d_#pi}",100,0.,TMath::Pi(),100,0.,TMath::Pi());
  fHistAngleCorr_dp_ppi = new TH2F("fHistAngleCorr_dp_ppi","Correlation: #alpha_{d_p} vs #gamma_{p_#pi};#alpha_{d_p};#gamma_{p_#pi}",100,0.,TMath::Pi(),100,0.,TMath::Pi());
  fHistAngleCorr_ppi_dpi = new TH2F("fHistAngleCorr_ppi_dpi","Correlation: #gamma_{p_#pi} vs #beta_{d_#pi};#gamma_{p_#pi};#beta_{d_#pi}",100,0.,TMath::Pi(),100,0.,TMath::Pi());
  fHistHyperRapidity = new TH1F("fHistHyperRapidity","rapidity distribution of ^{3}H_{#Lambda}; y; entries",400,-2.f,2.f);

  fHistCosPointingAngle= new TH1F("fHistCosPointingAngle", "Cos pointing angle distribution; cos point angle; entries", 220, -1.1, 1.1);
  fHistDalitz_dp_dpi = new TH2F("fHistDalitz_dp_dpi","Dalitz plot: m^{2}_{d#pi} vs m^{2}_{dp}; m^{2}_{d#pi} (GeV^{2}/c^{4}); m^{2}_{dp} (GeV^{2}/c^{4})",250,4.,9.,350.,7.9,15.);
  fHistDecayMomCM_X = new TH1F("fHistDecayMomCM_X","p_{^{3}H_{#Lambda},X} in center-of-mass; p_{X} (GeV/c); entries/0.02",200.,-2.,2.);
  fHistDecayMomCM_Y = new TH1F("fHistDecayMomCM_Y","p_{^{3}H_{#Lambda},Y} in center-of-mass; p_{Y} (GeV/c); entries/0.02",200.,-2.,2.);
  fHistDecayMomCM_Z = new TH1F("fHistDecayMomCM_Z","p_{^{3}H_{#Lambda},Z} in center-of-mass; p_{Z} (GeV/c); entries/0.02",200.,-2.,2.);
  fHistDecayMomCM_XY = new TH2F("fHistDecayMomCM_XY","p_{^{3}H_{#Lambda},X} vs p_{^{3}H_{#Lambda},Y} in center-of-mass; p_{X} (GeV/c); p_{Y} (GeV/c)",200.,-2.,2.,200.,-2.,2.);
  fHistDecayMomCM_XZ = new TH2F("fHistDecayMomCM_XZ","p_{^{3}H_{#Lambda},X} vs p_{^{3}H_{#Lambda},Z} in center-of-mass; p_{X} (GeV/c); p_{Z} (GeV/c)",200.,-2.,2.,200.,-2.,2.);
  fHistDecayMomCM_YZ = new TH2F("fHistDecayMomCM_YZ","p_{^{3}H_{#Lambda},Y} vs p_{^{3}H_{#Lambda},Z} in center-of-mass; p_{Y} (GeV/c); p_{Z} (GeV/c)",200.,-2.,2.,200.,-2.,2.);
  fHistDecayMomCM = new TH1F("fHistDecayMomCM","^{3}H_{#Lambda} momentum in center-of-mass; p_{^{3}H_{#Lambda}} (GeV/c); entries/0.01",150,0.,3.);
  fHistMassHypertriton = new TH1F("fHistMassHypertriton", "Invariant mass distribution d+p+#pi^{-};invariant mass d+p+#pi^{-} (GeV/c^{2}); entries ", 400, 2.9, 3.1);
  fHistMassAntiHypertriton = new TH1F("fHistMassAntiHypertriton", "Invariant mass distribution #bar{d} + #bar{p} + #pi^{+};invariant mass #bar{d} + #bar{p} + #pi^{+} (GeV/c^{2}); entries ", 400, 2.9, 3.1);

  if(fMC){
    fHistParticle = new TH1F("fHistParticle","Check particle candidate",23,-0.5,22.5);
    fHistParticle->GetXaxis()->SetBinLabel(1,"electron");
    fHistParticle->GetXaxis()->SetBinLabel(2,"positron");
    fHistParticle->GetXaxis()->SetBinLabel(3,"#pi^{+}");
    fHistParticle->GetXaxis()->SetBinLabel(4,"#pi^{-}");
    fHistParticle->GetXaxis()->SetBinLabel(5,"K^{+}");
    fHistParticle->GetXaxis()->SetBinLabel(6,"K^{-}");
    fHistParticle->GetXaxis()->SetBinLabel(7,"proton");
    fHistParticle->GetXaxis()->SetBinLabel(8,"anti-proton");
    fHistParticle->GetXaxis()->SetBinLabel(9,"deuteron");
    fHistParticle->GetXaxis()->SetBinLabel(10,"anti-deuteron");
    fHistParticle->GetXaxis()->SetBinLabel(11,"triton");
    fHistParticle->GetXaxis()->SetBinLabel(12,"anti-triton");
    fHistParticle->GetXaxis()->SetBinLabel(13,"He3");
    fHistParticle->GetXaxis()->SetBinLabel(14,"anti-He3");
    fHistParticle->GetXaxis()->SetBinLabel(15,"#alpha");
    fHistParticle->GetXaxis()->SetBinLabel(16,"anti-#alpha");
    fHistParticle->GetXaxis()->SetBinLabel(17,"^{3}H_{#Lambda}");
    fHistParticle->GetXaxis()->SetBinLabel(18,"anti-^{3}H_{#Lambda}");
    fHistParticle->GetXaxis()->SetBinLabel(19,"^{3}H_{#Lambda} - 2 body");
    fHistParticle->GetXaxis()->SetBinLabel(20,"anti-^{3}H_{#Lambda} - 2 body");
    fHistParticle->GetXaxis()->SetBinLabel(21,"#Lambda - 2 body");
    
    fHistpionTPCclsMCt = new TH1F("fHistpionTPCclsMCt","#pi^{-} TPC clusters - MCtruth; TPC clusters; entries",201,-0.5,200.5);
    fHistpTpionMCt = new TH1F("fHistpTpionMCt","pion p_{T} distribution; p_{T} (GeV/c);entries",800,0.,8.);
    fHistpTproMCt = new TH1F("fHistpTproMCt","proton p_{T} distribution; p_{T} (GeV/c);entries",800,0.,8.);
    fHistpTdeuMCt = new TH1F("fHistpTdeuMCt","deuteron p_{T} distribution; p_{T} (GeV/c);entries",800,0.,8.);
    fHistCorrDCAdprimaryMCt = new TH2F("fHistCorrDCAdprimaryMCt","DCA_{PV,xy} vs DCA_{PV,z} - deuteron MCtruth; DCA_{xy} (cm); DCA_{z} (cm)",320,-20.f,20.f,320,-20.f,20.f);
    fHistCorrDCApprimaryMCt = new TH2F("fHistCorrDCApprimaryMCt","DCA_{PV,xy} vs DCA_{PV,z} - proton MCtruth; DCA_{xy} (cm); DCA_{z} (cm)",320,-20.f,20.f,320,-20.f,20.f);
    fHistCorrDCApiprimaryMCt = new TH2F("fHistCorrDCApiprimaryMCt","DCA_{PV,xy} vs DCA_{PV,z} - pion MCtruth; DCA_{xy} (cm); DCA_{z} (cm)",320,-20.f,20.f,320,-20.f,20.f);
    fHistDCApiprimaryMCt = new TH1F("fHistDCApiprimaryMCt","DCA pion-primary vertex; DCA (cm); entries",3200,0.f,80.f);
    fHistDCApprimaryMCt = new TH1F("fHistDCApprimaryMCt","DCA proton-primary vertex; DCA (cm); entries",3200,0.f,80.f);
    fHistDCAdprimaryMCt = new TH1F("fHistDCAdprimaryMCt","DCA deuteron-primary vertex; DCA (cm); entries",3200,0.f,80.f);
    
    fHistDCAdeuproMCt = new TH1F("fHistDCAdeuproMCt","DCA d-p tracks - MCtruth;d-p DCA (cm);entries",250,-1.0,1.0);
    fHistDCApiondeuMCt = new TH1F("fHistDCApiondeuMCt","DCA #pi^{-}-d tracks - MCtruth;#pi^{-}-d DCA (cm);entries",250,-1.0,1.0);
    fHistDCApionproMCt = new TH1F("fHistDCApionproMCt","DCA #pi^{-}-p tracks - MCtruth;#pi^{-}-p DCA (cm);entries",250,-1.0,1.0);
    
    fHistZDecayVtxMCt = new TH1F("fHistZDecayVtxMCt","decay vertex - z coordinate - MCtruth; z_{decay vtx} (cm); entries",800,-20.f,20.f);
    fHistXDecayVtxMCt = new TH1F("fHistXDecayVtxMCt","decay vertex - x coordinate - MCtruth; x_{decay vtx} (cm); entries",8000,-20.f,20.f);
    fHistYDecayVtxMCt = new TH1F("fHistYDecayVtxMCt","decay vertex - y coordinate - MCtruth; y_{decay vtx} (cm); entries",8000,-20.f,20.f);
    fHistDCAXYdeuvtxMCt = new TH1F("fHistDCAXYdeuvtxMCt","DCA candidate d-decay vertex - xy coordinate - MCtruth; DCA_{xy} (cm); entries",200,-5.,5.);
    fHistDCAZdeuvtxMCt = new TH1F("fHistDCAZdeuvtxMCt","DCA candidate d-decay vertex - z coordinate - MCtruth; DCA_{z} (cm); entries",200,-10.,10.);
    fHistDCAXYprovtxMCt = new TH1F("fHistDCAXYprovtxMCt","DCA candidate p-decay vertex - xy coordinate - MCtruth; DCA_{xy} (cm); entries",200,-5.,5.);
    fHistDCAZprovtxMCt = new TH1F("fHistDCAZprovtxMCt","DCA candidate p-decay vertex - z coordinate - MCtruth; DCA_{z} (cm); entries",200,-10.,10.);
    fHistDCAXYpionvtxMCt = new TH1F("fHistDCAXYpionvtxMCt","DCA candidate #pi^{-}-decay vertex - xy coordinate - MCtruth; DCA_{xy} (cm); entries",200,-5.,5.);
    fHistDCAZpionvtxMCt = new TH1F("fHistDCAZpionvtxMCt","DCA candidate #pi^{-}-decay vertex - z coordinate - MCtruth; DCA_{z} (cm); entries",200,-10.,10.);
    
    
    fHistMassHypertritonMCt = new TH1F("fHistMassHypertritonMCt","^{3}H_{#Lambda} invariant mass - MCtruth; invariant mass d+p+#pi^{-} (GeV/c^{2}); entries", 400,2.9,3.1);
  }
  
  fOutput->Add(fHistCount);
  fOutput->Add(fHistCentralityClass);
  fOutput->Add(fHistCentralityPercentile);
  fOutput->Add(fHistTrigger);
  fOutput->Add(fHistMultiplicity);
  fOutput->Add(fHistZPrimaryVtx);
  fOutput->Add(fHistXPrimaryVtx);
  fOutput->Add(fHistYPrimaryVtx);
  fOutput->Add(fHistChi2perTPCcluster);
  fOutput->Add(fHistTPCpid);
  fOutput->Add(fHistTPCdeusignal);
  fOutput->Add(fHistTPCprosignal);
  fOutput->Add(fHistTPCpionsignal);
  fOutput->Add(fHistTPCantideusignal);
  fOutput->Add(fHistTPCantiprosignal);
  fOutput->Add(fHistTPCpionplussignal);
  fOutput->Add(fHistTOFsignal);
  //fOutput->Add(fHistTOFdeusignal);
  //fOutput->Add(fHistTOFprosignal);
  //fOutput->Add(fHistTOFantideusignal);
  //fOutput->Add(fHistTOFantiprosignal);
  fOutput->Add(fHistTOFdeumass);
  fOutput->Add(fHistTOFpromass);
  fOutput->Add(fHistpionTPCcls);
  fOutput->Add(fHistpTpion);
  //fOutput->Add(fHistCorrDCAdprimary);
  //fOutput->Add(fHistCorrDCApprimary);
  //fOutput->Add(fHistCorrDCApiprimary);
  fOutput->Add(fHistDCApiprimary);
  fOutput->Add(fHistDCApprimary);
  fOutput->Add(fHistDCAdprimary);
  fOutput->Add(fHistDCAdeupro);
  fOutput->Add(fHistDCApiondeu);	  
  fOutput->Add(fHistDCApionpro);
  fOutput->Add(fHistDCAdpdpi);
  fOutput->Add(fHistDCApdppi);
  fOutput->Add(fHistDCApidpip);
  fOutput->Add(fHistZDecayVtx);
  fOutput->Add(fHistXDecayVtx);
  fOutput->Add(fHistYDecayVtx);
  fOutput->Add(fHistDCAXYdeuvtx);
  fOutput->Add(fHistDCAZdeuvtx);
  fOutput->Add(fHistDCAXYprovtx);
  fOutput->Add(fHistDCAZprovtx);
  fOutput->Add(fHistDCAXYpionvtx);
  fOutput->Add(fHistDCAZpionvtx);
  fOutput->Add(fHistDecayLengthH3L);
  fOutput->Add(fHistNormalizedDecayL);
  fOutput->Add(fHistLifetime);
  fOutput->Add(fHistAngle_deu_pro);
  fOutput->Add(fHistAngle_deu_pion);
  fOutput->Add(fHistAngle_pro_pion);
  fOutput->Add(fHistAngleCorr_dp_dpi);
  fOutput->Add(fHistAngleCorr_dp_ppi);
  fOutput->Add(fHistAngleCorr_ppi_dpi);
  fOutput->Add(fHistHyperRapidity);
  fOutput->Add(fHistCosPointingAngle);
  fOutput->Add(fHistDalitz_dp_dpi);
  fOutput->Add(fHistDecayMomCM_X);
  fOutput->Add(fHistDecayMomCM_Y);
  fOutput->Add(fHistDecayMomCM_Z);
  fOutput->Add(fHistDecayMomCM_XY);
  fOutput->Add(fHistDecayMomCM_XZ);
  fOutput->Add(fHistDecayMomCM_YZ);
  fOutput->Add(fHistDecayMomCM);
  fOutput->Add(fHistMassHypertriton);
  fOutput->Add(fHistMassAntiHypertriton);

  if(fMC){
    fOutput->Add(fHistParticle);
    fOutput->Add(fHistpionTPCclsMCt);
    fOutput->Add(fHistpTpionMCt);
    fOutput->Add(fHistpTproMCt);
    fOutput->Add(fHistpTdeuMCt);
    fOutput->Add(fHistCorrDCAdprimaryMCt);
    fOutput->Add(fHistCorrDCApprimaryMCt);
    fOutput->Add(fHistCorrDCApiprimaryMCt);
    fOutput->Add(fHistDCApiprimaryMCt);
    fOutput->Add(fHistDCApprimaryMCt);
    fOutput->Add(fHistDCAdprimaryMCt);
    fOutput->Add(fHistDCAdeuproMCt);
    fOutput->Add(fHistDCApiondeuMCt);
    fOutput->Add(fHistDCApionproMCt);
    fOutput->Add(fHistZDecayVtxMCt);
    fOutput->Add(fHistXDecayVtxMCt);
    fOutput->Add(fHistYDecayVtxMCt);
    fOutput->Add(fHistDCAXYdeuvtxMCt);
    fOutput->Add(fHistDCAZdeuvtxMCt);
    fOutput->Add(fHistDCAXYprovtxMCt);
    fOutput->Add(fHistDCAZprovtxMCt);
    fOutput->Add(fHistDCAXYpionvtxMCt);
    fOutput->Add(fHistDCAZpionvtxMCt);
    fOutput->Add(fHistMassHypertritonMCt);
  }
  
  // Post output data.
  PostData(1,fOutput);
  //printf("**************************************************\n");
  //printf("end of fOutput\n");
  //printf("**************************************************\n");

  if(fFillTree){
  OpenFile(2);
  fTTree = new TTree("hypertriton","hypertriton candidates");
  fTTree->Branch("Chi2NDFdeu",&fTchi2NDFdeu,"Chi2NDFdeu/F");
  fTTree->Branch("TPCclsdeu",&fTPCclsdeu,"TPCclsdeu/s");
  fTTree->Branch("TPCclsPIDdeu",&fTPCclsPIDdeu,"TPCclsPIDdeu/s");
  fTTree->Branch("pTPCdeu",&fTpTPCdeu,"pTPCdeu/F");
  fTTree->Branch("pTdeu",&fTpTdeu,"pTdeu/F");
  fTTree->Branch("pdeu",&fTpdeu,"pdeu/F");
  fTTree->Branch("TPCnsigmadeu",&fTTPCnsigmadeu,"TPCnsigmadeu/F");
  fTTree->Branch("TOFmassdeu",&fTTOFmassdeu,"TOFmassdeu/F");
  fTTree->Branch("DCAxydeuprim",&fTDCAXYdeuprvtx,"DCAxydeuprim/F");
  fTTree->Branch("DCAzdeuprim",&fTDCAZdeuprvtx,"DCAzdeuprim/F");
  fTTree->Branch("Chi2NDFpro",&fTchi2NDFpro,"Chi2NDFpro/F");
  fTTree->Branch("TPCclspro",&fTPCclspro,"TPCclspro/s");
  fTTree->Branch("TPCclsPIDpro",&fTPCclsPIDpro,"TPCclsPIDpro/s");
  fTTree->Branch("pTPCpro",&fTpTPCpro,"pTPCpro/F");
  fTTree->Branch("pTpro",&fTpTpro,"pTpro/F");
  fTTree->Branch("ppro",&fTppro,"ppro/F");
  fTTree->Branch("TPCnsigmapro",&fTTPCnsigmapro,"TPCnsigmapro/F");
  fTTree->Branch("TOFmasspro",&fTTOFmasspro,"TOFmasspro/F");
  fTTree->Branch("DCAxyproprim",&fTDCAXYproprvtx,"DCAxyproprim/F");
  fTTree->Branch("DCAzproprim",&fTDCAZproprvtx,"DCAzproprim/F");
  fTTree->Branch("Chi2NDFpion",&fTchi2NDFpion,"Chi2NDFpion/F");
  fTTree->Branch("TPCclspion",&fTPCclspion,"TPCclspion/s");
  fTTree->Branch("TPCclsPIDpion",&fTPCclsPIDpion,"TPCclsPIDpion/s");
  fTTree->Branch("pTPCpion",&fTpTPCpion,"pTPCpion/F");
  fTTree->Branch("pTpion",&fTpTpion,"pTpion/F");
  fTTree->Branch("ppion",&fTppion,"ppion/F");
  fTTree->Branch("TPCnsigmapion",&fTTPCnsigmapion,"TPCnsigmapion/F");
  fTTree->Branch("DCAxypioprim",&fTDCAXYpioprvtx,"DCAxypioprim/F");
  fTTree->Branch("DCAzpioprim",&fTDCAZpioprvtx,"DCAzpioprim/F");
  fTTree->Branch("DCAdp",&fTDCAdp,"DCAdp/F");
  fTTree->Branch("DCAdpi",&fTDCAdpi,"DCAdpi/F");
  fTTree->Branch("DCAppi",&fTDCAppi,"DCAppi/F");
  fTTree->Branch("DCAXYdeuvtx",&fTDCAXYdvtx,"DCAXYdeuvtx/F");
  fTTree->Branch("DCAZdeuvtx",&fTDCAZdvtx,"DCAZdeuvtx/F");
  fTTree->Branch("DCAXYprovtx",&fTDCAXYpvtx,"DCAXYprovtx/F");
  fTTree->Branch("DCAZprovtx",&fTDCAZpvtx,"DCAZprovtx/F");
  fTTree->Branch("DCAXYpionvtx",&fTDCAXYpivtx,"DCAXYpionvtx/F");
  fTTree->Branch("DCAZpionvtx",&fTDCAZpivtx,"DCAZpionvtx/F");
  fTTree->Branch("DecayLength",&fTDecayLength,"DecayLength/F");
  fTTree->Branch("CosPA",&fTCosPA,"CosPA/F");
  fTTree->Branch("InvariantMass",&fTInvariantMass,"InvariantMass/F");

  fTTree->SetAutoSave(150000000);
  PostData(2,fTTree);
  } else {
    fTTree = new TTree();
  } 

  
  //printf("**************************************************\n");
  //printf("end of CreateOutputObjects\n");
  //printf("**************************************************\n");
  
} // end of UserCreateOutputObjects

//________________________________________________________________________
void AliAnalysisTaskHypertriton3AOD::UserExec(Option_t *){
  // Main loop
  // Called for each event
  //----------------------------------------------------------------------------
  // Mass definition
  Double_t pionMass     = 0.13957; // pion mass = TDatabasePDG::Instance()->GetParticle(211)->Mass() GeV/c2
  Double_t protonMass   = 0.93827; // proton mass = TDatabasePDG::Instance()->GetParticle(2212)->Mass() GeV/c2
  Double_t deuteronMass = 1.87561; // deuteron mass = AliPID::ParticleMass(AliPID::kDeuteron) GeV/c2
  //Double_t helium3Mass = AliPID::ParticleMass(AliPID::kHe3); // helium3 mass = 2.80923 GeV/c2
  Double_t hypertritonMass = 2.991106;
  
  //define PDGCodes  
  //Long_t pdgPionPlus           =              211;
  Long_t pdgPionMinus          =             -211;
  Long_t pdgProton             =             2212;
  //Long_t pdgAntiProton         =            -2212;
  Long_t pdgDeuteron           =       1000010020;
  //Long_t pdgAntiDeuteron       =      -1000010020;
  //Long_t pdgHypertriton        =       1010010030;
  //Long_t pdgAntiHypertriton    =      -1010010030;
  //----------------------------------------------------------------------------
  
  
  //==========Define variables==========//
  //===PID loop===//
  Int_t ntracks,label, labelM = 0 ;
  Double_t chi2PerClusterTPC, nClustersTPC=0.;
  Double_t p = 0.;
  Float_t mass, beta, time, time0, gamma = 0.;
  Float_t minPos = 9999.;
  Float_t nsigma[3] = {9999.,9999.,9999.};
  Int_t partID = -1;
  AliAODTrack *track = 0x0;


  //===Combining tracks loop===//
  Int_t ndau = 0;
  Float_t dcapiprim, dcapprim, dcadprim = 0.;
  Double_t dprim[2] = {0.,0.}; //array for the dca from primary vertex coordinates (xy,z) - deuteron
  Double_t dprimc[3] = {0.,0.,0.}; // covariance matrix elements of the TPC only impact parameter
  Double_t pprim[2] = {0.,0.}; //array for the dca from primary vertex coordinates (xy,z) - proton
  Double_t pprimc[3] = {0.,0.,0.}; // covariance matrix elements of the TPC only impact parameter
  Double_t piprim[2] = {0.,0.}; //array for the dca from primary vertex coordinates (xy,z) - pion
  Double_t piprimc[3] = {0.,0.,0.}; // covariance matrix elements of the TPC only impact parameter
  Double_t dlh[3] = {0,0,0}; //array for the coordinates of the decay length
  Double_t dca_dp, dca_dpi, dca_ppi = 0.;
  Double_t dcad[2] = {0.,0.}; // dca between the candidate d,p,pi
  Double_t dcap[2] = {0.,0.}; // and the candidate decay vertex
  Double_t dcapi[2] = {0.,0.}; // dcad[0]= transverse plane coordinate; dcad[1]= z coordinate
  Double_t decayLengthH3L, normalizedDecayL, pTmom, rapidity, pointingAngleH, ctau= 0.;
  Double_t lD, lP, lPi = 0;
  Double_t decVt[3] = {0.,0.,0.};
  Bool_t brotherHood = kFALSE;
  TLorentzVector posD, posP, negPi; //Lorentz vector of deuteron, proton and pion in the LAB
  TLorentzVector posDCM, posPCM, negPiCM; // Lorentz vector of deuteron, proton and pion in the CM
  AliAODTrack *trackD = 0x0;
  AliAODTrack *trackP = 0x0;
  AliAODTrack *trackNPi = 0x0;
  
  
  
  //==========ESD event==========
  fAODevent=(AliVEvent*)InputEvent();
  if(!fAODevent){
    printf("AliAnalysisTaskHypertriton3AOD::Exec(): bad ESD\n");
    PostData(1,fOutput);
    return;
  }
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  AliInputEventHandler *handl = (AliInputEventHandler*)mgr->GetInputEventHandler();

  fHistCount->Fill(0);
  
  
 //==========MC info==========
  AliStack *stack=0x0;
  AliMCEvent *mcEvent = 0x0;
  if(fMC){//MC info and sample selection
    AliMCEventHandler* eventHandler = dynamic_cast<AliMCEventHandler*> (mgr->GetMCtruthEventHandler());
    if (!eventHandler) {
      printf("ERROR: Could not retrieve MC event handler");
      PostData(1,fOutput);
      return;
    }
    mcEvent = eventHandler->MCEvent();
    if (!mcEvent) {
      printf("ERROR: Could not retrieve MC event");
      PostData(1,fOutput);
      return;
    }
    stack = mcEvent->Stack();
    if (!stack) {
      printf("ERROR: stack not available\n");
      PostData(1,fOutput);
      return;
    }
  } // end of MC info

  
  
  //==========Trigger class==========
  UInt_t maskPhysSel = handl->IsEventSelected();
  Bool_t isSelectedCentral = (maskPhysSel & AliVEvent::kCentral);
  Bool_t isSelectedSemiCentral = (maskPhysSel & AliVEvent::kSemiCentral);
  Bool_t isSelectedMB = (maskPhysSel & AliVEvent::kMB);

 
  
  if(fTriggerConfig == 1){ //kMB + kCentral + kSemiCentral
    if(!isSelectedMB && !isSelectedCentral && !isSelectedSemiCentral){
      PostData(1,fOutput);
      return;
    }
  }else if(fTriggerConfig == 2){ //kCentral + kSemiCentral
    if(!isSelectedCentral && !isSelectedSemiCentral){
      PostData(1,fOutput);
      return;
    }
  }else if(fTriggerConfig == 3){ //kSemiCentral
    if(!isSelectedSemiCentral){
      PostData(1,fOutput);
      return;
    }
  }else if(fTriggerConfig == 4){ //kCentral
    if(!isSelectedCentral){
      PostData(1,fOutput);
      return;
    }
  }

  if(isSelectedMB) { // Minimum Bias
    fHistTrigger->Fill(0);
  }
  
  if(isSelectedCentral) { // Central
    fHistTrigger->Fill(1);
  }
  
  if(isSelectedSemiCentral) { // SemiCentral
    fHistTrigger->Fill(2);
  }
  
  
  //==========Centrality==========
  //if(fAODevent->GetEventSpecie() == 4){ // Event Specie == 4 == PbPb
    AliCentrality *centr=fAODevent->GetCentrality();
    fCentrality = centr->GetCentralityClass10("V0M");    
    fCentralityPercentile = centr->GetCentralityPercentile("V0M");
    //}
  
  fHistCentralityClass->Fill(fCentrality);
  fHistCentralityPercentile->Fill(fCentralityPercentile);

  if (fCentralityPercentile < fLowCentrality || fCentralityPercentile > fHighCentrality) {
    PostData(1,fOutput);
    return; //0 bis 80 %
  }
  
  //==========Multiplicity==========
  //Int_t refMultTpc = AliESDtrackCuts::GetReferenceMultiplicity(fAODevent, kTRUE);
  //fHistMultiplicity->Fill(refMultTpc);
  
  
  //==========Primary Vertex==========
  fPrimaryVertex = (AliAODVertex*)fAODevent->GetPrimaryVertex();
  if (TMath::Abs(fPrimaryVertex->GetZ()) > 10) {
    PostData(1,fOutput);
    return;
  }

  fHistZPrimaryVtx->Fill(fPrimaryVertex->GetZ());
  fHistXPrimaryVtx->Fill(fPrimaryVertex->GetX());
  fHistYPrimaryVtx->Fill(fPrimaryVertex->GetY());
 
  
  // -------------------------------------------------------
  // Loop for PID on ESD tracks
  // -------------------------------------------------------
  
  fPIDResponse = handl->GetPIDResponse();

  ntracks = fAODevent->GetNumberOfTracks();

  TArrayI cdeuteron(ntracks);
  UInt_t nDeuTPC = 0;
  TArrayI cproton(ntracks);
  UInt_t nProTPC = 0;
  TArrayI cpion(ntracks);
  UInt_t nPioTPC = 0;
  
  //vector <Float_t> cmassd;
  //vector <Float_t> cmassp;
    

  for(Int_t i=0; i < ntracks; i++) {
    track = dynamic_cast<AliAODTrack*>(fAODevent->GetTrack(i));
    minPos = 9999.;
    partID = -1;
    nsigma[0] = 9999.;
    nsigma[1] = 9999.;
    nsigma[2] = 9999.;

    // Chi2/TPCcls
    nClustersTPC = track->GetTPCncls();
    chi2PerClusterTPC = track->Chi2perNDF();
    fHistChi2perTPCcluster->Fill(chi2PerClusterTPC);
    
    if(fMC){if(track->GetLabel()<0) continue;}
    if(track->GetID()<0) continue;     
    if(!AcceptTrack(track)) continue;
    
    //if(!track->GetInnerParam()) continue;
  
    p = track->GetTPCmomentum(); //track
    //if(p<0.2) continue;

    fHistTPCpid->Fill(p, track->GetTPCsignal());

    // TOF signal
    mass = 0.;
    beta = 0.;  
 
      if(track->GetIntegratedLength() > 350.){
	time0 = fPIDResponse->GetTOFResponse().GetStartTime(track->P());
	time = track->GetTOFsignal() - time0;

	if(time > 0){
	  beta = (track->GetIntegratedLength()) / (2.99792457999999984e-02 * time);
	  gamma = 1/TMath::Sqrt(1-beta*beta);
	  mass = p/TMath::Sqrt(gamma*gamma - 1);

	  fHistTOFsignal->Fill(p,beta);
	} // time > 0	
      } // track->GetIntergratedLength > 350
      
    //Filling PID histo
    label = track->GetLabel();
    
    if(fMC){
      TParticle *tparticleDaughter = stack->Particle(TMath::Abs(label));
      if(track->Charge()>0){ // track sign +
	if(TMath::Abs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kDeuteron)) <= 4 && tparticleDaughter->GetPdgCode() == 1000010020){ //d
	  fHistTPCdeusignal->Fill(p, track->GetTPCsignal());
	  if(track->GetIntegratedLength() > 350.){
	    //fHistTOFdeusignal->Fill(p,beta);
	    fHistTOFdeumass->Fill(mass);
	  }
	  //cdeuteron.push_back(track);
	  //cmassd.push_back(mass); // da spostare nel if length > 350.?
	}
	else if (TMath::Abs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kProton)) <= 4 && tparticleDaughter->GetPdgCode() == 2212) { // p
	  fHistTPCprosignal->Fill(p, track->GetTPCsignal());
	  if(track->GetIntegratedLength() > 350.){
	    //fHistTOFprosignal->Fill(p,beta);
	    fHistTOFpromass->Fill(mass);
	  }
	  //cproton.push_back(track);
	  //cmassp.push_back(mass); // da spostare nel if length > 350.?
	}
	else if (tparticleDaughter->GetPdgCode() == 211) fHistTPCpionplussignal->Fill(p, track->GetTPCsignal());
      } // end of "track sign +"
      else if(track->Charge()<0){ // track sign -
	if (tparticleDaughter->GetPdgCode() == -1000010020) { //anti-d
	  fHistTPCantideusignal->Fill(p, track->GetTPCsignal());
	  //if(track->GetIntegratedLength() > 350.) fHistTOFantideusignal->Fill(p,beta);
	}
	else if (tparticleDaughter->GetPdgCode() == -2212) { //anti-p
	  fHistTPCantiprosignal->Fill(p, track->GetTPCsignal());
	  //if(track->GetIntegratedLength() > 350.) fHistTOFantiprosignal->Fill(p,beta);
	}
	else if (TMath::Abs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kPion)) <= 4 && tparticleDaughter->GetPdgCode() == -211) { // pi-
	  fHistTPCpionsignal->Fill(p, track->GetTPCsignal());
	  //cpion.push_back(track);
	}
      } // end of "track sign -"
    } // end of MC PID
    
    else {
      if(TMath::Abs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kPion)) <= 3) nsigma[0] = TMath::Abs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kPion));
      if(TMath::Abs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kProton)) <= 3) nsigma[1] = TMath::Abs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kProton));
      if(TMath::Abs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kDeuteron)) <= 3) nsigma[2] = TMath::Abs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kDeuteron));

      for(Int_t iS = 0; iS < 3; iS++){
	if(nsigma[iS] < minPos){
	  minPos = nsigma[iS];
	  partID = iS;
	}
      }
      
      
      if(track->Charge()>0){
	if(partID == 2) { //deuteron
	  fHistTPCdeusignal->Fill(p, track->GetTPCsignal());
	  if(track->GetIntegratedLength() > 350.){
	    //fHistTOFdeusignal->Fill(p,beta);
	    fHistTOFdeumass->Fill(mass);
	    //cmassd.push_back(mass);
	  }
	  cdeuteron[nDeuTPC++] = i;
	}
	
	if(partID == 1) { // proton
	  fHistTPCprosignal->Fill(p, track->GetTPCsignal());
	  if(track->GetIntegratedLength() > 350.){
	    //fHistTOFprosignal->Fill(p,beta);
	    fHistTOFpromass->Fill(mass);
	    //cmassp.push_back(mass);
	  }
	  cproton[nProTPC++] = i;
	}
	
	if(partID == 0) { //pion^+
	  fHistTPCpionplussignal->Fill(p, track->GetTPCsignal());
	  cpion[nPioTPC++] = i;
	}
	
      } // end of "track sign +"
      else if(track->Charge()<0){

	if(partID == 2) { //deuteron
	  fHistTPCantideusignal->Fill(p, track->GetTPCsignal());
	  if(track->GetIntegratedLength() > 350.){
	    //fHistTOFantideusignal->Fill(p,beta);
	    fHistTOFdeumass->Fill(mass);
	    //cmassd.push_back(mass);
	  }
	  cdeuteron[nDeuTPC++] = i;
	}
	
	if(partID == 1) { // proton
	  fHistTPCantiprosignal->Fill(p, track->GetTPCsignal());
	  if(track->GetIntegratedLength() > 350.){
	    //fHistTOFantiprosignal->Fill(p,beta);
	    fHistTOFpromass->Fill(mass);
	    //cmassp.push_back(mass);
	  }
	  cproton[nProTPC++] = i;
	}
	if(partID == 0) { //pion^-
	  fHistTPCpionsignal->Fill(p, track->GetTPCsignal());
	  cpion[nPioTPC++] = i;
	}
      }// end of "track sign -"
    } 
  } // end of PID loop
  
  cdeuteron.Set(nDeuTPC);
  cproton.Set(nProTPC);
  cpion.Set(nPioTPC);
  
  // -------------------------------------------------------
  // Loop for Invariant Mass
  // -------------------------------------------------------
  
  Double_t bz = fAODevent->GetMagneticField();
  fVertexer->SetFieldkG(bz);
  Double_t xthiss(0.0);
  Double_t xpp(0.0);
  
  AliESDVertex *decayVtx = 0x0;

  TLorentzVector Hypertriton;
  TVector3 h1, d1, p1, pi1;
  TVector3 betaHyper;
  TLorentzVector HypertritonCM;
  Double_t pTotHyper = 0.;
  Double_t pTotHyperCM=0.;
  TLorentzRotation bet;
  TLorentzVector p_dp,p_dpi;
  Int_t deuIdx, proIdx, pioIdx = 0.;
  Double_t charge_d, charge_p, charge_pi = 0.;
  AliExternalTrackParam etd, etp, etpi;
  
  for(UInt_t j=0; j<nDeuTPC; j++){ // candidate deuteron loop cdeuteron.size()
    
    trackD = dynamic_cast<AliAODTrack*>(fAODevent->GetTrack(cdeuteron[j]));
    
    trackD->PropagateToDCA(fPrimaryVertex,bz,100,dprim,dprimc); //change: GetImpactParameters --> PropagateToDCA
    dcadprim = TMath::Sqrt((dprim[0]*dprim[0])+(dprim[1]*dprim[1]));
    fHistDCAdprimary->Fill(dcadprim);
    //fHistCorrDCAdprimary->Fill(dprim[0],dprim[1]);
    
    if(dcadprim < fDCADPVmin) continue;
    
    charge_d = trackD->Charge();
    etd.CopyFromVTrack(trackD);
    
    for(UInt_t m=0; m<nProTPC; m++){ // candidate proton loop cproton.size()
          
      trackP = dynamic_cast<AliAODTrack*>(fAODevent->GetTrack(cproton[m]));
	  
      if(fMC) {if(trackD->GetLabel() == trackP->GetLabel()) continue;}

      charge_p = trackP->Charge();

      if((charge_d*charge_p)<0) continue; //avoid coupling d-pbar and dbar-p
      
      trackP->PropagateToDCA(fPrimaryVertex,bz,100,pprim,pprimc);
      dcapprim = TMath::Sqrt((pprim[0]*pprim[0])+(pprim[1]*pprim[1]));
      fHistDCApprimary->Fill(dcapprim);
      //fHistCorrDCApprimary->Fill(pprim[0],pprim[1]);

      if(dcapprim < fDCAPPVmin) continue;

      etp.CopyFromVTrack(trackP);

      dca_dp = etd.GetDCA(&etp,bz,xthiss,xpp);

      fHistDCAdeupro->Fill(dca_dp);

      if(dca_dp > fDCAdp) continue;

            
      for(UInt_t s=0; s<nPioTPC; s++ ){ // candidate pion loop cpion.size()

	fTrkArray->Clear();
	Hypertriton.Clear();
	HypertritonCM.Clear();
	h1.Clear();
	posD.Clear();
	posP.Clear();
	negPi.Clear();
	d1.Clear();
	p1.Clear();
	pi1.Clear();
	p_dp.Clear();
	p_dpi.Clear();
	
	trackNPi = dynamic_cast<AliAODTrack*>(fAODevent->GetTrack(cpion[s]));
	brotherHood = kFALSE;

	charge_pi = trackNPi->Charge();

	if((charge_p*charge_pi)>0) continue;
	
	fHistpionTPCcls->Fill(trackNPi->GetTPCncls(0));
	fHistpTpion->Fill(trackNPi->Pt());

	if(!AcceptTrack(trackNPi,211)) continue;
       

	if(fMC){
	if(trackNPi->GetLabel() == trackP->GetLabel()) continue;
	if(trackNPi->GetLabel() == trackD->GetLabel()) continue;
	}

	trackNPi->PropagateToDCA(fPrimaryVertex,bz,100,piprim,piprimc);
	
	dcapiprim = TMath::Sqrt((piprim[0]*piprim[0])+(piprim[1]*piprim[1]));
	       
	fHistDCApiprimary->Fill(dcapiprim);
	//fHistCorrDCApiprimary->Fill(piprim[0],piprim[1]);
	
	if(dcapiprim < fDCAPiPVmin) continue;

	etpi.CopyFromVTrack(trackNPi);

	//====Triplets building====
	
	dca_dpi = etpi.GetDCA(&etd,bz,xthiss,xpp);
	dca_ppi = etpi.GetDCA(&etp,bz,xthiss,xpp);


	fHistDCAdpdpi->Fill(dca_dp,dca_dpi);
	fHistDCApdppi->Fill(dca_dp,dca_ppi);
	fHistDCApidpip->Fill(dca_ppi,dca_dpi);
	
	
	if(dca_dpi > GetDCAcut(5,dca_dp)) continue;
	if(dca_ppi > GetDCAcut(4,dca_dp)) continue;

	fHistDCApiondeu->Fill(dca_dpi);
	fHistDCApionpro->Fill(dca_ppi);
	
	fTrkArray->AddAt(&etd,0);
	fTrkArray->AddAt(&etp,1);
	fTrkArray->AddAt(&etpi,2);

	//====Decay vertex and topology====

	fVertexer->SetVtxStart(fPrimaryVertex->GetX(),fPrimaryVertex->GetY(),fPrimaryVertex->GetZ());
	decayVtx = (AliESDVertex*)fVertexer->VertexForSelectedESDTracks(fTrkArray);

	SetConvertedAODVertices(decayVtx);
	
	fHistZDecayVtx->Fill(decayVtx->GetZ());
	fHistXDecayVtx->Fill(decayVtx->GetX());
	fHistYDecayVtx->Fill(decayVtx->GetY());
	decVt[0] = decayVtx->GetX();
	decVt[1] = decayVtx->GetY();
	decVt[2] = decayVtx->GetZ();
	
	dlh[0]=fAODevent->GetPrimaryVertex()->GetX() - decayVtx->GetX();
	dlh[1]=fAODevent->GetPrimaryVertex()->GetY() - decayVtx->GetY();
	dlh[2]=fAODevent->GetPrimaryVertex()->GetZ() - decayVtx->GetZ();

	decayLengthH3L = TMath::Sqrt((dlh[0]*dlh[0]) + (dlh[1]*dlh[1]) + (dlh[2]*dlh[2]));
	normalizedDecayL = fVtx2->DistanceToVertex(fPrimaryVertex)/fVtx2->ErrorDistanceToVertex(fPrimaryVertex);
	
	fHistDecayLengthH3L->Fill(decayLengthH3L);
	fHistNormalizedDecayL->Fill(normalizedDecayL);
	
	etd.PropagateToDCA(decayVtx, bz, 10,dcad);
	fHistDCAXYdeuvtx->Fill(dcad[0]);
	fHistDCAZdeuvtx->Fill(dcad[1]);
	
	etp.PropagateToDCA(decayVtx, bz, 10,dcap);
	fHistDCAXYprovtx->Fill(dcap[0]);
	fHistDCAZprovtx->Fill(dcap[1]);
	
	etpi.PropagateToDCA(decayVtx, bz, 10,dcapi);
        
	fHistDCAXYpionvtx->Fill(dcapi[0]);
	fHistDCAZpionvtx->Fill(dcapi[1]);

	delete decayVtx;
	
	if(decayLengthH3L > fMaxDecayLength || decayLengthH3L < fMinDecayLength) continue;
	if(TMath::Sqrt((dcad[0]*dcad[0])+(dcad[1]*dcad[1])) > fDCADeuSVmax) continue;
	if(TMath::Sqrt((dcap[0]*dcap[0])+(dcap[1]*dcap[1])) > fDCAProSVmax) continue;
	if(TMath::Abs(dcapi[0]) > fDCAPiSVxymax) continue;
	if(TMath::Abs(dcapi[1]) > fDCAPiSVzmax) continue;

	if(normalizedDecayL < fMinNormalizedDecL) continue;
	
	posD.SetXYZM(etd.Px(),etd.Py(),etd.Pz(),deuteronMass);
	
	posP.SetXYZM(etp.Px(),etp.Py(),etp.Pz(),protonMass);
	
	negPi.SetXYZM(etpi.Px(),etpi.Py(),etpi.Pz(),pionMass);
	
	Hypertriton=posD+posP+negPi;
	pTotHyper = Hypertriton.P();

	if(Hypertriton.Pt() < fMinPtMother || Hypertriton.Pt() > fMaxPtMother) continue;
	
	if(fSideBand == kTRUE && (Hypertriton.M() < 3.08 || Hypertriton.M() > 3.18)) continue;
	ctau = (Hypertriton.M()*decayLengthH3L)/pTotHyper;
	fHistLifetime->Fill(ctau);

	if(ctau < fMinLifeTime || ctau > fMaxLifeTime) continue;
	
	rapidity = Hypertriton.Rapidity();
	fHistHyperRapidity->Fill(rapidity);
	
	if(TMath::Abs(rapidity) > fRapidity) continue;

	h1.SetXYZ(-dlh[0],-dlh[1],-dlh[2]);
	pointingAngleH = Hypertriton.Angle(h1);

	fHistCosPointingAngle->Fill(TMath::Cos(pointingAngleH));
	if (TMath::Cos(pointingAngleH) < fCosPointingAngle) continue;

	//	p_dp = posD + posP;
	//	p_dpi = posD + negPi;
	
	//fHistDalitz_dp_dpi->Fill(p_dpi.M2(),p_dp.M2());
	
	d1.SetXYZ(etd.Px(),etd.Py(),etd.Pz());  
	p1.SetXYZ(etp.Px(),etp.Py(),etp.Pz());
	pi1.SetXYZ(etpi.Px(),etpi.Py(),etpi.Pz());
	
	//====Angular correlation====
	
	fHistAngle_deu_pro->Fill(d1.Angle(p1));
	fHistAngle_deu_pion->Fill(d1.Angle(pi1));
	fHistAngle_pro_pion->Fill(p1.Angle(pi1));
	
	fHistAngleCorr_dp_dpi->Fill(d1.Angle(p1),d1.Angle(pi1));
	fHistAngleCorr_dp_ppi->Fill(d1.Angle(p1),p1.Angle(pi1));
	fHistAngleCorr_ppi_dpi->Fill(p1.Angle(pi1),d1.Angle(pi1));
	
	//====Momentum in the CM: Boost LAB-->CM====
	
	betaHyper.SetX(-Hypertriton.Px()/TMath::Sqrt((pTotHyper*pTotHyper)+(hypertritonMass*hypertritonMass)));
	betaHyper.SetY(-Hypertriton.Py()/TMath::Sqrt((pTotHyper*pTotHyper)+(hypertritonMass*hypertritonMass)));
	betaHyper.SetZ(-Hypertriton.Pz()/TMath::Sqrt((pTotHyper*pTotHyper)+(hypertritonMass*hypertritonMass)));
	TLorentzRotation beta_3hl(betaHyper);
	posDCM = beta_3hl*posD;
	posPCM = beta_3hl*posP;
	negPiCM = beta_3hl*negPi;
	
	//Alternative way to boost in CM
	//posDCM.Clear();
	//posDCM.SetXYZM(trackD->Px(),trackD->Py(),trackD->Pz(),deuteronMass);
	//posDCM.Boost(betaHyper);
	
	HypertritonCM = posDCM + posPCM + negPiCM;
	
	pTotHyperCM = TMath::Sqrt((HypertritonCM.Px()*HypertritonCM.Px())+(HypertritonCM.Py()*HypertritonCM.Py())+(HypertritonCM.Pz()*HypertritonCM.Pz()));
	fHistDecayMomCM_X->Fill(HypertritonCM.Px());
	fHistDecayMomCM_Y->Fill(HypertritonCM.Py());
	fHistDecayMomCM_Z->Fill(HypertritonCM.Pz());
	fHistDecayMomCM_XY->Fill(HypertritonCM.Px(),HypertritonCM.Py());
	fHistDecayMomCM_XZ->Fill(HypertritonCM.Px(),HypertritonCM.Pz());
	fHistDecayMomCM_YZ->Fill(HypertritonCM.Py(),HypertritonCM.Pz());
	fHistDecayMomCM->Fill(pTotHyperCM);

	if(pTotHyperCM > fMaxPMotherCM) continue;
	
	if(d1.Angle(p1) > fAngledp) continue;
	
	if(charge_d>0 && charge_p>0 && charge_pi<0)	fHistMassHypertriton->Fill(Hypertriton.M());
	
	if(charge_d<0 && charge_p<0 && charge_pi>0)	fHistMassAntiHypertriton->Fill(Hypertriton.M());

	//====MC====
	
	if(fMC){
	  lD = trackD->GetLabel();
	  lP = trackP->GetLabel();
	  lPi = trackNPi->GetLabel();

	  TParticle *tparticleD = stack->Particle(TMath::Abs(lD));
	  labelM = tparticleD->GetFirstMother();
	  TParticle *tparticleMother = stack->Particle(TMath::Abs(labelM));
	  ndau = tparticleMother->GetNDaughters();
	  
	  if(tparticleMother->GetPdgCode() == 1010010030){
	    Int_t labelThirdDau = tparticleMother->GetLastDaughter();
	    Int_t labelFirstDau = labelThirdDau - 2;
	    Int_t labelSecondDau = labelThirdDau - 1;
	    
	    if(labelFirstDau == lD && labelSecondDau == lP && labelThirdDau == lPi){
	      //signature of dca pion-primary in case of MCtruth
	      
	      brotherHood = kTRUE;
	    }
	    else brotherHood = kFALSE;
	  }
	  else brotherHood = kFALSE;
	}
	
	/*if(fFillTree){
	  fTchi2NDFdeu = trackD->GetTPCchi2()/trackD->GetTPCclusters(0);
	  fTPCclsdeu = trackD->GetTPCclusters(0);
	  fTPCclsPIDdeu = trackD->GetTPCsignalN() ;
	  fTpTPCdeu = trackD->GetTPCmomentum();
	  fTpTdeu = trackD->Pt();
	  fTpdeu = trackD->P();
	  fTTPCnsigmadeu = fPIDResponse->NumberOfSigmasTPC(trackD,AliPID::kDeuteron);
	  //fTTOFmassdeu = cmassd.at(j) ;
	  fTDCAXYdeuprvtx = dprim[0];
	  fTDCAZdeuprvtx = dprim[1];
	  fTchi2NDFpro = trackP->GetTPCchi2()/trackP->GetTPCclusters(0);
	  fTPCclspro = trackP->GetTPCclusters(0);
	  fTPCclsPIDpro = trackP->GetTPCsignalN();
	  fTpTPCpro = trackP->GetTPCmomentum();
	  fTpTpro = trackP->Pt();
	  fTppro = trackP->P();
	  fTTPCnsigmapro = fPIDResponse->NumberOfSigmasTPC(trackP,AliPID::kProton);
	  //fTTOFmasspro = cmassp.at(m);
	  fTDCAXYproprvtx = pprim[0];
	  fTDCAZproprvtx = pprim[1];
	  fTchi2NDFpion = trackNPi->GetTPCchi2()/trackNPi->GetTPCclusters(0);
	  fTPCclspion = trackNPi->GetTPCclusters(0);
	  fTPCclsPIDpion = trackNPi->GetTPCsignalN();
	  fTpTPCpion = trackNPi->GetTPCmomentum();
	  fTpTpion = trackNPi->Pt();
	  fTppion = trackNPi->P();
	  fTTPCnsigmapion = fPIDResponse->NumberOfSigmasTPC(trackNPi,AliPID::kPion);
	  fTDCAXYpioprvtx = piprim[0];
	  fTDCAZpioprvtx = piprim[1];
	  fTDCAdp = dca_dp;
	  fTDCAdpi = dca_dpi;
	  fTDCAppi = dca_ppi;
	  fTDCAXYdvtx = dcad[0];
	  fTDCAZdvtx = dcad[1];
	  fTDCAXYpvtx = dcap[0];
	  fTDCAZpvtx = dcap[1];
	  fTDCAXYpivtx = dcapi[0];
	  fTDCAZpivtx = dcapi[1];
	  fTDecayLength = decayLengthH3L;
	  fTCosPA = TMath::Cos(pointingAngleH);
	  fTInvariantMass = Hypertriton.M();

	  fTTree->Fill();
	  PostData(2,fTTree);
	} //end of Fill Tree
	*/
	//Pure MC part - Hypertriton truth

	if(fMC){
	  if(brotherHood) {// H3L --> d+p+pi
	    fHistpionTPCclsMCt->Fill(trackNPi->GetTPCncls(0));
	    fHistpTpionMCt->Fill(trackNPi->Pt());
	    fHistpTproMCt->Fill(trackP->Pt());
	    fHistpTdeuMCt->Fill(trackD->Pt());
	    fHistDCAdeuproMCt->Fill(dca_dp);
	    fHistDCApiondeuMCt->Fill(dca_dpi);
	    fHistDCApionproMCt->Fill(dca_ppi);
	    fHistCorrDCApiprimaryMCt->Fill(piprim[0],piprim[1]);
	    fHistCorrDCApprimaryMCt->Fill(pprim[0],pprim[1]);
	    fHistCorrDCAdprimaryMCt->Fill(dprim[0],dprim[1]);
	    fHistDCApiprimaryMCt->Fill(dcapiprim);
	    fHistDCApprimaryMCt->Fill(dcapprim);
	    fHistDCAdprimaryMCt->Fill(dcadprim);
	    fHistDCAXYdeuvtxMCt->Fill(dcad[0]);
	    fHistDCAZdeuvtxMCt->Fill(dcad[1]);
	    fHistDCAXYprovtxMCt->Fill(dcap[0]);
	    fHistDCAZprovtxMCt->Fill(dcap[1]);
	    fHistDCAXYpionvtxMCt->Fill(dcapi[0]);
	    fHistDCAZpionvtxMCt->Fill(dcapi[1]);
	    fHistZDecayVtxMCt->Fill(decVt[2]);
	    fHistXDecayVtxMCt->Fill(decVt[0]);
	    fHistYDecayVtxMCt->Fill(decVt[1]);
	    fHistMassHypertritonMCt->Fill(Hypertriton.M());
	  }
	} // end of Pure MC part
      } // end of candidate pion loop
    } // end of candidate proton loop
  }// end of candidate deuteron loop



  if(fMC){
    for(Int_t iMC=0; iMC<stack->GetNtrack(); iMC++){ // check MC stack content
      TParticle *pstack = stack->Particle(iMC);
      if(pstack->GetPdgCode() == 11) fHistParticle->Fill(0); // e-
      if(pstack->GetPdgCode() == -11) fHistParticle->Fill(1); // e+
      if(pstack->GetPdgCode() == 211) fHistParticle->Fill(2); // pi+
      if(pstack->GetPdgCode() == -211) fHistParticle->Fill(3); // pi-
      if(pstack->GetPdgCode() == 321) fHistParticle->Fill(4); // k+
      if(pstack->GetPdgCode() == -321) fHistParticle->Fill(5); // k-
      if(pstack->GetPdgCode() == 2212) fHistParticle->Fill(6); // p
      if(pstack->GetPdgCode() == -2212) fHistParticle->Fill(7); // pbar
      if(pstack->GetPdgCode() == 1000010020) fHistParticle->Fill(8); // d
      if(pstack->GetPdgCode() == -1000010020) fHistParticle->Fill(9); // dbar
      if(pstack->GetPdgCode() == 1000010030) fHistParticle->Fill(10); // t
      if(pstack->GetPdgCode() == -1000010030) fHistParticle->Fill(11); // tbar
      if(pstack->GetPdgCode() == 1000020030) fHistParticle->Fill(12); // He3
      if(pstack->GetPdgCode() == -1000020030) fHistParticle->Fill(13); // He3bar
      if(pstack->GetPdgCode() == 1000020040) fHistParticle->Fill(14); // He4
      if(pstack->GetPdgCode() == -1000020040) fHistParticle->Fill(15); // He4bar
      if(pstack->GetPdgCode() == 1010010030) fHistParticle->Fill(16); // H3L
      if(pstack->GetPdgCode() == -1010010030) fHistParticle->Fill(17); // H3Lbar
      if(pstack->GetPdgCode() == 1010010030 && pstack->GetNDaughters() == 2) fHistParticle->Fill(18); // H3L
      if(pstack->GetPdgCode() == -1010010030 && pstack->GetNDaughters() == 2) fHistParticle->Fill(19); // H3Lbar
      if(pstack->GetPdgCode() == 3122 && pstack->GetNDaughters() == 2) fHistParticle->Fill(20); // Lambda
    } //end of MC content truth check
  }
   
  cdeuteron.Reset();
  cproton.Reset();
  cpion.Reset();

  //cmassd.clear();
  //cmassp.clear();
  
  // Post output data.
  PostData(1,fOutput);
  if(fFillTree) PostData(2,fTTree);
  
  
  
} // end of UserExec

//________________________________________________________________________
void AliAnalysisTaskHypertriton3AOD::Terminate(Option_t *){
  // Merge output
  // Called once at the end of the query

  fOutput = dynamic_cast<TList*>(GetOutputData(1));
  if (!fOutput) {
    printf("ERROR: fOutput not available\n");
    return;
  }

  printf("end of Terminate");
  return;

} // end of Terminate

//________________________________________________________________________
Bool_t AliAnalysisTaskHypertriton3AOD::AcceptTrack(AliAODTrack *t, Int_t part){
  ULong_t status = t->GetStatus();
  if(t->GetTPCncls() < fRequireMinTPCcls) return kFALSE;
  if(t->GetTPCsignalN() < fRequireMinTPCclsSignal) return kFALSE;
  if(t->Chi2perNDF() > fRequireMaxChi2perNDF) return kFALSE;
  if(t->Eta() < fRequireEtaMin || t->Eta() > fRequireEtaMax) return kFALSE;
  if(!(status&AliVTrack::kTPCrefit) && fRequireTPCrefit) return kFALSE;
  AliAODVertex *vert = (AliAODVertex*)t->GetProdVertex();
  if(Int_t(vert->GetType()) == AliAODVertex::kKink && fRequireNoKinks) return kFALSE;

  if(TMath::Abs(part) == 211){
    if(t->GetTPCncls() < fRequireMinPionTPCcls) return kFALSE;
    if(t->Pt() < fMinPionPt || t->Pt() > fMaxPionPt) return kFALSE;
  }

  return kTRUE;
}
