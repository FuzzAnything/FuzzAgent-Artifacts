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
//<ID> 639
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 1
//<Quality> {"density":22,"unique_branches":{"cmsGetPostScriptCRD":[[1561,9,1561,23,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsCreateXYZProfileTHR","cmsDeleteContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateTransform","cmsFloat2LabEncodedV2","cmsPipelineAlloc","cmsStageAllocIdentity","cmsStageType","cmsPipelineInsertStage","cmsGetPostScriptCRD","cmsGetPostScriptCRD","cmsIT8LoadFromFile","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsDoTransform","cmsDeleteTransform","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsBuildGamma","cmsFreeToneCurve","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateXYZProfileTHR","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateTransform","cmsFloat2LabEncodedV2","cmsPipelineAlloc","cmsStageAllocIdentity","cmsStageType","cmsPipelineInsertStage","cmsGetPostScriptCRD","cmsGetPostScriptCRD","cmsIT8LoadFromFile","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsDoTransform","cmsDeleteTransform","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsBuildGamma","cmsFreeToneCurve","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
	return 0;
	}
	 // Minimal size for meaningful testing
    
    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Split input data for multiple uses
    size_t cube_size = size / 3;
    size_t it8_size = size / 3;
    size_t pipeline_size = size - cube_size - it8_size;
    
    // Create temporary files
    FILE *cube_file = fopen("input_cube.cube", "wb");
    if (!cube_file) {
        cmsDeleteContext(context);
        assert_file_closed(&cube_file);
	return 0;
    }
    fwrite(data, 1, cube_size, cube_file);
    assert_file_closed(&cube_file);;
    
    FILE *it8_file = fopen("input_it8.it8", "wb");
    if (!it8_file) {
        remove("input_cube.cube");
        cmsDeleteContext(context);
        assert_file_closed(&it8_file);
	assert_file_closed(&cube_file);
	return 0;
    }
    fwrite(data + cube_size, 1, it8_size, it8_file);
    assert_file_closed(&it8_file);;
    
    // Create XYZ profile
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(context);
    if (!xyz_profile) {
        cleanup_files:
        remove("input_cube.cube");
        remove("input_it8.it8");
        cmsDeleteContext(context);
        assert_file_closed(&it8_file);
	assert_file_closed(&cube_file);
	return 0;
    }
    
    // Create device link from cube file
    cmsHPROFILE cube_profile = cmsCreateDeviceLinkFromCubeFileTHR(context, "input_cube.cube");
    
    // Create transform
    cmsUInt32Number input_format = TYPE_Lab_16;
    cmsUInt32Number output_format = TYPE_XYZ_16;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = cmsFLAGS_NOCACHE;
    
    cmsHTRANSFORM transform = nullptr;
    if (cube_profile) {
        transform = cmsCreateTransform(xyz_profile, input_format, 
                                      cube_profile, output_format, 
                                      intent, flags);
    }
    
    // Test cmsFloat2LabEncodedV2
    cmsCIELab lab = {50.0, 20.0, -30.0};
    cmsUInt16Number encoded_lab[3];
    cmsFloat2LabEncodedV2(encoded_lab, &lab);
    
    // Test pipeline operations
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        cmsStage* identity_stage = cmsStageAllocIdentity(context, 3);
        if (identity_stage) {
            cmsStageType(identity_stage); // Check stage type
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, identity_stage);
        }
    }
    
    // Test PostScript CRD
    if (cube_profile) {
        cmsUInt32Number buffer_size = cmsGetPostScriptCRD(context, cube_profile, 
                                                         intent, flags, 
                                                         nullptr, 0);
        if (buffer_size > 0 && buffer_size < 65536) {
            void* buffer = malloc(buffer_size);
            if (buffer) {
                cmsGetPostScriptCRD(context, cube_profile, intent, flags, 
                                   buffer, buffer_size);
                free(buffer);
            }
        }
    }
    
    // Test IT8 operations
    cmsHANDLE it8 = cmsIT8LoadFromFile(context, "input_it8.it8");
    if (it8) {
        cmsIT8SetIndexColumn(it8, "Sample1");
        cmsIT8TableCount(it8);
        cmsIT8Free(it8);
    }
    
    // Test transformation if we have a valid transform
    if (transform) {
        // Prepare input/output buffers using remaining data
        size_t sample_size = pipeline_size > 16 ? 16 : pipeline_size;
        if (sample_size >= 6) { // Need at least 3 Lab channels (2 bytes each)
            cmsUInt16Number input_buffer[3];
            cmsUInt16Number output_buffer[3];
            
            // Use data for sample values
            for (int i = 0; i < 3 && (i * 2 + 1) < sample_size; i++) {
                input_buffer[i] = (data[cube_size + it8_size + i * 2] << 8) | 
                                  data[cube_size + it8_size + i * 2 + 1];
            }
            
            cmsDoTransform(transform, input_buffer, output_buffer, 1);
        }
        
        cmsDeleteTransform(transform);
    }
    
    // Cleanup
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (cube_profile) {
        cmsCloseProfile(cube_profile);
    }
    
    if (xyz_profile) {
        cmsCloseProfile(xyz_profile);
    }
    
    // Create and free a tone curve to use cmsFreeToneCurve
    cmsToneCurve* curve = cmsBuildGamma(context, 2.2);
    if (curve) {
        cmsFreeToneCurve(curve);
    }
    
    // Clean up files
    remove("input_cube.cube");
    remove("input_it8.it8");
    
    cmsDeleteContext(context);
    
    assert_file_closed(&it8_file);
	assert_file_closed(&cube_file);
	return 0;
}