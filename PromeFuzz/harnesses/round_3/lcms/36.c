// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

// Structure to hold both the data pointer and ownership flag
typedef struct {
    void* data;
    int owned;
} StreamData;

static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    StreamData* stream_data = (StreamData*)iohandler->stream;
    if (!stream_data || !stream_data->data) return 0;
    
    size_t total = size * count;
    if (iohandler->UsedSpace + total > iohandler->ReportedSize) {
        total = iohandler->ReportedSize - iohandler->UsedSpace;
    }
    if (total > 0) {
        memcpy(Buffer, (uint8_t*)stream_data->data + iohandler->UsedSpace, total);
        iohandler->UsedSpace += total;
    }
    return total / size;
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (offset <= iohandler->ReportedSize) {
        iohandler->UsedSpace = offset;
        return TRUE;
    }
    return FALSE;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    if (iohandler && iohandler->stream) {
        StreamData* stream_data = (StreamData*)iohandler->stream;
        if (stream_data) {
            if (stream_data->owned && stream_data->data) {
                free(stream_data->data);
            }
            free(stream_data);
        }
    }
    free(iohandler);
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    return iohandler->UsedSpace;
}

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    StreamData* stream_data = (StreamData*)iohandler->stream;
    if (!stream_data || !stream_data->data) return FALSE;
    
    if (iohandler->UsedSpace + size <= iohandler->ReportedSize) {
        memcpy((uint8_t*)stream_data->data + iohandler->UsedSpace, Buffer, size);
        iohandler->UsedSpace += size;
        return TRUE;
    }
    return FALSE;
}

static cmsIOHANDLER* create_dummy_iohandler(const uint8_t* data, size_t size, cmsBool for_writing) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    StreamData* stream_data = (StreamData*)calloc(1, sizeof(StreamData));
    if (!stream_data) {
        free(io);
        return NULL;
    }
    
    if (for_writing) {
        // For writing, allocate a buffer
        stream_data->data = malloc(size);
        if (!stream_data->data) {
            free(stream_data);
            free(io);
            return NULL;
        }
        stream_data->owned = 1;
        if (data && size > 0) {
            memcpy(stream_data->data, data, size);
        }
    } else {
        // For reading, just point to the data (don't own it)
        stream_data->data = (void*)data;
        stream_data->owned = 0;
    }
    
    io->stream = stream_data;
    io->ContextID = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = (cmsUInt32Number)size;
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    
    return io;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create I/O handlers for testing
    cmsIOHANDLER* read_io = create_dummy_iohandler(Data, Size, FALSE);
    cmsIOHANDLER* write_io = create_dummy_iohandler(NULL, Size, TRUE);
    
    if (!read_io || !write_io) {
        if (read_io) cmsCloseIOhandler(read_io);
        if (write_io) cmsCloseIOhandler(write_io);
        return 0;
    }
    
    // Test _cmsReadUInt16Number
    cmsUInt16Number read_val = 0;
    _cmsReadUInt16Number(read_io, &read_val);
    
    // Test _cmsWriteUInt16Number with various values
    _cmsWriteUInt16Number(write_io, read_val);
    _cmsWriteUInt16Number(write_io, 0x1234);
    _cmsWriteUInt16Number(write_io, 0xFFFF);
    
    // Test alignment functions
    _cmsReadAlignment(read_io);
    _cmsWriteAlignment(write_io);
    
    // Test _cmsWriteTypeBase with different signatures
    if (Size >= 4) {
        cmsTagTypeSignature sig = *(cmsTagTypeSignature*)Data;
        _cmsWriteTypeBase(write_io, sig);
    }
    
    // Reset read handler position and test more
    read_io->Seek(read_io, 0);
    read_io->UsedSpace = 0;
    
    // Try reading after writing
    cmsUInt16Number another_val = 0;
    _cmsReadUInt16Number(read_io, &another_val);
    _cmsWriteUInt16Number(write_io, another_val);
    
    // Test alignment again
    _cmsReadAlignment(read_io);
    _cmsWriteAlignment(write_io);
    
    // Clean up
    cmsCloseIOhandler(read_io);
    cmsCloseIOhandler(write_io);
    
    return 0;
}