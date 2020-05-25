#!/bin/bash

inputDir=$1

fit_regions_1l=( "HTX_c1lep3_5jwin1bex1fjin0LTex0Hex1Vin"
    "HTX_c1lep3_5jwin1bex1fjin0Tex1Lin0Hex1Vin"
    "HTX_c1lep3_5jwin2bex1fjin0LTex0Hex1Vin"
    "HTX_c1lep3_5jwin2bex1fjin0Tex1Lin0Hex1Vin"
    "HTX_c1lep3_5jwin3bex1fjin0LTex1Hin0Vex"
    "HTX_c1lep3_5jwin3bex1fjin0Tex1Lin1Hin0Vex"
    "HTX_c1lep3_5jwin3bex1fjin1Tin0Lex1Hin0Vex"
    "HTX_c1lep3_5jwin4bin1fjin0LTex1Hin0Vex"
    "HTX_c1lep3_5jwin4bin1fjin0Tex1Lin1Hin0Vex"
    "HTX_c1lep3_5jwin4bin1fjin1Tin0Lex1Hin0Vex"
    "HTX_c1lep6jin1bex1fjin1Lex0Tex0Hex1Vin"
    "HTX_c1lep6jin1bex1fjin0Lex1Tex0Hex1Vin"
    "HTX_c1lep6jin1bex1fjin2LTin0Hex1Vin"
    "HTX_c1lep6jin2bex1fjin1Lex0Tex0Hex1Vin"
    "HTX_c1lep6jin2bex1fjin0Lex1Tex0Hex1Vin"
    "HTX_c1lep6jin2bex1fjin2LTin0Hex1Vin"
    "HTX_c1lep6jin3bex1fjin1Lex0VTex1Hin"
    "HTX_c1lep6jin3bex1fjin0Lex1VTex1Hin"
    "HTX_c1lep6jin3bex1fjin2VLTin1Hin"
    "HTX_c1lep6jin4bin1fjin1Lex0VTex1Hin"
    "HTX_c1lep6jin4bin1fjin0Lex1VTex1Hin"
    "HTX_c1lep6jin4bin1fjin2VLTin1Hin"
    "HTX_c1lep3_5jwin4bin0fjex1Lin0VTex0Hex"
    "HTX_c1lep6jin4bin0fjex1Lin0VTex0Hex" )

validation_regions_1l=( "HTX_c1lep3_5jwin1bex0fjex0Tex0Lex0Hex1Vin"
    "HTX_c1lep3_5jwin1bex0fjex0Tex1Lin0Hex1Vin"
    "HTX_c1lep3_5jwin1bex1fjin1LTin0Hex0Vex"
    "HTX_c1lep3_5jwin1bex1fjin1Tin0Lex0Hex1Vin"
    "HTX_c1lep3_5jwin2bex0fjex0Tex0Lex0Hex1Vin"
    "HTX_c1lep3_5jwin2bex0fjex0Tex1Lin0Hex1Vin"
    "HTX_c1lep3_5jwin2bex1fjin1LTin0Hex0Vex"
    "HTX_c1lep3_5jwin2bex1fjin1Tin0Lex0Hex1Vin"
    "HTX_c1lep3_5jwin3bex0fjex0Tex0Lex1Hin0Vex"
    "HTX_c1lep3_5jwin3bex0fjex0Tex1Lin1Hin0Vex"
    "HTX_c1lep3_5jwin3bex1fjin1VLTin0Hex"
    "HTX_c1lep3_5jwin4bin0fjex0Tex0Lex1Hin0Vex"
    "HTX_c1lep3_5jwin4bin0fjex0Tex1Lin1Hin0Vex"
    "HTX_c1lep3_5jwin4bin1fjin1VLTin0Hex"
    "HTX_c1lep6jin1bex0fjex1LTex0Hex1Vin"
    "HTX_c1lep6jin1bex0fjex2LTin0Hex1Vin"
    "HTX_c1lep6jin1bex1fjin0Tex0Lex1Hin1Vin"
    "HTX_c1lep6jin1bex1fjin2LTin1Hin0Vex"
    "HTX_c1lep6jin2bex0fjex1LTex0Hex1Vin"
    "HTX_c1lep6jin2bex0fjex2LTin0Hex1Vin"
    "HTX_c1lep6jin2bex1fjin0Tex0Lex1Hin1Vin"
    "HTX_c1lep6jin2bex1fjin2LTin1Hin0Vex"
    "HTX_c1lep6jin3bex0fjex1VLTex1Hin"
    "HTX_c1lep6jin3bex0fjex2VLTin1Hin"
    "HTX_c1lep6jin3bex1fjin1VLTex0Hex"
    "HTX_c1lep6jin3bex1fjin2VLTin0Hex"
    "HTX_c1lep6jin4bin0fjex1VLTex1Hin"
    "HTX_c1lep6jin4bin1fjin1VLTex0Hex"
    "HTX_c1lep6jin4bin1fjin2VLTin0Hex" )



mkdir -p ${inputDir}/SR
mkdir -p ${inputDir}/VR

echo "MOVING SR PLOTS"
for region in ${fit_regions_1l[@]}
do
    #ls ${inputDir}/${region}* 
    mv ${inputDir}/${region}* ${inputDir}/SR/.
    #echo ""
done
#echo ""
echo "MOVING VR PLOTS"

for region in ${validation_regions_1l[@]}
do
    #ls ${inputDir}/${region}* 
    mv ${inputDir}/${region}* ${inputDir}/VR/.
    #echo ""
done

