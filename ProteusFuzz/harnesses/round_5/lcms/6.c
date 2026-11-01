// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
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
#include <wchar.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsContext create_dummy_context(void) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return NULL;
    }
    return ctx;
}

static cmsHPROFILE create_dummy_profile(void) {
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    return hProfile;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = create_dummy_context();
    if (ctx == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = create_dummy_profile();
    if (hProfile == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Step 1: Invoke cmsGetProfileInfo
    wchar_t buffer[256];
    memset(buffer, 0, sizeof(buffer));
    const char langCode[3] = "en";
    const char countryCode[3] = "US";
    
    cmsUInt32Number infoResult1 = cmsGetProfileInfo(hProfile, cmsInfoDescription, langCode, countryCode, buffer, sizeof(buffer) / sizeof(wchar_t));
    
    // Step 2: Invoke _cmsMalloc
    cmsUInt32Number allocSize = 0;
    if (Size >= 4) {
        allocSize = *(const cmsUInt32Number*)(Data);
        // Prevent excessively large allocations
        if (allocSize > 1024 * 1024) {
            allocSize = 1024 * 1024;
        }
    } else {
        allocSize = 1;
    }
    
    void* ptr = _cmsMalloc(ctx, allocSize);
    
    // Step 3: Invoke cmsGetProfileInfo again (possibly with different parameters derived from input)
    cmsInfoType infoType = cmsInfoDescription;
    if (Size > 4) {
        infoType = (cmsInfoType)(Data[4] % 100); // Use input to vary info type
    }
    
    wchar_t buffer2[256];
    memset(buffer2, 0, sizeof(buffer2));
    cmsUInt32Number infoResult2 = cmsGetProfileInfo(hProfile, infoType, langCode, countryCode, buffer2, sizeof(buffer2) / sizeof(wchar_t));
    
    // Step 4: Invoke _cmsFree
    if (ptr != NULL) {
        _cmsFree(ctx, ptr);
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}