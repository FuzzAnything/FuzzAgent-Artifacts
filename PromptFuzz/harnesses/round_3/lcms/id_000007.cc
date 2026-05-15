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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 154, nr_unique_branch: 14
//<Quality> {"density":22,"unique_branches":{"cmsCreateRGBProfileTHR":[[185,13,185,55,0,0,4,1],[191,17,191,83,0,0,4,1],[194,13,194,55,0,0,4,1],[200,17,200,82,0,0,4,1]],"ComputeChromaticAdaptation":[[218,9,218,58,0,0,4,0]],"cmsTempFromWhitePoint":[[163,15,163,23,0,0,4,1]],"cmsMLUgetWide":[[640,9,640,20,0,0,4,1],[643,9,643,21,0,0,4,1],[646,9,646,23,0,0,4,1],[649,9,649,37,0,0,4,1],[652,9,652,46,0,0,4,0],[652,9,652,46,0,0,4,1]],"cmsMLUtranslationsCodes":[[705,9,705,32,0,0,4,0]],"cmsGetProfileInfo":[[1021,9,1021,20,0,0,4,1]]},"library_calls":["cmsOpenProfileFromMem","cmsReadTag","cmsReadTag","cmsXYZ2xyY","cmsTempFromWhitePoint","cmsGetSupportedIntents","cmsReadTag","cmsMLUtranslationsCodes","cmsGetProfileInfo","cmsTagLinkedTo","cmsBuildGamma","cmsCreateRGBProfileTHR","cmsCreateTransform","cmsGetTransformPipeline","cmsPipelineEvalFloat","cmsPipelineEval16","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsDeleteTransform","cmsCloseProfile","cmsGetColorSpace","cmsChannelsOf","cmsFreeToneCurve","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsReadTag","cmsReadTag","cmsXYZ2xyY","cmsTempFromWhitePoint","cmsGetSupportedIntents","cmsReadTag","cmsMLUtranslationsCodes","cmsGetProfileInfo","cmsTagLinkedTo","cmsBuildGamma","cmsCreateRGBProfileTHR","cmsCreateTransform","cmsGetTransformPipeline","cmsPipelineEvalFloat","cmsPipelineEval16","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsDeleteTransform","cmsCloseProfile","cmsGetColorSpace","cmsChannelsOf","cmsFreeToneCurve","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // 1. Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	return 0;
	}
	
    
    // 2. Get white point and convert to temperature
    cmsCIExyY whitePoint;
    cmsFloat64Number tempK = 0.0;
    if (cmsReadTag(hProfile, cmsSigMediaWhitePointTag)) {
        // Read white point from profile
        cmsCIEXYZ* mediaWhite = (cmsCIEXYZ*)cmsReadTag(hProfile, cmsSigMediaWhitePointTag);
        if (mediaWhite && mediaWhite->Z != 0.0) {
            // Convert XYZ to xyY
            cmsXYZ2xyY(&whitePoint, mediaWhite);
            cmsTempFromWhitePoint(&tempK, &whitePoint);
        }
    }
    
    // 3. Get supported intents
    cmsUInt32Number intents[10];
    char* descriptions[10];
    cmsUInt32Number numIntents = cmsGetSupportedIntents(10, intents, descriptions);
    
    // 4. Get profile info using MLU translations
    cmsMLU* mlu = (cmsMLU*)cmsReadTag(hProfile, cmsSigProfileDescriptionTag);
    if (mlu) {
        char langCode[3] = {0};
        char countryCode[3] = {0};
        wchar_t buffer[256];
        
        // Get first translation code
        if (cmsMLUtranslationsCodes(mlu, 0, langCode, countryCode)) {
            // Get profile info for this language/country
            cmsGetProfileInfo(hProfile, cmsInfoDescription, langCode, countryCode, 
                             buffer, sizeof(buffer)/sizeof(wchar_t));
        }
    }
    
    // 5. Check tag linking
    cmsTagSignature linked = cmsTagLinkedTo(hProfile, cmsSigAToB0Tag);
    
    // 6. Create a simple pipeline for evaluation
    cmsToneCurve* curves[3];
    for (int i = 0; i < 3; i++) {
        curves[i] = cmsBuildGamma(NULL, 2.2);
    }
    
    cmsHPROFILE rgbProfile = cmsCreateRGBProfileTHR(NULL, &whitePoint, NULL, curves);
    if (rgbProfile) {
        cmsHTRANSFORM transform = cmsCreateTransform(hProfile, TYPE_RGB_FLT, 
                                                    rgbProfile, TYPE_RGB_FLT, 
                                                    INTENT_PERCEPTUAL, 0);
        
        if (transform) {
            // Get pipeline from transform (internal access)
            cmsPipeline* pipeline = (cmsPipeline*)cmsGetTransformPipeline(transform);
            if (pipeline) {
                // 7. Evaluate pipeline with float and 16-bit data
                cmsFloat32Number inFloat[3] = {0.5f, 0.5f, 0.5f};
                cmsFloat32Number outFloat[3];
                cmsPipelineEvalFloat(inFloat, outFloat, pipeline);
                
                cmsUInt16Number in16[3] = {32768, 32768, 32768};
                cmsUInt16Number out16[3];
                cmsPipelineEval16(in16, out16, pipeline);
                
                // 8. Get stage output channels
                cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                if (firstStage) {
                    cmsUInt32Number outputChannels = cmsStageOutputChannels(firstStage);
                }
            }
            cmsDeleteTransform(transform);
        }
        cmsCloseProfile(rgbProfile);
    }
    
    // 9. Get channels of color space
    cmsColorSpaceSignature cs = cmsGetColorSpace(hProfile);
    cmsUInt32Number channels = cmsChannelsOf(cs);
    
    // Cleanup
    for (int i = 0; i < 3; i++) {
        if (curves[i]) cmsFreeToneCurve(curves[i]);
    }
    
    cmsCloseProfile(hProfile);
    
    return 0;
}