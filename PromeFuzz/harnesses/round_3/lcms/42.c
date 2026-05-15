// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
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

// Custom I/O handler implementation for fuzzing
typedef struct {
    cmsIOHANDLER base;
    const uint8_t* data;
    size_t size;
    size_t pos;
    cmsBool is_closed;
} FuzzIOHandler;

static cmsUInt32Number FuzzRead(cmsIOHANDLER* iohandler, void* buffer, 
                                cmsUInt32Number size, cmsUInt32Number count) {
    FuzzIOHandler* fuzz_io = (FuzzIOHandler*)iohandler;
    if (fuzz_io->is_closed) return 0;
    
    size_t total_bytes = size * count;
    size_t available = fuzz_io->size - fuzz_io->pos;
    size_t to_read = total_bytes < available ? total_bytes : available;
    
    if (to_read > 0) {
        memcpy(buffer, fuzz_io->data + fuzz_io->pos, to_read);
        fuzz_io->pos += to_read;
    }
    return (cmsUInt32Number)(to_read / size);
}

static cmsBool FuzzSeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    FuzzIOHandler* fuzz_io = (FuzzIOHandler*)iohandler;
    if (fuzz_io->is_closed) return FALSE;
    
    if (offset <= fuzz_io->size) {
        fuzz_io->pos = offset;
        return TRUE;
    }
    return FALSE;
}

static cmsBool FuzzClose(cmsIOHANDLER* iohandler) {
    FuzzIOHandler* fuzz_io = (FuzzIOHandler*)iohandler;
    if (fuzz_io->is_closed) return FALSE;
    
    fuzz_io->is_closed = TRUE;
    return TRUE;
}

static cmsUInt32Number FuzzTell(cmsIOHANDLER* iohandler) {
    FuzzIOHandler* fuzz_io = (FuzzIOHandler*)iohandler;
    if (fuzz_io->is_closed) return 0;
    return (cmsUInt32Number)fuzz_io->pos;
}

static cmsBool FuzzWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, 
                         const void* buffer) {
    FuzzIOHandler* fuzz_io = (FuzzIOHandler*)iohandler;
    if (fuzz_io->is_closed) return FALSE;
    
    // For fuzzing, we don't actually write anywhere
    fuzz_io->pos += size;
    return TRUE;
}

static FuzzIOHandler* CreateFuzzIOHandler(cmsContext ctx, const uint8_t* data, size_t size) {
    FuzzIOHandler* fuzz_io = (FuzzIOHandler*)_cmsMalloc(ctx, sizeof(FuzzIOHandler));
    if (!fuzz_io) return NULL;
    
    memset(fuzz_io, 0, sizeof(FuzzIOHandler));
    fuzz_io->base.ContextID = ctx;
    fuzz_io->base.Read = FuzzRead;
    fuzz_io->base.Seek = FuzzSeek;
    fuzz_io->base.Close = FuzzClose;
    fuzz_io->base.Tell = FuzzTell;
    fuzz_io->base.Write = FuzzWrite;
    fuzz_io->data = data;
    fuzz_io->size = size;
    fuzz_io->pos = 0;
    fuzz_io->is_closed = FALSE;
    
    return fuzz_io;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create a dummy file with fuzzing data
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }
    
    // Test cmsOpenProfileFromIOhandler2THR with custom I/O handler
    FuzzIOHandler* fuzz_io = CreateFuzzIOHandler(ctx, Data, Size);
    if (fuzz_io) {
        cmsHPROFILE profile = cmsOpenProfileFromIOhandler2THR(ctx, (cmsIOHANDLER*)fuzz_io, FALSE);
        
        if (profile) {
            // Test cmsGetProfileIOhandler
            cmsIOHANDLER* profile_io = cmsGetProfileIOhandler(profile);
            
            // Test _cmsReadUInt16Array
            if (profile_io && Size >= 2) {
                cmsUInt16Number array[10];
                cmsUInt32Number n = Size / 2;
                if (n > 10) n = 10;
                _cmsReadUInt16Array(profile_io, n, array);
            }
            
            // Test cmsSaveProfileToIOhandler
            FuzzIOHandler* write_io = CreateFuzzIOHandler(ctx, NULL, 0);
            if (write_io) {
                cmsSaveProfileToIOhandler(profile, (cmsIOHANDLER*)write_io);
                cmsCloseIOhandler((cmsIOHANDLER*)write_io);
                _cmsFree(ctx, write_io);
            }
            
            // Test cmsGetPostScriptColorResource
            FuzzIOHandler* ps_io = CreateFuzzIOHandler(ctx, NULL, 0);
            if (ps_io) {
                // Use a valid resource type from the fuzzing input
                cmsPSResourceType resourceType = (cmsPSResourceType)(Data[0] % 2); // 0 or 1
                cmsGetPostScriptColorResource(ctx, resourceType, profile, 
                                             INTENT_PERCEPTUAL, 0, (cmsIOHANDLER*)ps_io);
                cmsCloseIOhandler((cmsIOHANDLER*)ps_io);
                _cmsFree(ctx, ps_io);
            }
            
            cmsCloseProfile(profile);
        } else {
            // Profile creation failed, I/O handler already closed by library
            _cmsFree(ctx, fuzz_io);
        }
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    cmsDeleteContext(ctx);
    return 0;
}