// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void* dummy_plugin = NULL;
static void* dummy_userdata = NULL;

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    // Use fuzzer input to determine behavior variations
    int variant = 0;
    if (Size > 0) {
        variant = Data[0] % 4;
    }
    
    // Create context with different parameters based on fuzzer input
    cmsContext context = NULL;
    
    if (variant == 0) {
        context = cmsCreateContext(NULL, NULL);
    } else if (variant == 1) {
        context = cmsCreateContext(dummy_plugin, NULL);
    } else if (variant == 2) {
        context = cmsCreateContext(NULL, dummy_userdata);
    } else {
        context = cmsCreateContext(dummy_plugin, dummy_userdata);
    }
    
    if (context == NULL) {
        // If context creation fails, try to cleanup global context
        cmsDeleteContext(NULL);
        return 0;
    }
    
    // Test mutex creation
    void* mutex = _cmsCreateMutex(context);
    
    // Test mutex unlocking if mutex was created
    if (mutex != NULL) {
        _cmsUnlockMutex(context, mutex);
    }
    
    // Test mutex destruction if mutex was created - MUST be done BEFORE unregistering plugins
    // to ensure the plugin that created the mutex is still available for cleanup
    if (mutex != NULL) {
        _cmsDestroyMutex(context, mutex);
    }
    
    // Test plugin unregistration
    cmsUnregisterPluginsTHR(context);
    
    // Test context deletion
    cmsDeleteContext(context);
    
    // Also test deleting NULL context to cleanup global context
    if (Size > 1 && (Data[1] % 2 == 0)) {
        cmsDeleteContext(NULL);
    }
    
    return 0;
}