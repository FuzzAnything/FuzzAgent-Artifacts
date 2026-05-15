// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void cleanup_handler(cmsIOHANDLER* io) {
    if (io) {
        cmsCloseIOhandler(io);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Reopen for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    // Test cmsOpenIOhandlerFromStream
    cmsIOHANDLER* io = cmsOpenIOhandlerFromStream(NULL, fp);
    if (!io) {
        fclose(fp);
        return 0;
    }
    
    // Test cmsOpenProfileFromIOhandler2THR with read mode
    cmsHPROFILE hProfile = cmsOpenProfileFromIOhandler2THR(NULL, io, FALSE);
    
    // If profile creation failed, io is already closed by library
    // So we need to track this state
    int io_closed_by_library = 0;
    
    if (hProfile) {
        // Profile opened successfully, io is still valid
        
        // Test cmsGetProfileIOhandler
        cmsIOHANDLER* retrieved_io = cmsGetProfileIOhandler(hProfile);
        
        // Test cmsSaveProfileToIOhandler - create new file for output
        FILE *out_fp = fopen("./dummy_file_out", "wb");
        if (out_fp) {
            cmsIOHANDLER* out_io = cmsOpenIOhandlerFromStream(NULL, out_fp);
            if (out_io) {
                cmsSaveProfileToIOhandler(hProfile, out_io);
                cleanup_handler(out_io);  // This closes out_fp too
            } else {
                fclose(out_fp);
            }
        }
        
        // Test _cmsReadUInt16Array
        if (retrieved_io) {
            // Try to read some data from current position
            cmsUInt16Number array[16];
            size_t elements_to_read = Size > 32 ? 16 : Size / 2;
            if (elements_to_read > 0) {
                _cmsReadUInt16Array(retrieved_io, (cmsUInt32Number)elements_to_read, array);
            }
        }
        
        // Test cmsGetPostScriptColorResource
        // Create another file for PS output
        FILE *ps_fp = fopen("./dummy_file_ps", "wb");
        if (ps_fp) {
            cmsIOHANDLER* ps_io = cmsOpenIOhandlerFromStream(NULL, ps_fp);
            if (ps_io) {
                // Try different resource types
                for (int type = 0; type < 3; type++) {
                    cmsGetPostScriptColorResource(NULL, (cmsPSResourceType)type, hProfile, 
                                                  0, 0, ps_io);
                }
                cleanup_handler(ps_io);  // This closes ps_fp too
            } else {
                fclose(ps_fp);
            }
        }
        
        // Clean up profile
        cleanup_profile(hProfile);
        // Now we can safely close the original io since profile is closed
        cleanup_handler(io);
    } else {
        // Profile creation failed, io was already closed by library
        // Don't call cleanup_handler(io) - it would double-free
        io_closed_by_library = 1;
        // But we still need to close the FILE* stream
        // Actually, when cmsOpenProfileFromIOhandler2THR fails and closes io,
        // it also closes the underlying FILE* stream, so we shouldn't fclose(fp)
        // Just mark that we shouldn't close it
    }
    
    // Clean up temporary files
    remove("./dummy_file");
    remove("./dummy_file_out");
    remove("./dummy_file_ps");
    
    return 0;
}