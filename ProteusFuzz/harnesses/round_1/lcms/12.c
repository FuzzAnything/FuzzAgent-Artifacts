// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext g_context = NULL;

static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (iohandler == NULL || Buffer == NULL) return 0;
    size_t total = size * count;
    if (total == 0) return 0;
    memset(Buffer, 0, total);
    return count;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    if (iohandler == NULL) return FALSE;
    return TRUE;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    if (iohandler == NULL) return FALSE;
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    if (iohandler == NULL) return 0;
    return 0;
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    if (iohandler == NULL) return FALSE;
    return TRUE;
}

static cmsIOHANDLER* create_dummy_io_handler(void) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (io == NULL) return NULL;
    
    memset(io, 0, sizeof(cmsIOHANDLER));
    io->ContextID = g_context;
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    io->stream = NULL;
    memset(io->PhysicalFile, 0, sizeof(io->PhysicalFile));
    
    return io;
}

static void cleanup_io_handler(cmsIOHANDLER* io) {
    if (io != NULL) {
        cmsCloseIOhandler(io);
        free(io);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (g_context == NULL) {
        g_context = cmsCreateContext(NULL, NULL);
        if (g_context == NULL) return 0;
    }

    cmsIOHANDLER* io = create_dummy_io_handler();
    if (io == NULL) return 0;

    cmsUInt32Number val32 = 0;
    _cmsReadUInt32Number(io, &val32);
    _cmsReadUInt32Number(io, NULL);

    cmsCIEXYZ xyz = {0.0, 0.0, 0.0};
    _cmsReadXYZNumber(io, &xyz);
    _cmsReadXYZNumber(io, NULL);

    _cmsWriteAlignment(io);

    _cmsWriteXYZNumber(io, &xyz);
    // Removed _cmsWriteXYZNumber(io, NULL) to avoid assertion failure

    _cmsReadAlignment(io);

    cleanup_io_handler(io);

    return 0;
}