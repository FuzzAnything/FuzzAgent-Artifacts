// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
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

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    // Create a profile from the fuzz input
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (hProfile == NULL) {
        // If input is not a valid profile, create a default one to still test the APIs
        hProfile = cmsCreate_sRGBProfile();
        if (hProfile == NULL) {
            return 0;
        }
    }

    // Test cmsGetHeaderManufacturer
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    (void)manufacturer; // Use result to avoid unused variable warning

    // Test cmsSetProfileVersion and cmsGetProfileVersion
    // Use first 8 bytes of input to create a version number if available
    double version = 2.0; // Default version
    if (Size >= sizeof(double)) {
        // Use the first 8 bytes as a double (with some bounds checking)
        memcpy(&version, Data, sizeof(double));
        // Ensure version is reasonable (ICC versions are typically 2.0-4.4)
        if (version < 1.0 || version > 5.0) {
            version = 2.0 + ((double)(Data[0] % 30)) / 10.0; // 2.0 to 5.0
        }
    } else if (Size > 0) {
        version = 2.0 + ((double)(Data[0] % 30)) / 10.0;
    }
    cmsSetProfileVersion(hProfile, version);
    double retrievedVersion = cmsGetProfileVersion(hProfile);
    (void)retrievedVersion;

    // Test cmsGetHeaderCreator
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    (void)creator;

    // Test cmsGetEncodedICCversion
    cmsUInt32Number encodedVersion = cmsGetEncodedICCversion(hProfile);
    (void)encodedVersion;

    // Test cmsGetProfileInfoASCII
    // Prepare parameters using fuzz data
    char LanguageCode[3] = "en";
    char CountryCode[3] = "US";
    
    // Use fuzz data to set language/country codes if available
    if (Size >= 6) {
        // Use bytes 0-2 for language code, 3-5 for country code
        memcpy(LanguageCode, Data, 3);
        memcpy(CountryCode, Data + 3, 3);
    }
    
    // Use fuzz data to determine info type
    cmsInfoType infoType = cmsInfoDescription; // Default
    if (Size >= 7) {
        // Use byte 6 to select info type (modulo number of available types)
        // Based on common lcms2.h enum values
        int typeIndex = Data[6] % 4; // Only 4 types available
        switch (typeIndex) {
            case 0: infoType = cmsInfoDescription; break;
            case 1: infoType = cmsInfoManufacturer; break;
            case 2: infoType = cmsInfoModel; break;
            case 3: infoType = cmsInfoCopyright; break;
        }
    }
    
    // Buffer for retrieved info
    char Buffer[256];
    cmsUInt32Number bufferSize = sizeof(Buffer);
    
    // Call the function
    cmsUInt32Number bytesCopied = cmsGetProfileInfoASCII(hProfile, infoType, 
                                                         LanguageCode, CountryCode, 
                                                         Buffer, bufferSize);
    (void)bytesCopied;

    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}