// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
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

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform) {
        cmsDeleteTransform(transform);
    }
}

static void cleanup_pipeline(cmsPipeline* pipeline) {
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
}

static void cleanup_context(cmsContext context) {
    if (context) {
        cmsDeleteContext(context);
    }
}

static cmsHPROFILE create_dummy_profile(const uint8_t* data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE* file = fopen("./dummy_file", "wb");
    if (!file) {
        return cmsCreate_sRGBProfile();
    }
    
    fwrite(data, 1, size > 4096 ? 4096 : size, file);
    fclose(file);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    
    remove("./dummy_file");
    return profile;
}

int LLVMFuzzerTestOneInput_117(const uint8_t* data, size_t size) {
    if (size < 4) {
        return 0;
    }

    // Initialize context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Create dummy profiles for testing
    cmsHPROFILE profiles[3];
    for (int i = 0; i < 3; i++) {
        profiles[i] = create_dummy_profile(data + i * 16, size > i * 16 ? size - i * 16 : 0);
    }

    // Extract parameters from fuzz data
    size_t offset = 0;
    
    // For cmsGetSupportedIntentsTHR
    if (size - offset >= 8) {
        cmsUInt32Number nMax = *(cmsUInt32Number*)(data + offset);
        offset += 4;
        cmsUInt32Number* codes = NULL;
        char** descriptions = NULL;
        
        if (nMax > 0 && nMax < 100) {
            codes = (cmsUInt32Number*)malloc(nMax * sizeof(cmsUInt32Number));
            descriptions = (char**)malloc(nMax * sizeof(char*));
            if (codes && descriptions) {
                for (cmsUInt32Number i = 0; i < nMax; i++) {
                    descriptions[i] = (char*)malloc(256);
                }
                cmsGetSupportedIntentsTHR(context, nMax, codes, descriptions);
                
                for (cmsUInt32Number i = 0; i < nMax; i++) {
                    free(descriptions[i]);
                }
            }
            free(codes);
            free(descriptions);
        }
    }

    // For cmsCreateTransform
    if (size - offset >= 16) {
        cmsUInt32Number inputFormat = *(cmsUInt32Number*)(data + offset);
        offset += 4;
        cmsUInt32Number outputFormat = *(cmsUInt32Number*)(data + offset);
        offset += 4;
        cmsUInt32Number intent = *(cmsUInt32Number*)(data + offset) % 5;
        offset += 4;
        cmsUInt32Number flags = *(cmsUInt32Number*)(data + offset);
        offset += 4;
        
        cmsHTRANSFORM transform = cmsCreateTransform(
            profiles[0], inputFormat,
            profiles[1], outputFormat,
            intent, flags
        );
        cleanup_transform(transform);
    }

    // For cmsIsIntentSupported
    if (size - offset >= 8) {
        cmsUInt32Number intent = *(cmsUInt32Number*)(data + offset) % 5;
        offset += 4;
        cmsUInt32Number direction = *(cmsUInt32Number*)(data + offset) % 4;
        offset += 4;
        
        cmsIsIntentSupported(profiles[0], intent, direction);
    }

    // For cmsCreateProofingTransform
    if (size - offset >= 20) {
        cmsUInt32Number inputFormat = *(cmsUInt32Number*)(data + offset);
        offset += 4;
        cmsUInt32Number outputFormat = *(cmsUInt32Number*)(data + offset);
        offset += 4;
        cmsUInt32Number intent = *(cmsUInt32Number*)(data + offset) % 5;
        offset += 4;
        cmsUInt32Number proofingIntent = *(cmsUInt32Number*)(data + offset) % 5;
        offset += 4;
        cmsUInt32Number flags = *(cmsUInt32Number*)(data + offset);
        offset += 4;
        
        cmsHTRANSFORM transform = cmsCreateProofingTransform(
            profiles[0], inputFormat,
            profiles[1], outputFormat,
            profiles[2],
            intent, proofingIntent, flags
        );
        cleanup_transform(transform);
    }

    // For _cmsDefaultICCintents
    if (size - offset >= 12) {
        cmsUInt32Number nProfiles = *(cmsUInt32Number*)(data + offset) % 4;
        offset += 4;
        
        if (nProfiles > 0 && nProfiles <= 3) {
            cmsUInt32Number* intents = (cmsUInt32Number*)malloc(nProfiles * sizeof(cmsUInt32Number));
            cmsBool* bpc = (cmsBool*)malloc(nProfiles * sizeof(cmsBool));
            cmsFloat64Number* adaptationStates = (cmsFloat64Number*)malloc(nProfiles * sizeof(cmsFloat64Number));
            
            if (intents && bpc && adaptationStates) {
                for (cmsUInt32Number i = 0; i < nProfiles; i++) {
                    intents[i] = *(cmsUInt32Number*)(data + offset) % 5;
                    offset = (offset + 4) % size;
                    bpc[i] = *(cmsUInt32Number*)(data + offset) % 2;
                    offset = (offset + 4) % size;
                    adaptationStates[i] = (cmsFloat64Number)(*(cmsUInt32Number*)(data + offset) % 1000) / 100.0;
                    offset = (offset + 4) % size;
                }
                
                cmsUInt32Number flags = *(cmsUInt32Number*)(data + offset);
                offset += 4;
                
                cmsPipeline* pipeline = _cmsDefaultICCintents(
                    context, nProfiles, intents, profiles, bpc, adaptationStates, flags
                );
                cleanup_pipeline(pipeline);
            }
            
            free(intents);
            free(bpc);
            free(adaptationStates);
        }
    }

    // For cmsGetSupportedIntents
    if (size - offset >= 4) {
        cmsUInt32Number nMax = *(cmsUInt32Number*)(data + offset);
        offset += 4;
        
        if (nMax > 0 && nMax < 100) {
            cmsUInt32Number* codes = (cmsUInt32Number*)malloc(nMax * sizeof(cmsUInt32Number));
            char** descriptions = (char**)malloc(nMax * sizeof(char*));
            if (codes && descriptions) {
                for (cmsUInt32Number i = 0; i < nMax; i++) {
                    descriptions[i] = (char*)malloc(256);
                }
                cmsGetSupportedIntents(nMax, codes, descriptions);
                
                for (cmsUInt32Number i = 0; i < nMax; i++) {
                    free(descriptions[i]);
                }
            }
            free(codes);
            free(descriptions);
        }
    }

    // Cleanup
    for (int i = 0; i < 3; i++) {
        if (profiles[i]) {
            cmsCloseProfile(profiles[i]);
        }
    }
    
    cleanup_context(context);
    
    return 0;
}