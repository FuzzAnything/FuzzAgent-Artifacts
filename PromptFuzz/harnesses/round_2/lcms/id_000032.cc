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
//<ID> 218
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 4
//<Quality> {"density":14,"unique_branches":{"cmsIT8SaveToFile":[[2023,9,2023,19,0,0,4,1],[2025,15,2025,36,0,0,4,0],[2029,13,2029,40,0,0,4,1],[2035,13,2035,28,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsOpenProfileFromMemTHR","cmsReadRawTag","cmsIT8LoadFromFile","cmsIT8SetPropertyUncooked","cmsIT8GetPropertyMulti","cmsIT8SaveToFile","cmsCreate_sRGBProfileTHR","cmsCreateMultiprofileTransformTHR","cmsGetTransformGamutCheckPipeline","cmsCloseProfile","cmsSetAdaptationState","cmsLabEncoded2Float","cmsDoTransform","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsReadRawTag","cmsIT8LoadFromFile","cmsIT8SetPropertyUncooked","cmsIT8GetPropertyMulti","cmsIT8SaveToFile","cmsCreate_sRGBProfileTHR","cmsCreateMultiprofileTransformTHR","cmsGetTransformGamutCheckPipeline","cmsCloseProfile","cmsSetAdaptationState","cmsLabEncoded2Float","cmsDoTransform","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
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
	 // Minimum size for meaningful processing
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // Use fmemopen to read input data as FILE*
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for IT8 operations
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // API 1: cmsReadRawTag - Read raw tag from a profile
    // First create a profile from memory
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(context, data, size);
    if (profile) {
        // Read raw tag - try reading profile description tag
        char tagBuffer[256];
        cmsUInt32Number bytesRead = cmsReadRawTag(profile, cmsSigProfileDescriptionTag, 
                                                  tagBuffer, sizeof(tagBuffer));
        (void)bytesRead; // Use bytesRead to avoid unused variable warning
    }
    
    // API 2 & 7: IT8 operations - Create IT8 handle and set/get properties
    cmsHANDLE it8 = cmsIT8LoadFromFile(context, "input_file"); // Using file name directly
    if (it8) {
        // Set property using uncooked data
        const char* sampleData = "SampleData";
        cmsIT8SetPropertyUncooked(it8, "KEY1", sampleData);
        
        // Get property multi
        const char* value = cmsIT8GetPropertyMulti(it8, "KEY1", NULL);
        (void)value; // Use value to avoid unused variable warning
        
        // Save IT8 to file
        cmsIT8SaveToFile(it8, "output_file");
    }
    
    // API 3: Create multiprofile transform
    cmsHTRANSFORM transform = NULL;
    if (profile) {
        // Create sRGB profile as second profile
        cmsHPROFILE srgbProfile = cmsCreate_sRGBProfileTHR(context);
        if (srgbProfile) {
            cmsHPROFILE profiles[2] = {profile, srgbProfile};
            
            // Create multiprofile transform
            transform = cmsCreateMultiprofileTransformTHR(context, profiles, 2,
                                                         TYPE_RGB_8, TYPE_RGB_8,
                                                         INTENT_PERCEPTUAL, 0);
            
            // API 4: Get gamut check pipeline
            if (transform) {
                cmsPipeline* gamutPipeline = cmsGetTransformGamutCheckPipeline(transform);
                (void)gamutPipeline; // Use gamutPipeline to avoid unused variable warning
            }
            
            cmsCloseProfile(srgbProfile);
        }
    }
    
    // API 5: Set adaptation state
    cmsFloat64Number oldState = cmsSetAdaptationState(0.5);
    (void)oldState; // Use oldState to avoid unused variable warning
    
    // API 6: Lab encoded to float conversion
    cmsUInt16Number encodedLab[3] = {0x8000, 0x8000, 0x8000}; // Neutral gray
    cmsCIELab labFloat;
    cmsLabEncoded2Float(&labFloat, encodedLab);
    
    // Process data through transform if available
    if (transform) {
        unsigned char input[3] = {128, 128, 128};
        unsigned char output[3];
        cmsDoTransform(transform, input, output, 1);
        
        // Clean up transform
        cmsDeleteTransform(transform);
    }
    
    // Clean up resources
    if (it8) {
        cmsIT8Free(it8);
    }
    
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}