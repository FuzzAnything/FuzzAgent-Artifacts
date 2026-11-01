// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Dummy callback implementations
static cmsBool dummy_Close(cmsIOHANDLER *h) {
    if (h && h->stream) {
        FILE *f = (FILE*)h->stream;
        fclose(f);
        h->stream = NULL;
    }
    return TRUE;
}

static cmsBool dummy_Write(cmsIOHANDLER *h, cmsUInt32Number size, const void *buf) {
    if (!h || !h->stream) return FALSE;
    FILE *f = (FILE*)h->stream;
    if (fwrite(buf, 1, size, f) != size) return FALSE;
    return TRUE;
}

static cmsUInt32Number dummy_Read(cmsIOHANDLER *h, void *buf, cmsUInt32Number size, cmsUInt32Number count) {
    if (!h || !h->stream) return 0;
    FILE *f = (FILE*)h->stream;
    return fread(buf, size, count, f);
}

static cmsBool dummy_Seek(cmsIOHANDLER *h, cmsUInt32Number offset) {
    if (!h || !h->stream) return FALSE;
    FILE *f = (FILE*)h->stream;
    return fseek(f, offset, SEEK_SET) == 0;
}

static cmsUInt32Number dummy_Tell(cmsIOHANDLER *h) {
    if (!h || !h->stream) return 0;
    FILE *f = (FILE*)h->stream;
    long pos = ftell(f);
    return (pos < 0) ? 0 : (cmsUInt32Number)pos;
}

static cmsIOHANDLER* create_dummy_io_handler(const uint8_t *data, size_t size) {
    cmsIOHANDLER *io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;

    memset(io, 0, sizeof(cmsIOHANDLER));

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        free(io);
        return NULL;
    }
    io->ContextID = ctx;

    // Create a dummy file to use as stream
    FILE *f = fopen("./dummy_file", "w+b");
    if (!f) {
        cmsDeleteContext(ctx);
        free(io);
        return NULL;
    }

    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    fclose(f);

    // Reopen for reading/writing
    f = fopen("./dummy_file", "r+b");
    if (!f) {
        cmsDeleteContext(ctx);
        free(io);
        return NULL;
    }

    io->stream = f;
    io->UsedSpace = 0;
    io->ReportedSize = size;
    snprintf(io->PhysicalFile, cmsMAX_PATH, "./dummy_file");

    // Assign dummy callbacks
    io->Read = dummy_Read;
    io->Seek = dummy_Seek;
    io->Close = dummy_Close;
    io->Tell = dummy_Tell;
    io->Write = dummy_Write;

    return io;
}

static void cleanup_io_handler(cmsIOHANDLER *io) {
    if (!io) return;

    if (io->stream) {
        FILE *f = (FILE*)io->stream;
        fclose(f);
    }
    if (io->ContextID) {
        cmsDeleteContext(io->ContextID);
    }
    free(io);
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy IO handler
    cmsIOHANDLER *io = create_dummy_io_handler(Data, Size);
    if (!io) return 0;

    cmsBool result;
    cmsFloat32Number fval;
    cmsUInt64Number u64val;

    // Test 1: cmsCloseIOhandler
    result = cmsCloseIOhandler(io);
    // After close, io->stream is NULL. We need to reopen or recreate for further tests.
    // For simplicity, we will just cleanup and recreate if needed, or skip further IO if stream is null.
    // However, the callbacks reset the stream to NULL. Let's just cleanup and recreate for the next batch.
    cleanup_io_handler(io);
    io = create_dummy_io_handler(Data, Size);
    if (!io) return 0;

    // Test 2: _cmsWriteFloat32Number
    // Extract float from data if possible, else use 0.0f
    if (Size >= sizeof(float)) {
        memcpy(&fval, Data, sizeof(float));
        result = _cmsWriteFloat32Number(io, fval);
        (void)result;
    } else {
        result = _cmsWriteFloat32Number(io, 0.0f);
        (void)result;
    }

    // Test 3: _cmsReadFloat32Number
    // Seek to beginning
    io->Seek(io, 0);
    result = _cmsReadFloat32Number(io, &fval);
    (void)result;

    // Test 4: _cmsWriteUInt64Number
    if (Size >= sizeof(cmsUInt64Number)) {
        memcpy(&u64val, Data, sizeof(cmsUInt64Number));
        result = _cmsWriteUInt64Number(io, &u64val);
        (void)result;
    } else {
        u64val = 0;
        result = _cmsWriteUInt64Number(io, &u64val);
        (void)result;
    }

    // Test 5: _cmsReadAlignment
    io->Seek(io, 0);
    result = _cmsReadAlignment(io);
    (void)result;

    // Test 6: _cmsWriteAlignment
    result = _cmsWriteAlignment(io);
    (void)result;

    // Cleanup
    cleanup_io_handler(io);

    // Remove dummy file
    remove("./dummy_file");

    return 0;
}