// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

// Correctly typed callback functions matching libtiff definitions
// TIFFSizeProc expects unsigned long return type
static unsigned long dummy_size(thandle_t fd) {
    (void)fd;
    return 0;
}

// TIFFMapFileProc expects unsigned long * for size argument
static int dummy_mmap(thandle_t fd, void **base, unsigned long *size) {
    (void)fd;
    (void)base;
    (void)size;
    return 0;
}

// TIFFUnmapFileProc expects unsigned long for size argument
static void dummy_unmap(thandle_t fd, void *base, unsigned long size) {
    (void)fd;
    (void)base;
    (void)size;
}

static tmsize_t dummy_read(thandle_t fd, void *buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t dummy_write(thandle_t fd, void *buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return size;
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

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    TIFF *tif = NULL;
    thandle_t client_data = (thandle_t)Data;

    // Create a dummy file to satisfy potential file operations if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Test TIFFClientOpenExt
    TIFFOpenOptions *opts = NULL;

    tif = TIFFClientOpenExt(
        "./dummy_file",
        "r",
        client_data,
        dummy_read,
        dummy_write,
        dummy_seek,
        dummy_close,
        dummy_size,
        dummy_mmap,
        dummy_unmap,
        opts
    );

    if (tif) {
        // Test TIFFGetCloseProc
        TIFFCloseProc close_proc = TIFFGetCloseProc(tif);
        (void)close_proc;

        // Test TIFFClientdata
        thandle_t data = TIFFClientdata(tif);
        (void)data;

        // Test TIFFGetWriteProc
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        (void)write_proc;

        // Cleanup resources
        TIFFCleanup(tif);
    }

    // Test TIFFClientOpen (wrapper for Ext, no options struct needed)
    tif = TIFFClientOpen(
        "./dummy_file",
        "r",
        client_data,
        dummy_read,
        dummy_write,
        dummy_seek,
        dummy_close,
        dummy_size,
        dummy_mmap,
        dummy_unmap
    );

    if (tif) {
        // Test getters again on second instance
        TIFFCloseProc close_proc = TIFFGetCloseProc(tif);
        (void)close_proc;

        thandle_t data = TIFFClientdata(tif);
        (void)data;

        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        (void)write_proc;

        // Cleanup
        TIFFCleanup(tif);
    }

    // Test with write mode
    tif = TIFFClientOpen(
        "./dummy_file",
        "w",
        client_data,
        dummy_read,
        dummy_write,
        dummy_seek,
        dummy_close,
        dummy_size,
        dummy_mmap,
        dummy_unmap
    );

    if (tif) {
        TIFFCloseProc close_proc = TIFFGetCloseProc(tif);
        (void)close_proc;
        thandle_t data = TIFFClientdata(tif);
        (void)data;
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        (void)write_proc;
        TIFFCleanup(tif);
    }

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}