// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
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
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    // Set up error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write input data to file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    write(fd, Data, Size);
    close(fd);
    
    // Open TIFF file for reading
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Try to open for writing if reading fails
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
        
        // Set up minimal valid TIFF structure for writing
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 256);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 256);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 256);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        
        // Test tile functions in write mode
        uint32_t tile = 0;
        if (Size > 0) tile = Data[0] % 256;
        
        // Test TIFFCheckTile
        uint32_t x = 0, y = 0, z = 0;
        uint16_t s = 0;
        if (Size > 4) {
            x = ((uint32_t*)Data)[0] % 256;
            y = ((uint32_t*)Data)[1] % 256;
            s = ((uint16_t*)Data)[4] % 256;
        }
        TIFFCheckTile(tif, x, y, z, s);
        
        // Test TIFFTileSize
        tmsize_t tile_size = TIFFTileSize(tif);
        
        // Test TIFFNumberOfTiles
        uint32_t num_tiles = TIFFNumberOfTiles(tif);
        
        // Test TIFFWriteEncodedTile if we have buffer
        if (tile_size > 0 && Size >= tile_size) {
            void* buf = malloc(tile_size);
            if (buf) {
                memcpy(buf, Data, tile_size > Size ? Size : tile_size);
                TIFFWriteEncodedTile(tif, tile, buf, tile_size);
                free(buf);
            }
        }
        
        TIFFClose(tif);
        unlink("./dummy_file");
        return 0;
    }
    
    // File opened successfully for reading
    // Test tile functions
    
    // Get number of tiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // Test TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // Test TIFFCheckTile with various coordinates
    uint32_t x = 0, y = 0, z = 0;
    uint16_t s = 0;
    if (Size > 4) {
        x = ((uint32_t*)Data)[0] % 256;
        y = ((uint32_t*)Data)[1] % 256;
        s = ((uint16_t*)Data)[4] % 256;
    }
    TIFFCheckTile(tif, x, y, z, s);
    
    // Test TIFFReadEncodedTile if we have tiles
    if (num_tiles > 0 && tile_size > 0) {
        uint32_t tile = 0;
        if (Size > 0) tile = Data[0] % num_tiles;
        
        void* buf = malloc(tile_size);
        if (buf) {
            TIFFReadEncodedTile(tif, tile, buf, tile_size);
            free(buf);
        }
    }
    
    // Test TIFFReadRGBATile if image is tiled
    uint32_t rgba_buf[256 * 256]; // Max reasonable tile size
    if (num_tiles > 0 && Size > 8) {
        uint32_t col = ((uint32_t*)Data)[0] % 256;
        uint32_t row = ((uint32_t*)Data)[1] % 256;
        TIFFReadRGBATile(tif, col, row, rgba_buf);
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    return 0;
}

#ifdef __cplusplus
}
#endif