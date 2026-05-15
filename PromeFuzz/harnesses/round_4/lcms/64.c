// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static cmsHPROFILE create_profile_from_data(const uint8_t *data, size_t size, cmsContext context) {
    if (size == 0) return NULL;
    
    // Try to create profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (hProfile) return hProfile;
    
    // If that fails, write to file and try to open
    write_dummy_file(data, size);
    hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    
    // If still fails, create a simple sRGB profile as fallback
    if (!hProfile) {
        hProfile = cmsCreate_sRGBProfileTHR(context);
    }
    
    return hProfile;
}

static void cleanup_transform(cmsHTRANSFORM hTransform) {
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile, cmsContext context) {
    if (hProfile) {
        // Only close if it's not the built-in sRGB profile from context
        if (context) {
            cmsCloseProfile(hProfile);
        }
    }
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Create profiles from fuzzer data
    size_t profile_size = Size / 4;
    cmsHPROFILE profiles[4] = {0};
    
    for (int i = 0; i < 4 && i * profile_size < Size; i++) {
        size_t offset = i * profile_size;
        size_t chunk_size = (i == 3) ? (Size - offset) : profile_size;
        profiles[i] = create_profile_from_data(Data + offset, chunk_size, context);
    }
    
    // Extract parameters from fuzzer data
    cmsUInt32Number nBytes = (Data[0] & 1) ? 1 : 2;
    cmsBool lIsFloat = (Data[1] & 1) ? 1 : 0;
    cmsUInt32Number intent = Data[2] % 4;
    cmsUInt32Number proofingIntent = Data[3] % 4;
    cmsUInt32Number dwFlags = Data[4];
    cmsUInt32Number inputFormat = Data[5] | (Data[6] << 8);
    cmsUInt32Number outputFormat = Data[7] | (Data[8] << 8);
    cmsUInt32Number nProfiles = (Data[9] % 4) + 1;
    
    // Test cmsFormatterForColorspaceOfProfile
    if (profiles[0]) {
        cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(profiles[0], nBytes, lIsFloat);
        (void)formatter; // Use result to avoid unused variable warning
    }
    
    // Test cmsFormatterForPCSOfProfile
    if (profiles[0]) {
        cmsUInt32Number pcsFormatter = cmsFormatterForPCSOfProfile(profiles[0], nBytes, lIsFloat);
        (void)pcsFormatter;
    }
    
    // Test cmsCreateTransform
    if (profiles[0] && profiles[1]) {
        cmsHTRANSFORM hTransform = cmsCreateTransform(profiles[0], inputFormat, 
                                                      profiles[1], outputFormat, 
                                                      intent, dwFlags);
        cleanup_transform(hTransform);
    }
    
    // Test cmsCreateMultiprofileTransform
    if (nProfiles > 0) {
        cmsHTRANSFORM hMultiTransform = cmsCreateMultiprofileTransform(profiles, nProfiles, 
                                                                       inputFormat, outputFormat, 
                                                                       intent, dwFlags);
        cleanup_transform(hMultiTransform);
    }
    
    // Test cmsCreateExtendedTransform
    if (nProfiles > 0 && profiles[0]) {
        cmsBool BPC[4] = {0};
        cmsUInt32Number Intents[4] = {0};
        cmsFloat64Number AdaptationStates[4] = {0.0};
        
        for (cmsUInt32Number i = 0; i < nProfiles; i++) {
            BPC[i] = (Data[10 + i] & 1) ? 1 : 0;
            Intents[i] = Data[11 + i] % 4;
            AdaptationStates[i] = (cmsFloat64Number)Data[12 + i] / 255.0;
        }
        
        cmsHTRANSFORM hExtended = cmsCreateExtendedTransform(context, nProfiles, profiles,
                                                             BPC, Intents, AdaptationStates,
                                                             NULL, 0, inputFormat, outputFormat, dwFlags);
        cleanup_transform(hExtended);
    }
    
    // Test cmsCreateProofingTransformTHR
    if (profiles[0] && profiles[1] && profiles[2]) {
        cmsHTRANSFORM hProofing = cmsCreateProofingTransformTHR(context, profiles[0], inputFormat,
                                                                profiles[1], outputFormat,
                                                                profiles[2], intent, 
                                                                proofingIntent, dwFlags);
        cleanup_transform(hProofing);
    }
    
    // Cleanup profiles
    for (int i = 0; i < 4; i++) {
        cleanup_profile(profiles[i], context);
    }
    
    // Cleanup context
    cmsDeleteContext(context);
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}