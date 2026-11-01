// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2_plugin.h"

// Helper to create a minimal valid IO handler for testing
static cmsContext create_dummy_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void destroy_dummy_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

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

static cmsIOHANDLER* create_dummy_io_handler(void) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;

    memset(io, 0, sizeof(cmsIOHANDLER));
    io->ContextID = create_dummy_context();
    io->stream = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    
    return io;
}

static void destroy_dummy_io_handler(cmsIOHANDLER* io) {
    if (io) {
        if (io->ContextID) {
            cmsDeleteContext(io->ContextID);
        }
        free(io);
    }
}

int LLVMFuzzerTestOneInput_183(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Test 1: _cmsDoubleTo8Fixed8
    cmsFloat64Number val_double = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&val_double, Data, sizeof(double));
    } else {
        val_double = (double)(Data[0] / 255.0);
    }
    
    cmsUInt16Number res1 = _cmsDoubleTo8Fixed8(val_double);
    (void)res1;

    cmsUInt16Number res1_neg = _cmsDoubleTo8Fixed8(-val_double);
    (void)res1_neg;
    
    cmsUInt16Number res1_large = _cmsDoubleTo8Fixed8(val_double * 1000000.0);
    (void)res1_large;

    // Test 2: _cms15Fixed16toDouble
    cmsS15Fixed16Number fix32 = 0;
    if (Size >= sizeof(cmsS15Fixed16Number)) {
        memcpy(&fix32, Data, sizeof(cmsS15Fixed16Number));
    } else {
        fix32 = (cmsS15Fixed16Number)Data[0];
    }

    cmsFloat64Number res2 = _cms15Fixed16toDouble(fix32);
    (void)res2;

    // Test 3: _cmsDoubleTo15Fixed16
    cmsS15Fixed16Number res3 = _cmsDoubleTo15Fixed16(val_double);
    (void)res3;

    // Test 4: _cms8Fixed8toDouble
    cmsUInt16Number fixed8 = 0;
    if (Size >= sizeof(cmsUInt16Number)) {
        memcpy(&fixed8, Data, sizeof(cmsUInt16Number));
    } else {
        fixed8 = (cmsUInt16Number)Data[0];
    }

    cmsFloat64Number res4 = _cms8Fixed8toDouble(fixed8);
    (void)res4;

    // Test 5 & 6: IO functions
    // Create a valid dummy IO handler to avoid NULL pointer assertions
    cmsIOHANDLER* dummy_io = create_dummy_io_handler();
    
    if (dummy_io) {
        cmsFloat64Number dummy_val = 0.0;
        cmsBool res5 = _cmsRead15Fixed16Number(dummy_io, &dummy_val);
        (void)res5;

        cmsBool res6 = _cmsWrite15Fixed16Number(dummy_io, dummy_val);
        (void)res6;
        
        destroy_dummy_io_handler(dummy_io);
    }

    return 0;
}