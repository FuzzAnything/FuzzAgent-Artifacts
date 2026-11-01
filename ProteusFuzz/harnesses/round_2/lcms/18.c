// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Custom IO handler implementation for memory buffer
static cmsUInt32Number MemRead(cmsIOHANDLER* io, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    size_t total = size * count;
    if (io == NULL || Buffer == NULL) return 0;
    
    // Custom data structure to hold buffer info
    struct {
        const uint8_t* data;
        size_t size;
        size_t offset;
    } *ctx = (struct {
        const uint8_t* data;
        size_t size;
        size_t offset;
    }*)io->stream;
    
    if (ctx == NULL) return 0;
    if (ctx->offset + total > ctx->size) return 0;
    
    memcpy(Buffer, ctx->data + ctx->offset, total);
    ctx->offset += total;
    return count;
}

static cmsBool MemSeek(cmsIOHANDLER* io, cmsUInt32Number offset) {
    if (io == NULL) return FALSE;
    
    struct {
        const uint8_t* data;
        size_t size;
        size_t offset;
    } *ctx = (struct {
        const uint8_t* data;
        size_t size;
        size_t offset;
    }*)io->stream;
    
    if (ctx == NULL) return FALSE;
    if (offset > ctx->size) return FALSE;
    
    ctx->offset = offset;
    return TRUE;
}

static cmsBool MemClose(cmsIOHANDLER* io) {
    if (io == NULL) return FALSE;
    return TRUE;
}

static cmsUInt32Number MemTell(cmsIOHANDLER* io) {
    if (io == NULL) return 0;
    
    struct {
        const uint8_t* data;
        size_t size;
        size_t offset;
    } *ctx = (struct {
        const uint8_t* data;
        size_t size;
        size_t offset;
    }*)io->stream;
    
    if (ctx == NULL) return 0;
    return ctx->offset;
}

static cmsBool MemWrite(cmsIOHANDLER* io, cmsUInt32Number size, const void* Buffer) {
    if (io == NULL || Buffer == NULL) return FALSE;
    return TRUE; // Simulate successful write
}

static void setup_io_handler(cmsIOHANDLER* io, const uint8_t* data, size_t size) {
    memset(io, 0, sizeof(cmsIOHANDLER));
    
    // Allocate context for stream
    struct {
        const uint8_t* data;
        size_t size;
        size_t offset;
    } *ctx = (struct {
        const uint8_t* data;
        size_t size;
        size_t offset;
    }*)malloc(sizeof(struct {
        const uint8_t* data;
        size_t size;
        size_t offset;
    }));
    
    if (ctx == NULL) return;
    
    ctx->data = data;
    ctx->size = size;
    ctx->offset = 0;
    
    io->stream = ctx;
    io->ContextID = cmsCreateContext(NULL, 0);
    io->UsedSpace = 0;
    io->ReportedSize = size;
    io->Read = MemRead;
    io->Seek = MemSeek;
    io->Close = MemClose;
    io->Tell = MemTell;
    io->Write = MemWrite;
}

static void cleanup_io_handler(cmsIOHANDLER* io) {
    if (io == NULL) return;
    
    if (io->stream != NULL) {
        free(io->stream);
        io->stream = NULL;
    }
    
    if (io->ContextID != NULL) {
        cmsDeleteContext(io->ContextID);
        io->ContextID = NULL;
    }
    
    memset(io, 0, sizeof(cmsIOHANDLER));
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    cmsIOHANDLER io;
    cmsUInt16Number val16;
    cmsUInt16Number array[100];
    cmsUInt32Number count;
    
    // Initialize IO handler
    setup_io_handler(&io, Data, Size);
    
    // Test _cmsReadUInt16Number
    _cmsReadUInt16Number(&io, &val16);
    
    // Test _cmsWriteUInt16Number
    _cmsWriteUInt16Number(&io, val16);
    
    // Test _cmsReadUInt16Array
    if (Size > 4) {
        count = (Size > 200) ? 100 : Size / 2;
        _cmsReadUInt16Array(&io, count, array);
    }
    
    // Test _cmsWriteUInt16Array
    if (Size > 4) {
        count = (Size > 200) ? 100 : Size / 2;
        _cmsWriteUInt16Array(&io, count, array);
    }
    
    // Test _cmsReadAlignment
    _cmsReadAlignment(&io);
    
    // Test cmsCloseIOhandler
    cmsCloseIOhandler(&io);
    
    // Cleanup
    cleanup_io_handler(&io);
    
    return 0;
}