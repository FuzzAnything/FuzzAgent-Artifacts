// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Dummy functions for placeholder stage with correct signatures
static void dummy_eval(const float* in, float* out, const cmsStage* mpe) {
    // Do nothing
    (void)in;
    (void)out;
    (void)mpe;
}

static void* dummy_dup(cmsStage* mpe) {
    (void)mpe;
    return NULL;
}

static void dummy_free(cmsStage* mpe) {
    // Do nothing
    (void)mpe;
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // For cmsStageAllocCLut16bit
    cmsUInt32Number nGridPoints = 0;
    cmsUInt32Number inputChan = 0;
    cmsUInt32Number outputChan = 0;
    
    if (offset + 12 <= Size) {
        nGridPoints = *(cmsUInt32Number*)(Data + offset);
        offset += 4;
        inputChan = *(cmsUInt32Number*)(Data + offset);
        offset += 4;
        outputChan = *(cmsUInt32Number*)(Data + offset);
        offset += 4;
    }
    
    // Limit values to reasonable ranges to avoid excessive memory allocation
    if (nGridPoints > 256) nGridPoints = 256;
    if (inputChan > 16) inputChan = 16;
    if (outputChan > 16) outputChan = 16;
    if (inputChan < 1) inputChan = 1;
    if (outputChan < 1) outputChan = 1;
    if (nGridPoints < 2) nGridPoints = 2;

    // For cmsStageAllocIdentity
    cmsUInt32Number nChannels = 0;
    if (offset + 4 <= Size) {
        nChannels = *(cmsUInt32Number*)(Data + offset);
        offset += 4;
    }
    if (nChannels > 16) nChannels = 16;
    if (nChannels < 1) nChannels = 1;

    // For cmsStageAllocCLut16bitGranular
    cmsUInt32Number clutPoints[3] = {2, 2, 2};
    if (offset + 12 <= Size) {
        for (int i = 0; i < 3; i++) {
            clutPoints[i] = *(cmsUInt32Number*)(Data + offset);
            offset += 4;
            if (clutPoints[i] > 256) clutPoints[i] = 256;
            if (clutPoints[i] < 2) clutPoints[i] = 2;
        }
    }

    // Calculate table size for CLUT stages
    size_t tableSize = 1;
    for (cmsUInt32Number i = 0; i < inputChan; i++) {
        tableSize *= nGridPoints;
    }
    tableSize *= outputChan;
    
    // Allocate and initialize table
    cmsUInt16Number* table = NULL;
    if (tableSize > 0 && tableSize < 1000000) { // Reasonable limit
        table = (cmsUInt16Number*)calloc(tableSize, sizeof(cmsUInt16Number));
        if (table) {
            // Fill table with fuzzer data
            size_t bytesToCopy = Size - offset;
            if (bytesToCopy > tableSize * sizeof(cmsUInt16Number)) {
                bytesToCopy = tableSize * sizeof(cmsUInt16Number);
            }
            if (bytesToCopy > 0) {
                memcpy(table, Data + offset, bytesToCopy);
            }
        }
    }

    // Test cmsStageAllocCLut16bit
    cmsStage* clutStage = NULL;
    if (table) {
        clutStage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table);
    }

    // Test cmsStageInputChannels and cmsStageOutputChannels
    if (clutStage) {
        cmsUInt32Number inChannels = cmsStageInputChannels(clutStage);
        cmsUInt32Number outChannels = cmsStageOutputChannels(clutStage);
        (void)inChannels; // Use variables to avoid unused warnings
        (void)outChannels;
        
        // Free the stage
        cmsStageFree(clutStage);
    }

    // Test cmsStageAllocIdentity
    cmsStage* identityStage = cmsStageAllocIdentity(ctx, nChannels);
    if (identityStage) {
        cmsUInt32Number inChannels = cmsStageInputChannels(identityStage);
        cmsUInt32Number outChannels = cmsStageOutputChannels(identityStage);
        (void)inChannels;
        (void)outChannels;
        
        cmsStageFree(identityStage);
    }

    // Test cmsStageAllocCLut16bitGranular
    cmsStage* granularStage = NULL;
    if (table) {
        granularStage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, 3, outputChan, table);
    }
    
    if (granularStage) {
        cmsUInt32Number inChannels = cmsStageInputChannels(granularStage);
        cmsUInt32Number outChannels = cmsStageOutputChannels(granularStage);
        (void)inChannels;
        (void)outChannels;
        
        cmsStageFree(granularStage);
    }

    // Test _cmsStageAllocPlaceholder
    cmsStage* placeholderStage = _cmsStageAllocPlaceholder(
        ctx,
        (cmsStageSignature)0x4D504520, // "MPE" in hex
        inputChan,
        outputChan,
        dummy_eval,
        dummy_dup,
        dummy_free,
        NULL
    );
    
    if (placeholderStage) {
        cmsUInt32Number inChannels = cmsStageInputChannels(placeholderStage);
        cmsUInt32Number outChannels = cmsStageOutputChannels(placeholderStage);
        (void)inChannels;
        (void)outChannels;
        
        cmsStageFree(placeholderStage);
    }

    // Cleanup
    if (table) {
        free(table);
    }
    cmsDeleteContext(ctx);

    return 0;
}