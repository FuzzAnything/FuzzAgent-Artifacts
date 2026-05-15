// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lcms2.h"

static cmsHPROFILE create_test_profile(const uint8_t* data, size_t size) {
    if (size == 0) {
        return cmsCreateRGBProfile(NULL, NULL, NULL);
    }
    
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreateRGBProfile(NULL, NULL, NULL);
    }
    
    return profile;
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsHPROFILE hProfile = create_test_profile(Data, Size);
    if (!hProfile) return 0;
    
    // Use first 4 bytes for tag signature
    cmsTagSignature sig = *(const cmsTagSignature*)Data;
    
    // Test cmsWriteTag with random data
    const void* write_data = (Size > 8) ? (const void*)(Data + 4) : NULL;
    cmsWriteTag(hProfile, sig, write_data);
    
    // Test cmsGetTagCount
    cmsInt32Number tag_count = cmsGetTagCount(hProfile);
    
    // Test cmsGetTagSignature for valid indices
    if (tag_count > 0) {
        for (cmsUInt32Number i = 0; i < 4 && i < (cmsUInt32Number)tag_count; i++) {
            cmsGetTagSignature(hProfile, i);
        }
        
        // Test with out-of-bounds index
        cmsGetTagSignature(hProfile, (cmsUInt32Number)tag_count + 1);
    }
    
    // Test cmsReadTag
    void* tag_data = cmsReadTag(hProfile, sig);
    (void)tag_data; // Suppress unused warning
    
    // Test cmsReadRawTag
    if (Size > 8) {
        cmsUInt32Number buffer_size = Size % 256;
        if (buffer_size > 0) {
            void* buffer = malloc(buffer_size);
            if (buffer) {
                cmsUInt32Number needed = cmsReadRawTag(hProfile, sig, NULL, 0);
                cmsUInt32Number copied = cmsReadRawTag(hProfile, sig, buffer, buffer_size);
                (void)needed;
                (void)copied;
                free(buffer);
            }
        }
    }
    
    // Test cmsGetHeaderCreationDateTime
    struct tm dest;
    cmsGetHeaderCreationDateTime(hProfile, &dest);
    
    cleanup_profile(hProfile);
    return 0;
}