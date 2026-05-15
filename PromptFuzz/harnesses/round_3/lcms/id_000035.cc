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
//<ID> 243
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsBuildTabulatedToneCurveFloat","cmsCreateGrayProfile","cmsMLUalloc","cmsMLUtranslationsCodes","cmsGetProfileInfoASCII","cmsTagLinkedTo","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8Free","cmsPipelineAlloc","cmsPipelineEval16","cmsCreateTransform","cmsGetNamedColorList","cmsNamedColorCount","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsTempFromWhitePoint","cmsCreate_sRGBProfile","cmsGetColorSpace","cmsChannelsOf","cmsStageFree","cmsPipelineFree","cmsDeleteTransform","cmsFreeToneCurve","cmsMLUfree","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsBuildTabulatedToneCurveFloat","cmsCreateGrayProfile","cmsMLUalloc","cmsMLUtranslationsCodes","cmsGetProfileInfoASCII","cmsTagLinkedTo","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8Free","cmsPipelineAlloc","cmsPipelineEval16","cmsCreateTransform","cmsGetNamedColorList","cmsNamedColorCount","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsTempFromWhitePoint","cmsCreate_sRGBProfile","cmsGetColorSpace","cmsChannelsOf","cmsStageFree","cmsPipelineFree","cmsDeleteTransform","cmsFreeToneCurve","cmsMLUfree","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 1. Open profile from memory (consumes input data)
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, static_cast<cmsUInt32Number>(size));
    
    // 2. Create tone curve from tabulated data
    const int nEntries = 256;
    cmsFloat32Number values[nEntries];
    for (int i = 0; i < nEntries; i++) {
        values[i] = static_cast<cmsFloat32Number>(i) / (nEntries - 1);
    }
    cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurveFloat(context, nEntries, values);
    
    // 3. Create gray profile
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};  // D65 white point
    cmsHPROFILE grayProfile = cmsCreateGrayProfile(&whitePoint, toneCurve);
    
    // 4. Create MLU object
    cmsMLU* mlu = cmsMLUalloc(context, 10);
    
    // 5. Get MLU translation codes
    char langCode[3] = {0};
    char countryCode[3] = {0};
    if (mlu) {
        cmsMLUtranslationsCodes(mlu, 0, langCode, countryCode);
    }
    
    // 6. Get profile info ASCII
    char buffer[256] = {0};
    if (profile) {
        cmsGetProfileInfoASCII(profile, cmsInfoDescription, "en", "US", 
                               buffer, sizeof(buffer));
    }
    
    // 7. Check tag linking
    if (profile) {
        cmsTagSignature linked = cmsTagLinkedTo(profile, cmsSigMediaBlackPointTag);
    }
    
    // 8. Define IT8 double format (create dummy IT8 handle first)
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (it8) {
        cmsIT8DefineDblFormat(it8, "%.6f");
        cmsIT8Free(it8);
    }
    
    // 9. Create pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    
    // 10. Evaluate pipeline
    if (pipeline) {
        cmsUInt16Number in[3] = {0x8000, 0x8000, 0x8000};
        cmsUInt16Number out[3] = {0};
        cmsPipelineEval16(in, out, pipeline);
    }
    
    // 11. Get named color list (requires transform - create dummy one)
    cmsHTRANSFORM transform = nullptr;
    if (profile && grayProfile) {
        transform = cmsCreateTransform(profile, TYPE_RGB_8, 
                                      grayProfile, TYPE_GRAY_8, 
                                      INTENT_PERCEPTUAL, 0);
    }
    
    cmsNAMEDCOLORLIST* namedColorList = nullptr;
    if (transform) {
        namedColorList = cmsGetNamedColorList(transform);
    }
    
    // 12. Get named color count
    cmsUInt32Number colorCount = 0;
    if (namedColorList) {
        colorCount = cmsNamedColorCount(namedColorList);
    }
    
    // 13. Get pipeline stage count
    cmsUInt32Number stageCount = 0;
    if (pipeline) {
        stageCount = cmsPipelineStageCount(pipeline);
    }
    
    // 14. Get first stage of pipeline
    cmsStage* firstStage = nullptr;
    if (pipeline) {
        firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
    }
    
    // 15. Get output channels of stage
    cmsUInt32Number outputChannels = 0;
    if (firstStage) {
        outputChannels = cmsStageOutputChannels(firstStage);
    }
    
    // 16. Compute temperature from white point
    cmsFloat64Number tempK = 0.0;
    cmsTempFromWhitePoint(&tempK, &whitePoint);
    
    // 17. Create sRGB profile
    cmsHPROFILE srgbProfile = cmsCreate_sRGBProfile();
    
    // 18. Get color space
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(srgbProfile);
    
    // 19. Get channels of color space
    cmsUInt32Number channels = cmsChannelsOf(colorSpace);
    
    // Cleanup resources
    if (firstStage) {
        cmsStageFree(firstStage);
    }
    
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }
    
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    if (grayProfile) {
        cmsCloseProfile(grayProfile);
    }
    
    if (srgbProfile) {
        cmsCloseProfile(srgbProfile);
    }
    
    cmsDeleteContext(context);
    
    return 0;
}