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
//<ID> 240
//<Prompt> []
/*<Combination>: [
*/
//<score> 55, nr_unique_branch: 29
//<Quality> {"density":22,"unique_branches":{"cmsMLUgetASCII":[[563,9,563,23,0,0,4,0]],"Type_Text_Description_Write":[[1229,9,1229,17,0,0,4,1],[1237,13,1237,25,0,0,4,1],[1240,13,1240,25,0,0,4,1],[1262,9,1262,45,0,0,4,1],[1263,9,1263,40,0,0,4,1],[1265,9,1265,38,0,0,4,1],[1267,9,1267,45,0,0,4,1],[1269,9,1269,49,0,0,4,1],[1272,9,1272,38,0,0,4,1],[1273,9,1273,37,0,0,4,1],[1275,9,1275,36,0,0,4,1],[1278,9,1278,42,0,0,4,0],[1278,9,1278,42,0,0,4,1],[1279,11,1279,69,0,0,4,1],[1284,9,1284,13,0,0,4,0],[1285,9,1285,13,0,0,4,0]],"cmsNamedColorInfo":[[865,9,865,53,0,0,4,1],[868,9,868,13,0,0,4,0],[869,9,869,15,0,0,4,1],[870,9,870,15,0,0,4,1],[871,9,871,12,0,0,4,0],[874,9,874,17,0,0,4,1]],"Type_ColorantTable_Write":[[3307,9,3307,44,0,0,4,1],[3309,15,3309,26,0,0,4,0],[3309,15,3309,26,0,0,4,1],[3316,13,3316,79,0,0,4,1],[3319,13,3319,38,0,0,4,1],[3320,13,3320,46,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreate_sRGBProfile","cmsBuildTabulatedToneCurveFloat","cmsD50_xyY","cmsCreateGrayProfile","cmsMLUalloc","cmsMLUtranslationsCodes","cmsGetProfileInfoASCII","cmsTagLinkedTo","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8LoadFromMem","cmsIT8Free","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsCreateTransform","cmsGetNamedColorList","cmsNamedColorCount","cmsTempFromWhitePoint","cmsGetColorSpace","cmsChannelsOf","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsFreeNamedColorList","cmsDeleteTransform","cmsPipelineFree","cmsIT8Free","cmsMLUfree","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreate_sRGBProfile","cmsBuildTabulatedToneCurveFloat","cmsD50_xyY","cmsCreateGrayProfile","cmsMLUalloc","cmsMLUtranslationsCodes","cmsGetProfileInfoASCII","cmsTagLinkedTo","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8LoadFromMem","cmsIT8Free","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsCreateTransform","cmsGetNamedColorList","cmsNamedColorCount","cmsTempFromWhitePoint","cmsGetColorSpace","cmsChannelsOf","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsFreeNamedColorList","cmsDeleteTransform","cmsPipelineFree","cmsIT8Free","cmsMLUfree","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


// lcms2 headers

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for basic operations
    if (size < 100) {
	return 0;
	}
	
    
    cmsContext context = nullptr;
    cmsHPROFILE srcProfile = nullptr;
    cmsHPROFILE grayProfile = nullptr;
    cmsHPROFILE sRGBProfile = nullptr;
    cmsToneCurve* toneCurve = nullptr;
    cmsMLU* mlu = nullptr;
    cmsPipeline* pipeline = nullptr;
    cmsHANDLE it8 = nullptr;
    cmsHTRANSFORM transform = nullptr;
    cmsNAMEDCOLORLIST* namedColorList = nullptr;
    
    // 1. Create context
    context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Try to open profile from input data
    srcProfile = cmsOpenProfileFromMem(data, size);
    
    // 3. Create sRGB profile as fallback/alternative
    sRGBProfile = cmsCreate_sRGBProfile();
    
    // 4. Create tone curve using input data
    std::vector<cmsFloat32Number> curveValues;
    size_t maxEntries = size / sizeof(cmsFloat32Number);
    if (maxEntries > 256) maxEntries = 256;
    
    curveValues.resize(maxEntries);
    for (size_t i = 0; i < maxEntries; i++) {
        // Use data to create curve values (normalize to 0-1)
        curveValues[i] = static_cast<cmsFloat32Number>(data[i % size]) / 255.0f;
    }
    
    toneCurve = cmsBuildTabulatedToneCurveFloat(context, 
                                               static_cast<cmsUInt32Number>(curveValues.size()),
                                               curveValues.data());
    
    // 5. Create gray profile with D50 white point and tone curve
    const cmsCIExyY* d50 = cmsD50_xyY();
    grayProfile = cmsCreateGrayProfile(d50, toneCurve);
    
    // 6. Allocate MLU and work with translations
    mlu = cmsMLUalloc(context, 5);
    if (mlu) {
        char langCode[3] = {'e', 'n', '\0'};
        char countryCode[3] = {'U', 'S', '\0'};
        
        // Try to get translation codes
        cmsBool hasTranslations = cmsMLUtranslationsCodes(mlu, 0, langCode, countryCode);
        
        // Get profile info ASCII
        char buffer[256];
        if (srcProfile) {
            cmsGetProfileInfoASCII(srcProfile, cmsInfoDescription, 
                                 langCode, countryCode, 
                                 buffer, sizeof(buffer));
        }
    }
    
    // 7. Check tag linking if we have a source profile
    if (srcProfile) {
        cmsTagSignature linked = cmsTagLinkedTo(srcProfile, cmsSigMediaWhitePointTag);
    }
    
    // 8. IT8 operations
    it8 = cmsIT8Alloc(context);
    if (it8) {
        cmsIT8DefineDblFormat(it8, "%.4f");
        
        // Try to load IT8 from input data if enough size
        if (size > 50) {
            cmsHANDLE it8FromMem = cmsIT8LoadFromMem(context, data, static_cast<cmsUInt32Number>(size));
            if (it8FromMem) {
                cmsIT8Free(it8FromMem);
            }
        }
    }
    
    // 9. Create and evaluate pipeline
    pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        // Create a simple pipeline stage
        cmsToneCurve* curves[3] = {toneCurve, toneCurve, toneCurve};
        cmsStage* stage = cmsStageAllocToneCurves(context, 3, curves);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
            
            // Evaluate pipeline
            cmsUInt16Number input[3] = {0x8000, 0x8000, 0x8000};
            cmsUInt16Number output[3];
            cmsPipelineEval16(input, output, pipeline);
            
            // Get pipeline stage count
            cmsUInt32Number stageCount = cmsPipelineStageCount(pipeline);
            
            // Get first stage and its output channels
            cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (firstStage) {
                cmsUInt32Number outputChannels = cmsStageOutputChannels(firstStage);
            }
        }
    }
    
    // 10. Create transform for named color operations
    if (srcProfile && sRGBProfile) {
        transform = cmsCreateTransform(srcProfile, TYPE_RGB_8,
                                      sRGBProfile, TYPE_RGB_8,
                                      INTENT_PERCEPTUAL, 0);
        
        if (transform) {
            // Get named color list
            namedColorList = cmsGetNamedColorList(transform);
            if (namedColorList) {
                cmsUInt32Number colorCount = cmsNamedColorCount(namedColorList);
            }
        }
    }
    
    // 11. Temperature conversion
    cmsFloat64Number tempK;
    if (d50 && cmsTempFromWhitePoint(&tempK, d50)) {
        // Temperature conversion successful
    }
    
    // 12. Get color space info
    if (srcProfile) {
        cmsColorSpaceSignature colorSpace = cmsGetColorSpace(srcProfile);
        cmsUInt32Number channels = cmsChannelsOf(colorSpace);
    }
    
    // 13. Create output file for testing
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        if (srcProfile) {
            cmsUInt32Number bytesNeeded = 0;
            cmsSaveProfileToMem(srcProfile, nullptr, &bytesNeeded);
            
            if (bytesNeeded > 0) {
                std::vector<uint8_t> profileBuffer(bytesNeeded);
                if (cmsSaveProfileToMem(srcProfile, profileBuffer.data(), &bytesNeeded)) {
                    fwrite(profileBuffer.data(), 1, bytesNeeded, out_file);
                }
            }
        }
        assert_file_closed(&out_file);;
    }
    
    // 14. Clean up all allocated resources
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
    
    if (grayProfile) {
        cmsCloseProfile(grayProfile);
    }
    
    if (sRGBProfile) {
        cmsCloseProfile(sRGBProfile);
    }
    
    if (srcProfile) {
        cmsCloseProfile(srcProfile);
    }
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    assert_file_closed(&out_file);
	return 0;
}