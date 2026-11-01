// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <wchar.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (hProfile == NULL) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);

    // Fuzz cmsGetPostScriptCSA
    {
        cmsUInt32Number requiredSize = cmsGetPostScriptCSA(ContextID, hProfile, 0, 0, NULL, 0);
        if (requiredSize > 0) {
            char *buffer = (char *)malloc(requiredSize + 1);
            if (buffer != NULL) {
                buffer[requiredSize] = '\0';
                cmsGetPostScriptCSA(ContextID, hProfile, 0, 0, buffer, requiredSize + 1);
                free(buffer);
            }
        }
    }

    // Fuzz cmsReadRawTag with various tag signatures
    // Using valid signatures defined in lcms2.h
    cmsTagSignature validTags[] = {
        cmsSigProfileDescriptionTag,
        cmsSigCopyrightTag,
        cmsSigDeviceModelDescTag,
        cmsSigDeviceSettingsTag,
        cmsSigPs2RenderingIntentTag,
        cmsSigMediaWhitePointTag,
        cmsSigMediaBlackPointTag,
        cmsSigChromaticAdaptationTag,
        cmsSigProfileSequenceDescTag,
        0x00000000, // Invalid tag
        0xDEADBEEF  // Random tag
    };

    for (int i = 0; i < sizeof(validTags) / sizeof(validTags[0]); i++) {
        if (Size < 100) continue;
        uint8_t *rawBuffer = (uint8_t *)malloc(100);
        if (rawBuffer != NULL) {
            cmsReadRawTag(hProfile, validTags[i], rawBuffer, 100);
            free(rawBuffer);
        }
    }

    // Fuzz cmsGetProfileInfoUTF8
    {
        const char langCode[3] = {'e', 'n', '\0'};
        const char countryCode[3] = {'U', 'S', '\0'};
        char buffer[256];
        cmsGetProfileInfoUTF8(hProfile, cmsInfoDescription, langCode, countryCode, buffer, sizeof(buffer));
    }

    // Fuzz cmsGetProfileInfo
    {
        const char langCode[3] = {'e', 'n', '\0'};
        const char countryCode[3] = {'U', 'S', '\0'};
        wchar_t wbuffer[256];
        cmsGetProfileInfo(hProfile, cmsInfoDescription, langCode, countryCode, wbuffer, sizeof(wbuffer) / sizeof(wchar_t));
    }

    // Fuzz cmsGetProfileInfoASCII
    {
        const char langCode[3] = {'e', 'n', '\0'};
        const char countryCode[3] = {'U', 'S', '\0'};
        char abuffer[256];
        cmsGetProfileInfoASCII(hProfile, cmsInfoDescription, langCode, countryCode, abuffer, sizeof(abuffer));
    }

    cmsDeleteContext(ContextID);
    cleanup_profile(hProfile);

    return 0;
}