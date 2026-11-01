// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "lcms2.h"

static void* safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

static cmsContext create_context(void) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    return ctx;
}

static void destroy_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsHPROFILE create_profile_from_file(const char *filename) {
    cmsHPROFILE hProfile = cmsOpenProfileFromFile(filename, "r");
    return hProfile;
}

static cmsHPROFILE create_profile_from_data(const uint8_t *data, size_t size) {
    cmsHPROFILE hProfile = NULL;
    if (data && size > 0) {
        hProfile = cmsOpenProfileFromMem((const unsigned char *)data, size);
    }
    return hProfile;
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = create_context();
    if (!ctx) return 0;

    // Create profile from data
    cmsHPROFILE hProfile = create_profile_from_data(Data, Size);
    
    // Also try to create a dummy file for file-based profile loading
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
        cmsHPROFILE hProfileFile = create_profile_from_file(dummy_file);
        
        // Only proceed if the profile was successfully loaded
        if (hProfileFile) {
            // Prepare buffers
            char lang_code[4] = {0};
            char country_code[4] = {0};
            char ascii_buf[256] = {0};
            wchar_t wide_buf[256] = {0};
            char utf8_buf[256] = {0};

            // Extract or generate language/country codes from input
            if (Size >= 2) {
                lang_code[0] = Data[0] ? Data[0] : 'e';
                lang_code[1] = Data[1] ? Data[1] : 'n';
                lang_code[2] = '\0';
            } else {
                strcpy(lang_code, "en");
            }

            if (Size >= 4) {
                country_code[0] = Data[2] ? Data[2] : 'U';
                country_code[1] = Data[3] ? Data[3] : 'S';
                country_code[2] = '\0';
            } else {
                strcpy(country_code, "US");
            }

            // Use only valid cmsInfoType values defined in lcms2.h
            cmsInfoType valid_info_types[] = {
                cmsInfoDescription,
                cmsInfoManufacturer,
                cmsInfoModel,
                cmsInfoCopyright
            };
            int num_types = sizeof(valid_info_types) / sizeof(valid_info_types[0]);

            for (int i = 0; i < num_types; i++) {
                cmsInfoType info = valid_info_types[i];
                
                // cmsGetProfileInfoASCII
                cmsUInt32Number alen = cmsGetProfileInfoASCII(hProfileFile, info, lang_code, country_code, NULL, 0);
                if (alen > 0) {
                    char *abuf = safe_malloc(alen + 1);
                    if (abuf) {
                        cmsGetProfileInfoASCII(hProfileFile, info, lang_code, country_code, abuf, alen + 1);
                        free(abuf);
                    }
                }
                cmsGetProfileInfoASCII(hProfileFile, info, lang_code, country_code, ascii_buf, sizeof(ascii_buf));

                // cmsGetProfileInfoUTF8
                cmsUInt32Number u8plen = cmsGetProfileInfoUTF8(hProfileFile, info, lang_code, country_code, NULL, 0);
                if (u8plen > 0) {
                    char *u8pbuf = safe_malloc(u8plen + 1);
                    if (u8pbuf) {
                        cmsGetProfileInfoUTF8(hProfileFile, info, lang_code, country_code, u8pbuf, u8plen + 1);
                        free(u8pbuf);
                    }
                }
                cmsGetProfileInfoUTF8(hProfileFile, info, lang_code, country_code, utf8_buf, sizeof(utf8_buf));

                // cmsGetProfileInfo (Wide)
                cmsUInt32Number wplen = cmsGetProfileInfo(hProfileFile, info, lang_code, country_code, NULL, 0);
                if (wplen > 0) {
                    wchar_t *wpbuf = safe_malloc(wplen * sizeof(wchar_t));
                    if (wpbuf) {
                        cmsGetProfileInfo(hProfileFile, info, lang_code, country_code, wpbuf, wplen);
                        free(wpbuf);
                    }
                }
                cmsGetProfileInfo(hProfileFile, info, lang_code, country_code, wide_buf, sizeof(wide_buf) / sizeof(wchar_t));
            }
            cleanup_profile(hProfileFile);
        }
    }

    // Fuzz profile info functions with hProfile (memory based)
    if (hProfile) {
        cmsInfoType valid_info_types[] = {
            cmsInfoDescription,
            cmsInfoManufacturer,
            cmsInfoModel,
            cmsInfoCopyright
        };
        int num_types = sizeof(valid_info_types) / sizeof(valid_info_types[0]);

        char lang_code[4] = {0};
        char country_code[4] = {0};
        char ascii_buf[256] = {0};
        wchar_t wide_buf[256] = {0};
        char utf8_buf[256] = {0};

        if (Size >= 2) {
            lang_code[0] = Data[0] ? Data[0] : 'e';
            lang_code[1] = Data[1] ? Data[1] : 'n';
            lang_code[2] = '\0';
        } else {
            strcpy(lang_code, "en");
        }

        if (Size >= 4) {
            country_code[0] = Data[2] ? Data[2] : 'U';
            country_code[1] = Data[3] ? Data[3] : 'S';
            country_code[2] = '\0';
        } else {
            strcpy(country_code, "US");
        }

        for (int i = 0; i < num_types; i++) {
            cmsInfoType info = valid_info_types[i];
            
            // cmsGetProfileInfoASCII
            cmsUInt32Number alen = cmsGetProfileInfoASCII(hProfile, info, lang_code, country_code, NULL, 0);
            if (alen > 0) {
                char *abuf = safe_malloc(alen + 1);
                if (abuf) {
                    cmsGetProfileInfoASCII(hProfile, info, lang_code, country_code, abuf, alen + 1);
                    free(abuf);
                }
            }
            cmsGetProfileInfoASCII(hProfile, info, lang_code, country_code, ascii_buf, sizeof(ascii_buf));

            // cmsGetProfileInfoUTF8
            cmsUInt32Number u8plen = cmsGetProfileInfoUTF8(hProfile, info, lang_code, country_code, NULL, 0);
            if (u8plen > 0) {
                char *u8pbuf = safe_malloc(u8plen + 1);
                if (u8pbuf) {
                    cmsGetProfileInfoUTF8(hProfile, info, lang_code, country_code, u8pbuf, u8plen + 1);
                    free(u8pbuf);
                }
            }
            cmsGetProfileInfoUTF8(hProfile, info, lang_code, country_code, utf8_buf, sizeof(utf8_buf));

            // cmsGetProfileInfo (Wide)
            cmsUInt32Number wplen = cmsGetProfileInfo(hProfile, info, lang_code, country_code, NULL, 0);
            if (wplen > 0) {
                wchar_t *wpbuf = safe_malloc(wplen * sizeof(wchar_t));
                if (wpbuf) {
                    cmsGetProfileInfo(hProfile, info, lang_code, country_code, wpbuf, wplen);
                    free(wpbuf);
                }
            }
            cmsGetProfileInfo(hProfile, info, lang_code, country_code, wide_buf, sizeof(wide_buf) / sizeof(wchar_t));
        }
    }

    // Cleanup
    cleanup_profile(hProfile);
    destroy_context(ctx);

    // Remove dummy file
    remove(dummy_file);

    return 0;
}