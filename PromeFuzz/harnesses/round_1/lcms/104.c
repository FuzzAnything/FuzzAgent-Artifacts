// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
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

static cmsHPROFILE create_test_profile(const uint8_t *data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (hProfile == NULL) {
        hProfile = cmsCreate_sRGBProfile();
    }
    return hProfile;
}

static void fuzz_cmsLinkTag(cmsHPROFILE hProfile, const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    cmsTagSignature sig = *(cmsTagSignature*)data;
    cmsTagSignature dest = *(cmsTagSignature*)(data + 4);
    
    cmsLinkTag(hProfile, sig, dest);
}

static void fuzz_cmsIsTag(cmsHPROFILE hProfile, const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    cmsTagSignature sig = *(cmsTagSignature*)data;
    cmsIsTag(hProfile, sig);
}

static void fuzz_cmsWriteTag(cmsHPROFILE hProfile, const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    cmsTagSignature sig = *(cmsTagSignature*)data;
    const void* tag_data = NULL;
    
    if (size > 8) {
        tag_data = data + 4;
    }
    
    cmsWriteTag(hProfile, sig, tag_data);
}

static void fuzz_cmsWriteRawTag(cmsHPROFILE hProfile, const uint8_t *data, size_t size) {
    if (size < 12) return;
    
    cmsTagSignature sig = *(cmsTagSignature*)data;
    cmsUInt32Number data_size = *(cmsUInt32Number*)(data + 4);
    
    if (data_size > size - 8) {
        data_size = size - 8;
    }
    
    const void* tag_data = data + 8;
    cmsWriteRawTag(hProfile, sig, tag_data, data_size);
}

static void fuzz_cmsMD5computeID(cmsHPROFILE hProfile) {
    cmsMD5computeID(hProfile);
}

static void fuzz_cmsSaveProfileToMem(cmsHPROFILE hProfile) {
    cmsUInt32Number bytes_needed = 0;
    
    cmsSaveProfileToMem(hProfile, NULL, &bytes_needed);
    
    if (bytes_needed > 0 && bytes_needed < 1024 * 1024) {
        void* buffer = malloc(bytes_needed);
        if (buffer) {
            cmsSaveProfileToMem(hProfile, buffer, &bytes_needed);
            free(buffer);
        }
    }
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsHPROFILE hProfile = create_test_profile(Data, Size);
    if (hProfile == NULL) return 0;
    
    fuzz_cmsLinkTag(hProfile, Data, Size);
    fuzz_cmsIsTag(hProfile, Data, Size);
    fuzz_cmsWriteTag(hProfile, Data, Size);
    fuzz_cmsWriteRawTag(hProfile, Data, Size);
    fuzz_cmsMD5computeID(hProfile);
    fuzz_cmsSaveProfileToMem(hProfile);
    
    cmsCloseProfile(hProfile);
    return 0;
}