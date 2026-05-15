#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <lcms2.h>
}
//<ID> 453
//<Prompt> []
/*<Combination>: [
*/
//<score> 260, nr_unique_branch: 68
//<Quality> {"density":26,"unique_branches":{"FixColorSpaces":[[1038,9,1038,44,0,0,4,1]],"CheckOne":[[1164,37,1164,48,0,0,4,0],[1164,37,1164,48,0,0,4,1],[1166,13,1166,30,0,0,4,0],[1166,13,1166,30,0,0,4,1],[1167,13,1167,51,0,0,4,0],[1167,13,1167,51,0,0,4,1]],"TetrahedralInterp16":[[762,13,762,21,0,0,4,1],[776,20,776,28,0,0,4,0],[776,20,776,28,0,0,4,1],[779,20,779,28,0,0,4,0],[779,20,779,28,0,0,4,1],[793,20,793,28,0,0,4,0],[793,20,793,28,0,0,4,1],[806,13,806,21,0,0,4,0],[809,20,809,28,0,0,4,0],[809,20,809,28,0,0,4,1],[820,20,820,28,0,0,4,1],[837,20,837,28,0,0,4,0],[837,20,837,28,0,0,4,1]],"ComputeCorrelates":[[201,9,201,22,0,0,4,1],[205,14,205,26,0,0,4,0]],"OptimizeByComputingLinearization":[[1072,9,1072,43,0,0,4,0],[1073,13,1073,58,0,0,4,0]],"SlopeLimiting":[[835,9,835,36,0,0,4,0]],"cmsPipelineDup":[[1504,9,1504,35,0,0,4,0]],"cmsTransform2DeviceLink":[[1211,9,1211,29,0,0,4,1],[1217,9,1217,20,0,0,4,0],[1219,13,1219,58,0,0,4,1],[1226,9,1226,20,0,0,4,1],[1229,9,1229,51,0,0,4,1],[1236,9,1236,50,0,0,4,1],[1245,9,1245,18,0,0,4,1],[1264,10,1264,42,0,0,4,1],[1270,9,1270,38,0,0,4,1],[1275,9,1275,27,0,0,4,0],[1275,9,1275,27,0,0,4,1],[1284,9,1284,27,0,0,4,0],[1284,9,1284,27,0,0,4,1],[1294,13,1294,31,0,0,4,0],[1294,35,1294,78,0,0,4,0],[1295,18,1295,109,0,0,4,1],[1299,13,1299,30,0,0,4,0],[1299,34,1299,76,0,0,4,0],[1300,18,1300,110,0,0,4,1],[1307,9,1307,27,0,0,4,1],[1312,9,1312,44,0,0,4,1],[1316,9,1316,46,0,0,4,1],[1319,9,1319,52,0,0,4,1],[1323,9,1323,39,0,0,4,0],[1323,9,1323,39,0,0,4,1],[1324,9,1324,39,0,0,4,0],[1326,16,1326,84,0,0,4,1],[1329,9,1329,40,0,0,4,1],[1341,9,1341,41,0,0,4,0],[1341,45,1341,71,0,0,4,1],[1346,9,1346,40,0,0,4,1],[1350,14,1350,87,0,0,4,1]],"FindCombination":[[1179,15,1179,38,0,0,4,0],[1179,15,1179,38,0,0,4,1],[1183,13,1183,31,0,0,4,0],[1183,13,1183,31,0,0,4,1],[1184,13,1184,37,0,0,4,0],[1184,13,1184,37,0,0,4,1],[1184,41,1184,78,0,0,4,0],[1184,41,1184,78,0,0,4,1],[1186,13,1186,31,0,0,4,0],[1186,13,1186,31,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenIOhandlerFromNULL","cmsDeleteContext","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsJoinToneCurve","cmsFreeToneCurve","cmsCIECAM02Init","cmsCIECAM02Forward","cmsCIECAM02Done","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseIOhandler","cmsDeleteContext","cmsOpenProfileFromFile","cmsCreate_sRGBProfileTHR","cmsCreateNULLProfileTHR","cmsFormatterForColorspaceOfProfile","cmsCreateProofingTransformTHR","cmsTransform2DeviceLink","cmsCloseProfile","cmsDeleteTransform","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8LoadFromMem","cmsIT8GetSheetType","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenIOhandlerFromNULL","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsJoinToneCurve","cmsFreeToneCurve","cmsCIECAM02Init","cmsCIECAM02Forward","cmsCIECAM02Done","cmsOpenProfileFromFile","cmsCreate_sRGBProfileTHR","cmsCreateNULLProfileTHR","cmsFormatterForColorspaceOfProfile","cmsCreateProofingTransformTHR","cmsTransform2DeviceLink","cmsCloseProfile","cmsDeleteTransform","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8LoadFromMem","cmsIT8GetSheetType","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // Create a context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 1. Create I/O handler from NULL (simulating null device)
    cmsIOHANDLER* nullHandler = cmsOpenIOhandlerFromNULL(context);
    if (!nullHandler) {
        cmsDeleteContext(context);
        return 0;
    }

    // 2. Create tone curves for demonstration
    cmsUInt16Number curve1[256], curve2[256];
    for (int i = 0; i < 256; i++) {
        curve1[i] = (cmsUInt16Number)(i * 257); // Simple linear curve
        curve2[i] = (cmsUInt16Number)((255 - i) * 257); // Inverse curve
    }
    
    cmsToneCurve* toneCurve1 = cmsBuildTabulatedToneCurve16(context, 256, curve1);
    cmsToneCurve* toneCurve2 = cmsBuildTabulatedToneCurve16(context, 256, curve2);
    
    if (toneCurve1 && toneCurve2) {
        // Join the two tone curves
        cmsToneCurve* joinedCurve = cmsJoinToneCurve(context, toneCurve1, toneCurve2, 256);
        if (joinedCurve) {
            cmsFreeToneCurve(joinedCurve);
        }
    }

    // 3. Create CIECAM02 model and perform forward transform
    cmsViewingConditions viewingConditions;
    memset(&viewingConditions, 0, sizeof(viewingConditions));
    viewingConditions.whitePoint.X = 0.95047;
    viewingConditions.whitePoint.Y = 1.00000;
    viewingConditions.whitePoint.Z = 1.08883;
    viewingConditions.La = 100.0;
    viewingConditions.Yb = 20.0;
    viewingConditions.surround = 1.0;
    viewingConditions.D_value = 1.0;
    
    cmsHANDLE cam02 = cmsCIECAM02Init(context, &viewingConditions);
    if (cam02) {
        cmsCIEXYZ inputXYZ = {0.5, 0.5, 0.5};
        cmsJCh outputJCh;
        cmsCIECAM02Forward(cam02, &inputXYZ, &outputJCh);
        cmsCIECAM02Done(cam02);
    }

    // 4. Create profiles and transform
    // Create input profile from data (simulating ICC profile data)
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        if (toneCurve1) cmsFreeToneCurve(toneCurve1);
        if (toneCurve2) cmsFreeToneCurve(toneCurve2);
        cmsCloseIOhandler(nullHandler);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE inputProfile = cmsOpenProfileFromFile("input_file", "r");
    assert_file_closed(&in_file);;
    
    // Create simple output and proofing profiles
    cmsHPROFILE outputProfile = cmsCreate_sRGBProfileTHR(context);
    cmsHPROFILE proofingProfile = cmsCreateNULLProfileTHR(context);
    
    if (inputProfile && outputProfile && proofingProfile) {
        // Get formatter for colorspace
        cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(
            inputProfile, 4, FALSE);
        
        // Create proofing transform
        cmsHTRANSFORM transform = cmsCreateProofingTransformTHR(
            context,
            inputProfile,
            TYPE_RGB_8,
            outputProfile,
            TYPE_RGB_8,
            proofingProfile,
            INTENT_PERCEPTUAL,
            INTENT_ABSOLUTE_COLORIMETRIC,
            cmsFLAGS_NOCACHE);
        
        if (transform) {
            // 5. Convert transform to device link
            cmsHPROFILE deviceLink = cmsTransform2DeviceLink(
                transform, 4.2, cmsFLAGS_NOCACHE);
            if (deviceLink) {
                cmsCloseProfile(deviceLink);
            }
            cmsDeleteTransform(transform);
        }
    }

    // 6. Free tone curve triple (using array of 3 curves)
    cmsToneCurve* toneCurveTriple[3] = {nullptr, nullptr, nullptr};
    if (toneCurve1 && toneCurve2) {
        // Create third curve
        cmsUInt16Number curve3[256];
        for (int i = 0; i < 256; i++) {
            curve3[i] = (cmsUInt16Number)((i * i * 257) / 255);
        }
        cmsToneCurve* toneCurve3 = cmsBuildTabulatedToneCurve16(context, 256, curve3);
        
        toneCurveTriple[0] = toneCurve1;
        toneCurveTriple[1] = toneCurve2;
        toneCurveTriple[2] = toneCurve3;
        
        // Note: cmsFreeToneCurveTriple is not in provided API list, 
        // so we'll free individually
        if (toneCurveTriple[0]) cmsFreeToneCurve(toneCurveTriple[0]);
        if (toneCurveTriple[1]) cmsFreeToneCurve(toneCurveTriple[1]);
        if (toneCurveTriple[2]) cmsFreeToneCurve(toneCurveTriple[2]);
    } else {
        if (toneCurve1) cmsFreeToneCurve(toneCurve1);
        if (toneCurve2) cmsFreeToneCurve(toneCurve2);
    }

    // 7. IT8 operations
    // Create simple IT8 data from input
    const char* it8Data = "CTI1\nSAMPLE_ID\tSAMPLE_NAME\n1\tTest\n";
    cmsHANDLE it8Handle = cmsIT8LoadFromMem(context, it8Data, strlen(it8Data));
    if (it8Handle) {
        const char* sheetType = cmsIT8GetSheetType(it8Handle);
        (void)sheetType; // Use to avoid unused variable warning
        cmsIT8Free(it8Handle);
    }

    // Cleanup
    if (inputProfile) cmsCloseProfile(inputProfile);
    if (outputProfile) cmsCloseProfile(outputProfile);
    if (proofingProfile) cmsCloseProfile(proofingProfile);
    
    cmsCloseIOhandler(nullHandler);
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}