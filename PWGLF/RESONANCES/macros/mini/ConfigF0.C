/***************************************************************************
fbellini@cern.ch - created on 25/08/2017
Configuration script for f0(980) analysis
****************************************************************************/

Bool_t ConfigF0(AliRsnMiniAnalysisTask *task, 
		Bool_t                 isMC, 
		AliPIDResponse::EBeamType collSys = AliPIDResponse::kPP, //=0, kPPB=1, kPBPB=2
		AliRsnCutSet           *cutsPair,             //cuts on the pair
		Bool_t                 enaMultSel = kTRUE,    //enable multiplicity axis
      		Float_t                masslow = 0.6,         //inv mass axis low edge 
		Float_t                massup = 1.2,          //inv mass axis upper edge 
		Int_t                  nbins = 600,           //inv mass axis n bins
		Int_t                  aodFilterBit = 5,      //filter bit for AOD analysis
		AliRsnCutSetDaughterParticle::ERsnDaughterCutSet cutPid = AliRsnCutSetDaughterParticle::kTPCpidTOFveto3s, // pid cut set
		Float_t                nsigma = 3.0,          //nsigma of TPC PID cut
		Bool_t                 enableMonitor = kTRUE) //enable single track QA plots
{
  //-----------------------
  //General 
  //-----------------------
  TString partname="f0";
  Int_t   pdgCode=9010221;
  Float_t mass = 0.990;// 2014 PDG: M = 0.990 ± 20 GeV
  RSNPID  d1 = AliRsnDaughter::kPion;
  RSNPID  d2 = AliRsnDaughter::kPion;

  //Additional options for monitoring plots
  TString monitorOpt = "NoSIGN";
  
  //-----------------------
  // CUTS
  //-----------------------
  //use default quality cuts std 2010 with crossed rows TPC
  Bool_t useCrossedRows = 1; 
  AliRsnCutSetDaughterParticle * cutSetPi = new AliRsnCutSetDaughterParticle("cutPi", cutPid, AliPID::kPion, nsigma, aodFilterBit, useCrossedRows);
  Int_t icutPi = task->AddTrackCuts(cutSetPi);

  //set daughter cuts
  Int_t icut1 = icutPi;
  Int_t icut2 = icutPi;

  //monitor single-track selection based on track quality cuts only
  AliRsnCutSetDaughterParticle * cutSetQuality = new AliRsnCutSetDaughterParticle("cutQuality", AliRsnCutSetDaughterParticle::kQualityStd2011, AliPID::kPion, 10.0, aodFilterBit, useCrossedRows);
  Int_t icutQuality = task->AddTrackCuts(cutSetQuality);
 
  //QA plots 
  if (enableMonitor){
    Printf("======== Cut monitoring enabled");
    gROOT->LoadMacro("$ALICE_PHYSICS/PWGLF/RESONANCES/macros/mini/AddMonitorOutput.C");
    AddMonitorOutput(isMC, cutSetQuality->GetMonitorOutput(), monitorOpt.Data());
    AddMonitorOutput(isMC, cutSetPi->GetMonitorOutput(), monitorOpt.Data());
  }  

  //-----------------------
  // OUTPUT
  //-----------------------
  /* invariant mass   */ Int_t imID   = task->CreateValue(AliRsnMiniValue::kInvMass, kFALSE);
  /* IM resolution    */ Int_t resID  = task->CreateValue(AliRsnMiniValue::kInvMassRes, kTRUE);
  /* transv. momentum */ Int_t ptID   = task->CreateValue(AliRsnMiniValue::kPt, kFALSE);
  /* centrality       */ Int_t multID = task->CreateValue(AliRsnMiniValue::kMult, kFALSE);
  /* pseudorapidity   */ Int_t etaID  = task->CreateValue(AliRsnMiniValue::kEta, kFALSE);
  /* rapidity         */ Int_t yID    = task->CreateValue(AliRsnMiniValue::kY, kFALSE);
  /* 1st daughter pt  */ Int_t fdpt   = task->CreateValue(AliRsnMiniValue::kFirstDaughterPt, kFALSE);
  /* 2nd daughter pt  */ Int_t sdpt   = task->CreateValue(AliRsnMiniValue::kSecondDaughterPt, kFALSE);
  /* 1st daughter p   */ Int_t fdp    = task->CreateValue(AliRsnMiniValue::kFirstDaughterP, kFALSE);
  /* 2nd daughter p   */ Int_t sdp    = task->CreateValue(AliRsnMiniValue::kSecondDaughterP, kFALSE);
  TString output = "HIST"; // or "SPARSE"
  TString name[6] = {"UnlikePM", "MixingPM", "LikePP", "LikeMM", "MixingPP", "MixingMM"};
  TString comp[6] = {"PAIR"    , "MIX",      "PAIR"  , "PAIR"  , "MIX"     , "MIX"     };
  Char_t charge1[6] = {'+', '+', '+', '-', '+', '-'};
  Char_t charge2[6] = {'-', '-', '+', '-', '+', '-'};

  //DATA 
  for (Int_t i = 0; i < 6; i++) {
    AliRsnMiniOutput *out = task->CreateOutput(Form("f0_%s", name[i].Data()), output.Data(), comp[i].Data());
    out->SetCutID(0, icut1);
    out->SetCutID(1, icut2);
    out->SetDaughter(0, d1);
    out->SetDaughter(1, d2);
    out->SetCharge(0, charge1[i]);
    out->SetCharge(1, charge2[i]);
    out->SetMotherPDG(pdgCode);
    out->SetMotherMass(mass);
    out->SetPairCuts(cutsPair);
    // axis X: invmass 
    out->AddAxis(imID, nbins, masslow, massup);
    //axis Y: mother pt
    out->AddAxis(ptID, 200, 0.0, 20.0); //default use mother pt
    //axis Z: multiplicity
    if (enaMultSel) out->AddAxis(multID, 100, 0.0, 100.0);
  }
  

  //Template for BG
  TString bgTemplate[6]  = {"rho", "omega", "Kstar", "antiKstar", "K0s", "phi"};
  Char_t bgTemplateC1[6] = {'+', '+', '+', '+', '+', '+'};
  Char_t bgTemplateC2[6] = {'-', '-', '-', '-', '-', '-'};
  Int_t bgTemplatePDG[6] = {113, 223, 313, -313, 310, 333};
  Int_t bgTemplateM[6]   = {775.26, 8.49, 891.66, 891.66, 497.611, 1019.461};
  RSNPID bgID1[6] = {AliRsnDaughter::kPion, AliRsnDaughter::kPion,AliRsnDaughter::kKaon, AliRsnDaughter::kPion, AliRsnDaughter::kPion, AliRsnDaughter::kKaon};
  RSNPID bgID2[6] = {AliRsnDaughter::kPion, AliRsnDaughter::kPion,AliRsnDaughter::kPion, AliRsnDaughter::kKaon, AliRsnDaughter::kPion, AliRsnDaughter::kKaon};

  if (isMC) {
    //TRUE RECO PAIRS - TEMPLATE FOR BG
    for (Int_t ibg = 0; ibg <6; ibg++) {
      AliRsnMiniOutput * outtempl = task->CreateOutput(Form("bg_%s", bgTemplate[ibg].Data()), output.Data(),"TRUE");
      outtempl->SetCutID(0, icut1);
      outtempl->SetCutID(1, icut2);
      outtempl->SetCharge(0, bgTemplateC1[0]);
      outtempl->SetCharge(1, bgTemplateC2[0]);
      outtempl->SetDaughter(0, bgID1[ibg]);
      outtempl->SetDaughter(1, bgID2[ibg]);
      outtempl->SetMotherPDG(bgTemplatePDG[ibg]);
      outtempl->SetMotherMass(bgTemplateM[ibg]);
      outtempl->SetPairCuts(cutsPair);
      // axis X: invmass 
      outtempl->AddAxis(imID, nbins, masslow, massup);
      //axis Y: mother pt
      outtempl->AddAxis(ptID, 200, 0.0, 20.0); //default use mother pt
      // axis Z: multrality-multiplicity
      if (enaMultSel) outtempl->AddAxis(multID, 100, 0.0, 100.0);
    }
    //TRUE RECO PAIRS - MASS
    AliRsnMiniOutput * outtrue = task->CreateOutput(Form("truef0_%s", partname.Data()), output.Data(),"TRUE");
    outtrue->SetCutID(0, icut1);
    outtrue->SetCutID(1, icut2);
    outtrue->SetCharge(0, charge1[0]);
    outtrue->SetCharge(1, charge2[0]);
    outtrue->SetDaughter(0, d1);
    outtrue->SetDaughter(1, d2);
    outtrue->SetMotherPDG(pdgCode);
    outtrue->SetMotherMass(mass);
    outtrue->SetPairCuts(cutsPair);
    // axis X: invmass 
    outtrue->AddAxis(imID, nbins, masslow, massup);
    //axis Y: mother pt
    outtrue->AddAxis(ptID, 200, 0.0, 20.0); //default use mother pt
    // axis Z: multiplicity
    if (enaMultSel) outtrue->AddAxis(multID, 100, 0.0, 100.0);

    
    //TRUE RECO PAIRS - MASS RESOLUTION
    AliRsnMiniOutput * outres = task->CreateOutput(Form("Mres_%s", partname.Data()), output.Data(),"TRUE");
    outres->SetCutID(0, icut1);
    outres->SetCutID(1, icut2);
    outres->SetCharge(0, charge1[0]);
    outres->SetCharge(1, charge2[0])
    outres->SetDaughter(0, d1);
    outres->SetDaughter(1, d2);
    outres->SetMotherPDG(pdgCode);
    outres->SetMotherMass(mass);
    outres->SetPairCuts(cutsPair);
    // axis X: invmass resolution
    outres->AddAxis(resID, 200, -0.01, 0.01);
    //axis Y: mother pt
    outres->AddAxis(ptID, 200, 0.0, 20.0);
    // axis Z: multiplicity
    if (enaMultSel) outres->AddAxis(multID, 100, 0.0, 100.0);
    
    //GENERATED PAIRS
    AliRsnMiniOutput * outm = task->CreateOutput(Form("motherf0_%s", partname.Data()), output.Data(),"MOTHER");
    outm->SetDaughter(0, d1);
    outm->SetDaughter(1, d2);
    outm->SetMotherPDG(pdgCode);
    outm->SetMotherMass(mass);
    outm->SetPairCuts(cutsPair);
    outm->AddAxis(imID, nbins, masslow, massup);
    outm->AddAxis(ptID, 200, 0.0, 20.0);
    if (enaMultSel) outm->AddAxis(multID, 100, 0.0, 100.0);
  }

  return kTRUE;
}
