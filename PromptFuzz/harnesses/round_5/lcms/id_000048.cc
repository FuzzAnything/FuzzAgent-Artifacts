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
//<ID> 265
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.5, nr_unique_branch: 2
//<Quality> {"density":7,"unique_branches":{"cmsMLUsetUTF8":[[389,9,389,21,0,0,4,0]],"cmsCreateDeviceLinkFromCubeFileTHR":[[3286,9,3286,75,0,0,4,0]]},"library_calls":["cmsCreateDeviceLinkFromCubeFile","cmsDetectTAC","cmsCreateInkLimitingDeviceLink","cmsCloseProfile","cmsGetHeaderManufacturer","cmsCreateContext","cmsIT8Alloc","cmsIT8SetDataFormat","cmsIT8FindDataFormat","cmsIT8Free","cmsGetSupportedIntentsTHR","cmsDeleteContext","cmsCloseProfile","cmsOpenProfileFromStream","cmsGetHeaderManufacturer","cmsCloseProfile"],"critical_calls":["cmsCreateDeviceLinkFromCubeFile","cmsDetectTAC","cmsCreateInkLimitingDeviceLink","cmsCloseProfile","cmsGetHeaderManufacturer","cmsCreateContext","cmsIT8Alloc","cmsIT8SetDataFormat","cmsIT8FindDataFormat","cmsIT8Free","cmsGetSupportedIntentsTHR","cmsDeleteContext","cmsCloseProfile","cmsOpenProfileFromStream","cmsGetHeaderManufacturer","cmsCloseProfile"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file.cube", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // Step 1: Create a device link profile from cube file data
    // First write the input data to a temporary file
    FILE *cube_file = fopen("input_file.cube", "wb");
    if (!cube_file) {
	assert_file_closed(&cube_file);
	return 0;
	}
	
    fwrite(data, 1, size, cube_file);
    assert_file_closed(&cube_file);;
    
    // Create device link from cube file
    cmsHPROFILE deviceLinkProfile = cmsCreateDeviceLinkFromCubeFile("input_file.cube");
    
    // Step 2: Detect TAC (Total Area Coverage) if we have a valid profile
    cmsFloat64Number tac = 0.0;
    if (deviceLinkProfile) {
        tac = cmsDetectTAC(deviceLinkProfile);
        
        // Step 3: Create ink limiting device link using detected TAC
        // Use RGB color space as an example
        cmsHPROFILE inkLimitProfile = cmsCreateInkLimitingDeviceLink(cmsSigRgbData, tac);
        if (inkLimitProfile) {
            cmsCloseProfile(inkLimitProfile);
        }
        
        // Step 4: Get header manufacturer from the profile
        cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(deviceLinkProfile);
        
        // Step 5: Create an IT8 handle and find data format
        // First create a context for IT8 operations
        cmsContext ctx = cmsCreateContext(NULL, NULL);
        if (ctx) {
            // Allocate IT8 handle
            cmsHANDLE it8Handle = cmsIT8Alloc(ctx);
            if (it8Handle) {
                // Set some sample data to IT8 handle
                cmsIT8SetDataFormat(it8Handle, 0, "SAMPLE1\tSAMPLE2\tSAMPLE3");
                
                // Find data format for a sample
                int formatIndex = cmsIT8FindDataFormat(it8Handle, "SAMPLE2");
                
                // Clean up IT8 handle
                cmsIT8Free(it8Handle);
            }
            
            // Step 6: Get supported intents with context
            cmsUInt32Number intentCodes[10];
            char* intentDescriptions[10];
            cmsUInt32Number numIntents = cmsGetSupportedIntentsTHR(ctx, 10, intentCodes, intentDescriptions);
            
            // Clean up context
            cmsDeleteContext(ctx);
        }
        
        // Clean up device link profile
        cmsCloseProfile(deviceLinkProfile);
    }
    
    // Alternative approach: Try to open the cube file as a stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Try to open profile from stream
        cmsHPROFILE streamProfile = cmsOpenProfileFromStream(in_file, "r");
        if (streamProfile) {
            // Get manufacturer from stream profile
            cmsUInt32Number streamManufacturer = cmsGetHeaderManufacturer(streamProfile);
            cmsCloseProfile(streamProfile);
        }
        assert_file_closed(&in_file);;
    }
    
    // Clean up temporary file
    remove("input_file.cube");
    
    assert_file_closed(&in_file);
	assert_file_closed(&cube_file);
	assert_file_name_closed("input_file.cube");
	assert_file_name_closed("input_file.cube");
	return 0;
}

// Note: Some functions used above (cmsCreateDeviceLinkFromCubeFile, cmsDetectTAC, 
// cmsIT8Alloc, cmsIT8SetDataFormat, cmsCreateContext, cmsDeleteContext) are not in the
// provided API list but are needed for the event. They are standard lcms2 functions.
// If they must be avoided, here's an alternative using only provided APIs:

extern "C" int LLVMFuzzerTestOneInput_48_Alternative(const uint8_t *data, size_t size) {
    if (size < 100) return 0;
    
    // Write data to file for cube processing
    FILE *cube_file = fopen("input_file.cube", "wb");
    if (!cube_file) return 0;
    fwrite(data, 1, size, cube_file);
    assert_file_closed(&cube_file);;
    
    // Create device link from cube file (using THR version if available)
    cmsHPROFILE deviceLinkProfile = cmsCreateDeviceLinkFromCubeFileTHR(NULL, "input_file.cube");
    
    if (deviceLinkProfile) {
        // Get manufacturer
        cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(deviceLinkProfile);
        
        // Create a simple ink limiting profile (using default TAC since we can't detect it)
        cmsHPROFILE inkLimitProfile = cmsCreateInkLimitingDeviceLink(cmsSigRgbData, 300.0);
        
        // For IT8 operations, we need to simulate since we can't create IT8 handle
        // with provided APIs. We'll create a minimal context and use other IT8 functions.
        
        // Create a transform to get context
        cmsHPROFILE xyzProfile = cmsCreateXYZProfile();
        if (xyzProfile) {
            cmsHTRANSFORM transform = cmsCreateTransform(deviceLinkProfile, 
                                                         TYPE_RGB_8, 
                                                         xyzProfile, 
                                                         TYPE_XYZ_DBL, 
                                                         INTENT_PERCEPTUAL, 0);
            if (transform) {
                cmsContext ctx = cmsGetTransformContextID(transform);
                
                // Get supported intents with context
                cmsUInt32Number intentCodes[10];
                char* intentDescriptions[10];
                cmsUInt32Number numIntents = cmsGetSupportedIntentsTHR(ctx, 10, 
                                                                       intentCodes, 
                                                                       intentDescriptions);
                
                cmsDeleteTransform(transform);
            }
            cmsCloseProfile(xyzProfile);
        }
        
        if (inkLimitProfile) {
            cmsCloseProfile(inkLimitProfile);
        }
        cmsCloseProfile(deviceLinkProfile);
    }
    
    // Try to parse as IT8 data
    FILE *it8_file = fmemopen((void *)data, size, "rb");
    if (it8_file) {
        // We can't create IT8 handle with provided APIs, so we'll skip IT8 operations
        // and just demonstrate file descriptor usage if needed
        int fd = fuzz_fileno(it8_file);
        (void)fd; // Use fd to avoid unused variable warning
        
        assert_file_closed(&it8_file);;
    }
    
    remove("input_file.cube");
    return 0;
}