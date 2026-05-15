// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_handler(cmsIOHANDLER* handler) {
    if (handler) {
        // Use cmsCloseIOhandler instead of accessing internal structure
        cmsCloseIOhandler(handler);
    }
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Write fuzzed data to dummy file
    FILE* dummy = fopen("./dummy_file", "wb");
    if (!dummy) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy);
    fclose(dummy);

    // Test cmsfilelength
    dummy = fopen("./dummy_file", "rb");
    if (dummy) {
        long length = cmsfilelength(dummy);
        fclose(dummy);
    }

    // Test cmsOpenProfileFromStream with read access
    dummy = fopen("./dummy_file", "rb");
    if (dummy) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStream(dummy, "r");
        cleanup_profile(hProfile);
    }

    // Test cmsOpenProfileFromStreamTHR with read access
    dummy = fopen("./dummy_file", "rb");
    if (dummy) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(NULL, dummy, "r");
        cleanup_profile(hProfile);
    }

    // Test cmsOpenProfileFromStream with write access
    dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStream(dummy, "w");
        cleanup_profile(hProfile);
    }

    // Test cmsOpenProfileFromStreamTHR with write access
    dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(NULL, dummy, "w");
        cleanup_profile(hProfile);
    }

    // Test cmsOpenIOhandlerFromStream
    dummy = fopen("./dummy_file", "rb");
    if (dummy) {
        cmsIOHANDLER* handler = cmsOpenIOhandlerFromStream(NULL, dummy);
        cleanup_handler(handler);
    }

    // Test cmsOpenIOhandlerFromFile
    cmsIOHANDLER* handler = cmsOpenIOhandlerFromFile(NULL, "./dummy_file", "r");
    cleanup_handler(handler);

    // Test cmsSaveProfileToStream with a created profile
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile) {
        dummy = fopen("./dummy_file", "wb");
        if (dummy) {
            cmsSaveProfileToStream(hProfile, dummy);
            fclose(dummy);
        }
        cleanup_profile(hProfile);
    }

    // Test cmsSaveProfileToStream with NULL profile
    dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        cmsSaveProfileToStream(NULL, dummy);
        fclose(dummy);
    }

    // Remove dummy file to avoid accumulation
    remove("./dummy_file");

    return 0;
}