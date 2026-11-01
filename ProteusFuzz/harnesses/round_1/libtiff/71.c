// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
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
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "tiffio.h"

/* Dummy I/O callbacks for fuzzing */
static tmsize_t
DummyReadProc(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    return 0;
}

static tmsize_t
DummyWriteProc(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    return size;
}

/* TIFFSeekProc returns toff_t (unsigned long on 64-bit), not int */
static toff_t
DummySeekProc(thandle_t clientdata, toff_t off, int whence) {
    return 0;
}

static int
DummyCloseProc(thandle_t clientdata) {
    return 0;
}

static toff_t
DummySizeProc(thandle_t clientdata) {
    return 0;
}

/* Map/Unmap callbacks */
static int
DummyMapProc(thandle_t clientdata, tdata_t *buf, toff_t *size) {
    if (size) *size = 0;
    if (buf) *buf = NULL;
    return 0;
}

static void
DummyUnmapProc(thandle_t clientdata, tdata_t base, toff_t size) {
    /* No-op */
}

/* Error and Warning handlers matching TIFFErrorHandlerExtR signature:
 * int (*)(TIFF*, void*, const char*, const char*, va_list)
 */
static int
DummyErrorHandler(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    /* No-op */
    return 0;
}

static int
DummyWarnHandler(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    /* No-op */
    return 0;
}

int
LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Create a dummy file to ensure the system has a valid path if needed */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Prepare client data from input */
    thandle_t clientdata = (thandle_t)Data;

    /* Test 1: TIFFClientOpen with basic callbacks */
    TIFF *tif = TIFFClientOpen("fuzz_test", "w", clientdata,
                               DummyReadProc, DummyWriteProc,
                               DummySeekProc, DummyCloseProc,
                               DummySizeProc, DummyMapProc, DummyUnmapProc);

    if (tif) {
        /* Exercise TIFFGetWriteProc */
        TIFFReadWriteProc writeProc = TIFFGetWriteProc(tif);
        (void)writeProc;

        /* Exercise TIFFGetCloseProc */
        TIFFCloseProc closeProc = TIFFGetCloseProc(tif);
        (void)closeProc;

        /* Exercise TIFFGetSizeProc */
        TIFFSizeProc sizeProc = TIFFGetSizeProc(tif);
        (void)sizeProc;

        /* Close the handle properly */
        TIFFClose(tif);
    }

    /* Test 2: TIFFClientOpenExt with options */
    /* Define struct locally to match the library's expectation if not fully exposed */
    struct TIFFOpenOptions {
        TIFFErrorHandlerExtR errorhandler;
        void *errorhandler_user_data;
        TIFFErrorHandlerExtR warnhandler;
        void *warnhandler_user_data;
        tmsize_t max_single_mem_alloc;
        tmsize_t max_cumulated_mem_alloc;
        int warn_about_unknown_tags;
    };

    struct TIFFOpenOptions opts;
    memset(&opts, 0, sizeof(opts));
    opts.errorhandler = DummyErrorHandler;
    opts.warnhandler = DummyWarnHandler;
    opts.max_single_mem_alloc = 0;
    opts.max_cumulated_mem_alloc = 0;
    opts.warn_about_unknown_tags = 0;

    TIFF *tif2 = TIFFClientOpenExt("fuzz_test_ext", "w", clientdata,
                                   DummyReadProc, DummyWriteProc,
                                   DummySeekProc, DummyCloseProc,
                                   DummySizeProc, DummyMapProc, DummyUnmapProc,
                                   &opts);

    if (tif2) {
        /* Exercise getters again on the second instance */
        TIFFReadWriteProc writeProc2 = TIFFGetWriteProc(tif2);
        (void)writeProc2;

        TIFFCloseProc closeProc2 = TIFFGetCloseProc(tif2);
        (void)closeProc2;

        TIFFSizeProc sizeProc2 = TIFFGetSizeProc(tif2);
        (void)sizeProc2;

        /* Close the handle */
        TIFFClose(tif2);
    }

    /* Test 3: Directly test TIFFCleanup */
    TIFF *tif3 = TIFFClientOpen("fuzz_test_cleanup", "w", clientdata,
                                DummyReadProc, DummyWriteProc,
                                DummySeekProc, DummyCloseProc,
                                DummySizeProc, DummyMapProc, DummyUnmapProc);

    if (tif3) {
        /* Call cleanup directly without close to test that path */
        TIFFCleanup(tif3);
    }

    return 0;
}