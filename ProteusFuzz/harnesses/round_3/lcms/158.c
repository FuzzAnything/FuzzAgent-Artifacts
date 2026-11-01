// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsChannelsOf at cmspcs.c:945:27 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void safe_cleanup(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number channels = 0;
    cmsColorSpaceSignature sig = 0;
    cmsColorSpaceSignature pcs = 0;
    int notation = 0;

    // Parse input to derive parameters
    unsigned int val1 = Data[0];
    unsigned int val2 = Data[1];
    unsigned int val3 = Data[2];
    unsigned int val4 = Data[3];

    // 1. Test cmsChannelsOf
    cmsColorSpaceSignature testSig = (cmsColorSpaceSignature)val1;
    channels = cmsChannelsOf(testSig);

    // 2. Test _cmsICCcolorSpace
    // Map input to known PT_* constants or use raw value to trigger edge cases
    notation = val2 % 10; // Limit to small range to test known constants and some unknowns
    sig = _cmsICCcolorSpace(notation);

    // 3. Create a dummy profile to test profile-related functions
    // We create a minimal valid profile in memory
    cmsUInt8Number dummyData[128] = {0};
    // Write minimal ICC header structure (simplified for fuzzing)
    // Size
    *(uint32_t*)(dummyData + 0) = 128;
    // CMM Type
    *(uint32_t*)(dummyData + 4) = 0x61637370; // "acsp"
    // Version
    *(uint32_t*)(dummyData + 8) = 0x00020000; // 2.0
    // Device Class
    *(uint32_t*)(dummyData + 12) = 0x72676220; // "rgb "
    // Color Space
    *(uint32_t*)(dummyData + 16) = 0x52474220; // "RGB "
    // PCS
    *(uint32_t*)(dummyData + 20) = 0x58595A20; // "XYZ "
    // Creation Date
    *(uint64_t*)(dummyData + 24) = 0;
    // Profile Flags
    *(uint32_t*)(dummyData + 32) = 0;
    // Device Manufacturer
    *(uint32_t*)(dummyData + 36) = 0;
    // Device Model
    *(uint32_t*)(dummyData + 40) = 0;
    // Attributes
    *(uint64_t*)(dummyData + 44) = 0;
    // Rendering Intent
    *(uint32_t*)(dummyData + 52) = 0;
    // PCS Illuminant
    *(uint32_t*)(dummyData + 56) = 0;
    // Profile Creator
    *(uint32_t*)(dummyData + 60) = 0;
    // Profile ID
    memset(dummyData + 64, 0, 16);

    // Fix: cmsOpenProfileFromMem takes only 2 arguments (MemPtr, dwSize)
    hProfile = cmsOpenProfileFromMem(dummyData, 128);

    if (hProfile != NULL) {
        // 4. Test cmsSetColorSpace
        cmsSetColorSpace(hProfile, sig);

        // 5. Test cmsGetColorSpace
        sig = cmsGetColorSpace(hProfile);

        // 6. Test cmsSetHeaderModel
        cmsSetHeaderModel(hProfile, val3);

        // 7. Test cmsGetPCS
        pcs = cmsGetPCS(hProfile);

        // Additional diverse calls with different inputs
        cmsSetColorSpace(hProfile, (cmsColorSpaceSignature)val4);
        cmsSetHeaderModel(hProfile, val2);
        sig = cmsGetColorSpace(hProfile);
        pcs = cmsGetPCS(hProfile);
    }

    safe_cleanup(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}