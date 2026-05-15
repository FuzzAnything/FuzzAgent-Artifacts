#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_014 {
// Fuzzing harness for Little CMS 2 library
// Target: LUT pipeline evaluation and gamut desaturation operations
// Coverage: 100+ undiscovered branches
// Target APIs: cmsDesaturateLab (37 branches), cmsPipelineEvalReverseFloat (32 branches),
//              cmsStageSampleCLutFloat (30 branches), cmsStageAllocCLutFloat (14 branches),
//              cmsSliceSpaceFloat (10 branches)
// Setup: cmsCreateContext, cmsPipelineAlloc, cmsStageAllocCLutFloat
// Cleanup: cmsPipelineFree, cmsDeleteContext
// Invocation sequence:
//   1. cmsCreateContext create context
//   2. cmsPipelineAlloc allocate pipeline
//   3. cmsStageAllocCLutFloat create CLUT float stage
//   4. cmsPipelineInsertStage add stage to pipeline
//   5. cmsStageSampleCLutFloat sample CLUT
//   6. cmsSliceSpaceFloat space slicing
//   7. cmsPipelineEvalReverseFloat evaluate pipeline reverse
//   8. cmsDesaturateLab perform gamut desaturation
//   9. cmsPipelineFree free pipeline (frees all stages)
//  10. cmsDeleteContext cleanup context
// Strategy: Test various CLUT configurations, pipeline evaluations, and gamut desaturation
//           using fuzzed floating-point data and Lab color values


// Define a simple sampler function for cmsStageSampleCLutFloat
static int SimpleFloatSampler(const cmsFloat32Number In[],
                              cmsFloat32Number Out[],
                              void* Cargo) {
    // Simple identity transformation for testing
    // Just copy input to output
    // Check for NULL Out pointer to prevent crashes (cmsSliceSpaceFloat may pass NULL)
    if (Out == NULL) {
        return 1; // Return success without writing
    }
    
    size_t num_channels = reinterpret_cast<size_t>(Cargo);
    for (size_t i = 0; i < num_channels; i++) {
        Out[i] = In[i];
    }
    return 1; // Success
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for context, pipeline setup,
    // CLUT table data, and Lab values
    if (size < 200) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create context for pipeline operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        // If context creation fails, return early
        return 0;
    }
    
    // Step 2: Allocate pipeline with fuzzed channel counts
    // Determine input and output channel counts (1-8 channels for reasonable testing)
    cmsUInt32Number input_channels = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 8);
    cmsUInt32Number output_channels = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 8);
    
    cmsPipeline* pipeline = cmsPipelineAlloc(context, input_channels, output_channels);
    if (pipeline == NULL) {
        // If pipeline allocation fails, cleanup and return
        cmsDeleteContext(context);
        return 0;
    }
    
    // Step 3: Create CLUT float stage
    // Determine grid points (2-10 for reasonable CLUT size)
    cmsUInt32Number grid_points = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 10);
    
    // Calculate required table size for CLUT
    size_t table_size = 1;
    for (cmsUInt32Number i = 0; i < input_channels; i++) {
        table_size *= grid_points;
    }
    table_size *= output_channels;
    
    // Generate CLUT table data from fuzzer input
    std::vector<cmsFloat32Number> clut_table;
    clut_table.reserve(table_size);
    
    // Consume floating-point values for CLUT table
    // Limit consumption to avoid exhausting input
    size_t max_table_entries = fdp.ConsumeIntegralInRange<size_t>(0, table_size);
    for (size_t i = 0; i < max_table_entries && fdp.remaining_bytes() >= sizeof(float); i++) {
        cmsFloat32Number value = fdp.ConsumeFloatingPoint<cmsFloat32Number>();
        clut_table.push_back(value);
    }
    
    // If we didn't get enough data, pad with zeros
    while (clut_table.size() < table_size) {
        clut_table.push_back(0.0f);
    }
    
    // Create CLUT float stage
    cmsStage* clut_stage = cmsStageAllocCLutFloat(context, grid_points, 
                                                  input_channels, output_channels,
                                                  clut_table.data());
    
    if (clut_stage == NULL) {
        // If CLUT stage allocation fails, cleanup and return
        cmsPipelineFree(pipeline);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Add the stage to the pipeline
    cmsPipelineInsertStage(pipeline, cmsAT_END, clut_stage);
    
    // Step 4: Sample CLUT using cmsStageSampleCLutFloat
    // Use our simple sampler function
    size_t sampler_cargo = output_channels; // Pass output channel count as cargo
    cmsBool sample_result = cmsStageSampleCLutFloat(clut_stage, SimpleFloatSampler,
                                                    reinterpret_cast<void*>(sampler_cargo),
                                                    0); // No special flags
    // Sample result can be TRUE or FALSE, we continue regardless
    
    // Step 5: Test space slicing with cmsSliceSpaceFloat
    // Create array of grid points for slicing
    std::vector<cmsUInt32Number> slice_points(input_channels);
    for (cmsUInt32Number i = 0; i < input_channels; i++) {
        slice_points[i] = grid_points;
    }
    
    // Perform space slicing
    cmsBool slice_result = cmsSliceSpaceFloat(input_channels, slice_points.data(),
                                              SimpleFloatSampler,
                                              reinterpret_cast<void*>(output_channels));
    // Slice result can be TRUE or FALSE, we continue regardless
    
    // Step 6: Evaluate pipeline in reverse using cmsPipelineEvalReverseFloat
    // Generate target values from fuzzer input
    std::vector<cmsFloat32Number> target(std::max(output_channels, 4u));
    std::vector<cmsFloat32Number> result(input_channels);
    std::vector<cmsFloat32Number> hint(std::max(output_channels, 4u));
    
    // Fill target array with fuzzed floating-point values
    for (cmsUInt32Number i = 0; i < output_channels && fdp.remaining_bytes() >= sizeof(float); i++) {
        target[i] = fdp.ConsumeFloatingPoint<cmsFloat32Number>();
    }
    
    // Fill hint array (optional, can be NULL)
    for (cmsUInt32Number i = 0; i < output_channels && fdp.remaining_bytes() >= sizeof(float); i++) {
        hint[i] = fdp.ConsumeFloatingPoint<cmsFloat32Number>();
    }
    
    // Perform reverse pipeline evaluation
    cmsBool eval_result = cmsPipelineEvalReverseFloat(target.data(), result.data(),
                                                      hint.data(), pipeline);
    // Evaluation result can be TRUE or FALSE, we continue regardless
    
    // Step 7: Test gamut desaturation with cmsDesaturateLab
    // Generate Lab color values from fuzzer input
    cmsCIELab lab_color;
    if (fdp.remaining_bytes() >= sizeof(double) * 4) {
        lab_color.L = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
        lab_color.a = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        lab_color.b = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        
        // Generate desaturation bounds from fuzzer input
        double amax = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        double amin = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        double bmax = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        double bmin = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        
        // Ensure amax > amin and bmax > bmin for valid bounds
        if (amax < amin) std::swap(amax, amin);
        if (bmax < bmin) std::swap(bmax, bmin);
        
        // Perform gamut desaturation
        cmsBool desat_result = cmsDesaturateLab(&lab_color, amax, amin, bmax, bmin);
        // Desaturation result can be TRUE or FALSE
        
        // Test with extreme values as well
        if (fdp.remaining_bytes() >= sizeof(double) * 4) {
            cmsCIELab extreme_lab;
            extreme_lab.L = fdp.ConsumeFloatingPoint<double>();
            extreme_lab.a = fdp.ConsumeFloatingPoint<double>();
            extreme_lab.b = fdp.ConsumeFloatingPoint<double>();
            
            cmsDesaturateLab(&extreme_lab, 
                            fdp.ConsumeFloatingPoint<double>(),
                            fdp.ConsumeFloatingPoint<double>(),
                            fdp.ConsumeFloatingPoint<double>(),
                            fdp.ConsumeFloatingPoint<double>());
        }
    }

    // Step 9: Free the pipeline
    cmsPipelineFree(pipeline);
    
    // Step 10: Delete the context
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_014
