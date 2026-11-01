// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Read function must return cmsUInt32Number (unsigned int), not cmsBool
static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (iohandler == NULL || Buffer == NULL) return 0;
    FILE* f = (FILE*)iohandler->stream;
    if (f == NULL) return 0;
    size_t total = size * count;
    size_t read = fread(Buffer, 1, total, f);
    return (cmsUInt32Number)read;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    if (iohandler == NULL) return FALSE;
    FILE* f = (FILE*)iohandler->stream;
    if (f == NULL) return FALSE;
    return (fseek(f, offset, SEEK_SET) == 0);
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    if (iohandler == NULL) return FALSE;
    FILE* f = (FILE*)iohandler->stream;
    if (f == NULL) return FALSE;
    fclose(f);
    iohandler->stream = NULL;
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    if (iohandler == NULL) return 0;
    FILE* f = (FILE*)iohandler->stream;
    if (f == NULL) return 0;
    long pos = ftell(f);
    if (pos < 0) return 0;
    return (cmsUInt32Number)pos;
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    if (iohandler == NULL || Buffer == NULL) return FALSE;
    FILE* f = (FILE*)iohandler->stream;
    if (f == NULL) return FALSE;
    size_t written = fwrite(Buffer, 1, size, f);
    return (written == size);
}

static cmsIOHANDLER* create_dummy_iohandler(const uint8_t *data, size_t size, int mode) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (io == NULL) return NULL;
    memset(io, 0, sizeof(cmsIOHANDLER));

    if (mode == 0) {
        // Read mode: create a temporary file
        FILE* f = fopen("./dummy_file", "wb");
        if (f == NULL) {
            free(io);
            return NULL;
        }
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
        f = fopen("./dummy_file", "rb");
        if (f == NULL) {
            free(io);
            return NULL;
        }
        io->stream = f;
        io->Read = dummy_read;
        io->Seek = dummy_seek;
        io->Close = dummy_close;
        io->Tell = dummy_tell;
        io->Write = dummy_write;
    } else {
        // Write mode
        FILE* f = fopen("./dummy_file", "wb");
        if (f == NULL) {
            free(io);
            return NULL;
        }
        io->stream = f;
        io->Read = dummy_read;
        io->Seek = dummy_seek;
        io->Close = dummy_close;
        io->Tell = dummy_tell;
        io->Write = dummy_write;
    }
    io->ContextID = cmsCreateContext(NULL, NULL);
    io->UsedSpace = 0;
    io->ReportedSize = size;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);
    return io;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create context for safety
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Test 1: _cmsReadUInt64Number with read-only file
    cmsIOHANDLER* io_read = create_dummy_iohandler(Data, Size, 0);
    if (io_read != NULL) {
        cmsUInt64Number val64;
        _cmsReadUInt64Number(io_read, &val64);
        _cmsReadUInt64Number(io_read, NULL); // Discard data
        cmsCloseIOhandler(io_read);
        free(io_read);
    }

    // Test 2: _cmsWriteUInt64Number with write-only file
    cmsIOHANDLER* io_write = create_dummy_iohandler(Data, Size, 1);
    if (io_write != NULL) {
        cmsUInt64Number write_val = 0x123456789ABCDEF0ULL;
        _cmsWriteUInt64Number(io_write, &write_val);
        cmsCloseIOhandler(io_write);
        free(io_write);
    }

    // Test 3: _cmsWriteFloat32Number
    cmsIOHANDLER* io_write2 = create_dummy_iohandler(Data, Size, 1);
    if (io_write2 != NULL) {
        cmsFloat32Number fval = 3.14159f;
        _cmsWriteFloat32Number(io_write2, fval);
        cmsCloseIOhandler(io_write2);
        free(io_write2);
    }

    // Test 4: _cmsReadAlignment
    cmsIOHANDLER* io_read2 = create_dummy_iohandler(Data, Size, 0);
    if (io_read2 != NULL) {
        _cmsReadAlignment(io_read2);
        cmsCloseIOhandler(io_read2);
        free(io_read2);
    }

    // Test 5: _cmsWriteAlignment
    cmsIOHANDLER* io_write3 = create_dummy_iohandler(Data, Size, 1);
    if (io_write3 != NULL) {
        // Write some data to cause misalignment
        uint8_t dummy = 0x42;
        io_write3->Write(io_write3, 1, &dummy);
        _cmsWriteAlignment(io_write3);
        cmsCloseIOhandler(io_write3);
        free(io_write3);
    }

    cmsDeleteContext(ctx);
    return 0;
}