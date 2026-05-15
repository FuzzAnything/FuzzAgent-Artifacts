// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
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

static void fuzz_profile_tags(cmsHPROFILE hProfile) {
    if (!hProfile) return;
    
    cmsInt32Number tag_count = cmsGetTagCount(hProfile);
    if (tag_count <= 0) return;
    
    for (cmsInt32Number i = 0; i < tag_count; i++) {
        cmsTagSignature sig = cmsGetTagSignature(hProfile, (cmsUInt32Number)i);
        if (sig == 0) continue;
        
        cmsUInt32Number required_size = cmsReadRawTag(hProfile, sig, NULL, 0);
        if (required_size == 0 || required_size > 10 * 1024 * 1024) continue;
        
        void* buffer = malloc(required_size);
        if (buffer) {
            cmsReadRawTag(hProfile, sig, buffer, required_size);
            free(buffer);
        }
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 128) return 0;
    
    FILE* file = fopen("./dummy_file", "wb");
    if (!file) return 0;
    fwrite(Data, 1, Size, file);
    fclose(file);
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (hProfile) {
        fuzz_profile_tags(hProfile);
        cmsCloseProfile(hProfile);
    }
    
    unlink("./dummy_file");
    return 0;
}