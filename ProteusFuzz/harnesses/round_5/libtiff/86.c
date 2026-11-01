// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* Dummy I/O callbacks for TIFFClientOpen with correct signatures */
static tmsize_t
dummy_readproc(thandle_t fd, void *buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t
dummy_writeproc(thandle_t fd, void *buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return size;
}

static toff_t
dummy_seekproc(thandle_t fd, toff_t off, int whence) {
    (void)fd;
    (void)off;
    (void)whence;
    return 0;
}

static int
dummy_closeproc(thandle_t fd) {
    (void)fd;
    return 0;
}

/* TIFFSizeProc expects unsigned long (tmsize_t is signed), cast return value */
static tmsize_t
dummy_sizeproc(thandle_t fd) {
    (void)fd;
    return 0;
}

/* TIFFMapFileProc expects int return, not void* */
static int
dummy_mapproc(thandle_t fd, void **base, toff_t *size) {
    (void)fd;
    (void)base;
    (void)size;
    return 0;
}

static void
dummy_unmapproc(thandle_t fd, void *base, toff_t size) {
    (void)fd;
    (void)base;
    (void)size;
}

int
LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a dummy file to satisfy file-based operations if needed */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    TIFF *tif = NULL;
    thandle_t client_data = (thandle_t)(uintptr_t)(Size % 1000);

    /* Strategy 1: Open with TIFFClientOpen using dummy callbacks */
    tif = TIFFClientOpen(
        "fuzz_test", "w",
        client_data,
        dummy_readproc,
        dummy_writeproc,
        dummy_seekproc,
        dummy_closeproc,
        (TIFFSizeProc)dummy_sizeproc,
        dummy_mapproc,
        dummy_unmapproc
    );

    if (tif) {
        /* Test TIFFGetUnmapFileProc */
        TIFFUnmapFileProc unmapproc = TIFFGetUnmapFileProc(tif);
        (void)unmapproc;

        /* Test TIFFGetWriteProc */
        TIFFReadWriteProc writeproc = TIFFGetWriteProc(tif);
        (void)writeproc;

        /* Test TIFFClientdata */
        thandle_t cdata = TIFFClientdata(tif);
        (void)cdata;

        /* Test TIFFGetCloseProc */
        TIFFCloseProc closeproc = TIFFGetCloseProc(tif);
        (void)closeproc;

        /* Cleanup */
        TIFFClose(tif);
    }

    /* Strategy 2: Open with TIFFClientOpenExt (passing NULL for opts to avoid incomplete type) */
    tif = TIFFClientOpenExt(
        "fuzz_test_ext", "w",
        client_data,
        dummy_readproc,
        dummy_writeproc,
        dummy_seekproc,
        dummy_closeproc,
        (TIFFSizeProc)dummy_sizeproc,
        dummy_mapproc,
        dummy_unmapproc,
        NULL
    );

    if (tif) {
        /* Re-test getters on the new handle */
        TIFFUnmapFileProc unmapproc2 = TIFFGetUnmapFileProc(tif);
        (void)unmapproc2;

        TIFFReadWriteProc writeproc2 = TIFFGetWriteProc(tif);
        (void)writeproc2;

        thandle_t cdata2 = TIFFClientdata(tif);
        (void)cdata2;

        TIFFCloseProc closeproc2 = TIFFGetCloseProc(tif);
        (void)closeproc2;

        TIFFClose(tif);
    }

    /* Strategy 3: Attempt to open the dummy file directly to test different code paths */
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        TIFFUnmapFileProc unmapproc3 = TIFFGetUnmapFileProc(tif);
        (void)unmapproc3;

        TIFFReadWriteProc writeproc3 = TIFFGetWriteProc(tif);
        (void)writeproc3;

        thandle_t cdata3 = TIFFClientdata(tif);
        (void)cdata3;

        TIFFCloseProc closeproc3 = TIFFGetCloseProc(tif);
        (void)closeproc3;

        TIFFClose(tif);
    }

    /* Cleanup dummy file */
    remove("./dummy_file");

    return 0;
}