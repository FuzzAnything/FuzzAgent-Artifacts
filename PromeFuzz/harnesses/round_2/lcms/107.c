// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    // Write input data to dummy file for file-based operations
    write_dummy_file(Data, Size);
    
    // Create a context for THR functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Test cmsOpenProfileFromStreamTHR
    if (Size > 0) {
        FILE *stream = fopen("./dummy_file", "rb");
        if (stream) {
            cmsHPROFILE profile1 = cmsOpenProfileFromStreamTHR(context, stream, "r");
            if (profile1) {
                cmsCloseProfile(profile1);
            }
            // Note: Do NOT close the FILE* stream - library takes ownership
        }
    }
    
    // Test cmsOpenIOhandlerFromFile
    cmsIOHANDLER *iohandler1 = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
    if (iohandler1) {
        cmsCloseIOhandler(iohandler1);
    }
    
    // Test cmsOpenIOhandlerFromMem
    if (Size > 0) {
        void *buffer = malloc(Size);
        if (buffer) {
            memcpy(buffer, Data, Size);
            cmsIOHANDLER *iohandler2 = cmsOpenIOhandlerFromMem(context, buffer, (cmsUInt32Number)Size, "r");
            if (iohandler2) {
                cmsCloseIOhandler(iohandler2);
            }
            free(buffer);
        }
    }
    
    // Test cmsOpenProfileFromStream
    if (Size > 0) {
        FILE *stream2 = fopen("./dummy_file", "rb");
        if (stream2) {
            cmsHPROFILE profile2 = cmsOpenProfileFromStream(stream2, "r");
            if (profile2) {
                cmsCloseProfile(profile2);
            }
            // Note: Do NOT close the FILE* stream - library takes ownership
        }
    }
    
    // Test cmsOpenProfileFromFileTHR
    cmsHPROFILE profile3 = cmsOpenProfileFromFileTHR(context, "./dummy_file", "r");
    if (profile3) {
        cmsCloseProfile(profile3);
    }
    
    // Test cmsOpenProfileFromFile
    cmsHPROFILE profile4 = cmsOpenProfileFromFile("./dummy_file", "r");
    if (profile4) {
        cmsCloseProfile(profile4);
    }
    
    // Clean up context
    cmsDeleteContext(context);
    
    // Clean up dummy file
    cleanup_dummy_file();
    
    return 0;
}