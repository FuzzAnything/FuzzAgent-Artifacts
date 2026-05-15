// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void cleanup_iohandler(cmsIOHANDLER* io) {
    if (io != NULL) {
        cmsCloseIOhandler(io);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Create a context for the operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }

    // Test 1: cmsOpenIOhandlerFromMem with read mode
    cmsIOHANDLER* mem_io = cmsOpenIOhandlerFromMem(context, (void*)Data, (cmsUInt32Number)Size, "r");
    if (mem_io != NULL) {
        // Test 4: cmsOpenProfileFromIOhandlerTHR with memory I/O handler
        cmsHPROFILE profile1 = cmsOpenProfileFromIOhandlerTHR(context, mem_io);
        if (profile1 != NULL) {
            // Test 1: cmsGetProfileIOhandler on valid profile
            cmsIOHANDLER* retrieved_io = cmsGetProfileIOhandler(profile1);
            (void)retrieved_io; // Use to avoid unused variable warning
            
            cleanup_profile(profile1);
        } else {
            // Profile creation failed, but I/O handler is already closed by cmsOpenProfileFromIOhandlerTHR
            // Do not close mem_io here
            mem_io = NULL;
        }
    }

    // Test 2: cmsOpenIOhandlerFromMem with write mode (should fail with NULL buffer)
    cmsIOHANDLER* mem_io_write = cmsOpenIOhandlerFromMem(context, NULL, 0, "w");
    (void)mem_io_write; // Use to avoid unused variable warning

    // Test 3: cmsOpenIOhandlerFromNULL
    cmsIOHANDLER* null_io = cmsOpenIOhandlerFromNULL(context);
    if (null_io != NULL) {
        // Test 6: cmsOpenProfileFromIOhandler2THR with NULL I/O handler and write=false
        cmsHPROFILE profile2 = cmsOpenProfileFromIOhandler2THR(context, null_io, 0);
        if (profile2 != NULL) {
            cleanup_profile(profile2);
        } else {
            // If profile2 is NULL, the I/O handler is already closed by the library
            null_io = NULL;
        }
    }

    // Test 5: cmsOpenIOhandlerFromStream with a file
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file != NULL) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);

        dummy_file = fopen("./dummy_file", "rb");
        if (dummy_file != NULL) {
            cmsIOHANDLER* stream_io = cmsOpenIOhandlerFromStream(context, dummy_file);
            if (stream_io != NULL) {
                // Test 6: cmsOpenProfileFromIOhandler2THR with stream I/O handler and write=false
                cmsHPROFILE profile3 = cmsOpenProfileFromIOhandler2THR(context, stream_io, 0);
                if (profile3 != NULL) {
                    cleanup_profile(profile3);
                } else {
                    // If profile3 is NULL, the I/O handler is already closed
                    stream_io = NULL;
                }
            }
            fclose(dummy_file);
        }
        remove("./dummy_file");
    }

    // Test 6: cmsOpenProfileFromIOhandler2THR with write mode (should fail for read-only handler)
    // We'll create a new memory I/O handler for writing
    if (Size > 0) {
        void* buffer = malloc(Size);
        if (buffer != NULL) {
            memcpy(buffer, Data, Size);
            cmsIOHANDLER* mem_io_write2 = cmsOpenIOhandlerFromMem(context, buffer, (cmsUInt32Number)Size, "w");
            if (mem_io_write2 != NULL) {
                cmsHPROFILE profile4 = cmsOpenProfileFromIOhandler2THR(context, mem_io_write2, 1);
                if (profile4 != NULL) {
                    cleanup_profile(profile4);
                } else {
                    // If profile4 is NULL, the I/O handler is already closed
                    mem_io_write2 = NULL;
                }
            }
            free(buffer);
        }
    }

    // Cleanup any remaining I/O handlers (those that weren't closed by the library)
    if (mem_io != NULL) {
        cleanup_iohandler(mem_io);
    }
    if (null_io != NULL) {
        cleanup_iohandler(null_io);
    }

    cmsDeleteContext(context);
    return 0;
}