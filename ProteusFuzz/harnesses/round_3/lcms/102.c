// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profiles(cmsHPROFILE *profiles, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (profiles[i] != NULL) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = NULL;
        }
    }
}

static cmsHPROFILE create_profile_from_fuzz_data(const uint8_t *Data, size_t Size) {
    if (Size < 128) return NULL;
    cmsHPROFILE h = cmsOpenProfileFromMem(Data, Size);
    if (h != NULL) return h;
    
    // Try to create a profile from a file if memory load fails
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
        h = cmsOpenProfileFromFile("./dummy_file", "r");
    }
    return h;
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    cmsHPROFILE hInput = NULL;
    cmsHPROFILE hOutput = NULL;
    cmsHPROFILE hProofing = NULL;
    cmsHPROFILE hProfiles[256];
    cmsHTRANSFORM hTransform = NULL;
    cmsHTRANSFORM hExtTransform = NULL;
    cmsBool BPC[256];
    cmsUInt32Number Intents[256];
    cmsFloat64Number AdaptationStates[256];
    cmsHPROFILE hGamutProfile = NULL;
    
    // Initialize arrays with safe defaults
    for (int i = 0; i < 256; i++) {
        hProfiles[i] = NULL;
        BPC[i] = 0; // Use 0 instead of cmsFALSE
        Intents[i] = 0;
        AdaptationStates[i] = 0.0;
    }

    // Create input profile
    hInput = create_profile_from_fuzz_data(Data, Size);
    if (!hInput) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Create output profile (reuse input or create new if possible)
    hOutput = create_profile_from_fuzz_data(Data, Size);
    if (!hOutput) {
        hOutput = hInput; // Fallback to input if output creation fails
    }

    // Create proofing profile
    hProofing = create_profile_from_fuzz_data(Data, Size);
    if (!hProofing) {
        hProofing = hInput;
    }

    // Create gamut profile
    hGamutProfile = create_profile_from_fuzz_data(Data, Size);
    if (!hGamutProfile) {
        hGamutProfile = hInput;
    }

    // Fuzz cmsFormatterForColorspaceOfProfile
    cmsUInt32Number fmt = cmsFormatterForColorspaceOfProfile(hInput, 1, 0); // Use 0 instead of cmsFALSE
    if (fmt != 0) {
        fmt = cmsFormatterForColorspaceOfProfile(hInput, 2, 1); // Use 1 instead of cmsTRUE
    }

    // Fuzz cmsCreateProofingTransform
    cmsHTRANSFORM hProofTransform = cmsCreateProofingTransform(
        hInput, fmt, 
        hOutput, fmt, 
        hProofing, 
        0, 0, 0
    );
    if (hProofTransform) {
        // Fuzz cmsGetTransformOutputFormat
        cmsUInt32Number outFmt = cmsGetTransformOutputFormat(hProofTransform);
        (void)outFmt;

        // Fuzz cmsChangeBuffersFormat
        cmsChangeBuffersFormat(hProofTransform, fmt, fmt);
        cmsChangeBuffersFormat(hProofTransform, 0, 0);

        cmsDeleteTransform(hProofTransform);
    }

    // Fuzz cmsCreateMultiprofileTransform
    hProfiles[0] = hInput;
    if (hOutput && hOutput != hInput) hProfiles[1] = hOutput;
    
    cmsUInt32Number nProfiles = (hProfiles[1] != NULL) ? 2 : 1;
    
    hTransform = cmsCreateMultiprofileTransform(
        hProfiles, nProfiles, 
        fmt, fmt, 
        0, 0
    );

    if (hTransform) {
        cmsUInt32Number outFmt = cmsGetTransformOutputFormat(hTransform);
        (void)outFmt;
        cmsChangeBuffersFormat(hTransform, fmt, fmt);
        cmsDeleteTransform(hTransform);
    }

    // Fuzz cmsCreateExtendedTransform
    // Prepare arrays
    for (int i = 0; i < nProfiles; i++) {
        Intents[i] = (Size > i) ? (Data[i] % 4) : 0;
        AdaptationStates[i] = (Size > i) ? (Data[i] / 255.0) : 0.0;
    }
    
    hExtTransform = cmsCreateExtendedTransform(
        ContextID,
        nProfiles, hProfiles,
        BPC, Intents, AdaptationStates,
        hGamutProfile, 0,
        fmt, fmt,
        0
    );

    if (hExtTransform) {
        cmsUInt32Number outFmt = cmsGetTransformOutputFormat(hExtTransform);
        (void)outFmt;
        cmsChangeBuffersFormat(hExtTransform, fmt, fmt);
        cmsDeleteTransform(hExtTransform);
    }

    // Cleanup
    cleanup_profiles(hProfiles, nProfiles);
    if (hOutput != hInput) {
        cmsCloseProfile(hOutput);
    }
    if (hProofing != hInput) {
        cmsCloseProfile(hProofing);
    }
    if (hGamutProfile != hInput) {
        cmsCloseProfile(hGamutProfile);
    }
    cmsCloseProfile(hInput);
    cmsDeleteContext(ContextID);

    return 0;
}