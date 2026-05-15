// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    /* First sequence: TIFFOpen -> TIFFGetField -> TIFFTileSize -> TIFFReadEncodedTile -> 
       TIFFSetField -> TIFFTileSize -> TIFFReadEncodedTile -> TIFFClose */
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t width = 0, height = 0;
        uint16_t samples_per_pixel = 0, bits_per_sample = 0;
        
        /* Try to get various fields with different tags */
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
        
        /* Get tile size and try to read encoded tile */
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < 1024*1024*10) { /* Reasonable limit */
            void *buf = malloc(tile_size);
            if (buf) {
                /* Try reading tile 0 if it exists */
                TIFFReadEncodedTile(tif, 0, buf, tile_size);
                free(buf);
            }
        }
        
        /* Try setting some fields */
        uint16_t photometric = PHOTOMETRIC_RGB;
        uint16_t compression = COMPRESSION_NONE;
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
        
        /* Get tile size again and try reading another tile */
        tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < 1024*1024*10) {
            void *buf2 = malloc(tile_size);
            if (buf2) {
                /* Try reading tile 1 if it exists */
                TIFFReadEncodedTile(tif, 1, buf2, tile_size);
                free(buf2);
            }
        }
        
        TIFFClose(tif);
    }
    
    /* Second sequence: TIFFOpen -> TIFFReadRGBATile -> TIFFClose */
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        /* Try to read RGBA tile at position (0,0) */
        uint32_t *raster = NULL;
        uint32_t tile_width = 0, tile_height = 0;
        
        /* First try to get tile dimensions to allocate buffer */
        if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width) &&
            TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height) &&
            tile_width > 0 && tile_width < 4096 &&
            tile_height > 0 && tile_height < 4096) {
            
            raster = (uint32_t *)malloc(tile_width * tile_height * sizeof(uint32_t));
            if (raster) {
                TIFFReadRGBATile(tif, 0, 0, raster);
                free(raster);
            }
        } else {
            /* If tile dimensions not available, try with default size */
            raster = (uint32_t *)malloc(256 * 256 * sizeof(uint32_t));
            if (raster) {
                TIFFReadRGBATile(tif, 0, 0, raster);
                free(raster);
            }
        }
        
        TIFFClose(tif);
    }
    
    remove_dummy_file();
    return 0;
}