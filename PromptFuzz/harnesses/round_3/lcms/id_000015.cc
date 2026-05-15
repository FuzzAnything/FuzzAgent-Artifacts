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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 69, nr_unique_branch: 2
//<Quality> {"density":23,"unique_branches":{"cmsCreateExtendedTransform":[[1168,9,1168,86,0,0,4,0]],"GetXFormColorSpaces":[[1023,9,1023,29,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenIOhandlerFromFile","cmsOpenProfileFromFileTHR","cmsCreate_sRGBProfileTHR","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsD50_XYZ","cmsPipelineDup","cmsPipelineFree","cmsIT8Alloc","cmsIT8SaveToFile","cmsPipelineGetPtrToFirstStage","cmsStageNext","cmsIT8EnumProperties","cmsIT8GetProperty","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsDoTransformStride","cmsCloseIOhandler","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsPipelineFree","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenIOhandlerFromFile","cmsOpenProfileFromFileTHR","cmsCreate_sRGBProfileTHR","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsD50_XYZ","cmsPipelineDup","cmsPipelineFree","cmsIT8Alloc","cmsIT8SaveToFile","cmsPipelineGetPtrToFirstStage","cmsStageNext","cmsIT8EnumProperties","cmsIT8GetProperty","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsDoTransformStride","cmsCloseIOhandler","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsPipelineFree","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 4) {
        return 0;
    }

    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }

    // Write input data to a temporary file for profile loading
    FILE *profile_file = fmemopen((void *)data, size, "rb");
    if (!profile_file) {
        cmsDeleteContext(context);
        assert_file_closed(&profile_file);
	return 0;
    }

    // Open profile from file using the context
    cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromFile(context, "input_file", "rb");
    cmsHPROFILE input_profile = cmsOpenProfileFromFileTHR(context, "input_file", "rb");
    
    // Create sRGB profile
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfileTHR(context);
    
    // Create multiprofile transform
    cmsHPROFILE profiles[] = {input_profile, srgb_profile};
    cmsUInt32Number input_format = TYPE_RGB_8;
    cmsUInt32Number output_format = TYPE_RGB_8;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = 0;
    
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
        context, profiles, 2, input_format, output_format, intent, flags);
    
    // Allocate pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    
    // Get D50 XYZ white point
    const cmsCIEXYZ *d50 = cmsD50_XYZ();
    
    // Duplicate pipeline
    cmsPipeline *dup_pipeline = cmsPipelineDup(pipeline);
    
    // Free duplicated pipeline
    cmsPipelineFree(dup_pipeline);
    
    // Allocate IT8 handle
    cmsHANDLE it8 = cmsIT8Alloc(context);
    
    // Save IT8 to file
    cmsIT8SaveToFile(it8, "output_file");
    
    // Iterate through pipeline stages (if pipeline exists)
    if (pipeline) {
        cmsStage *stage = cmsPipelineGetPtrToFirstStage(pipeline);
        while (stage) {
            stage = cmsStageNext(stage);
            break; // Just demonstrate one iteration
        }
    }
    
    // Enumerate IT8 properties
    char **property_names = nullptr;
    cmsUInt32Number prop_count = cmsIT8EnumProperties(it8, &property_names);
    
    // Get IT8 property
    const char *prop_value = cmsIT8GetProperty(it8, "Sample");
    
    // Get named color list from transform
    cmsNAMEDCOLORLIST *named_color_list = cmsGetNamedColorList(transform);
    
    // Get named color count
    cmsUInt32Number color_count = 0;
    if (named_color_list) {
        color_count = cmsNamedColorCount(named_color_list);
    }
    
    // Get named color info (if colors exist)
    if (color_count > 0) {
        char name[256];
        char prefix[256];
        char suffix[256];
        cmsUInt16Number pcs[3];
        cmsUInt16Number colorant[16];
        
        cmsNamedColorInfo(named_color_list, 0, name, prefix, suffix, pcs, colorant);
    }
    
    // Perform transform on sample data
    if (transform) {
        unsigned char input_rgb[3] = {128, 128, 128};
        unsigned char output_rgb[3];
        cmsDoTransformStride(transform, input_rgb, output_rgb, 1, 3);
    }
    
    // Cleanup
    if (io_handler) {
        cmsCloseIOhandler(io_handler);
    }
    
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (input_profile) {
        cmsCloseProfile(input_profile);
    }
    
    if (srgb_profile) {
        cmsCloseProfile(srgb_profile);
    }
    
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (it8) {
        cmsIT8Free(it8);
    }
    
    cmsDeleteContext(context);
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&profile_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}