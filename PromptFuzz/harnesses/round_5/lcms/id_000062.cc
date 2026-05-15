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
//<ID> 372
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsDeleteContext","cmsGetSupportedIntentsTHR","cmsDetectBlackPoint","cmsGetSupportedIntentsTHR","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsCreateRGBProfileTHR","cmsCreate_OkLabProfile","cmsCreateTransformTHR","cmsPipelineAlloc","cmsPipelineDup","cmsXYZEncoded2Float","cmsFloat2XYZEncoded","cmsXYZEncoded2Float","cmsDeleteTransform","cmsFreeToneCurve","cmsPipelineFree","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsGetSupportedIntentsTHR","cmsDetectBlackPoint","cmsGetSupportedIntentsTHR","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsCreateRGBProfileTHR","cmsCreate_OkLabProfile","cmsCreateTransformTHR","cmsPipelineAlloc","cmsPipelineDup","cmsXYZEncoded2Float","cmsFloat2XYZEncoded","cmsXYZEncoded2Float","cmsDeleteTransform","cmsFreeToneCurve","cmsPipelineFree","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to achieve an event involving profile creation, black point detection, and color space transformation:

*/


extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    cmsContext context = nullptr;
    cmsHPROFILE rgbProfile = nullptr;
    cmsHPROFILE okLabProfile = nullptr;
    cmsHPROFILE memProfile = nullptr;
    cmsPipeline* pipeline = nullptr;
    cmsPipeline* dupPipeline = nullptr;
    FILE* in_file = nullptr;
    
    // 1. Create context
    context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Create a profile from input data
    memProfile = cmsOpenProfileFromMemTHR(context, data, size);
    if (!memProfile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 3. Use cmsDetectBlackPoint on the memory profile
    cmsCIEXYZ blackPoint;
    cmsUInt32Number intent = 0;
    const char* descArray[10];
    cmsUInt32Number codes[10];
    
    // Get supported intents first to use with black point detection
    cmsUInt32Number supportedIntents = cmsGetSupportedIntentsTHR(
        context, 10, codes, (char**)descArray);
    
    if (supportedIntents > 0) {
        intent = codes[0];  // Use first supported intent
        
        // Detect black point
        cmsDetectBlackPoint(&blackPoint, memProfile, intent, 0);
    }
    
    // 4. Use cmsGetSupportedIntentsTHR (already called above, but ensure it's used)
    // We'll call it again to demonstrate usage
    cmsUInt32Number intentsCount = cmsGetSupportedIntentsTHR(
        context, 10, codes, (char**)descArray);
    
    // 5. Create RGB profile using standard sRGB parameters
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},  // Red
        {0.3000, 0.6000, 1.0},  // Green
        {0.1500, 0.0600, 1.0}   // Blue
    };
    
    // Create simple gamma curves
    cmsToneCurve* transferFunctions[3];
    cmsUInt16Number gammaValues[256];
    for (int i = 0; i < 256; i++) {
        gammaValues[i] = (i * 65535) / 255;
    }
    
    transferFunctions[0] = cmsBuildTabulatedToneCurve16(context, 256, gammaValues);
    transferFunctions[1] = cmsBuildTabulatedToneCurve16(context, 256, gammaValues);
    transferFunctions[2] = cmsBuildTabulatedToneCurve16(context, 256, gammaValues);
    
    rgbProfile = cmsCreateRGBProfileTHR(context, &whitePoint, &primaries, 
                                       (cmsToneCurve* const*)transferFunctions);
    
    // 6. Create OkLab profile
    okLabProfile = cmsCreate_OkLabProfile(context);
    
    // 7. Create pipeline if we have both profiles
    if (rgbProfile && okLabProfile) {
        cmsUInt32Number inputFormat = TYPE_RGB_16;
        cmsUInt32Number outputFormat = TYPE_Lab_16;
        
        // Create transform to get pipeline
        cmsHTRANSFORM transform = cmsCreateTransformTHR(context,
            rgbProfile, inputFormat,
            okLabProfile, outputFormat,
            intent, 0);
            
        if (transform) {
            // Get pipeline from transform (simulated - actual extraction would need transform internals)
            // For fuzzing purposes, we'll create a simple pipeline
            pipeline = cmsPipelineAlloc(context, 3, 3);
            
            if (pipeline) {
                // 8. Use cmsPipelineDup
                dupPipeline = cmsPipelineDup(pipeline);
                
                // 9. Use cmsXYZEncoded2Float
                cmsUInt16Number encodedXYZ[3] = {32768, 32768, 32768};  // Midpoint values
                cmsCIEXYZ floatXYZ;
                cmsXYZEncoded2Float(&floatXYZ, encodedXYZ);
                
                // Also convert black point to encoded and back
                cmsUInt16Number blackEncoded[3];
                cmsFloat2XYZEncoded(blackEncoded, &blackPoint);
                cmsCIEXYZ blackFloat;
                cmsXYZEncoded2Float(&blackFloat, blackEncoded);
            }
            
            cmsDeleteTransform(transform);
        }
    }
    
    // Cleanup
    for (int i = 0; i < 3; i++) {
        if (transferFunctions[i]) {
            cmsFreeToneCurve(transferFunctions[i]);
        }
    }
    
    if (dupPipeline) {
        cmsPipelineFree(dupPipeline);
    }
    
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (okLabProfile) {
        cmsCloseProfile(okLabProfile);
    }
    
    if (rgbProfile) {
        cmsCloseProfile(rgbProfile);
    }
    
    if (memProfile) {
        cmsCloseProfile(memProfile);
    }
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    return 0;
}