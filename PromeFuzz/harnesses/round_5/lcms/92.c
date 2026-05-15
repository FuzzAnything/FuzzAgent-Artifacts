// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <wchar.h>

static void fuzz_cmsGetProfileInfoUTF8(cmsHPROFILE hProfile, const uint8_t *data, size_t size) {
    char LanguageCode[3] = {0};
    char CountryCode[3] = {0};
    
    if (size >= 6) {
        LanguageCode[0] = data[0];
        LanguageCode[1] = data[1];
        LanguageCode[2] = data[2];
        CountryCode[0] = data[3];
        CountryCode[1] = data[4];
        CountryCode[2] = data[5];
    } else {
        LanguageCode[0] = 'e';
        LanguageCode[1] = 'n';
        LanguageCode[2] = '\0';
        CountryCode[0] = 'U';
        CountryCode[1] = 'S';
        CountryCode[2] = '\0';
    }
    
    cmsInfoType infoTypes[] = {
        cmsInfoDescription,
        cmsInfoManufacturer,
        cmsInfoModel,
        cmsInfoCopyright
    };
    
    size_t numInfoTypes = sizeof(infoTypes) / sizeof(infoTypes[0]);
    for (size_t i = 0; i < numInfoTypes; i++) {
        cmsUInt32Number needed = cmsGetProfileInfoUTF8(hProfile, infoTypes[i], 
                                                      LanguageCode, CountryCode, 
                                                      NULL, 0);
        
        if (needed > 0) {
            char *buffer = (char*)malloc(needed + 1);
            if (buffer) {
                cmsGetProfileInfoUTF8(hProfile, infoTypes[i], 
                                     LanguageCode, CountryCode, 
                                     buffer, needed + 1);
                free(buffer);
            }
        }
    }
}

static void fuzz_cmsGetProfileInfo(cmsHPROFILE hProfile, const uint8_t *data, size_t size) {
    char LanguageCode[3] = {0};
    char CountryCode[3] = {0};
    
    if (size >= 6) {
        LanguageCode[0] = data[0];
        LanguageCode[1] = data[1];
        LanguageCode[2] = data[2];
        CountryCode[0] = data[3];
        CountryCode[1] = data[4];
        CountryCode[2] = data[5];
    } else {
        LanguageCode[0] = 'e';
        LanguageCode[1] = 'n';
        LanguageCode[2] = '\0';
        CountryCode[0] = 'U';
        CountryCode[1] = 'S';
        CountryCode[2] = '\0';
    }
    
    cmsInfoType infoTypes[] = {
        cmsInfoDescription,
        cmsInfoManufacturer,
        cmsInfoModel,
        cmsInfoCopyright
    };
    
    size_t numInfoTypes = sizeof(infoTypes) / sizeof(infoTypes[0]);
    for (size_t i = 0; i < numInfoTypes; i++) {
        cmsUInt32Number needed = cmsGetProfileInfo(hProfile, infoTypes[i], 
                                                  LanguageCode, CountryCode, 
                                                  NULL, 0);
        
        if (needed > 0) {
            wchar_t *buffer = (wchar_t*)malloc((needed + 1) * sizeof(wchar_t));
            if (buffer) {
                cmsGetProfileInfo(hProfile, infoTypes[i], 
                                 LanguageCode, CountryCode, 
                                 buffer, needed + 1);
                free(buffer);
            }
        }
    }
}

static void fuzz_cmsGetProfileInfoASCII(cmsHPROFILE hProfile, const uint8_t *data, size_t size) {
    char LanguageCode[3] = {0};
    char CountryCode[3] = {0};
    
    if (size >= 6) {
        LanguageCode[0] = data[0];
        LanguageCode[1] = data[1];
        LanguageCode[2] = data[2];
        CountryCode[0] = data[3];
        CountryCode[1] = data[4];
        CountryCode[2] = data[5];
    } else {
        LanguageCode[0] = 'e';
        LanguageCode[1] = 'n';
        LanguageCode[2] = '\0';
        CountryCode[0] = 'U';
        CountryCode[1] = 'S';
        CountryCode[2] = '\0';
    }
    
    cmsInfoType infoTypes[] = {
        cmsInfoDescription,
        cmsInfoManufacturer,
        cmsInfoModel,
        cmsInfoCopyright
    };
    
    size_t numInfoTypes = sizeof(infoTypes) / sizeof(infoTypes[0]);
    for (size_t i = 0; i < numInfoTypes; i++) {
        cmsUInt32Number needed = cmsGetProfileInfoASCII(hProfile, infoTypes[i], 
                                                       LanguageCode, CountryCode, 
                                                       NULL, 0);
        
        if (needed > 0) {
            char *buffer = (char*)malloc(needed + 1);
            if (buffer) {
                cmsGetProfileInfoASCII(hProfile, infoTypes[i], 
                                      LanguageCode, CountryCode, 
                                      buffer, needed + 1);
                free(buffer);
            }
        }
    }
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (!hProfile) {
        return 0;
    }
    
    fuzz_cmsGetProfileInfoUTF8(hProfile, Data, Size);
    
    cmsGetHeaderCreator(hProfile);
    
    fuzz_cmsGetProfileInfo(hProfile, Data, Size);
    
    fuzz_cmsGetProfileInfoASCII(hProfile, Data, Size);
    
    cmsGetHeaderManufacturer(hProfile);
    
    cmsCloseProfile(hProfile);
    
    return 0;
}