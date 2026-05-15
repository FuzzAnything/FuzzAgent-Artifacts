// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_dummy_profile_from_data(const uint8_t* data, size_t size) {
    if (size == 0) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE* file = fopen("./dummy_file", "wb");
    if (!file) {
        return cmsCreate_sRGBProfile();
    }
    
    size_t write_size = size > 4096 ? 4096 : size;
    if (write_size > 0) {
        fwrite(data, 1, write_size, file);
    }
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

static void cleanup_pipeline(cmsPipeline* pipeline) {
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    uint8_t selector = Data[0] % 6;
    const uint8_t* profile_data = Data + 1;
    size_t profile_size = Size > 1 ? Size - 1 : 0;
    
    switch (selector) {
        case 0: {  // cmsCreateMultiprofileTransform
            if (profile_size < 4) break;
            
            cmsUInt32Number nProfiles = (profile_data[0] % 254) + 1;
            cmsHPROFILE* profiles = (cmsHPROFILE*)malloc(sizeof(cmsHPROFILE) * nProfiles);
            if (!profiles) break;
            
            for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                profiles[i] = create_dummy_profile_from_data(profile_data, profile_size);
            }
            
            cmsUInt32Number InputFormat = profile_data[1] % 4;
            cmsUInt32Number OutputFormat = profile_data[2] % 4;
            cmsUInt32Number Intent = profile_data[3] % 4;
            cmsUInt32Number dwFlags = profile_data[0] % 256;
            
            cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(
                profiles, nProfiles, InputFormat, OutputFormat, Intent, dwFlags);
            
            cleanup_transform(transform);
            
            for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                cleanup_profile(profiles[i]);
            }
            free(profiles);
            break;
        }
        
        case 1: {  // cmsCreateProofingTransform
            if (profile_size < 5) break;
            
            cmsHPROFILE Input = create_dummy_profile_from_data(profile_data, profile_size);
            cmsHPROFILE Output = create_dummy_profile_from_data(profile_data + 1, 
                profile_size > 1 ? profile_size - 1 : 0);
            cmsHPROFILE Proofing = create_dummy_profile_from_data(profile_data + 2, 
                profile_size > 2 ? profile_size - 2 : 0);
            
            cmsUInt32Number InputFormat = profile_data[0] % 4;
            cmsUInt32Number OutputFormat = profile_data[1] % 4;
            cmsUInt32Number Intent = profile_data[2] % 4;
            cmsUInt32Number ProofingIntent = profile_data[3] % 4;
            cmsUInt32Number dwFlags = profile_data[4] % 256;
            
            cmsHTRANSFORM transform = cmsCreateProofingTransform(
                Input, InputFormat, Output, OutputFormat, Proofing, 
                Intent, ProofingIntent, dwFlags);
            
            cleanup_transform(transform);
            cleanup_profile(Input);
            cleanup_profile(Output);
            cleanup_profile(Proofing);
            break;
        }
        
        case 2: {  // cmsCreateExtendedTransform
            if (profile_size < 8) break;
            
            cmsContext context = cmsCreateContext(NULL, NULL);
            if (!context) break;
            
            cmsUInt32Number nProfiles = (profile_data[0] % 10) + 1;
            if (nProfiles > profile_size) nProfiles = profile_size;
            
            cmsHPROFILE* profiles = (cmsHPROFILE*)malloc(sizeof(cmsHPROFILE) * nProfiles);
            cmsBool* BPC = (cmsBool*)malloc(sizeof(cmsBool) * nProfiles);
            cmsUInt32Number* Intents = (cmsUInt32Number*)malloc(sizeof(cmsUInt32Number) * nProfiles);
            cmsFloat64Number* AdaptationStates = (cmsFloat64Number*)malloc(sizeof(cmsFloat64Number) * nProfiles);
            
            if (!profiles || !BPC || !Intents || !AdaptationStates) {
                free(profiles); free(BPC); free(Intents); free(AdaptationStates);
                cmsDeleteContext(context);
                break;
            }
            
            for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                profiles[i] = create_dummy_profile_from_data(profile_data + i, 
                    profile_size > i ? profile_size - i : 0);
                BPC[i] = (profile_data[i] % 2) ? TRUE : FALSE;
                Intents[i] = profile_data[i] % 4;
                AdaptationStates[i] = (cmsFloat64Number)(profile_data[i] % 100) / 100.0;
            }
            
            cmsHPROFILE hGamutProfile = create_dummy_profile_from_data(profile_data + nProfiles, 
                profile_size > nProfiles ? profile_size - nProfiles : 0);
            cmsUInt32Number nGamutPCSposition = profile_size > 1 ? (profile_data[1] % nProfiles) : 0;
            cmsUInt32Number InputFormat = profile_size > 2 ? (profile_data[2] % 4) : 0;
            cmsUInt32Number OutputFormat = profile_size > 3 ? (profile_data[3] % 4) : 0;
            cmsUInt32Number dwFlags = profile_size > 4 ? (profile_data[4] % 256) : 0;
            
            cmsHTRANSFORM transform = cmsCreateExtendedTransform(
                context, nProfiles, profiles, BPC, Intents, AdaptationStates,
                hGamutProfile, nGamutPCSposition, InputFormat, OutputFormat, dwFlags);
            
            cleanup_transform(transform);
            cleanup_profile(hGamutProfile);
            
            for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                cleanup_profile(profiles[i]);
            }
            
            free(profiles);
            free(BPC);
            free(Intents);
            free(AdaptationStates);
            cmsDeleteContext(context);
            break;
        }
        
        case 3: {  // _cmsDefaultICCintents
            if (profile_size < 4) break;
            
            cmsContext context = cmsCreateContext(NULL, NULL);
            if (!context) break;
            
            cmsUInt32Number nProfiles = (profile_data[0] % 10) + 1;
            if (nProfiles > profile_size) nProfiles = profile_size;
            
            cmsUInt32Number* Intents = (cmsUInt32Number*)malloc(sizeof(cmsUInt32Number) * nProfiles);
            cmsHPROFILE* profiles = (cmsHPROFILE*)malloc(sizeof(cmsHPROFILE) * nProfiles);
            cmsBool* BPC = (cmsBool*)malloc(sizeof(cmsBool) * nProfiles);
            cmsFloat64Number* AdaptationStates = (cmsFloat64Number*)malloc(sizeof(cmsFloat64Number) * nProfiles);
            
            if (!Intents || !profiles || !BPC || !AdaptationStates) {
                free(Intents); free(profiles); free(BPC); free(AdaptationStates);
                cmsDeleteContext(context);
                break;
            }
            
            for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                Intents[i] = profile_data[i] % 4;
                profiles[i] = create_dummy_profile_from_data(profile_data + i, 
                    profile_size > i ? profile_size - i : 0);
                BPC[i] = (profile_data[i] % 2) ? TRUE : FALSE;
                AdaptationStates[i] = (cmsFloat64Number)(profile_data[i] % 100) / 100.0;
            }
            
            cmsUInt32Number dwFlags = profile_size > 1 ? (profile_data[1] % 256) : 0;
            
            cmsPipeline* pipeline = _cmsDefaultICCintents(
                context, nProfiles, Intents, profiles, BPC, AdaptationStates, dwFlags);
            
            cleanup_pipeline(pipeline);
            
            for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                cleanup_profile(profiles[i]);
            }
            
            free(Intents);
            free(profiles);
            free(BPC);
            free(AdaptationStates);
            cmsDeleteContext(context);
            break;
        }
        
        case 4: {  // cmsIsIntentSupported
            if (profile_size < 2) break;
            
            cmsHPROFILE profile = create_dummy_profile_from_data(profile_data, profile_size);
            cmsUInt32Number Intent = profile_data[0] % 4;
            cmsUInt32Number UsedDirection = profile_data[1] % 2;
            
            cmsBool result = cmsIsIntentSupported(profile, Intent, UsedDirection);
            (void)result;
            
            cleanup_profile(profile);
            break;
        }
        
        case 5: {  // cmsIsCLUT
            if (profile_size < 2) break;
            
            cmsHPROFILE profile = create_dummy_profile_from_data(profile_data, profile_size);
            cmsUInt32Number Intent = profile_data[0] % 4;
            cmsUInt32Number UsedDirection = profile_data[1] % 2;
            
            cmsBool result = cmsIsCLUT(profile, Intent, UsedDirection);
            (void)result;
            
            cleanup_profile(profile);
            break;
        }
    }
    
    return 0;
}