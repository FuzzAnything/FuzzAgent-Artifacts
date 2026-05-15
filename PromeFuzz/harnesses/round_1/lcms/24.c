// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static double readDouble(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + sizeof(double) > size) return 0.0;
    double value;
    memcpy(&value, data + *offset, sizeof(double));
    *offset += sizeof(double);
    return value;
}

static cmsCIExyY readCIExyY(const uint8_t* data, size_t size, size_t* offset) {
    cmsCIExyY wp;
    wp.x = readDouble(data, size, offset);
    wp.y = readDouble(data, size, offset);
    wp.Y = readDouble(data, size, offset);
    return wp;
}

static cmsCIExyYTRIPLE readCIExyYTRIPLE(const uint8_t* data, size_t size, size_t* offset) {
    cmsCIExyYTRIPLE primaries;
    primaries.Red = readCIExyY(data, size, offset);
    primaries.Green = readCIExyY(data, size, offset);
    primaries.Blue = readCIExyY(data, size, offset);
    return primaries;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    size_t offset = 0;
    int selector = Data[0] % 6;
    offset = 1;

    switch (selector) {
        case 0: {
            cmsHPROFILE profile = cmsCreateNULLProfileTHR(ctx);
            if (profile) cmsCloseProfile(profile);
            break;
        }
        case 1: {
            cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
            if (profile) cmsCloseProfile(profile);
            break;
        }
        case 2: {
            cmsHPROFILE profile = cmsCreateXYZProfileTHR(ctx);
            if (profile) cmsCloseProfile(profile);
            break;
        }
        case 3: {
            cmsCIExyY whitePoint = readCIExyY(Data, Size, &offset);
            double gamma = readDouble(Data, Size, &offset);
            if (gamma < 0.1) gamma = 2.2;
            cmsToneCurve* curve = cmsBuildGamma(ctx, gamma);
            if (curve) {
                cmsHPROFILE profile = cmsCreateGrayProfileTHR(ctx, &whitePoint, curve);
                if (profile) cmsCloseProfile(profile);
                cmsFreeToneCurve(curve);
            }
            break;
        }
        case 4: {
            cmsCIExyY whitePoint = readCIExyY(Data, Size, &offset);
            cmsCIExyYTRIPLE primaries = readCIExyYTRIPLE(Data, Size, &offset);
            cmsToneCurve* curves[3] = {NULL, NULL, NULL};
            for (int i = 0; i < 3; i++) {
                double gamma = readDouble(Data, Size, &offset);
                if (gamma < 0.1) gamma = 2.2;
                curves[i] = cmsBuildGamma(ctx, gamma);
            }
            if (curves[0] && curves[1] && curves[2]) {
                cmsHPROFILE profile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, curves);
                if (profile) cmsCloseProfile(profile);
            }
            for (int i = 0; i < 3; i++) {
                if (curves[i]) cmsFreeToneCurve(curves[i]);
            }
            break;
        }
        case 5: {
            cmsCIExyY whitePoint = readCIExyY(Data, Size, &offset);
            cmsHPROFILE profile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
            if (profile) cmsCloseProfile(profile);
            break;
        }
    }

    cmsDeleteContext(ctx);
    return 0;
}