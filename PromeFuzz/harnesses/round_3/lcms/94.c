// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsGetHeaderAttributes at cmsio0.c:1038:16 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "lcms2.h"

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file() {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_94(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;

    write_dummy_file(Data, Size);
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!hProfile) {
        hProfile = cmsCreate_sRGBProfile();
        if (!hProfile) {
            cleanup_dummy_file();
            return 0;
        }
    }

    if (Size >= 8) {
        cmsUInt64Number attr_flags = 0;
        memcpy(&attr_flags, Data, sizeof(cmsUInt64Number));
        cmsSetHeaderAttributes(hProfile, attr_flags);
        
        cmsUInt64Number retrieved_attr = 0;
        cmsGetHeaderAttributes(hProfile, &retrieved_attr);
    }

    cmsUInt32Number version = cmsGetEncodedICCversion(hProfile);
    (void)version;

    if (Size >= 12) {
        cmsUInt32Number header_flags = 0;
        memcpy(&header_flags, Data + 8, sizeof(cmsUInt32Number));
        cmsSetHeaderFlags(hProfile, header_flags);
    }

    if (Size >= 16) {
        cmsUInt32Number model = 0;
        memcpy(&model, Data + 12, sizeof(cmsUInt32Number));
        cmsSetHeaderModel(hProfile, model);
    }

    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    (void)creator;

    cmsCloseProfile(hProfile);
    cleanup_dummy_file();
    
    return 0;
}