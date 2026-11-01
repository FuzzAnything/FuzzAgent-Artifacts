// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* Dummy I/O callbacks for TIFFClientOpen matching expected signatures */
static tmsize_t dummy_read(thandle_t fd, void *buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

/* Note: TIFFReadWriteProc expects (void*, void*, tmsize_t), not const void* */
static tmsize_t dummy_write(thandle_t fd, void *buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

static toff_t dummy_seek(thandle_t fd, toff_t off, int whence) {
    (void)fd;
    (void)off;
    (void)whence;
    return 0;
}

static int dummy_close(thandle_t fd) {
    (void)fd;
    return 0;
}

static toff_t dummy_size(thandle_t fd) {
    (void)fd;
    return 0;
}

/* TIFFMapFileProc expects: int (*)(thandle_t, void **, toff_t *) */
static int dummy_map(thandle_t fd, void **base, toff_t *size) {
    (void)fd;
    (void)base;
    (void)size;
    return 0;
}

/* TIFFUnmapFileProc expects: void (*)(thandle_t, void *, toff_t) */
static void dummy_unmap(thandle_t fd, void *base, toff_t size) {
    (void)fd;
    (void)base;
    (void)size;
}

/* Helper to safely close and cleanup */
static void safe_cleanup(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
        TIFFCleanup(tif);
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a dummy file to ensure valid file descriptor if needed */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    TIFF *tif = NULL;
    TIFFSizeProc size_proc;
    TIFFReadWriteProc write_proc;
    TIFFCloseProc close_proc;

    /* Attempt to open with custom callbacks using TIFFClientOpen */
    tif = TIFFClientOpen(
        "./dummy_file",
        "r",
        NULL,
        dummy_read,
        dummy_write,
        dummy_seek,
        dummy_close,
        dummy_size,
        dummy_map,
        dummy_unmap
    );

    if (tif != NULL) {
        /* Fuzz TIFFGetSizeProc */
        size_proc = TIFFGetSizeProc(tif);
        (void)size_proc;

        /* Fuzz TIFFGetWriteProc */
        write_proc = TIFFGetWriteProc(tif);
        (void)write_proc;

        /* Fuzz TIFFGetCloseProc */
        close_proc = TIFFGetCloseProc(tif);
        (void)close_proc;

        /* Cleanup resources */
        safe_cleanup(tif);
    }

    /* Attempt to open with TIFFClientOpenExt if available (API 4.1+) */
    /* We use a pointer to avoid initialization of incomplete type if struct is opaque */
    TIFFOpenOptions *opts = NULL;
    
    tif = TIFFClientOpenExt(
        "./dummy_file",
        "r",
        NULL,
        dummy_read,
        dummy_write,
        dummy_seek,
        dummy_close,
        dummy_size,
        dummy_map,
        dummy_unmap,
        opts
    );

    if (tif != NULL) {
        /* Fuzz TIFFGetSizeProc */
        size_proc = TIFFGetSizeProc(tif);
        (void)size_proc;

        /* Fuzz TIFFGetWriteProc */
        write_proc = TIFFGetWriteProc(tif);
        (void)write_proc;

        /* Fuzz TIFFGetCloseProc */
        close_proc = TIFFGetCloseProc(tif);
        (void)close_proc;

        /* Cleanup resources */
        safe_cleanup(tif);
    }

    /* Test with write mode */
    tif = TIFFClientOpen(
        "./dummy_file",
        "w",
        NULL,
        dummy_read,
        dummy_write,
        dummy_seek,
        dummy_close,
        dummy_size,
        dummy_map,
        dummy_unmap
    );

    if (tif != NULL) {
        size_proc = TIFFGetSizeProc(tif);
        (void)size_proc;
        write_proc = TIFFGetWriteProc(tif);
        (void)write_proc;
        close_proc = TIFFGetCloseProc(tif);
        (void)close_proc;
        safe_cleanup(tif);
    }

    /* Test TIFFClientOpenExt with write mode */
    tif = TIFFClientOpenExt(
        "./dummy_file",
        "w",
        NULL,
        dummy_read,
        dummy_write,
        dummy_seek,
        dummy_close,
        dummy_size,
        dummy_map,
        dummy_unmap,
        opts
    );

    if (tif != NULL) {
        size_proc = TIFFGetSizeProc(tif);
        (void)size_proc;
        write_proc = TIFFGetWriteProc(tif);
        (void)write_proc;
        close_proc = TIFFGetCloseProc(tif);
        (void)close_proc;
        safe_cleanup(tif);
    }

    return 0;
}