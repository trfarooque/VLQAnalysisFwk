#include "VLQAnalysis/VLQ_OutputData.h"
#include "VLQAnalysis/VLQ_Options.h"
#include "VLQAnalysis/VLQ_ResonanceMaker.h"

#include "IFAETopFramework/AnalysisObject.h"
#include "IFAETopFramework/AnalysisUtils.h"
#include <iostream>

const int VLQ_ResonanceMaker::MW_CONSTRAINT = 0x1 << 0;
const int VLQ_ResonanceMaker::MTOP_CONSTRAINT = 0x1 << 1;
const int VLQ_ResonanceMaker::COLLINEAR = 0x1 << 2;
const int VLQ_ResonanceMaker::VANILLA = 0x1 << 3;

const int VLQ_ResonanceMaker::USE_LIGHT = 0x1 << 0;
const int VLQ_ResonanceMaker::PREF_BTAG = 0x1 << 1;
const int VLQ_ResonanceMaker::VETO_RCMATCH = 0x1 << 2;


const double VLQ_ResonanceMaker::MW = 80.4;

VLQ_ResonanceMaker::VLQ_ResonanceMaker( VLQ_Options *opt, VLQ_OutputData *outData /*, const VLQ_NtupleData *ntupData*/ ) : 
  m_opt(opt),
  m_outData(outData)
{ }


VLQ_ResonanceMaker::VLQ_ResonanceMaker( const VLQ_ResonanceMaker & q){

  m_opt     = q.m_opt;
  m_outData = q.m_outData;
}

VLQ_ResonanceMaker::~VLQ_ResonanceMaker(){}

int VLQ_ResonanceMaker::MakeLepW( const int mode ){

  if(!m_outData -> o_selLep){ std::cerr << "Cannot reconstruct leptonic W in the absence of a selected lepton "<< std::endl; return -1; }
  if(!m_outData -> o_AO_met){ std::cerr << "Cannot reconstruct leptonic W in the absence of the MET vector "<< std::endl; return -1; }

  double dphi_lmet = m_outData -> o_AO_met -> DeltaPhi( *(m_outData -> o_selLep) );

  double lep_pt = m_outData -> o_selLep -> Pt();
  double lep_eta = m_outData -> o_selLep -> Eta();
  double lep_phi = m_outData -> o_selLep -> Phi();

  double met_pt = m_outData -> o_AO_met -> Pt();
  double met_phi = m_outData -> o_AO_met -> Phi();

  double phi_nu = -10.; double eta_nu = -10.; double pT_nu = -10.;

  double minv_W = 0.;

  int n_soln = -1;

  if(mode & VANILLA){
    double discr = MW*MW/(2*lep_pt*met_pt) + cos(lep_phi-met_phi);

    if(discr < 1){
      pT_nu = MW*MW / (2*lep_pt*(1-cos(lep_phi-met_phi)));
      discr = 1;
      n_soln = 0;
    }
    else{
      pT_nu = met_pt;
    }

    phi_nu = met_phi;

    double deta = acosh( discr );
    double eta_nu_1 = lep_eta + deta; 
    double eta_nu_2 = lep_eta - deta; 

    if( (n_soln < 0) ){
      if(deta < 1.e-10) n_soln = 1;
      else n_soln = 2;
    }
    //eta_nu = (fabs(eta_nu_1)>fabs(eta_nu_2)) ? eta_nu_1 : eta_nu_2; 
    eta_nu = (fabs(eta_nu_1)<fabs(eta_nu_2)) ? eta_nu_1 : eta_nu_2; 

  }

  else{

    n_soln = 1;

    if(mode & COLLINEAR){
      phi_nu = lep_phi;
      eta_nu = lep_eta;
      pT_nu = met_pt * cos(dphi_lmet);
    }
    if(mode & MW_CONSTRAINT){

      //start from collinear assumption
      phi_nu = lep_phi;
      eta_nu = lep_eta; //100.;//lep_eta;

      TLorentzVector cur_nu;
      for( double dphi=-1.0; fabs(dphi) <= 1.0; dphi+=0.1 ){

	double cur_phi_nu = lep_phi + dphi;
	double cur_pT_nu = met_pt*cos(met_phi - phi_nu);

	for( double deta=-1.0; fabs(deta) <= 1.0; deta+=0.1 ){


	  /*
	    If assuming collinearity in eta
	    //double cur_minv_W = sqrt( 2*lep_pt*cur_pT_nu*(1-cos(lep_phi - cur_phi_nu)) );

	    */

	  double cur_eta_nu = lep_eta + deta;
	  cur_nu.SetPtEtaPhiM(cur_pT_nu, cur_eta_nu, cur_phi_nu, 0.);
	  TLorentzVector cur_W = cur_nu + *(m_outData->o_selLep);

	  double cur_minv_W = cur_W.M();
	  if( (pT_nu < 0.) || (fabs(MW - cur_minv_W) < fabs(MW - minv_W)) ){
	    minv_W = cur_minv_W;
	    phi_nu = cur_phi_nu;
	    eta_nu = cur_eta_nu;
	    pT_nu = cur_pT_nu;
	  }
	}//eta-scan

	/*
	  double cur_eta_nu = lep_eta - acosh( MW*MW/(2*lep_pt*cur_pT_nu) + cos(lep_phi - cur_phi_nu) );
	  if( fabs(cur_eta_nu-lep_eta) < fabs(eta_nu-lep_eta) ){
	  phi_nu = cur_phi_nu;
	  pT_nu = cur_pT_nu;
	  eta_nu = cur_eta_nu;
	  }
	*/
      }//phi-scan
      /*    
	    if(pT_nu < 0.){ 
	    eta_nu = lep_eta;
	    for( double dphi=-1.0; fabs(dphi) <= 1.0; dphi+=0.1 ){
	    double cur_phi_nu = lep_phi + dphi;
	    double cur_pT_nu = met_pt*cos(met_phi - phi_nu);

	    double cur_minv_W = sqrt( 2*lep_pt*cur_pT_nu*(1-cos(lep_phi - cur_phi_nu)) );
	    if( fabs(MW - cur_minv_W) < fabs(MW - minv_W) ){
	    minv_W = cur_minv_W;
	    phi_nu = cur_phi_nu;
	    pT_nu = cur_pT_nu;
	    }

	    }

	    }// if solution not found with first method
      */

      // 1. assume collinearity in z, then, scan phi until mass is closest to MW

      // 2. no assumption on eta. scan phi, for each phi, find eta that gives mass = MW
 

      /*
	if(mode & MTOP_CONSTRAINT){
	//theta_nu = theta_l + dz
	//p_z^nu = p_T^{nu}.tan(theta_l  + dz) \simeq p_T^nu.tan(theta_l).dz
	// ||p_nu + pl + pb|| = m_top
	//NOT YET IMPLEMENTED 
	}
	else{
	eta_nu = lep_eta;
	pT_nu = (MW*MW)/(2*lep_pt);
	phi_nu = ((pT_nu/met_pt) - cos(dphi_lmet))/sin(dphi_lmet);
	}
      */
    }
  }//if not vanilla

  //
  m_outData -> o_nu = new AnalysisObject();
  m_outData -> o_nu -> SetPtEtaPhiM(0.,0.,0.,0.);

  m_outData -> o_nu -> SetPtEtaPhiM(pT_nu, eta_nu, phi_nu, 0.);
  m_outData -> o_nu -> SetMoment ( "N_soln", n_soln);
  m_outData -> o_nu -> SetMoment ( "fracMET", m_outData -> o_nu -> Pt() / m_outData-> o_AO_met -> Pt());
  m_outData -> o_nu -> SetMoment ( "dphi_MET", fabs(m_outData -> o_nu -> DeltaPhi( *(m_outData-> o_AO_met) )) );

  m_outData -> o_lepW = new AnalysisObject();
  m_outData -> o_lepW -> SetPtEtaPhiM(0.,0.,0.,0.);

  m_outData -> o_lepW -> SetPtEtaPhiM(0.,0.,0.,0.);
  *(m_outData -> o_lepW) += *(m_outData -> o_selLep);
  *(m_outData -> o_lepW) += *(m_outData -> o_nu);

  return 0;

}

int VLQ_ResonanceMaker::MakeLeptop(const int mode, const double drmax){

  if(!m_outData -> o_lepW){ std::cerr << "Cannot make leptonic top in the absence of a leptonic W" << std::endl; return -1; }

  double drmin = drmax; AnalysisObject* sel_jet = NULL; 
  for( AnalysisObject* jet : *(m_outData -> o_jets) ){

    // Discard any jets in the cone of a tagged reclustered jet
    if( (mode & VETO_RCMATCH) && (jet -> GetMoment("RCtag_match") > 0 ) ) continue;
    if( !(mode & USE_LIGHT) || (mode & PREF_BTAG) ){
      if(jet -> GetMoment("bjet") < 1) continue;
    } // Require b-tagging on the first pass

    double drcur = (m_outData -> o_lepW)->DeltaR( *jet );
    if(drcur < drmin){ 
      drmin = drcur; 
      sel_jet = jet; 
    }
  }

  if( !sel_jet && (mode & USE_LIGHT) && !(mode &PREF_BTAG) ){

    for( AnalysisObject* jet : *(m_outData -> o_jets) ){

      // Discard any jets in the cone of a tagged reclustered jet
      if( (mode & VETO_RCMATCH) && (jet -> GetMoment("RCtag_match") > 0) ) continue;
      if(jet -> GetMoment("bjet") >= 1) continue; //bjets have been considered already

      double drcur = (m_outData -> o_lepW)->DeltaR( *jet );
      if(drcur < drmin){ 
	drmin = drcur; 
	sel_jet = jet; 
      }
    }

  }// rerun

  if(!sel_jet){ return 1; }

  m_outData -> o_leptop = new AnalysisObject();
  m_outData -> o_leptop -> SetPtEtaPhiM(0.,0.,0.,0.);

  m_outData -> o_leptop -> SetPtEtaPhiM(0.,0.,0.,0.);
  *(m_outData -> o_leptop) += ( *(m_outData -> o_lepW) ); 
  *(m_outData -> o_leptop) += ( *sel_jet ); 

  m_outData -> o_leptop -> SetMoment("dR_blep", drmin);
  m_outData -> o_leptop -> SetMoment("dR_bW", (m_outData -> o_lepW)->DeltaR(*sel_jet));
  m_outData -> o_leptop -> SetMoment("dPhi_bnu", fabs((m_outData ->o_nu)->DeltaPhi(*sel_jet)));

  m_outData -> o_leptop_b = sel_jet;

  return 0;
}


int VLQ_ResonanceMaker::MakeSemiBoostedHadtop( const double mmin, const double mmax, const double drmax ){

  //Loop over RCMV jets
  //Loop over b-tagged jets
  //Put them together to make hadronic tops

  AOVector b_cands{};
  for( AnalysisObject* jet : *(m_outData -> o_jets) ){
    jet -> SetMoment("hadtop_b", 0); //Start with default false

    // Discard any jets in the cone of a tagged reclustered jet
    if( jet -> GetMoment("RCtag_match") > 0 ) continue;
      
    // Discard the jet tagged as the b-jet from the leptonic top
    if( jet == m_outData -> o_leptop_b ) continue;

    // For now, discard light jets
    if( jet -> GetMoment("bjet") < 1 ) continue;

    b_cands.push_back(jet);

  }

  AOVector* W_cands = m_outData -> o_taggedjets . at("RCMV");

  for( AnalysisObject* Wjet : *W_cands ){

    double drmin = drmax; 
    AnalysisObject* paired_b = NULL;

    for( AnalysisObject* jet : b_cands ){

      // Discard any jets already in a hadtop
      if( jet -> GetMoment("hadtop_b") > 0 ) continue;

      // Only consider hadtop candidates within mass window
      //double minv = (*Wjet + *jet).M();
      //if(minv < mmin || minv > mmax) continue;

      double drcur = Wjet -> DeltaR( *jet );
      if( drcur < drmin ){
	if(paired_b) paired_b -> UpdateMoment("hadtop_b", 0); //no longer the match
	drmin = drcur; paired_b = jet;
	jet -> UpdateMoment("hadtop_b", 1); // new match
      }
    } // Loop over small-R jets
    if(paired_b){
      AnalysisObject* bW = new AnalysisObject();
      bW->SetPtEtaPhiM(0.,0.,0.,0.);
      *(bW) += *Wjet;
      *(bW) += *paired_b;
      bW ->SetMoment("dR_bW", drmin);

      m_outData -> o_bW_hadtop -> push_back(bW);

    }//if match for b-jet found

  }// Loop over RCMV jets

  b_cands.clear();

  return 0;

}

int VLQ_ResonanceMaker::MakeSingleVLQ( const std::string& decay ){

  // For now, do not consider bW hadronic tops 

  AOVector top_cands {};
  AOVector b_cands {};
  AOVector W_cands {};

  if(decay != "Wb"){
    for( AnalysisObject* hadtop : *(m_outData -> o_taggedjets . at("RCMTop")) ){ top_cands.push_back(hadtop); }
    if(m_outData -> o_leptop){ top_cands.push_back(m_outData -> o_leptop); } 
  }
  else{

    for( AnalysisObject* bjet : *(m_outData -> o_bjets) ){ 

       // Discard any jets in the cone of a tagged reclustered jet
      if( bjet -> GetMoment("RCtag_match") > 0 ) continue;
      
      // Discard the jet tagged as the b-jet from the leptonic top
      if( bjet == m_outData -> o_leptop_b ) continue;

      // Discard any jet tagged as b-jet from hadronic top
      if( bjet -> GetMoment("hadtop_b") > 0 ) continue;

      b_cands.push_back(bjet);
    }

    for( AnalysisObject* hadW : *(m_outData -> o_taggedjets . at("RCMV")) ){ W_cands.push_back(hadW); }
    //if(m_outData -> o_lepW && !(m_outData -> o_leptop) ){ W_cands.push_back(m_outData -> o_lepW); }

  }

  std::vector<std::pair<int, double> > pair_pt;
  std::vector<std::pair<AnalysisObject*, AnalysisObject*> > pair_res;

  AOVector& res1_cands = (decay == "Wb") ? W_cands : 
    ( (decay == "Zt") ? *(m_outData -> o_taggedjets . at("RCMV")) : 
      *(m_outData -> o_taggedjets . at("RCMHiggs")) );

  AOVector& res2_cands = (decay == "Ht" || decay == "Zt") ? top_cands : b_cands;

  // Cannot reconstruct VLQ if either collection is empty
  if( (res1_cands.size() == 0) || (res2_cands.size() == 0) ) return 0; 

  AOVector* reco_collection = m_outData -> o_recoVLQ.at(decay);

  int index = 0;

  for(AnalysisObject* res1: res1_cands){

    for(AnalysisObject* res2: res2_cands){
      double ptcur = res1->Pt() + res2->Pt();

      res1 -> SetMoment("VLQchild", 0);
      res2 -> SetMoment("VLQchild", 0);

      pair_pt.push_back(std::pair<int,double>(index,ptcur));
      pair_res.push_back( std::pair<AnalysisObject*, AnalysisObject*>(res1, res2) );
      index++;

    }

  }
  if(pair_pt.size() == 0) return 0;

  AnalysisUtils::SortVectorPairs(pair_pt, -1, true);

  for(const std::pair<int, double>&  ppt : pair_pt){

    AnalysisObject* res1 = pair_res.at(ppt.first).first;
    AnalysisObject* res2 = pair_res.at(ppt.first).second;

    if( (res1->GetMoment("VLQchild") > 0) || (res2->GetMoment("VLQchild") > 0) ) continue;

    AnalysisObject* recoVLQ = new AnalysisObject();
    recoVLQ->SetPtEtaPhiM(0.,0.,0.,0.);
    *recoVLQ = (*res1) + (*res2);
    /*
    recoVLQ->SetMoment("dR12", pdr.second);
    recoVLQ->SetMoment("dPhi12", res1->DeltaPhi(*res2));
    recoVLQ->SetMoment("dEta12", fabs(res1->Eta() - res2->Eta()));
    */
    recoVLQ->SetMoment("dR12", res1->DeltaR(*res2));
    recoVLQ->SetMoment("dPhi12", res1->DeltaPhi(*res2));
    recoVLQ->SetMoment("dEta12", fabs(res1->Eta() - res2->Eta()));
    recoVLQ->SetMoment("m1", res1->M());
    recoVLQ->SetMoment("m2", res2->M());
    recoVLQ->SetMoment("redM", recoVLQ->M() - res1->M() - res2->M());
    if(decay == "Ht"){
      recoVLQ->SetMoment("redM2", recoVLQ->M() - fabs(res1->M()-125.18) - fabs(res2->M()-172.84));
    }
    if(decay == "Zt"){
      recoVLQ->SetMoment("redM2", recoVLQ->M() - fabs(res1->M()-91.1876) - fabs(res2->M()-172.84));
    }
    if(decay == "Wb"){
      recoVLQ->SetMoment("redM2", recoVLQ->M() - fabs(res1->M()-80.370) - fabs(res2->M()-4.18));
    }

    double fpt = (decay=="Wb") ? res2->Pt()/res1->Pt() : res1->Pt()/res2->Pt();
    recoVLQ -> SetMoment("fpT12", fpt);

    res1->UpdateMoment("VLQchild", 1);
    res2->UpdateMoment("VLQchild", 1);

    recoVLQ -> SetMoment("nmatch_truth", 0.);
    recoVLQ -> SetMoment("pdgId_truth", 0.);

    reco_collection -> push_back(recoVLQ);
    m_outData -> o_recoVLQ_n.at(decay)++;
  }

  top_cands.clear(); W_cands.clear(); b_cands.clear();

  return 0;
}

int VLQ_ResonanceMaker::MakePairVLQ( const std::string& decay, const double drmax ){

  // For now, do not consider bW hadronic tops 

  AOVector top_cands {};
  AOVector b_cands {};
  AOVector W_cands {};

  if(decay != "Wb"){
    for( AnalysisObject* hadtop : *(m_outData -> o_taggedjets . at("RCMTop")) ){ top_cands.push_back(hadtop); }
    if(m_outData -> o_leptop){ top_cands.push_back(m_outData -> o_leptop); } 
  }
  else{

    for( AnalysisObject* bjet : *(m_outData -> o_bjets) ){ 

       // Discard any jets in the cone of a tagged reclustered jet
      if( bjet -> GetMoment("RCtag_match") > 0 ) continue;
      
      // Discard the jet tagged as the b-jet from the leptonic top
      if( bjet == m_outData -> o_leptop_b ) continue;

      // Discard any jet tagged as b-jet from hadronic top
      if( bjet -> GetMoment("hadtop_b") > 0 ) continue;

      b_cands.push_back(bjet);
    }

    for( AnalysisObject* hadW : *(m_outData -> o_taggedjets . at("RCMV")) ){ W_cands.push_back(hadW); }
    //if(m_outData -> o_lepW && !(m_outData -> o_leptop) ){ W_cands.push_back(m_outData -> o_lepW); }

  }

  std::vector<std::pair<int, double> > pair_dr;
  std::vector<std::pair<AnalysisObject*, AnalysisObject*> > pair_res;

  AOVector& res1_cands = (decay == "Wb") ? W_cands : 
    ( (decay == "Zt") ? *(m_outData -> o_taggedjets . at("RCMV")) : 
      *(m_outData -> o_taggedjets . at("RCMHiggs")) );

  AOVector& res2_cands = (decay == "Ht" || decay == "Zt") ? top_cands : b_cands;

  // Cannot reconstruct VLQ if either collection is empty
  if( (res1_cands.size() == 0) || (res2_cands.size() == 0) ) return 0; 

  AOVector* reco_collection = m_outData -> o_recoVLQ.at(decay);

  int index = 0;

  for(AnalysisObject* res1: res1_cands){

    for(AnalysisObject* res2: res2_cands){
      //double drcur = res1->DeltaR( *res2 );
      double drcur = fabs(res1->Eta() - res2->Eta() );

      if(drcur < drmax){

	res1 -> SetMoment("VLQchild", 0);
	res2 -> SetMoment("VLQchild", 0);
	
	pair_dr.push_back(std::pair<int,double>(index,drcur));
	pair_res.push_back( std::pair<AnalysisObject*, AnalysisObject*>(res1, res2) );
	index++;

      }
    }

  }
  if(pair_dr.size() == 0) return 0;

  AnalysisUtils::SortVectorPairs(pair_dr, -1, false);

  for(const std::pair<int, double>&  pdr : pair_dr){

    AnalysisObject* res1 = pair_res.at(pdr.first).first;
    AnalysisObject* res2 = pair_res.at(pdr.first).second;

    if( (res1->GetMoment("VLQchild") > 0) || (res2->GetMoment("VLQchild") > 0) ) continue;

    AnalysisObject* recoVLQ = new AnalysisObject();
    recoVLQ->SetPtEtaPhiM(0.,0.,0.,0.);
    *recoVLQ = (*res1) + (*res2);
    /*
    recoVLQ->SetMoment("dR12", pdr.second);
    recoVLQ->SetMoment("dPhi12", res1->DeltaPhi(*res2));
    recoVLQ->SetMoment("dEta12", fabs(res1->Eta() - res2->Eta()));
    */
    recoVLQ->SetMoment("dR12", res1->DeltaR(*res2));
    recoVLQ->SetMoment("dPhi12", res1->DeltaPhi(*res2));
    recoVLQ->SetMoment("dEta12", fabs(res1->Eta() - res2->Eta()));
    recoVLQ->SetMoment("m1", res1->M());
    recoVLQ->SetMoment("m2", res2->M());
    //recoVLQ->SetMoment("mAsymm", fabs(res1->M() - res2->M()));
    recoVLQ->SetMoment("redM", recoVLQ->M() - res1->M() - res2->M());
    if(decay == "Ht"){
      recoVLQ->SetMoment("redM2", recoVLQ->M() - fabs(res1->M()-125.18) - fabs(res2->M()-172.84));
    }
    if(decay == "Zt"){
      recoVLQ->SetMoment("redM2", recoVLQ->M() - fabs(res1->M()-91.1876) - fabs(res2->M()-172.84));
    }
    if(decay == "Wb"){
      recoVLQ->SetMoment("redM2", recoVLQ->M() - fabs(res1->M()-80.370) - fabs(res2->M()-4.18));
    }
    
    double fpt = (decay=="Wb") ? res2->Pt()/res1->Pt() : res1->Pt()/res2->Pt();
    recoVLQ -> SetMoment("fpT12", fpt);

    res1->UpdateMoment("VLQchild", 1);
    res2->UpdateMoment("VLQchild", 1);

    recoVLQ -> SetMoment("nmatch_truth", 0.);
    recoVLQ -> SetMoment("pdgId_truth", 0.);

    reco_collection -> push_back(recoVLQ);
    m_outData -> o_recoVLQ_n.at(decay)++;
  }

  top_cands.clear(); W_cands.clear(); b_cands.clear();

  return 0;
}

/*
std::vector<double> VLQ_ResonanceMaker::SolveCubic(const double& a, const double& b, const double& c, const double& d){

  double discriminant = 18*a*b*c*d - 4*pow(b,3)*d + b*b*c*c - 4*a*pow(c,3) -27*a*a*d*d;


  std::vector<double> solution_list{};

  double delta_0 = b*b - 3*a*c;
  double delta_1 = 2*pow(b,3) - 9*a*b*c +27*a*a*d;



  double delta_nested = discr_1*discr_1 - 4*pow(discr_0,3);
  if(discr_nested < 0 ){ 
    //no solutions
    return solution_list;
  }
  discr_nested = sqrt(discr_nested);


}

*/
