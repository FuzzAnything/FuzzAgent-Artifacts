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
//<ID> 413
//<Prompt> []
/*<Combination>: [
*/
//<score> 99, nr_unique_branch: 31
//<Quality> {"density":22,"unique_branches":{"BuildGrayOutputPipeline":[[418,9,418,24,0,0,4,0]],"cmsFormatterForColorspaceOfProfile":[[4031,46,4031,54,0,0,4,0]],"cmsCloseProfile":[[1674,9,1674,13,0,0,4,0]],"cmsDetectTAC":[[471,9,471,57,0,0,4,0],[471,9,471,57,0,0,4,1],[479,9,479,25,0,0,4,0],[479,9,479,25,0,0,4,1],[485,9,485,42,0,0,4,1],[488,9,488,21,0,0,4,1],[494,9,494,30,0,0,4,0],[494,9,494,30,0,0,4,1],[502,9,502,58,0,0,4,1]],"TrilinearInterp16":[[586,16,586,35,0,0,4,0]],"IsInkSpace":[[1063,6,1063,18,0,0,4,0],[1076,6,1076,13,0,0,4,1]],"EstimateTAC":[[442,22,442,43,0,0,4,0],[442,22,442,43,0,0,4,1],[446,9,446,26,0,0,4,0],[446,9,446,26,0,0,4,1],[450,23,450,44,0,0,4,0],[450,23,450,44,0,0,4,1]],"PackFloatsFromFloat":[[3069,35,3069,65,0,0,4,0],[3076,12,3076,22,0,0,4,1],[3079,20,3079,29,0,0,4,0],[3079,20,3079,29,0,0,4,1],[3081,39,3081,45,0,0,4,1],[3085,19,3085,26,0,0,4,1],[3088,19,3088,25,0,0,4,1],[3095,12,3095,22,0,0,4,0],[3095,26,3095,35,0,0,4,1],[731,31,731,44,7,0,4,1]]},"library_calls":["cmsCreateContext","cmsCreate_OkLabProfile","cmsCreateInkLimitingDeviceLink","cmsSetAdaptationState","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDetectTAC","cmsLinkTag","cmsPipelineAlloc","cmsGetPipelineContextID","cmsBuildGamma","cmsBuildGamma","cmsJoinToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsPipelineFree","cmsCloseProfile","cmsIT8Alloc","cmsIT8SetDataDbl","cmsIT8Free","cmsSetAdaptationState","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreate_OkLabProfile","cmsCreateInkLimitingDeviceLink","cmsSetAdaptationState","cmsOpenProfileFromStreamTHR","cmsDetectTAC","cmsLinkTag","cmsPipelineAlloc","cmsGetPipelineContextID","cmsBuildGamma","cmsBuildGamma","cmsJoinToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsPipelineFree","cmsCloseProfile","cmsIT8Alloc","cmsIT8SetDataDbl","cmsIT8Free","cmsSetAdaptationState","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // Create profiles first
    cmsHPROFILE okLabProfile = cmsCreate_OkLabProfile(context);
    cmsHPROFILE inkLimitProfile = cmsCreateInkLimitingDeviceLink(cmsSigRgbData, 250.0);

    // Set adaptation state
    double oldAdaptation = cmsSetAdaptationState(0.75);

    // Process input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsCloseProfile(inkLimitProfile);
        cmsCloseProfile(okLabProfile);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        cmsCloseProfile(inkLimitProfile);
        cmsCloseProfile(okLabProfile);
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Try to open profile from input data
    cmsHPROFILE inputProfile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (inputProfile) {
        // Detect TAC
        double tac = cmsDetectTAC(inputProfile);

        // Link tag
        cmsLinkTag(inputProfile, cmsSigAToB0Tag, cmsSigBToA0Tag);

        // Create pipeline for processing
        cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
        if (pipeline) {
            // Get pipeline context
            cmsContext pipeContext = cmsGetPipelineContextID(pipeline);

            // Create tone curves for joining
            cmsToneCurve *curve1 = cmsBuildGamma(context, 2.2);
            cmsToneCurve *curve2 = cmsBuildGamma(context, 1.8);
            
            if (curve1 && curve2) {
                // Join tone curves
                cmsToneCurve *joinedCurve = cmsJoinToneCurve(context, curve1, curve2, 256);
                if (joinedCurve) {
                    cmsFreeToneCurve(joinedCurve);
                }
            }
            
            if (curve1) cmsFreeToneCurve(curve1);
            if (curve2) cmsFreeToneCurve(curve2);
            
            cmsPipelineFree(pipeline);
        }

        cmsCloseProfile(inputProfile);
    }

    // Create IT8 handle and set data
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (it8) {
        // Use some values from input data
        if (size >= 8) {
            double value;
            memcpy(&value, data, sizeof(value));
            cmsIT8SetDataDbl(it8, "Patch1", "Sample1", value);
        }
        cmsIT8Free(it8);
    }

    // Restore adaptation state
    cmsSetAdaptationState(oldAdaptation);

    // Cleanup
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    cmsCloseProfile(inkLimitProfile);
    cmsCloseProfile(okLabProfile);
    cmsDeleteContext(context);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}