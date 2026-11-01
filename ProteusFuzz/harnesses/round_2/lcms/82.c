// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_resources(cmsContext ctx, cmsHPROFILE hProfile, cmsIOHANDLER* iohandler, void* ptr) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
    if (iohandler != NULL) {
        cmsCloseIOhandler(iohandler);
    }
    if (ptr != NULL) {
        free(ptr);
    }
    // Only delete the context if it is not the global context.
    // We cannot easily compare pointers without the global context function,
    // so we rely on the fact that cmsCreateContext(NULL, NULL) returns a new context
    // or the global one. If we created it, we delete it.
    // However, since we can't check equality safely without the function,
    // we will just pass the responsibility to the caller or assume ctx is local.
    // A safer approach for fuzzing is to always delete if we created it,
    // but here we'll skip deleting if we suspect it's global to avoid double free.
    // Given the error, we cannot call cmsGetGlobalContext.
    // We will assume ctx is always a new context created by cmsCreateContext for this test.
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Create a new context. If this fails, we cannot proceed safely with context-specific calls.
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsIOHANDLER* iohandler = NULL;
    void* ptr = NULL;
    void* realloc_ptr = NULL;
    cmsUInt32Number size_val = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    cmsUInt32Number nMax = 0;
    cmsUInt32Number* codes = NULL;
    char** descriptions = NULL;
    void* buffer = NULL;
    cmsUInt32Number buffer_len = 0;

    // Extract values from fuzz input
    if (Size >= 8) {
        size_val = ((uint32_t)Data[0] << 24) | ((uint32_t)Data[1] << 16) | ((uint32_t)Data[2] << 8) | (uint32_t)Data[3];
        intent = ((uint32_t)Data[4] << 24) | ((uint32_t)Data[5] << 16) | ((uint32_t)Data[6] << 8) | (uint32_t)Data[7];
        if (size_val > 1024 * 1024) size_val = 1024 * 1024; // Limit size
        if (intent > 10) intent = 0; // Limit intent
    }

    // Test cmsOpenIOhandlerFromMem
    if (Size > 10) {
        iohandler = cmsOpenIOhandlerFromMem(ctx, (void*)(Data + 10), Size - 10, "r");
        if (iohandler != NULL) {
            cmsCloseIOhandler(iohandler);
            iohandler = NULL;
        }
    }

    // Test cmsOpenProfileFromMemTHR
    if (Size > 20) {
        hProfile = cmsOpenProfileFromMemTHR(ctx, (const void*)(Data + 20), Size - 20);
        if (hProfile != NULL) {
            // Test cmsGetPostScriptCRD
            cmsUInt32Number crd_size = cmsGetPostScriptCRD(ctx, hProfile, intent, flags, NULL, 0);
            if (crd_size > 0 && crd_size < 1024 * 1024) {
                buffer = malloc(crd_size);
                if (buffer != NULL) {
                    cmsGetPostScriptCRD(ctx, hProfile, intent, flags, buffer, crd_size);
                    free(buffer);
                    buffer = NULL;
                }
            }
            cmsCloseProfile(hProfile);
            hProfile = NULL;
        }
    }

    // Test _cmsMallocZero (via standard malloc/calloc as per instructions)
    if (size_val > 0) {
        ptr = calloc(1, size_val);
        if (ptr != NULL) {
            // Simulate usage
            memset(ptr, 0, size_val);
            free(ptr);
            ptr = NULL;
        }
    }

    // Test _cmsRealloc
    if (Size > 30) {
        ptr = malloc(100);
        if (ptr != NULL) {
            cmsUInt32Number new_size = ((uint32_t)Data[30] << 24) | ((uint32_t)Data[31] << 16) | ((uint32_t)Data[32] << 8) | (uint32_t)Data[33];
            if (new_size > 1024 * 1024) new_size = 1024 * 1024;
            realloc_ptr = _cmsRealloc(ctx, ptr, new_size);
            if (realloc_ptr != NULL) {
                // _cmsRealloc uses the context's allocator, so we must use _cmsFree
                _cmsFree(ctx, realloc_ptr);
            } else {
                // If realloc failed, the original pointer is still valid and owned by standard malloc
                free(ptr);
            }
            ptr = NULL;
        }
    }

    // Test cmsGetSupportedIntentsTHR
    if (Size > 40) {
        nMax = ((uint32_t)Data[40] << 24) | ((uint32_t)Data[41] << 16) | ((uint32_t)Data[42] << 8) | (uint32_t)Data[43];
        if (nMax > 100) nMax = 100;
        
        codes = malloc(nMax * sizeof(cmsUInt32Number));
        descriptions = malloc(nMax * sizeof(char*));
        if (codes != NULL && descriptions != NULL) {
            cmsUInt32Number count = cmsGetSupportedIntentsTHR(ctx, nMax, codes, descriptions);
            if (count > 0) {
                for (cmsUInt32Number i = 0; i < count; i++) {
                    if (descriptions[i] != NULL) {
                        free(descriptions[i]);
                    }
                }
            }
            free(codes);
            free(descriptions);
            codes = NULL;
            descriptions = NULL;
        }
    }

    cleanup_resources(ctx, hProfile, iohandler, ptr);
    return 0;
}