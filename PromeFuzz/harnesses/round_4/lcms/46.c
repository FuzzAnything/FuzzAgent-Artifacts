// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Memory-based I/O handler implementation
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
    cmsBool read_only;
    uint8_t* write_buffer;
    size_t write_buffer_size;
} MemoryStream;

static cmsUInt32Number MemoryRead(cmsIOHANDLER* iohandler, void* Buffer,
                                 cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* mem = (MemoryStream*)iohandler->stream;
    cmsUInt32Number total_bytes = size * count;
    
    if (mem->pos + total_bytes > mem->size) {
        total_bytes = mem->size - mem->pos;
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
    
    if (!mem->read_only && mem->write_buffer) {
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

static cmsBool MemoryWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                          const void* Buffer) {
    MemoryStream* mem = (MemoryStream*)iohandler->stream;
    
    if (mem->read_only) {
        return FALSE;
    }
    
    // Grow write buffer if needed
    if (mem->pos + size > mem->write_buffer_size) {
        size_t new_size = mem->pos + size;
        uint8_t* new_buffer = (uint8_t*)realloc(mem->write_buffer, new_size);
        if (!new_buffer) {
            return FALSE;
        }
        mem->write_buffer = new_buffer;
        mem->write_buffer_size = new_size;
    }
    
    memcpy(mem->write_buffer + mem->pos, Buffer, size);
    mem->pos += size;
    
    if (mem->pos > mem->size) {
        mem->size = mem->pos;
    }
    
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryHandler(const uint8_t* data, size_t size, cmsBool read_only) {
    MemoryStream* mem = (MemoryStream*)malloc(sizeof(MemoryStream));
    if (!mem) return NULL;
    
    mem->data = data;
    mem->size = size;
    mem->pos = 0;
    mem->read_only = read_only;
    mem->write_buffer = NULL;
    mem->write_buffer_size = 0;
    
    if (!read_only) {
        mem->write_buffer = (uint8_t*)malloc(size);
        if (!mem->write_buffer) {
            free(mem);
            return NULL;
        }
        mem->write_buffer_size = size;
        if (size > 0) {
            memcpy(mem->write_buffer, data, size);
        }
    }
    
    cmsIOHANDLER* handler = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!handler) {
        if (!read_only && mem->write_buffer) {
            free(mem->write_buffer);
        }
        free(mem);
        return NULL;
    }
    
    handler->stream = mem;
    handler->ContextID = 0;
    handler->UsedSpace = 0;
    handler->ReportedSize = 0;
    handler->PhysicalFile[0] = '\0';
    handler->Read = MemoryRead;
    handler->Seek = MemorySeek;
    handler->Close = MemoryClose;
    handler->Tell = MemoryTell;
    handler->Write = MemoryWrite;
    
    return handler;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Use first byte to determine which function to test
    uint8_t function_selector = Data[0] % 6;
    const uint8_t* fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (function_selector) {
        case 0: {
            // _cmsAdjustEndianess64
            if (fuzz_size >= sizeof(cmsUInt64Number) * 2) {
                cmsUInt64Number input, output;
                memcpy(&input, fuzz_data, sizeof(cmsUInt64Number));
                _cmsAdjustEndianess64(&output, &input);
                // Also test with swapped input
                _cmsAdjustEndianess64(&input, &output);
            }
            break;
        }
        
        case 1: {
            // _cmsWriteXYZNumber
            if (fuzz_size >= sizeof(cmsCIEXYZ)) {
                cmsIOHANDLER* io = CreateMemoryHandler(fuzz_data, fuzz_size, FALSE);
                if (io) {
                    cmsCIEXYZ xyz;
                    memcpy(&xyz, fuzz_data, sizeof(cmsCIEXYZ));
                    _cmsWriteXYZNumber(io, &xyz);
                    io->Close(io);
                }
            }
            break;
        }
        
        case 2: {
            // _cmsReadUInt64Number
            if (fuzz_size >= 1) {
                cmsIOHANDLER* io = CreateMemoryHandler(fuzz_data, fuzz_size, TRUE);
                if (io) {
                    cmsUInt64Number n;
                    _cmsReadUInt64Number(io, &n);
                    io->Close(io);
                }
            }
            break;
        }
        
        case 3: {
            // _cmsReadUInt16Number
            if (fuzz_size >= 1) {
                cmsIOHANDLER* io = CreateMemoryHandler(fuzz_data, fuzz_size, TRUE);
                if (io) {
                    cmsUInt16Number n;
                    _cmsReadUInt16Number(io, &n);
                    io->Close(io);
                }
            }
            break;
        }
        
        case 4: {
            // _cmsReadXYZNumber
            if (fuzz_size >= 1) {
                cmsIOHANDLER* io = CreateMemoryHandler(fuzz_data, fuzz_size, TRUE);
                if (io) {
                    cmsCIEXYZ xyz;
                    _cmsReadXYZNumber(io, &xyz);
                    io->Close(io);
                }
            }
            break;
        }
        
        case 5: {
            // _cmsWriteUInt64Number
            if (fuzz_size >= sizeof(cmsUInt64Number)) {
                cmsIOHANDLER* io = CreateMemoryHandler(fuzz_data, fuzz_size, FALSE);
                if (io) {
                    cmsUInt64Number n;
                    memcpy(&n, fuzz_data, sizeof(cmsUInt64Number));
                    _cmsWriteUInt64Number(io, &n);
                    io->Close(io);
                }
            }
            break;
        }
    }
    
    return 0;
}