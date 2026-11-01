// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 100) return NULL;
    
    // Write minimal ICC profile data to a dummy file
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return NULL;
    
    // Ensure we only write the available data
    size_t bytes_to_write = size;
    fwrite(data, 1, bytes_to_write, f);
    fclose(f);
    
    cmsHPROFILE h = cmsOpenProfileFromFile("./dummy_file", "r");
    if (h) {
        // Clean up dummy file
        remove("./dummy_file");
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

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;

    cmsHPROFILE profiles[10] = {0};
    cmsHTRANSFORM transforms[10] = {0};
    int profile_count = 0;

    // Try to create up to 5 profiles from the input data
    for (int i = 0; i < 5 && profile_count < 10; i++) {
        size_t chunk_size = (Size / 5) + (i == 4 ? (Size % 5) : 0);
        if (chunk_size < 100) chunk_size = 100;
        
        // Check if we have enough data for this chunk
        size_t offset = i * chunk_size;
        if (offset + chunk_size > Size) {
            // Adjust the last chunk to fit remaining data
            if (i == 4) {
                chunk_size = Size - offset;
                if (chunk_size < 100) break;
            } else {
                break;
            }
        }

        cmsHPROFILE h = create_dummy_profile(Data + offset, chunk_size);
        if (h) {
            profiles[profile_count++] = h;
        }
    }

    if (profile_count == 0) {
        return 0;
    }

    // Test cmsCreateTransform
    if (profile_count >= 2) {
        cmsHTRANSFORM t = cmsCreateTransform(
            profiles[0],
            0, // InputFormat (fuzzed, likely invalid but safe to test)
            profiles[1],
            0, // OutputFormat
            0, // Intent
            0  // Flags
        );
        if (t) {
            transforms[0] = t;
        }
    }

    // Test cmsCreateMultiprofileTransform
    if (profile_count >= 1) {
        cmsHTRANSFORM t = cmsCreateMultiprofileTransform(
            profiles,
            (profile_count < 256) ? profile_count : 255,
            0, // InputFormat
            0, // OutputFormat
            0, // Intent
            0  // Flags
        );
        if (t && !transforms[1]) {
            transforms[1] = t;
        }
    }

    // Allocate buffers for transformation
    size_t buffer_size = Size > 1024 ? 1024 : Size;
    if (buffer_size < 100) buffer_size = 100;
    
    uint8_t *input_buf = (uint8_t *)malloc(buffer_size);
    uint8_t *output_buf = (uint8_t *)malloc(buffer_size);
    if (!input_buf || !output_buf) {
        cleanup_transforms(transforms, 2);
        cleanup_profiles(profiles, profile_count);
        free(input_buf);
        free(output_buf);
        return 0;
    }
    memset(input_buf, 0, buffer_size);
    memset(output_buf, 0, buffer_size);

    // Test cmsGetTransformOutputFormat
    for (int i = 0; i < 2; i++) {
        if (transforms[i]) {
            cmsGetTransformOutputFormat(transforms[i]);
        }
    }

    // Test cmsDoTransform
    for (int i = 0; i < 2; i++) {
        if (transforms[i]) {
            cmsDoTransform(transforms[i], input_buf, output_buf, (buffer_size > 0) ? 1 : 0);
        }
    }

    // Test cmsDoTransformStride
    for (int i = 0; i < 2; i++) {
        if (transforms[i]) {
            cmsDoTransformStride(transforms[i], input_buf, output_buf, (buffer_size > 0) ? 1 : 0, 0);
        }
    }

    // Test cmsDoTransformLineStride
    for (int i = 0; i < 2; i++) {
        if (transforms[i]) {
            cmsDoTransformLineStride(
                transforms[i],
                input_buf,
                output_buf,
                (buffer_size > 0) ? 1 : 0,
                1,
                0,
                0,
                0,
                0
            );
        }
    }

    // Cleanup
    free(input_buf);
    free(output_buf);
    cleanup_transforms(transforms, 2);
    cleanup_profiles(profiles, profile_count);

    return 0;
}