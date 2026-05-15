// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    // Suppress libtiff error/warning messages
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);

    // Write data to dummy file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    write(fd, Data, Size);
    close(fd);

    // Open TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }

    // Call target API functions
    TIFFIsUpSampled(tif);
    TIFFIsTiled(tif);

    // Try to read encoded tile if image is tiled
    if (TIFFIsTiled(tif)) {
        uint32_t tile_width = 0, tile_height = 0;
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
        
        if (tile_width > 0 && tile_height > 0) {
            tmsize_t tile_size = TIFFTileSize(tif);
            if (tile_size > 0 && tile_size < 1024*1024) { // Limit size for fuzzing
                void* buf = malloc(tile_size);
                if (buf) {
                    // Try reading first few tiles
                    for (uint32_t tile = 0; tile < 4 && tile < TIFFNumberOfTiles(tif); tile++) {
                        TIFFReadEncodedTile(tif, tile, buf, tile_size);
                    }
                    free(buf);
                }
            }
            
            // Try reading RGB tile
            uint32_t* raster = NULL;
            if (tile_width <= 256 && tile_height <= 256) { // Limit size
                raster = (uint32_t*)malloc(tile_width * tile_height * sizeof(uint32_t));
                if (raster) {
                    TIFFReadRGBATile(tif, 0, 0, raster);
                    free(raster);
                }
            }
            
            // Check some tile coordinates
            TIFFCheckTile(tif, 0, 0, 0, 0);
            if (tile_width > 0) {
                TIFFCheckTile(tif, tile_width, 0, 0, 0);
            }
        }
    } else {
        // For strip-based images, try reading RGBA strip
        uint32_t rows_per_strip = 0;
        TIFFGetFieldDefaulted(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
        uint32_t image_width = 0;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &image_width);
        
        if (rows_per_strip > 0 && image_width > 0 && image_width <= 4096) {
            uint32_t* raster = (uint32_t*)malloc(image_width * rows_per_strip * sizeof(uint32_t));
            if (raster) {
                // Try reading first few strips
                uint32_t num_strips = TIFFNumberOfStrips(tif);
                for (uint32_t strip = 0; strip < 4 && strip < num_strips; strip++) {
                    TIFFReadRGBAStrip(tif, strip * rows_per_strip, raster);
                }
                free(raster);
            }
        }
    }

    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}