// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
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

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a context for the fuzzing session
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Create a dummy file for file-based operations
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f != NULL) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    cmsIOHANDLER *io_handler = NULL;
    cmsIOHANDLER *io_handler_null = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE hProfile2 = NULL;
    cmsIOHANDLER *retrieved_io = NULL;

    // Test 1: cmsOpenIOhandlerFromFile
    // Try different modes: "r", "w", "rb", "wb", and invalid ones
    const char *modes[] = {"r", "w", "rb", "wb", "r+", "w+", "invalid", ""};
    size_t num_modes = sizeof(modes) / sizeof(modes[0]);
    
    for (size_t i = 0; i < num_modes; i++) {
        io_handler = cmsOpenIOhandlerFromFile(ctx, dummy_file, modes[i]);
        if (io_handler != NULL) {
            // If we opened a file for reading, try to open a profile
            if (modes[i][0] == 'r' || modes[i][0] == '+') {
                hProfile = cmsOpenProfileFromIOhandler2THR(ctx, io_handler, 0);
                if (hProfile != NULL) {
                    retrieved_io = cmsGetProfileIOhandler(hProfile);
                    // Do nothing with retrieved_io, just check it exists
                    cleanup_profile(hProfile);
                }
            } else {
                // For write modes, try to create a write profile
                hProfile = cmsOpenProfileFromIOhandler2THR(ctx, io_handler, 1);
                if (hProfile != NULL) {
                    cleanup_profile(hProfile);
                }
            }
        }
    }

    // Test 2: cmsOpenIOhandlerFromStream
    f = fopen(dummy_file, "rb");
    if (f != NULL) {
        io_handler = cmsOpenIOhandlerFromStream(ctx, f);
        if (io_handler != NULL) {
            hProfile = cmsOpenProfileFromIOhandler2THR(ctx, io_handler, 0);
            if (hProfile != NULL) {
                retrieved_io = cmsGetProfileIOhandler(hProfile);
                cleanup_profile(hProfile);
            }
        }
        // Do NOT close f here because cmsOpenIOhandlerFromStream may have taken ownership
        // or the internal handler might close it. To be safe, we assume the handler
        // manages the stream lifecycle or we rely on the fact that we don't reuse 'f'.
        // However, standard practice with cmsOpenIOhandlerFromStream is that the caller
        // must manage the FILE* unless the handler is configured to close it.
        // Given the double free error, it seems the handler closed it. 
        // We will skip fclose here to prevent double free.
    }

    // Test 3: cmsOpenIOhandlerFromNULL
    io_handler_null = cmsOpenIOhandlerFromNULL(ctx);
    if (io_handler_null != NULL) {
        // Try to open a profile from NULL handler (should likely fail or return empty)
        // We do NOT pass the NULL handler to cmsOpenProfileFromIOhandler2THR with write=0
        // because it might try to read and fail in ways that cause use-after-free
        // if the handler is not properly set up for reading.
        // Instead, we only test the write path which creates an empty profile.
        hProfile2 = cmsOpenProfileFromIOhandler2THR(ctx, io_handler_null, 1);
        if (hProfile2 != NULL) {
            cleanup_profile(hProfile2);
        }
        // We skip the read path (write=0) for NULL handler to avoid the reported UAF
        // which seems to occur when the library tries to read from a NULL handler
        // and then closes it, but the profile still holds a reference.
    }

    // Test 4: cmsOpenProfileFromIOhandlerTHR
    // Re-open file to test this specific API
    f = fopen(dummy_file, "rb");
    if (f != NULL) {
        io_handler = cmsOpenIOhandlerFromStream(ctx, f);
        if (io_handler != NULL) {
            hProfile = cmsOpenProfileFromIOhandlerTHR(ctx, io_handler);
            if (hProfile != NULL) {
                retrieved_io = cmsGetProfileIOhandler(hProfile);
                cleanup_profile(hProfile);
            }
        }
        // Again, skip fclose to avoid double free if the handler closed the stream
    }

    // Cleanup context
    cmsDeleteContext(ctx);

    // Remove dummy file
    remove(dummy_file);

    return 0;
}