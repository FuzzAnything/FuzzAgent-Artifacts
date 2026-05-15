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
//<ID> 89
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsCreate_sRGBProfileTHR","cmsCloseProfile","cmsCreateTransform","cmsCloseProfile","cmsCloseProfile","cmsGetTransformInputFormat","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyStr","cmsIT8SetComment","cmsDictAlloc","cmsMLUalloc","cmsMLUsetWide","cmsDictAddEntry","cmsMLUfree","cmsDictFree","cmsIT8Free","cmsBuildGamma","cmsBuildGamma","cmsJoinToneCurve","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineGetPtrToFirstStage","cmsStageData","cmsStageNext","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsCreate_sRGBProfileTHR","cmsCreateTransform","cmsGetTransformInputFormat","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyStr","cmsIT8SetComment","cmsDictAlloc","cmsMLUalloc","cmsMLUsetWide","cmsDictAddEntry","cmsMLUfree","cmsDictFree","cmsIT8Free","cmsBuildGamma","cmsBuildGamma","cmsJoinToneCurve","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineGetPtrToFirstStage","cmsStageData","cmsStageNext","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // 1. Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	return 0;
	}
	

    // 2. Get profile info using cmsGetProfileInfo
    wchar_t profileInfoBuffer[256];
    cmsUInt32Number infoSize = cmsGetProfileInfo(hProfile, cmsInfoDescription, 
                                                 "en", "US", 
                                                 profileInfoBuffer, 
                                                 sizeof(profileInfoBuffer)/sizeof(wchar_t));

    // 3. Create a transform to get input format
    cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfileTHR(NULL);
    if (!sRGBProfile) {
        cmsCloseProfile(hProfile);
        return 0;
    }

    cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, 
                                                  TYPE_RGB_8, 
                                                  sRGBProfile, 
                                                  TYPE_RGB_8, 
                                                  INTENT_PERCEPTUAL, 0);
    if (!hTransform) {
        cmsCloseProfile(sRGBProfile);
        cmsCloseProfile(hProfile);
        return 0;
    }

    // 4. Get transform input format using cmsGetTransformInputFormat
    cmsUInt32Number inputFormat = cmsGetTransformInputFormat(hTransform);

    // 5. Create IT8 handle and set properties
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (hIT8) {
        // Set sheet type
        cmsIT8SetSheetType(hIT8, "CGATS.17");
        
        // Set property string
        const char* propName = "DESCRIPTOR";
        const char* propValue = "Test Data";
        cmsIT8SetPropertyStr(hIT8, propName, propValue);
        
        // Also test setting a comment
        cmsIT8SetComment(hIT8, "Fuzzer generated");
        
        // Create a dictionary from IT8 (simulated)
        // Note: We need to create a dictionary handle first
        cmsHANDLE hDict = cmsDictAlloc(NULL);
        if (hDict) {
            // Add an entry to dictionary
            cmsMLU* mlu = cmsMLUalloc(NULL, 1);
            if (mlu) {
                cmsMLUsetWide(mlu, "en", "US", L"Display Name");
                cmsDictAddEntry(hDict, L"TestKey", L"TestValue", mlu, NULL);
                cmsMLUfree(mlu);
            }
            
            // Free dictionary using cmsDictFree
            cmsDictFree(hDict);
        }
        
        // Free IT8
        cmsIT8Free(hIT8);
    }

    // 6. Create tone curves and join them
    cmsToneCurve* curve1 = cmsBuildGamma(NULL, 2.2); // Gamma 2.2 curve
    cmsToneCurve* curve2 = cmsBuildGamma(NULL, 1.8); // Gamma 1.8 curve
    
    if (curve1 && curve2) {
        // Join the two curves using cmsJoinToneCurve
        cmsUInt32Number nPoints = 256;
        cmsToneCurve* joinedCurve = cmsJoinToneCurve(NULL, curve1, curve2, nPoints);
        
        if (joinedCurve) {
            // Create a simple pipeline with stages
            cmsPipeline* pipeline = cmsPipelineAlloc(NULL, 3, 3);
            if (pipeline) {
                // Add first stage (identity)
                cmsStage* stage1 = cmsStageAllocIdentity(NULL, 3);
                cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage1);
                
                // Add second stage (tone curves)
                cmsToneCurve* curves[3] = {joinedCurve, joinedCurve, joinedCurve};
                cmsStage* stage2 = cmsStageAllocToneCurves(NULL, 3, curves);
                cmsPipelineInsertStage(pipeline, cmsAT_END, stage2);
                
                // Traverse stages using cmsStageNext
                cmsStage* currentStage = cmsPipelineGetPtrToFirstStage(pipeline);
                while (currentStage) {
                    // Do something with the stage
                    void* stageData = cmsStageData(currentStage);
                    (void)stageData; // Use stageData to avoid unused variable warning
                    
                    // Get next stage
                    currentStage = cmsStageNext(currentStage);
                }
                
                // Free pipeline
                cmsPipelineFree(pipeline);
            }
            
            // Free joined curve
            cmsFreeToneCurve(joinedCurve);
        }
        
        // Free original curves
        cmsFreeToneCurve(curve1);
        cmsFreeToneCurve(curve2);
    }

    // 7. Clean up
    cmsDeleteTransform(hTransform);
    cmsCloseProfile(sRGBProfile);
    cmsCloseProfile(hProfile);

    return 0;
}