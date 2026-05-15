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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["cmsSetLogErrorHandler","cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsDeleteContext","cmsDeleteContext","cmsOpenProfileFromMemTHR","cmsDeleteContext","cmsGetTagOffsetAndSize","cmsCreateLab2ProfileTHR","cmsCreateExtendedTransform","cmsPipelineAlloc","cmsPipelineInputChannels","cmsPipelineSetSaveAs8bitsFlag","cmsPipelineFree","cmsFloat2LabEncoded","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsSetLogErrorHandler","cmsCreateContext","cmsOpenProfileFromMemTHR","cmsGetTagOffsetAndSize","cmsCreateLab2ProfileTHR","cmsCreateExtendedTransform","cmsPipelineAlloc","cmsPipelineInputChannels","cmsPipelineSetSaveAs8bitsFlag","cmsPipelineFree","cmsFloat2LabEncoded","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Set up error handler
    cmsSetLogErrorHandler([](cmsContext /*ContextID*/, cmsUInt32Number /*ErrorCode*/, const char* /*Text*/) {
        // Error handler callback - can be empty for fuzzing
    });

    // 2. Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
        return 0;
    }

    // 3. Create profiles from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the file to get profile data
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_size <= 0) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    uint8_t* profile_data = (uint8_t*)malloc(file_size);
    if (!profile_data) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_size = fread(profile_data, 1, file_size, in_file);
    assert_file_closed(&in_file);;
    
    if (read_size != (size_t)file_size) {
        free(profile_data);
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ctx, profile_data, file_size);
    if (!hProfile) {
        free(profile_data);
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Use cmsGetTagOffsetAndSize
    cmsUInt32Number offset = 0, tag_size = 0;
    cmsBool tag_result = cmsGetTagOffsetAndSize(hProfile, 0, &offset, &tag_size);
    (void)tag_result; // Use result to avoid unused variable warning

    // 5. Create second profile (dummy lab profile for transform)
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(ctx, nullptr);
    
    // 6. Prepare arrays for extended transform
    cmsHPROFILE profiles[2] = {hProfile, labProfile};
    cmsBool bpc[2] = {FALSE, FALSE};
    cmsUInt32Number intents[2] = {INTENT_PERCEPTUAL, INTENT_PERCEPTUAL};
    cmsFloat64Number adaptationStates[2] = {1.0, 1.0};

    // 7. Create extended transform
    cmsHTRANSFORM transform = cmsCreateExtendedTransform(
        ctx, 
        2, 
        profiles, 
        bpc, 
        intents, 
        adaptationStates, 
        nullptr, // No gamut profile
        0, 
        TYPE_Lab_16, // Input format
        TYPE_Lab_16, // Output format
        cmsFLAGS_NOCACHE
    );

    if (transform) {
        // 8. Get pipeline from transform (need to access pipeline functions)
        // Note: This is a workaround since cmsGetTransformPipeline is not in the provided list
        // We'll create a dummy pipeline instead
        cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
        if (pipeline) {
            // 9. Use cmsPipelineInputChannels
            cmsUInt32Number input_channels = cmsPipelineInputChannels(pipeline);
            (void)input_channels; // Use result
            
            // 10. Use cmsPipelineSetSaveAs8bitsFlag
            cmsPipelineSetSaveAs8bitsFlag(pipeline, TRUE);
            
            cmsPipelineFree(pipeline);
        }

        // 11. Use cmsFloat2LabEncoded
        cmsCIELab lab;
        lab.L = 50.0;
        lab.a = 0.0;
        lab.b = 0.0;
        cmsUInt16Number encodedLab[3];
        cmsFloat2LabEncoded(encodedLab, &lab);

        // 12. Clean up transform
        cmsDeleteTransform(transform);
    }

    // 13. Clean up
    if (labProfile) {
        cmsCloseProfile(labProfile);
    }
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    free(profile_data);
    cmsDeleteContext(ctx);

    assert_file_closed(&in_file);
	return 0;
}