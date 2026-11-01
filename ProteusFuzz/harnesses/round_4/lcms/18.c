// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext g_context = NULL;

static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer,
                                  cmsUInt32Number size, cmsUInt32Number count) {
    if (iohandler == NULL || Buffer == NULL) return 0;
    cmsUInt32Number total = size * count;
    if (total == 0) return 0;
    
    // Simulate reading from a static buffer or return dummy data
    // For fuzzing, we just fill with zeros to ensure safety
    memset(Buffer, 0, total);
    return total;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    if (iohandler == NULL) return FALSE;
    // Simulate successful seek
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

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size,
                           const void* Buffer) {
    if (iohandler == NULL) return FALSE;
    // Simulate successful write
    return TRUE;
}

static cmsIOHANDLER* create_dummy_io_handler(cmsContext ctx) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (io == NULL) return NULL;

    memset(io, 0, sizeof(cmsIOHANDLER));
    io->stream = NULL;
    io->ContextID = ctx;
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    io->PhysicalFile[0] = '\0';

    return io;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize context if not already done
    if (g_context == NULL) {
        g_context = cmsCreateContext(NULL, NULL);
        if (g_context == NULL) return 0;
    }

    // Create IO handler
    cmsIOHANDLER* io = create_dummy_io_handler(g_context);
    if (io == NULL) return 0;

    // Fuzz _cmsWriteAlignment
    _cmsWriteAlignment(io);

    // Fuzz _cmsWrite15Fixed16Number
    cmsFloat64Number val64 = 0.0;
    if (Size >= 8) {
        memcpy(&val64, Data, 8);
        // Clamp to avoid extreme values that might cause issues
        if (isinf(val64) || isnan(val64)) val64 = 1.0;
        _cmsWrite15Fixed16Number(io, val64);
    }

    // Fuzz _cmsRead15Fixed16Number
    cmsFloat64Number readVal64;
    _cmsRead15Fixed16Number(io, &readVal64);
    _cmsRead15Fixed16Number(io, NULL); // Test with NULL output

    // Fuzz _cmsReadAlignment
    _cmsReadAlignment(io);

    // Fuzz _cmsReadFloat32Number
    cmsFloat32Number readVal32;
    _cmsReadFloat32Number(io, &readVal32);
    _cmsReadFloat32Number(io, NULL); // Test with NULL output

    // Cleanup
    cmsCloseIOhandler(io);
    free(io);

    return 0;
}