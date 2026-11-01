// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
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

int LLVMFuzzerTestOneInput_193(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a context for this fuzzing iteration
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsIOHANDLER* io = NULL;
    cmsHPROFILE hProfile = NULL;
    FILE* stream = NULL;
    int file_created = 0;

    // Use the first byte to decide which function to test
    uint8_t mode = Data[0] % 5;

    if (mode == 0) {
        // Test cmsOpenIOhandlerFromStream
        // Create a dummy file
        FILE* f = fopen("./dummy_file", "wb");
        if (f != NULL) {
            size_t write_size = (Size > 1024) ? 1024 : Size;
            fwrite(Data, 1, write_size, f);
            fclose(f);
            file_created = 1;
        }

        stream = fopen("./dummy_file", "rb");
        if (stream != NULL) {
            io = cmsOpenIOhandlerFromStream(ContextID, stream);
            // If io is created, lcms takes ownership of the stream.
            // We should NOT close stream manually if io is not NULL.
            if (io != NULL) {
                hProfile = cmsOpenProfileFromIOhandlerTHR(ContextID, io);
                if (hProfile != NULL) {
                    cmsIOHANDLER* retrieved_io = cmsGetProfileIOhandler(hProfile);
                    (void)retrieved_io;
                }
            } else {
                // If io creation failed, we still own the stream
                fclose(stream);
            }
        }
    } else if (mode == 1) {
        // Test cmsOpenIOhandlerFromNULL
        io = cmsOpenIOhandlerFromNULL(ContextID);
        if (io != NULL) {
            hProfile = cmsOpenProfileFromIOhandlerTHR(ContextID, io);
            if (hProfile != NULL) {
                cmsIOHANDLER* retrieved_io = cmsGetProfileIOhandler(hProfile);
                (void)retrieved_io;
            }
        }
    } else if (mode == 2) {
        // Test cmsOpenIOhandlerFromMem
        // Ensure we don't pass a buffer that goes out of bounds.
        // Data is [0, Size). We use Data[0] for mode, so buffer starts at Data+1.
        // Available size for buffer is Size - 1.
        if (Size > 1) {
            cmsUInt32Number size = (Size - 1 > 100000) ? 100000 : (Size - 1);
            io = cmsOpenIOhandlerFromMem(ContextID, (void*)(Data + 1), size, "r");
            if (io != NULL) {
                hProfile = cmsOpenProfileFromIOhandlerTHR(ContextID, io);
                if (hProfile != NULL) {
                    cmsIOHANDLER* retrieved_io = cmsGetProfileIOhandler(hProfile);
                    (void)retrieved_io;
                }
            }
        }
    } else if (mode == 3) {
        // Test cmsOpenIOhandlerFromFile
        // Write data to dummy file first
        FILE* f = fopen("./dummy_file", "wb");
        if (f != NULL) {
            size_t write_size = (Size > 1024) ? 1024 : Size;
            fwrite(Data, 1, write_size, f);
            fclose(f);
            file_created = 1;
        }

        // Try reading
        io = cmsOpenIOhandlerFromFile(ContextID, "./dummy_file", "r");
        if (io != NULL) {
            hProfile = cmsOpenProfileFromIOhandlerTHR(ContextID, io);
            if (hProfile != NULL) {
                cmsIOHANDLER* retrieved_io = cmsGetProfileIOhandler(hProfile);
                (void)retrieved_io;
            }
        }

        // Try writing (might fail if file is not valid ICC, but tests the path)
        io = cmsOpenIOhandlerFromFile(ContextID, "./dummy_file", "w");
        if (io != NULL) {
            // We don't open a profile from a write handler, but we test the handler creation
            // Just ensure it doesn't crash
        }
    } else if (mode == 4) {
        // Test cmsGetProfileIOhandler with NULL profile
        cmsIOHANDLER* retrieved_io = cmsGetProfileIOhandler(NULL);
        (void)retrieved_io;
    }

    // Cleanup
    cleanup_profile(hProfile);
    // io is managed by profile or context, no direct free needed for the struct itself
    // Destroy context to free all associated resources
    cmsDeleteContext(ContextID);

    return 0;
}