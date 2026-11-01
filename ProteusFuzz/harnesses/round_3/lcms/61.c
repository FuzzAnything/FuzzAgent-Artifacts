// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
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

static void cleanup_transform(cmsHTRANSFORM hTransform) {
    if (hTransform != NULL) {
        cmsDeleteTransform(hTransform);
    }
}

// Helper to safely read a value from the buffer
static int safe_read_u32(const uint8_t *Data, size_t Size, size_t offset, cmsUInt32Number *out) {
    if (offset + sizeof(cmsUInt32Number) > Size) return 0;
    memcpy(out, Data + offset, sizeof(cmsUInt32Number));
    return 1;
}

static int safe_read_f64(const uint8_t *Data, size_t Size, size_t offset, cmsFloat64Number *out) {
    if (offset + sizeof(cmsFloat64Number) > Size) return 0;
    memcpy(out, Data + offset, sizeof(cmsFloat64Number));
    return 1;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsUInt32Number nLUTPoints = 0;
    cmsFloat64Number Bright = 0.0;
    cmsFloat64Number Contrast = 0.0;
    cmsFloat64Number Hue = 0.0;
    cmsFloat64Number Saturation = 0.0;
    cmsUInt32Number TempSrc = 0;
    cmsUInt32Number TempDest = 0;

    size_t offset = 0;

    if (safe_read_u32(Data, Size, offset, &nLUTPoints)) offset += 4;
    if (safe_read_f64(Data, Size, offset, &Bright)) offset += 8;
    if (safe_read_f64(Data, Size, offset, &Contrast)) offset += 8;
    if (safe_read_f64(Data, Size, offset, &Hue)) offset += 8;
    if (safe_read_f64(Data, Size, offset, &Saturation)) offset += 8;
    if (safe_read_u32(Data, Size, offset, &TempSrc)) offset += 4;
    if (safe_read_u32(Data, Size, offset, &TempDest)) offset += 4;

    // Ensure nLUTPoints is within reasonable bounds to avoid internal allocation errors
    if (nLUTPoints == 0) nLUTPoints = 1;
    if (nLUTPoints > 10000) nLUTPoints = 10000;

    cmsHPROFILE hProfile1 = NULL;
    cmsHPROFILE hProfile2 = NULL;
    cmsHTRANSFORM hTransform = NULL;
    cmsHTRANSFORM hTransformMulti = NULL;
    cmsHTRANSFORM hTransformExtended = NULL;

    // Test cmsCreateBCHSWabstractProfile
    hProfile1 = cmsCreateBCHSWabstractProfile(nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
    cleanup_profile(hProfile1);

    // Test cmsCreateBCHSWabstractProfileTHR
    hProfile1 = cmsCreateBCHSWabstractProfileTHR(ContextID, nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
    // Do NOT cleanup here yet, we need it for transform tests

    // Test cmsOpenProfileFromMemTHR with remaining data as dummy profile
    if (Size > offset) {
        size_t memSize = Size - offset;
        cmsHPROFILE hProfileMem = cmsOpenProfileFromMemTHR(ContextID, Data + offset, memSize);
        cleanup_profile(hProfileMem);
    }

    // Create a second profile for transform tests
    hProfile2 = cmsCreateBCHSWabstractProfileTHR(ContextID, nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);

    // Test cmsCreateTransformTHR
    if (hProfile1 != NULL && hProfile2 != NULL) {
        hTransform = cmsCreateTransformTHR(ContextID, hProfile1, 0, hProfile2, 0, 0, 0);
        cleanup_transform(hTransform);
    }

    if (hProfile1 != NULL) {
        hTransform = cmsCreateTransformTHR(ContextID, hProfile1, 0, NULL, 0, 0, 0);
        cleanup_transform(hTransform);
    }

    // Test cmsCreateMultiprofileTransformTHR
    cmsHPROFILE hProfiles[2] = {NULL, NULL};
    if (hProfile1 != NULL) {
        hProfiles[0] = hProfile1;
        if (hProfile2 != NULL) {
            hProfiles[1] = hProfile2;
            hTransformMulti = cmsCreateMultiprofileTransformTHR(ContextID, hProfiles, 2, 0, 0, 0, 0);
            cleanup_transform(hTransformMulti);
        }
        hTransformMulti = cmsCreateMultiprofileTransformTHR(ContextID, hProfiles, 1, 0, 0, 0, 0);
        cleanup_transform(hTransformMulti);
    }

    // Test cmsCreateExtendedTransform
    if (hProfile1 != NULL) {
        cmsHPROFILE hProfilesExt[1] = {hProfile1};
        cmsBool BPC[1] = {0};
        cmsUInt32Number Intents[1] = {0};
        cmsFloat64Number AdaptationStates[1] = {0.0};
        
        hTransformExtended = cmsCreateExtendedTransform(
            ContextID, 1, hProfilesExt, BPC, Intents, AdaptationStates,
            NULL, 0, 0, 0, 0
        );
        cleanup_transform(hTransformExtended);

        // Test with NULL transform flag
        hTransformExtended = cmsCreateExtendedTransform(
            ContextID, 1, hProfilesExt, BPC, Intents, AdaptationStates,
            NULL, 0, 0, 0, cmsFLAGS_NULLTRANSFORM
        );
        cleanup_transform(hTransformExtended);
    }

    // Cleanup profiles only after all transform tests are done
    cleanup_profile(hProfile1);
    cleanup_profile(hProfile2);
    cmsDeleteContext(ContextID);

    return 0;
}