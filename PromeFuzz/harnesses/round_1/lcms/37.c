// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"

// Memory stream structure
typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
    size_t pos;
} mem_stream;

// Define memory I/O handler functions
static cmsUInt32Number mem_read(cmsIOHANDLER* iohandler, void* buffer, 
                                cmsUInt32Number size, cmsUInt32Number count) {
    if (!iohandler || !buffer || !iohandler->stream) return 0;
    
    size_t total_bytes = (size_t)size * (size_t)count;
    if (total_bytes == 0) return 0;
    
    mem_stream* stream = (mem_stream*)iohandler->stream;
    size_t available = stream->size - stream->pos;
    size_t to_read = total_bytes < available ? total_bytes : available;
    
    if (to_read > 0) {
        memcpy(buffer, stream->data + stream->pos, to_read);
        stream->pos += to_read;
    }
    
    return (cmsUInt32Number)(to_read / size);
}

static cmsBool mem_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (!iohandler || !iohandler->stream) return FALSE;
    
    mem_stream* stream = (mem_stream*)iohandler->stream;
    if (offset <= stream->size) {
        stream->pos = offset;
        return TRUE;
    }
    return FALSE;
}

static cmsBool mem_close(cmsIOHANDLER* iohandler) {
    if (!iohandler) return FALSE;
    if (iohandler->stream) {
        mem_stream* stream = (mem_stream*)iohandler->stream;
        free(stream->data);
        free(stream);
        iohandler->stream = NULL;
    }
    free(iohandler);
    return TRUE;
}

static cmsUInt32Number mem_tell(cmsIOHANDLER* iohandler) {
    if (!iohandler || !iohandler->stream) return 0;
    mem_stream* stream = (mem_stream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool mem_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* buffer) {
    if (!iohandler || !buffer || !iohandler->stream) return FALSE;
    
    mem_stream* stream = (mem_stream*)iohandler->stream;
    size_t new_pos = stream->pos + size;
    
    // Resize if needed
    if (new_pos > stream->capacity) {
        size_t new_capacity = new_pos * 2;
        uint8_t* new_data = realloc(stream->data, new_capacity);
        if (!new_data) return FALSE;
        stream->data = new_data;
        stream->capacity = new_capacity;
    }
    
    memcpy(stream->data + stream->pos, buffer, size);
    stream->pos = new_pos;
    if (stream->pos > stream->size) {
        stream->size = stream->pos;
    }
    return TRUE;
}

// Create memory I/O handler
static cmsIOHANDLER* create_mem_handler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* handler = malloc(sizeof(cmsIOHANDLER));
    if (!handler) return NULL;
    
    mem_stream* stream = malloc(sizeof(mem_stream));
    if (!stream) {
        free(handler);
        return NULL;
    }
    
    stream->capacity = size * 2 + 1;
    stream->data = malloc(stream->capacity);
    if (!stream->data) {
        free(stream);
        free(handler);
        return NULL;
    }
    
    if (size > 0 && data) {
        memcpy(stream->data, data, size);
    }
    stream->size = size;
    stream->pos = 0;
    
    handler->stream = stream;
    handler->ContextID = NULL;
    handler->UsedSpace = 0;
    handler->ReportedSize = (cmsUInt32Number)size;
    memset(handler->PhysicalFile, 0, sizeof(handler->PhysicalFile));
    handler->Read = mem_read;
    handler->Seek = mem_seek;
    handler->Close = mem_close;
    handler->Tell = mem_tell;
    handler->Write = mem_write;
    
    return handler;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create memory I/O handler
    cmsIOHANDLER* io = create_mem_handler(Data, Size);
    if (!io) return 0;
    
    // Test _cmsReadUInt32Number
    cmsUInt32Number read_val = 0;
    _cmsReadUInt32Number(io, &read_val);
    
    // Reset position
    io->Seek(io, 0);
    
    // Test _cmsWriteUInt32Number
    cmsUInt32Number write_val = read_val ^ 0xFFFFFFFF;
    _cmsWriteUInt32Number(io, write_val);
    
    // Test _cmsReadUInt16Array
    io->Seek(io, 0);
    cmsUInt32Number array_size = (cmsUInt32Number)(Size / sizeof(cmsUInt16Number));
    if (array_size > 100) array_size = 100; // Limit size
    cmsUInt16Number* array = malloc(array_size * sizeof(cmsUInt16Number));
    if (array) {
        _cmsReadUInt16Array(io, array_size, array);
        
        // Test _cmsWriteUInt16Array
        io->Seek(io, 0);
        _cmsWriteUInt16Array(io, array_size, array);
        free(array);
    }
    
    // Test _cmsWriteAlignment
    io->Seek(io, 0);
    _cmsWriteAlignment(io);
    
    // Test _cmsReadAlignment
    io->Seek(io, 0);
    _cmsReadAlignment(io);
    
    // Cleanup
    io->Close(io);
    
    return 0;
}