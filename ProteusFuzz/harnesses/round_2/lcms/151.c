// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext gCtx = NULL;

static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer,
                                  cmsUInt32Number size, cmsUInt32Number count) {
    cmsUInt32Number total = size * count;
    if (total == 0) return 0;
    memset(Buffer, 0xAA, total);
    return total;
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

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size,
                           const void* Buffer) {
    return TRUE;
}

static cmsIOHANDLER* create_dummy_handler(cmsContext ctx) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;

    memset(io, 0, sizeof(cmsIOHANDLER));
    io->stream = NULL;
    io->ContextID = ctx;
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);

    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;

    return io;
}

static void cleanup_handler(cmsIOHANDLER* io) {
    if (io) {
        if (io->Close) {
            io->Close(io);
        }
        free(io);
    }
}

int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    if (gCtx == NULL) {
        gCtx = cmsCreateContext(NULL, NULL);
        if (gCtx == NULL) return 0;
    }

    cmsIOHANDLER* io = create_dummy_handler(gCtx);
    if (!io) return 0;

    cmsBool res;
    cmsUInt64Number val64 = 0;
    cmsUInt8Number val8 = 0;

    // Fuzz _cmsReadUInt64Number
    res = _cmsReadUInt64Number(io, &val64);
    (void)res;
    res = _cmsReadUInt64Number(io, NULL);
    (void)res;

    // Fuzz _cmsWriteAlignment
    res = _cmsWriteAlignment(io);
    (void)res;

    // Fuzz _cmsWriteUInt64Number
    if (Size >= 8) {
        val64 = 0;
        for (int i = 0; i < 8; i++) {
            val64 = (val64 << 8) | Data[i];
        }
    } else {
        val64 = (cmsUInt64Number)Size;
    }
    res = _cmsWriteUInt64Number(io, &val64);
    (void)res;

    // Fuzz _cmsReadAlignment
    res = _cmsReadAlignment(io);
    (void)res;

    // Fuzz _cmsWriteUInt8Number
    if (Size > 0) {
        val8 = Data[0];
    } else {
        val8 = 0;
    }
    res = _cmsWriteUInt8Number(io, val8);
    (void)res;

    // Cleanup
    cleanup_handler(io);

    return 0;
}