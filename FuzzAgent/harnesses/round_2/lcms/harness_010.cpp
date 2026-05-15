#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include "lcms2.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_010 {
// Fuzzing harness for LittleCMS2 library
// harness_010.cpp - Pipeline and multi-processing element (MPE) manipulation
// Targeting comprehensive pipeline and stage APIs for maximum coverage
// Semantic diversity from previous harnesses:
// harness_000 (basic profile parsing), harness_001 (proofing/PostScript)
// harness_002 (virtual profiles/gamut), harness_003 (CGATS/IT8)
// harness_004 (CIECAM02), harness_005 (TAC detection/sampling - touched CLUT)
// harness_006 (device link creation/I/O), harness_007 (BCHSW abstract profiles)
// harness_008 (advanced CGATS/IT8), harness_009 (cube file device link/stream I/O)
// This harness specifically targets pipeline creation, stage manipulation, and MPE operations


// Sampler function for CLUT sampling
static int CustomSampler16(const cmsUInt16Number In[], cmsUInt16Number Out[], void* Cargo) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(Cargo);
    
    // Handle NULL Out parameter (called from cmsSliceSpace16)
    if (Out == NULL) {
        // Just consume some data to advance the fuzzer state
        if (fdp->remaining_bytes() > 0) {
            fdp->ConsumeIntegral<cmsUInt16Number>();
        }
        return 1;
    }
    
    // Generate output based on input and fuzzer data
    // Assume up to 16 channels (typical maximum for LCMS)
    for (int i = 0; i < 16 && fdp->remaining_bytes() > 0; i++) {
        // Use input value as base if available, otherwise use 0
        cmsUInt16Number base = (In != NULL) ? In[i] : 0;
        Out[i] = base ^ fdp->ConsumeIntegralInRange<cmsUInt16Number>(0, 65535);
    }
    return 1;
}

static int CustomSamplerFloat(const cmsFloat32Number In[], cmsFloat32Number Out[], void* Cargo) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(Cargo);
    
    // Handle NULL Out parameter (called from cmsSliceSpaceFloat)
    if (Out == NULL) {
        // Just consume some data to advance the fuzzer state
        if (fdp->remaining_bytes() > 0) {
            fdp->ConsumeFloatingPoint<cmsFloat32Number>();
        }
        return 1;
    }
    
    // Generate output based on input and fuzzer data
    // Assume up to 16 channels (typical maximum for LCMS)
    for (int i = 0; i < 16 && fdp->remaining_bytes() > 0; i++) {
        // Use input value as base if available, otherwise use 0
        cmsFloat32Number base = (In != NULL) ? In[i] : 0.0f;
        // Generate output by adding random offset to base
        Out[i] = base + fdp->ConsumeFloatingPointInRange<cmsFloat32Number>(-0.5f, 0.5f);
        // Clamp to valid range [0.0f, 1.0f]
        if (Out[i] < 0.0f) Out[i] = 0.0f;
        if (Out[i] > 1.0f) Out[i] = 1.0f;
    }
    return 1;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) return 0;  // Need minimum data for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) return 0;
    
    // Test various pipeline creation and manipulation scenarios
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Basic pipeline with identity stage
            cmsUInt32Number inputChannels = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 16);
            cmsUInt32Number outputChannels = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 16);
            
            cmsPipeline* pipeline = cmsPipelineAlloc(context, inputChannels, outputChannels);
            if (!pipeline) {
                cmsDeleteContext(context);
                return 0;
            }
            
            // Add identity stage
            cmsStage* identity = cmsStageAllocIdentity(context, outputChannels);
            if (identity) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, identity);
            }
            
            // Test pipeline properties
            cmsUInt32Number actualInput = cmsPipelineInputChannels(pipeline);
            cmsUInt32Number actualOutput = cmsPipelineOutputChannels(pipeline);
            cmsUInt32Number stageCount = cmsPipelineStageCount(pipeline);
            cmsContext pipeContext = cmsGetPipelineContextID(pipeline);
            
            // Evaluate pipeline
            std::vector<cmsFloat32Number> inFloat(actualInput);
            std::vector<cmsFloat32Number> outFloat(actualOutput);
            std::vector<cmsUInt16Number> in16(actualInput);
            std::vector<cmsUInt16Number> out16(actualOutput);
            
            for (size_t i = 0; i < actualInput && fdp.remaining_bytes() > 0; i++) {
                inFloat[i] = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
                in16[i] = fdp.ConsumeIntegralInRange<cmsUInt16Number>(0, 65535);
            }
            
            cmsPipelineEvalFloat(inFloat.data(), outFloat.data(), pipeline);
            cmsPipelineEval16(in16.data(), out16.data(), pipeline);
            
            cmsPipelineFree(pipeline);
            break;
        }
        
        case 1: {
            // Scenario 2: Pipeline with tone curve stages
            cmsUInt32Number channels = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 8);
            
            cmsPipeline* pipeline = cmsPipelineAlloc(context, channels, channels);
            if (!pipeline) {
                cmsDeleteContext(context);
                return 0;
            }
            
            // Create tone curves
            std::vector<cmsToneCurve*> curves(channels);
            for (size_t i = 0; i < channels; i++) {
                if (fdp.ConsumeBool()) {
                    // Parametric tone curve
                    cmsFloat64Number params[10];
                    for (int j = 0; j < 10 && fdp.remaining_bytes() > 0; j++) {
                        params[j] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-10.0, 10.0);
                    }
                    int type = fdp.ConsumeIntegralInRange<int>(1, 5);
                    curves[i] = cmsBuildParametricToneCurve(context, type, params);
                } else {
                    // Tabulated tone curve
                    cmsUInt32Number nEntries = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 256);
                    std::vector<cmsUInt16Number> values(nEntries);
                    for (size_t j = 0; j < nEntries && fdp.remaining_bytes() > 0; j++) {
                        values[j] = fdp.ConsumeIntegralInRange<cmsUInt16Number>(0, 65535);
                    }
                    curves[i] = cmsBuildTabulatedToneCurve16(context, nEntries, values.data());
                }
            }
            
            // Add tone curve stage
            cmsStage* toneCurveStage = cmsStageAllocToneCurves(context, channels, curves.data());
            if (toneCurveStage) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, toneCurveStage);
            }
            
            // Test tone curve operations
            for (size_t i = 0; i < channels; i++) {
                if (curves[i]) {
                    cmsBool isMulti = cmsIsToneCurveMultisegment(curves[i]);
                    cmsBool isLinear = cmsIsToneCurveLinear(curves[i]);
                    cmsBool isMonotonic = cmsIsToneCurveMonotonic(curves[i]);
                    cmsBool isDescending = cmsIsToneCurveDescending(curves[i]);
                    
                    // Evaluate curve
                    cmsFloat32Number evalFloat = cmsEvalToneCurveFloat(curves[i], 
                        fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f));
                    cmsUInt16Number eval16 = cmsEvalToneCurve16(curves[i],
                        fdp.ConsumeIntegralInRange<cmsUInt16Number>(0, 65535));
                    
                    // Get parametric type if applicable
                    cmsInt32Number paramType = cmsGetToneCurveParametricType(curves[i]);
                    
                    // Estimate gamma
                    cmsFloat64Number gamma = cmsEstimateGamma(curves[i], 0.001);
                    
                    // Get segment information
                    const cmsCurveSegment* segment = cmsGetToneCurveSegment(0, curves[i]);
                    
                    // Get estimated table
                    cmsUInt32Number tableEntries = cmsGetToneCurveEstimatedTableEntries(curves[i]);
                    if (tableEntries > 0) {
                        const cmsUInt16Number* table = cmsGetToneCurveEstimatedTable(curves[i]);
                    }
                }
            }
            
            // Clean up tone curves
            for (size_t i = 0; i < channels; i++) {
                if (curves[i]) cmsFreeToneCurve(curves[i]);
            }
            
            cmsPipelineFree(pipeline);
            break;
        }
        
        case 2: {
            // Scenario 3: Pipeline with matrix stage
            cmsUInt32Number rows = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 8);
            cmsUInt32Number cols = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 8);
            
            cmsPipeline* pipeline = cmsPipelineAlloc(context, cols, rows);
            if (!pipeline) {
                cmsDeleteContext(context);
                return 0;
            }
            
            // Create matrix and offset
            std::vector<cmsFloat64Number> matrix(rows * cols);
            std::vector<cmsFloat64Number> offset(rows);
            
            for (size_t i = 0; i < matrix.size() && fdp.remaining_bytes() > 0; i++) {
                matrix[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-10.0, 10.0);
            }
            for (size_t i = 0; i < offset.size() && fdp.remaining_bytes() > 0; i++) {
                offset[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-1.0, 1.0);
            }
            
            cmsStage* matrixStage = cmsStageAllocMatrix(context, rows, cols, matrix.data(), offset.data());
            if (matrixStage) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, matrixStage);
            }
            
            cmsPipelineFree(pipeline);
            break;
        }
        
        case 3: {
            // Scenario 4: Pipeline with CLUT stages
            cmsUInt32Number inputChannels = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 8);
            cmsUInt32Number outputChannels = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 8);
            cmsUInt32Number gridPoints = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 17);
            
            cmsPipeline* pipeline = cmsPipelineAlloc(context, inputChannels, outputChannels);
            if (!pipeline) {
                cmsDeleteContext(context);
                return 0;
            }
            
            // Test 16-bit CLUT
            size_t tableSize = 1;
            for (cmsUInt32Number i = 0; i < inputChannels; i++) {
                tableSize *= gridPoints;
            }
            tableSize *= outputChannels;
            
            if (tableSize > 0 && tableSize < 1000000) {  // Reasonable size limit
                std::vector<cmsUInt16Number> table16(tableSize);
                for (size_t i = 0; i < tableSize && fdp.remaining_bytes() > 0; i++) {
                    table16[i] = fdp.ConsumeIntegralInRange<cmsUInt16Number>(0, 65535);
                }
                
                cmsStage* clut16 = cmsStageAllocCLut16bit(context, gridPoints, 
                                                         inputChannels, outputChannels, 
                                                         table16.data());
                if (clut16) {
                    cmsPipelineInsertStage(pipeline, cmsAT_END, clut16);
                    
                    // Test CLUT sampling
                    cmsStageSampleCLut16bit(clut16, CustomSampler16, &fdp, 0);
                    
                    // Test with inspect flag
                    if (fdp.ConsumeBool()) {
                        cmsStageSampleCLut16bit(clut16, CustomSampler16, &fdp, SAMPLER_INSPECT);
                    }
                }
            }
            
            cmsPipelineFree(pipeline);
            break;
        }
        
        case 4: {
            // Scenario 5: Complex pipeline with multiple stages
            cmsUInt32Number channels = fdp.ConsumeIntegralInRange<cmsUInt32Number>(3, 4);
            
            cmsPipeline* pipeline = cmsPipelineAlloc(context, channels, channels);
            if (!pipeline) {
                cmsDeleteContext(context);
                return 0;
            }
            
            // Add identity stage at beginning
            cmsStage* identity1 = cmsStageAllocIdentity(context, channels);
            if (identity1) {
                cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, identity1);
            }
            
            // Create and add matrix stage
            std::vector<cmsFloat64Number> matrix(channels * channels);
            std::vector<cmsFloat64Number> offset(channels);
            for (size_t i = 0; i < matrix.size() && fdp.remaining_bytes() > 0; i++) {
                matrix[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-2.0, 2.0);
            }
            for (size_t i = 0; i < offset.size() && fdp.remaining_bytes() > 0; i++) {
                offset[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-0.5, 0.5);
            }
            
            cmsStage* matrixStage = cmsStageAllocMatrix(context, channels, channels, 
                                                       matrix.data(), offset.data());
            if (matrixStage) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, matrixStage);
            }
            
            // Add tone curve stage
            std::vector<cmsToneCurve*> curves(channels);
            for (size_t i = 0; i < channels; i++) {
                cmsFloat64Number gamma = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 5.0);
                curves[i] = cmsBuildGamma(context, gamma);
            }
            
            cmsStage* toneCurveStage = cmsStageAllocToneCurves(context, channels, curves.data());
            if (toneCurveStage) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, toneCurveStage);
            }
            
            // Add identity stage at end
            cmsStage* identity2 = cmsStageAllocIdentity(context, channels);
            if (identity2) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, identity2);
            }
            
            // Test stage navigation
            cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            cmsStage* lastStage = cmsPipelineGetPtrToLastStage(pipeline);
            
            // Traverse stages
            cmsStage* current = firstStage;
            while (current) {
                cmsUInt32Number inChannels = cmsStageInputChannels(current);
                cmsUInt32Number outChannels = cmsStageOutputChannels(current);
                cmsStageSignature sig = cmsStageType(current);
                void* stageData = cmsStageData(current);
                cmsContext stageContext = cmsGetStageContextID(current);
                
                current = cmsStageNext(current);
            }
            
            // Test pipeline concatenation
            cmsPipeline* pipeline2 = cmsPipelineAlloc(context, channels, channels);
            if (pipeline2) {
                cmsBool catResult = cmsPipelineCat(pipeline, pipeline2);
                cmsPipelineFree(pipeline2);
            }
            
            // Test reverse evaluation
            if (channels == 3) {  // Reverse eval requires specific channel counts
                std::vector<cmsFloat32Number> target(3);
                std::vector<cmsFloat32Number> result(3);
                std::vector<cmsFloat32Number> hint(3);
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                    target[i] = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
                    hint[i] = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
                }
                cmsPipelineEvalReverseFloat(target.data(), result.data(), hint.data(), pipeline);
            }
            
            // Clean up tone curves
            for (size_t i = 0; i < channels; i++) {
                if (curves[i]) cmsFreeToneCurve(curves[i]);
            }
            
            cmsPipelineFree(pipeline);
            break;
        }
        
        case 5: {
            // Scenario 6: Stage manipulation (insert, unlink, dup)
            cmsUInt32Number channels = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 4);
            
            cmsPipeline* pipeline = cmsPipelineAlloc(context, channels, channels);
            if (!pipeline) {
                cmsDeleteContext(context);
                return 0;
            }
            
            // Add initial stage
            cmsStage* stage1 = cmsStageAllocIdentity(context, channels);
            if (stage1) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, stage1);
            }
            
            // Duplicate and insert
            cmsStage* stage2 = cmsStageDup(stage1);
            if (stage2) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, stage2);
            }
            
            // Create and insert tone curve stage
            std::vector<cmsToneCurve*> curves(channels);
            for (size_t i = 0; i < channels; i++) {
                cmsFloat64Number gamma = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.5, 2.5);
                curves[i] = cmsBuildGamma(context, gamma);
            }
            
            cmsStage* toneStage = cmsStageAllocToneCurves(context, channels, curves.data());
            if (toneStage) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, toneStage);
            }
            
            // Unlink a stage
            cmsStage* unlinked = nullptr;
            if (fdp.ConsumeBool()) {
                cmsPipelineUnlinkStage(pipeline, cmsAT_END, &unlinked);
                if (unlinked) {
                    cmsStageFree(unlinked);
                }
            }
            
            // Test stage removal from beginning
            if (fdp.ConsumeBool()) {
                cmsPipelineUnlinkStage(pipeline, cmsAT_BEGIN, &unlinked);
                if (unlinked) {
                    cmsStageFree(unlinked);
                }
            }
            
            // Clean up
            for (size_t i = 0; i < channels; i++) {
                if (curves[i]) cmsFreeToneCurve(curves[i]);
            }
            
            cmsPipelineFree(pipeline);
            break;
        }
        
        case 6: {
            // Scenario 7: Granular CLUT
            cmsUInt32Number inputChannels = 3;
            cmsUInt32Number outputChannels = 3;
            
            cmsPipeline* pipeline = cmsPipelineAlloc(context, inputChannels, outputChannels);
            if (!pipeline) {
                cmsDeleteContext(context);
                return 0;
            }
            
            // Create granular CLUT points
            std::vector<cmsUInt32Number> clutPoints(inputChannels);
            for (cmsUInt32Number i = 0; i < inputChannels; i++) {
                clutPoints[i] = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 9);
            }
            
            // Calculate table size
            size_t tableSize = 1;
            for (cmsUInt32Number i = 0; i < inputChannels; i++) {
                tableSize *= clutPoints[i];
            }
            tableSize *= outputChannels;
            
            if (tableSize > 0 && tableSize < 50000) {  // Reasonable limit
                std::vector<cmsUInt16Number> table16(tableSize);
                for (size_t i = 0; i < tableSize && fdp.remaining_bytes() > 0; i++) {
                    table16[i] = fdp.ConsumeIntegralInRange<cmsUInt16Number>(0, 65535);
                }
                
                cmsStage* granularClut = cmsStageAllocCLut16bitGranular(context, 
                                                                       clutPoints.data(),
                                                                       inputChannels, 
                                                                       outputChannels, 
                                                                       table16.data());
                if (granularClut) {
                    cmsPipelineInsertStage(pipeline, cmsAT_END, granularClut);
                }
            }
            
            cmsPipelineFree(pipeline);
            break;
        }
        
        case 7: {
            // Scenario 8: Float CLUT and sampling
            cmsUInt32Number inputChannels = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 4);
            cmsUInt32Number outputChannels = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 4);
            cmsUInt32Number gridPoints = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 9);
            
            cmsPipeline* pipeline = cmsPipelineAlloc(context, inputChannels, outputChannels);
            if (!pipeline) {
                cmsDeleteContext(context);
                return 0;
            }
            
            // Test float CLUT
            size_t tableSize = 1;
            for (cmsUInt32Number i = 0; i < inputChannels; i++) {
                tableSize *= gridPoints;
            }
            tableSize *= outputChannels;
            
            if (tableSize > 0 && tableSize < 100000) {
                std::vector<cmsFloat32Number> tableFloat(tableSize);
                for (size_t i = 0; i < tableSize && fdp.remaining_bytes() > 0; i++) {
                    tableFloat[i] = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
                }
                
                cmsStage* clutFloat = cmsStageAllocCLutFloat(context, gridPoints, 
                                                            inputChannels, outputChannels, 
                                                            tableFloat.data());
                if (clutFloat) {
                    cmsPipelineInsertStage(pipeline, cmsAT_END, clutFloat);
                    
                    // Test float sampling
                    cmsStageSampleCLutFloat(clutFloat, CustomSamplerFloat, &fdp, 0);
                }
            }
            
            cmsPipelineFree(pipeline);
            break;
        }
    }
    
    // Test additional standalone tone curve operations
    if (fdp.remaining_bytes() > 50) {
        // Create and test multisegment tone curve
        cmsCurveSegment segments[2];
        
        // First segment: parametric
        segments[0].x0 = 0.0f;
        segments[0].x1 = 0.5f;
        segments[0].Type = 4;  // Type 4 parametric curve
        for (int i = 0; i < 10 && fdp.remaining_bytes() > 0; i++) {
            segments[0].Params[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-5.0, 5.0);
        }
        
        // Second segment: sampled
        segments[1].x0 = 0.5f;
        segments[1].x1 = 1.0f;
        segments[1].Type = 0;
        segments[1].nGridPoints = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 10);
        if (segments[1].nGridPoints > 0 && fdp.remaining_bytes() > segments[1].nGridPoints * sizeof(cmsFloat32Number)) {
            std::vector<cmsFloat32Number> sampledPoints(segments[1].nGridPoints);
            for (cmsUInt32Number i = 0; i < segments[1].nGridPoints && fdp.remaining_bytes() > 0; i++) {
                sampledPoints[i] = fdp.ConsumeFloatingPointInRange<cmsFloat32Number>(0.0f, 1.0f);
            }
            segments[1].SampledPoints = sampledPoints.data();
            
            cmsToneCurve* multiCurve = cmsBuildSegmentedToneCurve(context, 2, segments);
            if (multiCurve) {
                // Test curve operations
                cmsToneCurve* reversed = cmsReverseToneCurve(multiCurve);
                if (reversed) {
                    cmsFreeToneCurve(reversed);
                }
                
                cmsToneCurve* reversedEx = cmsReverseToneCurveEx(
                    fdp.ConsumeIntegralInRange<cmsUInt32Number>(10, 1000), 
                    multiCurve);
                if (reversedEx) {
                    cmsFreeToneCurve(reversedEx);
                }
                
                // Join with gamma curve
                cmsToneCurve* gammaCurve = cmsBuildGamma(context, 2.2);
                if (gammaCurve) {
                    cmsToneCurve* joined = cmsJoinToneCurve(context, multiCurve, gammaCurve, 256);
                    if (joined) {
                        cmsFreeToneCurve(joined);
                    }
                    cmsFreeToneCurve(gammaCurve);
                }
                
                // Smooth curve
                cmsSmoothToneCurve(multiCurve, fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 10.0));
                
                cmsFreeToneCurve(multiCurve);
            }
        }
    }
    
    // Test slice space functions
    if (fdp.remaining_bytes() > 100) {
        cmsUInt32Number nInputs = 3;
        std::vector<cmsUInt32Number> clutPoints(nInputs);
        for (cmsUInt32Number i = 0; i < nInputs; i++) {
            clutPoints[i] = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 5);
        }
        
        cmsSliceSpace16(nInputs, clutPoints.data(), CustomSampler16, &fdp);
        cmsSliceSpaceFloat(nInputs, clutPoints.data(), CustomSamplerFloat, &fdp);
    }
    
    cmsDeleteContext(context);
    return 0;
}

} // namespace harness_010
