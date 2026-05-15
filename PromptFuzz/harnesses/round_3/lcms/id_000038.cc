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
//<ID> 257
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 0
//<Quality> {"density":33,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsD50_xyY","cmsCreateLab2ProfileTHR","cmsCreateXYZProfileTHR","cmsDetectDestinationBlackPoint","cmsCreateMultiprofileTransformTHR","cmsOpenIOhandlerFromStream","cmsCloseIOhandler","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsPipelineAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsMLUfree","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SaveToFile","cmsIT8EnumProperties","cmsIT8Free","cmsAllocNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsFreeNamedColorList","cmsDoTransformStride","cmsDeleteTransform","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsD50_xyY","cmsCreateLab2ProfileTHR","cmsCreateXYZProfileTHR","cmsDetectDestinationBlackPoint","cmsCreateMultiprofileTransformTHR","cmsOpenIOhandlerFromStream","cmsCloseIOhandler","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsPipelineAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsMLUfree","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SaveToFile","cmsIT8EnumProperties","cmsIT8Free","cmsAllocNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsFreeNamedColorList","cmsDoTransformStride","cmsDeleteTransform","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
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
	 // Need minimum data for operations
    
    // Step 1: Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // Step 2: Write input data to a file for profile loading
    FILE *input_file = fopen("input_file", "wb");
    if (!input_file) {
        cmsDeleteContext(context);
        assert_file_closed(&input_file);
	return 0;
    }
    fwrite(data, 1, size, input_file);
    assert_file_closed(&input_file);;
    
    // Step 3: Open profile from file
    cmsHPROFILE input_profile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    
    // Step 4: Create Lab2 profile with D50 white point
    const cmsCIExyY* d50_white = cmsD50_xyY();
    cmsHPROFILE lab_profile = cmsCreateLab2ProfileTHR(context, d50_white);
    
    // Step 5: Create XYZ profile
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(context);
    
    // Step 6: Detect destination black point
    cmsCIEXYZ black_point;
    if (input_profile) {
        cmsDetectDestinationBlackPoint(&black_point, input_profile, INTENT_PERCEPTUAL, 0);
    }
    
    // Step 7: Create multi-profile transform
    cmsHPROFILE profiles[3];
    int profile_count = 0;
    
    if (input_profile) profiles[profile_count++] = input_profile;
    if (lab_profile) profiles[profile_count++] = lab_profile;
    if (xyz_profile) profiles[profile_count++] = xyz_profile;
    
    cmsHTRANSFORM transform = NULL;
    if (profile_count > 0) {
        transform = cmsCreateMultiprofileTransformTHR(context, profiles, profile_count, 
                                                     TYPE_RGB_8, TYPE_RGB_8, 
                                                     INTENT_PERCEPTUAL, 0);
    }
    
    // Step 8: Create I/O handler from stream using fmemopen
    FILE *in_stream = fmemopen((void *)data, size, "rb");
    if (in_stream) {
        cmsIOHANDLER *stream_handler = cmsOpenIOhandlerFromStream(context, in_stream);
        if (stream_handler) {
            cmsCloseIOhandler(stream_handler);
        }
        assert_file_closed(&in_stream);;
    }
    
    // Step 9: Create I/O handler from file
    cmsIOHANDLER *file_handler = cmsOpenIOhandlerFromFile(context, "input_file", "r");
    if (file_handler) {
        cmsCloseIOhandler(file_handler);
    }
    
    // Step 10: Create pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3); // RGB to RGB
    
    // Step 11: Create and test MLU
    cmsMLU *mlu1 = cmsMLUalloc(context, 10);
    cmsMLU *mlu2 = cmsMLUalloc(context, 5);
    
    if (mlu1) {
        char lang[3] = {'e', 'n', '\0'};
        char country[3] = {'U', 'S', '\0'};
        char obtained_lang[3], obtained_country[3];
        cmsMLUgetTranslation(mlu1, lang, country, obtained_lang, obtained_country);
        cmsMLUfree(mlu1);
    }
    if (mlu2) cmsMLUfree(mlu2);
    
    // Step 12: Create and test IT8
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (it8) {
        cmsIT8SetSheetType(it8, "CGATS.17");
        cmsIT8SaveToFile(it8, "it8_output.txt");
        
        // Test enum properties
        char **prop_names;
        cmsUInt32Number num_props = cmsIT8EnumProperties(it8, &prop_names);
        cmsIT8Free(it8);
    }
    
    // Step 13: Create and test named color list
    cmsNAMEDCOLORLIST *named_colors = cmsAllocNamedColorList(context, 5, 3, "Prefix", "Suffix");
    if (named_colors) {
        char name[256], prefix[256], suffix[256];
        cmsUInt16Number pcs[3], colorant[3];
        
        if (cmsNamedColorCount(named_colors) > 0) {
            cmsNamedColorInfo(named_colors, 0, name, prefix, suffix, pcs, colorant);
        }
        cmsFreeNamedColorList(named_colors);
    }
    
    // Step 14: Test transform if available
    if (transform) {
        uint8_t input_buffer[9] = {100, 150, 200, 50, 100, 150, 200, 50, 100};
        uint8_t output_buffer[9];
        
        cmsDoTransformStride(transform, input_buffer, output_buffer, 3, 3); // 3 pixels, stride=3
        cmsDeleteTransform(transform);
    }
    
    // Step 15: Clean up pipeline
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    // Step 16: Close profiles
    if (input_profile) cmsCloseProfile(input_profile);
    if (lab_profile) cmsCloseProfile(lab_profile);
    if (xyz_profile) cmsCloseProfile(xyz_profile);
    
    // Step 17: Delete context
    cmsDeleteContext(context);
    
    // Clean up temporary files
    remove("input_file");
    remove("it8_output.txt");
    
    assert_file_closed(&in_stream);
	assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}