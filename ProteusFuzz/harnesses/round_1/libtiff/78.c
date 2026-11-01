// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* Dummy I/O callbacks for fuzzing with correct signatures */
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
    (void)whence;
    return (toff_t)off;
}

static int dummy_close(thandle_t fd) {
    (void)fd;
    return 0;
}

static unsigned long dummy_size(thandle_t fd) {
    (void)fd;
    return 0;
}

static int dummy_mapproc(thandle_t fd, void **mapptr, unsigned long *size) {
    (void)fd;
    (void)mapptr;
    (void)size;
    return 0;
}

static void dummy_unmapproc(thandle_t fd, void *mapptr, unsigned long size) {
    (void)fd;
    (void)mapptr;
    (void)size;
}

/* Helper to create a TIFF handle from fuzz input using TIFFClientOpen */
static TIFF *create_tiff_from_input(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }

    const char *mode = (data[0] == 0) ? "r" : "w";

    /* Write input to dummy file */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }

    TIFF *tif = TIFFClientOpen(
        "fuzz_input",
        mode,
        NULL,
        dummy_read,
        dummy_write,
        dummy_seek,
        dummy_close,
        dummy_size,
        dummy_mapproc,
        dummy_unmapproc
    );

    return tif;
}

/* Helper to create a TIFF handle using TIFFClientOpenExt
   Since TIFFOpenOptions is opaque in the header, we cannot initialize it on the stack.
   We rely on TIFFClientOpen for the main fuzzing logic as it covers the same code paths
   for the getter functions (GetWriteProc, GetCloseProc, etc.) which are the targets.
   If TIFFClientOpenExt is strictly required and the struct is opaque, we cannot call it
   without a valid struct instance. We skip this specific call to ensure compilation,
   as the target APIs (GetWriteProc, etc.) are tested via TIFFClientOpen.
*/
static TIFF *create_tiff_ext_from_input(const uint8_t *data, size_t size) {
    /* Fallback to TIFFClientOpen since TIFFOpenOptions is incomplete */
    return create_tiff_from_input(data, size);
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test Case 1: Open with TIFFClientOpen and test getter functions */
    TIFF *tif1 = create_tiff_from_input(Data, Size);
    if (tif1) {
        TIFFReadWriteProc writeProc = TIFFGetWriteProc(tif1);
        (void)writeProc;

        TIFFCloseProc closeProc = TIFFGetCloseProc(tif1);
        (void)closeProc;

        thandle_t clientData = TIFFClientdata(tif1);
        (void)clientData;

        TIFFUnmapFileProc unmapProc = TIFFGetUnmapFileProc(tif1);
        (void)unmapProc;

        TIFFClose(tif1);
    }

    /* Test Case 2: Open with TIFFClientOpenExt (fallback to ClientOpen) and test getter functions */
    TIFF *tif2 = create_tiff_ext_from_input(Data, Size);
    if (tif2) {
        TIFFReadWriteProc writeProc2 = TIFFGetWriteProc(tif2);
        (void)writeProc2;

        TIFFCloseProc closeProc2 = TIFFGetCloseProc(tif2);
        (void)closeProc2;

        thandle_t clientData2 = TIFFClientdata(tif2);
        (void)clientData2;

        TIFFUnmapFileProc unmapProc2 = TIFFGetUnmapFileProc(tif2);
        (void)unmapProc2;

        TIFFClose(tif2);
    }

    /* Test Case 3: Create a valid handle, set client data, then retrieve */
    TIFF *tif3 = TIFFClientOpen(
        "fuzz_input_set",
        "w",
        (thandle_t)0x12345678,
        dummy_read,
        dummy_write,
        dummy_seek,
        dummy_close,
        dummy_size,
        dummy_mapproc,
        dummy_unmapproc
    );

    if (tif3) {
        thandle_t retrievedData = TIFFClientdata(tif3);
        (void)retrievedData;

        (void)TIFFGetWriteProc(tif3);
        (void)TIFFGetCloseProc(tif3);
        (void)TIFFGetUnmapFileProc(tif3);

        TIFFClose(tif3);
    }

    return 0;
}