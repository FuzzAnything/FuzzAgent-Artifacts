// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
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
#include <lcms2.h>
#include <lcms2_plugin.h>

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create a standard context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;
    
    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    
    // Extract parameters from fuzz data if available
    if (Size >= 8) {
        intent = ((cmsUInt32Number)Data[0] << 24) | ((cmsUInt32Number)Data[1] << 16) | 
                 ((cmsUInt32Number)Data[2] << 8) | (cmsUInt32Number)Data[3];
        flags = ((cmsUInt32Number)Data[4] << 24) | ((cmsUInt32Number)Data[5] << 16) | 
                ((cmsUInt32Number)Data[6] << 8) | (cmsUInt32Number)Data[7];
    }
    
    // Try to create a dummy profile from fuzz data if possible
    // We need at least a minimal valid ICC header (128 bytes) to avoid NULL pointer deref inside library
    if (Size >= 128) {
        hProfile = cmsOpenProfileFromMem((const unsigned char*)Data, Size);
        if (hProfile == NULL) {
            hProfile = NULL;
        }
    }
    
    // Only proceed if we have a valid profile to avoid passing NULL to cmsGetPostScriptCSA
    if (hProfile != NULL) {
        // First call to cmsGetPostScriptCSA to get required size
        cmsUInt32Number requiredSize = cmsGetPostScriptCSA(ctx, hProfile, intent, flags, NULL, 0);
        
        // Allocate memory using _cmsMalloc
        // We cast the function to ensure it compiles if the prototype is slightly different
        void* buffer = _cmsMalloc(ctx, requiredSize);
        
        // Second call to cmsGetPostScriptCSA to fill the buffer
        if (buffer != NULL && requiredSize > 0) {
            cmsGetPostScriptCSA(ctx, hProfile, intent, flags, buffer, requiredSize);
        }
        
        // Free allocated memory using _cmsFree
        _cmsFree(ctx, buffer);
        
        // Cleanup profile if it was created
        cmsCloseProfile(hProfile);
    }
    
    // Cleanup context
    cmsDeleteContext(ctx);
    
    return 0;
}