// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "libtiff/tiffio.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t width = 0, height = 0;
    uint16_t samples = 0, bits = 0, planar = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar);
    
    uint32_t rows_per_strip = 0;
    TIFFGetFieldDefaulted(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    
    uint16_t compression = 0;
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
    
    uint32_t tile_width = 0, tile_height = 0, tile_depth = 0;
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
    TIFFGetField(tif, TIFFTAG_TILEDEPTH, &tile_depth);
    
    uint32_t total_strips = TIFFNumberOfStrips(tif);
    uint32_t total_tiles = TIFFNumberOfTiles(tif);
    
    if (total_strips > 0) {
        for (int i = 0; i < 3 && i < total_strips; i++) {
            uint32_t strip = i;
            if (i == 1 && total_strips > 1) strip = total_strips - 1;
            if (i == 2 && total_strips > 2) strip = total_strips / 2;
            
            int err = 0;
            uint64_t byte_count = TIFFGetStrileByteCountWithErr(tif, strip, &err);
            
            if (byte_count > 0 && byte_count < 1000000) {
                tmsize_t strip_size = TIFFStripSize(tif);
                if (strip_size > 0) {
                    void *buf = _TIFFmalloc(strip_size);
                    if (buf) {
                        TIFFReadEncodedStrip(tif, strip, buf, strip_size);
                        _TIFFfree(buf);
                    }
                }
            }
        }
        
        if (height > 0 && rows_per_strip > 0) {
            for (int i = 0; i < 3; i++) {
                uint32_t row = 0;
                if (i == 0) row = 0;
                if (i == 1 && height > 1) row = height - 1;
                if (i == 2 && height > 2) row = height / 2;
                
                uint16_t sample = 0;
                if (samples > 1 && i == 1) sample = samples - 1;
                if (samples > 2 && i == 2) sample = samples / 2;
                
                uint32_t strip_idx = TIFFComputeStrip(tif, row, sample);
            }
        }
        
        uint32_t current_strip = TIFFCurrentStrip(tif);
    }
    
    if (TIFFIsTiled(tif) && total_tiles > 0 && tile_width > 0 && tile_height > 0) {
        for (int i = 0; i < 3 && i < total_tiles; i++) {
            uint32_t x = 0, y = 0, z = 0;
            uint16_t s = 0;
            
            if (i == 0) {
                x = 0; y = 0; z = 0; s = 0;
            } else if (i == 1 && width > tile_width) {
                x = width - tile_width;
                y = height > tile_height ? height - tile_height : 0;
                z = 0;
                s = samples > 0 ? samples - 1 : 0;
            } else if (i == 2 && width > 2 * tile_width) {
                x = width / 2;
                y = height / 2;
                z = 0;
                s = samples > 1 ? samples / 2 : 0;
            }
            
            uint32_t tile = TIFFComputeTile(tif, x, y, z, s);
        }
        
        uint32_t num_tiles = TIFFNumberOfTiles(tif);
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}