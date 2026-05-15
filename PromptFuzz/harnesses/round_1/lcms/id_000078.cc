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
//<ID> 574
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 1
//<Quality> {"density":36,"unique_branches":{"ToSpherical":[[130,8,130,19,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsMD5computeID","cmsIsIntentSupported","cmsGetProfileContextID","cmsGetProfileIOhandler","cmsOpenIOhandlerFromStream","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsPipelineAlloc","cmsGetProfileInfo","cmsMLUalloc","cmsMLUdup","cmsMLUgetUTF8","cmsMLUfree","cmsMLUfree","cmsCreate_sRGBProfileTHR","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsGBDAlloc","cmsGDBAddPoint","cmsGDBCompute","cmsGBDFree","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8SetComment","cmsIT8Free","cmsWhitePointFromTemp","cmsAdaptToIlluminant","cmsfilelength","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsOpenProfileFromStreamTHR","cmsMD5computeID","cmsIsIntentSupported","cmsGetProfileContextID","cmsGetProfileIOhandler","cmsOpenIOhandlerFromStream","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsPipelineAlloc","cmsGetProfileInfo","cmsMLUalloc","cmsMLUdup","cmsMLUgetUTF8","cmsMLUfree","cmsMLUfree","cmsCreate_sRGBProfileTHR","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsGBDAlloc","cmsGDBAddPoint","cmsGDBCompute","cmsGBDFree","cmsIT8LoadFromFile","cmsIT8GetPropertyMulti","cmsIT8SetComment","cmsIT8Free","cmsWhitePointFromTemp","cmsAdaptToIlluminant","cmsfilelength","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified lcms APIs to process ICC profile data:

*/



extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	

    // 1. Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // 2. Set error handler
    cmsSetLogErrorHandlerTHR(ctx, NULL);
    
    // 3. Create input stream from fuzzed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Open profile from stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(ctx, in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Compute MD5 for profile
    cmsBool md5_result = cmsMD5computeID(hProfile);
    
    // 6. Check if intent is supported
    cmsBool intent_supported = cmsIsIntentSupported(hProfile, INTENT_PERCEPTUAL, LCMS_USED_AS_INPUT);
    
    // 7. Get profile context ID
    cmsContext profile_ctx = cmsGetProfileContextID(hProfile);
    
    // 8. Get I/O handler from profile
    cmsIOHANDLER *io_handler = cmsGetProfileIOhandler(hProfile);
    
    // 9. Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // 10. Create output I/O handler
        cmsIOHANDLER *out_io = cmsOpenIOhandlerFromStream(ctx, out_file);
        
        // 11. Save profile to I/O handler
        if (out_io) {
            cmsUInt32Number saved = cmsSaveProfileToIOhandler(hProfile, out_io);
            cmsCloseIOhandler(out_io);
        }
        assert_file_closed(&out_file);;
    }
    
    // 12. Create pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(ctx, 3, 3);
    
    // 13. Get MLU data from profile
    wchar_t info_buffer[256];
    cmsUInt32Number info_size = cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", 
                                                   info_buffer, sizeof(info_buffer)/sizeof(wchar_t));
    
    // Duplicate MLU if we have one
    cmsMLU *mlu = NULL;
    if (info_size > 0) {
        // Create temporary MLU for duplication test
        mlu = cmsMLUalloc(ctx, 1);
        if (mlu) {
            cmsMLU *mlu_dup = cmsMLUdup(mlu);
            if (mlu_dup) {
                char utf8_buffer[256];
                cmsUInt32Number utf8_size = cmsMLUgetUTF8(mlu_dup, "en", "US", 
                                                         utf8_buffer, sizeof(utf8_buffer));
                cmsMLUfree(mlu_dup);
            }
            cmsMLUfree(mlu);
        }
    }
    
    // 14. Create transform and test it
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfileTHR(ctx);
    if (srgb_profile) {
        cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, hProfile, 
                                                       TYPE_RGB_8, 
                                                       srgb_profile, 
                                                       TYPE_RGB_8,
                                                       INTENT_PERCEPTUAL, 
                                                       0);
        
        if (transform) {
            // Test transform with some data
            unsigned char input_pixels[12] = {0};
            unsigned char output_pixels[12] = {0};
            
            // Use fuzzed data for input pixels if we have enough
            size_t pixels_to_use = size < sizeof(input_pixels) ? size : sizeof(input_pixels);
            if (pixels_to_use > 0) {
                memcpy(input_pixels, data, pixels_to_use);
                cmsDoTransform(transform, input_pixels, output_pixels, 4);
            }
            
            cmsDeleteTransform(transform);
        }
        cmsCloseProfile(srgb_profile);
    }
    
    // 15. Test GDB computation
    cmsHANDLE gdb = cmsGBDAlloc(ctx);
    if (gdb) {
        // Add some sample points
        cmsCIELab lab = {50, 0, 0};
        cmsGDBAddPoint(gdb, &lab);
        
        cmsBool gdb_computed = cmsGDBCompute(gdb, 0);
        cmsGBDFree(gdb);
    }
    
    // 16. Test IT8 operations
    const char *temp_filename = "temp_it8.txt";
    FILE *temp_file = fopen(temp_filename, "wb");
    if (temp_file && size > 0) {
        fwrite(data, 1, size < 100 ? size : 100, temp_file);
        assert_file_closed(&temp_file);;
        
        cmsHANDLE it8 = cmsIT8LoadFromFile(ctx, temp_filename);
        if (it8) {
            const char *prop = cmsIT8GetPropertyMulti(it8, "SAMPLE", "ID");
            cmsIT8SetComment(it8, "Fuzzer comment");
            cmsIT8Free(it8);
        }
        remove(temp_filename);
    }
    
    // 17. Test white point from temperature
    cmsCIExyY white_point;
    cmsBool white_point_ok = cmsWhitePointFromTemp(&white_point, 6500.0);
    
    // 18. Test color adaptation
    if (white_point_ok) {
        cmsCIEXYZ source_white = {0.9505, 1.0000, 1.0890};
        cmsCIEXYZ illuminant = {0.9642, 1.0000, 0.8249};
        cmsCIEXYZ value = {0.5, 0.5, 0.5};
        cmsCIEXYZ result;
        
        cmsBool adapted = cmsAdaptToIlluminant(&result, &source_white, &illuminant, &value);
    }
    
    // 19. Get file length
    long file_len = cmsfilelength(in_file);
    
    // 20. Clean up pipeline if created
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    // 21. Clean up resources
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);
    
    assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}