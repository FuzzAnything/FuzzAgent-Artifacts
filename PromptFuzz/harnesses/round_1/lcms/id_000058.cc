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
//<ID> 388
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 19
//<Quality> {"density":21,"unique_branches":{"cmsPipelineEvalReverseFloat":[[1766,9,1766,33,0,0,4,0],[1766,37,1766,61,0,0,4,0]],"MemoryWrite":[[219,9,219,29,0,0,4,0]],"Type_LUT16_Write":[[2421,24,2421,60,0,0,4,1],[2431,9,2431,20,0,0,4,1],[2485,9,2485,23,0,0,4,1],[2488,17,2488,46,0,0,4,1],[2491,9,2491,24,0,0,4,1],[2494,13,2494,42,0,0,4,1],[2500,9,2500,23,0,0,4,1],[2504,19,2504,36,0,0,4,0],[2504,19,2504,36,0,0,4,1],[2506,17,2506,46,0,0,4,1],[2507,17,2507,51,0,0,4,1],[2521,9,2521,24,0,0,4,1],[2525,19,2525,37,0,0,4,0],[2525,19,2525,37,0,0,4,1],[2527,17,2527,46,0,0,4,1],[2528,17,2528,51,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsD50_xyY","cmsCreateLab2Profile","cmsDeleteContext","cmsGetSupportedIntents","cmsGetPostScriptCRD","cmsIT8LoadFromFile","cmsIT8SetIndexColumn","cmsIT8Free","cmsBuildParametricToneCurve","cmsReverseToneCurveEx","cmsPipelineAlloc","cmsPipelineInsertStage","cmsStageAllocToneCurves","cmsPipelineEvalReverseFloat","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsD50_xyY","cmsCreateLab2Profile","cmsGetSupportedIntents","cmsGetPostScriptCRD","cmsIT8LoadFromFile","cmsIT8SetIndexColumn","cmsIT8Free","cmsBuildParametricToneCurve","cmsReverseToneCurveEx","cmsPipelineAlloc","cmsPipelineInsertStage","cmsStageAllocToneCurves","cmsPipelineEvalReverseFloat","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Return early if there's not enough data
    if (size < 100) {
	return 0;
	}
	
    
    // Initialize context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // Create a Lab2 profile with D50 white point
    const cmsCIExyY* d50_white = cmsD50_xyY();
    cmsHPROFILE lab_profile = cmsCreateLab2Profile(d50_white);
    if (!lab_profile) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Get supported intents
    cmsUInt32Number max_intents = 10;
    cmsUInt32Number intent_codes[10];
    char* intent_descriptions[10];
    
    cmsUInt32Number num_intents = cmsGetSupportedIntents(max_intents, intent_codes, 
                                                        (char**)intent_descriptions);
    
    // Get PostScript CRD if we have intents
    if (num_intents > 0) {
        // Use a portion of input data as buffer for CRD
        cmsUInt32Number buffer_len = (size > 4096) ? 4096 : size;
        uint8_t* crd_buffer = new uint8_t[buffer_len];
        
        // Try to get CRD for first available intent
        cmsUInt32Number crd_size = cmsGetPostScriptCRD(ctx, lab_profile, 
                                                      intent_codes[0], 0,
                                                      crd_buffer, buffer_len);
        
        // Process the CRD data if we got some
        if (crd_size > 0 && crd_size <= buffer_len) {
            // Here we would typically process the CRD data
        }
        
        delete[] crd_buffer;
    }
    
    // Create an IT8 handle from file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        cmsHANDLE it8_handle = cmsIT8LoadFromFile(ctx, "input_file");
        
        if (it8_handle) {
            // Set index column using part of input data as column name
            char column_name[64];
            size_t name_len = (size > 63) ? 63 : size;
            memcpy(column_name, data, name_len);
            column_name[name_len] = '\0';
            
            cmsIT8SetIndexColumn(it8_handle, column_name);
            cmsIT8Free(it8_handle);
        }
        
        assert_file_closed(&in_file);;
    }
    
    // Create a simple tone curve from input data
    // Use first 256 bytes to create parametric tone curve
    if (size >= 256) {
        cmsFloat64Number params[3] = {1.0, 0.0, 0.0};
        params[0] = data[0] / 255.0 * 5.0;  // Gamma between 0-5
        
        cmsToneCurve* tone_curve = cmsBuildParametricToneCurve(ctx, 1, params);
        
        if (tone_curve) {
            // Reverse the tone curve with 256 samples
            cmsToneCurve* reversed_curve = cmsReverseToneCurveEx(256, tone_curve);
            
            // Create a simple pipeline with the reversed curve
            cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 1, 1);
            if (pipeline && reversed_curve) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, 
                                     cmsStageAllocToneCurves(ctx, 1, &reversed_curve));
                
                // Evaluate reverse float using random data from input
                if (size >= 16) {
                    cmsFloat32Number target[4] = {0.5f, 0.5f, 0.5f, 0.5f};
                    cmsFloat32Number result[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                    cmsFloat32Number hint[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                    
                    // Use data to initialize target values
                    for (int i = 0; i < 4 && i * 4 + 4 <= size; i++) {
                        target[i] = *((cmsFloat32Number*)(data + i * 4));
                    }
                    
                    cmsPipelineEvalReverseFloat(target, result, hint, pipeline);
                }
                
                cmsPipelineFree(pipeline);
            }
            
            if (reversed_curve) cmsFreeToneCurve(reversed_curve);
            cmsFreeToneCurve(tone_curve);
        }
    }
    
    // Write profile to output file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Determine required buffer size
        cmsUInt32Number bytes_needed = 0;
        cmsSaveProfileToMem(lab_profile, nullptr, &bytes_needed);
        
        if (bytes_needed > 0) {
            uint8_t* profile_buffer = new uint8_t[bytes_needed];
            if (cmsSaveProfileToMem(lab_profile, profile_buffer, &bytes_needed)) {
                fwrite(profile_buffer, 1, bytes_needed, out_file);
            }
            delete[] profile_buffer;
        }
        
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    cmsCloseProfile(lab_profile);
    cmsDeleteContext(ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}