// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
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

#include "libtiff/tiffio.h"

static int write_to_file(const char* filename, const uint8_t* data, size_t size) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

static void cleanup_tiff(TIFF* tif) {
    if (tif) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    const char* filename = "./dummy_file";
    if (write_to_file(filename, Data, Size) != 0) {
        return 0;
    }
    
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        unlink(filename);
        return 0;
    }
    
    uint32_t width = 0, height = 0;
    uint32_t tile_width = 0, tile_height = 0;
    uint16_t samples_per_pixel = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    
    int is_tiled = TIFFIsTiled(tif);
    
    if (is_tiled) {
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
        
        uint32_t tile_count = TIFFNumberOfTiles(tif);
        
        for (uint32_t i = 0; i < tile_count && i < 10; i++) {
            tmsize_t tile_size = TIFFTileSize(tif);
            void* buf = malloc(tile_size);
            if (buf) {
                TIFFReadEncodedTile(tif, i, buf, tile_size);
                free(buf);
            }
        }
        
        uint32_t rgba_buffer_size = tile_width * tile_height;
        uint32_t* rgba_buffer = (uint32_t*)malloc(rgba_buffer_size * sizeof(uint32_t));
        
        if (rgba_buffer) {
            for (uint32_t row = 0; row < height; row += tile_height) {
                for (uint32_t col = 0; col < width; col += tile_width) {
                    TIFFCheckTile(tif, col, row, 0, 0);
                    
                    if (TIFFCheckTile(tif, col, row, 0, 0)) {
                        TIFFReadRGBATile(tif, col, row, rgba_buffer);
                        TIFFReadRGBATileExt(tif, col, row, rgba_buffer, Data[0] & 1);
                    }
                }
            }
            free(rgba_buffer);
        }
    } else {
        uint32_t rows_per_strip = 0;
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
        
        if (rows_per_strip > 0) {
            uint32_t strip_count = TIFFNumberOfStrips(tif);
            uint32_t rgba_buffer_size = width * rows_per_strip;
            uint32_t* rgba_buffer = (uint32_t*)malloc(rgba_buffer_size * sizeof(uint32_t));
            
            if (rgba_buffer) {
                for (uint32_t row = 0; row < height; row += rows_per_strip) {
                    if (row < strip_count * rows_per_strip) {
                        TIFFReadRGBAStrip(tif, row, rgba_buffer);
                    }
                }
                free(rgba_buffer);
            }
        }
    }
    
    cleanup_tiff(tif);
    unlink(filename);
    
    return 0;
}