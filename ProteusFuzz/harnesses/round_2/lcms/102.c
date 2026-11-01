// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsContext create_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void cleanup_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = create_context();
    if (!ctx) {
        return 0;
    }

    // Test cmsOpenIOhandlerFromNULL
    cmsIOHANDLER *io_null = cmsOpenIOhandlerFromNULL(ctx);
    if (io_null) {
        cmsHPROFILE prof_null = cmsOpenProfileFromIOhandlerTHR(ctx, io_null);
        if (prof_null) {
            cleanup_profile(prof_null);
        }
        // If prof_null is NULL, the library closes io_null. Do not close manually.
    }

    // Test cmsOpenIOhandlerFromMem
    if (Size > 0) {
        cmsIOHANDLER *io_mem = cmsOpenIOhandlerFromMem(ctx, (void*)Data, Size, "r");
        if (io_mem) {
            cmsHPROFILE prof_mem = cmsOpenProfileFromIOhandlerTHR(ctx, io_mem);
            if (prof_mem) {
                cleanup_profile(prof_mem);
            }
            // If prof_mem is NULL, the library closes io_mem. Do not close manually.
        }
    }

    // Test cmsOpenProfileFromIOhandler2THR with NULL handler (write mode)
    cmsIOHANDLER *io_null2 = cmsOpenIOhandlerFromNULL(ctx);
    if (io_null2) {
        // Use 1 for cmsTrue (write mode)
        cmsHPROFILE prof_write = cmsOpenProfileFromIOhandler2THR(ctx, io_null2, 1);
        if (prof_write) {
            cleanup_profile(prof_write);
        }
        // If NULL, library closes handler.
    }

    // Test cmsOpenProfileFromIOhandler2THR with NULL handler (read mode)
    cmsIOHANDLER *io_null3 = cmsOpenIOhandlerFromNULL(ctx);
    if (io_null3) {
        // Use 0 for cmsFalse (read mode)
        cmsHPROFILE prof_read = cmsOpenProfileFromIOhandler2THR(ctx, io_null3, 0);
        if (prof_read) {
            cleanup_profile(prof_read);
        }
        // If NULL, library closes handler.
    }

    // Test cmsOpenIOhandlerFromStream
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
        
        f = fopen("./dummy_file", "rb");
        if (f) {
            cmsIOHANDLER *io_stream = cmsOpenIOhandlerFromStream(ctx, f);
            if (io_stream) {
                cmsHPROFILE prof_stream = cmsOpenProfileFromIOhandlerTHR(ctx, io_stream);
                if (prof_stream) {
                    cleanup_profile(prof_stream);
                }
                // If NULL, library closes io_stream.
                // The library's IO handler for streams typically closes the file upon destruction.
                // If cmsOpenProfileFromIOhandlerTHR returns NULL, the library cleans up the handler,
                // which likely closes the file. If it returns a profile, cmsCloseProfile closes the handler/file.
                // Therefore, we should NOT call fclose(f) here if the handler was created.
                // However, if io_stream is NULL, we must close f.
                // But wait, if io_stream is created, the file is managed by the handler.
                // If we don't create a profile (prof_stream is NULL), the handler is destroyed by the library, closing the file.
                // If we create a profile, cmsCloseProfile destroys the handler, closing the file.
                // So in both cases, the file is closed by the library if io_stream is valid.
                // We only need to close f if io_stream is NULL.
                if (!io_stream) {
                    fclose(f);
                }
            } else {
                // io_stream is NULL, so we must close f.
                fclose(f);
            }
        }
    }

    // Test cmsGetProfileIOhandler with a valid profile if we can create one from memory
    if (Size > 0) {
        cmsIOHANDLER *io_test = cmsOpenIOhandlerFromMem(ctx, (void*)Data, Size, "r");
        if (io_test) {
            cmsHPROFILE prof_test = cmsOpenProfileFromIOhandlerTHR(ctx, io_test);
            if (prof_test) {
                cmsIOHANDLER *retrieved = cmsGetProfileIOhandler(prof_test);
                (void)retrieved; // Use to avoid warning
                cleanup_profile(prof_test);
            }
            // If NULL, library closes io_test.
        }
    }

    cleanup_context(ctx);
    return 0;
}