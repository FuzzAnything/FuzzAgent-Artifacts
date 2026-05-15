// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "lcms2_plugin.h"

// Memory stream implementation for cmsIOHANDLER
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
    cmsBool write_mode;
    uint8_t* write_buffer;
    size_t write_buffer_size;
    size_t write_buffer_capacity;
} MemoryStream;

static cmsUInt32Number MemoryStreamRead(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    cmsUInt32Number total_bytes = size * count;
    
    if (stream->pos + total_bytes > stream->size) {
        total_bytes = stream->size - stream->pos;
    }
    
    if (total_bytes > 0) {
        memcpy(Buffer, stream->data + stream->pos, total_bytes);
        stream->pos += total_bytes;
    }
    
    return total_bytes / size; // Return number of items read
}

static cmsBool MemoryStreamSeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (offset > stream->size) {
        return FALSE;
    }
    
    stream->pos = offset;
    return TRUE;
}

static cmsBool MemoryStreamClose(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (stream->write_buffer) {
        free(stream->write_buffer);
    }
    free(stream);
    free(iohandler);
    
    return TRUE;
}

static cmsUInt32Number MemoryStreamTell(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool MemoryStreamWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (!stream->write_mode) {
        return FALSE;
    }
    
    // Resize write buffer if needed
    if (stream->pos + size > stream->write_buffer_capacity) {
        size_t new_capacity = stream->write_buffer_capacity * 2;
        if (new_capacity < stream->pos + size) {
            new_capacity = stream->pos + size;
        }
        
        uint8_t* new_buffer = (uint8_t*)realloc(stream->write_buffer, new_capacity);
        if (!new_buffer) {
            return FALSE;
        }
        
        stream->write_buffer = new_buffer;
        stream->write_buffer_capacity = new_capacity;
    }
    
    memcpy(stream->write_buffer + stream->pos, Buffer, size);
    stream->pos += size;
    
    if (stream->pos > stream->write_buffer_size) {
        stream->write_buffer_size = stream->pos;
    }
    
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryHandler(const uint8_t* data, size_t size, cmsContext context, cmsBool write_mode) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) {
        return NULL;
    }
    
    MemoryStream* stream = (MemoryStream*)calloc(1, sizeof(MemoryStream));
    if (!stream) {
        free(io);
        return NULL;
    }
    
    stream->data = data;
    stream->size = size;
    stream->pos = 0;
    stream->write_mode = write_mode;
    stream->write_buffer = NULL;
    stream->write_buffer_size = 0;
    stream->write_buffer_capacity = 0;
    
    if (write_mode) {
        stream->write_buffer_capacity = size > 1024 ? size * 2 : 2048;
        stream->write_buffer = (uint8_t*)malloc(stream->write_buffer_capacity);
        if (!stream->write_buffer) {
            free(stream);
            free(io);
            return NULL;
        }
    }
    
    io->stream = stream;
    io->ContextID = context;
    io->UsedSpace = 0;
    io->ReportedSize = (cmsUInt32Number)size;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);
    
    io->Read = MemoryStreamRead;
    io->Seek = MemoryStreamSeek;
    io->Close = MemoryStreamClose;
    io->Tell = MemoryStreamTell;
    io->Write = MemoryStreamWrite;
    
    return io;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }
    
    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    // Test _cmsWriteAlignment
    cmsIOHANDLER* write_io = CreateMemoryHandler(Data, Size, context, TRUE);
    if (write_io) {
        _cmsWriteAlignment(write_io);
        write_io->Close(write_io);
    }
    
    // Test _cmsReadUInt64Number
    cmsIOHANDLER* read_io = CreateMemoryHandler(Data, Size, context, FALSE);
    if (read_io) {
        cmsUInt64Number n;
        _cmsReadUInt64Number(read_io, &n);
        read_io->Close(read_io);
    }
    
    // Test _cmsReadAlignment
    read_io = CreateMemoryHandler(Data, Size, context, FALSE);
    if (read_io) {
        _cmsReadAlignment(read_io);
        read_io->Close(read_io);
    }
    
    // Test _cmsWriteXYZNumber
    write_io = CreateMemoryHandler(Data, Size, context, TRUE);
    if (write_io) {
        cmsCIEXYZ xyz;
        // Initialize with some values from fuzzer data
        if (Size >= sizeof(cmsCIEXYZ)) {
            memcpy(&xyz, Data, sizeof(cmsCIEXYZ));
        } else {
            memset(&xyz, 0, sizeof(cmsCIEXYZ));
        }
        _cmsWriteXYZNumber(write_io, &xyz);
        write_io->Close(write_io);
    }
    
    // Test _cmsWriteUInt64Number
    write_io = CreateMemoryHandler(Data, Size, context, TRUE);
    if (write_io) {
        cmsUInt64Number n;
        // Initialize with some value from fuzzer data
        if (Size >= sizeof(cmsUInt64Number)) {
            memcpy(&n, Data, sizeof(cmsUInt64Number));
        } else {
            n = 0;
        }
        _cmsWriteUInt64Number(write_io, &n);
        write_io->Close(write_io);
    }
    
    // Test _cmsReadXYZNumber
    read_io = CreateMemoryHandler(Data, Size, context, FALSE);
    if (read_io) {
        cmsCIEXYZ xyz;
        _cmsReadXYZNumber(read_io, &xyz);
        read_io->Close(read_io);
    }
    
    // Clean up context
    cmsDeleteContext(context);
    
    return 0;
}