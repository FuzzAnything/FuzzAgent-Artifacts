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
//<ID> 82
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":26,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsOpenIOhandlerFromNULL","cmsCreateXYZProfileTHR","cmsSetHeaderRenderingIntent","cmsFloat2XYZEncoded","cmsPipelineAlloc","cmsBuildGamma","cmsPipelineInsertStage","cmsStageAllocToneCurves","cmsFreeToneCurve","cmsPipelineSetSaveAs8bitsFlag","cmsPipelineFree","cmsCloseProfile","cmsCloseIOhandler","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8SetDataDbl","cmsIT8Free","cmsOpenProfileFromStream","cmsGetColorSpace","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCreateLab4Profile","cmsSaveProfileToStream","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenIOhandlerFromNULL","cmsCreateXYZProfileTHR","cmsSetHeaderRenderingIntent","cmsFloat2XYZEncoded","cmsPipelineAlloc","cmsBuildGamma","cmsPipelineInsertStage","cmsStageAllocToneCurves","cmsFreeToneCurve","cmsPipelineSetSaveAs8bitsFlag","cmsPipelineFree","cmsCloseProfile","cmsCloseIOhandler","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8SetDataDbl","cmsIT8Free","cmsOpenProfileFromStream","cmsGetColorSpace","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCreateLab4Profile","cmsSaveProfileToStream","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Use cmsOpenIOhandlerFromNULL to create a NULL IO handler
    cmsIOHANDLER *null_handler = cmsOpenIOhandlerFromNULL(ctx);
    if (null_handler) {
        // Create a simple XYZ profile using the NULL handler context
        cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(ctx);
        if (xyz_profile) {
            // 2. Use cmsSetHeaderRenderingIntent
            cmsSetHeaderRenderingIntent(xyz_profile, INTENT_PERCEPTUAL);
            
            // 3. Use cmsFloat2XYZEncoded to convert float XYZ to encoded
            cmsCIEXYZ float_xyz = {0.5, 0.5, 0.5}; // Sample values
            cmsUInt16Number encoded_xyz[3];
            cmsFloat2XYZEncoded(encoded_xyz, &float_xyz);
            
            // 4. Create a pipeline and use cmsPipelineSetSaveAs8bitsFlag
            cmsPipeline *pipeline = cmsPipelineAlloc(ctx, 3, 3);
            if (pipeline) {
                // Add a simple curve stage
                cmsToneCurve *curve = cmsBuildGamma(ctx, 2.2);
                if (curve) {
                    cmsPipelineInsertStage(pipeline, cmsAT_END, cmsStageAllocToneCurves(ctx, 1, &curve));
                    cmsFreeToneCurve(curve);
                }
                
                // Set 8-bit flag
                cmsPipelineSetSaveAs8bitsFlag(pipeline, TRUE);
                
                // Clean up pipeline
                cmsPipelineFree(pipeline);
            }
            
            // Clean up profile
            cmsCloseProfile(xyz_profile);
        }
        cmsCloseIOhandler(null_handler);
    }
    
    // 5. Use cmsIT8DefineDblFormat with an IT8 handle
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    if (it8) {
        // Define double format
        cmsIT8DefineDblFormat(it8, "%.6f");
        
        // Set some sample data
        cmsIT8SetDataDbl(it8, "1", "R", 0.5);
        
        cmsIT8Free(it8);
    }
    
    // Try to open profile from input stream
    fseek(in_file, 0, SEEK_SET);
    cmsHPROFILE input_profile = cmsOpenProfileFromStream(in_file, "r");
    
    if (input_profile) {
        // Get and modify some profile properties
        cmsColorSpaceSignature cs = cmsGetColorSpace(input_profile);
        
        // Create a simple transform if we have a valid profile
        cmsUInt32Number format = TYPE_RGB_8;
        cmsHTRANSFORM transform = cmsCreateTransformTHR(
            ctx, 
            input_profile, format,
            input_profile, format,
            INTENT_PERCEPTUAL, 0
        );
        
        if (transform) {
            // Test the transform with some data
            uint8_t input[3] = {128, 128, 128};
            uint8_t output[3];
            
            if (size >= 3) {
                memcpy(input, data, 3);
            }
            
            cmsDoTransform(transform, input, output, 1);
            
            // Write output to file
            fwrite(output, 1, 3, out_file);
            
            cmsDeleteTransform(transform);
        }
        
        cmsCloseProfile(input_profile);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Try to save a simple profile to output file (reopening in append mode)
    out_file = fopen("output_file", "ab");
    if (out_file) {
        cmsHPROFILE lab_profile = cmsCreateLab4Profile(NULL); // D50 white point
        if (lab_profile) {
            cmsSaveProfileToStream(lab_profile, out_file);
            cmsCloseProfile(lab_profile);
        }
        assert_file_closed(&out_file);;
    }
    
    // Final cleanup
    cmsDeleteContext(ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}