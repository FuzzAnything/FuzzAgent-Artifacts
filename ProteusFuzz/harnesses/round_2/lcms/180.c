// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
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
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Correct signature for _cmsStageEvalFn: void (*)(const float *, float *, const cmsStage *)
static void dummy_eval_fn(const cmsFloat32Number *In, cmsFloat32Number *Out, const cmsStage *mpe) {
    (void)In;
    (void)Out;
    (void)mpe;
}

// Correct signature for _cmsStageDupElemFn: void *(*)(cmsStage *)
static void* dummy_dup_fn(cmsStage *mpe) {
    (void)mpe;
    return NULL;
}

// Correct signature for _cmsStageFreeElemFn: void (*)(cmsStage *)
static void dummy_free_fn(cmsStage *mpe) {
    (void)mpe;
}

int LLVMFuzzerTestOneInput_180(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsStageSignature type = (cmsStageSignature)((Data[0] & 0x0F) + 1);
    cmsUInt32Number inputChannels = (Data[1] % 10) + 1;
    cmsUInt32Number outputChannels = (Data[2] % 10) + 1;
    uint8_t mode = Data[3];

    cmsStage *stage = NULL;
    cmsStage *dup_stage = NULL;
    void *data_ptr = NULL;

    // Allocate a placeholder stage
    stage = _cmsStageAllocPlaceholder(
        ContextID,
        type,
        inputChannels,
        outputChannels,
        dummy_eval_fn,
        dummy_dup_fn,
        dummy_free_fn,
        NULL
    );

    if (stage != NULL) {
        // Test cmsStageType
        cmsStageSignature sig = cmsStageType(stage);
        (void)sig;

        // Test cmsStageData
        data_ptr = cmsStageData(stage);
        (void)data_ptr;

        // Test cmsStageNext
        cmsStage *next = cmsStageNext(stage);
        (void)next;

        // Test cmsStageDup
        dup_stage = cmsStageDup(stage);
        if (dup_stage != NULL) {
            cmsStageSignature dup_sig = cmsStageType(dup_stage);
            (void)dup_sig;
            void *dup_data = cmsStageData(dup_stage);
            (void)dup_data;
            cmsStage *dup_next = cmsStageNext(dup_stage);
            (void)dup_next;

            // Note: We cannot directly modify stage->Next because the struct is opaque in the public API.
            // The linked list traversal is tested via cmsStageNext, but manual linking is not possible
            // without internal struct access. We rely on the internal logic of Dup or other functions
            // to create linked lists if they exist, or simply test the NULL case.
        }

        // Free the duplicated stage if it exists
        if (dup_stage != NULL) {
            cmsStageFree(dup_stage);
        }

        // Free the original stage
        cmsStageFree(stage);
    }

    // Cleanup context
    cmsDeleteContext(ContextID);

    return 0;
}