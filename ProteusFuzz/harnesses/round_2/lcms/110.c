// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsColorSpaceSignature sig;
    cmsInt32Number channels;
    int internalType;
    cmsFloat64Number limit;
    int notation;

    // Extract values from input data
    if (Size >= sizeof(cmsFloat64Number)) {
        limit = *(const cmsFloat64Number*)(Data);
    } else {
        limit = 0.0;
    }

    if (Size >= sizeof(int)) {
        notation = *(const int*)(Data + (Size % (sizeof(int) + 1)));
    } else {
        notation = 0;
    }

    // Test _cmsICCcolorSpace
    sig = _cmsICCcolorSpace(notation);

    // Test _cmsLCMScolorSpace with various signatures
    internalType = _cmsLCMScolorSpace(sig);
    internalType = _cmsLCMScolorSpace(0); // Invalid
    internalType = _cmsLCMScolorSpace(cmsSigRgbData);
    internalType = _cmsLCMScolorSpace(cmsSigCmykData);
    internalType = _cmsLCMScolorSpace(cmsSigLuvData);

    // Test cmsChannelsOfColorSpace
    channels = cmsChannelsOfColorSpace(sig);
    channels = cmsChannelsOfColorSpace(cmsSigRgbData);
    channels = cmsChannelsOfColorSpace(cmsSigCmykData);
    channels = cmsChannelsOfColorSpace(cmsSigGrayData);
    channels = cmsChannelsOfColorSpace(0); // Invalid

    // Create context for thread-safe function
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx != NULL) {
        // Test cmsCreateInkLimitingDeviceLinkTHR
        hProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, cmsSigCmykData, limit);
        cleanup_profile(hProfile);
        hProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, cmsSigRgbData, limit); // Should return NULL
        cleanup_profile(hProfile);
        hProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, 0, limit); // Invalid space
        cleanup_profile(hProfile);
    }

    // Test cmsCreateInkLimitingDeviceLink (non-thread safe)
    hProfile = cmsCreateInkLimitingDeviceLink(cmsSigCmykData, limit);
    cleanup_profile(hProfile);
    hProfile = cmsCreateInkLimitingDeviceLink(cmsSigRgbData, limit); // Should return NULL
    cleanup_profile(hProfile);
    hProfile = cmsCreateInkLimitingDeviceLink(0, limit); // Invalid space
    cleanup_profile(hProfile);

    // Test cmsGetColorSpace with a valid profile
    // Create a dummy profile to test cmsGetColorSpace
    // Since we cannot easily create a valid ICC profile from scratch without more data,
    // we will skip direct testing of cmsGetColorSpace with a valid handle unless we have one.
    // However, we can test with NULL if the library handles it gracefully, though the spec says undefined.
    // We will avoid calling cmsGetColorSpace with NULL to prevent crashes.
    // Instead, we rely on the profiles created above if they are valid.
    // Re-create a profile to test cmsGetColorSpace
    hProfile = cmsCreateInkLimitingDeviceLink(cmsSigCmykData, 100.0);
    if (hProfile != NULL) {
        sig = cmsGetColorSpace(hProfile);
        // Verify the signature is what we expect
        if (sig != cmsSigCmykData) {
            // This is unexpected, but we just log it implicitly by continuing
        }
        cleanup_profile(hProfile);
    }

    // Cleanup context
    cleanup_context(ctx);

    return 0;
}