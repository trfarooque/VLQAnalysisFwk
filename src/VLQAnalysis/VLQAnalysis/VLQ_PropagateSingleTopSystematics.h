#ifndef VLQ_PROPAGATESINGLETOPSYSTEMATICS_H
#define VLQ_PROPAGATESINGLETOPSYSTEMATICS_H
#include <vector>
#include <map>
#include <string>

class VLQ_Options;
class OutputHistManager;
class TFile;
class Selection;

class VLQ_PropagateSingleTopSystematics {

 public:
  VLQ_PropagateSingleTopSystematics ( const VLQ_Options* opt, OutputHistManager* outMngrHist);
  ~VLQ_PropagateSingleTopSystematics();
  bool Init( const std::map<int, Selection*>* selection_tree, const std::string& fileName );
  bool WriteAllSystHistograms(const bool writeToFile=true, const bool newFile=false);
  bool MakeSystHistogram(const std::string& region, const std::string& variable, TFile* outFile=NULL);

 protected:
  const VLQ_Options* m_opt;
  OutputHistManager* m_outMngrHist;

  std::vector<std::string> m_varList; //The variables that need systematics
  std::vector<std::string> m_regList; //The regions that need systematics
  std::map<std::string, TH1F*> m_sourceHists; //The list of source histograms
};
#endif 
