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
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["cmsCreateContext","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsDeleteContext","cmsOpenProfileFromMemTHR","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsFreeToneCurve","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageData","cmsPipelineUnlinkStage","cmsStageFree","cmsCreateTransformTHR","cmsGBDAlloc","cmsDoTransformLineStride","cmsGBDFree","cmsDeleteTransform","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsOpenProfileFromMemTHR","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsFreeToneCurve","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageData","cmsPipelineUnlinkStage","cmsStageFree","cmsCreateTransformTHR","cmsGBDAlloc","cmsDoTransformLineStride","cmsGBDFree","cmsDeleteTransform","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<59) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeDoublingArray(fuzz_double_array_1, fuzz_double_array_size_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_7, fuzz_char_array_size_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
	
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_7_2 = fuzz_double_array_1;
		const void * fuzzer_var_cmsDoTransformLineStride_18_1 = fuzz_char_array_7;
		FDPGetRandomInteger(cmsUInt32Number, fuzzer_index_cmsDoTransformLineStride_18_1, fuzz_char_array_size_7)//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Need minimum data for ICC header
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 1. Create Lab4 profile using D50 white point
    const cmsCIExyY* d50 = cmsD50_xyY();
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(context, d50);
    if (!labProfile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 2. Get header information from input data as profile
    cmsHPROFILE inputProfile = cmsOpenProfileFromMemTHR(context, data, size);
    if (inputProfile) {
        // Get manufacturer and encoded version
        cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(inputProfile);
        cmsUInt32Number encodedVersion = cmsGetEncodedICCversion(inputProfile);
        
        // 3. Create a tone curve and check monotonicity
        cmsFloat64Number gammaParams[1] = {2.2};
        cmsToneCurve* toneCurve = cmsBuildParametricToneCurve(context, 1, fuzzer_var_cmsBuildParametricToneCurve_7_2);
        if (toneCurve) {
            cmsBool isMonotonic = cmsIsToneCurveMonotonic(toneCurve);
            cmsFreeToneCurve(toneCurve);
        }
        
        // 4. Create a simple pipeline and manipulate stages
        cmsPipeline* pipeline = cmsPipelineAlloc(context, fuzz_uint32_t_2, fuzz_uint32_t_3);
        if (pipeline) {
            // Create identity stage
            cmsStage* identityStage = cmsStageAllocIdentity(context, fuzz_uint32_t_4);
            if (identityStage) {
                // Add stage to pipeline
                cmsPipelineInsertStage(pipeline, cmsAT_END, identityStage);
                
                // Get stage data
                void* stageData = cmsStageData(identityStage);
                
                // Unlink the stage (but don't remove since we need to free pipeline)
                cmsStage* unlinkedStage = nullptr;
                cmsPipelineUnlinkStage(pipeline, cmsAT_END, &unlinkedStage);
                
                // Free the unlinked stage
                if (unlinkedStage) {
                    cmsStageFree(unlinkedStage);
                }
            }
            
            // 5. Create transform and use cmsDoTransformLineStride
            cmsHTRANSFORM transform = cmsCreateTransformTHR(context, 
                inputProfile, TYPE_RGB_8, 
                labProfile, TYPE_RGB_8, 
                fuzz_uint32_t_5, fuzz_uint32_t_6);
            
            if (transform) {
                // Allocate GBD for gamut boundary description
                cmsHANDLE gbd = cmsGBDAlloc(context);
                
                if (gbd) {
                    // Prepare input/output buffers
                    cmsUInt8Number inputBuffer[9] = {0}; // 3 pixels of RGB
                    cmsUInt8Number outputBuffer[9] = {0};
                    
                    // Copy some data from input if available
                    size_t bytesToCopy = size < 9 ? size : 9;
                    memcpy(inputBuffer, data, bytesToCopy);
                    
                    // Perform transform with stride
                    cmsDoTransformLineStride(transform, 
                        fuzzer_var_cmsDoTransformLineStride_18_1, outputBuffer,
                        fuzzer_index_cmsDoTransformLineStride_18_1,  // Pixels per line
                        1,  // Line count
                        fuzz_uint32_t_8,  // Bytes per line in (3 pixels * 1 byte per channel)
                        fuzz_uint32_t_9,  // Bytes per line out
                        fuzz_uint32_t_10,  // Bytes per plane in (planar not used)
                        fuzz_uint32_t_11); // Bytes per plane out
                    
                    // Free GBD
                    cmsGBDFree(gbd);
                }
                
                cmsDeleteTransform(transform);
            }
            
            cmsPipelineFree(pipeline);
        }
        
        cmsCloseProfile(inputProfile);
    }
    
    // 6. Clean up
    cmsCloseProfile(labProfile);
    cmsDeleteContext(context);
    
    return 0;
}