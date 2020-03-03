#include <iostream>
#include "TChain.h"
#include "TBranch.h"
#include <map>
#include <set>

int CheckDuplicates(){
  TChain *ch = new TChain("nominal");
  ch -> Add("/pnfs/pic.es/data/at3/disk/VLQ/MBJ_VLQ_00-00-05/user.lvalery.periodD.data.DAOD_SUSY10.grp15_v01_p2425_VLQ_00_00_05_output_tree.root/*.root*");
  ch -> Add("/pnfs/pic.es/data/at3/disk/VLQ/MBJ_VLQ_00-00-05/user.lvalery.periodE.data.DAOD_SUSY10.grp15_v01_p2425_VLQ_00_00_05_output_tree.root/*.root*");
  ch -> Add("/pnfs/pic.es/data/at3/disk/VLQ/MBJ_VLQ_00-00-05/user.lvalery.periodF.data.DAOD_SUSY10.grp15_v01_p2425_VLQ_00_00_05_output_tree.root/*.root*");
  ch -> Add("/pnfs/pic.es/data/at3/disk/VLQ/MBJ_VLQ_00-00-05/user.lvalery.periodG.data.DAOD_SUSY10.grp15_v01_p2425_VLQ_00_00_05_output_tree.root/*.root*");
  ch -> Add("/pnfs/pic.es/data/at3/disk/VLQ/MBJ_VLQ_00-00-05/user.lvalery.periodH.data.DAOD_SUSY10.grp15_v01_p2425_VLQ_00_00_05_output_tree.root/*.root*");
  ch -> Add("/pnfs/pic.es/data/at3/disk/VLQ/MBJ_VLQ_00-00-05/user.lvalery.periodJ.data.DAOD_SUSY10.grp15_v01_p2425_VLQ_00_00_05_output_tree.root/*.root*");

  int event_number = 0;
  int run_number = 0;
  TBranch *b_run = 0;
  TBranch *b_event = 0;

  ch -> SetBranchStatus("*",0);
  ch ->SetBranchStatus("event_number",1);  ch ->SetBranchStatus("run_number",1);
  ch -> SetBranchAddress("event_number",&event_number,&b_event);
  ch ->SetBranchAddress("run_number",&run_number,&b_run);

  std::map < int, std::set < int > > run_event;

  for ( unsigned int i = 0; i < ch -> GetEntries(); ++i ){
    ch -> GetEntry(i);
    if(i%10000 == 0 ){
	std::cout << "=> Ran over " << i << " events out of " << ch -> GetEntries() << std::endl;
    }
    
    if( run_event.find(run_number) != run_event.end() ){
      if(run_event[run_number].find(event_number) != run_event[run_number].end() ){
	std::cout << "<!> ERROR: Run number " << run_number << " and event number: " << event_number << " is duplicated !!" << std::endl;
      } else {
	run_event[run_number].insert(event_number);
      }
    } else {
      std::set < int > temp;
      temp.insert(event_number);
      run_event.insert( std::pair < int, std::set < int > > ( run_number, temp ) );
    }
  }
  
  return 1;
}
