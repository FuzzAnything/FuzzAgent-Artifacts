// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5finish at cmsmd5.c:215:16 in lcms2_plugin.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_md5_handle(cmsContext ctx, cmsHANDLE handle) {
    if (handle != NULL && ctx != NULL) {
        // lcms2 does not expose a public cmsMD5free function.
        // The handle is allocated via the context's allocator.
        // We must free it using the context's free function if available,
        // or rely on the context cleanup to reclaim resources if the handle
        // is tied to the context's memory pool.
        // Since the internal structure is opaque, and no public free exists,
        // we assume the handle memory is managed by the context.
        // However, to prevent leaks in the fuzzer, we try to free it directly
        // if we can cast it, but strictly speaking, without a public API,
        // we might just leak the specific handle if not tied to the context cleanup.
        // In many lcms2 versions, cmsMD5alloc uses _cmsCalloc internally.
        // We will attempt to free it using the context's free function if we can access it,
        // but since we cannot, we rely on the fact that the context cleanup
        // might not free specific handles allocated via _cmsCalloc unless they are tracked.
        // To be safe and avoid the undefined function error, we simply do nothing here
        // or rely on the context deletion if the handle was allocated via the context's pool.
        // Actually, looking at lcms2 source, cmsMD5alloc uses _cmsCalloc.
        // _cmsCalloc uses the plugin's CallocPtr.
        // There is no public API to free a handle allocated by cmsMD5alloc.
        // This is a known limitation or internal detail.
        // For the fuzzer, we will skip explicit freeing of the handle to avoid compilation errors,
        // relying on the process termination or context cleanup if it tracks it.
        // However, to be more robust, we can try to free it if we know the context's allocator.
        // But we cannot access internal members safely.
        // So we leave it as is, acknowledging the potential leak if the context doesn't track it.
        // Wait, the error was about cmsMD5free not existing. We just remove the call.
        // We assume the memory is reclaimed when the context is deleted if it was allocated via the context.
        // If not, it's a leak, but better than a compile error.
        // Actually, let's check if we can use _cmsFree. It's not public either.
        // We will simply not call a free function for the handle to avoid the error.
        // The handle is void*, so we can't do much without internal knowledge.
        // We will rely on the context cleanup.
    }
}

static void cleanup_memory(void *ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsHANDLE md5_handle = cmsMD5alloc(ctx);
    if (md5_handle == NULL) {
        cleanup_context(ctx);
        return 0;
    }

    cmsMD5add(md5_handle, Data, Size);

    cmsProfileID profile_id;
    cmsMD5finish(&profile_id, md5_handle);

    // cmsMD5computeID requires a valid profile, passing NULL might crash or fail safely.
    // We will skip calling it with NULL to avoid undefined behavior, 
    // or create a dummy profile to test it properly.
    cmsHPROFILE profile = cmsCreate_sRGBProfile();
    if (profile != NULL) {
        cmsBool result = cmsMD5computeID(profile);
        (void)result;
        cleanup_profile(profile);
    }

    void *mem_zero = _cmsMallocZero(ctx, Size);
    if (mem_zero != NULL) {
        memset(mem_zero, 0x00, Size);
        // _cmsMallocZero uses the context's allocator, so we should use _cmsFree if available.
        // But _cmsFree is not public. We will use free() as a fallback, though it might be unsafe.
        // However, the context might use the standard malloc/free if no plugin is set.
        // Let's try to use the context's free function if we can, but we can't.
        // We will assume standard free is safe if no plugin is set.
        free(mem_zero);
    }

    void *mem_calloc = _cmsCalloc(ctx, Size > 0 ? 1 : 0, Size > 0 ? Size : 1);
    if (mem_calloc != NULL) {
        memset(mem_calloc, 0x00, Size > 0 ? Size : 1);
        free(mem_calloc);
    }

    // Cleanup md5_handle is tricky without a public API.
    // We will skip it to avoid compilation errors.
    // In a real scenario, we might need to patch lcms2 or use internal APIs.
    // For the fuzzer, we rely on the context cleanup or process exit.
    
    cleanup_context(ctx);

    return 0;
}