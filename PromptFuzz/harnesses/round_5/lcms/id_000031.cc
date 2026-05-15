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
//<ID> 184
//<Prompt> []
/*<Combination>: [
*/
//<score> 69, nr_unique_branch: 8
//<Quality> {"density":23,"unique_branches":{"cmsPipelineUnlinkStage":[[1563,9,1563,31,0,0,4,0],[1564,13,1564,16,0,0,4,0]],"cmsPipelineEvalReverseFloat":[[1808,13,1808,23,0,0,4,0],[1833,13,1833,51,0,0,4,1],[1842,19,1842,24,0,0,4,0],[1842,19,1842,24,0,0,4,1],[1843,17,1843,25,0,0,4,1],[1845,21,1845,31,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsDeleteContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsCreate_sRGBProfileTHR","cmsGetHeaderModel","cmsAllocNamedColorList","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsPipelineEvalReverseFloat","cmsPipelineUnlinkStage","cmsStageFree","cmsFreeToneCurve","cmsPipelineFree","cmsGBDAlloc","cmsGDBCompute","cmsGBDFree","cmsCIE2000DeltaE","cmsGetContextUserData","cmsFreeNamedColorList","cmsCloseProfile","cmsOpenProfileFromFileTHR","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromFileTHR","cmsCreate_sRGBProfileTHR","cmsGetHeaderModel","cmsAllocNamedColorList","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsPipelineEvalReverseFloat","cmsPipelineUnlinkStage","cmsStageFree","cmsFreeToneCurve","cmsPipelineFree","cmsGBDAlloc","cmsGDBCompute","cmsGBDFree","cmsCIE2000DeltaE","cmsGetContextUserData","cmsFreeNamedColorList","cmsCloseProfile","cmsOpenProfileFromFileTHR","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


#ifdef __cplusplus
extern "C" {
#endif

// Custom log error handler
static void myLogErrorHandler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text) {
    // Silent error handler for fuzzing
    (void)ContextID;
    (void)ErrorCode;
    (void)Text;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
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
	  // Need minimum data
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Set custom error handler
    cmsSetLogErrorHandlerTHR(ctx, myLogErrorHandler);
    
    // Write input data to temporary file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for profile operations
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);
    (void)in_fd; // Use to avoid unused variable warning
    
    // Create a profile from input file
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(ctx, "input_file", "r");
    
    // If profile creation from file fails, create a synthetic profile
    if (!hProfile) {
        // Create a simple sRGB profile instead
        hProfile = cmsCreate_sRGBProfileTHR(ctx);
    }
    
    if (hProfile) {
        // Get header model
        cmsUInt32Number model = cmsGetHeaderModel(hProfile);
        (void)model;
        
        // Allocate named color list
        cmsNAMEDCOLORLIST *namedColors = cmsAllocNamedColorList(ctx, 10, 3, "Prefix", "Suffix");
        
        // Allocate pipeline
        cmsPipeline *pipeline = cmsPipelineAlloc(ctx, 3, 3);
        
        if (pipeline) {
            // Build gamma tone curve
            cmsToneCurve *gammaCurve = cmsBuildGamma(ctx, 2.2);
            
            if (gammaCurve) {
                // Create array of tone curves for stage
                cmsToneCurve *curves[3] = {gammaCurve, gammaCurve, gammaCurve};
                
                // Allocate tone curves stage
                cmsStage *toneCurveStage = cmsStageAllocToneCurves(ctx, 3, curves);
                
                if (toneCurveStage) {
                    // Note: Normally we would insert stage into pipeline here
                    // but we need it for unlinking demonstration
                    
                    // Prepare data for pipeline evaluation
                    cmsFloat32Number target[3] = {0.5f, 0.5f, 0.5f};
                    cmsFloat32Number result[3] = {0.0f, 0.0f, 0.0f};
                    cmsFloat32Number hint[3] = {0.0f, 0.0f, 0.0f};
                    
                    // Try to evaluate pipeline in reverse (may fail if pipeline is empty)
                    cmsBool evalResult = cmsPipelineEvalReverseFloat(target, result, hint, pipeline);
                    (void)evalResult;
                    
                    // Unlink stage (demonstration - pipeline is empty)
                    cmsStage *unlinkedStage = NULL;
                    cmsPipelineUnlinkStage(pipeline, cmsAT_BEGIN, &unlinkedStage);
                    
                    // Free the stage
                    cmsStageFree(toneCurveStage);
                }
                
                // Free gamma curve
                cmsFreeToneCurve(gammaCurve);
            }
            
            // Free pipeline
            cmsPipelineFree(pipeline);
        }
        
        // Allocate GBD (Gamut Boundary Descriptor)
        cmsHANDLE hGBD = cmsGBDAlloc(ctx);
        
        if (hGBD) {
            // Try to compute GBD
            cmsBool gbdResult = cmsGDBCompute(hGBD, 0);
            (void)gbdResult;
            
            // Free GBD
            cmsGBDFree(hGBD);
        }
        
        // Calculate CIE2000 Delta E between two colors
        cmsCIELab lab1 = {50.0, 20.0, 30.0};
        cmsCIELab lab2 = {55.0, 25.0, 35.0};
        cmsFloat64Number deltaE = cmsCIE2000DeltaE(&lab1, &lab2, 1.0, 1.0, 1.0);
        (void)deltaE;
        
        // Get context user data
        void *userData = cmsGetContextUserData(ctx);
        (void)userData;
        
        // Free named color list
        if (namedColors) {
            cmsFreeNamedColorList(namedColors);
        }
        
        // Close profile
        cmsCloseProfile(hProfile);
    }
    
    // Cleanup files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    // Write input data to file for profile reading
    FILE *temp_file = fopen("input_file", "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
        
        // Try to open profile from the file we just created
        cmsHPROFILE tempProfile = cmsOpenProfileFromFileTHR(ctx, "input_file", "r");
        if (tempProfile) {
            cmsCloseProfile(tempProfile);
        }
        
        // Remove temporary file
        remove("input_file");
    }
    
    // Delete context
    cmsDeleteContext(ctx);
    
    assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
}

#ifdef __cplusplus
}
#endif