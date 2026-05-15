// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsPipeline* create_test_pipeline(cmsContext context, const uint8_t* data, size_t size) {
    if (size < 2) return NULL;
    
    cmsUInt32Number input_channels = (data[0] % 16) + 1;
    cmsUInt32Number output_channels = (data[1] % 16) + 1;
    
    return cmsPipelineAlloc(context, input_channels, output_channels);
}

static void add_test_stages(cmsPipeline* pipeline, cmsContext context, const uint8_t* data, size_t size) {
    if (!pipeline || size < 3) return;
    
    size_t offset = 2;
    
    while (offset + 1 < size) {
        uint8_t stage_type = data[offset] % 3;
        offset++;
        
        cmsStage* stage = NULL;
        
        switch (stage_type) {
            case 0:
                stage = cmsStageAllocIdentity(context, cmsPipelineInputChannels(pipeline));
                break;
            case 1:
                if (offset + 9 <= size && cmsPipelineInputChannels(pipeline) >= 3 && cmsPipelineOutputChannels(pipeline) >= 3) {
                    cmsFloat64Number matrix[9];
                    cmsFloat64Number offset_vec[3];
                    
                    for (int i = 0; i < 9 && offset < size; i++) {
                        matrix[i] = (cmsFloat64Number)data[offset] / 255.0;
                        offset++;
                    }
                    for (int i = 0; i < 3 && offset < size; i++) {
                        offset_vec[i] = (cmsFloat64Number)data[offset] / 255.0;
                        offset++;
                    }
                    
                    stage = cmsStageAllocMatrix(context, 3, 3, matrix, offset_vec);
                }
                break;
            case 2:
                if (offset + 1 <= size) {
                    cmsUInt32Number n_entries = (data[offset] % 16) + 2;
                    offset++;
                    
                    if (offset + n_entries * 3 <= size && cmsPipelineInputChannels(pipeline) == 3) {
                        cmsToneCurve* curves[3];
                        for (int i = 0; i < 3; i++) {
                            cmsFloat32Number table[256];
                            for (int j = 0; j < n_entries && offset < size; j++) {
                                table[j] = (cmsFloat32Number)data[offset] / 255.0;
                                offset++;
                            }
                            curves[i] = cmsBuildTabulatedToneCurveFloat(context, n_entries, table);
                        }
                        stage = cmsStageAllocToneCurves(context, 3, curves);
                        for (int i = 0; i < 3; i++) {
                            cmsFreeToneCurve(curves[i]);
                        }
                    }
                }
                break;
        }
        
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        }
        
        if (offset >= size) break;
    }
}

int LLVMFuzzerTestOneInput_101(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    cmsPipeline* pipeline = create_test_pipeline(context, Data, Size);
    if (!pipeline) {
        cmsDeleteContext(context);
        return 0;
    }
    
    add_test_stages(pipeline, context, Data, Size);
    
    cmsUInt32Number input_channels = cmsPipelineInputChannels(pipeline);
    cmsUInt32Number output_channels = cmsPipelineOutputChannels(pipeline);
    cmsUInt32Number stage_count = cmsPipelineStageCount(pipeline);
    
    if (stage_count > 0) {
        cmsStage* retrieved_stages[5] = {NULL};
        cmsStageSignature expected_types[5];
        
        cmsStage* current = NULL;
        for (cmsUInt32Number i = 0; i < stage_count && i < 5; i++) {
            if (i == 0) {
                current = cmsPipelineGetPtrToFirstStage(pipeline);
            } else if (current) {
                current = cmsStageNext(current);
            }
            
            if (current) {
                expected_types[i] = cmsStageType(current);
            }
        }
        
        if (stage_count > 0) {
            cmsPipelineCheckAndRetreiveStages(pipeline, stage_count < 5 ? stage_count : 5,
                                              expected_types[0], &retrieved_stages[0],
                                              expected_types[1], &retrieved_stages[1],
                                              expected_types[2], &retrieved_stages[2],
                                              expected_types[3], &retrieved_stages[3],
                                              expected_types[4], &retrieved_stages[4]);
        }
    }
    
    cmsPipeline* dup_pipeline = cmsPipelineDup(pipeline);
    if (dup_pipeline) {
        cmsPipelineInputChannels(dup_pipeline);
        cmsPipelineOutputChannels(dup_pipeline);
        cmsPipelineStageCount(dup_pipeline);
        cmsPipelineFree(dup_pipeline);
    }
    
    cmsPipelineFree(pipeline);
    cmsDeleteContext(context);
    
    return 0;
}