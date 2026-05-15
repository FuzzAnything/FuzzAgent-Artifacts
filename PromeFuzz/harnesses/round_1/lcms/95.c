// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
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

static void fuzz_cmsCreateLab2ProfileTHR(const uint8_t* data, size_t size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return;
    
    cmsCIExyY whitePoint;
    if (size >= sizeof(cmsCIExyY)) {
        memcpy(&whitePoint, data, sizeof(cmsCIExyY));
    } else {
        whitePoint.x = 0.3127;
        whitePoint.y = 0.3290;
        whitePoint.Y = 1.0;
    }
    
    cmsHPROFILE profile = cmsCreateLab2ProfileTHR(ctx, &whitePoint);
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    cmsDeleteContext(ctx);
}

static void fuzz_cmsCreateLab2Profile(const uint8_t* data, size_t size) {
    cmsCIExyY whitePoint;
    if (size >= sizeof(cmsCIExyY)) {
        memcpy(&whitePoint, data, sizeof(cmsCIExyY));
    } else {
        whitePoint.x = 0.3127;
        whitePoint.y = 0.3290;
        whitePoint.Y = 1.0;
    }
    
    cmsHPROFILE profile = cmsCreateLab2Profile(&whitePoint);
    if (profile) {
        cmsCloseProfile(profile);
    }
}

static void fuzz_cmsD50_xyY(void) {
    const cmsCIExyY* d50 = cmsD50_xyY();
    (void)d50;
}

static void fuzz_cmsCreateRGBProfileTHR(const uint8_t* data, size_t size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return;
    
    cmsCIExyY whitePoint;
    cmsCIExyYTRIPLE primaries;
    
    size_t offset = 0;
    if (size >= sizeof(cmsCIExyY)) {
        memcpy(&whitePoint, data + offset, sizeof(cmsCIExyY));
        offset += sizeof(cmsCIExyY);
    } else {
        whitePoint.x = 0.3127;
        whitePoint.y = 0.3290;
        whitePoint.Y = 1.0;
    }
    
    if (size - offset >= sizeof(cmsCIExyYTRIPLE)) {
        memcpy(&primaries, data + offset, sizeof(cmsCIExyYTRIPLE));
        offset += sizeof(cmsCIExyYTRIPLE);
    } else {
        primaries.Red.x = 0.64;
        primaries.Red.y = 0.33;
        primaries.Red.Y = 1.0;
        primaries.Green.x = 0.30;
        primaries.Green.y = 0.60;
        primaries.Green.Y = 1.0;
        primaries.Blue.x = 0.15;
        primaries.Blue.y = 0.06;
        primaries.Blue.Y = 1.0;
    }
    
    cmsToneCurve* transfer[3] = { NULL, NULL, NULL };
    
    cmsHPROFILE profile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, transfer);
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    cmsDeleteContext(ctx);
}

static void fuzz_cmsxyY2XYZ(const uint8_t* data, size_t size) {
    cmsCIExyY source;
    cmsCIEXYZ dest;
    
    if (size >= sizeof(cmsCIExyY)) {
        memcpy(&source, data, sizeof(cmsCIExyY));
        cmsxyY2XYZ(&dest, &source);
    }
}

static void fuzz_cmsCreateLab4ProfileTHR(const uint8_t* data, size_t size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return;
    
    cmsCIExyY whitePoint;
    if (size >= sizeof(cmsCIExyY)) {
        memcpy(&whitePoint, data, sizeof(cmsCIExyY));
    } else {
        whitePoint.x = 0.3127;
        whitePoint.y = 0.3290;
        whitePoint.Y = 1.0;
    }
    
    cmsHPROFILE profile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    cmsDeleteContext(ctx);
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            fuzz_cmsCreateLab2ProfileTHR(Data + 1, Size - 1);
            break;
        case 1:
            fuzz_cmsCreateLab2Profile(Data + 1, Size - 1);
            break;
        case 2:
            fuzz_cmsD50_xyY();
            break;
        case 3:
            fuzz_cmsCreateRGBProfileTHR(Data + 1, Size - 1);
            break;
        case 4:
            fuzz_cmsxyY2XYZ(Data + 1, Size - 1);
            break;
        case 5:
            fuzz_cmsCreateLab4ProfileTHR(Data + 1, Size - 1);
            break;
    }
    
    return 0;
}