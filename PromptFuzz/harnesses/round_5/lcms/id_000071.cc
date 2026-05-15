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
//<ID> 459
//<Prompt> []
/*<Combination>: [
*/
//<score> 80, nr_unique_branch: 4
//<Quality> {"density":16,"unique_branches":{"bchswSampler":[[845,9,845,25,0,0,4,1]],"cmsIT8EnumDataFormat":[[2666,9,2666,20,0,0,4,0]],"cmsCreateBCHSWabstractProfileTHR":[[882,9,882,28,0,0,4,0]],"Clamp_L_doubleV4":[[271,9,271,14,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDupContext","cmsGetSupportedIntentsTHR","cmsCreateBCHSWabstractProfileTHR","cmsIsTag","cmsIsTag","cmsLinkTag","cmsCreate_sRGBProfileTHR","cmsCreateTransformTHR","cmsTransform2DeviceLink","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8DefineDblFormat","cmsIT8SetComment","cmsIT8SetComment","cmsIT8Free","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsDupContext","cmsGetSupportedIntentsTHR","cmsCreateBCHSWabstractProfileTHR","cmsIsTag","cmsIsTag","cmsLinkTag","cmsCreate_sRGBProfileTHR","cmsCreateTransformTHR","cmsTransform2DeviceLink","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8DefineDblFormat","cmsIT8SetComment","cmsIT8Free","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for processing

    // 1. Create and duplicate context
    cmsContext context = cmsCreateContext(NULL, NULL);
    cmsContext dupContext = cmsDupContext(context, NULL);

    // 2. Get supported intents
    cmsUInt32Number intents[10];
    char* descriptions[10];
    cmsUInt32Number numIntents = cmsGetSupportedIntentsTHR(dupContext, 10, intents, descriptions);

    // 3. Create BCHSW abstract profile using parameters from input data
    // Extract parameters from input data
    cmsUInt32Number nLUTPoints = *reinterpret_cast<const cmsUInt32Number*>(data) % 100 + 2;
    cmsFloat64Number Bright = static_cast<cmsFloat64Number>(data[4] % 200 - 100) / 100.0;
    cmsFloat64Number Contrast = static_cast<cmsFloat64Number>(data[5] % 200 - 100) / 100.0;
    cmsFloat64Number Hue = static_cast<cmsFloat64Number>(data[6] % 200 - 100) / 100.0;
    cmsFloat64Number Saturation = static_cast<cmsFloat64Number>(data[7] % 200 - 100) / 100.0;
    cmsUInt32Number TempSrc = *reinterpret_cast<const cmsUInt32Number*>(data + 8) % 10000;
    cmsUInt32Number TempDest = *reinterpret_cast<const cmsUInt32Number*>(data + 12) % 10000;

    cmsHPROFILE bchswProfile = cmsCreateBCHSWabstractProfileTHR(
        context, nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);

    if (bchswProfile) {
        // 4. Link a tag (if profile has appropriate tags)
        // Try linking mediaWhitePointTag to mediaBlackPointTag as an example
        if (cmsIsTag(bchswProfile, cmsSigMediaWhitePointTag) && 
            cmsIsTag(bchswProfile, cmsSigMediaBlackPointTag)) {
            cmsLinkTag(bchswProfile, cmsSigMediaWhitePointTag, cmsSigMediaBlackPointTag);
        }

        // 5. Create transform and device link
        // Create sRGB profile for output
        cmsHPROFILE srgbProfile = cmsCreate_sRGBProfileTHR(context);
        
        if (srgbProfile) {
            cmsHTRANSFORM transform = cmsCreateTransformTHR(
                context, bchswProfile, TYPE_RGB_8, srgbProfile, TYPE_RGB_8, 
                INTENT_PERCEPTUAL, 0);
            
            if (transform) {
                // Create device link from transform
                cmsFloat64Number version = 4.2;
                cmsUInt32Number flags = 0;
                cmsHPROFILE deviceLink = cmsTransform2DeviceLink(transform, version, flags);
                
                if (deviceLink) {
                    cmsCloseProfile(deviceLink);
                }
                cmsDeleteTransform(transform);
            }
            cmsCloseProfile(srgbProfile);
        }

        // 6. IT8 operations
        // Create IT8 handle from input data (skip first 16 bytes for parameters)
        size_t it8Offset = 16;
        size_t it8Size = size - it8Offset;
        if (it8Size > 0) {
            cmsHANDLE it8Handle = cmsIT8LoadFromMem(context, data + it8Offset, it8Size);
            
            if (it8Handle) {
                // Enumerate data formats
                char** sampleNames = NULL;
                int numFormats = cmsIT8EnumDataFormat(it8Handle, &sampleNames);
                
                // Define double format
                cmsIT8DefineDblFormat(it8Handle, "%.6f");
                
                // Set comment using data from input
                const char* comment = "Fuzzer-generated IT8 data";
                if (it8Size > 50) {
                    // Use part of input data as comment (ensure null termination)
                    char tempComment[51];
                    size_t commentLen = it8Size < 50 ? it8Size : 50;
                    memcpy(tempComment, data + it8Offset, commentLen);
                    tempComment[commentLen] = '\0';
                    cmsIT8SetComment(it8Handle, tempComment);
                } else {
                    cmsIT8SetComment(it8Handle, comment);
                }
                
                cmsIT8Free(it8Handle);
            }
        }

        cmsCloseProfile(bchswProfile);
    }

    // Cleanup
    cmsDeleteContext(dupContext);
    cmsDeleteContext(context);

    return 0;
}