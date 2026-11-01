// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Helper IO functions for memory-based I/O
static cmsUInt32Number MemRead(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (iohandler == NULL || Buffer == NULL) return 0;
    cmsUInt32Number total = size * count;
    cmsUInt32Number* pos = (cmsUInt32Number*)iohandler->stream;
    cmsUInt8Number* data = (cmsUInt8Number*)iohandler->stream + sizeof(cmsUInt32Number) + *pos;
    cmsUInt32Number available = iohandler->ReportedSize - *pos;
    cmsUInt32Number toRead = (total < available) ? total : available;
    if (toRead > 0) {
        memcpy(Buffer, data, toRead);
        *pos += toRead;
    }
    return toRead / size;
}

static cmsBool MemSeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (iohandler == NULL) return FALSE;
    cmsUInt32Number* pos = (cmsUInt32Number*)iohandler->stream;
    if (offset <= iohandler->ReportedSize) {
        *pos = offset;
        return TRUE;
    }
    return FALSE;
}

static cmsBool MemClose(cmsIOHANDLER* iohandler) {
    if (iohandler == NULL) return FALSE;
    return TRUE;
}

static cmsUInt32Number MemTell(cmsIOHANDLER* iohandler) {
    if (iohandler == NULL) return 0;
    cmsUInt32Number* pos = (cmsUInt32Number*)iohandler->stream;
    return *pos;
}

static cmsBool MemWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    if (iohandler == NULL || Buffer == NULL) return FALSE;
    cmsUInt32Number* pos = (cmsUInt32Number*)iohandler->stream;
    cmsUInt8Number* data = (cmsUInt8Number*)iohandler->stream + sizeof(cmsUInt32Number) + *pos;
    cmsUInt32Number available = iohandler->ReportedSize - *pos;
    if (size > available) return FALSE;
    memcpy(data, Buffer, size);
    *pos += size;
    return TRUE;
}

static cmsIOHANDLER* CreateMemIOHandler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (io == NULL) return NULL;
    
    // Allocate space for data + position indicator
    // We need: 1 * sizeof(cmsUInt32Number) for position + size for data
    cmsUInt32Number* stream = (cmsUInt32Number*)malloc(size + sizeof(cmsUInt32Number));
    if (stream == NULL) {
        free(io);
        return NULL;
    }
    
    *stream = 0; // Initial position
    memcpy(stream + 1, data, size);
    
    io->stream = stream;
    io->ContextID = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = size;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);
    
    io->Read = MemRead;
    io->Seek = MemSeek;
    io->Close = MemClose;
    io->Tell = MemTell;
    io->Write = MemWrite;
    
    return io;
}

int LLVMFuzzerTestOneInput_168(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsIOHANDLER* io = NULL;
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    // Create a memory-based IO handler
    io = CreateMemIOHandler(Data, Size);
    if (io == NULL) return 0;
    
    if (ctx != NULL) {
        io->ContextID = ctx;
    }
    
    // Fuzz _cmsReadUInt8Number
    cmsUInt8Number val8;
    _cmsReadUInt8Number(io, &val8);
    _cmsReadUInt8Number(io, NULL); // Skip byte
    
    // Fuzz _cmsReadUInt16Number
    cmsUInt16Number val16;
    _cmsReadUInt16Number(io, &val16);
    
    // Fuzz _cmsReadAlignment
    _cmsReadAlignment(io);
    
    // Seek to beginning for write operations
    MemSeek(io, 0);
    
    // Fuzz _cmsWriteUInt8Number
    _cmsWriteUInt8Number(io, 0xFF);
    _cmsWriteUInt8Number(io, 0x00);
    
    // Fuzz _cmsWriteUInt16Number
    _cmsWriteUInt16Number(io, 0xFFFF);
    _cmsWriteUInt16Number(io, 0x0000);
    
    // Test edge cases with different positions
    MemSeek(io, Size - 1);
    _cmsReadUInt8Number(io, &val8);
    
    MemSeek(io, Size - 2);
    _cmsReadUInt16Number(io, &val16);
    
    // Test alignment at various positions
    MemSeek(io, 1);
    _cmsReadAlignment(io);
    
    MemSeek(io, 2);
    _cmsReadAlignment(io);
    
    MemSeek(io, 3);
    _cmsReadAlignment(io);
    
    MemSeek(io, 4);
    _cmsReadAlignment(io);
    
    // Cleanup
    if (io != NULL) {
        // Only close if we own it and haven't passed it to other functions
        cmsCloseIOhandler(io);
        if (io->stream != NULL) {
            free(io->stream);
        }
        free(io);
    }
    
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
    
    return 0;
}