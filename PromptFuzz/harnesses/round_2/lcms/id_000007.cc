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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["cmsSetLogErrorHandler","cmsCreateContext","cmsOpenProfileFromFile","cmsCreateProfilePlaceholder","cmsSetProfileVersion","cmsSetDeviceClass","cmsSetEncodedICCversion","cmsCreateLab2ProfileTHR","cmsCreateLab2ProfileTHR","cmsCreateProfilePlaceholder","cmsGetTagOffsetAndSize","cmsCreateExtendedTransform","cmsGetTransformGamutCheckPipeline","cmsPipelineInputChannels","cmsPipelineSetSaveAs8bitsFlag","cmsFloat2LabEncoded","cmsDoTransform","cmsFloat2LabEncoded","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsSetLogErrorHandler","cmsCreateContext","cmsOpenProfileFromFile","cmsCreateProfilePlaceholder","cmsSetProfileVersion","cmsSetDeviceClass","cmsSetEncodedICCversion","cmsCreateLab2ProfileTHR","cmsCreateProfilePlaceholder","cmsGetTagOffsetAndSize","cmsCreateExtendedTransform","cmsGetTransformGamutCheckPipeline","cmsPipelineInputChannels","cmsPipelineSetSaveAs8bitsFlag","cmsFloat2LabEncoded","cmsDoTransform","cmsFloat2LabEncoded","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified LCMS APIs to achieve color management transformation:

*/


// Custom error handler function
static void  id_000007_custom_log_error_handler(cmsContext /*ContextID*/, cmsUInt32Number /*ErrorCode*/, const char* /*Text*/) {
    // Simple error handler that does nothing
    return;
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    cmsContext context = nullptr;
    cmsHPROFILE profile = nullptr;
    cmsHPROFILE gamutProfile = nullptr;
    cmsHTRANSFORM transform = nullptr;
    cmsPipeline* pipeline = nullptr;
    FILE* temp_file = nullptr;
    
    // 1. Set custom error handler
    cmsSetLogErrorHandler( id_000007_custom_log_error_handler);
    
    // 2. Create a context
    context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 3. Create profiles from input data
    // Use the input data as both source and destination profiles
    // First, create a temporary file with the input data
    temp_file = fopen("temp_profile.icc", "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
        
        // Open profile from file
        profile = cmsOpenProfileFromFile("temp_profile.icc", "r");
        
        // Clean up temp file
        remove("temp_profile.icc");
    }
    
    if (!profile) {
        // Fallback: create a placeholder profile
        profile = cmsCreateProfilePlaceholder(context);
        
        // Set some basic properties
        cmsSetProfileVersion(profile, 4.2);
        cmsSetDeviceClass(profile, cmsSigInputClass);
        cmsSetEncodedICCversion(profile, 0x04020000);
        
        // Create a simple Lab profile as gamut profile
        gamutProfile = cmsCreateLab2ProfileTHR(context, nullptr);
    } else {
        // Try to create a gamut profile from the same data
        gamutProfile = cmsCreateLab2ProfileTHR(context, nullptr);
    }
    
    if (!gamutProfile) {
        // Fallback gamut profile
        gamutProfile = cmsCreateProfilePlaceholder(context);
    }
    
    // 4. Get tag offset and size if profile is valid
    cmsUInt32Number offset = 0, tag_size = 0;
    if (profile) {
        // Try to get first tag's offset and size
        cmsGetTagOffsetAndSize(profile, 0, &offset, &tag_size);
    }
    
    // 5. Prepare parameters for extended transform
    cmsUInt32Number nProfiles = 2;
    cmsHPROFILE profiles[2] = {profile, profile}; // Use same profile for input and output
    cmsBool BPC[2] = {FALSE, FALSE};
    cmsUInt32Number Intents[2] = {INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC};
    cmsFloat64Number AdaptationStates[2] = {1.0, 1.0};
    
    // 6. Create extended transform
    // Use Lab_8 for both input and output formats (commonly supported)
    cmsUInt32Number InputFormat = TYPE_Lab_8;
    cmsUInt32Number OutputFormat = TYPE_Lab_8;
    cmsUInt32Number dwFlags = cmsFLAGS_NOCACHE | cmsFLAGS_NOOPTIMIZE;
    
    transform = cmsCreateExtendedTransform(
        context,
        nProfiles,
        profiles,
        BPC,
        Intents,
        AdaptationStates,
        gamutProfile,  // Use gamut profile
        0,             // Gamut PCS position
        InputFormat,
        OutputFormat,
        dwFlags
    );
    
    // 7. Get pipeline from transform and use pipeline-related functions
    if (transform) {
        pipeline = cmsGetTransformGamutCheckPipeline(transform);
        
        if (pipeline) {
            // Get input channels of pipeline
            cmsUInt32Number inputChannels = cmsPipelineInputChannels(pipeline);
            
            // Set save as 8 bits flag
            cmsPipelineSetSaveAs8bitsFlag(pipeline, TRUE);
            
            // Note: inputChannels is used to ensure the API is called
            (void)inputChannels; // Avoid unused variable warning
        }
    }
    
    // 8. Test Lab conversion
    if (transform) {
        // Prepare Lab data
        cmsCIELab lab_color;
        lab_color.L = 50.0;  // Lightness
        lab_color.a = 20.0;  // Green-Red
        lab_color.b = 40.0;  // Blue-Yellow
        
        cmsUInt16Number encoded_lab[3];
        
        // Convert float Lab to encoded Lab
        cmsFloat2LabEncoded(encoded_lab, &lab_color);
        
        // Test the transform with some sample data
        // Allocate buffers for transformation
        const size_t pixel_count = 4;
        cmsUInt8Number input_pixels[pixel_count * 3];  // 3 channels for Lab
        cmsUInt8Number output_pixels[pixel_count * 3];
        
        // Fill input with some test data (using part of the fuzzer input)
        size_t data_to_use = size < sizeof(input_pixels) ? size : sizeof(input_pixels);
        memcpy(input_pixels, data, data_to_use);
        
        // Apply the transform if we have one
        if (transform) {
            cmsDoTransform(transform, input_pixels, output_pixels, pixel_count);
        }
        
        // Convert another Lab value using the encoded function
        lab_color.L = 75.0;
        lab_color.a = -10.0;
        lab_color.b = 30.0;
        cmsFloat2LabEncoded(encoded_lab, &lab_color);
    }
    
    // 9. Cleanup
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (pipeline) {
        // Note: The pipeline is owned by the transform, so we don't free it separately
    }
    
    if (gamutProfile) {
        cmsCloseProfile(gamutProfile);
    }
    
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    assert_file_closed(&temp_file);
	return 0;
}