// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void cleanup_handler(cmsIOHANDLER* handler) {
    if (handler != NULL) {
        cmsCloseIOhandler(handler);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }
    
    // Test cmsOpenIOhandlerFromNULL
    cmsIOHANDLER* null_handler = cmsOpenIOhandlerFromNULL(context);
    if (null_handler != NULL) {
        cleanup_handler(null_handler);
    }
    
    // Test cmsOpenIOhandlerFromFile with various access modes
    const char* access_modes[] = {"r", "rb", "w", "wb", "r+", "rb+", "w+", "wb+"};
    for (size_t i = 0; i < sizeof(access_modes)/sizeof(access_modes[0]); i++) {
        cmsIOHANDLER* file_handler = cmsOpenIOhandlerFromFile(context, "./dummy_file", access_modes[i]);
        if (file_handler != NULL) {
            // Test profile creation from file handler
            cmsHPROFILE profile1 = cmsOpenProfileFromIOhandlerTHR(context, file_handler);
            if (profile1 != NULL) {
                // Test cmsGetProfileIOhandler
                cmsIOHANDLER* retrieved_handler = cmsGetProfileIOhandler(profile1);
                // Don't close retrieved_handler as it's owned by the profile
                cleanup_profile(profile1);
            }
            
            // Re-open file handler for second test
            cleanup_handler(file_handler);
            file_handler = cmsOpenIOhandlerFromFile(context, "./dummy_file", "rb");
            if (file_handler != NULL) {
                cmsHPROFILE profile2 = cmsOpenProfileFromIOhandler2THR(context, file_handler, 0); // read mode
                if (profile2 != NULL) {
                    cleanup_profile(profile2);
                }
                // If profile2 is NULL, handler is already closed by library
                if (profile2 == NULL) {
                    file_handler = NULL; // Avoid double-free
                }
            }
            
            if (file_handler != NULL) {
                cleanup_handler(file_handler);
            }
        }
    }
    
    // Test cmsOpenIOhandlerFromStream
    dummy_file = fopen("./dummy_file", "rb");
    if (dummy_file != NULL) {
        cmsIOHANDLER* stream_handler = cmsOpenIOhandlerFromStream(context, dummy_file);
        if (stream_handler != NULL) {
            cmsHPROFILE profile3 = cmsOpenProfileFromIOhandlerTHR(context, stream_handler);
            if (profile3 != NULL) {
                cleanup_profile(profile3);
            }
            cleanup_handler(stream_handler);
        } else {
            fclose(dummy_file);
        }
    }
    
    // Clean up context
    cmsDeleteContext(context);
    
    return 0;
}