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
//<ID> 397
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsGetProfileInfoASCII","cmsSetHeaderFlags","cmsSetAlarmCodes","cmsIT8Alloc","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsCIE94DeltaE","cmsReadTag","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsFreeNamedColorList","cmsPipelineAlloc","cmsBuildGamma","cmsPipelineGetPtrToFirstStage","cmsFreeToneCurve","cmsPipelineFree","_cmsICCcolorSpace","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsGetProfileInfoASCII","cmsSetHeaderFlags","cmsSetAlarmCodes","cmsIT8Alloc","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsCIE94DeltaE","cmsReadTag","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsFreeNamedColorList","cmsPipelineAlloc","cmsBuildGamma","cmsPipelineGetPtrToFirstStage","cmsFreeToneCurve","cmsPipelineFree","_cmsICCcolorSpace","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a context first
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // Open a FILE* from the input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    // Try to open profile from stream
    cmsHPROFILE profile = cmsOpenProfileFromStreamTHR(ctx, in_file, "r");
    if (!profile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get profile info ASCII
    char buffer[256];
    cmsUInt32Number buffer_size = sizeof(buffer);
    cmsGetProfileInfoASCII(profile, cmsInfoDescription, "en", "US", buffer, buffer_size);

    // Set header flags
    cmsSetHeaderFlags(profile, 0);

    // Set alarm codes
    cmsUInt16Number alarm_codes[16] = {0};
    cmsSetAlarmCodes(alarm_codes);

    // Allocate IT8 handle
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    if (it8) {
        // Enumerate property multi
        const char **subprop_names = nullptr;
        cmsUInt32Number count = cmsIT8EnumPropertyMulti(it8, "Sample", &subprop_names);
        
        // Free IT8 handle
        cmsIT8Free(it8);
    }

    // Compute CIE94 Delta E between two Lab colors
    cmsCIELab lab1 = {50.0, 2.0, -3.0};
    cmsCIELab lab2 = {52.0, 1.5, -2.5};
    cmsFloat64Number delta_e = cmsCIE94DeltaE(&lab1, &lab2);

    // Read a tag from profile
    void *tag_data = cmsReadTag(profile, cmsSigMediaBlackPointTag);

    // Get named color list if available
    cmsNAMEDCOLORLIST *named_color_list = cmsGetNamedColorList(nullptr);
    if (named_color_list) {
        cmsUInt32Number color_count = cmsNamedColorCount(named_color_list);
        if (color_count > 0) {
            char name[256];
            char prefix[256];
            char suffix[256];
            cmsUInt16Number pcs[3];
            cmsUInt16Number colorant[16];
            cmsNamedColorInfo(named_color_list, 0, name, prefix, suffix, pcs, colorant);
        }
        cmsFreeNamedColorList(named_color_list);
    }

    // Create a pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline) {
        // Build gamma curve
        cmsToneCurve *gamma_curve = cmsBuildGamma(ctx, 2.2);
        if (gamma_curve) {
            // Get pointer to first stage (will be null since pipeline is empty)
            cmsStage *first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
            
            // Free the gamma curve
            cmsFreeToneCurve(gamma_curve);
        }
        
        // Free the pipeline
        cmsPipelineFree(pipeline);
    }

    // Convert integer to color space signature
    cmsColorSpaceSignature cs = _cmsICCcolorSpace(3); // 3 = RGB

    // Close profile
    cmsCloseProfile(profile);

    // Close the input file
    assert_file_closed(&in_file);;

    // Delete context
    cmsDeleteContext(ctx);

    assert_file_closed(&in_file);
	return 0;
}