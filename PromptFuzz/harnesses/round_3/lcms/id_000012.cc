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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsMLUalloc","cmsMLUtranslationsCodes","cmsTagLinkedTo","cmsBuildTabulatedToneCurveFloat","cmsCreateRGBProfileTHR","cmsCreateTransform","cmsPipelineAlloc","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsPipelineEval16","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsGetSupportedIntents","cmsTempFromWhitePoint","cmsGetColorSpace","cmsChannelsOf","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsMLUfree","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsMLUalloc","cmsMLUtranslationsCodes","cmsTagLinkedTo","cmsBuildTabulatedToneCurveFloat","cmsCreateRGBProfileTHR","cmsCreateTransform","cmsPipelineAlloc","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsPipelineEval16","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsGetSupportedIntents","cmsTempFromWhitePoint","cmsGetColorSpace","cmsChannelsOf","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsMLUfree","cmsCloseProfile"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Too small for meaningful processing
    
    // 1. Open profile from memory using fuzzer input
    cmsHPROFILE hInProfile = cmsOpenProfileFromMem(data, size);
    if (!hInProfile) {
	return 0;
	}
	
    
    // 2. Get profile information
    wchar_t buffer[256];
    cmsUInt32Number infoSize = cmsGetProfileInfo(hInProfile, cmsInfoDescription, "EN", "US", buffer, 256);
    
    // 3. Allocate MLU structure
    cmsMLU *mlu = cmsMLUalloc(NULL, 10);
    if (mlu) {
        char langCode[3], countryCode[3];
        // Try to get translation codes (this may fail if MLU is empty)
        cmsMLUtranslationsCodes(mlu, 0, langCode, countryCode);
    }
    
    // 4. Check for linked tags
    cmsTagSignature linked = cmsTagLinkedTo(hInProfile, cmsSigMediaWhitePointTag);
    
    // 5. Create tabulated tone curve from data
    // Extract some floats from input data for the tone curve
    const size_t curvePoints = 256;
    std::vector<cmsFloat32Number> curveValues;
    if (size >= curvePoints * sizeof(float)) {
        curveValues.resize(curvePoints);
        memcpy(curveValues.data(), data + size - curvePoints * sizeof(float), 
               curvePoints * sizeof(float));
        for (auto& v : curveValues) {
            // Normalize to 0-1 range
            uint32_t intVal;
            memcpy(&intVal, &v, sizeof(float));
            intVal &= 0x7FFFFFFF; // Keep positive
            v = (cmsFloat32Number)(intVal % 1000) / 1000.0f;
        }
    } else {
        // Fallback to simple curve
        curveValues = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
    }
    
    cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurveFloat(NULL, 
        static_cast<cmsUInt32Number>(curveValues.size()), curveValues.data());
    
    // 6. Create RGB profile
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0}; // D65
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},  // Red
        {0.3000, 0.6000, 1.0},  // Green  
        {0.1500, 0.0600, 1.0}   // Blue
    };
    
    cmsToneCurve* transferFuncs[3] = {toneCurve, toneCurve, toneCurve};
    cmsHPROFILE hRGBProfile = cmsCreateRGBProfileTHR(NULL, &whitePoint, &primaries, transferFuncs);
    
    // 7. Create transform between profiles
    cmsHTRANSFORM transform = NULL;
    if (hRGBProfile) {
        transform = cmsCreateTransform(hInProfile, TYPE_RGB_8, 
                                      hRGBProfile, TYPE_RGB_8, 
                                      INTENT_PERCEPTUAL, 0);
    }
    
    // 8. Create pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(NULL, 3, 3);
    
    // 9. Get named color list from transform
    cmsNAMEDCOLORLIST* namedColors = NULL;
    if (transform) {
        namedColors = cmsGetNamedColorList(transform);
    }
    
    // 10. Get named color count
    cmsUInt32Number colorCount = 0;
    if (namedColors) {
        colorCount = cmsNamedColorCount(namedColors);
        
        // 11. Get named color info for first few colors
        for (cmsUInt32Number i = 0; i < colorCount && i < 5; ++i) {
            char name[256], prefix[256], suffix[256];
            cmsUInt16Number pcs[3], colorant[16];
            cmsNamedColorInfo(namedColors, i, name, prefix, suffix, pcs, colorant);
        }
    }
    
    // 12. Evaluate pipeline with some input
    if (pipeline) {
        cmsUInt16Number in[3] = {0x8000, 0x4000, 0x2000};
        cmsUInt16Number out[3];
        cmsPipelineEval16(in, out, pipeline);
    }
    
    // 13. Get stage output channels (if pipeline has stages)
    if (pipeline && cmsPipelineStageCount(pipeline) > 0) {
        cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
        cmsUInt32Number outChannels = cmsStageOutputChannels(firstStage);
    }
    
    // 14. Get supported intents
    cmsUInt32Number intents[10];
    char* descriptions[10];
    cmsUInt32Number numIntents = cmsGetSupportedIntents(10, intents, descriptions);
    
    // 15. Get temperature from white point
    cmsFloat64Number tempK;
    cmsTempFromWhitePoint(&tempK, &whitePoint);
    
    // 16. Get color space of input profile
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hInProfile);
    
    // 17. Get channels of color space
    cmsUInt32Number channels = cmsChannelsOf(colorSpace);
    
    // Cleanup
    if (pipeline) cmsPipelineFree(pipeline);
    if (transform) cmsDeleteTransform(transform);
    if (hRGBProfile) cmsCloseProfile(hRGBProfile);
    if (toneCurve) cmsFreeToneCurve(toneCurve);
    if (mlu) cmsMLUfree(mlu);
    if (hInProfile) cmsCloseProfile(hInProfile);
    
    return 0;
}