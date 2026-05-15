// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsPipeline* create_test_pipeline(cmsContext ctx) {
    // Create a simple pipeline with identity curves
    cmsPipeline* lut = cmsPipelineAlloc(ctx, 3, 3);
    if (!lut) return NULL;
    
    // Create identity curves for each channel
    cmsToneCurve* curves[3];
    for (int i = 0; i < 3; i++) {
        curves[i] = cmsBuildTabulatedToneCurve16(ctx, 256, NULL);
        if (!curves[i]) {
            cmsPipelineFree(lut);
            return NULL;
        }
    }
    
    // Create a matrix stage (identity matrix)
    cmsStage* matrix = cmsStageAllocMatrix(ctx, 3, 3, 
        (const cmsFloat64Number[]){1,0,0,0,1,0,0,0,1}, NULL);
    if (!matrix) {
        for (int i = 0; i < 3; i++) cmsFreeToneCurve(curves[i]);
        cmsPipelineFree(lut);
        return NULL;
    }
    
    // Create a curve stage
    cmsStage* curve = cmsStageAllocToneCurves(ctx, 3, curves);
    if (!curve) {
        cmsStageFree(matrix);
        for (int i = 0; i < 3; i++) cmsFreeToneCurve(curves[i]);
        cmsPipelineFree(lut);
        return NULL;
    }
    
    // Add stages to pipeline
    cmsPipelineInsertStage(lut, cmsAT_BEGIN, matrix);
    cmsPipelineInsertStage(lut, cmsAT_BEGIN, curve);
    
    // Free the curves (they are now owned by the stage)
    for (int i = 0; i < 3; i++) cmsFreeToneCurve(curves[i]);
    
    return lut;
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create test pipeline
    cmsPipeline* lut = create_test_pipeline(ctx);
    if (!lut) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Use first byte to determine which operations to perform
    uint8_t op_selector = Data[0];
    
    // Test cmsPipelineGetPtrToFirstStage and cmsPipelineGetPtrToLastStage
    cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(lut);
    cmsStage* last_stage = cmsPipelineGetPtrToLastStage(lut);
    
    // Test cmsStageData on first and last stages if they exist
    if (first_stage) {
        void* stage_data = cmsStageData(first_stage);
        (void)stage_data;
    }
    
    if (last_stage && last_stage != first_stage) {
        void* stage_data = cmsStageData(last_stage);
        (void)stage_data;
    }
    
    // Test cmsStageNext
    if (first_stage) {
        cmsStage* next_stage = cmsStageNext(first_stage);
        if (next_stage) {
            void* next_stage_data = cmsStageData(next_stage);
            (void)next_stage_data;
        }
    }
    
    // Test cmsStageDup
    if (first_stage && (op_selector & 0x01)) {
        cmsStage* dup_stage = cmsStageDup(first_stage);
        if (dup_stage) {
            // Add duplicated stage to end of pipeline
            cmsPipelineInsertStage(lut, cmsAT_END, dup_stage);
        }
    }
    
    // Test cmsPipelineUnlinkStage
    if (op_selector & 0x02) {
        cmsStage* unlinked_stage = NULL;
        
        // Try to unlink from beginning
        cmsPipelineUnlinkStage(lut, cmsAT_BEGIN, &unlinked_stage);
        if (unlinked_stage) {
            cmsStageFree(unlinked_stage);
        }
        
        // Try to unlink from end
        unlinked_stage = NULL;
        cmsPipelineUnlinkStage(lut, cmsAT_END, &unlinked_stage);
        if (unlinked_stage) {
            cmsStageFree(unlinked_stage);
        }
    }
    
    // Cleanup
    cmsPipelineFree(lut);
    cmsDeleteContext(ctx);
    
    return 0;
}