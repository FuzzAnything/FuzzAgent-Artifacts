// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
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

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t width = 256;
    uint32_t height = 256;
    uint16_t bits_per_sample = 8;
    uint16_t samples_per_pixel = 3;
    uint16_t photometric = PHOTOMETRIC_RGB;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;
    uint32_t rows_per_strip = 16;
    uint32_t tile_width = 64;
    uint32_t tile_height = 64;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
    
    TIFFDeferStrileArrayWriting(tif);
    TIFFWriteCheck(tif, 0, "test");
    TIFFWriteDirectory(tif);
    
    TIFFSetDirectory(tif, 0);
    TIFFForceStrileArrayWriting(tif);
    TIFFSetDirectory(tif, 0);
    TIFFForceStrileArrayWriting(tif);
    TIFFSetDirectory(tif, 0);
    
    size_t tile_size = tile_width * tile_height * samples_per_pixel;
    uint8_t* tile_data = (uint8_t*)malloc(tile_size);
    if (tile_data) {
        memset(tile_data, 0, tile_size);
        size_t copy_size = Size > tile_size ? tile_size : Size;
        memcpy(tile_data, Data, copy_size);
        TIFFWriteEncodedTile(tif, 0, tile_data, tile_size);
        free(tile_data);
    }
    
    size_t strip_size = width * rows_per_strip * samples_per_pixel;
    uint8_t* strip_data = (uint8_t*)malloc(strip_size);
    if (strip_data) {
        memset(strip_data, 0, strip_size);
        size_t copy_size = Size > strip_size ? strip_size : Size;
        memcpy(strip_data, Data, copy_size);
        TIFFWriteEncodedStrip(tif, 0, strip_data, strip_size);
        free(strip_data);
    }
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        size_t read_tile_size = tile_width * tile_height * samples_per_pixel;
        uint8_t* read_tile_buf = (uint8_t*)malloc(read_tile_size);
        if (read_tile_buf) {
            TIFFReadEncodedTile(tif, 0, read_tile_buf, read_tile_size);
            free(read_tile_buf);
        }
        TIFFClose(tif);
    }
    
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        size_t read_strip_size = width * rows_per_strip * samples_per_pixel;
        uint8_t* read_strip_buf = (uint8_t*)malloc(read_strip_size);
        if (read_strip_buf) {
            TIFFReadEncodedStrip(tif, 0, read_strip_buf, read_strip_size);
            free(read_strip_buf);
        }
        TIFFClose(tif);
    }
    
    cleanup_dummy_file();
    return 0;
}