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
//<ID> 513
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 1
//<Quality> {"density":24,"unique_branches":{"AllocEmptyTransform":[[977,21,977,51,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDupContext","cmsDeleteContext","cmsDeleteContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsCreateLab2Profile","cmsCreate_sRGBProfileTHR","cmsCreateLab2Profile","cmsCreateXYZProfile","cmsCreateProofingTransform","cmsIsTag","cmsLinkTag","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsStageAllocCLutFloat","cmsStageAllocCLutFloatGranular","cmsStageFree","cmsStageFree","cmsGetPostScriptCRD","cmsPluginTHR","cmsFreeNamedColorList","cmsxyY2XYZ","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsDupContext","cmsOpenProfileFromFileTHR","cmsCreateLab2Profile","cmsCreate_sRGBProfileTHR","cmsCreateLab2Profile","cmsCreateXYZProfile","cmsCreateProofingTransform","cmsIsTag","cmsLinkTag","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsStageAllocCLutFloat","cmsStageAllocCLutFloatGranular","cmsStageFree","cmsStageFree","cmsGetPostScriptCRD","cmsPluginTHR","cmsFreeNamedColorList","cmsxyY2XYZ","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
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
    
    // 1. Create context and duplicate it
    cmsContext context1 = cmsCreateContext(nullptr, nullptr);
    if (!context1) {
	return 0;
	}
	
    
    cmsContext context2 = cmsDupContext(context1, nullptr);
    if (!context2) {
        cmsDeleteContext(context1);
        return 0;
    }
    
    // 2. Write input data to a file for profile loading
    FILE *profile_file = fopen("input_file", "wb");
    if (!profile_file) {
        cmsDeleteContext(context2);
        cmsDeleteContext(context1);
        assert_file_closed(&profile_file);
	return 0;
    }
    fwrite(data, 1, size, profile_file);
    assert_file_closed(&profile_file);;
    
    // 3. Open profile from the file (using duplicated context)
    cmsHPROFILE input_profile = cmsOpenProfileFromFileTHR(context2, "input_file", "r");
    if (!input_profile) {
        // If file isn't valid ICC, create synthetic profiles instead
        cmsCIExyY white_point = {0.3127, 0.3290, 1.0};
        input_profile = cmsCreateLab2Profile(&white_point);
    }
    
    // 4. Create standard profiles
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfileTHR(context1);
    cmsCIExyY d50 = {0.3457, 0.3585, 1.0};
    cmsHPROFILE lab_profile = cmsCreateLab2Profile(&d50);
    cmsHPROFILE xyz_profile = cmsCreateXYZProfile();
    
    // 5. Create proofing transform
    cmsHTRANSFORM transform = cmsCreateProofingTransform(
        input_profile, TYPE_RGB_8,      // Input: RGB 8-bit
        srgb_profile, TYPE_RGB_8,       // Output: sRGB 8-bit
        lab_profile,                    // Proofing: Lab profile
        INTENT_PERCEPTUAL,              // Rendering intent
        INTENT_ABSOLUTE_COLORIMETRIC,   // Proofing intent
        cmsFLAGS_GAMUTCHECK             // Flags
    );
    
    // 6. Link tags if input profile is valid
    if (input_profile && cmsIsTag(input_profile, cmsSigMediaWhitePointTag)) {
        cmsLinkTag(input_profile, cmsSigMediaWhitePointTag, cmsSigRedColorantTag);
    }
    
    // 7. Get and inspect transform pipeline
    if (transform) {
        cmsPipeline *pipeline = cmsGetTransformPipeline(transform);
        if (pipeline) {
            cmsStage *first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (first_stage) {
                cmsStageSignature stage_type = cmsStageType(first_stage);
                
                // 8. Create CLUT stages with data from input
                // Use first 24 floats from input (or default if not enough)
                cmsFloat32Number table[24];
                if (size >= sizeof(table)) {
                    memcpy(table, data, sizeof(table));
                } else {
                    for (int i = 0; i < 24; i++) table[i] = 0.5f;
                }
                
                cmsUInt32Number grid_points = 2;
                cmsStage *clut_float = cmsStageAllocCLutFloat(
                    context1, grid_points, 3, 3, table
                );
                
                cmsUInt32Number clut_points[3] = {2, 2, 2};
                cmsStage *clut_granular = cmsStageAllocCLutFloatGranular(
                    context1, clut_points, 3, 3, table
                );
                
                // Clean up CLUT stages
                if (clut_float) cmsStageFree(clut_float);
                if (clut_granular) cmsStageFree(clut_granular);
            }
        }
    }
    
    // 9. Generate PostScript CRD
    if (srgb_profile) {
        char buffer[1024];
        cmsUInt32Number written = cmsGetPostScriptCRD(
            context1, srgb_profile, INTENT_PERCEPTUAL, 0, buffer, sizeof(buffer)
        );
    }
    
    // 10. Register a plugin (dummy, just to call the API)
    cmsPluginTHR(context1, nullptr);
    
    // 11. Free named color list (dummy call)
    cmsFreeNamedColorList(nullptr);
    
    // 12. Color space conversion
    cmsCIExyY xyY = {0.3, 0.3, 1.0};
    cmsCIEXYZ xyz;
    cmsxyY2XYZ(&xyz, &xyY);
    
    // 13. Clean up resources
    if (transform) cmsDeleteTransform(transform);
    if (input_profile) cmsCloseProfile(input_profile);
    if (srgb_profile) cmsCloseProfile(srgb_profile);
    if (lab_profile) cmsCloseProfile(lab_profile);
    if (xyz_profile) cmsCloseProfile(xyz_profile);
    
    cmsDeleteContext(context2);
    cmsDeleteContext(context1);
    
    // Remove temporary file
    remove("input_file");
    
    assert_file_closed(&profile_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}