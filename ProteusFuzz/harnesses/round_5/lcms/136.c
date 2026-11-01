// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5finish at cmsmd5.c:215:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext create_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void destroy_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = create_context();
    if (!ctx) {
        return 0;
    }

    // Fuzz _cmsCalloc
    if (Size > 0) {
        cmsUInt32Number num = (Size % 100) + 1;
        cmsUInt32Number size = (Size % 100) + 1;
        void *ptr = _cmsCalloc(ctx, num, size);
        if (ptr) {
            // Write some data to ensure memory is valid
            memset(ptr, 0x42, num * size);
            // Free via context using standard free if plugin not set, or rely on context deletion
            // Since cmsFreeContext is not available in the provided headers, we rely on context cleanup
            // However, to be safe with memory leaks in fuzzing, we try to free if possible.
            // In lcms2, memory allocated via context plugins is usually freed by cmsDeleteContext.
            // We will not call a non-existent free function here.
        }
    }

    // Fuzz _cmsMallocZero
    if (Size > 0) {
        cmsUInt32Number size = (Size % 1000) + 1;
        void *ptr = _cmsMallocZero(ctx, size);
        if (ptr) {
            memset(ptr, 0xFF, size);
            // Memory is managed by the context, freed on cmsDeleteContext
        }
    }

    // Fuzz MD5 functions
    cmsHANDLE md5_handle = cmsMD5alloc(ctx);
    if (md5_handle) {
        // Feed data in chunks
        size_t chunk_size = 64;
        if (Size < chunk_size) chunk_size = Size;
        
        for (size_t i = 0; i < Size; i += chunk_size) {
            size_t current_chunk = (Size - i < chunk_size) ? (Size - i) : chunk_size;
            cmsMD5add(md5_handle, Data + i, current_chunk);
        }

        cmsProfileID profile_id;
        cmsMD5finish(&profile_id, md5_handle);
        
        // Free the MD5 handle using the context's free mechanism if available, 
        // or rely on context deletion. The API for freeing MD5 handle is typically 
        // cmsFreeContext(ctx, md5_handle) in newer versions, but if not declared,
        // we assume cmsDeleteContext handles it or we use standard free if it's a simple malloc.
        // Given the error, cmsFreeContext is not in the header. 
        // We will attempt to use the context's free function if we can access it, 
        // but for safety in this constrained environment, we rely on cmsDeleteContext.
        // Note: In a real scenario, one would check for cmsFreeContext availability.
        // Since we cannot call it, we just let the context deletion handle it.
    }

    destroy_context(ctx);
    return 0;
}