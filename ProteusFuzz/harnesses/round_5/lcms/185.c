// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "lcms2_plugin.h"

static cmsContext g_ctx = NULL;

static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (iohandler == NULL || Buffer == NULL) return 0;
    int fd = (int)(intptr_t)iohandler->stream;
    if (fd < 0) return 0;
    cmsUInt32Number total = size * count;
    ssize_t ret = read(fd, Buffer, total);
    return (ret > 0) ? (cmsUInt32Number)ret : 0;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    if (iohandler == NULL) return FALSE;
    int fd = (int)(intptr_t)iohandler->stream;
    if (fd < 0) return FALSE;
    if (lseek(fd, offset, SEEK_SET) == -1) return FALSE;
    return TRUE;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    if (iohandler == NULL) return FALSE;
    int fd = (int)(intptr_t)iohandler->stream;
    if (fd >= 0) {
        close(fd);
        iohandler->stream = NULL;
    }
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    if (iohandler == NULL) return 0;
    int fd = (int)(intptr_t)iohandler->stream;
    if (fd < 0) return 0;
    off_t ret = lseek(fd, 0, SEEK_CUR);
    return (ret >= 0) ? (cmsUInt32Number)ret : 0;
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    if (iohandler == NULL || Buffer == NULL) return FALSE;
    int fd = (int)(intptr_t)iohandler->stream;
    if (fd < 0) return FALSE;
    if (write(fd, Buffer, size) != (ssize_t)size) return FALSE;
    return TRUE;
}

static void init_io_handler(cmsIOHANDLER* io, int fd) {
    memset(io, 0, sizeof(cmsIOHANDLER));
    io->stream = (void*)(intptr_t)fd;
    io->ContextID = g_ctx;
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);
}

static void cleanup_io_handler(cmsIOHANDLER* io) {
    if (io && io->stream != NULL) {
        int fd = (int)(intptr_t)io->stream;
        if (fd >= 0) {
            close(fd);
            io->stream = NULL;
        }
    }
}

static cmsContext get_context(void) {
    if (g_ctx == NULL) {
        g_ctx = cmsCreateContext(NULL, NULL);
    }
    return g_ctx;
}

int LLVMFuzzerTestOneInput_185(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    get_context();

    // Test _cmsDoubleTo8Fixed8
    if (Size >= 8) {
        cmsFloat64Number val1 = *(const cmsFloat64Number*)(Data);
        cmsUInt16Number res1 = _cmsDoubleTo8Fixed8(val1);
        (void)res1;
    }

    // Test _cms15Fixed16toDouble
    if (Size >= 4) {
        cmsS15Fixed16Number fix32 = *(const cmsS15Fixed16Number*)(Data);
        cmsFloat64Number res2 = _cms15Fixed16toDouble(fix32);
        (void)res2;
    }

    // Test _cmsDoubleTo15Fixed16
    if (Size >= 16) {
        cmsFloat64Number val3 = *(const cmsFloat64Number*)(Data + 8);
        cmsS15Fixed16Number res3 = _cmsDoubleTo15Fixed16(val3);
        (void)res3;
    }

    // Test _cms8Fixed8toDouble
    if (Size >= 2) {
        cmsUInt16Number fixed8 = *(const cmsUInt16Number*)(Data);
        cmsFloat64Number res4 = _cms8Fixed8toDouble(fixed8);
        (void)res4;
    }

    // Prepare dummy file for IO tests
    const char* dummy_file = "./dummy_file";
    int fd = open(dummy_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;

    // Write some data to the file
    size_t write_size = (Size > 1024) ? 1024 : Size;
    if (write(fd, Data, write_size) != (ssize_t)write_size) {
        close(fd);
        return 0;
    }

    cmsIOHANDLER io;
    init_io_handler(&io, fd);

    // Test _cmsRead15Fixed16Number
    cmsFloat64Number read_val;
    cmsBool res5 = _cmsRead15Fixed16Number(&io, &read_val);
    (void)res5;
    (void)read_val;

    // Test _cmsRead15Fixed16Number with NULL output
    cmsBool res5_null = _cmsRead15Fixed16Number(&io, NULL);
    (void)res5_null;

    // Seek back for write test
    dummy_seek(&io, 0);

    // Test _cmsWrite15Fixed16Number
    cmsFloat64Number write_val = 1.0;
    if (Size >= 16) {
        write_val = *(const cmsFloat64Number*)(Data + 8);
    }
    cmsBool res6 = _cmsWrite15Fixed16Number(&io, write_val);
    (void)res6;

    // Cleanup IO handler
    cleanup_io_handler(&io);

    // Close file explicitly if not done by handler
    if (fd >= 0) close(fd);

    // Cleanup context on exit (optional, but good practice)
    if (g_ctx != NULL) {
        cmsDeleteContext(g_ctx);
        g_ctx = NULL;
    }

    return 0;
}