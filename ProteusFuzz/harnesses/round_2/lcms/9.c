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

static void* safe_malloc(size_t size) {
    if (size == 0) return NULL;
    return malloc(size);
}

static void safe_free(void* ptr) {
    free(ptr);
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    cmsHPROFILE hProfile = NULL;
    
    if (Size >= 128) {
        hProfile = cmsOpenProfileFromMem((const unsigned char*)Data, Size);
    }

    wchar_t* wBuffer = NULL;
    cmsUInt32Number bufSize = 0;

    // Step 1: Invoke cmsGetProfileInfo (First call)
    // Only proceed if hProfile is valid to avoid SEGV
    if (hProfile != NULL && Size > 0) {
        bufSize = (Size > 1000) ? 1000 : Size;
        wBuffer = (wchar_t*)safe_malloc(bufSize * sizeof(wchar_t));
        if (wBuffer != NULL) {
            memset(wBuffer, 0, bufSize * sizeof(wchar_t));
            cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", wBuffer, bufSize);
        }
    }

    // Step 2: Invoke _cmsMalloc
    cmsUInt32Number allocSize = 0;
    if (Size >= 4) {
        allocSize = *(const cmsUInt32Number*)(Data);
        if (allocSize > 1024 * 1024) allocSize = 1024 * 1024;
        if (allocSize == 0) allocSize = 64;
    } else {
        allocSize = 64;
    }

    void* ptr = _cmsMalloc(ContextID, allocSize);
    if (ptr != NULL) {
        memset(ptr, 0xAA, allocSize);
    }

    // Step 3: Invoke cmsGetProfileInfo (Second call)
    // Only proceed if hProfile is valid
    if (hProfile != NULL) {
        if (wBuffer != NULL) {
            free(wBuffer);
            wBuffer = NULL;
        }
        
        cmsUInt32Number newBufSize = (allocSize > 0) ? allocSize : 256;
        wBuffer = (wchar_t*)safe_malloc(newBufSize * sizeof(wchar_t));
        if (wBuffer != NULL) {
            memset(wBuffer, 0, newBufSize * sizeof(wchar_t));
            char lang[3] = "en";
            char country[3] = "US";
            if (Size >= 6) {
                lang[0] = Data[0]; lang[1] = Data[1]; lang[2] = '\0';
                country[0] = Data[2]; country[1] = Data[3]; country[2] = '\0';
            }
            cmsGetProfileInfo(hProfile, cmsInfoDescription, lang, country, wBuffer, newBufSize);
        }
    }

    // Step 4: Invoke _cmsFree
    if (ptr != NULL) {
        _cmsFree(ContextID, ptr);
    }

    // Cleanup
    if (wBuffer != NULL) {
        safe_free(wBuffer);
    }
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
    cmsDeleteContext(ContextID);

    return 0;
}