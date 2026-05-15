// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

static void test_functions(cmsHPROFILE hProfile, const uint8_t *Data, size_t Size) {
    // Test cmsGetHeaderProfileID
    cmsUInt8Number ProfileID[16];
    cmsGetHeaderProfileID(hProfile, ProfileID);
    
    // Test cmsSetEncodedICCversion - use first 4 bytes of input as version
    cmsUInt32Number encoded_version = 0x4000000; // Default to 4.0.0.0
    if (Size >= sizeof(cmsUInt32Number)) {
        memcpy(&encoded_version, Data, sizeof(cmsUInt32Number));
    }
    cmsSetEncodedICCversion(hProfile, encoded_version);
    
    // Test cmsSetProfileVersion - use next 8 bytes as double version
    cmsFloat64Number profile_version = 2.0; // Default to 2.0
    if (Size >= sizeof(cmsUInt32Number) + sizeof(cmsFloat64Number)) {
        memcpy(&profile_version, Data + sizeof(cmsUInt32Number), sizeof(cmsFloat64Number));
    }
    cmsSetProfileVersion(hProfile, profile_version);
    
    // Test cmsGetEncodedICCversion
    cmsUInt32Number retrieved_encoded = cmsGetEncodedICCversion(hProfile);
    
    // Test cmsGetProfileVersion
    cmsFloat64Number retrieved_version = cmsGetProfileVersion(hProfile);
    
    // Test cmsGetDeviceClass
    cmsProfileClassSignature device_class = cmsGetDeviceClass(hProfile);
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    // Create a dummy ICC profile file from input data
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    // Write at least enough data for a minimal ICC profile header
    if (Size > 0) {
        fwrite(Data, 1, Size > 128 ? 128 : Size, fp);
    } else {
        // Write minimal ICC profile header if no data
        unsigned char minimal_header[128] = {0};
        fwrite(minimal_header, 1, 128, fp);
    }
    fclose(fp);
    
    // Try to open profile from the dummy file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    
    // If file opening fails, create a synthetic profile
    if (!hProfile) {
        hProfile = cmsCreate_sRGBProfile();
    }
    
    if (hProfile) {
        test_functions(hProfile, Data, Size);
        cmsCloseProfile(hProfile);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}