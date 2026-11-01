// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

// Forward declarations for internal function pointer types used in cmsStage
// These are typically defined internally in lcms2 but not exposed in the public header.
typedef void (*_cmsStageEvalFn)(void *Data, cmsUInt32Number n, cmsFloat32Number *in, cmsFloat32Number *out);
typedef void* (*_cmsStageDupElemFn)(void *Data);
typedef void (*_cmsStageFreeElemFn)(void *Data);

// Define the internal structure manually since it's not fully exposed in public headers
// This matches the provided data type definition
struct _cmsStage_struct {
    cmsContext ContextID;
    cmsStageSignature Type;
    cmsStageSignature Implements;
    cmsUInt32Number InputChannels;
    cmsUInt32Number OutputChannels;
    _cmsStageEvalFn EvalPtr;
    _cmsStageDupElemFn DupElemPtr;
    _cmsStageFreeElemFn FreePtr;
    void* Data;
    struct _cmsStage_struct* Next;
};

typedef struct _cmsStage_struct cmsStage;

static cmsContext create_dummy_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = create_dummy_context();
    if (ctx == NULL) return 0;

    // We need at least sizeof(cmsStage) bytes to simulate a stage structure.
    if (Size >= sizeof(cmsStage)) {
        // Allocate memory to hold the stage structure
        cmsStage* stage = (cmsStage*)malloc(sizeof(cmsStage));
        if (stage == NULL) {
            cleanup_context(ctx);
            return 0;
        }

        // Copy input data into the allocated stage structure
        memcpy(stage, Data, sizeof(cmsStage));

        // Set ContextID to our valid context to allow _cmsFree to work if needed
        stage->ContextID = ctx;

        // Test cmsStageType
        cmsStageType(stage);

        // Test cmsStageData
        cmsStageData(stage);

        // Test cmsStageDup
        cmsStage* dup = cmsStageDup(stage);
        if (dup != NULL) {
            // Free the duplicated stage
            cmsStageFree(dup);
        }

        // Test cmsStageNext
        cmsStage* next = cmsStageNext(stage);
        // We do not free 'next' as it might be part of a chain we don't own.
        // In this crafted scenario, 'next' is just a pointer from input data.

        // Test cmsStageFree on the original stage
        // This will attempt to free the memory we allocated with malloc.
        // If the stage's FreePtr is set, it might be called first.
        cmsStageFree(stage);
    }

    // Test cmsStageDup with NULL as it is documented to return NULL safely.
    cmsStage* null_dup = cmsStageDup(NULL);
    if (null_dup != NULL) {
        cmsStageFree(null_dup);
    }

    cleanup_context(ctx);
    return 0;
}