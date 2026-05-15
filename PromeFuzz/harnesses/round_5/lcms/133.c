// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    write_dummy_file(Data, Size);
    
    FILE *stream = fopen("./dummy_file", "rb");
    if (!stream) {
        cleanup_file();
        return 0;
    }
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        fclose(stream);
        cleanup_file();
        return 0;
    }
    
    cmsIOHANDLER *io1 = cmsOpenIOhandlerFromStream(context, stream);
    if (!io1) {
        fclose(stream);
        cmsDeleteContext(context);
        cleanup_file();
        return 0;
    }
    
    cmsHPROFILE profile = cmsOpenProfileFromIOhandler2THR(context, io1, FALSE);
    if (profile) {
        cmsIOHANDLER *io2 = cmsGetProfileIOhandler(profile);
        if (io2) {
            cmsUInt16Number array[16];
            size_t n = Size % 16;
            if (n > 0) {
                _cmsReadUInt16Array(io2, n, array);
            }
        }
        
        FILE *stream2 = fopen("./dummy_file", "rb");
        if (stream2) {
            cmsIOHANDLER *io3 = cmsOpenIOhandlerFromStream(context, stream2);
            if (io3) {
                cmsGetPostScriptColorResource(context, cmsPS_RESOURCE_CSA, profile, 0, 0, io3);
                cmsCloseIOhandler(io3);
            } else {
                fclose(stream2);
            }
        }
        
        FILE *stream3 = fopen("./dummy_file", "wb");
        if (stream3) {
            cmsIOHANDLER *io4 = cmsOpenIOhandlerFromStream(context, stream3);
            if (io4) {
                cmsSaveProfileToIOhandler(profile, io4);
                cmsCloseIOhandler(io4);
            } else {
                fclose(stream3);
            }
        }
        
        cmsCloseProfile(profile);
    }
    
    cmsDeleteContext(context);
    cleanup_file();
    return 0;
}