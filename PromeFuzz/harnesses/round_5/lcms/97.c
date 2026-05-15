// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void* dummy_plugin = NULL;
static void* dummy_user_data = NULL;

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    // Create a context for all operations
    cmsContext context = cmsCreateContext(dummy_plugin, dummy_user_data);
    if (!context) {
        return 0;
    }

    // Use fuzzer input to derive parameters
    // Ensure we have at least some bytes for parameter derivation
    if (Size < 4) {
        cmsDeleteContext(context);
        return 0;
    }

    // Derive parameters from fuzzer input
    cmsUInt32Number param1 = (cmsUInt32Number)Data[0];
    cmsUInt32Number param2 = (cmsUInt32Number)Data[1];
    cmsUInt32Number param3 = (cmsUInt32Number)Data[2];
    cmsUInt32Number param4 = (cmsUInt32Number)Data[3];

    // Test _cmsMalloc
    void* malloc_ptr = _cmsMalloc(context, param1 % 1024); // Limit size to avoid excessive allocation
    if (malloc_ptr) {
        // Test _cmsCalloc
        void* calloc_ptr = _cmsCalloc(context, param2 % 16, param3 % 64); // Limit parameters
        if (calloc_ptr) {
            // Test _cmsRealloc - store result in separate variable
            void* realloc_ptr = _cmsRealloc(context, malloc_ptr, param4 % 2048); // Limit size
            if (realloc_ptr) {
                // If realloc succeeded, the original malloc_ptr was freed
                // Use the new pointer for further operations
                malloc_ptr = realloc_ptr;
            } else {
                // If realloc failed, original pointer is still valid
                // Don't assign NULL to malloc_ptr to avoid double-free
                // Keep malloc_ptr as is
            }
            
            // Test cmsGBDAlloc
            cmsHANDLE gbd_handle = cmsGBDAlloc(context);
            if (gbd_handle) {
                // Clean up GBD handle
                _cmsFree(context, gbd_handle);
            }
            
            // Clean up calloc pointer
            _cmsFree(context, calloc_ptr);
        }
        
        // Clean up malloc_ptr only if it wasn't replaced by realloc_ptr
        // or if realloc failed and malloc_ptr is still valid
        // Note: if realloc succeeded, malloc_ptr now points to the new allocation
        // which needs to be freed exactly once
        if (malloc_ptr) {
            _cmsFree(context, malloc_ptr);
        }
    }

    // Test edge cases with NULL pointers
    _cmsFree(context, NULL);
    
    // Test _cmsRealloc with NULL pointer (should act like malloc)
    void* realloc_null_ptr = _cmsRealloc(context, NULL, param1 % 512);
    if (realloc_null_ptr) {
        _cmsFree(context, realloc_null_ptr);
    }

    // Test _cmsCalloc with zero parameters
    void* zero_calloc_ptr = _cmsCalloc(context, 0, 0);
    if (zero_calloc_ptr) {
        _cmsFree(context, zero_calloc_ptr);
    }

    // Test _cmsMalloc with zero size
    void* zero_malloc_ptr = _cmsMalloc(context, 0);
    if (zero_malloc_ptr) {
        _cmsFree(context, zero_malloc_ptr);
    }

    // Clean up context
    cmsDeleteContext(context);

    return 0;
}