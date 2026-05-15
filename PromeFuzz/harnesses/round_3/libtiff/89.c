// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzing data to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Try to open the file as TIFF
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Clean up and return if not a valid TIFF
        unlink("./dummy_file");
        return 0;
    }
    
    // Get basic TIFF information
    uint32_t width = 0, height = 0;
    uint16_t samplesperpixel = 0, bitspersample = 0;
    uint32_t rowsperstrip = 0;
    uint16_t planarconfig = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig);
    
    // Calculate number of strips
    uint32_t num_strips = 0;
    if (rowsperstrip > 0 && height > 0) {
        num_strips = TIFFNumberOfStrips(tif);
    }
    
    // Test TIFFStripSize
    tmsize_t strip_size = TIFFStripSize(tif);
    
    // Test TIFFVStripSize with various row counts
    if (height > 0) {
        uint32_t test_rows = height / 4;
        if (test_rows > 0) {
            TIFFVStripSize(tif, test_rows);
        }
        TIFFVStripSize(tif, 1);
        if (height > 1) {
            TIFFVStripSize(tif, height - 1);
        }
    }
    
    // Test strip-related functions if we have strips
    if (num_strips > 0) {
        // Test with different strip indices
        for (uint32_t strip = 0; strip < num_strips && strip < 10; strip++) {
            // Test TIFFRawStripSize
            tmsize_t raw_size = TIFFRawStripSize(tif, strip);
            
            // Test TIFFReadEncodedStrip
            if (strip_size > 0 && strip_size < 1024 * 1024 * 10) { // Limit to 10MB
                void *buf = malloc(strip_size);
                if (buf) {
                    TIFFReadEncodedStrip(tif, strip, buf, strip_size);
                    // Test partial read
                    if (strip_size > 10) {
                        TIFFReadEncodedStrip(tif, strip, buf, strip_size / 2);
                    }
                    free(buf);
                }
            }
            
            // Test TIFFReadRawStrip
            if (raw_size > 0 && raw_size < 1024 * 1024 * 10) { // Limit to 10MB
                void *raw_buf = malloc(raw_size);
                if (raw_buf) {
                    TIFFReadRawStrip(tif, strip, raw_buf, raw_size);
                    // Test partial read
                    if (raw_size > 10) {
                        TIFFReadRawStrip(tif, strip, raw_buf, raw_size / 2);
                    }
                    free(raw_buf);
                }
            }
        }
        
        // Test TIFFWriteEncodedStrip if we can open for writing
        TIFFClose(tif);
        tif = TIFFOpen("./dummy_file", "r+");
        if (tif) {
            // Try to write to first strip if we have valid data
            if (strip_size > 0 && strip_size < 1024 * 1024 && Size > 0) {
                void *write_buf = malloc(strip_size);
                if (write_buf) {
                    // Copy some fuzzing data into buffer
                    size_t copy_size = Size < strip_size ? Size : strip_size;
                    memcpy(write_buf, Data, copy_size);
                    
                    // Try writing to first strip
                    if (num_strips > 0) {
                        TIFFWriteEncodedStrip(tif, 0, write_buf, strip_size);
                    }
                    
                    free(write_buf);
                }
            }
        }
    }
    
    // Clean up
    if (tif) {
        TIFFClose(tif);
    }
    unlink("./dummy_file");
    
    return 0;
}