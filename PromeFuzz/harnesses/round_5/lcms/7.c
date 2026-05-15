// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

static cmsPipeline* create_test_pipeline(cmsContext ctx, const uint8_t* data, size_t size) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (!pipeline) return NULL;
    
    if (size > 0) {
        cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(ctx, 256, NULL);
        if (curve) {
            cmsStage* stage = cmsStageAllocToneCurves(ctx, 3, &curve);
            if (stage) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
            }
            cmsFreeToneCurve(curve);
        }
    }
    
    if (size > 1) {
        cmsStage* matrix_stage = cmsStageAllocMatrix(ctx, 3, 3, NULL, NULL);
        if (matrix_stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, matrix_stage);
        }
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsPipeline* pipeline = create_test_pipeline(ctx, Data, Size);
    if (!pipeline) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
    cmsStage* last_stage = cmsPipelineGetPtrToLastStage(pipeline);
    
    if (first_stage) {
        void* stage_data = cmsStageData(first_stage);
        (void)stage_data;
        
        if (Size % 2 == 0) {
            cmsStage* dup_stage = cmsStageDup(first_stage);
            if (dup_stage) {
                cmsStageFree(dup_stage);
            }
        }
    }
    
    if (last_stage && last_stage != first_stage) {
        void* stage_data = cmsStageData(last_stage);
        (void)stage_data;
        
        if (Size % 3 == 0) {
            cmsStage* dup_stage = cmsStageDup(last_stage);
            if (dup_stage) {
                cmsStageFree(dup_stage);
            }
        }
    }
    
    cmsStage* current = first_stage;
    while (current) {
        void* stage_data = cmsStageData(current);
        (void)stage_data;
        current = cmsStageNext(current);
    }
    
    cmsPipelineFree(pipeline);
    cmsDeleteContext(ctx);
    
    return 0;
}