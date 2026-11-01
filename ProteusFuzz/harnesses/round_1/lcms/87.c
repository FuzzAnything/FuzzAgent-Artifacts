// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsContext g_context = NULL;

static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer,
                                  cmsUInt32Number size, cmsUInt32Number count) {
    if (iohandler == NULL || Buffer == NULL) return 0;
    cmsUInt32Number total = size * count;
    if (total == 0) return 0;
    
    // Simulate reading from a static buffer or return partial data based on internal state
    static unsigned char dummy_data[256] = {0};
    static int read_idx = 0;
    
    cmsUInt32Number to_read = (total < 256) ? total : 256;
    if (read_idx + to_read > 256) {
        to_read = 256 - read_idx;
    }
    
    if (to_read == 0) return 0;
    
    memcpy(Buffer, dummy_data + read_idx, to_read);
    read_idx += to_read;
    if (read_idx >= 256) read_idx = 0;
    
    return to_read / size;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    // Reset read index based on offset for simulation
    if (iohandler != NULL) {
        static int read_idx = 0;
        read_idx = (offset % 256);
    }
    return TRUE;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    static int read_idx = 0;
    return read_idx;
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size,
                           const void* Buffer) {
    if (iohandler == NULL || Buffer == NULL) return FALSE;
    // Simulate successful write
    return TRUE;
}

static cmsIOHANDLER* create_dummy_handler(cmsContext ctx, const uint8_t *Data, size_t Size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)_cmsCalloc(ctx, 1, sizeof(cmsIOHANDLER));
    if (io == NULL) return NULL;

    io->ContextID = ctx;
    io->UsedSpace = 0;
    io->ReportedSize = (Size > 0xFFFFFFFF) ? 0xFFFFFFFF : (cmsUInt32Number)Size;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);
    
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    
    io->stream = (void*)Data; 
    
    return io;
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    if (g_context == NULL) {
        g_context = cmsCreateContext(NULL, NULL);
        if (g_context == NULL) return 0;
    }

    cmsIOHANDLER* io = create_dummy_handler(g_context, Data, Size);
    if (io == NULL) return 0;

    cmsUInt64Number u64_val = 0;
    cmsFloat32Number f32_val = 0.0f;
    cmsBool result;

    // Test _cmsReadUInt64Number
    // Case 1: Normal read
    result = _cmsReadUInt64Number(io, &u64_val);
    (void)result;

    // Case 2: Read with NULL output (data discarded)
    result = _cmsReadUInt64Number(io, NULL);
    (void)result;

    // Test _cmsWriteUInt64Number
    u64_val = 0xDEADBEEFCAFEBABEULL;
    result = _cmsWriteUInt64Number(io, &u64_val);
    (void)result;

    // Test _cmsWriteAlignment
    result = _cmsWriteAlignment(io);
    (void)result;

    // Test _cmsReadAlignment
    result = _cmsReadAlignment(io);
    (void)result;

    // Test _cmsReadFloat32Number
    // Case 1: Normal read
    result = _cmsReadFloat32Number(io, &f32_val);
    (void)result;

    // Case 2: Read with NULL output
    result = _cmsReadFloat32Number(io, NULL);
    (void)result;

    // Cleanup
    cmsCloseIOhandler(io);
    _cmsFree(g_context, io);

    return 0;
}