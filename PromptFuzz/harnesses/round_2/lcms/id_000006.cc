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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["cmsCreateContext","cmsSetLogErrorHandler","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsDeleteContext","cmsGetTagOffsetAndSize","cmsCreateProfilePlaceholder","cmsCreateExtendedTransform","cmsGetTransformPipeline","cmsPipelineInputChannels","cmsPipelineSetSaveAs8bitsFlag","cmsFloat2LabEncoded","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandler","cmsOpenProfileFromFileTHR","cmsGetTagOffsetAndSize","cmsCreateProfilePlaceholder","cmsCreateExtendedTransform","cmsGetTransformPipeline","cmsPipelineInputChannels","cmsPipelineSetSaveAs8bitsFlag","cmsFloat2LabEncoded","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Too small for meaningful processing
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // Set up error handler
    cmsSetLogErrorHandler([](cmsContext, cmsUInt32Number, const char *) {
        // Silent error handler for fuzzing
    });
    
    // Create a profile from input data
    cmsHPROFILE profiles[2] = {0};
    cmsBool bpc[2] = {FALSE, FALSE};
    cmsUInt32Number intents[2] = {INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC};
    cmsFloat64Number adaptation[2] = {1.0, 1.0};
    
    // Try to open profile from memory
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read first part as profile
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    profiles[0] = hProfile;
    
    // Get tag information
    cmsUInt32Number offset = 0, tagSize = 0;
    cmsGetTagOffsetAndSize(hProfile, 0, &offset, &tagSize);
    
    // Create second profile as placeholder
    cmsHPROFILE hProfile2 = cmsCreateProfilePlaceholder(context);
    if (hProfile2) {
        profiles[1] = hProfile2;
    }
    
    // Create extended transform
    cmsHTRANSFORM transform = cmsCreateExtendedTransform(
        context, 
        hProfile2 ? 2 : 1,
        profiles,
        bpc,
        intents,
        adaptation,
        NULL,  // No gamut profile
        0,     // No gamut PCS position
        TYPE_Lab_16,
        TYPE_RGB_16,
        0
    );
    
    if (transform) {
        // Get pipeline from transform (using internal knowledge)
        cmsPipeline* pipeline = cmsGetTransformPipeline(transform);
        if (pipeline) {
            // Get input channels
            cmsUInt32Number inputChannels = cmsPipelineInputChannels(pipeline);
            
            // Set save as 8 bits flag
            cmsPipelineSetSaveAs8bitsFlag(pipeline, TRUE);
        }
        
        // Test float to Lab encoded conversion
        cmsCIELab lab = {50.0, 0.0, 0.0};
        cmsUInt16Number encodedLab[3];
        cmsFloat2LabEncoded(encodedLab, &lab);
        
        // Clean up transform
        cmsDeleteTransform(transform);
    }
    
    // Clean up profiles
    if (hProfile) cmsCloseProfile(hProfile);
    if (hProfile2) cmsCloseProfile(hProfile2);
    
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}