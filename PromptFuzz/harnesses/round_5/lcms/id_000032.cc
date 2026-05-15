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
//<ID> 188
//<Prompt> []
/*<Combination>: [
*/
//<score> 15.333333, nr_unique_branch: 2
//<Quality> {"density":23,"unique_branches":{"DefaultEvalParametricFn":[[436,5,436,11,0,0,4,0],[438,13,438,51,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDupContext","cmsBuildSegmentedToneCurve","cmsDeleteContext","cmsDeleteContext","cmsCreateGrayProfile","cmsCreateNULLProfile","cmsGetHeaderCreationDateTime","cmsGetColorSpace","cmsGetColorSpace","cmsFormatterForPCSOfProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUtranslationsCodes","cmsMLUfree","cmsStageAllocIdentity","cmsStageInputChannels","cmsStageFree","cmsIT8LoadFromFile","cmsIT8GetDataDbl","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsDeleteContext","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsDupContext","cmsBuildSegmentedToneCurve","cmsCreateGrayProfile","cmsCreateNULLProfile","cmsGetHeaderCreationDateTime","cmsGetColorSpace","cmsGetColorSpace","cmsFormatterForPCSOfProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUtranslationsCodes","cmsMLUfree","cmsStageAllocIdentity","cmsStageInputChannels","cmsStageFree","cmsIT8LoadFromFile","cmsIT8GetDataDbl","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsDeleteContext","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Minimal size for meaningful operations
    
    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Create duplicate context
    cmsContext dupContext = cmsDupContext(context, nullptr);
    
    // Create segmented tone curve
    cmsCurveSegment segments[2];
    memset(segments, 0, sizeof(segments));
    
    // Initialize first segment (linear)
    segments[0].x0 = 0.0;
    segments[0].x1 = 0.5;
    segments[0].Type = 1;  // Linear
    segments[0].Params[0] = 1.0;  // Gamma = 1.0
    
    // Initialize second segment (sRGB gamma)
    segments[1].x0 = 0.5;
    segments[1].x1 = 1.0;
    segments[1].Type = 3;  // sRGB gamma
    segments[1].Params[0] = 2.4;
    
    cmsToneCurve* toneCurve = cmsBuildSegmentedToneCurve(context, 2, segments);
    if (!toneCurve) {
        cmsDeleteContext(dupContext);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Create white point
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    
    // Create gray profile
    cmsHPROFILE grayProfile = cmsCreateGrayProfile(&whitePoint, toneCurve);
    
    // Create NULL profile
    cmsHPROFILE nullProfile = cmsCreateNULLProfile();
    
    // Get creation date/time from gray profile
    struct tm creationDate;
    if (grayProfile) {
        cmsGetHeaderCreationDateTime(grayProfile, &creationDate);
    }
    
    // Get color spaces
    cmsColorSpaceSignature graySpace = cmsGetColorSpace(grayProfile);
    cmsColorSpaceSignature nullSpace = cmsGetColorSpace(nullProfile);
    
    // Test formatter for PCS
    cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(grayProfile, 4, false);
    
    // Create and use MLU
    cmsMLU* mlu = cmsMLUalloc(context, 10);
    if (mlu) {
        // Use some data from input for MLU strings
        char langCode[3] = {'e', 'n', '\0'};
        char countryCode[3] = {'U', 'S', '\0'};
        
        // Create ASCII string from input data (limited to safe size)
        char asciiStr[256];
        size_t strLen = size < 255 ? size : 255;
        memcpy(asciiStr, data, strLen);
        asciiStr[strLen] = '\0';
        
        cmsMLUsetASCII(mlu, langCode, countryCode, asciiStr);
        
        // Get translation codes
        char outLang[3], outCountry[3];
        cmsMLUtranslationsCodes(mlu, 0, outLang, outCountry);
        
        cmsMLUfree(mlu);
    }
    
    // Create identity stage
    cmsStage* identityStage = cmsStageAllocIdentity(context, 3);
    if (identityStage) {
        cmsUInt32Number inputChannels = cmsStageInputChannels(identityStage);
        cmsStageFree(identityStage);
    }
    
    // Write input data to temporary file for IT8 loading
    FILE* tempFile = fmemopen((void*)data, size, "rb");
    if (tempFile) {
        // Create a temporary filename for IT8
        const char* tempFilename = "temp_it8_file";
        FILE* writeFile = fopen(tempFilename, "wb");
        if (writeFile) {
            fwrite(data, 1, size, writeFile);
            assert_file_closed(&writeFile);;
            
            // Load IT8 from file
            cmsHANDLE it8Handle = cmsIT8LoadFromFile(context, tempFilename);
            if (it8Handle) {
                // Try to get data from IT8
                const char* patchName = "Patch1";
                const char* sampleName = "Sample1";
                cmsFloat64Number it8Value = cmsIT8GetDataDbl(it8Handle, patchName, sampleName);
                
                cmsIT8Free(it8Handle);
            }
            
            // Clean up temporary file
            remove(tempFilename);
        }
        assert_file_closed(&tempFile);;
    }
    
    // Clean up
    if (grayProfile) cmsCloseProfile(grayProfile);
    if (nullProfile) cmsCloseProfile(nullProfile);
    cmsFreeToneCurve(toneCurve);
    cmsDeleteContext(dupContext);
    cmsDeleteContext(context);
    
    assert_file_closed(&tempFile);
	return 0;
}