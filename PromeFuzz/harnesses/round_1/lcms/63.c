// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

// Memory stream implementation for fuzzing
typedef struct {
    const uint8_t *read_data;
    size_t read_size;
    size_t read_pos;
    uint8_t *write_buffer;
    size_t write_size;
    size_t write_pos;
} mem_stream;

static cmsUInt32Number mem_read(cmsIOHANDLER* iohandler, void* Buffer,
                               cmsUInt32Number size, cmsUInt32Number count) {
    mem_stream *stream = (mem_stream*)iohandler->stream;
    size_t total = size * count;
    if (stream->read_pos + total > stream->read_size) {
        total = stream->read_size - stream->read_pos;
        count = total / size;
    }
    if (total) {
        memcpy(Buffer, stream->read_data + stream->read_pos, total);
        stream->read_pos += total;
    }
    return count;
}

static cmsBool mem_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    mem_stream *stream = (mem_stream*)iohandler->stream;
    if (offset > stream->read_size) return FALSE;
    stream->read_pos = offset;
    return TRUE;
}

static cmsBool mem_close(cmsIOHANDLER* iohandler) {
    mem_stream *stream = (mem_stream*)iohandler->stream;
    if (stream->write_buffer) {
        free(stream->write_buffer);
    }
    free(stream);
    free(iohandler);
    return TRUE;
}

static cmsUInt32Number mem_tell(cmsIOHANDLER* iohandler) {
    mem_stream *stream = (mem_stream*)iohandler->stream;
    return (cmsUInt32Number)stream->read_pos;
}

static cmsBool mem_write(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                        const void* Buffer) {
    mem_stream *stream = (mem_stream*)iohandler->stream;
    if (stream->write_pos + size > stream->write_size) {
        // Resize write buffer if needed
        size_t new_size = stream->write_size * 2;
        if (new_size < stream->write_pos + size) {
            new_size = stream->write_pos + size;
        }
        uint8_t *new_buffer = realloc(stream->write_buffer, new_size);
        if (!new_buffer) return FALSE;
        stream->write_buffer = new_buffer;
        stream->write_size = new_size;
    }
    memcpy(stream->write_buffer + stream->write_pos, Buffer, size);
    stream->write_pos += size;
    return TRUE;
}

static cmsIOHANDLER* create_mem_handler(const uint8_t *data, size_t size) {
    mem_stream *stream = malloc(sizeof(mem_stream));
    if (!stream) return NULL;
    
    stream->read_data = data;
    stream->read_size = size;
    stream->read_pos = 0;
    stream->write_buffer = malloc(1024);
    stream->write_size = 1024;
    stream->write_pos = 0;
    
    cmsIOHANDLER* handler = malloc(sizeof(cmsIOHANDLER));
    if (!handler) {
        free(stream->write_buffer);
        free(stream);
        return NULL;
    }
    
    handler->stream = stream;
    handler->ContextID = NULL;
    handler->UsedSpace = 0;
    handler->ReportedSize = 0;
    handler->PhysicalFile[0] = '\0';
    handler->Read = mem_read;
    handler->Seek = mem_seek;
    handler->Close = mem_close;
    handler->Tell = mem_tell;
    handler->Write = mem_write;
    
    return handler;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    // Extract values from fuzzer input
    cmsFloat64Number double_val;
    cmsS15Fixed16Number fixed32_val;
    cmsUInt16Number fixed16_val;
    
    // Use first 8 bytes for double
    memcpy(&double_val, Data, sizeof(double_val));
    
    // Use next 4 bytes for 32-bit fixed point
    if (Size >= 12) {
        memcpy(&fixed32_val, Data + 8, sizeof(fixed32_val));
    } else {
        fixed32_val = 0;
    }
    
    // Use next 2 bytes for 16-bit fixed point
    if (Size >= 14) {
        memcpy(&fixed16_val, Data + 12, sizeof(fixed16_val));
    } else {
        fixed16_val = 0;
    }
    
    // Test conversion functions
    cmsFloat64Number result_double;
    cmsUInt16Number result_fixed8;
    cmsS15Fixed16Number result_fixed16;
    
    // 1. _cms15Fixed16toDouble
    result_double = _cms15Fixed16toDouble(fixed32_val);
    
    // 2. _cms8Fixed8toDouble
    result_double = _cms8Fixed8toDouble(fixed16_val);
    
    // 3. _cmsDoubleTo8Fixed8
    result_fixed8 = _cmsDoubleTo8Fixed8(double_val);
    
    // 4. _cmsDoubleTo15Fixed16
    result_fixed16 = _cmsDoubleTo15Fixed16(double_val);
    
    // 5. Test I/O functions with memory stream
    cmsIOHANDLER* io = create_mem_handler(Data, Size);
    if (!io) return 0;
    
    // 6. _cmsRead15Fixed16Number
    cmsFloat64Number read_val;
    cmsBool read_success = _cmsRead15Fixed16Number(io, &read_val);
    
    // 7. _cmsWrite15Fixed16Number
    cmsBool write_success = _cmsWrite15Fixed16Number(io, double_val);
    
    // Cleanup - properly close the handler which will free all memory
    io->Close(io);
    
    return 0;
}