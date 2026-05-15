// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSaveProfileToFile at cmsio0.c:1502:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void cleanup_files(void) {
    remove("./dummy_file");
    remove("./saved_profile.icc");
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Test cmsOpenIOhandlerFromMem
    cmsIOHANDLER* mem_handler = cmsOpenIOhandlerFromMem(context, (void*)Data, (cmsUInt32Number)Size, "r");
    if (mem_handler) {
        // Close handler properly
        cmsCloseIOhandler(mem_handler);
    }
    
    // Test cmsOpenIOhandlerFromFile
    cmsIOHANDLER* file_handler = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
    if (file_handler) {
        cmsCloseIOhandler(file_handler);
    }
    
    // Test cmsOpenProfileFromFile
    cmsHPROFILE profile1 = cmsOpenProfileFromFile("./dummy_file", "r");
    if (profile1) {
        cmsCloseProfile(profile1);
    }
    
    // Test cmsOpenProfileFromFileTHR
    cmsHPROFILE profile2 = cmsOpenProfileFromFileTHR(context, "./dummy_file", "r");
    if (profile2) {
        cmsCloseProfile(profile2);
    }
    
    // Test cmsOpenProfileFromStream
    FILE* stream = fopen("./dummy_file", "rb");
    if (stream) {
        cmsHPROFILE profile3 = cmsOpenProfileFromStream(stream, "r");
        if (profile3) {
            cmsCloseProfile(profile3);
        }
        fclose(stream);
    }
    
    // Test cmsSaveProfileToFile - create a simple profile first
    cmsHPROFILE test_profile = cmsCreate_sRGBProfile();
    if (test_profile) {
        cmsSaveProfileToFile(test_profile, "./saved_profile.icc");
        cmsCloseProfile(test_profile);
    }
    
    // Test with different access modes
    if (Size > 10) {
        // Try write mode if we have enough data
        cmsIOHANDLER* write_handler = cmsOpenIOhandlerFromMem(context, (void*)Data, (cmsUInt32Number)Size, "w");
        if (write_handler) {
            cmsCloseIOhandler(write_handler);
        }
        
        // Try file handler with write mode
        cmsIOHANDLER* file_write_handler = cmsOpenIOhandlerFromFile(context, "./dummy_file", "w");
        if (file_write_handler) {
            cmsCloseIOhandler(file_write_handler);
        }
    }
    
    // Test with different context
    cmsContext alt_context = cmsCreateContext(NULL, NULL);
    if (alt_context) {
        cmsHPROFILE profile4 = cmsOpenProfileFromFileTHR(alt_context, "./dummy_file", "r");
        if (profile4) {
            cmsCloseProfile(profile4);
        }
        cmsDeleteContext(alt_context);
    }
    
    cmsDeleteContext(context);
    cleanup_files();
    
    return 0;
}