// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
// cmsSliceSpaceFloat at cmslut.c:910:26 in lcms2.h
// cmsSliceSpace16 at cmslut.c:879:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

// Callback for 16-bit sampler
// Note: The first argument must be const cmsUInt16Number* to match cmsSAMPLER16
static cmsBool Sampler16(const cmsUInt16Number* In, cmsUInt16Number* Out, void* Cargo) {
    // Simple pass-through or modification based on Cargo to explore states
    if (In && Out) {
        // Use Cargo to potentially alter behavior or check bounds
        size_t count = (size_t)(uintptr_t)Cargo;
        if (count > 0) {
            Out[0] = In[0] ^ 0xAAAA;
        } else {
            Out[0] = In[0];
        }
        return TRUE;
    }
    return FALSE;
}

// Callback for Float sampler
// Note: The first argument must be const cmsFloat32Number* to match cmsSAMPLERFLOAT
static cmsBool SamplerFloat(const cmsFloat32Number* In, cmsFloat32Number* Out, void* Cargo) {
    if (In && Out) {
        // Simple transformation
        Out[0] = In[0] * 0.5f;
        return TRUE;
    }
    return FALSE;
}

static cmsContext CreateContextFromData(const uint8_t *Data, size_t Size) {
    // Create a new context. If Size is 0, just use default.
    // We don't strictly need to use the data for context creation, 
    // but we can use it to seed a custom allocator if needed. 
    // For simplicity and stability, we create a standard context.
    return cmsCreateContext(NULL, NULL);
}

static void CleanupContext(cmsContext Context) {
    if (Context) {
        cmsDeleteContext(Context);
    }
}

static void CleanupStage(cmsStage* Stage) {
    if (Stage) {
        cmsStageFree(Stage);
    }
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext Context = CreateContextFromData(Data, Size);
    if (!Context) return 0;

    // Extract parameters from fuzz data to drive diversity
    cmsUInt32Number nGridPoints = 2; // Default small grid
    cmsUInt32Number inputChan = 3;   // Default RGB
    cmsUInt32Number outputChan = 3;  // Default RGB
    cmsUInt32Number clutPoints[4] = {2, 2, 2, 2}; // Default 2x2x2x2

    if (Size >= 1) nGridPoints = Data[0] % 20 + 2; // Limit grid size to avoid OOM
    if (Size >= 2) inputChan = Data[1] % 8 + 1;
    if (Size >= 3) outputChan = Data[2] % 8 + 1;
    
    // Ensure grid points match input channels for granular alloc
    for (int i = 0; i < 4; i++) {
        if (Size >= 4 + i) {
            clutPoints[i] = Data[4 + i] % 20 + 2;
        }
    }

    // Limit channels to valid range for lcms
    if (inputChan > 4) inputChan = 4;
    if (outputChan > 4) outputChan = 4;
    if (nGridPoints > 10) nGridPoints = 10;

    cmsStage *stage16 = NULL;
    cmsStage *stageFloat = NULL;
    cmsBool result = FALSE;

    // Test 1: cmsStageAllocCLut16bit
    // Calculate table size
    size_t tableSize16 = 1;
    for (int i = 0; i < inputChan; i++) tableSize16 *= nGridPoints;
    tableSize16 *= outputChan;
    
    cmsUInt16Number *table16 = NULL;
    if (tableSize16 < 100000) { // Safety cap
        table16 = (cmsUInt16Number *)malloc(tableSize16 * sizeof(cmsUInt16Number));
        if (table16) {
            memset(table16, 0, tableSize16 * sizeof(cmsUInt16Number));
            // Fill with some data from fuzz input if available
            size_t fillLen = (Size < tableSize16 * 2) ? Size : tableSize16 * 2;
            if (fillLen > 0) {
                memcpy(table16, Data, fillLen);
            }
            stage16 = cmsStageAllocCLut16bit(Context, nGridPoints, inputChan, outputChan, table16);
        }
    }
    free(table16);

    // Test 2: cmsStageAllocCLutFloatGranular
    size_t tableSizeFloat = 1;
    for (int i = 0; i < inputChan; i++) tableSizeFloat *= clutPoints[i];
    tableSizeFloat *= outputChan;

    cmsFloat32Number *tableFloat = NULL;
    if (tableSizeFloat < 100000) {
        tableFloat = (cmsFloat32Number *)malloc(tableSizeFloat * sizeof(cmsFloat32Number));
        if (tableFloat) {
            memset(tableFloat, 0, tableSizeFloat * sizeof(cmsFloat32Number));
            // Fill with some data
            size_t fillLen = (Size < tableSizeFloat * 4) ? Size : tableSizeFloat * 4;
            if (fillLen > 0) {
                memcpy(tableFloat, Data, fillLen);
            }
            stageFloat = cmsStageAllocCLutFloatGranular(Context, clutPoints, inputChan, outputChan, tableFloat);
        }
    }
    free(tableFloat);

    // Test 3: cmsStageSampleCLut16bit
    if (stage16) {
        // Pass a pointer to an integer as cargo to vary behavior
        result = cmsStageSampleCLut16bit(stage16, Sampler16, (void*)(uintptr_t)(Size % 10), 0);
        (void)result; // Suppress unused warning
    }

    // Test 4: cmsStageSampleCLutFloat
    if (stageFloat) {
        result = cmsStageSampleCLutFloat(stageFloat, SamplerFloat, (void*)(uintptr_t)(Size % 10), 0);
        (void)result;
    }

    // Test 5: cmsSliceSpaceFloat
    // Use inputChan and clutPoints
    if (inputChan <= 4) {
        result = cmsSliceSpaceFloat(inputChan, clutPoints, SamplerFloat, (void*)(uintptr_t)(Size % 10));
        (void)result;
    }

    // Test 6: cmsSliceSpace16
    // Use inputChan and nGridPoints (uniform)
    cmsUInt32Number uniformGrid[4] = {nGridPoints, nGridPoints, nGridPoints, nGridPoints};
    if (inputChan <= 4) {
        result = cmsSliceSpace16(inputChan, uniformGrid, Sampler16, (void*)(uintptr_t)(Size % 10));
        (void)result;
    }

    // Cleanup
    CleanupStage(stage16);
    CleanupStage(stageFloat);
    CleanupContext(Context);

    return 0;
}