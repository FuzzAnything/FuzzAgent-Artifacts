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
//<ID> 304
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.6666665, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["cmsCreateContext","cmsPluginTHR","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsFreeToneCurve","cmsDeleteContext","cmsGetHeaderRenderingIntent","cmsGetHeaderAttributes","cmsSetProfileVersion","cmsSetAlarmCodes","cmsMLUalloc","cmsReadTag","cmsMLUtranslationsCodes","cmsMLUfree","cmsBuildGamma","cmsCreateRGBProfile","cmsFreeToneCurve","cmsCreateTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsSaveProfileToFile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsPluginTHR","cmsOpenProfileFromStreamTHR","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsFreeToneCurve","cmsGetHeaderRenderingIntent","cmsGetHeaderAttributes","cmsSetProfileVersion","cmsSetAlarmCodes","cmsMLUalloc","cmsReadTag","cmsMLUtranslationsCodes","cmsMLUfree","cmsBuildGamma","cmsCreateRGBProfile","cmsFreeToneCurve","cmsCreateTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsSaveProfileToFile","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Minimum size for meaningful operations
    
    // 1. Create context for plugin operations
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Try to register a plugin (passing NULL since we don't have actual plugin)
    cmsPluginTHR(context, nullptr);
    
    // 3. Open profile from input data (consuming the data)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (!hProfile) {
        // If opening fails, create a minimal profile to continue testing APIs
        const cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
        cmsToneCurve* gamma = cmsBuildGamma(context, 2.2);
        hProfile = cmsCreateGrayProfileTHR(context, &whitePoint, gamma);
        cmsFreeToneCurve(gamma);
        if (!hProfile) {
            assert_file_closed(&in_file);;
            cmsDeleteContext(context);
            assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // 4. Get rendering intent from profile
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);
    
    // 5. Get profile attributes
    cmsUInt64Number flags;
    cmsGetHeaderAttributes(hProfile, &flags);
    
    // 6. Set profile version
    cmsSetProfileVersion(hProfile, 4.0); // ICC version 4.0
    
    // 7. Set alarm codes
    cmsUInt16Number alarmCodes[16] = {0};
    for (int i = 0; i < 16 && i * sizeof(cmsUInt16Number) < size; ++i) {
        alarmCodes[i] = *reinterpret_cast<const cmsUInt16Number*>(data + (i * sizeof(cmsUInt16Number)) % size);
    }
    cmsSetAlarmCodes(alarmCodes);
    
    // 8. Create MLU and get translation codes (if available in profile)
    cmsMLU* mlu = cmsMLUalloc(context, 1);
    if (mlu) {
        char langCode[3] = {0};
        char countryCode[3] = {0};
        
        // Try to get MLU from profile's description tag
        cmsMLU* descMLU = static_cast<cmsMLU*>(cmsReadTag(hProfile, cmsSigProfileDescriptionTag));
        if (descMLU) {
            // Get translation codes for first entry
            cmsMLUtranslationsCodes(descMLU, 0, langCode, countryCode);
        }
        cmsMLUfree(mlu);
    }
    
    // 9. Create a transform using the profile
    cmsUInt32Number inputFormat = TYPE_RGB_8;
    cmsUInt32Number outputFormat = TYPE_RGB_8;
    
    // Create output profile (sRGB-like)
    const cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},
        {0.3000, 0.6000, 1.0},
        {0.1500, 0.0600, 1.0}
    };
    const cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    
    cmsToneCurve* transferFunc[3];
    for (int i = 0; i < 3; ++i) {
        transferFunc[i] = cmsBuildGamma(context, 2.2);
    }
    
    cmsHPROFILE outProfile = cmsCreateRGBProfile(&whitePoint, &primaries, transferFunc);
    
    for (int i = 0; i < 3; ++i) {
        cmsFreeToneCurve(transferFunc[i]);
    }
    
    if (outProfile) {
        cmsHTRANSFORM transform = cmsCreateTransform(
            hProfile,
            inputFormat,
            outProfile,
            outputFormat,
            intent,
            0
        );
        
        if (transform) {
            // Test the transform with some input
            unsigned char input[3] = {0x80, 0x80, 0x80};
            unsigned char output[3];
            cmsDoTransform(transform, input, output, 1);
            
            cmsDeleteTransform(transform);
        }
        
        cmsCloseProfile(outProfile);
    }
    
    // 10. Write modified profile to output (for testing)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        cmsSaveProfileToFile(hProfile, "output_file");
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}