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
//<ID> 365
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.3333335, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDupContext","cmsOpenProfileFromFileTHR","cmsCreate_sRGBProfileTHR","cmsCreateLab2Profile","cmsCreateXYZProfile","cmsCreateProofingTransform","cmsLinkTag","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsStageAllocCLutFloat","cmsStageAllocCLutFloatGranular","cmsStageFree","cmsStageFree","cmsReadTag","cmsDupProfileSequenceDescription","cmsxyY2XYZ","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsCreateContext","cmsDupContext","cmsOpenProfileFromFileTHR","cmsCreate_sRGBProfileTHR","cmsCreateLab2Profile","cmsCreateXYZProfile","cmsCreateProofingTransform","cmsLinkTag","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsStageAllocCLutFloat","cmsStageAllocCLutFloatGranular","cmsStageFree","cmsStageFree","cmsReadTag","cmsDupProfileSequenceDescription","cmsxyY2XYZ","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
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
	  // Need enough data for meaningful processing

    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create context and duplicate it
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    cmsContext dup_ctx = cmsDupContext(ctx, nullptr);
    
    // Read profile from input data
    cmsHPROFILE input_profile = cmsOpenProfileFromFileTHR(dup_ctx, "input_file", "r");
    if (!input_profile) {
        // Create fallback profiles if reading fails
        input_profile = cmsCreate_sRGBProfileTHR(dup_ctx);
        cmsHPROFILE output_profile = cmsCreateLab2Profile(nullptr);
        cmsHPROFILE proofing_profile = cmsCreateXYZProfile();
        
        // Create proofing transform
        cmsHTRANSFORM transform = cmsCreateProofingTransform(
            input_profile, TYPE_RGB_8,
            output_profile, TYPE_Lab_8,
            proofing_profile, INTENT_PERCEPTUAL, INTENT_ABSOLUTE_COLORIMETRIC, 0);
        
        if (transform) {
            // Link a tag if possible
            if (cmsLinkTag(input_profile, cmsSigMediaWhitePointTag, cmsSigMediaBlackPointTag)) {
                // Tag linking successful
            }
            
            // Get pipeline and check stage types
            cmsPipeline *pipeline = cmsGetTransformPipeline(transform);
            if (pipeline) {
                const cmsStage *first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
                if (first_stage) {
                    cmsStageType(first_stage);  // Check stage type
                }
                
                // Create CLUT stages with data from input
                if (size >= 100 + 8*8*8*3*4) {  // Check if we have enough data for CLUT
                    // Prepare table data from input
                    cmsFloat32Number table1[8*8*8*3];
                    cmsFloat32Number table2[8*8*8*3];
                    
                    // Use data for CLUT tables (skip first 100 bytes for safety)
                    size_t offset = 100;
                    size_t table_size = sizeof(cmsFloat32Number) * 8*8*8*3;
                    
                    if (size >= offset + 2*table_size) {
                        memcpy(table1, data + offset, table_size);
                        memcpy(table2, data + offset + table_size, table_size);
                        
                        // Create CLUT stages
                        cmsUInt32Number grid_points = 8;
                        cmsStage *clut_stage1 = cmsStageAllocCLutFloat(
                            dup_ctx, grid_points, 3, 3, table1);
                        
                        cmsUInt32Number granular_points[] = {8, 8, 8};
                        cmsStage *clut_stage2 = cmsStageAllocCLutFloatGranular(
                            dup_ctx, granular_points, 3, 3, table2);
                        
                        // Clean up CLUT stages
                        if (clut_stage1) cmsStageFree(clut_stage1);
                        if (clut_stage2) cmsStageFree(clut_stage2);
                    }
                }
            }
            
            // Process sequence description
            const cmsSEQ *seq = (const cmsSEQ *)cmsReadTag(input_profile, cmsSigProfileSequenceDescTag);
            if (seq) {
                cmsSEQ *dup_seq = cmsDupProfileSequenceDescription(seq);
                if (dup_seq) {
                    // Process duplicated sequence
                    // Note: cmsSEQ structure would need proper cleanup in real usage
                }
            }
            
            // Convert xyY to XYZ using data from input
            if (size >= 100 + 24) {  // Need 24 bytes for 3 doubles
                cmsCIExyY xyY;
                memcpy(&xyY, data + 100, sizeof(xyY));
                
                cmsCIEXYZ xyz;
                cmsxyY2XYZ(&xyz, &xyY);
            }
            
            cmsDeleteTransform(transform);
        }
        
        cmsCloseProfile(proofing_profile);
        cmsCloseProfile(output_profile);
        cmsCloseProfile(input_profile);
    } else {
        // Profile was successfully read from file
        cmsCloseProfile(input_profile);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    
    // Note: Contexts cannot be freed with given APIs
    // cmsDeleteContext is not available in the provided API list
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}