// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Static helper callbacks for memory IO handler
static cmsUInt32Number mem_read(cmsIOHANDLER* h, void* buf, cmsUInt32Number size, cmsUInt32Number count) {
    if (!h || !h->stream || !buf) return 0;
    cmsUInt32Number total = size * count;
    if (h->UsedSpace + total > h->ReportedSize) total = h->ReportedSize - h->UsedSpace;
    if (total == 0) return 0;
    memcpy(buf, (uint8_t*)h->stream + h->UsedSpace, total);
    h->UsedSpace += total;
    return total / size;
}

static cmsBool mem_seek(cmsIOHANDLER* h, cmsUInt32Number offset) {
    if (!h) return FALSE;
    if (offset > h->ReportedSize) return FALSE;
    h->UsedSpace = offset;
    return TRUE;
}

static cmsBool mem_close(cmsIOHANDLER* h) {
    if (!h) return FALSE;
    return TRUE;
}

static cmsUInt32Number mem_tell(cmsIOHANDLER* h) {
    if (!h) return 0;
    return h->UsedSpace;
}

static cmsBool mem_write(cmsIOHANDLER* h, cmsUInt32Number size, const void* buf) {
    if (!h || !h->stream || !buf) return FALSE;
    cmsUInt32Number total = size; 
    // Check bounds based on reported size + buffer overhead
    if (h->UsedSpace + total > h->ReportedSize + 256) return FALSE; 
    memcpy((uint8_t*)h->stream + h->UsedSpace, buf, total);
    h->UsedSpace += total;
    return TRUE;
}

// Static helper to create a memory-based IO handler for testing
static cmsIOHANDLER* create_memory_io_handler(const uint8_t* data, size_t size, int is_write, cmsContext ctx) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)_cmsCalloc(ctx, 1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;

    // Allocate a buffer for the stream
    if (is_write) {
        io->stream = malloc(size + 256); // Extra space for writes
        if (!io->stream) {
            _cmsFree(ctx, io);
            return NULL;
        }
        // Initialize with zeros
        memset(io->stream, 0, size + 256);
    } else {
        io->stream = (void*)data;
    }

    io->ContextID = ctx;
    io->UsedSpace = 0;
    io->ReportedSize = size;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);

    io->Read = mem_read;
    io->Seek = mem_seek;
    io->Close = mem_close;
    io->Tell = mem_tell;
    io->Write = mem_write;

    return io;
}

static void cleanup_io_handler(cmsIOHANDLER* io, cmsContext ctx, int is_write) {
    if (!io) return;
    
    // Free the stream if it was allocated (write mode)
    if (is_write && io->stream) {
        free(io->stream);
    }
    
    _cmsFree(ctx, io);
}

int LLVMFuzzerTestOneInput_165(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsIOHANDLER* io_read = NULL;
    cmsIOHANDLER* io_write = NULL;
    cmsBool result;
    cmsUInt16Number u16;
    cmsUInt8Number u8;

    // 1. Test Read Functions (using input data)
    io_read = create_memory_io_handler(Data, Size, 0, ctx);
    if (io_read) {
        // Test _cmsReadUInt16Number
        result = _cmsReadUInt16Number(io_read, &u16);
        (void)result;

        // Reset position for next test
        io_read->Seek(io_read, 0);

        // Test _cmsReadUInt8Number
        result = _cmsReadUInt8Number(io_read, &u8);
        (void)result;

        // Test _cmsReadAlignment
        io_read->Seek(io_read, 1);
        result = _cmsReadAlignment(io_read);
        (void)result;

        // Cleanup read handler (stream is input data, do not free)
        cleanup_io_handler(io_read, ctx, 0);
        io_read = NULL;
    }

    // 2. Test Write Functions (using a temporary buffer)
    size_t write_buf_size = Size > 0 ? Size : 1;
    uint8_t* write_buffer = malloc(write_buf_size + 256);
    if (!write_buffer) {
        cmsDeleteContext(ctx);
        return 0;
    }
    memset(write_buffer, 0, write_buf_size + 256);

    io_write = create_memory_io_handler(write_buffer, write_buf_size + 256, 1, ctx);
    if (io_write) {
        // Test _cmsWriteUInt16Number
        result = _cmsWriteUInt16Number(io_write, 0xABCD);
        (void)result;

        // Test _cmsWriteUInt8Number
        result = _cmsWriteUInt8Number(io_write, 0xFF);
        (void)result;

        // Cleanup write handler (stream is write_buffer, free it)
        cleanup_io_handler(io_write, ctx, 1);
        io_write = NULL;
    } else {
        free(write_buffer);
    }

    // 3. Test cmsCloseIOhandler directly
    cmsIOHANDLER* io_temp = create_memory_io_handler(Data, Size, 0, ctx);
    if (io_temp) {
        result = cmsCloseIOhandler(io_temp);
        (void)result;
        cleanup_io_handler(io_temp, ctx, 0);
    }

    cmsDeleteContext(ctx);
    return 0;
}