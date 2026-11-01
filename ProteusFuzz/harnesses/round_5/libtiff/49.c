// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {
    // Suppress error messages during fuzzing
}

static void dummy_warning_handler(const char *module, const char *fmt, va_list ap) {
    // Suppress warning messages during fuzzing
}

static TIFF* open_tiff_from_data(const uint8_t *data, size_t size) {
    if (size == 0 || data == NULL) {
        return NULL;
    }

    // Write fuzzed data to a temporary file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp == NULL) {
        return NULL;
    }
    if (fwrite(data, 1, size, fp) != size) {
        fclose(fp);
        return NULL;
    }
    fclose(fp);

    // Open the file with libtiff
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return NULL;
    }

    // Set custom error handlers to suppress output
    TIFFSetWarningHandler(dummy_warning_handler);
    TIFFSetErrorHandler(dummy_error_handler);

    return tif;
}

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
    // Remove the temporary file
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    TIFF *tif = open_tiff_from_data(Data, Size);
    if (tif == NULL) {
        return 0;
    }

    // Get image dimensions
    uint32_t width, height;
    if (!TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) ||
        !TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        cleanup_tiff(tif);
        return 0;
    }

    // Avoid excessively large allocations
    if (width > 10000 || height > 10000 || width * height > 10000000) {
        cleanup_tiff(tif);
        return 0;
    }

    uint32_t *rgba_buffer = (uint32_t *)malloc(width * height * sizeof(uint32_t));
    if (rgba_buffer == NULL) {
        cleanup_tiff(tif);
        return 0;
    }

    // 1. Fuzz TIFFReadRGBAImageOriented
    // Try different orientations
    int orientations[] = {ORIENTATION_TOPLEFT, ORIENTATION_TOPRIGHT, ORIENTATION_BOTLEFT, ORIENTATION_BOTRIGHT, ORIENTATION_LEFTTOP, ORIENTATION_RIGHTTOP, ORIENTATION_LEFTBOT, ORIENTATION_RIGHTBOT};
    for (int i = 0; i < 8; i++) {
        memset(rgba_buffer, 0, width * height * sizeof(uint32_t));
        TIFFReadRGBAImageOriented(tif, width, height, rgba_buffer, orientations[i], 0);
    }

    // 2. Fuzz TIFFReadRGBATileExt
    uint32_t tile_width, tile_length;
    if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width) &&
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length)) {
        uint32_t *tile_buffer = (uint32_t *)malloc(tile_width * tile_length * sizeof(uint32_t));
        if (tile_buffer != NULL) {
            // Try stop_on_error = 0 and 1
            TIFFReadRGBATileExt(tif, 0, 0, tile_buffer, 0);
            TIFFReadRGBATileExt(tif, 0, 0, tile_buffer, 1);
            free(tile_buffer);
        }
    }

    // 3. Fuzz TIFFReadRGBAImage
    memset(rgba_buffer, 0, width * height * sizeof(uint32_t));
    TIFFReadRGBAImage(tif, width, height, rgba_buffer, 0);
    TIFFReadRGBAImage(tif, width, height, rgba_buffer, 1);

    // 4. Fuzz TIFFIsUpSampled
    TIFFIsUpSampled(tif);

    // 5. Fuzz TIFFReadRGBAStrip
    uint32_t rows_per_strip;
    if (TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip)) {
        if (rows_per_strip > 0 && rows_per_strip <= height) {
            uint32_t strip_size = width * rows_per_strip * sizeof(uint32_t);
            uint32_t *strip_buffer = (uint32_t *)malloc(strip_size);
            if (strip_buffer != NULL) {
                // Try reading the first strip
                TIFFReadRGBAStrip(tif, 0, strip_buffer);
                free(strip_buffer);
            }
        }
    }

    // 6. Fuzz TIFFReadRGBAStripExt
    if (TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip)) {
        if (rows_per_strip > 0 && rows_per_strip <= height) {
            uint32_t strip_size = width * rows_per_strip * sizeof(uint32_t);
            uint32_t *strip_buffer = (uint32_t *)malloc(strip_size);
            if (strip_buffer != NULL) {
                // Try stop_on_error = 0 and 1
                TIFFReadRGBAStripExt(tif, 0, strip_buffer, 0);
                TIFFReadRGBAStripExt(tif, 0, strip_buffer, 1);
                free(strip_buffer);
            }
        }
    }

    free(rgba_buffer);
    cleanup_tiff(tif);
    return 0;
}