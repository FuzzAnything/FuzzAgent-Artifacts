// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static cmsHPROFILE create_test_profile(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    
    // Try to open profile from memory first
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (hProfile) return hProfile;
    
    // Fallback to file-based profile creation
    write_dummy_file(data, size);
    hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    remove("./dummy_file");
    
    return hProfile;
}

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create test profile from fuzz data
    cmsHPROFILE hProfile = create_test_profile(Data, Size);
    if (!hProfile) return 0;
    
    // Extract parameters from fuzz data
    cmsUInt32Number intent = Data[0] % 4; // 0-3 for standard intents
    cmsUInt32Number direction = Data[1] % 2; // 0-1 for direction
    cmsUInt32Number flags = Data[2];
    
    // Test cmsIsMatrixShaper
    (void)cmsIsMatrixShaper(hProfile);
    
    // Test cmsDetectDestinationBlackPoint
    cmsCIEXYZ blackPoint1;
    (void)cmsDetectDestinationBlackPoint(&blackPoint1, hProfile, intent, flags);
    
    // Test cmsDetectBlackPoint
    cmsCIEXYZ blackPoint2;
    (void)cmsDetectBlackPoint(&blackPoint2, hProfile, intent, flags);
    
    // Test cmsGetPostScriptCSA
    cmsContext context = create_test_context();
    if (context) {
        // First call to get required buffer size
        cmsUInt32Number bufferSize = cmsGetPostScriptCSA(context, hProfile, intent, flags, NULL, 0);
        
        // Allocate buffer and make second call if size is reasonable
        if (bufferSize > 0 && bufferSize < 1024 * 1024) { // Limit to 1MB
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
    
    // Test cmsIsCLUT
    (void)cmsIsCLUT(hProfile, intent, direction);
    
    // Cleanup
    cmsCloseProfile(hProfile);
    
    return 0;
}