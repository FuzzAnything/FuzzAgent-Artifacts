// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
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

// Helper to extract a 32-bit signature from fuzz data
static cmsColorSpaceSignature extract_signature(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    cmsColorSpaceSignature sig;
    memcpy(&sig, data, 4);
    return sig;
}

// Helper to extract a float64 limit from fuzz data
static cmsFloat64Number extract_limit(const uint8_t *data, size_t size) {
    if (size < 8) return 0.0;
    cmsFloat64Number limit;
    memcpy(&limit, data, 8);
    return limit;
}

// Helper to extract an int notation from fuzz data
static int extract_notation(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    int notation;
    memcpy(&notation, data, 4);
    return notation;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;

    // Initialize context for thread-safe functions
    ctx = cmsCreateContext(NULL, NULL);

    // Extract data for testing
    cmsColorSpaceSignature sig = extract_signature(Data, Size);
    cmsFloat64Number limit = extract_limit(Data, Size);
    int notation = extract_notation(Data, Size);

    // 1. Test _cmsLCMScolorSpace
    int type = _cmsLCMScolorSpace(sig);
    (void)type; // Use result to avoid unused variable warning

    // 2. Test cmsCreateInkLimitingDeviceLink
    cmsHPROFILE profile1 = cmsCreateInkLimitingDeviceLink(sig, limit);
    if (profile1 != NULL) {
        cmsCloseProfile(profile1);
    }

    // 3. Test _cmsICCcolorSpace
    cmsColorSpaceSignature icsig = _cmsICCcolorSpace(notation);
    (void)icsig;

    // 4. Test cmsCreateInkLimitingDeviceLinkTHR
    cmsHPROFILE profile2 = cmsCreateInkLimitingDeviceLinkTHR(ctx, sig, limit);
    if (profile2 != NULL) {
        cmsCloseProfile(profile2);
    }

    // 5. Test cmsGetPCS (requires a valid profile)
    // We try to create a profile using the thread-safe function if possible, or reuse profile1 logic
    // Since we already tested creation, we can try to create a simple one for PCS testing if needed.
    // However, cmsCreateInkLimitingDeviceLink returns a profile we can use if not null.
    // Let's try to create a profile specifically for PCS testing if the previous ones failed or to ensure coverage.
    cmsHPROFILE profileForPCS = cmsCreateInkLimitingDeviceLinkTHR(ctx, cmsSigCmykData, 200.0);
    if (profileForPCS != NULL) {
        cmsColorSpaceSignature pcs = cmsGetPCS(profileForPCS);
        (void)pcs;
        cmsCloseProfile(profileForPCS);
    }

    // 6. Test cmsChannelsOfColorSpace
    cmsInt32Number channels = cmsChannelsOfColorSpace(sig);
    (void)channels;

    // Cleanup
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }

    return 0;
}