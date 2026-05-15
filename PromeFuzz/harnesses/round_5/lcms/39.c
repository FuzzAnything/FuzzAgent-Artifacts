// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
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
    if (size > 0) {
        return cmsOpenProfileFromMem(data, size);
    }
    return cmsCreate_sRGBProfile();
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Initialize variables from fuzz data
    cmsUInt32Number intent = Data[0] % 4;  // 0-3 valid intents
    cmsUInt32Number flags = *(const cmsUInt32Number*)Data % 256;
    cmsUInt32Number direction = Data[1] % 2;  // 0 or 1
    
    // Create test profile
    cmsHPROFILE profile = create_test_profile(Data, Size);
    if (!profile) {
        // Fallback to sRGB profile if creation fails
        profile = cmsCreate_sRGBProfile();
        if (!profile) return 0;
    }
    
    // Test cmsDetectDestinationBlackPoint
    cmsCIEXYZ blackPoint1;
    cmsDetectDestinationBlackPoint(&blackPoint1, profile, intent, flags);
    
    // Test cmsGetPostScriptCSA
    cmsUInt32Number bufferSize = cmsGetPostScriptCSA(0, profile, intent, flags, NULL, 0);
    if (bufferSize > 0 && bufferSize < 65536) {
        void *buffer = malloc(bufferSize);
        if (buffer) {
            cmsGetPostScriptCSA(0, profile, intent, flags, buffer, bufferSize);
            free(buffer);
        }
    }
    
    // Test cmsDetectBlackPoint
    cmsCIEXYZ blackPoint2;
    cmsDetectBlackPoint(&blackPoint2, profile, intent, flags);
    
    // Test cmsIsMatrixShaper
    cmsIsMatrixShaper(profile);
    
    // Test cmsIsIntentSupported
    cmsIsIntentSupported(profile, intent, direction);
    
    // Test cmsIsCLUT
    cmsIsCLUT(profile, intent, direction);
    
    // Cleanup
    cmsCloseProfile(profile);
    
    return 0;
}