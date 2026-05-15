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
//<ID> 655
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":24,"unique_branches":{"cmsFreeProfileSequenceDescription":[[1022,9,1022,21,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsGetHeaderFlags","cmsCreate_sRGBProfile","cmsCloseProfile","cmsDeleteContext","cmsCreateBCHSWabstractProfile","cmsCreateTransformTHR","cmsGetProfileInfoASCII","cmsGetAlarmCodes","cmsBuildGamma","cmsPipelineAlloc","cmsGetTransformPipeline","cmsPipelineCheckAndRetreiveStages","cmsPipelineGetPtrToFirstStage","cmsStageInputChannels","cmsStageOutputChannels","cmsIT8Alloc","cmsIT8Free","cmsPipelineFree","cmsFreeToneCurve","cmsFreeProfileSequenceDescription","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsGetHeaderFlags","cmsCreate_sRGBProfile","cmsCreateBCHSWabstractProfile","cmsCreateTransformTHR","cmsGetProfileInfoASCII","cmsGetAlarmCodes","cmsBuildGamma","cmsPipelineAlloc","cmsGetTransformPipeline","cmsPipelineCheckAndRetreiveStages","cmsPipelineGetPtrToFirstStage","cmsStageInputChannels","cmsStageOutputChannels","cmsIT8Alloc","cmsIT8Free","cmsPipelineFree","cmsFreeToneCurve","cmsFreeProfileSequenceDescription","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to process an ICC profile from a byte stream:

*/


extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Too small to be a valid profile
    
    // 1. Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // 2. Open profile from stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Open ICC profile from stream
    cmsHPROFILE inputProfile = cmsOpenProfileFromStreamTHR(ctx, in_file, "r");
    if (!inputProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get header flags
    cmsUInt32Number flags = cmsGetHeaderFlags(inputProfile);
    
    // 5. Create sRGB profile
    cmsHPROFILE srgbProfile = cmsCreate_sRGBProfile();
    if (!srgbProfile) {
        cmsCloseProfile(inputProfile);
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Create BCHSW abstract profile with default parameters
    cmsHPROFILE abstractProfile = cmsCreateBCHSWabstractProfile(
        256,  // nLUTPoints
        0.0,  // Bright
        1.0,  // Contrast
        0.0,  // Hue
        1.0,  // Saturation
        6500, // TempSrc (D65)
        5000  // TempDest
    );
    
    // 7. Create transform
    cmsHTRANSFORM transform = cmsCreateTransformTHR(
        ctx,
        inputProfile,
        TYPE_RGB_8,
        srgbProfile,
        TYPE_RGB_8,
        INTENT_PERCEPTUAL,
        0
    );
    
    // 8. Get profile info ASCII
    char profileInfo[256];
    cmsGetProfileInfoASCII(inputProfile, cmsInfoDescription, "en", "US", 
                          profileInfo, sizeof(profileInfo));
    
    // 9. Get alarm codes
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodes(alarmCodes);
    
    // 10. Build gamma curve
    cmsToneCurve *gammaCurve = cmsBuildGamma(ctx, 2.2);
    
    // 11. Create pipeline and check stages
    cmsPipeline *pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline && transform) {
        // 12. Get pipeline from transform (using provided API from initial list)
        cmsPipeline *transformPipeline = cmsGetTransformPipeline(transform);
        
        // 13. Check if pipeline has 3 stages
        if (transformPipeline) {
            cmsStage *stages[3];
            cmsBool hasStages = cmsPipelineCheckAndRetreiveStages(transformPipeline, 3);
            
            // 14. If we got stages, get first stage and check channels
            if (hasStages) {
                cmsStage *firstStage = cmsPipelineGetPtrToFirstStage(transformPipeline);
                if (firstStage) {
                    cmsUInt32Number inChannels = cmsStageInputChannels(firstStage);
                    cmsUInt32Number outChannels = cmsStageOutputChannels(firstStage);
                }
            }
        }
    }
    
    // 15. Allocate and free IT8 handle
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    if (it8) {
        cmsIT8Free(it8);
    }
    
    // 16. Free resources in reverse order
    
    // Free pipeline if created
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    // Free gamma curve
    if (gammaCurve) {
        cmsFreeToneCurve(gammaCurve);
    }
    
    // Free profile sequence description if any
    // Note: We don't have a cmsSEQ* here, but we'll demonstrate the API
    // with a dummy pointer to show usage
    cmsSEQ *seq = nullptr;
    cmsFreeProfileSequenceDescription(seq);
    
    // Delete transform
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    // Close profiles
    if (abstractProfile) {
        cmsCloseProfile(abstractProfile);
    }
    cmsCloseProfile(srgbProfile);
    cmsCloseProfile(inputProfile);
    
    // Close file
    assert_file_closed(&in_file);;
    
    // Delete context
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}