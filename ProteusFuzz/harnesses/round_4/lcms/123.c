// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineEvalReverseFloat at cmslut.c:1753:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_pipeline(cmsPipeline* p) {
    if (p != NULL) {
        cmsPipelineFree(p);
    }
}

static cmsPipeline* create_empty_pipeline(cmsContext ctx) {
    // Create a simple identity pipeline for testing
    cmsPipeline* p = cmsPipelineAlloc(ctx, 3, 3);
    if (p == NULL) return NULL;
    
    // Create a simple identity stage
    cmsStage* s = cmsStageAllocIdentity(ctx, 3);
    if (s == NULL) {
        cmsPipelineFree(p);
        return NULL;
    }
    
    cmsPipelineInsertStage(p, cmsAT_END, s);
    return p;
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;
    
    cmsPipeline *p1 = NULL, *p2 = NULL;
    cmsBool success = FALSE;
    
    // Use input data to determine operations
    uint8_t op = Data[0] % 7;
    uint8_t flag = (Data[1] % 2) ? TRUE : FALSE;
    
    // Initialize pipelines based on operation
    if (op != 4 && op != 5) {
        p1 = create_empty_pipeline(ctx);
    }
    if (op != 4 && op != 6) {
        p2 = create_empty_pipeline(ctx);
    }
    
    // Test different API functions based on input
    switch (op) {
        case 0: // cmsPipelineSetSaveAs8bitsFlag
            if (p1 != NULL) {
                cmsBool prev = cmsPipelineSetSaveAs8bitsFlag(p1, flag);
                // Try again with opposite flag
                cmsPipelineSetSaveAs8bitsFlag(p1, !flag);
                success = TRUE;
            }
            break;
            
        case 1: // cmsPipelineEvalFloat
            if (p1 != NULL) {
                cmsFloat32Number in[4] = {0.0, 0.0, 0.0, 1.0};
                cmsFloat32Number out[4] = {0.0, 0.0, 0.0, 0.0};
                
                // Use input data to seed values
                if (Size >= 4) {
                    in[0] = Data[2] / 255.0;
                    in[1] = Data[3] / 255.0;
                    if (Size > 4) in[2] = Data[4] / 255.0;
                }
                
                cmsPipelineEvalFloat(in, out, p1);
                success = TRUE;
            }
            break;
            
        case 2: // cmsPipelineEvalReverseFloat
            if (p1 != NULL) {
                cmsFloat32Number target[3] = {0.5, 0.5, 0.5};
                cmsFloat32Number result[3] = {0.0, 0.0, 0.0};
                cmsFloat32Number hint[3] = {0.3, 0.3, 0.3};
                
                if (Size >= 6) {
                    target[0] = Data[2] / 255.0;
                    target[1] = Data[3] / 255.0;
                    target[2] = Data[4] / 255.0;
                    hint[0] = Data[5] / 255.0;
                }
                
                cmsBool res = cmsPipelineEvalReverseFloat(target, result, hint, p1);
                success = TRUE;
            }
            break;
            
        case 3: // cmsPipelineCat
            if (p1 != NULL && p2 != NULL) {
                success = cmsPipelineCat(p1, p2);
            }
            break;
            
        case 4: // cmsPipelineInsertStage with NULL pipeline (error case)
            {
                cmsStage* s = cmsStageAllocIdentity(ctx, 3);
                if (s != NULL) {
                    success = cmsPipelineInsertStage(NULL, cmsAT_END, s);
                    cmsStageFree(s);
                }
            }
            break;
            
        case 5: // cmsPipelineInsertStage with valid pipeline
            if (p1 != NULL) {
                cmsStage* s = cmsStageAllocIdentity(ctx, 3);
                if (s != NULL) {
                    cmsStageLoc loc = (Data[1] % 2) ? cmsAT_BEGIN : cmsAT_END;
                    success = cmsPipelineInsertStage(p1, loc, s);
                }
            }
            break;
            
        case 6: // cmsPipelineFree with NULL (should be safe)
            cmsPipelineFree(NULL);
            success = TRUE;
            break;
    }
    
    // Cleanup
    cleanup_pipeline(p1);
    cleanup_pipeline(p2);
    cmsDeleteContext(ctx);
    
    return 0;
}