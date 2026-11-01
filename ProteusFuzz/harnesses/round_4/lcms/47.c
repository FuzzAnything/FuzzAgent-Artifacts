// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

// Forward declarations for function pointer types used in the struct
typedef void (*_cmsPipelineEval16Fn)(const cmsPipeline* pipe, const cmsUInt16Number* in, cmsUInt16Number* out, cmsUInt32Number count);
typedef void (*_cmsPipelineEvalFloatFn)(const cmsPipeline* pipe, const cmsFloat32Number* in, cmsFloat32Number* out, cmsUInt32Number count);
typedef void (*_cmsFreeUserDataFn)(void* data);
typedef void* (*_cmsDupUserDataFn)(const void* data);

// Define the internal structures explicitly to avoid forward declaration issues
// Note: This duplicates the internal definitions to allow direct memory manipulation for fuzzing
struct _cmsStage_struct_local {
    cmsContext ContextID;
    cmsStageSignature Type;
    cmsStageSignature Implements;
    cmsUInt32Number InputChannels;
    cmsUInt32Number OutputChannels;
    void* EvalPtr; // Cast to void* to avoid undefined function pointer types
    void* DupElemPtr;
    void* FreePtr;
    void* Data;
    struct _cmsStage_struct_local* Next;
};

struct _cmsPipeline_struct_local {
    cmsStage* Elements;
    cmsUInt32Number InputChannels, OutputChannels;
    void* Data;
    void* Eval16Fn; // Cast to void*
    void* EvalFloatFn;
    void* FreeDataFn;
    void* DupDataFn;
    cmsContext ContextID;
    cmsBool SaveAs8Bits;
};

// Helper to create a minimal valid pipeline for testing
static cmsPipeline* create_dummy_pipeline(cmsContext ctx) {
    cmsPipeline* pipe = (cmsPipeline*)malloc(sizeof(struct _cmsPipeline_struct_local));
    if (!pipe) return NULL;
    
    memset(pipe, 0, sizeof(struct _cmsPipeline_struct_local));
    
    // Access members using the local struct definition logic
    // Since cmsPipeline is a typedef to the struct, we can cast to access fields
    struct _cmsPipeline_struct_local* local_pipe = (struct _cmsPipeline_struct_local*)pipe;
    
    local_pipe->ContextID = ctx;
    local_pipe->InputChannels = 3;
    local_pipe->OutputChannels = 3;
    local_pipe->Elements = NULL;
    local_pipe->Data = NULL;
    local_pipe->Eval16Fn = NULL;
    local_pipe->EvalFloatFn = NULL;
    local_pipe->FreeDataFn = NULL;
    local_pipe->DupDataFn = NULL;
    local_pipe->SaveAs8Bits = 0;
    
    return pipe;
}

// Helper to create a dummy stage
static cmsStage* create_dummy_stage(cmsContext ctx) {
    cmsStage* stage = (cmsStage*)malloc(sizeof(struct _cmsStage_struct_local));
    if (!stage) return NULL;
    
    memset(stage, 0, sizeof(struct _cmsStage_struct_local));
    
    struct _cmsStage_struct_local* local_stage = (struct _cmsStage_struct_local*)stage;
    
    local_stage->ContextID = ctx;
    local_stage->Type = 0;
    local_stage->Implements = 0;
    local_stage->InputChannels = 3;
    local_stage->OutputChannels = 3;
    local_stage->EvalPtr = NULL;
    local_stage->DupElemPtr = NULL;
    local_stage->FreePtr = NULL;
    local_stage->Data = NULL;
    local_stage->Next = NULL;
    
    return stage;
}

// Helper to free a dummy pipeline
static void free_dummy_pipeline(cmsPipeline* pipe) {
    if (!pipe) return;
    // Free the chain of stages if any
    cmsStage* current = ((struct _cmsPipeline_struct_local*)pipe)->Elements;
    while (current) {
        cmsStage* next = ((struct _cmsStage_struct_local*)current)->Next;
        cmsStageFree(current);
        current = next;
    }
    free(pipe);
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Test 1: cmsStageFree with NULL
    cmsStage* null_stage = NULL;
    cmsStageFree(null_stage);

    // Test 2: cmsStageData with NULL
    void* data_null = cmsStageData(null_stage);
    (void)data_null;

    // Test 3: cmsPipelineGetPtrToFirstStage with NULL
    cmsPipeline* null_pipe = NULL;
    cmsStage* first_null = cmsPipelineGetPtrToFirstStage(null_pipe);
    (void)first_null;

    // Test 4: cmsStageNext with NULL
    cmsStage* next_null = cmsStageNext(null_stage);
    (void)next_null;

    // Test 5: cmsStageDup with NULL
    cmsStage* dup_null = cmsStageDup(null_stage);
    (void)dup_null;

    // Test 6: cmsPipelineGetPtrToLastStage with NULL
    cmsStage* last_null = cmsPipelineGetPtrToLastStage(null_pipe);
    (void)last_null;

    // Create valid objects to test non-NULL paths
    cmsPipeline* pipe = create_dummy_pipeline(ctx);
    cmsStage* stage = create_dummy_stage(ctx);
    
    // Link stage to pipeline for testing traversal
    if (pipe && stage) {
        struct _cmsPipeline_struct_local* local_pipe = (struct _cmsPipeline_struct_local*)pipe;
        local_pipe->Elements = stage;
    }

    // Test cmsPipelineGetPtrToFirstStage with valid pipe
    if (pipe) {
        cmsStage* first = cmsPipelineGetPtrToFirstStage(pipe);
        if (first) {
            // Test cmsStageData
            void* data = cmsStageData(first);
            (void)data;

            // Test cmsStageNext
            cmsStage* next = cmsStageNext(first);
            (void)next;

            // Test cmsStageDup
            cmsStage* dup = cmsStageDup(first);
            if (dup) {
                // Test cmsStageData on dup
                void* dup_data = cmsStageData(dup);
                (void)dup_data;
                
                // Test cmsStageNext on dup
                cmsStage* dup_next = cmsStageNext(dup);
                (void)dup_next;
                
                // Free the duplicate
                cmsStageFree(dup);
            }
        }
        
        // Test cmsPipelineGetPtrToLastStage
        cmsStage* last = cmsPipelineGetPtrToLastStage(pipe);
        if (last) {
            void* last_data = cmsStageData(last);
            (void)last_data;
            cmsStage* last_next = cmsStageNext(last);
            (void)last_next;
        }
    }

    // Create a chain for thorough testing
    cmsStage* s1 = create_dummy_stage(ctx);
    cmsStage* s2 = create_dummy_stage(ctx);
    
    if (s1 && s2) {
        struct _cmsStage_struct_local* local_s1 = (struct _cmsStage_struct_local*)s1;
        local_s1->Next = s2;
        
        // Replace the single stage in pipe with this chain
        if (pipe) {
            struct _cmsPipeline_struct_local* local_pipe = (struct _cmsPipeline_struct_local*)pipe;
            local_pipe->Elements = s1;
        }
        
        // Traverse and free manually to test the chain
        cmsStage* curr = s1;
        while (curr) {
            cmsStage* next = ((struct _cmsStage_struct_local*)curr)->Next;
            cmsStageFree(curr);
            curr = next;
        }
        if (pipe) {
            struct _cmsPipeline_struct_local* local_pipe = (struct _cmsPipeline_struct_local*)pipe;
            local_pipe->Elements = NULL;
        }
    } else {
        if (s1) cmsStageFree(s1);
        if (s2) cmsStageFree(s2);
    }

    // Cleanup pipeline
    if (pipe) {
        free_dummy_pipeline(pipe);
    }

    cmsDeleteContext(ctx);

    return 0;
}