// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Static helper to create a dummy plugin structure for fuzzing
// We cast to cmsPluginBase* but initialize carefully based on known fields
static cmsPluginBase* create_dummy_plugin(const uint8_t *Data, size_t Size) {
    // Allocate memory for the plugin structure
    cmsPluginBase *plugin = (cmsPluginBase*)malloc(sizeof(cmsPluginBase));
    if (!plugin) return NULL;

    // Initialize with magic number expected by lcms
    // The structure typically has 'Magic' as the first field
    plugin->Magic = 0x434D5320; // "CMS " magic number

    // We cannot set 'Version' directly as it's not a member in the public struct definition
    // The internal structure might have it, but we rely on the default or zero-initialization
    // for fields we don't explicitly set, which is safe for testing validation logic.
    
    // Zero out the rest to avoid garbage data issues
    // We skip the first sizeof(cmsPluginBase) - sizeof(cmsPluginBase) logic and just memset the whole thing
    // then overwrite Magic.
    memset(plugin, 0, sizeof(cmsPluginBase));
    plugin->Magic = 0x434D5320;

    return plugin;
}

// Static dummy mutex structure
static void* create_dummy_mutex(const uint8_t *Data, size_t Size) {
    // Allocate a small buffer to act as a mutex handle
    void *mtx = malloc(64);
    if (!mtx) return NULL;
    
    // Copy some data into it to vary state
    size_t copy_size = (Size < 64) ? Size : 64;
    memcpy(mtx, Data, copy_size);
    return mtx;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare the environment
    // cmsCreateContext requires two arguments: Plugin and UserData
    // We pass NULL for both to create a default context
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    // Create a dummy plugin
    cmsPluginBase *plugin = create_dummy_plugin(Data, Size);
    if (!plugin) return 0;

    // Create a dummy mutex
    void *mtx = create_dummy_mutex(Data, Size);

    // 2. Invoke target functions diversely

    // Test cmsPluginTHR: Register plugin in context
    // This tests validation of magic numbers and version compatibility
    cmsBool ret1 = cmsPluginTHR(ContextID, plugin);
    
    // Test cmsPlugin: Register plugin globally
    // This tests global registration logic
    cmsBool ret2 = cmsPlugin(plugin);

    // Test _cmsLockMutex: Attempt to lock
    // This tests the mutex callback logic
    cmsBool ret3 = _cmsLockMutex(ContextID, mtx);

    // Test _cmsUnlockMutex: Attempt to unlock
    // This tests the unlock callback logic
    _cmsUnlockMutex(ContextID, mtx);

    // Test _cmsDestroyMutex: Attempt to destroy mutex
    // This tests the destroy callback logic
    // Note: _cmsDestroyMutex internally calls free() on the mutex handle via the default memory manager.
    // Therefore, we must NOT free 'mtx' manually after this call to avoid double-free.
    _cmsDestroyMutex(ContextID, mtx);

    // Test cmsUnregisterPluginsTHR: Clear plugins
    // This tests resetting handlers to defaults
    cmsUnregisterPluginsTHR(ContextID);

    // 3. Cleanup
    // Free allocated resources
    if (plugin) {
        free(plugin);
    }
    // 'mtx' has already been freed by _cmsDestroyMutex, so we do not free it here.
    // if (mtx) { free(mtx); } // REMOVED to fix double-free

    // Destroy the context
    cmsDeleteContext(ContextID);

    return 0;
}