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
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 81, nr_unique_branch: 2
//<Quality> {"density":27,"unique_branches":{"cmsPipelineUnlinkStage":[[1594,17,1594,25,0,0,4,0]],"BuildRGBOutputMatrixShaper":[[464,9,464,37,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsD50_xyY","cmsDeleteContext","cmsCreateLab4ProfileTHR","cmsDeleteContext","cmsOpenProfileFromMemTHR","cmsCloseProfile","cmsDeleteContext","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsGBDAlloc","cmsGDBAddPoint","cmsGBDFree","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsStageData","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineUnlinkStage","cmsStageFree","cmsFreeToneCurve","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsOpenProfileFromMemTHR","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsGBDAlloc","cmsGDBAddPoint","cmsGBDFree","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsStageData","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineUnlinkStage","cmsStageFree","cmsFreeToneCurve","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to process ICC profile data:

*/


// Include necessary lcms headers (these would typically be in the actual environment)

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<75) return 0;

	
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
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
	
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_18_2 = fuzz_double_array_8;//fuzzer shim end}




    // Early return for insufficient data
    if (size < 64) {
	return 0;
	}
	
    
    cmsContext context = nullptr;
    cmsHPROFILE labProfile = nullptr;
    cmsHPROFILE inputProfile = nullptr;
    cmsHTRANSFORM transform = nullptr;
    cmsPipeline* pipeline = nullptr;
    cmsStage* stage = nullptr;
    cmsToneCurve* toneCurve = nullptr;
    cmsHANDLE gbd = nullptr;
    
    // 1. Create context
    context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Get D50 white point
    const cmsCIExyY* d50 = cmsD50_xyY();
    if (!d50) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 3. Create Lab4 profile using D50
    labProfile = cmsCreateLab4ProfileTHR(context, d50);
    if (!labProfile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 4. Open profile from input memory
    inputProfile = cmsOpenProfileFromMemTHR(context, data, static_cast<cmsUInt32Number>(size));
    if (!inputProfile) {
        cmsCloseProfile(labProfile);
        cmsDeleteContext(context);
        return 0;
    }
    
    // 5. Get header manufacturer
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(inputProfile);
    (void)manufacturer; // Use variable to avoid unused warning
    
    // 6. Get encoded ICC version
    cmsUInt32Number version = cmsGetEncodedICCversion(inputProfile);
    (void)version;
    
    // 7. Create GBD and add a sample Lab point
    gbd = cmsGBDAlloc(context);
    if (gbd) {
        cmsCIELab sampleLab = {50.0, 0.0, 0.0}; // Neutral gray
        cmsGDBAddPoint(gbd, &sampleLab);
        cmsGBDFree(gbd);
    }
    
    // 8. Create transform from Lab4 to input profile
    transform = cmsCreateTransformTHR(context,
                                     labProfile,
                                     TYPE_Lab_DBL,
                                     inputProfile,
                                     TYPE_RGB_8,
                                     fuzz_uint32_t_1,
                                     fuzz_uint32_t_2);
    
    if (transform) {
        // 9. Create sample data for transformation
        const size_t pixels = 4;
        const size_t lines = 2;
        std::vector<cmsCIELab> inputBuffer(pixels * lines);
        std::vector<unsigned char> outputBuffer(pixels * lines * 3);
        
        // Initialize with some data
        for (size_t i = 0; i < inputBuffer.size(); i++) {
            inputBuffer[i].L = 50.0 + (i % 20);
            inputBuffer[i].a = 0.0;
            inputBuffer[i].b = 0.0;
        }
        
        // Perform transformation with stride
        cmsDoTransformLineStride(transform,
                                inputBuffer.data(),
                                outputBuffer.data(),
                                static_cast<cmsUInt32Number>(pixels),
                                static_cast<cmsUInt32Number>(lines),
                                sizeof(cmsCIELab) * pixels,
                                3 * pixels,
                                fuzz_uint32_t_3,
                                fuzz_uint32_t_4);
        
        // 10. Create pipeline
        pipeline = cmsPipelineAlloc(context, fuzz_uint32_t_5, fuzz_uint32_t_6);
        if (pipeline) {
            // 11. Allocate identity stage
            stage = cmsStageAllocIdentity(context, fuzz_uint32_t_7);
            if (stage) {
                // 12. Insert stage into pipeline
                cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
                stage = nullptr; // Pipeline now owns the stage
                
                // 13. Create parametric tone curve
                cmsFloat64Number params[5] = {2.4, 1.0, 0.0, 0.0, 0.0};
                toneCurve = cmsBuildParametricToneCurve(context, 1, fuzzer_var_cmsBuildParametricToneCurve_18_2);
                if (toneCurve) {
                    // 14. Check if tone curve is monotonic
                    cmsBool isMonotonic = cmsIsToneCurveMonotonic(toneCurve);
                    (void)isMonotonic;
                    
                    // 15. Get stage data (for demonstration, even though we unlinked it)
                    if (stage) {
                        void* stageData = cmsStageData(stage);
                        (void)stageData;
                    }
                    
                    // 16. Unlink stage (if we still had a reference)
                    // Note: We can't unlink because stage is nullptr after pipeline took ownership
                    // For demonstration, we'll create another stage and unlink it
                    cmsStage* tempStage = cmsStageAllocIdentity(context, fuzz_uint32_t_9);
                    if (tempStage) {
                        cmsPipelineInsertStage(pipeline, cmsAT_END, tempStage);
                        cmsPipelineUnlinkStage(pipeline, cmsAT_END, &tempStage);
                        if (tempStage) {
                            cmsStageFree(tempStage);
                        }
                    }
                    
                    // 17. Free tone curve
                    cmsFreeToneCurve(toneCurve);
                }
            }
        }
    }
    
    // Cleanup in reverse order
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (inputProfile) {
        cmsCloseProfile(inputProfile);
    }
    
    if (labProfile) {
        cmsCloseProfile(labProfile);
    }
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    return 0;
}