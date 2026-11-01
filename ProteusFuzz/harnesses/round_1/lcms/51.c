// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

// Helper to safely extract a float64 from fuzzer input
static cmsFloat64Number extract_limit(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsFloat64Number)) {
        return 100.0; // Default safe value
    }
    cmsFloat64Number limit;
    memcpy(&limit, data, sizeof(cmsFloat64Number));
    // Clamp to reasonable range for ink limiting (0-400)
    if (limit < 0.0) limit = 0.0;
    if (limit > 400.0) limit = 400.0;
    return limit;
}

// Helper to extract color space signature
static cmsColorSpaceSignature extract_color_space(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsColorSpaceSignature)) {
        return cmsSigCmykData; // Default to CMYK for ink limiting tests
    }
    cmsColorSpaceSignature sig;
    memcpy(&sig, data, sizeof(cmsColorSpaceSignature));
    return sig;
}

// Helper to extract int (for internal notation)
static int extract_int(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) {
        return 0;
    }
    int val;
    memcpy(&val, data, sizeof(int));
    return val;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsColorSpaceSignature csSig;
    cmsFloat64Number limit;
    int internalNotation;
    cmsInt32Number channels;
    int pixelType;

    // Extract inputs from fuzzer data
    limit = extract_limit(Data, Size);
    csSig = extract_color_space(Data, Size);
    internalNotation = extract_int(Data, Size);

    // 1. Test cmsCreateInkLimitingDeviceLink
    // Creates a device-link profile limiting total ink
    hProfile = cmsCreateInkLimitingDeviceLink(csSig, limit);
    if (hProfile != NULL) {
        // Cleanup
        cmsCloseProfile(hProfile);
        hProfile = NULL;
    }

    // 2. Test cmsCreateInkLimitingDeviceLinkTHR
    // Try with global context (NULL) and a custom context
    hProfile = cmsCreateInkLimitingDeviceLinkTHR(NULL, csSig, limit);
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
        hProfile = NULL;
    }

    // Create a custom context to test THR version with non-NULL context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx != NULL) {
        hProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, csSig, limit);
        if (hProfile != NULL) {
            cmsCloseProfile(hProfile);
            hProfile = NULL;
        }
        cmsDeleteContext(ctx);
        ctx = NULL;
    }

    // 3. Test cmsGetColorSpace
    // We need a valid profile to test this properly.
    // Create a simple RGB profile to use as a valid handle
    hProfile = cmsCreateRGBProfile(NULL, NULL, NULL);
    if (hProfile != NULL) {
        csSig = cmsGetColorSpace(hProfile);
        // Verify we got a signature
        (void)csSig; 
        cmsCloseProfile(hProfile);
        hProfile = NULL;
    }

    // 4. Test _cmsLCMScolorSpace
    // Map ICC signature to internal pixel type
    pixelType = _cmsLCMScolorSpace(csSig);
    (void)pixelType;

    // 5. Test _cmsICCcolorSpace
    // Map internal notation to ICC signature
    csSig = _cmsICCcolorSpace(internalNotation);
    (void)csSig;

    // 6. Test cmsChannelsOfColorSpace
    // Get channel count for a signature
    channels = cmsChannelsOfColorSpace(csSig);
    (void)channels;

    // Additional exploration: try with known signatures
    // Fixed list of valid signatures from lcms2.h
    cmsColorSpaceSignature known_sigs[] = {
        cmsSigRgbData, 
        cmsSigCmykData, 
        cmsSigLuvData, 
        cmsSigLabData, 
        cmsSigHsvData, 
        cmsSigHlsData, 
        cmsSigYxyData,
        cmsSigXYZData, 
        cmsSigGrayData, 
        cmsSigYCbCrData
    };
    int num_sigs = sizeof(known_sigs) / sizeof(known_sigs[0]);

    for (int i = 0; i < num_sigs; i++) {
        cmsColorSpaceSignature testSig = known_sigs[i];

        // Test cmsCreateInkLimitingDeviceLink with known sigs
        hProfile = cmsCreateInkLimitingDeviceLink(testSig, limit);
        if (hProfile != NULL) {
            cmsCloseProfile(hProfile);
        }

        // Test cmsCreateInkLimitingDeviceLinkTHR with known sigs
        hProfile = cmsCreateInkLimitingDeviceLinkTHR(NULL, testSig, limit);
        if (hProfile != NULL) {
            cmsCloseProfile(hProfile);
        }

        // Test _cmsLCMScolorSpace with known sigs
        pixelType = _cmsLCMScolorSpace(testSig);
        (void)pixelType;

        // Test cmsChannelsOfColorSpace with known sigs
        channels = cmsChannelsOfColorSpace(testSig);
        (void)channels;
    }

    // Test _cmsICCcolorSpace with known PT constants
    // Using only constants that are guaranteed to exist in standard lcms2.h
    int known_pts[] = {
        PT_RGB, PT_CMYK, PT_Lab, PT_XYZ, PT_HSV, PT_HLS, 
        PT_Yxy, PT_YCbCr
    };
    int num_pts = sizeof(known_pts) / sizeof(known_pts[0]);

    for (int i = 0; i < num_pts; i++) {
        csSig = _cmsICCcolorSpace(known_pts[i]);
        (void)csSig;
    }

    return 0;
}