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
//<ID> 302
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsDeleteContext","cmsGetColorSpace","cmsGetHeaderCreator","cmsCreateLab2ProfileTHR","cmsCloseProfile","cmsDeleteContext","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsPipelineInputChannels","cmsPipelineOutputChannels","cmsSetAdaptationState","cmsSliceSpaceFloat","cmsXYZEncoded2Float","cmsLabEncoded2Float","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsStageNext","cmsSetAdaptationState","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsGetColorSpace","cmsGetHeaderCreator","cmsCreateLab2ProfileTHR","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsPipelineInputChannels","cmsPipelineOutputChannels","cmsSetAdaptationState","cmsSliceSpaceFloat","cmsXYZEncoded2Float","cmsLabEncoded2Float","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsStageNext","cmsSetAdaptationState","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<23) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegralArray(uint32_t, fuzz_uint32_t_array_3, fuzz_uint32_t_array_size_3, fdp);
	
		const cmsUInt32Number * fuzzer_var_cmsSliceSpaceFloat_13_1 = fuzz_uint32_t_array_3;
		cmsUInt32Number fuzzer_size_cmsSliceSpaceFloat_13_1 = static_cast<cmsUInt32Number>(fuzz_uint32_t_array_size_3);//fuzzer shim end}




    // Create a context for lcms operations
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }

    // Open a profile from the input data (assumed to be ICC profile data)
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        cmsDeleteContext(context);
        return 0;
    }

    // Get profile information
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile);
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    
    // Create a transform from the input profile to Lab color space
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(context, nullptr);
    if (!labProfile) {
        cmsCloseProfile(hProfile);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Create a transform
    cmsHTRANSFORM hTransform = cmsCreateTransformTHR(context, hProfile, 
                                                    TYPE_Lab_DBL, 
                                                    labProfile, 
                                                    TYPE_Lab_DBL, 
                                                    fuzz_uint32_t_1, fuzz_uint32_t_2);
    
    if (hTransform) {
        // Get the transform pipeline
        cmsPipeline* pipeline = cmsGetTransformPipeline(hTransform);
        if (pipeline) {
            // Get pipeline channel information
            cmsUInt32Number inputChannels = cmsPipelineInputChannels(pipeline);
            cmsUInt32Number outputChannels = cmsPipelineOutputChannels(pipeline);
            
            // Set adaptation state
            cmsFloat64Number prevAdaptation = cmsSetAdaptationState(1.0);
            
            // Prepare for sampling
            if (inputChannels > 0 && outputChannels > 0) {
                // Create simple CLUT points array
                cmsUInt32Number clutPoints[3] = {2, 2, 2}; // For 3 input channels
                
                // Define a sampler function
                auto sampler = [](const cmsFloat32Number* in, cmsFloat32Number* out, void* cargo) {
                    // Simple identity sampler for demonstration
                    for (int i = 0; i < 3; i++) {
                        out[i] = in[i];
                    }
                    return 1;
                };
                
                // Sample the color space
                cmsSliceSpaceFloat(fuzzer_size_cmsSliceSpaceFloat_13_1, fuzzer_var_cmsSliceSpaceFloat_13_1, sampler, nullptr);
            }
            
            // Convert encoded color values (if we have enough data)
            if (size >= 6) {
                cmsCIEXYZ xyz;
                cmsUInt16Number encodedXYZ[3];
                memcpy(encodedXYZ, data, 6);
                cmsXYZEncoded2Float(&xyz, encodedXYZ);
            }
            
            if (size >= 6) {
                cmsCIELab lab;
                cmsUInt16Number encodedLab[3];
                memcpy(encodedLab, data, 6);
                cmsLabEncoded2Float(&lab, encodedLab);
            }
            
            // Check stage types in pipeline (if any)
            if (pipeline) {
                cmsStage* stage = cmsPipelineGetPtrToFirstStage(pipeline);
                while (stage) {
                    cmsStageSignature stageType = cmsStageType(stage);
                    stage = cmsStageNext(stage);
                }
            }
            
            // Restore adaptation state
            cmsSetAdaptationState(prevAdaptation);
        }
        
        // Clean up transform
        cmsDeleteTransform(hTransform);
    }
    
    // Clean up profiles
    cmsCloseProfile(labProfile);
    cmsCloseProfile(hProfile);
    
    // Delete context
    cmsDeleteContext(context);
    
    return 0;
}