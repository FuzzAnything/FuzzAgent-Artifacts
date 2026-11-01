// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
// cmsCreateLinearizationDeviceLink at cmsvirt.c:340:23 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void cleanup_tone_curves(cmsToneCurve **curves, int count) {
    for (int i = 0; i < count; i++) {
        if (curves[i] != NULL) {
            cmsFreeToneCurve(curves[i]);
            curves[i] = NULL;
        }
    }
}

static void cleanup_profiles(cmsHPROFILE **profiles, int count) {
    for (int i = 0; i < count; i++) {
        if (profiles[i] != NULL) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    // Extract parameters from input data safely
    cmsColorSpaceSignature ColorSpace = (cmsColorSpaceSignature)(Data[0]);
    cmsUInt32Number nPoints = ((cmsUInt32Number)Data[1] << 24) | 
                              ((cmsUInt32Number)Data[2] << 16) | 
                              ((cmsUInt32Number)Data[3] << 8) | 
                              ((cmsUInt32Number)Data[4]);
    
    if (nPoints == 0) nPoints = 256; // Default value to avoid invalid input

    // Allocate arrays for tone curves
    cmsToneCurve *TransferFunctions[3] = {NULL, NULL, NULL};
    cmsToneCurve *SrcCurve = NULL;
    cmsToneCurve *ReversedCurve = NULL;
    cmsToneCurve *JoinedCurve = NULL;
    cmsHPROFILE Profile1 = NULL;
    cmsHPROFILE Profile2 = NULL;

    // Create initial tone curves if data is sufficient
    if (Size > 5) {
        cmsFloat64Number gamma = (cmsFloat64Number)(Data[5] / 255.0) + 0.5;
        SrcCurve = cmsBuildGamma(ContextID, gamma);
        if (SrcCurve != NULL) {
            TransferFunctions[0] = cmsDupToneCurve(SrcCurve);
            TransferFunctions[1] = cmsDupToneCurve(SrcCurve);
            TransferFunctions[2] = cmsDupToneCurve(SrcCurve);
        }
    }

    // Test cmsDupToneCurve
    if (SrcCurve != NULL) {
        cmsToneCurve *DuplicatedCurve = cmsDupToneCurve(SrcCurve);
        if (DuplicatedCurve != NULL) {
            cmsFreeToneCurve(DuplicatedCurve);
        }
    }

    // Test cmsReverseToneCurveEx
    if (SrcCurve != NULL) {
        ReversedCurve = cmsReverseToneCurveEx(nPoints, SrcCurve);
    }

    // Test cmsJoinToneCurve
    if (SrcCurve != NULL && ReversedCurve != NULL) {
        JoinedCurve = cmsJoinToneCurve(ContextID, SrcCurve, ReversedCurve, nPoints);
    }

    // Test cmsCreateLinearizationDeviceLinkTHR
    Profile1 = cmsCreateLinearizationDeviceLinkTHR(ContextID, ColorSpace, TransferFunctions);

    // Test cmsCreateLinearizationDeviceLink
    Profile2 = cmsCreateLinearizationDeviceLink(ColorSpace, TransferFunctions);

    // Test cmsFreeToneCurveTriple
    cmsFreeToneCurveTriple(TransferFunctions);

    // Cleanup allocated resources
    if (SrcCurve != NULL) cmsFreeToneCurve(SrcCurve);
    if (ReversedCurve != NULL) cmsFreeToneCurve(ReversedCurve);
    if (JoinedCurve != NULL) cmsFreeToneCurve(JoinedCurve);
    cleanup_profiles(&Profile1, 1);
    cleanup_profiles(&Profile2, 1);
    cmsDeleteContext(ContextID);

    return 0;
}