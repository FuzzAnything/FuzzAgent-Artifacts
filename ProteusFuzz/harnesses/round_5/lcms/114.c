// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHPROFILE load_profile_from_fuzz(const uint8_t *Data, size_t Size, const char *filename) {
    if (Size == 0) return NULL;
    
    FILE *f = fopen(filename, "wb");
    if (!f) return NULL;
    fwrite(Data, 1, Size, f);
    fclose(f);

    cmsHPROFILE h = cmsOpenProfileFromFile(filename, "r");
    if (h) {
        // Basic validation check
        cmsUInt32Number profileType = cmsGetProfileVersion(h);
        if (profileType == 0) {
            cmsCloseProfile(h);
            return NULL;
        }
    }
    return h;
}

static void cleanup_profiles(cmsHPROFILE *profiles, int count) {
    for (int i = 0; i < count; i++) {
        if (profiles[i]) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = NULL;
        }
    }
}

static void cleanup_transforms(cmsHTRANSFORM *transforms, int count) {
    for (int i = 0; i < count; i++) {
        if (transforms[i]) {
            cmsDeleteTransform(transforms[i]);
            transforms[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Split data into chunks for different profiles
    size_t chunk_size = Size / 4;
    if (chunk_size < 4) chunk_size = Size / 4;

    cmsHPROFILE profiles[4] = {NULL, NULL, NULL, NULL};
    cmsHTRANSFORM transforms[4] = {NULL, NULL, NULL, NULL};
    
    // Use the default context (NULL) instead of calling cmsGetGlobalContext
    cmsContext ctx = NULL;

    // Load up to 4 profiles from the fuzz data
    for (int i = 0; i < 4; i++) {
        const char *fname = "./dummy_file";
        if (i == 1) fname = "./dummy_file1";
        if (i == 2) fname = "./dummy_file2";
        if (i == 3) fname = "./dummy_file3";

        size_t start = i * chunk_size;
        size_t len = (i == 3) ? (Size - start) : chunk_size;
        if (len > 0 && start < Size) {
            profiles[i] = load_profile_from_fuzz(Data + start, len, fname);
        }
    }

    // Test cmsFormatterForColorspaceOfProfile
    for (int i = 0; i < 4; i++) {
        if (profiles[i]) {
            cmsUInt32Number fmt = cmsFormatterForColorspaceOfProfile(profiles[i], 1, FALSE);
            (void)fmt; // Use the value
            fmt = cmsFormatterForColorspaceOfProfile(profiles[i], 2, FALSE);
            (void)fmt;
            fmt = cmsFormatterForColorspaceOfProfile(profiles[i], 1, TRUE);
            (void)fmt;
        }
    }

    // Test cmsCreateProofingTransform
    if (profiles[0] && profiles[1] && profiles[2]) {
        cmsUInt32Number inputFmt = cmsFormatterForColorspaceOfProfile(profiles[0], 2, FALSE);
        cmsUInt32Number outputFmt = cmsFormatterForColorspaceOfProfile(profiles[1], 2, FALSE);
        cmsUInt32Number proofFmt = cmsFormatterForColorspaceOfProfile(profiles[2], 2, FALSE);
        
        if (inputFmt && outputFmt && proofFmt) {
            cmsHTRANSFORM t = cmsCreateProofingTransform(
                profiles[0], inputFmt,
                profiles[1], outputFmt,
                profiles[2], 0, // Intent
                0,              // ProofingIntent
                0               // dwFlags
            );
            if (t) {
                transforms[0] = t;
            }
        }
    }

    // Test cmsCreateExtendedTransform
    if (profiles[0]) {
        cmsHPROFILE hProfiles[1] = {profiles[0]};
        cmsBool BPC[1] = {FALSE};
        cmsUInt32Number Intents[1] = {0};
        cmsFloat64Number AdaptationStates[1] = {0.0};
        
        cmsUInt32Number inputFmt = cmsFormatterForColorspaceOfProfile(profiles[0], 2, FALSE);
        cmsUInt32Number outputFmt = inputFmt;

        cmsHTRANSFORM t = cmsCreateExtendedTransform(
            ctx,
            1, hProfiles,
            BPC,
            Intents,
            AdaptationStates,
            NULL, // hGamutProfile
            0,    // nGamutPCSposition
            inputFmt,
            outputFmt,
            0     // dwFlags
        );
        if (t) {
            transforms[1] = t;
        }
    }

    // Test cmsChangeBuffersFormat
    for (int i = 0; i < 4; i++) {
        if (transforms[i]) {
            cmsHPROFILE refProfile = profiles[0] ? profiles[0] : (profiles[1] ? profiles[1] : NULL);
            if (refProfile) {
                cmsUInt32Number newIn = cmsFormatterForColorspaceOfProfile(refProfile, 1, FALSE);
                cmsUInt32Number newOut = cmsFormatterForColorspaceOfProfile(refProfile, 1, FALSE);
                if (newIn && newOut) {
                    cmsChangeBuffersFormat(transforms[i], newIn, newOut);
                }
            }
        }
    }

    // Test cmsGetTransformOutputFormat
    for (int i = 0; i < 4; i++) {
        if (transforms[i]) {
            cmsUInt32Number fmt = cmsGetTransformOutputFormat(transforms[i]);
            (void)fmt;
        }
    }

    // Test cmsDoTransformLineStride
    // Allocate small buffers for testing
    uint8_t *inBuf = malloc(1024);
    uint8_t *outBuf = malloc(1024);
    if (inBuf && outBuf) {
        memset(inBuf, 0, 1024);
        memset(outBuf, 0, 1024);
        
        for (int i = 0; i < 4; i++) {
            if (transforms[i]) {
                cmsDoTransformLineStride(
                    transforms[i],
                    inBuf, outBuf,
                    10, // PixelsPerLine
                    10, // LineCount
                    10, // BytesPerLineIn
                    10, // BytesPerLineOut
                    0,  // BytesPerPlaneIn
                    0   // BytesPerPlaneOut
                );
            }
        }
    }

    // Cleanup
    cleanup_profiles(profiles, 4);
    cleanup_transforms(transforms, 4);
    if (inBuf) free(inBuf);
    if (outBuf) free(outBuf);

    return 0;
}