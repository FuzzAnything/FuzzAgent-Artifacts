// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* Static helper to avoid name conflicts */
static thandle_t dummy_client_data = NULL;

/* Dummy I/O callbacks for TIFFClientOpenExt */
static tmsize_t dummy_readproc(thandle_t handle, void *buf, tmsize_t size) {
    return 0;
}

static tmsize_t dummy_writeproc(thandle_t handle, void *buf, tmsize_t size) {
    return 0;
}

static toff_t dummy_seekproc(thandle_t handle, toff_t off, int whence) {
    return 0;
}

static int dummy_closeproc(thandle_t handle) {
    return 0;
}

/* Fix: TIFFSizeProc expects unsigned long return type */
static unsigned long dummy_sizeproc(thandle_t handle) {
    return 0;
}

/* Fix: TIFFMapFileProc expects int return type and specific args */
static int dummy_mapproc(thandle_t handle, void **base, unsigned long *size) {
    if (base) *base = NULL;
    if (size) *size = 0;
    return 0;
}

/* Fix: TIFFUnmapFileProc expects void return type */
static void dummy_unmapproc(thandle_t handle, void *base, unsigned long size) {
    /* No-op */
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Prepare environment */
    TIFFOpenOptions *dummy_opts = NULL;

    /* Create a dummy file to satisfy file-based operations if needed */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Target API: TIFFClientOpenExt */
    TIFF *tif = TIFFClientOpenExt(
        "./dummy_file",
        "r",
        (thandle_t)Data, /* client data */
        dummy_readproc,
        dummy_writeproc,
        dummy_seekproc,
        dummy_closeproc,
        dummy_sizeproc,
        dummy_mapproc,
        dummy_unmapproc,
        dummy_opts
    );

    if (tif) {
        /* Target API: TIFFSetClientdata */
        TIFFSetClientdata(tif, (thandle_t)Data);

        /* Target API: TIFFClientdata */
        thandle_t client_data = TIFFClientdata(tif);
        (void)client_data;

        /* Target API: TIFFSetClientInfo */
        TIFFSetClientInfo(tif, (void *)Data, "fuzz_info");

        /* Target API: TIFFPrintDirectory */
        /* Use stdout for the FILE stream; flags derived from input */
        long flags = 0;
        if (Size > 0) flags = (long)Data[0];
        if (Size > 1) flags |= ((long)Data[1] << 8);
        
        TIFFPrintDirectory(tif, stdout, flags);

        /* Target API: TIFFCleanup */
        TIFFCleanup(tif);
    }

    /* Cleanup: Remove dummy file */
    remove("./dummy_file");

    return 0;
}