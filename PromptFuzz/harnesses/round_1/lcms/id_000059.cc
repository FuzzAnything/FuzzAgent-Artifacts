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
//<ID> 408
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"_cmsWriteHeader":[[1030,9,1030,80,0,0,4,0]],"cmsCreateTransformTHR":[[1351,65,1351,79,0,0,4,0]],"cmsSaveProfileToIOhandler":[[1564,13,1564,45,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsMD5computeID","cmsIsIntentSupported","cmsCIECAM02Forward","cmsPipelineAlloc","cmsGetProfileContextID","cmsPipelineFree","cmsGetProfileInfo","cmsWhitePointFromTemp","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8Free","cmsGetProfileIOhandler","cmsSaveProfileToIOhandler","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromStreamTHR","cmsMD5computeID","cmsIsIntentSupported","cmsCIECAM02Forward","cmsPipelineAlloc","cmsGetProfileContextID","cmsPipelineFree","cmsGetProfileInfo","cmsWhitePointFromTemp","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8Free","cmsGetProfileIOhandler","cmsSaveProfileToIOhandler","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Minimum size for meaningful operations
    
    // Open input data as FILE*
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create context and set error handler
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    cmsSetLogErrorHandlerTHR(context, [](cmsContext, cmsUInt32Number, const char*) {});
    
    // Open profile from input stream
    cmsHPROFILE profile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (!profile) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to compute MD5 ID
    cmsMD5computeID(profile);
    
    // Check if perceptual intent is supported
    cmsIsIntentSupported(profile, INTENT_PERCEPTUAL, LCMS_USED_AS_INPUT);
    
    // Create CIECAM02 model (simplified)
    cmsHANDLE cam_model = nullptr;
    cmsCIEXYZ xyz_input = {0.4, 0.3, 0.2};
    cmsJCh jch_output;
    if (cam_model) {
        cmsCIECAM02Forward(cam_model, &xyz_input, &jch_output);
    }
    
    // Create pipeline and get context
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        cmsContext pipe_context = cmsGetProfileContextID(profile);  // Using profile context
        (void)pipe_context;
        cmsPipelineFree(pipeline);
    }
    
    // Get profile info
    wchar_t buffer[256];
    cmsUInt32Number info_size = cmsGetProfileInfo(profile, cmsInfoDescription, "en", "US", 
                                                 buffer, sizeof(buffer)/sizeof(wchar_t));
    
    // Get white point from temperature
    cmsCIExyY white_point;
    cmsBool white_result = cmsWhitePointFromTemp(&white_point, 6500.0);
    
    // Create IT8 handle and get property
    cmsHANDLE it8 = cmsIT8LoadFromFile(context, "dummy.it8");
    if (it8) {
        const char* prop = cmsIT8GetPropertyMulti(it8, "KEY", "SUBKEY");
        (void)prop;
        cmsIT8Free(it8);
    }
    
    // Save profile using IO handler
    cmsIOHANDLER* io_handler = cmsGetProfileIOhandler(profile);
    if (io_handler) {
        cmsUInt32Number saved = cmsSaveProfileToIOhandler(profile, io_handler);
        (void)saved;
    }
    
    // Write transformed data
    cmsHTRANSFORM transform = cmsCreateTransformTHR(context, profile, TYPE_RGB_8, 
                                                   nullptr, TYPE_RGB_8, 
                                                   INTENT_PERCEPTUAL, 0);
    if (transform) {
        uint8_t input[3] = {128, 64, 192};
        uint8_t output[3];
        cmsDoTransform(transform, input, output, 1);
        cmsDeleteTransform(transform);
    }
    
    // Cleanup
    cmsCloseProfile(profile);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}