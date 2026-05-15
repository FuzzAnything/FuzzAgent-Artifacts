// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to a dummy file for file-based operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Use default context
    cmsContext context = NULL;
    
    // Test cmsOpenIOhandlerFromFile
    cmsIOHANDLER* file_handler = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
    if (file_handler) {
        // Test cmsOpenProfileFromIOhandler2THR with read mode
        cmsHPROFILE profile1 = cmsOpenProfileFromIOhandler2THR(context, file_handler, 0);
        if (profile1) {
            // Test cmsGetProfileIOhandler on valid profile
            cmsIOHANDLER* retrieved_handler = cmsGetProfileIOhandler(profile1);
            // Clean up profile
            cmsCloseProfile(profile1);
        }
        // If profile creation failed, handler is already closed by library
        // so we don't need to close it here
    }
    
    // Test cmsOpenIOhandlerFromStream
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        cmsIOHANDLER* stream_handler = cmsOpenIOhandlerFromStream(context, fp);
        if (stream_handler) {
            // Test cmsOpenProfileFromIOhandlerTHR
            cmsHPROFILE profile2 = cmsOpenProfileFromIOhandlerTHR(context, stream_handler);
            if (profile2) {
                cmsCloseProfile(profile2);
            }
            // If profile creation failed, handler is already closed by library
        } else {
            // Handler creation failed, close the stream
            fclose(fp);
        }
    }
    
    // Test cmsOpenIOhandlerFromNULL
    cmsIOHANDLER* null_handler = cmsOpenIOhandlerFromNULL(context);
    if (null_handler) {
        // Try to open profile from NULL handler (will likely fail but exercises code)
        cmsHPROFILE profile3 = cmsOpenProfileFromIOhandler2THR(context, null_handler, 0);
        if (profile3) {
            cmsCloseProfile(profile3);
        }
        // If profile creation failed, handler is already closed by library
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}