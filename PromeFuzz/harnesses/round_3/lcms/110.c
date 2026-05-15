// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsPipeline* create_dummy_pipeline(cmsContext ctx, const uint8_t* data, size_t size) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (!pipeline) return NULL;
    
    if (size > 0) {
        cmsStage* stage = cmsStageAllocIdentity(ctx, 3);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        }
    }
    
    if (size > 10) {
        double matrix[9];
        for (int i = 0; i < 9 && i < (int)size - 10; i++) {
            matrix[i] = (double)data[i] / 255.0;
        }
        cmsStage* stage = cmsStageAllocMatrix(ctx, 3, 3, matrix, NULL);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        }
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsPipeline* pipeline = create_dummy_pipeline(ctx, Data, Size);
    if (!pipeline) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    cmsStage* stage = cmsPipelineGetPtrToFirstStage(pipeline);
    while (stage) {
        cmsStageType(stage);
        
        void* stage_data = cmsStageData(stage);
        if (stage_data && Size > 20) {
            volatile char dummy = *((char*)stage_data);
            (void)dummy;
        }
        
        cmsStage* dup_stage = cmsStageDup(stage);
        if (dup_stage) {
            cmsStageType(dup_stage);
            cmsStageFree(dup_stage);
        }
        
        stage = cmsStageNext(stage);
    }
    
    cmsStage* last_stage = cmsPipelineGetPtrToLastStage(pipeline);
    if (last_stage) {
        cmsStageType(last_stage);
    }
    
    cmsPipelineFree(pipeline);
    cmsDeleteContext(ctx);
    
    return 0;
}