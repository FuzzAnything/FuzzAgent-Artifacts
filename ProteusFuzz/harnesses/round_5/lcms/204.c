// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *Data, size_t Size) {
    if (Size < 128) return NULL;
    
    // Allocate buffer and copy data
    uint8_t *buf = malloc(Size);
    if (!buf) return NULL;
    memcpy(buf, Data, Size);
    
    // Force minimal valid header (Size field must be at least 128)
    // Header: 4 bytes size, 4 bytes CMM type, etc.
    buf[0] = 0; buf[1] = 0; buf[2] = 0; buf[3] = (uint8_t)(Size >> 24);
    buf[4] = (uint8_t)(Size >> 16);
    buf[5] = (uint8_t)(Size >> 8);
    buf[6] = (uint8_t)(Size);
    
    // Ensure CMM type is valid (e.g., 'acsp')
    buf[8] = 'a'; buf[9] = 'c'; buf[10] = 's'; buf[11] = 'p';
    
    cmsHPROFILE h = cmsOpenProfileFromMem(buf, Size);
    free(buf);
    return h;
}

static void cleanup_profiles(cmsHPROFILE *profiles, int count) {
    for (int i = 0; i < count; i++) {
        if (profiles[i]) {
            cmsCloseProfile(profiles[i]);
        }
    }
}

int LLVMFuzzerTestOneInput_204(const uint8_t *Data, size_t Size) {
    if (Size < 256) return 0;

    // Split data for profiles and buffers
    size_t profile_size = Size / 4;
    if (profile_size < 128) profile_size = 128;
    size_t buffer_size = (Size - profile_size) / 2;
    if (buffer_size < 64) buffer_size = 64;

    cmsHPROFILE profiles[256];
    int profile_count = 0;

    // Try to create 1 to 4 profiles from the input data
    for (int i = 0; i < 4 && profile_count < 255; i++) {
        size_t offset = i * profile_size;
        if (offset + profile_size > Size) break;
        
        cmsHPROFILE h = create_dummy_profile(Data + offset, profile_size);
        if (h) {
            profiles[profile_count++] = h;
        }
    }

    if (profile_count == 0) return 0;

    // Allocate input and output buffers
    uint8_t *in_buf = malloc(buffer_size);
    uint8_t *out_buf = malloc(buffer_size);
    if (!in_buf || !out_buf) {
        cleanup_profiles(profiles, profile_count);
        return 0;
    }
    memset(in_buf, 0, buffer_size);
    memset(out_buf, 0, buffer_size);

    // Define valid formats available in lcms2.h
    cmsUInt32Number formats[] = {
        cmsSigRgbData, 
        cmsSigLabData, 
        cmsSigLuvData, 
        cmsSigYCbCrData,
        cmsSigGrayData,
        cmsSigCmykData
    };
    
    // Use integer constants for intents as defined in lcms2.h
    // 0: Perceptual, 1: Relative, 2: Saturation, 3: Absolute
    cmsUInt32Number intents[] = { 0, 1, 2, 3 };
    cmsUInt32Number flags[] = { 0, cmsFLAGS_NOOPTIMIZE, cmsFLAGS_BLACKPOINTCOMPENSATION };

    cmsHTRANSFORM transform = NULL;

    // Fuzz cmsCreateMultiprofileTransform
    for (int i = 0; i < profile_count; i++) {
        for (int j = 0; j < profile_count; j++) {
            cmsHPROFILE *p_arr = &profiles[i];
            cmsUInt32Number n_profiles = (j < profile_count) ? (j - i + 1) : 1;
            if (n_profiles > 255) n_profiles = 255;
            if (i + n_profiles > profile_count) n_profiles = profile_count - i;
            if (n_profiles <= 0) continue;

            for (int f_in = 0; f_in < 6; f_in++) {
                for (int f_out = 0; f_out < 6; f_out++) {
                    for (int intent = 0; intent < 4; intent++) {
                        for (int flag = 0; flag < 3; flag++) {
                            transform = cmsCreateMultiprofileTransform(
                                &profiles[i], n_profiles,
                                formats[f_in], formats[f_out],
                                intents[intent], flags[flag]
                            );

                            if (transform) {
                                // Test cmsGetTransformInputFormat
                                cmsGetTransformInputFormat(transform);
                                // Test cmsGetTransformOutputFormat
                                cmsGetTransformOutputFormat(transform);

                                // Test cmsDoTransform
                                // Estimate pixel count based on buffer size (assume 4 bytes per pixel max)
                                cmsUInt32Number pixel_count = buffer_size / 4;
                                if (pixel_count > 0) {
                                    cmsDoTransform(transform, in_buf, out_buf, pixel_count);
                                }

                                // Test cmsDoTransformStride
                                cmsUInt32Number stride = 4;
                                if (pixel_count > 0) {
                                    cmsDoTransformStride(transform, in_buf, out_buf, pixel_count, stride);
                                }

                                // Test cmsDoTransformLineStride
                                cmsUInt32Number pixels_per_line = (pixel_count > 10) ? 10 : pixel_count;
                                cmsUInt32Number line_count = (pixel_count > 0) ? (pixel_count / pixels_per_line) : 0;
                                cmsUInt32Number bytes_per_line_in = pixels_per_line * 4;
                                cmsUInt32Number bytes_per_line_out = pixels_per_line * 4;
                                cmsUInt32Number bytes_per_plane_in = 4;
                                cmsUInt32Number bytes_per_plane_out = 4;
                                
                                if (pixels_per_line > 0 && line_count > 0) {
                                    cmsDoTransformLineStride(transform, in_buf, out_buf,
                                        pixels_per_line, line_count,
                                        bytes_per_line_in, bytes_per_line_out,
                                        bytes_per_plane_in, bytes_per_plane_out);
                                }

                                cmsDeleteTransform(transform);
                                transform = NULL;
                            }
                        }
                    }
                }
            }
        }
    }

    // Test with NULL handle
    cmsGetTransformInputFormat(NULL);
    cmsGetTransformOutputFormat(NULL);
    cmsDoTransform(NULL, in_buf, out_buf, 1);
    cmsDoTransformStride(NULL, in_buf, out_buf, 1, 4);
    cmsDoTransformLineStride(NULL, in_buf, out_buf, 1, 1, 4, 4, 4, 4);

    free(in_buf);
    free(out_buf);
    cleanup_profiles(profiles, profile_count);

    return 0;
}