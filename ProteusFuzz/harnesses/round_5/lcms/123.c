// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext g_context = NULL;

// Dummy evaluation function matching _cmsStageEvalFn signature
static void dummy_eval_fn(const float *In, float *Out, const cmsStage *mpe) {
    if (In && Out && mpe) {
        // Safe dummy operation: copy first float if available
        *Out = *In; 
    }
}

// Dummy duplication function matching _cmsStageDupElemFn signature
static void* dummy_dup_fn(cmsStage *mpe) {
    if (mpe == NULL) return NULL;
    void* data = cmsStageData(mpe);
    if (data == NULL) return NULL;
    // Return NULL to test allocation failure path in cmsStageDup safely
    return NULL; 
}

// Dummy free function matching _cmsStageFreeElemFn signature
static void dummy_free_fn(cmsStage *mpe) {
    if (mpe == NULL) return;
    void* data = cmsStageData(mpe);
    if (data != NULL) {
        free(data);
    }
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize context if not already done
    if (g_context == NULL) {
        g_context = cmsCreateContext(NULL, NULL);
        if (g_context == NULL) return 0;
    }

    // Extract parameters from fuzz input with bounds checking
    cmsStageSignature type = (cmsStageSignature)(Data[0] % 5); 
    cmsUInt32Number input_channels = 1;
    cmsUInt32Number output_channels = 1;
    void* data_ptr = NULL;

    if (Size > 1) {
        input_channels = (Data[1] % 10) + 1;
    }
    if (Size > 2) {
        output_channels = (Data[2] % 10) + 1;
    }

    // Allocate some data if needed
    if (Size > 3 && Data[3] % 2 == 0) {
        size_t data_size = 1;
        if (Size > 4) {
            data_size = (Data[4] % 100) + 1;
        }
        data_ptr = malloc(data_size);
        if (data_ptr) {
            uint8_t fill_val = 0;
            if (Size > 5) fill_val = Data[5] % 256;
            memset(data_ptr, fill_val, data_size);
        }
    }

    // Allocate a placeholder stage
    cmsStage* stage = _cmsStageAllocPlaceholder(
        g_context,
        type,
        input_channels,
        output_channels,
        dummy_eval_fn,
        dummy_dup_fn,
        dummy_free_fn,
        data_ptr
    );

    if (stage == NULL) {
        if (data_ptr) free(data_ptr);
        return 0;
    }

    // Test cmsStageType
    cmsStageSignature retrieved_type = cmsStageType(stage);
    (void)retrieved_type; 

    // Test cmsStageData
    void* retrieved_data = cmsStageData(stage);
    (void)retrieved_data; 

    // Test cmsStageDup
    cmsStage* dup_stage = cmsStageDup(stage);
    if (dup_stage != NULL) {
        cmsStageSignature dup_type = cmsStageType(dup_stage);
        void* dup_data = cmsStageData(dup_stage);
        (void)dup_type;
        (void)dup_data;

        // Test cmsStageNext (assuming Next is NULL by default)
        cmsStage* next_stage = cmsStageNext(dup_stage);
        (void)next_stage;

        cmsStageFree(dup_stage);
    }

    // Test cmsStageNext on original stage
    cmsStage* next_stage = cmsStageNext(stage);
    (void)next_stage;

    // Free the original stage
    cmsStageFree(stage);

    return 0;
}