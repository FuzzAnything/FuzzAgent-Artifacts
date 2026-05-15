// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Create a context for all operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Test cmsOpenIOhandlerFromNULL
    cmsIOHANDLER* null_handler = cmsOpenIOhandlerFromNULL(context);
    if (null_handler) {
        // Try to open a profile from the NULL handler
        cmsHPROFILE null_profile = cmsOpenProfileFromIOhandlerTHR(context, null_handler);
        if (null_profile) {
            // Test cmsGetProfileIOhandler on the NULL-based profile
            cmsIOHANDLER* retrieved_handler = cmsGetProfileIOhandler(null_profile);
            cmsCloseProfile(null_profile);
        }
        // DO NOT close the NULL handler - cmsOpenProfileFromIOhandlerTHR takes ownership
    }

    // Test cmsOpenIOhandlerFromMem with the fuzz data
    if (Size > 0) {
        // Create a copy of the data since the handler might modify it
        void* buffer = malloc(Size);
        if (buffer) {
            memcpy(buffer, Data, Size);
            cmsIOHANDLER* mem_handler = cmsOpenIOhandlerFromMem(context, buffer, Size, "r");
            if (mem_handler) {
                // Try to open a profile from the memory handler
                cmsHPROFILE mem_profile = cmsOpenProfileFromIOhandlerTHR(context, mem_handler);
                if (mem_profile) {
                    // Test cmsGetProfileIOhandler on the memory-based profile
                    cmsIOHANDLER* retrieved_mem_handler = cmsGetProfileIOhandler(mem_profile);
                    cmsCloseProfile(mem_profile);
                }
                // DO NOT close the memory handler - cmsOpenProfileFromIOhandlerTHR takes ownership
            }
            free(buffer);
        }
    }

    // Test cmsOpenIOhandlerFromFile
    if (Size > 0) {
        // Write fuzz data to a dummy file
        FILE* dummy_file = fopen("./dummy_file", "wb");
        if (dummy_file) {
            fwrite(Data, 1, Size, dummy_file);
            fclose(dummy_file);
            
            // Open the file for reading
            cmsIOHANDLER* file_handler = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
            if (file_handler) {
                // Try to open a profile from the file handler
                cmsHPROFILE file_profile = cmsOpenProfileFromIOhandlerTHR(context, file_handler);
                if (file_profile) {
                    // Test cmsGetProfileIOhandler on the file-based profile
                    cmsIOHANDLER* retrieved_file_handler = cmsGetProfileIOhandler(file_profile);
                    cmsCloseProfile(file_profile);
                }
                // DO NOT close the file handler - cmsOpenProfileFromIOhandlerTHR takes ownership
            }
            // Clean up the dummy file
            remove("./dummy_file");
        }
    }

    // Test cmsOpenIOhandlerFromStream
    if (Size > 0) {
        // Create a temporary file with the fuzz data
        FILE* stream_file = fopen("./dummy_stream_file", "wb");
        if (stream_file) {
            fwrite(Data, 1, Size, stream_file);
            fclose(stream_file);
            
            // Open the file as a stream
            FILE* stream = fopen("./dummy_stream_file", "rb");
            if (stream) {
                cmsIOHANDLER* stream_handler = cmsOpenIOhandlerFromStream(context, stream);
                if (stream_handler) {
                    // Try to open a profile from the stream handler
                    cmsHPROFILE stream_profile = cmsOpenProfileFromIOhandlerTHR(context, stream_handler);
                    if (stream_profile) {
                        // Test cmsGetProfileIOhandler on the stream-based profile
                        cmsIOHANDLER* retrieved_stream_handler = cmsGetProfileIOhandler(stream_profile);
                        cmsCloseProfile(stream_profile);
                    }
                    // DO NOT close the stream handler - cmsOpenProfileFromIOhandlerTHR takes ownership
                }
                fclose(stream);
            }
            // Clean up the temporary file
            remove("./dummy_stream_file");
        }
    }

    // Clean up context
    cmsDeleteContext(context);
    
    return 0;
}