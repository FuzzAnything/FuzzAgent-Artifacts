// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
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
#include <stdio.h>

#define MAX_BUFFER_SIZE 4096

typedef struct {
    uint8_t* data;
    size_t size;
    size_t pos;
    size_t reported_size;
    int is_closed;
} memory_stream;

static cmsUInt32Number memory_read(cmsIOHANDLER* iohandler, void* buffer, cmsUInt32Number size, cmsUInt32Number count) {
    memory_stream* stream = (memory_stream*)iohandler->stream;
    if (!stream || stream->is_closed) return 0;
    
    size_t total_bytes = size * count;
    size_t available = stream->size - stream->pos;
    size_t to_read = total_bytes < available ? total_bytes : available;
    
    if (to_read > 0) {
        memcpy(buffer, stream->data + stream->pos, to_read);
        stream->pos += to_read;
    }
    
    return (cmsUInt32Number)(to_read / size);
}

static cmsBool memory_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    memory_stream* stream = (memory_stream*)iohandler->stream;
    if (!stream || stream->is_closed) return FALSE;
    
    if (offset <= stream->size) {
        stream->pos = offset;
        return TRUE;
    }
    return FALSE;
}

static cmsBool memory_close(cmsIOHANDLER* iohandler) {
    memory_stream* stream = (memory_stream*)iohandler->stream;
    if (!stream) return FALSE;
    
    stream->is_closed = 1;
    return TRUE;
}

static cmsUInt32Number memory_tell(cmsIOHANDLER* iohandler) {
    memory_stream* stream = (memory_stream*)iohandler->stream;
    if (!stream || stream->is_closed) return 0;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool memory_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* buffer) {
    memory_stream* stream = (memory_stream*)iohandler->stream;
    if (!stream || stream->is_closed) return FALSE;
    
    if (stream->pos + size > stream->size) {
        size_t new_size = stream->pos + size;
        uint8_t* new_data = realloc(stream->data, new_size);
        if (!new_data) return FALSE;
        
        stream->data = new_data;
        stream->size = new_size;
        stream->reported_size = new_size;
    }
    
    memcpy(stream->data + stream->pos, buffer, size);
    stream->pos += size;
    if (stream->pos > stream->reported_size) {
        stream->reported_size = stream->pos;
    }
    
    return TRUE;
}

static cmsIOHANDLER* create_memory_handler(const uint8_t* data, size_t size) {
    memory_stream* stream = malloc(sizeof(memory_stream));
    if (!stream) return NULL;
    
    stream->data = malloc(size);
    if (!stream->data) {
        free(stream);
        return NULL;
    }
    
    memcpy(stream->data, data, size);
    stream->size = size;
    stream->pos = 0;
    stream->reported_size = size;
    stream->is_closed = 0;
    
    cmsIOHANDLER* handler = malloc(sizeof(cmsIOHANDLER));
    if (!handler) {
        free(stream->data);
        free(stream);
        return NULL;
    }
    
    handler->stream = stream;
    handler->ContextID = NULL;
    handler->UsedSpace = 0;
    handler->ReportedSize = (cmsUInt32Number)size;
    strncpy(handler->PhysicalFile, "./dummy_file", cmsMAX_PATH - 1);
    handler->PhysicalFile[cmsMAX_PATH - 1] = '\0';
    
    handler->Read = memory_read;
    handler->Seek = memory_seek;
    handler->Close = memory_close;
    handler->Tell = memory_tell;
    handler->Write = memory_write;
    
    return handler;
}

static void destroy_memory_handler(cmsIOHANDLER* handler) {
    if (!handler) return;
    
    memory_stream* stream = (memory_stream*)handler->stream;
    if (stream) {
        free(stream->data);
        free(stream);
    }
    free(handler);
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create memory-based I/O handler
    cmsIOHANDLER* io = create_memory_handler(Data, Size);
    if (!io) return 0;
    
    // Use first byte to determine which function to test
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0: {
            // Test _cmsWriteAlignment
            _cmsWriteAlignment(io);
            break;
        }
        case 1: {
            // Test _cmsReadAlignment
            _cmsReadAlignment(io);
            break;
        }
        case 2: {
            // Test _cmsIOPrintf with various format strings
            const char* formats[] = {
                "Test %d",
                "Value: %f",
                "String: %s",
                "Hex: %x",
                "Char: %c"
            };
            int fmt_idx = (Size > 1 ? Data[1] : 0) % 5;
            int value = (Size > 2 ? Data[2] : 0);
            _cmsIOPrintf(io, formats[fmt_idx], value);
            break;
        }
        case 3: {
            // Test _cmsWriteUInt8Number
            cmsUInt8Number value = (Size > 1 ? Data[1] : 0);
            _cmsWriteUInt8Number(io, value);
            break;
        }
        case 4: {
            // Test _cmsReadUInt8Number
            cmsUInt8Number n;
            _cmsReadUInt8Number(io, &n);
            break;
        }
        case 5: {
            // Test cmsCloseIOhandler
            cmsCloseIOhandler(io);
            // Clean up the handler after closing
            destroy_memory_handler(io);
            return 0;
        }
    }
    
    // Clean up for other cases
    cmsCloseIOhandler(io);
    destroy_memory_handler(io);
    
    return 0;
}