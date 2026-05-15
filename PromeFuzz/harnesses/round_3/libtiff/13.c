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

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    write_dummy_file(Data, Size);
    
    // First sequence
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t tile_width = 0;
        uint32_t tile_height = 0;
        
        // Try to get various fields
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
        
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0) {
            void* buf = malloc(tile_size);
            if (buf) {
                // Try to read first tile
                TIFFReadEncodedTile(tif, 0, buf, tile_size);
                free(buf);
            }
        }
        
        // Try to set some fields (might fail for read-only)
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        
        // Try tile operations again
        tile_size = TIFFTileSize(tif);
        if (tile_size > 0) {
            void* buf = malloc(tile_size);
            if (buf) {
                // Try to read tile with index derived from data
                uint32_t tile_index = (Data[0] << 8) | Data[1];
                TIFFReadEncodedTile(tif, tile_index % 100, buf, tile_size);
                free(buf);
            }
        }
        
        TIFFClose(tif);
    }
    
    // Second sequence
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t width = 0;
        uint32_t height = 0;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        
        // Allocate buffer for RGBA tile
        if (width > 0 && height > 0) {
            uint32_t* rgba_buf = malloc(width * height * sizeof(uint32_t));
            if (rgba_buf) {
                // Try to read RGBA tile
                uint32_t col = (Data[2] << 8) | Data[3];
                uint32_t row = (Data[4] << 8) | Data[5];
                TIFFReadRGBATile(tif, col % width, row % height, rgba_buf);
                free(rgba_buf);
            }
        }
        
        TIFFClose(tif);
    }
    
    remove_dummy_file();
    return 0;
}