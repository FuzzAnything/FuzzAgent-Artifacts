// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext g_context = NULL;

static cmsUInt32Number dummy_read(cmsIOHANDLER* io, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (io == NULL || Buffer == NULL) return 0;
    cmsUInt32Number total = size * count;
    cmsUInt32Number remaining = io->ReportedSize - io->UsedSpace;
    cmsUInt32Number to_read = (total < remaining) ? total : remaining;
    if (to_read == 0) return 0;
    
    // Simulate reading from a buffer stored in stream
    unsigned char* src = (unsigned char*)io->stream;
    memcpy(Buffer, src + io->UsedSpace, to_read);
    io->UsedSpace += to_read;
    return to_read;
}

static cmsBool dummy_seek(cmsIOHANDLER* io, cmsUInt32Number offset) {
    if (io == NULL) return FALSE;
    if (offset > io->ReportedSize) return FALSE;
    io->UsedSpace = offset;
    return TRUE;
}

static cmsBool dummy_close(cmsIOHANDLER* io) {
    if (io == NULL) return FALSE;
    if (io->stream != NULL) {
        free(io->stream);
        io->stream = NULL;
    }
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* io) {
    if (io == NULL) return 0;
    return io->UsedSpace;
}

static cmsBool dummy_write(cmsIOHANDLER* io, cmsUInt32Number size, const void* Buffer) {
    if (io == NULL || Buffer == NULL) return FALSE;
    // For fuzzing write operations, we just simulate success if space allows
    if (io->UsedSpace + size > io->ReportedSize) return FALSE;
    unsigned char* dest = (unsigned char*)io->stream;
    memcpy(dest + io->UsedSpace, Buffer, size);
    io->UsedSpace += size;
    return TRUE;
}

static cmsIOHANDLER* create_dummy_iohandler(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return NULL;
    
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (io == NULL) return NULL;
    
    memset(io, 0, sizeof(cmsIOHANDLER));
    
    io->stream = malloc(size);
    if (io->stream == NULL) {
        free(io);
        return NULL;
    }
    memcpy(io->stream, data, size);
    
    io->ContextID = g_context;
    io->UsedSpace = 0;
    io->ReportedSize = size;
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    
    return io;
}

static void cleanup_iohandler(cmsIOHANDLER* io) {
    if (io != NULL) {
        cmsCloseIOhandler(io);
        free(io);
    }
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    if (g_context == NULL) {
        g_context = cmsCreateContext(NULL, NULL);
        if (g_context == NULL) return 0;
    }

    // Create IO handler for reading
    cmsIOHANDLER* io_read = create_dummy_iohandler(Data, Size);
    if (io_read == NULL) return 0;

    // Create IO handler for writing (separate buffer)
    cmsIOHANDLER* io_write = create_dummy_iohandler(Data, Size);
    if (io_write == NULL) {
        cleanup_iohandler(io_read);
        return 0;
    }

    cmsUInt8Number u8_val;
    cmsFloat32Number f32_val;
    cmsBool result;

    // Test _cmsReadUInt8Number
    result = _cmsReadUInt8Number(io_read, &u8_val);
    (void)result;

    // Test _cmsReadUInt8Number with NULL output (skip byte)
    result = _cmsReadUInt8Number(io_read, NULL);
    (void)result;

    // Test _cmsReadFloat32Number
    result = _cmsReadFloat32Number(io_read, &f32_val);
    (void)result;

    // Test _cmsReadAlignment
    result = _cmsReadAlignment(io_read);
    (void)result;

    // Reset write handler position
    dummy_seek(io_write, 0);

    // Test _cmsWriteUInt8Number
    result = _cmsWriteUInt8Number(io_write, 0xFF);
    (void)result;

    // Test _cmsWriteFloat32Number
    result = _cmsWriteFloat32Number(io_write, 3.14159f);
    (void)result;

    // Test with extreme float values
    result = _cmsWriteFloat32Number(io_write, 1e30f);
    (void)result;
    result = _cmsWriteFloat32Number(io_write, -1e30f);
    (void)result;
    
    // Test with NaN and Inf if supported
    result = _cmsWriteFloat32Number(io_write, NAN);
    (void)result;
    result = _cmsWriteFloat32Number(io_write, INFINITY);
    (void)result;
    result = _cmsWriteFloat32Number(io_write, -INFINITY);
    (void)result;

    // Cleanup
    cleanup_iohandler(io_read);
    cleanup_iohandler(io_write);

    return 0;
}