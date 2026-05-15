// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUsetASCII at cmsnamed.c:336:19 in lcms2.h
// cmsMLUsetASCII at cmsnamed.c:336:19 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

static cmsMLU* create_test_mlu(const uint8_t* data, size_t size) {
    cmsMLU* mlu = cmsMLUalloc(NULL, 1);
    if (!mlu) return NULL;
    
    // Add some test entries to the MLU
    char lang_en[3] = {'e', 'n', '\0'};
    char country_us[3] = {'U', 'S', '\0'};
    char lang_fr[3] = {'f', 'r', '\0'};
    char country_fr[3] = {'F', 'R', '\0'};
    
    // Use fuzzer data for ASCII string if available
    const char* ascii_str = (size > 0) ? (const char*)data : "Test String";
    size_t str_len = (size > 100) ? 100 : size;
    
    cmsMLUsetASCII(mlu, lang_en, country_us, ascii_str);
    
    // Add another entry if we have enough data
    if (size > 50) {
        char buffer[51];
        size_t copy_len = (size > 50) ? 50 : size;
        memcpy(buffer, data, copy_len);
        buffer[copy_len] = '\0';
        cmsMLUsetASCII(mlu, lang_fr, country_fr, buffer);
    }
    
    return mlu;
}

static cmsHPROFILE create_test_profile(const uint8_t* data, size_t size) {
    // Try to open profile from memory first
    if (size > 0) {
        cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
        if (profile) return profile;
    }
    
    // Fallback to creating a placeholder profile
    return cmsCreate_sRGBProfile();
}

int LLVMFuzzerTestOneInput_131(const uint8_t* Data, size_t Size) {
    if (Size < 6) return 0;
    
    // Extract language and country codes from fuzzer input
    char lang_code[3];
    char country_code[3];
    
    lang_code[0] = (Size > 0) ? Data[0] : 'e';
    lang_code[1] = (Size > 1) ? Data[1] : 'n';
    lang_code[2] = '\0';
    
    country_code[0] = (Size > 2) ? Data[2] : 'U';
    country_code[1] = (Size > 3) ? Data[3] : 'S';
    country_code[2] = '\0';
    
    // Create test MLU
    cmsMLU* mlu = create_test_mlu(Data, Size);
    if (!mlu) return 0;
    
    // Create test profile
    cmsHPROFILE profile = create_test_profile(Data, Size);
    if (!profile) {
        cmsMLUfree(mlu);
        return 0;
    }
    
    // Buffer for function outputs
    char buffer[256];
    wchar_t wbuffer[256];
    cmsUInt32Number buf_size = sizeof(buffer);
    
    // 1. Test cmsMLUgetUTF8
    cmsMLUgetUTF8(mlu, lang_code, country_code, buffer, buf_size);
    cmsMLUgetUTF8(mlu, lang_code, country_code, NULL, 0);
    
    // 2. Test cmsGetProfileInfoASCII
    cmsGetProfileInfoASCII(profile, cmsInfoDescription, lang_code, country_code, buffer, buf_size);
    cmsGetProfileInfoASCII(profile, cmsInfoManufacturer, lang_code, country_code, NULL, 0);
    
    // 3. Test cmsMLUtranslationsCodes
    char out_lang[3], out_country[3];
    for (cmsUInt32Number i = 0; i < 5; i++) {
        cmsMLUtranslationsCodes(mlu, i, out_lang, out_country);
    }
    
    // 4. Test cmsMLUgetASCII
    cmsMLUgetASCII(mlu, lang_code, country_code, buffer, buf_size);
    cmsMLUgetASCII(mlu, lang_code, country_code, NULL, 0);
    
    // 5. Test cmsGetProfileInfoUTF8
    cmsGetProfileInfoUTF8(profile, cmsInfoDescription, lang_code, country_code, buffer, buf_size);
    cmsGetProfileInfoUTF8(profile, cmsInfoModel, lang_code, country_code, NULL, 0);
    
    // 6. Test cmsMLUgetWide
    cmsMLUgetWide(mlu, lang_code, country_code, wbuffer, 256);
    cmsMLUgetWide(mlu, lang_code, country_code, NULL, 0);
    
    // Cleanup
    cmsMLUfree(mlu);
    cmsCloseProfile(profile);
    
    return 0;
}