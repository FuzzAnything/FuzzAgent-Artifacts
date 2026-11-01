// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext g_context = NULL;

static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (!iohandler || !Buffer) return 0;
    size_t total = size * count;
    if (total == 0) return 0;
    
    // Simulate reading from a static buffer or return zeros
    memset(Buffer, 0, total);
    return count;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    return TRUE;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    return 0;
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    return TRUE;
}

static cmsIOHANDLER* create_dummy_io_handler(void) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    memset(io, 0, sizeof(cmsIOHANDLER));
    io->ContextID = g_context;
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    io->stream = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);
    
    return io;
}

static void cleanup_io_handler(cmsIOHANDLER* io) {
    if (io) {
        if (io->Close) {
            io->Close(io);
        }
        free(io);
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    if (g_context == NULL) {
        g_context = cmsCreateContext(NULL, NULL);
        if (!g_context) return 0;
    }

    cmsIOHANDLER* io = create_dummy_io_handler();
    if (!io) return 0;

    // Fuzz _cmsReadUInt32Number
    cmsUInt32Number val32 = 0;
    _cmsReadUInt32Number(io, &val32);
    _cmsReadUInt32Number(io, NULL);

    // Fuzz _cmsReadUInt16Array
    cmsUInt16Number arr16[10];
    cmsUInt32Number count16 = (Size > 0) ? (Data[0] % 10) + 1 : 1;
    _cmsReadUInt16Array(io, count16, arr16);
    _cmsReadUInt16Array(io, count16, NULL);

    // Fuzz _cmsWriteAlignment
    _cmsWriteAlignment(io);

    // Fuzz _cmsWriteUInt32Number
    cmsUInt32Number write_val = (Size > 0) ? Data[0] : 0;
    _cmsWriteUInt32Number(io, write_val);

    // Fuzz _cmsReadAlignment
    _cmsReadAlignment(io);

    // Fuzz cmsCloseIOhandler
    cmsCloseIOhandler(io);
    io = NULL;

    return 0;
}