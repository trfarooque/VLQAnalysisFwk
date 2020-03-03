/**
   Example usage:
   VLQAnalysis --inputfile /var/tmp/gerbaudo/ntuples/user.lvalery.410000MC15b_7349564._000001.output_tree.root
 */
#include <iostream>
#include "TStopwatch.h"

#include "VLQAnalysis/VLQ_Analysis_Data2015.h"
#include "VLQAnalysis/VLQ_Options.h"

int main(int argc, char** argv){
    
    TStopwatch *timer = new TStopwatch();
    timer -> Start();
    
    VLQ_Options* opt = new VLQ_Options();
    opt->ParseUserOpts(argc, argv);
    
    VLQ_Analysis_Data2015 ana(opt);
    ana.Begin();
    ana.Loop();
    ana.Terminate();
    
    timer -> Stop();
    
    std::cout << "Processing time: " << timer -> RealTime() << " s" << std::endl;
    
    return 0;
}
