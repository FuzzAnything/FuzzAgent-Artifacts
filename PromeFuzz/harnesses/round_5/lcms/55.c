// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
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

// Custom I/O handler implementation for fuzzing
static cmsUInt32Number fuzz_read(struct _cms_io_handler* iohandler, void* Buffer,
                                 cmsUInt32Number size, cmsUInt32Number count) {
    cmsUInt32Number total = size * count;
    if (iohandler->UsedSpace + total > iohandler->ReportedSize) {
        total = iohandler->ReportedSize - iohandler->UsedSpace;
    }
    if (total > 0) {
        memcpy(Buffer, (uint8_t*)iohandler->stream + iohandler->UsedSpace, total);
        iohandler->UsedSpace += total;
    }
    return total / size;
}

static cmsBool fuzz_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    if (offset > iohandler->ReportedSize) {
        return FALSE;
    }
    iohandler->UsedSpace = offset;
    return TRUE;
}

static cmsBool fuzz_close(struct _cms_io_handler* iohandler) {
    // Nothing to clean up for memory streams
    return TRUE;
}

static cmsUInt32Number fuzz_tell(struct _cms_io_handler* iohandler) {
    return iohandler->UsedSpace;
}

static cmsBool fuzz_write(struct _cms_io_handler* iohandler, cmsUInt32Number size,
                          const void* Buffer) {
    if (iohandler->UsedSpace + size > iohandler->ReportedSize) {
        return FALSE;
    }
    memcpy((uint8_t*)iohandler->stream + iohandler->UsedSpace, Buffer, size);
    iohandler->UsedSpace += size;
    return TRUE;
}

static cmsIOHANDLER* create_fuzz_iohandler(const uint8_t* data, size_t size, cmsBool for_writing) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    // Allocate buffer for stream
    size_t buffer_size = for_writing ? 4096 : size;
    io->stream = malloc(buffer_size);
    if (!io->stream) {
        free(io);
        return NULL;
    }
    
    if (for_writing) {
        // For writing, initialize with zeros
        memset(io->stream, 0, buffer_size);
    } else {
        // For reading, copy fuzzer data
        memcpy(io->stream, data, size);
    }
    
    io->ContextID = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = (cmsUInt32Number)buffer_size;
    io->PhysicalFile[0] = '\0';
    
    io->Read = fuzz_read;
    io->Seek = fuzz_seek;
    io->Close = fuzz_close;
    io->Tell = fuzz_tell;
    io->Write = fuzz_write;
    
    return io;
}

static void cleanup_fuzz_iohandler(cmsIOHANDLER* io) {
    if (io) {
        if (io->stream) free(io->stream);
        free(io);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create I/O handlers for reading and writing
    cmsIOHANDLER* read_io = create_fuzz_iohandler(Data, Size, FALSE);
    cmsIOHANDLER* write_io = create_fuzz_iohandler(NULL, 0, TRUE);
    
    if (!read_io || !write_io) {
        cleanup_fuzz_iohandler(read_io);
        cleanup_fuzz_iohandler(write_io);
        return 0;
    }
    
    // Test _cmsReadUInt16Number
    cmsUInt16Number read_val;
    if (_cmsReadUInt16Number(read_io, &read_val)) {
        // Test _cmsWriteUInt16Number with the read value
        _cmsWriteUInt16Number(write_io, read_val);
    }
    
    // Reset read position for more tests
    read_io->UsedSpace = 0;
    
    // Test _cmsReadUInt16Array
    cmsUInt32Number array_size = Size < 100 ? (cmsUInt32Number)Size / 2 : 50;
    if (array_size > 0) {
        cmsUInt16Number* read_array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
        if (read_array) {
            if (_cmsReadUInt16Array(read_io, array_size, read_array)) {
                // Test _cmsWriteUInt16Array with the read array
                _cmsWriteUInt16Array(write_io, array_size, read_array);
            }
            free(read_array);
        }
    }
    
    // Reset read position for alignment test
    read_io->UsedSpace = 0;
    
    // Test _cmsReadAlignment
    _cmsReadAlignment(read_io);
    
    // Clean up
    cmsCloseIOhandler(read_io);
    cmsCloseIOhandler(write_io);
    cleanup_fuzz_iohandler(read_io);
    cleanup_fuzz_iohandler(write_io);
    
    return 0;
}