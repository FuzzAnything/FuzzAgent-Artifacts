// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Set up basic TIFF parameters for writing
    uint32_t width = 256;
    uint32_t height = 256;
    uint16_t samples_per_pixel = 1;
    uint16_t bits_per_sample = 8;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    
    // Set up tiling parameters - use TIFFTAG_TILELENGTH instead of TIFFTAG_TILEHEIGHT
    uint32_t tile_width = 64;
    uint32_t tile_length = 64;
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_length);
    
    // Call functions in specified order
    const char *filename = TIFFFileName(tif);
    (void)filename;
    
    // Extract tags from fuzzer input
    uint32_t tag1 = *(uint32_t*)Data;
    uint32_t tag2 = tag1 + 1;
    uint32_t tag3 = tag1 + 2;
    uint32_t tag4 = tag1 + 3;
    
    const TIFFField *field1 = TIFFFieldWithTag(tif, tag1);
    const TIFFField *field2 = TIFFFieldWithTag(tif, tag2);
    const TIFFField *field3 = TIFFFieldWithTag(tif, tag3);
    const TIFFField *field4 = TIFFFieldWithTag(tif, tag4);
    (void)field1; (void)field2; (void)field3; (void)field4;
    
    tmsize_t tile_size = TIFFTileSize(tif);
    
    const TIFFField *field5 = TIFFFieldWithTag(tif, TIFFTAG_IMAGEWIDTH);
    (void)field5;
    
    void *buffer = _TIFFmalloc(tile_size);
    if (buffer && tile_size > 0) {
        // Fill buffer with fuzzer data
        size_t copy_size = tile_size < Size ? tile_size : Size;
        memcpy(buffer, Data, copy_size);
        
        uint32_t num_tiles = TIFFNumberOfTiles(tif);
        
        // Write encoded tile if we have tiles
        if (num_tiles > 0) {
            tmsize_t written = TIFFWriteEncodedTile(tif, 0, buffer, tile_size);
            (void)written;
        }
        
        // Write scanlines
        uint32_t row = 0;
        uint16_t sample = 0;
        int result1 = TIFFWriteScanline(tif, buffer, row, sample);
        (void)result1;
        
        row = 1;
        int result2 = TIFFWriteScanline(tif, buffer, row, sample);
        (void)result2;
        
        _TIFFfree(buffer);
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}