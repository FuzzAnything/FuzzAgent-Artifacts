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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["cmsCreateContext","cmsGetAlarmCodesTHR","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreate_sRGBProfile","cmsCreateMultiprofileTransform","cmsDoTransformLineStride","cmsStageAllocCLutFloatGranular","cmsWriteTag","cmsStageFree","cmsIT8LoadFromFile","cmsIT8SaveToFile","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsGetAlarmCodesTHR","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreate_sRGBProfile","cmsCreateMultiprofileTransform","cmsDoTransformLineStride","cmsStageAllocCLutFloatGranular","cmsWriteTag","cmsStageFree","cmsIT8LoadFromFile","cmsIT8SaveToFile","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    // Create temporary files for cube and IT8 data
    FILE *cube_file = fopen("temp_cube.cube", "wb");
    if (!cube_file) {
	assert_file_closed(&cube_file);
	return 0;
	}
	
    fwrite(data, 1, size / 2, cube_file); // Use first half for cube file
    assert_file_closed(&cube_file);;
    
    FILE *it8_file = fopen("temp_it8.it8", "wb");
    if (!it8_file) {
	assert_file_closed(&it8_file);
	assert_file_closed(&cube_file);
	return 0;
	}
	
    fwrite(data + size / 2, 1, size - size / 2, it8_file); // Use second half for IT8
    assert_file_closed(&it8_file);;
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // 1. Get alarm codes
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodesTHR(context, alarmCodes);
    
    // 2. Create device link profile from cube file
    cmsHPROFILE cubeProfile = cmsCreateDeviceLinkFromCubeFileTHR(context, "temp_cube.cube");
    
    if (cubeProfile) {
        // 3. Create sRGB profile for multiprofile transform
        cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfile();
        
        // Create array of profiles for multiprofile transform
        cmsHPROFILE profiles[2] = {sRGBProfile, cubeProfile};
        
        // 4. Create multiprofile transform
        cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(
            profiles, 2, TYPE_RGB_8, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
        
        if (transform) {
            // Prepare test data for transform
            const size_t pixels = 16;
            const size_t planes = 3;
            uint8_t input[pixels * planes];
            uint8_t output[pixels * planes];
            
            // Initialize with some test pattern
            for (size_t i = 0; i < sizeof(input); i++) {
                input[i] = static_cast<uint8_t>(i * 17);
            }
            
            // 5. Perform transform with line stride
            cmsDoTransformLineStride(
                transform, 
                input, 
                output, 
                pixels,    // PixelsPerLine
                1,         // LineCount
                pixels * planes,  // BytesPerLineIn
                pixels * planes,  // BytesPerLineOut
                pixels,    // BytesPerPlaneIn
                pixels     // BytesPerPlaneOut
            );
            
            // 6. Create a simple CLUT stage
            cmsUInt32Number clutPoints[] = {2, 2, 2}; // Small 2x2x2 CLUT
            cmsFloat32Number table[2 * 2 * 2 * 3];    // 8 entries × 3 output channels
            
            // Fill table with test data
            for (int i = 0; i < 24; i++) {
                table[i] = 0.5f;
            }
            
            cmsStage *clutStage = cmsStageAllocCLutFloatGranular(
                context, clutPoints, 3, 3, table);
            
            if (clutStage) {
                // 7. Write a tag to the cube profile
                // Create simple tag data (16 bytes for demonstration)
                uint8_t tagData[16];
                for (int i = 0; i < 16; i++) {
                    tagData[i] = static_cast<uint8_t>(i * 16);
                }
                
                // Try to write a copyright tag
                cmsWriteTag(cubeProfile, cmsSigCopyrightTag, tagData);
                
                // Clean up the stage
                cmsStageFree(clutStage);
            }
            
            // 8. Create and save an IT8 file
            // First, load an IT8 file to get a handle
            cmsHANDLE it8Handle = cmsIT8LoadFromFile(context, "temp_it8.it8");
            
            if (it8Handle) {
                // Save the IT8 data to a new file
                cmsIT8SaveToFile(it8Handle, "temp_it8_saved.it8");
                
                // Clean up IT8 handle
                cmsIT8Free(it8Handle);
            }
            
            // Clean up transform
            cmsDeleteTransform(transform);
        }
        
        // Clean up profiles
        if (sRGBProfile) cmsCloseProfile(sRGBProfile);
        cmsCloseProfile(cubeProfile);
    }
    
    // Clean up context
    cmsDeleteContext(context);
    
    // Clean up temporary files
    remove("temp_cube.cube");
    remove("temp_it8.it8");
    remove("temp_it8_saved.it8");
    
    assert_file_closed(&it8_file);
	assert_file_closed(&cube_file);
	return 0;
}