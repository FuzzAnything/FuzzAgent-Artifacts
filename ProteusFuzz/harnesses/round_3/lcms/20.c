// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsHPROFILE hProfile = NULL;
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    
    if (!ContextID) return 0;

    // Try to open a profile from the input data
    // Note: cmsOpenProfileFromMem does not take a context argument in this version
    hProfile = cmsOpenProfileFromMem(Data, Size);

    if (hProfile) {
        // Test getters
        cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
        (void)manufacturer;

        cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
        (void)creator;

        cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
        (void)flags;

        cmsUInt32Number version = cmsGetEncodedICCversion(hProfile);
        (void)version;

        // Test setters - these might fail if the profile is read-only
        // We try to set flags and manufacturer
        cmsSetHeaderFlags(hProfile, flags | 0x1); // Modify flags slightly
        cmsSetHeaderManufacturer(hProfile, manufacturer ^ 0x1); // Modify manufacturer slightly

        // Close the profile
        cmsCloseProfile(hProfile);
    }

    // Try to create a profile from a minimal buffer if the input was too small
    if (!hProfile && Size < 128) {
        // Construct a minimal ICC header (128 bytes)
        uint8_t minimal_header[128] = {0};
        // Set size to 128 (0x00000080)
        minimal_header[0] = 0x00;
        minimal_header[1] = 0x00;
        minimal_header[2] = 0x00;
        minimal_header[3] = 0x80;
        // Set CMM type (e.g., "lcms")
        minimal_header[4] = 'l';
        minimal_header[5] = 'c';
        minimal_header[6] = 'm';
        minimal_header[7] = 's';
        // Set version (e.g., 2.0 -> 0x02000000)
        minimal_header[8] = 0x02;
        minimal_header[9] = 0x00;
        minimal_header[10] = 0x00;
        minimal_header[11] = 0x00;
        // Set device class (e.g., "mntr")
        minimal_header[12] = 'm';
        minimal_header[13] = 'n';
        minimal_header[14] = 't';
        minimal_header[15] = 'r';
        // Set color space (e.g., "RGB ")
        minimal_header[16] = 'R';
        minimal_header[17] = 'G';
        minimal_header[18] = 'B';
        minimal_header[19] = ' ';
        // Rest of header is zeros (checksum, etc.)
        
        hProfile = cmsOpenProfileFromMem(minimal_header, sizeof(minimal_header));
        
        if (hProfile) {
            // Test getters
            cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
            (void)manufacturer;

            cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
            (void)creator;

            cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
            (void)flags;

            cmsUInt32Number version = cmsGetEncodedICCversion(hProfile);
            (void)version;

            // Test setters
            cmsSetHeaderFlags(hProfile, flags | 0x1);
            cmsSetHeaderManufacturer(hProfile, manufacturer ^ 0x1);

            cmsCloseProfile(hProfile);
        }
    }

    // Cleanup context
    cmsDeleteContext(ContextID);

    return 0;
}