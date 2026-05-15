// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write fuzzer data to dummy file
    write_dummy_file(Data, Size);
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Test 1: cmsOpenIOhandlerFromFile with cmsOpenProfileFromIOhandlerTHR
    cmsIOHANDLER* file_io = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
    if (file_io) {
        cmsHPROFILE profile1 = cmsOpenProfileFromIOhandlerTHR(ctx, file_io);
        if (profile1) {
            // Test 2: cmsGetProfileIOhandler
            cmsIOHANDLER* retrieved_io = cmsGetProfileIOhandler(profile1);
            // Don't close retrieved_io - it's owned by the profile
            cleanup_profile(profile1);
        }
        // Don't close file_io here - cmsOpenProfileFromIOhandlerTHR takes ownership
        // even on failure, so closing would cause double-free
    }
    
    // Test 3: cmsOpenProfileFromIOhandler2THR with new handler
    file_io = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
    if (file_io) {
        // Test with write=false (reading mode)
        cmsHPROFILE profile2 = cmsOpenProfileFromIOhandler2THR(ctx, file_io, 0);
        if (profile2) {
            cleanup_profile(profile2);
        }
        // Don't close file_io - cmsOpenProfileFromIOhandler2THR takes ownership
        // even on failure as documented
    }
    
    // Test 4: cmsOpenIOhandlerFromStream
    FILE* stream = fopen("./dummy_file", "rb");
    if (stream) {
        cmsIOHANDLER* stream_io = cmsOpenIOhandlerFromStream(ctx, stream);
        if (stream_io) {
            cmsHPROFILE profile3 = cmsOpenProfileFromIOhandlerTHR(ctx, stream_io);
            if (profile3) {
                cleanup_profile(profile3);
            }
            // Don't close stream_io - cmsOpenProfileFromIOhandlerTHR takes ownership
        } else {
            // Handler creation failed, close the stream
            fclose(stream);
        }
    }
    
    // Test 5: cmsOpenIOhandlerFromNULL
    cmsIOHANDLER* null_io = cmsOpenIOhandlerFromNULL(ctx);
    if (null_io) {
        cmsHPROFILE profile4 = cmsOpenProfileFromIOhandlerTHR(ctx, null_io);
        if (profile4) {
            cleanup_profile(profile4);
        } else {
            // Profile creation failed, but handler was created successfully
            // We need to close it since cmsOpenProfileFromIOhandlerTHR doesn't
            // take ownership of NULL handlers on failure
            cmsCloseIOhandler(null_io);
        }
    }
    
    // Cleanup context
    cmsDeleteContext(ctx);
    
    return 0;
}