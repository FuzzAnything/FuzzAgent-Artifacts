#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <algorithm>

namespace harness_014 {
/*
 * Harness 014 - Floating-point Pipeline Evaluation Operations for Little CMS 2
 * 
 * Target APIs:
 * - cmsPipelineEvalReverseFloat (32 undiscovered branches)
 * - cmsStageSampleCLutFloat (30 undiscovered branches) 
 * - cmsPipelineEvalFloat (2 undiscovered branches)
 * - cmsSliceSpaceFloat (10 undiscovered branches)
 * 
 * Required Setup APIs:
 * - cmsPipelineAlloc (Pipeline creation)
 * - cmsStageAllocMatrix (Matrix stage creation)
 * - cmsStageAllocCLutFloat (CLUT stage creation)
 * - cmsPipelineInsertStage (Stage assembly)
 * 
 * Required Cleanup APIs:
 * - cmsPipelineFree (Pipeline cleanup)
 * - cmsStageFree (Stage cleanup)
 * 
 * Invocation sequence:
 * 1. Call cmsPipelineAlloc to create an empty pipeline (3 input, 3 output channels)
 * 2. Call cmsStageAllocMatrix to create a 3x3 matrix stage
 * 3. Call cmsStageAllocCLutFloat to create a 3D CLUT stage
 * 4. Call cmsPipelineInsertStage to add both stages to the pipeline
 * 5. Call cmsSliceSpaceFloat to initialize CLUT data points
 * 6. Call cmsPipelineEvalFloat to test forward evaluation
 * 7. Call cmsPipelineEvalReverseFloat to test reverse evaluation (Newton method)
 * 8. Call cmsStageSampleCLutFloat to sample the CLUT stage
 * 9. Call cmsPipelineFree and cmsStageFree for cleanup
 * 
 * These functions represent 74+ points of undiscovered complexity in the cmslut.c module
 * (currently at 49.56% branch coverage). They form a logical lifecycle for pipeline testing
 * (Create → Configure → Evaluate Forward/Reverse → Sample → Cleanup).
 * 
 * Existing harnesses focus on profile I/O, tone curves, dictionaries, and MD5, but lack
 * dedicated pipeline evaluation testing, making this a semantically diverse target.
 */

// Sampler function for cmsSliceSpaceFloat and cmsStageSampleCLutFloat
static cmsInt32Number FloatSampler(CMSREGISTER const cmsFloat32Number In[],
                                   CMSREGISTER cmsFloat32Number Out[],
                                   CMSREGISTER void* Cargo) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(Cargo);
    if (!fdp || fdp->remaining_bytes() < 12) {
        return 0;
    }
    
    // Check if Out is NULL before writing to it
    if (Out) {
        for (int i = 0; i < 3; i++) {
            Out[i] = fdp->ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
        }
    } else {
        // Still consume data to maintain consistent fuzzer state
        for (int i = 0; i < 3; i++) {
            fdp->ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
        }
    }
    
    return 1; // Success
}
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for pipeline operations
    // Fixed-size data: matrix (9 doubles), CLUT grid points (1-4 ints), operation flags, etc.
    // Variable-size data: CLUT table data, sampler parameters
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t use_context = fdp.ConsumeBool();
    uint8_t grid_points = fdp.ConsumeIntegralInRange<uint8_t>(2, 8); // CLUT grid points (2-8)
    uint8_t matrix_type = fdp.ConsumeIntegral<uint8_t>() % 3; // 0=identity, 1=random, 2=zero
    
    // Create context if requested
    cmsContext context = use_context ? cmsCreateContext(NULL, NULL) : NULL;
    
    // 1. Create pipeline with 3 input and 3 output channels (RGB)
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (!pipeline) {
        if (context) cmsDeleteContext(context);
        return 0;
    }
    
    // 2. Create matrix stage
    cmsStage* matrix_stage = NULL;
    cmsFloat64Number matrix[9];
    cmsFloat64Number offset[3];
    
    // Generate matrix data from fuzzer input
    for (int i = 0; i < 9; i++) {
        matrix[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-10.0, 10.0);
    }
    for (int i = 0; i < 3; i++) {
        offset[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-1.0, 1.0);
    }
    
    // Adjust matrix for different types
    if (matrix_type == 0) {
        // Identity matrix
        for (int i = 0; i < 9; i++) matrix[i] = (i % 4 == 0) ? 1.0 : 0.0; // Diagonal
        for (int i = 0; i < 3; i++) offset[i] = 0.0;
    } else if (matrix_type == 2) {
        // Zero matrix
        for (int i = 0; i < 9; i++) matrix[i] = 0.0;
        for (int i = 0; i < 3; i++) offset[i] = 0.0;
    }
    
    matrix_stage = cmsStageAllocMatrix(context, 3, 3, matrix, offset);
    if (matrix_stage) {
        cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, matrix_stage);
    }
    
    // 3. Create CLUT stage
    cmsStage* clut_stage = NULL;
    // Generate CLUT table data from fuzzer input
    // For 3D CLUT with grid_points^3 entries, each with 3 output channels
    size_t clut_entries = grid_points * grid_points * grid_points;
    size_t clut_size = clut_entries * 3 * sizeof(cmsFloat32Number);
    
    if (fdp.remaining_bytes() < clut_size) {
        // Not enough data for CLUT table
        // Clean up: pipeline owns any inserted stages, so only free the pipeline
        cmsPipelineFree(pipeline);
        if (context) cmsDeleteContext(context);
        return 0;
    }
    
    std::vector<cmsFloat32Number> clut_table(clut_entries * 3);
    for (size_t i = 0; i < clut_table.size(); i++) {
        clut_table[i] = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
    }
    
    clut_stage = cmsStageAllocCLutFloat(context, grid_points, 3, 3, clut_table.data());
    if (clut_stage) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, clut_stage);
    }
    // 4. Initialize CLUT data points using cmsSliceSpaceFloat
    if (clut_stage) {
        cmsUInt32Number clut_points[] = {grid_points, grid_points, grid_points};
        // Use remaining fuzzer data for sampler
        cmsSliceSpaceFloat(3, clut_points, FloatSampler, &fdp);
    }
    
    // 5. Test forward evaluation with cmsPipelineEvalFloat
    cmsFloat32Number input[3], output[3];
    for (int i = 0; i < 3; i++) {
        input[i] = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
    }
    
    cmsPipelineEvalFloat(input, output, pipeline);
    
    // 6. Test reverse evaluation with cmsPipelineEvalReverseFloat
    if (fdp.remaining_bytes() >= 24) {
        cmsFloat32Number target[3], result[3], hint[3];
        
        for (int i = 0; i < 3; i++) {
            target[i] = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
            hint[i] = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
        }
        
        cmsPipelineEvalReverseFloat(target, result, hint, pipeline);
        
        // Test with NULL hint as well
        if (fdp.remaining_bytes() >= 12) {
            for (int i = 0; i < 3; i++) {
                target[i] = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
            }
            cmsPipelineEvalReverseFloat(target, result, NULL, pipeline);
        }
    }
    
    // 7. Sample the CLUT stage with cmsStageSampleCLutFloat
    if (clut_stage && fdp.remaining_bytes() >= 12) {
        cmsStageSampleCLutFloat(clut_stage, FloatSampler, &fdp, 0);
    }
    
    // 8. Cleanup
    // Important: Once stages are inserted into a pipeline via cmsPipelineInsertStage,
    // the pipeline owns them. Calling cmsPipelineFree will free all contained stages.
    // Do NOT call cmsStageFree individually on stages that have been inserted.
    cmsPipelineFree(pipeline);
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    return 0;
}

} // namespace harness_014
