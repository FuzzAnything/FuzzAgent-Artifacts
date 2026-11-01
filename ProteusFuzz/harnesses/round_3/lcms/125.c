// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Helper structure to hold stream data
typedef struct {
    const uint8_t *data;
    size_t size;
    size_t pos;
    uint8_t *write_buf;
    size_t write_pos;
    size_t write_size;
    cmsContext ContextID;
} dummy_stream_t;

// Callback implementations
static cmsUInt32Number dummy_read(struct _cms_io_handler *h, void *buf, cmsUInt32Number size, cmsUInt32Number count) {
    dummy_stream_t *s = (dummy_stream_t*)h->stream;
    cmsUInt32Number total = size * count;
    cmsUInt32Number available = (cmsUInt32Number)(s->size - s->pos);
    cmsUInt32Number to_read = total < available ? total : available;
    if (to_read == 0) return 0;
    memcpy(buf, s->data + s->pos, to_read);
    s->pos += to_read;
    return to_read / size;
}

static cmsBool dummy_seek(struct _cms_io_handler *h, cmsUInt32Number offset) {
    dummy_stream_t *s = (dummy_stream_t*)h->stream;
    if (offset > s->size) return FALSE;
    s->pos = offset;
    return TRUE;
}

static cmsBool dummy_close(struct _cms_io_handler *h) {
    dummy_stream_t *s = (dummy_stream_t*)h->stream;
    if (s) {
        if (s->write_buf) _cmsFree(s->ContextID, s->write_buf);
        _cmsFree(s->ContextID, s);
    }
    // Free the handler itself
    _cmsFree(h->ContextID, h);
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler *h) {
    dummy_stream_t *s = (dummy_stream_t*)h->stream;
    return (cmsUInt32Number)s->pos;
}

static cmsBool dummy_write(struct _cms_io_handler *h, cmsUInt32Number size, const void *buf) {
    dummy_stream_t *s = (dummy_stream_t*)h->stream;
    if (!s->write_buf) return FALSE;
    if (s->write_pos + size > s->write_size) return FALSE;
    memcpy(s->write_buf + s->write_pos, buf, size);
    s->write_pos += size;
    return TRUE;
}

// Helper to create a minimal IO handler for fuzzing
static cmsIOHANDLER* create_dummy_io_handler(const uint8_t *data, size_t size) {
    cmsContext ctx = cmsCreateContext(NULL, 0);
    if (!ctx) return NULL;

    cmsIOHANDLER *io = (cmsIOHANDLER*)_cmsMalloc(ctx, sizeof(cmsIOHANDLER));
    if (!io) {
        cmsDeleteContext(ctx);
        return NULL;
    }

    memset(io, 0, sizeof(cmsIOHANDLER));
    
    dummy_stream_t *stream = (dummy_stream_t*)_cmsMalloc(ctx, sizeof(dummy_stream_t));
    if (!stream) {
        _cmsFree(ctx, io);
        cmsDeleteContext(ctx);
        return NULL;
    }
    memset(stream, 0, sizeof(dummy_stream_t));
    stream->data = data;
    stream->size = size;
    stream->pos = 0;
    stream->ContextID = ctx;
    
    // Allocate a write buffer
    stream->write_size = size > 0 ? size : 1024;
    stream->write_buf = (uint8_t*)_cmsMalloc(ctx, stream->write_size);
    if (!stream->write_buf) {
        _cmsFree(ctx, stream);
        _cmsFree(ctx, io);
        cmsDeleteContext(ctx);
        return NULL;
    }
    memset(stream->write_buf, 0, stream->write_size);

    io->stream = stream;
    io->ContextID = ctx;
    io->UsedSpace = 0;
    io->ReportedSize = size;
    memset(io->PhysicalFile, 0, sizeof(io->PhysicalFile));

    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;

    return io;
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy IO handler
    cmsIOHANDLER *io = create_dummy_io_handler(Data, Size);
    if (!io) return 0;

    cmsBool result;
    cmsCIEXYZ xyz = {0, 0, 0};
    cmsUInt64Number n64 = 0;

    // Test _cmsReadXYZNumber
    // Reset position to 0
    if (io->Seek) io->Seek(io, 0);
    result = _cmsReadXYZNumber(io, &xyz);
    (void)result; 

    // Test _cmsWriteXYZNumber
    // Reset position to 0 for write buffer
    if (io->Seek) io->Seek(io, 0);
    xyz.X = 1.0; xyz.Y = 2.0; xyz.Z = 3.0;
    result = _cmsWriteXYZNumber(io, &xyz);
    (void)result;

    // Test _cmsReadAlignment
    if (io->Seek) io->Seek(io, 0);
    result = _cmsReadAlignment(io);
    (void)result;

    // Test _cmsWriteAlignment
    if (io->Seek) io->Seek(io, 0);
    result = _cmsWriteAlignment(io);
    (void)result;

    // Test _cmsReadUInt64Number
    if (io->Seek) io->Seek(io, 0);
    result = _cmsReadUInt64Number(io, &n64);
    (void)result;

    // Test with NULL pointers where safe
    if (io->Seek) io->Seek(io, 0);
    result = _cmsReadUInt64Number(io, NULL);
    (void)result;

    // Test cmsCloseIOhandler
    result = cmsCloseIOhandler(io);
    (void)result;

    return 0;
}