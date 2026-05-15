// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lcms2_plugin.h"

typedef struct {
    const uint8_t *data;
    size_t size;
    size_t pos;
} mem_stream;

static cmsUInt32Number mem_read(cmsIOHANDLER* iohandler, void* buffer,
                               cmsUInt32Number size, cmsUInt32Number count) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    size_t total = size * count;
    if (stream->pos + total > stream->size) {
        total = stream->size - stream->pos;
    }
    if (total > 0) {
        memcpy(buffer, stream->data + stream->pos, total);
        stream->pos += total;
    }
    return (cmsUInt32Number)(total / size);
}

static cmsBool mem_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    if (offset > stream->size) return FALSE;
    stream->pos = offset;
    return TRUE;
}

static cmsUInt32Number mem_tell(cmsIOHANDLER* iohandler) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool mem_close(cmsIOHANDLER* iohandler) {
    free(iohandler->stream);
    free(iohandler);
    return TRUE;
}

static cmsBool mem_write(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                        const void* buffer) {
    (void)iohandler;
    (void)size;
    (void)buffer;
    return TRUE;
}

static cmsIOHANDLER* create_mem_handler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    mem_stream* stream = (mem_stream*)calloc(1, sizeof(mem_stream));
    if (!stream) {
        free(io);
        return NULL;
    }
    
    stream->data = data;
    stream->size = size;
    stream->pos = 0;
    
    io->stream = stream;
    io->Read = mem_read;
    io->Seek = mem_seek;
    io->Tell = mem_tell;
    io->Close = mem_close;
    io->Write = mem_write;
    
    return io;
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    cmsIOHANDLER* io = create_mem_handler(Data, Size);
    if (!io) return 0;
    
    cmsTagTypeSignature sig = (cmsTagTypeSignature)(*(uint32_t*)Data);
    _cmsWriteTypeBase(io, sig);
    
    cmsCIEXYZ xyz;
    if (Size >= sizeof(cmsCIEXYZ)) {
        memcpy(&xyz, Data, sizeof(cmsCIEXYZ));
        _cmsWriteXYZNumber(io, &xyz);
    }
    
    cmsUInt64Number n64;
    if (_cmsReadUInt64Number(io, &n64)) {
        _cmsWriteUInt64Number(io, &n64);
    }
    
    _cmsWriteAlignment(io);
    _cmsReadAlignment(io);
    
    io->Close(io);
    return 0;
}