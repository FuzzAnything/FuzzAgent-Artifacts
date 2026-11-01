// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        return NULL;
    }
    cmsCloseProfile(hProfile);
    return cmsCreate_sRGBProfile();
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = create_dummy_profile(ctx);
    if (hProfile == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsUInt32Number flags = 0;
    cmsUInt32Number manufacturer = 0;
    cmsUInt32Number model = 0;
    cmsUInt32Number intent = 0;

    if (Size >= 16) {
        memcpy(&flags, Data, sizeof(cmsUInt32Number));
        memcpy(&manufacturer, Data + 4, sizeof(cmsUInt32Number));
        memcpy(&model, Data + 8, sizeof(cmsUInt32Number));
        memcpy(&intent, Data + 12, sizeof(cmsUInt32Number));
    } else if (Size >= 12) {
        memcpy(&flags, Data, sizeof(cmsUInt32Number));
        memcpy(&manufacturer, Data + 4, sizeof(cmsUInt32Number));
        memcpy(&model, Data + 8, sizeof(cmsUInt32Number));
    } else if (Size >= 8) {
        memcpy(&flags, Data, sizeof(cmsUInt32Number));
        memcpy(&manufacturer, Data + 4, sizeof(cmsUInt32Number));
    } else if (Size >= 4) {
        memcpy(&flags, Data, sizeof(cmsUInt32Number));
    }

    cmsSetHeaderFlags(hProfile, flags);
    cmsSetHeaderManufacturer(hProfile, manufacturer);
    cmsSetHeaderModel(hProfile, model);
    cmsSetHeaderRenderingIntent(hProfile, intent & 0x3);

    cmsUInt32Number retrievedFlags = cmsGetHeaderFlags(hProfile);
    cmsUInt32Number retrievedCreator = cmsGetHeaderCreator(hProfile);

    (void)retrievedFlags;
    (void)retrievedCreator;

    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}