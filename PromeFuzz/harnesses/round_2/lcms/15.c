// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    cmsContext context = NULL;
    cmsIOHANDLER* io_handler = NULL;
    cmsHPROFILE profile = NULL;
    cmsHPROFILE profile2 = NULL;
    FILE* dummy_file = NULL;
    
    // Test 1: cmsOpenIOhandlerFromMem with fuzzer data
    if (Size > 0) {
        io_handler = cmsOpenIOhandlerFromMem(context, (void*)Data, Size, "r");
        if (io_handler) {
            // Test 4: cmsOpenProfileFromIOhandlerTHR with memory handler
            profile = cmsOpenProfileFromIOhandlerTHR(context, io_handler);
            if (profile) {
                // Test 1: cmsGetProfileIOhandler on valid profile
                cmsIOHANDLER* retrieved_handler = cmsGetProfileIOhandler(profile);
                (void)retrieved_handler; // Use to avoid unused variable warning
                
                // Test 6: cmsOpenProfileFromIOhandler2THR with write flag
                // First create another memory handler for testing
                cmsIOHANDLER* io_handler2 = cmsOpenIOhandlerFromMem(context, (void*)Data, Size, "r");
                if (io_handler2) {
                    profile2 = cmsOpenProfileFromIOhandler2THR(context, io_handler2, 0); // read mode
                    // Note: if profile2 is NULL, io_handler2 is already closed by library
                    if (profile2) {
                        cmsCloseProfile(profile2);
                    }
                }
                
                cmsCloseProfile(profile);
            }
            // If profile creation failed, io_handler might still need closing
            // but for memory handler, the library handles it in profile opening failure
        }
    }
    
    // Test 3: cmsOpenIOhandlerFromNULL
    io_handler = cmsOpenIOhandlerFromNULL(context);
    if (io_handler) {
        // Try to open profile from NULL handler (should likely fail)
        profile = cmsOpenProfileFromIOhandlerTHR(context, io_handler);
        if (profile) {
            cmsCloseProfile(profile);
        }
        // For NULL handler, we don't need to close it since the library handles it
    }
    
    // Test 5: cmsOpenIOhandlerFromStream with dummy file
    dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file && Size > 0) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
        
        dummy_file = fopen("./dummy_file", "rb");
        if (dummy_file) {
            io_handler = cmsOpenIOhandlerFromStream(context, dummy_file);
            if (io_handler) {
                // Test 6: cmsOpenProfileFromIOhandler2THR with write flag
                profile = cmsOpenProfileFromIOhandler2THR(context, io_handler, 0); // read mode
                if (profile) {
                    cmsCloseProfile(profile);
                }
                // If profile is NULL, io_handler is already closed by library
                // so we don't need to close the stream
            } else {
                // If IOhandler creation failed, close the stream
                fclose(dummy_file);
            }
        }
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}