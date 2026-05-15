// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write fuzzer data to a file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    // Write data to file
    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        unlink("./dummy_file");
        return 0;
    }
    close(fd);
    
    // Try to open the file as TIFF for reading
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // Test the API functions
        uint32_t current_strip = TIFFCurrentStrip(tif);
        uint32_t current_row = TIFFCurrentRow(tif);
        uint32_t current_tile = TIFFCurrentTile(tif);
        uint32_t num_strips = TIFFNumberOfStrips(tif);
        
        // Try to read encoded strips if there are any
        if (num_strips > 0 && num_strips < 1000) { // Avoid huge allocations
            // Try a few different strips
            for (uint32_t strip = 0; strip < num_strips && strip < 5; strip++) {
                tmsize_t strip_size = TIFFStripSize(tif);
                if (strip_size > 0 && strip_size < 1024*1024) { // Limit size to 1MB
                    void* buf = malloc(strip_size);
                    if (buf) {
                        TIFFReadEncodedStrip(tif, strip, buf, strip_size);
                        free(buf);
                    }
                }
            }
        }
        
        TIFFClose(tif);
    }
    
    // Try to open for writing to test TIFFWriteScanline
    tif = TIFFOpen("./dummy_file", "w");
    if (tif) {
        // Set up minimal TIFF parameters for writing
        uint32_t width = 64;
        uint32_t height = 64;
        uint16_t samples_per_pixel = 1;
        uint16_t bits_per_sample = 8;
        uint16_t photometric = PHOTOMETRIC_MINISBLACK;
        uint16_t planar_config = PLANARCONFIG_CONTIG;
        uint32_t rows_per_strip = 16;
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
        
        // Try to write a few scanlines
        size_t scanline_size = TIFFScanlineSize(tif);
        if (scanline_size > 0 && scanline_size < 1024*1024) {
            void* buf = malloc(scanline_size);
            if (buf) {
                // Initialize with some data
                memset(buf, 0, scanline_size);
                
                // Write a few scanlines
                for (uint32_t row = 0; row < height && row < 10; row++) {
                    TIFFWriteScanline(tif, buf, row, 0);
                }
                free(buf);
            }
        }
        
        // Test the API functions again after some writing
        TIFFCurrentStrip(tif);
        TIFFCurrentRow(tif);
        TIFFCurrentTile(tif);
        TIFFNumberOfStrips(tif);
        
        TIFFClose(tif);
    }
    
    // Clean up
    unlink("./dummy_file");
    
    return 0;
}