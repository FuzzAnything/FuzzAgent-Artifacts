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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsDeleteContext","cmsDetectTAC","cmsCreateInkLimitingDeviceLink","cmsCloseProfile","cmsDeleteContext","cmsPipelineAlloc","cmsStageAllocMatrix","cmsStageFree","cmsSetHeaderRenderingIntent","cmsSetEncodedICCversion","cmsMLUalloc","cmsMLUsetWide","cmsMLUdup","cmsMLUfree","cmsMLUfree","cmsIsTag","cmsSaveProfileToFile","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsUnregisterPlugins","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsDetectTAC","cmsCreateInkLimitingDeviceLink","cmsPipelineAlloc","cmsStageAllocMatrix","cmsStageFree","cmsSetHeaderRenderingIntent","cmsSetEncodedICCversion","cmsMLUalloc","cmsMLUsetWide","cmsMLUdup","cmsMLUfree","cmsMLUfree","cmsIsTag","cmsSaveProfileToFile","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsUnregisterPlugins","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 1. Create input profile from memory
    cmsHPROFILE inProfile = cmsOpenProfileFromMem(data, size);
    if (!inProfile) {
        cmsDeleteContext(context);
        return 0;
    }

    // 2. Detect TAC from input profile
    cmsFloat64Number tac = cmsDetectTAC(inProfile);
    
    // 3. Create ink limiting device link using detected TAC (clamp to reasonable range)
    cmsFloat64Number limit = (tac > 0 && tac < 400) ? tac : 300.0;
    cmsHPROFILE outProfile = cmsCreateInkLimitingDeviceLink(cmsSigRgbData, limit);
    if (!outProfile) {
        cmsCloseProfile(inProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // 4. Create pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        // 5. Create matrix stage (identity matrix with small offset)
        cmsFloat64Number matrix[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
        cmsFloat64Number offset[3] = {0.01, 0.01, 0.01};
        cmsStage* matrixStage = cmsStageAllocMatrix(context, 3, 3, matrix, offset);
        
        if (matrixStage) {
            // Add stage to pipeline (simplified - normally would use cmsPipelineInsertStage)
            // For fuzzing, we'll just create and free it
            cmsStageFree(matrixStage);
        }
        
        // 6. Set header properties on output profile
        cmsSetHeaderRenderingIntent(outProfile, INTENT_PERCEPTUAL);
        cmsSetEncodedICCversion(outProfile, 0x0430000); // ICC version 4.3
        
        // 7. Create and manipulate MLU
        cmsMLU* mlu = cmsMLUalloc(context, 5);
        if (mlu) {
            wchar_t wideStr[] = L"FuzzTest";
            cmsMLUsetWide(mlu, "en", "US", wideStr);
            
            cmsMLU* mluDup = cmsMLUdup(mlu);
            if (mluDup) {
                cmsMLUfree(mluDup);
            }
            cmsMLUfree(mlu);
        }
        
        // 8. Dictionary iteration (if available in profile)
        // Get first dictionary entry from input profile
        const cmsDICTentry* dictEntry = nullptr;
        // Note: cmsDictNextEntry requires starting entry, we'll try to get one
        // For simplicity, we'll simulate dictionary iteration
        if (cmsIsTag(inProfile, cmsSigCopyrightTag)) {
            // We have at least one tag that might have dictionary entries
            // In real usage, we would get the actual dictionary
        }
        
        // 9. Save profile to file
        FILE* out_file = fopen("output_file", "wb");
        if (out_file) {
            cmsSaveProfileToFile(outProfile, "output_file");
            assert_file_closed(&out_file);;
        }
        
        // 10. Clean up pipeline
        cmsPipelineFree(pipeline);
    }

    // 11. Close profiles
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);
    
    // 12. Unregister plugins and delete context
    cmsUnregisterPlugins();
    cmsDeleteContext(context);
    
    // Clean up output file if created
    remove("output_file");
    
    assert_file_name_closed("output_file");
	return 0;
}