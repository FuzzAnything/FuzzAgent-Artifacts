// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
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

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write data to dummy file for file-based functions
    write_dummy_file(Data, Size);
    
    cmsContext context = NULL;
    
    // Test cmsOpenIOhandlerFromMem
    cmsIOHANDLER* mem_handler = cmsOpenIOhandlerFromMem(context, (void*)Data, Size, "r");
    if (mem_handler) {
        cmsCloseIOhandler(mem_handler);
    }
    
    // Test cmsOpenIOhandlerFromFile with read mode
    cmsIOHANDLER* file_handler = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
    if (file_handler) {
        cmsCloseIOhandler(file_handler);
    }
    
    // Test cmsCreateDeviceLinkFromCubeFileTHR
    cmsHPROFILE cube_profile = cmsCreateDeviceLinkFromCubeFileTHR(context, "./dummy_file");
    if (cube_profile) {
        cmsCloseProfile(cube_profile);
    }
    
    // Test cmsOpenProfileFromStreamTHR - DO NOT close the FILE* stream after passing it to lcms
    // lcms takes ownership of the stream and will close it internally
    FILE* stream = fopen("./dummy_file", "rb");
    if (stream) {
        cmsHPROFILE stream_profile = cmsOpenProfileFromStreamTHR(context, stream, "r");
        if (stream_profile) {
            cmsCloseProfile(stream_profile);
        } else {
            // Only close the stream if lcms didn't take ownership (returned NULL)
            fclose(stream);
        }
    }
    
    // Test cmsIT8LoadFromFile
    cmsHANDLE it8_handle = cmsIT8LoadFromFile(context, "./dummy_file");
    if (it8_handle) {
        cmsIT8Free(it8_handle);
    }
    
    // Test cmsOpenProfileFromFileTHR with different modes
    const char* modes[] = {"r", "w", "r+"};
    for (int i = 0; i < 3; i++) {
        cmsHPROFILE file_profile = cmsOpenProfileFromFileTHR(context, "./dummy_file", modes[i]);
        if (file_profile) {
            cmsCloseProfile(file_profile);
        }
    }
    
    // Test cmsOpenIOhandlerFromMem with different modes
    const char* mem_modes[] = {"r", "w", "r+"};
    for (int i = 0; i < 3; i++) {
        cmsIOHANDLER* mem_handler2 = cmsOpenIOhandlerFromMem(context, (void*)Data, Size, mem_modes[i]);
        if (mem_handler2) {
            cmsCloseIOhandler(mem_handler2);
        }
    }
    
    // Test cmsOpenIOhandlerFromFile with write mode
    cmsIOHANDLER* write_handler = cmsOpenIOhandlerFromFile(context, "./dummy_file", "w");
    if (write_handler) {
        cmsCloseIOhandler(write_handler);
    }
    
    remove_dummy_file();
    return 0;
}