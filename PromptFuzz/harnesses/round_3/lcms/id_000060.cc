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
//<ID> 403
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"cmsCreateExtendedTransform":[[1226,34,1226,65,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsOpenProfileFromStream","cmsGetProfileVersion","cmsCreate_sRGBProfileTHR","cmsD50_xyY","cmsCreateLab2ProfileTHR","cmsCreateProofingTransformTHR","cmsPipelineAlloc","cmsStageAllocCLut16bit","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineInsertStage","cmsPipelineGetPtrToFirstStage","cmsStageSampleCLut16bit","cmsStageAllocCLutFloat","cmsStageAllocCLutFloatGranular","cmsStageFree","cmsStageFree","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsOpenProfileFromStream","cmsGetProfileVersion","cmsCreate_sRGBProfileTHR","cmsD50_xyY","cmsCreateLab2ProfileTHR","cmsCreateProofingTransformTHR","cmsPipelineAlloc","cmsStageAllocCLut16bit","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineInsertStage","cmsPipelineGetPtrToFirstStage","cmsStageSampleCLut16bit","cmsStageAllocCLutFloat","cmsStageAllocCLutFloatGranular","cmsStageFree","cmsStageFree","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a context first
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }

    // Open profile from memory using input data
    cmsHPROFILE inputProfile = cmsOpenProfileFromMem(data, size);
    if (!inputProfile) {
        // If opening from memory fails, try from file
        FILE* in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            inputProfile = cmsOpenProfileFromStream(in_file, "rb");
            assert_file_closed(&in_file);;
        }
    }

    // Get profile version
    cmsFloat64Number version = 0.0;
    if (inputProfile) {
        version = cmsGetProfileVersion(inputProfile);
    }

    // Create sRGB profile
    cmsHPROFILE srgbProfile = cmsCreate_sRGBProfileTHR(context);
    
    // Create Lab profile with D50 white point
    const cmsCIExyY* d50 = cmsD50_xyY();
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(context, d50);

    // Create proofing transform if we have valid profiles
    cmsHTRANSFORM transform = nullptr;
    if (inputProfile && srgbProfile && labProfile) {
        transform = cmsCreateProofingTransformTHR(
            context,
            inputProfile, TYPE_RGB_8,      // Input: RGB 8-bit
            srgbProfile, TYPE_RGB_8,       // Output: sRGB 8-bit
            labProfile,                    // Proofing profile
            INTENT_PERCEPTUAL,             // Rendering intent
            INTENT_ABSOLUTE_COLORIMETRIC,  // Proofing intent
            cmsFLAGS_SOFTPROOFING
        );
    }

    // Create a pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        // Create 16-bit CLUT stage
        cmsUInt32Number gridPoints = 2; // Small grid for fuzzing
        cmsUInt16Number table16[2*2*2*3] = {0}; // 2^3 * 3 channels
        if (size >= sizeof(table16)) {
            memcpy(table16, data, sizeof(table16));
        }
        cmsStage* clut16 = cmsStageAllocCLut16bit(
            context, gridPoints, 3, 3, table16
        );
        
        // Create matrix stage
        cmsFloat64Number matrix[9] = {1,0,0,0,1,0,0,0,1};
        cmsFloat64Number offset[3] = {0,0,0};
        cmsStage* matrixStage = cmsStageAllocMatrix(
            context, 3, 3, matrix, offset
        );
        
        // Insert stages into pipeline
        if (clut16) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, clut16);
        }
        if (matrixStage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, matrixStage);
        }
        
        // Get first stage
        cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
        
        // Sample CLUT if we have one
        if (clut16) {
            // Dummy sampler function
            auto dummySampler = [](const cmsUInt16Number* in, 
                                  cmsUInt16Number* out, 
                                  void* cargo) -> cmsInt32Number {
                if (in && out) {
                    out[0] = in[0];
                    out[1] = in[1];
                    out[2] = in[2];
                }
                return 1;
            };
            
            cmsStageSampleCLut16bit(clut16, dummySampler, nullptr, 0);
        }
        
        // Create float CLUT stages
        cmsFloat32Number tableFloat[2*2*2*3] = {0};
        if (size >= sizeof(tableFloat)) {
            memcpy(tableFloat, data, sizeof(tableFloat));
        }
        
        cmsStage* clutFloat = cmsStageAllocCLutFloat(
            context, gridPoints, 3, 3, tableFloat
        );
        
        cmsUInt32Number clutPoints[3] = {2, 2, 2};
        cmsStage* clutFloatGranular = cmsStageAllocCLutFloatGranular(
            context, clutPoints, 3, 3, tableFloat
        );
        
        // Free stages
        if (clutFloat) cmsStageFree(clutFloat);
        if (clutFloatGranular) cmsStageFree(clutFloatGranular);
        
        // Free pipeline (will free inserted stages)
        cmsPipelineFree(pipeline);
    }

    // Try to create IT8 handle and free it
    cmsHANDLE it8 = nullptr;
    // Note: We can't create IT8 from the provided APIs list, so we skip
    // cmsIT8Free(it8); // Can't call without valid handle

    // Try dictionary operations
    // Note: We can't create dictionary from provided APIs, so we skip
    // const cmsDICTentry* dictEntry = nullptr;
    // dictEntry = cmsDictNextEntry(dictEntry);

    // Cleanup
    if (transform) {
        cmsDeleteTransform(transform);
    }
    if (inputProfile) {
        cmsCloseProfile(inputProfile);
    }
    if (srgbProfile) {
        cmsCloseProfile(srgbProfile);
    }
    if (labProfile) {
        cmsCloseProfile(labProfile);
    }
    
    cmsDeleteContext(context);
    
    return 0;
}