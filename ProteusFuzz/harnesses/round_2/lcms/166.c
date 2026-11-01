// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_transform(cmsHTRANSFORM hTransform) {
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

static cmsHPROFILE create_dummy_profile(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return NULL;
    }
    return cmsOpenProfileFromMem((const unsigned char *)Data, Size);
}

int LLVMFuzzerTestOneInput_166(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Split input data to potentially create multiple profiles
    size_t split1 = Size / 2;
    size_t split2 = Size / 4;
    size_t split3 = Size / 3;

    cmsHPROFILE prof1 = create_dummy_profile(Data, split1);
    cmsHPROFILE prof2 = create_dummy_profile(Data + split1, Size - split1);
    cmsHPROFILE prof3 = create_dummy_profile(Data + split2, Size - split2);
    cmsHPROFILE prof4 = create_dummy_profile(Data + split3, Size - split3);

    cmsHTRANSFORM hTransform = NULL;
    cmsHPROFILE hDeviceLink = NULL;

    // Test cmsSetHeaderRenderingIntent
    if (prof1) {
        cmsSetHeaderRenderingIntent(prof1, Data[0] % 4);
    }
    if (prof2) {
        cmsSetHeaderRenderingIntent(prof2, Data[1] % 4);
    }
    if (prof3) {
        cmsSetHeaderRenderingIntent(prof3, Data[2] % 4);
    }
    if (prof4) {
        cmsSetHeaderRenderingIntent(prof4, Data[3] % 4);
    }

    // Test cmsCreateTransform
    if (prof1 && prof2) {
        hTransform = cmsCreateTransform(prof1, Data[0] % 100 + 1, prof2, Data[1] % 100 + 1, Data[2] % 4, Data[3]);
        if (hTransform) {
            cleanup_transform(hTransform);
            hTransform = NULL;
        }
    }

    // Test cmsCreateProofingTransform
    if (prof1 && prof2 && prof3) {
        hTransform = cmsCreateProofingTransform(prof1, Data[0] % 100 + 1, prof2, Data[1] % 100 + 1, prof3, Data[2] % 4, Data[3] % 4, Data[4]);
        if (hTransform) {
            cleanup_transform(hTransform);
            hTransform = NULL;
        }
    }

    // Test cmsCreateMultiprofileTransform
    cmsHPROFILE profiles[255];
    int nProfiles = 0;
    if (prof1) profiles[nProfiles++] = prof1;
    if (prof2) profiles[nProfiles++] = prof2;
    if (prof3) profiles[nProfiles++] = prof3;
    if (prof4) profiles[nProfiles++] = prof4;

    if (nProfiles > 0 && nProfiles <= 255) {
        hTransform = cmsCreateMultiprofileTransform(profiles, nProfiles, Data[0] % 100 + 1, Data[1] % 100 + 1, Data[2] % 4, Data[3]);
        if (hTransform) {
            cleanup_transform(hTransform);
            hTransform = NULL;
        }
    }

    // Test cmsCreateExtendedTransform
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx && nProfiles > 0 && nProfiles <= 255) {
        cmsHPROFILE *hProfiles = malloc(nProfiles * sizeof(cmsHPROFILE));
        cmsBool *BPC = malloc(nProfiles * sizeof(cmsBool));
        cmsUInt32Number *Intents = malloc(nProfiles * sizeof(cmsUInt32Number));
        cmsFloat64Number *AdaptationStates = malloc(nProfiles * sizeof(cmsFloat64Number));

        if (hProfiles && BPC && Intents && AdaptationStates) {
            for (int i = 0; i < nProfiles; i++) {
                hProfiles[i] = profiles[i];
                BPC[i] = Data[i % Size] % 2;
                Intents[i] = Data[(i + 1) % Size] % 4;
                AdaptationStates[i] = (cmsFloat64Number)(Data[(i + 2) % Size]);
            }

            hTransform = cmsCreateExtendedTransform(ctx, nProfiles, hProfiles, BPC, Intents, AdaptationStates, prof4, 0, Data[0] % 100 + 1, Data[1] % 100 + 1, Data[2]);
            if (hTransform) {
                cleanup_transform(hTransform);
                hTransform = NULL;
            }
        }

        free(hProfiles);
        free(BPC);
        free(Intents);
        free(AdaptationStates);
    }
    if (ctx) {
        cmsDeleteContext(ctx);
    }

    // Create a transform specifically for cmsTransform2DeviceLink if possible
    // Use standard format constants from lcms2.h (e.g., 0 for undefined, 1 for 8-bit, etc.)
    // cmsSigU8DataFormat is typically defined as 0x00000000 or similar, but we use raw values to avoid header dependency issues
    // Common formats: 0x00000000 (undefined), 0x00000001 (8-bit), 0x00000002 (16-bit), etc.
    // We'll use 0x00000001 as a safe bet for 8-bit data format
    if (prof1 && prof2) {
        cmsUInt32Number format = 0x00000001; // Equivalent to cmsSigU8DataFormat usually
        hTransform = cmsCreateTransform(prof1, format, prof2, format, 0, 0);
        if (hTransform) {
            hDeviceLink = cmsTransform2DeviceLink(hTransform, 4.0, 0);
            if (hDeviceLink) {
                cleanup_profile(hDeviceLink);
            }
            cleanup_transform(hTransform);
        }
    }

    // Cleanup profiles
    cleanup_profile(prof1);
    cleanup_profile(prof2);
    cleanup_profile(prof3);
    cleanup_profile(prof4);

    return 0;
}