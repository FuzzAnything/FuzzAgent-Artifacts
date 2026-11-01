// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void cleanup_stages(cmsStage *s1, cmsStage *s2, cmsStage *s3, cmsStage *s4, cmsStage *s5, cmsStage *s6) {
    if (s1) cmsStageFree(s1);
    if (s2) cmsStageFree(s2);
    if (s3) cmsStageFree(s3);
    if (s4) cmsStageFree(s4);
    if (s5) cmsStageFree(s5);
    if (s6) cmsStageFree(s6);
}

static cmsContext create_context(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    return cmsCreateContext(NULL, NULL);
}

int LLVMFuzzerTestOneInput_181(const uint8_t *Data, size_t Size) {
    if (Size < 15) {
        return 0;
    }

    cmsContext ContextID = create_context(Data, Size);
    if (!ContextID) {
        return 0;
    }

    // Parse input for parameters
    cmsUInt32Number nGridPoints = (Data[0] % 10) + 1; // Reduced max to 10 to keep sizes manageable
    cmsUInt32Number inputChan = (Data[1] % 4) + 1;    // 1 to 4 (MAX_INPUT_DIMENSIONS is typically 4)
    cmsUInt32Number outputChan = (Data[2] % 5) + 1;   // 1 to 5
    cmsUInt32Number nChannels = (Data[3] % 5) + 1;    // 1 to 5

    // Prepare granular points
    cmsUInt32Number clutPoints[5] = {0};
    for (int i = 0; i < 4; i++) {
        clutPoints[i] = (Data[4 + i] % 10) + 1;
    }

    // Calculate required table size based on the ACTUAL input channels used for the specific call,
    // but ensure we respect the internal MAX_INPUT_DIMENSIONS (4) logic if the library expects it.
    // The error indicates a buffer overflow when reading from the table.
    // The library likely calculates the size as (product of grid points for all input channels) * outputChan.
    // For granular, it uses clutPoints[i] for i < inputChan.
    // For uniform, it uses nGridPoints for i < inputChan.
    
    // Calculate size for uniform CLUT (cmsStageAllocCLut16bit)
    // The library might internally assume up to 4 dimensions even if inputChan is less, 
    // OR it strictly uses inputChan. The prompt says: "buffer size should be (nGridPoints ^ 4) * outputChan".
    // Let's strictly follow the prompt's instruction for the uniform case to be safe.
    size_t table16_size_uniform = 1;
    for (int i = 0; i < 4; i++) { 
        table16_size_uniform *= nGridPoints; 
    }
    table16_size_uniform *= outputChan;
    table16_size_uniform *= sizeof(cmsUInt16Number);

    // Calculate size for granular CLUT (cmsStageAllocCLut16bitGranular)
    // Size = (product of clutPoints[i] for i < inputChan) * outputChan
    size_t table16_size_granular = 1;
    for (int i = 0; i < inputChan; i++) {
        table16_size_granular *= clutPoints[i];
    }
    table16_size_granular *= outputChan;
    table16_size_granular *= sizeof(cmsUInt16Number);

    // Use the larger of the two to ensure safety for both calls, or allocate separately.
    // To be safe and simple, we allocate based on the prompt's specific instruction for the uniform case
    // which seems to be the source of the overflow if we under-allocated.
    // However, the granular call failed in the ASAN log.
    // Let's calculate the exact size needed for the granular call specifically.
    
    // Re-evaluating: The ASAN log shows a read at an address just past the allocated buffer.
    // This implies the library read more elements than we provided.
    // If inputChan is 4, and clutPoints are [2,2,2,2], size is 16 * outputChan.
    // If the library internally pads to 4 dimensions regardless of inputChan, we must provide 4 dims.
    // The prompt says: "Specifically, the buffer size should be (nGridPoints ^ 4) * outputChan... to match the internal behavior".
    // This implies even for granular, if we want to be safe, we might need to consider the max dimensions.
    // But granular takes an array. Let's assume the library reads exactly inputChan dimensions.
    // The overflow suggests inputChan might be > 4 or the calculation is wrong.
    // inputChan is capped at 4 in the code above.
    // Let's ensure the buffer is large enough for the worst-case scenario of 4 dimensions.
    
    size_t max_table16_size = 1;
    // Assume worst case 4 dimensions for both uniform and granular to be safe
    for (int i = 0; i < 4; i++) {
        max_table16_size *= (nGridPoints > clutPoints[i] ? nGridPoints : clutPoints[i]);
    }
    max_table16_size *= outputChan;
    max_table16_size *= sizeof(cmsUInt16Number);

    // Cap size to prevent huge allocations
    if (max_table16_size > 1024 * 1024) max_table16_size = 1024 * 1024;

    cmsUInt16Number *table16 = NULL;
    if (max_table16_size > 0) {
        table16 = (cmsUInt16Number *)malloc(max_table16_size);
        if (table16) {
            memset(table16, 0, max_table16_size);
            for (size_t i = 0; i < max_table16_size / sizeof(cmsUInt16Number) && i < Size; i++) {
                table16[i] = Data[i % Size];
            }
        }
    }

    // For Float CLUT
    size_t tableFloat_size = max_table16_size / sizeof(cmsUInt16Number) * sizeof(cmsFloat32Number);
    if (tableFloat_size > 1024 * 1024) tableFloat_size = 1024 * 1024;
    
    cmsFloat32Number *tableFloat = NULL;
    if (tableFloat_size > 0) {
        tableFloat = (cmsFloat32Number *)malloc(tableFloat_size);
        if (tableFloat) {
            memset(tableFloat, 0, tableFloat_size);
            for (size_t i = 0; i < tableFloat_size / sizeof(cmsFloat32Number) && i < Size; i++) {
                tableFloat[i] = (float)(Data[i % Size]) / 255.0f;
            }
        }
    }

    // Prepare Tone Curves
    cmsToneCurve *curves[5] = {NULL};
    cmsToneCurve **curve_array = NULL;
    
    // Ensure we have enough data for nChannels (max 5) starting from offset 10
    // We need at least 10 + nChannels bytes
    if (nChannels <= 4 && Size >= (10 + nChannels)) {
        curve_array = (cmsToneCurve **)malloc(nChannels * sizeof(cmsToneCurve*));
        if (curve_array) {
            for (int i = 0; i < nChannels; i++) {
                cmsFloat32Number gamma = (float)(Data[10 + i]) / 255.0f;
                if (gamma == 0.0f) gamma = 1.0f;
                cmsToneCurve *c = cmsBuildGamma(ContextID, gamma);
                if (!c) {
                    c = cmsBuildGamma(ContextID, 1.0f);
                }
                curve_array[i] = c;
                curves[i] = c;
            }
        }
    }

    cmsStage *s1 = NULL, *s2 = NULL, *s3 = NULL, *s4 = NULL, *s5 = NULL, *s6 = NULL;

    // 1. cmsStageAllocCLut16bit
    s1 = cmsStageAllocCLut16bit(ContextID, nGridPoints, inputChan, outputChan, table16);

    // 2. cmsStageAllocToneCurves
    if (curve_array) {
        s2 = cmsStageAllocToneCurves(ContextID, nChannels, curve_array);
    } else {
        s2 = cmsStageAllocToneCurves(ContextID, nChannels, NULL);
    }

    // 3. cmsStageAllocCLut16bitGranular
    // Ensure inputChan does not exceed 4 (MAX_INPUT_DIMENSIONS) to avoid internal errors
    cmsUInt32Number safe_inputChan = (inputChan > 4) ? 4 : inputChan;
    s3 = cmsStageAllocCLut16bitGranular(ContextID, clutPoints, safe_inputChan, outputChan, table16);

    // 4. cmsStageAllocCLutFloatGranular
    s4 = cmsStageAllocCLutFloatGranular(ContextID, clutPoints, safe_inputChan, outputChan, tableFloat);

    // 5. cmsStageAllocCLutFloat
    s5 = cmsStageAllocCLutFloat(ContextID, nGridPoints, safe_inputChan, outputChan, tableFloat);

    // 6. cmsStageAllocIdentity
    s6 = cmsStageAllocIdentity(ContextID, nChannels);

    // Cleanup stages
    cleanup_stages(s1, s2, s3, s4, s5, s6);

    // Cleanup curves
    if (curve_array) {
        for (int i = 0; i < nChannels; i++) {
            if (curves[i]) cmsFreeToneCurve(curves[i]);
        }
        free(curve_array);
    }

    // Cleanup buffers
    free(table16);
    free(tableFloat);

    // Cleanup context
    cmsDeleteContext(ContextID);

    return 0;
}