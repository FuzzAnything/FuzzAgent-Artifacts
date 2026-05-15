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
//<ID> 151
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 0
//<Quality> {"density":34,"unique_branches":{},"library_calls":["cmsCreateContext","cmsCreateNULLProfile","cmsDeleteContext","cmsCreateInkLimitingDeviceLinkTHR","cmsCloseProfile","cmsDeleteContext","cmsMLUalloc","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsIT8Alloc","cmsMLUfree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsIT8Free","cmsMLUfree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsIT8SetPropertyStr","cmsIT8GetData","cmsCreateTransformTHR","cmsGetTransformOutputFormat","cmsGetTransformPipeline","cmsPipelineEvalFloat","cmsBuildGamma","cmsGetToneCurveEstimatedTableEntries","cmsMLUtranslationsCount","cmsAdaptToIlluminant","cmsFreeToneCurve","cmsDeleteTransform","cmsIT8Free","cmsMLUfree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsMLUalloc","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8GetData","cmsCreateTransformTHR","cmsGetTransformOutputFormat","cmsGetTransformPipeline","cmsPipelineEvalFloat","cmsBuildGamma","cmsGetToneCurveEstimatedTableEntries","cmsMLUtranslationsCount","cmsAdaptToIlluminant","cmsFreeToneCurve","cmsDeleteTransform","cmsIT8Free","cmsMLUfree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful processing

    // Initialize context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // 1. Create NULL profile
    cmsHPROFILE null_profile = cmsCreateNULLProfile();
    if (!null_profile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // 2. Create ink limiting device link
    cmsHPROFILE ink_limit_profile = cmsCreateInkLimitingDeviceLinkTHR(
        ctx, cmsSigCmykData, 300.0); // CMYK with 300% ink limit
    if (!ink_limit_profile) {
        cmsCloseProfile(null_profile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // 3. Create MLU for translations
    cmsMLU* mlu = cmsMLUalloc(ctx, 5);
    if (!mlu) {
        cmsCloseProfile(ink_limit_profile);
        cmsCloseProfile(null_profile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // 4. Create IT8 handle and set properties
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    if (!it8) {
        cmsMLUfree(mlu);
        cmsCloseProfile(ink_limit_profile);
        cmsCloseProfile(null_profile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Use input data for IT8 property string
    size_t str_len = size > 255 ? 255 : size;
    char* prop_str = (char*)malloc(str_len + 1);
    if (!prop_str) {
        cmsIT8Free(it8);
        cmsMLUfree(mlu);
        cmsCloseProfile(ink_limit_profile);
        cmsCloseProfile(null_profile);
        cmsDeleteContext(ctx);
        return 0;
    }
    memcpy(prop_str, data, str_len);
    prop_str[str_len] = '\0';
    
    // Set IT8 property using input data
    cmsBool it8_set = cmsIT8SetPropertyStr(it8, "SampleProperty", prop_str);
    
    // Get data back from IT8
    const char* retrieved_data = cmsIT8GetData(it8, "Patch1", "Sample1");
    // Note: The retrieved data might be NULL if no data was set

    // 5. Create transform to get output format
    cmsUInt32Number input_format = TYPE_Lab_DBL;
    cmsUInt32Number output_format = TYPE_CMYK_DBL;
    
    cmsHTRANSFORM transform = cmsCreateTransformTHR(
        ctx, null_profile, input_format,
        ink_limit_profile, output_format,
        INTENT_PERCEPTUAL, 0);
    
    cmsUInt32Number transform_output_format = 0;
    if (transform) {
        transform_output_format = cmsGetTransformOutputFormat(transform);
        
        // 6. Get pipeline for eval float
        const cmsPipeline* pipeline = cmsGetTransformPipeline(transform);
        if (pipeline) {
            cmsFloat32Number in_color[4] = {0.5f, 0.3f, 0.2f, 0.0f};
            cmsFloat32Number out_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
            cmsPipelineEvalFloat(in_color, out_color, pipeline);
        }
    }

    // 7. Create tone curve for table entries estimation
    cmsFloat64Number gamma_val = 2.2;
    cmsToneCurve* tone_curve = cmsBuildGamma(ctx, gamma_val);
    cmsUInt32Number table_entries = 0;
    if (tone_curve) {
        table_entries = cmsGetToneCurveEstimatedTableEntries(tone_curve);
    }

    // 8. Get MLU translations count
    cmsUInt32Number translation_count = cmsMLUtranslationsCount(mlu);

    // 9. Use cmsAdaptToIlluminant with sample data
    cmsCIEXYZ source_white = {0.9505, 1.0000, 1.0890}; // D50
    cmsCIEXYZ illuminant = {0.9642, 1.0000, 0.8251};   // D65
    cmsCIEXYZ sample_color;
    
    // Use input data for color values (ensure we have enough data)
    if (size >= 24) {
        memcpy(&sample_color, data, sizeof(cmsCIEXYZ));
    } else {
        sample_color.X = 0.5;
        sample_color.Y = 0.5;
        sample_color.Z = 0.5;
    }
    
    cmsCIEXYZ adapted_color;
    cmsBool adapted = cmsAdaptToIlluminant(&adapted_color, &source_white, &illuminant, &sample_color);

    // Clean up
    if (tone_curve) cmsFreeToneCurve(tone_curve);
    if (transform) cmsDeleteTransform(transform);
    free(prop_str);
    cmsIT8Free(it8);
    cmsMLUfree(mlu);
    cmsCloseProfile(ink_limit_profile);
    cmsCloseProfile(null_profile);
    cmsDeleteContext(ctx);

    return 0;
}