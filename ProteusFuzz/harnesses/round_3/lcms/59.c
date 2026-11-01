// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profiles(cmsHPROFILE *profiles, int count) {
    for (int i = 0; i < count; i++) {
        if (profiles[i] != NULL) {
            cmsCloseProfile(profiles[i]);
        }
    }
}

static void cleanup_transforms(cmsHTRANSFORM *transforms, int count) {
    for (int i = 0; i < count; i++) {
        if (transforms[i] != NULL) {
            cmsDeleteTransform(transforms[i]);
        }
    }
}

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 128) return NULL; // ICC profiles must be at least 128 bytes
    cmsHPROFILE h = cmsOpenProfileFromMem(data, (cmsUInt32Number)size);
    if (h == NULL) {
        return NULL;
    }
    return h;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Split input data for different purposes
    size_t split1 = Size / 3;
    size_t split2 = (Size / 3) * 2;
    
    const uint8_t *data1 = Data;
    size_t size1 = split1;
    const uint8_t *data2 = Data + split1;
    size_t size2 = split2 - split1;
    const uint8_t *data3 = Data + split2;
    size_t size3 = Size - split2;

    cmsHPROFILE profiles[4] = { NULL, NULL, NULL, NULL };
    cmsHTRANSFORM transforms[4] = { NULL, NULL, NULL, NULL };
    cmsHPROFILE devicelink = NULL;
    int profile_count = 0;

    // Attempt to create profiles from fuzz data
    if (size1 >= 128) profiles[0] = create_dummy_profile(data1, size1);
    if (size2 >= 128) profiles[1] = create_dummy_profile(data2, size2);
    if (size3 >= 128) profiles[2] = create_dummy_profile(data3, size3);

    // Count valid profiles
    for (int i = 0; i < 3; i++) {
        if (profiles[i] != NULL) profile_count++;
    }

    // Helper to get a valid profile or use the first one
    cmsHPROFILE input_prof = (profile_count > 0) ? profiles[0] : NULL;
    cmsHPROFILE output_prof = (profile_count > 1) ? profiles[1] : profiles[0];

    // Fuzz cmsCreateTransform
    if (input_prof && output_prof) {
        cmsUInt32Number fmtIn = (Size % 10) + 1; // Arbitrary format selection
        cmsUInt32Number fmtOut = ((Size >> 4) % 10) + 1;
        cmsUInt32Number intent = Size % 4;
        cmsUInt32Number flags = (Size >> 8) & 0xFF;
        
        transforms[0] = cmsCreateTransform(input_prof, fmtIn, output_prof, fmtOut, intent, flags);
    }

    // Fuzz cmsCreateMultiprofileTransform
    if (profile_count >= 1) {
        cmsHPROFILE *arr = profiles;
        cmsUInt32Number n = profile_count;
        cmsUInt32Number fmtIn = (Size % 10) + 1;
        cmsUInt32Number fmtOut = ((Size >> 4) % 10) + 1;
        cmsUInt32Number intent = Size % 4;
        cmsUInt32Number flags = (Size >> 8) & 0xFF;
        
        transforms[1] = cmsCreateMultiprofileTransform(arr, n, fmtIn, fmtOut, intent, flags);
    }

    // Fuzz cmsCreateExtendedTransform
    if (profile_count >= 1) {
        cmsHPROFILE *arr = profiles;
        cmsUInt32Number n = profile_count;
        cmsBool bpc[256];
        cmsUInt32Number intents[256];
        cmsFloat64Number adap[256];
        
        for (int i = 0; i < n && i < 256; i++) {
            bpc[i] = (Data[i % Size] & 1);
            intents[i] = (Data[(i+1) % Size] % 4);
            adap[i] = (Data[(i+2) % Size] / 255.0);
        }

        cmsHPROFILE gamut = (profile_count > 2) ? profiles[2] : NULL;
        cmsUInt32Number gamutPos = (Size >> 16) & 0xFF;
        cmsUInt32Number fmtIn = (Size % 10) + 1;
        cmsUInt32Number fmtOut = ((Size >> 4) % 10) + 1;
        cmsUInt32Number flags = (Size >> 8) & 0xFF;

        transforms[2] = cmsCreateExtendedTransform(ctx, n, arr, bpc, intents, adap, gamut, gamutPos, fmtIn, fmtOut, flags);
    }

    // Fuzz cmsGetTransformInputFormat
    for (int i = 0; i < 3; i++) {
        if (transforms[i] != NULL) {
            cmsGetTransformInputFormat(transforms[i]);
        }
    }

    // Fuzz cmsTransform2DeviceLink
    if (transforms[0] != NULL) {
        cmsFloat64Number version = 2.0 + ((double)(Size % 10) / 100.0);
        cmsUInt32Number flags = (Size >> 12) & 0xFF;
        devicelink = cmsTransform2DeviceLink(transforms[0], version, flags);
    }

    // Fuzz cmsDoTransformStride
    // Prepare buffers
    size_t buf_size = (Size > 1024) ? 1024 : Size;
    uint8_t *in_buf = malloc(buf_size);
    uint8_t *out_buf = malloc(buf_size);
    
    if (in_buf && out_buf) {
        memset(in_buf, 0, buf_size);
        memset(out_buf, 0, buf_size);
        
        for (int i = 0; i < 3; i++) {
            if (transforms[i] != NULL) {
                cmsUInt32Number size = (Size % 100) + 1;
                cmsUInt32Number stride = (Size % 200) + 1;
                
                // Ensure buffers are large enough for the requested stride/size
                if (buf_size >= (size * stride)) {
                    cmsDoTransformStride(transforms[i], in_buf, out_buf, size, stride);
                }
            }
        }
    }

    // Cleanup
    if (devicelink) cmsCloseProfile(devicelink);
    cleanup_transforms(transforms, 3);
    cleanup_profiles(profiles, 3);
    cmsDeleteContext(ctx);
    
    if (in_buf) free(in_buf);
    if (out_buf) free(out_buf);

    return 0;
}