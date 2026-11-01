// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        context = NULL;
    }

    int ourNotation = 0;
    cmsFloat64Number limit = 0.0;
    cmsColorSpaceSignature colorSpaceSig = 0;

    if (Size >= 1) {
        ourNotation = Data[0];
    }
    if (Size >= 2) {
        limit = (Data[1] * 256.0 + (Size > 2 ? Data[2] : 0)) / 256.0;
        if (limit < 0) limit = 0;
        if (limit > 400) limit = 400;
    }
    
    // Ensure we have enough bytes to construct a 4-byte signature safely
    // We need at least 7 bytes (indices 3, 4, 5, 6) to read 4 bytes for the signature
    if (Size >= 7) {
        colorSpaceSig = ((cmsColorSpaceSignature)Data[3] << 24) |
                        ((cmsColorSpaceSignature)Data[4] << 16) |
                        ((cmsColorSpaceSignature)Data[5] << 8) |
                        ((cmsColorSpaceSignature)Data[6]);
    } else {
        // If not enough data, use a default valid signature or 0
        colorSpaceSig = 0;
    }

    // Test 1: _cmsICCcolorSpace
    cmsColorSpaceSignature sig1 = _cmsICCcolorSpace(ourNotation);
    (void)sig1;

    // Test 2: cmsChannelsOfColorSpace
    cmsInt32Number channels = cmsChannelsOfColorSpace(colorSpaceSig);
    (void)channels;

    // Test 3: _cmsLCMScolorSpace
    int lcmsType = _cmsLCMScolorSpace(colorSpaceSig);
    (void)lcmsType;

    // Test 4: cmsCreateInkLimitingDeviceLink (non-threaded)
    cmsHPROFILE profile1 = cmsCreateInkLimitingDeviceLink(colorSpaceSig, limit);
    cleanup_profile(profile1);

    // Test 5: cmsCreateInkLimitingDeviceLinkTHR (threaded)
    cmsHPROFILE profile2 = cmsCreateInkLimitingDeviceLinkTHR(context, colorSpaceSig, limit);
    cleanup_profile(profile2);

    // Test 6: cmsGetColorSpace with a valid profile (if created)
    cmsHPROFILE profile3 = cmsCreateInkLimitingDeviceLink(cmsSigCmykData, limit);
    if (profile3 != NULL) {
        cmsColorSpaceSignature retrievedSig = cmsGetColorSpace(profile3);
        (void)retrievedSig;
        cleanup_profile(profile3);
    }

    if (context != NULL) {
        cmsDeleteContext(context);
    }

    return 0;
}