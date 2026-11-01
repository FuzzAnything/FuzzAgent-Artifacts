// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_iohandler(cmsIOHANDLER *ioHandler) {
    if (ioHandler != NULL) {
        cmsCloseIOhandler(ioHandler);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsIOHANDLER *ioHandler = NULL;
    cmsHPROFILE hProfile = NULL;

    // Test cmsOpenIOhandlerFromNULL
    ioHandler = cmsOpenIOhandlerFromNULL(ContextID);
    if (ioHandler != NULL) {
        // Test cmsOpenProfileFromIOhandler2THR with write flag false (0)
        hProfile = cmsOpenProfileFromIOhandler2THR(ContextID, ioHandler, 0);
        cleanup_profile(hProfile);
        hProfile = NULL;

        // Test cmsOpenProfileFromIOhandler2THR with write flag true (1)
        hProfile = cmsOpenProfileFromIOhandler2THR(ContextID, ioHandler, 1);
        cleanup_profile(hProfile);
        hProfile = NULL;

        // Test cmsOpenProfileFromIOhandlerTHR
        hProfile = cmsOpenProfileFromIOhandlerTHR(ContextID, ioHandler);
        cleanup_profile(hProfile);
        hProfile = NULL;

        // Test cmsGetProfileIOhandler on NULL profile (should return NULL)
        cmsIOHANDLER *retrieved = cmsGetProfileIOhandler(NULL);
        (void)retrieved;

        // Clean up the IO handler before moving to the next test
        cleanup_iohandler(ioHandler);
        ioHandler = NULL;
    }

    // Test cmsOpenIOhandlerFromMem
    if (Size > 0) {
        ioHandler = cmsOpenIOhandlerFromMem(ContextID, (void *)Data, Size, "r");
        if (ioHandler != NULL) {
            hProfile = cmsOpenProfileFromIOhandler2THR(ContextID, ioHandler, 0);
            cleanup_profile(hProfile);
            hProfile = NULL;

            hProfile = cmsOpenProfileFromIOhandlerTHR(ContextID, ioHandler);
            cleanup_profile(hProfile);
            hProfile = NULL;

            cleanup_iohandler(ioHandler);
            ioHandler = NULL;
        }
    }

    // Test cmsOpenIOhandlerFromStream with a dummy file
    FILE *stream = fopen("./dummy_file", "wb");
    if (stream != NULL) {
        if (Size > 0) {
            fwrite(Data, 1, Size, stream);
        }
        fclose(stream);

        stream = fopen("./dummy_file", "rb");
        if (stream != NULL) {
            ioHandler = cmsOpenIOhandlerFromStream(ContextID, stream);
            if (ioHandler != NULL) {
                hProfile = cmsOpenProfileFromIOhandler2THR(ContextID, ioHandler, 0);
                cleanup_profile(hProfile);
                hProfile = NULL;

                hProfile = cmsOpenProfileFromIOhandlerTHR(ContextID, ioHandler);
                cleanup_profile(hProfile);
                hProfile = NULL;

                cleanup_iohandler(ioHandler);
                ioHandler = NULL;
            }
            fclose(stream);
        }
    }

    // Cleanup context
    if (ContextID != NULL) {
        cmsDeleteContext(ContextID);
    }

    return 0;
}