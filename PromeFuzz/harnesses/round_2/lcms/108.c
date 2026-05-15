// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSaveProfileToFile at cmsio0.c:1502:20 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
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

static cmsHPROFILE create_test_profile(const uint8_t* data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
    if (profile == NULL) {
        profile = cmsCreate_sRGBProfile();
    }
    return profile;
}

static cmsPipeline* create_test_pipeline(void) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    
    cmsStage* stage = cmsStageAllocIdentity(ctx, 3);
    if (stage) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
    }
    
    cmsDeleteContext(ctx);
    return pipeline;
}

static void cleanup_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_108(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    
    cmsHPROFILE profile = create_test_profile(data, size);
    if (!profile) return 0;
    
    cmsPipeline* pipeline = create_test_pipeline();
    
    size_t data_offset = 0;
    
    // Test cmsSaveProfileToFile
    cleanup_file();
    FILE* dummy = fopen("./dummy_file", "wb");
    if (dummy) fclose(dummy);
    cmsSaveProfileToFile(profile, "./dummy_file");
    cleanup_file();
    
    // Test cmsWriteRawTag
    if (size > 8) {
        cmsTagSignature sig = *(cmsTagSignature*)(data + data_offset);
        data_offset += 4;
        size_t tag_size = size - data_offset;
        if (tag_size > 0) {
            cmsWriteRawTag(profile, sig, data + data_offset, 
                          tag_size > 1024 ? 1024 : tag_size);
        }
    }
    
    // Test cmsWriteTag
    if (size > 12) {
        cmsTagSignature sig = *(cmsTagSignature*)(data + 8);
        if (size > 20) {
            cmsWriteTag(profile, sig, data + 12);
        } else {
            cmsWriteTag(profile, sig, NULL);
        }
    }
    
    // Test cmsPipelineSetSaveAs8bitsFlag
    if (pipeline) {
        cmsBool flag = (data[0] & 1) ? TRUE : FALSE;
        cmsPipelineSetSaveAs8bitsFlag(pipeline, flag);
    }
    
    // Test cmsSaveProfileToMem
    cmsUInt32Number bytes_needed = 0;
    cmsSaveProfileToMem(profile, NULL, &bytes_needed);
    if (bytes_needed > 0 && bytes_needed < 1024 * 1024) {
        void* buffer = malloc(bytes_needed);
        if (buffer) {
            cmsSaveProfileToMem(profile, buffer, &bytes_needed);
            free(buffer);
        }
    }
    
    // Test cmsLinkTag
    if (size > 16) {
        cmsTagSignature sig1 = *(cmsTagSignature*)(data + 4);
        cmsTagSignature sig2 = *(cmsTagSignature*)(data + 8);
        cmsLinkTag(profile, sig1, sig2);
    }
    
    // Cleanup
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    cmsCloseProfile(profile);
    
    return 0;
}