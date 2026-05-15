// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!hProfile) {
        hProfile = cmsCreate_sRGBProfile();
        if (!hProfile) {
            remove_dummy_file();
            return 0;
        }
    }
    
    cmsTagSignature sig = *(cmsTagSignature*)Data;
    cmsUInt32Number index = *(cmsUInt32Number*)Data % 100;
    
    cmsBool exists = cmsIsTag(hProfile, sig);
    
    void *tag_data = cmsReadTag(hProfile, sig);
    if (tag_data) {
        cmsCloseProfile(hProfile);
        hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
        if (!hProfile) {
            hProfile = cmsCreate_sRGBProfile();
        }
    }
    
    cmsTagSignature linked = cmsTagLinkedTo(hProfile, sig);
    
    cmsUInt32Number raw_size = cmsReadRawTag(hProfile, sig, NULL, 0);
    if (raw_size > 0 && raw_size < 1024*1024) {
        void *buffer = malloc(raw_size);
        if (buffer) {
            cmsReadRawTag(hProfile, sig, buffer, raw_size);
            free(buffer);
        }
    }
    
    cmsTagSignature indexed_sig = cmsGetTagSignature(hProfile, index);
    
    if (Size >= 8) {
        cmsTagSignature dest_sig = *(cmsTagSignature*)(Data + 4);
        cmsBool linked_result = cmsLinkTag(hProfile, sig, dest_sig);
        (void)linked_result;
    }
    
    cmsCloseProfile(hProfile);
    remove_dummy_file();
    
    return 0;
}