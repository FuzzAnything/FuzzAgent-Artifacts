// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (!iohandler || !Buffer) return 0;
    return 1;
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    return TRUE;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    return 0;
}

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    return TRUE;
}

static cmsIOHANDLER* create_dummy_io_handler(const uint8_t *Data, size_t Size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;

    memset(io, 0, sizeof(cmsIOHANDLER));
    io->stream = NULL;
    io->ContextID = cmsCreateContext(NULL, NULL);
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

static void cleanup_io_handler(cmsIOHANDLER* io) {
    if (io) {
        if (io->ContextID) {
            cmsDeleteContext(io->ContextID);
        }
        free(io);
    }
}

int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsIOHANDLER* io = create_dummy_io_handler(Data, Size);
    if (!io) return 0;

    cmsBool result;
    cmsUInt32Number val32;
    cmsUInt8Number val8;

    // Test cmsCloseIOhandler
    cmsIOHANDLER* io_close = create_dummy_io_handler(Data, Size);
    if (io_close) {
        result = cmsCloseIOhandler(io_close);
        (void)result;
        cleanup_io_handler(io_close);
    }

    // Test _cmsWriteUInt32Number
    result = _cmsWriteUInt32Number(io, (Size > 0) ? Data[0] : 0);
    (void)result;

    // Test _cmsWriteUInt8Number
    result = _cmsWriteUInt8Number(io, (Size > 0) ? Data[0] : 0);
    (void)result;

    // Test _cmsReadUInt32Number
    result = _cmsReadUInt32Number(io, &val32);
    (void)result;

    // Test _cmsReadAlignment
    result = _cmsReadAlignment(io);
    (void)result;

    // Test _cmsReadUInt8Number
    result = _cmsReadUInt8Number(io, &val8);
    (void)result;

    // Test with NULL pointer for read functions (should return FALSE)
    result = _cmsReadUInt32Number(io, NULL);
    (void)result;
    result = _cmsReadUInt8Number(io, NULL);
    (void)result;

    // Cleanup
    cleanup_io_handler(io);

    return 0;
}