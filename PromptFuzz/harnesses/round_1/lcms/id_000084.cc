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
//<ID> 659
//<Prompt> []
/*<Combination>: [
*/
//<score> 145, nr_unique_branch: 14
//<Quality> {"density":29,"unique_branches":{"bchswSampler":[[845,9,845,25,0,0,4,1]],"Clamp_L_doubleV4":[[271,9,271,14,0,0,4,0]],"cmsCreateBCHSWabstractProfileTHR":[[882,9,882,28,0,0,4,0]],"PrelinEval8":[[967,13,967,21,0,0,4,1],[974,17,974,25,0,0,4,0],[974,29,974,37,0,0,4,0],[974,29,974,37,0,0,4,1],[981,21,981,29,0,0,4,1],[981,33,981,41,0,0,4,1],[988,25,988,33,0,0,4,0],[988,37,988,45,0,0,4,0],[988,37,988,45,0,0,4,1],[995,29,995,37,0,0,4,0],[995,41,995,49,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsCreate_sRGBProfile","cmsCloseProfile","cmsDeleteContext","cmsCreateBCHSWabstractProfile","cmsGetProfileInfoASCII","cmsGetHeaderFlags","cmsReadTag","cmsFreeProfileSequenceDescription","cmsCreateTransformTHR","cmsCreateTransformTHR","cmsPipelineAlloc","cmsPipelineGetPtrToFirstStage","cmsPipelineCheckAndRetreiveStages","cmsStageInputChannels","cmsStageOutputChannels","cmsPipelineFree","cmsBuildGamma","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8Free","cmsGetAlarmCodes","cmsDoTransform","cmsDeleteTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsCreate_sRGBProfile","cmsCreateBCHSWabstractProfile","cmsGetProfileInfoASCII","cmsGetHeaderFlags","cmsReadTag","cmsFreeProfileSequenceDescription","cmsCreateTransformTHR","cmsCreateTransformTHR","cmsPipelineAlloc","cmsPipelineGetPtrToFirstStage","cmsPipelineCheckAndRetreiveStages","cmsStageInputChannels","cmsStageOutputChannels","cmsPipelineFree","cmsBuildGamma","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8Free","cmsGetAlarmCodes","cmsDoTransform","cmsDeleteTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified lcms APIs in a logical sequence:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1024) {
	return 0;
	}
	  // Need enough data for meaningful testing
    
    // 1. Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // 2. Open profile from input stream using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Open profile from stream
    cmsHPROFILE input_profile = cmsOpenProfileFromStreamTHR(ctx, in_file, "r");
    if (!input_profile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create sRGB profile
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfile();
    if (!srgb_profile) {
        cmsCloseProfile(input_profile);
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Create BCHSW abstract profile using values from input data
    cmsUInt32Number lut_points = (data[0] % 50) + 2;  // Between 2-51 points
    cmsFloat64Number bright = (data[1] % 200 - 100) / 100.0;  // -1.0 to 1.0
    cmsFloat64Number contrast = (data[2] % 200 - 100) / 100.0 + 1.0;  // 0.0 to 2.0
    cmsFloat64Number hue = (data[3] % 360) * 3.14159 / 180.0;  // 0 to 2π radians
    cmsFloat64Number saturation = (data[4] % 200) / 100.0;  // 0.0 to 2.0
    cmsUInt32Number temp_src = 5000 + (data[5] % 5000);  // 5000-10000K
    cmsUInt32Number temp_dest = 5000 + (data[6] % 5000);  // 5000-10000K
    
    cmsHPROFILE bchsw_profile = cmsCreateBCHSWabstractProfile(
        lut_points, bright, contrast, hue, saturation, temp_src, temp_dest);
    
    // 6. Get profile info
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cmsUInt32Number info_size = cmsGetProfileInfoASCII(
        input_profile, cmsInfoDescription, "en", "US", buffer, sizeof(buffer));
    
    // 7. Get header flags
    cmsUInt32Number flags = cmsGetHeaderFlags(input_profile);
    
    // 8. Read profile sequence description if present
    cmsSEQ *seq_desc = (cmsSEQ*)cmsReadTag(input_profile, cmsSigProfileSequenceDescTag);
    if (seq_desc) {
        cmsFreeProfileSequenceDescription(seq_desc);
    }
    
    // 9. Create transform from input to sRGB
    cmsHTRANSFORM transform1 = cmsCreateTransformTHR(
        ctx, input_profile, TYPE_RGB_8, srgb_profile, TYPE_RGB_8, 
        INTENT_PERCEPTUAL, cmsFLAGS_NOCACHE);
    
    // 10. Create transform from sRGB to BCHSW
    cmsHTRANSFORM transform2 = nullptr;
    if (bchsw_profile) {
        transform2 = cmsCreateTransformTHR(
            ctx, srgb_profile, TYPE_RGB_8, bchsw_profile, TYPE_RGB_8,
            INTENT_PERCEPTUAL, cmsFLAGS_NOCACHE);
    }
    
    // 11. Create and test pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline) {
        // 12. Get first stage pointer
        cmsStage *first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
        
        // 13. Check and retrieve stages (dummy test with n=0)
        cmsStage *stage_array[1] = {nullptr};
        cmsBool has_stages = cmsPipelineCheckAndRetreiveStages(pipeline, 0);
        
        if (first_stage) {
            // 14. Get stage channels
            cmsUInt32Number in_channels = cmsStageInputChannels(first_stage);
            cmsUInt32Number out_channels = cmsStageOutputChannels(first_stage);
        }
        
        // 15. Free pipeline
        cmsPipelineFree(pipeline);
    }
    
    // 16. Build gamma curve and free it
    cmsFloat64Number gamma = 2.2;
    cmsToneCurve *gamma_curve = cmsBuildGamma(ctx, gamma);
    if (gamma_curve) {
        cmsFreeToneCurve(gamma_curve);
    }
    
    // 17. Allocate and free IT8 handle
    cmsHANDLE it8_handle = cmsIT8Alloc(ctx);
    if (it8_handle) {
        cmsIT8Free(it8_handle);
    }
    
    // 18. Get alarm codes
    cmsUInt16Number alarm_codes[16];
    cmsGetAlarmCodes(alarm_codes);
    
    // 19. Test transforms with sample data if available
    if (transform1 && size >= 1024 + 9) {  // Need extra data for pixel testing
        const uint8_t *pixel_data = data + 1024;
        size_t pixel_count = (size - 1024) / 3;
        if (pixel_count > 0) {
            uint8_t input_pixel[3];
            uint8_t output_pixel[3];
            
            // Test first few pixels
            for (size_t i = 0; i < pixel_count && i < 10; i++) {
                memcpy(input_pixel, pixel_data + i*3, 3);
                cmsDoTransform(transform1, input_pixel, output_pixel, 1);
            }
        }
    }
    
    // 20. Clean up resources
    if (transform2) cmsDeleteTransform(transform2);
    if (transform1) cmsDeleteTransform(transform1);
    if (bchsw_profile) cmsCloseProfile(bchsw_profile);
    cmsCloseProfile(srgb_profile);
    cmsCloseProfile(input_profile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}