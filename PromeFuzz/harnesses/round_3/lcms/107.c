// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_dummy_profile(const uint8_t* data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE* file = fopen("./dummy_file", "wb");
    if (!file) {
        return cmsCreate_sRGBProfile();
    }
    
    fwrite(data, 1, size > 1024 ? 1024 : size, file);
    fclose(file);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    
    remove("./dummy_file");
    return profile;
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform) {
        cmsDeleteTransform(transform);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_context(cmsContext context) {
    if (context) {
        cmsDeleteContext(context);
    }
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Create dummy profiles for testing
    cmsHPROFILE dummy_profile = create_dummy_profile(Data, Size);
    cmsHPROFILE dummy_profile2 = create_dummy_profile(Data + (Size > 100 ? 100 : 0), 
                                                     Size > 100 ? Size - 100 : Size);
    cmsHPROFILE dummy_profile3 = create_dummy_profile(Data + (Size > 200 ? 200 : 0), 
                                                     Size > 200 ? Size - 200 : Size);
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Test cmsFormatterForColorspaceOfProfile
    for (int i = 0; i < 4; i++) {
        cmsUInt32Number nBytes = (i & 1) ? 1 : 2;
        cmsBool lIsFloat = (i & 2) ? 1 : 0;
        cmsFormatterForColorspaceOfProfile(dummy_profile, nBytes, lIsFloat);
    }
    
    // Test cmsFormatterForPCSOfProfile
    for (int i = 0; i < 4; i++) {
        cmsUInt32Number nBytes = (i & 1) ? 1 : 2;
        cmsBool lIsFloat = (i & 2) ? 1 : 0;
        cmsFormatterForPCSOfProfile(dummy_profile, nBytes, lIsFloat);
    }
    
    // Test cmsCreateMultiprofileTransform
    cmsHPROFILE profiles1[3] = {dummy_profile, dummy_profile2, dummy_profile3};
    cmsUInt32Number nProfiles1 = Size % 3 + 1;
    cmsUInt32Number InputFormat1 = Size % 0xFFFF;
    cmsUInt32Number OutputFormat1 = (Size + 1) % 0xFFFF;
    cmsUInt32Number Intent1 = Size % 4;
    cmsUInt32Number dwFlags1 = Size % 0xFF;
    
    cmsHTRANSFORM transform1 = cmsCreateMultiprofileTransform(
        profiles1, nProfiles1, InputFormat1, OutputFormat1, Intent1, dwFlags1);
    cleanup_transform(transform1);
    
    // Test cmsCreateMultiprofileTransformTHR
    cmsHTRANSFORM transform2 = cmsCreateMultiprofileTransformTHR(
        context, profiles1, nProfiles1, InputFormat1, OutputFormat1, Intent1, dwFlags1);
    cleanup_transform(transform2);
    
    // Test cmsCreateExtendedTransform
    cmsBool BPC[3] = {Size & 1, (Size >> 1) & 1, (Size >> 2) & 1};
    cmsUInt32Number Intents[3] = {Size % 4, (Size + 1) % 4, (Size + 2) % 4};
    cmsFloat64Number AdaptationStates[3] = {(double)(Size % 100) / 100.0, 
                                           (double)((Size + 1) % 100) / 100.0, 
                                           (double)((Size + 2) % 100) / 100.0};
    cmsUInt32Number nGamutPCSposition = Size % 3;
    cmsUInt32Number nProfiles2 = Size % 3 + 1;
    
    cmsHTRANSFORM transform3 = cmsCreateExtendedTransform(
        context, nProfiles2, profiles1, BPC, Intents, AdaptationStates,
        dummy_profile, nGamutPCSposition, InputFormat1, OutputFormat1, dwFlags1);
    cleanup_transform(transform3);
    
    // Test cmsCreateProofingTransformTHR
    cmsUInt32Number InputFormat2 = Size % 0xFFFF;
    cmsUInt32Number OutputFormat2 = (Size + 2) % 0xFFFF;
    cmsUInt32Number ProofingIntent = (Size + 1) % 4;
    cmsUInt32Number dwFlags2 = (Size + 3) % 0xFF;
    
    cmsHTRANSFORM transform4 = cmsCreateProofingTransformTHR(
        context, dummy_profile, InputFormat2, dummy_profile2, OutputFormat2,
        dummy_profile3, Intent1, ProofingIntent, dwFlags2);
    cleanup_transform(transform4);
    
    // Cleanup
    cleanup_profile(dummy_profile);
    cleanup_profile(dummy_profile2);
    cleanup_profile(dummy_profile3);
    cleanup_context(context);
    
    return 0;
}