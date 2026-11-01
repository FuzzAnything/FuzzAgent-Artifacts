// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext create_context_from_data(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return NULL;
    return ctx;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    cmsContext ctx;
    cmsHPROFILE hProfile = NULL;
    wchar_t buffer[1024];
    cmsUInt32Number buffer_size = sizeof(buffer) / sizeof(wchar_t);
    cmsUInt32Number info_result;
    void *malloc_ptr;
    
    // Initialize buffer
    memset(buffer, 0, sizeof(buffer));
    
    // 1. Prepare environment
    // Create a context
    ctx = create_context_from_data(Data, Size);
    if (ctx == NULL) {
        return 0;
    }
    
    // Create a dummy profile from input data if Size is sufficient
    // We need at least a minimal ICC header (128 bytes) to create a profile
    if (Size >= 128) {
        hProfile = cmsOpenProfileFromMem((const unsigned char*)Data, Size);
    }
    
    // 2. Invoke cmsGetProfileInfo (First time)
    // We use "en" for language and "US" for country as defaults.
    const char lang_code[3] = {'e', 'n', '\0'};
    const char country_code[3] = {'U', 'S', '\0'};
    
    if (hProfile != NULL) {
        info_result = cmsGetProfileInfo(hProfile, cmsInfoDescription, lang_code, country_code, buffer, buffer_size);
        (void)info_result; // Suppress unused variable warning
    }
    
    // 3. Invoke _cmsMalloc
    // Use input data to determine size, clamped to reasonable limits to avoid OOM or huge allocations
    cmsUInt32Number alloc_size = 0;
    if (Size >= 2) {
        alloc_size = ((cmsUInt32Number)Data[0] << 8) | (cmsUInt32Number)Data[1];
        if (alloc_size == 0) alloc_size = 1;
        if (alloc_size > 1024 * 1024) alloc_size = 1024 * 1024; // Cap at 1MB
    } else {
        alloc_size = 64;
    }
    
    malloc_ptr = _cmsMalloc(ctx, alloc_size);
    
    // 4. Invoke cmsGetProfileInfo (Second time)
    if (hProfile != NULL) {
        info_result = cmsGetProfileInfo(hProfile, cmsInfoDescription, lang_code, country_code, buffer, buffer_size);
        (void)info_result;
    }
    
    // 5. Invoke _cmsFree
    if (malloc_ptr != NULL) {
        _cmsFree(ctx, malloc_ptr);
    }
    
    // 6. Cleanup
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
    
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
    
    return 0;
}