// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Write profile data to dummy file
    write_dummy_file(Data, Size);
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        remove_dummy_file();
        return 0;
    }
    
    // Open profile from dummy file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    
    // Only proceed if we have a valid profile
    if (hProfile) {
        // Extract intent and flags from fuzzer input
        cmsUInt32Number intent = 0;
        cmsUInt32Number dwFlags = 0;
        if (Size >= 8) {
            memcpy(&intent, Data, 4);
            memcpy(&dwFlags, Data + 4, 4);
        } else if (Size >= 4) {
            memcpy(&intent, Data, 4);
        }
        
        // First call to cmsGetPostScriptCSA to calculate required buffer size
        cmsUInt32Number bufferSize = cmsGetPostScriptCSA(context, hProfile, intent, dwFlags, NULL, 0);
        
        // Allocate memory using _cmsMalloc
        void *buffer = NULL;
        if (bufferSize > 0 && bufferSize < 1024*1024) { // Limit allocation size
            buffer = _cmsMalloc(context, bufferSize);
        }
        
        // Second call to cmsGetPostScriptCSA with allocated buffer
        if (buffer) {
            cmsUInt32Number bytesUsed = cmsGetPostScriptCSA(context, hProfile, intent, dwFlags, buffer, bufferSize);
            (void)bytesUsed; // Suppress unused variable warning
        }
        
        // Free allocated memory
        _cmsFree(context, buffer);
        
        // Close the profile
        cmsCloseProfile(hProfile);
    }
    
    // Cleanup
    cmsDeleteContext(context);
    remove_dummy_file();
    
    return 0;
}