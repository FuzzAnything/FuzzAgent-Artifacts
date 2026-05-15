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
//<ID> 743
//<Prompt> []
/*<Combination>: [
*/
//<score> 81, nr_unique_branch: 4
//<Quality> {"density":27,"unique_branches":{"Type_ProfileSequenceDesc_Read":[[3546,9,3546,42,0,0,4,1],[3548,9,3548,44,0,0,4,1],[3553,9,3553,23,0,0,4,0]],"cmsAllocProfileSequenceDescription":[[995,9,995,16,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsReadTag","cmsReadTag","cmsFreeProfileSequenceDescription","cmsGetProfileInfo","cmsGetProfileInfoASCII","cmsWhitePointFromTemp","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsOpenIOhandlerFromFile","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsCreateLab4Profile","cmsCreateTransform","cmsGetTransformPipeline","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromStreamTHR","cmsReadTag","cmsReadTag","cmsFreeProfileSequenceDescription","cmsGetProfileInfo","cmsGetProfileInfoASCII","cmsWhitePointFromTemp","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsOpenIOhandlerFromFile","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsCreateLab4Profile","cmsCreateTransform","cmsGetTransformPipeline","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// Simple error handler that does nothing
static void  id_000090_NullErrorHandler(cmsContext /*ContextID*/, cmsUInt32Number /*ErrorCode*/, const char* /*ErrorText*/) {
    // Do nothing - just prevent default error reporting
}

extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a context with null plugin and user data
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }
    
    // Set a null error handler
    cmsSetLogErrorHandlerTHR(context,  id_000090_NullErrorHandler);
    
    // Create a FILE stream from input data for profile reading
    FILE* profile_stream = fmemopen((void*)data, size, "rb");
    if (!profile_stream) {
        cmsDeleteContext(context);
        assert_file_closed(&profile_stream);
	return 0;
    }
    
    // Open profile from the stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(context, profile_stream, "r");
    if (hProfile) {
        // Read a tag from the profile
        void* tag_data = cmsReadTag(hProfile, cmsSigCopyrightTag);
        
        // Get and free profile sequence description if available
        cmsSEQ* seq_desc = static_cast<cmsSEQ*>(cmsReadTag(hProfile, cmsSigProfileSequenceDescTag));
        if (seq_desc) {
            cmsFreeProfileSequenceDescription(seq_desc);
        }
        
        // Get profile info
        wchar_t buffer[256];
        cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", buffer, sizeof(buffer)/sizeof(wchar_t));
        
        // Get profile info ASCII
        char ascii_buffer[256];
        cmsGetProfileInfoASCII(hProfile, cmsInfoDescription, "en", "US", ascii_buffer, sizeof(ascii_buffer));
        
        // Create white point from temperature
        cmsCIExyY white_point;
        cmsBool white_point_success = cmsWhitePointFromTemp(&white_point, 6500.0); // D65
        
        // Create IT8 handle from file
        // First, write input data to a temporary file for IT8 loading
        FILE* temp_file = fopen("input_file", "wb");
        if (temp_file) {
            fwrite(data, 1, size, temp_file);
            assert_file_closed(&temp_file);;
            
            // Load IT8 from the file
            cmsHANDLE it8_handle = cmsIT8LoadFromFile(context, "input_file");
            if (it8_handle) {
                // Get property from IT8
                const char* prop = cmsIT8GetPropertyMulti(it8_handle, "SAMPLE_ID", "0");
                
                // Set index column
                cmsIT8SetIndexColumn(it8_handle, "0");
                
                // Get table count
                cmsUInt32Number table_count = cmsIT8TableCount(it8_handle);
                
                // Free IT8 handle
                cmsIT8Free(it8_handle);
            }
        }
        
        // Create output IO handler for saving profile
        cmsIOHANDLER* output_handler = cmsOpenIOhandlerFromFile(context, "output_file", "w");
        if (output_handler) {
            // Save profile to IO handler
            cmsUInt32Number saved_size = cmsSaveProfileToIOhandler(hProfile, output_handler);
            
            // Close IO handler
            cmsCloseIOhandler(output_handler);
        }
        
        // Create a pipeline for processing
        cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
        if (pipeline) {
            // Create a matrix stage (identity matrix with no offset)
            cmsFloat64Number identity_matrix[9] = {1.0, 0.0, 0.0,
                                                   0.0, 1.0, 0.0,
                                                   0.0, 0.0, 1.0};
            cmsFloat64Number zero_offset[3] = {0.0, 0.0, 0.0};
            
            cmsStage* matrix_stage = cmsStageAllocMatrix(context, 3, 3, identity_matrix, zero_offset);
            if (matrix_stage) {
                // Insert matrix stage into pipeline
                cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, matrix_stage);
            }
        }
        
        // Create a Lab4 profile if white point was successful
        cmsHPROFILE lab_profile = nullptr;
        if (white_point_success) {
            lab_profile = cmsCreateLab4Profile(&white_point);
        }
        
        // Create a transform if we have both profiles
        cmsHTRANSFORM transform = nullptr;
        if (hProfile && lab_profile) {
            transform = cmsCreateTransform(hProfile, 
                                          TYPE_Lab_DBL, 
                                          lab_profile, 
                                          TYPE_Lab_DBL, 
                                          INTENT_PERCEPTUAL, 
                                          cmsFLAGS_NOCACHE);
            
            if (transform) {
                // Get transform pipeline
                cmsPipeline* transform_pipeline = cmsGetTransformPipeline(transform);
                
                // Do a transformation with sample data
                cmsCIELab input_lab = {50.0, 0.0, 0.0};
                cmsCIELab output_lab;
                
                cmsDoTransform(transform, &input_lab, &output_lab, 1);
                
                // Delete the transform
                cmsDeleteTransform(transform);
            }
            
            // Close Lab profile
            cmsCloseProfile(lab_profile);
        }
        
        // Free the pipeline if created
        if (pipeline) {
            cmsPipelineFree(pipeline);
        }
        
        // Close the main profile
        cmsCloseProfile(hProfile);
    }
    
    // Clean up
    if (profile_stream) {
        assert_file_closed(&profile_stream);;
    }
    
    // Remove temporary files if they exist
    remove("input_file");
    remove("output_file");
    
    // Delete context
    cmsDeleteContext(context);
    
    assert_file_closed(&profile_stream);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}