#include "IFAETopFramework/AnalysisObject.h"

#include "IFAETopFramework/AnalysisObject.h"
#include "IFAETopFramework/AnalysisUtils.h"
#include "VLQAnalysis/VLQ_Options.h"
#include "VLQAnalysis/VLQ_NtupleData.h"
#include "VLQAnalysis/VLQ_OutputData.h"
#include "VLQAnalysis/VLQ_VariableComputer.h"
#include "VLQAnalysis/VLQ_Enums.h"

#include "VLQAnalysis/VLQ_TruthManager.h"


//________________________________________________________
//
VLQ_TruthManager::VLQ_TruthManager( VLQ_Options *opt, const VLQ_NtupleData *ntup, VLQ_OutputData* outData, VLQ_VariableComputer* varComputer ):
m_opt(opt),
m_ntupData(ntup),
m_outData(outData),
m_varComputer(varComputer),
m_doVLQ(0),
m_dottbar(0)
{}

//________________________________________________________
//
VLQ_TruthManager::VLQ_TruthManager( const VLQ_TruthManager &q ){
    m_opt           = q.m_opt;
    m_ntupData      = q.m_ntupData;
    m_outData       = q.m_outData;
    m_varComputer   = q.m_varComputer;
    m_doVLQ         = q.m_doVLQ;
    m_dottbar       = q.m_dottbar;
}

//________________________________________________________
//
VLQ_TruthManager::~VLQ_TruthManager()
{
}

//________________________________________________________
//
bool VLQ_TruthManager::Initialize() {

  if(!m_ntupData){
    std::cout << "<!> ERROR in VLQ_TruthManager: the NtupleData object seems absent, please check !" << std::endl;
    return false;
  }

  for ( unsigned int iTruth = 0; iTruth < m_ntupData -> d_mc_pt -> size(); ++iTruth ) {
    AnalysisObject *temp = new AnalysisObject();
    temp -> SetPtEtaPhiM( m_ntupData -> d_mc_pt -> at(iTruth), m_ntupData -> d_mc_eta -> at(iTruth), m_ntupData -> d_mc_phi -> at(iTruth), m_ntupData -> d_mc_m -> at(iTruth) );
    temp -> SetMoment( "pdgId", m_ntupData -> d_mc_pdgId -> at(iTruth) );
    temp -> SetMoment( "absPdgId", TMath::Abs(m_ntupData -> d_mc_pdgId -> at(iTruth)) );
    temp -> SetMoment( "children_n", m_ntupData -> d_mc_children_index -> at(iTruth).size() );
    
    //Children information
    int counter = 0;
    for ( const int index : m_ntupData -> d_mc_children_index -> at(iTruth) ) {
      std::string str_ind = "children_";
      str_ind += std::to_string(counter);
      temp -> SetMoment( str_ind, index );
      counter++;
    }

    //Decay type (always useful)
    int code = 0;
    if(m_ntupData -> d_mc_children_index -> at(iTruth).size() >= 2){
      for( unsigned int i = 0; i < m_ntupData -> d_mc_children_index -> at(iTruth).size(); ++i ){
        const int index = m_ntupData -> d_mc_children_index -> at(iTruth)[i];
        if( index < (int)m_ntupData -> d_mc_pt -> size() ){
          const int abspdgId = TMath::Abs( m_ntupData -> d_mc_pdgId -> at(index) );
          code |= 1<<abspdgId;
        }
      }
    }
    temp -> SetMoment( "decayType", code );
    m_outData -> o_truth_all_particles -> push_back(temp);

  }

  if(m_opt -> SampleName () == SampleName::VLQ){
    m_doVLQ=true;
  }
  if( m_outData -> o_is_ttbar ){
    m_dottbar=true;
  }
  //PrintTruthContent();
  return true;
}

//________________________________________________________
//
double VLQ_TruthManager::GetTopPt() const {
  if(m_outData -> o_truth_all_particles){
    for( const AnalysisObject *truthpart : *m_outData -> o_truth_all_particles ){
      if ( (int)truthpart -> GetMoment( "pdgId" ) == 6 ){
        return truthpart->Pt();
      }
    }
  }
  return 0;
}

//_______________________________________________________
//
double VLQ_TruthManager::GetHiggsPt() const{
  if(m_outData -> o_truth_all_particles){
    for(const AnalysisObject *truthpart : *m_outData -> o_truth_all_particles){
      if( (int)truthpart->GetMoment("pdgId") == 25){
	return truthpart->Pt();
      }
    }
  }
  return 0;
}

//________________________________________________________
//
int VLQ_TruthManager::GetVLQDecayType() {

    //long totalDecays = m_ntupData -> d_truth_top_decay + m_ntupData -> d_truth_antitop_decay;
    std::string decayType = "";
    if( ( m_ntupData -> d_truth_top_decay & Ht ) == Ht ) decayType += "Ht";
    else if( ( m_ntupData -> d_truth_top_decay & Wb ) == Wb ) decayType += "Wb";
    else if( ( m_ntupData -> d_truth_top_decay & Zt ) == Zt ) decayType += "Zt";

    if( ( m_ntupData -> d_truth_antitop_decay & Ht ) == Ht ) decayType += "Ht";
    else if( ( m_ntupData -> d_truth_antitop_decay & Wb ) == Wb ) decayType += "Wb";
    else if( ( m_ntupData -> d_truth_antitop_decay & Zt ) == Zt ) decayType += "Zt";

    if( decayType == "HtHt" ) return VLQ_Enums::HtHt;
    if( decayType == "HtWb" || decayType == "WbHt" ) return VLQ_Enums::HtWb;
    if( decayType == "HtZt" || decayType == "ZtHt" ) return VLQ_Enums::HtZt;
    if( decayType == "WbWb" ) return VLQ_Enums::WbWb;
    if( decayType == "ZtWb" || decayType == "WbZt" ) return VLQ_Enums::WbZt;
    if( decayType == "ZtZt" ) return VLQ_Enums::ZtZt;
    return -1;
}

//________________________________________________________
//
int VLQ_TruthManager::PrintTruthContent() const {
    std::cout << "================================================================" << std::endl;
    std::cout << "=============== Printing truth information =====================" << std::endl;
    std::cout << "================================================================" << std::endl;
    std::cout << std::endl;
    std::cout << "Brute force truth info dumping ================================= " << std::endl;;
    for ( unsigned int iTruth = 0; iTruth < m_outData -> o_truth_all_particles -> size(); ++iTruth) {
        AnalysisObject *obj = m_outData -> o_truth_all_particles -> at(iTruth);
        std::cout << "=> Index = " << iTruth << " \t  pdgId: " << obj -> GetMoment("pdgId") << " \t  Children: ";
        for ( int iChild = 0; iChild < obj -> GetMoment("children_n"); ++iChild ) {
            std::string str_ind = "children_";
            str_ind += std::to_string(iChild);
            std::cout << obj -> GetMoment(str_ind) << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Decay chain plotting =========================================== " << std::endl;
    for ( unsigned int iTruth = 0; iTruth < m_outData -> o_truth_all_particles -> size(); ++iTruth) {
      if((TMath::Abs(m_outData -> o_truth_all_particles -> at(iTruth) -> GetMoment("pdgId"))==6)||(m_outData -> o_truth_all_particles->at(iTruth)->GetMoment("pdgId") == 25)){
            std::string space = "";
            FollowDecayChain(iTruth,space);
        }
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "============================= DONE =============================" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    return 1;
}

//________________________________________________________
//
int VLQ_TruthManager::FollowDecayChain( const int index, std::string &space ) const {



  if(!m_outData -> o_truth_all_particles){
        std::cerr << "<!> Error in VLQ_TruthManager::FollowDecayChain(" << index << "): no m_outData -> o_truth_all_particles vector found :-( " << std::endl;
        return -1;
    }
    if(index >= (int)m_outData -> o_truth_all_particles -> size()){
        std::cerr << "<!> Error in VLQ_TruthManager::FollowDecayChain(" << index << "): index is larger than vector size !!" << std::endl;
        return -1;
    }

    //std::cout << space << "-> index: " << index << " (pdgId = " << m_outData -> o_truth_all_particles -> at(index) -> GetMoment("pdgId") << ", n_children = "<< m_outData -> o_truth_all_particles -> at(index) -> GetMoment("children_n") <<")" << std::endl;

    for ( int iChild = 0; iChild < m_outData -> o_truth_all_particles->at(index)->GetMoment("children_n"); ++iChild ) {
        std::string str_ind = "children_";
        str_ind += std::to_string(iChild);
        std::string temp_space = space + " ";
        FollowDecayChain( m_outData -> o_truth_all_particles -> at(index) -> GetMoment(str_ind), temp_space );
    }

    return 1;
}
//
// //________________________________________________________
// //
int VLQ_TruthManager::PrepareBoostedTopStudies() {

  if(!m_outData -> o_truth_all_particles){
     std::cerr << "<!> Error in VLQ_TruthManager::PrepareBoostedTopStudies(): no m_outData -> o_truth_all_particles vector found :-( " << std::endl;
     return -1;
  }

  //Search for tops and computes needed variables for boosted top studies
  for ( AnalysisObject *particle : *m_outData -> o_truth_all_particles ){
    if( (int)particle -> GetMoment("absPdgId") != 6 ) continue;
    AnalysisObject *W = 0;
    AnalysisObject *Wdecay1 = 0;
    AnalysisObject *Wdecay2 = 0;
    AnalysisObject *b = 0;

    int HasHadDecay=0;
    int q1_idx=-1;
    int q2_idx=-1;
    int bt_idx=-1;
    for ( int iChild = 0; iChild < particle -> GetMoment("children_n"); ++iChild ) {
      std::string str_ind = "children_";
      str_ind += std::to_string(iChild);
      int child_index = particle -> GetMoment(str_ind);
      if(child_index < (int)m_outData -> o_truth_all_particles -> size()){
	int pdgId = m_outData -> o_truth_all_particles -> at(child_index) -> GetMoment("absPdgId");
	// std::cout << "child_absPdgId[" << child_index << "] = " << pdgId << std::endl;
	if( pdgId == 24 ){
	  W = m_outData -> o_truth_all_particles -> at(child_index);
	  if(W-> GetMoment( "decayType")<64&&W-> GetMoment( "decayType")>0){
	    HasHadDecay=1;
	  }
	  for ( int iWChild = 0; iWChild < W -> GetMoment("children_n"); ++iWChild ) {
	    std::string str_ind_W = "children_";
	    str_ind_W += std::to_string(iWChild);
	    if(!Wdecay1){
	      Wdecay1 = m_outData -> o_truth_all_particles -> at( W -> GetMoment( str_ind_W ) );
	      q1_idx=W -> GetMoment( str_ind_W ) ;
	    }
	    else if(!Wdecay2){
	      Wdecay2 = m_outData -> o_truth_all_particles -> at( W -> GetMoment( str_ind_W ) );
	      q2_idx=W -> GetMoment( str_ind_W ) ;
	    }
	  }
	}
       else if( pdgId == 5 ){
	  b = m_outData -> o_truth_all_particles -> at(child_index);
 	  bt_idx=child_index ;
 	}
      }
    }//loop on children
    double dR_Wb = -1;
    if(W && b){
      dR_Wb = b -> DeltaR( *W );
      //  std::cout << "delta R between W and b = " << dR_Wb << std::endl;

    }
    double dR_max_decays = -1;
    if(b && Wdecay1 && Wdecay2){
      dR_max_decays = TMath::Max( b->DeltaR(*Wdecay1), b->DeltaR(*Wdecay2));
      dR_max_decays = TMath::Max( dR_max_decays, Wdecay1 -> DeltaR(*Wdecay2) );
    }

    // std::cout << "delta R maximum " << dR_max_decays << std::endl;
    particle -> SetMoment("dR_Wb",dR_Wb);
    particle -> SetMoment("dR_max_decays",dR_max_decays);
    particle -> SetMoment("IsHadDecay",HasHadDecay);
    particle -> SetMoment("q1_idx",q1_idx);
    particle -> SetMoment("q2_idx",q2_idx);
    particle -> SetMoment("bt_idx",bt_idx);

    // std::cout << "q1_idx = " << q1_idx << std::endl;
    // std::cout << "q2_idx = " << q2_idx << std::endl;
    // std::cout << "bt_idx = " << bt_idx << std::endl;

  }//loop on truth particles


   return 1;
 }

// //________________________________________________________
// //
int VLQ_TruthManager::PrepareHiggsStudies(){
  if(!m_outData -> o_truth_all_particles){
    std::cerr << "<!> Error in VLQ_TruthManager::PrepareHiggsStudies(): no m_outData -> o_truth_all_particles vector found" << std::endl;
    return -1;                                                                                                                                                                                                
  }  
  for(AnalysisObject *particle : *m_outData -> o_truth_all_particles){ // Start loop through particles

    if(particle->GetMoment("absPdgId") != 25) continue;// Ignore if particle is not a Higgs                                                                                                                   

    AnalysisObject *W =0;
    AnalysisObject *Wdecay1 = 0;
    AnalysisObject *Wdecay2 = 0;
    AnalysisObject *Wdecay3 = 0;
    AnalysisObject *Wdecay4 = 0;

    AnalysisObject *Z = 0;
    AnalysisObject *Zdecay1 = 0;
    AnalysisObject *Zdecay2 = 0;
    AnalysisObject *Zdecay3 = 0;
    AnalysisObject *Zdecay4 = 0;


    AnalysisObject *Hdecay1 =0;
    AnalysisObject *Hdecay2 =0;

    int q1_idx = -1;
    int q2_idx = -1;
    int q3_idx = -1;
    int q4_idx = -1;


    int Hdecay1_idx = -1;
    int Hdecay2_idx = -1;

    bool btag = false;

    for(int iChild = 0; iChild < particle->GetMoment("children_n"); iChild++){ // Start loop through Higgs children                                                                                            
      std::string str_ind = "children_";
      str_ind+= std::to_string(iChild);
      int child_index= particle->GetMoment(str_ind);
      int pdgId = 0;
      if(!Hdecay1){
	Hdecay1 = m_outData -> o_truth_all_particles->at(child_index);
	Hdecay1_idx = child_index;
	pdgId = Hdecay1->GetMoment("absPdgId");
      }
      else if(!Hdecay2){
	Hdecay2 = m_outData -> o_truth_all_particles->at(child_index);
	Hdecay2_idx = child_index;
	pdgId = Hdecay2->GetMoment("absPdgId");
      }
      
      // int pdgId = m_outData -> o_truth_all_particles->at(child_index)->GetMoment("absPdgId");
     

      if(pdgId == 24){ // If Higgs child is W
	W = ((!Hdecay2) ? Hdecay1 : Hdecay2); 
	//	W = m_outData -> o_truth_all_particles->at(child_index);

        for(int iWChild = 0; iWChild < W->GetMoment("children_n"); iWChild++){ // Start loop through W children
	  std::string str_ind_W = "children_" ;
          str_ind_W += std::to_string(iWChild);
          if(!Wdecay1){
            Wdecay1 = m_outData -> o_truth_all_particles->at(W->GetMoment(str_ind_W));
            q1_idx = W->GetMoment(str_ind_W);
          }
          else if(!Wdecay2){
            Wdecay2 = m_outData -> o_truth_all_particles->at(W->GetMoment(str_ind_W));
	    q2_idx = W->GetMoment(str_ind_W);
	  }
	  else if(!Wdecay3){
	    Wdecay3 = m_outData -> o_truth_all_particles->at(W->GetMoment(str_ind_W));
	    q3_idx = W->GetMoment(str_ind_W);
	  }
	  else if(!Wdecay4){
	    Wdecay4 = m_outData -> o_truth_all_particles->at(W->GetMoment(str_ind_W));
	    q4_idx = W->GetMoment(str_ind_W);
	  }
	} // End loop through W children

	// std::cout << "q1_idx W = " << q1_idx << std::endl;
	// std::cout << "q2_idx W = " << q2_idx << std::endl;
	// std::cout << "q3_idx W = " << q3_idx << std::endl;
	// std::cout << "q4_idx W = " << q4_idx << std::endl;
      }// End if Higgs child is W


      else if(pdgId == 23){ // If Higgs child is Z
	Z = ((!Hdecay2) ? Hdecay1 : Hdecay2);

	// Z = m_outData -> o_truth_all_particles->at(child_index);
	for(int iZChild = 0; iZChild < Z->GetMoment("children_n"); iZChild++){ // Start loop through Z children
	  std::string str_ind_Z = "children_";
	  str_ind_Z += std::to_string(iZChild);
	  if(!Zdecay1){
	    Zdecay1 = m_outData -> o_truth_all_particles->at(Z->GetMoment(str_ind_Z));
	    q1_idx = Z->GetMoment(str_ind_Z);
	  }
	  else if(!Zdecay2){
	    Zdecay2 = m_outData -> o_truth_all_particles->at(Z->GetMoment(str_ind_Z));
	    q2_idx = Z->GetMoment(str_ind_Z);
	  }
	  else if(!Zdecay3){
	    Zdecay3 = m_outData -> o_truth_all_particles->at(Z->GetMoment(str_ind_Z));
	    q3_idx = Z->GetMoment(str_ind_Z);
	  }
	  else if(!Zdecay4){
	    Zdecay4 = m_outData -> o_truth_all_particles->at(Z->GetMoment(str_ind_Z));
	    q4_idx = Z->GetMoment(str_ind_Z);
	  }
	} // End loop through Z children

	// std::cout << "q1_idx Z = " << q1_idx << std::endl;
	// std::cout << "q2_idx Z = " << q2_idx << std::endl;
	// std::cout << "q3_idx Z = " << q3_idx << std::endl;
	// std::cout << "q4_idx Z = " << q4_idx << std::endl;
      } // End if Higgs child is Z
                                                    

    } // End loop through Higgs children                            

    if((Hdecay1->GetMoment("absPdgId") == 5) && (Hdecay2->GetMoment("absPdgId") == 5)){
      btag = true;
    }

    double dR_Hdecays = -1;
    if(Hdecay1 && Hdecay2){
      dR_Hdecays = Hdecay1->DeltaR(*Hdecay2);
    }

    double dR_max_decays = -1;
    if(Wdecay1 && Wdecay2 && Wdecay3 && Wdecay4){
      dR_max_decays = TMath::Max(Wdecay1->DeltaR(*Wdecay2), TMath::Max(Wdecay1->DeltaR(*Wdecay3), TMath::Max(Wdecay1->DeltaR(*Wdecay4), TMath::Max(Wdecay2->DeltaR(*Wdecay3), TMath::Max(Wdecay2->DeltaR(*Wdecay4), Wdecay3->DeltaR(*Wdecay4))))));

      //std::cout << "Max deltaR decays = " << dR_max_decays << std::endl;

    }
    else if(Zdecay1 && Zdecay2 && Zdecay3 && Zdecay4){
      dR_max_decays = TMath::Max(Zdecay1->DeltaR(*Zdecay2), TMath::Max(Zdecay1->DeltaR(*Zdecay3), TMath::Max(Zdecay1->DeltaR(*Zdecay4), TMath::Max(Zdecay2->DeltaR(*Zdecay3), TMath::Max(Zdecay2->DeltaR(*Zdecay4), Zdecay3->DeltaR(*Zdecay4))))));

      //std::cout << "Max deltaR decays = " << dR_max_decays << std::endl;
    }

    particle->SetMoment("H_dR_Hdecays", dR_Hdecays);
    particle->SetMoment("H_dR_max_decays", dR_max_decays);
    particle->SetMoment("H_decay1_idx", Hdecay1_idx);
    particle->SetMoment("H_decay2_idx", Hdecay2_idx);
    particle->SetMoment("H_q1_idx", q1_idx);
    particle->SetMoment("H_q2_idx", q2_idx);
    particle->SetMoment("H_q3_idx", q3_idx);
    particle->SetMoment("H_q4_idx", q4_idx);
    particle->SetMoment("H_btag", btag);

  } // End loop through particles                                                                                                                                                                           
    return 1 ;

}

// //_______________________________________________________
// //
int VLQ_TruthManager::MatchHiggsPartonsToFatJets(AOVector &Fatjets)const{
  if(!m_outData -> o_truth_all_particles){
    std::cerr << "<!> Error in VLQ_TruthManager::MatchHiggsPartonsToFatJets(): no m_outData -> o_truth_all_particles vector found" << std::endl;
    return -1;
  }
  
  std::vector<AOVector> VecHiggsDecays;
  VecHiggsDecays.clear();
  
  //Fill vector of top decays
  for(AnalysisObject *particle : *m_outData -> o_truth_all_particles){ // Start loop through particles
    
    if(particle->GetMoment("absPdgId") != 25) continue; // Ignore particle if it is not Higgs
    
    if(!(particle->GetMoment("H_btag"))) continue; // Ignore Higgs if it did not decay into b and bbar
    
    AOVector HiggsDecay;
    
    HiggsDecay.push_back(m_outData -> o_truth_all_particles->at(particle->GetMoment("H_decay1_idx")));
    HiggsDecay.push_back(m_outData -> o_truth_all_particles->at(particle->GetMoment("H_decay2_idx")));
    
    VecHiggsDecays.push_back(HiggsDecay);
    
  } // End loop through particles
  
  int fjet_n = Fatjets.size();
  
  int btag_Higgs_matched = 0;

  for(int fj_idx = 0; fj_idx < fjet_n; fj_idx++){ // Start loop through fat jets
    int classification_code = 0;
    bool HiggsIn = false;
    float HiggsPt = -99;
    for(int H_idx = 0; H_idx < (int)VecHiggsDecays.size(); H_idx++){ // Start loop through VecHiggsDecays elements
      for(int H_dp_idx = 0; H_dp_idx < (int)VecHiggsDecays.at(H_idx).size(); H_dp_idx++){ // Start loop through Higgs decay products
	
	float dR_tmp = Fatjets.at(fj_idx)->DeltaR(*VecHiggsDecays.at(H_idx).at(H_dp_idx));
	
	if(dR_tmp < 1.0){
	  classification_code |= 1  << ((H_idx*3)+H_dp_idx);
	}
      } // End loop through VecHiggsDecays elements
    }
    std::vector < double > ContainedHiggsPt;
    
    // Match jet with Higgs with lowest delta R
    for(AnalysisObject *particle : *m_outData -> o_truth_all_particles){

      if(particle->GetMoment("absPdgId") != 25) continue;

      if(!(particle->GetMoment("H_btag"))) continue;

      if(Fatjets.at(fj_idx)->DeltaR(*particle) < 0.75){

	HiggsIn = true;

	ContainedHiggsPt.push_back(particle->Pt());

	HiggsPt = particle->Pt();

      }
    }
     
    if(HiggsIn){
      btag_Higgs_matched++;
    }

    
    for(int cont_higgs_idx = 0; cont_higgs_idx < (int)ContainedHiggsPt.size(); cont_higgs_idx++){
      if(HiggsPt > ContainedHiggsPt.at(cont_higgs_idx)){
	HiggsPt= ContainedHiggsPt.at(cont_higgs_idx);
      }  
    }
 
    Fatjets.at(fj_idx)->SetMoment("HiggsPt", HiggsPt);
    Fatjets.at(fj_idx)->SetMoment("HiggsIn", HiggsIn);
    Fatjets.at(fj_idx)->SetMoment("H_Classification_code", classification_code);


  } // End loop through fat jets

  return btag_Higgs_matched;

}

// //________________________________________________________
// //
std::vector < double > VLQ_TruthManager::GetBTagHiggsMass (){
  std::vector < double > mass;
  for(AnalysisObject *particle : *m_outData -> o_truth_all_particles){
    if(particle->GetMoment("absPdgId")==25){
      if(particle->GetMoment("H_btag")){
	mass.push_back(particle->GetMoment("M"));
      }
    }
  }
  return mass;
}

// //________________________________________________________
// //
std::vector < double > VLQ_TruthManager::GetFatJetsMatchedToHiggsMass(AOVector &Fatjets) const{
  std::vector < double > fjmass;
  for(int fj_idx = 0 ; fj_idx < (int)Fatjets.size() ; fj_idx++){
    if(Fatjets.at(fj_idx)->IsKnownMoment("HiggsIn")){
      if(Fatjets.at(fj_idx)->GetMoment("HiggsIn")){
	fjmass.push_back(Fatjets.at(fj_idx)->GetMoment("M"));
      }
    }
  }
  return fjmass;
}

// //________________________________________________________
// //
std::vector < double > VLQ_TruthManager::GetFatJetsMatchedToTopMass(AOVector &Fatjets) const{
  std::vector < double > fjmass;
  for(int fj_idx = 0; fj_idx < (int)Fatjets.size(); fj_idx++){
    if(Fatjets.at(fj_idx)->IsKnownMoment("TopIn")){
      if(Fatjets.at(fj_idx)->GetMoment("TopIn")){
	fjmass.push_back(Fatjets.at(fj_idx)->GetMoment("M"));
      }
    }

  }
  return fjmass;
}

// //________________________________________________________
// //
int VLQ_TruthManager::MatchTopPartonsToFatJets(AOVector &Fatjets) const{

  if(!m_outData -> o_truth_all_particles){
    std::cerr << "<!> Error in VLQ_TruthManager::MatchTopPartonsToFatJets(): no m_outData -> o_truth_all_particles vector found :-( " << std::endl;
    return -1;
  }

  std::vector<AOVector> VecTopDecays;
  VecTopDecays.clear();

  //Fill vector of top decays
  for ( AnalysisObject *particle : *m_outData -> o_truth_all_particles ){
    if( (int)particle -> GetMoment("absPdgId") != 6 ) continue;

    if( (int)particle -> GetMoment("IsHadDecay") != 1) continue;


    AOVector TopDecay;
    if(particle->GetMoment("q1_idx")!=-1){
      TopDecay.push_back(m_outData -> o_truth_all_particles->at( particle->GetMoment("q1_idx") ) );
    }
    if(particle->GetMoment("q2_idx")!=-1){
      TopDecay.push_back(m_outData -> o_truth_all_particles->at( particle->GetMoment("q2_idx") ) );
    }
    if(particle->GetMoment("bt_idx")!=-1){
      TopDecay.push_back(m_outData -> o_truth_all_particles->at( particle->GetMoment("bt_idx") ) );
    }

    VecTopDecays.push_back(TopDecay);

  }

  int fjet_n=Fatjets.size();
  int hadr_tops_matched=0;
  // loop over fatjets
  for( int fj_idx=0;fj_idx<fjet_n;fj_idx++){
    int classification_code = 0;
    bool isFullyBoosted=false;
    bool TopIn=false;
    float TopPt=-99.;
    //loop over vector of hadronic top decay products
    for( int t_idx=0;t_idx<(int)VecTopDecays.size();t_idx++){
      //loop over decay products
       bool TopContained=true;
       for(int dp_idx=0;dp_idx<(int)VecTopDecays.at(t_idx).size();dp_idx++){
	float DR_tmp=Fatjets.at(fj_idx)->DeltaR(*VecTopDecays.at(t_idx).at(dp_idx));

	if(DR_tmp<1.0){

	  classification_code |= 1<<((t_idx*3)+dp_idx);
	}
	else{
	  TopContained=false;
	}

      }//end loop partons

      if(TopContained){
	isFullyBoosted=true;
      }

    }//end loop vector

    std::vector < double > ContainedTopPt;

    for ( AnalysisObject *particle : *m_outData -> o_truth_all_particles ){
      if( (int)particle -> GetMoment("absPdgId") != 6 ) continue;
      if( (int)particle -> GetMoment("IsHadDecay") != 1) continue;

      // std::cout << "Delta R[" << particle->GetMoment("pdgId") << "] = " << Fatjets.at(fj_idx)->DeltaR(*particle) << std::endl;
      if(Fatjets.at(fj_idx)->DeltaR(*particle)<0.75){
	TopIn=true;
	TopPt=particle->Pt();
	ContainedTopPt.push_back(particle->Pt());
      }
     }

    if(TopIn){
      hadr_tops_matched++;
    }

    for(int cont_top_idx = 0; cont_top_idx < (int)ContainedTopPt.size(); cont_top_idx++){
      if(TopPt > ContainedTopPt.at(cont_top_idx)){
	TopPt = ContainedTopPt.at(cont_top_idx);
      }
    }


    if(Fatjets.at(fj_idx)->IsKnownMoment("TopPt")){
      Fatjets.at(fj_idx)->UpdateMoment("TopPt", TopPt);
    }
    else{
      Fatjets.at(fj_idx)->SetMoment("TopPt", TopPt);
    }

    if(Fatjets.at(fj_idx)->IsKnownMoment("TopIn")){
      Fatjets.at(fj_idx)->UpdateMoment("TopIn", TopIn);
    }
    else{
      Fatjets.at(fj_idx)->SetMoment("TopIn", TopIn);
    }

    if(Fatjets.at(fj_idx)->IsKnownMoment("IsFullyBoosted")){
      Fatjets.at(fj_idx)->UpdateMoment("IsFullyBoosted", isFullyBoosted);
    }
    else{
      Fatjets.at(fj_idx)->SetMoment("IsFullyBoosted", isFullyBoosted);
    }

    if(Fatjets.at(fj_idx)->IsKnownMoment("Classification_code")){
      Fatjets.at(fj_idx)->UpdateMoment("Classification_code", classification_code);
    }
    else{
      Fatjets.at(fj_idx)->SetMoment("Classification_code", classification_code);
    }




    /* Fatjets.at(fj_idx)->UpdateMoment("TopPt", TopPt);
    Fatjets.at(fj_idx)->UpdateMoment("TopIn", TopIn);
    Fatjets.at(fj_idx)->UpdateMoment("IsFullyBoosted", isFullyBoosted);
    Fatjets.at(fj_idx)->UpdateMoment("Classification_code", classification_code );*/


    /* Fatjets.at(fj_idx)->SetMoment("TopPt",TopPt);
    Fatjets.at(fj_idx)->SetMoment("TopIn",TopIn);
    Fatjets.at(fj_idx)->SetMoment("IsFullyBoosted",isFullyBoosted);
    Fatjets.at(fj_idx)->SetMoment("Classification_code",classification_code );*/
  }//end loop fatjets


  return hadr_tops_matched;
}
//
//
//
//
//
//

//
//
//
int VLQ_TruthManager::MatchParticlesPartonsToFatJets(AOVector &Fatjets) const{

   if(!m_outData -> o_truth_all_MEParticles){
     std::cerr << "<!> Error in VLQ_TruthManager::MatchParticlesPartonsToFatJets(): no m_outData -> o_truth_all_MEParticles vector found :-( " << std::endl;
     return -1;
   }

   if(!m_outData -> o_truth_all_partons){
     std::cerr << "<!> Error in VLQ_TruthManager::MatchParticlesPartonsToFatJets(): no m_outData -> o_truth_all_partons vector found :-( " << std::endl;
     return -1;
   }


   int fjet_n=Fatjets.size();
   // loop over fatjets
   for( int fj_idx=0;fj_idx<fjet_n;fj_idx++){

     int counter = 0 ;
     
     int top1=-1;
     int W1=-1;
     int Z1=-1;
     int H1=-1;
     int b1=-1;
     //loop over vector of ME particles
     

     for(AnalysisObject *particle : *m_outData -> o_truth_all_MEParticles){

       std::cout << "kek" << std::endl;

       float DR_tmp=Fatjets.at(fj_idx)->DeltaR(*particle);

       if(DR_tmp<0.75){

 	int pdgIdPart= (int) particle -> GetMoment("absPdgId");

 	if(pdgIdPart==6){
 	  if(top1==-1){
 	    top1++;
 	    Fatjets.at(fj_idx)->SetMoment("PartonTop1In",1);
	    counter++;
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("PartonTop2In",1);
	    counter++;
 	  }
 	}
 	else if(pdgIdPart==23){
 	  if(Z1==-1){
 	    Z1++;
 	    Fatjets.at(fj_idx)->SetMoment("PartonZ1In",1);
	    counter++;
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("PartonZ2In",1);
	    counter++;
 	  }
 	}
 	else if(pdgIdPart==24){
 	  if(W1==-1){
 	    W1++;
 	    Fatjets.at(fj_idx)->SetMoment("PartonW1In",1);
	    counter++;
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("PartonW2In",1);
	    counter++;
 	  }
 	}
 	else if(pdgIdPart==25){
 	  if(H1==-1){
 	    H1++;
 	    Fatjets.at(fj_idx)->SetMoment("PartonH1In",1);
	    counter++;
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("PartonH2In",1);
	    counter++;
	  } 
	}
 	else if(pdgIdPart==5){
 	  if(b1==-1){
 	    b1++;
 	    Fatjets.at(fj_idx)->SetMoment("PartonB1In",1);
	    counter++;
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("PartonB2In",1);
	    counter++;
 	  }
 	}
 	else{
 	  Fatjets.at(fj_idx)->SetMoment("PartonUknIn",1);
	  counter++;
 	}


	std::cout << "Number of moments set: " << counter << std::endl;

       } //DR cut

     }//end loop ME particle



     //loop over partons
     for(AnalysisObject *parton : *m_outData -> o_truth_all_partons){

       float DR_tmp=Fatjets.at(fj_idx)->DeltaR(*parton);

       if(DR_tmp<0.75){

 	if(parton->IsKnownMoment("isFromTop1")&&parton->GetMoment("isFromTop1")==1){
 	  if(Fatjets.at(fj_idx)->IsKnownMoment("nFromTop1")){
 	    int ntop1=(int) Fatjets.at(fj_idx)->GetMoment("nFromTop1");
 	    ntop1++;
	    std::cout << "ntop1 = " << ntop1 << std::endl;
	    Fatjets.at(fj_idx)->UpdateMoment("nFromTop1",ntop1);
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("nFromTop1",1);
 	  }
	  if(parton->IsKnownMoment("isFromLepTop1")&&parton->GetMoment("isFromLepTop1")==1){
 	    if(Fatjets.at(fj_idx)->IsKnownMoment("nFromLepTop1")){
 	      int nltop1=(int) Fatjets.at(fj_idx)->GetMoment("nFromLepTop1");
 	      nltop1++;
	      std::cout << "nltop1 = " << nltop1 << std::endl;
 	      Fatjets.at(fj_idx)->UpdateMoment("nFromLepTop1",nltop1);
 	    }
 	    else{
 	      Fatjets.at(fj_idx)->SetMoment("nFromLepTop1",1);
 	    }


 	  }

 	}
 	else if(parton->IsKnownMoment("isFromTop2")&&parton->GetMoment("isFromTop2")==1){
 	  if(Fatjets.at(fj_idx)->IsKnownMoment("nFromTop2")){
 	    int ntop2=(int) Fatjets.at(fj_idx)->GetMoment("nFromTop2");
 	    ntop2++;
	    std::cout << "ntop2 = " << ntop2 << std::endl;
 	    Fatjets.at(fj_idx)->UpdateMoment("nFromTop2",ntop2);
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("nFromTop2",1);
 	  }



 	  if(parton->IsKnownMoment("isFromLepTop2")&&parton->GetMoment("isFromLepTop2")==1){
 	    if(Fatjets.at(fj_idx)->IsKnownMoment("nFromLepTop2")){
 	      int nltop2=(int) Fatjets.at(fj_idx)->GetMoment("nFromLepTop2");
 	      nltop2++;
	      std::cout << "nltop2 = " << nltop2 << std::endl;
 	      Fatjets.at(fj_idx)->UpdateMoment("nFromLepTop2",nltop2);
 	    }
 	    else{
 	      Fatjets.at(fj_idx)->SetMoment("nFromLepTop2",1);
	    }


 	  }



 	}
 	else if(parton->IsKnownMoment("isFromZ1")&&parton->GetMoment("isFromZ1")==1){
 	  if(Fatjets.at(fj_idx)->IsKnownMoment("nFromZ1")){
 	    int nz1=(int) Fatjets.at(fj_idx)->GetMoment("nFromZ1");
 	    nz1++;
	    std::cout << "nz1 = " << nz1 << std::endl;
 	    Fatjets.at(fj_idx)->UpdateMoment("nFromZ1",nz1);
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("nFromZ1",1);
 	  }

 	}
 	else if(parton->IsKnownMoment("isFromZ2")&&parton->GetMoment("isFromZ2")==1){
 	  if(Fatjets.at(fj_idx)->IsKnownMoment("nFromZ2")){
 	    int nz2=(int) Fatjets.at(fj_idx)->GetMoment("nFromZ2");
 	    nz2++;
	    std::cout << "nz2 = " << nz2 << std::endl;
 	    Fatjets.at(fj_idx)->UpdateMoment("nFromZ2",nz2);
 	  }
 	  else{
	    Fatjets.at(fj_idx)->SetMoment("nFromZ2",1);
 	  }

 	}
 	else if(parton->IsKnownMoment("isFromW1")&&parton->GetMoment("isFromW1")==1){
 	  if(Fatjets.at(fj_idx)->IsKnownMoment("nFromW1")){
 	    int nw1=(int) Fatjets.at(fj_idx)->GetMoment("nFromW1");
 	    nw1++;
	    std::cout << "nw1 = " << nw1 << std::endl;
 	    Fatjets.at(fj_idx)->UpdateMoment("nFromW1",nw1);
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("nFromW1",1);
 	  }

 	}
 	else if(parton->IsKnownMoment("isFromW2")&&parton->GetMoment("isFromW2")==1){
 	  if(Fatjets.at(fj_idx)->IsKnownMoment("nFromW2")){
 	    int nw2=(int) Fatjets.at(fj_idx)->GetMoment("nFromW2");
 	    nw2++;
	    std::cout << "nw2 = " << nw2 << std::endl;
 	    Fatjets.at(fj_idx)->UpdateMoment("nFromW2",nw2);
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("nFromW2",1);
	  }

 	}
 	else if(parton->IsKnownMoment("isFromH1")&&parton->GetMoment("isFromH1")==1){
 	  if(Fatjets.at(fj_idx)->IsKnownMoment("nFromH1")){
 	    int nh1=(int) Fatjets.at(fj_idx)->GetMoment("nFromH1");
 	    nh1++;
	    std::cout << "nh1 = " << nh1  << std::endl;
 	    Fatjets.at(fj_idx)->UpdateMoment("nFromH1",nh1);
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("nFromH1",1);
 	  }

 	}
 	else if(parton->IsKnownMoment("isFromH2")&&parton->GetMoment("isFromH2")==1){
 	  if(Fatjets.at(fj_idx)->IsKnownMoment("nFromH2")){
 	    int nh2=(int) Fatjets.at(fj_idx)->GetMoment("nFromH2");
 	    nh2++;
	    std::cout << "nh2 = " << nh2 << std::endl;
 	    Fatjets.at(fj_idx)->UpdateMoment("nFromH2",nh2);
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("nFromH2",1);
 	  }

 	}
 	else if(parton->IsKnownMoment("isFromB1")&&parton->GetMoment("isFromB1")==1){
 	  if(Fatjets.at(fj_idx)->IsKnownMoment("nFromB1")){
 	    int nb1=(int) Fatjets.at(fj_idx)->GetMoment("nFromB1");
 	    nb1++;
	    std::cout << "nb1 = " << nb1 << std::endl;
 	    Fatjets.at(fj_idx)->UpdateMoment("nFromB1",nb1);
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("nFromB1",1);
 	  }

 	}
	else if(parton->IsKnownMoment("isFromB2")&&parton->GetMoment("isFromB2")==1){
 	  if(Fatjets.at(fj_idx)->IsKnownMoment("nFromB2")){
 	    int nb2=(int) Fatjets.at(fj_idx)->GetMoment("nFromB2");
 	    nb2++;
	    std::cout << "nb2 = " << nb2 << std::endl;
 	    Fatjets.at(fj_idx)->UpdateMoment("nFromB2",nb2);
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("nFromB2",1);
 	  }

 	}
 	else{
 	  if(Fatjets.at(fj_idx)->IsKnownMoment("nFromUkn")){
 	    int nuk=(int) Fatjets.at(fj_idx)->GetMoment("nFromUkn");
 	    nuk++;
	    std::cout << "nuk = " << nuk << std::endl;
 	    Fatjets.at(fj_idx)->UpdateMoment("nFromUkn",nuk);
 	  }
 	  else{
 	    Fatjets.at(fj_idx)->SetMoment("nFromUkn",1);
 	  }

 	}

       }//DR cut

     }//end loop partons

   }//end loop fatjets


   return 1;

}

// //###############################################################################
//
int VLQ_TruthManager::FillParticlesPartonsVectors(){
  if(!m_outData -> o_truth_all_particles){
    std::cerr << "<!> Error in VLQ_TruthManager::FillParticlesPartonsVectors(): no m_outData -> o_truth_all_particles vector found :-( " << std::endl;
    return -1;
  }


  //
  // Find b-quarks
  //

  for ( AnalysisObject *particle : *m_outData -> o_truth_all_particles ){
    if( particle -> GetMoment("absPdgId") != 5 ) continue;
    bool selfdecaying = false;
    //start loop to check self decaying
    for ( int iChild = 0; iChild < particle -> GetMoment("children_n"); ++iChild ) {
      AnalysisObject *child = RetrieveChild(particle, iChild);
      if(child){
	int childpdgId=(int) child -> GetMoment("absPdgId");
	if(particle -> GetMoment("absPdgId") == childpdgId){
	  selfdecaying=true;
	}
      }
    }//end loop to check self decaying
    if(!selfdecaying) { m_outData -> o_truth_all_bpartons -> push_back(particle); }
  }


  //Fill Matrix Element particles
  //Search for decays of VLQ
  for ( AnalysisObject *particle : *m_outData -> o_truth_all_particles ){
    int pdgIdParent= (int) particle -> GetMoment("absPdgId");
    if(m_doVLQ){
      if( (pdgIdParent != 8) && (pdgIdParent != 6000006) ) continue;
    }
    if(m_dottbar){
      if( pdgIdParent != 6 ) continue;
    }
    bool motherselfdecaying=false;
    for ( int iChild = 0; iChild < particle -> GetMoment("children_n"); ++iChild ) {

      bool selfdecaying=false;
      AnalysisObject *child = RetrieveChild(particle, iChild);
      if(!child) continue;

      int childpdgId = (int) child -> GetMoment("absPdgId");

      if(m_dottbar){
	if(pdgIdParent==childpdgId){
	  motherselfdecaying=true;
	}
      }
      if(m_doVLQ){
	//start loop to check self decaying
	for ( int iGrandChild = 0; iGrandChild < child -> GetMoment("children_n"); ++iGrandChild ) {
	  AnalysisObject *grandchild = RetrieveChild(child, iGrandChild);
	  if(grandchild){
	    int grandchildpdgId=(int) grandchild -> GetMoment("absPdgId");
	    if(childpdgId==grandchildpdgId){
	      selfdecaying=true;
	    }
	  }
	}//end loop to check self decaying

 	//build parton vector

	
 	if(!selfdecaying){
	  
 	  if(m_doVLQ){
 	    m_outData -> o_truth_all_MEParticles->push_back(child);
 	  }
	  
 	}//if not self-decaying
	
      }//doVLQ
      
    }//loop over children
    
    if(!motherselfdecaying){
      //if(m_dottbar){
	
 	m_outData -> o_truth_all_MEParticles->push_back(particle);
	//}
    }
    
  }//loop over particles
  
  if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout<<" ME particles identified " << std::endl; }
  
  int top1=-1;
  int W1=-1;
  int Z1=-1;
  int H1=-1;
  int b1=-1;
  
  AOVector list_inv = {}; //invisible particles (neutrinos)

  //building vector of interesting partons
  for(AnalysisObject *part : *m_outData -> o_truth_all_MEParticles){

    int pdgIdPart= (int) part -> GetMoment("absPdgId");
     for ( int iCd = 0; iCd < part -> GetMoment("children_n"); ++iCd ) {
       AnalysisObject* cd = RetrieveChild(part, iCd);
       if(cd){ m_outData -> o_truth_all_partons -> push_back(cd); }
     }

     if(m_opt->MsgLevel() == Debug::DEBUG){ std::cout << " Classifying particles pdgIdPart " << pdgIdPart << std::endl; }

     if( (pdgIdPart == 8) || (pdgIdPart == 6000006) ){ //VLQ

       m_outData -> o_truth_partons.at("VLQ") -> push_back(part);
       m_outData -> o_truth_partons_n.at("VLQ") ++;
       AnalysisObject* vcd1 = 0;
       AnalysisObject* vcd2 = 0;

       std::string vlq_decay = "";
       std::string vlq_decay_ext = "";
       for ( int iCd = 0; iCd < part -> GetMoment("children_n"); ++iCd ) {
	 AnalysisObject* cd = RetrieveChild(part, iCd);
	 if(!cd) continue;

	 if(cd->GetMoment("absPdgId")==5){
	   m_outData -> o_truth_all_b -> push_back(cd);

	   if(b1==-1){
	     b1++;
	     cd->SetMoment("isFromB1",1);
	     m_outData -> o_truth_partons.at("b1") -> push_back(cd);
	     m_outData -> o_truth_partons_n.at("b1") ++;
	   }
	   else{
	     cd->SetMoment("isFromB2",1);
	     m_outData -> o_truth_partons.at("b2") -> push_back(cd);
	     m_outData -> o_truth_partons_n.at("b2") ++;
	   }

	 }

	 if(!vcd1) vcd1 = cd;
	 else vcd2 = cd;

	 //
	 // Categorisation of VLQ decay
	 //
	 if     ( (cd->GetMoment("absPdgId")==5) || (cd->GetMoment("absPdgId")==24) ){ vlq_decay = "Wb"; }
	 else if( (cd->GetMoment("absPdgId")==25) ){ vlq_decay = "Ht"; }
	 else if( (cd->GetMoment("absPdgId")==23) ){ vlq_decay = "Zt"; }

	 //
	 // Finer categorisation of VLQ decay
	 //
	 if( cd->GetMoment("absPdgId") > 6 ){  

	   bool cd_had = false; bool cd_lep = false; bool cd_inv = false; bool cd_bdec = false;

	   for ( int iGc = 0; iGc < cd -> GetMoment("children_n"); ++iGc ) {

	     AnalysisObject* gc = RetrieveChild(cd, iGc);
	     if(!gc) continue;
	     if(gc->GetMoment("absPdgId") <= 6){
	       if(gc->GetMoment("absPdgId") == 5){ cd_bdec = true; }
	       cd_had = true; break;
	     }
	     else if( (gc -> GetMoment("absPdgId") == 11) || (gc -> GetMoment("absPdgId") == 13) || (gc -> GetMoment("absPdgId") == 15)) {
	       cd_lep = true; break;
	     }
	     else if( (gc -> GetMoment("absPdgId") == 12) || (gc -> GetMoment("absPdgId") == 14) || (gc -> GetMoment("absPdgId") == 16)) {
	       if(cd->GetMoment("absPdgId")==23){ cd_inv = true; }
	       if(cd->GetMoment("absPdgId")==24){ cd_lep = true; }
	       break;
	     }
	   }// Grandchildren

	   if((cd->GetMoment("absPdgId")==25) && cd_bdec){ vlq_decay_ext = "Hbdect"; }
	   if(cd->GetMoment("absPdgId")==24){ 
	     if(cd_lep) vlq_decay_ext = "Wlepb"; 
	     if(cd_had) vlq_decay_ext = "Whadb"; 
	   }
	   if((cd->GetMoment("absPdgId")==23) && cd_had){ vlq_decay_ext = "Zhadt"; }

	 }//H,W or Z child

       }// loop over children
       if(vcd1 && vcd2){

	 double fpt = 0.;
	 if(vlq_decay == "Wb"){
	   fpt = (vcd1->GetMoment("absPdgId")==24) ? vcd2->Pt() / vcd1->Pt() : vcd1->Pt() / vcd2->Pt();
	 }
	 else if( (vlq_decay == "Ht") || (vlq_decay == "Zt") ){
	   fpt = (vcd1->GetMoment("absPdgId")==6) ? vcd2->Pt() / vcd1->Pt() : vcd1->Pt() / vcd2->Pt();
	 }

	 part -> SetMoment( "fpT12", fpt );
	 part -> SetMoment( "dR12", vcd1 -> DeltaR(*vcd2) );
	 part -> SetMoment( "dPhi12", TMath::Abs(vcd1 -> DeltaPhi(*vcd2)) );
	 part -> SetMoment( "dEta12", TMath::Abs(vcd1 -> Eta() - vcd2 -> Eta()) );
       }
       /*
       else{
	 part -> SetMoment( "fpT12", -1. );
	 part -> SetMoment( "dR12", -1. );
	 part -> SetMoment( "dPhi12", -10. );
	 part -> SetMoment( "dEta12", -10. );
       }
       */
       if(vlq_decay != ""){
	 m_outData -> o_truth_partons.at("VLQ_"+vlq_decay) -> push_back(part);
	 m_outData -> o_truth_partons_n.at("VLQ_"+vlq_decay) ++;
       }
       if(vlq_decay_ext != ""){
	 m_outData -> o_truth_partons.at("VLQ_"+vlq_decay_ext) -> push_back(part);
	 m_outData -> o_truth_partons_n.at("VLQ_"+vlq_decay_ext) ++;
       }

     }

     else if(pdgIdPart==6){ //top
       top1++;
       bool leptonic=false;

       AnalysisObject* bchild = 0;
       AnalysisObject* Wchild = 0;
       AnalysisObject* q1 = 0;
       AnalysisObject* q2 = 0;

       AnalysisObject* lep = 0;
       AnalysisObject* nu = 0;

       for ( int iCd = 0; iCd < part -> GetMoment("children_n"); ++iCd ) {
	 AnalysisObject* cd = RetrieveChild(part, iCd);
	 if(!cd) continue;

	 if(cd->GetMoment("absPdgId")==5){
	   bchild = cd;
	   m_outData -> o_truth_all_b -> push_back(cd);
	 }
	 else if(cd->GetMoment("absPdgId")==24){
	   Wchild = cd;
	   if(cd-> GetMoment("decayType")>64){
	     leptonic=true;
	     m_outData -> o_truth_partons.at("leptop_W") -> push_back(cd);
	     m_outData -> o_truth_partons_n.at("leptop_W") ++;
	   }
	   else{
	     m_outData -> o_truth_partons.at("hadtop_W") -> push_back(cd);
	     m_outData -> o_truth_partons_n.at("hadtop_W") ++;
	   }

	   for ( int iGc = 0; iGc < cd -> GetMoment("children_n"); ++iGc ) {

	     AnalysisObject* gc = RetrieveChild(cd, iGc);
	     if(!gc) continue;

	     if(gc -> GetMoment("absPdgId") <= 6){
	       if(!q1) q1 = gc;
	       else q2 = gc;
	       m_outData -> o_truth_all_q -> push_back(gc);
	     }
	     else if( (gc -> GetMoment("absPdgId") == 11) || (gc -> GetMoment("absPdgId") == 13) || (gc -> GetMoment("absPdgId") == 15)) {
	       lep = gc;
	       m_outData -> o_truth_partons.at("leptop_lep") -> push_back(gc);
	       m_outData -> o_truth_partons_n.at("leptop_lep") ++;
	       m_outData -> o_truth_all_lep -> push_back(gc);
	     }
	     else if( (gc -> GetMoment("absPdgId") == 12) || (gc -> GetMoment("absPdgId") == 14) || (gc -> GetMoment("absPdgId") == 16)) {
	       nu = gc;
	       m_outData -> o_truth_partons.at("leptop_nu") -> push_back(gc);
	       m_outData -> o_truth_partons_n.at("leptop_nu") ++;
	       list_inv.push_back(gc);
	       *(m_outData -> o_truth_partonMET) += *gc;
	     }
	   }// grandchildren of top
	   if(!leptonic && q1 && q2){

	       m_outData -> o_truth_partons_n.at("hadtop_q1") ++;
	       m_outData -> o_truth_partons_n.at("hadtop_q2") ++;

	     if( q1->Pt() > q2->Pt()){ 
	       m_outData -> o_truth_partons.at("hadtop_q1") -> push_back(q1);
	       m_outData -> o_truth_partons.at("hadtop_q2") -> push_back(q2);
	     }
	     else{
	       m_outData -> o_truth_partons.at("hadtop_q1") -> push_back(q2);
	       m_outData -> o_truth_partons.at("hadtop_q2") -> push_back(q1);
	     }

	   }//hadronic W

	   if(top1<1){
	     cd->SetMoment("isFromTop1",1);
	     if(leptonic){
	       cd->SetMoment("isFromLepTop1",1);
	     }
	   }
	   else{
	     cd->SetMoment("isFromTop2",1);
	     if(leptonic){
	       cd->SetMoment("isFromLepTop2",1);
	     }
	   }

	 }//W daughter of top

       }// children of top
       if( bchild && Wchild ){
	 part -> SetMoment("dR_bW", bchild->DeltaR(*Wchild) );
       }
       /*
       else{
	 part -> SetMoment("dR_bW", -1. );
       }
       */
       if(leptonic){

	 if(bchild){
	   part -> SetMoment("dR_blep", bchild->DeltaR(*lep) );
	   part -> SetMoment("dPhi_blep", TMath::Abs(bchild->DeltaPhi(*lep)) );
	     
	   part -> SetMoment("dPhi_bnu", TMath::Abs(bchild->DeltaPhi(*nu)) );
	   part -> SetMoment("mT_bnu",  m_varComputer -> GetMTw(bchild, nu) );

	   m_outData -> o_truth_partons.at("leptop_b") -> push_back(bchild);
	   m_outData -> o_truth_partons_n.at("leptop_b") ++;

	 }
	 /*
	 else{
	   part -> SetMoment("dR_blep", -1.);
	   part -> SetMoment("dPhi_blep", -10.);
	     
	   part -> SetMoment("dPhi_bnu", -10.);
	   part -> SetMoment("mT_bnu", -1.);
	 }
	 */
	 if(lep && nu){
	   part -> SetMoment("dPhi_lepnu", TMath::Abs(lep->DeltaPhi(*nu)) );
	   part -> SetMoment("dEta_lepnu", TMath::Abs(lep->Eta() - nu->Eta()) );
	   part -> SetMoment("mT_lepnu", m_varComputer -> GetMTw(lep, nu) );
	 }
	 /*
	 else{
	   part -> SetMoment("dPhi_lepnu", -10. );
	   part -> SetMoment("dEta_lepnu", -99. );
	   part -> SetMoment("mT_lepnu", -1. );
	 }
	 */
	 m_outData -> o_truth_partons.at("leptop") -> push_back(part);
	 m_outData -> o_truth_partons_n.at("leptop") ++;

       } //leptonic
	   
       else{

	 if(q1 && q2 && bchild){
	   double drmax = std::max( std::max(bchild->DeltaR(*q1), bchild->DeltaR(*q2)), q1->DeltaR(*q2) );
	   part -> SetMoment( "dRmax_bq", drmax);
	   part -> SetMoment( "dR_qq", q1->DeltaR(*q2) );
	 }
	 /*
	 else{
	   part -> SetMoment( "dRmax_bq", -1.);
	   part -> SetMoment( "dR_qq", -1. );
	 }
	 */
	 if(bchild){
	   m_outData -> o_truth_partons.at("hadtop_b") -> push_back(bchild);
	   m_outData -> o_truth_partons_n.at("hadtop_b") ++;
	 }
	 m_outData -> o_truth_partons.at("hadtop") -> push_back(part);
	 m_outData -> o_truth_partons_n.at("hadtop") ++;

       }

     }//top


     else if(pdgIdPart==25){ //Higgs
       H1++;
       bool bbmode=false;
       AnalysisObject* b1 = 0;
       AnalysisObject* b2 = 0;

       for ( int iCd = 0; iCd < part -> GetMoment("children_n"); ++iCd ) {
	 AnalysisObject* cd = RetrieveChild(part, iCd);
	 if(!cd) continue;
	 if(cd->GetMoment("absPdgId")==5){
	   bbmode = true;
	   if(!b1) b1 = cd;
	   else b2 = cd;
	   m_outData -> o_truth_all_b -> push_back(cd);
	 }
	 else if( (cd->GetMoment("absPdgId")<5) || (cd->GetMoment("absPdgId")==21) ){
	   m_outData -> o_truth_all_q -> push_back(cd);
	 }
	 else if( (cd->GetMoment("absPdgId")==11) || (cd->GetMoment("absPdgId")==13) || (cd->GetMoment("absPdgId")==15) ){
	   m_outData -> o_truth_all_lep -> push_back(cd);
	 }
	 else if( (cd->GetMoment("absPdgId")==12) || (cd->GetMoment("absPdgId")==14) || (cd->GetMoment("absPdgId")==16) ){
	   *(m_outData -> o_truth_partonMET) += *cd;
	 }

	 else if( (cd->GetMoment("absPdgId")==23) || (cd->GetMoment("absPdgId")==24) ){

	   for ( int iGc = 0; iGc < cd -> GetMoment("children_n"); ++iGc ) {
	     AnalysisObject* gc = RetrieveChild(cd, iGc);
	     if(!gc) continue;

	     if(gc->GetMoment("absPdgId")==5){
	       m_outData -> o_truth_all_b -> push_back(gc);
	     }
	     else if( (gc->GetMoment("absPdgId")<5) || (gc->GetMoment("absPdgId")==21) ){
	       m_outData -> o_truth_all_q -> push_back(gc);
	     }
	     else if( (gc->GetMoment("absPdgId")==11) || (gc->GetMoment("absPdgId")==13) || (gc->GetMoment("absPdgId")==15) ){
	       m_outData -> o_truth_all_lep -> push_back(gc);
	     }
	     else if( (gc->GetMoment("absPdgId")==12) || (gc->GetMoment("absPdgId")<=14) || (gc->GetMoment("absPdgId")<=16) ){
	       *(m_outData -> o_truth_partonMET) += *gc;
	     }
	     
	   }//grandchildren

	 }//Vector boson children

	 if(H1<1){
	   cd->SetMoment("isFromH1",1);
	   if(bbmode){
	     cd->SetMoment("isFromHbb1",1);
	   }
	 }
	 else{
	   cd->SetMoment("isFromH2",1);
	   if(bbmode){
	     cd->SetMoment("isFromHbb2",1);
	   }
	 }

       }// children of Higgs
       if(bbmode){
	 if(b1 && b2){
	   part -> SetMoment( "dR_bb", b1->DeltaR(*b2) );
	   m_outData -> o_truth_partons_n.at("H_b1") ++;
	   m_outData -> o_truth_partons_n.at("H_b2") ++;

	   if(b1->Pt() > b2->Pt()){ 
	     m_outData -> o_truth_partons.at("H_b1") -> push_back(b1);
	     m_outData -> o_truth_partons.at("H_b2") -> push_back(b2);
	     part -> SetMoment( "is_bb", 1);
	   }
	   else{
	     m_outData -> o_truth_partons.at("H_b1") -> push_back(b2);
	     m_outData -> o_truth_partons.at("H_b2") -> push_back(b1);
	     part -> SetMoment( "is_bb", 0);
	   }
	 }
	 m_outData -> o_truth_partons.at("Hbb") -> push_back(part);
	 m_outData -> o_truth_partons_n.at("Hbb") ++;
       }
       else{
	 //part -> SetMoment( "dR_bb", -1. );
	 m_outData -> o_truth_partons.at("Hnonbb") -> push_back(part);
	 m_outData -> o_truth_partons_n.at("Hnonbb") ++;
       }
       m_outData -> o_truth_partons.at("H") -> push_back(part);
       m_outData -> o_truth_partons_n.at("H") ++;


     }//Higgs

     else if(pdgIdPart==24){ //W
       bool leptonic = false;
       W1++;
       AnalysisObject* q1 = 0;
       AnalysisObject* q2 = 0; 
       AnalysisObject* lep = 0;
       AnalysisObject* nu = 0;
       if(part-> GetMoment("decayType")>64){
	 leptonic=true;
	 m_outData -> o_truth_partons.at("lepW") -> push_back(part);
	 m_outData -> o_truth_partons_n.at("lepW") ++;
       }
       else{
	 m_outData -> o_truth_partons.at("hadW") -> push_back(part);
	 m_outData -> o_truth_partons_n.at("hadW") ++;
       }
       for ( int iCd = 0; iCd < part -> GetMoment("children_n"); ++iCd ) {
	 AnalysisObject* cd = RetrieveChild(part, iCd);
	 if(!cd) continue;
	 if(cd -> GetMoment("absPdgId") <= 6){
	   if(!q1) q1 = cd;
	   else q2 = cd;

	   if(cd -> GetMoment("absPdgId") == 5){
	     m_outData -> o_truth_all_b -> push_back(cd);
	   }
	   else{
	     m_outData -> o_truth_all_q -> push_back(cd);
	   }
	 }
	 else if( (cd -> GetMoment("absPdgId") == 11) || (cd -> GetMoment("absPdgId") == 13) || (cd -> GetMoment("absPdgId") == 15)) {
	   lep = cd;
	   m_outData -> o_truth_partons.at("lepW_lep") -> push_back(cd);
	   m_outData -> o_truth_partons_n.at("lepW_lep") ++;
	   m_outData -> o_truth_all_lep -> push_back(cd);
	 }
	 else if( (cd -> GetMoment("absPdgId") == 12) || (cd -> GetMoment("absPdgId") == 14) || (cd -> GetMoment("absPdgId") == 16)) {
	   nu = cd;
	   m_outData -> o_truth_partons.at("lepW_nu") -> push_back(cd);
	   m_outData -> o_truth_partons_n.at("lepW_nu") ++;
	   list_inv.push_back(cd);
	   *(m_outData -> o_truth_partonMET) += *cd;
	 }
	 if(W1<1){
	   cd->SetMoment("isFromW1",1);
	 }
	 else{
	   cd->SetMoment("isFromW2",1);
	 }

       }//children of W
       if(leptonic){
	 if(lep && nu){
	   part -> SetMoment("dPhi_lepnu", TMath::Abs(lep->DeltaPhi(*nu)) );
	   part -> SetMoment("dEta_lepnu", TMath::Abs(lep->Eta() - nu->Eta()) );
	   part -> SetMoment("mT_lepnu", m_varComputer -> GetMTw(lep, nu) );
	 }
	 /*
	 else{
	   part -> SetMoment("dPhi_lepnu", -10. );
	   part -> SetMoment("dEta_lepnu", -99. );
	   part -> SetMoment("mT_lepnu", -1. );
	 }
	 */
       } //leptonic W
       else{
	 if(q1 && q2){
	   part -> SetMoment ("dR_qq", q1->DeltaR(*q2) ); 

	   m_outData -> o_truth_partons_n.at("hadW_q1") ++;
	   m_outData -> o_truth_partons_n.at("hadW_q2") ++;

	   if(q1->Pt() > q2->Pt()){ 
	     m_outData -> o_truth_partons.at("hadW_q1") -> push_back(q1);
	     m_outData -> o_truth_partons.at("hadW_q2") -> push_back(q2);
	   }
	   else{
	     m_outData -> o_truth_partons.at("hadW_q1") -> push_back(q2);
	     m_outData -> o_truth_partons.at("hadW_q2") -> push_back(q1);
	   }
	 }
	 /*
	 else{
	   part -> SetMoment ("dR_qq", -1. ); 
	 }
	 */
       } //hadronic W

       
     }//W

     else if(pdgIdPart==23){ //Z
       Z1++;
       AnalysisObject* zd1 = 0;
       AnalysisObject* zd2 = 0;

       bool leptonic = false; bool hadronic = false; bool invisible = false;
       for ( int iCd = 0; iCd < part -> GetMoment("children_n"); ++iCd ) {
	 AnalysisObject* cd = RetrieveChild(part, iCd);
	 if(!cd) continue;
	 if(cd -> GetMoment("absPdgId") <=6){ 
	   hadronic = true;
	   
	   if(cd -> GetMoment("absPdgId") ==5){ 
	     m_outData -> o_truth_all_b -> push_back(cd);
	   }
	   else{
	     m_outData -> o_truth_all_q -> push_back(cd);
	   }
	 }
	 else if( (cd -> GetMoment("absPdgId") == 11) || (cd -> GetMoment("absPdgId") == 13) || (cd -> GetMoment("absPdgId") == 15) ){
	   leptonic = true;
	   m_outData -> o_truth_all_lep -> push_back(cd);
	 }
	 else if( (cd -> GetMoment("absPdgId") == 12) || (cd -> GetMoment("absPdgId") == 14) || (cd -> GetMoment("absPdgId") == 16) ){
	   invisible = true;
	   *(m_outData -> o_truth_partonMET) += *cd;
	 }

	 if(!zd1) zd1 = cd;
	 else zd2 = cd;

	 if(Z1<1){
	   cd->SetMoment("isFromZ1",1);
	 }
	 else{
	   cd->SetMoment("isFromZ2",1);
	 }

       }// children of Z

       if(leptonic){
	 m_outData -> o_truth_partons.at("lepZ") -> push_back(part);
	 m_outData -> o_truth_partons_n.at("lepZ") ++;
	 if(zd1 && zd2){
	   part-> SetMoment ("dR_leplep", zd1->DeltaR(*zd2)); 

	   m_outData -> o_truth_partons_n.at("lepZ_lep1") ++;
	   m_outData -> o_truth_partons_n.at("lepZ_lep2") ++;

	   if(zd1->Pt() > zd2->Pt()){ 
	     m_outData -> o_truth_partons.at("lepZ_lep1") -> push_back(zd1);
	     m_outData -> o_truth_partons.at("lepZ_lep2") -> push_back(zd2);
	   }
	   else{
	     m_outData -> o_truth_partons.at("lepZ_lep1") -> push_back(zd2);
	     m_outData -> o_truth_partons.at("lepZ_lep2") -> push_back(zd1);
	   }

	 }
	 /*
	 else{
	 part-> SetMoment ("dR_leplep", -1.); 
	 }
	 */

       }
       else if(hadronic){
	 if(zd1 && zd2){
	   part-> SetMoment ("dR_qq", zd1->DeltaR(*zd2)); 
	   m_outData -> o_truth_partons.at("hadZ") -> push_back(part);
	   m_outData -> o_truth_partons_n.at("hadZ") ++;

	   m_outData -> o_truth_partons_n.at("hadZ_q1") ++;
	   m_outData -> o_truth_partons_n.at("hadZ_q2") ++;

	   if(zd1->Pt() > zd2->Pt()){ 
	     m_outData -> o_truth_partons.at("hadZ_q1") -> push_back(zd1);
	     m_outData -> o_truth_partons.at("hadZ_q2") -> push_back(zd2);
	   }
	   else{
	     m_outData -> o_truth_partons.at("hadZ_q1") -> push_back(zd2);
	     m_outData -> o_truth_partons.at("hadZ_q2") -> push_back(zd1);
	   }
	 }
	 /*
	 else{
	   part-> SetMoment ("dR_qq", -1.); 
	 }
	 */
       }
       else if(invisible){
	 m_outData -> o_truth_partons.at("invZ") -> push_back(part);
	 m_outData -> o_truth_partons_n.at("invZ") ++;
	 list_inv.push_back(part);
       }

     }//Z


   }//loop on ME particles

  for(AnalysisObject* inv : list_inv){
    double _dphi   = inv->DeltaPhi( *(m_outData->o_truth_partonMET) );
    double _ptfrac = inv->Pt() / m_outData->o_truth_partonMET->Pt();

    inv->SetMoment("dPhi_MET", _dphi);
    inv->SetMoment("frac_MET", _ptfrac);

  }
  list_inv.clear();
   
   return 1;

}

int VLQ_TruthManager::MatchJetsToBQuarks(AOVector& jetcol, const double DRMATCH){

  for( AnalysisObject* jet : jetcol ){

    int nmatch_bpartons = 0;
    for( AnalysisObject* bquark : *(m_outData->o_truth_all_bpartons) ){
      if(bquark->DeltaR(*jet) < DRMATCH){ nmatch_bpartons++; }
    }
    jet->SetMoment("nmatch_bpartons", nmatch_bpartons);

    int nmatch_btruth = 0;
    for( AnalysisObject* bquark : *(m_outData->o_truth_all_b) ){
      if(bquark->DeltaR(*jet) < DRMATCH){ nmatch_btruth++; }
    }
    jet->SetMoment("nmatch_btruth", nmatch_btruth);

  }

  return 0;

}

int VLQ_TruthManager::MatchPartonToFatJets(AOVector& fatjets, AnalysisObject* parton, const double DRMATCH, 
					   const bool rcCollection, 
					   const bool containment, const double DRCONTAINED,
					   const bool checkPt, const double minPtFrac, const double maxPtFrac){

  bool _debug = false;

  if(_debug){ 
    std::cout << " DEBUG :: VLQ_TruthManager::MatchPartonToFatJets : Entering. Parton ID =  " << parton ->GetMoment("absPdgId") << std::endl; 
    std::cout << " DEBUG :: VLQ_TruthManager::MatchPartonToFatJets : Reco collection multiplicity =  " << fatjets.size() << std::endl; 
    std::cout << " DEBUG :: VLQ_TruthManager::MatchPartonToFatJets : DRMATCH =  " << DRMATCH << std::endl; 
  }

  int nmatch = 0; int ncontained = 0;
  double drmin = DRMATCH;
  AnalysisObject* matched_jet = NULL;

  for(AnalysisObject* fjet : fatjets){
    double drcur = fjet->DeltaR(*parton);
    double fpT =  fjet->Pt()/parton->Pt();
	
    if(drcur < DRMATCH && ((!checkPt) || ( (fpT >= minPtFrac) && ( (maxPtFrac < 0) || (fpT < maxPtFrac) ) )) ){

      nmatch++;

      if(containment){
	bool isContained = true;
	//Make child loop
	for ( int iChild = 0; iChild < parton -> GetMoment("children_n"); ++iChild ) {
	  AnalysisObject *child = RetrieveChild(parton, iChild);
	  if(!child) continue;
	  if( fjet->DeltaR(*child) > DRCONTAINED ){
	    isContained = false;
	    break;
	  }
	}//contains all decay daughters

	if(isContained) ncontained++;
      }//checking for containment
 
      if(drcur < drmin){

	drmin = drcur;
	matched_jet = fjet; 

      }//closest match 

    }//matched in DR

  }//loop over fat jets

  if(_debug){
    std::cout << " DEBUG :: VLQ_TruthManager::MatchPartonToFatJets : drmin =  " << drmin << " nmatch_reco = " << nmatch << std::endl; 
  }

  parton -> SetMoment( "nmatch_reco", nmatch );
  parton -> SetMoment( "ncontained_reco", ncontained );

  if(matched_jet){
    parton -> SetMoment( "pT_reco", matched_jet -> Pt() );
    parton -> SetMoment( "fpT_reco", matched_jet -> Pt() / parton -> Pt() );
    parton -> SetMoment( "m_reco",  matched_jet -> M() );
    parton -> SetMoment( "dR_reco", drmin );

    if(rcCollection){
      parton -> SetMoment( "nconsts_reco", matched_jet -> GetMoment("nconsts") );
      parton -> SetMoment( "nbconsts_reco", matched_jet -> GetMoment("nbconsts") );
      parton -> SetMoment( "isRCMTop_reco", matched_jet -> GetMoment("isRCMTop") );
      parton -> SetMoment( "isRCMHiggs_reco", matched_jet -> GetMoment("isRCMHiggs") );
      parton -> SetMoment( "isRCMV_reco", matched_jet -> GetMoment("isRCMV") );
    }

    //
    // Update number of matched parton for the jet
    // Only if this parton is a better match, update the truth match information
    //
    int nprev_match = matched_jet -> IsKnownMoment("nmatch_truth") ? matched_jet -> GetMoment("nmatch_truth") : 0.;
    if( (nprev_match <= 0) || (drmin < fabs(matched_jet -> GetMoment("dR_truth"))) ){

      matched_jet -> SetMoment( "pT_truth", parton ->Pt() );
      matched_jet -> SetMoment( "fpT_truth", matched_jet -> Pt() / parton ->Pt() );
      matched_jet -> SetMoment( "pdgId_truth", parton ->GetMoment("absPdgId") );
      matched_jet -> SetMoment( "dR_truth", drmin );

      matched_jet -> SetMoment( "nmatch_truth", nprev_match+1 );
    }
  }// if a matching jet is found


  return 0;

}
 



AnalysisObject* VLQ_TruthManager::RetrieveChild(const AnalysisObject* parent, int index){

  AnalysisObject *cd = 0;
  
  std::string str_ind_cd = "children_";
  str_ind_cd += std::to_string(index);
  int cd_index = parent -> GetMoment(str_ind_cd);
  if(cd_index < (int)m_outData -> o_truth_all_particles -> size()){
    cd = m_outData -> o_truth_all_particles -> at(cd_index);
  }

  return cd;

}

int VLQ_TruthManager::CalculateTruthVariables(){


  /*

    2. Sort vectors by pT
    3. Calculate variables
   */

  //  1. Add b's to collection of q's
  m_outData -> o_truth_all_q -> insert ( m_outData -> o_truth_all_q -> end(), m_outData -> o_truth_all_b -> begin(), m_outData -> o_truth_all_b -> end() );

  //  2. Sort vectors by pT
  AOVector sorted_b = AnalysisUtils::SortObjectValues( *(m_outData->o_truth_all_b));
  AOVector sorted_q = AnalysisUtils::SortObjectValues( *(m_outData->o_truth_all_b));
  AOVector sorted_lep = AnalysisUtils::SortObjectValues( *(m_outData->o_truth_all_lep));

  AnalysisObject* truth_lep = (sorted_lep.size() > 0) ? sorted_lep.at(0) : 0;

  //o_truth_dRmin_isHiggs_b = 0;
  m_outData -> o_truth_dRmin_bb        = m_varComputer -> GetMindR( *(m_outData->o_truth_all_b), *(m_outData->o_truth_all_b) );
  m_outData -> o_truth_minv_bb         = m_varComputer -> GetMbb( *(m_outData->o_truth_all_b) );
  m_outData -> o_truth_dPhiMin_jetmet  = TMath::Abs(m_varComputer -> GetMindPhi( m_outData->o_truth_partonMET, *(m_outData->o_truth_all_q), 4 ));

  if(m_outData->o_truth_all_b && m_outData -> o_truth_partonMET->Pt() > 0.){
    m_outData -> o_truth_mtbmin          = m_varComputer -> GetMTbmin( *(m_outData->o_truth_all_b), m_outData -> o_truth_partonMET );
  }
  else{
    m_outData -> o_truth_mtbmin           = -1.;
  }

  if(truth_lep && m_outData -> o_truth_partonMET->Pt() > 0.){
    m_outData -> o_truth_dPhiMin_lepmet  = TMath::Abs( m_outData -> o_truth_partonMET -> DeltaR( *truth_lep ) );

    m_outData -> o_truth_mtw             = m_varComputer -> GetMTw( truth_lep, m_outData->o_truth_partonMET );
    m_outData -> o_truth_ptw             = m_varComputer -> GetPTw( truth_lep, m_outData->o_truth_partonMET );
  }
  else{
    m_outData -> o_truth_dPhiMin_lepmet  = -10.;

    m_outData -> o_truth_mtw             = -1.;
    m_outData -> o_truth_ptw             = -1.;
  }

  return 0;

}
