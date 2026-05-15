// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_file(void) {
    unlink("./dummy_file");
}

static cmsHPROFILE create_test_profile(cmsContext ctx, const uint8_t *data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfileTHR(ctx);
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ctx, data, (cmsUInt32Number)(size > 65535 ? 65535 : size));
    if (hProfile == NULL) {
        hProfile = cmsCreate_sRGBProfileTHR(ctx);
    }
    return hProfile;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        cleanup_file();
        return 0;
    }
    
    uint8_t selector = Data[0] % 6;
    const uint8_t *profile_data = Data + 1;
    size_t profile_size = Size > 1 ? Size - 1 : 0;
    
    switch (selector) {
        case 0: {
            cmsUInt32Number nProfiles = (Data[0] % 254) + 1;
            cmsHPROFILE *hProfiles = (cmsHPROFILE*)malloc(sizeof(cmsHPROFILE) * nProfiles);
            if (!hProfiles) break;
            
            for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                hProfiles[i] = create_test_profile(ctx, profile_data, profile_size);
            }
            
            cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(
                hProfiles,
                nProfiles,
                TYPE_RGB_8,
                TYPE_RGB_8,
                INTENT_PERCEPTUAL,
                cmsFLAGS_NOCACHE
            );
            
            if (transform) cmsDeleteTransform(transform);
            
            for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                if (hProfiles[i]) cmsCloseProfile(hProfiles[i]);
            }
            free(hProfiles);
            break;
        }
        
        case 1: {
            cmsHPROFILE input = create_test_profile(ctx, profile_data, profile_size);
            cmsHPROFILE output = create_test_profile(ctx, profile_data + 128, profile_size > 128 ? profile_size - 128 : 0);
            cmsHPROFILE proofing = create_test_profile(ctx, profile_data + 256, profile_size > 256 ? profile_size - 256 : 0);
            
            cmsHTRANSFORM transform = cmsCreateProofingTransform(
                input,
                TYPE_RGB_8,
                output,
                TYPE_RGB_8,
                proofing,
                INTENT_PERCEPTUAL,
                INTENT_RELATIVE_COLORIMETRIC,
                cmsFLAGS_SOFTPROOFING
            );
            
            if (transform) cmsDeleteTransform(transform);
            
            if (input) cmsCloseProfile(input);
            if (output) cmsCloseProfile(output);
            if (proofing) cmsCloseProfile(proofing);
            break;
        }
        
        case 2: {
            cmsHPROFILE profile = cmsOpenProfileFromMemTHR(ctx, Data, (cmsUInt32Number)(Size > 65535 ? 65535 : Size));
            if (profile) cmsCloseProfile(profile);
            break;
        }
        
        case 3: {
            cmsUInt32Number nProfiles = (Data[0] % 10) + 1;
            cmsHPROFILE *hProfiles = (cmsHPROFILE*)malloc(sizeof(cmsHPROFILE) * nProfiles);
            cmsBool *BPC = (cmsBool*)malloc(sizeof(cmsBool) * nProfiles);
            cmsUInt32Number *Intents = (cmsUInt32Number*)malloc(sizeof(cmsUInt32Number) * nProfiles);
            cmsFloat64Number *AdaptationStates = (cmsFloat64Number*)malloc(sizeof(cmsFloat64Number) * nProfiles);
            
            if (!hProfiles || !BPC || !Intents || !AdaptationStates) {
                free(hProfiles);
                free(BPC);
                free(Intents);
                free(AdaptationStates);
                break;
            }
            
            for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                hProfiles[i] = create_test_profile(ctx, profile_data, profile_size);
                BPC[i] = (Data[i % Size] & 1) ? TRUE : FALSE;
                Intents[i] = Data[i % Size] % 4;
                AdaptationStates[i] = (cmsFloat64Number)(Data[i % Size] / 255.0);
            }
            
            cmsHPROFILE gamutProfile = create_test_profile(ctx, profile_data + 512, profile_size > 512 ? profile_size - 512 : 0);
            
            cmsHTRANSFORM transform = cmsCreateExtendedTransform(
                ctx,
                nProfiles,
                hProfiles,
                BPC,
                Intents,
                AdaptationStates,
                gamutProfile,
                0,
                TYPE_RGB_8,
                TYPE_RGB_8,
                cmsFLAGS_GAMUTCHECK
            );
            
            if (transform) cmsDeleteTransform(transform);
            
            for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                if (hProfiles[i]) cmsCloseProfile(hProfiles[i]);
            }
            if (gamutProfile) cmsCloseProfile(gamutProfile);
            
            free(hProfiles);
            free(BPC);
            free(Intents);
            free(AdaptationStates);
            break;
        }
        
        case 4: {
            cmsHPROFILE input = create_test_profile(ctx, profile_data, profile_size);
            cmsHPROFILE output = create_test_profile(ctx, profile_data + 128, profile_size > 128 ? profile_size - 128 : 0);
            
            cmsHTRANSFORM transform = cmsCreateTransformTHR(
                ctx,
                input,
                TYPE_RGB_8,
                output,
                TYPE_RGB_8,
                INTENT_PERCEPTUAL,
                cmsFLAGS_NOCACHE
            );
            
            if (transform) cmsDeleteTransform(transform);
            
            if (input) cmsCloseProfile(input);
            if (output) cmsCloseProfile(output);
            break;
        }
        
        case 5: {
            cmsUInt32Number nProfiles = (Data[0] % 254) + 1;
            cmsHPROFILE *hProfiles = (cmsHPROFILE*)malloc(sizeof(cmsHPROFILE) * nProfiles);
            if (!hProfiles) break;
            
            for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                hProfiles[i] = create_test_profile(ctx, profile_data, profile_size);
            }
            
            cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
                ctx,
                hProfiles,
                nProfiles,
                TYPE_RGB_8,
                TYPE_RGB_8,
                INTENT_PERCEPTUAL,
                cmsFLAGS_NOCACHE
            );
            
            if (transform) cmsDeleteTransform(transform);
            
            for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                if (hProfiles[i]) cmsCloseProfile(hProfiles[i]);
            }
            free(hProfiles);
            break;
        }
    }
    
    cmsDeleteContext(ctx);
    cleanup_file();
    return 0;
}