// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lcms2_plugin.h"

#define BUFFER_SIZE 4096
#define MAX_ARRAY_SIZE 256

// Memory stream implementation for fuzzing
typedef struct {
    uint8_t* data;
    size_t size;
    size_t pos;
    size_t capacity;
} MemoryStream;

static cmsUInt32Number MemoryRead(cmsIOHANDLER* iohandler, void* Buffer, 
                                  cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
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

static cmsBool MemorySeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (offset <= stream->size) {
        stream->pos = offset;
        return TRUE;
    }
    return FALSE;
}

static cmsBool MemoryWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, 
                           const void* Buffer) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (stream->pos + size > stream->capacity) {
        // Resize buffer if needed
        size_t new_capacity = stream->capacity * 2;
        if (new_capacity < stream->pos + size) {
            new_capacity = stream->pos + size;
        }
        
        uint8_t* new_data = realloc(stream->data, new_capacity);
        if (!new_data) return FALSE;
        
        stream->data = new_data;
        stream->capacity = new_capacity;
    }
    
    memcpy(stream->data + stream->pos, Buffer, size);
    stream->pos += size;
    
    if (stream->pos > stream->size) {
        stream->size = stream->pos;
    }
    
    return TRUE;
}

static cmsUInt32Number MemoryTell(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    return stream->pos;
}

static cmsBool MemoryClose(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    free(stream->data);
    free(stream);
    free(iohandler);
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryHandler() {
    cmsIOHANDLER* io = calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    MemoryStream* stream = calloc(1, sizeof(MemoryStream));
    if (!stream) {
        free(io);
        return NULL;
    }
    
    stream->data = malloc(BUFFER_SIZE);
    if (!stream->data) {
        free(stream);
        free(io);
        return NULL;
    }
    
    stream->capacity = BUFFER_SIZE;
    stream->size = 0;
    stream->pos = 0;
    
    io->stream = stream;
    io->Read = MemoryRead;
    io->Seek = MemorySeek;
    io->Write = MemoryWrite;
    io->Tell = MemoryTell;
    io->Close = MemoryClose;
    
    return io;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create memory I/O handler
    cmsIOHANDLER* io = CreateMemoryHandler();
    if (!io) return 0;
    
    // Test _cmsWriteUInt32Number
    cmsUInt32Number write_val = *(cmsUInt32Number*)Data;
    _cmsWriteUInt32Number(io, write_val);
    
    // Test _cmsWriteUInt16Array
    size_t array_size = Size / 2;
    if (array_size > MAX_ARRAY_SIZE) array_size = MAX_ARRAY_SIZE;
    
    cmsUInt16Number* write_array = malloc(array_size * sizeof(cmsUInt16Number));
    if (write_array) {
        for (size_t i = 0; i < array_size; i++) {
            write_array[i] = (cmsUInt16Number)(Data[i % Size] | (Data[(i + 1) % Size] << 8));
        }
        _cmsWriteUInt16Array(io, array_size, write_array);
        free(write_array);
    }
    
    // Test _cmsWriteAlignment
    _cmsWriteAlignment(io);
    
    // Reset position for reading
    MemorySeek(io, 0);
    
    // Test _cmsReadUInt32Number
    cmsUInt32Number read_val;
    _cmsReadUInt32Number(io, &read_val);
    
    // Test _cmsReadUInt16Array
    cmsUInt16Number* read_array = malloc(array_size * sizeof(cmsUInt16Number));
    if (read_array) {
        _cmsReadUInt16Array(io, array_size, read_array);
        free(read_array);
    }
    
    // Test _cmsReadAlignment
    _cmsReadAlignment(io);
    
    // Cleanup
    MemoryClose(io);
    
    return 0;
}