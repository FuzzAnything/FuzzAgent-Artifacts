// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_iohandler(cmsIOHANDLER* iohandler) {
    if (iohandler != NULL) {
        cmsCloseIOhandler(iohandler);
    }
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    // Create a context for all operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Test cmsOpenIOhandlerFromMem with various modes
    if (Size > 0) {
        // Test with read mode
        cmsIOHANDLER* mem_io = cmsOpenIOhandlerFromMem(ctx, (void*)Data, (cmsUInt32Number)Size, "r");
        if (mem_io != NULL) {
            // Test cmsOpenProfileFromIOhandlerTHR with the memory handler
            cmsHPROFILE profile1 = cmsOpenProfileFromIOhandlerTHR(ctx, mem_io);
            cleanup_profile(profile1);
            // Note: cmsOpenProfileFromIOhandlerTHR takes ownership of the handler,
            // so we don't need to close it separately if profile was created
            if (profile1 == NULL) {
                cleanup_iohandler(mem_io);
            }
        }

        // Test with write mode (requires writable buffer)
        if (Size > 4) {
            void* writable_buffer = malloc(Size);
            if (writable_buffer != NULL) {
                memcpy(writable_buffer, Data, Size);
                cmsIOHANDLER* mem_io_write = cmsOpenIOhandlerFromMem(ctx, writable_buffer, (cmsUInt32Number)Size, "w");
                cleanup_iohandler(mem_io_write);
                free(writable_buffer);
            }
        }
    }

    // Test file-based functions using dummy file
    if (Size > 0) {
        // Write data to dummy file
        FILE* dummy = fopen("./dummy_file", "wb");
        if (dummy != NULL) {
            fwrite(Data, 1, Size, dummy);
            fclose(dummy);
        }

        // Test cmsOpenIOhandlerFromFile
        cmsIOHANDLER* file_io = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
        if (file_io != NULL) {
            // Test cmsOpenProfileFromIOhandlerTHR with file handler
            cmsHPROFILE profile2 = cmsOpenProfileFromIOhandlerTHR(ctx, file_io);
            cleanup_profile(profile2);
            if (profile2 == NULL) {
                cleanup_iohandler(file_io);
            }
        }

        // Test cmsOpenProfileFromFileTHR
        cmsHPROFILE profile3 = cmsOpenProfileFromFileTHR(ctx, "./dummy_file", "r");
        cleanup_profile(profile3);

        // Test cmsOpenProfileFromStreamTHR
        FILE* stream = fopen("./dummy_file", "rb");
        if (stream != NULL) {
            cmsHPROFILE profile4 = cmsOpenProfileFromStreamTHR(ctx, stream, "r");
            cleanup_profile(profile4);
            // According to documentation, library takes ownership of stream
            // so we don't fclose() here
        }

        // Test cmsCreateDeviceLinkFromCubeFileTHR
        // First write data to a .cube file
        FILE* cube_file = fopen("./dummy_file.cube", "wb");
        if (cube_file != NULL) {
            fwrite(Data, 1, Size, cube_file);
            fclose(cube_file);
            
            cmsHPROFILE cube_profile = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "./dummy_file.cube");
            cleanup_profile(cube_profile);
            
            remove("./dummy_file.cube");
        }

        remove("./dummy_file");
    }

    // Test edge cases with minimal data
    if (Size == 0) {
        // Test with NULL buffer
        cmsIOHANDLER* null_io = cmsOpenIOhandlerFromMem(ctx, NULL, 0, "r");
        cleanup_iohandler(null_io);
        
        // Test with empty file
        FILE* empty = fopen("./empty_file", "wb");
        if (empty != NULL) {
            fclose(empty);
            
            cmsHPROFILE empty_profile = cmsOpenProfileFromFileTHR(ctx, "./empty_file", "r");
            cleanup_profile(empty_profile);
            
            remove("./empty_file");
        }
    }

    // Test with different access modes
    if (Size > 0) {
        // Create a writable file for testing write mode
        FILE* write_file = fopen("./write_test", "wb");
        if (write_file != NULL) {
            fwrite(Data, 1, Size, write_file);
            fclose(write_file);
            
            // Test write mode (this will create/overwrite the file)
            cmsHPROFILE write_profile = cmsOpenProfileFromFileTHR(ctx, "./write_test", "w");
            cleanup_profile(write_profile);
            
            remove("./write_test");
        }
    }

    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}