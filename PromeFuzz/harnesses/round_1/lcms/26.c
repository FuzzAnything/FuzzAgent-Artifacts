// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "lcms2_plugin.h"

// Structure for memory stream
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
} memory_stream;

// Read function for memory stream
static cmsUInt32Number memory_read(cmsIOHANDLER* iohandler, void* Buffer,
                                   cmsUInt32Number size, cmsUInt32Number count) {
    memory_stream* stream = (memory_stream*)iohandler->stream;
    cmsUInt32Number total_bytes = size * count;
    cmsUInt32Number bytes_to_read = total_bytes;
    
    if (stream->pos + bytes_to_read > stream->size) {
        if (stream->pos >= stream->size) return 0;
        bytes_to_read = stream->size - stream->pos;
    }
    
    if (bytes_to_read > 0) {
        memcpy(Buffer, stream->data + stream->pos, bytes_to_read);
        stream->pos += bytes_to_read;
    }
    
    return bytes_to_read / size;
}

// Write function for memory stream
static cmsBool memory_write(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                            const void* Buffer) {
    // For fuzzing, we don't actually need to write anywhere
    // Just simulate successful write
    return TRUE;
}

// Seek function for memory stream
static cmsBool memory_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    memory_stream* stream = (memory_stream*)iohandler->stream;
    if (offset > stream->size) return FALSE;
    stream->pos = offset;
    return TRUE;
}

// Tell function for memory stream
static cmsUInt32Number memory_tell(cmsIOHANDLER* iohandler) {
    memory_stream* stream = (memory_stream*)iohandler->stream;
    return stream->pos;
}

// Close function for memory stream
static cmsBool memory_close(cmsIOHANDLER* iohandler) {
    // Nothing to clean up for memory stream
    return TRUE;
}

// Create a memory-based I/O handler
static cmsIOHANDLER* create_memory_iohandler(const uint8_t* data, size_t size) {
    memory_stream* stream = (memory_stream*)malloc(sizeof(memory_stream));
    if (!stream) return NULL;
    
    stream->data = data;
    stream->size = size;
    stream->pos = 0;
    
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) {
        free(stream);
        return NULL;
    }
    
    io->stream = stream;
    io->ContextID = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    io->PhysicalFile[0] = '\0';
    
    io->Read = memory_read;
    io->Seek = memory_seek;
    io->Close = memory_close;
    io->Tell = memory_tell;
    io->Write = memory_write;
    
    return io;
}

// Clean up memory-based I/O handler
static void destroy_memory_iohandler(cmsIOHANDLER* io) {
    if (io) {
        if (io->stream) {
            free(io->stream);
        }
        free(io);
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Test _cmsAdjustEndianess64
    if (Size >= sizeof(cmsUInt64Number)) {
        cmsUInt64Number input, output;
        memcpy(&input, Data, sizeof(cmsUInt64Number));
        _cmsAdjustEndianess64(&output, &input);
    }
    
    // Create memory-based I/O handler for testing
    cmsIOHANDLER* io = create_memory_iohandler(Data, Size);
    if (!io) return 0;
    
    // Test _cmsWriteUInt64Number
    if (Size >= sizeof(cmsUInt64Number)) {
        cmsUInt64Number num;
        memcpy(&num, Data, sizeof(cmsUInt64Number));
        _cmsWriteUInt64Number(io, &num);
    }
    
    // Reset stream position for reading tests
    memory_stream* stream = (memory_stream*)io->stream;
    stream->pos = 0;
    
    // Test _cmsReadUInt64Number
    if (Size >= sizeof(cmsUInt64Number)) {
        cmsUInt64Number num;
        _cmsReadUInt64Number(io, &num);
    }
    
    // Reset stream position
    stream->pos = 0;
    
    // Test _cmsReadXYZNumber
    if (Size >= sizeof(cmsUInt64Number) * 3) {  // XYZ typically uses 3 fixed-point numbers
        cmsCIEXYZ xyz;
        _cmsReadXYZNumber(io, &xyz);
    }
    
    // Reset stream position
    stream->pos = 0;
    
    // Test _cmsWriteXYZNumber
    if (Size >= sizeof(cmsUInt64Number) * 3) {
        cmsCIEXYZ xyz;
        // Initialize with some data from fuzzer input
        memcpy(&xyz.X, Data, sizeof(double));
        if (Size >= sizeof(double) * 2) {
            memcpy(&xyz.Y, Data + sizeof(double), sizeof(double));
        }
        if (Size >= sizeof(double) * 3) {
            memcpy(&xyz.Z, Data + sizeof(double) * 2, sizeof(double));
        }
        _cmsWriteXYZNumber(io, &xyz);
    }
    
    // Test _cmsWrite15Fixed16Number
    if (Size >= sizeof(cmsFloat64Number)) {
        cmsFloat64Number num;
        memcpy(&num, Data, sizeof(cmsFloat64Number));
        _cmsWrite15Fixed16Number(io, num);
    }
    
    // Clean up
    destroy_memory_iohandler(io);
    
    return 0;
}