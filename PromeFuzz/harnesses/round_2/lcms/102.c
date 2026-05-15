// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Memory I/O handler implementation
typedef struct {
    uint8_t* data;
    size_t size;
    size_t pos;
    cmsBool is_read_only;
} MemoryStream;

static cmsUInt32Number MemoryRead(cmsIOHANDLER* iohandler, void* Buffer,
                                  cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    cmsUInt32Number total = size * count;
    
    if (stream->pos + total > stream->size) {
        total = stream->size - stream->pos;
        if (total < size) return 0;
        count = total / size;
        total = size * count;
    }
    
    if (total > 0) {
        memcpy(Buffer, stream->data + stream->pos, total);
        stream->pos += total;
    }
    
    return count;
}

static cmsBool MemorySeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    if (offset > stream->size) return FALSE;
    stream->pos = offset;
    return TRUE;
}

static cmsUInt32Number MemoryTell(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool MemoryWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                           const void* Buffer) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (stream->is_read_only) return FALSE;
    
    if (stream->pos + size > stream->size) {
        // Resize buffer if needed
        size_t new_size = stream->pos + size;
        uint8_t* new_data = realloc(stream->data, new_size);
        if (!new_data) return FALSE;
        stream->data = new_data;
        stream->size = new_size;
    }
    
    memcpy(stream->data + stream->pos, Buffer, size);
    stream->pos += size;
    
    if (stream->pos > iohandler->UsedSpace) {
        iohandler->UsedSpace = stream->pos;
    }
    
    return TRUE;
}

static cmsBool MemoryClose(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    if (stream) {
        free(stream->data);
        free(stream);
    }
    free(iohandler);
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryHandler(const uint8_t* data, size_t size, cmsBool for_reading) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    MemoryStream* stream = (MemoryStream*)calloc(1, sizeof(MemoryStream));
    if (!stream) {
        free(io);
        return NULL;
    }
    
    if (for_reading && data && size > 0) {
        stream->data = (uint8_t*)malloc(size);
        if (!stream->data) {
            free(stream);
            free(io);
            return NULL;
        }
        memcpy(stream->data, data, size);
        stream->size = size;
        stream->is_read_only = TRUE;
    } else {
        // For writing, start with a reasonable buffer
        stream->size = size > 0 ? size : 4096;
        stream->data = (uint8_t*)malloc(stream->size);
        if (!stream->data) {
            free(stream);
            free(io);
            return NULL;
        }
        stream->is_read_only = FALSE;
    }
    
    stream->pos = 0;
    
    io->stream = stream;
    io->Read = MemoryRead;
    io->Seek = MemorySeek;
    io->Tell = MemoryTell;
    io->Write = MemoryWrite;
    io->Close = MemoryClose;
    io->UsedSpace = 0;
    io->ReportedSize = (cmsUInt32Number)stream->size;
    
    return io;
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Test _cmsWriteUInt16Number and _cmsReadUInt16Number
    {
        cmsIOHANDLER* write_io = CreateMemoryHandler(NULL, Size, FALSE);
        if (write_io) {
            // Write a 16-bit number derived from the first 2 bytes
            cmsUInt16Number write_value = (Data[0] << 8) | Data[1];
            _cmsWriteUInt16Number(write_io, write_value);
            
            // Seek back and read it
            write_io->Seek(write_io, 0);
            cmsUInt16Number read_value;
            _cmsReadUInt16Number(write_io, &read_value);
            
            write_io->Close(write_io);
        }
    }
    
    // Test _cmsWriteUInt16Array and _cmsReadUInt16Array
    if (Size >= 4) {
        cmsIOHANDLER* write_io = CreateMemoryHandler(NULL, Size, FALSE);
        if (write_io) {
            // Determine array size (limited to prevent excessive allocation)
            cmsUInt32Number array_size = (Size / 2) % 256;
            if (array_size == 0) array_size = 1;
            
            cmsUInt16Number* array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
            if (array) {
                // Fill array with data from fuzzer input
                for (cmsUInt32Number i = 0; i < array_size && (i * 2 + 1) < Size; i++) {
                    array[i] = (Data[i * 2] << 8) | Data[i * 2 + 1];
                }
                
                _cmsWriteUInt16Array(write_io, array_size, array);
                
                // Seek back and read the array
                write_io->Seek(write_io, 0);
                cmsUInt16Number* read_array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
                if (read_array) {
                    _cmsReadUInt16Array(write_io, array_size, read_array);
                    free(read_array);
                }
                
                free(array);
            }
            
            write_io->Close(write_io);
        }
    }
    
    // Test _cmsReadAlignment and _cmsWriteAlignment
    {
        // Create a handler with some data
        cmsIOHANDLER* io = CreateMemoryHandler(Data, Size, TRUE);
        if (io) {
            // Test read alignment
            _cmsReadAlignment(io);
            
            // Close and create a write handler for write alignment test
            io->Close(io);
            
            cmsIOHANDLER* write_io = CreateMemoryHandler(NULL, Size, FALSE);
            if (write_io) {
                // Write some data to create a non-aligned position
                if (Size >= 1) {
                    write_io->Write(write_io, 1, Data);
                }
                
                // Test write alignment
                _cmsWriteAlignment(write_io);
                
                write_io->Close(write_io);
            }
        }
    }
    
    // Test combined operations
    if (Size >= 10) {
        cmsIOHANDLER* io = CreateMemoryHandler(Data, Size, TRUE);
        if (io) {
            // Read a number
            cmsUInt16Number num;
            if (_cmsReadUInt16Number(io, &num)) {
                // Try to read alignment
                _cmsReadAlignment(io);
                
                // Try to read an array if enough data remains
                cmsUInt32Number remaining = Size - io->Tell(io);
                if (remaining >= 4) {
                    cmsUInt32Number array_size = (remaining / 2) % 128;
                    if (array_size > 0) {
                        cmsUInt16Number* array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
                        if (array) {
                            _cmsReadUInt16Array(io, array_size, array);
                            free(array);
                        }
                    }
                }
            }
            
            io->Close(io);
        }
    }
    
    return 0;
}