// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsGetHeaderAttributes at cmsio0.c:1038:16 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    if (Size < 128) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    // Try to open a profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ContextID, Data, Size);
    if (hProfile != NULL) {
        cmsUInt64Number flags = 0;
        cmsGetHeaderAttributes(hProfile, &flags);

        // Fuzz setting attributes
        cmsSetHeaderAttributes(hProfile, flags);
        cmsSetHeaderAttributes(hProfile, 0);
        cmsSetHeaderAttributes(hProfile, (cmsUInt64Number)Data[0] << 32 | Data[1] << 24 | Data[2] << 16 | Data[3]);

        // Fuzz setting manufacturer
        cmsUInt32Number manufacturer = Data[4] << 24 | Data[5] << 16 | Data[6] << 8 | Data[7];
        cmsSetHeaderManufacturer(hProfile, manufacturer);

        // Fuzz endianess adjustments
        cmsUInt32Number val32 = Data[8] << 24 | Data[9] << 16 | Data[10] << 8 | Data[11];
        cmsUInt32Number adjusted32 = _cmsAdjustEndianess32(val32);
        (void)adjusted32;

        cmsUInt64Number val64 = ((cmsUInt64Number)Data[12] << 56) | ((cmsUInt64Number)Data[13] << 48) |
                                ((cmsUInt64Number)Data[14] << 40) | ((cmsUInt64Number)Data[15] << 32) |
                                ((cmsUInt64Number)Data[16] << 24) | ((cmsUInt64Number)Data[17] << 16) |
                                ((cmsUInt64Number)Data[18] << 8) | ((cmsUInt64Number)Data[19]);
        cmsUInt64Number adjusted64;
        _cmsAdjustEndianess64(&adjusted64, &val64);
        (void)adjusted64;

        cleanup_profile(hProfile);
    }

    // Do not call functions with NULL profile handle as they may crash (undefined behavior)
    // Removed: cmsGetHeaderAttributes(NULL, &dummyFlags);
    // Removed: cmsSetHeaderAttributes(NULL, 0);
    // Removed: cmsSetHeaderManufacturer(NULL, 0);

    cmsDeleteContext(ContextID);
    return 0;
}