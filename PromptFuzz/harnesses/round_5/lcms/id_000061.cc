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
//<ID> 368
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 25
//<Quality> {"density":13,"unique_branches":{"Type_LUT16_Write":[[2413,24,2413,58,0,0,4,0],[2416,13,2416,36,0,0,4,1],[2416,40,2416,64,0,0,4,1],[2421,24,2421,60,0,0,4,0],[2431,9,2431,20,0,0,4,0],[2431,24,2431,60,0,0,4,0],[2463,9,2463,23,0,0,4,0],[2465,21,2465,26,0,0,4,0],[2465,21,2465,26,0,0,4,1],[2467,17,2467,65,0,0,4,1],[2485,9,2485,23,0,0,4,0],[2486,13,2486,90,0,0,4,1],[2491,9,2491,24,0,0,4,0],[2492,13,2492,91,0,0,4,1],[2500,9,2500,23,0,0,4,0],[2501,13,2501,60,0,0,4,1],[2521,9,2521,24,0,0,4,0],[2522,13,2522,61,0,0,4,1]],"Write16bitTables":[[2292,15,2292,35,0,0,4,0],[2292,15,2292,35,0,0,4,1],[2296,19,2296,31,0,0,4,0],[2296,19,2296,31,0,0,4,1],[2299,17,2299,48,0,0,4,1]],"DecideLUTtypeB2A":[[1858,9,1858,25,0,0,4,0],[1859,13,1859,30,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsCreateInkLimitingDeviceLink","cmsDetectTAC","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsSetHeaderRenderingIntent","cmsSetEncodedICCversion","cmsMLUalloc","cmsMLUsetWide","cmsMLUdup","cmsMLUfree","cmsDictNextEntry","cmsOpenProfileFromStreamTHR","cmsDetectTAC","cmsSaveProfileToFile","cmsCloseProfile","cmsUnregisterPlugins","cmsStageFree","cmsPipelineFree","cmsMLUfree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateInkLimitingDeviceLink","cmsDetectTAC","cmsPipelineAlloc","cmsStageAllocMatrix","cmsPipelineInsertStage","cmsSetHeaderRenderingIntent","cmsSetEncodedICCversion","cmsMLUalloc","cmsMLUsetWide","cmsMLUdup","cmsMLUfree","cmsDictNextEntry","cmsOpenProfileFromStreamTHR","cmsDetectTAC","cmsSaveProfileToFile","cmsCloseProfile","cmsUnregisterPlugins","cmsStageFree","cmsPipelineFree","cmsMLUfree","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimum data for operations
    if (size < 128) {
	return 0;
	}
	
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 1. Create ink limiting device link profile
    cmsFloat64Number limit = 200.0; // Default ink limit
    cmsHPROFILE inkLimitProfile = cmsCreateInkLimitingDeviceLink(cmsSigRgbData, limit);
    
    // 2. Detect TAC from the ink limiting profile
    if (inkLimitProfile) {
        cmsDetectTAC(inkLimitProfile);
    }
    
    // 3. Create a pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    
    // 4. Create a matrix stage (3x3 identity matrix, no offset)
    cmsFloat64Number identityMatrix[9] = {1.0, 0.0, 0.0, 
                                          0.0, 1.0, 0.0, 
                                          0.0, 0.0, 1.0};
    cmsFloat64Number zeroOffset[3] = {0.0, 0.0, 0.0};
    cmsStage* matrixStage = cmsStageAllocMatrix(context, 3, 3, identityMatrix, zeroOffset);
    
    // 5. Add matrix stage to pipeline (using standard lcms API)
    if (pipeline && matrixStage) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, matrixStage);
    }
    
    // 6. Set header properties on ink limit profile
    if (inkLimitProfile) {
        cmsSetHeaderRenderingIntent(inkLimitProfile, INTENT_PERCEPTUAL);
        cmsSetEncodedICCversion(inkLimitProfile, 0x04300000); // ICC 4.3
    }
    
    // 7. Create and manipulate MLU (Multi Localized Unicode) objects
    cmsMLU* mlu = cmsMLUalloc(context, 1);
    if (mlu) {
        wchar_t wideStr[] = L"Fuzz Test Description";
        cmsMLUsetWide(mlu, "en", "US", wideStr);
        
        // Duplicate MLU
        cmsMLU* mluDup = cmsMLUdup(mlu);
        
        // Clean up duplicated MLU
        cmsMLUfree(mluDup);
    }
    
    // 8. Iterate through dictionary entries (simulate with minimal data)
    cmsDICTentry* dictEntry = nullptr; // In real usage, would come from a profile
    if (dictEntry) {
        const cmsDICTentry* nextEntry = cmsDictNextEntry(dictEntry);
        (void)nextEntry; // Prevent unused warning
    }
    
    // 9. Use input data to create a profile
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        cmsHPROFILE inputProfile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
        
        if (inputProfile) {
            // Detect TAC from input profile
            cmsDetectTAC(inputProfile);
            
            // Save profile to file
            cmsSaveProfileToFile(inputProfile, "output_file");
            
            // Close the input profile
            cmsCloseProfile(inputProfile);
        }
        assert_file_closed(&in_file);;
    }
    
    // 10. Unregister plugins
    cmsUnregisterPlugins();
    
    // 11. Clean up resources
    if (matrixStage && !pipeline) {
        // If matrix stage wasn't added to pipeline, free it separately
        cmsStageFree(matrixStage);
    }
    
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    if (inkLimitProfile) {
        cmsCloseProfile(inkLimitProfile);
    }
    
    cmsDeleteContext(context);
    
    // Clean up output file
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}