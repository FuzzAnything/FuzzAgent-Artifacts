// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    write_dummy_file(Data, Size);
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Test cmsOpenIOhandlerFromNULL
    cmsIOHANDLER* null_handler = cmsOpenIOhandlerFromNULL(context);
    if (null_handler) {
        cmsCloseIOhandler(null_handler);
    }
    
    // Test cmsOpenIOhandlerFromFile
    const char* modes[] = {"r", "w", "rb", "wb", "r+", "w+"};
    for (size_t i = 0; i < sizeof(modes)/sizeof(modes[0]); i++) {
        cmsIOHANDLER* file_handler = cmsOpenIOhandlerFromFile(context, "./dummy_file", modes[i]);
        if (file_handler) {
            cmsCloseIOhandler(file_handler);
        }
    }
    
    // Test cmsOpenIOhandlerFromStream
    FILE* stream = fopen("./dummy_file", "rb");
    if (stream) {
        cmsIOHANDLER* stream_handler = cmsOpenIOhandlerFromStream(context, stream);
        if (stream_handler) {
            cmsCloseIOhandler(stream_handler);
        } else {
            fclose(stream);
        }
    }
    
    // Re-open stream for profile testing
    stream = fopen("./dummy_file", "rb");
    if (stream) {
        cmsIOHANDLER* stream_handler = cmsOpenIOhandlerFromStream(context, stream);
        if (stream_handler) {
            // Test cmsOpenProfileFromIOhandlerTHR
            cmsHPROFILE profile = cmsOpenProfileFromIOhandlerTHR(context, stream_handler);
            if (profile) {
                // Test cmsGetProfileIOhandler
                cmsIOHANDLER* profile_handler = cmsGetProfileIOhandler(profile);
                if (profile_handler) {
                    // Profile handler obtained, but we don't own it - don't close it
                }
                cmsCloseProfile(profile);
            } else {
                // Profile creation failed, handler already closed by library
                // Do NOT close stream_handler or fclose(stream)
                stream = NULL; // Mark stream as already handled
            }
            
            // If profile creation succeeded, handler is still open
            if (profile) {
                cmsCloseIOhandler(stream_handler);
            }
        } else {
            fclose(stream);
        }
    }
    
    // Test cmsOpenProfileFromIOhandler2THR with read mode
    stream = fopen("./dummy_file", "rb");
    if (stream) {
        cmsIOHANDLER* stream_handler = cmsOpenIOhandlerFromStream(context, stream);
        if (stream_handler) {
            cmsHPROFILE profile2 = cmsOpenProfileFromIOhandler2THR(context, stream_handler, 0); // read mode
            if (profile2) {
                cmsCloseProfile(profile2);
                cmsCloseIOhandler(stream_handler);
            } else {
                // Profile creation failed, handler already closed
                stream = NULL;
            }
        } else {
            fclose(stream);
        }
    }
    
    // Test cmsOpenProfileFromIOhandler2THR with write mode
    stream = fopen("./dummy_file", "wb");
    if (stream) {
        cmsIOHANDLER* stream_handler = cmsOpenIOhandlerFromStream(context, stream);
        if (stream_handler) {
            cmsHPROFILE profile3 = cmsOpenProfileFromIOhandler2THR(context, stream_handler, 1); // write mode
            if (profile3) {
                cmsCloseProfile(profile3);
                cmsCloseIOhandler(stream_handler);
            } else {
                stream = NULL;
            }
        } else {
            fclose(stream);
        }
    }
    
    cmsDeleteContext(context);
    remove("./dummy_file");
    
    return 0;
}