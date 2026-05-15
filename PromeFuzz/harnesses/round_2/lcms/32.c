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
#include <stdlib.h>
#include <string.h>

// Define memory stream structure for I/O handler
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
} memory_stream;

// I/O handler function implementations
static cmsUInt32Number memory_read(cmsIOHANDLER* iohandler, void* buffer,
                                   cmsUInt32Number size, cmsUInt32Number count) {
    memory_stream* ms = (memory_stream*)iohandler->stream;
    cmsUInt32Number total_bytes = size * count;
    cmsUInt32Number bytes_to_read = total_bytes;
    
    if (ms->pos + bytes_to_read > ms->size) {
        if (ms->pos >= ms->size) return 0;
        bytes_to_read = ms->size - ms->pos;
    }
    
    if (bytes_to_read > 0) {
        memcpy(buffer, ms->data + ms->pos, bytes_to_read);
        ms->pos += bytes_to_read;
    }
    
    return bytes_to_read / size;
}

static cmsBool memory_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    memory_stream* ms = (memory_stream*)iohandler->stream;
    if (offset > ms->size) return FALSE;
    ms->pos = offset;
    return TRUE;
}

static cmsBool memory_close(cmsIOHANDLER* iohandler) {
    // Nothing to clean up for memory stream
    return TRUE;
}

static cmsUInt32Number memory_tell(cmsIOHANDLER* iohandler) {
    memory_stream* ms = (memory_stream*)iohandler->stream;
    return ms->pos;
}

static cmsBool memory_write(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                           const void* buffer) {
    memory_stream* ms = (memory_stream*)iohandler->stream;
    
    // For fuzzing, we just simulate writing by advancing position
    if (ms->pos + size > ms->size) {
        return FALSE;
    }
    
    // In real scenario we would write, but for fuzzing we just advance
    ms->pos += size;
    return TRUE;
}

static cmsIOHANDLER* create_memory_iohandler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    memory_stream* ms = (memory_stream*)calloc(1, sizeof(memory_stream));
    if (!ms) {
        free(io);
        return NULL;
    }
    
    ms->data = data;
    ms->size = size;
    ms->pos = 0;
    
    io->stream = ms;
    io->Read = memory_read;
    io->Seek = memory_seek;
    io->Close = memory_close;
    io->Tell = memory_tell;
    io->Write = memory_write;
    
    return io;
}

static void destroy_memory_iohandler(cmsIOHANDLER* io) {
    if (io) {
        if (io->stream) {
            free(io->stream);
        }
        free(io);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Test _cms15Fixed16toDouble
    cmsS15Fixed16Number fix32;
    memcpy(&fix32, Data, sizeof(fix32));
    cmsFloat64Number result1 = _cms15Fixed16toDouble(fix32);
    (void)result1; // Suppress unused variable warning
    
    // Test _cms8Fixed8toDouble
    if (Size >= 6) {
        cmsUInt16Number fixed8;
        memcpy(&fixed8, Data + 4, sizeof(fixed8));
        cmsFloat64Number result2 = _cms8Fixed8toDouble(fixed8);
        (void)result2;
    }
    
    // Test _cmsDoubleTo8Fixed8 and _cmsDoubleTo15Fixed16
    if (Size >= 14) {
        cmsFloat64Number val1, val2;
        memcpy(&val1, Data + 6, sizeof(val1));
        memcpy(&val2, Data + 14, sizeof(val2));
        
        cmsUInt16Number result3 = _cmsDoubleTo8Fixed8(val1);
        (void)result3;
        
        cmsS15Fixed16Number result4 = _cmsDoubleTo15Fixed16(val2);
        (void)result4;
    }
    
    // Test _cmsRead15Fixed16Number
    if (Size >= 18) {
        cmsIOHANDLER* io = create_memory_iohandler(Data, Size);
        if (io) {
            cmsFloat64Number n;
            cmsBool read_result = _cmsRead15Fixed16Number(io, &n);
            (void)read_result;
            destroy_memory_iohandler(io);
        }
    }
    
    // Test _cmsWrite15Fixed16Number
    if (Size >= 26) {
        cmsIOHANDLER* io = create_memory_iohandler(Data, Size);
        if (io) {
            cmsFloat64Number n;
            memcpy(&n, Data + 22, sizeof(n));
            cmsBool write_result = _cmsWrite15Fixed16Number(io, n);
            (void)write_result;
            destroy_memory_iohandler(io);
        }
    }
    
    return 0;
}