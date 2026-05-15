// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// Define a simple memory-based I/O handler structure
typedef struct {
    uint8_t* data;
    size_t size;
    size_t pos;
    cmsBool is_read_only;
} MemoryStream;

// Static functions for our memory-based I/O handler
static cmsUInt32Number MemoryRead(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* mem = (MemoryStream*)iohandler->stream;
    cmsUInt32Number total_bytes = size * count;
    
    if (mem->pos + total_bytes > mem->size) {
        total_bytes = mem->size - mem->pos;
        if (total_bytes < size) return 0;
        total_bytes = (total_bytes / size) * size;
    }
    
    if (total_bytes > 0) {
        memcpy(Buffer, mem->data + mem->pos, total_bytes);
        mem->pos += total_bytes;
    }
    
    return total_bytes / size;
}

static cmsBool MemorySeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MemoryStream* mem = (MemoryStream*)iohandler->stream;
    if (offset > mem->size) return FALSE;
    mem->pos = offset;
    return TRUE;
}

static cmsBool MemoryClose(cmsIOHANDLER* iohandler) {
    MemoryStream* mem = (MemoryStream*)iohandler->stream;
    if (!mem->is_read_only) {
        free(mem->data);
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
    
    // For simplicity, we'll just advance position without actually writing
    // since we're fuzzing and don't need to validate written data
    if (mem->pos + size > mem->size) {
        return FALSE;
    }
    
    mem->pos += size;
    return TRUE;
}

// Create a memory-based I/O handler
static cmsIOHANDLER* CreateMemoryHandler(const uint8_t* data, size_t size, cmsBool for_writing) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    MemoryStream* mem = (MemoryStream*)calloc(1, sizeof(MemoryStream));
    if (!mem) {
        free(io);
        return NULL;
    }
    
    if (for_writing) {
        // Allocate buffer for writing
        mem->data = (uint8_t*)malloc(size);
        if (!mem->data) {
            free(mem);
            free(io);
            return NULL;
        }
        mem->size = size;
        mem->is_read_only = FALSE;
    } else {
        // Use provided data for reading
        mem->data = (uint8_t*)data;
        mem->size = size;
        mem->is_read_only = TRUE;
    }
    mem->pos = 0;
    
    io->stream = mem;
    io->Read = MemoryRead;
    io->Seek = MemorySeek;
    io->Close = MemoryClose;
    io->Tell = MemoryTell;
    io->Write = MemoryWrite;
    io->ContextID = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = (cmsUInt32Number)size;
    io->PhysicalFile[0] = '\0';
    
    return io;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    // Test _cmsReadUInt64Number
    cmsIOHANDLER* read_io = CreateMemoryHandler(Data, Size, FALSE);
    if (read_io) {
        cmsUInt64Number n64;
        if (_cmsReadUInt64Number(read_io, &n64)) {
            // Test _cmsAdjustEndianess64 with the read value
            cmsUInt64Number adjusted;
            _cmsAdjustEndianess64(&adjusted, &n64);
            
            // Test _cmsWriteUInt64Number
            cmsIOHANDLER* write_io = CreateMemoryHandler(NULL, Size, TRUE);
            if (write_io) {
                _cmsWriteUInt64Number(write_io, &n64);
                MemoryClose(write_io);
            }
        }
        MemoryClose(read_io);
    }
    
    // Test _cmsReadUInt8Number
    read_io = CreateMemoryHandler(Data, Size, FALSE);
    if (read_io) {
        cmsUInt8Number n8;
        if (_cmsReadUInt8Number(read_io, &n8)) {
            // Test _cmsWriteUInt8Number
            cmsIOHANDLER* write_io = CreateMemoryHandler(NULL, Size, TRUE);
            if (write_io) {
                _cmsWriteUInt8Number(write_io, n8);
                MemoryClose(write_io);
            }
        }
        MemoryClose(read_io);
    }
    
    // Test _cmsReadFloat32Number
    read_io = CreateMemoryHandler(Data, Size, FALSE);
    if (read_io) {
        cmsFloat32Number f32;
        if (_cmsReadFloat32Number(read_io, &f32)) {
            // Valid float read, can use it for further operations if needed
        }
        MemoryClose(read_io);
    }
    
    // Test with file I/O if there's enough data
    if (Size > 64) {
        FILE* fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
            fclose(fp);
            
            // Note: In a real fuzzer, we would use lcms's file I/O handlers,
            // but for this example we'll stick with memory handlers
        }
    }
    
    return 0;
}