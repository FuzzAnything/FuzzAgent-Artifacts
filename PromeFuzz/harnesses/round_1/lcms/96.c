// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_dummy_tone_curve(cmsContext ctx) {
    // Create a simple linear tone curve
    cmsFloat64Number params[10] = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    return cmsBuildParametricToneCurve(ctx, 1, params);
}

static cmsCIExyY create_dummy_xyY(const uint8_t* data, size_t size, size_t offset) {
    cmsCIExyY xyY;
    if (offset + sizeof(cmsFloat64Number) * 3 <= size) {
        memcpy(&xyY.x, data + offset, sizeof(cmsFloat64Number));
        memcpy(&xyY.y, data + offset + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
        memcpy(&xyY.Y, data + offset + sizeof(cmsFloat64Number) * 2, sizeof(cmsFloat64Number));
    } else {
        // Default D50 white point
        xyY.x = 0.3457;
        xyY.y = 0.3585;
        xyY.Y = 1.0;
    }
    return xyY;
}

static cmsCIExyYTRIPLE create_dummy_primaries(const uint8_t* data, size_t size, size_t offset) {
    cmsCIExyYTRIPLE primaries;
    if (offset + sizeof(cmsFloat64Number) * 9 <= size) {
        primaries.Red = create_dummy_xyY(data, size, offset);
        primaries.Green = create_dummy_xyY(data, size, offset + sizeof(cmsFloat64Number) * 3);
        primaries.Blue = create_dummy_xyY(data, size, offset + sizeof(cmsFloat64Number) * 6);
    } else {
        // Default sRGB primaries
        primaries.Red.x = 0.6400; primaries.Red.y = 0.3300; primaries.Red.Y = 1.0;
        primaries.Green.x = 0.3000; primaries.Green.y = 0.6000; primaries.Green.Y = 1.0;
        primaries.Blue.x = 0.1500; primaries.Blue.y = 0.0600; primaries.Blue.Y = 1.0;
    }
    return primaries;
}

int LLVMFuzzerTestOneInput_96(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Create dummy tone curves
    cmsToneCurve* curves[3];
    for (int i = 0; i < 3; i++) {
        curves[i] = create_dummy_tone_curve(ctx);
        if (!curves[i]) {
            cmsDeleteContext(ctx);
            return 0;
        }
    }

    // Create RGB profile
    cmsCIExyY whitePoint = create_dummy_xyY(Data, Size, 0);
    cmsCIExyYTRIPLE primaries = create_dummy_primaries(Data, Size, sizeof(cmsFloat64Number) * 3);
    
    cmsHPROFILE rgbProfile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, curves);
    if (rgbProfile) {
        // Test cmsSetDeviceClass and cmsGetDeviceClass
        cmsProfileClassSignature devClass = (cmsProfileClassSignature)(Data[0] % 8);
        cmsSetDeviceClass(rgbProfile, devClass);
        cmsProfileClassSignature retrievedClass = cmsGetDeviceClass(rgbProfile);
        (void)retrievedClass; // Suppress unused variable warning

        // Test cmsSetColorSpace and cmsGetColorSpace
        cmsColorSpaceSignature colorSpace = (cmsColorSpaceSignature)(Data[1] % 16);
        cmsSetColorSpace(rgbProfile, colorSpace);
        cmsColorSpaceSignature retrievedColorSpace = cmsGetColorSpace(rgbProfile);
        (void)retrievedColorSpace; // Suppress unused variable warning

        cmsCloseProfile(rgbProfile);
    }

    // Create linearization device link profile
    cmsColorSpaceSignature linkColorSpace = (cmsColorSpaceSignature)(Data[2] % 16);
    cmsHPROFILE linkProfile = cmsCreateLinearizationDeviceLinkTHR(ctx, linkColorSpace, curves);
    if (linkProfile) {
        // Test device class operations on link profile
        cmsProfileClassSignature linkDevClass = (cmsProfileClassSignature)(Data[3] % 8);
        cmsSetDeviceClass(linkProfile, linkDevClass);
        cmsProfileClassSignature retrievedLinkClass = cmsGetDeviceClass(linkProfile);
        (void)retrievedLinkClass; // Suppress unused variable warning

        // Test color space operations on link profile
        cmsColorSpaceSignature linkCS = (cmsColorSpaceSignature)((Data[0] + Data[1]) % 16);
        cmsSetColorSpace(linkProfile, linkCS);
        cmsColorSpaceSignature retrievedLinkCS = cmsGetColorSpace(linkProfile);
        (void)retrievedLinkCS; // Suppress unused variable warning

        cmsCloseProfile(linkProfile);
    }

    // Clean up tone curves
    for (int i = 0; i < 3; i++) {
        if (curves[i]) {
            cmsFreeToneCurve(curves[i]);
        }
    }

    cmsDeleteContext(ctx);
    return 0;
}