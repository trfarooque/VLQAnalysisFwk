#ifndef VLQ_MVAMANAGER_H
#define VLQ_MVAMANAGER_H

#include <map>
#include "TMVA/Reader.h"
#include "TMVA/Reader.h"

class VLQ_Options;
class VLQ_OutputData;
class VLQ_WeightManager;
class OutputHistManager;
class OutputTreeManager;
class VariableDef;


class VLQ_MVAManager{

  //Instantiate a struct with variables that will be relevant to MVA training.
  //This will include the variable name, short label, and default value
  //Read from output data; may need VariableDef to do this properly

  //Initiate tree for MVA training

  //Read MVA xml file with or without cross evaluation
  //Initiate the MVA reader with the correct list of input and spectator observables
  //

 public:

  VLQ_MVAManager(VLQ_Options* opt=NULL, VLQ_OutputData* outData=NULL, VLQ_WeightManager* weightMngr=NULL, 
		 OutputHistManager* outHistMngr=NULL, OutputTreeManager* outTreeMngr=NULL);
  VLQ_MVAManager();
  ~VLQ_MVAManager();
  VLQ_MVAManager(VLQ_MVAManager& q);


  int Init(const std::vector<std::string>& inputList={});
  int EvaluateMVA();

  const std::map<std::string, VariableDef*>& GetVariableStore(){ return m_varStore; }
  const std::map<std::string, VariableDef*>& GetMetadataStore(){ return m_metadataStore; } 

  int MakeVariableStore();
  int MakeInputVarList(const std::vector<std::string>& inputList={});
  int AddMVAVarTreeBranches();
  int AddMVAVarHistograms();

  int ReadXmlWeightFile();

 protected:

  VLQ_Options* m_opt;
  VLQ_OutputData* m_outputData;
  VLQ_WeightManager* m_weightMngr;
  OutputHistManager* m_outputHistMngr;
  OutputTreeManager* m_outputTreeMngr;

  TMVA::Reader* m_mvaReader;

  std::map<std::string, VariableDef*> m_varStore; // ALL VARIABLES TO BE DEFINED
  std::map<std::string, VariableDef*> m_metadataStore; // ALL METADATA TO BE DEFINED

  std::map<std::string, VariableDef*> m_inputVarList; //mva variables to add as histograms
  std::map<std::string, VariableDef*> m_readerVarList; //mva variables in reader
  std::map<std::string, VariableDef*> m_readerSpecList; //mva variables in reader

};

#endif
