// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Mock I/O handler implementation for memory-based operations
static cmsUInt32Number mock_read(cmsIOHANDLER* iohandler, void* Buffer, 
                                 cmsUInt32Number size, cmsUInt32Number count) {
    if (!iohandler || !Buffer || size == 0 || count == 0) return 0;
    
    cmsUInt32Number total_bytes = size * count;
    cmsUInt32Number available = iohandler->ReportedSize - iohandler->UsedSpace;
    cmsUInt32Number to_read = total_bytes < available ? total_bytes : available;
    
    if (to_read > 0) {
        memcpy(Buffer, (uint8_t*)iohandler->stream + iohandler->UsedSpace, to_read);
        iohandler->UsedSpace += to_read;
    }
    
    return to_read / size;
}

static cmsBool mock_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (!iohandler || offset > iohandler->ReportedSize) return FALSE;
    iohandler->UsedSpace = offset;
    return TRUE;
}

static cmsBool mock_close(cmsIOHANDLER* iohandler) {
    if (!iohandler) return FALSE;
    free(iohandler->stream);
    free(iohandler);
    return TRUE;
}

static cmsUInt32Number mock_tell(cmsIOHANDLER* iohandler) {
    return iohandler ? iohandler->UsedSpace : 0;
}

static cmsBool mock_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    if (!iohandler || !Buffer || size == 0) return FALSE;
    
    cmsUInt32Number available = iohandler->ReportedSize - iohandler->UsedSpace;
    if (size > available) return FALSE;
    
    memcpy((uint8_t*)iohandler->stream + iohandler->UsedSpace, Buffer, size);
    iohandler->UsedSpace += size;
    return TRUE;
}

static cmsIOHANDLER* create_mock_iohandler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    io->stream = malloc(size);
    if (!io->stream) {
        free(io);
        return NULL;
    }
    
    memcpy(io->stream, data, size);
    io->ContextID = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = (cmsUInt32Number)size;
    memset(io->PhysicalFile, 0, sizeof(io->PhysicalFile));
    
    io->Read = mock_read;
    io->Seek = mock_seek;
    io->Close = mock_close;
    io->Tell = mock_tell;
    io->Write = mock_write;
    
    return io;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create mock I/O handler with fuzzer data
    cmsIOHANDLER* io = create_mock_iohandler(Data, Size);
    if (!io) return 0;
    
    // Test _cmsWriteUInt16Number
    cmsUInt16Number write_val16 = (Data[0] << 8) | Data[1];
    _cmsWriteUInt16Number(io, write_val16);
    
    // Reset position for reading
    io->Seek(io, 0);
    
    // Test _cmsReadUInt16Number
    cmsUInt16Number read_val16;
    _cmsReadUInt16Number(io, &read_val16);
    
    // Test _cmsWriteUInt32Number
    cmsUInt32Number write_val32 = 0;
    if (Size >= 4) {
        write_val32 = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
    }
    _cmsWriteUInt32Number(io, write_val32);
    
    // Reset position for reading
    io->Seek(io, 0);
    
    // Test _cmsReadUInt32Number
    cmsUInt32Number read_val32;
    _cmsReadUInt32Number(io, &read_val32);
    
    // Test _cmsWriteUInt16Array
    cmsUInt32Number array_size = Size < 100 ? (cmsUInt32Number)Size / 2 : 50;
    cmsUInt16Number* write_array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
    if (write_array) {
        for (cmsUInt32Number i = 0; i < array_size; i++) {
            write_array[i] = (Data[i % Size] << 8) | Data[(i + 1) % Size];
        }
        _cmsWriteUInt16Array(io, array_size, write_array);
        free(write_array);
    }
    
    // Reset position for reading
    io->Seek(io, 0);
    
    // Test _cmsReadUInt16Array
    cmsUInt16Number* read_array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
    if (read_array) {
        _cmsReadUInt16Array(io, array_size, read_array);
        free(read_array);
    }
    
    // Clean up
    io->Close(io);
    
    return 0;
}