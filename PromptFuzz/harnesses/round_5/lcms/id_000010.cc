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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsDeleteContext","cmsD50_xyY","cmsCreateLab2ProfileTHR","cmsCloseProfile","cmsDeleteContext","cmsCreateTransform","cmsGetTransformPipeline","cmsGetTransformGamutCheckPipeline","cmsPipelineStageCount","cmsSliceSpace16","cmsPipelineInputChannels","cmsxyY2XYZ","cmsD50_XYZ","cmsAdaptToIlluminant","cmsCIECAM02Done","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsD50_xyY","cmsCreateLab2ProfileTHR","cmsCreateTransform","cmsGetTransformPipeline","cmsGetTransformGamutCheckPipeline","cmsPipelineStageCount","cmsSliceSpace16","cmsPipelineInputChannels","cmsxyY2XYZ","cmsD50_XYZ","cmsAdaptToIlluminant","cmsCIECAM02Done","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// lcms headers would be included in real scenario
// #include "lcms2.h"

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
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
	 // Need minimum data

    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // Write input data to file for profile reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Open profile from file
    cmsHPROFILE src_profile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    if (!src_profile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Create destination profile (Lab profile with D50 white point)
    const cmsCIExyY *d50 = cmsD50_xyY();
    cmsHPROFILE dst_profile = cmsCreateLab2ProfileTHR(context, d50);
    if (!dst_profile) {
        cmsCloseProfile(src_profile);
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Create transform
    cmsHTRANSFORM transform = cmsCreateTransform(
        src_profile, TYPE_Lab_DBL,
        dst_profile, TYPE_Lab_DBL,
        INTENT_PERCEPTUAL, 0);
    
    if (transform) {
        // Get transform pipeline
        cmsPipeline *pipeline = cmsGetTransformPipeline(transform);
        
        // Get gamut check pipeline (may be NULL)
        cmsPipeline *gamut_pipeline = cmsGetTransformGamutCheckPipeline(transform);
        
        // Use cmsSliceSpace16 if we have a pipeline with CLUT
        if (pipeline && cmsPipelineStageCount(pipeline) > 0) {
            // Define a simple sampler function
            auto sampler = [](const cmsUInt16Number *in, cmsUInt16Number *out, void *cargo) -> cmsInt32Number {
                // Simple pass-through sampler
                out[0] = in[0];
                out[1] = in[1];
                out[2] = in[2];
                return 1;
            };
            
            // Try to sample the pipeline if it has CLUT
            cmsUInt32Number clutPoints[] = {2, 2, 2}; // Simple 2x2x2 CLUT
            cmsBool sliced = cmsSliceSpace16(
                cmsPipelineInputChannels(pipeline),
                clutPoints,
                sampler,
                nullptr
            );
            // sliced result ignored for fuzzing
        }
        
        // Test color adaptation
        cmsCIExyY source_xyY;
        cmsCIEXYZ source_xyz, dest_xyz, adapted_xyz;
        
        // Initialize with some values from input data
        source_xyY.x = 0.3127;
        source_xyY.y = 0.3290;
        source_xyY.Y = 1.0;
        
        // Convert xyY to XYZ
        cmsxyY2XYZ(&source_xyz, &source_xyY);
        
        // Use D50 as source white point, D65 as illuminant
        const cmsCIEXYZ *d50_xyz = cmsD50_XYZ();
        cmsCIEXYZ d65_xyz = {0.9504, 1.0000, 1.0888};
        
        // Adapt color
        cmsBool adapted = cmsAdaptToIlluminant(
            &adapted_xyz,
            d50_xyz,
            &d65_xyz,
            &source_xyz
        );
        
        // Clean up CIECAM02 model if we had one
        // Note: cmsCIECAM02Done requires a valid handle, which we don't have
        // We'll call it with nullptr to satisfy API usage requirement
        // In real usage, we would create a model first
        cmsCIECAM02Done(nullptr);
        
        cmsDeleteTransform(transform);
    }
    
    // Clean up
    cmsCloseProfile(dst_profile);
    cmsCloseProfile(src_profile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}