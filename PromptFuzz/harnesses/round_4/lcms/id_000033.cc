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
//<ID> 239
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 0
//<Quality> {"density":26,"unique_branches":{},"library_calls":["cmsCreateContext","cmsD50_xyY","cmsDeleteContext","cmsCreateLab4ProfileTHR","cmsDeleteContext","cmsOpenProfileFromMemTHR","cmsGetHeaderManufacturer","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsGBDAlloc","cmsGDBAddPoint","cmsGDBAddPoint","cmsGBDFree","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsFreeToneCurve","cmsStageData","cmsPipelineUnlinkStage","cmsStageFree","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsOpenProfileFromMemTHR","cmsGetHeaderManufacturer","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsGBDAlloc","cmsGDBAddPoint","cmsGDBAddPoint","cmsGBDFree","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsFreeToneCurve","cmsStageData","cmsPipelineUnlinkStage","cmsStageFree","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// Include lcms headers

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<123) return 0;

	
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
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_10, fuzz_double_array_size_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_18_2 = fuzz_double_array_10;//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Get D50 white point
    const cmsCIExyY* d50 = cmsD50_xyY();
    if (!d50) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 3. Create Lab4 profile
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(context, d50);
    if (!labProfile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 4. Open profile from input memory
    cmsHPROFILE memProfile = cmsOpenProfileFromMemTHR(context, data, size);
    
    // 5. Get header manufacturer from both profiles
    cmsUInt32Number labManufacturer = cmsGetHeaderManufacturer(labProfile);
    cmsUInt32Number memManufacturer = 0;
    if (memProfile) {
        memManufacturer = cmsGetHeaderManufacturer(memProfile);
    }
    
    // 6. Get encoded ICC version
    cmsUInt32Number iccVersion = 0;
    if (memProfile) {
        iccVersion = cmsGetEncodedICCversion(memProfile);
    }
    
    // 7. Create GBD (Gamut Boundary Descriptor)
    cmsHANDLE gbd = cmsGBDAlloc(context);
    if (gbd) {
        // Add some sample Lab points
        cmsCIELab labPoint = {50.0, 20.0, -30.0};
        cmsGDBAddPoint(gbd, &labPoint);
        
        // Add another point from data if possible
        if (size >= sizeof(cmsCIELab)) {
            cmsCIELab dataLab;
            memcpy(&dataLab, data, sizeof(cmsCIELab));
            cmsGDBAddPoint(gbd, &dataLab);
        }
        
        // Free GBD
        cmsGBDFree(gbd);
    }
    
    // 8. Create transform if we have both profiles
    cmsHTRANSFORM transform = nullptr;
    if (memProfile) {
        transform = cmsCreateTransformTHR(context, 
                                          labProfile, 
                                          TYPE_Lab_16, 
                                          memProfile, 
                                          TYPE_RGB_8, 
                                          fuzz_uint32_t_1, 
                                          fuzz_uint32_t_2);
    }
    
    // 9. Perform transformation if transform was created
    if (transform) {
        // Prepare input/output buffers
        cmsUInt16Number inputBuffer[3] = {32768, 32768, 32768}; // Neutral gray in Lab
        cmsUInt8Number outputBuffer[3] = {0};
        
        // Do transform with line stride (single pixel)
        cmsDoTransformLineStride(transform, 
                                 inputBuffer, 
                                 outputBuffer, 
                                 1,  // PixelsPerLine
                                 1,  // LineCount
                                 fuzz_uint32_t_3,  // BytesPerLineIn (3*2 bytes for Lab16)
                                 fuzz_uint32_t_4,  // BytesPerLineOut (3*1 byte for RGB8)
                                 fuzz_uint32_t_5,  // BytesPerPlaneIn
                                 fuzz_uint32_t_6); // BytesPerPlaneOut
    }
    
    // 10. Create pipeline for color processing
    cmsPipeline* pipeline = cmsPipelineAlloc(context, fuzz_uint32_t_7, fuzz_uint32_t_8);
    if (pipeline) {
        // Create identity stage
        cmsStage* identityStage = cmsStageAllocIdentity(context, fuzz_uint32_t_9);
        if (identityStage) {
            // Insert stage at beginning
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, identityStage);
            
            // Create parametric tone curve
            cmsFloat64Number params[10] = {2.4, 1.0, 0.0, 0.0, 0.0}; // Gamma 2.4
            cmsToneCurve* toneCurve = cmsBuildParametricToneCurve(context, fuzz_int32_t_11, fuzzer_var_cmsBuildParametricToneCurve_18_2);
            if (toneCurve) {
                // Check if monotonic
                cmsBool isMonotonic = cmsIsToneCurveMonotonic(toneCurve);
                
                // Free tone curve
                cmsFreeToneCurve(toneCurve);
            }
            
            // Get stage data (identity matrix)
            void* stageData = cmsStageData(identityStage);
            
            // Unlink the stage
            cmsStage* unlinkedStage = nullptr;
            cmsPipelineUnlinkStage(pipeline, cmsAT_BEGIN, &unlinkedStage);
            
            // Free the unlinked stage
            if (unlinkedStage) {
                cmsStageFree(unlinkedStage);
            }
        }
        
        // Free pipeline
        cmsPipelineFree(pipeline);
    }
    
    // 11. Try to use IT8 functions if we have enough data
    // Create a temporary file with input data for IT8
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Note: We don't have cmsIT8LoadFromFile in the required list,
        // but we can demonstrate the other IT8 functions if we had a handle
        // For fuzzing purposes, we'll skip actual IT8 loading
        
        assert_file_closed(&in_file);;
    }
    
    // 12. Clean up
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (memProfile) {
        cmsCloseProfile(memProfile);
    }
    
    cmsCloseProfile(labProfile);
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}