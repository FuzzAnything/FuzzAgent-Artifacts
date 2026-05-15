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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 8
//<Quality> {"density":12,"unique_branches":{"BuildRGBOutputMatrixShaper":[[479,9,479,19,0,0,4,0],[479,23,479,33,0,0,4,0],[479,37,479,47,0,0,4,0]],"cmsGetProfileInfo":[[1021,9,1021,20,0,0,4,0]],"OptimizeByComputingLinearization":[[1102,21,1102,54,0,0,4,0],[1251,13,1251,21,0,0,4,1]],"cmsDeleteTransform":[[162,9,162,28,0,0,4,0]],"cmsCreateExtendedTransform":[[1253,13,1253,69,0,0,4,0]]},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsMLUalloc","cmsMLUtranslationsCodes","cmsMLUfree","cmsTagLinkedTo","cmsBuildTabulatedToneCurveFloat","cmsCreateRGBProfileTHR","cmsCreateTransform","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineEvalFloat","cmsPipelineEval16","cmsStageOutputChannels","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsGetSupportedIntents","cmsTempFromWhitePoint","cmsGetColorSpace","cmsChannelsOf","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsMLUalloc","cmsMLUtranslationsCodes","cmsMLUfree","cmsTagLinkedTo","cmsBuildTabulatedToneCurveFloat","cmsCreateRGBProfileTHR","cmsCreateTransform","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineEvalFloat","cmsPipelineEval16","cmsStageOutputChannels","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsGetSupportedIntents","cmsTempFromWhitePoint","cmsGetColorSpace","cmsChannelsOf","cmsCloseProfile"],"visited":1}
/**/


// Include lcms2 header

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // 1. Create a profile from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Get profile information using cmsGetProfileInfo
    wchar_t buffer[256];
    cmsUInt32Number infoSize = cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", 
                                                 buffer, sizeof(buffer)/sizeof(wchar_t));

    // 3. Get MLU translations using cmsMLUtranslationsCodes
    // First get the MLU for description tag
    cmsMLU* mlu = cmsMLUalloc(NULL, 5);
    if (mlu) {
        char langCode[3] = {0};
        char countryCode[3] = {0};
        // Try to get translation codes (simulate - actual usage would require populated MLU)
        cmsMLUtranslationsCodes(mlu, 0, langCode, countryCode);
        cmsMLUfree(mlu);
    }

    // 4. Check for linked tags using cmsTagLinkedTo
    cmsTagSignature linked = cmsTagLinkedTo(hProfile, cmsSigAToB0Tag);

    // 5. Create a simple pipeline and evaluate it
    // Create tone curves for RGB pipeline
    cmsToneCurve* curves[3];
    cmsFloat32Number curveValues[] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
    
    for (int i = 0; i < 3; i++) {
        curves[i] = cmsBuildTabulatedToneCurveFloat(NULL, 5, curveValues);
    }
    
    // Create RGB profile
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},
        {0.3000, 0.6000, 1.0},
        {0.1500, 0.0600, 1.0}
    };
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    
    cmsHPROFILE rgbProfile = cmsCreateRGBProfileTHR(NULL, &whitePoint, &primaries, curves);
    
    if (rgbProfile) {
        // Create transform
        cmsHTRANSFORM hTransform = cmsCreateTransform(rgbProfile, TYPE_RGB_8, 
                                                     hProfile, TYPE_RGB_8, 
                                                     INTENT_PERCEPTUAL, 0);
        
        if (hTransform) {
            // Get pipeline from transform (internal - simulate evaluation)
            // Create a simple pipeline for evaluation
            cmsPipeline* pipeline = cmsPipelineAlloc(NULL, 3, 3);
            if (pipeline) {
                // Add a simple matrix stage
                cmsStage* matrix = cmsStageAllocMatrix(NULL, 3, 3, 
                    (const cmsFloat64Number[]){1,0,0,0,1,0,0,0,1}, NULL);
                if (matrix) {
                    cmsPipelineInsertStage(pipeline, cmsAT_END, matrix);
                    
                    // Evaluate with float input
                    cmsFloat32Number inFloat[3] = {0.5f, 0.5f, 0.5f};
                    cmsFloat32Number outFloat[3];
                    cmsPipelineEvalFloat(inFloat, outFloat, pipeline);
                    
                    // Evaluate with 16-bit input
                    cmsUInt16Number in16[3] = {32768, 32768, 32768};
                    cmsUInt16Number out16[3];
                    cmsPipelineEval16(in16, out16, pipeline);
                    
                    // Get stage output channels
                    cmsUInt32Number outputChannels = cmsStageOutputChannels(matrix);
                }
                
                cmsPipelineFree(pipeline);
            }
            
            cmsDeleteTransform(hTransform);
        }
        
        cmsCloseProfile(rgbProfile);
    }
    
    // Clean up tone curves
    for (int i = 0; i < 3; i++) {
        if (curves[i]) cmsFreeToneCurve(curves[i]);
    }

    // 6. Get supported intents
    cmsUInt32Number intents[10];
    char* descriptions[10];
    cmsUInt32Number numIntents = cmsGetSupportedIntents(10, intents, descriptions);

    // 7. Convert white point to temperature
    cmsFloat64Number tempK;
    cmsBool tempResult = cmsTempFromWhitePoint(&tempK, &whitePoint);

    // 8. Get channels of color space
    cmsColorSpaceSignature cs = cmsGetColorSpace(hProfile);
    cmsUInt32Number channels = cmsChannelsOf(cs);

    // 9. Write some output (optional)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write profile info size
        fwrite(&infoSize, sizeof(infoSize), 1, out_file);
        fwrite(buffer, sizeof(wchar_t), infoSize, out_file);
        assert_file_closed(&out_file);;
    }

    // Clean up
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}