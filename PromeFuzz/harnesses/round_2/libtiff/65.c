// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
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
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
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

static void cleanup_tiff(TIFF* tif) {
    if (tif) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0; // Need minimum data for meaningful fuzzing
    
    // Create dummy file for TIFF operations
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open TIFF file for reading/writing
    TIFF* tif = TIFFOpen(filename, "w+");
    if (!tif) {
        unlink(filename);
        return 0;
    }
    
    // Set some basic TIFF tags to make operations valid
    uint32_t width = 256;
    uint32_t height = 256;
    uint16_t samplesperpixel = 1;
    uint16_t bitspersample = 8;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint16_t planarconfig = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 16);
    
    // Set up tiled configuration for tile operations
    uint32_t tilewidth = 64;
    uint32_t tilelength = 64;
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, tilewidth);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, tilelength);
    
    // Test TIFFWriteBufferSetup with various configurations
    void* write_buffer = NULL;
    tmsize_t buffer_size = (tmsize_t)(Size % 65536);
    if (buffer_size > 0) {
        write_buffer = malloc(buffer_size);
        if (write_buffer) {
            // Test with allocated buffer
            TIFFWriteBufferSetup(tif, write_buffer, buffer_size);
            // Test with NULL buffer (letting libtiff allocate)
            TIFFWriteBufferSetup(tif, NULL, -1);
            free(write_buffer);
        }
    }
    
    // Test TIFFWriteEncodedStrip
    uint32_t strip = 0;
    tmsize_t strip_size = TIFFStripSize(tif);
    if (strip_size > 0) {
        void* strip_data = malloc(strip_size);
        if (strip_data) {
            size_t data_to_copy = Size < (size_t)strip_size ? Size : (size_t)strip_size;
            memcpy(strip_data, Data, data_to_copy);
            TIFFWriteEncodedStrip(tif, strip, strip_data, (tmsize_t)data_to_copy);
            free(strip_data);
        }
    }
    
    // Test TIFFWriteRawStrip
    if (strip_size > 0) {
        void* raw_strip_data = malloc(strip_size);
        if (raw_strip_data) {
            size_t data_to_copy = Size < (size_t)strip_size ? Size : (size_t)strip_size;
            memcpy(raw_strip_data, Data, data_to_copy);
            TIFFWriteRawStrip(tif, strip, raw_strip_data, (tmsize_t)data_to_copy);
            free(raw_strip_data);
        }
    }
    
    // Test TIFFWriteEncodedTile
    uint32_t tile = 0;
    tmsize_t tile_size = TIFFTileSize(tif);
    if (tile_size > 0) {
        void* tile_data = malloc(tile_size);
        if (tile_data) {
            size_t data_to_copy = Size < (size_t)tile_size ? Size : (size_t)tile_size;
            memcpy(tile_data, Data, data_to_copy);
            TIFFWriteEncodedTile(tif, tile, tile_data, (tmsize_t)data_to_copy);
            free(tile_data);
        }
    }
    
    // Flush and reopen for reading operations
    TIFFFlush(tif);
    
    // Test TIFFReadBufferSetup
    void* read_buffer = NULL;
    tmsize_t read_buffer_size = (tmsize_t)(Size % 32768);
    if (read_buffer_size > 0) {
        read_buffer = malloc(read_buffer_size);
        if (read_buffer) {
            TIFFReadBufferSetup(tif, read_buffer, read_buffer_size);
            free(read_buffer);
        }
    }
    
    // Test TIFFReadEncodedTile
    if (tile_size > 0) {
        void* read_tile_buffer = malloc(tile_size);
        if (read_tile_buffer) {
            TIFFReadEncodedTile(tif, tile, read_tile_buffer, tile_size);
            free(read_tile_buffer);
        }
    }
    
    // Cleanup
    cleanup_tiff(tif);
    unlink(filename);
    
    return 0;
}