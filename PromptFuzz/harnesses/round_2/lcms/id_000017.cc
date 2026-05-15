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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsDetectTAC","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsXYZEncoded2Float","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8GetData","cmsIT8Free","cmsSetHeaderModel","cmsBuildGamma","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineFree","cmsFreeToneCurve","cmsGetTagOffsetAndSize","cmsGetSupportedIntentsTHR","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsDetectTAC","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsXYZEncoded2Float","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8GetData","cmsIT8Free","cmsSetHeaderModel","cmsBuildGamma","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineFree","cmsFreeToneCurve","cmsGetTagOffsetAndSize","cmsGetSupportedIntentsTHR","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	

    // Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	return 0;
	}
	

    // Get profile context
    cmsContext context = cmsGetProfileContextID(hProfile);
    
    // Detect TAC
    cmsDetectTAC(hProfile);

    // Allocate MLU
    cmsMLU *mlu = cmsMLUalloc(context, 10);
    if (mlu) {
        char obtainedLang[3] = {0};
        char obtainedCountry[3] = {0};
        cmsMLUgetTranslation(mlu, "en", "US", obtainedLang, obtainedCountry);
        cmsMLUfree(mlu);
    }

    // Convert encoded XYZ to float
    cmsCIEXYZ fxyz;
    cmsUInt16Number xyz[3] = {0, 0, 0};
    if (size >= 6) {
        memcpy(xyz, data, 6);
        cmsXYZEncoded2Float(&fxyz, xyz);
    }

    // IT8 operations
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    if (hIT8) {
        cmsIT8DefineDblFormat(hIT8, "%.2f");
        
        // Try to get data (will likely return NULL since no data loaded)
        const char* patchData = cmsIT8GetData(hIT8, "patch1", "sample1");
        (void)patchData; // Avoid unused variable warning
        
        cmsIT8Free(hIT8);
    }

    // Set header model
    cmsSetHeaderModel(hProfile, 0);

    // Build gamma curve
    cmsToneCurve* gammaCurve = cmsBuildGamma(context, 2.2);
    
    // Create pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline && gammaCurve) {
        cmsToneCurve* curves[3] = {gammaCurve, gammaCurve, gammaCurve};
        cmsStage* toneCurveStage = cmsStageAllocToneCurves(context, 3, curves);
        cmsStage* identityStage = cmsStageAllocIdentity(context, 3);
        
        if (toneCurveStage) {
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, toneCurveStage);
        }
        if (identityStage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, identityStage);
        }
        
        // Evaluate pipeline with test input
        cmsUInt16Number in[3] = {0, 32768, 65535};
        cmsUInt16Number out[3];
        cmsPipelineEval16(in, out, pipeline);
        
        cmsPipelineFree(pipeline);
    }
    
    if (gammaCurve) {
        cmsFreeToneCurve(gammaCurve);
    }

    // Get tag offset and size
    cmsUInt32Number offset, tagSize;
    for (cmsUInt32Number i = 0; i < 5; i++) {
        if (cmsGetTagOffsetAndSize(hProfile, i, &offset, &tagSize)) {
            break;
        }
    }

    // Get supported intents
    cmsUInt32Number intents[10];
    char* descriptions[10];
    cmsGetSupportedIntentsTHR(context, 10, intents, descriptions);
    
    // Cleanup
    cmsCloseProfile(hProfile);
    
    return 0;
}