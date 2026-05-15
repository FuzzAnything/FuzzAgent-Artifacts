// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        cleanup_file();
        return 0;
    }
    
    uint32_t width = 256;
    uint32_t height = 256;
    uint16_t samples_per_pixel = 3;
    uint16_t bits_per_sample = 8;
    uint16_t photometric = PHOTOMETRIC_RGB;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;
    uint16_t orientation = ORIENTATION_TOPLEFT;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);
    
    TIFFDeferStrileArrayWriting(tif);
    TIFFWriteCheck(tif, 0, "test");
    TIFFWriteDirectory(tif);
    
    TIFFSetDirectory(tif, 0);
    TIFFForceStrileArrayWriting(tif);
    TIFFSetDirectory(tif, 0);
    TIFFForceStrileArrayWriting(tif);
    
    uint32_t tile_size = 256;
    uint32_t tile_width = 16;
    uint32_t tile_height = 16;
    
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
    
    uint8_t *tile_data = (uint8_t *)malloc(tile_size);
    if (tile_data) {
        size_t copy_size = Size > tile_size ? tile_size : Size;
        memcpy(tile_data, Data, copy_size);
        TIFFWriteEncodedTile(tif, 0, tile_data, tile_size);
        free(tile_data);
    }
    
    uint32_t rows_per_strip = 16;
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    
    uint32_t strip_size = width * samples_per_pixel * rows_per_strip;
    uint8_t *strip_data = (uint8_t *)malloc(strip_size);
    if (strip_data) {
        size_t copy_size = Size > strip_size ? strip_size : Size;
        memcpy(strip_data, Data, copy_size);
        TIFFWriteEncodedStrip(tif, 0, strip_data, strip_size);
        free(strip_data);
    }
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_file();
        return 0;
    }
    
    uint8_t *read_buf = (uint8_t *)malloc(1024);
    if (read_buf) {
        TIFFReadEncodedTile(tif, 0, read_buf, 1024);
        free(read_buf);
    }
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_file();
        return 0;
    }
    
    uint8_t *read_buf2 = (uint8_t *)malloc(1024);
    if (read_buf2) {
        TIFFReadEncodedStrip(tif, 0, read_buf2, 1024);
        free(read_buf2);
    }
    
    TIFFClose(tif);
    
    cleanup_file();
    return 0;
}