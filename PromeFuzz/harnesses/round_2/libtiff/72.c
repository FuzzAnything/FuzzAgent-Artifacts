// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 100) {
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    uint32_t tile_width = 0, tile_height = 0;
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
    
    uint16_t samples_per_pixel = 1;
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    
    uint16_t planar_config = 1;
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar_config);
    
    uint32_t rows_per_strip = 0;
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    
    if (TIFFIsTiled(tif)) {
        uint32_t num_tiles = TIFFNumberOfTiles(tif);
        
        if (num_tiles > 0) {
            tmsize_t tile_size = TIFFTileSize(tif);
            void *buf = malloc(tile_size);
            if (buf) {
                uint32_t tile_index = num_tiles > 1 ? 1 : 0;
                
                TIFFReadRawTile(tif, tile_index, buf, tile_size);
                
                uint32_t x = width > tile_width ? tile_width : 0;
                uint32_t y = height > tile_height ? tile_height : 0;
                uint32_t z = 0;
                uint16_t s = samples_per_pixel > 1 ? 1 : 0;
                
                TIFFReadTile(tif, buf, x, y, z, s);
                TIFFWriteTile(tif, buf, x, y, z, s);
                
                free(buf);
            }
        }
    } else {
        uint32_t num_strips = TIFFNumberOfStrips(tif);
        
        if (num_strips > 0) {
            tmsize_t strip_size = TIFFStripSize(tif);
            void *buf = malloc(strip_size);
            if (buf) {
                uint32_t strip = num_strips > 1 ? 1 : 0;
                TIFFReadEncodedStrip(tif, strip, buf, strip_size);
                free(buf);
            }
        }
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}