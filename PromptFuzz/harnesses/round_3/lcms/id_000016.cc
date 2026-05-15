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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["cmsOpenProfileFromMem","cmsBuildTabulatedToneCurveFloat","cmsCloseProfile","cmsCreateGrayProfile","cmsFreeToneCurve","cmsCloseProfile","cmsMLUalloc","cmsMLUtranslationsCodes","cmsTagLinkedTo","cmsBuildTabulatedToneCurveFloat","cmsCreateTransform","cmsFreeToneCurve","cmsMLUfree","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsPipelineAlloc","cmsPipelineEval16","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsTempFromWhitePoint","cmsGetColorSpace","cmsStageAllocCLut16bitGranular","cmsChannelsOf","cmsStageFree","cmsPipelineFree","cmsDeleteTransform","cmsFreeToneCurve","cmsMLUfree","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsBuildTabulatedToneCurveFloat","cmsCreateGrayProfile","cmsMLUalloc","cmsMLUtranslationsCodes","cmsTagLinkedTo","cmsBuildTabulatedToneCurveFloat","cmsCreateTransform","cmsPipelineAlloc","cmsPipelineEval16","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsTempFromWhitePoint","cmsGetColorSpace","cmsStageAllocCLut16bitGranular","cmsChannelsOf","cmsStageFree","cmsPipelineFree","cmsDeleteTransform","cmsFreeToneCurve","cmsMLUfree","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Open profile from input memory
    cmsHPROFILE hInputProfile = cmsOpenProfileFromMem(data, size);
    if (!hInputProfile) {
        return 0;
    }

    // 2. Create gray profile with D50 white point and linear transfer function
    cmsCIExyY d50WhitePoint = {0.3457, 0.3585, 1.0};
    cmsFloat32Number linearCurve[2] = {0.0f, 1.0f};
    cmsToneCurve* transferFunction = cmsBuildTabulatedToneCurveFloat(NULL, 2, linearCurve);
    if (!transferFunction) {
        cmsCloseProfile(hInputProfile);
        return 0;
    }
    
    cmsHPROFILE hGrayProfile = cmsCreateGrayProfile(&d50WhitePoint, transferFunction);
    if (!hGrayProfile) {
        cmsFreeToneCurve(transferFunction);
        cmsCloseProfile(hInputProfile);
        return 0;
    }

    // 3. Allocate MLU and check translation codes
    cmsMLU* mlu = cmsMLUalloc(NULL, 5);
    if (mlu) {
        char lang[3], country[3];
        cmsMLUtranslationsCodes(mlu, 0, lang, country);
    }

    // 4. Check tag linking in input profile
    cmsTagSignature linkedTag = cmsTagLinkedTo(hInputProfile, cmsSigMediaWhitePointTag);

    // 5. Build additional tone curve
    cmsFloat32Number toneValues[3] = {0.0f, 0.5f, 1.0f};
    cmsToneCurve* extraCurve = cmsBuildTabulatedToneCurveFloat(NULL, 3, toneValues);

    // 6. Create transform from input to gray profile
    cmsHTRANSFORM xform = cmsCreateTransform(hInputProfile, TYPE_GRAY_8, 
                                              hGrayProfile, TYPE_GRAY_8, 
                                              INTENT_PERCEPTUAL, 0);
    if (!xform) {
        if (extraCurve) cmsFreeToneCurve(extraCurve);
        if (mlu) cmsMLUfree(mlu);
        cmsCloseProfile(hGrayProfile);
        cmsFreeToneCurve(transferFunction);
        cmsCloseProfile(hInputProfile);
        return 0;
    }

    // 7. Allocate pipeline and evaluate
    cmsPipeline* pipeline = cmsPipelineAlloc(NULL, 3, 3);
    if (pipeline) {
        cmsUInt16Number in[3] = {0x8000, 0x8000, 0x8000};
        cmsUInt16Number out[3];
        cmsPipelineEval16(in, out, pipeline);
    }

    // 8. Get named color list from transform
    cmsNAMEDCOLORLIST* namedColors = cmsGetNamedColorList(xform);
    if (namedColors) {
        cmsUInt32Number colorCount = cmsNamedColorCount(namedColors);
        if (colorCount > 0) {
            char name[256], prefix[256], suffix[256];
            cmsUInt16Number pcs[3], colorant[3];
            cmsNamedColorInfo(namedColors, 0, name, prefix, suffix, pcs, colorant);
        }
    }

    // 9. Check pipeline stage information
    if (pipeline) {
        cmsUInt32Number stageCount = cmsPipelineStageCount(pipeline);
        if (stageCount > 0) {
            cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (firstStage) {
                cmsUInt32Number outputChannels = cmsStageOutputChannels(firstStage);
            }
        }
    }

    // 10. Convert white point to temperature
    cmsFloat64Number tempK;
    cmsTempFromWhitePoint(&tempK, &d50WhitePoint);

    // 11. Get color space of input profile
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hInputProfile);

    // 12. Allocate CLUT stage
    cmsUInt32Number clutPoints[3] = {2, 2, 2};
    cmsUInt16Number clutTable[24] = {0};
    for (int i = 0; i < 24; i++) {
        clutTable[i] = (i * 0xFFFF) / 24;
    }
    cmsStage* clutStage = cmsStageAllocCLut16bitGranular(NULL, clutPoints, 3, 3, clutTable);

    // 13. Get channels count for color space
    cmsUInt32Number channels = cmsChannelsOf(colorSpace);

    // 14. Clean up resources
    if (clutStage) {
        // Note: cmsStageFree is not in the provided list, but needed for cleanup
        // Assuming it exists in lcms as standard API
        cmsStageFree(clutStage);
    }
    
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    cmsDeleteTransform(xform);
    
    if (extraCurve) {
        cmsFreeToneCurve(extraCurve);
    }
    
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    cmsCloseProfile(hGrayProfile);
    cmsFreeToneCurve(transferFunction);
    cmsCloseProfile(hInputProfile);

    return 0;
}