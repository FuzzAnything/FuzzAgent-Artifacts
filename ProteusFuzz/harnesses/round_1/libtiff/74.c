// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* Dummy I/O callbacks for TIFFClientOpen */
static tmsize_t dummy_readproc(thandle_t fd, void *buf, tmsize_t size) {
    return 0;
}

static tmsize_t dummy_writeproc(thandle_t fd, void *buf, tmsize_t size) {
    return size;
}

static toff_t dummy_seekproc(thandle_t fd, toff_t off, int whence) {
    return 0;
}

static int dummy_closeproc(thandle_t fd) {
    return 0;
}

static toff_t dummy_sizeproc(thandle_t fd) {
    return 0;
}

/* TIFFMapFileProc signature: int (*)(thandle_t, void **, toff_t *) */
static int dummy_mapproc(thandle_t fd, void **base, toff_t *size) {
    *base = NULL;
    *size = 0;
    return 0;
}

/* TIFFUnmapFileProc signature: void (*)(thandle_t, void *, toff_t) */
static void dummy_unmapproc(thandle_t fd, void *base, toff_t size) {
    /* Do nothing */
}

/* Static helper to create a TIFF handle using client open with fuzzed data */
static TIFF* create_tiff_handle(const uint8_t *data, size_t size, thandle_t *out_client_data) {
    /* Create a dummy file to satisfy potential internal checks if needed */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }

    thandle_t client_data = NULL;
    if (size > 0) {
        client_data = (thandle_t)malloc(size);
        if (client_data) {
            memcpy(client_data, data, size);
        }
    }

    TIFF *tif = TIFFClientOpen(
        "./dummy_file",
        "rw",
        client_data,
        dummy_readproc,
        dummy_writeproc,
        dummy_seekproc,
        dummy_closeproc,
        dummy_sizeproc,
        dummy_mapproc,
        dummy_unmapproc
    );

    if (tif && client_data) {
        /* Set client data to the fuzzed data for testing TIFFClientdata */
        TIFFSetClientdata(tif, client_data);
    }

    if (out_client_data) {
        *out_client_data = client_data;
    }

    return tif;
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    thandle_t client_data_1 = NULL;
    TIFF *tif = create_tiff_handle(Data, Size, &client_data_1);
    
    if (tif) {
        /* Test 1: TIFFGetWriteProc */
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        (void)write_proc; 

        /* Test 2: TIFFGetCloseProc */
        TIFFCloseProc close_proc = TIFFGetCloseProc(tif);
        (void)close_proc;

        /* Test 3: TIFFClientdata */
        thandle_t retrieved_data = TIFFClientdata(tif);
        (void)retrieved_data;

        /* Test 4: TIFFGetUnmapFileProc */
        TIFFUnmapFileProc unmap_proc = TIFFGetUnmapFileProc(tif);
        (void)unmap_proc;

        /* Cleanup */
        TIFFClose(tif);
    }
    
    /* Free the client data allocated for the first handle if it wasn't freed by libtiff */
    /* Note: libtiff usually does not free client data on close, so we must free it here */
    if (client_data_1) {
        free(client_data_1);
    }

    /* Test 5: TIFFClientOpenExt with options */
    /* Passing NULL for opts to use defaults */
    TIFF *tif_ext = TIFFClientOpenExt(
        "./dummy_file",
        "rw",
        NULL, /* client_data */
        dummy_readproc,
        dummy_writeproc,
        dummy_seekproc,
        dummy_closeproc,
        dummy_sizeproc,
        dummy_mapproc,
        dummy_unmapproc,
        NULL /* opts */
    );

    if (tif_ext) {
        /* Re-test getters on the Ext handle */
        (void)TIFFGetWriteProc(tif_ext);
        (void)TIFFGetCloseProc(tif_ext);
        (void)TIFFClientdata(tif_ext);
        (void)TIFFGetUnmapFileProc(tif_ext);

        TIFFClose(tif_ext);
    }

    return 0;
}