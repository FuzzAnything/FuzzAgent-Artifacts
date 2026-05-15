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
//<ID> 205
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 7
//<Quality> {"density":11,"unique_branches":{"DefaultICCintents":[[583,18,583,49,0,0,4,1]],"AddConversion":[[481,5,481,12,0,0,4,0],[482,13,482,28,0,0,4,1]],"_cmsOptimizePipeline":[[1939,9,1939,37,0,0,4,0]],"cmsCreateExtendedTransform":[[1242,9,1242,69,0,0,4,0],[1245,13,1245,72,0,0,4,1]],"cmsGetNamedColorList":[[977,9,977,20,0,0,4,0]]},"library_calls":["cmsCreateXYZProfile","cmsCloseProfile","cmsCreate_OkLabProfile","cmsBuildGamma","cmsDupToneCurve","cmsPipelineAlloc","cmsStageAllocIdentity","cmsStageType","cmsPipelineInsertStage","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateTransform","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsDeleteTransform","cmsCloseProfile","cmsIT8LoadFromFile","cmsIT8TableCount","cmsIT8Free"],"critical_calls":["cmsCreateXYZProfile","cmsCloseProfile","cmsCreate_OkLabProfile","cmsBuildGamma","cmsDupToneCurve","cmsPipelineAlloc","cmsStageAllocIdentity","cmsStageType","cmsPipelineInsertStage","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateTransform","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsDeleteTransform","cmsCloseProfile","cmsIT8LoadFromFile","cmsIT8TableCount","cmsIT8Free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create a FILE* from input data for cube file processing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    cmsContext ctx = nullptr;
    
    // 2. Create an XYZ profile (uses no context)
    cmsHPROFILE xyz_profile = cmsCreateXYZProfile();
    if (xyz_profile) {
        cmsCloseProfile(xyz_profile);
    }
    
    // 3. Create an OkLab profile
    cmsHPROFILE oklab_profile = cmsCreate_OkLabProfile(ctx);
    if (oklab_profile) {
        // Create a simple tone curve for demonstration
        cmsToneCurve* gamma = cmsBuildGamma(ctx, 2.2);
        if (gamma) {
            // 4. Duplicate the tone curve
            cmsToneCurve* dup_gamma = cmsDupToneCurve(gamma);
            
            // Create a pipeline with an identity stage to test cmsStageType
            cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
            if (pipeline) {
                cmsStage* identity_stage = cmsStageAllocIdentity(ctx, 3);
                if (identity_stage) {
                    // 5. Get stage type
                    cmsStageSignature stage_type = cmsStageType(identity_stage);
                    (void)stage_type; // Use to avoid unused warning
                    
                    cmsPipelineInsertStage(pipeline, cmsAT_END, identity_stage);
                }
                cmsPipelineFree(pipeline);
            }
            
            if (dup_gamma) {
                cmsFreeToneCurve(dup_gamma);
            }
            cmsFreeToneCurve(gamma);
        }
        cmsCloseProfile(oklab_profile);
    }
    
    // 6. Create device link from cube file using the input data
    // First, write data to a temporary file for cube file reading
    FILE *cube_file = fopen("temp_input.cube", "wb");
    if (cube_file) {
        fwrite(data, 1, size, cube_file);
        assert_file_closed(&cube_file);;
        
        // Create device link from cube file
        cmsHPROFILE cube_profile = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "temp_input.cube");
        if (cube_profile) {
            // Create a transform to get named color list
            cmsHTRANSFORM transform = cmsCreateTransform(cube_profile, TYPE_RGB_8, 
                                                         cube_profile, TYPE_RGB_8, 
                                                         INTENT_PERCEPTUAL, 0);
            if (transform) {
                // 7. Get named color list and query color info
                cmsNAMEDCOLORLIST* named_color_list = cmsGetNamedColorList(transform);
                if (named_color_list) {
                    char name[256], prefix[256], suffix[256];
                    cmsUInt16Number pcs[3], colorant[16];
                    
                    // 8. Get named color info for first color if exists
                    if (cmsNamedColorCount(named_color_list) > 0) {
                        cmsNamedColorInfo(named_color_list, 0, name, prefix, suffix, pcs, colorant);
                    }
                }
                cmsDeleteTransform(transform);
            }
            cmsCloseProfile(cube_profile);
        }
        remove("temp_input.cube");
    }
    
    // 9. Process input as IT8 file
    rewind(in_file);
    
    // Write data to a file for IT8 loading (IT8 API requires filename)
    FILE *it8_file = fopen("temp_it8.it8", "wb");
    if (it8_file) {
        fwrite(data, 1, size, it8_file);
        assert_file_closed(&it8_file);;
        
        cmsHANDLE it8_handle = cmsIT8LoadFromFile(ctx, "temp_it8.it8");
        if (it8_handle) {
            // 10. Get IT8 table count
            cmsUInt32Number table_count = cmsIT8TableCount(it8_handle);
            (void)table_count; // Use to avoid unused warning
            
            cmsIT8Free(it8_handle);
        }
        remove("temp_it8.it8");
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    
    assert_file_closed(&it8_file);
	assert_file_closed(&cube_file);
	assert_file_closed(&in_file);
	return 0;
}