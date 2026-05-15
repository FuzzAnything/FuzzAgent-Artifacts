// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cmsWriteTag(cmsHPROFILE hProfile, const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    cmsTagSignature sig;
    memcpy(&sig, Data, 4);
    
    if (Size > 4) {
        // Write tag with data
        cmsWriteTag(hProfile, sig, Data + 4);
    } else {
        // Delete tag (NULL data)
        cmsWriteTag(hProfile, sig, NULL);
    }
}

static void fuzz_cmsWriteRawTag(cmsHPROFILE hProfile, const uint8_t *Data, size_t Size) {
    if (Size < 8) return;
    
    cmsTagSignature sig;
    cmsUInt32Number rawSize;
    
    memcpy(&sig, Data, 4);
    memcpy(&rawSize, Data + 4, 4);
    
    // Ensure rawSize doesn't exceed available data
    if (rawSize > Size - 8) {
        rawSize = (cmsUInt32Number)(Size - 8);
    }
    
    if (rawSize > 0) {
        cmsWriteRawTag(hProfile, sig, Data + 8, rawSize);
    }
}

static void fuzz_cmsIsTag(cmsHPROFILE hProfile, const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    cmsTagSignature sig;
    memcpy(&sig, Data, 4);
    
    cmsIsTag(hProfile, sig);
}

static void fuzz_cmsMD5computeID(cmsHPROFILE hProfile) {
    cmsMD5computeID(hProfile);
}

static void fuzz_cmsSaveProfileToMem(cmsHPROFILE hProfile) {
    cmsUInt32Number bytesNeeded = 0;
    
    // First call to get required size
    cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
    
    if (bytesNeeded > 0 && bytesNeeded < 1024 * 1024) { // Limit to 1MB
        void *buffer = malloc(bytesNeeded);
        if (buffer) {
            cmsSaveProfileToMem(hProfile, buffer, &bytesNeeded);
            free(buffer);
        }
    }
}

static void fuzz_cmsLinkTag(cmsHPROFILE hProfile, const uint8_t *Data, size_t Size) {
    if (Size < 8) return;
    
    cmsTagSignature sig, dest;
    memcpy(&sig, Data, 4);
    memcpy(&dest, Data + 4, 4);
    
    cmsLinkTag(hProfile, sig, dest);
}

static cmsHPROFILE create_test_profile(void) {
    // Create a simple RGB profile for testing
    cmsToneCurve* transferFunction[3];
    transferFunction[0] = cmsBuildGamma(NULL, 2.2);
    transferFunction[1] = cmsBuildGamma(NULL, 2.2);
    transferFunction[2] = cmsBuildGamma(NULL, 2.2);
    
    cmsHPROFILE hProfile = cmsCreateRGBProfile(
        cmsD50_xyY(),
        NULL,  // Use NULL for primaries to use sRGB primaries
        transferFunction
    );
    
    // Free the tone curves - cmsCreateRGBProfile creates copies
    cmsFreeToneCurve(transferFunction[0]);
    cmsFreeToneCurve(transferFunction[1]);
    cmsFreeToneCurve(transferFunction[2]);
    
    return hProfile;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create a test profile
    cmsHPROFILE hProfile = create_test_profile();
    if (!hProfile) return 0;
    
    // Use the first byte to determine which functions to call
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    // Call different API functions based on selector
    if (selector & 0x01) {
        fuzz_cmsWriteTag(hProfile, fuzz_data, fuzz_size);
    }
    
    if (selector & 0x02) {
        fuzz_cmsWriteRawTag(hProfile, fuzz_data, fuzz_size);
    }
    
    if (selector & 0x04) {
        fuzz_cmsIsTag(hProfile, fuzz_data, fuzz_size);
    }
    
    if (selector & 0x08) {
        fuzz_cmsMD5computeID(hProfile);
    }
    
    if (selector & 0x10) {
        fuzz_cmsSaveProfileToMem(hProfile);
    }
    
    if (selector & 0x20) {
        fuzz_cmsLinkTag(hProfile, fuzz_data, fuzz_size);
    }
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}