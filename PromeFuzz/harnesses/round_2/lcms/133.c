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
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
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

static cmsPipeline* create_test_pipeline(cmsContext ctx, const uint8_t* data, size_t size) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (!pipeline) return NULL;
    
    if (size > 0) {
        cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(ctx, 256, NULL);
        if (curve) {
            cmsToneCurve* curves[3] = { curve, curve, curve };
            cmsStage* stage = cmsStageAllocToneCurves(ctx, 3, curves);
            if (stage) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
            }
            cmsFreeToneCurve(curve);
        }
    }
    
    if (size > 1 && (data[0] % 2 == 0)) {
        double matrix[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
        cmsStage* stage = cmsStageAllocMatrix(ctx, 3, 3, matrix, NULL);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        }
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsPipeline* pipeline = create_test_pipeline(ctx, Data, Size);
    if (!pipeline) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    cmsStage* first = cmsPipelineGetPtrToFirstStage(pipeline);
    cmsStage* last = cmsPipelineGetPtrToLastStage(pipeline);
    
    if (first) {
        cmsStageType(first);
        cmsStageData(first);
        
        cmsStage* dup = cmsStageDup(first);
        if (dup) {
            cmsStageType(dup);
            cmsStageData(dup);
            cmsStageFree(dup);
        }
        
        cmsStage* current = first;
        while (current) {
            cmsStageType(current);
            cmsStageData(current);
            current = cmsStageNext(current);
        }
    }
    
    if (last && first != last) {
        cmsStageType(last);
        cmsStageData(last);
    }
    
    cmsPipelineFree(pipeline);
    cmsDeleteContext(ctx);
    
    return 0;
}