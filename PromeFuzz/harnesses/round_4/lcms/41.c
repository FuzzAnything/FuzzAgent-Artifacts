// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
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

// Mock I/O handler implementation
typedef struct {
    uint8_t* data;
    size_t size;
    size_t pos;
    size_t capacity;
    cmsContext context;
} MockStream;

static cmsUInt32Number MockRead(cmsIOHANDLER* iohandler, void* Buffer,
                                cmsUInt32Number size, cmsUInt32Number count) {
    MockStream* stream = (MockStream*)iohandler->stream;
    if (!stream || !Buffer) return 0;
    
    cmsUInt32Number total = size * count;
    
    if (stream->pos + total > stream->size) {
        if (stream->pos >= stream->size) return 0;
        total = (cmsUInt32Number)(stream->size - stream->pos);
        // Adjust to read complete elements if possible
        if (size > 0) total = total - (total % size);
    }
    
    if (total > 0) {
        memcpy(Buffer, stream->data + stream->pos, total);
        stream->pos += total;
    }
    
    return total;
}

static cmsBool MockSeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MockStream* stream = (MockStream*)iohandler->stream;
    if (!stream) return FALSE;
    
    if (offset > stream->size) {
        return FALSE;
    }
    
    stream->pos = offset;
    return TRUE;
}

static cmsBool MockClose(cmsIOHANDLER* iohandler) {
    if (!iohandler) return FALSE;
    
    MockStream* stream = (MockStream*)iohandler->stream;
    if (stream) {
        if (stream->data) free(stream->data);
        free(stream);
    }
    free(iohandler);
    return TRUE;
}

static cmsUInt32Number MockTell(cmsIOHANDLER* iohandler) {
    MockStream* stream = (MockStream*)iohandler->stream;
    if (!stream) return 0;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool MockWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                         const void* Buffer) {
    MockStream* stream = (MockStream*)iohandler->stream;
    if (!stream || !Buffer) return FALSE;
    
    if (stream->pos + size > stream->capacity) {
        // Resize buffer with some extra capacity
        size_t new_capacity = stream->pos + size + 4096;
        uint8_t* new_data = realloc(stream->data, new_capacity);
        if (!new_data) return FALSE;
        
        stream->data = new_data;
        stream->capacity = new_capacity;
    }
    
    memcpy(stream->data + stream->pos, Buffer, size);
    stream->pos += size;
    
    // Update size if we wrote past the previous end
    if (stream->pos > stream->size) {
        stream->size = stream->pos;
    }
    
    return TRUE;
}

static cmsIOHANDLER* CreateMockHandler(const uint8_t* data, size_t size, cmsBool for_writing) {
    MockStream* stream = malloc(sizeof(MockStream));
    if (!stream) return NULL;
    
    memset(stream, 0, sizeof(MockStream));
    
    if (!for_writing && data && size > 0) {
        stream->data = malloc(size);
        if (!stream->data) {
            free(stream);
            return NULL;
        }
        memcpy(stream->data, data, size);
        stream->size = size;
        stream->capacity = size;
    } else {
        // For writing, start with a small buffer
        stream->capacity = 4096;
        stream->data = malloc(stream->capacity);
        if (!stream->data) {
            free(stream);
            return NULL;
        }
        stream->size = 0;
    }
    
    stream->pos = 0;
    stream->context = NULL;
    
    cmsIOHANDLER* handler = malloc(sizeof(cmsIOHANDLER));
    if (!handler) {
        free(stream->data);
        free(stream);
        return NULL;
    }
    
    memset(handler, 0, sizeof(cmsIOHANDLER));
    
    handler->stream = stream;
    handler->ContextID = NULL;
    handler->UsedSpace = 0;
    handler->ReportedSize = 0;
    memset(handler->PhysicalFile, 0, sizeof(handler->PhysicalFile));
    handler->Read = MockRead;
    handler->Seek = MockSeek;
    handler->Close = MockClose;
    handler->Tell = MockTell;
    handler->Write = MockWrite;
    
    return handler;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create mock handler for reading
    cmsIOHANDLER* read_handler = CreateMockHandler(Data, Size, FALSE);
    if (!read_handler) return 0;
    
    // Create mock handler for writing
    cmsIOHANDLER* write_handler = CreateMockHandler(NULL, 0, TRUE);
    if (!write_handler) {
        read_handler->Close(read_handler);
        return 0;
    }
    
    // Test _cmsReadFloat32Number
    cmsFloat32Number float_val = 0.0f;
    if (Size >= 4) {
        _cmsReadFloat32Number(read_handler, &float_val);
    }
    
    // Reset read position
    read_handler->Seek(read_handler, 0);
    
    // Test _cmsReadUInt32Number
    cmsUInt32Number uint_val = 0;
    if (Size >= 4) {
        _cmsReadUInt32Number(read_handler, &uint_val);
    }
    
    // Test _cmsReadAlignment
    _cmsReadAlignment(read_handler);
    
    // Reset read position to test different states
    read_handler->Seek(read_handler, 0);
    
    // Test _cmsWriteFloat32Number
    _cmsWriteFloat32Number(write_handler, float_val);
    
    // Test _cmsWriteUInt32Number
    _cmsWriteUInt32Number(write_handler, uint_val);
    
    // Test _cmsWriteAlignment
    _cmsWriteAlignment(write_handler);
    
    // Test with different values from fuzzer input
    if (Size >= 8) {
        read_handler->Seek(read_handler, 4);
        _cmsReadFloat32Number(read_handler, &float_val);
        _cmsWriteFloat32Number(write_handler, float_val);
    }
    
    // Test alignment after writing
    if (Size >= 12) {
        read_handler->Seek(read_handler, 8);
        _cmsReadUInt32Number(read_handler, &uint_val);
        _cmsWriteUInt32Number(write_handler, uint_val);
        _cmsWriteAlignment(write_handler);
    }
    
    // Clean up
    read_handler->Close(read_handler);
    write_handler->Close(write_handler);
    
    return 0;
}