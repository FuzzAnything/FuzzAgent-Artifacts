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
//<ID> 526
//<Prompt> []
/*<Combination>: [
*/
//<score> 16.666666, nr_unique_branch: 1
//<Quality> {"density":25,"unique_branches":{"cmsMLUgetUTF8":[[602,9,602,20,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsMD5computeID","cmsIsIntentSupported","cmsGetProfileContextID","cmsGetProfileIOhandler","cmsOpenIOhandlerFromStream","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsPipelineAlloc","cmsMLUgetUTF8","cmsGetProfileInfo","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsPipelineFree","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8SetComment","cmsIT8Free","cmsWhitePointFromTemp","cmsfilelength","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromStreamTHR","cmsMD5computeID","cmsIsIntentSupported","cmsGetProfileContextID","cmsGetProfileIOhandler","cmsOpenIOhandlerFromStream","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsPipelineAlloc","cmsMLUgetUTF8","cmsGetProfileInfo","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsPipelineFree","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8SetComment","cmsIT8Free","cmsWhitePointFromTemp","cmsfilelength","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/


// Error handler function
static void  id_000072_myErrorHandler(cmsContext /*ContextID*/, cmsUInt32Number /*ErrorCode*/, const char* /*Text*/) {
    // Do nothing - just suppress errors
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 64) {
	return 0;
	}
	  // Not enough data for meaningful processing
    
    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Set error handler
    cmsSetLogErrorHandlerTHR(context,  id_000072_myErrorHandler);
    
    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open profile from stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Compute MD5 ID
    cmsMD5computeID(hProfile);
    
    // Check if intent is supported
    cmsBool intentSupported = cmsIsIntentSupported(hProfile, INTENT_PERCEPTUAL, LCMS_USED_AS_INPUT);
    
    // Get profile context ID
    cmsContext profileContext = cmsGetProfileContextID(hProfile);
    
    // Get profile IO handler
    cmsIOHANDLER* ioHandler = cmsGetProfileIOhandler(hProfile);
    
    // Save profile to output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        cmsIOHANDLER* outputHandler = cmsOpenIOhandlerFromStream(context, out_file);
        if (outputHandler) {
            cmsSaveProfileToIOhandler(hProfile, outputHandler);
            cmsCloseIOhandler(outputHandler);
        }
        assert_file_closed(&out_file);;
    }
    
    // Create a simple pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        // Get profile info in UTF8
        char buffer[256];
        cmsUInt32Number utf8Size = cmsMLUgetUTF8(nullptr, "en", "US", buffer, sizeof(buffer));
        
        // Get profile info in wide char
        wchar_t wbuffer[256];
        cmsUInt32Number wideSize = cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", wbuffer, 256);
        
        // Create transform
        cmsHTRANSFORM transform = cmsCreateTransformTHR(context, 
                                                        hProfile, 
                                                        TYPE_RGB_8, 
                                                        hProfile, 
                                                        TYPE_RGB_8, 
                                                        INTENT_PERCEPTUAL, 
                                                        0);
        
        if (transform) {
            // Apply transform
            uint8_t input[3] = {128, 128, 128};
            uint8_t output[3];
            cmsDoTransform(transform, input, output, 1);
            cmsDeleteTransform(transform);
        }
        
        cmsPipelineFree(pipeline);
    }
    
    // Load IT8 from file (create a temporary file first)
    FILE *temp_file = fopen("input_file", "wb");
    if (temp_file) {
        // Write some valid IT8-like data
        const char* it8_header = "CGATS.17\n\nBEGIN_DATA_FORMAT\nSAMPLE_NAME\nEND_DATA_FORMAT\n\nBEGIN_DATA\nPatch1\nEND_DATA\n";
        fwrite(it8_header, 1, strlen(it8_header), temp_file);
        assert_file_closed(&temp_file);;
    }
    
    // Load IT8 file
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(context, "input_file");
    if (hIT8) {
        // Get property
        const char* prop = cmsIT8GetPropertyMulti(hIT8, "KEY", "SUBKEY");
        
        // Set comment
        cmsIT8SetComment(hIT8, "Test comment");
        
        cmsIT8Free(hIT8);
    }
    
    // Test white point from temperature
    cmsCIExyY whitePoint;
    cmsBool whitePointResult = cmsWhitePointFromTemp(&whitePoint, 6500.0);
    
    // Get file length
    rewind(in_file);
    long file_len = cmsfilelength(in_file);
    
    // Clean up
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}