// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(cmsContext ctx, const uint8_t *data, size_t size) {
    if (size < 128) return NULL;
    return cmsOpenProfileFromMemTHR(ctx, data, size);
}

static void cleanup_profiles(cmsContext ctx, cmsHPROFILE *profiles, int count) {
    for (int i = 0; i < count; i++) {
        if (profiles[i] != NULL) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = NULL;
        }
    }
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform != NULL) {
        cmsDeleteTransform(transform);
    }
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsHPROFILE profiles[256];
    int profile_count = 0;
    size_t offset = 0;
    size_t chunk_size = Size / 10; 
    if (chunk_size < 128) chunk_size = 128;

    while (offset + chunk_size <= Size && profile_count < 10) {
        profiles[profile_count] = create_dummy_profile(ctx, Data + offset, chunk_size);
        if (profiles[profile_count] != NULL) {
            profile_count++;
        }
        offset += chunk_size;
        if (offset >= Size) break;
    }

    if (profile_count == 0 && Size >= 128) {
        profiles[0] = create_dummy_profile(ctx, Data, Size);
        if (profiles[0] != NULL) profile_count = 1;
    }

    cmsHPROFILE bchw_profile = NULL;
    cmsUInt32Number lut_points = (Data[0] % 20) + 1; 
    cmsFloat64Number bright = (Data[1] / 255.0) * 2.0 - 1.0;
    cmsFloat64Number contrast = (Data[2] / 255.0) * 2.0 - 1.0;
    cmsFloat64Number hue = (Data[3] / 255.0) * 360.0;
    cmsFloat64Number saturation = (Data[4] / 255.0) * 2.0 - 1.0;
    cmsUInt32Number temp_src = 2000 + (Data[5] * 100);
    cmsUInt32Number temp_dest = 2000 + (Data[6] * 100);

    bchw_profile = cmsCreateBCHSWabstractProfileTHR(ctx, lut_points, bright, contrast, hue, saturation, temp_src, temp_dest);
    if (bchw_profile != NULL) {
        cmsCloseProfile(bchw_profile);
    }

    cmsHPROFILE bchw_profile_global = cmsCreateBCHSWabstractProfile(lut_points, bright, contrast, hue, saturation, temp_src, temp_dest);
    if (bchw_profile_global != NULL) {
        cmsCloseProfile(bchw_profile_global);
    }

    cmsHTRANSFORM transform = NULL;
    cmsUInt32Number input_format = (Data[7] % 10) + 1; 
    cmsUInt32Number output_format = (Data[8] % 10) + 1;
    cmsUInt32Number intent = Data[9] % 4; 
    cmsUInt32Number flags = Data[10] % 8;

    if (profile_count > 0) {
        transform = cmsCreateTransformTHR(ctx, profiles[0], input_format, NULL, output_format, intent, flags);
        cleanup_transform(transform);
        transform = NULL;

        if (profile_count > 1) {
            transform = cmsCreateTransformTHR(ctx, profiles[0], input_format, profiles[1], output_format, intent, flags);
            cleanup_transform(transform);
            transform = NULL;
        }
    }

    if (profile_count > 0) {
        transform = cmsCreateMultiprofileTransformTHR(ctx, profiles, profile_count, input_format, output_format, intent, flags);
        cleanup_transform(transform);
        transform = NULL;
    }

    if (profile_count > 0) {
        transform = cmsCreateMultiprofileTransform(profiles, profile_count, input_format, output_format, intent, flags);
        cleanup_transform(transform);
        transform = NULL;
    }

    if (profile_count > 0) {
        cmsBool bpc[256];
        cmsUInt32Number intents[256];
        cmsFloat64Number adaptation[256];
        
        for (int i = 0; i < profile_count; i++) {
            bpc[i] = (Data[11 + (i % (Size - 11))] % 2) ? 1 : 0;
            intents[i] = Data[12 + (i % (Size - 12))] % 4;
            adaptation[i] = (Data[13 + (i % (Size - 13))] / 255.0);
        }

        cmsHPROFILE gamut_profile = (profile_count > 1) ? profiles[1] : NULL;
        cmsUInt32Number nGamutPCSposition = 0;

        transform = cmsCreateExtendedTransform(ctx, profile_count, profiles, bpc, intents, adaptation, gamut_profile, nGamutPCSposition, input_format, output_format, flags);
        cleanup_transform(transform);
        transform = NULL;
        
        transform = cmsCreateExtendedTransform(ctx, profile_count, profiles, bpc, intents, adaptation, gamut_profile, nGamutPCSposition, input_format, output_format, cmsFLAGS_NULLTRANSFORM);
        cleanup_transform(transform);
    }

    cleanup_profiles(ctx, profiles, profile_count);
    cmsDeleteContext(ctx);

    return 0;
}