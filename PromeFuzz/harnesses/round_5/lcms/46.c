// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
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

// Simple memory-based I/O handler implementation
typedef struct {
    const uint8_t *data;
    size_t size;
    size_t pos;
    uint8_t *write_buffer;
    size_t write_size;
    size_t write_capacity;
} memory_stream;

static cmsUInt32Number ReadMemory(cmsIOHANDLER* iohandler, void* Buffer, 
                                  cmsUInt32Number size, cmsUInt32Number count) {
    memory_stream* stream = (memory_stream*)iohandler->stream;
    size_t total = size * count;
    size_t available = stream->size - stream->pos;
    
    if (total > available) {
        total = available;
    }
    
    if (total > 0) {
        memcpy(Buffer, stream->data + stream->pos, total);
        stream->pos += total;
    }
    
    return (cmsUInt32Number)(total / size);
}

static cmsBool SeekMemory(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    memory_stream* stream = (memory_stream*)iohandler->stream;
    if (offset > stream->size) {
        return FALSE;
    }
    stream->pos = offset;
    return TRUE;
}

static cmsBool CloseMemory(cmsIOHANDLER* iohandler) {
    memory_stream* stream = (memory_stream*)iohandler->stream;
    if (stream->write_buffer) {
        free(stream->write_buffer);
    }
    free(stream);
    iohandler->stream = NULL;
    return TRUE;
}

static cmsUInt32Number TellMemory(cmsIOHANDLER* iohandler) {
    memory_stream* stream = (memory_stream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool WriteMemory(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    memory_stream* stream = (memory_stream*)iohandler->stream;
    
    // Allocate or expand write buffer if needed
    if (stream->write_size + size > stream->write_capacity) {
        size_t new_capacity = stream->write_capacity * 2;
        if (new_capacity < stream->write_size + size) {
            new_capacity = stream->write_size + size;
        }
        uint8_t* new_buffer = realloc(stream->write_buffer, new_capacity);
        if (!new_buffer) {
            return FALSE;
        }
        stream->write_buffer = new_buffer;
        stream->write_capacity = new_capacity;
    }
    
    // Copy data to write buffer
    memcpy(stream->write_buffer + stream->write_size, Buffer, size);
    stream->write_size += size;
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryHandler(const uint8_t* data, size_t size) {
    memory_stream* stream = malloc(sizeof(memory_stream));
    if (!stream) return NULL;
    
    stream->data = data;
    stream->size = size;
    stream->pos = 0;
    stream->write_buffer = NULL;
    stream->write_size = 0;
    stream->write_capacity = 0;
    
    cmsIOHANDLER* handler = malloc(sizeof(cmsIOHANDLER));
    if (!handler) {
        free(stream);
        return NULL;
    }
    
    handler->stream = stream;
    handler->ContextID = NULL;
    handler->UsedSpace = 0;
    handler->ReportedSize = 0;
    memset(handler->PhysicalFile, 0, sizeof(handler->PhysicalFile));
    
    handler->Read = ReadMemory;
    handler->Seek = SeekMemory;
    handler->Close = CloseMemory;
    handler->Tell = TellMemory;
    handler->Write = WriteMemory;
    
    return handler;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create memory-based I/O handler
    cmsIOHANDLER* io = CreateMemoryHandler(Data, Size);
    if (!io) return 0;
    
    // Test _cmsReadUInt8Number
    cmsUInt8Number read_value = 0;
    if (Size > 0) {
        _cmsReadUInt8Number(io, &read_value);
    }
    
    // Test _cmsWriteUInt8Number
    if (Size > 1) {
        _cmsWriteUInt8Number(io, Data[0]);
    }
    
    // Test _cmsReadAlignment
    _cmsReadAlignment(io);
    
    // Test _cmsWriteAlignment
    _cmsWriteAlignment(io);
    
    // Test _cmsIOPrintf with various format strings
    if (Size > 10) {
        // Test with integer
        _cmsIOPrintf(io, "Test %d", (int)Data[1]);
        
        // Test with float (locale safety)
        _cmsIOPrintf(io, "Float: %f", (double)Data[2] / 255.0);
        
        // Test with string
        char small_str[10];
        size_t str_len = Size > 20 ? 10 : Size - 10;
        if (str_len > 0 && str_len < sizeof(small_str)) {
            memcpy(small_str, Data + 10, str_len);
            small_str[str_len] = '\0';
            _cmsIOPrintf(io, "String: %s", small_str);
        }
    }
    
    // Test reading again after operations
    if (Size > 5) {
        // Seek to a position based on input data
        cmsUInt32Number seek_pos = Data[3] % (Size > 100 ? 100 : Size);
        io->Seek(io, seek_pos);
        
        cmsUInt8Number another_value = 0;
        _cmsReadUInt8Number(io, &another_value);
    }
    
    // Test closing the handler
    cmsCloseIOhandler(io);
    
    return 0;
}