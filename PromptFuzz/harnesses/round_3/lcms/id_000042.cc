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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 12
//<Quality> {"density":22,"unique_branches":{"Type_S15Fixed16_Write":[[781,15,781,25,0,0,4,0],[781,15,781,25,0,0,4,1],[783,13,783,52,0,0,4,1]],"SaveOneChromaticity":[[457,9,457,79,0,0,4,1],[458,9,458,79,0,0,4,1]],"cmsCreateBCHSWabstractProfileTHR":[[882,9,882,28,0,0,4,0]],"Type_Chromaticity_Write":[[468,9,468,38,0,0,4,1],[469,9,469,38,0,0,4,1],[471,9,471,65,0,0,4,1],[472,9,472,67,0,0,4,1],[473,9,473,66,0,0,4,1]],"bchswSampler":[[845,9,845,25,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreate_sRGBProfile","cmsBuildTabulatedToneCurveFloat","cmsD50_xyY","cmsCreateGrayProfile","cmsMLUalloc","cmsMLUtranslationsCodes","cmsGetProfileInfoASCII","cmsTagLinkedTo","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8LoadFromMem","cmsIT8Free","cmsIT8Free","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsDetectTAC","cmsPipelineEval16","cmsPipelineStageCount","cmsCreateBCHSWabstractProfileTHR","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsCreateTransform","cmsGetNamedColorList","cmsNamedColorCount","cmsTempFromWhitePoint","cmsGetColorSpace","cmsChannelsOf","cmsSaveProfileToMem","cmsFreeNamedColorList","cmsDeleteTransform","cmsPipelineFree","cmsStageFree","cmsMLUfree","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsGetPostScriptCSA","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreate_sRGBProfile","cmsBuildTabulatedToneCurveFloat","cmsD50_xyY","cmsCreateGrayProfile","cmsMLUalloc","cmsMLUtranslationsCodes","cmsGetProfileInfoASCII","cmsTagLinkedTo","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8LoadFromMem","cmsIT8Free","cmsIT8Free","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsDetectTAC","cmsPipelineEval16","cmsPipelineStageCount","cmsCreateBCHSWabstractProfileTHR","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsCreateTransform","cmsGetNamedColorList","cmsNamedColorCount","cmsTempFromWhitePoint","cmsGetColorSpace","cmsChannelsOf","cmsSaveProfileToMem","cmsFreeNamedColorList","cmsDeleteTransform","cmsPipelineFree","cmsMLUfree","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsGetPostScriptCSA","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 2. Try to open profile from memory (primary use of input data)
    cmsHPROFILE inputProfile = cmsOpenProfileFromMem(data, size);
    
    // 3. Create sRGB profile
    cmsHPROFILE srgbProfile = cmsCreate_sRGBProfile();
    
    // 4. Create tone curve from input data
    std::vector<cmsFloat32Number> curveValues;
    size_t curveEntries = size / sizeof(cmsFloat32Number);
    if (curveEntries > 256) curveEntries = 256;
    if (curveEntries > 0) {
        curveValues.resize(curveEntries);
        memcpy(curveValues.data(), data, curveEntries * sizeof(cmsFloat32Number));
        cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurveFloat(context, 
            static_cast<cmsUInt32Number>(curveEntries), curveValues.data());
        
        // 5. Get D50 white point
        const cmsCIExyY* d50 = cmsD50_xyY();
        
        // 6. Create gray profile with D50 and tone curve
        cmsHPROFILE grayProfile = cmsCreateGrayProfile(d50, toneCurve);
        
        // 7. Allocate MLU
        cmsMLU* mlu = cmsMLUalloc(context, 10);
        
        // 8. Get translation codes from MLU
        char langCode[3] = {0};
        char countryCode[3] = {0};
        if (mlu) {
            cmsMLUtranslationsCodes(mlu, 0, langCode, countryCode);
        }
        
        // 9. Get profile info (using safe indices)
        if (inputProfile) {
            char buffer[256];
            size_t infoSize = size % 256;
            cmsGetProfileInfoASCII(inputProfile, cmsInfoDescription, 
                                  langCode, countryCode, buffer, 
                                  static_cast<cmsUInt32Number>(infoSize));
        }
        
        // 10. Get tag linkage
        if (inputProfile) {
            cmsTagSignature linked = cmsTagLinkedTo(inputProfile, cmsSigMediaWhitePointTag);
        }
        
        // 11. Allocate IT8 handle
        cmsHANDLE it8 = cmsIT8Alloc(context);
        
        // 12. Define IT8 format
        if (it8) {
            cmsIT8DefineDblFormat(it8, "%f");
        }
        
        // 13. Try to load IT8 from memory (secondary use of input data)
        cmsHANDLE it8Loaded = cmsIT8LoadFromMem(context, data, static_cast<cmsUInt32Number>(size));
        
        // 14. Free IT8 handles
        if (it8) cmsIT8Free(it8);
        if (it8Loaded) cmsIT8Free(it8Loaded);
        
        // 15. Create pipeline
        cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
        
        // 16. Create tone curves array for stage
        cmsToneCurve* curves[3] = {toneCurve, toneCurve, toneCurve};
        
        // 17. Create stage with tone curves
        cmsStage* stage = cmsStageAllocToneCurves(context, 3, curves);
        
        // 18. Insert stage into pipeline
        if (pipeline && stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage);
        }
        
        // 19. Detect TAC if we have a valid profile
        if (inputProfile) {
            cmsDetectTAC(inputProfile);
        }
        
        // 20. Evaluate pipeline
        if (pipeline) {
            cmsUInt16Number in[3] = {0x8000, 0x8000, 0x8000};
            cmsUInt16Number out[3] = {0};
            cmsPipelineEval16(in, out, pipeline);
        }
        
        // 21. Get pipeline stage count
        if (pipeline) {
            cmsPipelineStageCount(pipeline);
        }
        
        // 22. Create BCHS abstract profile (using data-derived parameters)
        size_t lutPoints = (size % 100) + 2;
        double brightness = static_cast<double>(data[0]) / 255.0;
        double contrast = static_cast<double>(data[1]) / 255.0;
        double hue = static_cast<double>(data[2]) / 255.0 * 360.0;
        double saturation = static_cast<double>(data[3]) / 255.0;
        cmsUInt32Number tempSrc = 5000 + (data[4] % 5000);
        cmsUInt32Number tempDest = 5000 + (data[5] % 5000);
        
        cmsHPROFILE bchsProfile = cmsCreateBCHSWabstractProfileTHR(context,
            static_cast<cmsUInt32Number>(lutPoints),
            brightness, contrast, hue, saturation,
            tempSrc, tempDest);
        
        // 23. Get first stage from pipeline
        if (pipeline) {
            cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            
            // 24. Get output channels from stage
            if (firstStage) {
                cmsStageOutputChannels(firstStage);
            }
        }
        
        // 25. Create transform if we have profiles
        cmsHTRANSFORM transform = nullptr;
        if (inputProfile && srgbProfile) {
            transform = cmsCreateTransform(inputProfile, TYPE_RGB_8,
                                          srgbProfile, TYPE_RGB_8,
                                          INTENT_PERCEPTUAL, 0);
        }
        
        // 26. Get named color list from transform
        cmsNAMEDCOLORLIST* namedColors = nullptr;
        if (transform) {
            namedColors = cmsGetNamedColorList(transform);
        }
        
        // 27. Get named color count
        if (namedColors) {
            cmsNamedColorCount(namedColors);
        }
        
        // 28. Get temperature from white point
        cmsFloat64Number tempK = 0.0;
        cmsTempFromWhitePoint(&tempK, d50);
        
        // 29. Get color space of profile
        if (inputProfile) {
            cmsColorSpaceSignature cs = cmsGetColorSpace(inputProfile);
            
            // 30. Get channels of color space
            cmsChannelsOf(cs);
        }
        
        // 31. Save profile to memory
        if (srgbProfile) {
            std::vector<uint8_t> profileBuffer(size);
            cmsUInt32Number bytesNeeded = 0;
            cmsSaveProfileToMem(srgbProfile, profileBuffer.data(), &bytesNeeded);
        }
        
        // 32. Free named color list
        if (namedColors) {
            cmsFreeNamedColorList(namedColors);
        }
        
        // 33. Delete transform
        if (transform) {
            cmsDeleteTransform(transform);
        }
        
        // 34. Free pipeline
        if (pipeline) {
            cmsPipelineFree(pipeline);
        } else {
            // If pipeline wasn't created, free stage separately
            if (stage) cmsStageFree(stage);
        }
        
        // 35. Free MLU
        if (mlu) {
            cmsMLUfree(mlu);
        }
        
        // 36. Free tone curve
        if (toneCurve) {
            cmsFreeToneCurve(toneCurve);
        }
        
        // 37. Close profiles
        if (grayProfile) cmsCloseProfile(grayProfile);
        if (bchsProfile) cmsCloseProfile(bchsProfile);
        
        // 38. Get PostScript CSA
        if (inputProfile) {
            std::vector<uint8_t> csaBuffer(4096);
            cmsUInt32Number csaSize = cmsGetPostScriptCSA(context, inputProfile,
                INTENT_PERCEPTUAL, 0, csaBuffer.data(),
                static_cast<cmsUInt32Number>(csaBuffer.size()));
        }
    }
    
    // 39. Close profiles
    if (srgbProfile) cmsCloseProfile(srgbProfile);
    if (inputProfile) cmsCloseProfile(inputProfile);
    
    // 40. Delete context
    cmsDeleteContext(context);
    
    return 0;
}