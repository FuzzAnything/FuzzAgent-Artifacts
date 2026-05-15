// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
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
#include <fcntl.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static TIFF* open_dummy_tiff(const uint8_t *data, size_t size) {
    // Write data to dummy file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    write(fd, data, size);
    close(fd);
    
    // Open TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    return tif;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Open TIFF from input data
    TIFF* tif = open_dummy_tiff(Data, Size);
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Get basic image information
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    uint16_t samplesperpixel = 1;
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    
    uint16_t bitspersample = 1;
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    
    uint16_t planarconfig = PLANARCONFIG_CONTIG;
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig);
    
    // Test TIFFScanlineSize
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    
    // Test TIFFStripSize
    tmsize_t strip_size = TIFFStripSize(tif);
    
    // Test TIFFReadEncodedStrip if we have strips
    if (!TIFFIsTiled(tif) && strip_size > 0) {
        uint32_t strip_count = TIFFNumberOfStrips(tif);
        if (strip_count > 0) {
            // Try reading first strip
            uint32_t strip_to_read = 0;
            if (strip_count > 1 && Size > 0) {
                strip_to_read = Data[0] % strip_count;
            }
            
            void* buf = malloc(strip_size);
            if (buf) {
                TIFFReadEncodedStrip(tif, strip_to_read, buf, strip_size);
                free(buf);
            }
            
            // Try reading with different sizes
            if (strip_size > 0) {
                for (tmsize_t test_size = strip_size/2; 
                     test_size <= strip_size && test_size > 0; 
                     test_size += strip_size/4) {
                    void* buf2 = malloc(test_size);
                    if (buf2) {
                        TIFFReadEncodedStrip(tif, strip_to_read, buf2, test_size);
                        free(buf2);
                    }
                }
            }
        }
    }
    
    // Test tiled functions if image is tiled
    if (TIFFIsTiled(tif)) {
        // Test TIFFTileSize
        tmsize_t tile_size = TIFFTileSize(tif);
        
        // Test TIFFTileRowSize
        tmsize_t tile_row_size = TIFFTileRowSize(tif);
        
        // Test TIFFVTileSize with various row counts
        if (height > 0) {
            uint32_t tile_height = 0;
            TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
            
            if (tile_height > 0) {
                // Test with full tile height
                TIFFVTileSize(tif, tile_height);
                
                // Test with partial heights
                for (uint32_t nrows = 1; nrows <= tile_height && nrows <= 16; nrows++) {
                    TIFFVTileSize(tif, nrows);
                }
                
                // Test with random row count from input data
                if (Size > 4) {
                    uint32_t random_rows = *(uint32_t*)Data;
                    random_rows = random_rows % (tile_height * 2);
                    TIFFVTileSize(tif, random_rows);
                }
            }
        }
    }
    
    // Clean up
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}