// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static TIFF* create_test_tiff(const uint8_t* data, size_t size) {
    // Write data to a temporary file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    
    if (written != (ssize_t)size) return NULL;
    
    // Open as TIFF
    TIFF* tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        tif = TIFFOpen("./dummy_file", "r");
    }
    
    return tif;
}

static void cleanup_test_tiff(TIFF* tif) {
    if (tif) {
        TIFFClose(tif);
    }
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Create test TIFF from input data
    TIFF* tif = create_test_tiff(Data, Size);
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Extract parameters from fuzz data
    uint32_t x = 0, y = 0, z = 0, strip = 0, tile = 0;
    uint16_t s = 0;
    tmsize_t buf_size = 0;
    
    if (Size >= 20) {
        // Use first bytes for parameters
        x = *(uint32_t*)(Data);
        y = *(uint32_t*)(Data + 4);
        z = *(uint32_t*)(Data + 8);
        strip = *(uint32_t*)(Data + 12);
        tile = *(uint32_t*)(Data + 16);
        s = *(uint16_t*)(Data + 20);
        buf_size = (tmsize_t)(Size > 100 ? 100 : Size); // Limit buffer size
    }
    
    // Test TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // Test TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // Allocate buffers
    void* read_buf = NULL;
    void* write_buf = NULL;
    
    if (tile_size > 0 && tile_size < 1024*1024) { // Limit to 1MB
        read_buf = malloc(tile_size);
        write_buf = malloc(tile_size);
        
        if (read_buf && write_buf) {
            // Initialize write buffer with some data
            memset(write_buf, 0xAA, tile_size);
            
            // Test TIFFReadTile
            if (num_tiles > 0) {
                TIFFReadTile(tif, read_buf, x % 1000, y % 1000, z % 100, s % 10);
            }
            
            // Test TIFFWriteTile
            TIFFWriteTile(tif, write_buf, x % 1000, y % 1000, z % 100, s % 10);
            
            // Test TIFFReadRawTile
            if (num_tiles > 0) {
                TIFFReadRawTile(tif, tile % (num_tiles > 0 ? num_tiles : 1), 
                               read_buf, tile_size);
            }
        }
    }
    
    // Test TIFFReadEncodedStrip
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    if (num_strips > 0) {
        void* strip_buf = malloc(buf_size > 0 ? buf_size : 1024);
        if (strip_buf) {
            TIFFReadEncodedStrip(tif, strip % (num_strips > 0 ? num_strips : 1), 
                                strip_buf, buf_size);
            free(strip_buf);
        }
    }
    
    // Cleanup
    if (read_buf) free(read_buf);
    if (write_buf) free(write_buf);
    
    cleanup_test_tiff(tif);
    
    return 0;
}