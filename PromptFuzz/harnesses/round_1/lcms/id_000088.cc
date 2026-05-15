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
//<ID> 733
//<Prompt> []
/*<Combination>: [
*/
//<score> 132, nr_unique_branch: 8
//<Quality> {"density":33,"unique_branches":{"OptimizeMatrixShaper":[[1691,45,1691,75,0,0,4,0]],"BuildGrayOutputPipeline":[[418,9,418,24,0,0,4,1],[421,9,421,27,0,0,4,1],[424,9,424,20,0,0,4,1],[429,9,429,45,0,0,4,1],[435,13,435,110,0,0,4,1],[439,9,439,100,0,0,4,1]],"_MultiplyMatrix":[[214,65,214,98,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsPluginTHR","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsGetHeaderAttributes","cmsGetHeaderRenderingIntent","cmsSetProfileVersion","cmsMLUalloc","cmsMLUtranslationsCodes","cmsMLUfree","cmsReadTag","cmsDetectDestinationBlackPoint","cmsBuildGamma","cmsBuildGamma","cmsD50_xyY","cmsCreateGrayProfileTHR","cmsCreateRGBProfile","cmsSetAlarmCodes","cmsSetAlarmCodesTHR","cmsCreateTransform","cmsDoTransform","cmsStageAllocIdentity","cmsGetTagSignature","cmsSetColorSpace","cmsSaveProfileToFile","cmsDeleteTransform","cmsStageFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsPluginTHR","cmsOpenProfileFromStreamTHR","cmsGetHeaderAttributes","cmsGetHeaderRenderingIntent","cmsSetProfileVersion","cmsMLUalloc","cmsMLUtranslationsCodes","cmsMLUfree","cmsReadTag","cmsDetectDestinationBlackPoint","cmsBuildGamma","cmsBuildGamma","cmsD50_xyY","cmsCreateGrayProfileTHR","cmsCreateRGBProfile","cmsSetAlarmCodes","cmsSetAlarmCodesTHR","cmsCreateTransform","cmsDoTransform","cmsStageAllocIdentity","cmsGetTagSignature","cmsSetColorSpace","cmsSaveProfileToFile","cmsDeleteTransform","cmsStageFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1024) {
	return 0;
	}
	
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Register plugin (NULL for fuzzing)
    cmsPluginTHR(ctx, NULL);
    
    // Open profile from input data stream
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE src_profile = cmsOpenProfileFromStreamTHR(ctx, in_file, "r");
    if (!src_profile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get header information
    cmsUInt64Number flags;
    cmsGetHeaderAttributes(src_profile, &flags);
    
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(src_profile);
    
    // Set profile version
    cmsSetProfileVersion(src_profile, 4.0);
    
    // Create MLU and use translations
    cmsMLU* mlu = cmsMLUalloc(ctx, 5);
    char lang[3], country[3];
    cmsMLUtranslationsCodes(mlu, 0, lang, country);
    cmsMLUfree(mlu);
    
    // Read a tag
    void* tag_data = cmsReadTag(src_profile, cmsSigCopyrightTag);
    
    // Detect black point
    cmsCIEXYZ black_point;
    cmsBool black_detected = cmsDetectDestinationBlackPoint(&black_point, src_profile, intent, 0);
    
    // Create tone curves for new profiles
    cmsToneCurve* gamma_curve = cmsBuildGamma(ctx, 2.2);
    cmsToneCurve* identity_curve = cmsBuildGamma(ctx, 1.0);
    
    // Create white point from D50
    const cmsCIExyY* d50 = cmsD50_xyY();
    
    // Create gray profile
    cmsHPROFILE gray_profile = cmsCreateGrayProfileTHR(ctx, d50, gamma_curve);
    
    // Create RGB profile
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 0.2126},  // Red
        {0.3000, 0.6000, 0.7152},  // Green
        {0.1500, 0.0600, 0.0722}   // Blue
    };
    cmsToneCurve* rgb_curves[3] = {gamma_curve, gamma_curve, gamma_curve};
    cmsHPROFILE rgb_profile = cmsCreateRGBProfile(d50, &primaries, rgb_curves);
    
    // Set alarm codes
    cmsUInt16Number alarm_codes[16];
    for (int i = 0; i < 16; i++) alarm_codes[i] = i * 100;
    cmsSetAlarmCodes(alarm_codes);
    cmsSetAlarmCodesTHR(ctx, alarm_codes);
    
    // Create transform
    cmsHTRANSFORM transform = cmsCreateTransform(
        src_profile, 
        TYPE_RGB_8,      // Input format
        gray_profile,    // Output profile
        TYPE_GRAY_8,     // Output format
        INTENT_PERCEPTUAL,
        0
    );
    
    if (transform) {
        // Perform color transformation
        unsigned char input_pixels[9] = {100, 150, 200, 50, 100, 150, 0, 50, 100};
        unsigned char output_pixels[3];
        cmsDoTransform(transform, input_pixels, output_pixels, 3);
    }
    
    // Create identity stage
    cmsStage* identity_stage = cmsStageAllocIdentity(ctx, 3);
    
    // Get tag signature
    cmsTagSignature sig = cmsGetTagSignature(src_profile, 0);
    
    // Set color space (to RGB for example)
    cmsSetColorSpace(src_profile, cmsSigRgbData);
    
    // Save profile to file
    cmsSaveProfileToFile(src_profile, "output_file");
    
    // Clean up
    if (transform) cmsDeleteTransform(transform);
    if (identity_stage) cmsStageFree(identity_stage);
    cmsFreeToneCurve(gamma_curve);
    cmsFreeToneCurve(identity_curve);
    cmsCloseProfile(src_profile);
    cmsCloseProfile(gray_profile);
    cmsCloseProfile(rgb_profile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}