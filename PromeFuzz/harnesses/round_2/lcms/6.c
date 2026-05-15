// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <wchar.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Create a dummy ICC profile from fuzzer input for testing
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (!hProfile) {
        // If we can't create a profile from input, create a default sRGB profile
        hProfile = cmsCreate_sRGBProfile();
        if (!hProfile) {
            return 0;
        }
    }
    
    // Create a context for memory operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        cmsCloseProfile(hProfile);
        return 0;
    }
    
    // First call to cmsGetProfileInfo - get required buffer size
    char languageCode[3] = {'e', 'n', '\0'};
    char countryCode[3] = {'U', 'S', '\0'};
    
    // Use fuzzer data to determine info type if possible
    cmsInfoType infoType = cmsInfoDescription;
    if (Size > 0) {
        // Use first byte to select from common info types
        switch (Data[0] % 4) {
            case 0: infoType = cmsInfoDescription; break;
            case 1: infoType = cmsInfoManufacturer; break;
            case 2: infoType = cmsInfoModel; break;
            case 3: infoType = cmsInfoCopyright; break;
        }
    }
    
    // First call: get required buffer size
    cmsUInt32Number requiredSize = cmsGetProfileInfo(hProfile, infoType, 
                                                    languageCode, countryCode, 
                                                    NULL, 0);
    
    // Allocate memory using _cmsMalloc
    void* allocatedMem = NULL;
    if (requiredSize > 0) {
        // Calculate buffer size in bytes for wide characters
        cmsUInt32Number bufferSizeBytes = requiredSize * sizeof(wchar_t);
        allocatedMem = _cmsMalloc(ctx, bufferSizeBytes);
    } else {
        // Allocate a small buffer anyway for testing
        allocatedMem = _cmsMalloc(ctx, 64);
    }
    
    // Second call to cmsGetProfileInfo - actually retrieve the info
    if (allocatedMem) {
        wchar_t* buffer = (wchar_t*)allocatedMem;
        cmsUInt32Number bufferSize = 0;
        
        if (requiredSize > 0) {
            bufferSize = requiredSize;
        } else {
            bufferSize = 64 / sizeof(wchar_t);
        }
        
        cmsUInt32Number actualSize = cmsGetProfileInfo(hProfile, infoType,
                                                      languageCode, countryCode,
                                                      buffer, bufferSize);
        // Use the result to avoid optimization
        if (actualSize > 0 && buffer[0] != 0) {
            volatile wchar_t dummy = buffer[0];
            (void)dummy;
        }
    }
    
    // Free the allocated memory
    _cmsFree(ctx, allocatedMem);
    
    // Try another info type with different parameters
    if (Size > 1) {
        // Use second byte for another info type
        cmsInfoType infoType2 = (cmsInfoType)(Data[1] % 10);
        
        // Try with empty strings
        char emptyLang[3] = {'\0', '\0', '\0'};
        char emptyCountry[3] = {'\0', '\0', '\0'};
        
        cmsUInt32Number size2 = cmsGetProfileInfo(hProfile, infoType2,
                                                 emptyLang, emptyCountry,
                                                 NULL, 0);
        
        if (size2 > 0) {
            wchar_t* buffer2 = (wchar_t*)_cmsMalloc(ctx, size2 * sizeof(wchar_t));
            if (buffer2) {
                cmsUInt32Number actualSize2 = cmsGetProfileInfo(hProfile, infoType2,
                                                               emptyLang, emptyCountry,
                                                               buffer2, size2);
                volatile wchar_t dummy2 = buffer2[0];
                (void)dummy2;
                _cmsFree(ctx, buffer2);
            }
        }
    }
    
    // Clean up
    cmsDeleteContext(ctx);
    cmsCloseProfile(hProfile);
    
    return 0;
}