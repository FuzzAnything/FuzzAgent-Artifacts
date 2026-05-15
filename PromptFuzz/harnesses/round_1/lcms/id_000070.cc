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
//<ID> 500
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 7
//<Quality> {"density":9,"unique_branches":{"cmsSliceSpaceFloat":[[918,9,918,34,0,0,4,1],[921,9,921,26,0,0,4,1],[923,17,923,39,0,0,4,0],[923,17,923,39,0,0,4,1],[926,35,926,40,0,0,4,0],[926,35,926,40,0,0,4,1],[935,13,935,38,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsOpenProfileFromStreamTHR","cmsBuildTabulatedToneCurve16","cmsTagLinkedTo","cmsFormatterForPCSOfProfile","cmsSetHeaderProfileID","cmsCreateXYZProfile","cmsCreateTransformTHR","cmsGetTransformGamutCheckPipeline","cmsSliceSpace16","cmsSliceSpaceFloat","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8GetPatchName","cmsIT8Free","cmsFreeToneCurve","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsOpenProfileFromStreamTHR","cmsBuildTabulatedToneCurve16","cmsTagLinkedTo","cmsFormatterForPCSOfProfile","cmsSetHeaderProfileID","cmsCreateXYZProfile","cmsCreateTransformTHR","cmsGetTransformGamutCheckPipeline","cmsSliceSpace16","cmsSliceSpaceFloat","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8GetPatchName","cmsIT8Free","cmsFreeToneCurve","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified LCMS APIs to process ICC profile data from a byte stream:

*/


// Sample functions for cmsSliceSpace16 and cmsSliceSpaceFloat
static cmsInt32Number  id_000070_dummy_sampler16(const cmsUInt16Number In[], cmsUInt16Number Out[], void* Cargo) {
    (void)In; (void)Out; (void)Cargo;
    return 1;
}

static cmsInt32Number  id_000070_dummy_samplerfloat(const cmsFloat32Number In[], cmsFloat32Number Out[], void* Cargo) {
    (void)In; (void)Out; (void)Cargo;
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // 2. Try to open profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        // Try as IT8 data if not a valid profile
        FILE* in_file = fmemopen((void*)data, size, "rb");
        if (in_file) {
            hProfile = cmsOpenProfileFromStreamTHR(ctx, in_file, "r");
            assert_file_closed(&in_file);;
        }
    }
    
    // 3. Create a tone curve using part of input data
    cmsToneCurve* curve = nullptr;
    if (size >= 256) { // Need enough data for tone curve
        const size_t entries = 128;
        cmsUInt16Number values[entries];
        for (size_t i = 0; i < entries && i*2 < size; i++) {
            values[i] = (data[i*2] << 8) | data[i*2 + 1];
        }
        curve = cmsBuildTabulatedToneCurve16(ctx, entries, values);
    }
    
    // 4. Use profile-related APIs if we have a valid profile
    if (hProfile) {
        // Get tag linked to red TRC tag
        cmsTagLinkedTo(hProfile, cmsSigRedTRCTag);
        
        // Get formatter for PCS
        cmsFormatterForPCSOfProfile(hProfile, 8, FALSE);
        
        // Set header profile ID (use first 16 bytes of data)
        cmsUInt8Number profileID[16];
        size_t copySize = size < 16 ? size : 16;
        memcpy(profileID, data, copySize);
        if (copySize < 16) memset(profileID + copySize, 0, 16 - copySize);
        cmsSetHeaderProfileID(hProfile, profileID);
        
        // Create a simple transform to get gamut check pipeline
        cmsHPROFILE xyzProfile = cmsCreateXYZProfile();
        if (xyzProfile) {
            cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, hProfile, 
                TYPE_RGB_8, xyzProfile, TYPE_XYZ_16, INTENT_PERCEPTUAL, 0);
            
            if (transform) {
                // Get gamut check pipeline
                cmsPipeline* gamutPipeline = cmsGetTransformGamutCheckPipeline(transform);
                (void)gamutPipeline; // Use pipeline if needed
                
                // Sample using cmsSliceSpace16 and cmsSliceSpaceFloat
                cmsUInt32Number clutPoints[] = {5, 5, 5};
                void* cargo = nullptr;
                
                // Try 16-bit sampling
                cmsSliceSpace16(3, clutPoints,  id_000070_dummy_sampler16, cargo);
                
                // Try float sampling
                cmsSliceSpaceFloat(3, clutPoints,  id_000070_dummy_samplerfloat, cargo);
                
                cmsDeleteTransform(transform);
            }
            cmsCloseProfile(xyzProfile);
        }
        
        // Close profile
        cmsCloseProfile(hProfile);
    }
    
    // 5. Create and use IT8 handle
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    if (it8) {
        char buffer[256];
        
        // Set some properties using input data
        if (size > 50) {
            cmsIT8SetPropertyStr(it8, "SAMPLE_ID", "TestSample");
            
            // Try to get patch name
            const char* patchName = cmsIT8GetPatchName(it8, 0, buffer);
            (void)patchName;
        }
        
        cmsIT8Free(it8);
    }
    
    // 6. Clean up
    if (curve) {
        cmsFreeToneCurve(curve);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}