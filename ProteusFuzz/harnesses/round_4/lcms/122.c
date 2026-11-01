// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
// cmsSliceSpace16 at cmslut.c:879:19 in lcms2.h
// cmsSliceSpaceFloat at cmslut.c:910:26 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

// Sampler callback for 16-bit CLUT
// Signature must match cmsSAMPLER16: int (*)(const cmsUInt16Number*, cmsUInt16Number*, void*)
static cmsBool Sample16(const cmsUInt16Number* In, cmsUInt16Number* Out, void* Cargo) {
    if (In == NULL || Out == NULL) return 0;
    size_t n = *(size_t*)Cargo;
    for (size_t i = 0; i < n; i++) {
        Out[i] = In[i];
    }
    return 1;
}

// Sampler callback for float CLUT
// Signature must match cmsSAMPLERFLOAT: int (*)(const cmsFloat32Number*, cmsFloat32Number*, void*)
static cmsBool SampleFloat(const cmsFloat32Number* In, cmsFloat32Number* Out, void* Cargo) {
    if (In == NULL || Out == NULL) return 0;
    size_t n = *(size_t*)Cargo;
    for (size_t i = 0; i < n; i++) {
        Out[i] = In[i];
    }
    return 1;
}

// Helper to safely allocate a small table for CLUT
static void* alloc_table(size_t size, size_t element_size) {
    if (size == 0) return NULL;
    void* buf = malloc(size * element_size);
    if (!buf) return NULL;
    memset(buf, 0, size * element_size);
    return buf;
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Extract parameters from input data
    uint32_t grid_points = Data[0] + 1; // 1 to 256
    uint32_t input_chan = Data[1] % 10 + 1; // 1 to 10
    uint32_t output_chan = Data[2] % 10 + 1; // 1 to 10
    uint32_t flags = Data[3];

    // Limit grid points and channels to reasonable bounds to prevent OOM
    if (grid_points > 64) grid_points = 64;
    if (input_chan > 6) input_chan = 6;
    if (output_chan > 6) output_chan = 6;

    // Calculate table size
    size_t table_size_16 = 1;
    for (uint32_t i = 0; i < input_chan; i++) table_size_16 *= grid_points;
    table_size_16 *= output_chan;

    size_t table_size_float = table_size_16;

    cmsUInt16Number* table16 = NULL;
    cmsFloat32Number* tableFloat = NULL;

    // Allocate tables if size is reasonable
    if (table_size_16 < 100000) {
        table16 = (cmsUInt16Number*)alloc_table(table_size_16, sizeof(cmsUInt16Number));
        tableFloat = (cmsFloat32Number*)alloc_table(table_size_float, sizeof(cmsFloat32Number));
    }

    // Prepare clutPoints array for granular functions
    cmsUInt32Number clutPoints[10] = {0};
    for (uint32_t i = 0; i < input_chan && i < 10; i++) {
        clutPoints[i] = grid_points;
    }

    cmsStage* stage16 = NULL;
    cmsStage* stageFloat = NULL;

    // Test cmsStageAllocCLut16bit
    if (table16) {
        stage16 = cmsStageAllocCLut16bit(ctx, grid_points, input_chan, output_chan, table16);
    }

    // Test cmsStageAllocCLutFloatGranular
    if (tableFloat) {
        stageFloat = cmsStageAllocCLutFloatGranular(ctx, clutPoints, input_chan, output_chan, tableFloat);
    }

    // Test cmsStageSampleCLut16bit
    if (stage16) {
        size_t cargo_val = output_chan;
        cmsBool res16 = cmsStageSampleCLut16bit(stage16, Sample16, &cargo_val, flags);
        (void)res16;
    }

    // Test cmsStageSampleCLutFloat
    if (stageFloat) {
        size_t cargo_val = output_chan;
        cmsBool resFloat = cmsStageSampleCLutFloat(stageFloat, SampleFloat, &cargo_val, flags);
        (void)resFloat;
    }

    // Test cmsSliceSpace16
    if (input_chan <= 6) {
        size_t cargo_val = output_chan;
        cmsBool resSlice16 = cmsSliceSpace16(input_chan, clutPoints, Sample16, &cargo_val);
        (void)resSlice16;
    }

    // Test cmsSliceSpaceFloat
    if (input_chan <= 6) {
        size_t cargo_val = output_chan;
        cmsBool resSliceFloat = cmsSliceSpaceFloat(input_chan, clutPoints, SampleFloat, &cargo_val);
        (void)resSliceFloat;
    }

    // Cleanup
    if (stage16) cmsStageFree(stage16);
    if (stageFloat) cmsStageFree(stageFloat);
    if (table16) free(table16);
    if (tableFloat) free(tableFloat);
    cmsDeleteContext(ctx);

    return 0;
}