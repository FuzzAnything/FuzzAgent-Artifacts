// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_context(cmsContext ContextID) {
    if (ContextID != NULL) {
        cmsDeleteContext(ContextID);
    }
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Extract values from fuzz input
    cmsColorSpaceSignature colorSpace = (cmsColorSpaceSignature)Data[0];
    cmsFloat64Number limit = 0.0;
    if (Size >= 8) {
        memcpy(&limit, Data + 1, sizeof(cmsFloat64Number));
        // Clamp limit to reasonable range [0, 400] to avoid extreme values
        if (limit < 0.0) limit = 0.0;
        if (limit > 400.0) limit = 400.0;
    }

    int internalNotation = 0;
    if (Size >= 9) {
        internalNotation = (int)Data[8];
    }

    // Test cmsChannelsOfColorSpace
    cmsInt32Number channels = cmsChannelsOfColorSpace(colorSpace);
    (void)channels; // Use the result to avoid unused variable warnings

    // Test _cmsLCMScolorSpace
    int pixelType = _cmsLCMScolorSpace(colorSpace);
    (void)pixelType;

    // Test _cmsICCcolorSpace
    cmsColorSpaceSignature iccSig = _cmsICCcolorSpace(internalNotation);
    (void)iccSig;

    // Test cmsCreateInkLimitingDeviceLink
    cmsHPROFILE hProfile1 = cmsCreateInkLimitingDeviceLink(colorSpace, limit);
    cleanup_profile(hProfile1);

    // Test cmsCreateInkLimitingDeviceLinkTHR with default context
    // cmsCreateContext requires two arguments: Plugin and UserData
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID != NULL) {
        cmsHPROFILE hProfile2 = cmsCreateInkLimitingDeviceLinkTHR(ContextID, colorSpace, limit);
        cleanup_profile(hProfile2);
        
        // Test cmsGetPCS with a valid profile if created
        if (hProfile2 != NULL) {
            cmsColorSpaceSignature pcs = cmsGetPCS(hProfile2);
            (void)pcs;
        }

        // Test cmsGetPCS with NULL profile (undefined behavior, but safe to call in fuzzing context)
        cmsColorSpaceSignature pcsNull = cmsGetPCS(NULL);
        (void)pcsNull;

        cleanup_context(ContextID);
    }

    // Test cmsGetPCS with NULL profile directly
    cmsColorSpaceSignature pcsNullDirect = cmsGetPCS(NULL);
    (void)pcsNullDirect;

    return 0;
}