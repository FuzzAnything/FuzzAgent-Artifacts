// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsHPROFILE create_test_profile(cmsContext *ctx) {
    cmsHPROFILE h = NULL;

    // Create a placeholder profile which is writable
    h = cmsCreateProfilePlaceholder(ctx);
    if (!h) return NULL;

    return h;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Create context
    cmsContext *ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsHPROFILE hProfile = create_test_profile(ctx);
    if (!hProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsUInt32Number val32 = 0;
    if (Size >= 4) {
        memcpy(&val32, Data, sizeof(cmsUInt32Number));
    }

    // Fuzz cmsSetHeaderManufacturer
    cmsSetHeaderManufacturer(hProfile, val32);

    // Fuzz cmsSetHeaderFlags
    cmsUInt32Number flags = val32;
    if (Size >= 8) {
        memcpy(&flags, Data + 4, sizeof(cmsUInt32Number));
    }
    cmsSetHeaderFlags(hProfile, flags);

    // Fuzz cmsSetHeaderRenderingIntent (0-3)
    cmsUInt32Number intent = (val32 % 4);
    cmsSetHeaderRenderingIntent(hProfile, intent);

    // Fuzz cmsGetHeaderCreator
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    (void)creator; // Suppress unused variable warning

    // Fuzz _cmsAdjustEndianess32
    cmsUInt32Number adjusted = _cmsAdjustEndianess32(val32);
    (void)adjusted;

    // Fuzz cmsSetEncodedICCversion
    cmsUInt32Number version = val32;
    if (Size >= 12) {
        memcpy(&version, Data + 8, sizeof(cmsUInt32Number));
    }
    cmsSetEncodedICCversion(hProfile, version);

    // Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}