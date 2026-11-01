// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    TIFF *tif = NULL;
    tdir_t dir_idx = 0;
    int ret;

    // Write some data to the dummy file to ensure it exists and has content
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    // Open file for writing first to create a valid structure
    tif = TIFFOpen(filename, "w");
    if (tif == NULL) {
        return 0;
    }

    // Set initial directory
    TIFFSetDirectory(tif, dir_idx);

    // Force strip array writing (may fail if not ready, but safe to call)
    TIFFForceStrileArrayWriting(tif);

    // Flush data
    TIFFFlushData(tif);

    // Force strip array writing again
    TIFFForceStrileArrayWriting(tif);

    // Set directory again
    TIFFSetDirectory(tif, dir_idx);

    // Set directory again
    TIFFSetDirectory(tif, dir_idx);

    // Write directory
    TIFFWriteDirectory(tif);

    // Set directory
    TIFFSetDirectory(tif, dir_idx);

    // Write directory
    TIFFWriteDirectory(tif);

    // Set directory
    TIFFSetDirectory(tif, dir_idx);

    // Read directory (might fail on write-only file, but safe)
    TIFFReadDirectory(tif);

    // Set field (using a safe tag like ImageWidth with a value from input)
    uint32_t width = 0;
    if (Size >= 4) {
        memcpy(&width, Data, 4);
        width = width % 10000 + 1; // Ensure non-zero reasonable value
    } else {
        width = 1;
    }
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);

    // Write directory
    TIFFWriteDirectory(tif);

    // Set directory
    TIFFSetDirectory(tif, dir_idx);

    // Write directory
    TIFFWriteDirectory(tif);

    // Set error handler
    TIFFSetErrorHandler(dummy_error_handler);

    // Write directory
    TIFFWriteDirectory(tif);

    // Set error handler again
    TIFFSetErrorHandler(dummy_error_handler);

    // Write directory
    TIFFWriteDirectory(tif);

    // Set error handler again
    TIFFSetErrorHandler(dummy_error_handler);

    // Write directory
    TIFFWriteDirectory(tif);

    // Set error handler again
    TIFFSetErrorHandler(dummy_error_handler);

    // Set error handler again
    TIFFSetErrorHandler(dummy_error_handler);

    // Write directory
    TIFFWriteDirectory(tif);

    // Set error handler again
    TIFFSetErrorHandler(dummy_error_handler);

    // Write directory
    TIFFWriteDirectory(tif);

    // Set directory
    TIFFSetDirectory(tif, dir_idx);

    // Close the write handle
    TIFFClose(tif);

    // Reopen for reading to test read paths
    tif = TIFFOpen(filename, "r");
    if (tif == NULL) {
        return 0;
    }

    // Set directory
    TIFFSetDirectory(tif, dir_idx);

    // Close read handle
    TIFFClose(tif);

    // Close again (should handle NULL gracefully)
    TIFFClose(tif);

    return 0;
}