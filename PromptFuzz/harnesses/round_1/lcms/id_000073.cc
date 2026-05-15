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
//<ID> 540
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 7
//<Quality> {"density":7,"unique_branches":{"InkLimitingSampler":[[374,31,374,41,0,0,4,1],[377,13,377,22,0,0,4,0]],"ColorSpaceIsCompatible":[[499,9,499,30,0,0,4,0],[499,34,499,57,0,0,4,1]],"cmsCreateInkLimitingDeviceLinkTHR":[[407,9,407,20,0,0,4,0],[410,13,410,22,0,0,4,0],[411,13,411,24,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsCreate_sRGBProfile","cmsOpenProfileFromStreamTHR","cmsOpenProfileFromStreamTHR","cmsGetProfileInfoASCII","cmsCreateInkLimitingDeviceLink","cmsSetAdaptationState","cmsPlugin","cmsCreateTransform","cmsGetTransformGamutCheckPipeline","cmsPipelineGetPtrToFirstStage","cmsStageInputChannels","cmsStageOutputChannels","cmsGetHeaderFlags","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreate_sRGBProfile","cmsOpenProfileFromStreamTHR","cmsOpenProfileFromStreamTHR","cmsGetProfileInfoASCII","cmsCreateInkLimitingDeviceLink","cmsSetAdaptationState","cmsPlugin","cmsCreateTransform","cmsGetTransformGamutCheckPipeline","cmsPipelineGetPtrToFirstStage","cmsStageInputChannels","cmsStageOutputChannels","cmsGetHeaderFlags","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a fuzzer implementation that uses the specified APIs to process ICC profiles:

*/


extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
	return 0;
	}
	 // Need minimum data for meaningful processing
    
    // Create memory file streams for the input data
    FILE *in_file1 = fmemopen((void *)data, size, "rb");
    FILE *in_file2 = fmemopen((void *)data, size, "rb");
    if (!in_file1 || !in_file2) {
        if (in_file1) assert_file_closed(&in_file1);;
        if (in_file2) assert_file_closed(&in_file2);;
        assert_file_closed(&in_file2);
	assert_file_closed(&in_file1);
	return 0;
    }
    
    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        assert_file_closed(&in_file1);;
        assert_file_closed(&in_file2);;
        assert_file_closed(&in_file2);
	assert_file_closed(&in_file1);
	return 0;
    }
    
    // Create sRGB profile
    cmsHPROFILE sRGB_profile = cmsCreate_sRGBProfile();
    
    // Open profiles from stream
    cmsHPROFILE profile1 = cmsOpenProfileFromStreamTHR(context, in_file1, "r");
    cmsHPROFILE profile2 = cmsOpenProfileFromStreamTHR(context, in_file2, "r");
    
    // Get profile info in ASCII
    char buffer[256] = {0};
    if (profile1) {
        cmsGetProfileInfoASCII(profile1, cmsInfoDescription, "en", "US", 
                              buffer, sizeof(buffer));
    }
    
    // Create ink limiting device link
    cmsHPROFILE ink_limiting_profile = cmsCreateInkLimitingDeviceLink(
        cmsSigCmykData, 0.85);
    
    // Set adaptation state
    cmsSetAdaptationState(1.0);
    
    // Load plugin (even though we might not have one)
    cmsPlugin(nullptr);
    
    // Create transform if we have valid profiles
    cmsHTRANSFORM transform = nullptr;
    if (sRGB_profile && ink_limiting_profile) {
        transform = cmsCreateTransform(sRGB_profile, TYPE_RGB_8,
                                      ink_limiting_profile, TYPE_CMYK_8,
                                      INTENT_PERCEPTUAL, 0);
    }
    
    // Get transform gamut check pipeline and inspect stages
    if (transform) {
        cmsPipeline *pipeline = cmsGetTransformGamutCheckPipeline(transform);
        if (pipeline) {
            cmsStage *first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (first_stage) {
                // Get stage channel information
                cmsUInt32Number input_channels = cmsStageInputChannels(first_stage);
                cmsUInt32Number output_channels = cmsStageOutputChannels(first_stage);
                (void)input_channels;  // Mark as used
                (void)output_channels; // Mark as used
            }
        }
    }
    
    // Get header flags from profile
    if (profile1) {
        cmsUInt32Number flags = cmsGetHeaderFlags(profile1);
        (void)flags; // Mark as used
    }
    
    // Clean up resources in reverse order of creation
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    // Note: We don't call cmsStageFree or cmsFreeToneCurve here because
    // those functions are for freeing objects we explicitly allocated,
    // not for objects owned by pipelines/profiles
    
    if (ink_limiting_profile) {
        cmsCloseProfile(ink_limiting_profile);
    }
    
    if (profile1) {
        cmsCloseProfile(profile1);
    }
    
    if (profile2) {
        cmsCloseProfile(profile2);
    }
    
    if (sRGB_profile) {
        cmsCloseProfile(sRGB_profile);
    }
    
    cmsDeleteContext(context);
    
    // Close file streams
    assert_file_closed(&in_file1);;
    assert_file_closed(&in_file2);;
    
    assert_file_closed(&in_file2);
	assert_file_closed(&in_file1);
	return 0;
}