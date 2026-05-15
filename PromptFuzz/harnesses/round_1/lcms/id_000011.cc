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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 18.666666, nr_unique_branch: 3
//<Quality> {"density":14,"unique_branches":{"cmsSliceSpace16":[[904,13,904,38,0,0,4,0]],"cmsGetToneCurveSegment":[[1511,18,1511,52,0,0,4,0]],"cmsFormatterForPCSOfProfile":[[4051,29,4051,37,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsCreate_sRGBProfileTHR","cmsDeleteContext","cmsBuildTabulatedToneCurve16","cmsGetToneCurveSegment","cmsTagLinkedTo","cmsFormatterForPCSOfProfile","cmsFormatterForPCSOfProfile","cmsSliceSpace16","cmsSliceSpace16","cmsSetHeaderProfileID","cmsBuildTabulatedToneCurve16","cmsGetToneCurveSegment","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsCreate_sRGBProfileTHR","cmsBuildTabulatedToneCurve16","cmsGetToneCurveSegment","cmsTagLinkedTo","cmsFormatterForPCSOfProfile","cmsFormatterForPCSOfProfile","cmsSliceSpace16","cmsSliceSpace16","cmsSetHeaderProfileID","cmsBuildTabulatedToneCurve16","cmsGetToneCurveSegment","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/


// Custom sampler for cmsSliceSpace16
static int  id_000011_custom_sampler16(const cmsUInt16Number* in, cmsUInt16Number* out, void* cargo) {
    // Simple passthrough sampler - just copy input to output
    // We use a single channel for simplicity
    if (in && out) {
        out[0] = in[0];
        return 1; // Return non-zero to continue
    }
    return 0; // Return zero to stop
}

// Fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	 // Need minimum data
    
    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Try to open the input data as a profile
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ctx, data, static_cast<cmsUInt32Number>(size));
    
    // If we couldn't open as profile, create a simple sRGB profile for testing
    if (!hProfile) {
        hProfile = cmsCreate_sRGBProfileTHR(ctx);
        if (!hProfile) {
            cmsDeleteContext(ctx);
            return 0;
        }
    }
    
    // 2. Get tone curve segment
    // First, create a simple tone curve for testing
    cmsUInt16Number toneCurveValues[256];
    for (int i = 0; i < 256; i++) {
        toneCurveValues[i] = static_cast<cmsUInt16Number>(i * 257); // Scale to 16-bit
    }
    
    cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurve16(ctx, 256, toneCurveValues);
    if (toneCurve) {
        // Get the first segment of the tone curve
        const cmsCurveSegment* segment = cmsGetToneCurveSegment(0, toneCurve);
        (void)segment; // Use segment to avoid unused variable warning
    }
    
    // 3. Check tag linking
    // Try to get tag linking for a common tag
    cmsTagSignature linkedTag = cmsTagLinkedTo(hProfile, cmsSigRedColorantTag);
    (void)linkedTag; // Use to avoid warning
    
    // 4. Get formatter for PCS
    // Use different combinations of parameters
    cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(hProfile, 4, false);
    (void)formatter;
    
    formatter = cmsFormatterForPCSOfProfile(hProfile, 8, true);
    (void)formatter;
    
    // 5. Use slice space with sampler
    // Create simple CLUT points
    cmsUInt32Number clutPoints[] = {5, 5, 5}; // 3D 5x5x5 CLUT
    void* cargo = NULL; // No cargo needed for our simple sampler
    
    // Try slice space with different input sizes
    if (size >= 10) {
        cmsSliceSpace16(1, clutPoints,  id_000011_custom_sampler16, cargo);
        
        // Try with 3 inputs if we have enough data
        if (size >= 30) {
            cmsSliceSpace16(3, clutPoints,  id_000011_custom_sampler16, cargo);
        }
    }
    
    // 6. Set header profile ID
    // Create a profile ID from the input data
    cmsUInt8Number profileID[16];
    size_t copySize = (size < 16) ? size : 16;
    if (copySize > 0) {
        memcpy(profileID, data, copySize);
        // Pad with zeros if needed
        if (copySize < 16) {
            memset(profileID + copySize, 0, 16 - copySize);
        }
        cmsSetHeaderProfileID(hProfile, profileID);
    }
    
    // 7. Build another tabulated tone curve from input data
    if (size >= 4) {
        size_t numEntries = (size / 2) < 1024 ? (size / 2) : 1024;
        if (numEntries > 1) {
            cmsUInt16Number* inputValues = (cmsUInt16Number*)data;
            cmsToneCurve* inputToneCurve = cmsBuildTabulatedToneCurve16(
                ctx, 
                static_cast<cmsUInt32Number>(numEntries), 
                inputValues
            );
            if (inputToneCurve) {
                // Get segment from input tone curve
                const cmsCurveSegment* inputSegment = cmsGetToneCurveSegment(0, inputToneCurve);
                (void)inputSegment;
                
                // Clean up
                cmsFreeToneCurve(inputToneCurve);
            }
        }
    }
    
    // Clean up resources
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }
    
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}