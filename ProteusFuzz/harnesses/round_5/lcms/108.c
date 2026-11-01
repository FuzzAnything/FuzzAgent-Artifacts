// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext g_context = NULL;

static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    (void)iohandler;
    (void)Buffer;
    (void)size;
    (void)count;
    return 0;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    (void)iohandler;
    (void)offset;
    return TRUE;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    (void)iohandler;
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    (void)iohandler;
    return 0;
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    (void)iohandler;
    (void)size;
    (void)Buffer;
    return TRUE;
}

static cmsIOHANDLER* create_dummy_handler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;

    memset(io, 0, sizeof(cmsIOHANDLER));
    io->stream = NULL;
    io->ContextID = g_context;
    io->UsedSpace = 0;
    io->ReportedSize = (cmsUInt32Number)size;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);
    
    if (size > 0) {
        io->stream = malloc(size);
        if (io->stream) {
            memcpy(io->stream, data, size);
        }
    } else {
        io->stream = malloc(1);
    }

    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;

    return io;
}

static void free_dummy_handler(cmsIOHANDLER* io) {
    if (io) {
        if (io->stream) {
            free(io->stream);
        }
        free(io);
    }
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    if (g_context == NULL) {
        g_context = cmsCreateContext(NULL, NULL);
        if (g_context == NULL) return 0;
    }

    cmsIOHANDLER* io = create_dummy_handler(Data, Size);
    if (!io) return 0;

    cmsFloat32Number float_val = 0.0f;
    cmsUInt64Number uint64_val = 0;
    
    if (Size >= sizeof(float)) {
        memcpy(&float_val, Data, sizeof(float));
    } else {
        float_val = (float)(Data[0] + 0.1);
    }

    if (Size >= sizeof(uint64_t)) {
        memcpy(&uint64_val, Data, sizeof(uint64_t));
    } else {
        uint64_val = (uint64_t)Data[0];
        if (Size > 1) {
            uint64_val |= ((uint64_t)Data[1] << 8);
        }
    }

    // 1. Test _cmsWriteFloat32Number
    _cmsWriteFloat32Number(io, float_val);

    // 2. Test _cmsWriteUInt64Number
    _cmsWriteUInt64Number(io, &uint64_val);

    // 3. Test _cmsWriteAlignment
    _cmsWriteAlignment(io);

    // 4. Test _cmsReadFloat32Number
    cmsFloat32Number read_float;
    _cmsReadFloat32Number(io, &read_float);

    // 5. Test _cmsReadAlignment
    _cmsReadAlignment(io);

    // 6. Test cmsCloseIOhandler
    cmsBool close_result = cmsCloseIOhandler(io);
    (void)close_result;

    // Cleanup
    free_dummy_handler(io);

    return 0;
}