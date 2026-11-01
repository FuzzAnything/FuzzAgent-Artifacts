// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* Dummy I/O callbacks for fuzzing matching libtiff signatures */
static tmsize_t dummy_readproc(thandle_t clientdata, void *buf, tmsize_t size) {
    (void)clientdata;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t dummy_writeproc(thandle_t clientdata, void *buf, tmsize_t size) {
    (void)clientdata;
    (void)buf;
    (void)size;
    return size;
}

static toff_t dummy_seekproc(thandle_t clientdata, toff_t offset, int whence) {
    (void)clientdata;
    (void)offset;
    (void)whence;
    return 0;
}

static int dummy_closeproc(thandle_t clientdata) {
    (void)clientdata;
    return 0;
}

/* 
 * TIFFSizeProc is defined as tmsize_t (*)(thandle_t) in some versions,
 * but the error indicates it expects unsigned long. 
 * We cast the function pointer to match the expected type to avoid compilation errors.
 */
static tmsize_t dummy_sizeproc_raw(thandle_t clientdata) {
    (void)clientdata;
    return 0;
}

/* 
 * TIFFMapFileProc signature in modern libtiff is:
 * int (*TIFFMapFileProc)(thandle_t, void **, toff_t *)
 * We return 0 (failure) and set base to NULL.
 */
static int dummy_mapproc(thandle_t clientdata, void **mmap_base, toff_t *size) {
    (void)clientdata;
    (void)mmap_base;
    (void)size;
    return 0;
}

/* 
 * TIFFUnmapFileProc signature in modern libtiff is:
 * void (*TIFFUnmapFileProc)(thandle_t, void *, toff_t)
 */
static void dummy_unmapproc(thandle_t clientdata, void *mmap_base, toff_t size) {
    (void)clientdata;
    (void)mmap_base;
    (void)size;
}

/* Helper to create a dummy file for file-based operations if needed */
static void create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a dummy file to ensure file-based open attempts have a target */
    create_dummy_file("./dummy_file", Data, Size);

    TIFF *tif = NULL;
    
    /* Fuzzing Scenario 1: TIFFClientOpen with dummy callbacks */
    tif = TIFFClientOpen("fuzz_test", "w", (thandle_t)(void*)Data,
                         dummy_readproc, dummy_writeproc,
                         dummy_seekproc, dummy_closeproc,
                         (TIFFSizeProc)dummy_sizeproc_raw, 
                         dummy_mapproc, dummy_unmapproc);

    if (tif) {
        /* Fuzzing Scenario 2: TIFFGetWriteProc */
        TIFFReadWriteProc writeProc = TIFFGetWriteProc(tif);
        (void)writeProc; 

        /* Fuzzing Scenario 3: TIFFGetCloseProc */
        TIFFCloseProc closeProc = TIFFGetCloseProc(tif);
        (void)closeProc;

        /* Fuzzing Scenario 4: TIFFGetSizeProc */
        TIFFSizeProc sizeProc = TIFFGetSizeProc(tif);
        (void)sizeProc;

        /* Cleanup resources */
        TIFFClose(tif);
        tif = NULL;
    }

    /* Fuzzing Scenario 5: TIFFClientOpenExt with NULL options (default behavior) */
    tif = TIFFClientOpenExt("fuzz_test_ext", "w", (thandle_t)(void*)Data,
                            dummy_readproc, dummy_writeproc,
                            dummy_seekproc, dummy_closeproc,
                            (TIFFSizeProc)dummy_sizeproc_raw, 
                            dummy_mapproc, dummy_unmapproc,
                            NULL);

    if (tif) {
        /* Fuzzing Scenario 6: Retrieve procs again on new handle */
        TIFFReadWriteProc writeProc2 = TIFFGetWriteProc(tif);
        (void)writeProc2;

        TIFFCloseProc closeProc2 = TIFFGetCloseProc(tif);
        (void)closeProc2;

        TIFFSizeProc sizeProc2 = TIFFGetSizeProc(tif);
        (void)sizeProc2;

        /* Cleanup */
        TIFFClose(tif);
        tif = NULL;
    }

    /* 
     * Fuzzing Scenario 7: Direct TIFFCleanup on NULL.
     * NOTE: The provided documentation states "If tif is NULL, behavior is undefined."
     * However, AddressSanitizer crashed on the internal implementation checking tif->tif_mode.
     * To ensure stability and avoid crashing the fuzzer on undefined behavior,
     * we skip calling TIFFCleanup(NULL) directly.
     */
    // TIFFCleanup(NULL); 

    /* Fuzzing Scenario 8: Create a handle, don't close, just cleanup */
    tif = TIFFClientOpen("fuzz_test_cleanup", "w", (thandle_t)(void*)Data,
                         dummy_readproc, dummy_writeproc,
                         dummy_seekproc, dummy_closeproc,
                         (TIFFSizeProc)dummy_sizeproc_raw, 
                         dummy_mapproc, dummy_unmapproc);
    if (tif) {
        /* Skip TIFFClose, go straight to TIFFCleanup */
        TIFFCleanup(tif);
        tif = NULL;
    }

    return 0;
}