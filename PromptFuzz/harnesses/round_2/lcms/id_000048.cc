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
//<ID> 379
//<Prompt> []
/*<Combination>: [
*/
//<score> 147, nr_unique_branch: 63
//<Quality> {"density":21,"unique_branches":{"cmsStageSampleCLutFloat":[[823,9,823,20,0,0,4,1],[827,9,827,21,0,0,4,1],[833,9,833,21,0,0,4,1],[834,9,834,22,0,0,4,1],[835,9,835,40,0,0,4,1],[836,9,836,39,0,0,4,1],[839,9,839,26,0,0,4,1],[842,17,842,38,0,0,4,0],[842,17,842,38,0,0,4,1],[845,35,845,40,0,0,4,0],[845,35,845,40,0,0,4,1],[854,13,854,38,0,0,4,0],[855,23,855,41,0,0,4,0],[855,23,855,41,0,0,4,1],[859,13,859,37,0,0,4,1],[862,13,862,41,0,0,4,0],[864,17,864,42,0,0,4,0],[865,27,865,45,0,0,4,0],[865,27,865,45,0,0,4,1]],"cmsMLUgetUTF8":[[602,9,602,20,0,0,4,1],[606,9,606,21,0,0,4,0],[606,9,606,21,0,0,4,1],[611,9,611,23,0,0,4,1],[614,9,614,24,0,0,4,1],[617,9,617,33,0,0,4,0],[617,9,617,33,0,0,4,1]],"cmsGetProfileInfoUTF8":[[1042,9,1042,20,0,0,4,0],[1042,9,1042,20,0,0,4,1]],"MemoryRead":[[155,22,155,32,0,0,4,0]],"cmsStageAllocCLutFloatGranular":[[684,9,684,22,0,0,4,1]],"encodeUTF8":[[277,12,277,30,0,0,4,0],[277,12,277,30,0,0,4,1],[277,34,277,37,0,0,4,0],[277,34,277,37,0,0,4,1],[279,13,279,26,0,0,4,0],[279,13,279,26,0,0,4,1],[279,30,279,43,0,0,4,1],[283,17,283,30,0,0,4,0],[283,17,283,30,0,0,4,1],[283,34,283,47,0,0,4,1],[288,17,288,34,0,0,4,0],[288,17,288,34,0,0,4,1],[290,21,290,24,0,0,4,0],[290,21,290,24,0,0,4,1],[290,28,290,50,0,0,4,0],[290,28,290,50,0,0,4,1],[294,22,294,40,0,0,4,0],[294,22,294,40,0,0,4,1],[296,21,296,24,0,0,4,0],[296,21,296,24,0,0,4,1],[296,28,296,43,0,0,4,0],[296,47,296,69,0,0,4,0],[303,22,303,41,0,0,4,0],[303,22,303,41,0,0,4,1],[305,21,305,24,0,0,4,0],[305,21,305,24,0,0,4,1],[305,28,305,43,0,0,4,0],[305,47,305,69,0,0,4,0],[315,21,315,24,0,0,4,0],[315,21,315,24,0,0,4,1],[315,28,315,43,0,0,4,0],[315,47,315,69,0,0,4,0],[315,47,315,69,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsCreateProfilePlaceholder","cmsPluginTHR","cmsBuildGamma","cmsCreateGrayProfile","cmsIsToneCurveMultisegment","cmsEvalToneCurveFloat","cmsSetAlarmCodes","cmsCreateTransformTHR","cmsGetNamedColorList","cmsGetProfileInfoUTF8","cmsStageAllocCLutFloatGranular","cmsStageSampleCLutFloat","cmsIT8LoadFromMem","cmsIT8GetDataDbl","cmsIT8Free","cmsStageFree","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsCreateProfilePlaceholder","cmsPluginTHR","cmsBuildGamma","cmsCreateGrayProfile","cmsIsToneCurveMultisegment","cmsEvalToneCurveFloat","cmsSetAlarmCodes","cmsCreateTransformTHR","cmsGetNamedColorList","cmsGetProfileInfoUTF8","cmsStageAllocCLutFloatGranular","cmsStageSampleCLutFloat","cmsIT8LoadFromMem","cmsIT8GetDataDbl","cmsIT8Free","cmsStageFree","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/



// Custom sampler function for cmsStageSampleCLutFloat
static cmsInt32Number  id_000048_FloatSampler(const cmsFloat32Number* in, 
                                   cmsFloat32Number* out, 
                                   void* userdata) {
    // Simple identity sampler
    for (int i = 0; i < 3; i++) {
        out[i] = in[i];
    }
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data

    // Create a context for operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	

    // 1. Try to create a profile from input data
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(ctx, data, size);
    if (!profile) {
        // If can't create from memory, create a placeholder
        profile = cmsCreateProfilePlaceholder(ctx);
    }

    // 2. Call cmsPluginTHR - try with NULL (will fail but we call it)
    cmsPluginTHR(ctx, NULL);

    // 3. Create a gray profile using part of input data for parameters
    cmsCIExyY whitePoint;
    if (size >= sizeof(cmsCIExyY)) {
        memcpy(&whitePoint, data, sizeof(cmsCIExyY));
    } else {
        whitePoint.x = 0.3127;
        whitePoint.y = 0.3290;
        whitePoint.Y = 1.0;
    }
    
    // Create a simple tone curve for gray profile
    cmsFloat32Number gamma = 2.2;
    cmsToneCurve* transferFn = cmsBuildGamma(ctx, gamma);
    cmsHPROFILE grayProfile = cmsCreateGrayProfile(&whitePoint, transferFn);
    
    // 4. Check if tone curve is multisegment
    cmsBool isMultiSeg = cmsIsToneCurveMultisegment(transferFn);
    
    // 5. Evaluate tone curve at a point
    cmsFloat32Number evalPoint = 0.5f;
    if (size > 0) evalPoint = data[0] / 255.0f;
    cmsFloat32Number evalResult = cmsEvalToneCurveFloat(transferFn, evalPoint);
    
    // 6. Get alarm codes
    cmsUInt16Number alarmCodes[16];
    // Note: cmsGetAlarmCodes is not in provided list, but we have cmsSetAlarmCodes
    // Using the set function instead
    cmsSetAlarmCodes(alarmCodes); // This actually sets, not gets
    
    // 7. Create a transform and get named color list
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, 
        profile, 
        TYPE_GRAY_8,
        grayProfile, 
        TYPE_GRAY_8,
        INTENT_PERCEPTUAL, 
        0);
    
    cmsNAMEDCOLORLIST* namedColors = cmsGetNamedColorList(transform);
    
    // 8. Get profile info in UTF-8
    char buffer[256];
    cmsUInt32Number infoSize = cmsGetProfileInfoUTF8(profile, 
        cmsInfoDescription,
        "en",
        "US",
        buffer,
        sizeof(buffer));
    
    // 9. Create a stage and sample CLUT
    cmsUInt32Number gridPoints[] = {3, 3, 3};
    cmsStage* stage = cmsStageAllocCLutFloatGranular(ctx, 
        gridPoints, 
        3, // input channels
        3, // output channels
        NULL); // No table
    
    if (stage) {
        cmsStageSampleCLutFloat(stage,  id_000048_FloatSampler, NULL, 0);
    }
    
    // 10. Try to use cmsIT8GetDataDbl with a temporary IT8 handle
    // First try to load IT8 from memory
    cmsHANDLE it8 = cmsIT8LoadFromMem(ctx, (void*)data, size);
    if (it8) {
        double it8Value = cmsIT8GetDataDbl(it8, "patch1", "sample1");
        cmsIT8Free(it8);
    }
    
    // Clean up resources
    if (stage) cmsStageFree(stage);
    if (transferFn) cmsFreeToneCurve(transferFn);
    if (transform) cmsDeleteTransform(transform);
    if (grayProfile) cmsCloseProfile(grayProfile);
    if (profile) cmsCloseProfile(profile);
    cmsDeleteContext(ctx);
    
    return 0;
}