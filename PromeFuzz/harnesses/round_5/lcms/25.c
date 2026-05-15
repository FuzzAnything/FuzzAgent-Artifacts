// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Custom memory-based I/O handler implementation
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
    cmsBool is_read_only;
    uint8_t* write_buffer;
    size_t write_buffer_size;
} MemoryStream;

static cmsUInt32Number MemoryRead(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* mem = (MemoryStream*)iohandler->stream;
    cmsUInt32Number total_bytes = size * count;
    
    if (mem->pos + total_bytes > mem->size) {
        total_bytes = (cmsUInt32Number)(mem->size - mem->pos);
    }
    
    if (total_bytes > 0) {
        memcpy(Buffer, mem->data + mem->pos, total_bytes);
        mem->pos += total_bytes;
    }
    
    return total_bytes / size;
}

static cmsBool MemorySeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MemoryStream* mem = (MemoryStream*)iohandler->stream;
    
    if (offset > mem->size) {
        return FALSE;
    }
    
    mem->pos = offset;
    return TRUE;
}

static cmsBool MemoryClose(cmsIOHANDLER* iohandler) {
    MemoryStream* mem = (MemoryStream*)iohandler->stream;
    
    if (!mem->is_read_only && mem->write_buffer) {
        free(mem->write_buffer);
    }
    
    free(mem);
    free(iohandler);
    return TRUE;
}

static cmsUInt32Number MemoryTell(cmsIOHANDLER* iohandler) {
    MemoryStream* mem = (MemoryStream*)iohandler->stream;
    return (cmsUInt32Number)mem->pos;
}

static cmsBool MemoryWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    MemoryStream* mem = (MemoryStream*)iohandler->stream;
    
    if (mem->is_read_only) {
        return FALSE;
    }
    
    // Resize write buffer if needed
    if (mem->pos + size > mem->write_buffer_size) {
        size_t new_size = mem->pos + size;
        uint8_t* new_buffer = (uint8_t*)realloc(mem->write_buffer, new_size);
        if (!new_buffer) {
            return FALSE;
        }
        mem->write_buffer = new_buffer;
        mem->write_buffer_size = new_size;
        mem->size = new_size;
    }
    
    memcpy(mem->write_buffer + mem->pos, Buffer, size);
    mem->pos += size;
    
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryHandler(const uint8_t* data, size_t size, cmsBool for_writing) {
    cmsIOHANDLER* handler = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!handler) {
        return NULL;
    }
    
    MemoryStream* mem = (MemoryStream*)calloc(1, sizeof(MemoryStream));
    if (!mem) {
        free(handler);
        return NULL;
    }
    
    mem->data = data;
    mem->size = size;
    mem->pos = 0;
    mem->is_read_only = !for_writing;
    
    if (for_writing) {
        mem->write_buffer = (uint8_t*)malloc(size);
        if (!mem->write_buffer) {
            free(mem);
            free(handler);
            return NULL;
        }
        mem->write_buffer_size = size;
        mem->size = 0; // Start with empty buffer for writing
    }
    
    handler->stream = mem;
    handler->Read = MemoryRead;
    handler->Seek = MemorySeek;
    handler->Close = MemoryClose;
    handler->Tell = MemoryTell;
    handler->Write = MemoryWrite;
    handler->ContextID = NULL;
    handler->UsedSpace = 0;
    handler->ReportedSize = 0;
    handler->PhysicalFile[0] = '\0';
    
    return handler;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    // Test _cmsRead15Fixed16Number
    cmsIOHANDLER* read_handler = CreateMemoryHandler(Data, Size, FALSE);
    if (read_handler) {
        cmsFloat64Number read_result;
        _cmsRead15Fixed16Number(read_handler, &read_result);
        read_handler->Close(read_handler);
    }
    
    // Test _cmsWrite15Fixed16Number
    cmsIOHANDLER* write_handler = CreateMemoryHandler(Data, Size, TRUE);
    if (write_handler) {
        // Use first 8 bytes as double if available, otherwise use 0.0
        cmsFloat64Number write_value = 0.0;
        if (Size >= 8) {
            memcpy(&write_value, Data, sizeof(cmsFloat64Number));
        }
        _cmsWrite15Fixed16Number(write_handler, write_value);
        write_handler->Close(write_handler);
    }
    
    // Test _cmsDoubleTo15Fixed16
    cmsFloat64Number double_value = 0.0;
    if (Size >= 8) {
        memcpy(&double_value, Data, sizeof(cmsFloat64Number));
    } else if (Size >= 4) {
        // Use first 4 bytes as float if double not available
        float float_value;
        memcpy(&float_value, Data, sizeof(float));
        double_value = (cmsFloat64Number)float_value;
    }
    _cmsDoubleTo15Fixed16(double_value);
    
    // Test _cms8Fixed8toDouble
    if (Size >= 2) {
        cmsUInt16Number fixed8;
        memcpy(&fixed8, Data, sizeof(cmsUInt16Number));
        _cms8Fixed8toDouble(fixed8);
    }
    
    // Test _cms15Fixed16toDouble
    if (Size >= 4) {
        cmsS15Fixed16Number fix32;
        memcpy(&fix32, Data, sizeof(cmsS15Fixed16Number));
        _cms15Fixed16toDouble(fix32);
    }
    
    // Test _cmsDoubleTo8Fixed8
    _cmsDoubleTo8Fixed8(double_value);
    
    return 0;
}