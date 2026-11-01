// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsCreateLinearizationDeviceLink at cmsvirt.c:340:23 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsToneCurve* create_gamma_curve_from_fuzz(const uint8_t *data, size_t size, cmsContext ctx, cmsUInt32Number *out_samples) {
    if (size < 2) return NULL;
    
    cmsUInt32Number samples = (data[0] % 256) + 1;
    if (samples == 0) samples = 1;
    if (samples > 10000) samples = 10000;
    
    cmsFloat32Number gamma = (data[1] % 256) / 100.0f + 0.1f;
    if (gamma < 0.1f) gamma = 0.1f;
    if (gamma > 5.0f) gamma = 5.0f;
    
    cmsFloat64Number params[10] = {0};
    params[0] = (cmsFloat64Number)gamma;
    
    cmsToneCurve* curve = cmsBuildParametricToneCurve(ctx, 1, params);
    
    if (curve) {
        *out_samples = samples;
    }
    return curve;
}

static cmsToneCurve* create_parametric_curve_from_fuzz(const uint8_t *data, size_t size, cmsContext ctx) {
    if (size < 4) return NULL;
    
    cmsInt32Number type = (data[0] % 6) + 1;
    cmsFloat64Number params[10] = {0};
    
    for (int i = 0; i < 4 && (size > 4 + i); i++) {
        params[i] = (data[4 + i] % 256) / 100.0;
    }
    
    return cmsBuildParametricToneCurve(ctx, type, params);
}

int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        ctx = NULL;
    }

    cmsToneCurve *curves[3] = {NULL, NULL, NULL};
    cmsToneCurve *dup_curves[3] = {NULL, NULL, NULL};
    cmsToneCurve *rev_curves[3] = {NULL, NULL, NULL};
    cmsToneCurve *joined_curves[3] = {NULL, NULL, NULL};
    cmsHPROFILE profile1 = NULL;
    cmsHPROFILE profile2 = NULL;
    cmsColorSpaceSignature colorSpace = cmsSigRgbData;
    
    if (Size > 1) {
        int cs_val = Data[0] % 3;
        switch (cs_val) {
            case 0: colorSpace = cmsSigRgbData; break;
            case 1: colorSpace = cmsSigGrayData; break;
            case 2: colorSpace = cmsSigCmykData; break;
        }
    }

    cmsUInt32Number samples = 0;
    
    // Create initial curves
    for (int i = 0; i < 3; i++) {
        size_t offset = 2 + (i * 4);
        if (offset + 4 > Size) offset = 2;
        
        cmsToneCurve *temp = create_gamma_curve_from_fuzz(Data + offset, Size - offset, ctx, &samples);
        if (temp) {
            curves[i] = temp;
        } else {
            temp = create_parametric_curve_from_fuzz(Data + offset, Size - offset, ctx);
            if (temp) {
                curves[i] = temp;
            }
        }
    }

    // Test cmsDupToneCurve
    for (int i = 0; i < 3; i++) {
        if (curves[i]) {
            dup_curves[i] = cmsDupToneCurve(curves[i]);
        }
    }

    // Test cmsReverseToneCurveEx
    cmsUInt32Number rev_samples = (Size > 5) ? (Data[5] % 256) + 1 : 100;
    if (rev_samples == 0) rev_samples = 1;
    if (rev_samples > 10000) rev_samples = 10000;

    for (int i = 0; i < 3; i++) {
        if (curves[i]) {
            rev_curves[i] = cmsReverseToneCurveEx(rev_samples, curves[i]);
        }
    }

    // Test cmsJoinToneCurve
    cmsUInt32Number join_points = (Size > 6) ? (Data[6] % 256) + 1 : 100;
    if (join_points == 0) join_points = 1;
    if (join_points > 10000) join_points = 10000;

    for (int i = 0; i < 3; i++) {
        cmsToneCurve *X = (i < 3 && curves[i]) ? curves[i] : NULL;
        cmsToneCurve *Y = (i < 3 && rev_curves[i]) ? rev_curves[i] : NULL;
        
        if (X && Y) {
            joined_curves[i] = cmsJoinToneCurve(ctx, X, Y, join_points);
        }
    }

    // Prepare transfer functions array for device link
    cmsToneCurve* TransferFunctions[3] = {NULL, NULL, NULL};
    
    int source_type = (Size > 7) ? (Data[7] % 4) : 0;
    
    for (int i = 0; i < 3; i++) {
        if (source_type == 0 && curves[i]) {
            TransferFunctions[i] = curves[i];
        } else if (source_type == 1 && dup_curves[i]) {
            TransferFunctions[i] = dup_curves[i];
        } else if (source_type == 2 && rev_curves[i]) {
            TransferFunctions[i] = rev_curves[i];
        } else if (source_type == 3 && joined_curves[i]) {
            TransferFunctions[i] = joined_curves[i];
        } else {
            TransferFunctions[i] = NULL;
        }
    }

    // Determine number of channels based on color space to ensure array bounds safety
    int nChannels = 3;
    if (colorSpace == cmsSigGrayData) nChannels = 1;
    if (colorSpace == cmsSigCmykData) nChannels = 4;

    // Ensure we have valid curves for the required number of channels before calling device link
    // If any required channel is NULL, skip the device link creation to prevent internal array out-of-bounds
    int valid_for_link = 1;
    for (int i = 0; i < nChannels; i++) {
        if (TransferFunctions[i] == NULL) {
            valid_for_link = 0;
            break;
        }
    }

    if (valid_for_link) {
        profile1 = cmsCreateLinearizationDeviceLink(colorSpace, TransferFunctions);
        profile2 = cmsCreateLinearizationDeviceLinkTHR(ctx, colorSpace, TransferFunctions);
    }

    if (profile1) cmsCloseProfile(profile1);
    if (profile2) cmsCloseProfile(profile2);

    for (int i = 0; i < 3; i++) {
        if (joined_curves[i]) cmsFreeToneCurve(joined_curves[i]);
    }

    for (int i = 0; i < 3; i++) {
        if (rev_curves[i]) cmsFreeToneCurve(rev_curves[i]);
    }

    for (int i = 0; i < 3; i++) {
        if (dup_curves[i]) cmsFreeToneCurve(dup_curves[i]);
    }

    cmsFreeToneCurveTriple(curves);

    if (ctx) cmsDeleteContext(ctx);

    return 0;
}