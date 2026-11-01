// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(cmsContext ctx, const uint8_t *data, size_t size) {
    if (size < 128) return NULL;
    // cmsOpenProfileFromMem does not take a context argument in standard lcms2
    return cmsOpenProfileFromMem(data, size);
}

static void cleanup_transform(cmsHTRANSFORM t) {
    if (t) cmsDeleteTransform(t);
}

static void cleanup_profile(cmsHPROFILE p) {
    if (p) cmsCloseProfile(p);
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Parse data to determine test case
    uint8_t mode = Data[0] % 6;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    cmsHPROFILE prof1 = NULL, prof2 = NULL, prof3 = NULL;
    cmsHTRANSFORM trans = NULL;

    // Create dummy profiles if we have enough data
    if (payload_size >= 256) {
        prof1 = create_dummy_profile(ctx, payload, 128);
        if (payload_size >= 384) {
            prof2 = create_dummy_profile(ctx, payload + 128, 128);
        }
        if (payload_size >= 512) {
            prof3 = create_dummy_profile(ctx, payload + 256, 128);
        }
    }

    cmsUInt32Number input_fmt = (Data[1] % 10) + 1;
    cmsUInt32Number output_fmt = (Data[2] % 10) + 1;
    cmsUInt32Number intent = Data[3] % 4;
    cmsUInt32Number flags = Data[4] % 16;

    switch (mode) {
        case 0: // cmsCreateProofingTransform
            if (prof1 && prof2 && prof3) {
                trans = cmsCreateProofingTransform(prof1, input_fmt, prof2, output_fmt,
                                                   prof3, intent, intent, flags);
            }
            break;

        case 1: // cmsCreateMultiprofileTransformTHR
            {
                cmsHPROFILE profiles[255];
                uint32_t n_profiles = 0;
                if (prof1) { profiles[n_profiles++] = prof1; }
                if (prof2) { profiles[n_profiles++] = prof2; }
                if (prof3) { profiles[n_profiles++] = prof3; }
                
                if (n_profiles > 0) {
                    trans = cmsCreateMultiprofileTransformTHR(ctx, profiles, n_profiles,
                                                              input_fmt, output_fmt, intent, flags);
                }
            }
            break;

        case 2: // cmsSetHeaderRenderingIntent
            if (prof1) {
                cmsSetHeaderRenderingIntent(prof1, intent);
            }
            break;

        case 3: // cmsCreateTransformTHR
            if (prof1) {
                trans = cmsCreateTransformTHR(ctx, prof1, input_fmt, prof2, output_fmt, intent, flags);
            }
            break;

        case 4: // cmsGetSupportedIntents
            {
                cmsUInt32Number count = cmsGetSupportedIntents(0, NULL, NULL);
                if (count > 0) {
                    cmsUInt32Number *codes = malloc(count * sizeof(cmsUInt32Number));
                    char **descs = malloc(count * sizeof(char*));
                    if (codes && descs) {
                        cmsGetSupportedIntents(count, codes, descs);
                        free(codes);
                        free(descs);
                    }
                }
            }
            break;

        case 5: // cmsGetSupportedIntentsTHR
            {
                cmsUInt32Number count = cmsGetSupportedIntentsTHR(ctx, 0, NULL, NULL);
                if (count > 0) {
                    cmsUInt32Number *codes = malloc(count * sizeof(cmsUInt32Number));
                    char **descs = malloc(count * sizeof(char*));
                    if (codes && descs) {
                        cmsGetSupportedIntentsTHR(ctx, count, codes, descs);
                        free(codes);
                        free(descs);
                    }
                }
            }
            break;
    }

    cleanup_transform(trans);
    cleanup_profile(prof1);
    cleanup_profile(prof2);
    cleanup_profile(prof3);
    cmsDeleteContext(ctx);

    return 0;
}