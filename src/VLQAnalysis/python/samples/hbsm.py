
from sample import Sample

def build_samples_list(base_dir=None):
    signals = [
        Sample(short_name='bbH_m1000',
               full_name='user.gerbaudo.343434.MadGraphPythia8EvtGen.DAOD_TOPQ1.e4776_a766_a810_r6282_p2454.HtX4Tops_00-00-10_output.root',
               filelist_dir=base_dir,
               ds_input='mc15_13TeV.343434.MadGraphPythia8EvtGen_bbH_m1000_tanb10.merge.DAOD_TOPQ1.e4776_a766_a810_r6282_p2454',
               ),
        Sample(short_name='ttH_m400' ,
               full_name='user.gerbaudo.341547.aMcAtNloPythia8EvtGen.DAOD_TOPQ1.e4336_a766_a810_r6282_p2501.HtX4Tops_00-00-10_output.root',
               filelist_dir=base_dir,
               ds_input='mc15_13TeV.341547.aMcAtNloPythia8EvtGen_A14NNPDF23LO_Hplus4FS_H400_tb.merge.DAOD_TOPQ1.e4336_a766_a810_r6282_p250',
               ),
        Sample(short_name='ttH_m700' ,
               full_name='user.gerbaudo.304780.MadGraphPythia8EvtGen.DAOD_TOPQ1.e4791_s2726_r7326_r6282_p2516.HtX4Tops_00-00-10_output.root',
               filelist_dir=base_dir,
               ds_input='mc15_13TeV.304780.MadGraphPythia8EvtGen_A14_NNPDF23LO_4t2HDM_700.merge.DAOD_TOPQ1.e4791_s2726_r7326_r6282_p2516',
               ),
        Sample(short_name='tbH_m200',
               full_name='user.gerbaudo.341541.aMcAtNloPythia8EvtGen.DAOD_TOPQ1.e4336_a766_a810_r6282_p2501.HtX4Tops_00-00-10_output.root',
               filelist_dir=base_dir,
               ds_input='mc15_13TeV.341541.aMcAtNloPythia8EvtGen_A14NNPDF23LO_Hplus4FS_H200_tb.merge.DAOD_TOPQ1.e4336_a766_a810_r6282_p2501',
               ),
        Sample(short_name='tbH_m500',
               full_name='user.gerbaudo.341548.aMcAtNloPythia8EvtGen.DAOD_TOPQ1.e4336_a766_a810_r6282_p2501.HtX4Tops_00-00-10_output.root',
               filelist_dir=base_dir,
               ds_input='mc15_13TeV.341548.aMcAtNloPythia8EvtGen_A14NNPDF23LO_Hplus4FS_H500_tb.merge.DAOD_TOPQ1.e4336_a766_a810_r6282_p2501',
               ),
        Sample(short_name='tbH_m700',
               full_name='user.gerbaudo.341550.aMcAtNloPythia8EvtGen.DAOD_TOPQ1.e4336_a766_a810_r6282_p2501.HtX4Tops_00-00-10_output.root',
               filelist_dir=base_dir,
               ds_input='mc15_13TeV.341550.aMcAtNloPythia8EvtGen_A14NNPDF23LO_Hplus4FS_H700_tb.merge.DAOD_TOPQ1.e4336_a766_a810_r6282_p2501',
               ),
        Sample(short_name='tbH_m1000',
               full_name='user.gerbaudo.341553.aMcAtNloPythia8EvtGen.DAOD_TOPQ1.e4336_a766_a810_r6282_p2501.HtX4Tops_00-00-10_output.root',
               filelist_dir=base_dir,
               ds_input='mc15_13TeV.341553.aMcAtNloPythia8EvtGen_A14NNPDF23LO_Hplus4FS_H1000_tb.merge.DAOD_TOPQ1.e4336_a766_a810_r6282_p2501',
               ),

    # # mc15_13TeV.343432.MadGraphPythia8EvtGen_bbA_m1000_tanb10.merge.DAOD_TOPQ1.e4776_a766_a810_r6282_p2454
    # # user.gerbaudo.343432.MadGraphPythia8EvtGen.DAOD_TOPQ1.e4776_a766_a810_r6282_p2454.HtX4Tops_00-00-10_output.root/
        ]
    return signals

if __name__=='__main__':
    import pprint
    pprint.pprint(build_samples_list())
