// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Global state for the IO handler simulation
static const uint8_t *g_io_buffer = NULL;
static size_t g_io_size = 0;
static size_t g_io_offset = 0;

static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (!g_io_buffer || !Buffer) return 0;
    cmsUInt32Number total = size * count;
    
    // Prevent overflow and out-of-bounds access
    if (total == 0 || size == 0 || count == 0) return 0;
    
    if (g_io_offset > g_io_size) return 0;
    
    cmsUInt32Number remaining = (cmsUInt32Number)(g_io_size - g_io_offset);
    if (total > remaining) {
        total = remaining;
    }
    
    if (total == 0) return 0;
    
    memcpy(Buffer, g_io_buffer + g_io_offset, total);
    g_io_offset += total;
    return total / size;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    if (offset > g_io_size) return FALSE;
    g_io_offset = offset;
    return TRUE;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    return g_io_offset;
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    // Simulate write success without actually writing to prevent buffer issues
    return TRUE; 
}

static cmsIOHANDLER* prepare_io_handler(const uint8_t *Data, size_t Size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;

    g_io_buffer = Data;
    g_io_size = Size;
    g_io_offset = 0;

    io->stream = (void*)Data;
    io->ContextID = cmsCreateContext(0, NULL);
    io->UsedSpace = 0;
    io->ReportedSize = Size;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);

    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;

    return io;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsIOHANDLER* io = prepare_io_handler(Data, Size);
    if (!io) return 0;

    cmsCIEXYZ xyz = {0.0, 0.0, 0.0};
    cmsTagTypeSignature sig = 0;

    // Fuzz _cmsReadXYZNumber
    g_io_offset = 0;
    _cmsReadXYZNumber(io, &xyz);

    // Fuzz _cmsReadAlignment
    // Ensure we have enough data to avoid reading beyond buffer
    if (Size >= 4) {
        g_io_offset = 0;
        _cmsReadAlignment(io);
        
        g_io_offset = 1;
        _cmsReadAlignment(io);
        
        g_io_offset = 2;
        _cmsReadAlignment(io);
        
        g_io_offset = 3;
        _cmsReadAlignment(io);
    }

    // Fuzz _cmsWriteAlignment
    if (Size >= 4) {
        g_io_offset = 0;
        _cmsWriteAlignment(io);
        g_io_offset = 1;
        _cmsWriteAlignment(io);
        g_io_offset = 2;
        _cmsWriteAlignment(io);
        g_io_offset = 3;
        _cmsWriteAlignment(io);
    }

    // Fuzz _cmsWriteXYZNumber
    g_io_offset = 0;
    _cmsWriteXYZNumber(io, &xyz);

    // Fuzz _cmsWriteTypeBase
    if (Size >= 4) {
        memcpy(&sig, Data, 4);
    } else {
        sig = 0x00000000;
    }
    g_io_offset = 0;
    _cmsWriteTypeBase(io, sig);

    sig = 0;
    g_io_offset = 0;
    _cmsWriteTypeBase(io, sig);

    // Cleanup: Close the handler first, then free the structure
    // Note: cmsCloseIOhandler might access io->ContextID internally or via callbacks
    if (io) {
        cmsCloseIOhandler(io);
    }

    // Free the memory pool if a context was created
    if (io && io->ContextID) {
        cmsDeleteContext(io->ContextID);
    }

    // Now it is safe to free the io structure
    free(io);
    io = NULL; // Prevent use-after-free

    return 0;
}