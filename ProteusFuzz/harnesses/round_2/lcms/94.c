// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

/*
 * Fuzz driver for lcms internal transform API functions.
 * Target functions:
 * 1. _cmsGetTransformFormattersFloat
 * 2. _cmsGetTransformWorkerFlags
 * 3. _cmsGetTransformMaxWorkers
 * 4. _cmsGetTransformUserData
 * 5. _cmsSetTransformUserData
 * 6. _cmsGetTransformWorker
 */

static cmsContext g_context = NULL;
static cmsHPROFILE g_profile1 = NULL;
static cmsHPROFILE g_profile2 = NULL;
static cmsHTRANSFORM g_transform = NULL;

static void cleanup_resources(void) {
    if (g_transform != NULL) {
        cmsDeleteTransform(g_transform);
        g_transform = NULL;
    }
    if (g_profile1 != NULL) {
        cmsCloseProfile(g_profile1);
        g_profile1 = NULL;
    }
    if (g_profile2 != NULL) {
        cmsCloseProfile(g_profile2);
        g_profile2 = NULL;
    }
    if (g_context != NULL) {
        // cmsCloseContext is not available in standard lcms2 API for user contexts in some versions
        // We rely on global cleanup or just set to NULL if we can't close it explicitly
        // In many lcms2 versions, cmsCloseContext is only for internal use or not exported for user contexts
        // We will skip explicit close if it causes issues, but try to use standard cleanup
        // Actually, cmsCloseContext exists in lcms2.h for contexts created with cmsCreateContext
        // The error might be due to missing header or version mismatch. 
        // Let's assume standard lcms2.h is sufficient. If not, we might need to link differently.
        // However, the error says 'undeclared function'. Let's check if we need to include something else.
        // Usually cmsCloseContext is in lcms2.h. The error might be a build environment issue.
        // But to be safe, let's try to use the standard API.
        // If cmsCloseContext is truly missing, we might have to leak the context or use a different approach.
        // Given the error, let's assume the environment has lcms2.h but maybe the function is not exported?
        // Let's try to call it anyway as it should be there. If the compiler complains, we might need to adjust.
        // Actually, looking at lcms2.h, cmsCloseContext is defined.
        // The error might be due to the specific build flags or headers.
        // Let's proceed assuming it's available. If not, the fuzzer might crash or leak, but that's a build issue.
        // Wait, the error says "call to undeclared function". This means the header doesn't have it.
        // In some versions of lcms2, cmsCloseContext is not exposed for user contexts?
        // Actually, cmsCreateContext returns a cmsContext, and cmsCloseContext should close it.
        // Let's check the signature: CMSAPI void CMSEXPORT cmsCloseContext(cmsContext ContextID);
        // It should be in lcms2.h.
        // Maybe the error is due to the specific version or build.
        // To avoid compilation error, we can try to cast or use a different approach.
        // But let's assume the header is correct and the error is a red herring or specific to the build.
        // We will keep the call. If it fails, the build system needs to be fixed.
        // However, to strictly follow the error, we must remove or fix the call.
        // Since the error says "undeclared", we cannot call it.
        // Alternative: Don't create a context, use global context (NULL).
        // cmsCreateTransform can use NULL for context.
        // Let's change the approach: use NULL context for everything.
        g_context = NULL;
    }
}

// Helper to create a dummy profile from memory
static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 128) {
        return NULL;
    }
    // cmsOpenProfileFromMem takes (const void * MemPtr, cmsUInt32Number dwSize)
    return cmsOpenProfileFromMem(data, (cmsUInt32Number)size);
}

// User data free function matching _cmsFreeUserDataFn signature: void (*)(cmsContext, void *)
static void free_user_data_callback(cmsContext ctx, void *ptr) {
    (void)ctx; // Unused
    if (ptr) {
        free(ptr);
    }
}

static void *allocate_user_data(size_t size) {
    if (size == 0 || size > 1024 * 1024) {
        return NULL;
    }
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    // Cleanup from previous run
    cleanup_resources();

    // We will use the global context (NULL) to avoid context management issues
    cmsContext ctx = NULL;

    // Split input data to create two profiles
    size_t split = Size / 2;
    if (split < 1) split = 1;

    g_profile1 = create_dummy_profile(Data, split);
    g_profile2 = create_dummy_profile(Data + split, Size - split);

    // If we couldn't create both profiles, we cannot create a transform
    if (g_profile1 == NULL || g_profile2 == NULL) {
        cleanup_resources();
        return 0;
    }

    // Create a transform using global context (NULL)
    g_transform = cmsCreateTransform(g_profile1, cmsSigRgbData, g_profile2, cmsSigRgbData, 0, 0);
    if (g_transform == NULL) {
        cleanup_resources();
        return 0;
    }

    // Cast to internal struct
    struct _cmstransform_struct *transform_struct = (struct _cmstransform_struct *)g_transform;

    // Fuzz internal APIs
    // 1. _cmsGetTransformFormattersFloat
    cmsFormatterFloat from_input = NULL;
    cmsFormatterFloat to_output = NULL;
    _cmsGetTransformFormattersFloat(transform_struct, &from_input, &to_output);
    // Try with NULL pointers
    _cmsGetTransformFormattersFloat(transform_struct, NULL, NULL);
    _cmsGetTransformFormattersFloat(transform_struct, &from_input, NULL);
    _cmsGetTransformFormattersFloat(transform_struct, NULL, &to_output);

    // 2. _cmsGetTransformWorkerFlags
    cmsUInt32Number flags = _cmsGetTransformWorkerFlags(transform_struct);
    (void)flags;

    // 3. _cmsGetTransformMaxWorkers
    cmsInt32Number max_workers = _cmsGetTransformMaxWorkers(transform_struct);
    (void)max_workers;

    // 4. _cmsGetTransformUserData
    void *user_data = _cmsGetTransformUserData(transform_struct);
    (void)user_data;

    // 5. _cmsSetTransformUserData
    size_t user_data_size = 0;
    if (Size >= 2) {
        user_data_size = ((Data[0] << 8) | Data[1]) % 1024;
    }
    void *new_user_data = allocate_user_data(user_data_size);
    
    // Set user data with free function (matching _cmsFreeUserDataFn: void (*)(cmsContext, void *))
    _cmsSetTransformUserData(transform_struct, new_user_data, free_user_data_callback);
    
    // Set user data without free function
    void *new_user_data2 = allocate_user_data(user_data_size);
    _cmsSetTransformUserData(transform_struct, new_user_data2, NULL);
    
    // Retrieve user data again to verify
    void *retrieved_data = _cmsGetTransformUserData(transform_struct);
    (void)retrieved_data;

    // 6. _cmsGetTransformWorker
    _cmsTransform2Fn worker_fn = _cmsGetTransformWorker(transform_struct);
    (void)worker_fn;

    // Cleanup allocated user data if we didn't set a free function
    if (new_user_data2) {
        free(new_user_data2);
    }

    // Cleanup resources
    cleanup_resources();

    return 0;
}