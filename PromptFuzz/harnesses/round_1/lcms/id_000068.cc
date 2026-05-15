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
//<ID> 464
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 1
//<Quality> {"density":28,"unique_branches":{"AllocateDataSet":[[1736,54,1736,69,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsMD5computeID","cmsIsIntentSupported","cmsGetProfileContextID","cmsGetProfileInfo","cmsWhitePointFromTemp","cmsCreateLab4Profile","cmsCreateTransform","cmsGetTransformPipeline","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineFree","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsOpenIOhandlerFromFile","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsCloseProfile","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromStreamTHR","cmsMD5computeID","cmsIsIntentSupported","cmsGetProfileContextID","cmsGetProfileInfo","cmsWhitePointFromTemp","cmsCreateLab4Profile","cmsCreateTransform","cmsGetTransformPipeline","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsPipelineFree","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsOpenIOhandlerFromFile","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsCloseProfile","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }

    // Set error handler (dummy)
    cmsSetLogErrorHandlerTHR(context, nullptr);

    // Open profile from input stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE profile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (profile) {
        // Compute MD5 ID
        cmsMD5computeID(profile);
        
        // Check intent support
        cmsIsIntentSupported(profile, INTENT_PERCEPTUAL, LCMS_USED_AS_INPUT);
        
        // Get profile context ID
        cmsGetProfileContextID(profile);
        
        // Get profile info
        wchar_t info_buffer[256];
        cmsGetProfileInfo(profile, cmsInfoDescription, "en", "US", 
                         info_buffer, sizeof(info_buffer)/sizeof(wchar_t));
        
        // Get white point from temperature
        cmsCIExyY white_point;
        cmsWhitePointFromTemp(&white_point, 6500.0);
        
        // Use CIECAM02 (requires a model handle - we'll create dummy data)
        cmsJCh jch;
        cmsCIEXYZ xyz = {0.5, 0.5, 0.5};
        // Note: cmsCIECAM02Forward requires a model handle which we don't have
        // This API call is commented as it requires proper initialization
        // cmsCIECAM02Forward(nullptr, &xyz, &jch);
        
        // Create a simple transform to get pipeline
        cmsHPROFILE lab_profile = cmsCreateLab4Profile(nullptr);
        if (lab_profile) {
            cmsHTRANSFORM transform = cmsCreateTransform(profile, 
                                                        TYPE_Lab_DBL,
                                                        lab_profile,
                                                        TYPE_Lab_DBL,
                                                        INTENT_PERCEPTUAL, 0);
            if (transform) {
                // Get transform pipeline
                cmsPipeline *pipeline = cmsGetTransformPipeline(transform);
                
                // Allocate new pipeline
                cmsPipeline *new_pipeline = cmsPipelineAlloc(context, 3, 3);
                if (new_pipeline) {
                    // Create identity matrix
                    cmsFloat64Number matrix[9] = {1,0,0,0,1,0,0,0,1};
                    cmsFloat64Number offset[3] = {0,0,0};
                    
                    // Allocate matrix stage and insert
                    cmsStage *matrix_stage = cmsStageAllocMatrix(context, 3, 3, 
                                                                matrix, offset);
                    if (matrix_stage) {
                        cmsPipelineInsertStage(new_pipeline, cmsAT_BEGIN, 
                                              matrix_stage);
                    }
                    
                    // Free pipeline
                    cmsPipelineFree(new_pipeline);
                }
                
                // Do a transform
                cmsCIELab input_lab = {50, 0, 0};
                cmsCIELab output_lab;
                cmsDoTransform(transform, &input_lab, &output_lab, 1);
                
                // Delete transform
                cmsDeleteTransform(transform);
            }
            cmsCloseProfile(lab_profile);
        }
        
        // Save profile to file via I/O handler
        cmsIOHANDLER *io = cmsOpenIOhandlerFromFile(context, "output_file", "w");
        if (io) {
            cmsSaveProfileToIOhandler(profile, io);
            cmsCloseIOhandler(io);
        }
        
        // Close profile
        cmsCloseProfile(profile);
    }
    
    assert_file_closed(&in_file);;
    
    // IT8 operations (create dummy IT8 data in file)
    FILE *it8_file = fopen("input_file", "wb");
    if (it8_file && size > 0) {
        fwrite(data, 1, size < 100 ? size : 100, it8_file);
        assert_file_closed(&it8_file);;
        
        // Load IT8 from file
        cmsHANDLE it8 = cmsIT8LoadFromFile(context, "input_file");
        if (it8) {
            // Get property
            const char *prop = cmsIT8GetPropertyMulti(it8, "SAMPLE", "0");
            
            // Set index column
            cmsIT8SetIndexColumn(it8, "SAMPLE");
            
            // Get table count
            cmsUInt32Number table_count = cmsIT8TableCount(it8);
            
            // Free IT8 handle
            cmsIT8Free(it8);
        }
    }
    
    // Clean up
    cmsDeleteContext(context);
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&it8_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}