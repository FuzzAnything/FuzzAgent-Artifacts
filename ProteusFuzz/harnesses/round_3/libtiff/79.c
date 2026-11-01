// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <tiffio.h>

/* Dummy I/O callbacks for TIFFClientOpen */
static tmsize_t
dummy_readproc(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    (void)clientdata;
    (void)buf;
    (void)size;
    return -1;
}

static tmsize_t
dummy_writeproc(thandle_t clientdata, const tdata_t buf, tmsize_t size) {
    (void)clientdata;
    (void)buf;
    (void)size;
    return -1;
}

static toff_t
dummy_seekproc(thandle_t clientdata, toff_t offset, int whence) {
    (void)clientdata;
    (void)offset;
    (void)whence;
    return -1;
}

static int
dummy_closeproc(thandle_t clientdata) {
    (void)clientdata;
    return 0;
}

static tmsize_t
dummy_sizeproc(thandle_t clientdata) {
    (void)clientdata;
    return -1;
}

/* Correct signature for TIFFMapFileProc: int (*)(void *, void **, toff_t *) */
static int
dummy_mapproc(thandle_t clientdata, tdata_t *base, toff_t *size) {
    (void)clientdata;
    (void)base;
    (void)size;
    return 0;
}

/* Correct signature for TIFFUnmapFileProc: void (*)(void *, void *, toff_t) */
static void
dummy_unmapproc(thandle_t clientdata, tdata_t base, toff_t size) {
    (void)clientdata;
    (void)base;
    (void)size;
}

/* 
 * TIFFErrorHandlerExtR signature:
 * int (*TIFFErrorHandlerExtR)(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap);
 */
static int
dummy_error_handler(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

/* 
 * TIFFErrorHandlerExtR signature for warnhandler as well:
 * int (*TIFFErrorHandlerExtR)(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap);
 */
static int
dummy_warn_handler(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a temporary file for file descriptor based tests */
    const char *filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }

    /* Write fuzz data to the file */
    ssize_t written = write(fd, Data, Size);
    if (written < 0) {
        close(fd);
        return 0;
    }
    lseek(fd, 0, SEEK_SET);

    /* Define the struct locally to ensure it's complete and matches expected layout */
    typedef struct {
        TIFFErrorHandlerExtR errorhandler;
        void *errorhandler_user_data;
        TIFFErrorHandlerExtR warnhandler;
        void *warnhandler_user_data;
        tmsize_t max_single_mem_alloc;
        tmsize_t max_cumulated_mem_alloc;
        int warn_about_unknown_tags;
    } LocalTIFFOpenOptions;

    LocalTIFFOpenOptions opts_local;
    memset(&opts_local, 0, sizeof(opts_local));
    opts_local.errorhandler = dummy_error_handler;
    opts_local.errorhandler_user_data = NULL;
    opts_local.warnhandler = dummy_warn_handler;
    opts_local.warnhandler_user_data = NULL;
    opts_local.max_single_mem_alloc = 0;
    opts_local.max_cumulated_mem_alloc = 0;
    opts_local.warn_about_unknown_tags = 0;

    /* Cast to TIFFOpenOptions* for the function call */
    TIFFOpenOptions *opts = (TIFFOpenOptions *)&opts_local;

    /* Test 1: TIFFFdOpenExt */
    TIFF *tif_fd = TIFFFdOpenExt(fd, filename, "r", opts);
    if (tif_fd != NULL) {
        thandle_t client_data = TIFFClientdata(tif_fd);
        (void)client_data;
        TIFFCloseProc close_proc = TIFFGetCloseProc(tif_fd);
        (void)close_proc;
        TIFFClose(tif_fd);
        TIFFCleanup(tif_fd);
    }

    /* Re-open file descriptor for next test if needed, or use new one */
    lseek(fd, 0, SEEK_SET);

    /* Test 2: TIFFClientOpen */
    TIFF *tif_client = TIFFClientOpen(filename, "r", (thandle_t)Data,
                                      dummy_readproc, dummy_writeproc,
                                      dummy_seekproc, dummy_closeproc,
                                      (TIFFSizeProc)dummy_sizeproc,
                                      (TIFFMapFileProc)dummy_mapproc,
                                      (TIFFUnmapFileProc)dummy_unmapproc);
    if (tif_client != NULL) {
        thandle_t client_data = TIFFClientdata(tif_client);
        (void)client_data;
        TIFFCloseProc close_proc = TIFFGetCloseProc(tif_client);
        (void)close_proc;
        TIFFClose(tif_client);
        TIFFCleanup(tif_client);
    }

    /* Test 3: TIFFClientOpenExt */
    TIFF *tif_client_ext = TIFFClientOpenExt(filename, "r", (thandle_t)Data,
                                             dummy_readproc, dummy_writeproc,
                                             dummy_seekproc, dummy_closeproc,
                                             (TIFFSizeProc)dummy_sizeproc,
                                             (TIFFMapFileProc)dummy_mapproc,
                                             (TIFFUnmapFileProc)dummy_unmapproc, opts);
    if (tif_client_ext != NULL) {
        thandle_t client_data = TIFFClientdata(tif_client_ext);
        (void)client_data;
        TIFFCloseProc close_proc = TIFFGetCloseProc(tif_client_ext);
        (void)close_proc;
        TIFFClose(tif_client_ext);
        TIFFCleanup(tif_client_ext);
    }

    /* Cleanup file */
    close(fd);
    unlink(filename);

    return 0;
}