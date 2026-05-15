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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["cmsCreateContext","cmsBuildParametricToneCurve","cmsFreeToneCurve","cmsDeleteContext","cmsCreateRGBProfileTHR","cmsGetColorSpace","cmsChannelsOfColorSpace","cmsSetPCS","cmsReadTag","cmsIsCLUT","cmsGetHeaderManufacturer","cmsAllocNamedColorList","cmsWriteRawTag","cmsFreeNamedColorList","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsBuildParametricToneCurve","cmsCreateRGBProfileTHR","cmsGetColorSpace","cmsChannelsOfColorSpace","cmsSetPCS","cmsReadTag","cmsIsCLUT","cmsGetHeaderManufacturer","cmsAllocNamedColorList","cmsWriteRawTag","cmsFreeNamedColorList","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create RGB profile with standard sRGB parameters
    cmsCIExyY white_point = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},  // Red
        {0.3000, 0.6000, 1.0},  // Green
        {0.1500, 0.0600, 1.0}   // Blue
    };
    
    // Create tone curves for transfer functions
    cmsToneCurve* transfer_functions[3] = {NULL, NULL, NULL};
    cmsFloat64Number gamma_params[5] = {2.2, 1.0, 0.0, 0.0, 0.0};
    
    for (int i = 0; i < 3; i++) {
        transfer_functions[i] = cmsBuildParametricToneCurve(context, 1, gamma_params);
        if (!transfer_functions[i]) {
            for (int j = 0; j < i; j++) cmsFreeToneCurve(transfer_functions[j]);
            cmsDeleteContext(context);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Create RGB profile
    cmsHPROFILE rgb_profile = cmsCreateRGBProfileTHR(context, &white_point, &primaries, transfer_functions);
    
    // Get color space of created profile
    cmsColorSpaceSignature color_space = cmsGetColorSpace(rgb_profile);
    
    // Calculate channels of the color space
    cmsInt32Number channels = cmsChannelsOfColorSpace(color_space);
    
    // Set PCS for the profile
    cmsSetPCS(rgb_profile, cmsSigLabData);
    
    // Read a tag from the profile
    const cmsMLU* tag_data = static_cast<const cmsMLU*>(cmsReadTag(rgb_profile, cmsSigCopyrightTag));
    
    // Check if profile has CLUT for perceptual intent
    cmsBool has_clut = cmsIsCLUT(rgb_profile, INTENT_PERCEPTUAL, LCMS_USED_AS_INPUT);
    
    // Get header manufacturer
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(rgb_profile);
    
    // Create named color list
    cmsNAMEDCOLORLIST* named_colors = cmsAllocNamedColorList(context, 10, channels, "Prefix", "Suffix");
    
    // Write raw tag if we have some data
    if (size > 100) {
        cmsWriteRawTag(rgb_profile, cmsSigColorantTableTag, data, (cmsUInt32Number)(size > 1024 ? 1024 : size));
    }
    
    // Cleanup
    if (named_colors) {
        // Free individual color names would require iterating through list,
        // but cmsFreeNamedColorList should handle it
        cmsFreeNamedColorList(named_colors);
    }
    
    // Free tone curves
    for (int i = 0; i < 3; i++) {
        if (transfer_functions[i]) {
            cmsFreeToneCurve(transfer_functions[i]);
        }
    }
    
    // Close profile
    if (rgb_profile) {
        cmsCloseProfile(rgb_profile);
    }
    
    // Delete context
    cmsDeleteContext(context);
    
    // Close file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}