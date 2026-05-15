// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsMLUgetTranslation at cmsnamed.c:663:26 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUsetUTF8 at cmsnamed.c:368:19 in lcms2.h
// cmsMLUsetUTF8 at cmsnamed.c:368:19 in lcms2.h
// cmsMLUsetUTF8 at cmsnamed.c:368:19 in lcms2.h
// cmsMLUsetASCII at cmsnamed.c:336:19 in lcms2.h
// cmsMLUsetASCII at cmsnamed.c:336:19 in lcms2.h
// cmsMLUsetASCII at cmsnamed.c:336:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <wchar.h>

static void dummy_cleanup(void) {
    // Placeholder for any cleanup if needed
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a context and MLU object
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsMLU* mlu = cmsMLUalloc(ctx, 5);
    if (!mlu) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Extract language and country codes from input (if available)
    char lang[3] = "en";
    char country[3] = "US";
    if (Size >= 6) {
        memcpy(lang, Data, 3);
        memcpy(country, Data + 3, 3);
    }

    // Test different functions based on input characteristics
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0: {
            // Test cmsMLUsetUTF8
            if (Size > 6) {
                const char* utf8_str = (const char*)(Data + 6);
                size_t str_len = Size - 6;
                
                // Ensure we have a valid string (null-terminated or at least 1 byte)
                if (str_len > 0) {
                    // Create null-terminated string
                    char* temp = (char*)malloc(str_len + 1);
                    if (temp) {
                        memcpy(temp, utf8_str, str_len);
                        temp[str_len] = '\0';
                        cmsMLUsetUTF8(mlu, lang, country, temp);
                        free(temp);
                    }
                } else {
                    // Empty string
                    cmsMLUsetUTF8(mlu, lang, country, "");
                }
            } else {
                // Empty string for small inputs
                cmsMLUsetUTF8(mlu, lang, country, "");
            }
            break;
        }
        
        case 1: {
            // Test cmsMLUsetASCII
            if (Size > 6) {
                const char* ascii_str = (const char*)(Data + 6);
                size_t str_len = Size - 6;
                
                if (str_len > 0) {
                    char* temp = (char*)malloc(str_len + 1);
                    if (temp) {
                        memcpy(temp, ascii_str, str_len);
                        temp[str_len] = '\0';
                        cmsMLUsetASCII(mlu, lang, country, temp);
                        free(temp);
                    }
                } else {
                    cmsMLUsetASCII(mlu, lang, country, "");
                }
            } else {
                cmsMLUsetASCII(mlu, lang, country, "");
            }
            break;
        }
        
        case 2: {
            // Test cmsMLUsetWide
            if (Size > 6) {
                const uint8_t* str_data = Data + 6;
                size_t str_len = Size - 6;
                
                if (str_len >= sizeof(wchar_t)) {
                    size_t wlen = str_len / sizeof(wchar_t);
                    if (wlen > 0) {
                        wchar_t* wtemp = (wchar_t*)malloc((wlen + 1) * sizeof(wchar_t));
                        if (wtemp) {
                            memcpy(wtemp, str_data, wlen * sizeof(wchar_t));
                            wtemp[wlen] = L'\0';
                            cmsMLUsetWide(mlu, lang, country, wtemp);
                            free(wtemp);
                        }
                    } else {
                        cmsMLUsetWide(mlu, lang, country, L"");
                    }
                } else {
                    cmsMLUsetWide(mlu, lang, country, L"");
                }
            } else {
                cmsMLUsetWide(mlu, lang, country, L"");
            }
            break;
        }
        
        case 3: {
            // Test cmsMLUgetTranslation
            char obtained_lang[3] = {0};
            char obtained_country[3] = {0};
            cmsMLUgetTranslation(mlu, lang, country, obtained_lang, obtained_country);
            break;
        }
        
        case 4: {
            // Test cmsMLUtranslationsCodes
            char out_lang[3] = {0};
            char out_country[3] = {0};
            uint32_t idx = (Size > 0) ? (Data[0] % 5) : 0;
            cmsMLUtranslationsCodes(mlu, idx, out_lang, out_country);
            break;
        }
        
        case 5: {
            // Test cmsMLUgetWide
            cmsUInt32Number needed = cmsMLUgetWide(mlu, lang, country, NULL, 0);
            if (needed > 0) {
                wchar_t* buffer = (wchar_t*)malloc(needed * sizeof(wchar_t));
                if (buffer) {
                    cmsMLUgetWide(mlu, lang, country, buffer, needed);
                    free(buffer);
                }
            }
            break;
        }
    }

    // Cleanup
    cmsMLUfree(mlu);
    cmsDeleteContext(ctx);
    dummy_cleanup();
    
    return 0;
}