// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
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
#include <stdio.h>

// Memory-based I/O handler implementation
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
    cmsBool is_read;
    FILE* file; // For file-based operations
} MemoryStream;

static cmsUInt32Number MemoryRead(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    cmsUInt32Number total = size * count;
    
    if (stream->pos + total > stream->size) {
        total = stream->size - stream->pos;
    }
    
    if (total > 0) {
        memcpy(Buffer, stream->data + stream->pos, total);
        stream->pos += total;
    }
    
    return total / size;
}

static cmsBool MemorySeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (offset > stream->size) {
        return FALSE;
    }
    
    stream->pos = offset;
    return TRUE;
}

static cmsUInt32Number MemoryTell(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool MemoryClose(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (stream->file) {
        fclose(stream->file);
    }
    
    free(stream);
    iohandler->stream = NULL;
    return TRUE;
}

static cmsBool MemoryWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    
    if (stream->file) {
        // Write to file
        size_t written = fwrite(Buffer, 1, size, stream->file);
        return written == size;
    }
    
    // For memory write, we just advance position
    stream->pos += size;
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryIOHandler(const uint8_t* data, size_t size, cmsBool for_reading) {
    MemoryStream* stream = (MemoryStream*)calloc(1, sizeof(MemoryStream));
    if (!stream) return NULL;
    
    stream->data = data;
    stream->size = size;
    stream->pos = 0;
    stream->is_read = for_reading;
    stream->file = NULL;
    
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) {
        free(stream);
        return NULL;
    }
    
    io->stream = stream;
    io->Read = MemoryRead;
    io->Seek = MemorySeek;
    io->Tell = MemoryTell;
    io->Close = MemoryClose;
    io->Write = MemoryWrite;
    
    return io;
}

static cmsIOHANDLER* CreateFileIOHandler(const char* filename, cmsBool for_reading) {
    MemoryStream* stream = (MemoryStream*)calloc(1, sizeof(MemoryStream));
    if (!stream) return NULL;
    
    stream->file = fopen(filename, for_reading ? "rb" : "wb");
    if (!stream->file) {
        free(stream);
        return NULL;
    }
    
    stream->is_read = for_reading;
    
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) {
        fclose(stream->file);
        free(stream);
        return NULL;
    }
    
    io->stream = stream;
    io->Read = MemoryRead;
    io->Seek = MemorySeek;
    io->Tell = MemoryTell;
    io->Close = MemoryClose;
    io->Write = MemoryWrite;
    
    return io;
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    // Create file for file-based operations
    FILE* dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy);
        }
        fclose(dummy);
    }
    
    // Test _cmsReadUInt16Number
    cmsIOHANDLER* io1 = CreateMemoryIOHandler(Data, Size, TRUE);
    if (io1) {
        cmsUInt16Number n;
        _cmsReadUInt16Number(io1, &n);
        io1->Close(io1);
        free(io1);
    }
    
    // Test _cmsReadXYZNumber
    cmsIOHANDLER* io2 = CreateMemoryIOHandler(Data, Size, TRUE);
    if (io2) {
        cmsCIEXYZ xyz;
        _cmsReadXYZNumber(io2, &xyz);
        io2->Close(io2);
        free(io2);
    }
    
    // Test _cmsWriteTypeBase
    cmsIOHANDLER* io3 = CreateFileIOHandler("./dummy_file", FALSE);
    if (io3) {
        // Use first 4 bytes as tag signature if available
        cmsTagTypeSignature sig = 0;
        if (Size >= 4) {
            memcpy(&sig, Data, 4);
        }
        _cmsWriteTypeBase(io3, sig);
        io3->Close(io3);
        free(io3);
    }
    
    // Test _cmsReadAlignment
    cmsIOHANDLER* io4 = CreateMemoryIOHandler(Data, Size, TRUE);
    if (io4) {
        _cmsReadAlignment(io4);
        io4->Close(io4);
        free(io4);
    }
    
    // Test _cmsWriteXYZNumber
    cmsIOHANDLER* io5 = CreateFileIOHandler("./dummy_file", FALSE);
    if (io5) {
        cmsCIEXYZ xyz;
        // Initialize with data if available
        if (Size >= sizeof(cmsCIEXYZ)) {
            memcpy(&xyz, Data, sizeof(cmsCIEXYZ));
        } else {
            xyz.X = 0.0;
            xyz.Y = 0.0;
            xyz.Z = 0.0;
        }
        _cmsWriteXYZNumber(io5, &xyz);
        io5->Close(io5);
        free(io5);
    }
    
    // Test _cmsWriteAlignment
    cmsIOHANDLER* io6 = CreateFileIOHandler("./dummy_file", FALSE);
    if (io6) {
        _cmsWriteAlignment(io6);
        io6->Close(io6);
        free(io6);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}