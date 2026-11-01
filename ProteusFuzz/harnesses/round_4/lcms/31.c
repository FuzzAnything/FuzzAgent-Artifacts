// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
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

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE hProfile2 = NULL;
    cmsHPROFILE hProfile3 = NULL;
    cmsHPROFILE hProfile4 = NULL;
    cmsToneCurve *curves[3] = {NULL, NULL, NULL};
    cmsCIExyY whitePoint = {0, 0, 0};
    cmsCIExyYTRIPLE primaries = {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
    cmsProfileClassSignature deviceClass = 0;

    // Initialize context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        ctx = NULL; 
    }

    // Parse input data to seed values
    if (Size >= 4) {
        whitePoint.x = *(const cmsUInt16Number*)(Data);
        whitePoint.y = *(const cmsUInt16Number*)(Data + 2);
        if (Size >= 8) {
            whitePoint.Y = *(const cmsUInt32Number*)(Data + 4);
        }
    }
    if (Size >= 12) {
        primaries.Red.x = *(const cmsUInt16Number*)(Data + 8);
        primaries.Red.y = *(const cmsUInt16Number*)(Data + 10);
        if (Size >= 16) {
            primaries.Red.Y = *(const cmsUInt32Number*)(Data + 12);
        }
    }
    if (Size >= 20) {
        primaries.Green.x = *(const cmsUInt16Number*)(Data + 16);
        primaries.Green.y = *(const cmsUInt16Number*)(Data + 18);
        if (Size >= 24) {
            primaries.Green.Y = *(const cmsUInt32Number*)(Data + 20);
        }
    }
    if (Size >= 28) {
        primaries.Blue.x = *(const cmsUInt16Number*)(Data + 24);
        primaries.Blue.y = *(const cmsUInt16Number*)(Data + 26);
        if (Size >= 32) {
            primaries.Blue.Y = *(const cmsUInt32Number*)(Data + 28);
        }
    }
    if (Size >= 36) {
        deviceClass = *(const cmsProfileClassSignature*)(Data + 32);
    }

    // Fuzz cmsCreateLab2Profile (non-thread safe)
    hProfile = cmsCreateLab2Profile(&whitePoint);
    if (hProfile != NULL) {
        cmsProfileClassSignature sig = cmsGetDeviceClass(hProfile);
        cmsSetDeviceClass(hProfile, deviceClass);
        cmsProfileClassSignature sig2 = cmsGetDeviceClass(hProfile);
        cleanup_profile(hProfile);
    }

    // Fuzz cmsCreateLab2ProfileTHR
    hProfile2 = cmsCreateLab2ProfileTHR(ctx, &whitePoint);
    if (hProfile2 != NULL) {
        cmsProfileClassSignature sig = cmsGetDeviceClass(hProfile2);
        cmsSetDeviceClass(hProfile2, deviceClass);
        cmsProfileClassSignature sig2 = cmsGetDeviceClass(hProfile2);
        cleanup_profile(hProfile2);
    }

    // Fuzz cmsCreateLab4ProfileTHR
    hProfile3 = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
    if (hProfile3 != NULL) {
        cmsProfileClassSignature sig = cmsGetDeviceClass(hProfile3);
        cmsSetDeviceClass(hProfile3, deviceClass);
        cmsProfileClassSignature sig2 = cmsGetDeviceClass(hProfile3);
        cleanup_profile(hProfile3);
    }

    // Fuzz cmsCreateRGBProfileTHR
    // Create dummy tone curves if not already created
    // Use cmsBuildGamma which is available in lcms2.h
    if (curves[0] == NULL) curves[0] = cmsBuildGamma(ctx, 2.2);
    if (curves[1] == NULL) curves[1] = cmsBuildGamma(ctx, 2.2);
    if (curves[2] == NULL) curves[2] = cmsBuildGamma(ctx, 2.2);

    hProfile4 = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, curves);
    if (hProfile4 != NULL) {
        cmsProfileClassSignature sig = cmsGetDeviceClass(hProfile4);
        cmsSetDeviceClass(hProfile4, deviceClass);
        cmsProfileClassSignature sig2 = cmsGetDeviceClass(hProfile4);
        cleanup_profile(hProfile4);
    }

    // Cleanup tone curves
    if (curves[0] != NULL) cmsFreeToneCurve(curves[0]);
    if (curves[1] != NULL) cmsFreeToneCurve(curves[1]);
    if (curves[2] != NULL) cmsFreeToneCurve(curves[2]);

    // Cleanup context
    cleanup_context(ctx);

    return 0;
}