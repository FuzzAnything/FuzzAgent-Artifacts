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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 128, nr_unique_branch: 7
//<Quality> {"density":32,"unique_branches":{"cmsIsCLUT":[[835,9,835,55,0,0,4,0]],"cmsDetectRGBProfileGamma":[[618,9,618,52,0,0,4,0]],"NormalizeXYZ":[[1093,12,1093,26,0,0,4,0],[1094,12,1094,26,0,0,4,0],[1094,12,1094,26,0,0,4,1],[1095,12,1095,26,0,0,4,0],[1095,12,1095,26,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsBuildTabulatedToneCurveFloat","cmsDeleteContext","cmsIsToneCurveMonotonic","cmsGetToneCurveParametricType","cmsCIECAM02Init","cmsCreateLab4ProfileTHR","cmsFreeToneCurve","cmsCIECAM02Done","cmsDeleteContext","cmsCreate_sRGBProfileTHR","cmsCloseProfile","cmsFreeToneCurve","cmsCIECAM02Done","cmsDeleteContext","cmsCreateTransform","cmsTransform2DeviceLink","cmsFormatterForPCSOfProfile","cmsIsCLUT","cmsDetectRGBProfileGamma","cmsCloseProfile","cmsIT8Alloc","cmsIT8GetPatchByName","cmsIT8GetDataRowCol","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsCIECAM02Done","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsBuildTabulatedToneCurveFloat","cmsIsToneCurveMonotonic","cmsGetToneCurveParametricType","cmsCIECAM02Init","cmsCreateLab4ProfileTHR","cmsCreate_sRGBProfileTHR","cmsCreateTransform","cmsTransform2DeviceLink","cmsFormatterForPCSOfProfile","cmsIsCLUT","cmsDetectRGBProfileGamma","cmsCloseProfile","cmsIT8Alloc","cmsIT8GetPatchByName","cmsIT8GetDataRowCol","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsCIECAM02Done","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Minimum input size
    
    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // Prepare tone curve data from input
    const size_t curve_entries = size / sizeof(float) / 4;
    if (curve_entries < 2) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Use first part of input for tone curve values
    const float* curve_values = reinterpret_cast<const float*>(data);
    cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurveFloat(ctx, 
        static_cast<cmsUInt32Number>(curve_entries), curve_values);
    if (!toneCurve) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Check tone curve properties
    cmsBool isMonotonic = cmsIsToneCurveMonotonic(toneCurve);
    cmsInt32Number paramType = cmsGetToneCurveParametricType(toneCurve);
    
    // Create viewing conditions for CIECAM02
    cmsViewingConditions vc;
    memset(&vc, 0, sizeof(vc));
    vc.whitePoint.X = vc.whitePoint.Y = vc.whitePoint.Z = 1.0;
    vc.Yb = 20.0;
    vc.La = 64.0;
    vc.surround = 1;
    vc.D_value = 1.0;
    
    // Initialize CIECAM02
    cmsHANDLE ciecam = cmsCIECAM02Init(ctx, &vc);
    
    // Create white point from remaining input
    cmsCIExyY whitePoint;
    if (size >= sizeof(cmsCIExyY) + curve_entries * sizeof(float)) {
        const uint8_t* wp_data = data + curve_entries * sizeof(float);
        memcpy(&whitePoint, wp_data, sizeof(cmsCIExyY));
    } else {
        whitePoint.x = 0.3127;
        whitePoint.y = 0.3290;
        whitePoint.Y = 1.0;
    }
    
    // Create Lab profile
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
    if (!labProfile) {
        cmsFreeToneCurve(toneCurve);
        if (ciecam) {
            cmsCIECAM02Done(ciecam);
        }
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create sRGB profile
    cmsHPROFILE srgbProfile = cmsCreate_sRGBProfileTHR(ctx);
    if (!srgbProfile) {
        cmsCloseProfile(labProfile);
        cmsFreeToneCurve(toneCurve);
        if (ciecam) {
            cmsCIECAM02Done(ciecam);
        }
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create transform
    cmsHTRANSFORM transform = cmsCreateTransform(
        labProfile,
        TYPE_Lab_DBL,
        srgbProfile,
        TYPE_RGB_DBL,
        INTENT_PERCEPTUAL,
        0
    );
    
    if (transform) {
        // Convert transform to device link
        cmsHPROFILE devLink = cmsTransform2DeviceLink(transform, 4.2, 0);
        
        if (devLink) {
            // Check formatter
            cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(
                devLink, 8, FALSE);
            
            // Check if CLUT exists
            cmsBool hasCLUT = cmsIsCLUT(devLink, INTENT_PERCEPTUAL, 0);
            
            // Detect gamma
            cmsFloat64Number gamma = cmsDetectRGBProfileGamma(devLink, 0.1);
            
            cmsCloseProfile(devLink);
        }
        
        // Allocate IT8 handle
        cmsHANDLE it8 = cmsIT8Alloc(ctx);
        if (it8) {
            // Try to get patch by name (using input bytes as string)
            const char* patchName = "test_patch";
            int patchIndex = cmsIT8GetPatchByName(it8, patchName);
            
            // Get data from row/col (dummy values)
            const char* dataRC = cmsIT8GetDataRowCol(it8, 0, 0);
            
            cmsIT8Free(it8);
        }
        
        cmsDeleteTransform(transform);
    }
    
    // Cleanup
    cmsCloseProfile(labProfile);
    cmsCloseProfile(srgbProfile);
    cmsFreeToneCurve(toneCurve);
    
    if (ciecam) {
        cmsCIECAM02Done(ciecam);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}