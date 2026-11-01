// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
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

int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) ctx = cmsCreateContext(NULL, NULL); 

    cmsHPROFILE profiles[256] = {0};
    cmsHTRANSFORM transforms[10] = {0};
    int profile_count = 0;
    int transform_count = 0;

    size_t offset = 0;
    while (offset + 128 <= Size && profile_count < 255) {
        cmsHPROFILE h = cmsOpenProfileFromMem(Data + offset, Size - offset);
        if (h != NULL) {
            profiles[profile_count++] = h;
            offset += 128;
            if (offset > Size) break;
        } else {
            offset += 4;
            if (offset > Size) break;
        }
    }

    for (int i = 0; i < profile_count; i++) {
        if (profiles[i] != NULL) {
            cmsUInt32Number intent = cmsGetHeaderRenderingIntent(profiles[i]);
            (void)intent;
        }
    }

    cmsUInt32Number count = cmsGetSupportedIntents(0, NULL, NULL);
    if (count > 0) {
        cmsUInt32Number *codes = (cmsUInt32Number *)malloc(count * sizeof(cmsUInt32Number));
        char **descriptions = (char **)malloc(count * sizeof(char *));
        if (codes && descriptions) {
            cmsGetSupportedIntents(count, codes, descriptions);
            // Do not free descriptions[i] as they are internal strings owned by lcms
            free(codes);
            free(descriptions);
        }
    }

    if (ctx != NULL) {
        cmsUInt32Number countThr = cmsGetSupportedIntentsTHR(ctx, 0, NULL, NULL);
        if (countThr > 0) {
            cmsUInt32Number *codesThr = (cmsUInt32Number *)malloc(countThr * sizeof(cmsUInt32Number));
            char **descriptionsThr = (char **)malloc(countThr * sizeof(char *));
            if (codesThr && descriptionsThr) {
                cmsGetSupportedIntentsTHR(ctx, countThr, codesThr, descriptionsThr);
                // Do not free descriptionsThr[i] as they are internal strings owned by lcms
                free(codesThr);
                free(descriptionsThr);
            }
        }
    }

    if (profile_count >= 2) {
        cmsHPROFILE input = profiles[0];
        cmsHPROFILE output = profiles[1];
        cmsUInt32Number format = (Data[0] % 4) + 1;
        cmsUInt32Number intent = Data[1] % 4;
        cmsUInt32Number flags = Data[2] % 10;

        cmsHTRANSFORM t = cmsCreateTransform(input, format, output, format, intent, flags);
        if (t != NULL) {
            transforms[transform_count++] = t;
        }
    }

    if (profile_count >= 3) {
        cmsHPROFILE input = profiles[0];
        cmsHPROFILE output = profiles[1];
        cmsHPROFILE proofing = profiles[2];
        cmsUInt32Number format = (Data[3] % 4) + 1;
        cmsUInt32Number intent = Data[4] % 4;
        cmsUInt32Number proofingIntent = Data[5] % 4;
        cmsUInt32Number flags = Data[6] % 10;

        cmsHTRANSFORM t = cmsCreateProofingTransform(input, format, output, format, proofing, intent, proofingIntent, flags);
        if (t != NULL) {
            transforms[transform_count++] = t;
        }
    }

    if (profile_count >= 1) {
        cmsHPROFILE *hProfiles = profiles;
        cmsUInt32Number nProfiles = (Data[7] % profile_count) + 1;
        if (nProfiles > 255) nProfiles = 255;
        
        cmsUInt32Number format = (Data[8] % 4) + 1;
        cmsUInt32Number intent = Data[9] % 4;
        cmsUInt32Number flags = Data[10] % 10;

        cmsHTRANSFORM t = cmsCreateMultiprofileTransformTHR(ctx, hProfiles, nProfiles, format, format, intent, flags);
        if (t != NULL) {
            transforms[transform_count++] = t;
        }
    }

    cleanup_profiles(profiles, profile_count);
    cleanup_transforms(transforms, transform_count);
    
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }

    return 0;
}