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
//<ID> 611
//<Prompt> []
/*<Combination>: [
*/
//<score> 9.333333, nr_unique_branch: 0
//<Quality> {"density":28,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsCreate_sRGBProfile","cmsCloseProfile","cmsDeleteContext","cmsGetProfileInfoASCII","cmsGetProfileInfoASCII","cmsGetAlarmCodes","cmsBuildGamma","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsSetAdaptationStateTHR","cmsGetHeaderFlags","cmsCreateProofingTransformTHR","cmsCreateTransformTHR","cmsPipelineAlloc","cmsPipelineGetPtrToFirstStage","cmsStageInputChannels","cmsStageOutputChannels","cmsPipelineFree","cmsDeleteTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsCreate_sRGBProfile","cmsGetProfileInfoASCII","cmsGetProfileInfoASCII","cmsGetAlarmCodes","cmsBuildGamma","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsSetAdaptationStateTHR","cmsGetHeaderFlags","cmsCreateProofingTransformTHR","cmsCreateTransformTHR","cmsPipelineAlloc","cmsPipelineGetPtrToFirstStage","cmsStageInputChannels","cmsStageOutputChannels","cmsPipelineFree","cmsDeleteTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }

    // Create FILE* from input data
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Open profile from stream
    cmsHPROFILE input_profile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (!input_profile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create sRGB profile
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfile();
    if (!srgb_profile) {
        cmsCloseProfile(input_profile);
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get profile info ASCII
    char buffer[256];
    cmsUInt32Number buffer_size = sizeof(buffer);
    cmsGetProfileInfoASCII(input_profile, cmsInfoDescription, "en", "US", buffer, buffer_size);
    cmsGetProfileInfoASCII(srgb_profile, cmsInfoDescription, "en", "US", buffer, buffer_size);

    // Get alarm codes
    cmsUInt16Number alarm_codes[16];
    cmsGetAlarmCodes(alarm_codes);

    // Create a tone curve for testing
    cmsToneCurve* tone_curve = cmsBuildGamma(context, 2.2);
    if (tone_curve) {
        cmsFreeToneCurve(tone_curve);
    }

    // Create IT8 handle and enum property multi
    cmsHANDLE it8_handle = cmsIT8Alloc(context);
    if (it8_handle) {
        const char** subproperty_names = nullptr;
        cmsUInt32Number count = cmsIT8EnumPropertyMulti(it8_handle, "TEST", &subproperty_names);
        cmsIT8Free(it8_handle);
    }

    // Set adaptation state
    cmsSetAdaptationStateTHR(context, 0.5);

    // Get header flags
    cmsUInt32Number flags = cmsGetHeaderFlags(input_profile);

    // Create transforms with different formats
    // Use first few bytes to determine format types
    cmsUInt32Number input_format = (size > 0) ? (data[0] % 4) : 0;
    cmsUInt32Number output_format = (size > 1) ? (data[1] % 4) : 0;
    cmsUInt32Number intent = (size > 2) ? (data[2] % 4) : 0;
    cmsUInt32Number proofing_intent = (size > 3) ? (data[3] % 4) : 0;
    cmsUInt32Number dw_flags = 0;

    // Map input_format to TYPE_RGB_8, TYPE_GRAY_8, etc.
    cmsUInt32Number input_type = 0;
    switch (input_format) {
        case 0: input_type = TYPE_RGB_8; break;
        case 1: input_type = TYPE_GRAY_8; break;
        case 2: input_type = TYPE_CMYK_8; break;
        case 3: input_type = TYPE_Lab_8; break;
        default: input_type = TYPE_RGB_8;
    }

    cmsUInt32Number output_type = 0;
    switch (output_format) {
        case 0: output_type = TYPE_RGB_8; break;
        case 1: output_type = TYPE_GRAY_8; break;
        case 2: output_type = TYPE_CMYK_8; break;
        case 3: output_type = TYPE_Lab_8; break;
        default: output_type = TYPE_RGB_8;
    }

    // Create proofing transform
    cmsHTRANSFORM proofing_transform = cmsCreateProofingTransformTHR(
        context,
        input_profile,
        input_type,
        srgb_profile,
        output_type,
        input_profile,  // Use input profile as proofing profile
        intent,
        proofing_intent,
        dw_flags
    );

    // Create regular transform
    cmsHTRANSFORM regular_transform = cmsCreateTransformTHR(
        context,
        input_profile,
        input_type,
        srgb_profile,
        output_type,
        intent,
        dw_flags
    );

    // Create a simple pipeline to test stage functions
    // Use cmsPipelineAlloc from available APIs
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        // Get first stage (should be nullptr for empty pipeline)
        cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
        if (first_stage) {
            // These would be called if there were stages
            cmsStageInputChannels(first_stage);
            cmsStageOutputChannels(first_stage);
        }
        cmsPipelineFree(pipeline);
    }

    // Clean up transforms
    if (proofing_transform) {
        cmsDeleteTransform(proofing_transform);
    }
    if (regular_transform) {
        cmsDeleteTransform(regular_transform);
    }

    // Clean up profiles
    cmsCloseProfile(srgb_profile);
    cmsCloseProfile(input_profile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);

    assert_file_closed(&in_file);
	return 0;
}