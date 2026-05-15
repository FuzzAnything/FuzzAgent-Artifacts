// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "lcms2.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Create a profile for testing
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) return 0;

    // Extract tag signature from input data
    cmsTagSignature sig;
    memcpy(&sig, Data, 4);
    Data += 4;
    Size -= 4;

    // Test cmsIsTag
    cmsBool tag_exists = cmsIsTag(hProfile, sig);

    // Test cmsWriteRawTag
    if (Size > 0) {
        cmsUInt32Number write_size = (Size > 1024) ? 1024 : Size;
        cmsWriteRawTag(hProfile, sig, Data, write_size);
    }

    // Test cmsWriteTag (write and delete)
    static uint8_t dummy_tag_data[256];
    if (Size > 0) {
        size_t copy_size = (Size > sizeof(dummy_tag_data)) ? sizeof(dummy_tag_data) : Size;
        memcpy(dummy_tag_data, Data, copy_size);
        cmsWriteTag(hProfile, sig, dummy_tag_data);
    }
    // Try deleting the tag
    cmsWriteTag(hProfile, sig, NULL);

    // Test cmsSaveProfileToStream
    write_dummy_file(Data, (Size > 1024) ? 1024 : Size);
    FILE *stream = fopen("./dummy_file", "rb+");
    if (stream) {
        cmsSaveProfileToStream(hProfile, stream);
        fclose(stream);
    }
    remove_dummy_file();

    // Test cmsMD5computeID
    cmsMD5computeID(hProfile);

    // Test cmsSaveProfileToMem
    cmsUInt32Number bytes_needed = 0;
    cmsSaveProfileToMem(hProfile, NULL, &bytes_needed);
    
    if (bytes_needed > 0 && bytes_needed < 10*1024*1024) {
        void *buffer = malloc(bytes_needed);
        if (buffer) {
            cmsSaveProfileToMem(hProfile, buffer, &bytes_needed);
            free(buffer);
        }
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    return 0;
}