// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lcms2.h"

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the fuzz data
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Try to open the profile from the dummy file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile(filename, "r");
    
    // If we successfully opened a profile, test the API functions
    if (hProfile) {
        // Test getter functions
        cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
        cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
        cmsUInt32Number model = cmsGetHeaderModel(hProfile);
        cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);
        
        // Use the fuzz data to generate values for setter functions
        // Use first 4 bytes for manufacturer, next 4 for model (if available)
        cmsUInt32Number new_manufacturer = 0;
        cmsUInt32Number new_model = 0;
        
        if (Size >= sizeof(cmsUInt32Number)) {
            memcpy(&new_manufacturer, Data, sizeof(cmsUInt32Number));
        }
        if (Size >= 2 * sizeof(cmsUInt32Number)) {
            memcpy(&new_model, Data + sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
        }
        
        // Test setter functions
        cmsSetHeaderManufacturer(hProfile, new_manufacturer);
        cmsSetHeaderModel(hProfile, new_model);
        
        // Test getter functions again to see if values changed
        manufacturer = cmsGetHeaderManufacturer(hProfile);
        model = cmsGetHeaderModel(hProfile);
        
        // Clean up
        cmsCloseProfile(hProfile);
    }
    
    // Remove the dummy file
    remove(filename);
    
    return 0;
}