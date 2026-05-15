// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
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
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
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
        cmsFloat64Number matrix[9];
        cmsFloat64Number offset[3] = {0, 0, 0};
        
        size_t matrix_bytes = sizeof(matrix) < size ? sizeof(matrix) : size;
        memcpy(matrix, data, matrix_bytes);
        
        cmsStage* matrix_stage = cmsStageAllocMatrix(ctx, 3, 3, matrix, offset);
        if (matrix_stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, matrix_stage);
        }
    }
    
    if (size > sizeof(cmsFloat64Number) * 9) {
        cmsUInt32Number nGridPoints = 2;
        size_t clut_size = nGridPoints * nGridPoints * nGridPoints * 3 * sizeof(cmsUInt16Number);
        cmsUInt16Number* clut = (cmsUInt16Number*)malloc(clut_size);
        
        if (clut) {
            size_t copy_size = clut_size < (size - sizeof(cmsFloat64Number) * 9) ? 
                              clut_size : (size - sizeof(cmsFloat64Number) * 9);
            memcpy(clut, data + sizeof(cmsFloat64Number) * 9, copy_size);
            
            cmsStage* clut_stage = cmsStageAllocCLut16bit(ctx, nGridPoints, 3, 3, clut);
            if (clut_stage) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, clut_stage);
            }
            free(clut);
        }
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
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
        void* first_data = cmsStageData(first);
        
        cmsStage* dup_first = cmsStageDup(first);
        if (dup_first) {
            cmsStageFree(dup_first);
        }
        
        cmsStage* current = first;
        while (current) {
            void* stage_data = cmsStageData(current);
            cmsStage* dup_current = cmsStageDup(current);
            if (dup_current) {
                cmsStageFree(dup_current);
            }
            current = cmsStageNext(current);
        }
    }
    
    if (last && last != first) {
        cmsStage* dup_last = cmsStageDup(last);
        if (dup_last) {
            cmsStageFree(dup_last);
        }
    }
    
    cmsPipelineFree(pipeline);
    cmsDeleteContext(ctx);
    
    return 0;
}