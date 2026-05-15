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
//<ID> 372
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 7
//<Quality> {"density":16,"unique_branches":{"PrelinEval8":[[981,33,981,41,0,0,4,1],[988,37,988,45,0,0,4,1],[995,29,995,37,0,0,4,0],[995,29,995,37,0,0,4,1],[995,41,995,49,0,0,4,0],[1002,33,1002,41,0,0,4,0],[1002,45,1002,53,0,0,4,0]]},"library_calls":["cmsOpenProfileFromStream","cmsCreate_sRGBProfile","cmsCloseProfile","cmsCreateTransform","cmsCloseProfile","cmsCloseProfile","cmsGetTransformPipeline","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsDoTransformLineStride","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetPropertyStr","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsReadTag","cmsMLUtranslationsCount","cmsMLUtranslationsCodes","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromStream","cmsCreate_sRGBProfile","cmsCreateTransform","cmsGetTransformPipeline","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsDoTransformLineStride","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetPropertyStr","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsReadTag","cmsMLUtranslationsCount","cmsMLUtranslationsCodes","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to process color transformation data:

*/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for processing

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Open a profile from the input data
    cmsHPROFILE input_profile = cmsOpenProfileFromStream(in_file, "r");
    if (!input_profile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a simple sRGB output profile
    cmsHPROFILE output_profile = cmsCreate_sRGBProfile();
    if (!output_profile) {
        cmsCloseProfile(input_profile);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a transform
    cmsHTRANSFORM transform = cmsCreateTransform(
        input_profile,
        TYPE_RGB_8,
        output_profile,
        TYPE_RGB_8,
        INTENT_PERCEPTUAL,
        0
    );
    
    if (!transform) {
        cmsCloseProfile(output_profile);
        cmsCloseProfile(input_profile);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Use cmsPipelineStageCount and cmsPipelineGetPtrToFirstStage
    cmsPipeline *pipeline = cmsGetTransformPipeline(transform);
    if (pipeline) {
        cmsUInt32Number stage_count = cmsPipelineStageCount(pipeline);
        cmsStage *first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
        
        // Use the stage count and first stage (even if just checking)
        if (stage_count > 0 && first_stage) {
            // We have valid pipeline information
        }
    }

    // 2. Use cmsDoTransformLineStride
    // Prepare input and output buffers
    const int width = 4;
    const int height = 4;
    const int channels = 3;
    
    uint8_t *input_buffer = (uint8_t*)malloc(width * height * channels);
    uint8_t *output_buffer = (uint8_t*)malloc(width * height * channels);
    
    if (input_buffer && output_buffer) {
        // Fill input buffer with some data (use part of fuzzer input)
        size_t bytes_to_copy = width * height * channels;
        if (bytes_to_copy > size) bytes_to_copy = size;
        memcpy(input_buffer, data, bytes_to_copy);
        
        // Perform transformation with stride
        cmsDoTransformLineStride(
            transform,
            input_buffer,
            output_buffer,
            width,           // PixelsPerLine
            height,          // LineCount
            width * channels, // BytesPerLineIn
            width * channels, // BytesPerLineOut
            0,               // BytesPerPlaneIn
            0                // BytesPerPlaneOut
        );
    }
    
    // 3. Create and use IT8 handle for cmsIT8GetPropertyDbl
    cmsHANDLE it8_handle = cmsIT8Alloc(0); // Use default context
    if (it8_handle) {
        // Set some properties first
        cmsIT8SetPropertyStr(it8_handle, "SAMPLE_ID", "TestSample");
        cmsIT8SetPropertyStr(it8_handle, "NUMBER_OF_FIELDS", "3");
        
        // Try to get a property as double
        // Note: cmsIT8GetPropertyDbl is in the list but not in the provided prototypes
        // We'll use cmsIT8GetDataDbl instead which is available
        cmsIT8SetData(it8_handle, "1", "R", "0.5");
        cmsFloat64Number prop_value = cmsIT8GetDataDbl(it8_handle, "1", "R");
        
        // Use the value (even if just to prevent optimization)
        if (prop_value > 1.0) {
            // This shouldn't happen with our test data
        }
        
        // 4. Use cmsIT8Free
        cmsIT8Free(it8_handle);
    }

    // 5. Use cmsMLUtranslationsCodes
    // Get MLU from profile (description tag)
    cmsMLU *mlu = (cmsMLU*)cmsReadTag(input_profile, cmsSigProfileDescriptionTag);
    if (mlu) {
        char language_code[3] = {0};
        char country_code[3] = {0};
        
        // Try to get translation codes
        // Note: cmsMLUtranslationsCodes needs to be called with valid index
        cmsUInt32Number translations_count = cmsMLUtranslationsCount(mlu);
        if (translations_count > 0) {
            // Use cmsMLUtranslationsCodes (available in lcms2.h)
            // This function is in the provided list
            cmsMLUtranslationsCodes(mlu, 0, language_code, country_code);
        }
    }

    // Clean up
    if (input_buffer) free(input_buffer);
    if (output_buffer) free(output_buffer);
    
    cmsDeleteTransform(transform);
    cmsCloseProfile(output_profile);
    cmsCloseProfile(input_profile);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}