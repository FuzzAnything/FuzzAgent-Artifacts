// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsUnregisterPlugins at cmsplugin.c:627:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

/* 
 * Fuzz driver for Little CMS plugin management APIs.
 * Targets: cmsPluginTHR, cmsUnregisterPlugins, cmsPlugin, 
 * _cmsLockMutex, cmsUnregisterPluginsTHR, _cmsDestroyMutex.
 */

/* 
 * Note: The internal structure of cmsPluginBase varies by version.
 * In many versions, the mutex callbacks are not direct members of the base struct
 * but are part of specific plugin types (e.g., _cmsThreadPluginChunkType).
 * Since we cannot directly access internal private members without risking ABI breaks
 * or compilation errors (as seen in the prompt), we will construct a minimal valid
 * plugin by setting only the guaranteed public fields: Magic and Next.
 * We will rely on the library's internal validation to handle the rest or
 * simply pass a struct that looks like a valid base to trigger the registration logic.
 */

static cmsPluginBase create_dummy_plugin(const uint8_t *data, size_t size) {
    cmsPluginBase plugin;
    
    /* Initialize all fields to 0/NULL first */
    memset(&plugin, 0, sizeof(plugin));
    
    /* Magic number must be 0x434D5350 ('CMSP') */
    plugin.Magic = 0x434D5350;
    
    /* 
     * We cannot set 'Version' or specific callback pointers like 'LockMutexPtr'
     * directly if they are not exposed in the public header for this build.
     * The registration functions will validate the Magic and potentially the version
     * internally. By leaving other fields as 0, we test the error handling paths
     * for missing callbacks or invalid structures.
     */
    
    /* Ensure the linked list terminator is set */
    plugin.Next = NULL;
    
    return plugin;
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    cmsContext ctx = NULL;
    cmsBool result = FALSE;
    cmsPluginBase plugin;
    void *dummy_mutex = NULL;

    if (Size == 0) return 0;

    /* 
     * Strategy:
     * 1. Create a context (if first byte is even) or use NULL (global).
     * 2. Create a dummy plugin based on input data.
     * 3. Attempt to register via cmsPluginTHR or cmsPlugin.
     * 4. Attempt mutex operations if we can infer a valid context.
     * 5. Cleanup via cmsUnregisterPluginsTHR or cmsUnregisterPlugins.
     */

    /* Determine context strategy based on input */
    if ((Data[0] % 2) == 0) {
        ctx = cmsCreateContext(NULL, NULL);
        if (ctx == NULL) return 0;
    } else {
        ctx = NULL; /* Use global context */
    }

    /* Prepare plugin */
    plugin = create_dummy_plugin(Data, Size);

    /* 
     * Test cmsPluginTHR 
     * Note: Passing a linked list is supported, but for fuzzing simplicity,
     * we pass a single plugin or NULL.
     */
    if (Size > 1 && (Data[1] & 0x01)) {
        result = cmsPluginTHR(ctx, &plugin);
        (void)result; /* Ignore result to avoid early exit on failure */
    }

    /* 
     * Test cmsPlugin (Global registration)
     */
    if (Size > 1 && (Data[1] & 0x02)) {
        result = cmsPlugin(&plugin);
        (void)result;
    }

    /* 
     * Test Mutex operations
     * Since we cannot reliably set the mutex callbacks in the plugin struct
     * due to struct layout differences, we call these functions with a dummy mutex
     * to test the internal logic of _cmsLockMutex and _cmsDestroyMutex.
     * These functions check if the plugin has a callback; if not, they return safely.
     */
    if (Size > 1 && (Data[1] & 0x04)) {
        dummy_mutex = malloc(16);
        if (dummy_mutex) {
            if (Size > 1 && (Data[1] & 0x08)) {
                result = _cmsLockMutex(ctx, dummy_mutex);
                (void)result;
            }
            
            if (Size > 1 && (Data[1] & 0x10)) {
                _cmsDestroyMutex(ctx, dummy_mutex);
            }
            free(dummy_mutex);
        }
    }

    /* 
     * Test cmsUnregisterPluginsTHR
     * Reverts thread-local plugins for the specific context.
     */
    if (ctx != NULL && Size > 1 && (Data[1] & 0x20)) {
        cmsUnregisterPluginsTHR(ctx);
    }

    /* 
     * Test cmsUnregisterPlugins
     * Global cleanup.
     */
    if (Size > 1 && (Data[1] & 0x40)) {
        cmsUnregisterPlugins();
    }

    /* Cleanup context */
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }

    return 0;
}