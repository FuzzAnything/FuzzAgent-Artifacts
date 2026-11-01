// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsCreateXYZProfile at cmsvirt.c:617:23 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_profiles(cmsHPROFILE *profiles, int count) {
    for (int i = 0; i < count; i++) {
        if (profiles[i] != NULL) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = NULL;
        }
    }
}

static void cleanup_transforms(cmsHTRANSFORM *transforms, int count) {
    for (int i = 0; i < count; i++) {
        if (transforms[i] != NULL) {
            cmsDeleteTransform(transforms[i]);
            transforms[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Initialize context
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    // Extract parameters from fuzz data
    uint32_t nLUTPoints = Data[0] + 1; // Avoid 0
    double Bright = (double)(Data[1] - 128) / 128.0;
    double Contrast = (double)(Data[2 % Size] - 128) / 128.0;
    double Hue = (double)(Data[3 % Size] - 128) / 128.0;
    double Saturation = (double)(Data[4 % Size] - 128) / 128.0;
    uint32_t TempSrc = Data[5 % Size] * 100 + 2000;
    uint32_t TempDest = Data[6 % Size] * 100 + 2000;
    uint32_t Intent = Data[7 % Size] % 4;
    uint32_t dwFlags = Data[8 % Size] * 256 + (Data[9 % Size] % 4);
    uint32_t InputFormat = Data[10 % Size] % 10;
    uint32_t OutputFormat = Data[11 % Size] % 10;

    // Test cmsSetAdaptationState
    cmsFloat64Number adaptationState = (double)(Data[12 % Size] - 128) / 128.0;
    cmsSetAdaptationState(adaptationState);

    // Test cmsCreateXYZProfile
    cmsHPROFILE hXYZ = cmsCreateXYZProfile();
    // We do not create a second XYZ profile to avoid duplicate ownership issues
    // cmsHPROFILE hXYZTHR = cmsCreateXYZProfile(); 

    // Test cmsCreateBCHSWabstractProfile
    cmsHPROFILE hBCHSW = cmsCreateBCHSWabstractProfile(nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);

    // Test cmsCreateBCHSWabstractProfileTHR
    cmsHPROFILE hBCHSWTHR = cmsCreateBCHSWabstractProfileTHR(ContextID, nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);

    // Test cmsCreateTransformTHR with single profile
    cmsHTRANSFORM hTransform1 = NULL;
    if (hXYZ != NULL) {
        hTransform1 = cmsCreateTransformTHR(ContextID, hXYZ, InputFormat, NULL, OutputFormat, Intent, dwFlags);
    }

    // Test cmsCreateTransformTHR with two profiles
    cmsHTRANSFORM hTransform2 = NULL;
    if (hXYZ != NULL && hBCHSW != NULL) {
        hTransform2 = cmsCreateTransformTHR(ContextID, hXYZ, InputFormat, hBCHSW, OutputFormat, Intent, dwFlags);
    }

    // Test cmsCreateExtendedTransform
    cmsHPROFILE profiles[3];
    cmsBool bpc[3];
    cmsUInt32Number intents[3];
    cmsFloat64Number adaptationStates[3];
    int valid_profile_count = 0;

    // Initialize arrays safely
    for(int i=0; i<3; i++) {
        profiles[i] = NULL;
        bpc[i] = 0;
        intents[i] = 0;
        adaptationStates[i] = 0.0;
    }

    if (hXYZ != NULL) profiles[valid_profile_count++] = hXYZ;
    if (hBCHSW != NULL) profiles[valid_profile_count++] = hBCHSW;
    if (hBCHSWTHR != NULL) profiles[valid_profile_count++] = hBCHSWTHR;

    // Fill remaining slots with NULL if needed, but we only pass valid count
    // However, the API expects an array of nProfiles. If we pass NULLs, it might fail gracefully or crash.
    // Let's only pass non-NULL profiles to be safe, or ensure nProfiles matches valid count.
    // Actually, cmsCreateExtendedTransform expects exactly nProfiles entries. 
    // If we pass a count of 3 but some are NULL, it might be invalid. 
    // Let's adjust nProfiles to the number of valid profiles we have.
    
    int nProfiles = valid_profile_count;
    
    // Re-assign for clarity
    cmsHPROFILE *p_arr = profiles;
    cmsBool *bpc_arr = bpc;
    cmsUInt32Number *intents_arr = intents;
    cmsFloat64Number *adapt_arr = adaptationStates;

    // Fill the arrays used for the count we are passing
    for(int i=0; i<nProfiles; i++) {
        bpc_arr[i] = (Data[13 + i % Size] % 2) ? 1 : 0;
        intents_arr[i] = Data[16 + i % Size] % 4;
        adapt_arr[i] = (double)(Data[19 + i % Size] - 128) / 128.0;
    }

    cmsHPROFILE hGamutProfile = NULL;
    uint32_t nGamutPCSposition = 0;

    cmsHTRANSFORM hExtendedTransform = NULL;
    if (nProfiles > 0) {
        hExtendedTransform = cmsCreateExtendedTransform(
            ContextID, nProfiles, p_arr, bpc_arr, intents_arr, adapt_arr,
            hGamutProfile, nGamutPCSposition, InputFormat, OutputFormat, dwFlags
        );
    }

    // Cleanup transforms first
    if (hTransform1 != NULL) cmsDeleteTransform(hTransform1);
    if (hTransform2 != NULL) cmsDeleteTransform(hTransform2);
    if (hExtendedTransform != NULL) cmsDeleteTransform(hExtendedTransform);

    // Cleanup profiles
    // Only close profiles that were created and not NULL
    if (hXYZ != NULL) cmsCloseProfile(hXYZ);
    if (hBCHSW != NULL) cmsCloseProfile(hBCHSW);
    if (hBCHSWTHR != NULL) cmsCloseProfile(hBCHSWTHR);

    cmsDeleteContext(ContextID);

    return 0;
}