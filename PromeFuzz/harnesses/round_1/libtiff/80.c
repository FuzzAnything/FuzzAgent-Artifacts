// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Custom I/O handlers */
static tmsize_t dummyReadProc(thandle_t fd, void* buf, tmsize_t size) {
    (void)fd;
    /* Read from the buffer we have */
    return size;
}

static tmsize_t dummyWriteProc(thandle_t fd, void* buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    /* Pretend we wrote everything */
    return size;
}

static toff_t dummySeekProc(thandle_t fd, toff_t off, int whence) {
    (void)fd;
    (void)whence;
    /* Just return the offset */
    return off;
}

static int dummyCloseProc(thandle_t fd) {
    (void)fd;
    /* Nothing to close */
    return 0;
}

static toff_t dummySizeProc(thandle_t fd) {
    (void)fd;
    /* Return a dummy size */
    return 1024;
}

static int dummyMapProc(thandle_t fd, void** base, toff_t* size) {
    (void)fd;
    (void)base;
    (void)size;
    /* Mapping not supported */
    return 0;
}

static void dummyUnmapProc(thandle_t fd, void* base, toff_t size) {
    (void)fd;
    (void)base;
    (void)size;
    /* Nothing to unmap */
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    /* Write data to dummy file */
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) {
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    /* Open the dummy file with standard TIFFOpen first to get a TIFF* */
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        /* If standard open fails, try client open with dummy handlers */
        tif = TIFFClientOpen("./dummy_file", "r", (thandle_t)0,
                             dummyReadProc, dummyWriteProc,
                             dummySeekProc, dummyCloseProc,
                             dummySizeProc, dummyMapProc, dummyUnmapProc);
        if (!tif) {
            /* Clean up and return */
            remove("./dummy_file");
            return 0;
        }
    }

    /* Test TIFFGetSeekProc */
    TIFFSeekProc seekProc = TIFFGetSeekProc(tif);
    (void)seekProc;

    /* Test TIFFGetUnmapFileProc */
    TIFFUnmapFileProc unmapProc = TIFFGetUnmapFileProc(tif);
    (void)unmapProc;

    /* Test TIFFGetWriteProc */
    TIFFReadWriteProc writeProc = TIFFGetWriteProc(tif);
    (void)writeProc;

    /* Test TIFFGetReadProc */
    TIFFReadWriteProc readProc = TIFFGetReadProc(tif);
    (void)readProc;

    /* Test TIFFGetMapFileProc */
    TIFFMapFileProc mapProc = TIFFGetMapFileProc(tif);
    (void)mapProc;

    /* Clean up */
    TIFFClose(tif);
    remove("./dummy_file");

    return 0;
}