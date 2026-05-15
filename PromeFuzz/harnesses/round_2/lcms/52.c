// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
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

static cmsHPROFILE create_test_profile(void) {
    // Create a simple sRGB profile for testing
    return cmsCreate_sRGBProfile();
}

static cmsMLU* create_test_mlu(void) {
    cmsMLU* mlu = cmsMLUalloc(NULL, 1);
    if (!mlu) return NULL;
    
    // Add some test data to the MLU
    const wchar_t* test_string = L"Test String";
    cmsMLUsetWide(mlu, "en", "US", test_string);
    cmsMLUsetWide(mlu, "fr", "FR", L"Chaîne de test");
    
    return mlu;
}

static void init_language_country(const uint8_t* data, size_t size, 
                                  char* lang, char* country) {
    // Initialize language and country codes from fuzz data
    if (size >= 6) {
        lang[0] = data[0];
        lang[1] = data[1];
        lang[2] = '\0';
        
        country[0] = data[2];
        country[1] = data[3];
        country[2] = '\0';
    } else {
        // Default values if not enough data
        lang[0] = 'e'; lang[1] = 'n'; lang[2] = '\0';
        country[0] = 'U'; country[1] = 'S'; country[2] = '\0';
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create test objects
    cmsHPROFILE profile = create_test_profile();
    if (!profile) return 0;
    
    cmsMLU* mlu = create_test_mlu();
    if (!mlu) {
        cmsCloseProfile(profile);
        return 0;
    }
    
    // Initialize language/country codes
    char lang[3], country[3];
    init_language_country(Data, Size, lang, country);
    
    // Determine buffer size from fuzz data
    cmsUInt32Number buffer_size = 256;
    if (Size >= 8) {
        buffer_size = (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7];
        buffer_size = buffer_size % 4096 + 1; // Limit to reasonable size
    }
    
    // Use selector from fuzz data to choose which functions to test
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0: {
            // Test cmsMLUgetWide
            wchar_t* wide_buffer = (wchar_t*)calloc(buffer_size, sizeof(wchar_t));
            if (wide_buffer) {
                cmsUInt32Number needed = cmsMLUgetWide(mlu, lang, country, NULL, 0);
                if (needed > 0 && needed <= buffer_size * sizeof(wchar_t)) {
                    cmsMLUgetWide(mlu, lang, country, wide_buffer, buffer_size);
                }
                free(wide_buffer);
            }
            break;
        }
        
        case 1: {
            // Test cmsGetProfileInfoUTF8
            char* utf8_buffer = (char*)malloc(buffer_size);
            if (utf8_buffer) {
                cmsUInt32Number needed = cmsGetProfileInfoUTF8(profile, cmsInfoDescription, 
                                                              lang, country, NULL, 0);
                if (needed > 0 && needed <= buffer_size) {
                    cmsGetProfileInfoUTF8(profile, cmsInfoDescription, 
                                         lang, country, utf8_buffer, buffer_size);
                }
                free(utf8_buffer);
            }
            break;
        }
        
        case 2: {
            // Test cmsGetProfileInfoASCII
            char* ascii_buffer = (char*)malloc(buffer_size);
            if (ascii_buffer) {
                cmsUInt32Number needed = cmsGetProfileInfoASCII(profile, cmsInfoDescription, 
                                                               lang, country, NULL, 0);
                if (needed > 0 && needed <= buffer_size) {
                    cmsGetProfileInfoASCII(profile, cmsInfoDescription, 
                                          lang, country, ascii_buffer, buffer_size);
                }
                free(ascii_buffer);
            }
            break;
        }
        
        case 3: {
            // Test cmsMLUgetUTF8
            char* utf8_buffer = (char*)malloc(buffer_size);
            if (utf8_buffer) {
                cmsUInt32Number needed = cmsMLUgetUTF8(mlu, lang, country, NULL, 0);
                if (needed > 0 && needed <= buffer_size) {
                    cmsMLUgetUTF8(mlu, lang, country, utf8_buffer, buffer_size);
                }
                free(utf8_buffer);
            }
            break;
        }
        
        case 4: {
            // Test cmsMLUtranslationsCodes
            char out_lang[3], out_country[3];
            cmsUInt32Number idx = 0;
            if (Size >= 9) {
                idx = Data[8] % 10; // Limit index to reasonable range
            }
            cmsMLUtranslationsCodes(mlu, idx, out_lang, out_country);
            break;
        }
        
        case 5: {
            // Test cmsMLUgetASCII
            char* ascii_buffer = (char*)malloc(buffer_size);
            if (ascii_buffer) {
                cmsUInt32Number needed = cmsMLUgetASCII(mlu, lang, country, NULL, 0);
                if (needed > 0 && needed <= buffer_size) {
                    cmsMLUgetASCII(mlu, lang, country, ascii_buffer, buffer_size);
                }
                free(ascii_buffer);
            }
            break;
        }
    }
    
    // Cleanup
    cmsCloseProfile(profile);
    cmsMLUfree(mlu);
    
    return 0;
}