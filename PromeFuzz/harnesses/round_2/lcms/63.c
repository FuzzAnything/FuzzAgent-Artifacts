// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Dummy I/O handler implementation for memory streams
typedef struct {
    uint8_t* buffer;
    size_t size;
    size_t pos;
    cmsBool is_readable;
    cmsBool is_writable;
} mem_stream;

static cmsUInt32Number mem_read(cmsIOHANDLER* iohandler, void* buffer, 
                                 cmsUInt32Number size, cmsUInt32Number count) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    if (!stream->is_readable) return 0;
    
    size_t total_bytes = size * count;
    size_t available = stream->size - stream->pos;
    size_t to_read = total_bytes < available ? total_bytes : available;
    
    if (to_read > 0) {
        memcpy(buffer, stream->buffer + stream->pos, to_read);
        stream->pos += to_read;
    }
    return to_read / size;
}

static cmsBool mem_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    if (offset <= stream->size) {
        stream->pos = offset;
        return TRUE;
    }
    return FALSE;
}

static cmsBool mem_close(cmsIOHANDLER* iohandler) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    if (stream->buffer) {
        free(stream->buffer);
        stream->buffer = NULL;
    }
    free(stream);
    iohandler->stream = NULL;
    return TRUE;
}

static cmsUInt32Number mem_tell(cmsIOHANDLER* iohandler) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool mem_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* buffer) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    if (!stream->is_writable) return FALSE;
    
    // Resize buffer if needed
    if (stream->pos + size > stream->size) {
        size_t new_size = stream->pos + size;
        uint8_t* new_buffer = (uint8_t*)realloc(stream->buffer, new_size);
        if (!new_buffer) return FALSE;
        stream->buffer = new_buffer;
        stream->size = new_size;
    }
    
    memcpy(stream->buffer + stream->pos, buffer, size);
    stream->pos += size;
    return TRUE;
}

static cmsIOHANDLER* create_mem_handler(cmsContext ctx, cmsBool for_reading, 
                                        cmsBool for_writing, const uint8_t* data, size_t size) {
    mem_stream* stream = (mem_stream*)calloc(1, sizeof(mem_stream));
    if (!stream) return NULL;
    
    stream->is_readable = for_reading;
    stream->is_writable = for_writing;
    
    if (for_reading && data && size > 0) {
        stream->buffer = (uint8_t*)malloc(size);
        if (!stream->buffer) {
            free(stream);
            return NULL;
        }
        memcpy(stream->buffer, data, size);
        stream->size = size;
    } else {
        stream->buffer = NULL;
        stream->size = 0;
    }
    stream->pos = 0;
    
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) {
        if (stream->buffer) free(stream->buffer);
        free(stream);
        return NULL;
    }
    
    io->stream = stream;
    io->ContextID = ctx;
    io->Read = mem_read;
    io->Seek = mem_seek;
    io->Close = mem_close;
    io->Tell = mem_tell;
    io->Write = mem_write;
    
    return io;
}

static void destroy_mem_handler(cmsIOHANDLER* io) {
    if (io) {
        if (io->Close) io->Close(io);
        free(io);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    // Create a dummy context
    cmsContext ctx = NULL;
    
    // Test with read handler
    if (Size > 0) {
        cmsIOHANDLER* read_io = create_mem_handler(ctx, TRUE, FALSE, Data, Size);
        if (read_io) {
            cmsUInt32Number n32;
            if (_cmsReadUInt32Number(read_io, &n32)) {
                // Use the read value for array size (limit to reasonable size)
                cmsUInt32Number array_size = n32 % 256;
                if (array_size > 0) {
                    cmsUInt16Number* array = (cmsUInt16Number*)calloc(array_size, sizeof(cmsUInt16Number));
                    if (array) {
                        _cmsReadUInt16Array(read_io, array_size, array);
                        free(array);
                    }
                }
            }
            
            // Test alignment
            _cmsReadAlignment(read_io);
            
            destroy_mem_handler(read_io);
        }
    }
    
    // Test with write handler
    cmsIOHANDLER* write_io = create_mem_handler(ctx, FALSE, TRUE, NULL, 0);
    if (write_io) {
        // Write a 32-bit number derived from input size
        _cmsWriteUInt32Number(write_io, (cmsUInt32Number)Size);
        
        // Write an array of 16-bit numbers
        if (Size > 0) {
            cmsUInt32Number array_size = Size % 128;
            if (array_size > 0) {
                cmsUInt16Number* array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
                if (array) {
                    // Fill array with data from input
                    for (cmsUInt32Number i = 0; i < array_size && i * 2 < Size; i++) {
                        array[i] = (Data[i * 2] << 8) | (i * 2 + 1 < Size ? Data[i * 2 + 1] : 0);
                    }
                    _cmsWriteUInt16Array(write_io, array_size, array);
                    free(array);
                }
            }
        }
        
        // Test alignment
        _cmsWriteAlignment(write_io);
        
        destroy_mem_handler(write_io);
    }
    
    // Test with read/write handler using dummy file data
    if (Size > 0) {
        cmsIOHANDLER* rw_io = create_mem_handler(ctx, TRUE, TRUE, Data, Size);
        if (rw_io) {
            cmsUInt32Number val;
            if (_cmsReadUInt32Number(rw_io, &val)) {
                _cmsWriteUInt32Number(rw_io, val);
            }
            
            _cmsReadAlignment(rw_io);
            _cmsWriteAlignment(rw_io);
            
            destroy_mem_handler(rw_io);
        }
    }
    
    return 0;
}