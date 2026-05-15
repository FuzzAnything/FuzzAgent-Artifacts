// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <wchar.h>

static void fuzz_cmsGetProfileInfoUTF8(cmsHPROFILE hProfile) {
    if (!hProfile) return;
    
    // Test with available info types from lcms2.h
    cmsInfoType info_types[] = {
        cmsInfoDescription,
        cmsInfoManufacturer,
        cmsInfoModel,
        cmsInfoCopyright
    };
    
    char language[3] = {'e', 'n', '\0'};
    char country[3] = {'U', 'S', '\0'};
    
    size_t num_types = sizeof(info_types) / sizeof(info_types[0]);
    
    for (size_t i = 0; i < num_types; i++) {
        // First call to get required buffer size
        cmsUInt32Number needed = cmsGetProfileInfoUTF8(hProfile, info_types[i], 
                                                     language, country, 
                                                     NULL, 0);
        
        if (needed > 0) {
            // Allocate buffer and make second call
            char* buffer = (char*)malloc(needed);
            if (buffer) {
                cmsGetProfileInfoUTF8(hProfile, info_types[i], 
                                    language, country, 
                                    buffer, needed);
                free(buffer);
            }
        }
        
        // Also test with small buffer
        char small_buf[16];
        cmsGetProfileInfoUTF8(hProfile, info_types[i], 
                            language, country, 
                            small_buf, sizeof(small_buf));
    }
}

static void fuzz_cmsGetProfileInfo(cmsHPROFILE hProfile) {
    if (!hProfile) return;
    
    // Test with available info types from lcms2.h
    cmsInfoType info_types[] = {
        cmsInfoDescription,
        cmsInfoManufacturer,
        cmsInfoModel,
        cmsInfoCopyright
    };
    
    char language[3] = {'e', 'n', '\0'};
    char country[3] = {'U', 'S', '\0'};
    
    size_t num_types = sizeof(info_types) / sizeof(info_types[0]);
    
    for (size_t i = 0; i < num_types; i++) {
        // First call to get required buffer size
        cmsUInt32Number needed = cmsGetProfileInfo(hProfile, info_types[i], 
                                                 language, country, 
                                                 NULL, 0);
        
        if (needed > 0) {
            // Allocate buffer and make second call
            wchar_t* buffer = (wchar_t*)malloc(needed * sizeof(wchar_t));
            if (buffer) {
                cmsGetProfileInfo(hProfile, info_types[i], 
                                language, country, 
                                buffer, needed);
                free(buffer);
            }
        }
        
        // Also test with small buffer
        wchar_t small_buf[8];
        cmsGetProfileInfo(hProfile, info_types[i], 
                        language, country, 
                        small_buf, sizeof(small_buf)/sizeof(wchar_t));
    }
}

static void fuzz_cmsGetProfileInfoASCII(cmsHPROFILE hProfile) {
    if (!hProfile) return;
    
    // Test with available info types from lcms2.h
    cmsInfoType info_types[] = {
        cmsInfoDescription,
        cmsInfoManufacturer,
        cmsInfoModel
    };
    
    char language[3] = {'e', 'n', '\0'};
    char country[3] = {'U', 'S', '\0'};
    
    size_t num_types = sizeof(info_types) / sizeof(info_types[0]);
    
    for (size_t i = 0; i < num_types; i++) {
        // First call to get required buffer size
        cmsUInt32Number needed = cmsGetProfileInfoASCII(hProfile, info_types[i], 
                                                      language, country, 
                                                      NULL, 0);
        
        if (needed > 0) {
            // Allocate buffer and make second call
            char* buffer = (char*)malloc(needed);
            if (buffer) {
                cmsGetProfileInfoASCII(hProfile, info_types[i], 
                                     language, country, 
                                     buffer, needed);
                free(buffer);
            }
        }
        
        // Also test with small buffer
        char small_buf[16];
        cmsGetProfileInfoASCII(hProfile, info_types[i], 
                             language, country, 
                             small_buf, sizeof(small_buf));
    }
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (!hProfile) return 0;
    
    // Test cmsGetHeaderCreator
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    (void)creator;
    
    // Test cmsGetHeaderManufacturer
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    (void)manufacturer;
    
    // Test string info functions
    fuzz_cmsGetProfileInfoUTF8(hProfile);
    fuzz_cmsGetProfileInfo(hProfile);
    fuzz_cmsGetProfileInfoASCII(hProfile);
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}