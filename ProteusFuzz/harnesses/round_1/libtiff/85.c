// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/*
 * Custom I/O callback implementations for fuzzing.
 * These are static to avoid name conflicts.
 * Signatures must match libtiff's typedefs exactly.
 */

static ssize_t FuzzReadProc(thandle_t fd, tdata_t buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

static ssize_t FuzzWriteProc(thandle_t fd, const tdata_t buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return (ssize_t)size;
}

/* TIFFSeekProc is defined as toff_t (*)(thandle_t, toff_t, int) */
static toff_t FuzzSeekProc(thandle_t fd, toff_t off, int whence) {
    (void)fd;
    (void)off;
    (void)whence;
    return 0;
}

static int FuzzCloseProc(thandle_t fd) {
    (void)fd;
    return 0;
}

static toff_t FuzzSizeProc(thandle_t fd) {
    (void)fd;
    return 0;
}

/* TIFFMapFileProc is defined as tdata_t (*)(thandle_t, toff_t *) in older versions 
   or int (*)(thandle_t, tdata_t *, toff_t *) in newer. 
   Based on the error message: 'int (*)(void *, void **, unsigned long *)'
   We match the signature: int FuzzMapProc(thandle_t fd, tdata_t *base, toff_t *size)
*/
static int FuzzMapProc(thandle_t fd, tdata_t *base, toff_t *size) {
    (void)fd;
    (void)base;
    (void)size;
    return 0;
}

/* TIFFUnmapFileProc is defined as void (*)(thandle_t, tdata_t, toff_t) */
static void FuzzUnmapProc(thandle_t fd, tdata_t base, toff_t size) {
    (void)fd;
    (void)base;
    (void)size;
}

/*
 * Helper to create a dummy file if needed.
 */
static void create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    const char *mode = "w";
    thandle_t client_data = (thandle_t)Data;

    /* 
     * Strategy 1: Use TIFFClientOpen
     */
    TIFF *tif1 = TIFFClientOpen(
        filename, 
        mode, 
        client_data,
        FuzzReadProc,
        FuzzWriteProc,
        FuzzSeekProc,
        FuzzCloseProc,
        FuzzSizeProc,
        FuzzMapProc,
        FuzzUnmapProc
    );

    if (tif1 != NULL) {
        /* 
         * Test Target API Functions on tif1
         */
        
        /* 1. TIFFGetWriteProc */
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif1);
        (void)write_proc;

        /* 2. TIFFGetCloseProc */
        TIFFCloseProc close_proc = TIFFGetCloseProc(tif1);
        (void)close_proc;

        /* 3. TIFFClientdata */
        thandle_t cdata = TIFFClientdata(tif1);
        (void)cdata;

        /* 4. TIFFGetUnmapFileProc */
        TIFFUnmapFileProc unmap_proc = TIFFGetUnmapFileProc(tif1);
        (void)unmap_proc;

        /* Cleanup for tif1 */
        TIFFClose(tif1);
    }

    /* 
     * Strategy 2: Use TIFFClientOpenExt with options
     * Since TIFFOpenOptions is an incomplete type in tiffio.h, we cannot 
     * instantiate it directly or use sizeof. We pass NULL to skip this path 
     * or rely on the library's internal default handling if it accepts NULL.
     * However, the API signature expects a pointer. If we cannot construct it,
     * we skip this specific call to avoid compilation errors, or we assume 
     * the library might handle NULL gracefully (though not guaranteed).
     * Given the constraints, we will not call TIFFClientOpenExt with a custom struct
     * we cannot define. We can still call it with NULL if the API allows, 
     * but to be safe and strictly follow the "incomplete type" error, 
     * we will omit the complex options path or pass NULL.
     * Let's try passing NULL for options to test the function entry point.
     */
    TIFF *tif2 = TIFFClientOpenExt(
        filename,
        mode,
        client_data,
        FuzzReadProc,
        FuzzWriteProc,
        FuzzSeekProc,
        FuzzCloseProc,
        FuzzSizeProc,
        FuzzMapProc,
        FuzzUnmapProc,
        NULL /* opts */
    );

    if (tif2 != NULL) {
        /* 
         * Test Target API Functions on tif2
         */
        
        /* 1. TIFFGetWriteProc */
        TIFFReadWriteProc write_proc2 = TIFFGetWriteProc(tif2);
        (void)write_proc2;

        /* 2. TIFFGetCloseProc */
        TIFFCloseProc close_proc2 = TIFFGetCloseProc(tif2);
        (void)close_proc2;

        /* 3. TIFFClientdata */
        thandle_t cdata2 = TIFFClientdata(tif2);
        (void)cdata2;

        /* 4. TIFFGetUnmapFileProc */
        TIFFUnmapFileProc unmap_proc2 = TIFFGetUnmapFileProc(tif2);
        (void)unmap_proc2;

        /* Cleanup for tif2 */
        TIFFClose(tif2);
    }

    /* 
     * Strategy 3: Test with NULL handle (Edge Case)
     */
    TIFFReadWriteProc null_write = TIFFGetWriteProc(NULL);
    TIFFCloseProc null_close = TIFFGetCloseProc(NULL);
    thandle_t null_cdata = TIFFClientdata(NULL);
    TIFFUnmapFileProc null_unmap = TIFFGetUnmapFileProc(NULL);
    
    (void)null_write;
    (void)null_close;
    (void)null_cdata;
    (void)null_unmap;

    return 0;
}