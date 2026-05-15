// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
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

static void generate_random_tag_signature(const uint8_t *data, size_t size, cmsTagSignature *sig1, cmsTagSignature *sig2) {
    if (size >= 8) {
        memcpy(sig1, data, 4);
        memcpy(sig2, data + 4, 4);
    } else if (size >= 4) {
        memcpy(sig1, data, 4);
        *sig2 = 0;
    } else {
        *sig1 = 0;
        *sig2 = 0;
    }
}

static cmsHPROFILE create_test_profile(const uint8_t *data, size_t size) {
    if (size > 0) {
        return cmsOpenProfileFromMem(data, size);
    }
    return cmsCreate_sRGBProfile();
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsHPROFILE hProfile = create_test_profile(Data, Size);
    if (!hProfile) return 0;
    
    cmsTagSignature sig1, sig2;
    generate_random_tag_signature(Data, Size, &sig1, &sig2);
    
    cmsBool result;
    
    result = cmsIsTag(hProfile, sig1);
    
    cmsUInt32Number bytesNeeded = 0;
    cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
    if (bytesNeeded > 0 && bytesNeeded < 1024 * 1024) {
        void *buffer = malloc(bytesNeeded);
        if (buffer) {
            cmsSaveProfileToMem(hProfile, buffer, &bytesNeeded);
            free(buffer);
        }
    }
    
    cmsMD5computeID(hProfile);
    
    if (Size > 8) {
        size_t tag_data_size = Size - 8;
        if (tag_data_size > 0) {
            cmsWriteRawTag(hProfile, sig1, Data + 8, (cmsUInt32Number)tag_data_size);
        }
    }
    
    if (Size > 4) {
        cmsWriteTag(hProfile, sig1, NULL);
    }
    
    cmsLinkTag(hProfile, sig1, sig2);
    
    cmsCloseProfile(hProfile);
    return 0;
}