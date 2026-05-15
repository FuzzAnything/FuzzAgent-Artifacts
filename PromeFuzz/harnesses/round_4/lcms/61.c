// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
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
#include <stdio.h>

// Mock I/O handler structure
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
    size_t written;
    uint8_t* write_buffer;
    size_t write_buffer_size;
    cmsBool read_error;
    cmsBool write_error;
} MockStream;

// Mock I/O handler functions
static cmsUInt32Number mock_read(cmsIOHANDLER* iohandler, void* buffer, cmsUInt32Number size, cmsUInt32Number count) {
    MockStream* stream = (MockStream*)iohandler->stream;
    if (stream->read_error) return 0;
    
    size_t total_bytes = size * count;
    size_t available = stream->size - stream->pos;
    
    if (total_bytes > available) {
        total_bytes = available;
    }
    
    if (total_bytes > 0) {
        memcpy(buffer, stream->data + stream->pos, total_bytes);
        stream->pos += total_bytes;
    }
    
    return total_bytes / size;
}

static cmsBool mock_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MockStream* stream = (MockStream*)iohandler->stream;
    if (stream->read_error) return FALSE;
    
    if (offset <= stream->size) {
        stream->pos = offset;
        return TRUE;
    }
    return FALSE;
}

static cmsBool mock_close(cmsIOHANDLER* iohandler) {
    MockStream* stream = (MockStream*)iohandler->stream;
    if (stream->write_buffer) {
        free(stream->write_buffer);
    }
    free(stream);
    free(iohandler);
    return TRUE;
}

static cmsUInt32Number mock_tell(cmsIOHANDLER* iohandler) {
    MockStream* stream = (MockStream*)iohandler->stream;
    return stream->pos;
}

static cmsBool mock_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* buffer) {
    MockStream* stream = (MockStream*)iohandler->stream;
    if (stream->write_error) return FALSE;
    
    size_t new_size = stream->written + size;
    if (new_size > stream->write_buffer_size) {
        size_t new_buffer_size = new_size * 2;
        uint8_t* new_buffer = realloc(stream->write_buffer, new_buffer_size);
        if (!new_buffer) return FALSE;
        stream->write_buffer = new_buffer;
        stream->write_buffer_size = new_buffer_size;
    }
    
    memcpy(stream->write_buffer + stream->written, buffer, size);
    stream->written += size;
    return TRUE;
}

// Create mock I/O handler
static cmsIOHANDLER* create_mock_iohandler(const uint8_t* data, size_t size) {
    MockStream* stream = (MockStream*)calloc(1, sizeof(MockStream));
    if (!stream) return NULL;
    
    stream->data = data;
    stream->size = size;
    stream->pos = 0;
    stream->written = 0;
    stream->write_buffer_size = 1024;
    stream->write_buffer = malloc(stream->write_buffer_size);
    if (!stream->write_buffer) {
        free(stream);
        return NULL;
    }
    
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) {
        free(stream->write_buffer);
        free(stream);
        return NULL;
    }
    
    io->stream = stream;
    io->Read = mock_read;
    io->Seek = mock_seek;
    io->Close = mock_close;
    io->Tell = mock_tell;
    io->Write = mock_write;
    
    return io;
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create mock I/O handler
    cmsIOHANDLER* io = create_mock_iohandler(Data, Size);
    if (!io) return 0;
    
    // Test _cmsReadUInt16Number
    cmsUInt16Number read_num;
    _cmsReadUInt16Number(io, &read_num);
    
    // Test _cmsWriteUInt16Number
    cmsUInt16Number write_num = (cmsUInt16Number)(Data[0] << 8 | Data[1]);
    _cmsWriteUInt16Number(io, write_num);
    
    // Test _cmsReadAlignment
    _cmsReadAlignment(io);
    
    // Test _cmsWriteAlignment
    _cmsWriteAlignment(io);
    
    // Test array functions
    if (Size >= 8) {
        cmsUInt32Number array_size = (cmsUInt32Number)(Data[2] % 16); // Limit array size
        cmsUInt16Number* read_array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
        cmsUInt16Number* write_array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
        
        if (read_array && write_array) {
            // Initialize write array with fuzzer data
            for (cmsUInt32Number i = 0; i < array_size && (i * 2 + 4) < Size; i++) {
                write_array[i] = (cmsUInt16Number)(Data[i * 2 + 3] << 8 | Data[i * 2 + 4]);
            }
            
            // Test _cmsWriteUInt16Array
            _cmsWriteUInt16Array(io, array_size, write_array);
            
            // Reset position for reading
            MockStream* stream = (MockStream*)io->stream;
            stream->pos = 0;
            
            // Test _cmsReadUInt16Array
            _cmsReadUInt16Array(io, array_size, read_array);
        }
        
        if (read_array) free(read_array);
        if (write_array) free(write_array);
    }
    
    // Test error conditions
    MockStream* stream = (MockStream*)io->stream;
    stream->read_error = TRUE;
    _cmsReadUInt16Number(io, &read_num);
    _cmsReadAlignment(io);
    
    stream->read_error = FALSE;
    stream->write_error = TRUE;
    _cmsWriteUInt16Number(io, write_num);
    _cmsWriteAlignment(io);
    
    // Clean up
    io->Close(io);
    
    return 0;
}