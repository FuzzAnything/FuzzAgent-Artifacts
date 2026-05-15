// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
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

static void dummy_free(void* ptr) {
    // Dummy free function for profiles
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a minimal dummy profile for testing
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(ctx);
    if (hProfile) {
        // Set minimal required tags
        cmsSetProfileVersion(hProfile, 4.2);
        cmsSetDeviceClass(hProfile, cmsSigInputClass);
        cmsSetColorSpace(hProfile, cmsSigRgbData);
        cmsSetPCS(hProfile, cmsSigXYZData);
    }
    return hProfile;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create dummy profiles
    cmsHPROFILE profiles[3];
    for (int i = 0; i < 3; i++) {
        profiles[i] = create_dummy_profile(ctx);
        if (!profiles[i]) {
            for (int j = 0; j < i; j++) {
                cmsCloseProfile(profiles[j]);
            }
            cmsDeleteContext(ctx);
            return 0;
        }
    }
    
    // Use first byte to select which function to test
    uint8_t selector = Data[0] % 6;
    Data++; Size--;
    
    switch (selector) {
        case 0: { // _cmsDefaultICCintents
            if (Size < 4) break;
            
            cmsUInt32Number nProfiles = (Data[0] % 2) + 1; // 1-2 profiles
            cmsUInt32Number* intents = (cmsUInt32Number*)malloc(nProfiles * sizeof(cmsUInt32Number));
            cmsHPROFILE* hProfs = (cmsHPROFILE*)malloc(nProfiles * sizeof(cmsHPROFILE));
            cmsBool* bpc = (cmsBool*)malloc(nProfiles * sizeof(cmsBool));
            cmsFloat64Number* adapt = (cmsFloat64Number*)malloc(nProfiles * sizeof(cmsFloat64Number));
            
            if (intents && hProfs && bpc && adapt) {
                for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                    intents[i] = Data[i % Size] % 4; // 0-3 intents
                    hProfs[i] = profiles[i % 3];
                    bpc[i] = (Data[(i + 1) % Size] & 1) ? TRUE : FALSE;
                    adapt[i] = (cmsFloat64Number)(Data[(i + 2) % Size] / 255.0);
                }
                
                cmsUInt32Number flags = Data[Size-1] % 256;
                cmsPipeline* pipeline = _cmsDefaultICCintents(ctx, nProfiles, intents, hProfs, bpc, adapt, flags);
                if (pipeline) {
                    cmsPipelineFree(pipeline);
                }
            }
            
            free(intents);
            free(hProfs);
            free(bpc);
            free(adapt);
            break;
        }
        
        case 1: { // cmsCreateProofingTransform
            if (Size < 4) break;
            
            cmsUInt32Number inputFormat = Data[0] % 8;
            cmsUInt32Number outputFormat = Data[1] % 8;
            cmsUInt32Number intent = Data[2] % 4;
            cmsUInt32Number proofingIntent = Data[3] % 4;
            cmsUInt32Number flags = (Size > 4) ? Data[4] % 256 : 0;
            
            cmsHTRANSFORM transform = cmsCreateProofingTransform(
                profiles[0], inputFormat,
                profiles[1], outputFormat,
                profiles[2], intent, proofingIntent, flags);
            
            if (transform) {
                cmsDeleteTransform(transform);
            }
            break;
        }
        
        case 2: { // cmsIsIntentSupported
            if (Size < 2) break;
            
            cmsUInt32Number intent = Data[0] % 4;
            cmsUInt32Number direction = Data[1] % 2; // 0 or 1
            
            for (int i = 0; i < 3; i++) {
                cmsBool supported = cmsIsIntentSupported(profiles[i], intent, direction);
                (void)supported; // Use result to avoid unused warning
            }
            break;
        }
        
        case 3: { // cmsGetSupportedIntents
            // First call to get count
            cmsUInt32Number count = cmsGetSupportedIntents(0, NULL, NULL);
            
            if (count > 0) {
                cmsUInt32Number* codes = (cmsUInt32Number*)malloc(count * sizeof(cmsUInt32Number));
                char** descriptions = (char**)malloc(count * sizeof(char*));
                
                if (codes && descriptions) {
                    // Allocate memory for each description string
                    for (cmsUInt32Number i = 0; i < count; i++) {
                        descriptions[i] = (char*)malloc(256 * sizeof(char));
                    }
                    
                    cmsUInt32Number actual = cmsGetSupportedIntents(count, codes, descriptions);
                    (void)actual; // Use result
                    
                    // Free allocated description strings
                    for (cmsUInt32Number i = 0; i < count; i++) {
                        free(descriptions[i]);
                    }
                }
                
                free(codes);
                free(descriptions);
            }
            break;
        }
        
        case 4: { // cmsCreateTransform
            if (Size < 3) break;
            
            cmsUInt32Number inputFormat = Data[0] % 8;
            cmsUInt32Number outputFormat = Data[1] % 8;
            cmsUInt32Number intent = Data[2] % 4;
            cmsUInt32Number flags = (Size > 3) ? Data[3] % 256 : 0;
            
            cmsHTRANSFORM transform = cmsCreateTransform(
                profiles[0], inputFormat,
                profiles[1], outputFormat,
                intent, flags);
            
            if (transform) {
                cmsDeleteTransform(transform);
            }
            break;
        }
        
        case 5: { // cmsGetSupportedIntentsTHR
            // First call to get count
            cmsUInt32Number count = cmsGetSupportedIntentsTHR(ctx, 0, NULL, NULL);
            
            if (count > 0) {
                cmsUInt32Number* codes = (cmsUInt32Number*)malloc(count * sizeof(cmsUInt32Number));
                char** descriptions = (char**)malloc(count * sizeof(char*));
                
                if (codes && descriptions) {
                    // Allocate memory for each description string
                    for (cmsUInt32Number i = 0; i < count; i++) {
                        descriptions[i] = (char*)malloc(256 * sizeof(char));
                    }
                    
                    cmsUInt32Number actual = cmsGetSupportedIntentsTHR(ctx, count, codes, descriptions);
                    (void)actual; // Use result
                    
                    // Free allocated description strings
                    for (cmsUInt32Number i = 0; i < count; i++) {
                        free(descriptions[i]);
                    }
                }
                
                free(codes);
                free(descriptions);
            }
            break;
        }
    }
    
    // Cleanup
    for (int i = 0; i < 3; i++) {
        if (profiles[i]) {
            cmsCloseProfile(profiles[i]);
        }
    }
    
    cmsDeleteContext(ctx);
    return 0;
}