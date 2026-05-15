// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2_plugin.h"

// Define memory stream structure
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
} mem_stream;

// Memory stream read function
static cmsUInt32Number mem_read(cmsIOHANDLER* iohandler, void* buffer, 
                                cmsUInt32Number size, cmsUInt32Number count) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    cmsUInt32Number total_bytes = size * count;
    
    if (stream->pos + total_bytes > stream->size) {
        total_bytes = (cmsUInt32Number)(stream->size - stream->pos);
        if (total_bytes < size) return 0;
        count = total_bytes / size;
        total_bytes = count * size;
    }
    
    if (total_bytes > 0) {
        memcpy(buffer, stream->data + stream->pos, total_bytes);
        stream->pos += total_bytes;
    }
    
    return count;
}

// Memory stream seek function
static cmsBool mem_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    if (offset <= stream->size) {
        stream->pos = offset;
        return TRUE;
    }
    return FALSE;
}

// Memory stream close function
static cmsBool mem_close(cmsIOHANDLER* iohandler) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    free(stream);
    free(iohandler);
    return TRUE;
}

// Memory stream tell function
static cmsUInt32Number mem_tell(cmsIOHANDLER* iohandler) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

// Memory stream write function
static cmsBool mem_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* buffer) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    
    // For fuzzing, we don't actually need to write anywhere
    // Just simulate successful write by advancing position
    if (stream->pos + size <= stream->size) {
        stream->pos += size;
        return TRUE;
    }
    return FALSE;
}

// Create a memory-based I/O handler
static cmsIOHANDLER* create_mem_handler(const uint8_t* data, size_t size) {
    mem_stream* stream = (mem_stream*)malloc(sizeof(mem_stream));
    if (!stream) return NULL;
    
    stream->data = data;
    stream->size = size;
    stream->pos = 0;
    
    cmsIOHANDLER* handler = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!handler) {
        free(stream);
        return NULL;
    }
    
    memset(handler, 0, sizeof(cmsIOHANDLER));
    handler->stream = stream;
    handler->Read = mem_read;
    handler->Seek = mem_seek;
    handler->Close = mem_close;
    handler->Tell = mem_tell;
    handler->Write = mem_write;
    
    return handler;
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    // Create memory-based I/O handler
    cmsIOHANDLER* io = create_mem_handler(Data, Size);
    if (!io) return 0;
    
    // Test _cmsReadFloat32Number
    cmsFloat32Number float_val;
    _cmsReadFloat32Number(io, &float_val);
    
    // Test _cmsReadUInt32Number
    cmsUInt32Number uint32_val;
    _cmsReadUInt32Number(io, &uint32_val);
    
    // Test _cmsReadUInt64Number
    cmsUInt64Number uint64_val;
    _cmsReadUInt64Number(io, &uint64_val);
    
    // Test _cmsWriteUInt32Number
    _cmsWriteUInt32Number(io, uint32_val);
    
    // Test _cmsWriteFloat32Number
    _cmsWriteFloat32Number(io, float_val);
    
    // Test _cmsAdjustEndianess32
    cmsUInt32Number adjusted = _cmsAdjustEndianess32(uint32_val);
    (void)adjusted; // Prevent unused variable warning
    
    // Clean up
    if (io) {
        io->Close(io);
    }
    
    return 0;
}