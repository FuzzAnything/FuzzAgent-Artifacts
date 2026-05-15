// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
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
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    // First sequence
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t width = 0, height = 0;
        uint16_t samplesperpixel = 0, bitspersample = 0;
        
        // Try to get various fields
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
        
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < 1024*1024) { // Reasonable limit
            void *buf = malloc(tile_size);
            if (buf) {
                // Try to read first tile
                TIFFReadEncodedTile(tif, 0, buf, tile_size);
                free(buf);
            }
        }
        
        // Try to set some fields (might fail in read mode)
        uint32_t compression = COMPRESSION_NONE;
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
        
        // Second tile size and read
        tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < 1024*1024) {
            void *buf2 = malloc(tile_size);
            if (buf2) {
                // Try to read tile 1 if exists
                TIFFReadEncodedTile(tif, 1, buf2, tile_size);
                free(buf2);
            }
        }
        
        TIFFClose(tif);
    }
    
    // Second sequence
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t width = 0, height = 0;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        
        if (width > 0 && height > 0 && width < 4096 && height < 4096) {
            uint32_t *raster = (uint32_t *)malloc(width * height * sizeof(uint32_t));
            if (raster) {
                // Try to read first tile as RGBA
                TIFFReadRGBATile(tif, 0, 0, raster);
                free(raster);
            }
        }
        
        TIFFClose(tif);
    }
    
    remove_dummy_file();
    return 0;
}