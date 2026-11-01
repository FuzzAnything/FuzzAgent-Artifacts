// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2_plugin.h"

static cmsContext gCtx = NULL;

static cmsContext init_context(void) {
    if (gCtx == NULL) {
        gCtx = cmsCreateContext(NULL, NULL);
    }
    return gCtx;
}

static void cleanup_context(void) {
    if (gCtx != NULL) {
        cmsDeleteContext(gCtx);
        gCtx = NULL;
    }
}

/* Simple file-based IO handler implementation for testing _cmsRead15Fixed16Number and _cmsWrite15Fixed16Number */
static cmsUInt32Number file_read(struct _cms_io_handler* iohandler, void* Buffer,
                                 cmsUInt32Number size, cmsUInt32Number count) {
    FILE* f = (FILE*)iohandler->stream;
    if (f == NULL) return 0;
    return fread(Buffer, size, count, f);
}

static cmsBool file_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    FILE* f = (FILE*)iohandler->stream;
    if (f == NULL) return FALSE;
    if (fseek(f, offset, SEEK_SET) != 0) return FALSE;
    return TRUE;
}

static cmsBool file_close(struct _cms_io_handler* iohandler) {
    FILE* f = (FILE*)iohandler->stream;
    if (f != NULL) {
        fclose(f);
        iohandler->stream = NULL;
    }
    return TRUE;
}

static cmsUInt32Number file_tell(struct _cms_io_handler* iohandler) {
    FILE* f = (FILE*)iohandler->stream;
    if (f == NULL) return 0;
    long pos = ftell(f);
    if (pos < 0) return 0;
    return (cmsUInt32Number)pos;
}

static cmsBool file_write(struct _cms_io_handler* iohandler, cmsUInt32Number size,
                          const void* Buffer) {
    FILE* f = (FILE*)iohandler->stream;
    if (f == NULL) return FALSE;
    if (fwrite(Buffer, 1, size, f) != size) return FALSE;
    return TRUE;
}

static cmsIOHANDLER* create_file_io_handler(const char* filename, const char* mode, cmsContext ctx) {
    FILE* f = fopen(filename, mode);
    if (f == NULL) return NULL;

    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (io == NULL) {
        fclose(f);
        return NULL;
    }

    memset(io, 0, sizeof(cmsIOHANDLER));
    io->stream = f;
    io->ContextID = ctx;
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    io->Read = file_read;
    io->Seek = file_seek;
    io->Close = file_close;
    io->Tell = file_tell;
    io->Write = file_write;
    strncpy(io->PhysicalFile, filename, cmsMAX_PATH - 1);
    io->PhysicalFile[cmsMAX_PATH - 1] = '\0';

    return io;
}

static cmsFloat64Number safe_read_double(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + 8) return 0.0;
    return *(const cmsFloat64Number*)(Data + offset);
}

static cmsS15Fixed16Number safe_read_s15fixed16(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + 4) return 0;
    return *(const cmsS15Fixed16Number*)(Data + offset);
}

static cmsUInt16Number safe_read_u16(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + 2) return 0;
    return *(const cmsUInt16Number*)(Data + offset);
}

int LLVMFuzzerTestOneInput_187(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = init_context();
    if (ctx == NULL) return 0;

    /* Fuzz _cmsDoubleTo8Fixed8 */
    if (Size >= 8) {
        cmsFloat64Number val1 = safe_read_double(Data, Size, 0);
        cmsUInt16Number res1 = _cmsDoubleTo8Fixed8(val1);
        (void)res1;
    }

    /* Fuzz _cms15Fixed16toDouble */
    if (Size >= 4) {
        cmsS15Fixed16Number fix32 = safe_read_s15fixed16(Data, Size, 0);
        cmsFloat64Number res2 = _cms15Fixed16toDouble(fix32);
        (void)res2;
    }

    /* Fuzz _cmsDoubleTo15Fixed16 */
    if (Size >= 8) {
        cmsFloat64Number val3 = safe_read_double(Data, Size, 0);
        cmsS15Fixed16Number res3 = _cmsDoubleTo15Fixed16(val3);
        (void)res3;
    }

    /* Fuzz _cms8Fixed8toDouble */
    if (Size >= 2) {
        cmsUInt16Number fixed8 = safe_read_u16(Data, Size, 0);
        cmsFloat64Number res4 = _cms8Fixed8toDouble(fixed8);
        (void)res4;
    }

    /* Fuzz _cmsRead15Fixed16Number and _cmsWrite15Fixed16Number */
    if (Size >= 8) {
        cmsFloat64Number write_val = safe_read_double(Data, Size, 0);
        cmsFloat64Number read_val;

        cmsIOHANDLER* io = create_file_io_handler("./dummy_file", "wb+", ctx);
        if (io != NULL) {
            cmsBool write_ok = _cmsWrite15Fixed16Number(io, write_val);
            (void)write_ok;

            /* Seek back to beginning for read */
            if (io->Seek != NULL) {
                io->Seek(io, 0);
            }

            cmsBool read_ok = _cmsRead15Fixed16Number(io, &read_val);
            (void)read_ok;
            (void)read_val;

            /* Cleanup IO handler */
            if (io->Close != NULL) {
                io->Close(io);
            }
            free(io);
        }

        /* Also test with NULL output pointer */
        cmsIOHANDLER* io2 = create_file_io_handler("./dummy_file", "rb", ctx);
        if (io2 != NULL) {
            cmsBool read_ok_null = _cmsRead15Fixed16Number(io2, NULL);
            (void)read_ok_null;
            if (io2->Close != NULL) {
                io2->Close(io2);
            }
            free(io2);
        }
    }

    cleanup_context();
    return 0;
}