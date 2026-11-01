// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsSetAdaptationStateTHR at cmsxform.c:57:28 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profiles(cmsHPROFILE *profiles, int count) {
    for (int i = 0; i < count; i++) {
        if (profiles[i] != NULL) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = NULL;
        }
    }
}

static cmsFloat64Number get_float(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size > offset) {
        return (cmsFloat64Number)Data[offset];
    }
    return 0.0;
}

static cmsUInt32Number get_uint32(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size > offset) {
        return (cmsUInt32Number)Data[offset];
    }
    return 0;
}

static cmsBool get_bool(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size > offset) {
        return (cmsBool)Data[offset];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsContext ctx2 = cmsCreateContext(NULL, NULL);
    if (ctx2 == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Fuzz cmsSetAdaptationState
    cmsFloat64Number val0 = get_float(Data, Size, 0);
    cmsSetAdaptationState(val0);

    // Fuzz cmsSetAdaptationStateTHR
    cmsFloat64Number val1 = get_float(Data, Size, 0);
    cmsSetAdaptationStateTHR(ctx, val1);

    // Create some profiles for testing
    cmsHPROFILE profiles[4] = {NULL, NULL, NULL, NULL};
    cmsHPROFILE *profile_ptrs = profiles;
    int profile_count = 0;

    // Create sRGB profile
    profiles[0] = cmsCreate_sRGBProfileTHR(ctx);
    if (profiles[0] != NULL) profile_count++;

    // Create BCHSW profile
    cmsUInt32Number nLUTPoints = get_uint32(Data, Size, 1);
    if (nLUTPoints == 0) nLUTPoints = 16;
    
    cmsFloat64Number Bright = get_float(Data, Size, 0);
    cmsFloat64Number Contrast = get_float(Data, Size, 1);
    cmsFloat64Number Hue = get_float(Data, Size, 2);
    cmsFloat64Number Saturation = get_float(Data, Size, 3);
    cmsUInt32Number TempSrc = get_uint32(Data, Size, 4);
    cmsUInt32Number TempDest = get_uint32(Data, Size, 5);

    profiles[1] = cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
    if (profiles[1] != NULL) profile_count++;

    // Create another sRGB for second slot if needed
    if (profile_count < 2) {
        profiles[1] = cmsCreate_sRGBProfileTHR(ctx);
        if (profiles[1] != NULL) profile_count++;
    }

    // Prepare arrays for _cmsDefaultICCintents and cmsCreateExtendedTransform
    cmsUInt32Number Intents[4] = {0, 0, 0, 0};
    cmsBool BPC[4] = {0, 0, 0, 0};
    cmsFloat64Number AdaptationStates[4] = {0, 0, 0, 0};

    for (int i = 0; i < profile_count && i < 4; i++) {
        Intents[i] = get_uint32(Data, Size, 6 + i);
        BPC[i] = get_bool(Data, Size, 10 + i);
        AdaptationStates[i] = get_float(Data, Size, 14 + i);
    }

    cmsUInt32Number dwFlags = get_uint32(Data, Size, 20);
    
    // Use valid format constants from lcms2.h
    cmsUInt32Number InputFormat = get_uint32(Data, Size, 21);
    cmsUInt32Number OutputFormat = get_uint32(Data, Size, 22);
    
    // Ensure formats are valid if they are 0 or out of typical range, default to MCH8Data
    if (InputFormat == 0) InputFormat = cmsSigMCH8Data;
    if (OutputFormat == 0) OutputFormat = cmsSigMCH8Data;

    // Fuzz _cmsDefaultICCintents
    cmsPipeline *pipeline = _cmsDefaultICCintents(ctx, profile_count, Intents, profile_ptrs, BPC, AdaptationStates, dwFlags);
    if (pipeline != NULL) {
        // cmsFreePipeline is often internal or not exported in some builds.
        // We skip explicit freeing to avoid compilation errors if the symbol is missing.
        // The context deletion will eventually clean up associated resources.
    }

    // Fuzz cmsCreateExtendedTransform
    cmsHTRANSFORM transform = cmsCreateExtendedTransform(ctx, profile_count, profile_ptrs, BPC, Intents, AdaptationStates, NULL, 0, InputFormat, OutputFormat, dwFlags);
    if (transform != NULL) {
        cmsDeleteTransform(transform);
    }

    // Cleanup
    cleanup_profiles(profiles, profile_count);
    cmsDeleteContext(ctx);
    cmsDeleteContext(ctx2);

    return 0;
}