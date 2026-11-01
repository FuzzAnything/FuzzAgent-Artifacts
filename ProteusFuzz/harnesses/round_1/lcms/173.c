// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void create_dummy_profile(const uint8_t *data, size_t size, cmsHPROFILE *out_profile) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        *out_profile = NULL;
        return;
    }
    if (size == 0) {
        // Write a minimal valid-ish header if input is empty to avoid crash on read
        // Minimal ICC header is 128 bytes. We write zeros.
        uint8_t dummy[128] = {0};
        fwrite(dummy, 1, 128, f);
        fclose(f);
    } else {
        fwrite(data, 1, size, f);
        fclose(f);
    }

    *out_profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (*out_profile == NULL) {
        // If opening fails, try to create a minimal sRGB profile programmatically
        *out_profile = cmsCreate_sRGBProfile();
    }
}

static void cleanup_profiles(cmsHPROFILE *profiles, int count) {
    for (int i = 0; i < count; i++) {
        if (profiles[i] != NULL) {
            cmsCloseProfile(profiles[i]);
        }
    }
}

int LLVMFuzzerTestOneInput_173(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize context
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    // Allocate memory for profiles
    cmsHPROFILE hProfiles[256];
    memset(hProfiles, 0, sizeof(hProfiles));
    int profile_count = 0;

    // Determine how many profiles to attempt to create based on input size
    // We try to create up to 4 profiles to keep it manageable but diverse
    int max_profiles = (Size < 20) ? 1 : (Size < 100) ? 2 : 4;
    if (max_profiles > 255) max_profiles = 255;

    // Create dummy profiles
    for (int i = 0; i < max_profiles; i++) {
        size_t chunk_size = Size / max_profiles;
        const uint8_t *chunk_data = Data + (i * chunk_size);
        create_dummy_profile(chunk_data, chunk_size, &hProfiles[i]);
        if (hProfiles[i] != NULL) {
            profile_count++;
        }
    }

    if (profile_count == 0) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Define formats
    cmsUInt32Number input_format = 0;
    cmsUInt32Number output_format = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;

    // Extract parameters from fuzz input
    if (Size >= 4) {
        input_format = *(const cmsUInt32Number*)(Data);
        output_format = *(const cmsUInt32Number*)(Data + 4);
        intent = *(const cmsUInt32Number*)(Data + 8);
        flags = *(const cmsUInt32Number*)(Data + 12);
    }

    // Clamp intents and flags to reasonable ranges to avoid undefined behavior
    intent = intent % 4; // 0-3
    flags = flags % 100;

    cmsHTRANSFORM hTransform = NULL;

    // Test cmsCreateMultiprofileTransform
    if (profile_count > 0) {
        hTransform = cmsCreateMultiprofileTransform(hProfiles, profile_count, input_format, output_format, intent, flags);
    }

    // If multiprofile failed, try Extended Transform
    if (hTransform == NULL && profile_count > 0) {
        cmsBool *bpc = NULL;
        cmsUInt32Number *intents = NULL;
        cmsFloat64Number *adapt = NULL;
        
        // Allocate arrays for extended transform
        bpc = (cmsBool*)malloc(profile_count * sizeof(cmsBool));
        intents = (cmsUInt32Number*)malloc(profile_count * sizeof(cmsUInt32Number));
        adapt = (cmsFloat64Number*)malloc(profile_count * sizeof(cmsFloat64Number));

        if (bpc && intents && adapt) {
            for (int i = 0; i < profile_count; i++) {
                bpc[i] = (i % 2);
                intents[i] = intent;
                adapt[i] = (cmsFloat64Number)(i % 5);
            }
            
            hTransform = cmsCreateExtendedTransform(
                ContextID,
                profile_count,
                hProfiles,
                bpc,
                intents,
                adapt,
                NULL, // hGamutProfile
                0,    // nGamutPCSposition
                input_format,
                output_format,
                flags
            );
        }
        free(bpc);
        free(intents);
        free(adapt);
    }

    // If still no transform, try creating a simple one with the first profile
    if (hTransform == NULL && profile_count > 0) {
        hTransform = cmsCreateTransform(hProfiles[0], input_format, hProfiles[0], output_format, intent, flags);
    }

    if (hTransform != NULL) {
        // Test cmsGetTransformInputFormat
        cmsUInt32Number in_fmt = cmsGetTransformInputFormat(hTransform);
        (void)in_fmt; // Use the value to avoid unused variable warning

        // Test cmsTransform2DeviceLink
        cmsFloat64Number version = (Size > 16) ? (cmsFloat64Number)(*(const uint32_t*)(Data + 16)) / 1000.0 : 4.2;
        cmsHPROFILE hDL = cmsTransform2DeviceLink(hTransform, version, flags);
        if (hDL != NULL) {
            cmsCloseProfile(hDL);
        }

        // Prepare buffers for transform tests
        size_t buf_size = (Size > 1000) ? 1000 : Size;
        uint8_t *input_buf = (uint8_t*)malloc(buf_size);
        uint8_t *output_buf = (uint8_t*)malloc(buf_size);
        
        if (input_buf && output_buf) {
            memset(input_buf, 0xAA, buf_size);
            memset(output_buf, 0, buf_size);

            cmsUInt32Number pixels_per_line = (Size > 20) ? *(const uint32_t*)(Data + 20) % 100 : 1;
            cmsUInt32Number line_count = (Size > 24) ? *(const uint32_t*)(Data + 24) % 10 : 1;
            cmsUInt32Number stride_in = (Size > 28) ? *(const uint32_t*)(Data + 28) % 1000 : 0;
            cmsUInt32Number stride_out = (Size > 32) ? *(const uint32_t*)(Data + 32) % 1000 : 0;
            cmsUInt32Number plane_in = (Size > 36) ? *(const uint32_t*)(Data + 36) % 100 : 0;
            cmsUInt32Number plane_out = (Size > 40) ? *(const uint32_t*)(Data + 40) % 100 : 0;

            // Test cmsDoTransformLineStride
            cmsDoTransformLineStride(
                hTransform,
                input_buf,
                output_buf,
                pixels_per_line,
                line_count,
                stride_in,
                stride_out,
                plane_in,
                plane_out
            );

            // Test cmsDoTransformStride (Deprecated)
            cmsUInt32Number size_pixels = (Size > 44) ? *(const uint32_t*)(Data + 44) % 500 : 10;
            cmsUInt32Number stride = (Size > 48) ? *(const uint32_t*)(Data + 48) % 1000 : 0;
            cmsDoTransformStride(hTransform, input_buf, output_buf, size_pixels, stride);
        }
        free(input_buf);
        free(output_buf);

        // Cleanup transform
        cmsDeleteTransform(hTransform);
    }

    // Cleanup profiles
    cleanup_profiles(hProfiles, profile_count);

    // Cleanup context
    cmsDeleteContext(ContextID);

    return 0;
}