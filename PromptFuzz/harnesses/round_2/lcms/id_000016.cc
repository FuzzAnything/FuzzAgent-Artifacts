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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsDetectTAC","cmsMLUalloc","cmsMLUgetTranslation","cmsXYZEncoded2Float","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8GetData","cmsIT8Free","cmsSetHeaderModel","cmsBuildGamma","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineFree","cmsFreeToneCurve","cmsGetTagOffsetAndSize","cmsGetSupportedIntentsTHR","cmsMLUfree","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsDetectTAC","cmsMLUalloc","cmsMLUgetTranslation","cmsXYZEncoded2Float","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8GetData","cmsIT8Free","cmsSetHeaderModel","cmsBuildGamma","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineFree","cmsFreeToneCurve","cmsGetTagOffsetAndSize","cmsGetSupportedIntentsTHR","cmsMLUfree","cmsCloseProfile"],"visited":0}
/*To achieve an event using the specified lcms APIs and consume the fuzz input data, I'll create a fuzz driver that validates and processes ICC profile data. Here's a step-by-step implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // 1. Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	return 0;
	}
	
    
    // Get profile context ID
    cmsContext context = cmsGetProfileContextID(hProfile);
    
    // 2. Detect Total Area Coverage
    cmsFloat64Number tac = cmsDetectTAC(hProfile);
    
    // 3. Allocate MLU and attempt to get translation
    cmsMLU* mlu = cmsMLUalloc(context, 10);
    if (mlu) {
        char obtainedLang[3] = {0};
        char obtainedCountry[3] = {0};
        cmsMLUgetTranslation(mlu, "en", "US", obtainedLang, obtainedCountry);
    }
    
    // 4. Convert encoded XYZ to float (use sample data)
    if (size >= 6) {
        cmsCIEXYZ fxyz;
        cmsUInt16Number XYZ[3];
        // Use first 6 bytes for XYZ data
        for (int i = 0; i < 3 && i*2+1 < size; i++) {
            XYZ[i] = (data[i*2] << 8) | data[i*2+1];
        }
        cmsXYZEncoded2Float(&fxyz, XYZ);
    }
    
    // 5. IT8 operations
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    if (hIT8) {
        cmsIT8DefineDblFormat(hIT8, "%.6f");
        const char* dataValue = cmsIT8GetData(hIT8, "patch1", "sample1");
        cmsIT8Free(hIT8);
    }
    
    // 6. Set header model (use first 4 bytes for model value)
    if (size >= 4) {
        uint32_t model = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        cmsSetHeaderModel(hProfile, model);
    }
    
    // 7. Build gamma curve and pipeline operations
    cmsToneCurve* gammaCurve = cmsBuildGamma(context, 2.2);
    if (gammaCurve) {
        cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
        if (pipeline) {
            cmsToneCurve* curves[3] = {gammaCurve, gammaCurve, gammaCurve};
            cmsStage* toneCurveStage = cmsStageAllocToneCurves(context, 3, curves);
            cmsStage* identityStage = cmsStageAllocIdentity(context, 3);
            
            if (toneCurveStage && identityStage) {
                cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, toneCurveStage);
                cmsPipelineInsertStage(pipeline, cmsAT_END, identityStage);
                
                // Evaluate pipeline with sample data
                if (size >= 6) {
                    cmsUInt16Number in[3], out[3];
                    for (int i = 0; i < 3 && i*2+1 < size; i++) {
                        in[i] = (data[i*2] << 8) | data[i*2+1];
                    }
                    cmsPipelineEval16(in, out, pipeline);
                }
            }
            
            // Clean up pipeline resources
            if (pipeline) cmsPipelineFree(pipeline);
        }
        if (gammaCurve) cmsFreeToneCurve(gammaCurve);
    }
    
    // 8. Get tag offset and size
    cmsUInt32Number offset = 0, tagSize = 0;
    cmsGetTagOffsetAndSize(hProfile, 0, &offset, &tagSize);
    
    // 9. Get supported intents
    cmsUInt32Number codes[10];
    char* descriptions[10];
    cmsUInt32Number count = cmsGetSupportedIntentsTHR(context, 10, codes, descriptions);
    
    // 10. Clean up resources
    if (mlu) cmsMLUfree(mlu);
    cmsCloseProfile(hProfile);
    
    return 0;
}