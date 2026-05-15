// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "lcms2.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Create a profile from dummy file
    write_dummy_file(Data, Size);
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!hProfile) {
        // If file parsing fails, create a simple sRGB profile
        hProfile = cmsCreate_sRGBProfile();
        if (!hProfile) {
            unlink("./dummy_file");
            return 0;
        }
    }

    // Extract values from fuzzer input for various parameters
    cmsUInt32Number manufacturer = 0;
    cmsUInt32Number renderingIntent = 0;
    cmsUInt32Number flags = 0;
    
    // Safely extract values with bounds checking
    if (Size >= sizeof(cmsUInt32Number)) {
        manufacturer = *(cmsUInt32Number*)Data;
    }
    if (Size >= 2 * sizeof(cmsUInt32Number)) {
        renderingIntent = *(cmsUInt32Number*)(Data + sizeof(cmsUInt32Number));
    }
    if (Size >= 3 * sizeof(cmsUInt32Number)) {
        flags = *(cmsUInt32Number*)(Data + 2 * sizeof(cmsUInt32Number));
    }

    // Test cmsSetHeaderManufacturer
    cmsSetHeaderManufacturer(hProfile, manufacturer);

    // Test cmsGetHeaderCreator
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    (void)creator; // Use value to avoid optimization

    // Test cmsGetHeaderFlags - only call with valid profile
    cmsUInt32Number headerFlags = cmsGetHeaderFlags(hProfile);
    (void)headerFlags;

    // Test cmsGetHeaderModel - only call with valid profile
    cmsUInt32Number model = cmsGetHeaderModel(hProfile);
    (void)model;

    // Test cmsSetHeaderRenderingIntent
    cmsSetHeaderRenderingIntent(hProfile, renderingIntent);

    // Test cmsSetHeaderFlags
    cmsSetHeaderFlags(hProfile, flags);

    // Try different combinations to explore more states
    if (Size >= 4 * sizeof(cmsUInt32Number)) {
        // Alternate values from different positions
        cmsUInt32Number altManufacturer = *(cmsUInt32Number*)(Data + 3 * sizeof(cmsUInt32Number));
        cmsSetHeaderManufacturer(hProfile, altManufacturer);
        
        if (Size >= 5 * sizeof(cmsUInt32Number)) {
            cmsUInt32Number altFlags = *(cmsUInt32Number*)(Data + 4 * sizeof(cmsUInt32Number));
            cmsSetHeaderFlags(hProfile, altFlags);
        }
        
        // Get values again after modifications
        (void)cmsGetHeaderCreator(hProfile);
        (void)cmsGetHeaderFlags(hProfile);
        (void)cmsGetHeaderModel(hProfile);
    }

    // Clean up
    cmsCloseProfile(hProfile);
    unlink("./dummy_file");

    return 0;
}