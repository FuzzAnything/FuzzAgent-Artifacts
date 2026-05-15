// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Custom memory I/O handler implementation
typedef struct {
    uint8_t* buffer;
    size_t size;
    size_t pos;
    cmsBool for_writing;
} MemoryStream;

static cmsUInt32Number MemoryRead(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    cmsUInt32Number total_bytes = size * count;
    
    if (stream->pos + total_bytes > stream->size) {
        total_bytes = stream->size - stream->pos;
    }
    
    if (total_bytes > 0) {
        memcpy(Buffer, stream->buffer + stream->pos, total_bytes);
        stream->pos += total_bytes;
    }
    
    return total_bytes / size;
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

static cmsBool MemoryWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (!stream->for_writing) {
        return FALSE;
    }
    
    // Simple implementation - just advance position without actual write
    stream->pos += size;
    return TRUE;
}

static cmsBool MemoryClose(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (stream) {
        free(stream->buffer);
        free(stream);
    }
    
    free(iohandler);
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryHandler(const uint8_t* data, size_t size, cmsBool for_writing) {
    cmsIOHANDLER* handler = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!handler) return NULL;
    
    MemoryStream* stream = (MemoryStream*)calloc(1, sizeof(MemoryStream));
    if (!stream) {
        free(handler);
        return NULL;
    }
    
    if (for_writing) {
        // For writing, allocate a buffer
        stream->buffer = (uint8_t*)malloc(size);
        if (stream->buffer && data && size > 0) {
            memcpy(stream->buffer, data, size);
        }
        stream->size = size;
    } else {
        // For reading, just point to the data (won't be modified)
        stream->buffer = (uint8_t*)data;
        stream->size = size;
    }
    
    stream->pos = 0;
    stream->for_writing = for_writing;
    
    handler->stream = stream;
    handler->Read = MemoryRead;
    handler->Seek = MemorySeek;
    handler->Tell = MemoryTell;
    handler->Write = MemoryWrite;
    handler->Close = MemoryClose;
    
    return handler;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Test _cmsWriteAlignment
    cmsIOHANDLER* write_handler = CreateMemoryHandler(Data, Size, TRUE);
    if (write_handler) {
        _cmsWriteAlignment(write_handler);
        write_handler->Close(write_handler);
    }
    
    // Test _cmsWriteFloat32Number
    write_handler = CreateMemoryHandler(Data, Size, TRUE);
    if (write_handler) {
        cmsFloat32Number float_val = 0.0f;
        if (Size >= sizeof(float_val)) {
            memcpy(&float_val, Data, sizeof(float_val));
        }
        _cmsWriteFloat32Number(write_handler, float_val);
        write_handler->Close(write_handler);
    }
    
    // Test _cmsReadAlignment
    cmsIOHANDLER* read_handler = CreateMemoryHandler(Data, Size, FALSE);
    if (read_handler) {
        _cmsReadAlignment(read_handler);
        read_handler->Close(read_handler);
    }
    
    // Test _cmsWriteUInt8Number
    write_handler = CreateMemoryHandler(Data, Size, TRUE);
    if (write_handler) {
        cmsUInt8Number byte_val = (Size > 0) ? Data[0] : 0;
        _cmsWriteUInt8Number(write_handler, byte_val);
        write_handler->Close(write_handler);
    }
    
    // Test _cmsReadFloat32Number
    read_handler = CreateMemoryHandler(Data, Size, FALSE);
    if (read_handler) {
        cmsFloat32Number read_float = 0.0f;
        _cmsReadFloat32Number(read_handler, &read_float);
        read_handler->Close(read_handler);
    }
    
    // Test _cmsReadUInt8Number
    read_handler = CreateMemoryHandler(Data, Size, FALSE);
    if (read_handler) {
        cmsUInt8Number read_byte = 0;
        _cmsReadUInt8Number(read_handler, &read_byte);
        read_handler->Close(read_handler);
    }
    
    // Test combinations
    write_handler = CreateMemoryHandler(Data, Size, TRUE);
    if (write_handler) {
        // Write a byte
        cmsUInt8Number byte_val = (Size > 0) ? Data[0] : 42;
        _cmsWriteUInt8Number(write_handler, byte_val);
        
        // Write alignment
        _cmsWriteAlignment(write_handler);
        
        // Write a float
        cmsFloat32Number float_val = 1.0f;
        if (Size >= sizeof(float_val)) {
            memcpy(&float_val, Data, sizeof(float_val));
        }
        _cmsWriteFloat32Number(write_handler, float_val);
        
        write_handler->Close(write_handler);
    }
    
    read_handler = CreateMemoryHandler(Data, Size, FALSE);
    if (read_handler) {
        // Read a byte
        cmsUInt8Number read_byte = 0;
        _cmsReadUInt8Number(read_handler, &read_byte);
        
        // Read alignment
        _cmsReadAlignment(read_handler);
        
        // Read a float
        cmsFloat32Number read_float = 0.0f;
        _cmsReadFloat32Number(read_handler, &read_float);
        
        read_handler->Close(read_handler);
    }
    
    return 0;
}