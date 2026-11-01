// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void* safe_malloc(size_t size) {
    if (size == 0) return NULL;
    void* ptr = malloc(size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}

static void safe_free(void* ptr) {
    if (ptr) free(ptr);
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    // Create a dummy profile for testing with default parameters
    cmsHPROFILE hProfile = cmsCreateRGBProfile(NULL, NULL, NULL);
    if (hProfile == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    cmsUInt32Number Intent = 0;
    cmsUInt32Number dwFlags = 0;

    // Step 1: First call to cmsGetPostScriptCRD with NULL buffer to get size
    cmsUInt32Number bytesNeeded = cmsGetPostScriptCRD(ContextID, hProfile, Intent, dwFlags, NULL, 0);
    
    if (bytesNeeded > 0) {
        // Allocate buffer based on returned size
        void* Buffer = safe_malloc(bytesNeeded);
        if (Buffer != NULL) {
            // Step 2: Call _cmsMalloc to test custom allocator
            void* customAlloc = _cmsMalloc(ContextID, bytesNeeded);
            
            // Step 3: Second call to cmsGetPostScriptCRD with allocated buffer
            cmsGetPostScriptCRD(ContextID, hProfile, Intent, dwFlags, Buffer, bytesNeeded);
            
            // Cleanup custom allocation
            if (customAlloc != NULL) {
                _cmsFree(ContextID, customAlloc);
            }
            
            safe_free(Buffer);
        }
    }

    // Cleanup profile and context
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ContextID);

    return 0;
}