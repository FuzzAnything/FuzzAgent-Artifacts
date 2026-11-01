// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "tiffio.h"

static void dummy_error_handler(void *clientdata, const char* module, const char* fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(void *clientdata, const char* module, const char* fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to hold the fuzzed data
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the TIFF file
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        // File might not be a valid TIFF, which is expected in fuzzing
        return 0;
    }

    // Set error handlers to avoid crashes on invalid data
    // Note: TIFFSetErrorHandlerExt takes a handler with signature:
    // void (*)(void *, const char *, const char *, va_list)
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    // Read the first directory
    if (TIFFReadDirectory(tif) == 1) {
        uint32_t width, height;
        uint32_t *rgba_buf = NULL;
        uint32_t row;

        // Get image dimensions if possible
        if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {

            // Avoid overflow and excessive allocation
            if (width == 0 || height == 0) {
                TIFFClose(tif);
                remove(filename);
                return 0;
            }

            size_t buf_size = (size_t)width * (size_t)height * sizeof(uint32_t);
            // Limit allocation size to prevent OOM
            if (buf_size > 0 && buf_size < (1024 * 1024 * 1024)) { 
                rgba_buf = (uint32_t *)malloc(buf_size);
                if (rgba_buf) {
                    memset(rgba_buf, 0, buf_size);

                    // Test TIFFReadRGBAImageOriented
                    // Try different orientations: 0 (BOTTOMLEFT), 1 (TOPLEFT), etc.
                    int orientations[] = {0, 1, 2, 3};
                    for (int i = 0; i < 4; i++) {
                        TIFFReadRGBAImageOriented(tif, width, height, rgba_buf, 0, orientations[i]);
                    }

                    // Test TIFFReadRGBAStrip
                    // Try reading a few strips (0 to 3)
                    for (row = 0; row < 4; row++) {
                        TIFFReadRGBAStrip(tif, row, rgba_buf);
                    }

                    // Test TIFFReadRGBAStripExt
                    // Try with stop_on_error = 0 and 1
                    for (row = 0; row < 4; row++) {
                        TIFFReadRGBAStripExt(tif, row, rgba_buf, 0);
                        TIFFReadRGBAStripExt(tif, row, rgba_buf, 1);
                    }
                }
                free(rgba_buf);
            }
        }

        // Test TIFFIsMSB2LSB
        TIFFIsMSB2LSB(tif);

        // Test TIFFIsUpSampled
        TIFFIsUpSampled(tif);
    }

    // Close the TIFF file
    TIFFClose(tif);

    // Clean up the temporary file
    remove(filename);

    return 0;
}