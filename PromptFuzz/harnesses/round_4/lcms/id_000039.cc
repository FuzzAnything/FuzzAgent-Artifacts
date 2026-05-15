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
//<ID> 264
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["cmsSetLogErrorHandler","cmsCreateContext","cmsOpenProfileFromMemTHR","cmsDeleteContext","cmsCreateTransformTHR","cmsCloseProfile","cmsDeleteContext","cmsGetTransformContextID","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineEvalFloat","cmsIT8Alloc","cmsIT8SetIndexColumn","cmsIT8SetPropertyHex","cmsCIECAM02Init","cmsCIECAM02Forward","cmsCIECAM02Done","cmsBuildGamma","cmsGetToneCurveParametricType","cmsFreeToneCurve","cmsSliceSpace16","cmsPipelineFree","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsSetLogErrorHandler","cmsCreateContext","cmsOpenProfileFromMemTHR","cmsCreateTransformTHR","cmsGetTransformContextID","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineEvalFloat","cmsIT8Alloc","cmsIT8SetIndexColumn","cmsIT8SetPropertyHex","cmsCIECAM02Init","cmsCIECAM02Forward","cmsCIECAM02Done","cmsBuildGamma","cmsGetToneCurveParametricType","cmsFreeToneCurve","cmsSliceSpace16","cmsPipelineFree","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// Custom error handler to satisfy cmsSetLogErrorHandler API
static void  id_000039_custom_log_error_handler(cmsContext /*ContextID*/, cmsUInt32Number /*ErrorCode*/, const char* /*Text*/) {
    // Error handling logic can be added here if needed
}

// Sampler function for cmsSliceSpace16
static int  id_000039_custom_sampler16(const cmsUInt16Number* /*In*/, cmsUInt16Number* /*Out*/, void* /*Cargo*/) {
    return 1; // Return non-zero to continue sampling
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<76) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeFloatingArray(fuzz_float_array_6, fuzz_float_array_size_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegralArray(uint32_t, fuzz_uint32_t_array_10, fuzz_uint32_t_array_size_10, fdp);
	
		const cmsFloat32Number * fuzzer_var_cmsPipelineEvalFloat_11_0 = fuzz_float_array_6;
		const cmsUInt32Number * fuzzer_var_cmsSliceSpace16_21_1 = fuzz_uint32_t_array_10;
		cmsUInt32Number fuzzer_size_cmsSliceSpace16_21_1 = static_cast<cmsUInt32Number>(fuzz_uint32_t_array_size_10);//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for processing

    // Set custom error handler
    cmsSetLogErrorHandler( id_000039_custom_log_error_handler);

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	

    // Open profile from memory (simulating lcms output data)
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ctx, data, size);
    if (!hProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Create a transform from the profile
    cmsUInt32Number inputFormat = TYPE_RGB_16;
    cmsUInt32Number outputFormat = TYPE_RGB_16;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = cmsFLAGS_NOCACHE;
    
    cmsHTRANSFORM hTransform = cmsCreateTransformTHR(ctx, hProfile, inputFormat, 
                                                     NULL, outputFormat, 
                                                     fuzz_uint32_t_1, fuzz_uint32_t_2);
    if (!hTransform) {
        cmsCloseProfile(hProfile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Get transform context ID
    cmsContext transformCtx = cmsGetTransformContextID(hTransform);
    (void)transformCtx; // Use variable to avoid unused warning

    // Create a pipeline for evaluation
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, fuzz_uint32_t_3, fuzz_uint32_t_4);
    if (pipeline) {
        // Add an identity stage
        cmsStage* identity = cmsStageAllocIdentity(ctx, fuzz_uint32_t_5);
        if (identity) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, identity);
            
            // Evaluate float pipeline
            cmsFloat32Number inFloat[3] = {0.5f, 0.5f, 0.5f};
            cmsFloat32Number outFloat[3] = {0.0f, 0.0f, 0.0f};
            cmsPipelineEvalFloat(fuzzer_var_cmsPipelineEvalFloat_11_0, outFloat, pipeline);
        }
    }

    // Create and use IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        // Set index column
        cmsIT8SetIndexColumn(hIT8, fuzz_str_7);
        
        // Set property in hex
        cmsUInt32Number hexValue = 0xDEADBEEF;
        cmsIT8SetPropertyHex(hIT8, fuzz_str_8, fuzz_uint32_t_9);
    }

    // Create and use CIECAM02 model
    cmsViewingConditions viewingConditions;
    memset(&viewingConditions, 0, sizeof(viewingConditions));
    viewingConditions.whitePoint.X = 0.95047;
    viewingConditions.whitePoint.Y = 1.00000;
    viewingConditions.whitePoint.Z = 1.08883;
    viewingConditions.La = 100.0;
    viewingConditions.Yb = 20.0;
    viewingConditions.surround = DIM_SURROUND;
    viewingConditions.D_value = 0.95;
    
    cmsHANDLE hCIECAM = cmsCIECAM02Init(ctx, &viewingConditions);
    if (hCIECAM) {
        // Use the model
        cmsCIEXYZ inputXYZ = {0.5, 0.5, 0.5};
        cmsJCh outputJCh;
        cmsCIECAM02Forward(hCIECAM, &inputXYZ, &outputJCh);
        
        // Clean up CIECAM02 model
        cmsCIECAM02Done(hCIECAM);
    }

    // Create tone curve and get its parametric type
    cmsToneCurve* toneCurve = cmsBuildGamma(ctx, 2.2);
    if (toneCurve) {
        cmsInt32Number paramType = cmsGetToneCurveParametricType(toneCurve);
        (void)paramType; // Use variable to avoid unused warning
        cmsFreeToneCurve(toneCurve);
    }

    // Use cmsSliceSpace16
    if (pipeline) {
        cmsUInt32Number clutPoints[3] = {5, 5, 5};
        void* cargo = NULL;
        cmsBool sliceResult = cmsSliceSpace16(fuzzer_size_cmsSliceSpace16_21_1, fuzzer_var_cmsSliceSpace16_21_1,  id_000039_custom_sampler16, cargo);
        (void)sliceResult; // Use variable to avoid unused warning
    }

    // Clean up resources
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    cmsDeleteTransform(hTransform);
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}