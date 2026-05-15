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
//<ID> 673
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsIT8LoadFromFile","cmsIT8GetPropertyDbl","cmsIT8SetIndexColumn","cmsIT8GetDataDbl","cmsCreateDeviceLinkFromCubeFile","cmsCreateProfilePlaceholder","cmsMLUalloc","cmsMLUsetASCII","cmsBuildGamma","cmsGetToneCurveSegment","cmsCreateRGBProfile","cmsCreateTransform","cmsDoTransformLineStride","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineEvalReverseFloat","cmsPipelineFree","cmsGetContextUserData","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsMLUfree","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromFile","cmsIT8GetPropertyDbl","cmsIT8SetIndexColumn","cmsIT8GetDataDbl","cmsCreateDeviceLinkFromCubeFile","cmsCreateProfilePlaceholder","cmsMLUalloc","cmsMLUsetASCII","cmsBuildGamma","cmsGetToneCurveSegment","cmsCreateRGBProfile","cmsCreateTransform","cmsDoTransformLineStride","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineEvalReverseFloat","cmsPipelineFree","cmsGetContextUserData","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsMLUfree","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	

    // Write input data to temporary files
    FILE *it8_file = fopen("input_it8.its", "wb");
    if (!it8_file) {
        cmsDeleteContext(context);
        assert_file_closed(&it8_file);
	return 0;
    }
    fwrite(data, 1, size / 2, it8_file);
    assert_file_closed(&it8_file);;

    FILE *cube_file = fopen("input_cube.cube", "wb");
    if (!cube_file) {
        cmsDeleteContext(context);
        remove("input_it8.its");
        assert_file_closed(&cube_file);
	assert_file_closed(&it8_file);
	return 0;
    }
    fwrite(data + size / 2, 1, size - size / 2, cube_file);
    assert_file_closed(&cube_file);;

    // Load IT8 file
    cmsHANDLE it8 = cmsIT8LoadFromFile(context, "input_it8.its");
    if (it8) {
        // Get property as double
        cmsFloat64Number prop = cmsIT8GetPropertyDbl(it8, "SAMPLE_FIELD");
        
        // Set index column
        cmsIT8SetIndexColumn(it8, "INDEX_COL");
        
        // Get data as double
        cmsFloat64Number data_val = cmsIT8GetDataDbl(it8, "PATCH1", "SAMPLE1");
    }

    // Create device link from cube file
    cmsHPROFILE cube_profile = cmsCreateDeviceLinkFromCubeFile("input_cube.cube");

    // Create profile placeholder
    cmsHPROFILE placeholder = cmsCreateProfilePlaceholder(context);

    // Allocate MLU
    cmsMLU *mlu = cmsMLUalloc(context, 10);
    if (mlu) {
        // Set ASCII string in MLU
        cmsMLUsetASCII(mlu, "en", "US", "Test Description");
    }

    // Build gamma curve
    cmsToneCurve *gamma_curve = cmsBuildGamma(context, 2.2);
    
    // Get tone curve segment (if curve exists)
    if (gamma_curve) {
        const cmsCurveSegment *segment = cmsGetToneCurveSegment(0, gamma_curve);
    }

    // Create RGB profile with default values
    cmsCIExyY white_point = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.64, 0.33, 1.0},  // Red
        {0.30, 0.60, 1.0},  // Green
        {0.15, 0.06, 1.0}   // Blue
    };
    cmsToneCurve *transfer_func[3] = {gamma_curve, gamma_curve, gamma_curve};
    
    cmsHPROFILE rgb_profile = cmsCreateRGBProfile(&white_point, &primaries, transfer_func);

    // Create transform
    cmsUInt32Number input_format = TYPE_RGB_8;
    cmsUInt32Number output_format = TYPE_RGB_8;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = cmsFLAGS_NOCACHE;
    
    cmsHTRANSFORM transform = NULL;
    if (rgb_profile) {
        cmsHPROFILE output_profile = cube_profile ? cube_profile : rgb_profile;
        transform = cmsCreateTransform(rgb_profile, input_format, 
                                      output_profile, output_format, 
                                      intent, flags);
    }

    // Perform transform with line stride if transform exists
    if (transform) {
        const int width = 10;
        const int height = 10;
        unsigned char input_buffer[width * height * 3];
        unsigned char output_buffer[width * height * 3];
        
        // Fill input buffer with some data
        for (int i = 0; i < width * height * 3; i++) {
            input_buffer[i] = data[i % size];
        }
        
        cmsDoTransformLineStride(transform, input_buffer, output_buffer,
                                width, height,
                                width * 3, width * 3,  // Bytes per line
                                0, 0);  // Bytes per plane
    }

    // Create pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        // Create identity stage
        cmsStage *identity_stage = cmsStageAllocIdentity(context, 3);
        
        // Insert stage into pipeline
        if (identity_stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, identity_stage);
        }
        
        // Evaluate pipeline in reverse
        cmsFloat32Number target[3] = {0.5f, 0.5f, 0.5f};
        cmsFloat32Number result[3] = {0.0f, 0.0f, 0.0f};
        cmsFloat32Number hint[3] = {0.0f, 0.0f, 0.0f};
        
        cmsPipelineEvalReverseFloat(target, result, hint, pipeline);
        
        // Free pipeline
        cmsPipelineFree(pipeline);
    }

    // Get context user data
    void *user_data = cmsGetContextUserData(context);

    // Cleanup
    if (gamma_curve) {
        cmsFreeToneCurve(gamma_curve);
    }
    
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (rgb_profile) {
        cmsCloseProfile(rgb_profile);
    }
    
    if (cube_profile) {
        cmsCloseProfile(cube_profile);
    }
    
    if (placeholder) {
        cmsCloseProfile(placeholder);
    }
    
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    if (it8) {
        cmsIT8Free(it8);
    }
    
    cmsDeleteContext(context);
    
    // Remove temporary files
    remove("input_it8.its");
    remove("input_cube.cube");
    
    assert_file_closed(&cube_file);
	assert_file_closed(&it8_file);
	return 0;
}