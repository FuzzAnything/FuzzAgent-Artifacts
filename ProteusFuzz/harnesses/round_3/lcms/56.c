// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "lcms2_plugin.h"

/*
 * Static helper to create a minimal dummy IO handler for fuzzing.
 * This handler uses a memory buffer to simulate file I/O.
 */
static void* dummy_alloc(cmsContext id, size_t size) {
    return malloc(size);
}

static void dummy_free(cmsContext id, void* ptr) {
    if (ptr) free(ptr);
}

static cmsContext create_dummy_context(void) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx) {
        // cmsSetContextMemoryHandler is not available in all builds or might be internal.
        // We rely on the default context allocation which is usually safe for fuzzing
        // unless specific memory exhaustion tests are needed.
        // If the build requires explicit memory handler setup, it would be defined in lcms2.h
        // but the error suggests it's missing. We skip it to ensure compilation.
    }
    return ctx;
}

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    // Simulate a successful write by just ignoring the data
    return TRUE;
}

static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    // Simulate reading from the buffer
    if (!iohandler->stream || !Buffer) return 0;
    uint8_t* data = (uint8_t*)iohandler->stream;
    uint8_t* buf = (uint8_t*)Buffer;
    cmsUInt32Number total = size * count;
    cmsUInt32Number available = iohandler->ReportedSize - iohandler->UsedSpace;
    
    if (total > available) total = available;
    if (total == 0) return 0;

    memcpy(buf, data + iohandler->UsedSpace, total);
    iohandler->UsedSpace += total;
    return total / size;
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (iohandler->stream) {
        iohandler->UsedSpace = offset;
        return TRUE;
    }
    return FALSE;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    // No-op for memory buffer
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    return iohandler->UsedSpace;
}

static cmsIOHANDLER* setup_io_handler(cmsContext ctx, const uint8_t* data, size_t size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;

    memset(io, 0, sizeof(cmsIOHANDLER));
    io->ContextID = ctx;
    io->stream = (void*)data;
    io->UsedSpace = 0;
    io->ReportedSize = (uint32_t)size;
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;

    return io;
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = create_dummy_context();
    if (!ctx) return 0;

    // We need at least 8 bytes to extract two doubles (8 bytes each) for testing conversion functions
    // If the input is too small, we pad it or just use what we have for specific tests
    cmsFloat64Number val1 = 0.0;
    cmsFloat64Number val2 = 0.0;
    cmsFloat64Number val3 = 0.0;
    
    // Extract doubles from input if possible
    if (Size >= 8) {
        memcpy(&val1, Data, sizeof(double));
    }
    if (Size >= 16) {
        memcpy(&val2, Data + 8, sizeof(double));
    }
    if (Size >= 24) {
        memcpy(&val3, Data + 16, sizeof(double));
    }

    // Test 1: _cmsDoubleTo15Fixed16
    // Converts double to 15.16 fixed-point
    cmsS15Fixed16Number fixed16_1 = _cmsDoubleTo15Fixed16(val1);
    cmsS15Fixed16Number fixed16_2 = _cmsDoubleTo15Fixed16(val2);
    cmsS15Fixed16Number fixed16_3 = _cmsDoubleTo15Fixed16(val3);

    // Test 2: _cmsDoubleTo8Fixed8
    // Converts double to 8.8 fixed-point
    cmsUInt16Number fixed8_1 = _cmsDoubleTo8Fixed8(val1);
    cmsUInt16Number fixed8_2 = _cmsDoubleTo8Fixed8(val2);
    cmsUInt16Number fixed8_3 = _cmsDoubleTo8Fixed8(val3);

    // Test 3: _cms15Fixed16toDouble
    // Converts 15.16 fixed-point back to double
    cmsFloat64Number double_from_16_1 = _cms15Fixed16toDouble(fixed16_1);
    cmsFloat64Number double_from_16_2 = _cms15Fixed16toDouble(fixed16_2);
    cmsFloat64Number double_from_16_3 = _cms15Fixed16toDouble(fixed16_3);

    // Test 4: _cms8Fixed8toDouble
    // Converts 8.8 fixed-point back to double
    cmsFloat64Number double_from_8_1 = _cms8Fixed8toDouble(fixed8_1);
    cmsFloat64Number double_from_8_2 = _cms8Fixed8toDouble(fixed8_2);
    cmsFloat64Number double_from_8_3 = _cms8Fixed8toDouble(fixed8_3);

    // Test 5: _cmsWrite15Fixed16Number
    // Requires a valid IO handler. We create one from the input data buffer.
    cmsIOHANDLER* io = setup_io_handler(ctx, Data, Size);
    if (io) {
        cmsBool write_res1 = _cmsWrite15Fixed16Number(io, val1);
        cmsBool write_res2 = _cmsWrite15Fixed16Number(io, val2);
        cmsBool write_res3 = _cmsWrite15Fixed16Number(io, val3);
        
        // Reset IO for read test
        io->UsedSpace = 0;
        
        // Test 6: _cmsRead15Fixed16Number
        // We try to read from the input buffer directly.
        cmsFloat64Number read_val = 0.0;
        cmsBool read_res = _cmsRead15Fixed16Number(io, &read_val);
        
        // Test with NULL pointer for output
        cmsBool read_res_null = _cmsRead15Fixed16Number(io, NULL);

        free(io);
    }

    // Cleanup context
    cmsDeleteContext(ctx);

    return 0;
}