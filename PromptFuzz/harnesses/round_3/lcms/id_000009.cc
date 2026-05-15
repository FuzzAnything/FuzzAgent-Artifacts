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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 13.5, nr_unique_branch: 2
//<Quality> {"density":18,"unique_branches":{"CachedXFORM":[[538,17,538,71,0,0,4,0]],"cmsIT8EnumProperties":[[2693,9,2693,22,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsCreateXYZProfileTHR","cmsCreateLab2ProfileTHR","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsPipelineDup","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SaveToFile","cmsOpenIOhandlerFromFile","cmsAllocNamedColorList","cmsNamedColorInfo","cmsIT8EnumProperties","cmsDoTransformStride","cmsCloseIOhandler","cmsFreeNamedColorList","cmsIT8Free","cmsPipelineFree","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsCreateXYZProfileTHR","cmsCreateLab2ProfileTHR","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsPipelineDup","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SaveToFile","cmsOpenIOhandlerFromFile","cmsAllocNamedColorList","cmsNamedColorInfo","cmsIT8EnumProperties","cmsDoTransformStride","cmsCloseIOhandler","cmsFreeNamedColorList","cmsIT8Free","cmsPipelineFree","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":3}
/**/


// Include lcms2 headers

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
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
	 // Need minimum data for meaningful operations

    // Create a temporary file with input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Open profile from file using the input data
    cmsHPROFILE profile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    if (!profile) {
        // Create a simple profile for testing
        profile = cmsCreateXYZProfileTHR(context);
    }

    // 2. Create multiprofile transform
    cmsHPROFILE profiles[2] = {profile, cmsCreateLab2ProfileTHR(context, NULL)};
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
        context, profiles, 2, TYPE_RGB_8, TYPE_Lab_8, INTENT_PERCEPTUAL, 0);
    
    // 3. Duplicate pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    cmsPipeline *dup_pipeline = cmsPipelineDup(pipeline);
    
    // 4. Create IT8 handle and save to file
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (it8) {
        cmsIT8SetSheetType(it8, "CGATS.17");
        cmsIT8SaveToFile(it8, "it8_output.txt");
    }
    
    // 5. Open IO handler from file
    cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromFile(context, "input_file", "r");
    
    // 6. Use named color info
    cmsNAMEDCOLORLIST *named_color_list = cmsAllocNamedColorList(context, 10, 3, "Prefix", "Suffix");
    if (named_color_list) {
        char name[256], prefix[256], suffix[256];
        cmsUInt16Number pcs[3], colorant[3];
        cmsNamedColorInfo(named_color_list, 0, name, prefix, suffix, pcs, colorant);
    }
    
    // 7. Enumerate IT8 properties
    char **property_names = NULL;
    if (it8) {
        cmsUInt32Number prop_count = cmsIT8EnumProperties(it8, &property_names);
        // Free property names if allocated
        if (property_names) {
            for (cmsUInt32Number i = 0; i < prop_count; i++) {
                free(property_names[i]);
            }
            free(property_names);
        }
    }
    
    // 8. Perform transformation using input data
    if (transform) {
        // Use a portion of input data as transformation buffer
        size_t buffer_size = (size > 1024) ? 1024 : size;
        uint8_t *input_buffer = (uint8_t*)malloc(buffer_size);
        uint8_t *output_buffer = (uint8_t*)malloc(buffer_size);
        
        if (input_buffer && output_buffer) {
            memcpy(input_buffer, data, buffer_size);
            
            // Perform transformation with stride
            cmsDoTransformStride(transform, input_buffer, output_buffer, 
                                buffer_size / 3, 3); // Assuming RGB 8-bit
            
            // Write output to file
            fwrite(output_buffer, 1, buffer_size, out_file);
        }
        
        free(input_buffer);
        free(output_buffer);
    }
    
    // Clean up
    if (io_handler) cmsCloseIOhandler(io_handler);
    if (named_color_list) cmsFreeNamedColorList(named_color_list);
    if (it8) cmsIT8Free(it8);
    if (dup_pipeline) cmsPipelineFree(dup_pipeline);
    if (pipeline) cmsPipelineFree(pipeline);
    if (transform) cmsDeleteTransform(transform);
    if (profiles[1]) cmsCloseProfile(profiles[1]);
    if (profile) cmsCloseProfile(profile);
    
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("output_file");
    remove("it8_output.txt");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}