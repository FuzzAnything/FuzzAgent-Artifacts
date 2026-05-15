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
//<ID> 353
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 1
//<Quality> {"density":22,"unique_branches":{"LinLerp1Dfloat":[[243,27,243,44,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsCIECAM02Init","cmsCreateLab4ProfileTHR","cmsDeleteContext","cmsCreate_sRGBProfileTHR","cmsCloseProfile","cmsDeleteContext","cmsCreateTransform","cmsTransform2DeviceLink","cmsFormatterForPCSOfProfile","cmsCloseProfile","cmsBuildTabulatedToneCurveFloat","cmsIsToneCurveMonotonic","cmsGetToneCurveParametricType","cmsDetectRGBProfileGamma","cmsIT8Alloc","cmsIT8GetPatchByName","cmsIT8SetPropertyStr","cmsIT8GetDataRowCol","cmsIsCLUT","cmsIT8Free","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCIECAM02Done","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCIECAM02Init","cmsCreateLab4ProfileTHR","cmsCreate_sRGBProfileTHR","cmsCreateTransform","cmsTransform2DeviceLink","cmsFormatterForPCSOfProfile","cmsCloseProfile","cmsBuildTabulatedToneCurveFloat","cmsIsToneCurveMonotonic","cmsGetToneCurveParametricType","cmsDetectRGBProfileGamma","cmsIT8Alloc","cmsIT8GetPatchByName","cmsIT8SetPropertyStr","cmsIT8GetDataRowCol","cmsIsCLUT","cmsIT8Free","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCIECAM02Done","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Create CIECAM02 viewing conditions
    cmsViewingConditions vc;
    memset(&vc, 0, sizeof(vc));
    vc.whitePoint.X = 0.95047;
    vc.whitePoint.Y = 1.00000;
    vc.whitePoint.Z = 1.08883;
    vc.Yb = 20.0;
    vc.La = 64.0;
    vc.surround = 1; // Average surround
    vc.D_value = 1.0;
    
    cmsHANDLE ciecam = cmsCIECAM02Init(context, &vc);
    
    // 3. Create Lab4 profile with D50 white point
    cmsCIExyY d50White;
    d50White.x = 0.3457;
    d50White.y = 0.3585;
    d50White.Y = 1.0;
    
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(context, &d50White);
    if (!labProfile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 4. Create sRGB profile and transform
    cmsHPROFILE srgbProfile = cmsCreate_sRGBProfileTHR(context);
    if (!srgbProfile) {
        cmsCloseProfile(labProfile);
        cmsDeleteContext(context);
        return 0;
    }
    
    cmsHTRANSFORM transform = cmsCreateTransform(
        labProfile, TYPE_Lab_DBL,
        srgbProfile, TYPE_RGB_DBL,
        INTENT_PERCEPTUAL, 0
    );
    
    // 5. Convert transform to device link
    if (transform) {
        cmsHPROFILE deviceLink = cmsTransform2DeviceLink(transform, 4.0, 0);
        if (deviceLink) {
            // 6. Get formatter for device link profile
            cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(deviceLink, 8, FALSE);
            (void)formatter; // Use variable to avoid unused warning
            cmsCloseProfile(deviceLink);
        }
    }
    
    // 7. Build tabulated tone curve from input data
    const size_t maxEntries = 256;
    size_t curveEntries = size / sizeof(cmsFloat32Number);
    if (curveEntries > maxEntries) curveEntries = maxEntries;
    
    cmsFloat32Number* curveValues = nullptr;
    if (curveEntries > 0 && size >= curveEntries * sizeof(cmsFloat32Number)) {
        curveValues = (cmsFloat32Number*)data;
    } else {
        // Create dummy curve if insufficient data
        static cmsFloat32Number dummyCurve[2] = {0.0f, 1.0f};
        curveValues = dummyCurve;
        curveEntries = 2;
    }
    
    cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurveFloat(context, curveEntries, curveValues);
    
    // 8. Check tone curve properties
    if (toneCurve) {
        cmsBool isMonotonic = cmsIsToneCurveMonotonic(toneCurve);
        cmsInt32Number paramType = cmsGetToneCurveParametricType(toneCurve);
        (void)isMonotonic;
        (void)paramType;
    }
    
    // 9. Detect RGB profile gamma
    if (srgbProfile) {
        cmsFloat64Number gamma = cmsDetectRGBProfileGamma(srgbProfile, 0.01);
        (void)gamma;
    }
    
    // 10. Create IT8 handle and process data
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (it8) {
        // Use a portion of input data as patch name
        const char* patchName = "TestPatch";
        if (size > 0) {
            // Use first byte as patch number if possible
            int patchNum = data[0] % 100;
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "Patch%d", patchNum);
            patchName = buffer;
        }
        
        int patchIndex = cmsIT8GetPatchByName(it8, patchName);
        
        // Set some IT8 properties using input data
        if (size > 10) {
            char propValue[256];
            size_t copySize = size < 255 ? size : 255;
            memcpy(propValue, data, copySize);
            propValue[copySize] = '\0';
            
            cmsIT8SetPropertyStr(it8, "SAMPLE_ID", propValue);
            
            // Try to get data from row/col
            const char* rowColData = cmsIT8GetDataRowCol(it8, 0, 0);
            (void)rowColData;
        }
    }
    
    // 11. Check if profile has CLUT
    if (labProfile) {
        cmsBool hasCLUT = cmsIsCLUT(labProfile, INTENT_PERCEPTUAL, LCMS_USED_AS_INPUT);
        (void)hasCLUT;
    }
    
    // 12. Cleanup
    if (it8) cmsIT8Free(it8);
    if (toneCurve) cmsFreeToneCurve(toneCurve);
    if (transform) cmsDeleteTransform(transform);
    if (srgbProfile) cmsCloseProfile(srgbProfile);
    if (labProfile) cmsCloseProfile(labProfile);
    if (ciecam) cmsCIECAM02Done(ciecam);
    cmsDeleteContext(context);
    
    return 0;
}