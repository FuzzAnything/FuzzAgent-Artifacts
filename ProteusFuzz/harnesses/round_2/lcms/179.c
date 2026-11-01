// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Dummy callback functions matching the exact signatures required by lcms2_plugin.h
// _cmsStageEvalFn: void (*)(const float *, float *, const cmsStage *)
static void DummyEval(const float *In, float *Out, const cmsStage *Stage) {
    (void)In;
    (void)Out;
    (void)Stage;
    return;
}

// _cmsStageDupElemFn: void *(*)(cmsStage *)
static void* DummyDup(cmsStage *Stage) {
    (void)Stage;
    return NULL;
}

// _cmsStageFreeElemFn: void (*)(cmsStage *)
static void DummyFree(cmsStage *Stage) {
    (void)Stage;
    return;
}

int LLVMFuzzerTestOneInput_179(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a context to ensure valid ContextID for allocation
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsStage* stage = NULL;

    // Strategy 1: Create a stage using _cmsStageAllocPlaceholder
    if (Size >= 4) {
        cmsStageSignature type = Data[0];
        cmsUInt32Number in_ch = Data[1];
        cmsUInt32Number out_ch = Data[2];
        size_t data_size = (Data[3] % 256) + 1;
        void* data = NULL;
        
        if (data_size > 0) {
            data = malloc(data_size);
            if (data) {
                if (Size > 4) {
                    size_t copy_len = Size - 4;
                    if (copy_len > data_size) copy_len = data_size;
                    memcpy(data, Data + 4, copy_len);
                }
            }
        }

        stage = _cmsStageAllocPlaceholder(
            ctx,
            type,
            in_ch,
            out_ch,
            DummyEval,
            DummyDup,
            DummyFree,
            data
        );
    }

    if (stage) {
        // Test cmsStageType
        cmsStageSignature t = cmsStageType(stage);
        (void)t;

        // Test cmsStageData
        void* d = cmsStageData(stage);
        (void)d;

        // Test cmsStageNext
        cmsStage* next = cmsStageNext(stage);
        (void)next;

        // Test cmsStageDup
        cmsStage* dup = cmsStageDup(stage);
        if (dup) {
            cmsStageType(dup);
            cmsStageData(dup);
            cmsStageNext(dup);
            cmsStageFree(dup);
        }

        // Test cmsStageFree
        cmsStageFree(stage);
    }

    // Strategy 2: Test with NULL pointers (expecting potential crashes or undefined behavior)
    // cmsStageDup(NULL) is safe and returns NULL per description.
    cmsStage* null_dup = cmsStageDup(NULL);
    (void)null_dup;

    // Clean up context
    cmsDeleteContext(ctx);

    return 0;
}