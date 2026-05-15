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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 14.5, nr_unique_branch: 0
//<Quality> {"density":29,"unique_branches":{},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsDeleteContext","cmsGetColorSpace","cmsBuildParametricToneCurve","cmsCloseProfile","cmsDeleteContext","cmsCreateGrayProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext","cmsStageAllocIdentity","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext","cmsStageInputChannels","cmsMLUalloc","cmsMLUsetASCII","cmsMLUtranslationsCodes","cmsMLUfree","cmsIT8LoadFromFile","cmsIT8GetDataRowCol","cmsIT8Free","cmsStageFree","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsGetColorSpace","cmsBuildParametricToneCurve","cmsCreateGrayProfile","cmsStageAllocIdentity","cmsStageInputChannels","cmsMLUalloc","cmsMLUsetASCII","cmsMLUtranslationsCodes","cmsMLUfree","cmsIT8LoadFromFile","cmsIT8GetDataRowCol","cmsIT8Free","cmsStageFree","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create a context first
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Create NULL profile
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx);
    if (!nullProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 2. Get color space of NULL profile
    cmsColorSpaceSignature cs = cmsGetColorSpace(nullProfile);
    (void)cs; // Use variable to avoid unused warning
    
    // 3. Create white point from input data
    cmsCIExyY whitePoint;
    // Use first 24 bytes for white point (3 doubles)
    if (size >= 24) {
        memcpy(&whitePoint, data, sizeof(whitePoint));
        data += 24;
        size -= 24;
    } else {
        // Use default values if not enough data
        whitePoint.x = 0.3127;
        whitePoint.y = 0.3290;
        whitePoint.Y = 1.0;
    }
    
    // 4. Create parametric tone curve from input data
    cmsFloat64Number params[10];
    int paramCount = 5; // Use type 1 (power function) which needs 1 parameter
    if (size >= sizeof(cmsFloat64Number) * paramCount) {
        memcpy(params, data, sizeof(cmsFloat64Number) * paramCount);
        data += sizeof(cmsFloat64Number) * paramCount;
        size -= sizeof(cmsFloat64Number) * paramCount;
    } else {
        // Default gamma 2.2
        params[0] = 2.2;
    }
    
    cmsToneCurve* transferFunc = cmsBuildParametricToneCurve(ctx, 1, params);
    if (!transferFunc) {
        cmsCloseProfile(nullProfile);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 5. Create gray profile
    cmsHPROFILE grayProfile = cmsCreateGrayProfile(&whitePoint, transferFunc);
    if (!grayProfile) {
        cmsFreeToneCurve(transferFunc);
        cmsCloseProfile(nullProfile);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 6. Allocate identity stage
    cmsStage* identityStage = cmsStageAllocIdentity(ctx, 3); // 3 channels
    if (!identityStage) {
        cmsCloseProfile(grayProfile);
        cmsFreeToneCurve(transferFunc);
        cmsCloseProfile(nullProfile);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 7. Get input channels of stage
    cmsUInt32Number inputChannels = cmsStageInputChannels(identityStage);
    (void)inputChannels; // Use variable
    
    // 8. Create MLU and get translation codes
    cmsMLU* mlu = cmsMLUalloc(ctx, 5); // Allocate for 5 translations
    if (mlu) {
        // Set some translations from input data
        if (size >= 20) {
            char langCode[3] = {'e', 'n', '\0'};
            char countryCode[3] = {'U', 'S', '\0'};
            char asciiStr[11] = {0};
            size_t strLen = size > 10 ? 10 : size;
            memcpy(asciiStr, data, strLen);
            asciiStr[strLen] = '\0';
            
            cmsMLUsetASCII(mlu, langCode, countryCode, asciiStr);
        }
        
        // Get translation codes
        char obtainedLang[3], obtainedCountry[3];
        cmsMLUtranslationsCodes(mlu, 0, obtainedLang, obtainedCountry);
        
        cmsMLUfree(mlu);
    }
    
    // 9. Create IT8 handle from input file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        cmsHANDLE hIT8 = cmsIT8LoadFromFile(ctx, "input_file");
        if (hIT8) {
            // Try to get some data from IT8
            // Use row=1, col=1 as default
            const char* it8Data = cmsIT8GetDataRowCol(hIT8, 1, 1);
            (void)it8Data; // Use variable
            
            cmsIT8Free(hIT8);
        }
        assert_file_closed(&in_file);;
    }
    
    // Clean up
    cmsStageFree(identityStage);
    cmsCloseProfile(grayProfile);
    cmsFreeToneCurve(transferFunc);
    cmsCloseProfile(nullProfile);
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}