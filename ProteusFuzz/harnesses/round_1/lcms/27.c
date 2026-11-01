// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_OkLabProfile at cmsvirt.c:690:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsContext create_context_from_fuzz(const uint8_t *Data, size_t Size) {
    if (Size < 1) return NULL;
    return cmsCreateContext(NULL, NULL);
}

static void cleanup_profile(cmsHPROFILE profile, cmsContext ctx) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static cmsToneCurve* create_tone_curve_from_fuzz(const uint8_t *Data, size_t Size, cmsContext ctx) {
    if (Size < 1) return NULL;
    
    // Use Type 1 (Gamma) for parametric curve
    cmsFloat64Number gamma = 1.0 + ((double)(Data[0] & 0xFF) / 255.0);
    if (gamma <= 0.0) gamma = 0.1;
    
    cmsFloat64Number params[10] = {0};
    params[0] = gamma;
    
    // Correct function name: cmsBuildParametricToneCurve
    return cmsBuildParametricToneCurve(ctx, 1, params);
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    cmsContext ctx = create_context_from_fuzz(Data, Size);
    if (ctx == NULL) {
        return 0;
    }

    cmsHPROFILE profile = NULL;
    cmsToneCurve *curves[3] = {NULL, NULL, NULL};
    cmsToneCurve *gray_curve = NULL;
    cmsCIExyY white_point = {0.3127, 0.3290, 1.0}; // D65 default
    cmsCIExyYTRIPLE primaries = {{{0.64, 0.33, 0.0}, {0.30, 0.60, 0.0}, {0.15, 0.06, 0.0}}}; // sRGB default

    // Fuzz parameters from data if available
    if (Size >= 2) {
        white_point.x = 0.0 + ((double)(Data[0]) / 255.0);
        white_point.y = 0.0 + ((double)(Data[1]) / 255.0);
    }
    if (Size >= 5) {
        primaries.Red.x = 0.0 + ((double)(Data[2]) / 255.0);
        primaries.Red.y = 0.0 + ((double)(Data[3]) / 255.0);
        primaries.Green.x = 0.0 + ((double)(Data[4]) / 255.0);
    }
    if (Size >= 7) {
        primaries.Green.y = 0.0 + ((double)(Data[5]) / 255.0);
        primaries.Blue.x = 0.0 + ((double)(Data[6]) / 255.0);
    }
    if (Size >= 8) {
        primaries.Blue.y = 0.0 + ((double)(Data[7]) / 255.0);
    }

    // 1. cmsCreate_sRGBProfileTHR
    profile = cmsCreate_sRGBProfileTHR(ctx);
    if (profile != NULL) {
        cmsCloseProfile(profile);
        profile = NULL;
    }

    // 2. cmsCreate_OkLabProfile
    profile = cmsCreate_OkLabProfile(ctx);
    if (profile != NULL) {
        cmsCloseProfile(profile);
        profile = NULL;
    }

    // 3. cmsCreateLab2ProfileTHR (with white point)
    profile = cmsCreateLab2ProfileTHR(ctx, &white_point);
    if (profile != NULL) {
        cmsCloseProfile(profile);
        profile = NULL;
    }
    // 3b. cmsCreateLab2ProfileTHR (with NULL white point)
    profile = cmsCreateLab2ProfileTHR(ctx, NULL);
    if (profile != NULL) {
        cmsCloseProfile(profile);
        profile = NULL;
    }

    // 4. cmsCreateLab4ProfileTHR (with white point)
    profile = cmsCreateLab4ProfileTHR(ctx, &white_point);
    if (profile != NULL) {
        cmsCloseProfile(profile);
        profile = NULL;
    }
    // 4b. cmsCreateLab4ProfileTHR (with NULL white point)
    profile = cmsCreateLab4ProfileTHR(ctx, NULL);
    if (profile != NULL) {
        cmsCloseProfile(profile);
        profile = NULL;
    }

    // 5. cmsCreateGrayProfileTHR
    if (Size > 0) {
        gray_curve = create_tone_curve_from_fuzz(Data, Size, ctx);
        if (gray_curve != NULL) {
            profile = cmsCreateGrayProfileTHR(ctx, &white_point, gray_curve);
            if (profile != NULL) {
                cmsCloseProfile(profile);
                profile = NULL;
            }
            cmsFreeToneCurve(gray_curve);
            gray_curve = NULL;
        }
    }

    // 6. cmsCreateRGBProfileTHR
    if (Size > 0) {
        curves[0] = create_tone_curve_from_fuzz(Data, Size, ctx);
        curves[1] = create_tone_curve_from_fuzz(Data, Size, ctx);
        curves[2] = create_tone_curve_from_fuzz(Data, Size, ctx);
        
        if (curves[0] && curves[1] && curves[2]) {
            profile = cmsCreateRGBProfileTHR(ctx, &white_point, &primaries, curves);
            if (profile != NULL) {
                cmsCloseProfile(profile);
                profile = NULL;
            }
        }
        
        if (curves[0]) cmsFreeToneCurve(curves[0]);
        if (curves[1]) cmsFreeToneCurve(curves[1]);
        if (curves[2]) cmsFreeToneCurve(curves[2]);
    }

    cleanup_profile(profile, ctx);
    return 0;
}