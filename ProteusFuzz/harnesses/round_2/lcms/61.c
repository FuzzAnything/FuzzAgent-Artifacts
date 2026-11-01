// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsIOHANDLER *io = NULL;
    cmsBool result = FALSE;

    // Test cmsOpenIOhandlerFromFile with various modes and file states
    write_dummy_file(Data, Size);

    // Try opening in read mode
    io = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
    if (io) {
        // Test _cmsIOPrintf
        result = _cmsIOPrintf(io, "Test: %d, %s, %.2f", 123, "hello", 3.14);
        (void)result;

        // Test _cmsWriteAlignment
        result = _cmsWriteAlignment(io);
        (void)result;

        // Test _cmsWriteUInt16Array
        cmsUInt16Number arr[4] = {0, 1, 2, 3};
        result = _cmsWriteUInt16Array(io, 4, arr);
        (void)result;

        cmsCloseIOhandler(io);
        io = NULL;
    }

    // Try opening in write mode
    io = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "w");
    if (io) {
        result = _cmsIOPrintf(io, "Write: %d", 456);
        (void)result;
        result = _cmsWriteAlignment(io);
        (void)result;
        cmsCloseIOhandler(io);
        io = NULL;
    }

    // Test with invalid mode
    io = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "rw");
    if (io) {
        cmsCloseIOhandler(io);
        io = NULL;
    }

    // Avoid calling cmsIT8SaveToFile with NULL as it causes a crash inside the library
    // The function expects a valid IT8 handle and will dereference it immediately.
    // Since we cannot create a valid IT8 handle without more complex setup, we skip this call.

    // Cleanup
    cmsDeleteContext(ctx);
    return 0;
}