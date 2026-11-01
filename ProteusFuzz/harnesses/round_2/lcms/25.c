// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static cmsHPROFILE load_profile_from_data(const uint8_t *data, size_t size) {
    if (size < 128) return NULL;
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    return hProfile;
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

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    cmsHPROFILE hProfiles[256] = {0};
    cmsHTRANSFORM hTransforms[10] = {0};
    int profile_count = 0;
    int transform_count = 0;

    // Attempt to load profiles from the input data
    size_t offset = 0;
    while (offset + 128 <= Size && profile_count < 255) {
        size_t chunk_size = Size - offset;
        if (chunk_size > 65536) chunk_size = 65536; 

        cmsHPROFILE hProfile = load_profile_from_data(Data + offset, chunk_size);
        if (hProfile) {
            hProfiles[profile_count++] = hProfile;
        }
        offset += 64; 
        if (offset >= Size) break;
    }

    if (profile_count == 0) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Test 1: cmsFormatterForColorspaceOfProfile
    for (int i = 0; i < profile_count; i++) {
        if (hProfiles[i]) {
            cmsFormatterForColorspaceOfProfile(hProfiles[i], 1, 0);
            cmsFormatterForColorspaceOfProfile(hProfiles[i], 2, 0);
            cmsFormatterForColorspaceOfProfile(hProfiles[i], 1, 1);
            cmsFormatterForColorspaceOfProfile(hProfiles[i], 2, 1);
        }
    }

    // Define formats manually to avoid missing macros or functions
    cmsUInt32Number fmt8_4 = 0x11111111; // 4 channels, 8-bit unsigned
    cmsUInt32Number fmt16_4 = 0x22222222; // 4 channels, 16-bit unsigned

    // Test 2: cmsCreateTransform
    if (profile_count >= 2) {
        cmsHTRANSFORM hT = cmsCreateTransform(
            hProfiles[0],
            fmt8_4,
            hProfiles[1],
            fmt8_4,
            0, 
            0  
        );
        if (hT) {
            hTransforms[transform_count++] = hT;
        }
    }

    // Test 3: cmsCreateMultiprofileTransform
    if (profile_count >= 1) {
        cmsHTRANSFORM hT = cmsCreateMultiprofileTransform(
            hProfiles,
            profile_count,
            fmt8_4,
            fmt8_4,
            0,
            0
        );
        if (hT) {
            hTransforms[transform_count++] = hT;
        }
    }

    // Test 4: cmsCreateExtendedTransform
    if (profile_count >= 1) {
        cmsBool BPC[256] = {0};
        cmsUInt32Number Intents[256] = {0};
        cmsFloat64Number AdaptationStates[256] = {0.0};
        
        for (int i = 0; i < profile_count; i++) {
            BPC[i] = 0;
            Intents[i] = 0;
            AdaptationStates[i] = 0.0;
        }

        cmsHTRANSFORM hT = cmsCreateExtendedTransform(
            ContextID,
            profile_count,
            hProfiles,
            BPC,
            Intents,
            AdaptationStates,
            NULL, 
            0,    
            fmt8_4,
            fmt8_4,
            0
        );
        if (hT) {
            hTransforms[transform_count++] = hT;
        }
    }

    // Test 5: cmsGetTransformOutputFormat and cmsChangeBuffersFormat
    for (int i = 0; i < transform_count; i++) {
        if (hTransforms[i]) {
            cmsGetTransformOutputFormat(hTransforms[i]);
            
            cmsChangeBuffersFormat(
                hTransforms[i],
                fmt8_4,
                fmt16_4
            );
        }
    }

    // Cleanup
    cleanup_profiles(hProfiles, profile_count);
    cleanup_transforms(hTransforms, transform_count);
    cmsDeleteContext(ContextID);

    return 0;
}