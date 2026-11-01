// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profiles(cmsHPROFILE profiles[], size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (profiles[i] != NULL) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = NULL;
        }
    }
}

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 128) return NULL;

    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return NULL;

    size_t written = fwrite(data, 1, size, f);
    fclose(f);

    if (written != size) return NULL;

    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    return profile;
}

static cmsHPROFILE create_valid_profile(void) {
    return cmsCreate_sRGBProfile();
}

static uint8_t safe_read_byte(const uint8_t *Data, size_t Size, size_t index) {
    if (index < Size) {
        return Data[index];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    cmsHPROFILE profiles[256] = {0};
    cmsUInt32Number codes[256];
    char *descriptions[256] = {0};

    size_t chunk_size = Size / 10;
    if (chunk_size < 128) chunk_size = 128;

    for (int i = 0; i < 5 && i < 256; i++) {
        if (Size >= (i + 1) * chunk_size) {
            profiles[i] = create_dummy_profile(Data + i * chunk_size, chunk_size);
        } else {
            profiles[i] = create_valid_profile();
        }
    }

    cmsUInt32Number count = cmsGetSupportedIntents(0, NULL, NULL);
    if (count > 0 && count <= 256) {
        cmsGetSupportedIntents(count, codes, descriptions);
    }

    count = cmsGetSupportedIntentsTHR(ContextID, 0, NULL, NULL);
    if (count > 0 && count <= 256) {
        cmsGetSupportedIntentsTHR(ContextID, count, codes, descriptions);
    }

    for (int i = 0; i < 5; i++) {
        if (profiles[i] != NULL) {
            cmsGetHeaderRenderingIntent(profiles[i]);
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (profiles[i] != NULL && profiles[j] != NULL) {
                cmsUInt32Number intent = safe_read_byte(Data, Size, 0) % 4;
                cmsUInt32Number flags = safe_read_byte(Data, Size, 1) % 100;
                cmsHTRANSFORM t = cmsCreateTransformTHR(
                    ContextID,
                    profiles[i],
                    (cmsUInt32Number)(safe_read_byte(Data, Size, 2) % 10),
                    profiles[j],
                    (cmsUInt32Number)(safe_read_byte(Data, Size, 3) % 10),
                    intent,
                    flags
                );
                if (t != NULL) {
                    cmsDeleteTransform(t);
                }
            }
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                if (profiles[i] != NULL && profiles[j] != NULL && profiles[k] != NULL) {
                    cmsUInt32Number intent = safe_read_byte(Data, Size, 4) % 4;
                    cmsUInt32Number proofingIntent = safe_read_byte(Data, Size, 5) % 4;
                    cmsUInt32Number flags = safe_read_byte(Data, Size, 6) % 100;
                    cmsHTRANSFORM t = cmsCreateProofingTransform(
                        profiles[i],
                        (cmsUInt32Number)(safe_read_byte(Data, Size, 7) % 10),
                        profiles[j],
                        (cmsUInt32Number)(safe_read_byte(Data, Size, 8) % 10),
                        profiles[k],
                        intent,
                        proofingIntent,
                        flags
                    );
                    if (t != NULL) {
                        cmsDeleteTransform(t);
                    }
                }
            }
        }
    }

    for (int n = 1; n <= 5; n++) {
        cmsHTRANSFORM t = cmsCreateMultiprofileTransformTHR(
            ContextID,
            profiles,
            n,
            (cmsUInt32Number)(safe_read_byte(Data, Size, 9) % 10),
            (cmsUInt32Number)(safe_read_byte(Data, Size, 10) % 10),
            safe_read_byte(Data, Size, 11) % 4,
            safe_read_byte(Data, Size, 12) % 100
        );
        if (t != NULL) {
            cmsDeleteTransform(t);
        }
    }

    cleanup_profiles(profiles, 256);
    cmsDeleteContext(ContextID);

    return 0;
}