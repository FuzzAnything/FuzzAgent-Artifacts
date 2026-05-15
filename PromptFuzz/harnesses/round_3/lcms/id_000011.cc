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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"cmsMLUgetWide":[[643,9,643,21,0,0,4,0]]},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsMLUalloc","cmsMLUtranslationsCodes","cmsMLUfree","cmsTagLinkedTo","cmsBuildTabulatedToneCurveFloat","cmsCreateRGBProfileTHR","cmsCreateTransform","cmsPipelineAlloc","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsPipelineEval16","cmsStageAllocIdentity","cmsStageOutputChannels","cmsStageFree","cmsGetSupportedIntents","cmsTempFromWhitePoint","cmsGetColorSpace","cmsChannelsOf","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsMLUalloc","cmsMLUtranslationsCodes","cmsMLUfree","cmsTagLinkedTo","cmsBuildTabulatedToneCurveFloat","cmsCreateRGBProfileTHR","cmsCreateTransform","cmsPipelineAlloc","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsPipelineEval16","cmsStageAllocIdentity","cmsStageOutputChannels","cmsStageFree","cmsGetSupportedIntents","cmsTempFromWhitePoint","cmsGetColorSpace","cmsChannelsOf","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // 1. Open profile from memory
    cmsHPROFILE hInputProfile = cmsOpenProfileFromMem(data, size);
    if (!hInputProfile) {
	return 0;
	}
	
    
    // 2. Get profile info
    wchar_t buffer[256];
    cmsUInt32Number infoSize = cmsGetProfileInfo(hInputProfile, cmsInfoDescription, 
                                                 "en", "US", buffer, sizeof(buffer)/sizeof(wchar_t));
    
    // 3. Allocate MLU and use MLU functions
    cmsMLU* mlu = cmsMLUalloc(NULL, 10);
    if (mlu) {
        char lang[3], country[3];
        cmsMLUtranslationsCodes(mlu, 0, lang, country);
        cmsMLUfree(mlu);
    }
    
    // 4. Check for linked tags
    cmsTagSignature linked = cmsTagLinkedTo(hInputProfile, cmsSigAToB0Tag);
    
    // 5. Build a tone curve
    cmsFloat32Number curveValues[5] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
    cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurveFloat(NULL, 5, curveValues);
    
    // 6. Create RGB profile
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.64, 0.33, 1.0},    // Red
        {0.30, 0.60, 1.0},    // Green
        {0.15, 0.06, 1.0}     // Blue
    };
    cmsToneCurve* transferFuncs[3] = {toneCurve, toneCurve, toneCurve};
    cmsHPROFILE hRGBProfile = cmsCreateRGBProfileTHR(NULL, &whitePoint, &primaries, transferFuncs);
    
    // 7. Create transform
    cmsHTRANSFORM hTransform = cmsCreateTransform(hInputProfile, TYPE_RGB_8, 
                                                  hRGBProfile, TYPE_RGB_8, 
                                                  INTENT_PERCEPTUAL, 0);
    
    // 8. Allocate pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(NULL, 3, 3);
    
    // 9. Get named color list
    cmsNAMEDCOLORLIST* namedColors = cmsGetNamedColorList(hTransform);
    if (namedColors) {
        // 10. Get named color count
        cmsUInt32Number colorCount = cmsNamedColorCount(namedColors);
        
        if (colorCount > 0) {
            // 11. Get named color info
            char name[256], prefix[256], suffix[256];
            cmsUInt16Number pcs[3], colorant[3];
            cmsNamedColorInfo(namedColors, 0, name, prefix, suffix, pcs, colorant);
        }
    }
    
    // 12. Evaluate pipeline
    if (pipeline) {
        cmsUInt16Number in[3] = {0, 0, 0};
        cmsUInt16Number out[3];
        cmsPipelineEval16(in, out, pipeline);
    }
    
    // 13. Get stage output channels (need a stage first)
    if (pipeline) {
        // Create a simple stage to get output channels
        cmsStage* stage = cmsStageAllocIdentity(NULL, 3);
        if (stage) {
            cmsUInt32Number outputChannels = cmsStageOutputChannels(stage);
            cmsStageFree(stage);
        }
    }
    
    // 14. Get supported intents
    cmsUInt32Number intents[10];
    char* descriptions[10];
    cmsUInt32Number numIntents = cmsGetSupportedIntents(10, intents, descriptions);
    
    // 15. Get temperature from white point
    cmsFloat64Number tempK;
    cmsTempFromWhitePoint(&tempK, &whitePoint);
    
    // 16. Get color space
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hInputProfile);
    
    // 17. Get channels of color space
    cmsUInt32Number channels = cmsChannelsOf(colorSpace);
    
    // Cleanup
    if (pipeline) cmsPipelineFree(pipeline);
    if (hTransform) cmsDeleteTransform(hTransform);
    if (hRGBProfile) cmsCloseProfile(hRGBProfile);
    if (toneCurve) cmsFreeToneCurve(toneCurve);
    if (hInputProfile) cmsCloseProfile(hInputProfile);
    
    return 0;
}