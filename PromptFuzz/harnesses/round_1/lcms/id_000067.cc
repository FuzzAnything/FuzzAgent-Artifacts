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
//<ID> 461
//<Prompt> []
/*<Combination>: [
*/
//<score> 13.5, nr_unique_branch: 0
//<Quality> {"density":27,"unique_branches":{},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsMD5computeID","cmsIsIntentSupported","cmsGetProfileContextID","cmsGetProfileInfo","cmsWhitePointFromTemp","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsIT8LoadFromFile","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsOpenIOhandlerFromFile","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsCreateLab4Profile","cmsCreateTransform","cmsGetTransformPipeline","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromStreamTHR","cmsMD5computeID","cmsIsIntentSupported","cmsGetProfileContextID","cmsGetProfileInfo","cmsWhitePointFromTemp","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsIT8LoadFromFile","cmsIT8SetIndexColumn","cmsIT8TableCount","cmsIT8Free","cmsOpenIOhandlerFromFile","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsCreateLab4Profile","cmsCreateTransform","cmsGetTransformPipeline","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create context with default plugin and userdata
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // Set up error handler (using default)
    cmsSetLogErrorHandlerTHR(ctx, nullptr);
    
    // Create FILE* from input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open ICC profile from stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(ctx, in_file, "rb");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Compute MD5 ID for the profile
    cmsMD5computeID(hProfile);
    
    // Check if perceptual intent is supported in forward direction
    cmsIsIntentSupported(hProfile, INTENT_PERCEPTUAL, LCMS_USED_AS_INPUT);
    
    // Get the profile's context ID
    cmsContext profileCtx = cmsGetProfileContextID(hProfile);
    
    // Get profile information (description)
    wchar_t buffer[256];
    cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", buffer, sizeof(buffer)/sizeof(wchar_t));
    
    // Convert temperature to white point
    cmsCIExyY whitePoint;
    cmsWhitePointFromTemp(&whitePoint, 6500.0);
    
    // Write input data to temporary files for IT8 loading
    const char* it8_filenames[2] = {"it8_input1", "it8_input2"};
    for (int i = 0; i < 2; i++) {
        FILE* tmp = fopen(it8_filenames[i], "wb");
        if (tmp) {
            fwrite(data, 1, size, tmp);
            assert_file_closed(&tmp);;
        }
    }
    
    // Load first IT8 file
    cmsHANDLE hIT8_1 = cmsIT8LoadFromFile(ctx, it8_filenames[0]);
    if (hIT8_1) {
        // Get property with subkey
        const char* prop = cmsIT8GetPropertyMulti(hIT8_1, "ORIGINATOR", "MAIN");
        
        // Set index column
        cmsIT8SetIndexColumn(hIT8_1, "SAMPLE_ID");
        
        // Get table count
        cmsUInt32Number tableCount = cmsIT8TableCount(hIT8_1);
        
        // Free IT8 handle
        cmsIT8Free(hIT8_1);
    }
    
    // Load second IT8 file (using same data)
    cmsHANDLE hIT8_2 = cmsIT8LoadFromFile(ctx, it8_filenames[1]);
    if (hIT8_2) {
        // Set index column for second file
        cmsIT8SetIndexColumn(hIT8_2, "DATA_FORMAT");
        
        // Get table count for second file
        cmsUInt32Number tableCount2 = cmsIT8TableCount(hIT8_2);
        
        // Free IT8 handle
        cmsIT8Free(hIT8_2);
    }
    
    // Clean up temporary IT8 files
    for (int i = 0; i < 2; i++) {
        remove(it8_filenames[i]);
    }
    
    // Create IOHANDLER for writing profile
    cmsIOHANDLER* io = cmsOpenIOhandlerFromFile(ctx, "output_profile.icc", "wb");
    if (io) {
        // Save profile to IOHANDLER
        cmsSaveProfileToIOhandler(hProfile, io);
        
        // Close IOHANDLER
        cmsCloseIOhandler(io);
    }
    
    // Clean up output file
    remove("output_profile.icc");
    
    // Create a color transformation pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline) {
        // Create identity matrix (3x3)
        cmsFloat64Number identityMatrix[9] = {1.0, 0.0, 0.0,
                                               0.0, 1.0, 0.0,
                                               0.0, 0.0, 1.0};
        cmsFloat64Number offset[3] = {0.0, 0.0, 0.0};
        
        // Allocate matrix stage
        cmsStage* matrixStage = cmsStageAllocMatrix(ctx, 3, 3, identityMatrix, offset);
        if (matrixStage) {
            // Insert matrix stage at the beginning of pipeline
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, matrixStage);
            
            // CIECAM02 forward transformation (requires model, but we'll at least call it)
            cmsCIEXYZ inputXYZ = {0.5, 0.5, 0.5};
            cmsJCh outputJCh;
            
            // Note: We don't have a valid model handle here, but we're demonstrating the call
            // In practice, you'd need cmsCIECAM02Init() which isn't in our API list
            
            // For completeness, we'll create a dummy transform to get its pipeline
            cmsHPROFILE labProfile = cmsCreateLab4Profile(nullptr);
            cmsHTRANSFORM hTransform = nullptr;
            
            if (labProfile) {
                // Create a simple transform
                hTransform = cmsCreateTransform(hProfile, TYPE_RGB_8, labProfile, TYPE_Lab_8, INTENT_PERCEPTUAL, 0);
                
                if (hTransform) {
                    // Get transform pipeline
                    cmsPipeline* transformPipeline = cmsGetTransformPipeline(hTransform);
                    
                    // Test color transformation with sample data
                    uint8_t rgbInput[3] = {128, 128, 128};
                    uint8_t labOutput[3];
                    cmsDoTransform(hTransform, rgbInput, labOutput, 1);
                    
                    // Delete transform
                    cmsDeleteTransform(hTransform);
                }
                cmsCloseProfile(labProfile);
            }
        }
        
        // Free pipeline (also frees any stages)
        cmsPipelineFree(pipeline);
    }
    
    // Close input profile
    cmsCloseProfile(hProfile);
    
    // Close input file
    assert_file_closed(&in_file);;
    
    // Delete context
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}