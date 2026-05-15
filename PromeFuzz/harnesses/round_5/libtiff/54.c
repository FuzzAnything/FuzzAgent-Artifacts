// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
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
}

static TIFF* open_dummy_tiff(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to a temporary file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    ssize_t written = write(fd, Data, Size);
    close(fd);
    
    if (written != (ssize_t)Size) {
        return NULL;
    }
    
    // Open the file with libtiff
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    return TIFFOpen("./dummy_file", "r");
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0; // Need minimum data for meaningful TIFF
    
    TIFF* tif = open_dummy_tiff(Data, Size);
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Test TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // Test TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // If we have tiles and tile size is reasonable, test tile operations
    if (num_tiles > 0 && tile_size > 0 && tile_size < 1024*1024) {
        void* buffer = malloc(tile_size);
        if (buffer) {
            // Test TIFFReadTile with various coordinates
            // Use modulo operations to get valid coordinates
            uint32_t width = 0, height = 0;
            TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
            
            if (width > 0 && height > 0) {
                // Test a few different tile coordinates
                for (int i = 0; i < 3 && i < (int)num_tiles; i++) {
                    uint32_t x = (i * 16) % width;
                    uint32_t y = (i * 16) % height;
                    uint32_t z = 0;
                    uint16_t sample = 0;
                    
                    TIFFReadTile(tif, buffer, x, y, z, sample);
                    
                    // Also test TIFFWriteTile if we can write
                    TIFF* tif_write = TIFFOpen("./dummy_file", "r+");
                    if (tif_write) {
                        TIFFWriteTile(tif_write, buffer, x, y, z, sample);
                        TIFFClose(tif_write);
                    }
                }
            }
            
            // Test TIFFReadRawTile for first few tiles
            for (int i = 0; i < 3 && i < (int)num_tiles; i++) {
                TIFFReadRawTile(tif, i, buffer, tile_size);
            }
            
            free(buffer);
        }
    }
    
    // Test strip operations if not tiled
    if (!TIFFIsTiled(tif)) {
        uint32_t num_strips = TIFFNumberOfStrips(tif);
        tmsize_t strip_size = TIFFStripSize(tif);
        
        if (num_strips > 0 && strip_size > 0 && strip_size < 1024*1024) {
            void* strip_buffer = malloc(strip_size);
            if (strip_buffer) {
                // Test TIFFReadEncodedStrip for first few strips
                for (int i = 0; i < 3 && i < (int)num_strips; i++) {
                    TIFFReadEncodedStrip(tif, i, strip_buffer, strip_size);
                }
                free(strip_buffer);
            }
        }
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}