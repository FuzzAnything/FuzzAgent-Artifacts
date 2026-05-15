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
//<ID> 350
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["cmsCreateContext","cmsCIECAM02Init","cmsCreateLab4ProfileTHR","cmsCreate_sRGBProfileTHR","cmsCreateTransform","cmsTransform2DeviceLink","cmsFormatterForPCSOfProfile","cmsFormatterForPCSOfProfile","cmsIsCLUT","cmsIsCLUT","cmsBuildTabulatedToneCurveFloat","cmsIsToneCurveMonotonic","cmsGetToneCurveParametricType","cmsFreeToneCurve","cmsDetectRGBProfileGamma","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetData","cmsIT8GetDataRowCol","cmsIT8GetPatchByName","cmsIT8Free","cmsCIECAM02Done","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCIECAM02Init","cmsCreateLab4ProfileTHR","cmsCreate_sRGBProfileTHR","cmsCreateTransform","cmsTransform2DeviceLink","cmsFormatterForPCSOfProfile","cmsFormatterForPCSOfProfile","cmsIsCLUT","cmsIsCLUT","cmsBuildTabulatedToneCurveFloat","cmsIsToneCurveMonotonic","cmsGetToneCurveParametricType","cmsFreeToneCurve","cmsDetectRGBProfileGamma","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetData","cmsIT8GetDataRowCol","cmsIT8GetPatchByName","cmsIT8Free","cmsCIECAM02Done","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*To achieve the described event using the provided lcms APIs, we'll create a fuzzer that:

1. Creates a context and profiles
2. Builds a transform between profiles
3. Analyzes tone curves and profile properties
4. Processes IT8 color data
5. Handles errors gracefully with input validation

*/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Minimum size check for basic operations
    if (size < 100) {
	return 0;
	}
	
    
    cmsContext ctx = nullptr;
    cmsHPROFILE labProfile = nullptr, srgbProfile = nullptr;
    cmsHTRANSFORM transform = nullptr;
    cmsHPROFILE devicelink = nullptr;
    cmsHANDLE it8 = nullptr;
    cmsHANDLE ciecam = nullptr;
    cmsToneCurve* toneCurve = nullptr;
    
    // 1. Create context
    ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // 2. Create CIECAM02 viewing conditions (simplified)
    cmsViewingConditions vc;
    memset(&vc, 0, sizeof(vc));
    vc.whitePoint.X = 0.95047;
    vc.whitePoint.Y = 1.00000;
    vc.whitePoint.Z = 1.08883;
    vc.Yb = 20.0;
    vc.La = 64.0;
    vc.surround = 1;
    vc.D_value = 1.0;
    
    ciecam = cmsCIECAM02Init(ctx, &vc);
    
    // 3. Create Lab4 profile with D50 white point
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    
    labProfile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
    
    // 4. Create sRGB profile for transformation
    srgbProfile = cmsCreate_sRGBProfileTHR(ctx);
    
    if (labProfile && srgbProfile) {
        // 5. Create transform between profiles
        transform = cmsCreateTransform(labProfile, 
                                     TYPE_Lab_DBL,  // Input format
                                     srgbProfile, 
                                     TYPE_RGB_DBL,  // Output format
                                     INTENT_PERCEPTUAL, 
                                     0);
        
        if (transform) {
            // 6. Create device link from transform
            devicelink = cmsTransform2DeviceLink(transform, 4.0, 0);
            
            // 7. Get formatter for PCS
            cmsFormatterForPCSOfProfile(labProfile, 16, FALSE);
            cmsFormatterForPCSOfProfile(srgbProfile, 16, FALSE);
            
            // 8. Check if profile has CLUT
            cmsIsCLUT(labProfile, INTENT_PERCEPTUAL, 0);
            cmsIsCLUT(srgbProfile, INTENT_PERCEPTUAL, 0);
        }
    }
    
    // 9. Create tone curve from input data
    // We need at least 256 floats (1024 bytes) for a reasonable tone curve
    if (size >= 1024) {
        size_t numEntries = size / sizeof(cmsFloat32Number);
        if (numEntries > 256) numEntries = 256;  // Limit entries
        
        // Use the input data as float values for tone curve
        const cmsFloat32Number* values = reinterpret_cast<const cmsFloat32Number*>(data);
        
        toneCurve = cmsBuildTabulatedToneCurveFloat(ctx, 
                                                   static_cast<cmsUInt32Number>(numEntries), 
                                                   values);
        
        if (toneCurve) {
            // 10. Analyze tone curve
            cmsIsToneCurveMonotonic(toneCurve);
            cmsGetToneCurveParametricType(toneCurve);
            
            // 11. Clean up tone curve
            cmsFreeToneCurve(toneCurve);
            toneCurve = nullptr;
        }
    }
    
    // 12. Detect gamma of RGB profile
    if (srgbProfile) {
        cmsDetectRGBProfileGamma(srgbProfile, 0.01);
    }
    
    // 13. IT8 operations
    it8 = cmsIT8Alloc(ctx);
    if (it8) {
        // Add some sample data from input
        size_t it8DataSize = size > 1000 ? 1000 : size;
        
        // Create a null-terminated string from input data for IT8
        char* it8Buffer = static_cast<char*>(malloc(it8DataSize + 1));
        if (it8Buffer) {
            memcpy(it8Buffer, data, it8DataSize);
            it8Buffer[it8DataSize] = '\0';
            
            // Set IT8 properties
            cmsIT8SetPropertyStr(it8, "DESCRIPTOR", "Test IT8 Data");
            
            // Set some data values
            if (it8DataSize > 100) {
                char patchName[32];
                snprintf(patchName, sizeof(patchName), "Patch_%d", 1);
                
                // Extract sample value from input data
                char sampleValue[32];
                snprintf(sampleValue, sizeof(sampleValue), "%.6f", 
                        static_cast<double>(data[0]) / 255.0);
                
                cmsIT8SetData(it8, patchName, "R", sampleValue);
            }
            
            // Try to get data by row and column
            const char* rowColData = cmsIT8GetDataRowCol(it8, 0, 0);
            
            // Try to get patch by name
            int patchIndex = cmsIT8GetPatchByName(it8, "Patch_1");
            
            free(it8Buffer);
        }
        
        // Free IT8 handle
        cmsIT8Free(it8);
    }
    
    // 14. Clean up CIECAM02 if created
    if (ciecam) {
        cmsCIECAM02Done(ciecam);
    }
    
    // 15. Clean up device link if created
    if (devicelink) {
        cmsCloseProfile(devicelink);
    }
    
    // 16. Clean up transform if created
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    // 17. Clean up profiles
    if (labProfile) {
        cmsCloseProfile(labProfile);
    }
    if (srgbProfile) {
        cmsCloseProfile(srgbProfile);
    }
    
    // 18. Clean up context
    if (ctx) {
        cmsDeleteContext(ctx);
    }
    
    return 0;
}