// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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

// Custom I/O handler implementation for memory streams
typedef struct {
    uint8_t* data;
    size_t size;
    size_t pos;
} mem_stream;

static cmsUInt32Number mem_read(cmsIOHANDLER* iohandler, void* buffer, cmsUInt32Number size, cmsUInt32Number count) {
    mem_stream* stream = (mem_stream*)iohandler->stream;
    size_t total = size * count;
    if (stream->pos + total > stream->size) {
        total = stream->size - stream->pos;
    }
    if (total > 0) {
        memcpy(buffer, stream->data + stream->pos, total);
        stream->pos += total;
    }
    return (cmsUInt32Number)(total / size);
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
    free(stream->data);
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
    if (stream->pos + size > stream->size) {
        // Resize buffer
        size_t new_size = stream->pos + size;
        uint8_t* new_data = realloc(stream->data, new_size);
        if (!new_data) return FALSE;
        stream->data = new_data;
        stream->size = new_size;
    }
    memcpy(stream->data + stream->pos, buffer, size);
    stream->pos += size;
    if (stream->pos > iohandler->UsedSpace) {
        iohandler->UsedSpace = stream->pos;
    }
    return TRUE;
}

static cmsIOHANDLER* create_mem_handler(const uint8_t* data, size_t size) {
    mem_stream* stream = malloc(sizeof(mem_stream));
    if (!stream) return NULL;
    
    stream->data = malloc(size);
    if (!stream->data) {
        free(stream);
        return NULL;
    }
    
    memcpy(stream->data, data, size);
    stream->size = size;
    stream->pos = 0;
    
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
    handler->PhysicalFile[0] = '\0';
    handler->Read = mem_read;
    handler->Seek = mem_seek;
    handler->Close = mem_close;
    handler->Tell = mem_tell;
    handler->Write = mem_write;
    
    return handler;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create memory I/O handler
    cmsIOHANDLER* io = create_mem_handler(Data, Size);
    if (!io) return 0;
    
    // Test _cmsWriteUInt8Number
    cmsUInt8Number write_val = Data[0];
    _cmsWriteUInt8Number(io, write_val);
    
    // Test _cmsWriteAlignment
    _cmsWriteAlignment(io);
    
    // Reset position for reading
    mem_seek(io, 0);
    
    // Test _cmsReadUInt8Number
    cmsUInt8Number read_val;
    _cmsReadUInt8Number(io, &read_val);
    
    // Test _cmsReadAlignment
    _cmsReadAlignment(io);
    
    // Test _cmsReadFloat32Number
    cmsFloat32Number float_val;
    if (Size >= 6) {  // Need at least 4 bytes for float + some padding
        _cmsReadFloat32Number(io, &float_val);
    }
    
    // Test cmsCloseIOhandler
    cmsCloseIOhandler(io);
    
    // Also test with file I/O handler
    FILE* fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        cmsContext ctx = cmsCreateContext(NULL, NULL);
        if (ctx) {
            cmsIOHANDLER* file_io = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
            if (file_io) {
                // Test reading functions with file handler
                cmsUInt8Number file_val;
                _cmsReadUInt8Number(file_io, &file_val);
                
                _cmsReadAlignment(file_io);
                
                cmsFloat32Number file_float;
                if (Size >= 6) {
                    _cmsReadFloat32Number(file_io, &file_float);
                }
                
                cmsCloseIOhandler(file_io);
            }
            cmsDeleteContext(ctx);
        }
        remove("./dummy_file");
    }
    
    return 0;
}