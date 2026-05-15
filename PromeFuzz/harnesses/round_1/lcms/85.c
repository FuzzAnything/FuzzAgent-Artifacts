// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_file(const char* filename) {
    remove(filename);
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    const char* dummy_file = "./dummy_file";
    FILE* fp = NULL;
    cmsIOHANDLER* io = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsContext ctx = NULL;
    cmsBool write = FALSE;
    cmsUInt32Number result;
    cmsUInt16Number* array = NULL;
    cmsUInt32Number n;
    
    // Write fuzzing data to dummy file
    fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);
    fp = NULL;
    
    // Open file for reading
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        cleanup_file(dummy_file);
        return 0;
    }
    
    // Create I/O handler from stream
    io = cmsOpenIOhandlerFromStream(ctx, fp);
    if (!io) {
        fclose(fp);
        cleanup_file(dummy_file);
        return 0;
    }
    
    // Open profile from I/O handler
    hProfile = cmsOpenProfileFromIOhandler2THR(ctx, io, write);
    
    // Test cmsGetProfileIOhandler
    if (hProfile) {
        cmsIOHANDLER* retrieved_io = cmsGetProfileIOhandler(hProfile);
    }
    
    // Test _cmsReadUInt16Array with a separate I/O handler
    // Create a new I/O handler for reading test
    FILE* fp2 = fopen(dummy_file, "rb");
    if (fp2) {
        cmsIOHANDLER* io2 = cmsOpenIOhandlerFromStream(ctx, fp2);
        if (io2) {
            if (Size >= 2) {
                n = Size / 2;
                if (n > 100) n = 100; // Limit array size
                array = (cmsUInt16Number*)malloc(n * sizeof(cmsUInt16Number));
                if (array) {
                    _cmsReadUInt16Array(io2, n, array);
                    free(array);
                }
            }
            cmsCloseIOhandler(io2);
        } else {
            fclose(fp2);
        }
    }
    
    // Test cmsSaveProfileToIOhandler if we have a valid profile
    if (hProfile) {
        FILE* fp3 = fopen(dummy_file, "wb");
        if (fp3) {
            cmsIOHANDLER* io3 = cmsOpenIOhandlerFromStream(ctx, fp3);
            if (io3) {
                result = cmsSaveProfileToIOhandler(hProfile, io3);
                // io3 will be closed by cmsSaveProfileToIOhandler
            } else {
                fclose(fp3);
            }
        }
    }
    
    // Test cmsGetPostScriptColorResource if we have a valid profile
    if (hProfile) {
        FILE* fp4 = fopen(dummy_file, "wb");
        if (fp4) {
            cmsIOHANDLER* io4 = cmsOpenIOhandlerFromStream(ctx, fp4);
            if (io4) {
                // Try different resource types
                for (int type = 0; type < 2; type++) {
                    result = cmsGetPostScriptColorResource(ctx, 
                                                           (cmsPSResourceType)type,
                                                           hProfile,
                                                           0, // Intent
                                                           0, // dwFlags
                                                           io4);
                }
                // io4 will be closed by cmsGetPostScriptColorResource
            } else {
                fclose(fp4);
            }
        }
    }
    
    // Cleanup
    if (hProfile) {
        cmsCloseProfile(hProfile);
        // After closing profile, the original io is already closed
        // Don't close fp as it was closed by the I/O handler
    } else {
        // If profile creation failed, io was already closed by library
        // Don't close fp as it was closed by the I/O handler
    }
    
    // Only close fp if we failed to create the I/O handler (already handled above)
    // or if we never passed it to any function that takes ownership
    
    cleanup_file(dummy_file);
    
    return 0;
}