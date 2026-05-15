// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silence libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to silent
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Try to open as TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Also try with write mode for functions that need it
        tif = TIFFOpen("./dummy_file", "r+");
        if (!tif) {
            // Try to create a new file with fuzzer data
            tif = TIFFOpen("./dummy_file", "w");
            if (!tif) {
                unlink("./dummy_file");
                return 0;
            }
            // Write some basic TIFF structure if we're creating new
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 256);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 256);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 256);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        }
    }
    
    // Test TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // Test TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // Allocate buffer for tile operations if tile_size is reasonable
    void *tile_buf = NULL;
    if (tile_size > 0 && tile_size < 1024*1024*10) { // Limit to 10MB
        tile_buf = _TIFFmalloc(tile_size);
        if (tile_buf && Size > 0) {
            // Initialize with some data from fuzzer input
            size_t copy_size = tile_size < Size ? tile_size : Size;
            memcpy(tile_buf, Data, copy_size);
        }
    }
    
    // Test TIFFReadTile if we have a buffer
    if (tile_buf && tile_size > 0) {
        // Try different coordinates based on fuzzer data
        uint32_t x = 0, y = 0, z = 0;
        uint16_t s = 0;
        
        if (Size >= 14) {
            x = *(uint32_t*)(Data);
            y = *(uint32_t*)(Data + 4);
            z = *(uint32_t*)(Data + 8);
            s = *(uint16_t*)(Data + 12);
        }
        
        TIFFReadTile(tif, tile_buf, x, y, z, s);
    }
    
    // Test TIFFWriteTile if we have a buffer
    if (tile_buf && tile_size > 0) {
        uint32_t x = 0, y = 0, z = 0;
        uint16_t s = 0;
        
        if (Size >= 14) {
            x = *(uint32_t*)(Data + (Size >= 28 ? 14 : 0));
            y = *(uint32_t*)(Data + (Size >= 28 ? 18 : 4));
            z = *(uint32_t*)(Data + (Size >= 28 ? 22 : 8));
            s = *(uint16_t*)(Data + (Size >= 28 ? 26 : 12));
        }
        
        TIFFWriteTile(tif, tile_buf, x, y, z, s);
    }
    
    // Test TIFFReadEncodedStrip
    if (tile_buf && tile_size > 0) {
        uint32_t strip = 0;
        if (Size >= 4) {
            strip = *(uint32_t*)Data % 100; // Limit to reasonable range
        }
        
        // Try with different sizes
        tmsize_t read_size = tile_size;
        if (Size >= 8) {
            read_size = *(tmsize_t*)(Data + 4) % (tile_size * 2);
        }
        
        TIFFReadEncodedStrip(tif, strip, tile_buf, read_size);
    }
    
    // Test TIFFReadRawTile
    if (tile_buf && tile_size > 0 && num_tiles > 0) {
        uint32_t tile = 0;
        if (Size >= 4) {
            tile = *(uint32_t*)Data % num_tiles;
        }
        
        tmsize_t raw_size = tile_size;
        if (Size >= 8) {
            raw_size = *(tmsize_t*)(Data + 4) % (tile_size * 2);
        }
        
        TIFFReadRawTile(tif, tile, tile_buf, raw_size);
    }
    
    // Cleanup
    if (tile_buf) {
        _TIFFfree(tile_buf);
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}