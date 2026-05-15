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
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":28,"unique_branches":{},"library_calls":["cmsCreateContext","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsDeleteContext","cmsOpenProfileFromMemTHR","cmsCloseProfile","cmsDeleteContext","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsGBDAlloc","cmsGDBAddPoint","cmsGBDFree","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsStageData","cmsPipelineUnlinkStage","cmsStageFree","cmsFreeToneCurve","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsOpenProfileFromMemTHR","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsGBDAlloc","cmsGDBAddPoint","cmsGBDFree","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsStageData","cmsPipelineUnlinkStage","cmsStageFree","cmsFreeToneCurve","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<43) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_8, fuzz_double_array_size_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_17_2 = fuzz_double_array_8;//fuzzer shim end}




    if (size < 256) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // Create context for thread-safe operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Create Lab4 profile using D50 white point
    const cmsCIExyY* d50 = cmsD50_xyY();
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(ctx, d50);
    if (!labProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 2. Open input data as a profile
    cmsHPROFILE inputProfile = cmsOpenProfileFromMemTHR(ctx, data, size);
    if (!inputProfile) {
        // If not valid profile, create a simple one from the data
        cmsCloseProfile(labProfile);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 3. Get header information from input profile
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(inputProfile);
    cmsUInt32Number iccVersion = cmsGetEncodedICCversion(inputProfile);
    
    // 4. Create GBD (Gamut Boundary Description) and free it
    cmsHANDLE gbd = cmsGBDAlloc(ctx);
    if (gbd) {
        // Add a sample point to GBD if we have enough data
        if (size >= sizeof(cmsCIELab)) {
            cmsCIELab sampleLab;
            memcpy(&sampleLab, data, sizeof(cmsCIELab));
            cmsGDBAddPoint(gbd, &sampleLab);
        }
        cmsGBDFree(gbd);
    }
    
    // 5. Create transform for color conversion
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, inputProfile, 
                                                   TYPE_RGB_8, 
                                                   labProfile, 
                                                   TYPE_RGB_8, 
                                                   fuzz_uint32_t_1, 
                                                   fuzz_uint32_t_2);
    
    if (transform) {
        // 6. Perform transform with stride
        // Create sample input and output buffers
        const size_t pixels = 4;
        const size_t lines = 2;
        unsigned char inputBuffer[4 * 3 * 2];  // 4 pixels per line, 2 lines, RGB
        unsigned char outputBuffer[4 * 3 * 2];
        
        // Fill input buffer with some data from fuzzer input
        size_t copySize = sizeof(inputBuffer) < size ? sizeof(inputBuffer) : size;
        memcpy(inputBuffer, data, copySize);
        
        // Perform transform with stride
        cmsDoTransformLineStride(transform, 
                                inputBuffer, 
                                outputBuffer, 
                                pixels, 
                                lines, 
                                3 * pixels,      // BytesPerLineIn
                                3 * pixels,      // BytesPerLineOut
                                fuzz_uint32_t_3,              // BytesPerPlaneIn
                                fuzz_uint32_t_4);             // BytesPerPlaneOut
        
        // 7. Create pipeline and manipulate stages
        // First get pipeline from transform if possible
        // Since we can't directly get pipeline from transform, create a simple one
        cmsPipeline* pipeline = cmsPipelineAlloc(ctx, fuzz_uint32_t_5, fuzz_uint32_t_6);
        if (pipeline) {
            // Create identity stage
            cmsStage* identityStage = cmsStageAllocIdentity(ctx, fuzz_uint32_t_7);
            if (identityStage) {
                // Insert stage
                cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, identityStage);
                
                // Create a tone curve for testing
                cmsFloat64Number gammaParams[1] = {2.2};
                cmsToneCurve* toneCurve = cmsBuildParametricToneCurve(ctx, fuzz_int32_t_9, fuzzer_var_cmsBuildParametricToneCurve_17_2);
                
                if (toneCurve) {
                    // 8. Check if tone curve is monotonic
                    cmsBool isMonotonic = cmsIsToneCurveMonotonic(toneCurve);
                    
                    // Get stage data
                    void* stageData = cmsStageData(identityStage);
                    
                    // Unlink the stage
                    cmsPipelineUnlinkStage(pipeline, cmsAT_BEGIN, &identityStage);
                    
                    // Free the unlinked stage
                    if (identityStage) {
                        cmsStageFree(identityStage);
                    }
                    
                    cmsFreeToneCurve(toneCurve);
                }
            }
            cmsPipelineFree(pipeline);
        }
        
        cmsDeleteTransform(transform);
    }
    
    // Clean up
    cmsCloseProfile(inputProfile);
    cmsCloseProfile(labProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}