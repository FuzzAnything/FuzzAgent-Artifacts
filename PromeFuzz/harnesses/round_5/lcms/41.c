// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
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

#include "lcms2.h"
#include "lcms2_plugin.h"

typedef struct {
    uint8_t* data;
    size_t size;
    size_t pos;
} MemoryStream;

static cmsUInt32Number MemoryRead(cmsIOHANDLER* iohandler, void* buffer, 
                                  cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    cmsUInt32Number total = size * count;
    
    if (stream->pos + total > stream->size) {
        total = stream->size - stream->pos;
    }
    
    if (total > 0) {
        memcpy(buffer, stream->data + stream->pos, total);
        stream->pos += total;
    }
    
    return total / size;
}

static cmsBool MemorySeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (offset > stream->size) {
        return FALSE;
    }
    
    stream->pos = offset;
    return TRUE;
}

static cmsUInt32Number MemoryTell(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool MemoryWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, 
                           const void* buffer) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (stream->pos + size > stream->size) {
        return FALSE;
    }
    
    memcpy(stream->data + stream->pos, buffer, size);
    stream->pos += size;
    return TRUE;
}

static cmsBool MemoryClose(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    free(stream->data);
    free(stream);
    free(iohandler);
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryHandler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* handler = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!handler) return NULL;
    
    MemoryStream* stream = (MemoryStream*)calloc(1, sizeof(MemoryStream));
    if (!stream) {
        free(handler);
        return NULL;
    }
    
    stream->data = (uint8_t*)malloc(size);
    if (!stream->data) {
        free(stream);
        free(handler);
        return NULL;
    }
    
    memcpy(stream->data, data, size);
    stream->size = size;
    stream->pos = 0;
    
    handler->stream = stream;
    handler->Read = MemoryRead;
    handler->Seek = MemorySeek;
    handler->Tell = MemoryTell;
    handler->Write = MemoryWrite;
    handler->Close = MemoryClose;
    
    return handler;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsIOHANDLER* io = CreateMemoryHandler(Data, Size);
    if (!io) return 0;
    
    // Test _cmsReadUInt8Number
    cmsUInt8Number read_val = 0;
    if (Size > 0) {
        _cmsReadUInt8Number(io, &read_val);
    }
    
    // Test _cmsWriteFloat32Number
    cmsFloat32Number write_float = 0.0f;
    if (Size >= 4) {
        memcpy(&write_float, Data, 4);
    }
    _cmsWriteFloat32Number(io, write_float);
    
    // Test _cmsWriteAlignment
    _cmsWriteAlignment(io);
    
    // Test _cmsWriteUInt8Number
    cmsUInt8Number write_val = (cmsUInt8Number)(Size > 0 ? Data[0] : 0);
    _cmsWriteUInt8Number(io, write_val);
    
    // Test _cmsReadAlignment
    if (Size > 0) {
        MemorySeek(io, 0);
        _cmsReadAlignment(io);
    }
    
    // Test cmsCloseIOhandler
    cmsCloseIOhandler(io);
    
    return 0;
}