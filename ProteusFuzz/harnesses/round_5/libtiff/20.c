// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* Dummy I/O callbacks for TIFFClientOpen */
static tmsize_t dummy_readproc(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    return 0;
}

static tmsize_t dummy_writeproc(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    return size;
}

static toff_t dummy_seekproc(thandle_t clientdata, toff_t offset, int whence) {
    return 0;
}

static int dummy_closeproc(thandle_t clientdata) {
    return 0;
}

static toff_t dummy_sizeproc(thandle_t clientdata) {
    return 0;
}

/* TIFFMapFileProc signature: int (*TIFFMapFileProc)(thandle_t, tdata_t *, toff_t *) */
static int dummy_mapproc(thandle_t clientdata, tdata_t *buf, toff_t *size) {
    if (buf) {
        *buf = NULL;
    }
    if (size) {
        *size = 0;
    }
    return 0;
}

/* TIFFUnmapFileProc signature: void (*TIFFUnmapFileProc)(thandle_t, tdata_t, toff_t) */
static void dummy_unmapproc(thandle_t clientdata, tdata_t base, toff_t size) {
    /* No-op */
}

/* Helper to create a valid TIFF instance using client data */
static TIFF* create_tiff_from_input(const uint8_t *Data, size_t Size) {
    /* Use dummy file name and mode */
    const char *name = "dummy_tiff";
    const char *mode = "w";
    thandle_t clientdata = NULL;

    /* Try opening with standard callbacks first */
    TIFF *tif = TIFFClientOpen(
        name, mode, clientdata,
        dummy_readproc, dummy_writeproc,
        dummy_seekproc, dummy_closeproc,
        dummy_sizeproc, dummy_mapproc, dummy_unmapproc
    );

    if (tif == NULL) {
        return NULL;
    }

    /* Optionally write some data to ensure state changes if needed,
       though for getter functions this is not strictly required. */
    if (Size > 0 && Data != NULL) {
        /* Attempt to write fuzz data to trigger internal state changes */
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        
        /* Write a small amount of data to ensure the file is valid */
        uint8_t dummy_row[1];
        dummy_row[0] = Data[0];
        TIFFWriteScanline(tif, dummy_row, 0, 0);
    }

    return tif;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    TIFF *tif = create_tiff_from_input(Data, Size);
    if (tif == NULL) {
        return 0;
    }

    /* Test 1: TIFFGetSizeProc */
    TIFFSizeProc size_proc = TIFFGetSizeProc(tif);
    (void)size_proc; /* Use the result to avoid unused variable warnings */

    /* Test 2: TIFFGetWriteProc */
    TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
    (void)write_proc;

    /* Test 3: TIFFGetCloseProc */
    TIFFCloseProc close_proc = TIFFGetCloseProc(tif);
    (void)close_proc;

    /* Cleanup and close the first instance */
    /* Correct order: Close first (which internally calls Cleanup), do not call Cleanup separately */
    TIFFClose(tif);

    /* Test 4: TIFFClientOpenExt (with NULL options to test default behavior) */
    TIFF *tif_ext = TIFFClientOpenExt(
        "dummy_tiff_ext", "w", NULL,
        dummy_readproc, dummy_writeproc,
        dummy_seekproc, dummy_closeproc,
        dummy_sizeproc, dummy_mapproc, dummy_unmapproc,
        NULL /* opts */
    );

    if (tif_ext != NULL) {
        /* Exercise getters on the extended open instance */
        TIFFSizeProc size_proc_ext = TIFFGetSizeProc(tif_ext);
        (void)size_proc_ext;
        
        TIFFReadWriteProc write_proc_ext = TIFFGetWriteProc(tif_ext);
        (void)write_proc_ext;
        
        TIFFCloseProc close_proc_ext = TIFFGetCloseProc(tif_ext);
        (void)close_proc_ext;

        /* Correct order: Close first, then Cleanup is handled internally by Close */
        TIFFClose(tif_ext);
    }

    return 0;
}