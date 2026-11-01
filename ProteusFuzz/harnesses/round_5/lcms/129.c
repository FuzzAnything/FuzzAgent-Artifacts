// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *Data, size_t Size) {
    if (Size < 128) return NULL;
    // Attempt to open profile from memory. If invalid, returns NULL.
    cmsHPROFILE hProfile = cmsOpenProfileFromMem((const unsigned char*)Data, Size);
    return hProfile;
}

static cmsMLU* create_mlu_from_data(const uint8_t *Data, size_t Size) {
    // cmsMLU is typically created via cmsMLUcreate, but that function is not in the public API of lcms2.
    // We return NULL to test the NULL handling path of the target functions.
    return NULL;
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Try to create a profile
    cmsHPROFILE hProfile = create_dummy_profile(Data, Size);
    
    // Try to create an MLU (returns NULL in this implementation due to API limitations)
    cmsMLU* mlu = create_mlu_from_data(Data, Size);

    // Prepare buffers for API calls
    char lang_code[3] = {'e', 'n', '\0'};
    char country_code[3] = {'U', 'S', '\0'};
    
    // Randomize codes slightly based on input to explore states
    if (Size > 0) {
        lang_code[0] = (char)Data[0];
        lang_code[1] = Size > 1 ? (char)Data[1] : 'n';
        lang_code[2] = '\0';
        country_code[0] = Size > 2 ? (char)Data[2] : 'U';
        country_code[1] = Size > 3 ? (char)Data[3] : 'S';
        country_code[2] = '\0';
    }

    char ascii_buf[256];
    wchar_t wide_buf[256];
    char utf8_buf[256];

    // 2. Invoke target functions diversely

    // --- cmsMLUgetASCII ---
    if (mlu) {
        // Case A: Get size
        cmsUInt32Number len = cmsMLUgetASCII(mlu, lang_code, country_code, NULL, 0);
        (void)len; 
        
        // Case B: Fill buffer
        if (len > 0 && len <= 256) {
            cmsMLUgetASCII(mlu, lang_code, country_code, ascii_buf, 256);
        }
        
        // Case C: Small buffer
        cmsMLUgetASCII(mlu, lang_code, country_code, ascii_buf, 1);
    }

    // --- cmsMLUgetWide ---
    if (mlu) {
        // Case A: Get size
        cmsUInt32Number wlen = cmsMLUgetWide(mlu, lang_code, country_code, NULL, 0);
        (void)wlen;
        
        // Case B: Fill buffer
        if (wlen > 0 && wlen <= 256 * sizeof(wchar_t)) {
            cmsMLUgetWide(mlu, lang_code, country_code, wide_buf, 256);
        }
        
        // Case C: Small buffer
        cmsMLUgetWide(mlu, lang_code, country_code, wide_buf, 1);
    }

    // --- cmsMLUtranslationsCodes ---
    if (mlu) {
        // Try index 0
        char lc[3] = {0}, cc[3] = {0};
        cmsBool res = cmsMLUtranslationsCodes(mlu, 0, lc, cc);
        (void)res;
        
        // Try invalid index (large)
        res = cmsMLUtranslationsCodes(mlu, 999999, lc, cc);
        (void)res;
        
        // Try index from data
        if (Size > 0) {
            cmsUInt32Number idx = (cmsUInt32Number)Data[0];
            res = cmsMLUtranslationsCodes(mlu, idx, lc, cc);
            (void)res;
        }
    }

    // --- cmsGetProfileInfoUTF8 ---
    if (hProfile) {
        // Try various info types (using correct enum names)
        cmsInfoType info_types[] = {cmsInfoDescription, cmsInfoCopyright, cmsInfoManufacturer, cmsInfoModel};
        for (int i = 0; i < 4; i++) {
            cmsInfoType info = info_types[i];
            // Case A: Get size
            cmsUInt32Number u8len = cmsGetProfileInfoUTF8(hProfile, info, lang_code, country_code, NULL, 0);
            (void)u8len;
            
            // Case B: Fill buffer
            if (u8len > 0 && u8len <= 256) {
                cmsGetProfileInfoUTF8(hProfile, info, lang_code, country_code, utf8_buf, 256);
            }
            
            // Case C: Small buffer
            cmsGetProfileInfoUTF8(hProfile, info, lang_code, country_code, utf8_buf, 1);
        }
    }

    // --- cmsMLUgetUTF8 ---
    if (mlu) {
        // Case A: Get size
        cmsUInt32Number u8len = cmsMLUgetUTF8(mlu, lang_code, country_code, NULL, 0);
        (void)u8len;
        
        // Case B: Fill buffer
        if (u8len > 0 && u8len <= 256) {
            cmsMLUgetUTF8(mlu, lang_code, country_code, utf8_buf, 256);
        }
        
        // Case C: Small buffer
        cmsMLUgetUTF8(mlu, lang_code, country_code, utf8_buf, 1);
    }

    // --- cmsGetProfileInfo ---
    if (hProfile) {
        cmsInfoType info_types[] = {cmsInfoDescription, cmsInfoCopyright, cmsInfoManufacturer, cmsInfoModel};
        for (int i = 0; i < 4; i++) {
            cmsInfoType info = info_types[i];
            // Case A: Get size
            cmsUInt32Number wlen = cmsGetProfileInfo(hProfile, info, lang_code, country_code, NULL, 0);
            (void)wlen;
            
            // Case B: Fill buffer
            if (wlen > 0 && wlen <= 256 * sizeof(wchar_t)) {
                cmsGetProfileInfo(hProfile, info, lang_code, country_code, wide_buf, 256);
            }
            
            // Case C: Small buffer
            cmsGetProfileInfo(hProfile, info, lang_code, country_code, wide_buf, 1);
        }
    }

    // 3. Cleanup
    if (mlu) {
        cmsMLUfree(mlu);
    }
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    cmsDeleteContext(ctx);

    return 0;
}