// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsGetHeaderAttributes at cmsio0.c:1038:16 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
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

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ContextID, Data, Size);

    if (hProfile != NULL) {
        cmsUInt32Number version = 0;
        cmsUInt64Number flags = 0;
        cmsUInt64Number newFlags = 0;
        cmsUInt32Number endian32 = 0;
        cmsUInt64Number endian64Src = 0;
        cmsUInt64Number endian64Dst = 0;

        // Test cmsGetEncodedICCversion
        version = cmsGetEncodedICCversion(hProfile);

        // Test cmsGetHeaderAttributes
        cmsGetHeaderAttributes(hProfile, &flags);

        // Test cmsSetHeaderAttributes with various flag combinations
        newFlags = flags ^ 0x0000000000000001ULL;
        cmsSetHeaderAttributes(hProfile, newFlags);
        newFlags = flags | 0x0000000000000002ULL;
        cmsSetHeaderAttributes(hProfile, newFlags);
        newFlags = flags & ~0x0000000000000003ULL;
        cmsSetHeaderAttributes(hProfile, newFlags);

        // Test _cmsAdjustEndianess32
        if (Size >= 4) {
            endian32 = ((cmsUInt32Number)Data[0] << 24) | ((cmsUInt32Number)Data[1] << 16) |
                       ((cmsUInt32Number)Data[2] << 8) | (cmsUInt32Number)Data[3];
            endian32 = _cmsAdjustEndianess32(endian32);
        }

        // Test _cmsAdjustEndianess64
        if (Size >= 8) {
            endian64Src = ((cmsUInt64Number)Data[0] << 56) | ((cmsUInt64Number)Data[1] << 48) |
                          ((cmsUInt64Number)Data[2] << 40) | ((cmsUInt64Number)Data[3] << 32) |
                          ((cmsUInt64Number)Data[4] << 24) | ((cmsUInt64Number)Data[5] << 16) |
                          ((cmsUInt64Number)Data[6] << 8) | (cmsUInt64Number)Data[7];
            _cmsAdjustEndianess64(&endian64Dst, &endian64Src);
        }

        cleanup_profile(hProfile);
    }

    cmsDeleteContext(ContextID);
    return 0;
}