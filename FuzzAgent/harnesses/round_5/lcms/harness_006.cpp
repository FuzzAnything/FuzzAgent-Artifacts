#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "lcms2.h"

namespace harness_006 {
// Fuzzing harness for Little CMS (lcms) library
// Target: Matrix operations, tone curve creation, and pipeline evaluation
// Focus: cmsmtrx.c (matrix operations) and cmsintrp.c (interpolation) modules
// Semantically different from:
//   harness_000.cpp - profile parsing and basic transformations
//   harness_001.cpp - device link creation and virtual profiles  
//   harness_002.cpp - PostScript generation
//   harness_003.cpp - CGATS parsing and manipulation
//   harness_004.cpp - Cube file device link creation
//   harness_005.cpp - Unknown (haven't examined yet)


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for matrix and tone curve operations
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input for different operations
    // 1. Consume fixed-size parameters first
    cmsUInt32Number input_channels = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 8);
    cmsUInt32Number output_channels = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 8);
    cmsUInt32Number matrix_rows = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 8);
    cmsUInt32Number matrix_cols = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 8);
    cmsInt32Number tone_curve_type = fdp.ConsumeIntegralInRange<cmsInt32Number>(0, 10);
    cmsFloat64Number gamma_value = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 10.0);
    cmsUInt32Number n_grid_points = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 33);
    cmsUInt32Number n_entries = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 256);
    cmsUInt32Number flags = fdp.ConsumeIntegral<cmsUInt32Number>();
    
    // 2. Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;
    
    // 3. Create pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, input_channels, output_channels);
    if (pipeline == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 4. Test matrix stage creation and insertion
    // Allocate matrix data
    size_t matrix_elements = matrix_rows * matrix_cols;
    size_t offset_elements = matrix_rows;
    
    if (fdp.remaining_bytes() < (matrix_elements + offset_elements) * sizeof(cmsFloat64Number)) {
        cmsPipelineFree(pipeline);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    std::vector<cmsFloat64Number> matrix_data;
    std::vector<cmsFloat64Number> offset_data;
    
    for (size_t i = 0; i < matrix_elements; i++) {
        matrix_data.push_back(fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-10.0, 10.0));
    }
    
    for (size_t i = 0; i < offset_elements; i++) {
        offset_data.push_back(fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-10.0, 10.0));
    }
    
    // Create matrix stage
    cmsStage* matrix_stage = cmsStageAllocMatrix(ctx, matrix_rows, matrix_cols, 
                                                 matrix_data.data(), offset_data.data());
    if (matrix_stage != NULL) {
        // Insert matrix stage at beginning
        cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, matrix_stage);
        
        // Test stage properties
        cmsUInt32Number stage_input_chans = cmsStageInputChannels(matrix_stage);
        cmsUInt32Number stage_output_chans = cmsStageOutputChannels(matrix_stage);
        cmsStageSignature stage_type = cmsStageType(matrix_stage);
        void* stage_data = cmsStageData(matrix_stage);
        cmsContext stage_ctx = cmsGetStageContextID(matrix_stage);
        
        // Note: Stage data may be NULL or point to internal structure
    }
    
    // 5. Test tone curve creation and stage insertion
    // Create parametric tone curves
    if (fdp.remaining_bytes() >= 10 * sizeof(cmsFloat64Number)) {
        cmsFloat64Number params[10];
        for (int i = 0; i < 10; i++) {
            params[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-100.0, 100.0);
        }
        
        cmsToneCurve* tone_curve = cmsBuildParametricToneCurve(ctx, tone_curve_type, params);
        if (tone_curve != NULL) {
            // Create tone curve stage
            cmsToneCurve* curves[1] = { tone_curve };
            cmsStage* tone_stage = cmsStageAllocToneCurves(ctx, 1, curves);
            if (tone_stage != NULL) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, tone_stage);
            }
            cmsFreeToneCurve(tone_curve);
        }
    }
    
    // 6. Test gamma curve creation
    cmsToneCurve* gamma_curve = cmsBuildGamma(ctx, gamma_value);
    if (gamma_curve != NULL) {
        cmsToneCurve* gamma_curves[3] = { gamma_curve, gamma_curve, gamma_curve };
        if (input_channels <= 3) {
            cmsStage* gamma_stage = cmsStageAllocToneCurves(ctx, input_channels, gamma_curves);
            if (gamma_stage != NULL) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, gamma_stage);
            }
        }
        cmsFreeToneCurve(gamma_curve);
    }
    
    // 7. Test identity stage
    cmsStage* identity_stage = cmsStageAllocIdentity(ctx, output_channels);
    if (identity_stage != NULL) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, identity_stage);
    }
    
    // 8. Test tabulated tone curves (16-bit)
    if (fdp.remaining_bytes() >= n_entries * sizeof(cmsUInt16Number)) {
        std::vector<cmsUInt16Number> table_data;
        for (cmsUInt32Number i = 0; i < n_entries; i++) {
            table_data.push_back(fdp.ConsumeIntegralInRange<cmsUInt16Number>(0, 65535));
        }
        
        cmsToneCurve* tabulated_curve = cmsBuildTabulatedToneCurve16(ctx, n_entries, table_data.data());
        if (tabulated_curve != NULL) {
            cmsToneCurve* tab_curves[1] = { tabulated_curve };
            cmsStage* tab_stage = cmsStageAllocToneCurves(ctx, 1, tab_curves);
            if (tab_stage != NULL) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, tab_stage);
            }
            cmsFreeToneCurve(tabulated_curve);
        }
    }
    
    // 9. Test CLUT stage creation (16-bit)
    size_t table_size = 1;
    for (cmsUInt32Number i = 0; i < input_channels; i++) {
        table_size *= n_grid_points;
    }
    table_size *= output_channels;
    
    if (fdp.remaining_bytes() >= table_size * sizeof(cmsUInt16Number)) {
        std::vector<cmsUInt16Number> clut_table;
        for (size_t i = 0; i < table_size; i++) {
            clut_table.push_back(fdp.ConsumeIntegralInRange<cmsUInt16Number>(0, 65535));
        }
        
        cmsStage* clut_stage = cmsStageAllocCLut16bit(ctx, n_grid_points, 
                                                      input_channels, output_channels, 
                                                      clut_table.data());
        if (clut_stage != NULL) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, clut_stage);
        }
    }
    
    // 10. Test pipeline evaluation
    cmsUInt32Number current_input_channels = cmsPipelineInputChannels(pipeline);
    cmsUInt32Number current_output_channels = cmsPipelineOutputChannels(pipeline);
    
    if (current_input_channels > 0 && current_output_channels > 0) {
        // Test 16-bit evaluation
        std::vector<cmsUInt16Number> input_16(current_input_channels);
        std::vector<cmsUInt16Number> output_16(current_output_channels);
        
        if (fdp.remaining_bytes() >= current_input_channels * sizeof(cmsUInt16Number)) {
            for (cmsUInt32Number i = 0; i < current_input_channels; i++) {
                input_16[i] = fdp.ConsumeIntegralInRange<cmsUInt16Number>(0, 65535);
            }
            
            cmsPipelineEval16(input_16.data(), output_16.data(), pipeline);
        }
        
        // Test float evaluation if we have enough data
        if (fdp.remaining_bytes() >= current_input_channels * sizeof(cmsFloat32Number)) {
            std::vector<cmsFloat32Number> input_float(current_input_channels);
            std::vector<cmsFloat32Number> output_float(current_output_channels);
            
            for (cmsUInt32Number i = 0; i < current_input_channels; i++) {
                input_float[i] = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
            }
            
            cmsPipelineEvalFloat(input_float.data(), output_float.data(), pipeline);
        }
    }
    
    // 11. Test pipeline properties
    cmsUInt32Number stage_count = cmsPipelineStageCount(pipeline);
    cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
    cmsStage* last_stage = cmsPipelineGetPtrToLastStage(pipeline);
    cmsContext pipe_ctx = cmsGetPipelineContextID(pipeline);
    
    // 12. Test pipeline concatenation with a simple second pipeline
    // Use current output channels for pipeline2 allocation
    cmsPipeline* pipeline2 = cmsPipelineAlloc(ctx, current_output_channels, current_output_channels);
    if (pipeline2 != NULL) {
        cmsStage* identity2 = cmsStageAllocIdentity(ctx, current_output_channels);
        if (identity2 != NULL) {
            cmsPipelineInsertStage(pipeline2, cmsAT_BEGIN, identity2);
        }
        
        // Try to concatenate pipelines
        cmsBool concat_result = cmsPipelineCat(pipeline, pipeline2);
        // Note: concat_result may be FALSE if pipeline structures are incompatible
        
        // If concatenation succeeded, update current channel counts
        if (concat_result) {
            current_input_channels = cmsPipelineInputChannels(pipeline);
            current_output_channels = cmsPipelineOutputChannels(pipeline);
        }
        
        cmsPipelineFree(pipeline2);
    }
    
    // 13. Test pipeline duplication
    cmsPipeline* pipeline_dup = cmsPipelineDup(pipeline);
    if (pipeline_dup != NULL) {
        // Get current channel counts for duplicated pipeline
        cmsUInt32Number dup_input_channels = cmsPipelineInputChannels(pipeline_dup);
        cmsUInt32Number dup_output_channels = cmsPipelineOutputChannels(pipeline_dup);
        
        // Evaluate duplicated pipeline
        if (dup_input_channels > 0 && dup_output_channels > 0) {
            std::vector<cmsUInt16Number> input_dup(dup_input_channels);
            std::vector<cmsUInt16Number> output_dup(dup_output_channels);
            
            if (fdp.remaining_bytes() >= dup_input_channels * sizeof(cmsUInt16Number)) {
                for (cmsUInt32Number i = 0; i < dup_input_channels; i++) {
                    input_dup[i] = fdp.ConsumeIntegralInRange<cmsUInt16Number>(0, 65535);
                }
                
                cmsPipelineEval16(input_dup.data(), output_dup.data(), pipeline_dup);
            }
        }
        cmsPipelineFree(pipeline_dup);
    }
    
    // 14. Test cmsIsMatrixShaper with created profile
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfileTHR(ctx);
    if (srgb_profile != NULL) {
        cmsBool is_matrix_shaper = cmsIsMatrixShaper(srgb_profile);
        // Note: sRGB profile should be a matrix shaper profile
        cmsCloseProfile(srgb_profile);
    }
    
    // 15. Test pipeline stage unlinking
    if (first_stage != NULL) {
        cmsStage* unlinked_stage = NULL;
        cmsPipelineUnlinkStage(pipeline, cmsAT_BEGIN, &unlinked_stage);
        if (unlinked_stage != NULL) {
            cmsStageFree(unlinked_stage);
        }
    }
    
    // 16. Cleanup
    cmsPipelineFree(pipeline);
    cmsDeleteContext(ctx);
    
    return 0;
}

} // namespace harness_006
