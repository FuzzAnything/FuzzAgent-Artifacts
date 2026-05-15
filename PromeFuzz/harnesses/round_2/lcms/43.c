// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create a dummy profile for testing
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) return 0;
    
    // Use first 4 bytes for tag signature
    cmsTagSignature sig = *(cmsTagSignature*)Data;
    
    // Test cmsSaveProfileToStream
    create_dummy_file(Data, Size);
    FILE *stream = fopen("./dummy_file", "rb+");
    if (stream) {
        cmsSaveProfileToStream(hProfile, stream);
        fclose(stream);
    }
    cleanup_dummy_file();
    
    // Test cmsWriteRawTag
    const void *tag_data = Data + 4;
    size_t tag_size = Size > 4 ? Size - 4 : 0;
    cmsWriteRawTag(hProfile, sig, tag_data, (cmsUInt32Number)tag_size);
    
    // Test cmsWriteTag
    cmsWriteTag(hProfile, sig, tag_data);
    cmsWriteTag(hProfile, sig, NULL);  // Test deletion
    
    // Test cmsMD5computeID
    cmsMD5computeID(hProfile);
    
    // Test cmsSaveProfileToMem
    cmsUInt32Number bytes_needed = 0;
    cmsSaveProfileToMem(hProfile, NULL, &bytes_needed);
    if (bytes_needed > 0 && bytes_needed < 1024*1024) {
        void *buffer = malloc(bytes_needed);
        if (buffer) {
            cmsSaveProfileToMem(hProfile, buffer, &bytes_needed);
            free(buffer);
        }
    }
    
    // Test cmsIsTag
    cmsIsTag(hProfile, sig);
    
    // Cleanup
    cmsCloseProfile(hProfile);
    return 0;
}