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
//<ID> 241
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 5
//<Quality> {"density":28,"unique_branches":{"EvalSegmentedFn":[[731,38,731,44,0,0,4,1],[734,40,734,64,0,0,4,1]],"cmsGDBAddPoint":[[370,9,370,31,0,0,4,1],[379,13,379,30,0,0,4,0],[379,13,379,30,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsD50_xyY","cmsDeleteContext","cmsCreateLab4ProfileTHR","cmsDeleteContext","cmsOpenProfileFromMemTHR","cmsCloseProfile","cmsDeleteContext","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsGBDAlloc","cmsGDBAddPoint","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsStageData","cmsPipelineUnlinkStage","cmsStageFree","cmsFreeToneCurve","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsGBDFree","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsOpenProfileFromMemTHR","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsGBDAlloc","cmsGDBAddPoint","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsStageData","cmsPipelineUnlinkStage","cmsStageFree","cmsFreeToneCurve","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsGBDFree","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<107) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_6, fuzz_double_array_size_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_17_2 = fuzz_double_array_6;//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful processing
    
    // 1. Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // 2. Get D50 white point
    const cmsCIExyY* d50 = cmsD50_xyY();
    if (!d50) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 3. Create Lab4 profile with D50
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(ctx, d50);
    if (!labProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 4. Open profile from input data (first half)
    size_t profileSize = size / 2;
    cmsHPROFILE inputProfile = cmsOpenProfileFromMemTHR(ctx, data, profileSize);
    if (!inputProfile) {
        cmsCloseProfile(labProfile);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 5. Get manufacturer from opened profile
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(inputProfile);
    
    // 6. Get encoded ICC version
    cmsUInt32Number iccVersion = cmsGetEncodedICCversion(inputProfile);
    
    // 7. Allocate GBD and add points using remaining data
    cmsHANDLE gbd = cmsGBDAlloc(ctx);
    if (gbd) {
        size_t labDataOffset = profileSize;
        size_t remaining = size - profileSize;
        size_t maxPoints = remaining / sizeof(cmsCIELab);
        
        for (size_t i = 0; i < maxPoints && i < 10; i++) {
            cmsCIELab lab;
            memcpy(&lab, data + labDataOffset + i * sizeof(cmsCIELab), sizeof(cmsCIELab));
            cmsGDBAddPoint(gbd, &lab);
        }
    }
    
    // 8. Create transform
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, 
        labProfile, 
        TYPE_Lab_DBL, 
        inputProfile, 
        TYPE_RGB_8, 
        fuzz_uint32_t_1, 
        fuzz_uint32_t_2);
    
    if (transform) {
        // 9. Prepare test buffers for transformation
        const size_t pixelsPerLine = 4;
        const size_t lineCount = 2;
        const size_t bytesPerLineIn = pixelsPerLine * sizeof(cmsCIELab);
        const size_t bytesPerLineOut = pixelsPerLine * 3; // RGB 8-bit
        const size_t bytesPerPlaneIn = bytesPerLineIn;
        const size_t bytesPerPlaneOut = bytesPerLineOut;
        
        cmsCIELab inputBuffer[8]; // 4 pixels × 2 lines
        uint8_t outputBuffer[24]; // 4 pixels × 2 lines × 3 channels
        
        // Fill input buffer with data from remaining input
        size_t dataOffset = profileSize;
        size_t bytesToCopy = sizeof(inputBuffer);
        if (size - dataOffset < bytesToCopy) {
            bytesToCopy = size - dataOffset;
        }
        if (bytesToCopy > 0) {
            memcpy(inputBuffer, data + dataOffset, bytesToCopy);
        }
        
        // 10. Perform transformation with line stride
        cmsDoTransformLineStride(transform, 
            inputBuffer, 
            outputBuffer, 
            pixelsPerLine, 
            lineCount, 
            bytesPerLineIn, 
            bytesPerLineOut, 
            bytesPerPlaneIn, 
            bytesPerPlaneOut);
        
        // 11. Create pipeline
        cmsPipeline* pipeline = cmsPipelineAlloc(ctx, fuzz_uint32_t_3, fuzz_uint32_t_4);
        if (pipeline) {
            // 12. Create identity stage
            cmsStage* identityStage = cmsStageAllocIdentity(ctx, fuzz_uint32_t_5);
            if (identityStage) {
                // 13. Insert identity stage
                cmsPipelineInsertStage(pipeline, cmsAT_END, identityStage);
                
                // 14. Create parametric tone curve
                cmsFloat64Number params[10] = {2.4};
                cmsToneCurve* toneCurve = cmsBuildParametricToneCurve(ctx, fuzz_int32_t_7, fuzzer_var_cmsBuildParametricToneCurve_17_2);
                if (toneCurve) {
                    // 15. Check monotonicity
                    cmsBool isMonotonic = cmsIsToneCurveMonotonic(toneCurve);
                    
                    // 16. Get stage data
                    void* stageData = cmsStageData(identityStage);
                    
                    // 17. Unlink stage
                    cmsStage* unlinkedStage = nullptr;
                    cmsPipelineUnlinkStage(pipeline, cmsAT_END, &unlinkedStage);
                    
                    // 18. Free unlinked stage
                    if (unlinkedStage) {
                        cmsStageFree(unlinkedStage);
                    }
                    
                    // 19. Free tone curve
                    cmsFreeToneCurve(toneCurve);
                }
                
                // 20. Free pipeline
                cmsPipelineFree(pipeline);
            }
        }
        
        // 21. Delete transform
        cmsDeleteTransform(transform);
    }
    
    // 22. Close profiles
    cmsCloseProfile(inputProfile);
    cmsCloseProfile(labProfile);
    
    // 23. Free GBD
    if (gbd) {
        cmsGBDFree(gbd);
    }
    
    // 24. Delete context
    cmsDeleteContext(ctx);
    
    return 0;
}