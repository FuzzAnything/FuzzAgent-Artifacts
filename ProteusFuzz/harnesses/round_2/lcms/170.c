// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetHeaderAttributes at cmsio0.c:1038:16 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
// cmsGetHeaderAttributes at cmsio0.c:1038:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_170(const uint8_t *Data, size_t Size) {
    if (Size < 128) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ContextID, Data, Size);
    if (hProfile == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    cmsUInt64Number flags = 0;
    cmsUInt32Number version = 0;
    cmsUInt64Number endian64_src = 0;
    cmsUInt64Number endian64_dst = 0;
    cmsUInt32Number endian32_val = 0;

    // Fuzz cmsGetHeaderAttributes
    cmsGetHeaderAttributes(hProfile, &flags);

    // Fuzz cmsSetHeaderAttributes with various flag values derived from input
    if (Size >= 8) {
        cmsUInt64Number fuzz_flags = 0;
        memcpy(&fuzz_flags, Data, 8);
        cmsSetHeaderAttributes(hProfile, fuzz_flags);
    }

    // Fuzz cmsSetEncodedICCversion
    if (Size >= 4) {
        cmsUInt32Number fuzz_version = 0;
        memcpy(&fuzz_version, Data + 8, 4);
        cmsSetEncodedICCversion(hProfile, fuzz_version);
    }

    // Fuzz _cmsAdjustEndianess64
    if (Size >= 16) {
        memcpy(&endian64_src, Data + 12, 8);
        _cmsAdjustEndianess64(&endian64_dst, &endian64_src);
    }

    // Fuzz _cmsAdjustEndianess32
    if (Size >= 20) {
        memcpy(&endian32_val, Data + 20, 4);
        cmsUInt32Number adjusted32 = _cmsAdjustEndianess32(endian32_val);
        (void)adjusted32;
    }

    // Re-fetch attributes to ensure state consistency
    cmsGetHeaderAttributes(hProfile, &flags);

    cleanup_profile(hProfile);
    cmsDeleteContext(ContextID);

    return 0;
}