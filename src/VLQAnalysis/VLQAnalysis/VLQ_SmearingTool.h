#ifndef VLQ_SMEARINGTOOL_H
#define VLQ_SMEARINGTOOL_H

#include <string>
#include <map>

class TH1D;
class TH2D;
class TF1;

class VLQ_SmearingTool {

public:

    //
    // Standard C++ functions
    //
    VLQ_SmearingTool(  );
    VLQ_SmearingTool( const VLQ_SmearingTool & );
    ~VLQ_SmearingTool();

    //
    // Getter
    //


    //
    // Init
    //
    bool Init( const std::string &fileName, const std::string& histName, const double res_relative );

    //
    // Real function
    //
    double GetSmearFactor1D(const double param) const;

private:
    TH1D* m_smear_map;
    double m_res_relative;
};

#endif //VLQ_SMEARINGTOOL_H
