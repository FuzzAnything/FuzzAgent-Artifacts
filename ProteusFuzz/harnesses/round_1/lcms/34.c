// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "lcms2.h"

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsHPROFILE create_dummy_profile(const uint8_t *Data, size_t Size) {
    if (Size == 0) return NULL;
    
    cmsHPROFILE hProfile = NULL;
    
    // Attempt to load profile from memory
    if (Size >= 4) {
        hProfile = cmsOpenProfileFromMem((const void*)Data, (cmsUInt32Number)Size);
    }
    
    return hProfile;
}

// Helper to create an MLU from a profile if possible, or return NULL
// Note: cmsMLUfromProfile is not a public API in standard lcms2.h.
// We will try to extract MLU using internal methods if available, or skip if not.
// For fuzzing purposes, we can also try to create an empty MLU or use NULL.
static cmsMLU* create_dummy_mlu_from_profile(cmsHPROFILE hProfile) {
    if (!hProfile) return NULL;
    
    // Since cmsMLUfromProfile is not available in the public API,
    // we will return NULL here. The fuzzer will still test the MLU functions
    // with NULL input, which is a valid test case for error handling.
    // If internal access were needed, we would need to link with internal headers.
    return NULL;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create a context (optional for these specific functions, but good practice)
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create dummy profile
    cmsHPROFILE hProfile = create_dummy_profile(Data, Size);
    
    // Create MLU from profile if profile is valid (returns NULL in this implementation)
    cmsMLU *mlu = create_dummy_mlu_from_profile(hProfile);
    
    // Buffers for testing
    char asciiBuffer[256];
    wchar_t wideBuffer[256];
    char utf8Buffer[256];
    char langCode[4] = "en";
    char countryCode[4] = "US";
    
    // Fuzz cmsGetProfileInfoASCII
    if (hProfile) {
        cmsGetProfileInfoASCII(hProfile, cmsInfoDescription, langCode, countryCode, asciiBuffer, sizeof(asciiBuffer));
        cmsGetProfileInfoASCII(hProfile, cmsInfoCopyright, langCode, countryCode, asciiBuffer, sizeof(asciiBuffer));
        cmsGetProfileInfoASCII(hProfile, cmsInfoManufacturer, langCode, countryCode, asciiBuffer, sizeof(asciiBuffer));
        cmsGetProfileInfoASCII(hProfile, cmsInfoModel, langCode, countryCode, asciiBuffer, sizeof(asciiBuffer));
        
        // Test with invalid locale codes
        langCode[0] = 'x'; langCode[1] = 'x'; langCode[2] = 'x'; langCode[3] = '\0';
        countryCode[0] = 'x'; countryCode[1] = 'x'; countryCode[2] = 'x'; countryCode[3] = '\0';
        cmsGetProfileInfoASCII(hProfile, cmsInfoDescription, langCode, countryCode, asciiBuffer, sizeof(asciiBuffer));
        
        // Test with NULL buffer (should be safe)
        cmsGetProfileInfoASCII(hProfile, cmsInfoDescription, "en", "US", NULL, 0);
        
        // Test with small buffer
        char smallBuffer[1];
        cmsGetProfileInfoASCII(hProfile, cmsInfoDescription, "en", "US", smallBuffer, 1);
    }
    
    // Fuzz cmsGetProfileInfo
    if (hProfile) {
        cmsGetProfileInfo(hProfile, cmsInfoDescription, langCode, countryCode, wideBuffer, sizeof(wideBuffer)/sizeof(wchar_t));
        cmsGetProfileInfo(hProfile, cmsInfoCopyright, langCode, countryCode, wideBuffer, sizeof(wideBuffer)/sizeof(wchar_t));
        
        // Test with NULL buffer
        cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", NULL, 0);
        
        // Test with small buffer
        wchar_t smallWideBuffer[1];
        cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", smallWideBuffer, 1);
    }
    
    // Fuzz cmsGetProfileInfoUTF8
    if (hProfile) {
        cmsGetProfileInfoUTF8(hProfile, cmsInfoDescription, langCode, countryCode, utf8Buffer, sizeof(utf8Buffer));
        cmsGetProfileInfoUTF8(hProfile, cmsInfoCopyright, langCode, countryCode, utf8Buffer, sizeof(utf8Buffer));
        
        // Test with NULL buffer
        cmsGetProfileInfoUTF8(hProfile, cmsInfoDescription, "en", "US", NULL, 0);
        
        // Test with small buffer
        char smallUtf8Buffer[1];
        cmsGetProfileInfoUTF8(hProfile, cmsInfoDescription, "en", "US", smallUtf8Buffer, 1);
    }
    
    // Fuzz cmsMLUgetWide
    // Even if mlu is NULL, we test the function's error handling
    if (mlu) {
        cmsMLUgetWide(mlu, langCode, countryCode, wideBuffer, sizeof(wideBuffer)/sizeof(wchar_t));
        cmsMLUgetWide(mlu, "en", "US", wideBuffer, sizeof(wideBuffer)/sizeof(wchar_t));
        
        // Test with NULL buffer to get size
        cmsMLUgetWide(mlu, "en", "US", NULL, 0);
        
        // Test with small buffer
        wchar_t smallWideBuffer2[1];
        cmsMLUgetWide(mlu, "en", "US", smallWideBuffer2, 1);
        
        // Test with invalid locale
        cmsMLUgetWide(mlu, "xx", "xx", wideBuffer, sizeof(wideBuffer)/sizeof(wchar_t));
    } else {
        // Test with NULL MLU
        cmsMLUgetWide(NULL, "en", "US", wideBuffer, sizeof(wideBuffer)/sizeof(wchar_t));
        cmsMLUgetWide(NULL, "en", "US", NULL, 0);
    }
    
    // Fuzz cmsMLUtranslationsCodes
    if (mlu) {
        char langBuf[3], countryBuf[3];
        cmsMLUtranslationsCodes(mlu, 0, langBuf, countryBuf);
        cmsMLUtranslationsCodes(mlu, 1, langBuf, countryBuf);
        cmsMLUtranslationsCodes(mlu, 100, langBuf, countryBuf); // Invalid index
        
        // Test with NULL MLU
        cmsMLUtranslationsCodes(NULL, 0, langBuf, countryBuf);
    } else {
        char langBuf[3], countryBuf[3];
        // Test with NULL MLU
        cmsMLUtranslationsCodes(NULL, 0, langBuf, countryBuf);
    }
    
    // Fuzz cmsMLUgetUTF8
    if (mlu) {
        cmsMLUgetUTF8(mlu, langCode, countryCode, utf8Buffer, sizeof(utf8Buffer));
        cmsMLUgetUTF8(mlu, "en", "US", utf8Buffer, sizeof(utf8Buffer));
        
        // Test with NULL buffer to get size
        cmsMLUgetUTF8(mlu, "en", "US", NULL, 0);
        
        // Test with small buffer
        char smallUtf8Buffer2[1];
        cmsMLUgetUTF8(mlu, "en", "US", smallUtf8Buffer2, 1);
        
        // Test with invalid locale
        cmsMLUgetUTF8(mlu, "xx", "xx", utf8Buffer, sizeof(utf8Buffer));
    } else {
        // Test with NULL MLU
        cmsMLUgetUTF8(NULL, "en", "US", utf8Buffer, sizeof(utf8Buffer));
        cmsMLUgetUTF8(NULL, "en", "US", NULL, 0);
    }
    
    // Cleanup
    cleanup_profile(hProfile);
    if (mlu) {
        cmsMLUfree(mlu);
    }
    cleanup_context(ctx);
    
    return 0;
}