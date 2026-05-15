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
//<ID> 267
//<Prompt> []
/*<Combination>: [
*/
//<score> 92, nr_unique_branch: 6
//<Quality> {"density":23,"unique_branches":{"IsMyFile":[[2545,8,2545,11,0,0,4,0]],"cmsIT8LoadFromFile":[[2629,10,2629,37,0,0,4,0]],"IsAvailableOnList":[[1309,9,1309,20,0,0,4,1],[1312,12,1312,21,0,0,4,0],[1312,12,1312,21,0,0,4,1],[1314,13,1314,31,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsIT8LoadFromFile","cmsOpenProfileFromFileTHR","cmsGetHeaderModel","cmsGetAlarmCodesTHR","cmsGetAlarmCodes","cmsDetectRGBProfileGamma","cmsIsTag","cmsReadTag","cmsBuildParametricToneCurve","cmsGetToneCurveEstimatedTableEntries","cmsReverseToneCurve","cmsCreateGrayProfileTHR","cmsFreeToneCurve","cmsFreeToneCurve","cmsWriteRawTag","cmsCloseProfile","cmsCloseProfile","cmsIT8GetPropertyMulti","cmsIT8Free","cmsPlugin","cmsUnregisterPluginsTHR","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromFile","cmsOpenProfileFromFileTHR","cmsGetHeaderModel","cmsGetAlarmCodesTHR","cmsGetAlarmCodes","cmsDetectRGBProfileGamma","cmsIsTag","cmsReadTag","cmsBuildParametricToneCurve","cmsGetToneCurveEstimatedTableEntries","cmsReverseToneCurve","cmsCreateGrayProfileTHR","cmsFreeToneCurve","cmsFreeToneCurve","cmsWriteRawTag","cmsCloseProfile","cmsCloseProfile","cmsIT8GetPropertyMulti","cmsIT8Free","cmsPlugin","cmsUnregisterPluginsTHR","cmsDeleteContext"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Write input data to temporary files
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create temporary files for IT8 and ICC data
    FILE* temp_it8 = fopen("temp_it8.txt", "wb");
    FILE* temp_icc = fopen("temp_icc.icc", "wb");
    if (!temp_it8 || !temp_icc) {
        if (temp_it8) assert_file_closed(&temp_it8);;
        if (temp_icc) assert_file_closed(&temp_icc);;
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&temp_icc);
	assert_file_closed(&temp_it8);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Split input data between IT8 and ICC files
    size_t half_size = size / 2;
    fwrite(data, 1, half_size, temp_it8);
    fwrite(data + half_size, 1, size - half_size, temp_icc);
    assert_file_closed(&temp_it8);;
    assert_file_closed(&temp_icc);;
    
    // Load IT8 data
    cmsHANDLE it8 = cmsIT8LoadFromFile(ctx, "temp_it8.txt");
    
    // Open ICC profile
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(ctx, "temp_icc.icc", "r");
    
    if (hProfile) {
        // Get header model
        cmsUInt32Number model = cmsGetHeaderModel(hProfile);
        
        // Get alarm codes
        cmsUInt16Number alarmCodes[16];
        cmsUInt16Number alarmCodesNonTHR[16];
        cmsGetAlarmCodesTHR(ctx, alarmCodes);
        cmsGetAlarmCodes(alarmCodesNonTHR);
        
        // Detect gamma if it's an RGB profile
        cmsFloat64Number gamma = cmsDetectRGBProfileGamma(hProfile, 0.1);
        
        // Check for and read a tag
        if (cmsIsTag(hProfile, cmsSigRedColorantTag)) {
            void* tagData = cmsReadTag(hProfile, cmsSigRedColorantTag);
            // Could use tagData if valid
        }
        
        // Create a tone curve and its reverse
        cmsFloat64Number params[1] = {2.2};
        cmsToneCurve* toneCurve = cmsBuildParametricToneCurve(ctx, 1, params);
        if (toneCurve) {
            // Get estimated table entries
            cmsUInt32Number entries = cmsGetToneCurveEstimatedTableEntries(toneCurve);
            
            // Create reversed tone curve
            cmsToneCurve* reversedCurve = cmsReverseToneCurve(toneCurve);
            
            // Create gray profile using the tone curve
            cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0}; // D65
            cmsHPROFILE grayProfile = cmsCreateGrayProfileTHR(ctx, &whitePoint, toneCurve);
            
            // Clean up tone curves
            if (reversedCurve) cmsFreeToneCurve(reversedCurve);
            cmsFreeToneCurve(toneCurve);
            
            if (grayProfile) {
                // Write a raw tag to the gray profile
                const char* testData = "FuzzTest";
                cmsWriteRawTag(grayProfile, cmsSigCopyrightTag, testData, strlen(testData) + 1);
                cmsCloseProfile(grayProfile);
            }
        }
        
        // Try CIECAM02 reverse if we have valid input
        if (size > 200) {
            // Extract JCh values from input data
            cmsJCh jch;
            jch.J = *((cmsFloat64Number*)data) / 255.0 * 100.0;
            jch.C = *((cmsFloat64Number*)(data + 8)) / 255.0 * 150.0;
            jch.h = *((cmsFloat64Number*)(data + 16)) / 255.0 * 360.0;
            
            cmsCIEXYZ xyzOut;
            // Note: This would need a proper cmsHANDLE model, which we don't have
            // cmsCIECAM02Reverse(NULL, &jch, &xyzOut);
        }
        
        // Close the main profile
        cmsCloseProfile(hProfile);
    }
    
    // Get IT8 property if IT8 was loaded
    if (it8) {
        const char* prop = cmsIT8GetPropertyMulti(it8, "SAMPLE_ID", "0");
        cmsIT8Free(it8);
    }
    
    // Try to register a plugin (NULL for fuzzing)
    cmsPlugin(NULL);
    
    // Cleanup
    cmsUnregisterPluginsTHR(ctx);
    cmsDeleteContext(ctx);
    
    // Remove temporary files
    remove("temp_it8.txt");
    remove("temp_icc.icc");
    
    assert_file_closed(&temp_icc);
	assert_file_closed(&temp_it8);
	assert_file_closed(&in_file);
	return 0;
}