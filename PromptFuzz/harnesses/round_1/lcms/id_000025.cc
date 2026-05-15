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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 2, nr_unique_branch: 30
//<Quality> {"density":1,"unique_branches":{"DefaultEvalParametricFn":[[501,17,501,22,0,0,4,1]],"fclamp":[[226,13,226,26,0,0,4,0]],"TetrahedralInterpFloat":[[648,16,648,39,0,0,4,0],[648,16,648,39,0,0,4,1],[651,16,651,39,0,0,4,0],[651,16,651,39,0,0,4,1],[654,16,654,39,0,0,4,0],[654,16,654,39,0,0,4,1],[656,21,656,39,0,0,4,0],[656,21,656,39,0,0,4,1],[662,13,662,21,0,0,4,0],[662,13,662,21,0,0,4,1],[662,25,662,33,0,0,4,0],[662,25,662,33,0,0,4,1],[670,17,670,25,0,0,4,0],[670,17,670,25,0,0,4,1],[670,29,670,37,0,0,4,0],[670,29,670,37,0,0,4,1],[678,21,678,29,0,0,4,0],[678,21,678,29,0,0,4,1],[678,33,678,41,0,0,4,0],[678,33,678,41,0,0,4,1],[686,25,686,33,0,0,4,0],[686,37,686,45,0,0,4,0],[686,37,686,45,0,0,4,1],[694,29,694,37,0,0,4,0],[694,29,694,37,0,0,4,1],[694,41,694,49,0,0,4,0],[702,33,702,41,0,0,4,0],[702,45,702,53,0,0,4,0]]},"library_calls":["cmsSetLogErrorHandler","cmsBuildParametricToneCurve","cmsBuildParametricToneCurve","cmsCreateGrayProfile","cmsSetHeaderModel","cmsGetProfileInfoASCII","cmsPipelineAlloc","cmsStageAllocCLutFloat","cmsPipelineInsertStage","cmsStageSampleCLutFloat","cmsPipelineEval16","cmsSaveProfileToStream","cmsStageFree","cmsPipelineFree","cmsFreeToneCurve","cmsCloseProfile"],"critical_calls":["cmsSetLogErrorHandler","cmsSaveProfileToStream","cmsPipelineEval16","cmsStageSampleCLutFloat","cmsPipelineInsertStage","cmsStageAllocCLutFloat","cmsPipelineAlloc","cmsGetProfileInfoASCII","cmsSetHeaderModel","cmsCreateGrayProfile","cmsBuildParametricToneCurve","cmsBuildParametricToneCurve","cmsStageFree","cmsPipelineFree","cmsFreeToneCurve","cmsCloseProfile"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified lcms APIs in a coherent sequence:

*/


// Dummy sampler function for cmsStageSampleCLutFloat
static int  id_000025_DummySampler(const cmsFloat32Number in[], cmsFloat32Number out[], void* cargo) {
    // Simple passthrough sampler
    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];
    return 1;
}

// Custom error handler
static void  id_000025_CustomErrorHandler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text) {
    // Empty handler - just suppress errors
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Set custom error handler first
    cmsSetLogErrorHandler( id_000025_CustomErrorHandler);
    
    // Early exit if not enough data
    if (size < 100) {
	return 0;
	}
	
    
    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    (void)in_fd; // Use to avoid unused variable warning
    
    // Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    (void)out_fd; // Use to avoid unused variable warning
    
    cmsContext context = NULL;
    cmsHPROFILE profile = NULL;
    cmsPipeline *pipeline = NULL;
    cmsStage *stage = NULL;
    cmsToneCurve *curve = NULL;
    
    try {
        // 1. Create gray profile
        cmsCIExyY whitePoint;
        whitePoint.x = 0.3127;  // D65 standard
        whitePoint.y = 0.3290;
        whitePoint.Y = 1.0;
        
        // Create tone curve from input data
        if (size >= sizeof(cmsFloat64Number) * 10) {
            cmsFloat64Number params[10];
            memcpy(params, data, sizeof(cmsFloat64Number) * 10);
            
            // Create parametric tone curve (type 4 = gamma function)
            curve = cmsBuildParametricToneCurve(NULL, 4, params);
        }
        
        if (!curve) {
            // Fallback to linear curve
            cmsFloat64Number linear_params[] = {1.0, 0.0, 0.0};
            curve = cmsBuildParametricToneCurve(NULL, 1, linear_params);
        }
        
        if (!curve) goto cleanup;
        
        // Create gray profile
        profile = cmsCreateGrayProfile(&whitePoint, curve);
        if (!profile) goto cleanup;
        
        // 2. Set header model using data
        cmsUInt32Number model = 0;
        if (size >= sizeof(cmsUInt32Number)) {
            memcpy(&model, data, sizeof(cmsUInt32Number));
            cmsSetHeaderModel(profile, model);
        }
        
        // 3. Get profile info ASCII
        char infoBuffer[256];
        if (cmsGetProfileInfoASCII(profile, cmsInfoDescription, 
                                   "en", "US", 
                                   infoBuffer, sizeof(infoBuffer))) {
            // Info retrieved, could use it but not needed for fuzzing
        }
        
        // 4. Create a pipeline for evaluation
        pipeline = cmsPipelineAlloc(NULL, 3, 3);
        if (!pipeline) goto cleanup;
        
        // 5. Create and add a stage to the pipeline
        stage = cmsStageAllocCLutFloat(NULL, 5, 3, 3, NULL);
        if (!stage) goto cleanup;
        
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        
        // 6. Sample the CLUT using data as cargo
        void *cargo = (void *)(data + 50); // Use offset data as cargo
        cmsStageSampleCLutFloat(stage,  id_000025_DummySampler, cargo, 0);
        
        // 7. Evaluate pipeline with input from data
        if (size >= 6) { // Need at least 3 uint16_t values
            cmsUInt16Number in[3], out[3];
            
            // Use first 6 bytes for input
            memcpy(in, data, 6);
            
            // Evaluate the pipeline
            cmsPipelineEval16(in, out, pipeline);
            
            // Write result to output file
            fwrite(out, sizeof(cmsUInt16Number), 3, out_file);
        }
        
        // Try to save profile to file
        cmsSaveProfileToStream(profile, out_file);
        
    } catch (...) {
        // Catch any exceptions
    }
    
cleanup:
    // Clean up resources
    if (stage && !pipeline) {
        cmsStageFree(stage); // Only free if not inserted into pipeline
    }
    
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (curve) {
        cmsFreeToneCurve(curve);
    }
    
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}