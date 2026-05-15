// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_iohandler(cmsIOHANDLER* handler) {
    if (handler) {
        cmsCloseIOhandler(handler);
    }
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Write fuzzed data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Test cmsfilelength
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        long file_len = cmsfilelength(fp);
        (void)file_len; // Use result to avoid unused variable warning
        fclose(fp);
    }

    // Test cmsOpenIOhandlerFromFile with read mode
    cmsContext context = NULL;
    cmsIOHANDLER* io_handler = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
    cleanup_iohandler(io_handler);

    // Test cmsOpenIOhandlerFromFile with write mode
    io_handler = cmsOpenIOhandlerFromFile(context, "./dummy_file", "w");
    cleanup_iohandler(io_handler);

    // Test cmsOpenIOhandlerFromStream
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        io_handler = cmsOpenIOhandlerFromStream(context, fp);
        // cmsOpenIOhandlerFromStream takes ownership of fp - DO NOT close it
        cleanup_iohandler(io_handler);
        // fp is now owned by the handler and will be closed when handler is closed
    }

    // Test cmsOpenProfileFromStream
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStream(fp, "r");
        // cmsOpenProfileFromStream doesn't take ownership of fp
        cleanup_profile(hProfile);
        fclose(fp);
    }

    // Test cmsOpenProfileFromStream with write mode
    fp = fopen("./dummy_file", "wb");
    if (fp) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStream(fp, "w");
        // cmsOpenProfileFromStream doesn't take ownership of fp
        cleanup_profile(hProfile);
        fclose(fp);
    }

    // Test cmsOpenProfileFromStreamTHR with read mode
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(context, fp, "r");
        // cmsOpenProfileFromStreamTHR takes ownership of fp - DO NOT close it
        cleanup_profile(hProfile);
        // fp is now owned by the profile and will be closed when profile is closed
    }

    // Test cmsOpenProfileFromStreamTHR with write mode
    fp = fopen("./dummy_file", "wb");
    if (fp) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(context, fp, "w");
        // cmsOpenProfileFromStreamTHR takes ownership of fp - DO NOT close it
        cleanup_profile(hProfile);
        // fp is now owned by the profile and will be closed when profile is closed
    }

    // Test cmsSaveProfileToStream
    // First create a simple profile to save
    cmsHPROFILE src_profile = cmsCreate_sRGBProfile();
    if (src_profile) {
        fp = fopen("./dummy_file", "wb");
        if (fp) {
            cmsBool save_result = cmsSaveProfileToStream(src_profile, fp);
            (void)save_result; // Use result to avoid unused variable warning
            // cmsSaveProfileToStream doesn't take ownership of fp
            fclose(fp);
        }
        cleanup_profile(src_profile);
    }

    // Clean up dummy file
    remove("./dummy_file");

    return 0;
}