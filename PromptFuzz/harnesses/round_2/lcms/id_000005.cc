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
//<score> 6.6666665, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsDetectTAC","cmsMLUalloc","cmsMLUgetTranslation","cmsXYZEncoded2Float","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8GetData","cmsIT8Free","cmsSetHeaderModel","cmsBuildGamma","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineInsertStage","cmsPipelineEval16","cmsStageFree","cmsStageFree","cmsPipelineFree","cmsFreeToneCurve","cmsMLUfree","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsDetectTAC","cmsMLUalloc","cmsMLUgetTranslation","cmsXYZEncoded2Float","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8GetData","cmsIT8Free","cmsSetHeaderModel","cmsBuildGamma","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsPipelineInsertStage","cmsPipelineEval16","cmsStageFree","cmsStageFree","cmsPipelineFree","cmsFreeToneCurve","cmsMLUfree","cmsCloseProfile"],"visited":2}
/*Here's a fuzz driver that uses the specified lcms APIs to process color profile data. The scenario involves reading a color profile from memory, extracting color data, transforming it, and manipulating various color management structures.

*/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful processing

    // Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	return 0;
	}
	

    // Create context from profile
    cmsContext ctx = cmsGetProfileContextID(hProfile);

    // 1. Detect TAC (Total Area Coverage)
    cmsFloat64Number tac = cmsDetectTAC(hProfile);

    // 2. Read MLU (Multi-Localized Unicode) data
    cmsMLU* mlu = cmsMLUalloc(ctx, 5);  // Allocate MLU with 5 entries
    if (mlu) {
        char obtainedLang[3] = {0};
        char obtainedCountry[3] = {0};
        
        // Try to get translation for English/US
        const char* langCode = "en";
        const char* countryCode = "US";
        cmsMLUgetTranslation(mlu, langCode, countryCode, 
                           obtainedLang, obtainedCountry);
    }

    // 3. Convert encoded XYZ to float
    cmsCIEXYZ floatXYZ;
    cmsUInt16Number encodedXYZ[3] = {0, 0, 0};
    if (size >= 6) {
        // Use some input bytes for XYZ values
        memcpy(encodedXYZ, data, 6);
        cmsXYZEncoded2Float(&floatXYZ, encodedXYZ);
    }

    // 4. Create and use IT8 structure
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        // Define double format for IT8 data
        cmsIT8DefineDblFormat(hIT8, "%0.4f");
        
        // Set some sample data
        const char* patchName = "Patch1";
        const char* sampleName = "Lab";
        const char* sampleValue = "50.0 0.0 0.0";
        
        // Try to get data (will fail if not set, but API is called)
        const char* retrieved = cmsIT8GetData(hIT8, patchName, sampleName);
        
        // Clean up IT8
        cmsIT8Free(hIT8);
    }

    // 5. Set header model (use some bytes from input)
    if (size >= 4) {
        cmsUInt32Number model = 0;
        memcpy(&model, data, 4);
        cmsSetHeaderModel(hProfile, model);
    }

    // 6. Create pipeline and insert stage
    // First create some tone curves for the pipeline
    cmsToneCurve* curves[3];
    cmsFloat64Number gamma = 2.2;
    
    for (int i = 0; i < 3; i++) {
        curves[i] = cmsBuildGamma(ctx, gamma);
    }
    
    // Create pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline) {
        // Create stages
        cmsStage* toneCurveStage = cmsStageAllocToneCurves(ctx, 3, curves);
        cmsStage* identityStage = cmsStageAllocIdentity(ctx, 3);
        
        if (toneCurveStage && identityStage) {
            // Insert tone curve stage at beginning
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, toneCurveStage);
            
            // Insert identity stage at end
            cmsPipelineInsertStage(pipeline, cmsAT_END, identityStage);
            
            // Test the pipeline with some data
            if (size >= 6) {
                cmsUInt16Number input[3], output[3];
                memcpy(input, data, 6);
                cmsPipelineEval16(input, output, pipeline);
            }
        }
        
        // Free stages (pipeline takes ownership of inserted stages)
        if (!toneCurveStage) cmsStageFree(toneCurveStage);
        if (!identityStage) cmsStageFree(identityStage);
        
        cmsPipelineFree(pipeline);
    }
    
    // Free tone curves
    for (int i = 0; i < 3; i++) {
        if (curves[i]) cmsFreeToneCurve(curves[i]);
    }

    // 7. Clean up MLU if allocated
    if (mlu) {
        cmsMLUfree(mlu);
    }

    // Clean up profile
    cmsCloseProfile(hProfile);

    return 0;
}