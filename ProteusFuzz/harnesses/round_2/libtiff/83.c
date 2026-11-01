// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write fuzzing data to a temporary file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file with libtiff
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    // Set custom error handlers to prevent crashes from logging
    // Note: TIFFSetErrorHandlerExt takes only one argument in this version
    TIFFSetErrorHandlerExt((TIFFErrorHandlerExt)dummy_error_handler);
    TIFFSetWarningHandlerExt((TIFFErrorHandlerExt)dummy_warn_handler);

    // Read directory to initialize state
    TIFFReadDirectory(tif);

    // Get image dimensions for buffer allocation
    uint32_t width, height;
    if (!TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) ||
        !TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        TIFFClose(tif);
        return 0;
    }

    // Allocate buffer for RGBA data
    // Limit size to prevent excessive memory usage
    if (width > 0 && height > 0 && width * height > 1000000) {
        TIFFClose(tif);
        return 0;
    }

    size_t buffer_size = width * height * sizeof(uint32_t);
    uint32_t *buf = (uint32_t *)malloc(buffer_size);
    if (!buf) {
        TIFFClose(tif);
        return 0;
    }

    // Fuzz target: TIFFReadRGBAStripExt
    uint32_t row = 0;
    int stop_on_error = 0;
    TIFFReadRGBAStripExt(tif, row, buf, stop_on_error);
    
    // Try with stop_on_error = 1
    stop_on_error = 1;
    TIFFReadRGBAStripExt(tif, row, buf, stop_on_error);

    // Fuzz target: TIFFIsMSB2LSB
    int is_msb = TIFFIsMSB2LSB(tif);
    (void)is_msb; // Use return value to avoid unused warnings

    // Fuzz target: TIFFReadRGBAStrip
    TIFFReadRGBAStrip(tif, row, buf);

    // Fuzz target: TIFFIsUpSampled
    int is_upsampled = TIFFIsUpSampled(tif);
    (void)is_upsampled;

    // Fuzz target: TIFFReadRGBAImageOriented
    // Try different orientations using integer constants directly
    // ORIENTATION_TOPLEFT = 1, ORIENTATION_TOPRIGHT = 2, 
    // ORIENTATION_BOTTOMLEFT = 3, ORIENTATION_BOTTOMRIGHT = 4
    int orientations[] = {1, 2, 3, 4};
    for (int i = 0; i < 4; i++) {
        TIFFReadRGBAImageOriented(tif, width, height, buf, orientations[i], 0);
    }

    // Cleanup
    free(buf);
    TIFFClose(tif);
    
    return 0;
}