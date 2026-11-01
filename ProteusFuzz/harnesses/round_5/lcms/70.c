// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profiles(cmsHPROFILE p1, cmsHPROFILE p2, cmsHPROFILE p3, cmsHPROFILE p4) {
    if (p1) cmsCloseProfile(p1);
    if (p2) cmsCloseProfile(p2);
    if (p3) cmsCloseProfile(p3);
    if (p4) cmsCloseProfile(p4);
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    const uint8_t *ptr = Data;
    size_t offset = 0;

    // Create WhitePoint (cmsCIExyY)
    cmsCIExyY wp;
    if (Size >= 12) {
        wp.x = (cmsFloat64Number)(ptr[0] / 255.0);
        wp.y = (cmsFloat64Number)(ptr[1] / 255.0);
        wp.Y = (cmsFloat64Number)(ptr[2] / 255.0);
        offset += 12;
    } else {
        wp.x = 0.3127; wp.y = 0.3290; wp.Y = 1.0;
    }

    // Create Primaries (cmsCIExyYTRIPLE)
    cmsCIExyYTRIPLE prim;
    memset(&prim, 0, sizeof(prim));
    if (Size >= offset + 36) {
        prim.Red.x = (cmsFloat64Number)(ptr[offset + 0] / 255.0);
        prim.Red.y = (cmsFloat64Number)(ptr[offset + 1] / 255.0);
        prim.Red.Y = (cmsFloat64Number)(ptr[offset + 2] / 255.0);
        prim.Green.x = (cmsFloat64Number)(ptr[offset + 3] / 255.0);
        prim.Green.y = (cmsFloat64Number)(ptr[offset + 4] / 255.0);
        prim.Green.Y = (cmsFloat64Number)(ptr[offset + 5] / 255.0);
        prim.Blue.x = (cmsFloat64Number)(ptr[offset + 6] / 255.0);
        prim.Blue.y = (cmsFloat64Number)(ptr[offset + 7] / 255.0);
        prim.Blue.Y = (cmsFloat64Number)(ptr[offset + 8] / 255.0);
        offset += 36;
    } else {
        prim.Red.x = 0.64; prim.Red.y = 0.33; prim.Red.Y = 1.0;
        prim.Green.x = 0.30; prim.Green.y = 0.60; prim.Green.Y = 1.0;
        prim.Blue.x = 0.15; prim.Blue.y = 0.06; prim.Blue.Y = 1.0;
    }

    // Create Tone Curves using cmsBuildGamma
    cmsToneCurve *curves[3] = {NULL, NULL, NULL};
    if (Size > offset) {
        cmsFloat32Number gamma = (cmsFloat32Number)(ptr[offset] / 255.0) + 0.1;
        curves[0] = cmsBuildGamma(ctx, gamma);
        curves[1] = cmsBuildGamma(ctx, gamma);
        curves[2] = cmsBuildGamma(ctx, gamma);
        offset++;
    }

    cmsHPROFILE hRGB = NULL;
    cmsHPROFILE hLab = NULL;
    cmsHPROFILE hNull = NULL;
    cmsHPROFILE hGray = NULL;

    // 1. cmsCreateRGBProfileTHR
    if (curves[0] && curves[1] && curves[2]) {
        hRGB = cmsCreateRGBProfileTHR(ctx, &wp, &prim, curves);
    }

    // 2. cmsCreateLab4ProfileTHR
    hLab = cmsCreateLab4ProfileTHR(ctx, &wp);

    // 3. cmsCreateNULLProfileTHR
    hNull = cmsCreateNULLProfileTHR(ctx);

    // 4. cmsCreateGrayProfile
    if (curves[0]) {
        hGray = cmsCreateGrayProfile(&wp, curves[0]);
    }

    // 5. cmsSetHeaderProfileID and cmsGetHeaderProfileID
    cmsUInt8Number idBuf[16];
    cmsUInt8Number outBuf[16];

    // Prepare ID buffer from input safely
    memset(idBuf, 0, 16);
    size_t id_len = (Size >= 16) ? 16 : Size;
    if (id_len > 0) {
        memcpy(idBuf, Data, id_len);
    }

    // Test Set/Get on created profiles
    if (hRGB) {
        cmsSetHeaderProfileID(hRGB, idBuf);
        cmsGetHeaderProfileID(hRGB, outBuf);
    }
    if (hLab) {
        cmsSetHeaderProfileID(hLab, idBuf);
        cmsGetHeaderProfileID(hLab, outBuf);
    }
    if (hNull) {
        cmsSetHeaderProfileID(hNull, idBuf);
        cmsGetHeaderProfileID(hNull, outBuf);
    }
    if (hGray) {
        cmsSetHeaderProfileID(hGray, idBuf);
        cmsGetHeaderProfileID(hGray, outBuf);
    }

    // Cleanup
    if (curves[0]) cmsFreeToneCurve(curves[0]);
    if (curves[1]) cmsFreeToneCurve(curves[1]);
    if (curves[2]) cmsFreeToneCurve(curves[2]);

    cleanup_profiles(hRGB, hLab, hNull, hGray);

    if (ctx) cmsDeleteContext(ctx);

    return 0;
}