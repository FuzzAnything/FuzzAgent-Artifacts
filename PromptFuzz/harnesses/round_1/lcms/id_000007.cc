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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsGetColorSpace","cmsChannelsOfColorSpace","cmsBuildGamma","cmsCreateRGBProfileTHR","cmsSetPCS","cmsReadTag","cmsWriteRawTag","cmsIsCLUT","cmsAllocNamedColorList","cmsFreeNamedColorList","cmsGetHeaderManufacturer","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsGetColorSpace","cmsChannelsOfColorSpace","cmsBuildGamma","cmsCreateRGBProfileTHR","cmsSetPCS","cmsReadTag","cmsWriteRawTag","cmsIsCLUT","cmsAllocNamedColorList","cmsFreeNamedColorList","cmsGetHeaderManufacturer","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to process ICC profile data:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful operations

    // Initialize context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    // Open profile from input stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(ctx, in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get color space from input profile
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile);
    
    // Get channels count for this color space
    cmsInt32Number channels = cmsChannelsOfColorSpace(colorSpace);

    // Create a simple RGB profile
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0}; // D65
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},  // Red
        {0.3000, 0.6000, 1.0},  // Green
        {0.1500, 0.0600, 1.0}   // Blue
    };
    
    // Create tone curves for transfer functions (gamma 2.2)
    cmsToneCurve* transferFunctions[3];
    for (int i = 0; i < 3; i++) {
        cmsFloat64Number gamma = 2.2;
        transferFunctions[i] = cmsBuildGamma(nullptr, gamma);
    }
    
    // Create new RGB profile
    cmsHPROFILE newProfile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, 
                                                    transferFunctions);
    if (newProfile) {
        // Set PCS for new profile
        cmsSetPCS(newProfile, cmsSigLabData);
        
        // Read a tag from input profile
        void* tagData = cmsReadTag(hProfile, cmsSigMediaWhitePointTag);
        if (tagData) {
            // Write the tag to new profile
            cmsWriteRawTag(newProfile, cmsSigMediaWhitePointTag, tagData, 
                          sizeof(cmsCIEXYZ));
        }
        
        // Check if input profile has CLUT
        cmsBool hasCLUT = cmsIsCLUT(hProfile, INTENT_PERCEPTUAL, 0);
        
        // Allocate named color list
        cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(ctx, 10, 3, 
                                                                   "Prefix", "Suffix");
        if (namedColorList) {
            cmsFreeNamedColorList(namedColorList);
        }
        
        // Get manufacturer from input profile header
        cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
        
        // Clean up new profile
        cmsCloseProfile(newProfile);
    }
    
    // Clean up transfer functions
    for (int i = 0; i < 3; i++) {
        if (transferFunctions[i]) {
            cmsFreeToneCurve(transferFunctions[i]);
        }
    }

    // Close resources
    if (hProfile) cmsCloseProfile(hProfile);
    if (in_file) assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}