// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <wchar.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Create a dummy profile from fuzz data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (!hProfile) {
        // If we can't create a profile, use a default sRGB profile
        hProfile = cmsCreate_sRGBProfile();
        if (!hProfile) {
            return 0;
        }
    }

    // First cmsGetProfileInfo call with stack-allocated buffer
    wchar_t stackBuffer[64];
    const char lang[3] = {'e', 'n', '\0'};
    const char country[3] = {'U', 'S', '\0'};
    
    // Use fuzz data to determine info type if possible
    cmsInfoType infoType = cmsInfoDescription;
    if (Size > 0) {
        infoType = (cmsInfoType)(Data[0] % 17); // 17 valid cmsInfoType values
    }
    
    // Only call cmsGetProfileInfo if we have a valid profile
    if (hProfile) {
        cmsGetProfileInfo(hProfile, infoType, lang, country, stackBuffer, sizeof(stackBuffer)/sizeof(wchar_t));
    }

    // _cmsMalloc allocation
    size_t allocSize = 128;
    if (Size > 1) {
        allocSize = Data[1] * 16; // Scale to get various sizes
        if (allocSize == 0) allocSize = 16;
        if (allocSize > 1024*1024) allocSize = 1024*1024; // Cap allocation size
    }
    
    void* allocatedMem = _cmsMalloc(NULL, allocSize);
    
    // Second cmsGetProfileInfo call with allocated buffer if available
    if (allocatedMem && hProfile) {
        wchar_t* heapBuffer = (wchar_t*)allocatedMem;
        size_t bufferSize = allocSize / sizeof(wchar_t);
        if (bufferSize > 0) {
            cmsGetProfileInfo(hProfile, infoType, lang, country, heapBuffer, bufferSize);
        }
    }
    
    // _cmsFree cleanup
    _cmsFree(NULL, allocatedMem);
    
    // Clean up profile if we created one
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    return 0;
}