// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0 && data) {
            fwrite(data, 1, size, fp);
        }
        fclose(fp);
    }
}

static void cleanup_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        cleanup_file();
        return 0;
    }
    
    FILE *stream = fopen("./dummy_file", "rb");
    if (!stream) {
        cmsDeleteContext(ctx);
        cleanup_file();
        return 0;
    }
    
    cmsIOHANDLER* io = cmsOpenIOhandlerFromStream(ctx, stream);
    if (!io) {
        fclose(stream);
        cmsDeleteContext(ctx);
        cleanup_file();
        return 0;
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromIOhandler2THR(ctx, io, FALSE);
    
    if (hProfile) {
        cmsIOHANDLER* profile_io = cmsGetProfileIOhandler(hProfile);
        
        if (profile_io) {
            cmsUInt16Number array[16];
            _cmsReadUInt16Array(profile_io, (Size % 16) + 1, array);
        }
        
        FILE *out_stream = fopen("./dummy_file", "wb");
        if (out_stream) {
            cmsIOHANDLER* out_io = cmsOpenIOhandlerFromStream(ctx, out_stream);
            if (out_io) {
                cmsSaveProfileToIOhandler(hProfile, out_io);
                
                cmsGetPostScriptColorResource(ctx, cmsPS_RESOURCE_CSA, hProfile, 
                                             INTENT_PERCEPTUAL, 0, out_io);
                
                cmsCloseIOhandler(out_io);
            }
            fclose(out_stream);
        }
        
        cmsCloseProfile(hProfile);
    }
    
    if (io) {
        cmsCloseIOhandler(io);
    } else {
        fclose(stream);
    }
    
    cmsDeleteContext(ctx);
    cleanup_file();
    
    return 0;
}