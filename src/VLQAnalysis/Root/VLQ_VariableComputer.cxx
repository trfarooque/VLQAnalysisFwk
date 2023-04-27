#include <iostream>
#include <iostream>
#include <set>

#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/AnalysisObject.h"
#include "IFAETopFramework/AnalysisUtils.h"

#include "VLQAnalysis/VLQ_VariableComputer.h"

//________________________________________________________________
//
VLQ_VariableComputer::VLQ_VariableComputer( OptionsBase *opt ):
  m_opt(opt)
{}

//________________________________________________________________
//
VLQ_VariableComputer::VLQ_VariableComputer( const VLQ_VariableComputer &q )
{
    m_opt = q.m_opt;
}

//________________________________________________________________
//
VLQ_VariableComputer::~VLQ_VariableComputer()
{}

//________________________________________________________________
//
double VLQ_VariableComputer::GetMeff(  AOVector &v_jets, AOVector &v_el, AOVector &v_mu, AnalysisObject *met ) const
{
    if(!met){
        std::cerr << "<!> Error in VLQ_VariableComputer::GetMeff(): the MET pointer is null ! Aborting !" << std::endl;
        abort();
    }
    double meff = 0.;
    for ( const AnalysisObject* jet : v_jets ){ meff += jet -> Pt(); }
    for ( const AnalysisObject* el : v_el ){  meff += el -> Pt(); }
    for ( const AnalysisObject* mu : v_mu ){ meff += mu -> Pt(); }
    meff += met -> Pt();
    return meff;
}

//________________________________________________________________ 
//
double VLQ_VariableComputer::GetMeffRed( AOVector&v_jets, AOVector &v_el, AOVector &v_mu, AnalysisObject *met, int Njets_shift, double Pt_Scale ) const
{
  
  double meff = GetMeff(v_jets, v_el, v_mu, met);

  double meff_reduced = meff - ( ( ( static_cast<int>( v_jets.size() ) ) - Njets_shift ) * Pt_Scale );

  return meff_reduced;

}

//________________________________________________________________
//
double VLQ_VariableComputer::GetHtHad(  AOVector &v_jets, const std::string& mom ) const
{
    double ht = 0.;
    for ( const AnalysisObject* jet : v_jets ){ 
      if(mom!="" && !(int)jet->GetMoment(mom)) continue;
      ht += jet -> Pt(); 
    }
    return ht;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetHtHad6j(  AOVector &v_jets, const std::string& mom ) const
{
  double ht = 0;
  for(unsigned int i = 5; i < v_jets.size(); i++) ht += v_jets[i]->Pt();
  return ht;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetMJSum(  AOVector &v_jets, const std::string& mom ) const
{
    double ht = 0.;
    for ( const AnalysisObject* jet : v_jets ){ 
      if(mom!="" && !(int)jet->GetMoment(mom)) continue;
      ht += jet -> M(); 
    }
    return ht;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetMetSignificance( const double met, const double hthad ) const
{
  double met_sig = 0.; 
  if(hthad > 0.){ met_sig = met/sqrt(hthad); }
  else if(hthad < 0.){ met_sig = met/sqrt(-hthad); } 
  return met_sig;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetMTw(  AOVector &v_el,  AOVector &v_mu, AnalysisObject* met ) const
{
    if(!met){
        std::cerr << "<!> Error in VLQ_VariableComputer::GetMTw(): the MET pointer is null ! Aborting !" << std::endl;
        abort();
    }
    
    if((v_el.size()+v_mu.size())==0) return -1;
    
    const AnalysisObject *lep(0);
    double ptMax = 0;
    for( const AnalysisObject* el : v_el ){
        if(el -> Pt() > ptMax){
            ptMax = el->Pt();
            lep = el;
        }
    }
    for( const AnalysisObject* mu : v_mu ){
        if(mu -> Pt() > ptMax){
            ptMax = mu->Pt();
            lep = mu;
        }
    }

    return GetMTw(lep, met);
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetPTw(  AOVector &v_el,  AOVector &v_mu, AnalysisObject* met ) const
{
    if(!met){
        std::cerr << "<!> Error in VLQ_VariableComputer::GetPTw(): the MET pointer is null ! Aborting !" << std::endl;
        abort();
    }
    
    if((v_el.size()+v_mu.size())==0) return -1;
    
    const AnalysisObject *lep(0);
    double ptMax = 0;
    for( const AnalysisObject* el : v_el ){
        if(el -> Pt() > ptMax){
            ptMax = el->Pt();
            lep = el;
        }
    }
    for( const AnalysisObject* mu : v_mu ){
        if(mu -> Pt() > ptMax){
            ptMax = mu->Pt();
            lep = mu;
        }
    }

    return GetPTw(lep,met);
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetMTw(  const AnalysisObject* lep, const AnalysisObject* met ) const
{
    if(!met){
        std::cerr << "<!> Error in VLQ_VariableComputer::GetMTw(): the MET pointer is null ! Aborting !" << std::endl;
        abort();
    }
    if(!lep){
        std::cerr << "<!> Error in VLQ_VariableComputer::GetMTw(): the lep pointer is null ! Aborting !" << std::endl;
        abort();
    }
    
    double mt = 2*lep->Pt()*met->Pt()*(1-cos(lep -> DeltaPhi( *met )));
    return TMath::Sqrt(mt);
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetPTw(  const AnalysisObject* lep, const AnalysisObject* met ) const
{
    if(!met){
        std::cerr << "<!> Error in VLQ_VariableComputer::GetPTw(): the MET pointer is null ! Aborting !" << std::endl;
        abort();
    }
    if(!lep){
        std::cerr << "<!> Error in VLQ_VariableComputer::GetPTw(): the lep pointer is null ! Aborting !" << std::endl;
        abort();
    }
    
    double pt = lep->Pt()*lep->Pt() + met->Pt()*met->Pt() + 2*lep->Pt()*met->Pt()*cos(lep -> DeltaPhi(*met));
    return TMath::Sqrt(pt);
}

//________________________________________________________________
//
AnalysisObject* VLQ_VariableComputer::GetClosestAO( AnalysisObject *obj1, std::vector< AnalysisObject* > &v_obj2, double& minval, const int metric,
			      const int maxVec, const std::string &mom ) const 
{

  if(!obj1){ return NULL; }
  AnalysisObject* minobj = NULL;

  //The user is responsible for setting the maximum allowed value of the metric through minval

  int counter = 0;
  for ( AnalysisObject* obj2 : v_obj2 ){
    if(obj1==obj2) continue;
    if(mom!="" && !(int)obj2->GetMoment(mom)) continue;
    if(maxVec>=0 && counter>=maxVec) continue;
    double curval = 0.;
    if(metric == DELTAR) curval = fabs(obj1 -> DeltaR( *obj2 ));
    if(metric == DELTAETA) curval = fabs(obj1 -> DeltaPhi( *obj2 ));
    if(metric == DELTAPHI) curval = fabs(obj1 -> Eta() - obj2 -> Eta() );

    if(curval < minval){ 
      minval = curval;
      minobj = obj2;
    }
    counter++;
  }
  return minobj;


}

//________________________________________________________________
//
double VLQ_VariableComputer::GetMindR( AnalysisObject *obj1, AOVector &v_obj2, const int maxVec, const std::string &mom ) const
{
    if(!obj1){
        return 0;
    }
    
    double dR_min = 100;
    int counter = 0;
    for ( const AnalysisObject* obj2 : v_obj2 ){
        if(obj1==obj2) continue;
        if(mom!="" && !(int)obj2->GetMoment(mom)) continue;
        if(maxVec>=0 && counter>=maxVec) continue;
        double dR = obj1 -> DeltaR( *obj2 );
        if(TMath::Abs(dR) < TMath::Abs(dR_min)) dR_min = dR;
        counter++;
    }
    return dR_min;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetMindR(  AOVector &v_obj1, AOVector &v_obj2, const std::string &mom1, const std::string &mom2) const
{
    double dRmin = 100;
    for ( const AnalysisObject* obj1 : v_obj1 ){
        if(mom1!="" && !(int)obj1->GetMoment(mom1)) continue;
        for ( const AnalysisObject* obj2 : v_obj2 ){
            if(obj1==obj2) continue;
            if(mom2!="" && !(int)obj2->GetMoment(mom2)) continue;
            double dR = obj1 -> DeltaR( *obj2 );
            if(dR < dRmin) dRmin = dR;
        }
    }
    return dRmin;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetLeadingdR( AOVector &v_obj1, AOVector &v_obj2 ) const
{
  
  double dR = -100.;

  if(v_obj1.size() >=1 && v_obj2.size() >=1){

    if(v_obj1[0] != v_obj2[0]){
      dR = v_obj1[0] -> DeltaR( *v_obj2[0] );
    }
    else if(v_obj2.size() > 1){
      dR = v_obj1[0] -> DeltaR( *v_obj2[1] );
    }

  }

  return dR;
}


//________________________________________________________________
//
double VLQ_VariableComputer::GetLeadingdEta( AOVector &v_obj1, AOVector &v_obj2 ) const
{
  
  double dEta = -100.;

  if(v_obj1.size() >=1 && v_obj2.size() >=1){

    if(v_obj1[0] != v_obj2[0]){
      dEta =  TMath::Abs(v_obj1[0]->Eta() - v_obj2[0]->Eta());

    }
    else if(v_obj2.size() > 1){ //shoudnt this be obj2 ?--> I changed it.  
      dEta =  TMath::Abs(v_obj1[0]->Eta() - v_obj2[1]->Eta());
    }

  }

  return dEta;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetLeadingdPhi(  AnalysisObject *obj1,  AOVector &v_obj2, const int maxVec, const std::string &mom ) const
{
  if(!obj1){
    return 0;
  }

  double dPhi = -100.;

  if(v_obj2.size() >=1){

    if(obj1 != v_obj2[0]){
      dPhi = obj1 -> DeltaPhi( *v_obj2[0] );

    }

  }

  return dPhi;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetLeadingdPhi( AOVector &v_obj1, AOVector &v_obj2 ) const
{
  
  double dPhi = -100.;

  if(v_obj1.size() >=1 && v_obj2.size() >=1){

    if(v_obj1[0] != v_obj2[0]){
      dPhi = v_obj1[0] -> DeltaPhi( *v_obj2[0] );

    }
    else if(v_obj2.size() > 1){
      dPhi = v_obj1[0] -> DeltaPhi( *v_obj2[1] );
    }
  }

  return dPhi;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetAveragedR(  AOVector &v_obj1, AOVector &v_obj2, const std::string &mom1, const std::string &mom2 ) const
{
    double dRaverage = 0.;
    unsigned int npairs = 0;
    for ( const AnalysisObject* obj1 : v_obj1 ){
        if(mom1!="" && !(int)obj1->GetMoment(mom1)) continue;
        for ( const AnalysisObject* obj2 : v_obj2 ){
            if(obj1==obj2) continue;
            if(mom2!="" && !(int)obj2->GetMoment(mom2)) continue;
	    dRaverage += obj1 -> DeltaR( *obj2 );
	    npairs ++;
        }
    }
    dRaverage = (npairs > 0) ? dRaverage/npairs : 10.;
    return dRaverage;
}


//________________________________________________________________
//
double VLQ_VariableComputer::GetAveragedEta(  AOVector &v_obj1, AOVector &v_obj2, const std::string &mom1, const std::string &mom2 ) const
{
    double dEtaaverage = 0.;
    unsigned int npairs = 0;
    for ( const AnalysisObject* obj1 : v_obj1 ){
        if(mom1!="" && !(int)obj1->GetMoment(mom1)) continue;
        for ( const AnalysisObject* obj2 : v_obj2 ){
            if(obj1==obj2) continue;
            if(mom2!="" && !(int)obj2->GetMoment(mom2)) continue;
	    dEtaaverage += TMath::Abs(obj2->Eta() - obj1->Eta());
	    npairs ++;
        }
    }
    dEtaaverage = (npairs > 0) ? dEtaaverage/npairs : 10.;
    return dEtaaverage;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetAveragedPhi(AnalysisObject *obj1, AOVector &v_obj2, const int maxVec, const std::string &mom ) const
{
  if(!obj1){
    return 0;
  }
  double dPhiaverage = 0.;
  unsigned int npairs = 0;
  int counter=0;
  for ( const AnalysisObject* obj2 : v_obj2 ){
    if(obj1==obj2) continue;
    if(mom!="" && !(int)obj2->GetMoment(mom)) continue;
    if(maxVec>=0 && counter>=maxVec) continue;
    dPhiaverage += TMath::Abs( obj1 -> DeltaPhi( *obj2 ));
    counter++;
    npairs ++;
    
  }
  dPhiaverage = (npairs > 0) ? dPhiaverage/npairs : 10.;
  return dPhiaverage;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetAveragedPhi(  AOVector &v_obj1, AOVector &v_obj2, const std::string &mom1, const std::string &mom2 ) const
{
    double dPhiaverage = 0.;
    unsigned int npairs = 0;
    for ( const AnalysisObject* obj1 : v_obj1 ){
        if(mom1!="" && !(int)obj1->GetMoment(mom1)) continue;
        for ( const AnalysisObject* obj2 : v_obj2 ){
            if(obj1==obj2) continue;
            if(mom2!="" && !(int)obj2->GetMoment(mom2)) continue;
	    dPhiaverage += TMath::Abs( obj1 -> DeltaPhi( *obj2 ));
	    npairs ++;
        }
    }
    dPhiaverage = (npairs > 0) ? dPhiaverage/npairs : 10.;
    return dPhiaverage;
}


//_________________________________________________________________
//
AOVector VLQ_VariableComputer::GetMinMAsymmPair(std::vector< AnalysisObject* > &v_obj) const{

  AOVector ret_objs = GetMinMAsymmPair(v_obj, v_obj);

  return ret_objs;

}  

//_________________________________________________________________
//
/*
  Get the two pairs with minimum mass asymmetry
  Only unique pairings are allowed - i.e. an object cannot be in two pairs in the sorted list
  Decorate objects with a tag indicating their pair membership
  Return mass asymmetry value
*/

AOVector VLQ_VariableComputer::GetMinMAsymmPair(std::vector< AnalysisObject* > &v_obj1,
					      std::vector< AnalysisObject* > &v_obj2) const{
  
  double masymm = -1.;
  AOVector ret_objs{};

  std::vector<double> vpair_val{};
  std::vector<std::pair<AnalysisObject*, AnalysisObject*> > vpair_obj{};

  //Create pairs of AnalysisObjects and calculate their invariant masses
  for(AnalysisObject* obj1 : v_obj1){

    for(AnalysisObject* obj2 : v_obj2){
      if( (obj1 == obj2) ||
	  (std::find(vpair_obj.begin(), vpair_obj.end(), 
		     std::pair<AnalysisObject*, AnalysisObject*>(obj2,obj1)) != vpair_obj.end()) ){
	continue;
      }
      double minv = ((*obj1)+(*obj2)).M();
      vpair_obj.push_back(std::pair<AnalysisObject*, AnalysisObject*>(obj1,obj2));
      vpair_val.push_back(minv);
    }

  }

  //Need at least two pairs for the computation to be sensible
  if( vpair_val.size() < 2 ){
    return ret_objs;
  }

  //Re-loop over the list of AnalysisObject pairs, and calculate inter-pair mass asymmetries
  std::pair<AnalysisObject*, AnalysisObject*> pair1;
  std::pair<AnalysisObject*, AnalysisObject*> pair2;

  for(int i=0; i < vpair_val.size(); i++){

    std::pair<AnalysisObject*, AnalysisObject*> _pair1 = vpair_obj.at(i);

    for(int j=i; j < vpair_val.size(); j++){

      std::pair<AnalysisObject*, AnalysisObject*> _pair2 = vpair_obj.at(j);

      if( (_pair1.first == _pair2.first) || (_pair1.second == _pair2.second)
	  || (_pair1.first == _pair2.second) || (_pair1.second == _pair2.first) ) continue;

      double masymm_cur = TMath::Abs(vpair_val.at(i) - vpair_val.at(j));
      if( (masymm < 0.) || (masymm_cur < masymm) ){
	masymm = masymm_cur;
	pair1 = _pair1;
	pair2 = _pair2;
      }

    }

  }


  //======= Tag the objects in the selected pair ======
  if(pair1.first){
    pair1.first -> SetMoment("MAsymmIdx", 0);
    ret_objs.push_back(pair1.first);
  }
  if(pair1.second){ 
    pair1.second -> SetMoment("MAsymmIdx", 1);
    ret_objs.push_back(pair1.second);
  }
  if(pair2.first){
    pair2.first -> SetMoment("MAsymmIdx", 2);
    ret_objs.push_back(pair2.first);
  }
  if(pair2.second){
    pair2.second -> SetMoment("MAsymmIdx", 3);
     ret_objs.push_back(pair2.second);
   }

  return ret_objs;
  
}

//_________________________________________________________________
//
std::vector<double> VLQ_VariableComputer::GetInvariantMassSorted(std::vector< AnalysisObject* > &v_obj,
                                                                 const std::string& sort_by, int n_sort, bool b_descending) const{

  std::vector<double> ret_vals{};
  ret_vals = GetInvariantMassSorted(v_obj, v_obj, sort_by, n_sort, b_descending);

  return ret_vals;

}

//_________________________________________________________________
//
/*
  Get invariant masses from a sorted list of object pairs
  Only unique pairings are allowed - i.e. an object cannot be in two pairs in the sorted list
  Sorting can be done according to individual object pT, or by pairwise metrics (DeltaR, DeltaEta, DeltaPhi, fpT)
*/

std::vector<double> VLQ_VariableComputer::GetInvariantMassSorted(std::vector< AnalysisObject* > &v_obj1,
                                                                 std::vector< AnalysisObject* > &v_obj2,
                                                                 const std::string& sort_by, int n_sort, bool b_descending) const{

  std::vector<double> ret_vals{};
  int index=0;

  std::set<std::pair<AnalysisObject*, AnalysisObject*> > used_pairs;
  std::set<AnalysisObject*> used_objs;

  //Sort individual jets by their pT
  if( (sort_by=="") || (sort_by=="Pt") ){

    AOVector v_obj1_ptsort = AnalysisUtils::SortObjectValues(v_obj1, "Pt", -1, b_descending);
    AOVector v_obj2_ptsort = AnalysisUtils::SortObjectValues(v_obj2, "Pt", -1, b_descending);

    index = 0;
    for(AnalysisObject* obj1 : v_obj1_ptsort){

      for(AnalysisObject* obj2 : v_obj2_ptsort){
        if((n_sort>=0) && (index >= n_sort)) break;
        index++;

        if( (obj1 == obj2) || (used_objs.find(obj1) != used_objs.end()) || (used_objs.find(obj2) != used_objs.end()) ) continue;
        double minv = (*(obj1) + *(obj2)).M();
        ret_vals.push_back(minv);
        used_objs.insert(obj1);
        used_objs.insert(obj2);
      }

    }
    used_objs.clear();
    v_obj1_ptsort.clear();
    v_obj2_ptsort.clear();

  }//pt-sorting

  else if( (sort_by=="DeltaR") || (sort_by=="DeltaPhi") || (sort_by=="DeltaEta") || (sort_by=="fpT") ){

    std::vector<std::pair<int, double> > vpair_val{};
    std::vector<std::pair<AnalysisObject*, AnalysisObject*> > vpair_obj{};

    index = 0;
    for(AnalysisObject* obj1 : v_obj1){
      for(AnalysisObject* obj2 : v_obj2){

        if( (obj1 == obj2) ||
            (used_pairs.find(std::pair<AnalysisObject*, AnalysisObject*>(obj2,obj1)) != used_pairs.end()) ){
          continue;
        }

        double pair_val = 0.;
        if(sort_by=="DeltaR")         pair_val = obj1->DeltaR(*obj2);
        else if(sort_by=="DeltaPhi")  pair_val = TMath::Abs(obj1->DeltaR(*obj2));
        else if(sort_by=="DeltaEta")  pair_val = TMath::Abs(obj1->Eta() - obj2->Eta());
        else if(sort_by=="fpT")       pair_val = (obj1->Pt() < obj2->Pt()) ? obj1->Pt()/obj2->Pt() : obj2->Pt()/obj1->Pt();

        vpair_val.push_back(std::pair<int, double>(index, pair_val));
        vpair_obj.push_back(std::pair<AnalysisObject*, AnalysisObject*>(obj1, obj2));

        index++;
        used_pairs.insert(std::pair<AnalysisObject*, AnalysisObject*>(obj1,obj2));

      }
    }
    used_pairs.clear();

    AnalysisUtils::SortVectorPairs(vpair_val, -1, b_descending);


    index = 0;
    for(std::pair<int, double> ipair : vpair_val){

      if( (n_sort>=0) && (index >=n_sort) ) break;
      index++;

      std::pair<AnalysisObject*, AnalysisObject*> obj_pair = vpair_obj.at(ipair.first);
      if( used_objs.find(obj_pair.first) != used_objs.end() ) continue;
      if( used_objs.find(obj_pair.second) != used_objs.end() ) continue;

      double minv = (*(obj_pair.first) + *(obj_pair.second)).M();
      ret_vals.push_back(minv);
      used_objs.insert(obj_pair.first);
      used_objs.insert(obj_pair.second);

    }
    vpair_val.clear();
    vpair_obj.clear();
    used_objs.clear();


  }//pair-wise metrics


  return ret_vals;

}


//________________________________________________________________ 
//
double VLQ_VariableComputer::GetMinPairVLQMassAsymm( std::map< std::string, AOVector*  > &recoVLQs, const std::string vlqDecay1, const std::string vlqDecay2) const{

  double minMAsymm = -100.;

  if(vlqDecay1 == vlqDecay2){
    
    if(recoVLQs[vlqDecay1]->size() < 2) return minMAsymm;

    minMAsymm = TMath::Abs(recoVLQs[vlqDecay1]->at(0)->M() - recoVLQs[vlqDecay1]->at(1)->M());

    for(int i = 0; i < recoVLQs[vlqDecay1]->size(); i++){
      
      for(int j = i+1; j < recoVLQs[vlqDecay1]->size(); j++ ){
	
	double mAsymm = TMath::Abs(recoVLQs[vlqDecay1]->at(i)->M() - recoVLQs[vlqDecay1]->at(j)->M());

	if(minMAsymm > mAsymm) minMAsymm = mAsymm;

      }

    }

  }
  else{

    if(recoVLQs[vlqDecay1]->size() == 0 || recoVLQs[vlqDecay2]->size() == 0) return minMAsymm;

    minMAsymm = TMath::Abs(recoVLQs[vlqDecay1]->at(0)->M() - recoVLQs[vlqDecay2]->at(0)->M());
    
    for(int i = 0; i < recoVLQs[vlqDecay1]->size(); i++){

      for(int j = 0; j < recoVLQs[vlqDecay2]->size(); j++){
	
	double mAsymm = TMath::Abs(recoVLQs[vlqDecay1]->at(i)->M() - recoVLQs[vlqDecay2]->at(j)->M());
	
	if(minMAsymm > mAsymm) minMAsymm = mAsymm;
      
      }

    }

  }

  return minMAsymm;

}

//________________________________________________________________
//
double VLQ_VariableComputer::GetMindPhi( AnalysisObject *obj1, AOVector &v_obj2, const int maxVec, const std::string &mom ) const
{
  if(!obj1){
    return 0;
  }
  
  double dPhi_min = 100;
  int counter = 0;
  for ( const AnalysisObject* obj2 : v_obj2 ){
    if(obj1==obj2) continue;
    if(mom!="" && !(int)obj2->GetMoment(mom)) continue;
    if(maxVec>=0 && counter>=maxVec) continue;
    double dPhi = obj1 -> DeltaPhi( *obj2 );
    if(TMath::Abs(dPhi) < TMath::Abs(dPhi_min)) dPhi_min = dPhi;
    counter++;
  }
  return dPhi_min;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetMindPhi(  AOVector &v_obj1, AOVector &v_obj2 ) const
{
    double dPhi_min = 100;
    for ( const AnalysisObject* obj1 : v_obj1 ){
        for ( const AnalysisObject* obj2 : v_obj2 ){
            if(obj1==obj2) continue;
            double dPhi = obj1 -> DeltaPhi( *obj2 );
            if(TMath::Abs(dPhi) < TMath::Abs(dPhi_min)) dPhi_min = dPhi;
        }
    }
    return dPhi_min;
}


//________________________________________________________________
//
double VLQ_VariableComputer::GetMindEta( AnalysisObject *obj1, AOVector &v_obj2, const int maxVec, const std::string &mom ) const
{
    if(!obj1){
        return 0;
    }
    
    double dEta_min = 100;
    int counter = 0;
    for ( const AnalysisObject* obj2 : v_obj2 ){
        if(obj1==obj2) continue;
        if(mom!="" && !(int)obj2->GetMoment(mom)) continue;
        if(maxVec>=0 && counter>=maxVec) continue;
        double dEta = TMath::Abs(obj1->Eta() - obj2->Eta() );
        if(TMath::Abs(dEta) < TMath::Abs(dEta_min)) dEta_min = dEta;
        counter++;
    }
    return dEta_min;
}


//________________________________________________________________
//
double VLQ_VariableComputer::GetMindEta(  AOVector &v_obj1, AOVector &v_obj2 ) const
{
    double dEta_min = 100;
    for ( const AnalysisObject* obj1 : v_obj1 ){
        for ( const AnalysisObject* obj2 : v_obj2 ){
            if(obj1==obj2) continue;
            double dEta = TMath::Abs(obj1->Eta() - obj2->Eta() );                 
            if(TMath::Abs(dEta) < TMath::Abs(dEta_min)) dEta_min = dEta;
        }
    }

    return dEta_min;
}


//________________________________________________________________
//
double VLQ_VariableComputer::GetMbb( AOVector &v_jets, const std::string &bjets_moment ) const
{
    double mbb = 0;
    double dRmin = 10;
    
    for ( const AnalysisObject *jet1 : v_jets ){
        if( bjets_moment!="" && (int)jet1 -> GetMoment(bjets_moment) != 1 ) continue;
        
        for( const AnalysisObject *jet2 : v_jets ){
            if( jet1 == jet2 ) continue;
            if( bjets_moment!="" && (int)jet2 -> GetMoment(bjets_moment) != 1 ) continue;
            
            double dR = jet1 -> DeltaR( *jet2 );
            if(dR < dRmin){
                dRmin = dR;
                mbb = ( *jet1 + *jet2 ).M();
            }
        }
    }
    return mbb;
}
//________________________________________________________________
//
double VLQ_VariableComputer::GetMbbMaxDr(AOVector &v_jets, const std::string &bjets_moment ) const
{ 
    double mbb_max_dr=0.; 
    double dRmax = 0.; 

    for( const AnalysisObject *jet1 : v_jets ){
        if( bjets_moment!="" && (int)jet1 -> GetMoment(bjets_moment) != 1 ) continue;
         
        for( const AnalysisObject *jet2 : v_jets ){
	  if(jet1 == jet2 ) continue;
          if( bjets_moment!="" && (int)jet2 -> GetMoment(bjets_moment) !=1 ) continue;              
          double dR = jet1 -> DeltaR (*jet2) ;
          if(dR > dRmax){
	      dRmax = dR;
              mbb_max_dr = ( *jet1 + *jet2 ).M();
          }
        }
    }	
    return mbb_max_dr;
}
//________________________________________________________________
//
double VLQ_VariableComputer::GetDphibbLeadingBjets(AOVector &v_jets, const std::string &bjets_moment ) const
{
  double dphibb_leading_bjets = 0. ;
  AOVector bjets;
  for (AnalysisObject *jet : v_jets ){
    if( bjets_moment!="" && (int)jet -> GetMoment(bjets_moment) != 1) continue;
    bjets.push_back(jet);
  }
  if (bjets.size()>=2){
    dphibb_leading_bjets = TMath::Abs(bjets[0] -> DeltaPhi(*bjets[1]));
  }
  return dphibb_leading_bjets;
}
//_________________________________________________________________
//
double VLQ_VariableComputer::GetDphibbMinDr(AOVector &v_jets, const std::string &bjets_moment ) const
{
  double dphibb_min_dr=0.;
  double dRmin=10.; 

  for ( const AnalysisObject *jet1 : v_jets){
    if( bjets_moment!="" &&(int)jet1 -> GetMoment(bjets_moment) != 1 ) continue;

    for ( const AnalysisObject *jet2 : v_jets){
      if( jet1 == jet2 ) continue; 
      if ( bjets_moment!="" && (int)jet2 -> GetMoment(bjets_moment) != 1 ) continue; 
      double dR = jet1 -> DeltaR( *jet2 ) ;
      if ( dR < dRmin){
	dRmin = dR ;
	dphibb_min_dr = TMath::Abs(jet1 -> DeltaPhi( *jet2));
      }
    }
  }
  return dphibb_min_dr;
}
//_________________________________________________________________
//
double VLQ_VariableComputer::GetDphibbMaxDr(AOVector &v_jets, const std::string &bjets_moment ) const
{
  double dphibb_max_dr =0.;
  double dRmax = 0.;

  for ( const AnalysisObject *jet1 : v_jets){
    if ( bjets_moment!="" &&(int)jet1 -> GetMoment(bjets_moment) != 1 ) continue;

    for (const AnalysisObject *jet2 : v_jets){
      if(jet1 == jet2) continue;
      if ( bjets_moment!="" &&(int)jet2 -> GetMoment(bjets_moment ) !=1 ) continue;
      double dR = jet1 -> DeltaR( *jet2 );
      if (dR > dRmax){
	dRmax = dR;
	dphibb_max_dr = TMath::Abs(jet1 -> DeltaPhi( *jet2 ));
      }
    }
  }
  return dphibb_max_dr;
}
//_________________________________________________________________
//
double VLQ_VariableComputer::GetDetabbLeadingBjets(AOVector &v_jets, const std::string &bjets_moment ) const
{
  double deta_leading_bjets= 0.;
  AOVector bjets; 
  for (AnalysisObject *jet : v_jets ){
    if( bjets_moment!="" && (int)jet -> GetMoment(bjets_moment) != 1) continue;
    bjets.push_back(jet);
  }
  if (bjets.size()>=2){
    deta_leading_bjets = (bjets[0]->Eta() - bjets[1]->Eta() ); 
  }
  return deta_leading_bjets;
} 
//_________________________________________________________________
//
double VLQ_VariableComputer::GetDetabbMinDr(AOVector &v_jets, const std::string &bjets_moment ) const
{
  double detabb_min_dr=0.;
  double dRmin=10.;

  for ( const AnalysisObject *jet1 : v_jets){
    if( bjets_moment!="" &&(int)jet1 -> GetMoment(bjets_moment) != 1 ) continue;

    for ( const AnalysisObject *jet2 : v_jets){
      if( jet1 == jet2 ) continue;
      if ( bjets_moment!="" && (int)jet2 -> GetMoment(bjets_moment) != 1 ) continue;
      double dR = jet1 -> DeltaR( *jet2 ) ;
      if ( dR < dRmin){
        dRmin = dR ;
        detabb_min_dr =(jet1->Eta()- jet2->Eta());
      }
    }
  }
  return detabb_min_dr;
}
//_________________________________________________________________
//
double VLQ_VariableComputer::GetDetabbMaxDr(AOVector &v_jets, const std::string &bjets_moment ) const
{
  double detabb_max_dr=0.;
  double dRmax=0.;

  for ( const AnalysisObject *jet1 : v_jets){
    if( bjets_moment!="" &&(int)jet1 -> GetMoment(bjets_moment) != 1 ) continue;

    for ( const AnalysisObject *jet2 : v_jets){
      if( jet1 == jet2 ) continue;
      if ( bjets_moment!="" && (int)jet2 -> GetMoment(bjets_moment) != 1 ) continue;
      double dR = jet1 -> DeltaR( *jet2 ) ;
      if ( dR > dRmax){
        dRmax = dR ;
        detabb_max_dr =(jet1->Eta()- jet2->Eta());
      }
    }
  }
  return detabb_max_dr;

}
//_________________________________________________________________
//
double VLQ_VariableComputer::GetMjjMaxDr(AOVector &v_jets) const
{ 
  double mjj_max_dr=0.;
  double dRmax=0.;
  for (const AnalysisObject *jet1 : v_jets ){
    for (const AnalysisObject *jet2 : v_jets ){
      if (jet1 == jet2) continue;
      double dR = jet1-> DeltaR( *jet2);
      if( dR >dRmax){
	dRmax =dR;
	mjj_max_dr=(*jet1 + *jet2).M();
      }
    }
  }
  return mjj_max_dr;
}

//_________________________________________________________________
//
double VLQ_VariableComputer::GetMjjMaxDphi(AOVector &v_jets) const
{ 
  double mjj_max_dphi=0.;
  double dphimax=0.;
  for (const AnalysisObject *jet1 : v_jets ){
    for (const AnalysisObject *jet2 : v_jets ){
      if (jet1 == jet2) continue;
      double dphi = TMath::Abs( jet1-> DeltaPhi( *jet2));
      if( dphi >dphimax){
	dphimax =dphi;
	mjj_max_dphi=(*jet1 + *jet2).M();
      }
    }
  }
  return mjj_max_dphi;
}
//_________________________________________________________________
//
double VLQ_VariableComputer::GetMjjMinDr(AOVector &v_jets) const
{
  double mjj_min_dr=0.;
  double dRmin=10.;
  for (const AnalysisObject *jet1 : v_jets ){
    for (const AnalysisObject *jet2 : v_jets ){
      if (jet1 == jet2) continue;
      double dR = jet1-> DeltaR( *jet2);
      if( dR <dRmin){
        dRmin =dR;
        mjj_min_dr=(*jet1 + *jet2).M();
      }
    }
  }
  return mjj_min_dr;
}


//_________________________________________________________________
//
double VLQ_VariableComputer::GetMjjMinDeta(AOVector &v_jets) const
{
  double mjj_min_deta=0.;
  double detamin=10.;
  for (const AnalysisObject *jet1 : v_jets ){
    for (const AnalysisObject *jet2 : v_jets ){
      if (jet1 == jet2) continue;
      double deta = TMath::Abs( jet1->Eta() - jet2->Eta() );
      if( deta <detamin){
        detamin =deta;
        mjj_min_deta=(*jet1 + *jet2).M();
      }
    }
  }
  return mjj_min_deta;
}
//_________________________________________________________________
//
double VLQ_VariableComputer::GetMjjLeadingJets(AOVector &v_jets) const
{
  double mjj_leading_jets = 0. ;
  if (v_jets.size()>=2){
    mjj_leading_jets =( *v_jets[0] + *v_jets[1] ).M();
  }
  return mjj_leading_jets;
}
//_________________________________________________________________
//
double VLQ_VariableComputer::GetDphijjLeadingJets( AOVector &v_jets ) const
{
  double dphijj_leading_jets = 0. ;
  if (v_jets.size()>=2){
    dphijj_leading_jets =TMath::Abs(v_jets[0] -> DeltaPhi( *v_jets[1]));
  }
  return dphijj_leading_jets;
}
//_________________________________________________________________
//
double VLQ_VariableComputer::GetDphijjMinDr( AOVector &v_jets ) const
{
  double dphijj_min_dr=0.;
  double dRmin=10.;
  for (const AnalysisObject *jet1 : v_jets ){
    for (const AnalysisObject *jet2 : v_jets ){
      if (jet1 == jet2) continue;
      double dR = jet1-> DeltaR( *jet2);
      if( dR <dRmin){
        dRmin =dR;
        dphijj_min_dr = TMath::Abs(jet1->DeltaPhi( *jet2 ));
      }
    }
  }
  return dphijj_min_dr;
}
//_________________________________________________________________
//
double VLQ_VariableComputer::GetDphijjMaxDr( AOVector &v_jets ) const
{
  double dphijj_max_dr=0.;
  double dRmax=0.;
  for (const AnalysisObject *jet1 : v_jets ){
    for (const AnalysisObject *jet2 : v_jets ){
      if (jet1 == jet2) continue;
      double dR = jet1-> DeltaR( *jet2);
      if( dR >dRmax){
        dRmax =dR;
        dphijj_max_dr=TMath::Abs(jet1->DeltaPhi(*jet2 ));
      }
    }
  }
  return dphijj_max_dr;
}
//_________________________________________________________________
//
double VLQ_VariableComputer::GetDetajjLeadingJets( AOVector &v_jets ) const
{
  double detajj_leading_jets = 0. ;
  if (v_jets.size()>=2){
    detajj_leading_jets =v_jets[0]->Eta()-v_jets[1]->Eta();
  }
  return detajj_leading_jets;
}
//_________________________________________________________________
//
double VLQ_VariableComputer::GetDetajjMinDr(AOVector &v_jets) const
{
  double detajj_min_dr=0.;
  double dRmin=10.;
  for (const AnalysisObject *jet1 : v_jets ){
    for (const AnalysisObject *jet2 : v_jets ){
      if (jet1 == jet2) continue;
      double dR = jet1-> DeltaR( *jet2);
      if( dR <dRmin){
        dRmin =dR;
        detajj_min_dr=jet1->Eta()-jet2->Eta();
      }
    }
  }
  return detajj_min_dr;
}
//_________________________________________________________________
//
double VLQ_VariableComputer::GetDetajjMaxDr( AOVector &v_jets) const
{
  double detajj_max_dr=0.;
  double dRmax=0.;
  for (const AnalysisObject *jet1 : v_jets ){
    for (const AnalysisObject *jet2 : v_jets ){
      if (jet1 == jet2) continue;
      double dR = jet1-> DeltaR( *jet2);
      if( dR >dRmax){
        dRmax =dR;
        detajj_max_dr=jet1->Eta()-jet2->Eta();
      }
    }
  }
  return detajj_max_dr;
}
//_________________________________________________________________
//
int VLQ_VariableComputer::GetNbjets( AOVector &v_jets, const std::string &bjets_moment ) const
{
    int count = 0;
    for ( const AnalysisObject *jet1 : v_jets ){
        if( bjets_moment!="" && (int)jet1 -> GetMoment(bjets_moment) != 1 ) continue;
        count++;
    }
    return count;
}

//________________________________________________________________
//
int VLQ_VariableComputer::GetNjets( AOVector &v_jets, double ptcut ) const
{
    int count = 0;
    for ( const AnalysisObject *jet : v_jets ){
      if( jet->Pt() < ptcut ) continue;
        count++;
    }
    return count;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetCentrality( AOVector &v_jets, AOVector &v_el, AOVector &v_mu ) const
{
  double centrality = 0.;
  double transverse_energy = 0.;
  double energy = 0.;
  for ( const AnalysisObject* jet : v_jets ){ transverse_energy += jet -> Pt(); energy += jet -> E(); }
  for ( const AnalysisObject* el : v_el ){ transverse_energy += el -> Pt(); energy += el -> E(); }
  for ( const AnalysisObject* mu : v_mu ){ transverse_energy += mu -> Pt(); energy += mu -> E(); }
  centrality=transverse_energy/energy;  
  return centrality;
}

//________________________________________________________________
//Not efficienct, can we use b-tagged jets directly as input?
double VLQ_VariableComputer::GetMbbLeadingBjets( AOVector &v_jets, const std::string &bjets_moment ) const
{
  double mbb_leading_bjets = 0.;
  AOVector bjets;
  for ( AnalysisObject *jet : v_jets ){
    if( bjets_moment!="" && (int)jet -> GetMoment(bjets_moment) != 1 ) continue;
    bjets.push_back(jet);
  }
  if(bjets.size()>=2){
    //Get the leading b-jets -> already pt sorted in the ntuples
    mbb_leading_bjets=( *bjets[0] + *bjets[1] ).M();
  }
  return mbb_leading_bjets;
}

//________________________________________________________________
//Not efficienct, can we use b-tagged jets directly as input?
double VLQ_VariableComputer::GetMbbSoftestBjets( AOVector &v_jets, const std::string &bjets_moment ) const
{
  double mbb_softest_bjets = 0.;
  AOVector bjets;
  for ( AnalysisObject *jet : v_jets ){
    if( bjets_moment!="" && (int)jet -> GetMoment(bjets_moment) != 1 ) continue;
    bjets.push_back(jet);
  }
  if(bjets.size()>=2){
    //Get the softest b-jets -> already pt sorted in the ntuples
    mbb_softest_bjets=( *bjets[bjets.size()-1] + *bjets[bjets.size()-2] ).M();
  }
  return mbb_softest_bjets;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetJLeptonInvariantMass( AOVector &v_RCTTMjets, AOVector &v_el, AOVector &v_mu ) const
{
  double J_lepton_invariant_mass = 0.;
  //Here keep the leading RCTTM jet
  if( v_RCTTMjets.size()!=0 && v_el.size()!=0 ) J_lepton_invariant_mass = ( *v_RCTTMjets[0] + *v_el[0] ).M();
  else if( v_RCTTMjets.size()!=0 && v_mu.size()!=0 ) J_lepton_invariant_mass = ( *v_RCTTMjets[0] + *v_mu[0] ).M();
  return J_lepton_invariant_mass;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetJLeadingBInvariantMass( AOVector &v_RCTTMjets, AOVector &v_jets, const std::string &bjets_moment ) const
{
  double J_leadingb_invariant_mass = 0.;
  
  AOVector bjets;
  for ( AnalysisObject *jet : v_jets ){
    if( bjets_moment!="" && (int)jet -> GetMoment(bjets_moment) != 1 ) continue;
    bjets.push_back(jet);
  }
  if(bjets.size()!=0 && v_RCTTMjets.size()!=0){
    //Get the hardest b-jet -> already pt sorted in the ntuples
    J_leadingb_invariant_mass = ( *bjets[0] + *v_RCTTMjets[0] ).M();
  }
  return J_leadingb_invariant_mass;
}

//________________________________________________________________
//
double VLQ_VariableComputer::GetJJInvariantMass( AOVector &v_RCTTMjets ) const
{
  double J_J_invariant_mass = 0.;
  if(v_RCTTMjets.size()<2) return J_J_invariant_mass;
  J_J_invariant_mass= ( *v_RCTTMjets[0] + *v_RCTTMjets[1] ).M();
  return J_J_invariant_mass;
}

//____________________________________________________________________
//
double VLQ_VariableComputer::GetMTbmin( AOVector &v_jets, AnalysisObject *met, const std::string &mom ) const
{
    double mTbmin = 10000.;
    
    int nbjets = 0;
    for ( const AnalysisObject* jet : v_jets ){
        if( mom!="" && ( (int)jet -> GetMoment(mom) != 1 ) ) continue;
        nbjets++;
        if(nbjets>3) break;
        double mTb = pow(met->Pt()+jet->Pt(),2) - pow(met->Px() + jet->Px(), 2) - pow(met->Py() + jet->Py(), 2);
        mTb = (mTb >= 0.) ? sqrt(mTb) : sqrt(-mTb);
        if(mTb<mTbmin) mTbmin = mTb;
    }
    
    if(nbjets==0){return 0.;}
    
    return mTbmin;
}
