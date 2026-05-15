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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreate_sRGBProfile","cmsBuildTabulatedToneCurveFloat","cmsD50_xyY","cmsCreateGrayProfile","cmsMLUalloc","cmsGetProfileInfoASCII","cmsMLUtranslationsCodes","cmsTagLinkedTo","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8LoadFromMem","cmsIT8Free","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsCreateBCHSWabstractProfileTHR","cmsCreateTransform","cmsGetNamedColorList","cmsNamedColorCount","cmsTempFromWhitePoint","cmsGetColorSpace","cmsChannelsOf","cmsSaveProfileToMem","cmsDetectTAC","cmsGetPostScriptCSA","cmsFreeNamedColorList","cmsDeleteTransform","cmsPipelineFree","cmsIT8Free","cmsMLUfree","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreate_sRGBProfile","cmsBuildTabulatedToneCurveFloat","cmsD50_xyY","cmsCreateGrayProfile","cmsMLUalloc","cmsGetProfileInfoASCII","cmsMLUtranslationsCodes","cmsTagLinkedTo","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8LoadFromMem","cmsIT8Free","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsCreateBCHSWabstractProfileTHR","cmsCreateTransform","cmsGetNamedColorList","cmsNamedColorCount","cmsTempFromWhitePoint","cmsGetColorSpace","cmsChannelsOf","cmsSaveProfileToMem","cmsDetectTAC","cmsGetPostScriptCSA","cmsFreeNamedColorList","cmsDeleteTransform","cmsPipelineFree","cmsIT8Free","cmsMLUfree","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// Include lcms headers

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimum size for basic operations
    if (size < 16) {
	return 0;
	}
	
    
    cmsContext context = nullptr;
    cmsHPROFILE profile1 = nullptr, profile2 = nullptr, profile3 = nullptr, profile4 = nullptr;
    cmsToneCurve* toneCurve = nullptr;
    cmsMLU* mlu = nullptr;
    cmsHANDLE it8 = nullptr;
    cmsPipeline* pipeline = nullptr;
    cmsStage* stage = nullptr;
    cmsHTRANSFORM transform = nullptr;
    cmsNAMEDCOLORLIST* namedColorList = nullptr;
    
    // 1. Create context
    context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Try to open profile from memory (using first part of input)
    size_t profileSize = size > 1024 ? 1024 : size;
    profile1 = cmsOpenProfileFromMem(data, profileSize);
    
    // 3. Create sRGB profile
    profile2 = cmsCreate_sRGBProfile();
    
    // 4. Create tone curve from input data
    if (size >= 256) {
        cmsFloat32Number values[64];
        size_t valuesCount = size < 256 ? size / 4 : 64;
        for (size_t i = 0; i < valuesCount && i < 64; i++) {
            values[i] = (cmsFloat32Number)data[i] / 255.0f;
        }
        toneCurve = cmsBuildTabulatedToneCurveFloat(context, valuesCount, values);
    }
    
    // 5. Get D50 white point and create gray profile
    const cmsCIExyY* d50 = cmsD50_xyY();
    profile3 = cmsCreateGrayProfile(d50, toneCurve);
    
    // 6. Allocate MLU
    mlu = cmsMLUalloc(context, 10);
    
    // 7. Get profile info if profile1 was successfully opened
    if (profile1 && mlu) {
        char buffer[256];
        cmsUInt32Number bufSize = sizeof(buffer);
        cmsGetProfileInfoASCII(profile1, cmsInfoDescription, "en", "US", 
                               buffer, bufSize);
        
        // Try to get MLU translation codes
        char lang[3], country[3];
        cmsMLUtranslationsCodes(mlu, 0, lang, country);
    }
    
    // 8. Check tag linking if profile exists
    if (profile1) {
        cmsTagSignature linked = cmsTagLinkedTo(profile1, cmsSigMediaWhitePointTag);
        (void)linked; // Use to avoid unused variable warning
    }
    
    // 9. IT8 handling
    it8 = cmsIT8Alloc(context);
    if (it8) {
        cmsIT8DefineDblFormat(it8, "%.4f");
        
        // Try to load IT8 from remaining input data
        if (size > profileSize) {
            size_t it8Size = size - profileSize;
            cmsHANDLE loadedIT8 = cmsIT8LoadFromMem(context, data + profileSize, it8Size);
            if (loadedIT8) {
                cmsIT8Free(loadedIT8);
            }
        }
    }
    
    // 10. Pipeline operations
    pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline && toneCurve) {
        cmsToneCurve* curves[3] = {toneCurve, toneCurve, toneCurve};
        stage = cmsStageAllocToneCurves(context, 3, curves);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage);
            
            // Evaluate pipeline
            cmsUInt16Number in[3] = {0x8000, 0x4000, 0x2000};
            cmsUInt16Number out[3];
            cmsPipelineEval16(in, out, pipeline);
            
            // Get pipeline stage count
            cmsUInt32Number stageCount = cmsPipelineStageCount(pipeline);
            (void)stageCount;
            
            // Get pointer to first stage
            cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (firstStage) {
                cmsUInt32Number outputChannels = cmsStageOutputChannels(firstStage);
                (void)outputChannels;
            }
        }
    }
    
    // 11. Create abstract profile
    profile4 = cmsCreateBCHSWabstractProfileTHR(context, 17, 1.0, 1.0, 0.0, 1.0, 
                                                6500, 5000);
    
    // 12. Create transform and get named colors
    if (profile1 && profile2) {
        transform = cmsCreateTransform(profile1, TYPE_RGB_8, profile2, TYPE_RGB_8, 
                                       INTENT_PERCEPTUAL, 0);
        if (transform) {
            namedColorList = cmsGetNamedColorList(transform);
            if (namedColorList) {
                cmsUInt32Number colorCount = cmsNamedColorCount(namedColorList);
                (void)colorCount;
            }
        }
    }
    
    // 13. Temperature from white point
    cmsFloat64Number tempK;
    if (d50) {
        cmsTempFromWhitePoint(&tempK, d50);
    }
    
    // 14. Get color space and channels
    if (profile1) {
        cmsColorSpaceSignature colorSpace = cmsGetColorSpace(profile1);
        cmsUInt32Number channels = cmsChannelsOf(colorSpace);
        (void)channels;
    }
    
    // 15. Save profile to memory
    if (profile3) {
        void* buffer = malloc(4096);
        if (buffer) {
            cmsUInt32Number bytesNeeded;
            cmsSaveProfileToMem(profile3, buffer, &bytesNeeded);
            free(buffer);
        }
    }
    
    // 16. Detect TAC
    if (profile1) {
        cmsFloat64Number tac = cmsDetectTAC(profile1);
        (void)tac;
    }
    
    // 17. Get PostScript CSA
    if (profile1) {
        void* buffer = malloc(4096);
        if (buffer) {
            cmsUInt32Number bytesWritten = cmsGetPostScriptCSA(context, profile1, 
                                                               INTENT_PERCEPTUAL, 0,
                                                               buffer, 4096);
            (void)bytesWritten;
            free(buffer);
        }
    }
    
    // Cleanup in reverse order
    if (namedColorList) {
        cmsFreeNamedColorList(namedColorList);
    }
    
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (it8) {
        cmsIT8Free(it8);
    }
    
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }
    
    if (profile1) {
        cmsCloseProfile(profile1);
    }
    
    if (profile2) {
        cmsCloseProfile(profile2);
    }
    
    if (profile3) {
        cmsCloseProfile(profile3);
    }
    
    if (profile4) {
        cmsCloseProfile(profile4);
    }
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    return 0;
}