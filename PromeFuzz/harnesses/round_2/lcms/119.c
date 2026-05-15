// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
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
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *file = fopen("./dummy_file", "wb");
    if (file) {
        fwrite(data, 1, size, file);
        fclose(file);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create a dummy profile from memory for testing
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (!hProfile) {
        // If we can't create from memory, try to create a simple sRGB profile
        hProfile = cmsCreate_sRGBProfile();
        if (!hProfile) return 0;
    }
    
    // Extract parameters from fuzzer input
    cmsUInt32Number intent = *(const cmsUInt32Number*)Data;
    cmsUInt32Number flags = *(const cmsUInt32Number*)(Data + (Size > 4 ? 4 : 0));
    cmsUInt32Number direction = *(const cmsUInt32Number*)(Data + (Size > 8 ? 8 : 0));
    
    // Test cmsGetHeaderRenderingIntent
    (void)cmsGetHeaderRenderingIntent(hProfile);
    
    // Test cmsGetPostScriptCSA
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context) {
        // First call to get required buffer size
        cmsUInt32Number bufferSize = cmsGetPostScriptCSA(context, hProfile, intent, flags, NULL, 0);
        
        // Second call with actual buffer if size is reasonable
        if (bufferSize > 0 && bufferSize < 1024 * 1024) {
            void *buffer = malloc(bufferSize);
            if (buffer) {
                (void)cmsGetPostScriptCSA(context, hProfile, intent, flags, buffer, bufferSize);
                free(buffer);
            }
        }
        cmsDeleteContext(context);
    }
    
    // Test cmsIsIntentSupported
    (void)cmsIsIntentSupported(hProfile, intent, direction);
    
    // Test cmsDetectBlackPoint
    cmsCIEXYZ blackPoint;
    (void)cmsDetectBlackPoint(&blackPoint, hProfile, intent, flags);
    
    // Test cmsDetectDestinationBlackPoint
    (void)cmsDetectDestinationBlackPoint(&blackPoint, hProfile, intent, flags);
    
    // Test cmsIsCLUT
    (void)cmsIsCLUT(hProfile, intent, direction);
    
    // Cleanup
    if (hProfile) cmsCloseProfile(hProfile);
    
    // Test with file-based profile
    write_dummy_file(Data, Size);
    cmsHPROFILE fileProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (fileProfile) {
        (void)cmsGetHeaderRenderingIntent(fileProfile);
        cmsCloseProfile(fileProfile);
    }
    remove_dummy_file();
    
    return 0;
}