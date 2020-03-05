atlas_subdir ( VLQAnalysis )


atlas_depends_on_subdirs( PUBLIC
			  IFAETopFramework
			  IFAEReweightingTools
			  BtaggingTRWandRW
			  FakeLeptonEstimation
)

find_package( ROOT REQUIRED COMPONENTS Core RIO Hist)

atlas_add_root_dictionary( VLQAnalysis _cintDictSource
   ROOT_HEADERS VLQAnalysis/*.h Root/LinkDef.h
   EXTERNAL_PACKAGES ROOT )

atlas_add_library( VLQAnalysis Root/*.cxx Root/*.h 
		   VLQAnalysis/*h ${_cintDictSource}
		   PUBLIC_HEADERS VLQAnalysis
		   LINK_LIBRARIES IFAETopFramework
				  IFAEReweightingTools
				  BtaggingTRFandRW
				  FakeLeptonEstimation
				  ${ROOT_LIBRARIES}
		   INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} )

atlas_install_data( data/* )
atlas_install_scripts( scripts/*py )
# Install the Python modules of the package:
atlas_install_python_modules( python/*.py )

atlas_add_executable( VLQAna
		      util/VLQAnalysis.cxx
		      LINK_LIBRARIES IFAETopFramework
				     IFAEReweightingTools
				     BtaggingTRFandRW
				     FakeLeptonEstimation
				     ${ROOT_LIBRARIES}
				     VLQAnalysis)

atlas_add_executable( MakeQCDFile
                      util/MakeQCDFile.cxx
                      LINK_LIBRARIES IFAETopFramework
                                     IFAEReweightingTools
                                     BtaggingTRFandRW
                                     FakeLeptonEstimation
                                     ${ROOT_LIBRARIES}
                                     VLQAnalysis)
