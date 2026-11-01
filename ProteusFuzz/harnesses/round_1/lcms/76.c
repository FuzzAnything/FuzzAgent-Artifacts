// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSetAdaptationStateTHR at cmsxform.c:57:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsContext createContext(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;
    
    // Context is created, ready for use
    return ctx;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare the environment
    cmsContext ctx = createContext(Data, Size);
    if (!ctx) {
        // Fallback to global context if creation fails
        ctx = NULL;
    }

    // Extract values from input data to drive fuzzing
    int idx = 0;
    cmsFloat64Number d_val = 0.0;
    cmsUInt32Number nProfiles = 1;
    cmsUInt32Number InputFormat = cmsSigRgbData;
    cmsUInt32Number OutputFormat = cmsSigRgbData;
    cmsUInt32Number Intent = 0; // Default to Perceptual (0)
    cmsUInt32Number dwFlags = 0;
    cmsUInt32Number nLUTPoints = 10;
    cmsFloat64Number Bright = 0.0;
    cmsFloat64Number Contrast = 1.0;
    cmsFloat64Number Hue = 0.0;
    cmsFloat64Number Saturation = 1.0;
    cmsUInt32Number TempSrc = 6500;
    cmsUInt32Number TempDest = 6500;

    if (Size > 0) d_val = (cmsFloat64Number)(Data[idx++]);
    if (Size > 1) nProfiles = Data[idx++];
    if (Size > 2) InputFormat = Data[idx++];
    if (Size > 3) OutputFormat = Data[idx++];
    if (Size > 4) Intent = Data[idx++];
    if (Size > 5) dwFlags = Data[idx++];
    if (Size > 6) nLUTPoints = Data[idx++];
    if (Size > 7) Bright = (cmsFloat64Number)(Data[idx++]);
    if (Size > 8) Contrast = (cmsFloat64Number)(Data[idx++]);
    if (Size > 9) Hue = (cmsFloat64Number)(Data[idx++]);
    if (Size > 10) Saturation = (cmsFloat64Number)(Data[idx++]);
    if (Size > 12) {
        TempSrc = Data[idx++] | (Data[idx++] << 8);
    }
    if (Size > 14) {
        TempDest = Data[idx++] | (Data[idx++] << 8);
    }

    // Clamp nProfiles to valid range for array allocation
    if (nProfiles > 255) nProfiles = 255;
    if (nProfiles == 0) nProfiles = 1;

    // Allocate profile array
    cmsHPROFILE *hProfiles = NULL;
    cmsBool *BPC = NULL;
    cmsUInt32Number *Intents = NULL;
    cmsFloat64Number *AdaptationStates = NULL;

    if (nProfiles > 0) {
        hProfiles = (cmsHPROFILE *)malloc(nProfiles * sizeof(cmsHPROFILE));
        BPC = (cmsBool *)malloc(nProfiles * sizeof(cmsBool));
        Intents = (cmsUInt32Number *)malloc(nProfiles * sizeof(cmsUInt32Number));
        AdaptationStates = (cmsFloat64Number *)malloc(nProfiles * sizeof(cmsFloat64Number));
    }

    if (!hProfiles || !BPC || !Intents || !AdaptationStates) {
        free(hProfiles); free(BPC); free(Intents); free(AdaptationStates);
        if (ctx) cmsDeleteContext(ctx);
        return 0;
    }

    // Initialize arrays with dummy profiles or NULL
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    
    for (uint32_t i = 0; i < nProfiles; i++) {
        if (hProfile) {
            hProfiles[i] = hProfile;
        } else {
            hProfiles[i] = NULL;
        }
        // Use 1 for true and 0 for false as cmsBool is int
        BPC[i] = (i % 2) ? 1 : 0;
        Intents[i] = (i % 4); // 0, 1, 2, 3
        AdaptationStates[i] = (i % 2) ? 1.0 : 0.0;
    }

    // 2. Invoke target functions diversely

    // 1. cmsSetAdaptationStateTHR
    if (ctx) {
        cmsFloat64Number prev = cmsSetAdaptationStateTHR(ctx, d_val);
        (void)prev; // Use return value to suppress warnings
    }

    // 2. cmsCreateMultiprofileTransformTHR
    cmsHTRANSFORM hTransform = NULL;
    if (ctx && hProfiles[0]) {
        hTransform = cmsCreateMultiprofileTransformTHR(ctx, hProfiles, nProfiles, 
                                                       InputFormat, OutputFormat, 
                                                       Intent, dwFlags);
    }

    // 3. cmsCreateBCHSWabstractProfileTHR
    cmsHPROFILE hBCHSW = NULL;
    if (ctx) {
        hBCHSW = cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints, Bright, Contrast, 
                                                  Hue, Saturation, TempSrc, TempDest);
    }

    // 4. cmsSetAdaptationState (Global)
    cmsFloat64Number globalPrev = cmsSetAdaptationState(d_val);
    (void)globalPrev;

    // 5. _cmsDefaultICCintents
    cmsPipeline *pipeline = NULL;
    if (ctx && hProfiles[0]) {
        pipeline = _cmsDefaultICCintents(ctx, nProfiles, Intents, hProfiles, 
                                         BPC, AdaptationStates, dwFlags);
    }

    // 6. cmsCreateExtendedTransform
    cmsHTRANSFORM hExtTransform = NULL;
    if (ctx && hProfiles[0]) {
        hExtTransform = cmsCreateExtendedTransform(ctx, nProfiles, hProfiles, BPC, Intents, 
                                                   AdaptationStates, NULL, 0, 
                                                   InputFormat, OutputFormat, dwFlags);
    }

    // 3. Cleanup
    if (hTransform) cmsDeleteTransform(hTransform);
    if (hExtTransform) cmsDeleteTransform(hExtTransform);
    if (hBCHSW) cmsCloseProfile(hBCHSW);
    if (hProfile) cmsCloseProfile(hProfile);
    // pipeline is internal, usually managed by context or transform, no public free
    
    free(hProfiles);
    free(BPC);
    free(Intents);
    free(AdaptationStates);

    if (ctx) cmsDeleteContext(ctx);

    return 0;
}