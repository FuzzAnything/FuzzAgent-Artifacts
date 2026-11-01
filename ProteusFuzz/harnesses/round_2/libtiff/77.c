// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include "tiffio.h"

static void dummy_error_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "w+b");
    if (!fp) {
        return 0;
    }

    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);

    TIFF *tif = TIFFOpen(filename, "r");
    if (tif) {
        TIFFSetErrorHandlerExt(dummy_error_handler);
        TIFFSetWarningHandlerExt(dummy_warn_handler);

        TIFFFreeDirectory(tif);

        if (TIFFSetDirectory(tif, 0) == 0) {
            // Ignore failure
        }
        if (Size > 0) {
            tdir_t dir_idx = Data[0] % 100;
            TIFFSetDirectory(tif, dir_idx);
        }

        // Attempt to read custom directory with NULL array to test robustness
        TIFFReadCustomDirectory(tif, 0, NULL);

        if (Size >= 4) {
            toff_t offset = *(uint32_t *)(Data);
            TIFFReadCustomDirectory(tif, offset, NULL);
        }

        TIFFClose(tif);
    }

    fp = fopen(filename, "w+b");
    if (fp) {
        fclose(fp);
    }

    tif = TIFFOpen(filename, "w");
    if (tif) {
        TIFFSetErrorHandlerExt(dummy_error_handler);
        TIFFSetWarningHandlerExt(dummy_warn_handler);

        TIFFCreateEXIFDirectory(tif);
        TIFFCreateGPSDirectory(tif);

        TIFFFreeDirectory(tif);

        TIFFSetDirectory(tif, 0);
        if (Size > 0) {
            TIFFSetDirectory(tif, Data[0] % 100);
        }

        TIFFClose(tif);
    }

    unlink(filename);

    return 0;
}