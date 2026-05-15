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
//<ID> 324
//<Prompt> []
/*<Combination>: [
*/
//<score> 17.5, nr_unique_branch: 0
//<Quality> {"density":35,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsDeleteContext","cmsIsTag","cmsReadTag","cmsIsToneCurveMonotonic","cmsGetProfileInfo","cmsSetHeaderManufacturer","cmsIT8LoadFromMem","cmsIT8GetDataRowCol","cmsMLUalloc","cmsMLUsetWide","cmsCreateNULLProfileTHR","cmsCreateTransform","cmsChangeBuffersFormat","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsGetStageContextID","cmsStageNext","cmsDoTransform","cmsEvalToneCurve16","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageAllocCLutFloat","cmsPipelineInsertStage","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsOpenProfileFromMemTHR","cmsSetHeaderManufacturer","cmsSaveProfileToFile","cmsCloseProfile","cmsMLUfree","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsIsTag","cmsReadTag","cmsIsToneCurveMonotonic","cmsGetProfileInfo","cmsSetHeaderManufacturer","cmsIT8LoadFromMem","cmsIT8GetDataRowCol","cmsMLUalloc","cmsMLUsetWide","cmsCreateNULLProfileTHR","cmsCreateTransform","cmsChangeBuffersFormat","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsGetStageContextID","cmsStageNext","cmsDoTransform","cmsEvalToneCurve16","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageAllocCLutFloat","cmsPipelineInsertStage","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsOpenProfileFromMemTHR","cmsSetHeaderManufacturer","cmsSaveProfileToFile","cmsCloseProfile","cmsMLUfree","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified lcms APIs to process input data as a color profile and IT8 file, performing various operations:

*/


// Include lcms headers (adjust paths as needed)

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Open profile from memory
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(context, data, static_cast<cmsUInt32Number>(size));
    if (!profile) {
        // If not a valid profile, try other operations
        cmsDeleteContext(context);
        return 0;
    }
    
    // 3. Check for tone curve tag
    cmsTagSignature sig = (cmsTagSignature)(data[0] % 256); // Random tag for fuzzing
    cmsBool hasTag = cmsIsTag(profile, sig);
    
    // 4. Read tag if exists
    void* tagData = nullptr;
    if (hasTag) {
        tagData = cmsReadTag(profile, sig);
    }
    
    // 5. Check if tone curve is monotonic (if tag is tone curve)
    cmsToneCurve* toneCurve = nullptr;
    if (tagData && (sig == cmsSigRedTRCTag || sig == cmsSigGreenTRCTag || 
                    sig == cmsSigBlueTRCTag || sig == cmsSigGrayTRCTag)) {
        toneCurve = (cmsToneCurve*)tagData;
        cmsBool isMonotonic = cmsIsToneCurveMonotonic(toneCurve);
        (void)isMonotonic; // Use result
    }
    
    // 6. Get profile info
    wchar_t infoBuffer[256];
    cmsUInt32Number infoSize = cmsGetProfileInfo(profile, cmsInfoDescription, "en", "US", 
                                                 infoBuffer, sizeof(infoBuffer)/sizeof(wchar_t));
    (void)infoSize;
    
    // 7. Set manufacturer
    cmsUInt32Number manufacturer = static_cast<cmsUInt32Number>(data[1] % 65536);
    cmsSetHeaderManufacturer(profile, manufacturer);
    
    // 8. Load IT8 from memory
    cmsHANDLE it8 = cmsIT8LoadFromMem(context, data, static_cast<cmsUInt32Number>(size));
    
    // 9. Get IT8 data if loaded
    if (it8) {
        const char* it8Data = cmsIT8GetDataRowCol(it8, 0, 0);
        (void)it8Data;
    }
    
    // 10. Allocate MLU
    cmsMLU* mlu = cmsMLUalloc(context, 10);
    if (mlu) {
        wchar_t wideStr[] = L"Test String";
        cmsBool mluSet = cmsMLUsetWide(mlu, "en", "US", wideStr);
        (void)mluSet;
    }
    
    // 11. Create NULL profile and transform
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(context);
    if (nullProfile && profile) {
        // Create transform
        cmsHTRANSFORM transform = cmsCreateTransform(profile, TYPE_RGB_8, 
                                                    nullProfile, TYPE_RGB_8, 
                                                    INTENT_PERCEPTUAL, 0);
        
        if (transform) {
            // 12. Change buffer format
            cmsBool formatChanged = cmsChangeBuffersFormat(transform, 
                                                          TYPE_RGB_8, 
                                                          TYPE_CMYK_8);
            (void)formatChanged;
            
            // 13. Get stage context ID
            const cmsPipeline* pipeline = cmsGetTransformPipeline(transform);
            if (pipeline) {
                const cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                if (firstStage) {
                    cmsContext stageContext = cmsGetStageContextID(firstStage);
                    (void)stageContext;
                    
                    // 14. Iterate through stages
                    const cmsStage* stage = firstStage;
                    while (stage) {
                        stage = cmsStageNext(stage);
                    }
                }
            }
            
            // 15. Perform transform
            uint8_t inputBuffer[3] = {data[2], data[3], data[4]};
            uint8_t outputBuffer[4];
            cmsDoTransform(transform, inputBuffer, outputBuffer, 1);
            
            // 16. Evaluate tone curve if exists
            if (toneCurve) {
                cmsUInt16Number evalResult = cmsEvalToneCurve16(toneCurve, 
                                                               static_cast<cmsUInt16Number>(data[5] * 257));
                (void)evalResult;
            }
            
            // 17. Create and manipulate pipeline
            cmsPipeline* testPipeline = cmsPipelineAlloc(context, 3, 3);
            if (testPipeline) {
                // 18. Add identity stage
                cmsStage* identityStage = cmsStageAllocIdentity(context, 3);
                if (identityStage) {
                    cmsPipelineInsertStage(testPipeline, cmsAT_END, identityStage);
                }
                
                // 19. Add CLUT stage
                cmsFloat32Number clutTable[27] = {0}; // 3x3x3 CLUT
                for (int i = 0; i < 27 && i + 10 < static_cast<int>(size); i++) {
                    clutTable[i] = data[i + 10] / 255.0f;
                }
                cmsStage* clutStage = cmsStageAllocCLutFloat(context, 3, 3, 3, clutTable);
                if (clutStage) {
                    cmsPipelineInsertStage(testPipeline, cmsAT_END, clutStage);
                }
                
                // 20. Free pipeline
                cmsPipelineFree(testPipeline);
            }
            
            // 21. Delete transform
            cmsDeleteTransform(transform);
        }
        
        // 22. Close NULL profile
        if (nullProfile) {
            cmsCloseProfile(nullProfile);
        }
    }
    
    // 23. Save modified profile to file
    if (size > 256) { // Only save if we have enough data
        FILE* out_file = fopen("output_file", "wb");
        if (out_file) {
            // Create a modified version of the profile
            cmsHPROFILE tempProfile = cmsOpenProfileFromMemTHR(context, data, 
                                                              static_cast<cmsUInt32Number>(size));
            if (tempProfile) {
                cmsSetHeaderManufacturer(tempProfile, 0xFFFF);
                cmsBool saved = cmsSaveProfileToFile(tempProfile, "output_file");
                (void)saved;
                cmsCloseProfile(tempProfile);
            }
            assert_file_closed(&out_file);;
        }
    }
    
    // 24. Clean up MLU
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    // 25. Clean up IT8
    if (it8) {
        cmsIT8Free(it8);
    }
    
    // 26. Don't free toneCurve - it's owned by the profile
    // 27. Close main profile
    cmsCloseProfile(profile);
    
    // 28. Delete context
    cmsDeleteContext(context);
    
    return 0;
}