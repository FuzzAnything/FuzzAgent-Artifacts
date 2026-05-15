// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write fuzzed data to a file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }
    
    // Get image dimensions and other properties
    uint32_t width = 0, height = 0;
    uint16_t samplesperpixel = 0, bitspersample = 0;
    uint16_t planarconfig = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig);
    
    // Test TIFFStripSize
    tmsize_t strip_size = TIFFStripSize(tif);
    
    // Test TIFFScanlineSize
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    
    // Test TIFFTileSize and TIFFTileRowSize if image is tiled
    if (TIFFIsTiled(tif)) {
        tmsize_t tile_size = TIFFTileSize(tif);
        tmsize_t tile_row_size = TIFFTileRowSize(tif);
        
        // Test TIFFVTileSize with various row counts
        uint32_t tile_length = 0;
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length);
        
        if (tile_length > 0) {
            // Test with full tile height
            tmsize_t v_tile_size_full = TIFFVTileSize(tif, tile_length);
            
            // Test with partial tile height (if tile_length > 1)
            if (tile_length > 1) {
                tmsize_t v_tile_size_half = TIFFVTileSize(tif, tile_length / 2);
                tmsize_t v_tile_size_one = TIFFVTileSize(tif, 1);
            }
            
            // Test with row count that might be larger than tile
            tmsize_t v_tile_size_large = TIFFVTileSize(tif, tile_length + 1);
        }
    }
    
    // Test TIFFReadEncodedStrip if image has strips
    uint32_t strips_per_image = TIFFNumberOfStrips(tif);
    if (strips_per_image > 0 && strip_size > 0) {
        // Try to read a few strips
        uint32_t strips_to_test = strips_per_image > 10 ? 10 : strips_per_image;
        
        for (uint32_t strip = 0; strip < strips_to_test; strip++) {
            // Allocate buffer for strip data
            void *buf = _TIFFmalloc(strip_size);
            if (buf) {
                // Try to read the strip
                tmsize_t bytes_read = TIFFReadEncodedStrip(tif, strip, buf, strip_size);
                
                // Also try with size = -1 (read full strip)
                if (bytes_read >= 0) {
                    bytes_read = TIFFReadEncodedStrip(tif, strip, buf, (tmsize_t)-1);
                }
                
                _TIFFfree(buf);
            }
            
            // Also test with smaller buffer
            if (strip_size > 100) {
                void *small_buf = _TIFFmalloc(100);
                if (small_buf) {
                    tmsize_t bytes_read = TIFFReadEncodedStrip(tif, strip, small_buf, 100);
                    _TIFFfree(small_buf);
                }
            }
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}