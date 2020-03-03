#include <iostream>

#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/AnalysisObject.h"

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
double VLQ_VariableComputer::GetMindR(  AOVector &v_obj1, AOVector &v_obj2, const std::string &mom1, const std::string &mom2 ) const
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
    dRaverage=dRaverage/npairs;
    return dRaverage;
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

//_________________________________________________________________
//
int VLQ_VariableComputer::GetJetClass(AnalysisObject* jet) const
{
    if(!jet){
        return 0;
    }
    
    int ntop1=0,ntop2=0,nltop1=0,nltop2=0,nW1=0,nW2=0,nZ1=0,nZ2=0,nH1=0,nHbb1=0,nH2=0,nHbb2=0,nB1=0,nB2=0,nukn=0;
    
    if(jet->IsKnownMoment("nFromTop1")){
        ntop1= (int) jet->GetMoment("nFromTop1");
    }
    if(jet->IsKnownMoment("nFromTop2")){
        ntop2= (int) jet->GetMoment("nFromTop2");
    }
    if(jet->IsKnownMoment("nFromLepTop1")){
        nltop1= (int) jet->GetMoment("nFromLepTop1");
    }
    if(jet->IsKnownMoment("nFromLepTop2")){
        nltop2= (int) jet->GetMoment("nFromLepTop2");
    }
    if(jet->IsKnownMoment("nFromW1")){
        nW1= (int) jet->GetMoment("nFromW1");
    }
    if(jet->IsKnownMoment("nFromW2")){
        nW2= (int) jet->GetMoment("nFromW2");
    }
    if(jet->IsKnownMoment("nFromZ1")){
        nZ1= (int) jet->GetMoment("nFromZ1");
    }
    if(jet->IsKnownMoment("nFromZ2")){
        nZ2= (int) jet->GetMoment("nFromZ2");
    }
    if(jet->IsKnownMoment("nFromH1")){
        nH1= (int) jet->GetMoment("nFromH1");
    }
    if(jet->IsKnownMoment("nFromH2")){
        nH2= (int) jet->GetMoment("nFromH2");
    }
    if(jet->IsKnownMoment("nFromHbb1")){
        nHbb1= (int) jet->GetMoment("nFromHbb1");
    }
    if(jet->IsKnownMoment("nFromHbb2")){
        nHbb2= (int) jet->GetMoment("nFromHbb2");
    }
    if(jet->IsKnownMoment("nFromB1")){
        nB1= (int) jet->GetMoment("nFromB1");
    }
    if(jet->IsKnownMoment("nFromB2")){
        nB2= (int) jet->GetMoment("nFromB2");
    }
    if(jet->IsKnownMoment("nFromUkn")){
        nukn= (int) jet->GetMoment("nFromUkn");
    }
    
    bool top1in= ntop1==2;
    bool top2in= ntop2==2;
    
    bool ltop1in= nltop1>0;
    bool ltop2in= nltop2>0;
    
    bool W1in= nW1==2;
    bool W2in= nW2==2;
    
    bool Z1in= nZ1==2;
    bool Z2in= nZ2==2;
    
    bool H1in= nH1==2;
    bool H2in= nH2==2;
    
    bool Hbb1in= nHbb1>0;
    bool Hbb2in= nHbb2>0;
    
    bool B1in= nB1==1;
    bool B2in= nB2==1;
    
    bool top1=top1in && ntop2==0 && nW1==0 && nW2==0 && nZ1==0 && nZ2==0 && nH1==0 && nH2==0 && nB1==0 && nB2==0 && nukn==0;
    bool top2=ntop1==0 && top2in && nW1==0 && nW2==0 && nZ1==0 && nZ2==0 && nH1==0 && nH2==0 && nB1==0 && nB2==0 && nukn==0;
    bool W1=ntop1==0 && ntop2==0 && W1in && nW2==0 && nZ1==0 && nZ2==0 && nH1==0 && nH2==0 && nB1==0 && nB2==0 && nukn==0;
    bool W2=ntop1==0 && ntop2==0 && nW1==0 && W2in && nZ1==0 && nZ2==0 && nH1==0 && nH2==0 && nB1==0 && nB2==0 && nukn==0;
    bool Z1=ntop1==0 && ntop2==0 && nW1==0 && nW2==0 && Z1in && nZ2==0 && nH1==0 && nH2==0 && nB1==0 && nB2==0 && nukn==0;
    bool Z2=ntop1==0 && ntop2==0 && nW1==0 && nW2==0 && nZ1==0 && Z2in && nH1==0 && nH2==0 && nB1==0 && nB2==0 && nukn==0;
    bool H1=ntop1==0 && ntop2==0 && nW1==0 && nW2==0 && nZ1==0 && nZ2==0 && H1in && nH2==0 && nB1==0 && nB2==0 && nukn==0;
    bool H2=ntop1==0 && ntop2==0 && nW1==0 && nW2==0 && nZ1==0 && nZ2==0 && nH1==0 && H2in && nB1==0 && nB2==0 && nukn==0;
    bool B1=ntop1==0 && ntop2==0 && nW1==0 && nW2==0 && nZ1==0 && nZ2==0 && nH1==0 && nH2==0 && B1in && nB2==0 && nukn==0;
    bool B2=ntop1==0 && ntop2==0 && nW1==0 && nW2==0 && nZ1==0 && nZ2==0 && nH1==0 && nH2==0 && nB1==0 && B2in && nukn==0;
    bool extra=ntop1==0 && ntop2==0 && nW1==0 && nW2==0 && nZ1==0 && nZ2==0 && nH1==0 && nH2==0 && nB1==0 && nB2==0 && nukn==0;
    bool ukn=ntop1==0 && ntop2==0 && nW1==0 && nW2==0 && nZ1==0 && nZ2==0 && nH1==0 && nH2==0 && nB1==0 && nB2==0 && nukn>0;
    
    
    
    if(top1in){
        if(top1){
            if(ltop1in){
                return 4;
            }
            return 3;
        }
        else{
            if(ltop1in){
                return 2;
            }
            return 1;
        }
    }
    else if(top2in){
        if(top2){
            if(ltop2in){
                return 8;
            }
            return 7;
        }
        else{
            if(ltop2in){
                return 6;
            }
            return 5;
        }
        
        
    }
    else if(W1in){
        if(W1){return 10;}
        return 9;
    }
    else if(W2in){
        if(W2){return 12;}
        return 11;
    }
    else if(Z1in){
        if(Z1){return 14;}
        return 13;
    }
    else if(Z2in){
        if(Z2){return 16;}
        return 15;
    }
    else if(H1in){
        if(H1){
            if(Hbb1in){return 20;}
            return 19;
        }
        else{
            if(Hbb1in){return 18;}
            return 17;
            
        }
    }
    else if(H2in){
        if(H2){
            if(Hbb2in){return 24;}
            return 23;
        }
        else{
            if(Hbb2in){return 22;}
            return 21;
        }
    }
    else if(B1in){
        if(B1){return 26;}
        return 25;
    }
    else if(B2in){
        if(B2){return 28;}
        return 27;
    }
    else if(extra){
        return 29;
    }
    else if(ukn){
        return 31;
    }
    else{
        return 30;
    }
}
