// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {}
static void dummy_warn_handler(const char *module, const char *fmt, va_list ap) {}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Remove file if exists to ensure clean state
    remove(filename);

    // Open TIFF file for writing
    TIFF *tif = TIFFOpen(filename, "w8");
    if (tif == NULL) {
        return 0;
    }

    // Set custom error handlers to prevent fuzzer termination on warnings
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandler(dummy_warn_handler);

    // Set basic image properties to enable writing
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 16);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 16);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    // Allocate a buffer for test data
    size_t max_data_size = 1024 * 1024; // Limit to 1MB to prevent OOM
    if (Size > max_data_size) {
        Size = max_data_size;
    }
    
    uint8_t *test_buffer = (uint8_t *)malloc(Size);
    if (test_buffer == NULL) {
        TIFFClose(tif);
        return 0;
    }
    memcpy(test_buffer, Data, Size);

    // Fuzzing Logic: Invoke target APIs diversely

    // 1. TIFFWriteBufferSetup
    // Try with NULL buffer (auto-alloc) and custom size
    if (Size > 0) {
        TIFFWriteBufferSetup(tif, NULL, (tmsize_t)(Size % 10000));
    }
    // Try with allocated buffer
    void *custom_buf = malloc(1024);
    if (custom_buf) {
        TIFFWriteBufferSetup(tif, custom_buf, 1024);
        free(custom_buf);
    }

    // 2. TIFFWriteScanline
    // Write a scanline (row 0, sample 0)
    // Ensure buffer is large enough for a scanline (16 pixels * 1 byte)
    if (Size >= 16) {
        TIFFWriteScanline(tif, test_buffer, 0, 0);
    }
    // Try writing to a higher row index
    if (Size >= 16) {
        TIFFWriteScanline(tif, test_buffer, 100, 0);
    }

    // 3. TIFFWriteRawStrip
    // Write raw strip data
    if (Size >= 16) {
        TIFFWriteRawStrip(tif, 0, test_buffer, (tmsize_t)Size);
    }
    // Try with a larger strip index
    if (Size >= 16) {
        TIFFWriteRawStrip(tif, 5, test_buffer, (tmsize_t)Size);
    }

    // 4. TIFFWriteEncodedStrip
    // Write encoded strip data (same as raw for uncompressed)
    if (Size >= 16) {
        TIFFWriteEncodedStrip(tif, 0, test_buffer, (tmsize_t)Size);
    }
    if (Size >= 16) {
        TIFFWriteEncodedStrip(tif, 5, test_buffer, (tmsize_t)Size);
    }

    // 5. TIFFWriteEncodedTile
    // To test tiles, we need to set up tiling mode. 
    // Since we already set image properties, we might need to reset or use a new file,
    // but libtiff allows some flexibility. Let's try writing a tile directly.
    // Note: This might fail if the file is not tiled, but that's a valid state to test.
    if (Size >= 16) {
        TIFFWriteEncodedTile(tif, 0, test_buffer, (tmsize_t)Size);
    }
    if (Size >= 16) {
        TIFFWriteEncodedTile(tif, 10, test_buffer, (tmsize_t)Size);
    }

    // 6. TIFFFlushData
    // Force flush to disk
    TIFFFlushData(tif);

    // Cleanup
    free(test_buffer);
    TIFFClose(tif);
    remove(filename);

    return 0;
}