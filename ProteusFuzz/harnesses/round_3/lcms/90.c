// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f || !Buffer) return 0;
    cmsUInt32Number total = size * count;
    return fread(Buffer, 1, total, f);
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f) return FALSE;
    return (fseek(f, offset, SEEK_SET) == 0) ? TRUE : FALSE;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f) return FALSE;
    fclose(f);
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f) return 0;
    return (cmsUInt32Number)ftell(f);
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f || !Buffer) return FALSE;
    return (fwrite(Buffer, 1, size, f) == size) ? TRUE : FALSE;
}

static cmsIOHANDLER* create_dummy_iohandler(const uint8_t *Data, size_t Size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;

    memset(io, 0, sizeof(cmsIOHANDLER));
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    io->ContextID = cmsCreateContext(NULL, 0);
    io->UsedSpace = 0;
    io->ReportedSize = Size;

    // Write fuzz data to a temporary file
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) {
        free(io);
        return NULL;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Open the file for reading
    f = fopen("./dummy_file", "rb");
    if (!f) {
        free(io);
        return NULL;
    }
    io->stream = f;

    return io;
}

static cmsIOHANDLER* create_dummy_iohandler_write(const uint8_t *Data, size_t Size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;

    memset(io, 0, sizeof(cmsIOHANDLER));
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    io->ContextID = cmsCreateContext(NULL, 0);
    io->UsedSpace = 0;
    io->ReportedSize = Size;

    // Create a file for writing
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) {
        free(io);
        return NULL;
    }
    io->stream = f;

    return io;
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test _cmsWriteUInt16Number
    {
        cmsIOHANDLER* io = create_dummy_iohandler_write(Data, Size);
        if (io) {
            cmsUInt16Number val = (Size > 0) ? (cmsUInt16Number)Data[0] : 0;
            _cmsWriteUInt16Number(io, val);
            cmsCloseIOhandler(io);
            free(io);
        }
    }

    // Test _cmsReadUInt16Number
    {
        cmsIOHANDLER* io = create_dummy_iohandler(Data, Size);
        if (io) {
            cmsUInt16Number val;
            _cmsReadUInt16Number(io, &val);
            cmsCloseIOhandler(io);
            free(io);
        }
    }

    // Test _cmsWriteUInt16Array
    {
        cmsIOHANDLER* io = create_dummy_iohandler_write(Data, Size);
        if (io) {
            cmsUInt32Number count = (Size > 2) ? (cmsUInt32Number)((Data[0] % 10) + 1) : 1;
            cmsUInt16Number* arr = (cmsUInt16Number*)malloc(count * sizeof(cmsUInt16Number));
            if (arr) {
                for (cmsUInt32Number i = 0; i < count; i++) {
                    arr[i] = (Size > i + 1) ? (cmsUInt16Number)Data[i + 1] : 0;
                }
                _cmsWriteUInt16Array(io, count, arr);
                free(arr);
            }
            cmsCloseIOhandler(io);
            free(io);
        }
    }

    // Test _cmsReadUInt16Array
    {
        cmsIOHANDLER* io = create_dummy_iohandler(Data, Size);
        if (io) {
            cmsUInt32Number count = (Size > 2) ? (cmsUInt32Number)((Data[0] % 10) + 1) : 1;
            cmsUInt16Number* arr = (cmsUInt16Number*)malloc(count * sizeof(cmsUInt16Number));
            if (arr) {
                _cmsReadUInt16Array(io, count, arr);
                free(arr);
            }
            // Test with NULL array (skip data)
            _cmsReadUInt16Array(io, count, NULL);
            cmsCloseIOhandler(io);
            free(io);
        }
    }

    // Test _cmsReadAlignment
    {
        cmsIOHANDLER* io = create_dummy_iohandler(Data, Size);
        if (io) {
            _cmsReadAlignment(io);
            cmsCloseIOhandler(io);
            free(io);
        }
    }

    // Cleanup context if created
    cmsDeleteContext(NULL);

    // Remove dummy file
    remove("./dummy_file");

    return 0;
}