// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsCreateLinearizationDeviceLink at cmsvirt.c:340:23 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsUInt32Number nPoints = (Data[0] % 100) + 1;
    cmsUInt32Number nResultSamples = (Data[1] % 100) + 1;
    cmsColorSpaceSignature colorSpace = (cmsColorSpaceSignature)(Data[2] % 10);

    cmsToneCurve *X = NULL;
    cmsToneCurve *Y = NULL;
    cmsToneCurve *Z = NULL;
    cmsToneCurve *reversed = NULL;
    cmsToneCurve *joined = NULL;
    cmsToneCurve *dupX = NULL;
    cmsToneCurve *dupY = NULL;
    cmsToneCurve *dupZ = NULL;
    cmsToneCurve *dupReversed = NULL;
    cmsToneCurve *dupJoined = NULL;
    cmsHPROFILE profile1 = NULL;
    cmsHPROFILE profile2 = NULL;
    cmsToneCurve *curves[3] = {NULL, NULL, NULL};

    // Create initial tone curves using cmsBuildGamma
    // If data is insufficient, we skip creating that specific curve to avoid null pointer issues later
    // or use a default gamma if possible, but cmsBuildGamma requires a valid gamma.
    // We will use a safe default gamma of 1.0 (identity) if data is missing.
    
    cmsFloat32Number gammaValX = 1.0f;
    cmsFloat32Number gammaValY = 1.0f;
    cmsFloat32Number gammaValZ = 1.0f;

    if (Size > 2) {
        gammaValX = 0.1f + (Data[2] / 255.0f);
        X = cmsBuildGamma(ctx, gammaValX);
    }
    if (Size > 10) {
        gammaValY = 0.1f + (Data[10] / 255.0f);
        Y = cmsBuildGamma(ctx, gammaValY);
    }
    if (Size > 20) {
        gammaValZ = 0.1f + (Data[20] / 255.0f);
        Z = cmsBuildGamma(ctx, gammaValZ);
    }

    // Fallback: if creation failed or data was missing, create identity curves manually via gamma 1.0
    if (!X) X = cmsBuildGamma(ctx, 1.0f);
    if (!Y) Y = cmsBuildGamma(ctx, 1.0f);
    if (!Z) Z = cmsBuildGamma(ctx, 1.0f);

    // Test cmsDupToneCurve
    if (X) dupX = cmsDupToneCurve(X);
    if (Y) dupY = cmsDupToneCurve(Y);
    if (Z) dupZ = cmsDupToneCurve(Z);

    // Test cmsReverseToneCurveEx
    if (X) reversed = cmsReverseToneCurveEx(nResultSamples, X);
    if (reversed) dupReversed = cmsDupToneCurve(reversed);

    // Test cmsJoinToneCurve
    if (X && Y) {
        joined = cmsJoinToneCurve(ctx, X, Y, nPoints);
        if (joined) dupJoined = cmsDupToneCurve(joined);
    }

    // Prepare curves for cmsCreateLinearizationDeviceLink
    // Use duplicates or originals to test various states
    if (dupX) curves[0] = dupX;
    else if (X) curves[0] = X;

    if (dupY) curves[1] = dupY;
    else if (Y) curves[1] = Y;

    if (dupZ) curves[2] = dupZ;
    else if (Z) curves[2] = Z;

    // Test cmsCreateLinearizationDeviceLink
    profile1 = cmsCreateLinearizationDeviceLink(colorSpace, curves);

    // Test cmsCreateLinearizationDeviceLinkTHR
    profile2 = cmsCreateLinearizationDeviceLinkTHR(ctx, colorSpace, curves);

    // Test cmsFreeToneCurveTriple with the curves array
    // We need to be careful not to double free.
    // We will create a temporary array for the triple free that we own.
    cmsToneCurve *tempCurves[3] = {NULL, NULL, NULL};
    
    // Move ownership of duplicates to tempCurves for triple free
    if (dupX) { tempCurves[0] = dupX; dupX = NULL; }
    if (dupY) { tempCurves[1] = dupY; dupY = NULL; }
    if (dupZ) { tempCurves[2] = dupZ; dupZ = NULL; }
    
    // If we have reversed or joined duplicates, we can also free them here
    if (dupReversed) { 
        if (!tempCurves[0]) tempCurves[0] = dupReversed; 
        dupReversed = NULL; 
    }
    if (dupJoined) { 
        if (!tempCurves[1]) tempCurves[1] = dupJoined; 
        dupJoined = NULL; 
    }

    cmsFreeToneCurveTriple(tempCurves);

    // Cleanup remaining individual curves
    if (X) cmsFreeToneCurve(X);
    if (Y) cmsFreeToneCurve(Y);
    if (Z) cmsFreeToneCurve(Z);
    if (reversed) cmsFreeToneCurve(reversed);
    if (joined) cmsFreeToneCurve(joined);
    if (dupX) cmsFreeToneCurve(dupX);
    if (dupY) cmsFreeToneCurve(dupY);
    if (dupZ) cmsFreeToneCurve(dupZ);
    if (dupReversed) cmsFreeToneCurve(dupReversed);
    if (dupJoined) cmsFreeToneCurve(dupJoined);

    // Cleanup profiles
    cleanup_profile(profile1);
    cleanup_profile(profile2);

    // Cleanup context
    cmsDeleteContext(ctx);

    return 0;
}