// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_profile_info_functions(cmsHPROFILE hProfile) {
    if (!hProfile) return;
    
    // Test cmsGetHeaderManufacturer
    (void)cmsGetHeaderManufacturer(hProfile);
    
    // Test cmsGetHeaderCreator
    (void)cmsGetHeaderCreator(hProfile);
    
    // Test info functions with various parameters
    const char* lang_codes[] = {"en", "fr", "de", "ja", "  ", "\0\0\0"};
    const char* country_codes[] = {"US", "FR", "DE", "JP", "  ", "\0\0\0"};
    
    // Buffer for ASCII and UTF-8 functions
    char buffer[256];
    
    // Buffer for wide character function
    wchar_t wbuffer[256];
    
    // Test different info types (common values from lcms2.h)
    cmsInfoType info_types[] = {
        0,  // cmsInfoDescription
        1,  // cmsInfoManufacturer
        2,  // cmsInfoModel
        3,  // cmsInfoCopyright
        4,  // cmsInfoManufacturer
        5,  // cmsInfoDescriptionML
        6,  // cmsInfoManufacturerML
        7,  // cmsInfoModelML
        8,  // cmsInfoCopyrightML
        9,  // cmsInfoProfileID
        10, // cmsInfoRenderingIntent
        100 // Invalid value to test edge cases
    };
    
    // Test various combinations
    for (size_t i = 0; i < sizeof(info_types) / sizeof(info_types[0]); i++) {
        for (size_t l = 0; l < sizeof(lang_codes) / sizeof(lang_codes[0]); l++) {
            for (size_t c = 0; c < sizeof(country_codes) / sizeof(country_codes[0]); c++) {
                // Test cmsGetProfileInfoASCII
                (void)cmsGetProfileInfoASCII(hProfile, info_types[i], 
                                            lang_codes[l], country_codes[c],
                                            buffer, sizeof(buffer));
                
                // Test cmsGetProfileInfo with NULL buffer first to get size
                cmsUInt32Number needed = cmsGetProfileInfo(hProfile, info_types[i],
                                                          lang_codes[l], country_codes[c],
                                                          NULL, 0);
                if (needed > 0 && needed < sizeof(wbuffer) / sizeof(wbuffer[0])) {
                    (void)cmsGetProfileInfo(hProfile, info_types[i],
                                           lang_codes[l], country_codes[c],
                                           wbuffer, sizeof(wbuffer) / sizeof(wbuffer[0]));
                }
                
                // Test cmsGetProfileInfoUTF8
                (void)cmsGetProfileInfoUTF8(hProfile, info_types[i],
                                           lang_codes[l], country_codes[c],
                                           buffer, sizeof(buffer));
            }
        }
    }
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Try to open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, (cmsUInt32Number)Size);
    
    if (hProfile) {
        // Test all functions with the valid profile
        fuzz_profile_info_functions(hProfile);
        
        // Clean up
        cmsCloseProfile(hProfile);
    }
    
    // Also test with a smaller subset of data
    if (Size > 128) {
        size_t half_size = Size / 2;
        cmsHPROFILE hProfile2 = cmsOpenProfileFromMem(Data, (cmsUInt32Number)half_size);
        if (hProfile2) {
            fuzz_profile_info_functions(hProfile2);
            cmsCloseProfile(hProfile2);
        }
    }
    
    return 0;
}