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
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 128, nr_unique_branch: 7
//<Quality> {"density":32,"unique_branches":{"PrelinEval8":[[974,17,974,25,0,0,4,0],[974,29,974,37,0,0,4,0]],"cmsGetToneCurveSegment":[[1511,9,1511,14,0,0,4,1],[1511,18,1511,52,0,0,4,1]],"MatShaperEval16":[[1567,10,1567,18,0,0,4,0],[1568,26,1568,38,0,0,4,0],[1569,26,1569,38,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsCreate_sRGBProfileTHR","cmsDeleteContext","cmsCreate_sRGBProfileTHR","cmsCloseProfile","cmsDeleteContext","cmsSetEncodedICCversion","cmsBuildGamma","cmsGetToneCurveParametricType","cmsGetToneCurveSegment","cmsEvalToneCurve16","cmsBuildSegmentedToneCurve","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetTranslation","cmsMLUfree","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8Free","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsPipelineSetSaveAs8bitsFlag","cmsDoTransform","cmsDeleteTransform","cmsFreeToneCurve","cmsFreeToneCurve","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsCreate_sRGBProfileTHR","cmsCreate_sRGBProfileTHR","cmsSetEncodedICCversion","cmsBuildGamma","cmsGetToneCurveParametricType","cmsGetToneCurveSegment","cmsEvalToneCurve16","cmsBuildSegmentedToneCurve","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetTranslation","cmsMLUfree","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8Free","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsPipelineSetSaveAs8bitsFlag","cmsDoTransform","cmsDeleteTransform","cmsFreeToneCurve","cmsFreeToneCurve","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<58) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
	//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Minimum size for meaningful operations

    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 2. Create input profile from memory (using the provided data)
    cmsHPROFILE input_profile = cmsOpenProfileFromMemTHR(context, data, static_cast<cmsUInt32Number>(size));
    if (!input_profile) {
        // If opening from memory fails, try creating a simple sRGB profile
        input_profile = cmsCreate_sRGBProfileTHR(context);
        if (!input_profile) {
            cmsDeleteContext(context);
            return 0;
        }
    }

    // 3. Create output sRGB profile
    cmsHPROFILE output_profile = cmsCreate_sRGBProfileTHR(context);
    if (!output_profile) {
        cmsCloseProfile(input_profile);
        cmsDeleteContext(context);
        return 0;
    }

    // 4. Set encoded ICC version for output profile
    // Use some data from input to determine version (mod 4 for valid version numbers)
    cmsUInt32Number version = (data[0] % 4) + 0x04000000;
    cmsSetEncodedICCversion(output_profile, version);

    // 5. Create and manipulate tone curves
    // Use data to create gamma value (clamped to reasonable range)
    cmsFloat64Number gamma = 1.0 + (data[1] % 200) / 100.0; // Range: 1.0-3.0
    cmsToneCurve* gamma_curve = cmsBuildGamma(context, gamma);
    
    // 6. Get tone curve segment information
    if (gamma_curve) {
        // Get parametric type
        cmsInt32Number param_type = cmsGetToneCurveParametricType(gamma_curve);
        
        // Get first segment (if available)
        const cmsCurveSegment* segment = cmsGetToneCurveSegment(fuzz_int32_t_1, gamma_curve);
        
        // Evaluate the curve at some point
        cmsUInt16Number eval_value = 0x8000; // Midpoint
        if (param_type >= 0) { // Valid parametric curve
            eval_value = cmsEvalToneCurve16(gamma_curve, fuzz_uint16_t_2);
        }
        
        // Create segmented tone curve (using gamma curve's segment if available)
        cmsToneCurve* segmented_curve = nullptr;
        if (segment) {
            cmsCurveSegment segments[1];
            memcpy(&segments[0], segment, sizeof(cmsCurveSegment));
            segmented_curve = cmsBuildSegmentedToneCurve(context, 1, segments);
        }
        
        // 7. Create MLU (Multi Localized Unicode) object
        cmsMLU* mlu = cmsMLUalloc(context, fuzz_uint32_t_3);
        if (mlu) {
            // Set ASCII string using data
            char ascii_str[32];
            size_t copy_len = size < 31 ? size : 31;
            memcpy(ascii_str, data, copy_len);
            ascii_str[copy_len] = '\0';
            
            cmsMLUsetASCII(mlu, fuzz_str_4, fuzz_str_5, ascii_str);
            
            // Get translation
            char obtained_lang[3], obtained_country[3];
            cmsMLUgetTranslation(mlu, fuzz_str_6, fuzz_str_7, obtained_lang, obtained_country);
            
            cmsMLUfree(mlu);
        }
        
        // 8. Create IT8 handle
        cmsHANDLE it8 = cmsIT8Alloc(context);
        if (it8) {
            // Set property using data
            cmsFloat64Number prop_value = static_cast<cmsFloat64Number>(data[2] % 1000) / 10.0;
            cmsIT8SetPropertyDbl(it8, fuzz_str_8, prop_value);
            cmsIT8Free(it8);
        }
        
        // 9. Create transform using the profiles
        cmsUInt32Number input_format = TYPE_RGB_8;
        cmsUInt32Number output_format = TYPE_RGB_8;
        cmsUInt32Number intent = INTENT_PERCEPTUAL;
        cmsUInt32Number flags = cmsFLAGS_NOCACHE;
        
        cmsHTRANSFORM transform = cmsCreateTransformTHR(
            context,
            input_profile,
            input_format,
            output_profile,
            output_format,
            fuzz_uint32_t_9,
            fuzz_uint32_t_10
        );
        
        if (transform) {
            // 10. Get pipeline and set 8-bit flag
            cmsPipeline* pipeline = cmsGetTransformPipeline(transform);
            if (pipeline) {
                cmsPipelineSetSaveAs8bitsFlag(pipeline, data[3] % 2); // Random boolean
            }
            
            // 11. Perform color transformation
            unsigned char input_color[3] = {data[4], data[5], data[6]};
            unsigned char output_color[3];
            
            cmsDoTransform(transform, input_color, output_color, 1);
            
            // 12. Clean up transform
            cmsDeleteTransform(transform);
        }
        
        // 13. Clean up tone curves
        cmsFreeToneCurve(gamma_curve);
        if (segmented_curve) {
            cmsFreeToneCurve(segmented_curve);
        }
    }

    // 14. Use I/O handler API with file operations
    // Write data to a file first
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(data, 1, size < 1024 ? size : 1024, out_file);
        assert_file_closed(&out_file);;
    }
    
    // Open I/O handler from file
    cmsIOHANDLER* io_handler = cmsOpenIOhandlerFromFile(context, "output_file", fuzz_str_11);
    if (io_handler) {
        cmsCloseIOhandler(io_handler);
    }

    // 15. Clean up profiles
    cmsCloseProfile(input_profile);
    cmsCloseProfile(output_profile);
    
    // 16. Delete context
    cmsDeleteContext(context);
    
    assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	return 0;
}