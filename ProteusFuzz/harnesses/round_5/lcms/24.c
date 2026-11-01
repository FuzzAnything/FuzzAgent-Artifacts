// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Internal memory functions from lcms2_plugin.h
static void* my_malloc(cmsContext ctx, size_t size) {
    return _cmsMalloc(ctx, (cmsUInt32Number)size);
}

static void my_free(cmsContext ctx, void* ptr) {
    if (ptr) _cmsFree(ctx, ptr);
}

// Custom IO handler implementation for file-based fuzzing
static cmsUInt32Number file_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    int fd = (int)(intptr_t)iohandler->stream;
    if (fd < 0) return 0;
    size_t bytes_to_read = size * count;
    ssize_t ret = read(fd, Buffer, bytes_to_read);
    if (ret < 0) return 0;
    return (cmsUInt32Number)ret / size;
}

static cmsBool file_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    int fd = (int)(intptr_t)iohandler->stream;
    if (fd < 0) return FALSE;
    if (lseek(fd, offset, SEEK_SET) == (off_t)-1) return FALSE;
    return TRUE;
}

static cmsBool file_close(struct _cms_io_handler* iohandler) {
    int fd = (int)(intptr_t)iohandler->stream;
    if (fd < 0) return TRUE;
    close(fd);
    iohandler->stream = NULL;
    return TRUE;
}

static cmsUInt32Number file_tell(struct _cms_io_handler* iohandler) {
    int fd = (int)(intptr_t)iohandler->stream;
    if (fd < 0) return 0;
    off_t pos = lseek(fd, 0, SEEK_CUR);
    if (pos == (off_t)-1) return 0;
    return (cmsUInt32Number)pos;
}

static cmsBool file_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    int fd = (int)(intptr_t)iohandler->stream;
    if (fd < 0) return FALSE;
    ssize_t ret = write(fd, Buffer, size);
    if (ret < 0 || (size_t)ret != size) return FALSE;
    return TRUE;
}

static cmsIOHANDLER* create_io_handler(cmsContext ctx, int is_write) {
    cmsIOHANDLER *io = (cmsIOHANDLER*)my_malloc(ctx, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    memset(io, 0, sizeof(cmsIOHANDLER));

    io->ContextID = ctx;
    io->ReportedSize = 0;
    io->UsedSpace = 0;

    int flags = is_write ? (O_WRONLY | O_CREAT | O_TRUNC) : O_RDONLY;
    int fd = open("./dummy_file", flags, 0644);
    if (fd < 0) {
        my_free(ctx, io);
        return NULL;
    }
    
    io->stream = (void*)(intptr_t)fd;
    
    // Set up function pointers
    io->Read = file_read;
    io->Write = file_write;
    io->Seek = file_seek;
    io->Tell = file_tell;
    io->Close = file_close;

    return io;
}

static void cleanup_io_handler(cmsIOHANDLER *io, cmsContext ctx) {
    if (!io) return;
    if (io->Close) {
        io->Close(io);
    }
    my_free(ctx, io);
}

static void cleanup_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Create a temporary file with the fuzz data
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }

    cmsIOHANDLER *io_read = NULL;
    cmsIOHANDLER *io_write = NULL;
    cmsBool res;
    cmsUInt32Number val;

    // Test 1: Read operations
    io_read = create_io_handler(ctx, 0); // Read mode
    if (io_read) {
        // Seek to beginning
        if (io_read->Seek) io_read->Seek(io_read, 0);

        // Fuzz _cmsReadUInt32Number
        if (Size >= 4) {
            res = _cmsReadUInt32Number(io_read, &val);
            (void)res;
        }

        // Fuzz _cmsReadAlignment
        res = _cmsReadAlignment(io_read);
        (void)res;

        cleanup_io_handler(io_read, ctx);
    }

    // Test 2: Write operations
    io_write = create_io_handler(ctx, 1); // Write mode
    if (io_write) {
        // Fuzz _cmsWriteUInt32Number
        cmsUInt32Number test_val = 0;
        if (Size >= 4) {
            memcpy(&test_val, Data, 4);
        } else if (Size > 0) {
            test_val = Data[0];
        }
        res = _cmsWriteUInt32Number(io_write, test_val);
        (void)res;

        // Fuzz _cmsWriteAlignment
        res = _cmsWriteAlignment(io_write);
        (void)res;

        // Fuzz _cmsWriteTypeBase
        cmsTagTypeSignature sig = 0;
        if (Size >= 4) {
            memcpy(&sig, Data, 4);
        }
        res = _cmsWriteTypeBase(io_write, sig);
        (void)res;

        cleanup_io_handler(io_write, ctx);
    }

    // Test 3: Explicit close via cmsCloseIOhandler
    cmsIOHANDLER *io_temp = create_io_handler(ctx, 1);
    if (io_temp) {
        res = cmsCloseIOhandler(io_temp);
        (void)res;
        // Manually free the struct since Close was called
        my_free(ctx, io_temp);
    }

    cleanup_context(ctx);
    
    // Clean up dummy file
    unlink("./dummy_file");

    return 0;
}