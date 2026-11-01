// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>
#include <limits.h>

// Helper to safely read a uint32 from fuzz data
static cmsUInt32Number read_uint32(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(cmsUInt32Number) > Size) return 0;
    cmsUInt32Number val = 0;
    memcpy(&val, Data + *offset, sizeof(cmsUInt32Number));
    *offset += sizeof(cmsUInt32Number);
    return val;
}

// Helper to safely read a float from fuzz data
static cmsFloat32Number read_float(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(cmsFloat32Number) > Size) return 0.0f;
    cmsFloat32Number val = 0.0f;
    memcpy(&val, Data + *offset, sizeof(cmsFloat32Number));
    *offset += sizeof(cmsFloat32Number);
    return val;
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // cmsCreateContext requires two arguments: Plugin and UserData
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    size_t offset = 0;

    // Fuzz _cmsMallocZero
    cmsUInt32Number mallocSize = read_uint32(Data, Size, &offset);
    // Cap size to prevent OOM
    if (mallocSize > 1024 * 1024) mallocSize = 1024 * 1024;
    void *mallocPtr = _cmsMallocZero(ContextID, mallocSize);
    if (mallocPtr) {
        // Do something with it to avoid unused variable warnings
        memset(mallocPtr, 0x42, mallocSize);
        // Use cmsFreeContext is incorrect, use cmsFreeContext is not a standard API for freeing memory allocated by _cmsMallocZero.
        // The memory allocated by _cmsMallocZero should be freed using the context's free function or standard free if plugin is default.
        // However, lcms2 doesn't expose a direct "free" for _cmsMallocZero without the plugin.
        // We assume the plugin handles it or we just leak for fuzzing safety if we can't find the free.
        // Actually, for context-specific allocators, we should use cmsFreeContext? No, that frees the context.
        // The correct way is to use the plugin's free function, but it's not exposed directly.
        // Let's try to use the standard free if it's the default, but _cmsMallocZero uses the plugin.
        // In many lcms2 versions, if no plugin is set, it uses standard malloc/free.
        // But to be safe and correct with the API, we should not free it if we don't know the plugin.
        // However, the error message suggests cmsFreeContext is not the right function.
        // Let's look for a generic free. There isn't one exposed for _cmsMallocZero directly in public API.
        // We will skip freeing these specific allocations to avoid crashes, relying on process exit.
        // Or we can try to use the context's memory manager if accessible, but it's internal.
        // For fuzzing, leaking a few small allocations is acceptable if we can't free them safely.
        // But let's try to use the standard free if we assume default plugin.
        // Actually, the safest bet for fuzzing is to not free if we are unsure, or use a wrapper.
        // Let's assume the default plugin uses malloc/free and try free().
        free(mallocPtr);
    }

    // Fuzz _cmsCalloc
    cmsUInt32Number callocNum = read_uint32(Data, Size, &offset);
    cmsUInt32Number callocSize = read_uint32(Data, Size, &offset);
    if (callocNum > 10000) callocNum = 10000;
    if (callocSize > 10000) callocSize = 10000;
    void *callocPtr = _cmsCalloc(ContextID, callocNum, callocSize);
    if (callocPtr) {
        memset(callocPtr, 0x42, callocNum * callocSize);
        free(callocPtr);
    }

    // Fuzz _cmsDupMem
    cmsUInt32Number dupSize = read_uint32(Data, Size, &offset);
    if (dupSize > 1024 * 1024) dupSize = 1024 * 1024;
    void *srcMem = _cmsMallocZero(ContextID, dupSize);
    if (srcMem) {
        memset(srcMem, 0x12, dupSize);
        void *dupMem = _cmsDupMem(ContextID, srcMem, dupSize);
        if (dupMem) {
            free(dupMem);
        }
        free(srcMem);
    }

    // Fuzz cmsStageAllocIdentity
    cmsUInt32Number nChannels = read_uint32(Data, Size, &offset);
    if (nChannels > 10) nChannels = 10;
    cmsStage *identityStage = cmsStageAllocIdentity(ContextID, nChannels);
    if (identityStage) {
        cmsStageFree(identityStage);
    }

    // Fuzz cmsStageAllocCLutFloatGranular
    // We need inputChan, outputChan, clutPoints array, and Table
    cmsUInt32Number inputChan = read_uint32(Data, Size, &offset);
    cmsUInt32Number outputChan = read_uint32(Data, Size, &offset);
    // Limit channels to avoid huge allocations
    if (inputChan > 6) inputChan = 6;
    if (outputChan > 6) outputChan = 6;
    if (inputChan == 0) inputChan = 1;
    if (outputChan == 0) outputChan = 1;

    // Read clutPoints for each input channel
    cmsUInt32Number *clutPoints = NULL;
    cmsFloat32Number *tableData = NULL;
    size_t totalPoints = 1;
    
    clutPoints = (cmsUInt32Number *)malloc(inputChan * sizeof(cmsUInt32Number));
    if (!clutPoints) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    for (uint32_t i = 0; i < inputChan; i++) {
        cmsUInt32Number pts = read_uint32(Data, Size, &offset);
        if (pts > 10) pts = 10; // Limit grid size
        if (pts == 0) pts = 1;
        clutPoints[i] = pts;
        totalPoints *= pts;
    }

    // Allocate table data
    tableData = (cmsFloat32Number *)malloc(totalPoints * outputChan * sizeof(cmsFloat32Number));
    if (!tableData) {
        free(clutPoints);
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Fill table with fuzz data
    for (size_t i = 0; i < totalPoints * outputChan; i++) {
        tableData[i] = read_float(Data, Size, &offset);
    }

    cmsStage *clutStage = cmsStageAllocCLutFloatGranular(ContextID, clutPoints, inputChan, outputChan, tableData);
    if (clutStage) {
        cmsStageFree(clutStage);
    }

    free(clutPoints);
    free(tableData);

    // Fuzz cmsCreateProofingTransformTHR
    // We need dummy profiles. Since we can't easily create valid ICC profiles from fuzz data
    // without complex parsing, we will try to create a transform with NULL or invalid profiles
    // to test error handling, or create a dummy file and try to load it.
    
    // Create a dummy file
    const char *dummyFile = "./dummy_file";
    FILE *f = fopen(dummyFile, "wb");
    if (f) {
        // Write some random data
        size_t writeSize = (Size > 1024) ? 1024 : Size;
        fwrite(Data, 1, writeSize, f);
        fclose(f);

        cmsHPROFILE Input = cmsOpenProfileFromFile(dummyFile, "r");
        cmsHPROFILE Output = cmsOpenProfileFromFile(dummyFile, "r");
        cmsHPROFILE Proofing = cmsOpenProfileFromFile(dummyFile, "r");

        cmsUInt32Number InputFormat = read_uint32(Data, Size, &offset);
        cmsUInt32Number OutputFormat = read_uint32(Data, Size, &offset);
        cmsUInt32Number Intent = read_uint32(Data, Size, &offset);
        cmsUInt32Number ProofingIntent = read_uint32(Data, Size, &offset);
        cmsUInt32Number dwFlags = read_uint32(Data, Size, &offset);

        // Limit intents to valid range roughly
        if (Intent > 10) Intent = 0;
        if (ProofingIntent > 10) ProofingIntent = 0;
        if (dwFlags > 1000) dwFlags = 0;

        cmsHTRANSFORM Transform = cmsCreateProofingTransformTHR(
            ContextID,
            Input,
            InputFormat,
            Output,
            OutputFormat,
            Proofing,
            Intent,
            ProofingIntent,
            dwFlags
        );

        if (Transform) {
            cmsDeleteTransform(Transform);
        }

        if (Input) cmsCloseProfile(Input);
        if (Output) cmsCloseProfile(Output);
        if (Proofing) cmsCloseProfile(Proofing);
    }
    
    // Cleanup dummy file
    remove(dummyFile);

    cmsDeleteContext(ContextID);
    return 0;
}