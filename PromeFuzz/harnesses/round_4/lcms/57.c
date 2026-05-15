// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
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
#include <stdio.h>

static cmsHPROFILE create_dummy_profile(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfileTHR(ctx);
    }
    
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return cmsCreate_sRGBProfileTHR(ctx);
    
    fwrite(data, 1, size > 4096 ? 4096 : size, f);
    fclose(f);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreate_sRGBProfileTHR(ctx);
    }
    
    remove("./dummy_file");
    return profile;
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) cmsCloseProfile(profile);
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform) cmsDeleteTransform(transform);
}

static void cleanup_pipeline(cmsPipeline* pipeline) {
    if (pipeline) cmsPipelineFree(pipeline);
}

int LLVMFuzzerTestOneInput_57(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    uint32_t selector = Data[0] % 6;
    Data++;
    Size--;
    
    switch (selector) {
        case 0: {
            if (Size < 16) break;
            
            cmsUInt32Number nProfiles = 2;
            cmsUInt32Number Intents[2];
            cmsHPROFILE hProfiles[2];
            cmsBool BPC[2];
            cmsFloat64Number AdaptationStates[2];
            
            for (int i = 0; i < 2; i++) {
                Intents[i] = Data[i] % 4;
                BPC[i] = Data[i + 2] & 1;
                AdaptationStates[i] = (double)Data[i + 4] / 255.0;
            }
            
            size_t chunk_size = Size / 2;
            hProfiles[0] = create_dummy_profile(ctx, Data + 8, chunk_size);
            hProfiles[1] = create_dummy_profile(ctx, Data + 8 + chunk_size, Size - 8 - chunk_size);
            
            cmsUInt32Number dwFlags = *(uint32_t*)(Data + 4) & 0xFF;
            
            cmsPipeline* pipeline = _cmsDefaultICCintents(ctx, nProfiles, Intents, hProfiles, BPC, AdaptationStates, dwFlags);
            cleanup_pipeline(pipeline);
            
            for (int i = 0; i < 2; i++) {
                cleanup_profile(hProfiles[i]);
            }
            break;
        }
        
        case 1: {
            if (Size < 32) break;
            
            size_t chunk_size = Size / 3;
            cmsHPROFILE Input = create_dummy_profile(ctx, Data, chunk_size);
            cmsHPROFILE Output = create_dummy_profile(ctx, Data + chunk_size, chunk_size);
            cmsHPROFILE Proofing = create_dummy_profile(ctx, Data + 2 * chunk_size, Size - 2 * chunk_size);
            
            cmsUInt32Number InputFormat = *(uint32_t*)Data & 0xF;
            cmsUInt32Number OutputFormat = *(uint32_t*)(Data + 4) & 0xF;
            cmsUInt32Number Intent = Data[8] % 4;
            cmsUInt32Number ProofingIntent = Data[9] % 4;
            cmsUInt32Number dwFlags = *(uint32_t*)(Data + 12) & 0xFF;
            
            cmsHTRANSFORM transform = cmsCreateProofingTransform(Input, InputFormat, Output, OutputFormat, Proofing, Intent, ProofingIntent, dwFlags);
            cleanup_transform(transform);
            
            cleanup_profile(Input);
            cleanup_profile(Output);
            cleanup_profile(Proofing);
            break;
        }
        
        case 2: {
            if (Size < 8) break;
            
            cmsHPROFILE profile = create_dummy_profile(ctx, Data, Size);
            cmsUInt32Number Intent = Data[0] % 4;
            cmsUInt32Number UsedDirection = Data[1] & 1;
            
            cmsBool result = cmsIsIntentSupported(profile, Intent, UsedDirection);
            (void)result;
            
            cleanup_profile(profile);
            break;
        }
        
        case 3: {
            cmsUInt32Number count = cmsGetSupportedIntents(0, NULL, NULL);
            if (count > 0 && count < 100) {
                cmsUInt32Number* codes = (cmsUInt32Number*)malloc(count * sizeof(cmsUInt32Number));
                char* buffer = (char*)malloc(count * 256);
                
                if (codes && buffer) {
                    char** descriptions = (char**)malloc(count * sizeof(char*));
                    if (descriptions) {
                        for (cmsUInt32Number i = 0; i < count; i++) {
                            descriptions[i] = buffer + (i * 256);
                        }
                        
                        cmsUInt32Number actual = cmsGetSupportedIntents(count, codes, descriptions);
                        (void)actual;
                        
                        free(descriptions);
                    }
                }
                
                free(codes);
                free(buffer);
            }
            break;
        }
        
        case 4: {
            if (Size < 16) break;
            
            size_t chunk_size = Size / 2;
            cmsHPROFILE Input = create_dummy_profile(ctx, Data, chunk_size);
            cmsHPROFILE Output = create_dummy_profile(ctx, Data + chunk_size, Size - chunk_size);
            
            cmsUInt32Number InputFormat = *(uint32_t*)Data & 0xF;
            cmsUInt32Number OutputFormat = *(uint32_t*)(Data + 4) & 0xF;
            cmsUInt32Number Intent = Data[8] % 4;
            cmsUInt32Number dwFlags = *(uint32_t*)(Data + 12) & 0xFF;
            
            cmsHTRANSFORM transform = cmsCreateTransform(Input, InputFormat, Output, OutputFormat, Intent, dwFlags);
            cleanup_transform(transform);
            
            cleanup_profile(Input);
            cleanup_profile(Output);
            break;
        }
        
        case 5: {
            cmsUInt32Number count = cmsGetSupportedIntentsTHR(ctx, 0, NULL, NULL);
            if (count > 0 && count < 100) {
                cmsUInt32Number* codes = (cmsUInt32Number*)malloc(count * sizeof(cmsUInt32Number));
                char* buffer = (char*)malloc(count * 256);
                
                if (codes && buffer) {
                    char** descriptions = (char**)malloc(count * sizeof(char*));
                    if (descriptions) {
                        for (cmsUInt32Number i = 0; i < count; i++) {
                            descriptions[i] = buffer + (i * 256);
                        }
                        
                        cmsUInt32Number actual = cmsGetSupportedIntentsTHR(ctx, count, codes, descriptions);
                        (void)actual;
                        
                        free(descriptions);
                    }
                }
                
                free(codes);
                free(buffer);
            }
            break;
        }
    }
    
    cmsDeleteContext(ctx);
    return 0;
}