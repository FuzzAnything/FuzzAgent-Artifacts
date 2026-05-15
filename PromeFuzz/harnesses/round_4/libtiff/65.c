// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

    // Write fuzzer input to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the dummy file as TIFF
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Try opening for writing if read fails
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }

    // Get basic image information
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    uint32_t rows_per_strip = 0;
    TIFFGetFieldDefaulted(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    
    uint16_t samples_per_pixel = 1;
    TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    
    uint16_t bits_per_sample = 8;
    TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    
    // Calculate strip count
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    
    // Test TIFFStripSize
    tmsize_t strip_size = TIFFStripSize(tif);
    
    // Test TIFFVStripSize with various row counts
    if (rows_per_strip > 0) {
        uint32_t test_rows = rows_per_strip;
        if (height > 0 && rows_per_strip > height) {
            test_rows = height;
        }
        if (test_rows > 0) {
            tmsize_t vstrip_size = TIFFVStripSize(tif, test_rows);
            (void)vstrip_size; // Use result to avoid unused variable warning
        }
    }
    
    // Test strip-related functions if we have strips
    if (strip_count > 0) {
        // Choose a strip index based on fuzzer input
        uint32_t strip_idx = 0;
        if (Size >= sizeof(uint32_t)) {
            memcpy(&strip_idx, Data, sizeof(uint32_t));
        }
        strip_idx = strip_idx % strip_count;
        
        // Test TIFFRawStripSize
        tmsize_t raw_strip_size = TIFFRawStripSize(tif, strip_idx);
        
        // Allocate buffers for reading/writing
        tmsize_t buf_size = strip_size > 0 ? strip_size : 4096;
        if (raw_strip_size > 0 && raw_strip_size > buf_size) {
            buf_size = raw_strip_size;
        }
        
        void *buf = malloc(buf_size);
        void *write_buf = malloc(buf_size);
        
        if (buf && write_buf) {
            // Initialize write buffer with some data
            size_t init_size = Size < buf_size ? Size : buf_size;
            if (init_size > 0) {
                memcpy(write_buf, Data, init_size);
            }
            
            // Test TIFFReadRawStrip
            if (raw_strip_size > 0) {
                tmsize_t read_size = raw_strip_size;
                if (read_size > buf_size) {
                    read_size = buf_size;
                }
                TIFFReadRawStrip(tif, strip_idx, buf, read_size);
            }
            
            // Test TIFFReadEncodedStrip
            if (strip_size > 0) {
                tmsize_t read_size = strip_size;
                if (read_size > buf_size) {
                    read_size = buf_size;
                }
                TIFFReadEncodedStrip(tif, strip_idx, buf, read_size);
            }
            
            // Test TIFFWriteEncodedStrip if we opened in write mode
            // Check if file was opened for writing by checking mode flags
            uint32_t tif_mode = 0;
            // We can't directly access tif_mode from TIFF struct, so try to write
            TIFFWriteEncodedStrip(tif, strip_idx, write_buf, 
                                 buf_size < 1024 ? buf_size : 1024);
        }
        
        free(buf);
        free(write_buf);
        
        // Test with invalid strip indices
        if (strip_count < UINT32_MAX) {
            TIFFReadRawStrip(tif, strip_count + 1, NULL, 0);
            TIFFReadEncodedStrip(tif, strip_count + 1, NULL, 0);
            TIFFWriteEncodedStrip(tif, strip_count + 1, NULL, 0);
            TIFFRawStripSize(tif, strip_count + 1);
        }
    }
    
    // Test edge cases
    TIFFStripSize(tif);
    TIFFVStripSize(tif, 0);
    TIFFVStripSize(tif, 1);
    if (height > 0) {
        TIFFVStripSize(tif, height);
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}