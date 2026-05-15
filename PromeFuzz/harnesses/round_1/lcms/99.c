// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file for file-based operations
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Test cmsfilelength
    FILE* f1 = fopen("./dummy_file", "rb");
    if (f1 != NULL) {
        long int file_len = cmsfilelength(f1);
        fclose(f1);
    }
    
    // Test cmsOpenProfileFromFile
    cmsHPROFILE hProfile1 = cmsOpenProfileFromFile("./dummy_file", "r");
    cleanup_profile(hProfile1);
    
    // Test cmsOpenProfileFromFile with write mode
    cmsHPROFILE hProfile2 = cmsOpenProfileFromFile("./dummy_file", "w");
    cleanup_profile(hProfile2);
    
    // Test cmsOpenProfileFromStream - DO NOT close the FILE* after profile cleanup
    FILE* f2 = fopen("./dummy_file", "rb");
    if (f2 != NULL) {
        cmsHPROFILE hProfile3 = cmsOpenProfileFromStream(f2, "r");
        cleanup_profile(hProfile3);
        // fclose(f2); // REMOVED: lcms already closes the stream via cmsCloseProfile
    }
    
    // Test cmsOpenProfileFromStream with write mode - DO NOT close the FILE* after profile cleanup
    FILE* f3 = fopen("./dummy_file", "wb");
    if (f3 != NULL) {
        cmsHPROFILE hProfile4 = cmsOpenProfileFromStream(f3, "w");
        cleanup_profile(hProfile4);
        // fclose(f3); // REMOVED: lcms already closes the stream via cmsCloseProfile
    }
    
    // Test cmsOpenProfileFromStreamTHR - DO NOT close the FILE* after profile cleanup
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx != NULL) {
        FILE* f4 = fopen("./dummy_file", "rb");
        if (f4 != NULL) {
            cmsHPROFILE hProfile5 = cmsOpenProfileFromStreamTHR(ctx, f4, "r");
            cleanup_profile(hProfile5);
            // fclose(f4); // REMOVED: lcms already closes the stream via cmsCloseProfile
        }
        
        // Test cmsOpenIOhandlerFromFile
        cmsIOHANDLER* io = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
        // I/O handler cleanup is handled by the library when context is deleted
        
        // Test cmsSaveProfileToStream with a dummy profile
        cmsHPROFILE dummy_profile = cmsCreateRGBProfileTHR(ctx, NULL, NULL, NULL);
        if (dummy_profile != NULL) {
            FILE* f5 = fopen("./dummy_file", "wb");
            if (f5 != NULL) {
                cmsBool save_result = cmsSaveProfileToStream(dummy_profile, f5);
                fclose(f5); // This is OK - we opened this FILE* separately for saving
            }
            cleanup_profile(dummy_profile);
        }
        
        cmsDeleteContext(ctx);
    }
    
    // Test cmsOpenProfileFromStreamTHR with write mode - DO NOT close the FILE* after profile cleanup
    cmsContext ctx2 = cmsCreateContext(NULL, NULL);
    if (ctx2 != NULL) {
        FILE* f6 = fopen("./dummy_file", "wb");
        if (f6 != NULL) {
            cmsHPROFILE hProfile6 = cmsOpenProfileFromStreamTHR(ctx2, f6, "w");
            cleanup_profile(hProfile6);
            // fclose(f6); // REMOVED: lcms already closes the stream via cmsCloseProfile
        }
        cmsDeleteContext(ctx2);
    }
    
    // Test cmsOpenIOhandlerFromFile with write mode
    cmsContext ctx3 = cmsCreateContext(NULL, NULL);
    if (ctx3 != NULL) {
        cmsIOHANDLER* io2 = cmsOpenIOhandlerFromFile(ctx3, "./dummy_file", "w");
        // I/O handler cleanup is handled by the library when context is deleted
        cmsDeleteContext(ctx3);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}