// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
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

static void fuzz_profile_metadata(cmsHPROFILE hProfile) {
    // Fixed language and country codes for testing
    const char lang_en[3] = "en";
    const char country_us[3] = "US";
    const char lang_fr[3] = "fr";
    const char country_fr[3] = "FR";
    
    // Test cmsGetHeaderManufacturer
    (void)cmsGetHeaderManufacturer(hProfile);
    
    // Test cmsGetHeaderCreator
    (void)cmsGetHeaderCreator(hProfile);
    
    // Test cmsGetProfileInfoASCII with different info types
    char ascii_buffer[256];
    for (cmsInfoType info = 0; info < 20; info++) {
        (void)cmsGetProfileInfoASCII(hProfile, info, lang_en, country_us, ascii_buffer, sizeof(ascii_buffer));
        (void)cmsGetProfileInfoASCII(hProfile, info, lang_fr, country_fr, ascii_buffer, sizeof(ascii_buffer));
        // Test with zero buffer size
        (void)cmsGetProfileInfoASCII(hProfile, info, lang_en, country_us, NULL, 0);
    }
    
    // Test cmsGetProfileInfoUTF8
    char utf8_buffer[256];
    for (cmsInfoType info = 0; info < 20; info++) {
        (void)cmsGetProfileInfoUTF8(hProfile, info, lang_en, country_us, utf8_buffer, sizeof(utf8_buffer));
        (void)cmsGetProfileInfoUTF8(hProfile, info, lang_fr, country_fr, utf8_buffer, sizeof(utf8_buffer));
        // Test with zero buffer size
        (void)cmsGetProfileInfoUTF8(hProfile, info, lang_en, country_us, NULL, 0);
    }
    
    // Test cmsGetProfileInfo (wide character version)
    wchar_t wchar_buffer[256];
    for (cmsInfoType info = 0; info < 20; info++) {
        (void)cmsGetProfileInfo(hProfile, info, lang_en, country_us, wchar_buffer, sizeof(wchar_buffer)/sizeof(wchar_t));
        (void)cmsGetProfileInfo(hProfile, info, lang_fr, country_fr, wchar_buffer, sizeof(wchar_buffer)/sizeof(wchar_t));
        // Test with zero buffer size
        (void)cmsGetProfileInfo(hProfile, info, lang_en, country_us, NULL, 0);
    }
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, (cmsUInt32Number)Size);
    if (hProfile == NULL) {
        return 0;
    }
    
    // Test all metadata functions
    fuzz_profile_metadata(hProfile);
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}