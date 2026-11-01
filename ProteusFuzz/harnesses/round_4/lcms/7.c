// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext create_dummy_context(cmsContext ctx) {
    if (ctx == NULL) {
        ctx = cmsCreateContext(NULL, NULL);
    }
    return ctx;
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a minimal valid ICC profile for testing
    // We use a dummy file approach as per instructions
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (f) {
        // Write a minimal valid ICC header (128 bytes)
        uint8_t header[128];
        memset(header, 0, sizeof(header));
        // Set size to 128 (minimum)
        *(uint32_t*)header = 128;
        // Set signature to "acsp"
        header[4] = 'a';
        header[5] = 'c';
        header[6] = 's';
        header[7] = 'p';
        fwrite(header, 1, sizeof(header), f);
        fclose(f);

        cmsHPROFILE hProfile = cmsOpenProfileFromFile(filename, "r");
        return hProfile;
    }
    return NULL;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    wchar_t buffer[256];
    const char langCode[3] = { 'e', 'n', '\0' };
    const char countryCode[3] = { 'U', 'S', '\0' };
    cmsInfoType infoType = (cmsInfoType)(Data[0] % 10); // Limit to valid range
    cmsUInt32Number bufferSize = (Size > 1) ? (Data[1] % 256) : 100;

    // Step 1: Prepare environment
    ctx = create_dummy_context(ctx);
    if (ctx == NULL) return 0;

    hProfile = create_dummy_profile(ctx);
    if (hProfile == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Step 2: Invoke cmsGetProfileInfo (First call)
    memset(buffer, 0, sizeof(buffer));
    cmsGetProfileInfo(hProfile, infoType, langCode, countryCode, buffer, bufferSize);

    // Step 3: Invoke _cmsMalloc
    cmsUInt32Number allocSize = (Size > 2) ? (Data[2] % 1024) : 64;
    void *ptr = _cmsMalloc(ctx, allocSize);

    // Step 4: Invoke cmsGetProfileInfo (Second call)
    if (ptr != NULL) {
        memset(buffer, 0, sizeof(buffer));
        cmsGetProfileInfo(hProfile, infoType, langCode, countryCode, buffer, bufferSize);
    }

    // Step 5: Invoke _cmsFree
    if (ptr != NULL) {
        _cmsFree(ctx, ptr);
    }

    // Step 6: Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}