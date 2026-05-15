// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>
#include <unistd.h>

// Custom I/O handler implementation for fuzzing
static cmsUInt32Number fuzz_read(cmsIOHANDLER* iohandler, void* Buffer, 
                                 cmsUInt32Number size, cmsUInt32Number count) {
    // Use the stream pointer as our current position in fuzz data
    size_t* pos = (size_t*)iohandler->stream;
    size_t total_bytes = size * count;
    
    // Don't read past the end of our data
    if (*pos + total_bytes > iohandler->ReportedSize) {
        total_bytes = iohandler->ReportedSize - *pos;
        count = total_bytes / size;
    }
    
    if (total_bytes > 0) {
        // Copy from the fuzz data (stored in PhysicalFile as a hack)
        memcpy(Buffer, iohandler->PhysicalFile + *pos, total_bytes);
        *pos += total_bytes;
    }
    
    return count;
}

static cmsBool fuzz_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    size_t* pos = (size_t*)iohandler->stream;
    
    if (offset > iohandler->ReportedSize) {
        return FALSE;
    }
    
    *pos = offset;
    return TRUE;
}

static cmsUInt32Number fuzz_tell(cmsIOHANDLER* iohandler) {
    size_t* pos = (size_t*)iohandler->stream;
    return (cmsUInt32Number)*pos;
}

static cmsBool fuzz_close(cmsIOHANDLER* iohandler) {
    // Nothing to clean up
    return TRUE;
}

static cmsBool fuzz_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, 
                          const void* Buffer) {
    // Use PhysicalFile as output buffer (limited to cmsMAX_PATH size)
    size_t* pos = (size_t*)iohandler->stream;
    
    if (*pos + size > cmsMAX_PATH) {
        return FALSE;
    }
    
    memcpy(iohandler->PhysicalFile + *pos, Buffer, size);
    *pos += size;
    iohandler->UsedSpace = *pos;
    
    return TRUE;
}

// Create a fuzzing I/O handler
static cmsIOHANDLER* create_fuzz_handler(const uint8_t* data, size_t size) {
    static cmsIOHANDLER handler;
    static size_t position;
    
    // Initialize position
    position = 0;
    
    // Set up the handler
    handler.stream = &position;
    handler.ContextID = NULL;
    handler.UsedSpace = 0;
    handler.ReportedSize = (cmsUInt32Number)size;
    
    // Copy fuzz data into PhysicalFile buffer (hack to store data)
    size_t copy_size = size < cmsMAX_PATH ? size : cmsMAX_PATH;
    memcpy(handler.PhysicalFile, data, copy_size);
    
    // Set function pointers
    handler.Read = fuzz_read;
    handler.Seek = fuzz_seek;
    handler.Close = fuzz_close;
    handler.Tell = fuzz_tell;
    handler.Write = fuzz_write;
    
    return &handler;
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    // Create I/O handler with fuzz data
    cmsIOHANDLER* io = create_fuzz_handler(Data, Size);
    
    // Test _cmsReadFloat32Number
    if (Size >= 4) {
        cmsFloat32Number float_val;
        _cmsReadFloat32Number(io, &float_val);
    }
    
    // Reset position for next test
    fseek(io, 0, SEEK_SET);
    
    // Test _cmsReadUInt64Number
    if (Size >= 8) {
        cmsUInt64Number uint64_val;
        _cmsReadUInt64Number(io, &uint64_val);
    }
    
    // Reset position for write tests
    fseek(io, 0, SEEK_SET);
    
    // Test _cmsAdjustEndianess64
    if (Size >= 8) {
        cmsUInt64Number input, output;
        memcpy(&input, Data, Size >= 8 ? 8 : Size);
        _cmsAdjustEndianess64(&output, &input);
    }
    
    // Reset position for write tests
    fseek(io, 0, SEEK_SET);
    
    // Test _cmsWriteXYZNumber
    cmsCIEXYZ xyz;
    if (Size >= 12) {
        // Initialize with some fuzz data
        memcpy(&xyz, Data, Size >= sizeof(cmsCIEXYZ) ? sizeof(cmsCIEXYZ) : Size);
        _cmsWriteXYZNumber(io, &xyz);
    } else {
        // Use default values if not enough data
        xyz.X = xyz.Y = xyz.Z = 0.0;
        _cmsWriteXYZNumber(io, &xyz);
    }
    
    // Reset position for next write test
    fseek(io, 0, SEEK_SET);
    
    // Test _cmsWriteFloat32Number
    cmsFloat32Number write_float;
    if (Size >= 4) {
        memcpy(&write_float, Data, 4);
    } else {
        write_float = 0.0f;
    }
    _cmsWriteFloat32Number(io, write_float);
    
    // Reset position for final write test
    fseek(io, 0, SEEK_SET);
    
    // Test _cmsWriteUInt64Number
    cmsUInt64Number write_uint64;
    if (Size >= 8) {
        memcpy(&write_uint64, Data, 8);
    } else {
        write_uint64 = 0;
    }
    _cmsWriteUInt64Number(io, &write_uint64);
    
    // Clean up
    fuzz_close(io);
    
    return 0;
}