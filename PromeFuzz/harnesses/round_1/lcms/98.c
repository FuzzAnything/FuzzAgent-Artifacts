// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsSaveProfileToFile at cmsio0.c:1502:20 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
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

static void cleanup_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    // Create a profile from the input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (!hProfile) {
        // If we can't create a profile from input, create a placeholder
        hProfile = cmsCreateProfilePlaceholder(NULL);
        if (!hProfile) {
            return 0;
        }
    }

    // Parse some parameters from the input data
    cmsTagSignature sig = 0, dest = 0;
    cmsBool flag = 0;
    const void* tag_data = NULL;
    cmsUInt32Number tag_size = 0;
    
    if (Size >= sizeof(cmsTagSignature) * 2 + sizeof(cmsBool)) {
        memcpy(&sig, Data, sizeof(cmsTagSignature));
        memcpy(&dest, Data + sizeof(cmsTagSignature), sizeof(cmsTagSignature));
        memcpy(&flag, Data + sizeof(cmsTagSignature) * 2, sizeof(cmsBool));
        
        if (Size > sizeof(cmsTagSignature) * 2 + sizeof(cmsBool)) {
            tag_data = Data + sizeof(cmsTagSignature) * 2 + sizeof(cmsBool);
            tag_size = Size - (sizeof(cmsTagSignature) * 2 + sizeof(cmsBool));
        }
    }

    // Test cmsLinkTag
    cmsLinkTag(hProfile, sig, dest);

    // Test cmsSaveProfileToFile
    cmsSaveProfileToFile(hProfile, "./dummy_file");
    cleanup_file();

    // Test cmsWriteRawTag
    if (tag_data && tag_size > 0) {
        cmsWriteRawTag(hProfile, sig, tag_data, tag_size);
    }

    // Test cmsWriteTag
    if (tag_data && tag_size > 0) {
        cmsWriteTag(hProfile, sig, tag_data);
    } else {
        // Test deletion by passing NULL
        cmsWriteTag(hProfile, sig, NULL);
    }

    // Test cmsPipelineSetSaveAs8bitsFlag
    // Create a simple pipeline for testing
    cmsPipeline* lut = cmsPipelineAlloc(NULL, 3, 3);
    if (lut) {
        cmsPipelineSetSaveAs8bitsFlag(lut, flag);
        cmsPipelineFree(lut);
    }

    // Test cmsSaveProfileToMem
    cmsUInt32Number bytes_needed = 0;
    cmsSaveProfileToMem(hProfile, NULL, &bytes_needed);
    
    if (bytes_needed > 0 && bytes_needed < 1024 * 1024) { // Limit to 1MB
        void* buffer = malloc(bytes_needed);
        if (buffer) {
            cmsSaveProfileToMem(hProfile, buffer, &bytes_needed);
            free(buffer);
        }
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    return 0;
}