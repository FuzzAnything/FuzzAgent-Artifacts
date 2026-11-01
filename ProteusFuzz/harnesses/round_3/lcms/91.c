// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Static helper to safely create a context
static cmsContext create_dummy_context(void) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    return ctx;
}

// Static helper to safely free a context
static void free_dummy_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

// Dummy evaluation function matching _cmsStageEvalFn signature:
// void (*)(const cmsFloat32Number* In, cmsFloat32Number* Out, const cmsStage* mpe)
static void dummy_eval(const cmsFloat32Number* In, cmsFloat32Number* Out, const cmsStage* mpe) {
    // No-op implementation
    (void)In;
    (void)Out;
    (void)mpe;
}

// Dummy duplication function matching _cmsStageDupElemFn signature:
// void*(*)(cmsStage* mpe) - Note: The parameter is non-const in the actual type definition
static void* dummy_dup(cmsStage* mpe) {
    // Return NULL to indicate no specific data duplication needed
    (void)mpe;
    return NULL;
}

// Dummy free function matching _cmsStageFreeElemFn signature:
// void(*)(cmsStage* mpe) - Note: The parameter is cmsStage*, not void*
static void dummy_free(cmsStage* mpe) {
    // No-op implementation
    (void)mpe;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = create_dummy_context();
    if (!ctx) {
        return 0;
    }

    // Extract parameters from fuzz input
    cmsStageSignature type = (cmsStageSignature)(Data[0] & 0xFF);
    cmsUInt32Number input_channels = (Size > 1) ? (Data[1] & 0xFF) : 1;
    cmsUInt32Number output_channels = (Size > 2) ? (Data[2] & 0xFF) : 1;
    void* data_ptr = NULL;
    
    if (Size > 3) {
        // Allocate some data if requested by input
        size_t data_size = (Size > 4) ? (Data[3] & 0xFF) + 1 : 1;
        if (data_size > 1024) data_size = 1024; // Limit allocation
        data_ptr = malloc(data_size);
        if (data_ptr && Size > 4) {
            memcpy(data_ptr, Data + 4, data_size < (Size - 4) ? data_size : Size - 4);
        }
    }

    // Allocate a stage placeholder
    cmsStage* stage = _cmsStageAllocPlaceholder(
        ctx,
        type,
        input_channels,
        output_channels,
        dummy_eval,
        dummy_dup,
        dummy_free,
        data_ptr
    );

    if (stage) {
        // Test cmsStageType
        cmsStageSignature sig = cmsStageType(stage);
        (void)sig; // Suppress unused warning

        // Test cmsStageData
        void* stage_data = cmsStageData(stage);
        (void)stage_data;

        // Test cmsStageDup
        cmsStage* dup_stage = cmsStageDup(stage);
        if (dup_stage) {
            cmsStageSignature dup_sig = cmsStageType(dup_stage);
            (void)dup_sig;
            void* dup_data = cmsStageData(dup_stage);
            (void)dup_data;
            cmsStage* next_dup = cmsStageNext(dup_stage);
            (void)next_dup;
            cmsStageFree(dup_stage);
        }

        // Test cmsStageNext (on original, likely NULL unless linked)
        cmsStage* next_stage = cmsStageNext(stage);
        (void)next_stage;

        // Free the original stage
        cmsStageFree(stage);
    }

    // Cleanup allocated data if we allocated it
    if (data_ptr) {
        free(data_ptr);
    }

    free_dummy_context(ctx);

    return 0;
}