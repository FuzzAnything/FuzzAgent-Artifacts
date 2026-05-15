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
//<ID> 405
//<Prompt> []
/*<Combination>: [
*/
//<score> 50, nr_unique_branch: 4
//<Quality> {"density":25,"unique_branches":{"cmsIT8EnumProperties":[[2687,32,2687,41,0,0,4,0],[2693,9,2693,22,0,0,4,0],[2697,33,2697,42,0,0,4,0],[2697,33,2697,42,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsCreateXYZProfileTHR","cmsCreateLab2ProfileTHR","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyUncooked","cmsIT8SaveToFile","cmsIT8EnumProperties","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsPipelineDup","cmsXYZEncoded2Float","cmsDoTransformStride","cmsPipelineFree","cmsPipelineFree","cmsAllocNamedColorList","cmsNamedColorInfo","cmsFreeNamedColorList","cmsSetHeaderFlags","cmsOpenIOhandlerFromFile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsCloseIOhandler","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsCreateXYZProfileTHR","cmsCreateLab2ProfileTHR","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyUncooked","cmsIT8SaveToFile","cmsIT8EnumProperties","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsPipelineDup","cmsXYZEncoded2Float","cmsDoTransformStride","cmsPipelineFree","cmsPipelineFree","cmsAllocNamedColorList","cmsNamedColorInfo","cmsFreeNamedColorList","cmsSetHeaderFlags","cmsOpenIOhandlerFromFile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsCloseIOhandler","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
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
	
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // Write input data to a file for profile reading
    FILE *profile_file = fmemopen((void *)data, size, "rb");
    if (!profile_file) {
        cmsDeleteContext(context);
        assert_file_closed(&profile_file);
	return 0;
    }
    
    // Create input profile from file
    cmsHPROFILE input_profile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    
    // Create XYZ profile
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(context);
    
    // Create Lab2 profile with default white point
    cmsCIExyY d50_white = {0.3457, 0.3585, 1.0};
    cmsHPROFILE lab_profile = cmsCreateLab2ProfileTHR(context, &d50_white);
    
    // Create IT8 handle and set properties
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (it8) {
        cmsIT8SetSheetType(it8, "CGATS.17");
        
        // Use a portion of input data for IT8 property
        size_t prop_len = size > 100 ? 100 : size;
        char prop_buffer[101];
        memcpy(prop_buffer, data, prop_len);
        prop_buffer[prop_len] = '\0';
        cmsIT8SetPropertyUncooked(it8, "DESCRIPTOR", prop_buffer);
        
        // Save IT8 to file
        cmsIT8SaveToFile(it8, "output_it8.txt");
        
        // Enumerate properties
        char **prop_names = NULL;
        cmsUInt32Number prop_count = cmsIT8EnumProperties(it8, &prop_names);
    }
    
    // Create multiprofile transform
    cmsHPROFILE profiles[3];
    profiles[0] = input_profile ? input_profile : xyz_profile;
    profiles[1] = xyz_profile;
    profiles[2] = lab_profile;
    
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
        context, 
        profiles, 
        3, 
        TYPE_RGB_8, 
        TYPE_Lab_8, 
        INTENT_PERCEPTUAL, 
        cmsFLAGS_NOCACHE
    );
    
    // Create and use pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        cmsPipeline *pipeline_dup = cmsPipelineDup(pipeline);
        
        // Test XYZ encoded conversion
        cmsUInt16Number xyz_encoded[3] = {0x8000, 0x8000, 0x8000};
        cmsCIEXYZ xyz_float;
        cmsXYZEncoded2Float(&xyz_float, xyz_encoded);
        
        // Use transform if created successfully
        if (transform) {
            // Prepare sample data from input
            size_t sample_size = size > 1000 ? 1000 : size;
            uint8_t *input_buffer = (uint8_t *)malloc(sample_size);
            uint8_t *output_buffer = (uint8_t *)malloc(sample_size);
            
            if (input_buffer && output_buffer) {
                memcpy(input_buffer, data, sample_size);
                
                // Perform transformation with stride
                cmsDoTransformStride(
                    transform, 
                    input_buffer, 
                    output_buffer, 
                    sample_size / 12,  // Process as 4-byte pixels
                    12                  // Stride of 12 bytes (3 channels * 4 bytes)
                );
            }
            
            free(input_buffer);
            free(output_buffer);
        }
        
        cmsPipelineFree(pipeline_dup);
        cmsPipelineFree(pipeline);
    }
    
    // Create named color list
    cmsNAMEDCOLORLIST *named_color_list = cmsAllocNamedColorList(
        context, 
        10, 
        3, 
        "Prefix", 
        "Suffix"
    );
    
    if (named_color_list) {
        // Query named color info
        char name[256], prefix[256], suffix[256];
        cmsUInt16Number pcs[3], colorant[3];
        
        cmsNamedColorInfo(
            named_color_list, 
            0, 
            name, 
            prefix, 
            suffix, 
            pcs, 
            colorant
        );
        
        cmsFreeNamedColorList(named_color_list);
    }
    
    // Set header flags if profile exists
    if (input_profile) {
        cmsSetHeaderFlags(input_profile, 0);
    }
    
    // Open IO handler for file operations
    cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromFile(
        context, 
        "input_file", 
        "r"
    );
    
    // Clean up all resources
    if (transform) cmsDeleteTransform(transform);
    if (input_profile) cmsCloseProfile(input_profile);
    if (xyz_profile) cmsCloseProfile(xyz_profile);
    if (lab_profile) cmsCloseProfile(lab_profile);
    if (it8) cmsIT8Free(it8);
    if (io_handler) cmsCloseIOhandler(io_handler);
    if (profile_file) assert_file_closed(&profile_file);;
    
    cmsDeleteContext(context);
    
    // Clean up temporary files
    remove("input_file");
    remove("output_it8.txt");
    remove("output_file");
    
    assert_file_closed(&profile_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}