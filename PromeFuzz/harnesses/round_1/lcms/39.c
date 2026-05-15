// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Custom I/O handler implementation for memory-based operations
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
    size_t write_pos;
    uint8_t* write_buffer;
    size_t write_buffer_size;
} MemoryStream;

static cmsUInt32Number MemoryRead(struct _cms_io_handler* iohandler, void* Buffer,
                                  cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    cmsUInt32Number total_bytes = size * count;
    
    if (stream->pos + total_bytes > stream->size) {
        total_bytes = stream->size - stream->pos;
    }
    
    if (total_bytes > 0) {
        memcpy(Buffer, stream->data + stream->pos, total_bytes);
        stream->pos += total_bytes;
    }
    
    return total_bytes / size;
}

static cmsBool MemorySeek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (offset <= stream->size) {
        stream->pos = offset;
        return TRUE;
    }
    
    return FALSE;
}

static cmsBool MemoryClose(struct _cms_io_handler* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (stream->write_buffer) {
        free(stream->write_buffer);
    }
    free(stream);
    free(iohandler);
    
    return TRUE;
}

static cmsUInt32Number MemoryTell(struct _cms_io_handler* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool MemoryWrite(struct _cms_io_handler* iohandler, cmsUInt32Number size,
                          const void* Buffer) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    // Allocate or resize write buffer if needed
    if (stream->write_pos + size > stream->write_buffer_size) {
        size_t new_size = stream->write_buffer_size * 2;
        if (new_size < stream->write_pos + size) {
            new_size = stream->write_pos + size;
        }
        
        uint8_t* new_buffer = realloc(stream->write_buffer, new_size);
        if (!new_buffer) {
            return FALSE;
        }
        
        stream->write_buffer = new_buffer;
        stream->write_buffer_size = new_size;
    }
    
    // Copy data to write buffer
    memcpy(stream->write_buffer + stream->write_pos, Buffer, size);
    stream->write_pos += size;
    
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryIOHandler(const uint8_t* data, size_t size) {
    MemoryStream* stream = malloc(sizeof(MemoryStream));
    if (!stream) return NULL;
    
    stream->data = data;
    stream->size = size;
    stream->pos = 0;
    stream->write_pos = 0;
    stream->write_buffer = NULL;
    stream->write_buffer_size = 0;
    
    cmsIOHANDLER* io = malloc(sizeof(cmsIOHANDLER));
    if (!io) {
        free(stream);
        return NULL;
    }
    
    io->stream = stream;
    io->Read = MemoryRead;
    io->Seek = MemorySeek;
    io->Close = MemoryClose;
    io->Tell = MemoryTell;
    io->Write = MemoryWrite;
    io->ContextID = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = (cmsUInt32Number)size;
    memset(io->PhysicalFile, 0, sizeof(io->PhysicalFile));
    
    return io;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsIOHANDLER* io = CreateMemoryIOHandler(Data, Size);
    if (!io) return 0;
    
    // Test _cmsReadUInt16Number
    cmsUInt16Number uint16_val = 0;
    _cmsReadUInt16Number(io, &uint16_val);
    
    // Reset position for next test
    io->Seek(io, 0);
    
    // Test _cmsReadUInt32Number
    cmsUInt32Number uint32_val = 0;
    _cmsReadUInt32Number(io, &uint32_val);
    
    // Reset position for next test
    io->Seek(io, 0);
    
    // Test _cmsRead15Fixed16Number
    cmsFloat64Number fixed_val = 0.0;
    _cmsRead15Fixed16Number(io, &fixed_val);
    
    // Reset position for next test
    io->Seek(io, 0);
    
    // Test _cmsReadUInt16Array
    cmsUInt32Number array_size = Size / 2;
    if (array_size > 100) array_size = 100; // Limit to avoid excessive allocation
    
    cmsUInt16Number* array = malloc(array_size * sizeof(cmsUInt16Number));
    if (array) {
        _cmsReadUInt16Array(io, array_size, array);
        free(array);
    }
    
    // Test _cmsWriteAlignment
    io->Seek(io, 0);
    _cmsWriteAlignment(io);
    
    // Test _cmsWrite15Fixed16Number
    // Use a value derived from fuzzer input
    cmsFloat64Number write_val = 0.0;
    if (Size >= 8) {
        uint32_t temp;
        memcpy(&temp, Data, sizeof(temp));
        write_val = (cmsFloat64Number)temp / 65536.0;
    }
    _cmsWrite15Fixed16Number(io, write_val);
    
    // Cleanup
    io->Close(io);
    
    return 0;
}