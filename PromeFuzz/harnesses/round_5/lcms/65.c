// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    // Create dummy file with fuzzer input
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Create context for functions that require it
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Test cmsOpenProfileFromStream with read mode - use separate FILE*
    FILE *profile1_file = fopen("./dummy_file", "rb");
    cmsHPROFILE profile1 = NULL;
    if (profile1_file) {
        profile1 = cmsOpenProfileFromStream(profile1_file, "r");
        // Don't close profile1_file - lcms takes ownership
    }
    
    // Test cmsOpenProfileFromStreamTHR with read mode - use separate FILE*
    FILE *profile2_file = fopen("./dummy_file", "rb");
    cmsHPROFILE profile2 = NULL;
    if (profile2_file) {
        profile2 = cmsOpenProfileFromStreamTHR(context, profile2_file, "r");
        // Don't close profile2_file - lcms takes ownership
    }
    
    // Test cmsOpenIOhandlerFromStream - use separate FILE*
    FILE *iohandler1_file = fopen("./dummy_file", "rb");
    cmsIOHANDLER *iohandler1 = NULL;
    if (iohandler1_file) {
        iohandler1 = cmsOpenIOhandlerFromStream(context, iohandler1_file);
        // Don't close iohandler1_file - lcms takes ownership
    }
    
    // Test cmsOpenIOhandlerFromFile - this opens its own file
    cmsIOHANDLER *iohandler2 = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
    
    // Test cmsSaveProfileToStream if we have a valid profile
    if (profile1) {
        // Create output file for saving
        FILE *output_file = fopen("./dummy_output", "wb");
        if (output_file) {
            cmsSaveProfileToStream(profile1, output_file);
            fclose(output_file);  // cmsSaveProfileToStream doesn't take ownership
            remove("./dummy_output");
        }
    }
    
    // Cleanup
    if (profile1) {
        cmsCloseProfile(profile1);
    }
    if (profile2) {
        cmsCloseProfile(profile2);
    }
    if (iohandler1) {
        cmsCloseIOhandler(iohandler1);
    }
    if (iohandler2) {
        cmsCloseIOhandler(iohandler2);
    }
    
    remove("./dummy_file");
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    return 0;
}