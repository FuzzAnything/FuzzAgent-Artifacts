// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
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
    // Suppress libtiff error/warning messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    // Write fuzzer input to dummy file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    
    if (written != (ssize_t)size) {
        return NULL;
    }
    
    // Open the dummy file as TIFF
    TIFF* tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try read-only if read-write fails
        tif = TIFFOpen("./dummy_file", "r");
    }
    
    return tif;
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_62(const uint8_t* Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Create TIFF from fuzzer input
    TIFF* tif = create_dummy_tiff(Data, Size);
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Extract parameters from fuzzer input for function calls
    // Use the first bytes for parameters, ensuring we don't read out of bounds
    uint32_t x = 0, y = 0, z = 0, nrows = 0, strip = 0;
    uint16_t s = 0;
    
    if (Size >= 4) x = *(uint32_t*)Data;
    if (Size >= 8) y = *(uint32_t*)(Data + 4);
    if (Size >= 12) z = *(uint32_t*)(Data + 8);
    if (Size >= 16) nrows = *(uint32_t*)(Data + 12);
    if (Size >= 20) strip = *(uint32_t*)(Data + 16);
    if (Size >= 22) s = *(uint16_t*)(Data + 20);
    
    // Call target API functions
    
    // 1. TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // 2. TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // 3. TIFFVStripSize
    tmsize_t vstrip_size = TIFFVStripSize(tif, nrows);
    
    // 4. TIFFReadTile (only if we have a buffer)
    if (tile_size > 0 && tile_size <= 1024*1024) { // Limit buffer size
        void* tile_buf = malloc(tile_size);
        if (tile_buf) {
            TIFFReadTile(tif, tile_buf, x, y, z, s);
            free(tile_buf);
        }
    }
    
    // 5. TIFFWriteTile (only if we have a buffer)
    if (tile_size > 0 && tile_size <= 1024*1024) { // Limit buffer size
        void* write_buf = malloc(tile_size);
        if (write_buf) {
            // Fill with some data
            memset(write_buf, 0xAA, tile_size);
            TIFFWriteTile(tif, write_buf, x, y, z, s);
            free(write_buf);
        }
    }
    
    // 6. TIFFReadEncodedStrip
    tmsize_t strip_size = TIFFStripSize(tif);
    if (strip_size > 0 && strip_size <= 1024*1024) { // Limit buffer size
        void* strip_buf = malloc(strip_size);
        if (strip_buf) {
            // Try with -1 (read full strip) and with actual size
            TIFFReadEncodedStrip(tif, strip, strip_buf, -1);
            TIFFReadEncodedStrip(tif, strip, strip_buf, strip_size);
            free(strip_buf);
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}