// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext gContext = NULL;

static void cleanup(void) {
    if (gContext != NULL) {
        cmsDeleteContext(gContext);
        gContext = NULL;
    }
}

static cmsIOHANDLER* create_io_handler(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return NULL;
    }

    // Write data to a temporary file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp == NULL) return NULL;

    if (fwrite(Data, 1, Size, fp) != Size) {
        fclose(fp);
        return NULL;
    }
    fclose(fp);

    // Open the file as an IO handler using the public API cmsOpenIOhandlerFromFile
    // Requires ContextID, FileName, and AccessMode
    cmsIOHANDLER *io = cmsOpenIOhandlerFromFile(gContext, "./dummy_file", "r");
    return io;
}

static void test_write_type_base(cmsIOHANDLER *io) {
    if (io == NULL) return;
    cmsTagTypeSignature sig = (cmsTagTypeSignature)0x61626364; // dummy signature
    _cmsWriteTypeBase(io, sig);
}

static void test_read_xyz_number(cmsIOHANDLER *io) {
    if (io == NULL) return;
    cmsCIEXYZ xyz = {0};
    _cmsReadXYZNumber(io, &xyz);
}

static void test_write_uint8(cmsIOHANDLER *io) {
    if (io == NULL) return;
    cmsUInt8Number val = 0xFF;
    _cmsWriteUInt8Number(io, val);
}

static void test_read_alignment(cmsIOHANDLER *io) {
    if (io == NULL) return;
    _cmsReadAlignment(io);
}

static void test_write_alignment(cmsIOHANDLER *io) {
    if (io == NULL) return;
    _cmsWriteAlignment(io);
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Cleanup from previous runs
    cleanup();

    // Initialize context
    gContext = cmsCreateContext(NULL, NULL);
    if (gContext == NULL) {
        return 0;
    }

    // Create IO handler from input data
    cmsIOHANDLER *io = create_io_handler(Data, Size);
    if (io == NULL) {
        cleanup();
        return 0;
    }

    // Fuzz various API functions
    // 1. Write Type Base
    test_write_type_base(io);

    // 2. Read XYZ Number
    test_read_xyz_number(io);

    // 3. Write UInt8 Number
    test_write_uint8(io);

    // 4. Read Alignment
    test_read_alignment(io);

    // 5. Write Alignment
    test_write_alignment(io);

    // 6. Close IO handler
    cmsCloseIOhandler(io);

    // Cleanup
    cleanup();
    return 0;
}