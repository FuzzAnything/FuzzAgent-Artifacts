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
//<ID> 107
//<Prompt> []
/*<Combination>: [
*/
//<score> 50, nr_unique_branch: 9
//<Quality> {"density":25,"unique_branches":{"TetrahedralInterp16":[[776,20,776,28,0,0,4,1],[793,20,793,28,0,0,4,0],[793,20,793,28,0,0,4,1],[806,13,806,21,0,0,4,0],[809,20,809,28,0,0,4,0],[809,20,809,28,0,0,4,1],[820,20,820,28,0,0,4,1],[837,20,837,28,0,0,4,0],[837,20,837,28,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsDeleteContext","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsFreeToneCurve","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsStageData","cmsPipelineUnlinkStage","cmsStageFree","cmsPipelineFree","cmsOpenProfileFromMem","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsDeleteTransform","cmsCloseProfile","cmsDictAlloc","cmsDictFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsFreeToneCurve","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsStageData","cmsPipelineUnlinkStage","cmsStageFree","cmsPipelineFree","cmsOpenProfileFromMem","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsDeleteTransform","cmsCloseProfile","cmsDictAlloc","cmsDictFree","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<114) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeDoublingArray(fuzz_double_array_1, fuzz_double_array_size_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_5, fuzz_double_array_size_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
	
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_4_2 = fuzz_double_array_1;
		const cmsFloat64Number * fuzzer_var_cmsStageAllocMatrix_8_3 = fuzz_double_array_5;
		FDPGetRandomInteger(cmsUInt32Number, fuzzer_index_cmsStageAllocMatrix_8_3, fuzz_double_array_size_5)//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimal data for operations
    
    // 1. Create context for thread-safe operations
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // 2. Create Lab4 profile with D50 white point
    const cmsCIExyY* d50 = cmsD50_xyY();
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(ctx, d50);
    if (!labProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 3. Check monotonicity of a tone curve (create and test one)
    cmsFloat64Number gammaParams[1] = {2.2};
    cmsToneCurve* toneCurve = cmsBuildParametricToneCurve(ctx, fuzz_int32_t_2, fuzzer_var_cmsBuildParametricToneCurve_4_2);
    if (toneCurve) {
        cmsIsToneCurveMonotonic(toneCurve);
        cmsFreeToneCurve(toneCurve);
    }
    
    // 4. Create a simple pipeline for stage operations
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, fuzz_uint32_t_3, fuzz_uint32_t_4);
    if (pipeline) {
        // Create and add a matrix stage
        cmsFloat64Number matrix[9] = {1,0,0,0,1,0,0,0,1};
        cmsStage* matrixStage = cmsStageAllocMatrix(ctx, 3, fuzzer_index_cmsStageAllocMatrix_8_3, fuzzer_var_cmsStageAllocMatrix_8_3, nullptr);
        if (matrixStage) {
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, matrixStage);
            
            // Get stage data
            void* stageData = cmsStageData(matrixStage);
            
            // Unlink the stage
            cmsStage* unlinkedStage = nullptr;
            cmsPipelineUnlinkStage(pipeline, cmsAT_BEGIN, &unlinkedStage);
            
            if (unlinkedStage) {
                cmsStageFree(unlinkedStage);
            }
        }
        cmsPipelineFree(pipeline);
    }
    
    // 5. Create transform and perform line stride transformation
    // First, open input data as a profile
    cmsHPROFILE inputProfile = cmsOpenProfileFromMem(data, size);
    if (inputProfile) {
        // Get profile metadata
        cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(inputProfile);
        cmsUInt32Number iccVersion = cmsGetEncodedICCversion(inputProfile);
        
        // Create transform
        cmsUInt32Number intent = INTENT_PERCEPTUAL;
        cmsUInt32Number flags = cmsFLAGS_NOCACHE;
        cmsHTRANSFORM transform = cmsCreateTransformTHR(
            ctx, inputProfile, TYPE_RGB_8, 
            labProfile, TYPE_Lab_8, 
            fuzz_uint32_t_6, fuzz_uint32_t_7);
        
        if (transform) {
            // Prepare test buffers for transform
            const size_t pixels = 10;
            const size_t stride = pixels * 3;  // RGB 8-bit
            uint8_t inputBuffer[pixels * 3];
            uint8_t outputBuffer[pixels * 3];
            
            // Fill input buffer with test pattern
            for (size_t i = 0; i < sizeof(inputBuffer); i++) {
                inputBuffer[i] = (uint8_t)(i % 256);
            }
            
            // Perform transform with stride
            cmsDoTransformLineStride(
                transform,
                inputBuffer, outputBuffer,
                pixels, 1,  // 1 line of pixels
                stride, stride,  // bytes per line
                fuzz_uint32_t_8, fuzz_uint32_t_9  // planar not used
            );
            
            cmsDeleteTransform(transform);
        }
        cmsCloseProfile(inputProfile);
    }
    
    // 6. GBD (Gamut Boundary Descriptor) operations
    // Note: cmsGBDAlloc is not in provided API list, using available alternative
    // Create a dummy GBD-like structure using available APIs
    cmsHANDLE gbdHandle = cmsDictAlloc(ctx);
    if (gbdHandle) {
        cmsDictFree(gbdHandle);
    }
    
    // Cleanup
    cmsCloseProfile(labProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}