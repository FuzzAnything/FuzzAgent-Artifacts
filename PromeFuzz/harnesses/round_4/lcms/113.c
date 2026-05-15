// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateDeviceLinkFromCubeFile at cmscgats.c:3285:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateDeviceLinkFromCubeFile at cmscgats.c:3285:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
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

static void write_dummy_file(const uint8_t *data, size_t size, const char* filename) {
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    // Create a context for functions that require it
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Test cmsOpenIOhandlerFromMem with various access modes
    if (Size > 0) {
        // Test with read mode
        cmsIOHANDLER* handler1 = cmsOpenIOhandlerFromMem(context, (void*)Data, Size, "r");
        if (handler1) {
            cmsCloseIOhandler(handler1);
        }
        
        // Test with write mode (even though writing to read-only buffer may fail)
        cmsIOHANDLER* handler2 = cmsOpenIOhandlerFromMem(context, (void*)Data, Size, "w");
        if (handler2) {
            cmsCloseIOhandler(handler2);
        }
        
        // Test with invalid mode
        cmsIOHANDLER* handler3 = cmsOpenIOhandlerFromMem(context, (void*)Data, Size, "invalid");
        if (handler3) {
            cmsCloseIOhandler(handler3);
        }
    }
    
    // Test cmsOpenProfileFromStreamTHR
    if (Size > 0) {
        write_dummy_file(Data, Size, "./dummy_file");
        FILE* stream = fopen("./dummy_file", "rb");
        if (stream) {
            cmsHPROFILE profile1 = cmsOpenProfileFromStreamTHR(context, stream, "r");
            if (profile1) {
                cmsCloseProfile(profile1);
            } else {
                // Library already closed the stream when returning NULL
                // Do NOT call fclose(stream) here
            }
        }
        
        // Also test with write mode
        FILE* stream2 = fopen("./dummy_file", "wb");
        if (stream2) {
            cmsHPROFILE profile2 = cmsOpenProfileFromStreamTHR(context, stream2, "w");
            if (profile2) {
                cmsCloseProfile(profile2);
            }
        }
    }
    
    // Test cmsOpenIOhandlerFromFile
    if (Size > 0) {
        write_dummy_file(Data, Size, "./dummy_file");
        
        // Test with read mode
        cmsIOHANDLER* file_handler1 = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
        if (file_handler1) {
            cmsCloseIOhandler(file_handler1);
        }
        
        // Test with write mode
        cmsIOHANDLER* file_handler2 = cmsOpenIOhandlerFromFile(context, "./dummy_file", "w");
        if (file_handler2) {
            cmsCloseIOhandler(file_handler2);
        }
        
        // Test with invalid filename
        cmsIOHANDLER* file_handler3 = cmsOpenIOhandlerFromFile(context, "./nonexistent_file", "r");
        if (file_handler3) {
            cmsCloseIOhandler(file_handler3);
        }
    }
    
    // Test cmsCreateDeviceLinkFromCubeFile
    if (Size > 0) {
        write_dummy_file(Data, Size, "./dummy_file.cube");
        cmsHPROFILE cube_profile1 = cmsCreateDeviceLinkFromCubeFile("./dummy_file.cube");
        if (cube_profile1) {
            cmsCloseProfile(cube_profile1);
        }
        
        // Test with invalid filename
        cmsHPROFILE cube_profile2 = cmsCreateDeviceLinkFromCubeFile("./nonexistent.cube");
        if (cube_profile2) {
            cmsCloseProfile(cube_profile2);
        }
    }
    
    // Test cmsOpenProfileFromFileTHR
    if (Size > 0) {
        write_dummy_file(Data, Size, "./dummy_file");
        
        // Test with read mode
        cmsHPROFILE file_profile1 = cmsOpenProfileFromFileTHR(context, "./dummy_file", "r");
        if (file_profile1) {
            cmsCloseProfile(file_profile1);
        }
        
        // Test with write mode
        cmsHPROFILE file_profile2 = cmsOpenProfileFromFileTHR(context, "./dummy_file", "w");
        if (file_profile2) {
            cmsCloseProfile(file_profile2);
        }
        
        // Test with invalid filename
        cmsHPROFILE file_profile3 = cmsOpenProfileFromFileTHR(context, "./nonexistent_file", "r");
        if (file_profile3) {
            cmsCloseProfile(file_profile3);
        }
    }
    
    // Test cmsCreateDeviceLinkFromCubeFileTHR
    if (Size > 0) {
        write_dummy_file(Data, Size, "./dummy_file.cube");
        cmsHPROFILE cube_profile_thr1 = cmsCreateDeviceLinkFromCubeFileTHR(context, "./dummy_file.cube");
        if (cube_profile_thr1) {
            cmsCloseProfile(cube_profile_thr1);
        }
        
        // Test with invalid filename
        cmsHPROFILE cube_profile_thr2 = cmsCreateDeviceLinkFromCubeFileTHR(context, "./nonexistent.cube");
        if (cube_profile_thr2) {
            cmsCloseProfile(cube_profile_thr2);
        }
    }
    
    // Clean up context
    cmsDeleteContext(context);
    
    // Clean up dummy files
    remove("./dummy_file");
    remove("./dummy_file.cube");
    
    return 0;
}