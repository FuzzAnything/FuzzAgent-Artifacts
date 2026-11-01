// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsSliceSpace16 at cmslut.c:879:19 in lcms2.h
// cmsSliceSpaceFloat at cmslut.c:910:26 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static cmsBool SamplerFloat(const cmsFloat32Number* in, cmsFloat32Number* out, void* Cargo) {
    if (!in || !out || !Cargo) return FALSE;
    size_t input_size = *(size_t*)Cargo;
    size_t output_size = *(size_t*)((char*)Cargo + sizeof(size_t));
    if (input_size == 0 || output_size == 0) return FALSE;
    if (input_size > output_size) return FALSE;
    for (size_t i = 0; i < input_size; i++) {
        out[i] = in[i];
    }
    return TRUE;
}

static cmsBool Sampler16(const cmsUInt16Number* in, cmsUInt16Number* out, void* Cargo) {
    if (!in || !out || !Cargo) return FALSE;
    size_t input_size = *(size_t*)Cargo;
    size_t output_size = *(size_t*)((char*)Cargo + sizeof(size_t));
    if (input_size == 0 || output_size == 0) return FALSE;
    if (input_size > output_size) return FALSE;
    for (size_t i = 0; i < input_size; i++) {
        out[i] = in[i];
    }
    return TRUE;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsUInt32Number nInputs = Data[0] % 10 + 1; // 1 to 10
    cmsUInt32Number nGridPoints = (Data[1] % 10) + 2; // 2 to 11
    cmsUInt32Number inputChan = (Data[2] % 5) + 1; // 1 to 5
    cmsUInt32Number outputChan = (Data[3] % 5) + 1; // 1 to 5

    // Prepare clutPoints array safely
    cmsUInt32Number clutPoints[10] = {0};
    for (int i = 0; i < 10; i++) {
        if (4 + i < Size) {
            clutPoints[i] = (Data[4 + i] % 10) + 2;
        } else {
            clutPoints[i] = 2; // Default value if data runs out
        }
    }

    // Allocate table data for 16-bit CLUT
    size_t tableSize16 = nGridPoints;
    for (int i = 0; i < nInputs; i++) {
        // Prevent overflow
        if (tableSize16 > SIZE_MAX / nGridPoints) {
            cmsDeleteContext(ctx);
            return 0;
        }
        tableSize16 *= nGridPoints;
    }
    if (tableSize16 > SIZE_MAX / outputChan) {
        cmsDeleteContext(ctx);
        return 0;
    }
    tableSize16 *= outputChan;
    
    cmsUInt16Number* table16 = (cmsUInt16Number*)malloc(tableSize16 * sizeof(cmsUInt16Number));
    if (!table16) {
        cmsDeleteContext(ctx);
        return 0;
    }
    for (size_t i = 0; i < tableSize16; i++) {
        table16[i] = (Data[(4 + i) % Size] % 65536);
    }

    // Allocate table data for float CLUT
    size_t tableSizeFloat = nGridPoints;
    for (int i = 0; i < nInputs; i++) {
        if (tableSizeFloat > SIZE_MAX / nGridPoints) {
            free(table16);
            cmsDeleteContext(ctx);
            return 0;
        }
        tableSizeFloat *= nGridPoints;
    }
    if (tableSizeFloat > SIZE_MAX / outputChan) {
        free(table16);
        cmsDeleteContext(ctx);
        return 0;
    }
    tableSizeFloat *= outputChan;
    
    cmsFloat32Number* tableFloat = (cmsFloat32Number*)malloc(tableSizeFloat * sizeof(cmsFloat32Number));
    if (!tableFloat) {
        free(table16);
        cmsDeleteContext(ctx);
        return 0;
    }
    for (size_t i = 0; i < tableSizeFloat; i++) {
        tableFloat[i] = (Data[(4 + i) % Size] / 255.0f);
    }

    // Prepare cargo for samplers
    size_t cargoData[2] = {inputChan, outputChan};
    void* cargo = cargoData;

    // Test cmsStageAllocCLut16bit
    cmsStage* stage16 = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table16);
    if (stage16) {
        cmsBool res1 = cmsStageSampleCLut16bit(stage16, Sampler16, cargo, 0);
        (void)res1;
        cmsStageFree(stage16);
    }

    // Test cmsStageAllocCLutFloatGranular
    cmsStage* stageFloat = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, tableFloat);
    if (stageFloat) {
        cmsBool res2 = cmsStageSampleCLutFloat(stageFloat, SamplerFloat, cargo, 0);
        (void)res2;
        cmsStageFree(stageFloat);
    }

    // Test cmsSliceSpace16
    cmsBool res3 = cmsSliceSpace16(nInputs, clutPoints, Sampler16, cargo);
    (void)res3;

    // Test cmsSliceSpaceFloat
    cmsBool res4 = cmsSliceSpaceFloat(nInputs, clutPoints, SamplerFloat, cargo);
    (void)res4;

    // Cleanup
    free(table16);
    free(tableFloat);
    cmsDeleteContext(ctx);

    return 0;
}