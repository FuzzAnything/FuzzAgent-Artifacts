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
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 5
//<Quality> {"density":20,"unique_branches":{"OutputValueSampler":[[574,37,574,52,0,0,4,1],[580,21,580,86,0,0,4,1],[583,27,583,39,0,0,4,0],[583,27,583,39,0,0,4,1]],"PatchLUT":[[519,17,519,33,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsDeleteContext","cmsGetColorSpace","cmsChannelsOfColorSpace","cmsGetHeaderManufacturer","cmsReadTag","cmsMLUtranslationsCodes","cmsSetPCS","cmsBuildGamma","cmsCreateRGBProfileTHR","cmsAllocNamedColorList","cmsOpenIOhandlerFromNULL","cmsGetPostScriptColorResource","cmsCloseIOhandler","cmsXYZ2xyY","cmsCloseProfile","cmsCloseProfile","cmsFreeNamedColorList","cmsFreeToneCurve","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsGetColorSpace","cmsChannelsOfColorSpace","cmsGetHeaderManufacturer","cmsReadTag","cmsMLUtranslationsCodes","cmsSetPCS","cmsBuildGamma","cmsCreateRGBProfileTHR","cmsAllocNamedColorList","cmsOpenIOhandlerFromNULL","cmsGetPostScriptColorResource","cmsCloseIOhandler","cmsXYZ2xyY","cmsCloseProfile","cmsCloseProfile","cmsFreeNamedColorList","cmsFreeToneCurve","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 1: Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Step 2: Open profile from input data stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE input_profile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    if (!input_profile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 3: Get color space information
    cmsColorSpaceSignature colorspace = cmsGetColorSpace(input_profile);
    int channels = cmsChannelsOfColorSpace(colorspace);
    
    // Step 4: Get header manufacturer
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(input_profile);
    
    // Step 5: Read copyright tag (assuming it's an MLU)
    cmsMLU *copyright_mlu = (cmsMLU *)cmsReadTag(input_profile, cmsSigCopyrightTag);
    if (copyright_mlu) {
        char lang_code[3] = {0};
        char country_code[3] = {0};
        // Get first translation code
        cmsMLUtranslationsCodes(copyright_mlu, 0, lang_code, country_code);
    }
    
    // Step 6: Set PCS to Lab
    cmsSetPCS(input_profile, cmsSigLabData);
    
    // Step 7: Create RGB profile
    cmsCIExyY white_point = {0.3127, 0.3290, 1.0}; // D65
    cmsCIExyYTRIPLE primaries = {
        {0.64, 0.33, 1.0},  // Red
        {0.30, 0.60, 1.0},  // Green
        {0.15, 0.06, 1.0}   // Blue
    };
    
    // Create gamma curves for RGB channels
    cmsToneCurve *gamma_curve = cmsBuildGamma(context, 2.2);
    cmsToneCurve *transfer_funcs[3] = {gamma_curve, gamma_curve, gamma_curve};
    
    cmsHPROFILE rgb_profile = cmsCreateRGBProfileTHR(context, &white_point, &primaries, transfer_funcs);
    
    // Step 8: Allocate named color list
    cmsNAMEDCOLORLIST *named_colors = cmsAllocNamedColorList(context, 10, 3, "Prefix", "Suffix");
    
    // Step 9: Get PostScript color resource
    cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromNULL(context);
    if (io_handler) {
        cmsUInt32Number bytes_written = cmsGetPostScriptColorResource(
            context, cmsPS_RESOURCE_CRD, rgb_profile, INTENT_PERCEPTUAL, 0, io_handler);
        // Clean up handler
        cmsCloseIOhandler(io_handler);
    }
    
    // Step 10: Convert XYZ to xyY (using D50)
    cmsCIEXYZ d50_xyz = {0.9642, 1.0000, 0.8249};
    cmsCIExyY d50_xyy;
    cmsXYZ2xyY(&d50_xyy, &d50_xyz);
    
    // Step 11: Cleanup
    // Close profiles
    cmsCloseProfile(input_profile);
    if (rgb_profile) cmsCloseProfile(rgb_profile);
    
    // Free named color list
    if (named_colors) cmsFreeNamedColorList(named_colors);
    
    // Free tone curve
    if (gamma_curve) cmsFreeToneCurve(gamma_curve);
    
    // Close input file
    assert_file_closed(&in_file);;
    
    // Delete context
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}