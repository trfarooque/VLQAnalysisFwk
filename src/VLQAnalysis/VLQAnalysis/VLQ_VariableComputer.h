#ifndef VLQ_VARIABLECOMPUTER_H
#define VLQ_VARIABLECOMPUTER_H

#include <vector>

class AnalysisObject;
class OptionsBase;

class VLQ_VariableComputer {

  enum metric{
    DELTAR=1, DELTAETA, DELTAPHI 
  };
    
public:
    
    //
    // Standard C++ functions
    //
    VLQ_VariableComputer( OptionsBase* opt );
    VLQ_VariableComputer( const VLQ_VariableComputer & );
    ~VLQ_VariableComputer();
    
    //
    // Specific functions
    //
    double GetMeff( std::vector< AnalysisObject* > &v_jets,
                    std::vector< AnalysisObject* > &v_el,
                    std::vector< AnalysisObject* > &v_mu,
                    AnalysisObject *met) const;
    
    double GetHtHad( std::vector< AnalysisObject* > &v_jets, const std::string& mom="" ) const;
    double GetMJSum( std::vector< AnalysisObject* > &v_jets, const std::string& mom="" ) const;
    double GetMetSignificance( const double met, const double hthad) const;    
    double GetMTw( const AnalysisObject* lep,
                   const AnalysisObject* met ) const;

    double GetPTw( const AnalysisObject* lep,
                   const AnalysisObject* met ) const;

    double GetMTw( std::vector< AnalysisObject* > &v_el,
                   std::vector< AnalysisObject* > &v_mu,
                   AnalysisObject* met ) const;

    double GetPTw( std::vector< AnalysisObject* > &v_el,
                   std::vector< AnalysisObject* > &v_mu,
                   AnalysisObject* met ) const;
    
    AnalysisObject* GetClosestAO( AnalysisObject *v_obj1, std::vector< AnalysisObject* > &v_obj2, double& minval, const int metric=DELTAR, 
				  const int maxVec = -1, const std::string &mom = "" ) const ;

    double GetMindR( AnalysisObject *v_obj1, std::vector< AnalysisObject* > &v_obj2, const int maxVec = -1, const std::string &mom = "" ) const ;
    double GetMindR( std::vector< AnalysisObject* > &v_obj1,
                    std::vector< AnalysisObject* > &v_obj2,
                    const std::string &mom1 = "", const std::string &mom2 = "" ) const;

    double GetAveragedR( std::vector< AnalysisObject* > &v_obj1,
			 std::vector< AnalysisObject* > &v_obj2,
			 const std::string &mom1 = "", const std::string &mom2 = "" ) const;
    
    double GetMindPhi( AnalysisObject *v_obj1, std::vector< AnalysisObject* > &v_obj2, const int maxVec = -1, const std::string &mom = "" ) const ;
    
    double GetMindPhi( std::vector< AnalysisObject* > &v_obj1,
                      std::vector< AnalysisObject* > &v_obj2 ) const ;
    
    double GetMbb( std::vector< AnalysisObject* > &v_jets, const std::string &bjet_moment = "" ) const;
    double GetMbbMaxDr( std::vector< AnalysisObject* > &v_jets, const std::string &bjet_moment = "" ) const; 
    
    double GetDphibbLeadingBjets( std::vector< AnalysisObject* > &v_jets, const std::string &bjet_moment = "" ) const;
    double GetDphibbMinDr( std::vector< AnalysisObject* > &v_jets, const std::string &bjet_moment = "" ) const;
    double GetDphibbMaxDr( std::vector< AnalysisObject* > &v_jets, const std::string &bjet_moment = "" ) const;
    double GetDetabbLeadingBjets( std::vector< AnalysisObject* > &v_jets, const std::string &bjet_moment = "" ) const;
    double GetDetabbMinDr( std::vector< AnalysisObject* > &v_jets, const std::string &bjet_moment = "" ) const; 
    double GetDetabbMaxDr( std::vector< AnalysisObject* > &v_jets, const std::string &bjet_moment = "" ) const;

    double GetMjjLeadingJets( std::vector< AnalysisObject* > &v_jets ) const;
    double GetMjjMinDr ( std::vector< AnalysisObject* > &v_jets ) const;
    double GetMjjMaxDr ( std::vector< AnalysisObject* > &v_jets ) const;
    double GetDphijjLeadingJets( std::vector < AnalysisObject* > &v_jets ) const;
    double GetDphijjMinDr( std::vector< AnalysisObject* > &v_jets ) const;
    double GetDphijjMaxDr( std::vector< AnalysisObject* > &v_jets ) const;
    double GetDetajjLeadingJets( std::vector < AnalysisObject* > &v_jets ) const;
    double GetDetajjMinDr( std::vector<AnalysisObject*> &v_jets ) const;
    double GetDetajjMaxDr( std::vector<AnalysisObject*> &v_jets ) const;  
 
    int GetNbjets( std::vector< AnalysisObject* > &v_jets, const std::string &bjet_moment = "" ) const;

    int GetNjets( std::vector< AnalysisObject* > &v_jets, double ptcut ) const;
    double GetCentrality( std::vector< AnalysisObject* > &v_jets, std::vector< AnalysisObject* > &v_el, std::vector< AnalysisObject* > &v_mu ) const;
    double GetMbbLeadingBjets( std::vector< AnalysisObject* > &v_jets, const std::string &bjet_moment = "" ) const;
    double GetMbbSoftestBjets( std::vector< AnalysisObject* > &v_jets, const std::string &bjet_moment = "" ) const;
    double GetJLeptonInvariantMass(std::vector< AnalysisObject* > &v_RCTTMjets, std::vector< AnalysisObject* > &v_el, std::vector< AnalysisObject* > &v_mu ) const;
    double GetJLeadingBInvariantMass(std::vector< AnalysisObject* > &v_RCTTMjets,  std::vector< AnalysisObject* > &v_jets, const std::string &bjet_moment = ""  ) const;
    double GetJJInvariantMass( std::vector< AnalysisObject* > &v_RCTTMjets ) const;

    double GetMTbmin( std::vector< AnalysisObject* > &v_jets, AnalysisObject *met, const std::string &mom = "" ) const;
    int GetJetClass(AnalysisObject* jet) const;
    
private:
    OptionsBase* m_opt;
    
};

#endif// VLQ_VARIABLECOMPUTER_H
