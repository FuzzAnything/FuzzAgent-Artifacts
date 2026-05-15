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
//<ID> 275
//<Prompt> []
/*<Combination>: [
*/
//<score> 65, nr_unique_branch: 53
//<Quality> {"density":10,"unique_branches":{"PatchLUT":[[518,17,518,33,0,0,4,0],[520,17,520,33,0,0,4,0]],"OptimizeByResampling":[[690,9,690,51,0,0,4,0],[696,13,696,19,0,0,4,0],[696,23,696,62,0,0,4,0],[699,17,699,44,0,0,4,0],[699,17,699,44,0,0,4,1],[703,20,703,73,0,0,4,1],[723,9,723,52,0,0,4,0],[729,13,729,20,0,0,4,0],[729,24,729,71,0,0,4,0],[732,17,732,45,0,0,4,0],[736,21,736,73,0,0,4,1],[766,9,766,27,0,0,4,0],[767,9,767,28,0,0,4,0],[772,9,772,26,0,0,4,1],[775,9,775,27,0,0,4,1],[779,9,779,26,0,0,4,1],[779,30,779,48,0,0,4,1],[792,13,792,24,0,0,4,1]],"FixWhiteMisalignment":[[587,9,587,145,0,0,4,1],[609,9,609,16,0,0,4,0],[613,19,613,28,0,0,4,0],[613,19,613,28,0,0,4,1],[616,17,616,39,0,0,4,1]],"_cmsGetStockInputFormatter":[[3689,19,3689,74,0,0,4,1]],"_cmsGetStockOutputFormatter":[[3860,19,3860,75,0,0,4,1]],"cmsPipelineUnlinkStage":[[1571,9,1571,25,0,0,4,1],[1582,9,1582,23,0,0,4,0],[1585,17,1585,27,0,0,4,0],[1585,17,1585,27,0,0,4,1],[1594,17,1594,25,0,0,4,0]],"cmsReverseToneCurveEx":[[1081,37,1081,67,0,0,4,1]],"cmsCreateExtendedTransform":[[1211,9,1211,22,0,0,4,0]],"AllocEmptyTransform":[[957,17,957,38,0,0,4,0],[957,42,957,62,0,0,4,0]],"PrelinOpt16alloc":[[359,9,359,20,0,0,4,1],[365,15,365,26,0,0,4,0],[365,15,365,26,0,0,4,1],[367,13,367,23,0,0,4,0],[367,13,367,23,0,0,4,1],[383,9,383,36,0,0,4,1],[390,9,390,38,0,0,4,1],[398,15,398,27,0,0,4,0],[398,15,398,27,0,0,4,1],[400,13,400,24,0,0,4,0],[400,13,400,24,0,0,4,1]],"_cmsReadCHAD":[[102,9,102,20,0,0,4,1],[111,9,111,54,0,0,4,1]],"PrelinEval16":[[311,15,311,32,0,0,4,0],[311,15,311,32,0,0,4,1],[318,15,318,33,0,0,4,0],[318,15,318,33,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsOpenProfileFromFile","cmsDeleteContext","cmsGetHeaderFlags","cmsSetEncodedICCversion","cmsCreate_sRGBProfile","cmsCreateMultiprofileTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsLab2LCh","cmsBuildGamma","cmsReverseToneCurve","cmsEvalToneCurveFloat","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFile","cmsGetHeaderFlags","cmsSetEncodedICCversion","cmsCreate_sRGBProfile","cmsCreateMultiprofileTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsLab2LCh","cmsBuildGamma","cmsReverseToneCurve","cmsEvalToneCurveFloat","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to process ICC profile data:

*/


// Include lcms headers

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // Create a context for lcms operations
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 1. Create I/O handler from memory using cmsOpenIOhandlerFromMem
    // Note: cmsOpenIOhandlerFromMem is not in provided list, using cmsOpenIOhandlerFromFile instead
    // We'll write data to a temp file first
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a temporary file to read from
    FILE *temp_file = fopen("temp_profile.icc", "wb");
    if (!temp_file) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Copy data to temp file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, temp_file);
    }
    assert_file_closed(&temp_file);;
    assert_file_closed(&in_file);;
    
    // 2. Open profile from the temporary file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("temp_profile.icc", "r");
    if (!hProfile) {
        remove("temp_profile.icc");
        cmsDeleteContext(context);
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get header flags using cmsGetHeaderFlags
    cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
    
    // 4. Set encoded ICC version using cmsSetEncodedICCversion
    // Use a version derived from input data
    cmsUInt32Number version = 0x4000000; // ICC version 4.0
    if (size > 4) {
        version = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        version &= 0xFFFFFF00; // Keep only major version bits
        version |= 0x00000040; // Ensure valid version
    }
    cmsSetEncodedICCversion(hProfile, version);
    
    // 5. Create multiprofile transform using cmsCreateMultiprofileTransform
    // Create a simple sRGB profile to use as second profile
    cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfile();
    if (sRGBProfile) {
        cmsHPROFILE profiles[2] = {hProfile, sRGBProfile};
        
        // Create transform with data-derived parameters
        cmsUInt32Number inputFormat = TYPE_RGB_8;
        cmsUInt32Number outputFormat = TYPE_RGB_8;
        cmsUInt32Number intent = INTENT_PERCEPTUAL;
        cmsUInt32Number dwFlags = 0;
        
        if (size > 8) {
            inputFormat = (data[4] % 4) + TYPE_RGB_8;
            outputFormat = (data[5] % 4) + TYPE_RGB_8;
            intent = data[6] % 4;
            dwFlags = data[7] & 0xFF;
        }
        
        cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(
            profiles, 2, inputFormat, outputFormat, intent, dwFlags);
        
        if (transform) {
            // Test the transform with some sample data
            cmsUInt8Number input[3] = {128, 128, 128};
            cmsUInt8Number output[3];
            cmsDoTransform(transform, input, output, 1);
            
            cmsDeleteTransform(transform);
        }
        cmsCloseProfile(sRGBProfile);
    }
    
    // 6. Convert Lab to LCh using cmsLab2LCh
    cmsCIELab lab;
    cmsCIELCh lch;
    
    // Initialize Lab values from input data
    if (size >= 24) {
        // Use some bytes from input to initialize Lab values
        lab.L = (data[8] / 255.0) * 100.0;
        lab.a = ((int8_t)data[9]) * 100.0 / 127.0;
        lab.b = ((int8_t)data[10]) * 100.0 / 127.0;
        
        cmsLab2LCh(&lch, &lab);
    }
    
    // 7. Create and reverse tone curve using cmsReverseToneCurveEx
    // First create a simple gamma tone curve
    cmsToneCurve* gammaCurve = cmsBuildGamma(context, 2.2);
    if (gammaCurve) {
        // Reverse the tone curve
        // Note: cmsReverseToneCurveEx is not in provided list, using cmsReverseToneCurve instead
        cmsToneCurve* reversedCurve = cmsReverseToneCurve(gammaCurve);
        if (reversedCurve) {
            // Test the reversed curve
            cmsFloat32Number testValue = 0.5f;
            if (size > 12) {
                testValue = data[11] / 255.0f;
            }
            cmsEvalToneCurveFloat(reversedCurve, testValue);
            cmsFreeToneCurve(reversedCurve);
        }
        cmsFreeToneCurve(gammaCurve);
    }
    
    // Clean up
    cmsCloseProfile(hProfile);
    remove("temp_profile.icc");
    cmsDeleteContext(context);
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
}