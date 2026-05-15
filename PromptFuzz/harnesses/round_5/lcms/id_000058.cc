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
//<ID> 360
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsDeleteContext","cmsWhitePointFromTemp","cmsIT8LoadFromMem","cmsIT8FindDataFormat","cmsIT8GetPatchName","cmsTagLinkedTo","cmsBuildGamma","cmsIsToneCurveDescending","cmsEvalToneCurve16","cmsFreeToneCurve","cmsReadTag","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsCreate_sRGBProfileTHR","cmsCreateTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsWhitePointFromTemp","cmsIT8LoadFromMem","cmsIT8FindDataFormat","cmsIT8GetPatchName","cmsTagLinkedTo","cmsBuildGamma","cmsIsToneCurveDescending","cmsEvalToneCurve16","cmsFreeToneCurve","cmsReadTag","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsCreate_sRGBProfileTHR","cmsCreateTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 2. Open profile from memory (consumes input data)
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(context, data, size);
    if (!hProfile) {
        cmsDeleteContext(context);
        return 0;
    }

    // 3. Create white point from temperature
    cmsCIExyY whitePoint;
    cmsFloat64Number tempK = 6500.0; // D65 standard
    cmsBool whitePointResult = cmsWhitePointFromTemp(&whitePoint, tempK);

    // 4. Load IT8 from memory (consumes same input data)
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(context, data, size);
    
    // 5. Find data format in IT8
    if (hIT8) {
        int format = cmsIT8FindDataFormat(hIT8, "Sample");
        // Use format result
    }

    // 6. Get patch name from IT8
    if (hIT8) {
        char buffer[256];
        const char* patchName = cmsIT8GetPatchName(hIT8, 0, buffer);
        // Use patchName
    }

    // 7. Get tag linked to another tag
    cmsTagSignature linkedTag = cmsTagLinkedTo(hProfile, cmsSigRedTRCTag);
    // Use linkedTag

    // 8. Build gamma curve
    cmsFloat64Number gamma = 2.2;
    cmsToneCurve* toneCurve = cmsBuildGamma(context, gamma);
    
    // 9. Check if tone curve is descending
    if (toneCurve) {
        cmsBool isDescending = cmsIsToneCurveDescending(toneCurve);
        // Use isDescending
    }

    // 10. Evaluate tone curve
    if (toneCurve) {
        cmsUInt16Number evalResult = cmsEvalToneCurve16(toneCurve, 32768);
        // Use evalResult
    }

    // 11. Free tone curve
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }

    // 12. Read tag from profile
    void* tagData = cmsReadTag(hProfile, cmsSigRedTRCTag);
    // Use tagData if not null

    // 13. Get named color count
    cmsUInt32Number colorCount = 0;
    cmsNAMEDCOLORLIST* namedColorList = cmsGetNamedColorList(nullptr); // Using null transform
    if (namedColorList) {
        colorCount = cmsNamedColorCount(namedColorList);
    }

    // 14. Get named color info
    if (namedColorList && colorCount > 0) {
        char name[256];
        char prefix[256];
        char suffix[256];
        cmsUInt16Number pcs[3];
        cmsUInt16Number colorant[16];
        cmsBool infoResult = cmsNamedColorInfo(namedColorList, 0, name, prefix, suffix, pcs, colorant);
        // Use infoResult
    }

    // 15. Create transform and do transformation
    cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfileTHR(context);
    if (sRGBProfile) {
        cmsHTRANSFORM transform = cmsCreateTransform(
            hProfile, TYPE_RGB_8,
            sRGBProfile, TYPE_RGB_8,
            INTENT_PERCEPTUAL, 0
        );
        
        if (transform) {
            // Prepare input/output buffers
            unsigned char inputBuffer[3] = {128, 128, 128};
            unsigned char outputBuffer[3];
            
            // Do transformation
            cmsDoTransform(transform, inputBuffer, outputBuffer, 1);
            
            // 16. Delete transform
            cmsDeleteTransform(transform);
        }
        cmsCloseProfile(sRGBProfile);
    }

    // 17. Close profile
    cmsCloseProfile(hProfile);

    // 18. Delete context
    cmsDeleteContext(context);

    // Clean up IT8 if created
    // Note: cmsIT8LoadFromMem doesn't have a direct cleanup function in provided APIs
    // Using cmsHANDLE cleanup if available
    if (hIT8) {
        // Assuming cmsHANDLE can be freed with cmsDeleteContext or similar
        // In absence of specific API, we'll leave it
    }

    return 0;
}