// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "lcms2_plugin.h"

// Memory-based I/O handler implementation
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
    uint8_t* write_buffer;
    size_t write_size;
    size_t write_capacity;
    size_t write_pos;
} mem_stream;

static cmsUInt32Number mem_read(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    cmsUInt32Number total = size * count;
    
    if (stream->pos + total > stream->size) {
        total = stream->size - stream->pos;
    }
    
    if (total > 0) {
        memcpy(Buffer, stream->data + stream->pos, total);
        stream->pos += total;
    }
    
    return total / size;
}

static cmsBool mem_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    
    if (offset > stream->size) {
        return FALSE;
    }
    
    stream->pos = offset;
    return TRUE;
}

static cmsBool mem_close(cmsIOHANDLER* iohandler) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    
    if (stream->write_buffer) {
        free(stream->write_buffer);
    }
    free(stream);
    free(iohandler);
    
    return TRUE;
}

static cmsUInt32Number mem_tell(cmsIOHANDLER* iohandler) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool mem_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    
    if (stream->write_pos + size > stream->write_capacity) {
        size_t new_capacity = stream->write_capacity * 2;
        if (new_capacity < stream->write_pos + size) {
            new_capacity = stream->write_pos + size;
        }
        
        uint8_t* new_buffer = realloc(stream->write_buffer, new_capacity);
        if (!new_buffer) {
            return FALSE;
        }
        
        stream->write_buffer = new_buffer;
        stream->write_capacity = new_capacity;
    }
    
    memcpy(stream->write_buffer + stream->write_pos, Buffer, size);
    stream->write_pos += size;
    stream->write_size = stream->write_pos;
    
    return TRUE;
}

static cmsIOHANDLER* create_mem_handler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* handler = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!handler) {
        return NULL;
    }
    
    mem_stream* stream = (mem_stream*)calloc(1, sizeof(mem_stream));
    if (!stream) {
        free(handler);
        return NULL;
    }
    
    stream->data = data;
    stream->size = size;
    stream->pos = 0;
    stream->write_capacity = 4096;
    stream->write_buffer = (uint8_t*)malloc(stream->write_capacity);
    if (!stream->write_buffer) {
        free(stream);
        free(handler);
        return NULL;
    }
    stream->write_pos = 0;
    stream->write_size = 0;
    
    handler->stream = stream;
    handler->Read = mem_read;
    handler->Seek = mem_seek;
    handler->Close = mem_close;
    handler->Tell = mem_tell;
    handler->Write = mem_write;
    
    return handler;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }
    
    // Test _cmsAdjustEndianess64
    cmsUInt64Number result, qword;
    memcpy(&qword, Data, sizeof(cmsUInt64Number));
    _cmsAdjustEndianess64(&result, &qword);
    
    // Test I/O functions with memory handler
    cmsIOHANDLER* io = create_mem_handler(Data, Size);
    if (!io) {
        return 0;
    }
    
    // Test _cmsReadUInt64Number
    cmsUInt64Number read_u64;
    if (Size >= sizeof(cmsUInt64Number)) {
        _cmsReadUInt64Number(io, &read_u64);
    }
    
    // Reset position for next test
    io->Seek(io, 0);
    
    // Test _cmsWriteUInt32Number
    cmsUInt32Number write_u32;
    if (Size >= sizeof(cmsUInt32Number)) {
        memcpy(&write_u32, Data, sizeof(cmsUInt32Number));
        _cmsWriteUInt32Number(io, write_u32);
    }
    
    // Test _cmsWriteUInt64Number
    cmsUInt64Number write_u64;
    if (Size >= sizeof(cmsUInt64Number)) {
        memcpy(&write_u64, Data, sizeof(cmsUInt64Number));
        _cmsWriteUInt64Number(io, &write_u64);
    }
    
    // Reset position for next test
    io->Seek(io, 0);
    
    // Test _cmsReadFloat32Number
    cmsFloat32Number read_f32;
    if (Size >= sizeof(cmsFloat32Number)) {
        _cmsReadFloat32Number(io, &read_f32);
    }
    
    // Test _cmsWriteFloat32Number
    cmsFloat32Number write_f32;
    if (Size >= sizeof(cmsFloat32Number)) {
        memcpy(&write_f32, Data, sizeof(cmsFloat32Number));
        _cmsWriteFloat32Number(io, write_f32);
    }
    
    // Cleanup
    io->Close(io);
    
    return 0;
}