// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    // First sequence
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t width = 0;
        uint32_t height = 0;
        uint16_t samples = 0;
        uint16_t bits = 0;
        
        // Try to get various fields
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits);
        
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < 1024*1024) {
            void* buffer = malloc(tile_size);
            if (buffer) {
                // Use first 4 bytes as tile index
                uint32_t tile = *(uint32_t*)Data;
                TIFFReadEncodedTile(tif, tile, buffer, tile_size);
                free(buffer);
            }
        }
        
        // Try to set some fields (may fail in read mode)
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        
        tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < 1024*1024) {
            void* buffer = malloc(tile_size);
            if (buffer) {
                // Use next 4 bytes as tile index
                uint32_t tile = *(uint32_t*)(Data + 4);
                TIFFReadEncodedTile(tif, tile, buffer, tile_size);
                free(buffer);
            }
        }
        
        TIFFClose(tif);
    }
    
    // Second sequence
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t tile_width = 0;
        uint32_t tile_height = 0;
        
        // Check if tiled
        if (TIFFIsTiled(tif)) {
            TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
            TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
            
            if (tile_width > 0 && tile_height > 0 && 
                tile_width < 4096 && tile_height < 4096) {
                uint32_t* rgba_buffer = malloc(tile_width * tile_height * sizeof(uint32_t));
                if (rgba_buffer) {
                    // Use bytes 8-15 for tile coordinates
                    uint32_t x = Size > 8 ? *(uint32_t*)(Data + 8) : 0;
                    uint32_t y = Size > 12 ? *(uint32_t*)(Data + 12) : 0;
                    TIFFReadRGBATile(tif, x, y, rgba_buffer);
                    free(rgba_buffer);
                }
            }
        }
        
        TIFFClose(tif);
    }
    
    cleanup_dummy_file();
    return 0;
}