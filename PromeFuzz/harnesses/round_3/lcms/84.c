// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    // Create a dummy context for pipeline creation
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Create a simple pipeline with 3 input and 3 output channels
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (!pipeline) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Create some dummy stages to populate the pipeline
    // First stage: identity matrix
    cmsStage* stage1 = cmsStageAllocIdentity(context, 3);
    if (stage1) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage1);
    }
    
    // Second stage: identity matrix
    cmsStage* stage2 = cmsStageAllocIdentity(context, 3);
    if (stage2) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage2);
    }
    
    // Third stage: identity matrix
    cmsStage* stage3 = cmsStageAllocIdentity(context, 3);
    if (stage3) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage3);
    }
    
    // Now test the target API functions
    // Use fuzzer data to decide which operations to perform
    size_t index = 0;
    
    while (index < Size && pipeline) {
        uint8_t op = Data[index] % 6;
        index++;
        
        switch (op) {
            case 0: {
                // Test cmsPipelineGetPtrToLastStage
                cmsStage* lastStage = cmsPipelineGetPtrToLastStage(pipeline);
                if (lastStage) {
                    // Do something with the stage
                    void* data = cmsStageData(lastStage);
                    (void)data; // Use data to avoid compiler warning
                }
                break;
            }
            
            case 1: {
                // Test cmsStageNext
                cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                if (firstStage) {
                    cmsStage* nextStage = cmsStageNext(firstStage);
                    if (nextStage) {
                        void* data = cmsStageData(nextStage);
                        (void)data;
                    }
                }
                break;
            }
            
            case 2: {
                // Test cmsPipelineGetPtrToFirstStage
                cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                if (firstStage) {
                    void* data = cmsStageData(firstStage);
                    (void)data;
                }
                break;
            }
            
            case 3: {
                // Test cmsStageFree on a duplicated stage
                cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                if (firstStage) {
                    cmsStage* dupStage = cmsStageDup(firstStage);
                    if (dupStage) {
                        cmsStageFree(dupStage);
                    }
                }
                break;
            }
            
            case 4: {
                // Test cmsStageData
                cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                if (firstStage) {
                    void* data = cmsStageData(firstStage);
                    (void)data;
                }
                break;
            }
            
            case 5: {
                // Test cmsStageDup
                cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                if (firstStage) {
                    cmsStage* dupStage = cmsStageDup(firstStage);
                    if (dupStage) {
                        // Insert the duplicated stage at the end
                        cmsPipelineInsertStage(pipeline, cmsAT_END, dupStage);
                    }
                }
                break;
            }
        }
        
        // Traverse the entire pipeline chain
        if (index < Size && (Data[index] % 10 == 0)) {
            cmsStage* current = cmsPipelineGetPtrToFirstStage(pipeline);
            while (current) {
                void* data = cmsStageData(current);
                (void)data;
                current = cmsStageNext(current);
            }
        }
        
        if (index < Size) index++;
    }
    
    // Cleanup
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    cmsDeleteContext(context);
    
    return 0;
}

#ifdef __cplusplus
}
#endif