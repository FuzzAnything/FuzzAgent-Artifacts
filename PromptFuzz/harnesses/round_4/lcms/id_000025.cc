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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 2
//<Quality> {"density":16,"unique_branches":{"GetXFormColorSpaces":[[1035,13,1035,29,0,0,4,0]],"cmsCreateExtendedTransform":[[1168,9,1168,86,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDupContext","cmsDeleteContext","cmsDeleteContext","cmsDeleteContext","cmsDeleteContext","cmsCreate_sRGBProfile","cmsCreateLab2Profile","cmsCreateInkLimitingDeviceLinkTHR","cmsCreateExtendedTransform","cmsAdaptToIlluminant","cmsTempFromWhitePoint","cmsDeltaE","cmsGetPostScriptCSA","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageDup","cmsStageFree","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsDupContext","cmsCreate_sRGBProfile","cmsCreateLab2Profile","cmsCreateInkLimitingDeviceLinkTHR","cmsCreateExtendedTransform","cmsAdaptToIlluminant","cmsTempFromWhitePoint","cmsDeltaE","cmsGetPostScriptCSA","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageDup","cmsStageFree","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // Create context and duplicate it
    cmsContext context = cmsCreateContext(NULL, NULL);
    cmsContext dupContext = cmsDupContext(context, NULL);
    
    // Create a temporary file from input data for profile operations
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(dupContext);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for writing results
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(dupContext);
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create profiles for transform
    cmsHPROFILE profiles[3];
    profiles[0] = cmsCreate_sRGBProfile();
    profiles[1] = cmsCreateLab2Profile(NULL);
    profiles[2] = cmsCreateInkLimitingDeviceLinkTHR(dupContext, cmsSigRgbData, 0.8);
    
    // Setup transform parameters
    cmsBool bpc[3] = {TRUE, FALSE, TRUE};
    cmsUInt32Number intents[3] = {INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC, INTENT_SATURATION};
    cmsFloat64Number adaptationStates[3] = {1.0, 0.5, 0.0};
    
    // Create extended transform
    cmsHTRANSFORM transform = cmsCreateExtendedTransform(
        dupContext, 3, profiles, bpc, intents, adaptationStates,
        NULL, fuzz_uint32_t_1, TYPE_RGB_8, TYPE_Lab_8, fuzz_uint32_t_2
    );
    
    if (transform) {
        // Test color adaptation
        cmsCIEXYZ sourceWhite, illuminant, value, result;
        sourceWhite.X = 0.9505; sourceWhite.Y = 1.0000; sourceWhite.Z = 1.0890;
        illuminant.X = 0.9642; illuminant.Y = 1.0000; illuminant.Z = 0.8249;
        value.X = 0.5; value.Y = 0.5; value.Z = 0.5;
        
        cmsAdaptToIlluminant(&result, &sourceWhite, &illuminant, &value);
        
        // Test temperature from white point
        cmsCIExyY whitePoint;
        whitePoint.x = 0.3127; whitePoint.y = 0.3290; whitePoint.Y = 1.0;
        cmsFloat64Number tempK;
        cmsTempFromWhitePoint(&tempK, &whitePoint);
        
        // Test delta E calculation
        cmsCIELab lab1, lab2;
        lab1.L = 50.0; lab1.a = 0.0; lab1.b = 0.0;
        lab2.L = 55.0; lab2.a = 5.0; lab2.b = 5.0;
        cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);
        
        // Get PostScript CSA
        char buffer[1024];
        cmsUInt32Number csaSize = cmsGetPostScriptCSA(
            dupContext, profiles[0], fuzz_uint32_t_3, fuzz_uint32_t_4, buffer, sizeof(buffer)
        );
        
        // Write CSA to output file if available
        if (csaSize > 0 && csaSize <= sizeof(buffer)) {
            fwrite(buffer, 1, csaSize, out_file);
        }
        
        // Test stage duplication (if we can get a stage from transform)
        cmsPipeline* pipeline = cmsGetTransformPipeline(transform);
        if (pipeline) {
            cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (firstStage) {
                cmsStage* dupStage = cmsStageDup(firstStage);
                if (dupStage) {
                    cmsStageFree(dupStage);
                }
            }
        }
        
        // Clean up transform
        cmsDeleteTransform(transform);
    }
    
    // Clean up profiles
    for (int i = 0; i < 3; i++) {
        if (profiles[i]) cmsCloseProfile(profiles[i]);
    }
    
    // Clean up files and contexts
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsDeleteContext(dupContext);
    cmsDeleteContext(context);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}